#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "RecoTBCalo/EcalWriteGraphsForDisplay/src/EcalWriteGraphsWindows.h"
#include "RecoTBCalo/EcalWriteGraphsForDisplay/src/FoundMaximumCry.h"

DEFINE_SEAL_MODULE();

DEFINE_ANOTHER_FWK_MODULE( EcalWriteGraphsWindows );
DEFINE_ANOTHER_FWK_MODULE( FoundMaximumCry );
