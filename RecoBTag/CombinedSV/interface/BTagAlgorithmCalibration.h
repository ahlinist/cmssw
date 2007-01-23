#ifndef RecoBTag_BTagAlgorithmCalibration_h
#define RecoBTag_BTagAlgorithmCalibration_h

#include "RecoBTag/CombinedSV/interface/CombinedSVCalibrationCategory.h"
#include "RecoBTag/XMLCalibration/interface/CalibratedHistogramXML.h"
#include "RecoBTag/XMLCalibration/interface/AlgorithmCalibration.h"

typedef AlgorithmCalibration < combsv::CombinedSVCalibrationCategory,                                        
                              CalibratedHistogramXML> BTagAlgorithmCalibration;   

#endif
