#include "DQMServices/ProcessorUtilities/interface/InputControllerRegistry.h"
#include "DQMServices/ProcessorUtilities/interface/InputController.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

namespace dqmevf{

InputController::InputController(const std::string &name) : name_(name)
{
  if(edm::Service<InputControllerRegistry>())
    edm::Service<InputControllerRegistry>()->registerSource(name_, this);
}

}
