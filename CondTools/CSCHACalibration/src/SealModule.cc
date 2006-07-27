#include "FWCore/Framework/interface/MakerMacros.h"
#include "CondTools/CSCHACalibration/src/CSCZSensorsDataConverter.h"
#include "CondTools/CSCHACalibration/src/CSCRSensorsDataConverter.h"
#include "CondTools/CSCHACalibration/src/PX_sensorsDataConverter.h"
#include "CondTools/CSCHACalibration/src/InclinometersDataConverter.h"
DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(CSCZSensorsDataConverter)
DEFINE_ANOTHER_FWK_MODULE(CSCRSensorsDataConverter)
DEFINE_ANOTHER_FWK_MODULE(PX_sensorsDataConverter)
DEFINE_ANOTHER_FWK_MODULE(InclinometersDataConverter)
