#ifndef RecoBTag_CombinedSVCalibrationCategory_H
#define RecoBTag_CombinedSVCalibrationCategory_H

#include "RecoBTag/XMLCalibration/interface/CalibrationCategory.h"
#include "RecoBTag/CombinedSV/interface/CombinedSVCalibInput.h"
#include "DataFormats/BTauReco/interface/TaggingVariable.h"
#include "RecoBTag/CombinedSV/interface/PartonTypes.h"
#include "DataFormats/BTauReco/interface/VertexTypes.h"
#include <iosfwd>

namespace combsv {
  class CombinedSVCalibrationCategory : 
    public CalibrationCategory<combsv::CombinedSVCalibInput> {
  /** \class CombinedSVCalibrationCategory
    * \short calibration for combined b-tagging
    */
  public:
    CombinedSVCalibrationCategory() {};
    CombinedSVCalibrationCategory(double ptMin,  double ptMax,
            double etaMin, double etaMax,
            reco::btag::Vertices::VertexType      vertexType,
            combsv::Partons::PartonType      partonType,
            reco::btag::TaggingVariableName taggingVar);
    ~CombinedSVCalibrationCategory() {};

    bool match(const combsv::CombinedSVCalibInput& input) const;
    string name() {return "CombinedSVCalibrationCategory";}

    reco::btag::Vertices::VertexType      vertexType() const;
    combsv::Partons::PartonType      partonType() const;
    reco::btag::TaggingVariableName taggingVar() const;

    double jetPtMin() const;
    double jetPtMax() const;
    double jetEtaMin() const;
    double jetEtaMax() const;

    #ifndef RAVE
    void readFromDOM( XERCES_CPP_NAMESPACE::DOMElement * );
    void saveToDOM( XERCES_CPP_NAMESPACE::DOMElement * );
    #endif

    
 private:

  double jetPtMin_;
  double jetPtMax_;
  
  double jetEtaMin_;
  double jetEtaMax_;

  reco::btag::Vertices::VertexType      vertexType_;
  combsv::Partons::PartonType      partonType_;
  reco::btag::TaggingVariableName taggingVar_;
  };
}

std::ostream & operator<< ( std::ostream & os, const combsv::CombinedSVCalibrationCategory & );

#endif
