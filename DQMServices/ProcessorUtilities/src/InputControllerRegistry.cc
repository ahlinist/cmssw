#include "DQMServices/ProcessorUtilities/interface/InputControllerRegistry.h"
#include "DQMServices/ProcessorUtilities/interface/InputController.h"

#include <iostream>

namespace dqmevf{
InputControllerRegistry::InputControllerRegistry(const edm::ParameterSet &ps){}


void InputControllerRegistry::trapSource(const std::string &name, unsigned int timeout_sec)
{
  idct i = clm_.find(name);
  if(i != clm_.end())
    {
      try{
	(*i).second->trapSource(timeout_sec);
      }
      catch(...)
	{
	  std::cout << "exception caught when calling open backdoor for " << name << std::endl;
	}
    }
}

void InputControllerRegistry::untrapSource(const std::string &name)
{
  idct i = clm_.find(name);
  if(i != clm_.end())
    {
      try{
	(*i).second->untrapSource();
      }
      catch(...)
	{
	  std::cout << "exception caught when calling close backdoor for " << name << std::endl;
	}
    }
}

void InputControllerRegistry::publish(evfSourceVars *vars)
{
    idct i = clm_.begin();
    while (i != clm_.end())
      {
	(*i).second->publish(vars);
	i++;
      }

}

void InputControllerRegistry::clear(){clm_.clear();}


} //end namespace dqmevf
