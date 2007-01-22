#ifndef btagBTag_CombinedSVCalibInput_h
#define btagBTag_CombinedSVCalibInput_h

#include "DataFormats/BTauReco/interface/TaggingVariable.h"
#include "RecoBTag/CombinedSV/interface/PartonTypes.h"
#include "DataFormats/BTauReco/interface/VertexTypes.h"
#include <iosfwd>

namespace combsv {
  class CombinedSVCalibInput
  {
    /** \class CombinedSVCalibInput
      * \short collection of input parameters used for calibration
      * \long  specifically, this class collects the vertex type,
      * the parton type, the tagging variable, and jet pt and jet eta.
      * These five variables define the calibration category.
      */
    public:
      CombinedSVCalibInput ( reco::btag::Vertices::VertexType,
          combsv::Partons::PartonType,
          reco::btag::TaggingVariableName,
          double jetpt, double jeteta );

      void setTaggingVariable ( reco::btag::TaggingVariableName t );

      reco::btag::Vertices::VertexType vertexType() const;
      combsv::Partons::PartonType partonType() const;
      reco::btag::TaggingVariableName taggingVar() const;
      double jetPt() const;
      double jetEta() const;
      void print() const;
      std::string describe() const;

    private:
      reco::btag::Vertices::VertexType vertexType_;
      combsv::Partons::PartonType partonType_;
      reco::btag::TaggingVariableName taggingVar_;
      double jetPt_;
      double jetEta_;
  };
}

std::ostream & operator<< ( std::ostream & os, const combsv::CombinedSVCalibInput & );

#endif
