#include <map>

#include <boost/thread.hpp>

#include <memorymanagerinterface.hh>
#include <threadpool.hh>

const std::string SCROOM_MEMORY_HWM = "SCROOM_MEMORY_HWM";
const std::string SCROOM_MEMORY_LWM = "SCROOM_MEMORY_LWM";
const std::string SCROOM_FILES_HWM = "SCROOM_FILES_HWM";
const std::string SCROOM_FILES_LWM = "SCROOM_FILES_LWM";

struct ManagedInfo
{
  size_t size;
  int fdcount;
  bool isLoaded;
  unsigned long timestamp;

public:
  ManagedInfo(size_t size=0, int fdcount=0)
    : size(size), fdcount(fdcount), isLoaded(false), timestamp(0)
  {
  }
};

class MemoryManager
{
public:
  typedef std::map<MemoryManagedInterface*,ManagedInfo> ManagedInfoMap;
  
private:
  unsigned long memHwm;
  unsigned long memLwm;
  unsigned long filesHwm;
  unsigned long filesLwm;

  unsigned long memCurrent;
  unsigned long memTotal;
  unsigned long filesCurrent;
  unsigned long filesTotal;

  unsigned long timestamp;

  boost::mutex mut;
  ManagedInfoMap managedInfo;
  bool isGarbageCollecting;
  
public:
  static MemoryManager* instance();

public:
  MemoryManager();
  ~MemoryManager();

  void registerMMI(MemoryManagedInterface* object, size_t size, int fdcount);
  void unregisterMMI(MemoryManagedInterface* object);
  void loadNotification(MemoryManagedInterface* object);
  void unloadNotification(MemoryManagedInterface* object);

  void garbageCollect();
  
private:
  int fetchFromEnvironment(std::string v);
  void checkForOutOfResources();
};

class GarbageCollector : public WorkInterface
{
  virtual bool doWork()
  {
    MemoryManager::instance()->garbageCollect();
    return false;
  }
};

////////////////////////////////////////////////////////////////////////
/// MemoryManager

MemoryManager* MemoryManager::instance()
{
  static MemoryManager m;
  return &m;
}

MemoryManager::MemoryManager()
{
  printf("Creating memory manager\n");

  memHwm = 1024*1024*(unsigned long)fetchFromEnvironment(SCROOM_MEMORY_HWM);
  memLwm = 1024*1024*(unsigned long)fetchFromEnvironment(SCROOM_MEMORY_LWM);
  filesHwm = fetchFromEnvironment(SCROOM_FILES_HWM);
  filesLwm = fetchFromEnvironment(SCROOM_FILES_LWM);

  if(!memHwm)
  {
    memHwm = (unsigned long)2048*1024*1024;
    memLwm = (unsigned long)1920*1024*1024;
  }
  if(!memLwm)
  {
    memLwm = memHwm*7/8;
  }
  if(!filesHwm)
  {
    filesHwm = 768;
    filesLwm = 700;
  }
  if(!filesLwm)
  {
    filesLwm = filesHwm*8/10;
  }

  printf("Memory watermarks: High %lu, Low %lu\n", memHwm, memLwm);
  printf("File watermarks: High %lu, Low %lu\n", filesHwm, filesLwm);

  memCurrent = 0;
  memTotal = 0;
  filesCurrent = 0;
  filesTotal = 0;

  timestamp = 0;
}

MemoryManager::~MemoryManager()
{
}

void MemoryManager::registerMMI(MemoryManagedInterface* object, size_t size, int fdcount)
{
  boost::unique_lock<boost::mutex> lock(mut);
  managedInfo[object] = ManagedInfo(size, fdcount);
  memTotal+=size;
  filesTotal+=fdcount;
}

