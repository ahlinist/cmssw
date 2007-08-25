#include "RecoTauTag/CombinedTauTag/interface/CombinedTauTagAlg.h"

#include "TrackingTools/IPTools/interface/IPTools.h"

void CombinedTauTagAlg::init(const EventSetup& theEventSetup){
  passed_leadtk_selection=false;
  passed_tracker_signalcone_selection=false;
  passed_tracker_isolationring_selection=false;
  passed_tracker_selection=false;
  passed_neutrale_selection=false;
  needs_LikelihoodRatio_discrimination=false;
  is_GoodTauCand=false;
  passed_cutelectron=false;
  infact_GoodElectronCand=false;
  passed_cutmuon=false;  
  infact_GoodMuonCand=false;
  the_recjet_GV=0;    
  the_recjet_alternatHepLV.setPx(0.);
  the_recjet_alternatHepLV.setPy(0.);
  the_recjet_alternatHepLV.setPz(0.);
  the_recjet_alternatHepLV.setE(0.);
  TauCandJet_ref_et=NAN;
  therecTracks.clear();
  chargedpicand_fromtk_HepLV.clear();
  chargedpicand_frompropagtk_HepLV.clear();
  chargedpicand_fromECALclus_HepLV.clear();
  chargedpicand_ECALclus.clear(); 
  signalchargedpicand_fromtk_HepLV.clear();
  filtered_chargedpicand_tk.clear();
  filtered_chargedpicand_fromtk_HepLV.clear();
  filtered_chargedpicand_fromtk_HepLVsum.setPx(0.);
  filtered_chargedpicand_fromtk_HepLVsum.setPy(0.);
  filtered_chargedpicand_fromtk_HepLVsum.setPz(0.);
  filtered_chargedpicand_fromtk_HepLVsum.setE(0.);
  the_neutralECALclus_number=0;
  the_neutralECALclus_radius=NAN;
  ECALclus_withmass0_HepLV.clear();
  ECALclus.clear();
  gammacand_fromneutralECALclus_HepLV.clear();
  gammacand_neutralECALclus.clear();
  the_neutralE_ratio=NAN;
  the_tksEt_o_jetEt=NAN;
  the_neutrE_o_tksEneutrE=NAN;
  the_isolneutrE_o_tksEneutrE=NAN;
  the_leadtk_signedipt_significance=NAN;
  the_leadtk_signedip3D_significance=NAN;
  the_signedflightpath_significance=NAN;
  ECALEt_o_leadtkPt=NAN;
  HCALEt_o_leadtkPt=NAN;
  couldnotobtain_ECALEt_o_leadtkPt=false;
  couldnotobtain_HCALEt_o_leadtkPt=false;
  couldnotobtain_leadtk_signedipt=false;
  couldnotobtain_leadtk_signedip3D=false;
  couldnotproduce_SV=false;
  ESHandle<TransientTrackBuilder> builder;
  theEventSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",builder);
  theTransientTrackBuilder=builder.product();
}

