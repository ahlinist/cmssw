#include "RecoVertex/VertexPrimitives/interface/VertexFitter.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "RecoVertex/VertexPrimitives/interface/ConvertError.h"
#include "RecoVertex/VertexPrimitives/interface/VertexException.h"
#include "RecoVertex/AdaptiveVertexFit/interface/AdaptiveVertexFitter.h"
#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"

#include "RecoTauTag/PFCombinedTauTag/interface/PFCombinedTauTagAlg.h"

void PFCombinedTauTagAlg::init(const EventSetup& theEventSetup){
  passed_LeadChargedHadrCand_selection=false;
  passed_ChargedHadrCand_signalcone_selection=false;
  passed_ChargedHadrCand_isolationring_selection=false;
  passed_ChargedHadrCand_selection=false;
  passed_noGammaCand_selection=false;
  needs_LikelihoodRatio_discrimination=false;
  is_GoodTauCand=false;
  passed_cutelectron=false;
  infact_GoodElectronCand=false;
  passed_cutmuon=false;  
  infact_GoodMuonCand=false;
  recjet_G3DV_=0;
  recjet_alternatXYZTLorentzVector_.SetPx(0.);
  recjet_alternatXYZTLorentzVector_.SetPy(0.);
  recjet_alternatXYZTLorentzVector_.SetPz(0.);
  recjet_alternatXYZTLorentzVector_.SetE(0.);
  TauCandJet_ref_et=NAN;
  ChargedHadrCands_.clear();
  signalChargedHadrCands_.clear();
  isolChargedHadrCands_.clear();
  filteredChargedHadrCands_.clear();
  filteredChargedHadrCands_XYZTLorentzVectorsum_.SetPx(0.);
  filteredChargedHadrCands_XYZTLorentzVectorsum_.SetPy(0.);
  filteredChargedHadrCands_XYZTLorentzVectorsum_.SetPz(0.);
  filteredChargedHadrCands_XYZTLorentzVectorsum_.SetE(0.);
  GammaCands_number_=0;
  GammaCands_radius_=NAN;
  GammaCands_.clear();
  GammaCands_XYZTLorentzVectorsum_.SetPx(0.);
  GammaCands_XYZTLorentzVectorsum_.SetPy(0.);
  GammaCands_XYZTLorentzVectorsum_.SetPz(0.);
  GammaCands_XYZTLorentzVectorsum_.SetE(0.);
  isolGammaCands_XYZTLorentzVectorsum_.SetPx(0.);
  isolGammaCands_XYZTLorentzVectorsum_.SetPy(0.);
  isolGammaCands_XYZTLorentzVectorsum_.SetPz(0.);
  isolGammaCands_XYZTLorentzVectorsum_.SetE(0.);
  NeutrHadrCands_number_=0;
  NeutrHadrCands_radius_=NAN;
  NeutrHadrCands_.clear();
  NeutrHadrCands_XYZTLorentzVectorsum_.SetPx(0.);
  NeutrHadrCands_XYZTLorentzVectorsum_.SetPy(0.);
  NeutrHadrCands_XYZTLorentzVectorsum_.SetPz(0.);
  NeutrHadrCands_XYZTLorentzVectorsum_.SetE(0.);  
  GammaCands_Eratio_=NAN;
  ChargedHadrCandsEt_o_jetEt_=NAN;
  GammaCandsE_o_jetalternatE_=NAN;
  isolGammaCandsE_o_jetalternatE_=NAN;
  LeadChargedHadrCand_signedipt_significance_=NAN;
  LeadChargedHadrCand_signedip3D_significance_=NAN;
  signedflightpath_significance_=NAN;
  ECALEt_o_LeadChargedHadrCandPt_=NAN;
  HCALEt_o_LeadChargedHadrCandPt_=NAN;
  couldnotobtain_ECALEt_o_LeadChargedHadrCandPt=false;
  couldnotobtain_HCALEt_o_LeadChargedHadrCandPt=false;
  couldnotobtain_LeadChargedHadrCand_signedipt=false;
  couldnotobtain_LeadChargedHadrCand_signedip3D=false;
  couldnotproduce_SV=false;
  ESHandle<TransientTrackBuilder> builder;
  theEventSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",builder);
  TransientTrackBuilder_=builder.product();
}

