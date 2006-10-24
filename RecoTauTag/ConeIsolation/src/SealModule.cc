#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "RecoTauTag/ConeIsolation/interface/ConeIsolation.h"
#include "RecoTauTag/ConeIsolation/interface/TauTagTest.h"



DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(ConeIsolation);
DEFINE_ANOTHER_FWK_MODULE(TauTagTest);


