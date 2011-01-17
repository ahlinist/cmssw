// Class:      L25and3TauEfficiencyAnalyzer
// Original Author:  Eduardo Luiggi, modified by Sho Maruyama
//         Created:  Fri Apr  4 16:37:44 CDT 2008
// $Id: L25and3TauEfficiencyAnalyzer.cc,v 1.7 2010/09/09 18:25:02 eluiggi Exp $
#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L25and3TauEfficiencyAnalyzer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
using namespace edm;
using namespace reco;
using namespace std;
L25and3TauEfficiencyAnalyzer::L25and3TauEfficiencyAnalyzer(){}

L25and3TauEfficiencyAnalyzer::L25and3TauEfficiencyAnalyzer(const edm::ParameterSet& iConfig){}

L25and3TauEfficiencyAnalyzer::~L25and3TauEfficiencyAnalyzer(){}

void L25and3TauEfficiencyAnalyzer::Setup(const edm::ParameterSet& iConfig,TTree* l25tree){
  l25JetSource = iConfig.getParameter<InputTag>("l25JetSource");
  l25PtCutSource = iConfig.getParameter<InputTag>("l25PtCutSource");
  l3IsoSource  = iConfig.getParameter<InputTag>("l3IsoSource");
  l25MatchingCone = iConfig.getParameter<double>("l25MatchingCone");
  HLTPFTau = iConfig.getParameter<bool>("HLTPFTau");
  l25tree->Branch("l25Eta", &l25Eta,"l25Eta/F" );
  l25tree->Branch("l25Phi", &l25Phi,"l25Phi/F" );
  l25tree->Branch("l25Et", &l25Et,"l25Et/F" );
  l25tree->Branch("l25Pt", &l25Pt,"l25Pt/F" );
  l25tree->Branch("l25PtLdgLoose",&l25PtLdgLoose,"l25PtLdgLoose/F");
  l25tree->Branch("l25EtaLdgLoose",&l25EtaLdgLoose,"l25EtaLdgLoose/F");
  l25tree->Branch("l25PhiLdgLoose",&l25PhiLdgLoose,"l25PhiLdgLoose/F");
  l25tree->Branch("l25PtLdgJetDRLoose",&l25PtLdgJetDRLoose,"l25PtLdgJetDRLoose/F");
  l25tree->Branch("l25InvPt", &l25InvPt,"l25InvPt/F" );
  l25tree->Branch("l25Depth", &l25Depth,"l25Depth/B" );
  l25tree->Branch("leadDepth1", &leadDepth1,"leadDepth1/B" );
  l25tree->Branch("leadDepth2", &leadDepth2,"leadDepth2/B" );
  l25tree->Branch("minDR", &minDR,"minDR/F" );
  l25tree->Branch("bareEt", &bareEt,"bareEt/F" );
  l25tree->Branch("l25DefDisc_Trk5_IsoPtMin1_Ntrk0",&l25DefDisc_Trk5_IsoPtMin1_Ntrk0,"l25DefDisc_Trk5_IsoPtMin1_Ntrk0/B");
  l25tree->Branch("l25DefDisc_Trk5_IsoPtMin1_Ntrk1",&l25DefDisc_Trk5_IsoPtMin1_Ntrk1,"l25DefDisc_Trk5_IsoPtMin1_Ntrk1/B");
  l25tree->Branch("l25DefDisc_Trk5_IsoPtMin1_5_Ntrk0",&l25DefDisc_Trk5_IsoPtMin1_5_Ntrk0,"l25DefDisc_Trk5_IsoPtMin1_5_Ntrk0/B");
  l25tree->Branch("l25DefDisc_Trk5_IsoPtMin1_5_Ntrk1",&l25DefDisc_Trk5_IsoPtMin1_5_Ntrk1,"l25DefDisc_Trk5_IsoPtMin1_5_Ntrk1/B");
  l25tree->Branch("l25DefDisc_Trk5_IsoPtMin2_Ntrk0",&l25DefDisc_Trk5_IsoPtMin2_Ntrk0,"l25DefDisc_Trk5_IsoPtMin2_Ntrk0/B");
  l25tree->Branch("l25IsoPtSum",&l25IsoPtSum,"l25IsoPtSum/F");
  l25tree->Branch("l25IsoTrkNHits",&l25IsoTrkNHits,"l25IsoTrkNHits/I");
  l25tree->Branch("l25IsoTrkChi2",&l25IsoTrkChi2,"l25IsoTrkChi2/F");
  l25tree->Branch("l25IsoTrkPt",&l25IsoTrkPt,"l25IsoTrkPt/F");
  l25tree->Branch("l3GammaIsoSum",&l3GammaIsoSum,"l3GammaIsoSum/F");
  l25tree->Branch("l3TrkIsoSum",&l3TrkIsoSum,"l3TrkIsoSum/F");
  
}

