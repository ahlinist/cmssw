#include "CalibCalorimetry/EcalTiming/interface/EcalTimingCorrection.h"
#include "CalibCalorimetry/EcalTiming/interface/EcalXMLDelaysPlotter.h"
#include "CalibCalorimetry/EcalTiming/interface/EcalCreateTTAvgTimes.h"
#include "CalibCalorimetry/EcalTiming/plugins/EcalTimePi0TreeMaker.h"
#include "CalibCalorimetry/EcalTiming/plugins/EcalTimeCalibrationValidator.h"


DEFINE_FWK_MODULE(EcalTimingCorrection);
DEFINE_FWK_MODULE(EcalXMLDelaysPlotter);
DEFINE_FWK_MODULE(EcalCreateTTAvgTimes);
DEFINE_FWK_MODULE(EcalTimePi0TreeMaker);
DEFINE_FWK_MODULE(EcalTimeCalibrationValidator);