pair<JetTag,PFCombinedTauTagInfo> PFCombinedTauTagAlg::tag(const PFIsolatedTauTagInfoRef& thePFIsolatedTauTagInfoRef,const Vertex& iPV,Event& theEvent,const EventSetup& theEventSetup) {
  // ---
  init(theEventSetup);
  // ---
  math::XYZVector recjet_XYZVector((*thePFIsolatedTauTagInfoRef).pfjetRef()->px(),(*thePFIsolatedTauTagInfoRef).pfjetRef()->py(),(*thePFIsolatedTauTagInfoRef).pfjetRef()->pz());
  math::XYZTLorentzVector recjet_XYZTLorentzVector=(*thePFIsolatedTauTagInfoRef).pfjetRef()->p4();
  Global3DVector recjet_G3DV((*thePFIsolatedTauTagInfoRef).pfjetRef()->px(),(*thePFIsolatedTauTagInfoRef).pfjetRef()->py(),(*thePFIsolatedTauTagInfoRef).pfjetRef()->pz());
  recjet_G3DV_=&recjet_G3DV;
  
  math::XYZVector refAxis_XYZVector=recjet_XYZVector;
  math::XYZTLorentzVector refAxis_XYZTLorentzVector=recjet_XYZTLorentzVector;
  
  PFCombinedTauTagInfo resultExtended;
  
  // *************rec. PF charged hadron candidates info filling ***************
  // *********************begin***************************
  PFCandidateRef theLeadChargedHadrCand;
  if (UseOnlyChargedHadr_for_LeadCand_) theLeadChargedHadrCand=(*thePFIsolatedTauTagInfoRef).leadPFChargedHadrCand(recjet_XYZVector,MatchingConeSize_,LeadChargedHadrCand_minPt_); 
  else theLeadChargedHadrCand=(*thePFIsolatedTauTagInfoRef).leadPFCand(recjet_XYZVector,MatchingConeSize_,LeadChargedHadrCand_minPt_); 
  if(!theLeadChargedHadrCand.isNull()){
    passed_LeadChargedHadrCand_selection=true;
    refAxis_XYZVector=theLeadChargedHadrCand->momentum();
    refAxis_XYZTLorentzVector=theLeadChargedHadrCand->p4();
  }
  for(PFCandidateRefVector::const_iterator i_ChargedHadrCand=(*thePFIsolatedTauTagInfoRef).PFChargedHadrCands().begin();i_ChargedHadrCand!=(*thePFIsolatedTauTagInfoRef).PFChargedHadrCands().end();i_ChargedHadrCand++){
    if(ROOT::Math::VectorUtil::DeltaR((**i_ChargedHadrCand).p4(),refAxis_XYZTLorentzVector)>ChargedHadrLeadChargedHadrCand_ConeSize_)continue;
    ChargedHadrCands_.push_back(*i_ChargedHadrCand);
    if(ROOT::Math::VectorUtil::DeltaR((**i_ChargedHadrCand).p4(),refAxis_XYZTLorentzVector)>IsolConeSize_) continue; 
    filteredChargedHadrCands_.push_back(*i_ChargedHadrCand);
    filteredChargedHadrCands_XYZTLorentzVectorsum_+=(**i_ChargedHadrCand).p4();
  }
  // ***********end of rec. PF charged hadron candidates info filling *************
  
  // *************rec. PF gamma candidates info filling ***************
  // ************************begin*****************************
  for(PFCandidateRefVector::const_iterator i_GammaCand=(*thePFIsolatedTauTagInfoRef).PFGammaCands().begin();i_GammaCand!=(*thePFIsolatedTauTagInfoRef).PFGammaCands().end();i_GammaCand++){
    if(ROOT::Math::VectorUtil::DeltaR((**i_GammaCand).p4(),refAxis_XYZTLorentzVector)>GammaCandLeadChargedHadrCand_ConeSize_) continue;
    GammaCands_.push_back(*i_GammaCand);
  }
  // ***********end of rec. PF gamma candidates info filling *************
  
  // *************rec. PF neutral hadron candidates info filling ***************
  // ************************begin*****************************
  for(PFCandidateRefVector::const_iterator i_NeutrHadrCand=(*thePFIsolatedTauTagInfoRef).PFNeutrHadrCands().begin();i_NeutrHadrCand!=(*thePFIsolatedTauTagInfoRef).PFNeutrHadrCands().end();i_NeutrHadrCand++){
    if(ROOT::Math::VectorUtil::DeltaR((**i_NeutrHadrCand).p4(),refAxis_XYZTLorentzVector)>NeutrHadrCandLeadChargedHadrCand_ConeSize_) continue;
    NeutrHadrCands_.push_back(*i_NeutrHadrCand);
  }
  // ***********end of rec. PF neutral hadron candidates info filling *************
   
  // ************* recjet_alternatXYZTLorentzVector(tau candidate alternative math::XYZTLorentzVector) filling *************
  for (PFCandidateRefVector::const_iterator iGammaCand=GammaCands_.begin();iGammaCand!=GammaCands_.end();iGammaCand++) recjet_alternatXYZTLorentzVector_+=(**iGammaCand).p4();
  for (PFCandidateRefVector::const_iterator iChargedHadrCand=ChargedHadrCands_.begin();iChargedHadrCand!=ChargedHadrCands_.end();iChargedHadrCand++) recjet_alternatXYZTLorentzVector_+=(**iChargedHadrCand).p4();
  // *******************************end of recjet_alternatXYZTLorentzVector filling *********************************
  
  // tau candidate rec. Et: Et,recjet / Et,GammaCands+ChargedHadrCands.
  TauCandJet_ref_et=recjet_XYZTLorentzVector.Et();               // Et,recjet
  //TauCandJet_ref_et=recjet_alternatXYZTLorentzVector.et();      // Et,GammaCands+ChargedHadrCands
  
  // ***********defining evolutive size signal cones ************
  // *************************BEGIN******************************
  double GammaCand_signalConeSize = min(0.15,3./TauCandJet_ref_et+0.05);
  GammaCand_signalConeSize = max(GammaCand_signalConeSize,0.08);
  double ChargedHadrCand_SignalConeSize = SignalConeSize_ifnotvariable_;
  if(UseSignalConeVariableSize_){
    ChargedHadrCand_SignalConeSize = min(SignalConeVariableSize_max_,SignalConeVariableSize_Parameter_/TauCandJet_ref_et);
    ChargedHadrCand_SignalConeSize = max(SignalConeVariableSize_min_,0.05);
  }
  // **************************END*******************************
  
  // ***distinguishing between rec. PF charged hadron candidates inside signal cone AND inside isolation ring***
  // ***********************************************begin*******************************************************
  double ChargedHadrPFCand_isolation_discriminator=(*thePFIsolatedTauTagInfoRef).discriminatorByIsolPFChargedHadrCandsN(recjet_XYZVector,MatchingConeSize_,ChargedHadrCand_SignalConeSize,IsolConeSize_,UseOnlyChargedHadr_for_LeadCand_,LeadChargedHadrCand_minPt_,ChargedHadrCand_minPt_,IsolRing_Candsmaxn_); 
  if(ChargedHadrPFCand_isolation_discriminator==1.) passed_ChargedHadrCand_isolationring_selection=true;
  signalChargedHadrCands_=(*thePFIsolatedTauTagInfoRef).PFChargedHadrCandsInCone(refAxis_XYZVector,ChargedHadrCand_SignalConeSize,ChargedHadrCand_minPt_);
  isolChargedHadrCands_=(*thePFIsolatedTauTagInfoRef).PFChargedHadrCandsInBand(refAxis_XYZVector,ChargedHadrCand_SignalConeSize,IsolConeSize_,ChargedHadrCand_minPt_);
  
  int ChargedHadrCands_qsum=0;
  if((int)(signalChargedHadrCands_.size())!=0){
    for(PFCandidateRefVector::const_iterator i_ChargedHadrCand=signalChargedHadrCands_.begin();i_ChargedHadrCand!=signalChargedHadrCands_.end();i_ChargedHadrCand++) ChargedHadrCands_qsum+=(**i_ChargedHadrCand).charge();
  }
  if((((int)(signalChargedHadrCands_.size())==1 && refAxis_XYZVector.Rho()>LeadChargedHadrCand_minPt_case1signalChargedHadrCand_) || ((int)(signalChargedHadrCands_.size())==3 && refAxis_XYZVector.Rho()>LeadChargedHadrCand_minPt_case3signalChargedHadrCands_)) && abs(ChargedHadrCands_qsum)==1) passed_ChargedHadrCand_signalcone_selection=true; 
  // *** rec. PF charged hadron candidates selection
  if(passed_LeadChargedHadrCand_selection && passed_ChargedHadrCand_isolationring_selection && passed_ChargedHadrCand_signalcone_selection) passed_ChargedHadrCand_selection=true;
  // *********************************************end********************************************************
  
  // *************************** rec. PF gamma candidates treatment**************************
  // ***************************************** begin ****************************************
  if((int)GammaCands_.size()!=0){
    GammaCands_radius_=0.;
    for (PFCandidateRefVector::const_iterator iGammaCand=GammaCands_.begin();iGammaCand!=GammaCands_.end();iGammaCand++) {
      ++GammaCands_number_;
      GammaCands_radius_+=ROOT::Math::VectorUtil::DeltaR((**iGammaCand).p4(),refAxis_XYZTLorentzVector)*(**iGammaCand).energy();
      GammaCands_XYZTLorentzVectorsum_+=(**iGammaCand).p4();
      if(ROOT::Math::VectorUtil::DeltaR((**iGammaCand).p4(),refAxis_XYZTLorentzVector)>GammaCand_signalConeSize) isolGammaCands_XYZTLorentzVectorsum_+=(**iGammaCand).p4();
    }  
    if(GammaCands_XYZTLorentzVectorsum_.E()!=0.) GammaCands_radius_=GammaCands_radius_/GammaCands_XYZTLorentzVectorsum_.E();
  }
  // *** rec. PF gamma candidates selection
  if(GammaCands_XYZTLorentzVectorsum_.E()==0.) passed_noGammaCand_selection=true;
  // *****************************end of PF gamma candidates treatment*******************************
  
  // *************************** rec. PF neutral hadron candidates treatment**************************
  // ***************************************** begin ****************************************
  if((int)NeutrHadrCands_.size()!=0){
    NeutrHadrCands_radius_=0.;
    for (PFCandidateRefVector::const_iterator iNeutrHadrCand=NeutrHadrCands_.begin();iNeutrHadrCand!=NeutrHadrCands_.end();iNeutrHadrCand++) {
      ++NeutrHadrCands_number_;
      NeutrHadrCands_radius_+=ROOT::Math::VectorUtil::DeltaR((**iNeutrHadrCand).p4(),refAxis_XYZTLorentzVector)*(**iNeutrHadrCand).energy();
      NeutrHadrCands_XYZTLorentzVectorsum_+=(**iNeutrHadrCand).p4();
    }  
    if(NeutrHadrCands_XYZTLorentzVectorsum_.E()!=0.) NeutrHadrCands_radius_=NeutrHadrCands_radius_/NeutrHadrCands_XYZTLorentzVectorsum_.E();
  }
  // *****************************end of PF neutral hadron candidates treatment*******************************
  
  if(passed_ChargedHadrCand_selection && passed_noGammaCand_selection) is_GoodTauCand=true;
  if(passed_ChargedHadrCand_selection && !passed_noGammaCand_selection) needs_LikelihoodRatio_discrimination=true;
  
  // ******** filling the variables which may enter into the definition of a combined variable for discriminating tau jet from QCD jet *********
  if(GammaCands_XYZTLorentzVectorsum_.E()!=0.) GammaCands_Eratio_=isolGammaCands_XYZTLorentzVectorsum_.E()/GammaCands_XYZTLorentzVectorsum_.E();
  GammaCandsE_o_jetalternatE_=GammaCands_XYZTLorentzVectorsum_.E()/recjet_alternatXYZTLorentzVector_.E();
  isolGammaCandsE_o_jetalternatE_=isolGammaCands_XYZTLorentzVectorsum_.E()/recjet_alternatXYZTLorentzVector_.E();
  ChargedHadrCandsEt_o_jetEt_=filteredChargedHadrCands_XYZTLorentzVectorsum_.Et()/recjet_XYZTLorentzVector.Et();
  
  if(!theLeadChargedHadrCand.isNull()){
    try{
      LeadChargedHadrCand_signedipt_significance_=ChargedHadrCand_signedipt_significance(iPV,theLeadChargedHadrCand);
    }catch(cms::Exception& theexception) {
      couldnotobtain_LeadChargedHadrCand_signedipt=true;
    }
    try{      
      LeadChargedHadrCand_signedip3D_significance_=ChargedHadrCand_signedip3D_significance(iPV,theLeadChargedHadrCand);
    }catch(cms::Exception& theexception) {
      couldnotobtain_LeadChargedHadrCand_signedip3D=true;
    }
  }
  try{
    signedflightpath_significance_=signedflightpath_significance(iPV);
  }catch(cms::Exception& theexception) {
    couldnotproduce_SV=true;
  }
  
  // ***** end filling the variables which may enter into the definition of a combined variable for discriminating tau jet from QCD jet ******
  
  // *** would be temporary : code from RecoTauTag/CombinedTauTag/src/CombinedTauTagAlg.cc not translated
  couldnotobtain_ECALEt_o_LeadChargedHadrCandPt=true;
  couldnotobtain_HCALEt_o_LeadChargedHadrCandPt=true;
  infact_GoodElectronCand=true;
  infact_GoodMuonCand=true;
  /*
  // ******* 1 pi-prong/e/mu/discrimination *******  by Mangeol Dominique J., 05, slightly modified
  // ***************** begin **********************

  if(!theLeadChargedHadrCand.isNull()){
    try{          
      ECALEt_o_LeadChargedHadrCandPt_=ECALcellsEtSum_around_ChargedHadrCand(thePFIsolatedTauTagInfoRef,theEvent,theEventSetup,theLeadChargedHadrCand)/theLeadChargedHadrCand->momentum().Rho();
    }catch(cms::Exception& theexception) {
      couldnotobtain_ECALEt_o_LeadChargedHadrCandPt=true;
    }
    try{          
      HCALEt_o_LeadChargedHadrCandPt_=HCALtowersEtSum_around_ChargedHadrCand(thePFIsolatedTauTagInfoRef,theEvent,theEventSetup,theLeadChargedHadrCand)/theLeadChargedHadrCand->momentum().Rho();
    }catch(cms::Exception& theexception) {
      couldnotobtain_HCALEt_o_LeadChargedHadrCandPt=true;
    }
    if(passed_ChargedHadrCand_selection && (int)(signalChargedHadrCands_.size())==1 && !couldnotobtain_ECALEt_o_LeadChargedHadrCandPt && !couldnotobtain_HCALEt_o_LeadChargedHadrCandPt){    
      passed_cutmuon=true;
      passed_cutelectron=true;
      if((ECALEt_o_LeadChargedHadrCandPt_>electron_selection_1stmin_ECALEt_o_LeadChargedHadrCandPt_ && ECALEt_o_LeadChargedHadrCandPt_<electron_selection_1stmax_ECALEt_o_LeadChargedHadrCandPt_ && HCALEt_o_LeadChargedHadrCandPt_<electron_selection_1stmax_HCALEt_o_LeadChargedHadrCandPt_)
	 || (ECALEt_o_LeadChargedHadrCandPt_>electron_selection_2ndmin_ECALEt_o_LeadChargedHadrCandPt_ && ECALEt_o_LeadChargedHadrCandPt_<electron_selection_2ndmax_ECALEt_o_LeadChargedHadrCandPt_ && HCALEt_o_LeadChargedHadrCandPt_<electron_selection_2ndmax_HCALEt_o_LeadChargedHadrCandPt_)) passed_cutelectron=false;
      if(ECALEt_o_LeadChargedHadrCandPt_<muon_selection_max_ECALEt_o_LeadChargedHadrCandPt_ && HCALEt_o_LeadChargedHadrCandPt_<muon_selection_max_HCALEt_o_LeadChargedHadrCandPt_) passed_cutmuon=false;
    }else{
      passed_cutelectron=true;
      passed_cutmuon=true;
    }
  }else{
    passed_cutelectron=true;
    passed_cutmuon=true;
  }
  if(!passed_cutelectron) {
    infact_GoodElectronCand=true;
  }
  if(!passed_cutmuon){
    infact_GoodMuonCand=true;
  }
  */

  // ******* end 1 pi-prong/e/mu/discrimination *******
  resultExtended.setpfjetRef((*thePFIsolatedTauTagInfoRef).pfjetRef());  
  resultExtended.setisolatedtautaginfoRef(thePFIsolatedTauTagInfoRef);
  resultExtended.setselectedByPFChargedHadrCands(passed_ChargedHadrCand_selection);
  resultExtended.setPFChargedHadrCands(ChargedHadrCands_);
  resultExtended.setsignalPFChargedHadrCands(signalChargedHadrCands_);
  resultExtended.setisolPFChargedHadrCands(isolChargedHadrCands_);
  resultExtended.setselectedPFChargedHadrCands(filteredChargedHadrCands_);
  if (!couldnotobtain_LeadChargedHadrCand_signedipt) resultExtended.setleadPFChargedHadrCandsignedSipt(LeadChargedHadrCand_signedipt_significance_);
  if (!couldnotobtain_LeadChargedHadrCand_signedip3D) resultExtended.setleadPFChargedHadrCandsignedSip3D(LeadChargedHadrCand_signedip3D_significance_);
  if (!couldnotproduce_SV) resultExtended.setsignedSflightpath(signedflightpath_significance_);
  resultExtended.setPFChargedHadrCandsEtJetEtRatio(ChargedHadrCandsEt_o_jetEt_);
  resultExtended.setPFGammaCandsE(GammaCands_XYZTLorentzVectorsum_.E());
  resultExtended.setisolPFGammaCandsE(isolGammaCands_XYZTLorentzVectorsum_.E());
  resultExtended.setPFGammaCandsN(GammaCands_number_);
  if (GammaCands_XYZTLorentzVectorsum_.E()!=0.) resultExtended.setPFGammaCandsRadius(GammaCands_radius_);
  resultExtended.setPFNeutrHadrCandsE(NeutrHadrCands_XYZTLorentzVectorsum_.E());
  resultExtended.setPFNeutrHadrCandsN(NeutrHadrCands_number_);
  if (NeutrHadrCands_XYZTLorentzVectorsum_.E()!=0.) resultExtended.setPFNeutrHadrCandsRadius(NeutrHadrCands_radius_);
  resultExtended.setPFGammaCandsEJetalternatERatio(GammaCandsE_o_jetalternatE_);
  resultExtended.setisolPFGammaCandsEJetalternatERatio(isolGammaCandsE_o_jetalternatE_);
  if (GammaCands_XYZTLorentzVectorsum_.E()!=0.) resultExtended.setPFGammaCandsERatio(GammaCands_Eratio_);
  resultExtended.setalternatLorentzVect(recjet_alternatXYZTLorentzVector_);
  resultExtended.setelectronTagged(infact_GoodElectronCand);
  resultExtended.setmuonTagged(infact_GoodMuonCand);
  if (!couldnotobtain_ECALEt_o_LeadChargedHadrCandPt) resultExtended.setECALEtleadPFChargedHadrCandPtRatio(ECALEt_o_LeadChargedHadrCandPt_);
  if (!couldnotobtain_HCALEt_o_LeadChargedHadrCandPt) resultExtended.setHCALEtleadPFChargedHadrCandPtRatio(HCALEt_o_LeadChargedHadrCandPt_);
  // *** overall tau selection ***
  // **********begin**************
  if(!passed_ChargedHadrCand_selection){
    JetTag resultBase(0.);
    return pair<JetTag,PFCombinedTauTagInfo> (resultBase,resultExtended);
  }else{
    if(passed_noGammaCand_selection) {
      JetTag resultBase(1.);
      return pair<JetTag,PFCombinedTauTagInfo> (resultBase,resultExtended); 
    }else{
      (*LikelihoodRatio_).setCandidateCategoryParameterValues(signalChargedHadrCands_.size(),TauCandJet_ref_et);
      (*LikelihoodRatio_).setCandidateTaggingVariableList(taggingvariablesList());
      JetTag resultBase((*LikelihoodRatio_).value());
      return pair<JetTag,PFCombinedTauTagInfo> (resultBase,resultExtended); 
    }
  }
  // * end of overall tau selection *
}

