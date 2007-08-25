#include "RecoTauTag/PFCombinedTauTag/interface/LikelihoodRatio.h"

void LikelihoodRatio::setCandidateCategoryParameterValues(int signaltks_n,double TauCandidate_et){
  m_signaltks_n=signaltks_n;
  m_TauCandidate_et=TauCandidate_et;
}
void LikelihoodRatio::setCandidateTaggingVariableList(TaggingVariableList x){
  m_TaggingVariableList=x;
}
double LikelihoodRatio::value(){
  //         0<=  <=1  if candidate passed tracker selection, contained neutral ECAL clus. and goes through the likelihood ratio mechanism, 
  //         NaN       the values of the likelihood functions PDFs are 0;  
  
  // PDFs are obtained by variable-width slice of recjet Et, the slice width is little(big) when # tau candidates contained in slice is high(low), the center value of the recjet Et slice approximates the TauCandidate recjet Et value;  
  int minnumbercandidates_inpdf_byrecjetEtslice=0;
  if(m_signaltks_n==1) minnumbercandidates_inpdf_byrecjetEtslice=minnumbercandidates_inpdf_byrecjetEtslice_case1signalChargedHadrCand_;
  if(m_signaltks_n==3) minnumbercandidates_inpdf_byrecjetEtslice=minnumbercandidates_inpdf_byrecjetEtslice_case3signalChargedHadrCands_;
  double the_likelihoodratio_value;
  double truthmatched_likelihoodfunction_value=1.;
  double truthmatched_candidatesnumber_inrecjetEtslice=0.;
  vector<double> TagVartruthmatchedpdf_vector;
  TagVartruthmatchedpdf_vector.clear();
  double fake_likelihoodfunction_value=1.;
  double fake_candidatesnumber_inrecjetEtslice=0.;
  vector<double> TagVarfakepdf_vector;
  TagVarfakepdf_vector.clear();
  int i_recjetEtslice=0;
  bool aCalibratedHistogram_notempty=true;
  while(((int)truthmatched_candidatesnumber_inrecjetEtslice<minnumbercandidates_inpdf_byrecjetEtslice || (int)fake_candidatesnumber_inrecjetEtslice<minnumbercandidates_inpdf_byrecjetEtslice) && aCalibratedHistogram_notempty){
    aCalibratedHistogram_notempty=false;
    int i_TagVar=0;
    for (TaggingVariableList::const_iterator iTagVar=m_TaggingVariableList.begin();iTagVar!=m_TaggingVariableList.end();++iTagVar){
      int TagVarNumber=getTagVarNumber(*iTagVar);
      if (TagVarNumber==-1)continue;
      if (i_recjetEtslice==0){ 
	TagVartruthmatchedpdf_vector.push_back(0.);
	TagVarfakepdf_vector.push_back(0.);
      }
      CategoryDataInputCondDB theInput_truthmatchedcandidates(1,m_signaltks_n,m_TauCandidate_et,TagVarNumber,i_recjetEtslice);
      const CalibratedHistogram* theCalibratedHistogram_truthmatchedcandidates=(*m_CalibrationInterface).getCalibData(theInput_truthmatchedcandidates);
      if (theCalibratedHistogram_truthmatchedcandidates!=0){
	TagVartruthmatchedpdf_vector[i_TagVar]+=(*theCalibratedHistogram_truthmatchedcandidates).value((*iTagVar).second);
	if (i_TagVar==0){
	  truthmatched_candidatesnumber_inrecjetEtslice+=(*theCalibratedHistogram_truthmatchedcandidates).normalization();
	  aCalibratedHistogram_notempty=true;
	}
      }
      CategoryDataInputCondDB theInput_fakecandidates(0,m_signaltks_n,m_TauCandidate_et,TagVarNumber,i_recjetEtslice);
      const CalibratedHistogram* theCalibratedHistogram_fakecandidates=(*m_CalibrationInterface).getCalibData(theInput_fakecandidates);
      if (theCalibratedHistogram_fakecandidates!=0){
	TagVarfakepdf_vector[i_TagVar]+=(*theCalibratedHistogram_fakecandidates).value((*iTagVar).second);
	if (i_TagVar==0){
	  fake_candidatesnumber_inrecjetEtslice+=(*theCalibratedHistogram_fakecandidates).normalization();
	  aCalibratedHistogram_notempty=true;
	}
      }
      if (i_recjetEtslice!=0){
	CategoryDataInputCondDB theotherInput_truthmatchedcandidates(1,m_signaltks_n,m_TauCandidate_et,TagVarNumber,-i_recjetEtslice);
	const CalibratedHistogram* theotherCalibratedHistogram_truthmatchedcandidates=(*m_CalibrationInterface).getCalibData(theotherInput_truthmatchedcandidates);
	if (theotherCalibratedHistogram_truthmatchedcandidates!=0){
	  TagVartruthmatchedpdf_vector[i_TagVar]+=(*theotherCalibratedHistogram_truthmatchedcandidates).value((*iTagVar).second);
	  if (i_TagVar==0){
	    truthmatched_candidatesnumber_inrecjetEtslice+=(*theotherCalibratedHistogram_truthmatchedcandidates).normalization();
	    aCalibratedHistogram_notempty=true;
	  }
	}
	CategoryDataInputCondDB theotherInput_fakecandidates(0,m_signaltks_n,m_TauCandidate_et,TagVarNumber,-i_recjetEtslice);
	const CalibratedHistogram* theotherCalibratedHistogram_fakecandidates=(*m_CalibrationInterface).getCalibData(theotherInput_fakecandidates);
	if (theotherCalibratedHistogram_fakecandidates!=0){
	  TagVarfakepdf_vector[i_TagVar]+=(*theotherCalibratedHistogram_fakecandidates).value((*iTagVar).second);
	  if (i_TagVar==0){
	    fake_candidatesnumber_inrecjetEtslice+=(*theotherCalibratedHistogram_fakecandidates).normalization();  
	    aCalibratedHistogram_notempty=true;
	  }
	}    
      }
      ++i_TagVar;
    }
    ++i_recjetEtslice;
  }
  int i_TagVar=0;
  for (TaggingVariableList::const_iterator iTagVar=m_TaggingVariableList.begin();iTagVar!=m_TaggingVariableList.end();++iTagVar){
    if (truthmatched_candidatesnumber_inrecjetEtslice!=0.) TagVartruthmatchedpdf_vector[i_TagVar]/=truthmatched_candidatesnumber_inrecjetEtslice;
    if (fake_candidatesnumber_inrecjetEtslice!=0.) TagVarfakepdf_vector[i_TagVar]/=fake_candidatesnumber_inrecjetEtslice;
    if(TagVartruthmatchedpdf_vector[i_TagVar]!=0. || TagVarfakepdf_vector[i_TagVar]!=0.){
      truthmatched_likelihoodfunction_value*=TagVartruthmatchedpdf_vector[i_TagVar];
      fake_likelihoodfunction_value*=TagVarfakepdf_vector[i_TagVar];
    }
    ++i_TagVar;
  }
  if(truthmatched_likelihoodfunction_value+fake_likelihoodfunction_value!=0.){
    the_likelihoodratio_value=truthmatched_likelihoodfunction_value/(truthmatched_likelihoodfunction_value+fake_likelihoodfunction_value);  
    return (the_likelihoodratio_value);
  }else return (NAN); 
}
int LikelihoodRatio::getTagVarNumber(TaggingVariable x){
  int TagVarNumber=-1;
  switch (x.first){
  case btau::trackip2d: 
    TagVarNumber=0;
    break;
  case btau::flightDistance3DSignificance:
    TagVarNumber=1;
    break;
  case btau::piontracksEtjetEtRatio:
    TagVarNumber=2;
    break;
  case btau::neutralEnergy:
    TagVarNumber=3;
    break;
  case btau::neutralEnergyOverCombinedEnergy:
    TagVarNumber=4;
    break;
  case btau::neutralIsolEnergy:
    TagVarNumber=5;
    break;
  case btau::neutralIsolEnergyOverCombinedEnergy:
    TagVarNumber=6;
    break;
  case btau::neutralEnergyRatio:
    TagVarNumber=7;
    break;
  case btau::neutralclusterNumber:
    TagVarNumber=8;
    break;
  case btau::neutralclusterRadius:
    TagVarNumber=9;
    break;
  default:
    break;    
  }
  return (TagVarNumber);
}
