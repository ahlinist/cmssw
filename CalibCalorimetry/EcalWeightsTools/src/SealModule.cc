#include "FWCore/Framework/interface/MakerMacros.h"
#include "CalibCalorimetry/EcalWeightsTools/interface/EcalProfileBuilder.h"
#include "CalibCalorimetry/EcalWeightsTools/interface/EcalWeightsBuilder.h"
#include "CalibCalorimetry/EcalWeightsTools/interface/EcalPedestalAnalyzer.h"
#include "CalibCalorimetry/EcalWeightsTools/interface/EcalGroupIdBuilder.h"

DEFINE_SEAL_MODULE();

DEFINE_ANOTHER_FWK_MODULE(EcalProfileBuilder);
DEFINE_ANOTHER_FWK_MODULE(EcalWeightsBuilder);
DEFINE_ANOTHER_FWK_MODULE(EcalPedestalAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(EcalGroupIdBuilder);
