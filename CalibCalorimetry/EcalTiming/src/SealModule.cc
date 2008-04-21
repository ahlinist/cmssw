#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CalibCalorimetry/EcalTiming/interface/EcalTimingLaser.h"
#include "CalibCalorimetry/EcalTiming/interface/EcalTimingCosmic.h"

DEFINE_SEAL_MODULE();

DEFINE_ANOTHER_FWK_MODULE( EcalTimingLaser );
DEFINE_ANOTHER_FWK_MODULE( EcalTimingCosmic );
