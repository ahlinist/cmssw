#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "GeneratorInterface/EDDEInterface/interface/EDDESource.h"
#include "GeneratorInterface/EDDEInterface/interface/EDDEProducer.h"
//#include "GeneratorInterface/EDDEInterface/interface/EDDEFilter.h"

//using edm::EDDESource;
using edm::EDDEProducer;

DEFINE_SEAL_MODULE();
//DEFINE_ANOTHER_FWK_INPUT_SOURCE(EDDESource);
//DEFINE_ANOTHER_FWK_MODULE(EDDEFilter);
DEFINE_ANOTHER_FWK_MODULE(EDDEProducer);
