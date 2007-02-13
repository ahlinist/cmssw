#ifndef RecoBTag_LikelihoodTeacher
#define RecoBTag_LikelihoodTeacher

#include "DataFormats/BTauReco/interface/TaggingVariable.h"
#include "RecoBTag/CombinedSV/interface/BTagAlgorithmCalibration.h"
#include "RecoBTag/CombinedSV/interface/CombinedSVCalibInput.h"
#include "RecoBTag/CombinedSVLearning/interface/Teacher.h"
#include "DataFormats/BTauReco/interface/VertexTypes.h"

class LikelihoodTeacher : public Teacher {
   public:
     LikelihoodTeacher ( BTagAlgorithmCalibration * calib, const reco::btag::Vertices::VertexType t );
     /**
      *  get the PDFs for the Likelihood ratio method
      */
     void teach ( const reco::TaggingVariableList & l, char tag );
     void teachCategory ( const reco::TaggingVariableList & l, char tag );
     const BTagAlgorithmCalibration * calibration() const;

   private:
     void fill ( const combsv::CombinedSVCalibInput &, double value );

   private:
     BTagAlgorithmCalibration * calib_;
     reco::btag::Vertices::VertexType type_;
};

#endif
