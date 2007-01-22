#ifndef RecoBTag_BTagAlgorithmCalibration_h
#define RecoBTag_BTagAlgorithmCalibration_h

#include "RecoBTag/CombinedSV/interface/CombinedSVCalibrationCategory.h"
#include "RecoBTag/XMLCalibration/interface/CalibratedHistogram.h"
#include "RecoBTag/XMLCalibration/interface/AlgorithmCalibration.h"

typedef AlgorithmCalibration < combsv::CombinedSVCalibrationCategory,                                        
                              CalibratedHistogram> BTagAlgorithmCalibration;   

#endif
