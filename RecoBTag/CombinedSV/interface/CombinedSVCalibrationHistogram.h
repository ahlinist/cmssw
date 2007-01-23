#ifndef RecoBTag_CombinedSVCalibrationHistogram_h
#define RecoBTag_CombinedSVCalibrationHistogram_h

#include "DataFormats/BTauReco/interface/TaggingVariable.h"
#include "RecoBTag/XMLCalibration/interface/CalibratedHistogramXML.h"
#include "RecoBTag/CombinedSV/interface/PartonTypes.h"
#include "DataFormats/BTauReco/interface/VertexTypes.h"

namespace combsv {
  class CombinedSVCalibrationHistogram : public CalibratedHistogramXML {
  /** \class CombinedSVCalibrationHistogram
    * \short histogram used to store PDF
    */
  public:
    CombinedSVCalibrationHistogram();
    CombinedSVCalibrationHistogram(double ptMin,  double ptMax,
                                     double etaMin, double etaMax,
                                     reco::btag::Vertices::VertexType      vertexType,
                                     combsv::Partons::PartonType      partonType,
                                     reco::btag::TaggingVariableName taggingVar,
                                     const std::vector<double> & binlowerEdge);
    ~CombinedSVCalibrationHistogram() {}
    
    void startCalibration();
    void updateCalibration( const combsv::CombinedSVCalibInput &,
                            const std::vector<double> & binContent);
    void finishCalibration();

  private:
    std::vector<double> binLowerEdge_;
  };
}

#endif
