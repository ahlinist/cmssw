#ifndef EVF_INPUTCONTROLLER_REGISTRY_H
#define EVF_INPUTCONTROLLER_REGISTRY_H

#include <string>
#include <map>

//#include <list>

namespace edm{
  class ParameterSet;
}
//namespace xdaq{
//  class Application;
//}

namespace dqmevf
{
  struct evfSourceVars;
  class InputController;
  class InputControllerRegistry
    {
    public:
      InputControllerRegistry(const edm::ParameterSet &);

      void registerSource(std::string &name, InputController *cl)
	{
	  clm_.insert(std::pair<std::string, InputController*>(name,cl));
	} 
      void publish(evfSourceVars * vars);

    private:
      typedef std::map<std::string, InputController*> dct;
      typedef dct::iterator idct;
      void trapSource(const std::string &, unsigned int timeout_sec = 0);
      void untrapSource(const std::string &);
      void clear();
      dct clm_;
      friend class FWEPWrapper;
    };
}
#endif

