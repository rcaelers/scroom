#include <threadpool.hh>

#include <stdio.h>

#include <boost/thread.hpp>
#include <queue>

#include <scroom-semaphore.hh>

#include <workinterface.hh>

struct Job
{
public:
  int priority;
  WorkInterface* wi;

public:
  Job(int priority, WorkInterface* wi)
    : priority(priority), wi(wi)
  {
  }

  bool operator< (const Job& other) const
  {
    return priority < other.priority;
  }
};

class ThreadPool
{
private:
  std::list<boost::thread*> threads;
  Scroom::Semaphore jobcount;
  std::priority_queue<Job> jobs;
  boost::mutex mut;

private:
  static void work();
  ThreadPool();
  bool perform_one();
  
public:

  void schedule(Job j);
  void schedule(int priority, WorkInterface* wi);
  
public:
  static ThreadPool& instance();
};


////////////////////////////////////////////////////////////////////////
/// ThreadPool
////////////////////////////////////////////////////////////////////////

ThreadPool& ThreadPool::instance()
{
  static ThreadPool threadpool;

  return threadpool;
}

ThreadPool::ThreadPool()
{
  int count = boost::thread::hardware_concurrency();
  printf("Starting ThreadPool with %d threads\n", count);
  for(int i=0; i<count; i++)
  {
    threads.push_back(new boost::thread(work));
  }
}

void ThreadPool::work()
{
  while(instance().perform_one())
  {
  }

  printf("ThreadPool: Thread terminating...\n");
}

void ThreadPool::schedule(Job j)
{
  boost::unique_lock<boost::mutex> lock(mut);
  jobs.push(j);
  lock.unlock();
  jobcount.V();
}

void ThreadPool::schedule(int priority, WorkInterface* wi)
{
  schedule(Job(priority, wi));
}

bool ThreadPool::perform_one()
{
  jobcount.P();
  boost::unique_lock<boost::mutex> lock(mut);
  if(!jobs.empty())
  {
    Job j = jobs.top();
    jobs.pop();
    lock.unlock();

    bool result = j.wi->doWork();
    if(result)
    {
      schedule(j);
    }
    else
    {
      delete j.wi;
    }
  }
  else
  {
    printf("PANIC: JobQueue empty while it shouldn't be\n");
  }
  
  return true;
}

void schedule(WorkInterface* wi, int priority)
{
  ThreadPool::instance().schedule(Job(priority, wi));
}