void MemoryManager::unregisterMMI(MemoryManagedInterface* object)
{
  boost::unique_lock<boost::mutex> lock(mut);
  ManagedInfoMap::iterator info = managedInfo.find(object);
  if(info != managedInfo.end())
  {
    ManagedInfo& m = info->second;
    if(m.isLoaded)
    {
      memCurrent-=m.size;
      filesCurrent-=m.fdcount;
    }
    
    memTotal-=m.size;
    filesTotal-=m.fdcount;
    managedInfo.erase(info);
  }
}

void MemoryManager::loadNotification(MemoryManagedInterface* object)
{
  ManagedInfo& m = managedInfo[object];
  assert(m.size!=0 || m.fdcount!=0);

  if(!m.isLoaded)
  {
    m.isLoaded = true;
    memCurrent += m.size;
    filesCurrent += m.fdcount;
  }
  m.timestamp = ++timestamp;

  checkForOutOfResources();
}

void MemoryManager::unloadNotification(MemoryManagedInterface* object)
{
  ManagedInfo& m = managedInfo[object];
  assert(m.size!=0 || m.fdcount!=0);

  if(m.isLoaded)
  {
    m.isLoaded = false;
    memCurrent -= m.size;
    filesCurrent -= m.fdcount;
  }
}

int MemoryManager::fetchFromEnvironment(std::string v)
{
  std::string value = getenv(v.c_str());
  return atoi(value.c_str());
}

void MemoryManager::checkForOutOfResources()
{
  if(!isGarbageCollecting && (memCurrent>memHwm || filesCurrent>filesHwm))
  {
    isGarbageCollecting=true;
    schedule(new GarbageCollector(), PRIO_HIGHEST);
  }
}

void MemoryManager::garbageCollect()
{
  printf("Garbage collector activating.\n");
  printf("Files: %lu/%lu, thresholds %lu-%lu\n",
         filesCurrent, filesTotal, filesLwm, filesHwm);
  printf("Memory: %lu/%lu, thresholds %lu-%lu\n",
         memCurrent, memTotal, memLwm, memHwm);

  {
    boost::unique_lock<boost::mutex> lock(mut);
    std::map<unsigned long, std::list<MemoryManagedInterface*> > sortedInterfaces;

    for(ManagedInfoMap::iterator cur = managedInfo.begin();
        cur != managedInfo.end();
        ++cur)
    {
      sortedInterfaces[cur->second.timestamp].push_back(cur->first);
    }
    std::map<unsigned long, std::list<MemoryManagedInterface*> >::iterator cur = sortedInterfaces.begin();
    std::map<unsigned long, std::list<MemoryManagedInterface*> >::iterator end = sortedInterfaces.end();

    for(;cur!=end && (filesCurrent > filesLwm || memCurrent > memHwm); ++cur)
    {
      std::list<MemoryManagedInterface*>& list = cur->second;
      for(std::list<MemoryManagedInterface*>::iterator c = list.begin();
          c!=list.end();
          ++c)
      {
        if(managedInfo[*c].isLoaded)
        {
          (*c)->do_unload();
        }
      }
    }

    isGarbageCollecting = false;
  }

  printf("Garbage collector terminating.\n");
  printf("Files: %lu/%lu, thresholds %lu-%lu\n",
         filesCurrent, filesTotal, filesLwm, filesHwm);
  printf("Memory: %lu/%lu, thresholds %lu-%lu\n",
         memCurrent, memTotal, memLwm, memHwm);
}
  


////////////////////////////////////////////////////////////////////////
/// Regular functions

void registerMMI(MemoryManagedInterface* object, size_t size, int fdcount)
{
  MemoryManager::instance()->registerMMI(object, size, fdcount);
}

void unregisterMMI(MemoryManagedInterface* object)
{
  MemoryManager::instance()->unregisterMMI(object);
}

void loadNotification(MemoryManagedInterface* object)
{
  MemoryManager::instance()->loadNotification(object);
}

void unloadNotification(MemoryManagedInterface* object)
{
  MemoryManager::instance()->unloadNotification(object);
}
