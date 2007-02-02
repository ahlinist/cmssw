#ifndef RecoBTag_CombinedSVCalibrationHistogram_h
#define RecoBTag_CombinedSVCalibrationHistogram_h

#include "DataFormats/BTauReco/interface/TaggingVariable.h"
#include "RecoBTag/XMLCalibration/interface/CalibratedHistogramXML.h"
#include "RecoBTag/CombinedSV/interface/PartonTypes.h"
#include "DataFormats/BTauReco/interface/VertexTypes.h"
#include "RecoBTag/CombinedSV/interface/CombinedSVCalibrationCategory.h"

namespace combsv {
  class CombinedSVCalibrationHistogram : public CalibratedHistogramXML {
  /** \class CombinedSVCalibrationHistogram
    * \short histogram used to store PDF
    */
  public:
    CombinedSVCalibrationHistogram();
    CombinedSVCalibrationHistogram(float ptMin,  float ptMax,
                   float etaMin, float etaMax,
                   reco::btag::Vertices::VertexType vertexType,
                   combsv::Partons::PartonType partonType,
                   reco::btag::TaggingVariableName taggingVar,
                   const std::vector < float > & binlowerEdge,
                   int nentries );

    ~CombinedSVCalibrationHistogram() {}

    int binCount ( int bin ) const; /// get number of counts in bin
    void setBinCount ( int b, int v ); /// set number of counts in bin
    void addCount ( float x );

    int entries() const;
    void setEntries ( int i );
    void addEntries ( int i );

    combsv::CombinedSVCalibrationCategory & category();
    const combsv::CombinedSVCalibrationCategory & constCategory() const;
    
  protected:
    combsv::CombinedSVCalibrationCategory category_;
    std::vector< float > binLowerEdge_;
    int nentries_;
  };
}

#endif
