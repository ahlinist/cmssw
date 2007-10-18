#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "RecoEgamma/MaterialConversionModules/interface/Pi0ConversionProducer.h"
#include "RecoEgamma/MaterialConversionModules/interface/Pi0ConversionAnalyzer.h"


DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(Pi0ConversionProducer);
DEFINE_ANOTHER_FWK_MODULE(Pi0ConversionAnalyzer);
