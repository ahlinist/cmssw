#ifndef RecoBTag_NCategoriesTeacher
#define RecoBTag_NCategoriesTeacher

#include "DataFormats/BTauReco/interface/TaggingVariable.h"
#include "RecoBTag/CombinedSV/interface/BTagAlgorithmCalibration.h"
#include "RecoBTag/CombinedSVLearning/interface/Teacher.h"
#include "RecoBTag/CombinedSVLearning/interface/LikelihoodTeacher.h"

class NCategoriesTeacher : public Teacher {
   public:
     /**
      */
     NCategoriesTeacher ( BTagAlgorithmCalibration * reco,
                          BTagAlgorithmCalibration * pseudo,
                          BTagAlgorithmCalibration * no );
     /**
      *  get the PDFs for the Likelihood ratio method
      */
     void teach ( const reco::TaggingVariableList & l, char tag );
     const BTagAlgorithmCalibration * recoCalibration() const;
     const BTagAlgorithmCalibration * pseudoCalibration() const;
     const BTagAlgorithmCalibration * noCalibration() const;

   private:
     LikelihoodTeacher reco_;
     LikelihoodTeacher pseudo_;
     LikelihoodTeacher no_;
};

#endif
