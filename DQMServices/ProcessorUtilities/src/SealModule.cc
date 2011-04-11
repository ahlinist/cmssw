#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"
#include "DQMServices/ProcessorUtilities/interface/MSService.h"
#include "DQMServices/ProcessorUtilities/interface/InputControllerRegistry.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"
//#include "DQMServices/ProcessorUtilities/interface/EventStreamHttpReader.h"
//#include "DQMServices/ProcessorUtilities/interface/DQMHttpSource.h"

using namespace edm::serviceregistry;
using namespace dqmevf;
//using edm::EventStreamHttpReader;
//using edm::DQMHttpSource;

typedef AllArgsMaker<MSService> MSServiceMaker;
typedef ParameterSetMaker<InputControllerRegistry> InputControllerMaker;

DEFINE_FWK_SERVICE_MAKER(MSService, MSServiceMaker);
DEFINE_FWK_SERVICE_MAKER(InputControllerRegistry,InputControllerMaker);
//DEFINE_FWK_INPUT_SOURCE(EventStreamHttpReader);
//DEFINE_FWK_INPUT_SOURCE(DQMHttpSource);