void L25and3TauEfficiencyAnalyzer::fill(const edm::Event& iEvent, const reco::Particle& tau) {
  fill(iEvent,tau.p4());
}

void L25and3TauEfficiencyAnalyzer::fill(const edm::Event& iEvent, const reco::Candidate& tau) {
  fill(iEvent,tau.p4());
}

void L25and3TauEfficiencyAnalyzer::fill(const edm::Event& iEvent, const LorentzVector& tau) {
  // PF specific quantities are moved to TTEff analyzer.
  //this was originally the calotau method, but since it only uses direction from the calojet
  //make this method the LorentzVector one and add a CaloTau stub that calls this method - gfball
  
  //modified to collect extra discriminator parameters
  l25Et = 0;
  l25Phi = -999.;
  l25Eta = -999.;
  l25Pt = 0;
  l25InvPt = 0;
  l25Depth = 0;
  l25PtLdgJetDRLoose = 0;
  l25EtaLdgLoose = 0;
  l25PhiLdgLoose = 0;
  l25PtLdgLoose = 0;
  
  l25DefDisc_Trk5_IsoPtMin1_Ntrk0 = 0;
  l25DefDisc_Trk5_IsoPtMin1_Ntrk1 = 0;
  l25DefDisc_Trk5_IsoPtMin1_5_Ntrk0 = 0;
  l25DefDisc_Trk5_IsoPtMin1_5_Ntrk1 = 0;
  l25DefDisc_Trk5_IsoPtMin2_Ntrk0 = 0;
 
  l25IsoPtSum = 0;
  l25NTrksIso = 0;

  if(HLTPFTau == true){
    Handle<PFTauTagInfoCollection> tags;
    Handle<PFTauCollection> ptJets;
    Handle<PFTauCollection> isoJets;
    
    iEvent.getByLabel(l25JetSource, tags);
    iEvent.getByLabel(l25PtCutSource, ptJets); // Leading Pt Cut > X GeV/c applied, check HLT Config file
    iEvent.getByLabel(l3IsoSource, isoJets);
    
    if(tags.isValid()){
      for(unsigned int j = 0; j < tags->size(); j++){ // bare L2 Taus
	if(deltaR(tau, *(tags->at(j).pfjetRef())) < l25MatchingCone){ // dr < l25MatchingCone
	  if(l25Depth < 1) l25Depth = 1; // L2 match
	  l25Eta  = (tags->at(j).pfjetRef()->eta());
	  l25Phi  = (tags->at(j).pfjetRef()->phi());
	  l25Et   = (tags->at(j).pfjetRef()->et());
	  const PFCandidateRefVector chargedHads = tags->at(j).PFChargedHadrCands();
	  for(unsigned int k = 0; k < chargedHads.size(); k++){
	    if(chargedHads.at(k)->pt() > 1.0 && deltaR(*(tags->at(j).pfjetRef()),*(chargedHads.at(k)) ) < 0.2){ // lead charged had in cone 0.2
	      l25Pt  =  chargedHads.at(k)->pt();
	      if(l25Depth < 2){
		l25Depth = 2; // lead track
		break;
	      }
	    }// good lead cand
	  } // charged hadron loop
	}// offline and l25 pf tau match dr < l25MatchingCone
      } // tag loop
    }// non empty collection
    else {
      //edm::LogWarning("TTEffAnalyzer") <<"No L25Jetsource in event!"<<std::endl;
    }
    if(ptJets.isValid()){ // Leading Pt Cut > X GeV/c applied, check HLT Config file
      for(unsigned int j = 0; j < ptJets->size(); j++){
	if(deltaR(tau, ptJets->at(j) ) < l25MatchingCone){ // dr < l25MatchingCone
	  if(l25Depth < 3){
	    l25Depth = 3; // lead pt cut match
	    l3TrkIsoSum = ptJets->at(j).isolationPFChargedHadrCandsPtSum();
	    l3GammaIsoSum = ptJets->at(j).isolationPFGammaCandsEtSum();
	    break;
	  }
	}// pf and l25 tau match dr < l25MatchingCone
      }// for jet loop
    }// non empty collection
    else {
      //edm::LogWarning("TTEffAnalyzer") <<"No L25PtJetsource in event!"<<std::endl;
    }
/*
    if(isoJets.isValid()){
      for(unsigned int j = 0; j < isoJets->size(); j++){
	if(deltaR(tau, isoJets->at(j)) < l25MatchingCone){ // dr < l25MatchingCone
	  if(l25Depth < 4){
	    l25Depth = 4; // iso match
	    break;
	  }
	}
      }
    }

    else {
      //edm::LogWarning("TTEffAnalyzer") <<"No L3IsoJetsource in event!"<<std::endl;
    }
    
*/   
  }
  else {
    Handle<IsolatedTauTagInfoCollection> tags;
    Handle<CaloJetCollection> ptJets;
    Handle<CaloJetCollection> isoJets;

    iEvent.getByLabel(l25JetSource, tags);
    iEvent.getByLabel(l25PtCutSource, ptJets); // Leading Pt Cut > X GeV/c applied, check HLT Config file
    iEvent.getByLabel(l3IsoSource, isoJets);

    if(tags.isValid()){
      for(unsigned int j = 0; j < tags->size(); j++){ // bare L2 Taus
	if(deltaR(tau, *(tags->at(j).jet())) < l25MatchingCone){ // dr < l25MatchingCone
	  if(l25Depth < 1) l25Depth = 1; // L2 match
	  l25Eta  = (tags->at(j).jet()->eta());
	  l25Phi  = (tags->at(j).jet()->phi());
	  l25Et   = (tags->at(j).jet()->et());
	  const TrackRef leadTrk = tags->at(j).leadingSignalTrack(0.2,1.0);// track finding cone = 0.2
	  if(leadTrk.isNonnull()) {
	    if(l25Depth < 2){
	      l25Depth = 2;
	      l25Pt = (leadTrk->pt() );
	      l25InvPt = 1./(leadTrk->pt() );
	    }// good lead cand
	  }
	  const TrackRef leadTrkLoose = tags->at(j).leadingSignalTrack(100.,1.0);// Loose Track finding
	  if(leadTrkLoose.isNonnull()){
	    l25PtLdgLoose = leadTrkLoose->pt();
	    l25EtaLdgLoose = leadTrkLoose->eta(); 
	    l25PhiLdgLoose = leadTrkLoose->phi();
	    double dphi = fabs(l25PhiLdgLoose-l25Phi);
	    if(dphi>2*acos(-1.))dphi=2*acos(-1.)-dphi;
	    double deta = fabs(l25EtaLdgLoose-l25Eta);
	    l25PtLdgJetDRLoose = sqrt(dphi*dphi+deta*deta);
	  }
	  
	  // get the tracks in iso region around leading track 
	  if(leadTrk.isNonnull()){
	    l25IsoPtSum = 0.;
	    const TrackRefVector theSignalTracks = tags->at(j).tracksInCone(leadTrk->momentum(), 0.15, 1.0);
	    const TrackRefVector theIsoTracks = tags->at(j).tracksInCone(leadTrk->momentum(), 0.5, 1.0);
	    l25NTrksIso = theIsoTracks.size() - theSignalTracks.size();
	    
	    for(TrackRefVector::const_iterator isoIt = theIsoTracks.begin(); isoIt != theIsoTracks.end(); ++isoIt){
	      if(deltaR(leadTrk->momentum(), (*isoIt)->momentum()) > 0.15){
	      	l25IsoPtSum += (*isoIt)->pt();
	      	l25IsoTrkChi2 = (*isoIt)->chi2();
	      	l25IsoTrkPt = (*isoIt)->pt();
	      	l25IsoTrkNHits = (*isoIt)->numberOfValidHits();
	      }
	    }
	  }
	  	  
          //evaluate a series of different discriminator parameters
	  // MatchConeSize, sigCone, isoCone, ltPt, pt min, nTracksIsoRing, ltDz
	  if(tags->at(j).discriminator(0.1,0.15,0.5,5.,1.,0,0.2))l25DefDisc_Trk5_IsoPtMin1_Ntrk0=1;
	  if(tags->at(j).discriminator(0.1,0.15,0.5,5.,1.,1,0.2))l25DefDisc_Trk5_IsoPtMin1_Ntrk1=1;
	  if(tags->at(j).discriminator(0.2,0.15,0.5,5.,1.5,0,0.2))l25DefDisc_Trk5_IsoPtMin1_5_Ntrk0=1;
	  if(tags->at(j).discriminator(0.2,0.15,0.5,5.,1.5,1,0.2))l25DefDisc_Trk5_IsoPtMin1_5_Ntrk1=1;
	  if(tags->at(j).discriminator(0.2,0.15,0.5,5.,2.0,0,0.2))l25DefDisc_Trk5_IsoPtMin2_Ntrk0=1;
	  	  
	}// calo and l25 tau match dr < l25MatchingCone
      }// for jet loop
    }// non empty collection
    else {
      //edm::LogWarning("TTEffAnalyzer") <<"No L25Jetsource in event!"<<std::endl;
    }
    
    if(ptJets.isValid()){ // Leading Pt Cut > X GeV/c applied, check HLT Config file
      for(unsigned int j = 0; j < ptJets->size(); j++){
	if(deltaR(tau, ptJets->at(j) ) < l25MatchingCone){ // dr < l25MatchingCone
	  if(l25Depth < 3){
	    l25Depth = 3; // lead pt cut match
	    break;
	  }
	}// pf and l25 tau match dr < l25MatchingCone
      }// for jet loop
    }// non empty collection
    else {
      //edm::LogWarning("TTEffAnalyzer") <<"No L25PtJetsource in event!"<<std::endl;
    }
    
    if(isoJets.isValid()){
      for(unsigned int j = 0; j < isoJets->size(); j++){
	if(deltaR(tau, isoJets->at(j)) < l25MatchingCone){ // dr < l25MatchingCone
	  if(l25Depth < 4){
	    l25Depth = 4; // iso match
	    break;
	  }
	}
      }
    }
    else {
      //edm::LogWarning("TTEffAnalyzer") <<"No L3IsoJetsource in event!"<<std::endl;
    }
    
  }
}// tau ends

void L25and3TauEfficiencyAnalyzer::beginJob(const edm::EventSetup&){} 
void L25and3TauEfficiencyAnalyzer::analyze(const edm::Event&, const edm::EventSetup&){}
void L25and3TauEfficiencyAnalyzer::endJob(){} 
