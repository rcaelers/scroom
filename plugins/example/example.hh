#ifndef _EXAMPLE_HH
#define _EXAMPLE_HH

#include <plugininformationinterface.hh>
#include <presentationinterface.hh>
#include <workinterface.hh>

class Example : public PluginInformationInterface, public NewInterface
{
public:
  virtual std::string getPluginName();
  virtual std::string getPluginVersion();
  virtual void registerCapabilities(ScroomInterface* host);
  virtual void unregisterCapabilities(ScroomInterface* host);

  virtual PresentationInterface::Ptr createNew(FileOperationObserver* observer);

  virtual ~Example();
};

#endif