TaggingVariableList PFCombinedTauTagAlg::taggingvariablesList(){
  TaggingVariableList the_taggingvariables_list;
  if(signalChargedHadrCands_.size()==1){
    if(Use_GammaCands_number_case1signalChargedHadrCand_) {
      TaggingVariable GammaCands_number_TagVar(btau::neutralclusterNumber,(float)GammaCands_number_);
      the_taggingvariables_list.push_back(GammaCands_number_TagVar);
    }	
    if(Use_GammaCands_radius_case1signalChargedHadrCand_){
      TaggingVariable GammaCands_radius_TagVar(btau::neutralclusterRadius,GammaCands_radius_);
      the_taggingvariables_list.push_back(GammaCands_radius_TagVar);
    }
    if(Use_GammaCandsE_ratio_case1signalChargedHadrCand_){
      TaggingVariable GammaCandsE_ratio_TagVar(btau::neutralEnergyRatio,GammaCands_Eratio_);
      the_taggingvariables_list.push_back(GammaCandsE_ratio_TagVar);
    }
    if(Use_isolGammaCandsE_o_jetalternatE_case1signalChargedHadrCand_){
      TaggingVariable isolGammaCandsE_o_jetalternatE_TagVar(btau::neutralIsolEnergyOverCombinedEnergy,isolGammaCandsE_o_jetalternatE_);
      the_taggingvariables_list.push_back(isolGammaCandsE_o_jetalternatE_TagVar);
    }
    if(Use_ChargedHadrCandEt_o_jetEt_case1signalChargedHadrCand_){
      TaggingVariable ChargedHadrCandsEt_o_jetEt_TagVar(btau::piontracksEtjetEtRatio,ChargedHadrCandsEt_o_jetEt_);
      the_taggingvariables_list.push_back(ChargedHadrCandsEt_o_jetEt_TagVar);
    }
    if(Use_ChargedHadrCand_ipt_significance_case1signalChargedHadrCand_ && !couldnotobtain_LeadChargedHadrCand_signedipt){
      TaggingVariable LeadChargedHadrCand_ipt_significance_TagVar(btau::trackip2d,fabs(LeadChargedHadrCand_signedipt_significance_));
      the_taggingvariables_list.push_back(LeadChargedHadrCand_ipt_significance_TagVar);
    }
  }
  if(signalChargedHadrCands_.size()==3){
    if(Use_GammaCands_number_case3signalChargedHadrCands_){
      TaggingVariable GammaCands_number_TagVar(btau::neutralclusterNumber,(float)GammaCands_number_);
      the_taggingvariables_list.push_back(GammaCands_number_TagVar);
    }
    if(Use_GammaCands_radius_case3signalChargedHadrCands_){
      TaggingVariable GammaCands_radius_TagVar(btau::neutralclusterRadius,GammaCands_radius_);
      the_taggingvariables_list.push_back(GammaCands_radius_TagVar);
    }
    if(Use_GammaCandsE_ratio_case3signalChargedHadrCands_){
      TaggingVariable GammaCandsE_ratio_TagVar(btau::neutralEnergyRatio,GammaCands_Eratio_);
      the_taggingvariables_list.push_back(GammaCandsE_ratio_TagVar);
    }
    if(Use_isolGammaCandsE_o_jetalternatE_case3signalChargedHadrCands_){
      TaggingVariable isolGammaCandsE_o_jetalternatE_TagVar(btau::neutralIsolEnergyOverCombinedEnergy,isolGammaCandsE_o_jetalternatE_);
      the_taggingvariables_list.push_back(isolGammaCandsE_o_jetalternatE_TagVar);
    }
    if(Use_ChargedHadrCandsEt_o_jetEt_case3signalChargedHadrCands_){
      TaggingVariable ChargedHadrCandsEt_o_jetEt_TagVar(btau::piontracksEtjetEtRatio,ChargedHadrCandsEt_o_jetEt_);
      the_taggingvariables_list.push_back(ChargedHadrCandsEt_o_jetEt_TagVar);
    }
    if(Use_signedflightpath_significance_case3signalChargedHadrCands_ && !couldnotproduce_SV){
      TaggingVariable signedflightpath_significance_TagVar(btau::flightDistance3DSignificance,signedflightpath_significance_);
      the_taggingvariables_list.push_back(signedflightpath_significance_TagVar);
    }
  }
  return the_taggingvariables_list;
}
double PFCombinedTauTagAlg::ChargedHadrCand_signedipt_significance(const Vertex& thePV,const PFCandidateRef PFChargedHadrCand){
  TrackRef PFChargedHadrCand_rectk;
  if ((*PFChargedHadrCand).blockRef()->elements().size()!=0){
    for (OwnVector<PFBlockElement>::const_iterator iPFBlock=(*PFChargedHadrCand).blockRef()->elements().begin();iPFBlock!=(*PFChargedHadrCand).blockRef()->elements().end();iPFBlock++){
      if ((*iPFBlock).type()==PFRecTrack_codenumber && ROOT::Math::VectorUtil::DeltaR((*PFChargedHadrCand).momentum(),(*iPFBlock).trackRef()->momentum())<0.001) PFChargedHadrCand_rectk=(*iPFBlock).trackRef();
    }
  }else{
    string exception_message="In PFCombinedTauTagAlg::ChargedHadrCand_signedipt_significance(.,.) - could not retrieve reco::track from the charged hadron candidate.";  
    throw cms::Exception(exception_message);
  }
  if (PFChargedHadrCand_rectk.isNull()){
    string exception_message="In PFCombinedTauTagAlg::ChargedHadrCand_signedipt_significance(.,.) - could not retrieve reco::track from the charged hadron candidate.";  
    throw cms::Exception(exception_message);
  }  
  TransientTrack the_transTrack=TransientTrackBuilder_->build(&(*PFChargedHadrCand_rectk));
  SignedTransverseImpactParameter the_Track_STIP;
  double the_signedipt_significance;
  if(the_Track_STIP.apply(the_transTrack,*recjet_G3DV_,thePV).first)
    the_signedipt_significance=the_Track_STIP.apply(the_transTrack,*recjet_G3DV_,thePV).second.significance();
  else {
    string exception_message="In PFCombinedTauTagAlg::ChargedHadrCand_signedipt_significance(.,.) - could not obtain the charged hadron candidate signed ipt.";
    throw cms::Exception(exception_message);
  }
  return(the_signedipt_significance);  
}
double PFCombinedTauTagAlg::ChargedHadrCand_signedip3D_significance(const Vertex& thePV,const PFCandidateRef PFChargedHadrCand){
  TrackRef PFChargedHadrCand_rectk;
  if ((*PFChargedHadrCand).blockRef()->elements().size()!=0){
    for (OwnVector<PFBlockElement>::const_iterator iPFBlock=(*PFChargedHadrCand).blockRef()->elements().begin();iPFBlock!=(*PFChargedHadrCand).blockRef()->elements().end();iPFBlock++){
      if ((*iPFBlock).type()==PFRecTrack_codenumber && ROOT::Math::VectorUtil::DeltaR((*PFChargedHadrCand).momentum(),(*iPFBlock).trackRef()->momentum())<0.001) PFChargedHadrCand_rectk=(*iPFBlock).trackRef();
    }
  }else{
    string exception_message="In PFCombinedTauTagAlg::ChargedHadrCand_signedip3D_significance(.,.) - could not retrieve reco::track from the charged hadron candidate.";  
    throw cms::Exception(exception_message);
  }
  if (PFChargedHadrCand_rectk.isNull()){
    string exception_message="In PFCombinedTauTagAlg::ChargedHadrCand_signedip3D_significance(.,.) - could not retrieve reco::track from the charged hadron candidate.";  
    throw cms::Exception(exception_message);
  }    
  TransientTrack the_transTrack=TransientTrackBuilder_->build(&(*PFChargedHadrCand_rectk));
  SignedImpactParameter3D the_Track_STIP;
  double the_signedip3D_significance;
  if(the_Track_STIP.apply(the_transTrack,*recjet_G3DV_,thePV).first)
    the_signedip3D_significance=the_Track_STIP.apply(the_transTrack,*recjet_G3DV_,thePV).second.significance();
  else{
    string exception_message="In PFCombinedTauTagAlg::ChargedHadrCand_signedip3D_significance(.,.) - could not obtain the charged hadron candidate signed ip3D.";
    throw cms::Exception(exception_message);
  }
  return(the_signedip3D_significance);
}
double PFCombinedTauTagAlg::signedflightpath_significance(const Vertex& iPV){ 
  if(filteredChargedHadrCands_.size()>2){
    vector<TransientTrack> transientTracks;
    transientTracks.clear();
    for(PFCandidateRefVector::const_iterator i_ChargedHadrCand=filteredChargedHadrCands_.begin();i_ChargedHadrCand!=filteredChargedHadrCands_.end();i_ChargedHadrCand++){
      TrackRef PFChargedHadrCand_rectk;
      if ((**i_ChargedHadrCand).blockRef()->elements().size()!=0){
	for (OwnVector<PFBlockElement>::const_iterator iPFBlock=(**i_ChargedHadrCand).blockRef()->elements().begin();iPFBlock!=(**i_ChargedHadrCand).blockRef()->elements().end();iPFBlock++){
	  if ((*iPFBlock).type()==PFRecTrack_codenumber && ROOT::Math::VectorUtil::DeltaR((**i_ChargedHadrCand).momentum(),(*iPFBlock).trackRef()->momentum())<0.001) PFChargedHadrCand_rectk=(*iPFBlock).trackRef();
	}
      }else continue;
      if (PFChargedHadrCand_rectk.isNull())continue;
      TransientTrack theTransientTrack=TransientTrackBuilder_->build(&(*PFChargedHadrCand_rectk));
      transientTracks.push_back(theTransientTrack);
    }
    try{
      AdaptiveVertexFitter AVF;
      TransientVertex tv=AVF.vertex(transientTracks); 
      VertexDistance3D theVertexDistance3D;
      double thesignedflightpath_significance=theVertexDistance3D.signedDistance(iPV,tv,*recjet_G3DV_).significance();
      return(thesignedflightpath_significance);
    }catch(cms::Exception& exception){
      throw exception;
    }catch(VertexException& exception){
      string exception_message="In PFCombinedTauTagAlg::signedflightpath_significance(.) - could not build a vtx with >2 charged hadron candidates.";
      throw cms::Exception(exception_message);
    }
    string exception_message="In PFCombinedTauTagAlg::signedflightpath_significance(.) - could not build a vtx with >2 charged hadron candidates.";
    throw cms::Exception(exception_message);
  }else {
    string exception_message="In PFCombinedTauTagAlg::signedflightpath_significance(.) - did not try to build a vtx with <3 charged hadron candidate(s).";
    throw cms::Exception(exception_message);
  }
}

