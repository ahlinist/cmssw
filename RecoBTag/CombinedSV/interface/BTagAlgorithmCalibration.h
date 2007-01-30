#ifndef RecoBTag_BTagAlgorithmCalibration_h
#define RecoBTag_BTagAlgorithmCalibration_h

#include "RecoBTag/CombinedSV/interface/CombinedSVCalibrationCategory.h"
#include "RecoBTag/CombinedSV/interface/CombinedSVCalibrationHistogram.h"
#include "RecoBTag/XMLCalibration/interface/AlgorithmCalibration.h"

typedef AlgorithmCalibration < combsv::CombinedSVCalibrationCategory,                                        
                               combsv::CombinedSVCalibrationHistogram > BTagAlgorithmCalibration;

/*
class BTagAlgorithmCalibration : public AlgorithmCalibration 
      < combsv::CombinedSVCalibrationCategory, combsv::CombinedSVCalibrationHistogram > 
{
};*/

#endif
