#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "CaloOnlineTools/HcalDigiPatternProducer/interface/HcalText2DigiSource.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_INPUT_SOURCE(HcalText2DigiSource);