double PFCombinedTauTagAlg::HCALtowersEtSum_around_ChargedHadrCand(const PFIsolatedTauTagInfoRef& thePFIsolatedTauTagInfoRef,Event& theEvent,const EventSetup& theEventSetup,const TrackRef theChargedHadrCand){   
  /*
  if((double)(*theChargedHadrCand).momentum().r()!=0.){
    Hep3Vector thelastselectedHCALCaloRecHit_Hep3V(0.,0.,0.);	   // events from CSA06-106-os-ExoticSoup-0, mc-physval-111-DiTaus-Pt20To420 samples analyzed : redundancy in the list of HCAL Towers inside the jet
    Hep3Vector theHCALCaloRecHitsSum_Hep3V(0.,0.,0.);
    Hep3Vector thepropagatedChargedHadrCand_Hep3V(0.,0.,0.);
    math::XYZPoint theChargedHadrCandImpactPositiononECAL=ChargedHadrCandImpactPositiononECAL(theEvent,theEventSetup,theChargedHadrCand);
    if(theChargedHadrCandImpactPositiononECAL.z()!=0.){
      thepropagatedChargedHadrCand_Hep3V.setRThetaPhi((*theChargedHadrCand).momentum().r(),(double)theChargedHadrCandImpactPositiononECAL.theta(),(double)theChargedHadrCandImpactPositiononECAL.phi());
      vector<CaloTowerRef> theCaloTowers=(*thePFIsolatedTauTagInfoRef).jtaRef()->key->getConstituents();
      for(vector<CaloTowerRef>::const_iterator i_Tower=theCaloTowers.begin();i_Tower!=theCaloTowers.end();i_Tower++){
	size_t numRecHits = (**i_Tower).constituentsSize();       
	for(size_t j=0;j<numRecHits;j++) {
	  DetId RecHitDetID=(**i_Tower).constituent(j);
	  DetId::Detector DetNum=RecHitDetID.det();     
	  if(DetNum==DetId::Hcal){
	    Hep3Vector theHCALCaloRecHit_Hep3V(0.,0.,0.);
	    theHCALCaloRecHit_Hep3V.setREtaPhi((double)(**i_Tower).hadEnergy(),(double)(**i_Tower).eta(),(double)(**i_Tower).phi());
	    if(ROOT::Math::VectorUtil::DeltaR(theHCALCaloRecHit_Hep3V,thepropagatedChargedHadrCand_Hep3V)<CaloHitLeadChargedHadrCand_ConeSize_) {
	     if(thelastselectedHCALCaloRecHit_Hep3V.r()==0. || (thelastselectedHCALCaloRecHit_Hep3V.r()!=0. && theHCALCaloRecHit_Hep3V.diff2(thelastselectedHCALCaloRecHit_Hep3V)>0.01)){
	       thelastselectedHCALCaloRecHit_Hep3V=theHCALCaloRecHit_Hep3V;
	       theHCALCaloRecHitsSum_Hep3V+=theHCALCaloRecHit_Hep3V;
	     }
	    }  
	  }
	}
      }
      return(theHCALCaloRecHitsSum_Hep3V.perp());
    }else{
      string exception_message="In PFCombinedTauTagAlg::HCALcellsEtSum_around_ChargedHadrCand(.) - could not find the Lead. ChargedHadrCand contact on ECAL surface point.";
      throw cms::Exception(exception_message);      
    };
  }else{
    string exception_message="In PFCombinedTauTagAlg::HCALcellsEtSum_around_ChargedHadrCand(.) - the Lead. ChargedHadrCand has 0GeV impulsion.";
    throw cms::Exception(exception_message); 
  }
  */
  return(NAN);
}

