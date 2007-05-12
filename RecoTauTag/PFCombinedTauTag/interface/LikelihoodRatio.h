#ifndef RecoTauTag_LikelihoodRatio_h
#define RecoTauTag_LikelihoodRatio_h

/* compute discriminator value from a given set of tagging variables 
 */
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoTauTag/PFCombinedTauTag/interface/CategoryDataCondDB.h"

#include "RecoBTag/XMLCalibration/interface/CalibrationInterface.h"

#include "DataFormats/BTauReco/interface/TaggingVariable.h"

#include "CondFormats/BTauObjects/interface/CalibratedHistogram.h"

#include <math.h>

using namespace std; 
using namespace edm;
using namespace reco;

class LikelihoodRatio{
  public:
  LikelihoodRatio(const ParameterSet& parameters){
    minnumbercandidates_inpdf_byrecjetEtslice_case1signalChargedHadrCand_  = parameters.getParameter<int>("minnumbercandidates_inpdf_byrecjetEtslice_case1signalChargedHadrCand");
    minnumbercandidates_inpdf_byrecjetEtslice_case3signalChargedHadrCands_ = parameters.getParameter<int>("minnumbercandidates_inpdf_byrecjetEtslice_case3signalChargedHadrCands");   
  }
  ~LikelihoodRatio(){
    if(m_CalibrationInterface!=0) delete m_CalibrationInterface;
  };
  void setCalibrationInterface(CalibrationInterface<CategoryDataCondDB,CalibratedHistogram>* x){m_CalibrationInterface=x;}
  void setCandidateCategoryParameterValues(int signaltks_n,double TauCandidate_et);
  void setCandidateTaggingVariableList(TaggingVariableList x);
  double value();    
private:
  CalibrationInterface<CategoryDataCondDB,CalibratedHistogram>* m_CalibrationInterface;
  int getTagVarNumber(TaggingVariable);
  TaggingVariableList m_TaggingVariableList;
  int m_signaltks_n;
  double m_TauCandidate_et;
  int minnumbercandidates_inpdf_byrecjetEtslice_case1signalChargedHadrCand_;
  int minnumbercandidates_inpdf_byrecjetEtslice_case3signalChargedHadrCands_;      
};
#endif
