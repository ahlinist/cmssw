#include "RecoTauTag/CombinedTauTag/interface/CombinedTauTagAlg.h"

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
  the_recjet_G3DV=0;    
  the_recjet_alternatHepLV.setPx(0.);
  the_recjet_alternatHepLV.setPy(0.);
  the_recjet_alternatHepLV.setPz(0.);
  the_recjet_alternatHepLV.setE(0.);
  TauJet_ref_et=0.;
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
  the_neutralECALclus_radius=-100.;
  ECALclus_withmass0_HepLV.clear();
  ECALclus.clear();
  gammacand_fromneutralECALclus_HepLV.clear();
  gammacand_neutralECALclus.clear();
  the_neutralE_ratio=-100.;
  the_tksEt_o_jetEt=0.;
  the_neutrE_o_tksEneutrE=0.;
  the_isolneutrE_o_tksEneutrE=-100.;
  the_leadtk_signedipt_significance=-100.;
  the_leadtk_signedip3D_significance=-100.;
  the_signedflightpath_significance=-100.;
  ECALEt_o_leadtkPt=-100.;
  HCALEt_o_leadtkPt=-100.;
  couldnotproduce_SV=false;
  ESHandle<TransientTrackBuilder> builder;
  theEventSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",builder);
  theTransientTrackBuilder=builder.product();
}

