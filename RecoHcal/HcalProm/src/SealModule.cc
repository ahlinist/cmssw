#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#
#include "HcalTemplate.h"
#include "HcalProm.h"
#include "FRPedestalsMaker.h"
#include "L1GTFilter.h"
#include "HcalPromBremss.h"
#include "HcalZS.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(HcalTemplate);
DEFINE_ANOTHER_FWK_MODULE(HcalProm);
DEFINE_ANOTHER_FWK_MODULE(FRPedestalsMaker);
DEFINE_ANOTHER_FWK_MODULE(L1GTFilter);
DEFINE_ANOTHER_FWK_MODULE(HcalPromBremss);
DEFINE_ANOTHER_FWK_MODULE(HcalZS);