pair<JetTag,CombinedTauTagInfo> CombinedTauTagAlg::tag(const IsolatedTauTagInfoRef& theIsolatedTauTagInfoRef,const Vertex& iPV,Event& theEvent,const EventSetup& theEventSetup) {
  // ---
  init(theEventSetup);
  // ---
  math::XYZVector recjet_XYZVector(theIsolatedTauTagInfoRef->jet()->px(),theIsolatedTauTagInfoRef->jet()->py(),theIsolatedTauTagInfoRef->jet()->pz());
  HepLorentzVector recjet_HepLV(theIsolatedTauTagInfoRef->jet()->px(),theIsolatedTauTagInfoRef->jet()->py(),theIsolatedTauTagInfoRef->jet()->pz(),theIsolatedTauTagInfoRef->jet()->energy());
  GlobalVector recjet_GV(theIsolatedTauTagInfoRef->jet()->px(),theIsolatedTauTagInfoRef->jet()->py(),theIsolatedTauTagInfoRef->jet()->pz());
  the_recjet_GV=&recjet_GV;
  
  math::XYZVector refAxis_XYZVector=recjet_XYZVector;
  HepLorentzVector refAxis_HepLV=recjet_HepLV;
  
  double the_neutralECALclus_esum=0.;
  double the_neutralECALclus_isolband_esum=0.;
  double the_neutralECALclus_isolband_etsum=0.;
  double filtered_chargedpicand_fromtk_esum=0.;
  
  // *************rec. tracks info filling ***************
  // *********************begin***************************
  const TrackRef theLeadTrack=theIsolatedTauTagInfoRef->leadingSignalTrack(recjet_XYZVector,MatchingCone_,MinimumTransverseMomentumLeadingTrack_);
  if(!theLeadTrack.isNull()){
    passed_leadtk_selection=true;
    math::XYZVector leadingtk_XYZVector=theLeadTrack->momentum();
    HepLorentzVector leadingchargedpicand_fromtk_HepLV((double)theLeadTrack->momentum().x(),(double)theLeadTrack->momentum().y(),(double)theLeadTrack->momentum().z(),sqrt(pow((double)theLeadTrack->momentum().r(),2)+pow(chargedpi_mass,2)));
    refAxis_XYZVector=leadingtk_XYZVector;
    refAxis_HepLV=leadingchargedpicand_fromtk_HepLV;
  }
  RefVector<TrackCollection> therecTracks=theIsolatedTauTagInfoRef->selectedTracks() ;
  for(RefVector<TrackCollection>::const_iterator i_recTrack=therecTracks.begin();i_recTrack!=therecTracks.end();i_recTrack++){
    HepLorentzVector thechargedpicand_fromtk_HepLV((double)(**i_recTrack).momentum().x(),(double)(**i_recTrack).momentum().y(),(double)(**i_recTrack).momentum().z(),sqrt(pow((double)(**i_recTrack).momentum().r(),2)+pow(chargedpi_mass,2)));
    if(thechargedpicand_fromtk_HepLV.deltaR(refAxis_HepLV)>TracksLeadingTrackCone_)continue;
    chargedpicand_fromtk_HepLV.push_back(thechargedpicand_fromtk_HepLV);
    HepLorentzVector thechargedpicand_frompropagtk_HepLV(0.,0.,0.,0.);
    if((double)(**i_recTrack).momentum().r()!=0.){
      math::XYZPoint therecTrackImpactPositiononECAL=recTrackImpactPositiononECAL(theEvent,theEventSetup,*i_recTrack);
      if(therecTrackImpactPositiononECAL.z()!=0.){
	Hep3Vector thechargedpicand_frompropagtk_Hep3V(0.,0.,0.);
	thechargedpicand_frompropagtk_Hep3V.setRThetaPhi((**i_recTrack).momentum().r(),(double)therecTrackImpactPositiononECAL.theta(),(double)therecTrackImpactPositiononECAL.phi());
	thechargedpicand_frompropagtk_HepLV.setVectM(thechargedpicand_frompropagtk_Hep3V,chargedpi_mass);
      }
    }
    chargedpicand_frompropagtk_HepLV.push_back(thechargedpicand_frompropagtk_HepLV);  
    if(thechargedpicand_fromtk_HepLV.deltaR(refAxis_HepLV)>IsolationCone_) continue;  
    filtered_chargedpicand_tk.push_back(*i_recTrack);
    filtered_chargedpicand_fromtk_esum+=thechargedpicand_fromtk_HepLV.e();  
    filtered_chargedpicand_fromtk_HepLV.push_back(thechargedpicand_fromtk_HepLV);
    filtered_chargedpicand_fromtk_HepLVsum+=thechargedpicand_fromtk_HepLV;
  }
  // ***********end of rec. tracks info filling *************
  
  // *************basic ECAL clus. info filling ***************
  // ************************begin*****************************
  Handle<BasicClusterCollection> theBarrelBCCollection;
  theEvent.getByLabel("islandBasicClusters","islandBarrelBasicClusters",theBarrelBCCollection);
  for(BasicClusterCollection::const_iterator i_BC=theBarrelBCCollection->begin();i_BC!=theBarrelBCCollection->end();i_BC++) {
    Hep3Vector theBC_Hep3V(0.,0.,0.);
    theBC_Hep3V.setREtaPhi((*i_BC).energy(),(*i_BC).eta(),(*i_BC).phi());
    HepLorentzVector theBC_HepLV(theBC_Hep3V,(*i_BC).energy());      	    
    if(theBC_HepLV.deltaR(refAxis_HepLV)>ECALclusleadtk_conesize_) continue;
    if((*i_BC).energy()<ECALclus_min_e_) continue;
    ECALclus.push_back(&(*i_BC));
    ECALclus_withmass0_HepLV.push_back(theBC_HepLV);
  }
  Handle<BasicClusterCollection> theEndcapBCCollection;
  theEvent.getByLabel("islandBasicClusters","islandEndcapBasicClusters",theEndcapBCCollection);
  for(BasicClusterCollection::const_iterator i_BC=theEndcapBCCollection->begin();i_BC!=theEndcapBCCollection->end();i_BC++) {
    Hep3Vector theBC_Hep3V(0.,0.,0.);
    theBC_Hep3V.setREtaPhi((*i_BC).energy(),(*i_BC).eta(),(*i_BC).phi());
    HepLorentzVector theBC_HepLV(theBC_Hep3V,(*i_BC).energy());      	    
    if(theBC_HepLV.deltaR(refAxis_HepLV)>ECALclusleadtk_conesize_) continue;
    if((*i_BC).energy()<ECALclus_min_e_) continue;
    ECALclus.push_back(&(*i_BC));
    ECALclus_withmass0_HepLV.push_back(theBC_HepLV);
  }
  // ***********end of basic ECAL clus. info filling *************
  
  /*** call to void AssociateECALcluster_to_track()
       filling gammacand_fromneutralECALclus_HepLV,gammacand_fromneutralECALclus,
       & chargedpicand_fromECALclus_HepLV,chargedpicand_ECALclus */
  AssociateECALcluster_to_track();
  // ***
  
  // ************* the_recjet_alternatHepLV(tau candidate alternative HepLorentzVector) filling *************
  for (vector<HepLorentzVector>::const_iterator igammacand_fromneutralECALclus_HepLV=gammacand_fromneutralECALclus_HepLV.begin();igammacand_fromneutralECALclus_HepLV!=gammacand_fromneutralECALclus_HepLV.end();igammacand_fromneutralECALclus_HepLV++) the_recjet_alternatHepLV+=(*igammacand_fromneutralECALclus_HepLV);
  for (vector<HepLorentzVector>::const_iterator ichargedpicand_fromtk_HepLV=filtered_chargedpicand_fromtk_HepLV.begin();ichargedpicand_fromtk_HepLV!=filtered_chargedpicand_fromtk_HepLV.end();ichargedpicand_fromtk_HepLV++) the_recjet_alternatHepLV+=(*ichargedpicand_fromtk_HepLV);
  // *******************************end of the_recjet_alternatHepLV filling *********************************
  
  // tau candidate rec. Et: Et,recjet / Et,neutr.recclus+rectks
  TauCandJet_ref_et=recjet_HepLV.et();               // Et,recjet
  //TauCandJet_ref_et=the_recjet_alternatHepLV.et();      // Et,neutr.recclus+rectks
  
  // ***********defining evolutive size signal cones ************
  // *************************BEGIN******************************
  double neutralECALclus_signalconesize = min(0.15,3./TauCandJet_ref_et+0.05);
  neutralECALclus_signalconesize = max(neutralECALclus_signalconesize,0.08);
  double rectks_signalconesize = SignalCone_ifnotEvolutive_;
  if(EvolutiveSignalCone_){
    rectks_signalconesize = min(SignalConeVariableSize_max_,SignalConeVariableSize_Parameter_/TauCandJet_ref_et);
    rectks_signalconesize = max(rectks_signalconesize,SignalConeVariableSize_min_);
  }
  // **************************END*******************************

  // *************distinguishing between rec. tracks inside signal cone AND inside isolation ring***************
  // ***********************************************begin*******************************************************
  double trackerisolation_discriminator = 0;
  trackerisolation_discriminator=theIsolatedTauTagInfoRef->discriminator(recjet_XYZVector,MatchingCone_,rectks_signalconesize,IsolationCone_,MinimumTransverseMomentumLeadingTrack_,MinimumTransverseMomentum_,MaximumNumberOfTracksIsolationRing_); 
  if(trackerisolation_discriminator==1) passed_tracker_isolationring_selection=true;
  RefVector<TrackCollection> signalrecTracks;
  signalrecTracks=theIsolatedTauTagInfoRef->tracksInCone(refAxis_XYZVector,rectks_signalconesize,MinimumTransverseMomentum_);
  int chargedpicand_fromtk_qsum=0;
  if((int)(signalrecTracks.size())!=0){
    for(RefVector<TrackCollection>::const_iterator i_recTrack=signalrecTracks.begin();i_recTrack!=signalrecTracks.end();i_recTrack++){
      chargedpicand_fromtk_qsum+=(**i_recTrack).charge();
      HepLorentzVector thechargedpicand_fromtk_HepLV((**i_recTrack).momentum().x(),(**i_recTrack).momentum().y(),(**i_recTrack).momentum().z(),sqrt(pow((**i_recTrack).momentum().r(),2)+pow(chargedpi_mass,2)));
      signalchargedpicand_fromtk_HepLV.push_back(thechargedpicand_fromtk_HepLV);
    }
  }
  RefVector<TrackCollection> isolrecTracks;  
  RefVector<TrackCollection> isolrecTrackstmp=theIsolatedTauTagInfoRef->tracksInCone(refAxis_XYZVector,IsolationCone_,MinimumTransverseMomentum_);
  for(RefVector<TrackCollection>::const_iterator iTrack=isolrecTrackstmp.begin();iTrack!=isolrecTrackstmp.end();iTrack++){
    if(ROOT::Math::VectorUtil::DeltaR(refAxis_XYZVector,(*iTrack)->momentum())>=rectks_signalconesize) isolrecTracks.push_back(*iTrack);  
  }
  
  if((((int)(signalrecTracks.size())==1 && refAxis_XYZVector.Rho()>MinimumTransverseMomentumLeadingTrack_case1signalTrack_) || ((int)(signalrecTracks.size())==3 && refAxis_XYZVector.Rho()>MinimumTransverseMomentumLeadingTrack_case3signalTracks_)) && abs(chargedpicand_fromtk_qsum)==1) passed_tracker_signalcone_selection=true; 
    // *** tracker selection
  if(passed_leadtk_selection && passed_tracker_isolationring_selection && passed_tracker_signalcone_selection) passed_tracker_selection=true;
  // *********************************************end********************************************************
 
  // ****************************** ECAL neutral clus. treatment******************************
  // ***************************************** begin ****************************************
  if((int)gammacand_fromneutralECALclus_HepLV.size()!=0){
    the_neutralECALclus_radius=0.;
    for (vector<HepLorentzVector>::const_iterator igammacand_fromneutralECALclus_HepLV=gammacand_fromneutralECALclus_HepLV.begin();igammacand_fromneutralECALclus_HepLV!=gammacand_fromneutralECALclus_HepLV.end();igammacand_fromneutralECALclus_HepLV++) {
      ++the_neutralECALclus_number;
      the_neutralECALclus_radius+=(*igammacand_fromneutralECALclus_HepLV).deltaR(refAxis_HepLV)*(*igammacand_fromneutralECALclus_HepLV).e();
      the_neutralECALclus_esum+=(*igammacand_fromneutralECALclus_HepLV).e();
      if((*igammacand_fromneutralECALclus_HepLV).deltaR(refAxis_HepLV)>neutralECALclus_signalconesize){
	the_neutralECALclus_isolband_esum+=(*igammacand_fromneutralECALclus_HepLV).e();
	the_neutralECALclus_isolband_etsum+=(*igammacand_fromneutralECALclus_HepLV).et();
      }
    }  
    if(the_neutralECALclus_esum!=0.) the_neutralECALclus_radius=the_neutralECALclus_radius/the_neutralECALclus_esum;
  }
  // *** ECAL neutral clus. selection
  if(the_neutralECALclus_esum==0.) passed_neutrale_selection=true;
  // *****************************end of ECAL neutral clus. treatment*******************************
  if(passed_tracker_selection && passed_neutrale_selection) is_GoodTauCand=true;
  if(passed_tracker_selection && !passed_neutrale_selection) needs_LikelihoodRatio_discrimination=true;
  
  // ******** filling the variables which may enter into the definition of a combined variable for discriminating tau jet from QCD jet *********
  if(the_neutralECALclus_esum!=0.) the_neutralE_ratio=the_neutralECALclus_isolband_esum/the_neutralECALclus_esum;
  the_neutrE_o_tksEneutrE=the_neutralECALclus_esum/(the_neutralECALclus_esum+filtered_chargedpicand_fromtk_esum);
  the_isolneutrE_o_tksEneutrE=the_neutralECALclus_isolband_esum/(the_neutralECALclus_esum+filtered_chargedpicand_fromtk_esum);
  the_tksEt_o_jetEt=filtered_chargedpicand_fromtk_HepLVsum.et()/recjet_HepLV.et();
  if(!theLeadTrack.isNull()){
    try{
      the_leadtk_signedipt_significance=rectk_signedipt_significance(iPV,theLeadTrack);
    }catch(cms::Exception& theexception) {
      couldnotobtain_leadtk_signedipt=true;
    }
    try{      
      the_leadtk_signedip3D_significance=rectk_signedip3D_significance(iPV,theLeadTrack);
    }catch(cms::Exception& theexception) {
      couldnotobtain_leadtk_signedip3D=true;
    }
  }
  try{
    the_signedflightpath_significance=signedflightpath_significance(iPV);
  }catch(cms::Exception& theexception) {
    couldnotproduce_SV=true;
  }
  // ***** end filling the variables which may enter into the definition of a combined variable for discriminating tau jet from QCD jet ******
  
  // ******* 1 pi-prong/e/mu/discrimination *******  by Mangeol Dominique J., 05, slightly modified
  // ***************** begin **********************
  if(!theLeadTrack.isNull()){
    try{          
      ECALEt_o_leadtkPt=ECALcellsEtSum_around_rectk(theIsolatedTauTagInfoRef,theEvent,theEventSetup,theLeadTrack)/theLeadTrack->momentum().Rho();
    }catch(cms::Exception& theexception) {
      couldnotobtain_ECALEt_o_leadtkPt=true;
    }
    try{          
      HCALEt_o_leadtkPt=HCALtowersEtSum_around_rectk(theIsolatedTauTagInfoRef,theEvent,theEventSetup,theLeadTrack)/theLeadTrack->momentum().Rho();
    }catch(cms::Exception& theexception) {
      couldnotobtain_HCALEt_o_leadtkPt=true;
    }
    if(passed_tracker_selection && (int)(signalrecTracks.size())==1 && !couldnotobtain_ECALEt_o_leadtkPt && !couldnotobtain_HCALEt_o_leadtkPt){    
      passed_cutmuon=true;
      passed_cutelectron=true;
      if((ECALEt_o_leadtkPt>electron_selection_1stmin_ECALEt_o_leadtkPt_ && ECALEt_o_leadtkPt<electron_selection_1stmax_ECALEt_o_leadtkPt_ && HCALEt_o_leadtkPt<electron_selection_1stmax_HCALEt_o_leadtkPt_)
	 || (ECALEt_o_leadtkPt>electron_selection_2ndmin_ECALEt_o_leadtkPt_ && ECALEt_o_leadtkPt<electron_selection_2ndmax_ECALEt_o_leadtkPt_ && HCALEt_o_leadtkPt<electron_selection_2ndmax_HCALEt_o_leadtkPt_)) passed_cutelectron=false;
      if(ECALEt_o_leadtkPt<muon_selection_max_ECALEt_o_leadtkPt_ && HCALEt_o_leadtkPt<muon_selection_max_HCALEt_o_leadtkPt_) passed_cutmuon=false;
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
  // ******* end 1 pi-prong/e/mu/discrimination *******
  const JetTracksAssociationRef& theJetTracksAssociationRef=theIsolatedTauTagInfoRef->jtaRef();
  CombinedTauTagInfo resultExtended(theJetTracksAssociationRef);
  resultExtended.setisolatedtautaginfoRef(theIsolatedTauTagInfoRef);
  resultExtended.setpassed_trackerselection(passed_tracker_selection);
  resultExtended.setis_GoodTauCandidate(is_GoodTauCand);
  resultExtended.setneeds_LikelihoodRatio_discrimination(needs_LikelihoodRatio_discrimination);
  resultExtended.setsignalTks(signalrecTracks);
  resultExtended.setisolTks(isolrecTracks);
  resultExtended.setselectedTks(filtered_chargedpicand_tk);
  if (!couldnotobtain_leadtk_signedipt) resultExtended.setleadTk_signedipt_significance(the_leadtk_signedipt_significance);
  if (!couldnotobtain_leadtk_signedip3D) resultExtended.setleadTk_signedip3D_significance(the_leadtk_signedip3D_significance);
  if (!couldnotproduce_SV) resultExtended.setsignedflightpath_significance(the_signedflightpath_significance);
  resultExtended.setTksEt_o_JetEt(the_tksEt_o_jetEt);
  resultExtended.setneutralE(the_neutralECALclus_esum);
  resultExtended.setisolneutralE(the_neutralECALclus_isolband_esum);
  resultExtended.setisolneutralEtsum(the_neutralECALclus_isolband_etsum);
  resultExtended.setneutralECALClus_number(the_neutralECALclus_number);
  if (the_neutralECALclus_esum!=0.) resultExtended.setneutralECALClus_radius(the_neutralECALclus_radius);
  resultExtended.setneutralE_o_TksEneutralE(the_neutrE_o_tksEneutrE);
  resultExtended.setisolneutralE_o_TksEneutralE(the_isolneutrE_o_tksEneutrE);
  if (the_neutralECALclus_esum!=0.) resultExtended.setneutralE_ratio(the_neutralE_ratio);
  resultExtended.setalternatrecJet_HepLV(the_recjet_alternatHepLV);
  resultExtended.setinfact_GoodElectronCandidate(infact_GoodElectronCand);
  resultExtended.setinfact_GoodMuonCandidate(infact_GoodMuonCand);
  if (!couldnotobtain_ECALEt_o_leadtkPt) resultExtended.setECALEt_o_leadTkPt(ECALEt_o_leadtkPt);
  if (!couldnotobtain_HCALEt_o_leadtkPt) resultExtended.setHCALEt_o_leadTkPt(HCALEt_o_leadtkPt);
  // *** overall tau selection ***
  // **********begin**************
  if(!passed_tracker_selection){
    JetTag resultBase(0.);
    return pair<JetTag,CombinedTauTagInfo> (resultBase,resultExtended);
  }else{
    if(passed_neutrale_selection) {
      JetTag resultBase(1.);
      return pair<JetTag,CombinedTauTagInfo> (resultBase,resultExtended); 
    }else{
      (*theLikelihoodRatio).setCandidateCategoryParameterValues((int)signalchargedpicand_fromtk_HepLV.size(),TauCandJet_ref_et);
      (*theLikelihoodRatio).setCandidateTaggingVariableList(taggingvariablesList());
      JetTag resultBase((*theLikelihoodRatio).value());
      return pair<JetTag,CombinedTauTagInfo> (resultBase,resultExtended); 
    }
  }
  // * end of overall tau selection *
}

math::XYZPoint CombinedTauTagAlg::recTrackImpactPositiononECAL(Event& theEvent,const EventSetup& theEventSetup,TrackRef therecTrack){ 
  ESHandle<MagneticField> theMF;
  theEventSetup.get<IdealMagneticFieldRecord>().get(theMF);
  const MagneticField* theMagField=theMF.product();
  AnalyticalPropagator thefwdPropagator(theMagField,alongMomentum);
  math::XYZPoint proprecTrack_XYZPoint(0.,0.,0.);
  
  // get the initial rec. tk FreeTrajectoryState - at outermost point position if possible, else at innermost point position:
  GlobalVector therecTrack_initialGV(0.,0.,0.);
  GlobalPoint therecTrack_initialGP(0.,0.,0.);
  if(therecTrack->outerOk()){
    GlobalVector therecTrack_initialoutermostGV(therecTrack->outerMomentum().x(),therecTrack->outerMomentum().y(),therecTrack->outerMomentum().z());
    GlobalPoint therecTrack_initialoutermostGP(therecTrack->outerPosition().x(),therecTrack->outerPosition().y(),therecTrack->outerPosition().z());
    therecTrack_initialGV=therecTrack_initialoutermostGV;
    therecTrack_initialGP=therecTrack_initialoutermostGP;
  } else if(therecTrack->innerOk()){
    GlobalVector therecTrack_initialinnermostGV(therecTrack->innerMomentum().x(),therecTrack->innerMomentum().y(),therecTrack->innerMomentum().z());
    GlobalPoint therecTrack_initialinnermostGP(therecTrack->innerPosition().x(),therecTrack->innerPosition().y(),therecTrack->innerPosition().z());
    therecTrack_initialGV=therecTrack_initialinnermostGV;
    therecTrack_initialGP=therecTrack_initialinnermostGP;
  } else return (proprecTrack_XYZPoint);
  GlobalTrajectoryParameters therecTrack_initialGTPs(therecTrack_initialGP,therecTrack_initialGV,therecTrack->charge(),&*theMF);
  // FIX THIS !!!
  // need to convert from perigee to global or helix (curvilinear) frame
  // for now just an arbitrary matrix.
  HepSymMatrix covM_HepSM(6,1); covM_HepSM*=1e-6; // initialize to sigma=1e-3
  CartesianTrajectoryError cov_CTE(covM_HepSM);
  FreeTrajectoryState recTrack_initialFTS(therecTrack_initialGTPs,cov_CTE);
  // ***
  
  /*
    TransientTrack* the_transTrack=theTransientTrackBuilder->build(&(*therecTrack));
    the_transTrack->setES(theEventSetup);
    FreeTrajectoryState* recTrack_initialFTS=the_transTrack->outermostMeasurementState().freeTrajectoryState();
  */
  
  // propagate to ECAL surface: 
  double ECALcorner_tantheta=ECALBounds::barrel_innerradius()/ECALBounds::barrel_halfLength();
  TrajectoryStateOnSurface recTrack_propagatedonECAL_TSOS=thefwdPropagator.propagate((recTrack_initialFTS),ECALBounds::barrelBound());
  if(!recTrack_propagatedonECAL_TSOS.isValid() || fabs(recTrack_propagatedonECAL_TSOS.globalParameters().position().perp()/recTrack_propagatedonECAL_TSOS.globalParameters().position().z())<ECALcorner_tantheta) {
    if(recTrack_propagatedonECAL_TSOS.isValid() && fabs(recTrack_propagatedonECAL_TSOS.globalParameters().position().perp()/recTrack_propagatedonECAL_TSOS.globalParameters().position().z())<ECALcorner_tantheta){     
      if(recTrack_propagatedonECAL_TSOS.globalParameters().position().eta()>0.){
	recTrack_propagatedonECAL_TSOS=thefwdPropagator.propagate((recTrack_initialFTS),ECALBounds::positiveEndcapDisk());
      }else{ 
	recTrack_propagatedonECAL_TSOS=thefwdPropagator.propagate((recTrack_initialFTS),ECALBounds::negativeEndcapDisk());
      }
    }
    if(!recTrack_propagatedonECAL_TSOS.isValid()){
      if((recTrack_initialFTS).position().eta()>0.){
	recTrack_propagatedonECAL_TSOS=thefwdPropagator.propagate((recTrack_initialFTS),ECALBounds::positiveEndcapDisk());
      }else{ 
	recTrack_propagatedonECAL_TSOS=thefwdPropagator.propagate((recTrack_initialFTS),ECALBounds::negativeEndcapDisk());
      }
    }
  }
  if(recTrack_propagatedonECAL_TSOS.isValid()){
    math::XYZPoint validproprecTrack_XYZPoint(recTrack_propagatedonECAL_TSOS.globalPosition().x(),
					recTrack_propagatedonECAL_TSOS.globalPosition().y(),
					recTrack_propagatedonECAL_TSOS.globalPosition().z());
    proprecTrack_XYZPoint=validproprecTrack_XYZPoint;
  }
  return (proprecTrack_XYZPoint);
}

void CombinedTauTagAlg::AssociateECALcluster_to_track(){
  vector<const BasicCluster*> bisECALclus=ECALclus;
  vector<HepLorentzVector> bisECALclus_withmass0_HepLV=ECALclus_withmass0_HepLV;
  vector<const BasicCluster*>::iterator kmatchedECALclus;
  vector<HepLorentzVector>::iterator kmatchedECALclus_withmass0_HepLV;
  for (vector<HepLorentzVector>::iterator ichargedpicand_frompropagtk_HepLV=chargedpicand_frompropagtk_HepLV.begin();ichargedpicand_frompropagtk_HepLV!=chargedpicand_frompropagtk_HepLV.end();ichargedpicand_frompropagtk_HepLV++) {
    HepLorentzVector the_chargedpicand_fromECALclus_HepLV(0.,0.,0.,0.);
    if((*ichargedpicand_frompropagtk_HepLV).rho()==0.) {
      chargedpicand_fromECALclus_HepLV.push_back(the_chargedpicand_fromECALclus_HepLV);
      chargedpicand_ECALclus.push_back(0);
      continue;
    }
    double matchingEgammaBasicClustertrack_deltaR=matchingECALclustrack_deltaR_;
    bool track_matchedwithcluster=false;
    vector<const BasicCluster*>::iterator jECALclus=bisECALclus.begin();
    for (vector<HepLorentzVector>::iterator jECALclus_withmass0_HepLV=bisECALclus_withmass0_HepLV.begin();jECALclus_withmass0_HepLV!=bisECALclus_withmass0_HepLV.end();jECALclus_withmass0_HepLV++) {
      if((*ichargedpicand_frompropagtk_HepLV).deltaR(*jECALclus_withmass0_HepLV)<matchingEgammaBasicClustertrack_deltaR){
      	track_matchedwithcluster=true;
	matchingEgammaBasicClustertrack_deltaR=(*ichargedpicand_frompropagtk_HepLV).deltaR(*jECALclus_withmass0_HepLV);
	kmatchedECALclus_withmass0_HepLV=jECALclus_withmass0_HepLV;
	kmatchedECALclus=jECALclus;
      }
      ++jECALclus;
    }
    if(track_matchedwithcluster) {
      Hep3Vector the_chargedpicand_ECALclus_Hep3V(0.,0.,0.);
      the_chargedpicand_ECALclus_Hep3V.setRThetaPhi(sqrt(pow((double)(*kmatchedECALclus_withmass0_HepLV).e(),2)-pow(chargedpi_mass,2)),(double)(*kmatchedECALclus_withmass0_HepLV).theta(),(double)(*kmatchedECALclus_withmass0_HepLV).phi());
      the_chargedpicand_fromECALclus_HepLV.setVectM(the_chargedpicand_ECALclus_Hep3V,chargedpi_mass);
      chargedpicand_fromECALclus_HepLV.push_back(the_chargedpicand_fromECALclus_HepLV);
      chargedpicand_ECALclus.push_back(*kmatchedECALclus);
      kmatchedECALclus_withmass0_HepLV=bisECALclus_withmass0_HepLV.erase(kmatchedECALclus_withmass0_HepLV);
      kmatchedECALclus=bisECALclus.erase(kmatchedECALclus);
    }else{
      chargedpicand_fromECALclus_HepLV.push_back(the_chargedpicand_fromECALclus_HepLV);
      chargedpicand_ECALclus.push_back(0);
    }
  }
  gammacand_fromneutralECALclus_HepLV=bisECALclus_withmass0_HepLV;
  gammacand_neutralECALclus=bisECALclus;
}
TaggingVariableList CombinedTauTagAlg::taggingvariablesList(){
  TaggingVariableList the_taggingvariables_list;
  if(signalchargedpicand_fromtk_HepLV.size()==1){
    if(use_neutralECALclus_number_case1signaltk_) {
      TaggingVariable neutralECALclus_number_TagVar(btau::neutralclusterNumber,(float)the_neutralECALclus_number);
      the_taggingvariables_list.push_back(neutralECALclus_number_TagVar);
    }	
    if(use_neutralECALclus_radius_case1signaltk_){
      TaggingVariable neutralECALclus_radius_TagVar(btau::neutralclusterRadius,the_neutralECALclus_radius);
      the_taggingvariables_list.push_back(neutralECALclus_radius_TagVar);
    }
    if(use_neutralE_ratio_case1signaltk_){
      TaggingVariable neutralE_ratio_TagVar(btau::neutralEnergyRatio,the_neutralE_ratio);
      the_taggingvariables_list.push_back(neutralE_ratio_TagVar);
    }
    if(use_isolneutrE_o_tkEneutrE_case1signaltk_){
      TaggingVariable isolneutrE_o_tksEneutrE_TagVar(btau::neutralIsolEnergyOverCombinedEnergy,the_isolneutrE_o_tksEneutrE);
      the_taggingvariables_list.push_back(isolneutrE_o_tksEneutrE_TagVar);
    }
    if(use_tkEt_o_jetEt_case1signaltk_){
      TaggingVariable tksEt_o_jetEt_TagVar(btau::piontracksEtjetEtRatio,the_tksEt_o_jetEt);
      the_taggingvariables_list.push_back(tksEt_o_jetEt_TagVar);
    }
    if(use_leadtk_ipt_significance_case1signaltk_ && !couldnotobtain_leadtk_signedipt){
      TaggingVariable leadtk_ipt_significance_TagVar(btau::trackip2d,fabs(the_leadtk_signedipt_significance));
      the_taggingvariables_list.push_back(leadtk_ipt_significance_TagVar);
    }
  }
  if(signalchargedpicand_fromtk_HepLV.size()==3){
    if(use_neutralECALclus_number_case3signaltks_){
      TaggingVariable neutralECALclus_number_TagVar(btau::neutralclusterNumber,(float)the_neutralECALclus_number);
      the_taggingvariables_list.push_back(neutralECALclus_number_TagVar);
    }
    if(use_neutralECALclus_radius_case3signaltks_){
      TaggingVariable neutralECALclus_radius_TagVar(btau::neutralclusterRadius,the_neutralECALclus_radius);
      the_taggingvariables_list.push_back(neutralECALclus_radius_TagVar);
    }
    if(use_neutralE_ratio_case3signaltks_){
      TaggingVariable neutralE_ratio_TagVar(btau::neutralEnergyRatio,the_neutralE_ratio);
      the_taggingvariables_list.push_back(neutralE_ratio_TagVar);
    }
    if(use_isolneutrE_o_tksEneutrE_case3signaltks_){
      TaggingVariable isolneutrE_o_tksEneutrE_TagVar(btau::neutralIsolEnergyOverCombinedEnergy,the_isolneutrE_o_tksEneutrE);
      the_taggingvariables_list.push_back(isolneutrE_o_tksEneutrE_TagVar);
    }
    if(use_tksEt_o_jetEt_case3signaltks_){
      TaggingVariable tksEt_o_jetEt_TagVar(btau::piontracksEtjetEtRatio,the_tksEt_o_jetEt);
      the_taggingvariables_list.push_back(tksEt_o_jetEt_TagVar);
    }
    if(use_signedflightpath_significance_case3signaltks_ && !couldnotproduce_SV){
      TaggingVariable signedflightpath_significance_TagVar(btau::flightDistance3DSignificance,the_signedflightpath_significance);
      the_taggingvariables_list.push_back(signedflightpath_significance_TagVar);
    }
  }
  return the_taggingvariables_list;
}
double CombinedTauTagAlg::rectk_signedipt_significance(const Vertex& thePV,const TrackRef theTrack){ 
  TransientTrack the_transTrack=theTransientTrackBuilder->build(&(*theTrack));
  double the_signedipt_significance;
  if(IPTools::signedTransverseImpactParameter(the_transTrack,*the_recjet_GV,thePV).first)
    the_signedipt_significance=IPTools::signedTransverseImpactParameter(the_transTrack,*the_recjet_GV,thePV).second.significance();
  else {
    string exception_message="In CombinedTauTagAlg::rectk_signedipt_significance(.,.) - could not obtain the lead tk signed ipt.";
    throw cms::Exception(exception_message);
  }
  return(the_signedipt_significance);
}
double CombinedTauTagAlg::rectk_signedip3D_significance(const Vertex& thePV,const TrackRef theTrack){ 
  TransientTrack the_transTrack=theTransientTrackBuilder->build(&(*theTrack));
  double the_signedip3D_significance;
  if(IPTools::signedImpactParameter3D(the_transTrack,*the_recjet_GV,thePV).first)
    the_signedip3D_significance=IPTools::signedImpactParameter3D(the_transTrack,*the_recjet_GV,thePV).second.significance();
  else{
    string exception_message="In CombinedTauTagAlg::rectk_signedip3D_significance(.,.) - could not obtain the lead tk signed ip3D.";
    throw cms::Exception(exception_message);
  }
  return(the_signedip3D_significance);
}
double CombinedTauTagAlg::signedflightpath_significance(const Vertex& iPV){ 
  if(filtered_chargedpicand_tk.size()>2){
     vector<TransientTrack> transientTracks;
     transientTracks.clear();
     for(RefVector<TrackCollection>::const_iterator i_recTrack=filtered_chargedpicand_tk.begin();i_recTrack!=filtered_chargedpicand_tk.end();i_recTrack++){
       TransientTrack theTransientTrack=theTransientTrackBuilder->build(&(**i_recTrack));
       transientTracks.push_back(theTransientTrack);
     }
     try{
       AdaptiveVertexFitter AVF;
       TransientVertex tv = AVF.vertex(transientTracks); 
       VertexDistance3D theVertexDistance3D;
       double thesignedflightpath_significance=theVertexDistance3D.signedDistance(iPV,tv,*the_recjet_GV).significance();
       return(thesignedflightpath_significance);
     }catch(cms::Exception& exception){
       throw exception;
     }catch(VertexException& exception){
       string exception_message="In CombinedTauTagAlg::signedflightpath_significance(.) - could not build a vtx with >2 tks.";
       throw cms::Exception(exception_message);
     }
     string exception_message="In CombinedTauTagAlg::signedflightpath_significance(.) - could not build a vtx with >2 tks.";
     throw cms::Exception(exception_message);
  }else {
    string exception_message="In CombinedTauTagAlg::signedflightpath_significance(.) - did not try to build a vtx with <3 tk(s).";
    throw cms::Exception(exception_message);
  }
}

double CombinedTauTagAlg::HCALtowersEtSum_around_rectk(const IsolatedTauTagInfoRef& theIsolatedTauTagInfoRef,Event& theEvent,const EventSetup& theEventSetup,TrackRef therecTrack){   
  if((double)(*therecTrack).momentum().r()!=0.){
    Hep3Vector thelastselectedHCALCaloRecHit_Hep3V(0.,0.,0.);	   // events from CSA06-106-os-ExoticSoup-0, mc-physval-111-DiTaus-Pt20To420 samples analyzed : redundancy in the list of HCAL Towers inside the jet
    Hep3Vector theHCALCaloRecHitsSum_Hep3V(0.,0.,0.);
    Hep3Vector thepropagatedrectk_Hep3V(0.,0.,0.);
    math::XYZPoint therecTrackImpactPositiononECAL=recTrackImpactPositiononECAL(theEvent,theEventSetup,therecTrack);
    if(therecTrackImpactPositiononECAL.z()!=0.){
      thepropagatedrectk_Hep3V.setRThetaPhi((*therecTrack).momentum().r(),(double)therecTrackImpactPositiononECAL.theta(),(double)therecTrackImpactPositiononECAL.phi());
      vector<CaloTowerRef> theCaloTowers=theIsolatedTauTagInfoRef->jtaRef()->first.castTo<CaloJetRef>()->getConstituents();
      for(vector<CaloTowerRef>::const_iterator i_Tower=theCaloTowers.begin();i_Tower!=theCaloTowers.end();i_Tower++){
	size_t numRecHits = (**i_Tower).constituentsSize();       
	for(size_t j=0;j<numRecHits;j++) {
	  DetId RecHitDetID=(**i_Tower).constituent(j);
	  DetId::Detector DetNum=RecHitDetID.det();     
	  if(DetNum==DetId::Hcal){
	    Hep3Vector theHCALCaloRecHit_Hep3V(0.,0.,0.);
	    theHCALCaloRecHit_Hep3V.setREtaPhi((double)(**i_Tower).hadEnergy(),(double)(**i_Tower).eta(),(double)(**i_Tower).phi());
	    if(theHCALCaloRecHit_Hep3V.deltaR(thepropagatedrectk_Hep3V)<calotk_conesize_) {
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
      string exception_message="In CombinedTauTagAlg::HCALcellsEtSum_around_rectk(.) - could not find the lead. tk contact on ECAL surface point.";
      throw cms::Exception(exception_message);      
    };
  }else{
    string exception_message="In CombinedTauTagAlg::HCALcellsEtSum_around_rectk(.) - the lead. tk has 0GeV impulsion.";
    throw cms::Exception(exception_message); 
  }
}

double CombinedTauTagAlg::ECALcellsEtSum_around_rectk(const IsolatedTauTagInfoRef& theIsolatedTauTagInfoRef,Event& theEvent,const EventSetup& theEventSetup,TrackRef therecTrack){ 
  if((double)(*therecTrack).momentum().r()!=0.){
    ESHandle<CaloGeometry> theCaloGeometry;
    theEventSetup.get<IdealGeometryRecord>().get(theCaloGeometry);
    const CaloSubdetectorGeometry* theCaloSubdetectorGeometry;
    
    Hep3Vector theECALCaloRecHitsSum_Hep3V(0.,0.,0.);
    Hep3Vector thepropagatedrectk_Hep3V(0.,0.,0.);
    math::XYZPoint therecTrackImpactPositiononECAL=recTrackImpactPositiononECAL(theEvent,theEventSetup,therecTrack);
    if(therecTrackImpactPositiononECAL.z()!=0.){
      thepropagatedrectk_Hep3V.setRThetaPhi((*therecTrack).momentum().r(),(double)therecTrackImpactPositiononECAL.theta(),(double)therecTrackImpactPositiononECAL.phi());
      vector<CaloTowerRef> theCaloTowers=theIsolatedTauTagInfoRef->jtaRef()->first.castTo<CaloJetRef>()->getConstituents();
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
	      if(theECALCaloRecHit_Hep3V.deltaR(thepropagatedrectk_Hep3V)<calotk_conesize_) theECALCaloRecHitsSum_Hep3V+=theECALCaloRecHit_Hep3V;
            }
            else if(EcalNum==2){
	      theCaloSubdetectorGeometry = theCaloGeometry->getSubdetectorGeometry(DetId::Ecal,EcalEndcap);
	      EEDetId EcalID = RecHitDetID;
              EERecHitCollection::const_iterator theRecHit=EERecHits->find(EcalID);	    
	      const CaloCellGeometry* theRecHitCell=theCaloSubdetectorGeometry->getGeometry(RecHitDetID);
	      Hep3Vector theECALCaloRecHit_Hep3V(0.,0.,0.);
	      theECALCaloRecHit_Hep3V.setREtaPhi((double)theRecHit->energy(),(double)theRecHitCell->getPosition().eta(),(double)theRecHitCell->getPosition().phi());
	      if(theECALCaloRecHit_Hep3V.deltaR(thepropagatedrectk_Hep3V)<calotk_conesize_) theECALCaloRecHitsSum_Hep3V+=theECALCaloRecHit_Hep3V;
	    }
          }
	}
      }
      return(theECALCaloRecHitsSum_Hep3V.perp());
    }else{
      string exception_message="In CombinedTauTagAlg::ECALcellsEtSum_around_rectk(.) - could not find the lead. tk contact on ECAL surface point.";
      throw cms::Exception(exception_message);      
    };
  }else{
    string exception_message="In CombinedTauTagAlg::ECALcellsEtSum_around_rectk(.) - the lead. tk has 0GeV impulsion.";
    throw cms::Exception(exception_message); 
  }
}
