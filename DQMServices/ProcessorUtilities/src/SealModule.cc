#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"
#include "DQMServices/ProcessorUtilities/interface/MSService.h"
#include "DQMServices/ProcessorUtilities/interface/InputControllerRegistry.h"

using namespace edm::serviceregistry;
using namespace dqmevf;

typedef AllArgsMaker<MSService> MSServiceMaker;
typedef ParameterSetMaker<InputControllerRegistry> InputControllerMaker;

DEFINE_FWK_SERVICE_MAKER(MSService, MSServiceMaker);
DEFINE_FWK_SERVICE_MAKER(InputControllerRegistry,InputControllerMaker);