pair<JetTag,CombinedTauTagInfo> CombinedTauTagAlg::tag(const IsolatedTauTagInfoRef& theIsolatedTauTagInfoRef,const Vertex& iPV,Event& theEvent,const EventSetup& theEventSetup) {
  // ---
  init(theEventSetup);
  // ---
  XYZVector recjet_XYZVector(theIsolatedTauTagInfoRef->jet().px(),theIsolatedTauTagInfoRef->jet().py(),theIsolatedTauTagInfoRef->jet().pz());
  HepLorentzVector recjet_HepLV(theIsolatedTauTagInfoRef->jet().px(),theIsolatedTauTagInfoRef->jet().py(),theIsolatedTauTagInfoRef->jet().pz(),theIsolatedTauTagInfoRef->jet().energy());
  Global3DVector recjet_G3DV(theIsolatedTauTagInfoRef->jet().px(),theIsolatedTauTagInfoRef->jet().py(),theIsolatedTauTagInfoRef->jet().pz());
  the_recjet_G3DV=&recjet_G3DV;
  
  XYZVector refAxis_XYZVector=recjet_XYZVector;
  HepLorentzVector refAxis_HepLV=recjet_HepLV;
  
  double the_neutralECALclus_esum=0.;
  double the_neutralECALclus_isolband_esum=0.;
  double filtered_chargedpicand_fromtk_esum=0.;
  CombinedTauTagInfo resultExtended;
  
  // *************rec. tracks info filling ***************
  // *********************begin***************************
  const TrackRef theLeadTrack=theIsolatedTauTagInfoRef->leadingSignalTrack(recjet_XYZVector,MatchingCone_,MinimumTransverseMomentumLeadingTrack_);
  if (!theLeadTrack.isNull()){
    passed_leadtk_selection=true;
    XYZVector leadingtk_XYZVector=theLeadTrack->momentum();
    HepLorentzVector leadingchargedpicand_fromtk_HepLV((double)theLeadTrack->momentum().x(),(double)theLeadTrack->momentum().y(),(double)theLeadTrack->momentum().z(),sqrt(pow((double)theLeadTrack->momentum().r(),2)+pow(chargedpi_mass,2)));
    refAxis_XYZVector=leadingtk_XYZVector;
    refAxis_HepLV=leadingchargedpicand_fromtk_HepLV;
  }
  RefVector<TrackCollection> therecTracks=theIsolatedTauTagInfoRef->selectedTracks() ;
  for(RefVector<TrackCollection>::const_iterator i_recTrack=therecTracks.begin();i_recTrack!=therecTracks.end();i_recTrack++){
    HepLorentzVector thechargedpicand_fromtk_HepLV((double)(**i_recTrack).momentum().x(),(double)(**i_recTrack).momentum().y(),(double)(**i_recTrack).momentum().z(),sqrt(pow((double)(**i_recTrack).momentum().r(),2)+pow(chargedpi_mass,2)));
    if (thechargedpicand_fromtk_HepLV.deltaR(refAxis_HepLV)>TracksLeadingTrackCone_)continue;
    chargedpicand_fromtk_HepLV.push_back(thechargedpicand_fromtk_HepLV);
    HepLorentzVector thechargedpicand_frompropagtk_HepLV(0.,0.,0.,0.);
    if ((double)(**i_recTrack).momentum().r()!=0.){
      XYZPoint therecTrackImpactPositiononECAL=recTrackImpactPositiononECAL(theEvent,theEventSetup,*i_recTrack);
      if (therecTrackImpactPositiononECAL.z()!=0.){
	Hep3Vector thechargedpicand_frompropagtk_Hep3V(0.,0.,0.);
	thechargedpicand_frompropagtk_Hep3V.setRThetaPhi((**i_recTrack).momentum().r(),(double)therecTrackImpactPositiononECAL.theta(),(double)therecTrackImpactPositiononECAL.phi());
	thechargedpicand_frompropagtk_HepLV.setVectM(thechargedpicand_frompropagtk_Hep3V,chargedpi_mass);
      }
    }
    chargedpicand_frompropagtk_HepLV.push_back(thechargedpicand_frompropagtk_HepLV);  
    if (thechargedpicand_fromtk_HepLV.deltaR(refAxis_HepLV)>IsolationCone_) continue;  
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
    if (theBC_HepLV.deltaR(refAxis_HepLV)>ECALclusleadtk_conesize_) continue;
    if ((*i_BC).energy()<ECALclus_min_e_) continue;
    ECALclus.push_back(&(*i_BC));
    ECALclus_withmass0_HepLV.push_back(theBC_HepLV);
  }
  Handle<BasicClusterCollection> theEndcapBCCollection;
  theEvent.getByLabel("islandBasicClusters","islandEndcapBasicClusters",theEndcapBCCollection);
  for(BasicClusterCollection::const_iterator i_BC=theEndcapBCCollection->begin();i_BC!=theEndcapBCCollection->end();i_BC++) {
    Hep3Vector theBC_Hep3V(0.,0.,0.);
    theBC_Hep3V.setREtaPhi((*i_BC).energy(),(*i_BC).eta(),(*i_BC).phi());
    HepLorentzVector theBC_HepLV(theBC_Hep3V,(*i_BC).energy());      	    
    if (theBC_HepLV.deltaR(refAxis_HepLV)>ECALclusleadtk_conesize_) continue;
    if ((*i_BC).energy()<ECALclus_min_e_) continue;
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
  TauJet_ref_et=recjet_HepLV.et();               // Et,recjet
  //TauJet_ref_et=the_recjet_alternatHepLV.et();      // Et,neutr.recclus+rectks
  
  // ***********defining evolutive size signal cones ************
  // *************************BEGIN******************************
  double neutralECALclus_signalconesize = min(0.15,3./TauJet_ref_et+0.05);
  neutralECALclus_signalconesize = max((double)neutralECALclus_signalconesize,0.08);
  double rectks_signalconesize = SignalCone_ifnotEvolutive_;
  if (EvolutiveSignalCone_){
    rectks_signalconesize = min(0.17,3.5/TauJet_ref_et);
    rectks_signalconesize = max((double)rectks_signalconesize,0.05);
  }
  // **************************END*******************************

  // *************distinguishing between rec. tracks inside signal cone AND inside isolation ring***************
  // ***********************************************begin*******************************************************
  double trackerisolation_discriminator = 0;
  trackerisolation_discriminator=theIsolatedTauTagInfoRef->discriminator(recjet_XYZVector,MatchingCone_,rectks_signalconesize,IsolationCone_,MinimumTransverseMomentumLeadingTrack_,MinimumTransverseMomentum_,MaximumNumberOfTracksIsolationRing_); 
  if (trackerisolation_discriminator==1) passed_tracker_isolationring_selection=true;
  RefVector<TrackCollection> signalrecTracks;
  signalrecTracks=theIsolatedTauTagInfoRef->tracksInCone(refAxis_XYZVector,rectks_signalconesize,MinimumTransverseMomentum_);
  int chargedpicand_fromtk_qsum=0;
  if ((int)(signalrecTracks.size())!=0){
    for(RefVector<TrackCollection>::const_iterator i_recTrack=signalrecTracks.begin();i_recTrack!=signalrecTracks.end();i_recTrack++){
      chargedpicand_fromtk_qsum+=(**i_recTrack).charge();
      HepLorentzVector thechargedpicand_fromtk_HepLV((**i_recTrack).momentum().x(),(**i_recTrack).momentum().y(),(**i_recTrack).momentum().z(),sqrt(pow((**i_recTrack).momentum().r(),2)+pow(chargedpi_mass,2)));
      signalchargedpicand_fromtk_HepLV.push_back(thechargedpicand_fromtk_HepLV);
    }
  }
  //if ((((int)(signalrecTracks.size())==1 && refAxis_XYZVector.Rho()>MinimumTransverseMomentumLeadingTrack_case1signalTrack_) || ((int)(signalrecTracks.size())==3 && refAxis_XYZVector.Rho()>MinimumTransverseMomentumLeadingTrack_case3signalTracks_)) && abs(chargedpicand_fromtk_qsum)==1) passed_tracker_signalcone_selection=true; // events from mc-physval-111-DiTaus-Pt20To420 sample analyzed using CMSSW_1_2_0 : Track.charge()=+1;
  if (((int)(signalrecTracks.size())==1 && refAxis_XYZVector.Rho()>MinimumTransverseMomentumLeadingTrack_case1signalTrack_) || ((int)(signalrecTracks.size())==3 && refAxis_XYZVector.Rho()>MinimumTransverseMomentumLeadingTrack_case3signalTracks_)) passed_tracker_signalcone_selection=true;

    // *** tracker selection
  if (passed_leadtk_selection && passed_tracker_isolationring_selection && passed_tracker_signalcone_selection) passed_tracker_selection=true;
  // *********************************************end********************************************************
 
  // ****************************** ECAL neutral clus. treatment******************************
  // ***************************************** begin ****************************************
  if ((int)gammacand_fromneutralECALclus_HepLV.size()!=0){
    the_neutralECALclus_radius=0.;
    for (vector<HepLorentzVector>::const_iterator igammacand_fromneutralECALclus_HepLV=gammacand_fromneutralECALclus_HepLV.begin();igammacand_fromneutralECALclus_HepLV!=gammacand_fromneutralECALclus_HepLV.end();igammacand_fromneutralECALclus_HepLV++) {
      ++the_neutralECALclus_number;
      the_neutralECALclus_radius+=(*igammacand_fromneutralECALclus_HepLV).deltaR(refAxis_HepLV)*(*igammacand_fromneutralECALclus_HepLV).e();
      the_neutralECALclus_esum+=(*igammacand_fromneutralECALclus_HepLV).e();
      if ((*igammacand_fromneutralECALclus_HepLV).deltaR(refAxis_HepLV)>neutralECALclus_signalconesize) the_neutralECALclus_isolband_esum+=(*igammacand_fromneutralECALclus_HepLV).e();
    }  
    if (the_neutralECALclus_esum!=0.) the_neutralECALclus_radius=the_neutralECALclus_radius/the_neutralECALclus_esum;
  }
  // *** ECAL neutral clus. selection
  if (the_neutralECALclus_esum==0.) passed_neutrale_selection=true;
  // *****************************end of ECAL neutral clus. treatment*******************************
  if (passed_tracker_selection && passed_neutrale_selection) is_GoodTauCand=true;
  if (passed_tracker_selection && !passed_neutrale_selection) needs_LikelihoodRatio_discrimination=true;
  
  // ******** filling the variables which may enter into the definition of a combined variable for discriminating tau jet from QCD jet *********
  if (the_neutralECALclus_esum!=0.) the_neutralE_ratio=the_neutralECALclus_isolband_esum/the_neutralECALclus_esum;
  the_neutrE_o_tksEneutrE=the_neutralECALclus_esum/(the_neutralECALclus_esum+filtered_chargedpicand_fromtk_esum);
  the_isolneutrE_o_tksEneutrE=the_neutralECALclus_isolband_esum/(the_neutralECALclus_esum+filtered_chargedpicand_fromtk_esum);
  the_tksEt_o_jetEt=filtered_chargedpicand_fromtk_HepLVsum.et()/recjet_HepLV.et();
  if (!theLeadTrack.isNull()){
    the_leadtk_signedipt_significance=rectk_signedipt_significance(iPV,theLeadTrack);
    the_leadtk_signedip3D_significance=rectk_signedip3D_significance(iPV,theLeadTrack);
  }
  the_signedflightpath_significance=signedflightpath_significance(iPV);
  if (the_signedflightpath_significance==-100.) couldnotproduce_SV=true;
  // ***** end filling the variables which may enter into the definition of a combined variable for discriminating tau jet from QCD jet ******
  
  // ******* 1 pi-prong/e/mu/discrimination *******  by Mangeol Dominique J., 05, slightly modified
  // ***************** begin **********************
  if (!theLeadTrack.isNull()){
    ECALEt_o_leadtkPt=ECALcellsEtSum_around_rectk(theIsolatedTauTagInfoRef,theEvent,theEventSetup,theLeadTrack)/theLeadTrack->momentum().Rho();
    HCALEt_o_leadtkPt=HCALtowersEtSum_around_rectk(theIsolatedTauTagInfoRef,theEvent,theEventSetup,theLeadTrack)/theLeadTrack->momentum().Rho();
    if (passed_tracker_selection && (int)(signalrecTracks.size())==1 && ECALEt_o_leadtkPt!=-100. && HCALEt_o_leadtkPt!=-100.){    
      passed_cutmuon=true;
      passed_cutelectron=true;
      if ((ECALEt_o_leadtkPt>electron_selection_1stmin_ECALEt_o_leadtkPt_ && ECALEt_o_leadtkPt<electron_selection_1stmax_ECALEt_o_leadtkPt_ && HCALEt_o_leadtkPt<electron_selection_1stmax_HCALEt_o_leadtkPt_)
	  || (ECALEt_o_leadtkPt>electron_selection_2ndmin_ECALEt_o_leadtkPt_ && ECALEt_o_leadtkPt<electron_selection_2ndmax_ECALEt_o_leadtkPt_ && HCALEt_o_leadtkPt<electron_selection_2ndmax_HCALEt_o_leadtkPt_)) passed_cutelectron=false;
      if (ECALEt_o_leadtkPt<muon_selection_max_ECALEt_o_leadtkPt_ && HCALEt_o_leadtkPt<muon_selection_max_HCALEt_o_leadtkPt_) passed_cutmuon=false;
    }else{
      passed_cutelectron=true;
      passed_cutmuon=true;
    }
  }else{
    passed_cutelectron=true;
    passed_cutmuon=true;
  }
  if (!passed_cutelectron) {
    infact_GoodElectronCand=true;
  }
  if (!passed_cutmuon){
    infact_GoodMuonCand=true;
  }
  // ******* end 1 pi-prong/e/mu/discrimination *******
  resultExtended.setisolatedtautaginfoRef(theIsolatedTauTagInfoRef);
  resultExtended.setpassed_trackerselection(passed_tracker_selection);
  resultExtended.setis_GoodTauCandidate(is_GoodTauCand);
  resultExtended.setneeds_LikelihoodRatio_discrimination(needs_LikelihoodRatio_discrimination);
  resultExtended.setsignalTks(signalrecTracks);
  resultExtended.setselectedTks(filtered_chargedpicand_tk);
  resultExtended.setleadTk_signedipt_significance(the_leadtk_signedipt_significance);
  resultExtended.setleadTk_signedip3D_significance(the_leadtk_signedip3D_significance);
  resultExtended.setsignedflightpath_significance(the_signedflightpath_significance);
  resultExtended.setTksEt_o_JetEt(the_tksEt_o_jetEt);
  resultExtended.setneutralE(the_neutralECALclus_esum);
  resultExtended.setisolneutralE(the_neutralECALclus_isolband_esum);
  resultExtended.setneutralECALClus_number(the_neutralECALclus_number);
  resultExtended.setneutralECALClus_radius(the_neutralECALclus_radius);
  resultExtended.setneutralE_o_TksEneutralE(the_neutrE_o_tksEneutrE);
  resultExtended.setisolneutralE_o_TksEneutralE(the_isolneutrE_o_tksEneutrE);
  resultExtended.setneutralE_ratio(the_neutralE_ratio);
  resultExtended.setalternatrecJet_HepLV(the_recjet_alternatHepLV);
  resultExtended.setinfact_GoodElectronCandidate(infact_GoodElectronCand);
  resultExtended.setinfact_GoodMuonCandidate(infact_GoodMuonCand);
  resultExtended.setECALEt_o_leadTkPt(ECALEt_o_leadtkPt);
  resultExtended.setHCALEt_o_leadTkPt(HCALEt_o_leadtkPt);
  // *** overall tau selection ***
  // **********begin**************
  const JetTracksAssociationRef& theJetTracksAssociationRef=theIsolatedTauTagInfoRef->jetRef()->jtaRef();
  if (!passed_tracker_selection){
    JetTag resultBase(0.,theJetTracksAssociationRef);
    return pair<JetTag,CombinedTauTagInfo> (resultBase,resultExtended);
  }else{
    if (passed_neutrale_selection) {
      JetTag resultBase(1.,theJetTracksAssociationRef);
      return pair<JetTag,CombinedTauTagInfo> (resultBase,resultExtended); 
    }else{
      JetTag resultBase(LikelihoodRatiovalue(),theJetTracksAssociationRef);
      return pair<JetTag,CombinedTauTagInfo> (resultBase,resultExtended); 
    }
  }
  // * end of overall tau selection *
}

XYZPoint CombinedTauTagAlg::recTrackImpactPositiononECAL(Event& theEvent,const EventSetup& theEventSetup,TrackRef therecTrack){ 
  ESHandle<MagneticField> theMF;
  theEventSetup.get<IdealMagneticFieldRecord>().get(theMF);
  const MagneticField* theMagField=theMF.product();
  AnalyticalPropagator thefwdPropagator(theMagField,alongMomentum);
  XYZPoint proprecTrack_XYZPoint(0.,0.,0.);
  
  // get the initial rec. tk FreeTrajectoryState - at outermost point position if possible, else at innermost point position:
  Global3DVector therecTrack_initialG3DV(0.,0.,0.);
  Global3DPoint therecTrack_initialG3DP(0.,0.,0.);
  if (therecTrack->outerOk()){
    Global3DVector therecTrack_initialoutermostG3DV(therecTrack->outerMomentum().x(),therecTrack->outerMomentum().y(),therecTrack->outerMomentum().z());
    Global3DPoint therecTrack_initialoutermostG3DP(therecTrack->outerPosition().x(),therecTrack->outerPosition().y(),therecTrack->outerPosition().z());
    therecTrack_initialG3DV=therecTrack_initialoutermostG3DV;
    therecTrack_initialG3DP=therecTrack_initialoutermostG3DP;
  } else if (therecTrack->innerOk()){
    Global3DVector therecTrack_initialinnermostG3DV(therecTrack->innerMomentum().x(),therecTrack->innerMomentum().y(),therecTrack->innerMomentum().z());
    Global3DPoint therecTrack_initialinnermostG3DP(therecTrack->innerPosition().x(),therecTrack->innerPosition().y(),therecTrack->innerPosition().z());
    therecTrack_initialG3DV=therecTrack_initialinnermostG3DV;
    therecTrack_initialG3DP=therecTrack_initialinnermostG3DP;
  } else return (proprecTrack_XYZPoint);
  GlobalTrajectoryParameters therecTrack_initialGTPs(therecTrack_initialG3DP,therecTrack_initialG3DV,therecTrack->charge(),&*theMF);
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
  if (!recTrack_propagatedonECAL_TSOS.isValid() || fabs(recTrack_propagatedonECAL_TSOS.globalParameters().position().perp()/recTrack_propagatedonECAL_TSOS.globalParameters().position().z())<ECALcorner_tantheta) {
    if (recTrack_propagatedonECAL_TSOS.isValid() && fabs(recTrack_propagatedonECAL_TSOS.globalParameters().position().perp()/recTrack_propagatedonECAL_TSOS.globalParameters().position().z())<ECALcorner_tantheta){     
      if (recTrack_propagatedonECAL_TSOS.globalParameters().position().eta()>0.){
	recTrack_propagatedonECAL_TSOS=thefwdPropagator.propagate((recTrack_initialFTS),ECALBounds::positiveEndcapDisk());
      }else{ 
	recTrack_propagatedonECAL_TSOS=thefwdPropagator.propagate((recTrack_initialFTS),ECALBounds::negativeEndcapDisk());
      }
    }
    if (!recTrack_propagatedonECAL_TSOS.isValid()){
      if ((recTrack_initialFTS).position().eta()>0.){
	recTrack_propagatedonECAL_TSOS=thefwdPropagator.propagate((recTrack_initialFTS),ECALBounds::positiveEndcapDisk());
      }else{ 
	recTrack_propagatedonECAL_TSOS=thefwdPropagator.propagate((recTrack_initialFTS),ECALBounds::negativeEndcapDisk());
      }
    }
  }
  if (recTrack_propagatedonECAL_TSOS.isValid()){
    XYZPoint validproprecTrack_XYZPoint(recTrack_propagatedonECAL_TSOS.globalPosition().x(),
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
    if ((*ichargedpicand_frompropagtk_HepLV).rho()==0.) {
      chargedpicand_fromECALclus_HepLV.push_back(the_chargedpicand_fromECALclus_HepLV);
      chargedpicand_ECALclus.push_back(0);
      continue;
    }
    double matchingEgammaBasicClustertrack_deltaR=matchingECALclustrack_deltaR_;
    bool track_matchedwithcluster=false;
    vector<const BasicCluster*>::iterator jECALclus=bisECALclus.begin();
    for (vector<HepLorentzVector>::iterator jECALclus_withmass0_HepLV=bisECALclus_withmass0_HepLV.begin();jECALclus_withmass0_HepLV!=bisECALclus_withmass0_HepLV.end();jECALclus_withmass0_HepLV++) {
      if ((*ichargedpicand_frompropagtk_HepLV).deltaR(*jECALclus_withmass0_HepLV)<matchingEgammaBasicClustertrack_deltaR){
      	track_matchedwithcluster=true;
	matchingEgammaBasicClustertrack_deltaR=(*ichargedpicand_frompropagtk_HepLV).deltaR(*jECALclus_withmass0_HepLV);
	kmatchedECALclus_withmass0_HepLV=jECALclus_withmass0_HepLV;
	kmatchedECALclus=jECALclus;
      }
      ++jECALclus;
    }
    if (track_matchedwithcluster) {
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

double CombinedTauTagAlg::LikelihoodRatiovalue(){
  // returns 0.        if candidate did not pass tracker selection,
  //         1.        if candidate passed tracker selection and did not contain neutral obj.,
  //         0<=  <=1  if candidate passed tracker selection, contained neutral obj. and goes through the likelihood ratio mechanism, 
  //         -0.1      the values of the likelihood functions PDFs are 0;  
  // truth matched Tau candidate PDFs obtained with evts from ORCA reco. bt04_double_tau_had sample without PU,
  // fake Tau candidate PDFs obtained with evts from ORCA reco. jm03b_qcd30_50, jm03b_qcd50_80, jm03b_qcd80_120 and jm03b_qcd120_170 samples, all without PU.
  
  if (!passed_tracker_selection) return (0.);
  if (is_GoodTauCand) return (1.);
  FileInPath inputROOTFileInPath(ROOTfilename_gettingPDFs_);   
  TFile inputROOTfile(inputROOTFileInPath.fullPath().c_str());  
  string truthmatchedcand_var_TDirectoryname=Get_candvar_TDirectoryname(1);
  TDirectory *truthmatchedcand_var_TDirectory=(TDirectory*)inputROOTfile.Get(truthmatchedcand_var_TDirectoryname.c_str());
  string fakecand_var_TDirectoryname=Get_candvar_TDirectoryname(0);
  TDirectory *fakecand_var_TDirectory=(TDirectory*)inputROOTfile.Get(fakecand_var_TDirectoryname.c_str());
  double the_likelihoodratio_value=-0.1;
  double truthmatched_likelihoodfunction_value=1.;
  double fake_likelihoodfunction_value=1.;
  vector<double> var_vector;
  vector<string> varname_vector;
  vector<double> vartruthmatchedpdf_vector;
  vector<double> varfakepdf_vector;
  var_vector.clear();
  varname_vector.clear();
  vartruthmatchedpdf_vector.clear();
  varfakepdf_vector.clear();
  if (signalchargedpicand_fromtk_HepLV.size()==1){
    if (use_neutralECALclus_number_case1signaltk_) {
      var_vector.push_back((double)the_neutralECALclus_number);
      varname_vector.push_back("neutrECALclus_number");
      vartruthmatchedpdf_vector.push_back(0.);
      varfakepdf_vector.push_back(0.);
    }	
    if (use_neutralECALclus_radius_case1signaltk_){
      var_vector.push_back(the_neutralECALclus_radius);
      varname_vector.push_back("neutrECALclus_radius");
      vartruthmatchedpdf_vector.push_back(0.);
      varfakepdf_vector.push_back(0.);
    }
    if (use_neutralE_ratio_case1signaltk_){
      var_vector.push_back(the_neutralE_ratio);
      varname_vector.push_back("neutrE_ratio");
      vartruthmatchedpdf_vector.push_back(0.);
      varfakepdf_vector.push_back(0.);
    }
    if (use_isolneutrE_o_tkEneutrE_case1signaltk_){
      var_vector.push_back(the_isolneutrE_o_tksEneutrE);
      varname_vector.push_back("isolneutrE_neutrEtkE_ratio");
      vartruthmatchedpdf_vector.push_back(0.);
      varfakepdf_vector.push_back(0.);
    }
    if (use_tkEt_o_jetEt_case1signaltk_){
      var_vector.push_back(the_tksEt_o_jetEt);
      varname_vector.push_back("tkEt_jetEt_ratio");
      vartruthmatchedpdf_vector.push_back(0.);
      varfakepdf_vector.push_back(0.);
    }
    if (use_leadtk_ipt_significance_case1signaltk_ && the_leadtk_signedipt_significance!=-100.){
      var_vector.push_back(fabs(the_leadtk_signedipt_significance));   
      varname_vector.push_back("leadtk_ipt_significance");
      vartruthmatchedpdf_vector.push_back(0.);
      varfakepdf_vector.push_back(0.);
    }
  }
  if (signalchargedpicand_fromtk_HepLV.size()==3){
    if (use_neutralECALclus_number_case3signaltks_){
      var_vector.push_back((double)the_neutralECALclus_number);	
      varname_vector.push_back("neutrECALclus_number");
      vartruthmatchedpdf_vector.push_back(0.);
      varfakepdf_vector.push_back(0.);
    }
    if (use_neutralECALclus_radius_case3signaltks_){
      var_vector.push_back(the_neutralECALclus_radius);
      varname_vector.push_back("neutrECALclus_radius");
      vartruthmatchedpdf_vector.push_back(0.);
      varfakepdf_vector.push_back(0.);
    }
    if (use_neutralE_ratio_case3signaltks_){
      var_vector.push_back(the_neutralE_ratio);
      varname_vector.push_back("neutrE_ratio");
      vartruthmatchedpdf_vector.push_back(0.);
      varfakepdf_vector.push_back(0.);
    }
    if (use_isolneutrE_o_tksEneutrE_case3signaltks_){
      var_vector.push_back(the_isolneutrE_o_tksEneutrE);
      varname_vector.push_back("isolneutrE_neutrEtksE_ratio");
      vartruthmatchedpdf_vector.push_back(0.);
      varfakepdf_vector.push_back(0.);
    }
    if (use_tksEt_o_jetEt_case3signaltks_){
      var_vector.push_back(the_tksEt_o_jetEt);
      varname_vector.push_back("tksEt_jetEt_ratio");
      vartruthmatchedpdf_vector.push_back(0.);
      varfakepdf_vector.push_back(0.);
    }
    if (use_signedflightpath_significance_case3signaltks_ && !couldnotproduce_SV){
      var_vector.push_back(the_signedflightpath_significance);
      varname_vector.push_back("signedflightpath_significance");
      vartruthmatchedpdf_vector.push_back(0.);
      varfakepdf_vector.push_back(0.);
    }
  }
  // PDFs are obtained by variable-width slice of recjet Et, the slice width is little(big) when # tau candidates contained in slice is high(low), the center value of the recjet Et slice approximates the TauCandidate recjet Et value;  
  int minnumbercandidates_inpdf_byrecjetEtslice=0;
  if (signalchargedpicand_fromtk_HepLV.size()==1) minnumbercandidates_inpdf_byrecjetEtslice=minnumbercandidates_inpdf_byrecjetEtslice_case1signaltk_;
  if (signalchargedpicand_fromtk_HepLV.size()==3) minnumbercandidates_inpdf_byrecjetEtslice=minnumbercandidates_inpdf_byrecjetEtslice_case3signaltks_;
  bool stop_increasing_slicewidth=false;
  string truthmatched_recjetEt_TH1name=Get_recjetEt_TH1name(1);
  TH1F *truthmatched_recjetEt_TH1=(TH1F*)truthmatchedcand_var_TDirectory->Get(truthmatched_recjetEt_TH1name.c_str());
  int truthmatched_recjetEt_binnumber=(int)((TauJet_ref_et-(double)truthmatched_recjetEt_TH1->GetXaxis()->GetXmin())/(double)truthmatched_recjetEt_TH1->GetXaxis()->GetBinWidth(1));
  if (TauJet_ref_et>=200.) truthmatched_recjetEt_binnumber=(int)truthmatched_recjetEt_TH1->GetXaxis()->GetNbins()-1;
  double truthmatched_candidatesnumber_inrecjetEtslice=(double)truthmatched_recjetEt_TH1->GetBinContent(truthmatched_recjetEt_binnumber+1);
  string fake_recjetEt_TH1name=Get_recjetEt_TH1name(0);
  TH1F *fake_recjetEt_TH1=(TH1F*)fakecand_var_TDirectory->Get(fake_recjetEt_TH1name.c_str());
  int fake_recjetEt_binnumber=(int)((TauJet_ref_et-(double)fake_recjetEt_TH1->GetXaxis()->GetXmin())/(double)fake_recjetEt_TH1->GetXaxis()->GetBinWidth(1));
  if (TauJet_ref_et>=200.) fake_recjetEt_binnumber=(int)fake_recjetEt_TH1->GetXaxis()->GetNbins()-1;
  double fake_candidatesnumber_inrecjetEtslice=(double)fake_recjetEt_TH1->GetBinContent(fake_recjetEt_binnumber+1);
  int recjetEt_TH1_binsnumber=(int)truthmatched_recjetEt_TH1->GetXaxis()->GetNbins();
  
  for (int ivar=0;ivar<(int)var_vector.size();ivar++){
    double truthmatched_ivarvalue=var_vector[ivar];	   
    double truthmatched_ivarvalue_entriesnumber=0.;
    string truthmatched_ivar_vs_recjetEt_TH2name=Get_ivar_vs_recjetEt_TH2name(1,varname_vector[ivar]);
    TH2F *truthmatchedcand_var_vs_recjetEt_TH2=(TH2F*)truthmatchedcand_var_TDirectory->Get(truthmatched_ivar_vs_recjetEt_TH2name.c_str());
    if (truthmatched_ivarvalue>=(double)truthmatchedcand_var_vs_recjetEt_TH2->GetYaxis()->GetXmin() && truthmatched_ivarvalue<(double)truthmatchedcand_var_vs_recjetEt_TH2->GetYaxis()->GetXmax()){
      int truthmatched_ivar_binnumber=(int)((truthmatched_ivarvalue-(double)truthmatchedcand_var_vs_recjetEt_TH2->GetYaxis()->GetXmin())/(double)truthmatchedcand_var_vs_recjetEt_TH2->GetYaxis()->GetBinWidth(1));
      truthmatched_ivarvalue_entriesnumber=(double)truthmatchedcand_var_vs_recjetEt_TH2->GetBinContent(truthmatched_recjetEt_binnumber+1,truthmatched_ivar_binnumber+1);
    }
    if (truthmatched_ivarvalue<(double)truthmatchedcand_var_vs_recjetEt_TH2->GetYaxis()->GetXmin()) {
      truthmatched_ivarvalue_entriesnumber=(double)truthmatchedcand_var_vs_recjetEt_TH2->GetBinContent(truthmatched_recjetEt_binnumber+1,1);
    }
    if (truthmatched_ivarvalue>=(double)truthmatchedcand_var_vs_recjetEt_TH2->GetYaxis()->GetXmax()){
      truthmatched_ivarvalue_entriesnumber=(double)truthmatchedcand_var_vs_recjetEt_TH2->GetBinContent(truthmatched_recjetEt_binnumber+1,(int)truthmatchedcand_var_vs_recjetEt_TH2->GetNbinsY());
    }
    vartruthmatchedpdf_vector[ivar]+=truthmatched_ivarvalue_entriesnumber;
    double fake_ivarvalue=var_vector[ivar];
    double fake_ivarvalue_entriesnumber=0.;
    string fake_ivar_vs_recjetEt_TH2name=Get_ivar_vs_recjetEt_TH2name(0,varname_vector[ivar]);
    TH2F *fakecand_var_vs_recjetEt_TH2=(TH2F*)fakecand_var_TDirectory->Get(fake_ivar_vs_recjetEt_TH2name.c_str());
    if (fake_ivarvalue>=(double)fakecand_var_vs_recjetEt_TH2->GetYaxis()->GetXmin() && fake_ivarvalue<(double)fakecand_var_vs_recjetEt_TH2->GetYaxis()->GetXmax()){
      int fake_ivar_binnumber=(int)((fake_ivarvalue-(double)fakecand_var_vs_recjetEt_TH2->GetYaxis()->GetXmin())/(double)fakecand_var_vs_recjetEt_TH2->GetYaxis()->GetBinWidth(1));
      fake_ivarvalue_entriesnumber=(double)fakecand_var_vs_recjetEt_TH2->GetBinContent(fake_recjetEt_binnumber+1,fake_ivar_binnumber+1);
    }
    if (fake_ivarvalue<(double)fakecand_var_vs_recjetEt_TH2->GetYaxis()->GetXmin()){
      fake_ivarvalue_entriesnumber=(double)fakecand_var_vs_recjetEt_TH2->GetBinContent(fake_recjetEt_binnumber+1,1);
    }
    if (fake_ivarvalue>=(double)fakecand_var_vs_recjetEt_TH2->GetYaxis()->GetXmax()){
      fake_ivarvalue_entriesnumber=(double)fakecand_var_vs_recjetEt_TH2->GetBinContent(fake_recjetEt_binnumber+1,(int)fakecand_var_vs_recjetEt_TH2->GetNbinsX());
    }
    varfakepdf_vector[ivar]+=fake_ivarvalue_entriesnumber;
  }
  if ((int)truthmatched_candidatesnumber_inrecjetEtslice>minnumbercandidates_inpdf_byrecjetEtslice && (int)fake_candidatesnumber_inrecjetEtslice>minnumbercandidates_inpdf_byrecjetEtslice){
    stop_increasing_slicewidth=true;	   
  }
  if (!stop_increasing_slicewidth){
    for (int i=1;i<recjetEt_TH1_binsnumber+1;i++){
      if (truthmatched_recjetEt_binnumber+i<recjetEt_TH1_binsnumber) truthmatched_candidatesnumber_inrecjetEtslice+=(double)truthmatched_recjetEt_TH1->GetBinContent(truthmatched_recjetEt_binnumber+1+i);
      if (truthmatched_recjetEt_binnumber+1-i>0) truthmatched_candidatesnumber_inrecjetEtslice+=(double)truthmatched_recjetEt_TH1->GetBinContent(truthmatched_recjetEt_binnumber+1-i);
      if (fake_recjetEt_binnumber+i<recjetEt_TH1_binsnumber) fake_candidatesnumber_inrecjetEtslice+=(double)fake_recjetEt_TH1->GetBinContent(fake_recjetEt_binnumber+1+i);
      if (fake_recjetEt_binnumber+1-i>0) fake_candidatesnumber_inrecjetEtslice+=(double)fake_recjetEt_TH1->GetBinContent(fake_recjetEt_binnumber+1-i);
      for (int ivar=0;ivar<(int)var_vector.size();ivar++){
	if (truthmatched_recjetEt_binnumber+i<recjetEt_TH1_binsnumber){ 
	  double truthmatched_ivarvalue=var_vector[ivar];
	  double truthmatched_ivarvalue_entriesnumber=0.;
	  string truthmatched_ivar_vs_recjetEt_TH2name=Get_ivar_vs_recjetEt_TH2name(1,varname_vector[ivar]);
	  TH2F *truthmatchedcand_var_vs_recjetEt_TH2=(TH2F*)truthmatchedcand_var_TDirectory->Get(truthmatched_ivar_vs_recjetEt_TH2name.c_str());
	  if (truthmatched_ivarvalue>=(double)truthmatchedcand_var_vs_recjetEt_TH2->GetYaxis()->GetXmin() && truthmatched_ivarvalue<(double)truthmatchedcand_var_vs_recjetEt_TH2->GetYaxis()->GetXmax()){
	    int truthmatched_ivar_binnumber=(int)((truthmatched_ivarvalue-(double)truthmatchedcand_var_vs_recjetEt_TH2->GetYaxis()->GetXmin())/(double)truthmatchedcand_var_vs_recjetEt_TH2->GetYaxis()->GetBinWidth(1));
	    truthmatched_ivarvalue_entriesnumber=(double)truthmatchedcand_var_vs_recjetEt_TH2->GetBinContent(truthmatched_recjetEt_binnumber+i+1,truthmatched_ivar_binnumber+1);
	  }
	  if (truthmatched_ivarvalue<(double)truthmatchedcand_var_vs_recjetEt_TH2->GetYaxis()->GetXmin()) {
	    truthmatched_ivarvalue_entriesnumber=(double)truthmatchedcand_var_vs_recjetEt_TH2->GetBinContent(truthmatched_recjetEt_binnumber+i+1,1);
	  }
	  if (truthmatched_ivarvalue>=(double)truthmatchedcand_var_vs_recjetEt_TH2->GetYaxis()->GetXmax()){
	    truthmatched_ivarvalue_entriesnumber=(double)truthmatchedcand_var_vs_recjetEt_TH2->GetBinContent(truthmatched_recjetEt_binnumber+i+1,(int)truthmatchedcand_var_vs_recjetEt_TH2->GetNbinsY());
	  }
	  vartruthmatchedpdf_vector[ivar]+=truthmatched_ivarvalue_entriesnumber;
	  double fake_ivarvalue=var_vector[ivar];
	  double fake_ivarvalue_entriesnumber=0.;
	  string fake_ivar_vs_recjetEt_TH2name=Get_ivar_vs_recjetEt_TH2name(0,varname_vector[ivar]);
	  TH2F *fakecand_var_vs_recjetEt_TH2=(TH2F*)fakecand_var_TDirectory->Get(fake_ivar_vs_recjetEt_TH2name.c_str());
	  if (fake_ivarvalue>=(double)fakecand_var_vs_recjetEt_TH2->GetYaxis()->GetXmin() && fake_ivarvalue<(double)fakecand_var_vs_recjetEt_TH2->GetYaxis()->GetXmax()){
	    int fake_ivar_binnumber=(int)((fake_ivarvalue-(double)fakecand_var_vs_recjetEt_TH2->GetYaxis()->GetXmin())/(double)fakecand_var_vs_recjetEt_TH2->GetYaxis()->GetBinWidth(1));
	    fake_ivarvalue_entriesnumber=(double)fakecand_var_vs_recjetEt_TH2->GetBinContent(fake_recjetEt_binnumber+i+1,fake_ivar_binnumber+1);
	  }
	  if (fake_ivarvalue<(double)fakecand_var_vs_recjetEt_TH2->GetYaxis()->GetXmin()){
	    fake_ivarvalue_entriesnumber=(double)fakecand_var_vs_recjetEt_TH2->GetBinContent(fake_recjetEt_binnumber+i+1,1);
	  }
	  if (fake_ivarvalue>=(double)fakecand_var_vs_recjetEt_TH2->GetYaxis()->GetXmax()){
	    fake_ivarvalue_entriesnumber=(double)fakecand_var_vs_recjetEt_TH2->GetBinContent(fake_recjetEt_binnumber+i+1,(int)fakecand_var_vs_recjetEt_TH2->GetNbinsX());
	  }
	  varfakepdf_vector[ivar]+=fake_ivarvalue_entriesnumber;
	}
	if (truthmatched_recjetEt_binnumber+1-i>0){ 
	  double truthmatched_ivarvalue=var_vector[ivar];
	  double truthmatched_ivarvalue_entriesnumber=0.;
	  string truthmatched_ivar_vs_recjetEt_TH2name=Get_ivar_vs_recjetEt_TH2name(1,varname_vector[ivar]);
	  TH2F *truthmatchedcand_var_vs_recjetEt_TH2=(TH2F*)truthmatchedcand_var_TDirectory->Get(truthmatched_ivar_vs_recjetEt_TH2name.c_str());
	  if (truthmatched_ivarvalue>=(double)truthmatchedcand_var_vs_recjetEt_TH2->GetYaxis()->GetXmin() && truthmatched_ivarvalue<(double)truthmatchedcand_var_vs_recjetEt_TH2->GetYaxis()->GetXmax()){
	    int truthmatched_ivar_binnumber=(int)((truthmatched_ivarvalue-(double)truthmatchedcand_var_vs_recjetEt_TH2->GetYaxis()->GetXmin())/(double)truthmatchedcand_var_vs_recjetEt_TH2->GetYaxis()->GetBinWidth(1));
	    truthmatched_ivarvalue_entriesnumber=(double)truthmatchedcand_var_vs_recjetEt_TH2->GetBinContent(truthmatched_recjetEt_binnumber+1-i,truthmatched_ivar_binnumber+1);
	  }
	  if (truthmatched_ivarvalue<(double)truthmatchedcand_var_vs_recjetEt_TH2->GetYaxis()->GetXmin()) {
	    truthmatched_ivarvalue_entriesnumber=(double)truthmatchedcand_var_vs_recjetEt_TH2->GetBinContent(truthmatched_recjetEt_binnumber+1-i,1);
	  }
	  if (truthmatched_ivarvalue>=(double)truthmatchedcand_var_vs_recjetEt_TH2->GetYaxis()->GetXmax()){
	    truthmatched_ivarvalue_entriesnumber=(double)truthmatchedcand_var_vs_recjetEt_TH2->GetBinContent(truthmatched_recjetEt_binnumber+1-i,(int)truthmatchedcand_var_vs_recjetEt_TH2->GetNbinsY());
	  }
	  vartruthmatchedpdf_vector[ivar]+=truthmatched_ivarvalue_entriesnumber;
	  double fake_ivarvalue=var_vector[ivar];
	  double fake_ivarvalue_entriesnumber=0.;
	  string fake_ivar_vs_recjetEt_TH2name=Get_ivar_vs_recjetEt_TH2name(0,varname_vector[ivar]);
	  TH2F *fakecand_var_vs_recjetEt_TH2=(TH2F*)fakecand_var_TDirectory->Get(fake_ivar_vs_recjetEt_TH2name.c_str());
	  if (fake_ivarvalue>=(double)fakecand_var_vs_recjetEt_TH2->GetYaxis()->GetXmin() && fake_ivarvalue<(double)fakecand_var_vs_recjetEt_TH2->GetYaxis()->GetXmax()){
	    int fake_ivar_binnumber=(int)((fake_ivarvalue-(double)fakecand_var_vs_recjetEt_TH2->GetYaxis()->GetXmin())/(double)fakecand_var_vs_recjetEt_TH2->GetYaxis()->GetBinWidth(1));
	    fake_ivarvalue_entriesnumber=(double)fakecand_var_vs_recjetEt_TH2->GetBinContent(fake_recjetEt_binnumber+1-i,fake_ivar_binnumber+1);
	  }
	  if (fake_ivarvalue<(double)fakecand_var_vs_recjetEt_TH2->GetYaxis()->GetXmin()){
	    fake_ivarvalue_entriesnumber=(double)fakecand_var_vs_recjetEt_TH2->GetBinContent(fake_recjetEt_binnumber+1-i,1);
	  }
	  if (fake_ivarvalue>=(double)fakecand_var_vs_recjetEt_TH2->GetYaxis()->GetXmax()){
	    fake_ivarvalue_entriesnumber=(double)fakecand_var_vs_recjetEt_TH2->GetBinContent(fake_recjetEt_binnumber+1-i,(int)fakecand_var_vs_recjetEt_TH2->GetNbinsX());
	  }
	  varfakepdf_vector[ivar]+=fake_ivarvalue_entriesnumber;
	}
      }
      if ((int)truthmatched_candidatesnumber_inrecjetEtslice>minnumbercandidates_inpdf_byrecjetEtslice && (int)fake_candidatesnumber_inrecjetEtslice>minnumbercandidates_inpdf_byrecjetEtslice){
	stop_increasing_slicewidth=true;
	break;	   
      }
    }
  }
  for (int ivar=0;ivar<(int)var_vector.size();ivar++){
    if (truthmatched_recjetEt_binnumber!=0) vartruthmatchedpdf_vector[ivar]/=truthmatched_candidatesnumber_inrecjetEtslice;
    if (fake_recjetEt_binnumber!=0) varfakepdf_vector[ivar]/=fake_candidatesnumber_inrecjetEtslice;
    if (vartruthmatchedpdf_vector[ivar]!=0. || varfakepdf_vector[ivar]!=0.){
      truthmatched_likelihoodfunction_value*=vartruthmatchedpdf_vector[ivar];
      fake_likelihoodfunction_value*=varfakepdf_vector[ivar];
    }
  }
  if (truthmatched_likelihoodfunction_value+fake_likelihoodfunction_value!=0.) the_likelihoodratio_value=truthmatched_likelihoodfunction_value/(truthmatched_likelihoodfunction_value+fake_likelihoodfunction_value);
  return (the_likelihoodratio_value); 
}

string CombinedTauTagAlg::Get_candvar_TDirectoryname(int the_1truth0fake_type){
  string thestring_1truth0fake_type;
  if (the_1truth0fake_type==1)  thestring_1truth0fake_type.insert(0,"truthmatched");
  if (the_1truth0fake_type==0) thestring_1truth0fake_type.insert(0,"fake");
  
  string thestring_1signaltk3signaltks_type;
  if ((int)signalchargedpicand_fromtk_HepLV.size()==1) thestring_1signaltk3signaltks_type.insert(0,"1signaltk");
  if ((int)signalchargedpicand_fromtk_HepLV.size()==3) thestring_1signaltk3signaltks_type.insert(0,"3signaltks");
  
  string pdf_TDirectoryname(thestring_1truth0fake_type + "_TauCandidate_" + thestring_1signaltk3signaltks_type + "_dir" );
  return (pdf_TDirectoryname);
}
string  CombinedTauTagAlg::Get_recjetEt_TH1name(int the_1truth0fake_type){
  string thestring_1truth0fake_type;
  if (the_1truth0fake_type==1)  thestring_1truth0fake_type.insert(0,"truthmatched");
  if (the_1truth0fake_type==0) thestring_1truth0fake_type.insert(0,"fake");
  
  string thestring_1signaltk3signaltks_type;
  if ((int)signalchargedpicand_fromtk_HepLV.size()==1) thestring_1signaltk3signaltks_type.insert(0,"1signaltk");
  if ((int)signalchargedpicand_fromtk_HepLV.size()==3) thestring_1signaltk3signaltks_type.insert(0,"3signaltks");
  
  string recjetEt_TH1name(thestring_1truth0fake_type + "_TauCandidate_" + thestring_1signaltk3signaltks_type + "_recjetEt");
  return (recjetEt_TH1name);
}
string  CombinedTauTagAlg::Get_ivar_vs_recjetEt_TH2name(int the_1truth0fake_type,string thestring_Var){
  string thestring_1truth0fake_type;
  if (the_1truth0fake_type==1)  thestring_1truth0fake_type.insert(0,"truthmatched");
  if (the_1truth0fake_type==0) thestring_1truth0fake_type.insert(0,"fake");
  
  string thestring_1signaltk3signaltks_type;
  if ((int)signalchargedpicand_fromtk_HepLV.size()==1) thestring_1signaltk3signaltks_type.insert(0,"1signaltk");
  if ((int)signalchargedpicand_fromtk_HepLV.size()==3) thestring_1signaltk3signaltks_type.insert(0,"3signaltks");
  
  string ivar_vs_recjetEt_TH2name(thestring_1truth0fake_type + "_TauCandidate_" + thestring_1signaltk3signaltks_type + "_" + thestring_Var + "_vs_recjetEt");
  return (ivar_vs_recjetEt_TH2name);
}
double CombinedTauTagAlg::rectk_signedipt_significance(const Vertex& thePV,const TrackRef theTrack){ 
  const TransientTrack* the_transTrack=theTransientTrackBuilder->build(&(*theTrack));
  SignedTransverseImpactParameter the_Track_STIP;
  double the_signedipt_significance=-100.;
  if (the_Track_STIP.apply(*the_transTrack,*the_recjet_G3DV,thePV).first)
    the_signedipt_significance=the_Track_STIP.apply(*the_transTrack,*the_recjet_G3DV,thePV).second.significance();
  delete the_transTrack;
  return(the_signedipt_significance);
}
double CombinedTauTagAlg::rectk_signedip3D_significance(const Vertex& thePV,const TrackRef theTrack){ 
  const TransientTrack* the_transTrack=theTransientTrackBuilder->build(&(*theTrack));
  SignedImpactParameter3D the_Track_STIP;
  double the_signedip3D_significance=-100.;
  if (the_Track_STIP.apply(*the_transTrack,*the_recjet_G3DV,thePV).first)
    the_signedip3D_significance=the_Track_STIP.apply(*the_transTrack,*the_recjet_G3DV,thePV).second.significance();
  delete the_transTrack;
  return(the_signedip3D_significance);
}
double CombinedTauTagAlg::signedflightpath_significance(const Vertex& iPV){ 
  if (filtered_chargedpicand_tk.size()>2){
    vector<TransientTrack> transientTracks;
    transientTracks.clear();
    for(RefVector<TrackCollection>::const_iterator i_recTrack=filtered_chargedpicand_tk.begin();i_recTrack!=filtered_chargedpicand_tk.end();i_recTrack++){
      TransientTrack* theTransientTrack=theTransientTrackBuilder->build(&(**i_recTrack));
      transientTracks.push_back(*theTransientTrack);
    }
    try{
      KalmanVertexFitter kvf;
      TransientVertex tv = kvf.vertex(transientTracks); 
      VertexDistance3D theVertexDistance3D;
      double thesignedflightpath_significance=theVertexDistance3D.signedDistance(iPV,tv,*the_recjet_G3DV).significance();
      return (thesignedflightpath_significance);
    }catch (exception){
      return (-100.);
    }
    return (-100.);
  }else return (-100.);
}

double CombinedTauTagAlg::HCALtowersEtSum_around_rectk(const IsolatedTauTagInfoRef& theIsolatedTauTagInfoRef,Event& theEvent,const EventSetup& theEventSetup,TrackRef therecTrack){   
  if ((double)(*therecTrack).momentum().r()!=0.){
    Hep3Vector thelastselectedHCALCaloRecHit_Hep3V(0.,0.,0.);	   // events from CSA06-106-os-ExoticSoup-0, mc-physval-111-DiTaus-Pt20To420 samples analyzed : redundancy in the list of HCAL Towers inside the jet
    Hep3Vector theHCALCaloRecHitsSum_Hep3V(0.,0.,0.);
    Hep3Vector thepropagatedrectk_Hep3V(0.,0.,0.);
    XYZPoint therecTrackImpactPositiononECAL=recTrackImpactPositiononECAL(theEvent,theEventSetup,therecTrack);
    if (therecTrackImpactPositiononECAL.z()!=0.){
      thepropagatedrectk_Hep3V.setRThetaPhi((*therecTrack).momentum().r(),(double)therecTrackImpactPositiononECAL.theta(),(double)therecTrackImpactPositiononECAL.phi());
      Handle<CaloTowerCollection> caloTowers;      
      theEvent.getByLabel("towerMaker",caloTowers);
      const vector<CaloTowerDetId>&  theCaloTowerDetIds=theIsolatedTauTagInfoRef->jetRef()->jtaRef()->key->getTowerIndices();
      int theCaloTowerDetIds_n= theCaloTowerDetIds.size();      
      for (int i_CaloTowerDetId=0;i_CaloTowerDetId<theCaloTowerDetIds_n;i_CaloTowerDetId++) {
       CaloTowerCollection::const_iterator theTower=caloTowers->find(theCaloTowerDetIds[i_CaloTowerDetId]);  //Find the tower from its CaloTowerDetID	
       size_t numRecHits = theTower->constituentsSize();       
       for(size_t j=0;j<numRecHits;j++) {
	 DetId RecHitDetID=theTower->constituent(j);
	 DetId::Detector DetNum=RecHitDetID.det();     
	 if(DetNum==DetId::Hcal){
	   Hep3Vector theHCALCaloRecHit_Hep3V(0.,0.,0.);
	   theHCALCaloRecHit_Hep3V.setREtaPhi((double)theTower->hadEnergy(),(double)theTower->eta(),(double)theTower->phi());
	   if (theHCALCaloRecHit_Hep3V.deltaR(thepropagatedrectk_Hep3V)<calotk_conesize_) {
	     if (thelastselectedHCALCaloRecHit_Hep3V.r()==0. || (thelastselectedHCALCaloRecHit_Hep3V.r()!=0. && theHCALCaloRecHit_Hep3V.diff2(thelastselectedHCALCaloRecHit_Hep3V)>0.01)){
	       thelastselectedHCALCaloRecHit_Hep3V=theHCALCaloRecHit_Hep3V;
	       theHCALCaloRecHitsSum_Hep3V+=theHCALCaloRecHit_Hep3V;
	     }
	   }  
	 }
       }
      }
      return(theHCALCaloRecHitsSum_Hep3V.perp());
    }else return(-100.);
  }else return(-100.);
}

double CombinedTauTagAlg::ECALcellsEtSum_around_rectk(const IsolatedTauTagInfoRef& theIsolatedTauTagInfoRef,Event& theEvent,const EventSetup& theEventSetup,TrackRef therecTrack){ 
  if ((double)(*therecTrack).momentum().r()!=0.){
    ESHandle<CaloGeometry> theCaloGeometry;
    theEventSetup.get<IdealGeometryRecord>().get(theCaloGeometry);
    const CaloSubdetectorGeometry* theCaloSubdetectorGeometry;
    
    Hep3Vector theECALCaloRecHitsSum_Hep3V(0.,0.,0.);
    Hep3Vector thepropagatedrectk_Hep3V(0.,0.,0.);
    XYZPoint therecTrackImpactPositiononECAL=recTrackImpactPositiononECAL(theEvent,theEventSetup,therecTrack);
    if (therecTrackImpactPositiononECAL.z()!=0.){
      thepropagatedrectk_Hep3V.setRThetaPhi((*therecTrack).momentum().r(),(double)therecTrackImpactPositiononECAL.theta(),(double)therecTrackImpactPositiononECAL.phi());
      Handle<CaloTowerCollection> caloTowers;
      Handle<EBRecHitCollection> EBRecHits;
      Handle<EERecHitCollection> EERecHits;     
      theEvent.getByLabel("towerMaker",caloTowers);
      theEvent.getByLabel("ecalRecHit","EcalRecHitsEB",EBRecHits );
      theEvent.getByLabel("ecalRecHit","EcalRecHitsEE",EERecHits );
      const vector<CaloTowerDetId>&  theCaloTowerDetIds=theIsolatedTauTagInfoRef->jetRef()->jtaRef()->key->getTowerIndices();
      int theCaloTowerDetIds_n= theCaloTowerDetIds.size();
      for (int i_CaloTowerDetId=0;i_CaloTowerDetId<theCaloTowerDetIds_n;i_CaloTowerDetId++) {
       CaloTowerCollection::const_iterator theTower=caloTowers->find(theCaloTowerDetIds[i_CaloTowerDetId]);  //Find the tower from its CaloTowerDetID
	size_t numRecHits = theTower->constituentsSize();
	for(size_t j=0;j<numRecHits;j++) {
          DetId RecHitDetID=theTower->constituent(j);
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
	      if (theECALCaloRecHit_Hep3V.deltaR(thepropagatedrectk_Hep3V)<calotk_conesize_) theECALCaloRecHitsSum_Hep3V+=theECALCaloRecHit_Hep3V;
            }
            else if(EcalNum==2){
	      theCaloSubdetectorGeometry = theCaloGeometry->getSubdetectorGeometry(DetId::Ecal,EcalEndcap);
	      EEDetId EcalID = RecHitDetID;
              EERecHitCollection::const_iterator theRecHit=EERecHits->find(EcalID);	    
	      const CaloCellGeometry* theRecHitCell=theCaloSubdetectorGeometry->getGeometry(RecHitDetID);
	      Hep3Vector theECALCaloRecHit_Hep3V(0.,0.,0.);
	      theECALCaloRecHit_Hep3V.setREtaPhi((double)theRecHit->energy(),(double)theRecHitCell->getPosition().eta(),(double)theRecHitCell->getPosition().phi());
	      if (theECALCaloRecHit_Hep3V.deltaR(thepropagatedrectk_Hep3V)<calotk_conesize_) theECALCaloRecHitsSum_Hep3V+=theECALCaloRecHit_Hep3V;
	    }
          }
	}
      }
      return(theECALCaloRecHitsSum_Hep3V.perp());
    }else return(-100.);
  }else return(-100.);
}
;


