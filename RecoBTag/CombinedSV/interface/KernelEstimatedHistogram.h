#ifndef RecoBTag_KernelEstimatedHistogram_h
#define RecoBTag_KernelEstimatedHistogram_h

#include "RecoBTag/CombinedSV/interface/CombinedSVCalibrationHistogram.h"

namespace combsv {
  class KernelEstimatedHistogram : public CombinedSVCalibrationHistogram {
  /** \class KernelEstimatedHistogram
    * \short extension for CombinedSVCalibrationHistogram,
    * to account for the kernel estimate
    */
  public:
    KernelEstimatedHistogram( const CombinedSVCalibrationHistogram & h );
    float binContent(int bin) const;
  private:
    void copyTrueHistogram();
    std::vector < float > m_smoothed;
  };
}

#endif
