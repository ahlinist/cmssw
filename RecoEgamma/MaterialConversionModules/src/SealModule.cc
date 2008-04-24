#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "RecoEgamma/MaterialConversionModules/interface/ClusterAndHitsProducer.h"
#include "RecoEgamma/MaterialConversionModules/interface/ClusterAndHitsAnalyzer.h"


DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(ClusterAndHitsProducer);
DEFINE_ANOTHER_FWK_MODULE(ClusterAndHitsAnalyzer);