double PFCombinedTauTagAlg::ECALcellsEtSum_around_ChargedHadrCand(const PFIsolatedTauTagInfoRef& thePFIsolatedTauTagInfoRef,Event& theEvent,const EventSetup& theEventSetup,const TrackRef theChargedHadrCand){ 
  /*
  if((double)(*theChargedHadrCand).momentum().r()!=0.){
    ESHandle<CaloGeometry> theCaloGeometry;
    theEventSetup.get<IdealGeometryRecord>().get(theCaloGeometry);
    const CaloSubdetectorGeometry* theCaloSubdetectorGeometry;
    
    Hep3Vector theECALCaloRecHitsSum_Hep3V(0.,0.,0.);
    Hep3Vector thepropagatedChargedHadrCand_Hep3V(0.,0.,0.);
    math::XYZPoint theChargedHadrCandImpactPositiononECAL=ChargedHadrCandImpactPositiononECAL(theEvent,theEventSetup,theChargedHadrCand);
    if(theChargedHadrCandImpactPositiononECAL.z()!=0.){
      thepropagatedChargedHadrCand_Hep3V.setRThetaPhi((*theChargedHadrCand).momentum().r(),(double)theChargedHadrCandImpactPositiononECAL.theta(),(double)theChargedHadrCandImpactPositiononECAL.phi());
      vector<CaloTowerRef> theCaloTowers=(*thePFIsolatedTauTagInfoRef).jtaRef()->key->getConstituents();
      Handle<EBRecHitCollection> EBRecHits;
      Handle<EERecHitCollection> EERecHits;     
      theEvent.getByLabel("ecalRecHit","EcalRecHitsEB",EBRecHits );
      theEvent.getByLabel("ecalRecHit","EcalRecHitsEE",EERecHits );
      for(vector<CaloTowerRef>::const_iterator i_Tower=theCaloTowers.begin();i_Tower!=theCaloTowers.end();i_Tower++){
	size_t numRecHits = (**i_Tower).constituentsSize();
	for(size_t j=0;j<numRecHits;j++) {
          DetId RecHitDetID=(**i_Tower).constituent(j);
          DetId::Detector DetNum=RecHitDetID.det();     
          if(DetNum==DetId::Ecal){
            int EcalNum=RecHitDetID.subdetId();
	    if(EcalNum==1){
	      theCaloSubdetectorGeometry = theCaloGeometry->getSubdetectorGeometry(DetId::Ecal,EcalBarrel);
	      EBDetId EcalID=RecHitDetID;
              EBRecHitCollection::const_iterator theRecHit=EBRecHits->find(EcalID);
	      const CaloCellGeometry* theRecHitCell=theCaloSubdetectorGeometry->getGeometry(RecHitDetID);
	      Hep3Vector theECALCaloRecHit_Hep3V(0.,0.,0.);
	      theECALCaloRecHit_Hep3V.setREtaPhi((double)theRecHit->energy(),(double)theRecHitCell->getPosition().eta(),(double)theRecHitCell->getPosition().phi());
	      if(ROOT::Math::VectorUtil::DeltaR(theECALCaloRecHit_Hep3V,thepropagatedChargedHadrCand_Hep3V)<CaloHitLeadChargedHadrCand_ConeSize_) theECALCaloRecHitsSum_Hep3V+=theECALCaloRecHit_Hep3V;
            }
            else if(EcalNum==2){
	      theCaloSubdetectorGeometry = theCaloGeometry->getSubdetectorGeometry(DetId::Ecal,EcalEndcap);
	      EEDetId EcalID = RecHitDetID;
              EERecHitCollection::const_iterator theRecHit=EERecHits->find(EcalID);	    
	      const CaloCellGeometry* theRecHitCell=theCaloSubdetectorGeometry->getGeometry(RecHitDetID);
	      Hep3Vector theECALCaloRecHit_Hep3V(0.,0.,0.);
	      theECALCaloRecHit_Hep3V.setREtaPhi((double)theRecHit->energy(),(double)theRecHitCell->getPosition().eta(),(double)theRecHitCell->getPosition().phi());
	      if(ROOT::Math::VectorUtil::DeltaR(theECALCaloRecHit_Hep3V,thepropagatedChargedHadrCand_Hep3V)<CaloHitLeadChargedHadrCand_ConeSize_) theECALCaloRecHitsSum_Hep3V+=theECALCaloRecHit_Hep3V;
	    }
          }
	}
      }
      return(theECALCaloRecHitsSum_Hep3V.perp());
    }else{
      string exception_message="In PFCombinedTauTagAlg::ECALcellsEtSum_around_ChargedHadrCand(.) - could not find the Lead. ChargedHadrCand contact on ECAL surface point.";
      throw cms::Exception(exception_message);      
    };
  }else{
    string exception_message="In PFCombinedTauTagAlg::ECALcellsEtSum_around_ChargedHadrCand(.) - the Lead. ChargedHadrCand has 0GeV impulsion.";
    throw cms::Exception(exception_message); 
  }
  */
  return(NAN);
}
