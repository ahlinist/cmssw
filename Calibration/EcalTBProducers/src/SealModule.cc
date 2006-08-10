#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "Calibration/EcalTBProducers/interface/DumpADC.h"
#include "Calibration/EcalTBProducers/interface/TestCalib.h"

DEFINE_SEAL_MODULE();

DEFINE_ANOTHER_FWK_MODULE( DumpADC ) ;
DEFINE_ANOTHER_FWK_MODULE( TestCalib ) ;
