#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CalibCalorimetry/EcalTiming/interface/EcalTimingLaser.h"
#include "CalibCalorimetry/EcalTiming/interface/EcalTimingCosmic.h"
#include "CalibCalorimetry/EcalTiming/interface/EcalTimingAnalysis.h"

DEFINE_SEAL_MODULE();

DEFINE_ANOTHER_FWK_MODULE( EcalTimingLaser );
DEFINE_ANOTHER_FWK_MODULE( EcalTimingCosmic );
DEFINE_ANOTHER_FWK_MODULE( EcalTimingAnalysis );

