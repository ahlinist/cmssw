#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "RecoTBCalo/EcalTBTiming/src/EcalTimingLaser.h"
#include "RecoTBCalo/EcalTBTiming/src/EcalTimingTBElectron.h"

DEFINE_SEAL_MODULE();

DEFINE_ANOTHER_FWK_MODULE( EcalTimingLaser );
DEFINE_ANOTHER_FWK_MODULE( EcalTimingTBElectron );
