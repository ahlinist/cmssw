#ifndef CATEGORYDATACONDDB_H
#define CATEGORYDATACONDDB_H

#include "DataFormats/BTauReco/interface/TaggingVariable.h"

#include "CondFormats/BTauObjects/interface/CombinedTauTagCategoryData.h"

using namespace reco;

class CategoryDataInputCondDB{
 public:
  CategoryDataInputCondDB(int truthmatched1orfake0candidates,int signaltks_n,double candidate_et,TaggingVariable theTagVar,int Etslicetranslation_steps_n):truthmatched1orfake0candidates_(truthmatched1orfake0candidates),signaltks_n_(signaltks_n),candidate_et_(candidate_et),theTagVar_(theTagVar),Etslicetranslation_steps_n_(Etslicetranslation_steps_n){}
  const int truthmatched1orfake0candidates_;
  const int signaltks_n_;
  const double candidate_et_;
  const TaggingVariable& theTagVar_;
  const int Etslicetranslation_steps_n_;
};
class CategoryDataCondDB{
public:
  CategoryDataCondDB(const CombinedTauTagCategoryData& x){
    CombinedTauTagCategoryData_truthmatched1orfake0candidates=x.truthmatched1orfake0candidates;
    CombinedTauTagCategoryData_signaltks_n=x.signaltks_n;
    CombinedTauTagCategoryData_EtMin=x.EtMin;
    CombinedTauTagCategoryData_EtMax=x.EtMax;
    CombinedTauTagCategoryData_theTagVar=x.theTagVar;
  }

  typedef CategoryDataInputCondDB Input;
  bool match(const CategoryDataInputCondDB& input) const{
    double modcandidate_et=input.candidate_et_+(CombinedTauTagCategoryData_EtMax-CombinedTauTagCategoryData_EtMin)*input.Etslicetranslation_steps_n_;
    bool result=(input.truthmatched1orfake0candidates_==CombinedTauTagCategoryData_truthmatched1orfake0candidates
		 && input.signaltks_n_==CombinedTauTagCategoryData_signaltks_n
		 && modcandidate_et>=CombinedTauTagCategoryData_EtMin && modcandidate_et<CombinedTauTagCategoryData_EtMax
		 && input.theTagVar_==CombinedTauTagCategoryData_theTagVar);
    return result;
  }
 private:
  int CombinedTauTagCategoryData_truthmatched1orfake0candidates;
  int CombinedTauTagCategoryData_signaltks_n;
  float CombinedTauTagCategoryData_EtMin;
  float CombinedTauTagCategoryData_EtMax;
  TaggingVariable CombinedTauTagCategoryData_theTagVar;
};

#endif
