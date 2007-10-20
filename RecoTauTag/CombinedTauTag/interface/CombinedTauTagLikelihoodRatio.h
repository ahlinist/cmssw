#ifndef RecoTauTag_CombinedTauTagLikelihoodRatio_h
#define RecoTauTag_CombinedTauTagLikelihoodRatio_h

/* compute discriminator value from a given set of tagging variables 
 */
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoTauTag/CombinedTauTag/interface/CategoryDataCondDB.h"

#include "RecoBTag/XMLCalibration/interface/CalibrationInterface.h"

#include "DataFormats/BTauReco/interface/TaggingVariable.h"

#include "CondFormats/BTauObjects/interface/CalibratedHistogram.h"

#include <math.h>

using namespace std; 
using namespace edm;
using namespace reco;

class CombinedTauTagLikelihoodRatio{
  public:
  CombinedTauTagLikelihoodRatio(const ParameterSet& parameters){
    minnumbercandidates_inpdf_byrecjetEtslice_case1signaltk_  = parameters.getParameter<int>("minnumbercandidates_inpdf_byrecjetEtslice_case1signaltk");
    minnumbercandidates_inpdf_byrecjetEtslice_case3signaltks_ = parameters.getParameter<int>("minnumbercandidates_inpdf_byrecjetEtslice_case3signaltks");   
  }
  ~CombinedTauTagLikelihoodRatio(){
    if(m_CalibrationInterface!=0) delete m_CalibrationInterface;
  };
  void setCalibrationInterface(CalibrationInterface<CategoryDataCondDB,CalibratedHistogram>* x){m_CalibrationInterface=x;}
  void setCandidateCategoryParameterValues(int signaltks_n,double TauCandidate_et);
  void setCandidateTaggingVariableList(TaggingVariableList x);
  double value();    
private:
  CalibrationInterface<CategoryDataCondDB,CalibratedHistogram>* m_CalibrationInterface;
  TaggingVariableList m_TaggingVariableList;
  int getTagVarNumber(TaggingVariable);
  int m_signaltks_n;
  double m_TauCandidate_et;
  int minnumbercandidates_inpdf_byrecjetEtslice_case1signaltk_;
  int minnumbercandidates_inpdf_byrecjetEtslice_case3signaltks_;      
};
#endif
