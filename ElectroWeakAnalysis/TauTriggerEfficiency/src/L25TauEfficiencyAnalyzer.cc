// Class:      L25TauEfficiencyAnalyzer
// Original Author:  Eduardo Luiggi, modified by Sho Maruyama
//         Created:  Fri Apr  4 16:37:44 CDT 2008
// $Id: L25TauEfficiencyAnalyzer.cc,v 1.16 2008/12/12 15:42:56 gfball Exp $
#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L25TauEfficiencyAnalyzer.h"
using namespace edm;
using namespace reco;
using namespace std;
L25TauEfficiencyAnalyzer::L25TauEfficiencyAnalyzer(){}

L25TauEfficiencyAnalyzer::L25TauEfficiencyAnalyzer(const edm::ParameterSet& iConfig){}

L25TauEfficiencyAnalyzer::~L25TauEfficiencyAnalyzer(){}

void L25TauEfficiencyAnalyzer::Setup(const edm::ParameterSet& iConfig,TTree* l25tree){
  l25JetSource = iConfig.getParameter<InputTag>("l25JetSource");
  l25PtCutSource = iConfig.getParameter<InputTag>("l25PtCutSource");
  l25IsoSource = iConfig.getParameter<InputTag>("l25IsoSource");
  l25tauSource = iConfig.getParameter<InputTag>("PFTauCollection");
  l25MatchingCone = iConfig.getParameter<double>("l25MatchingCone");
  l25tree->Branch("l25Eta", &l25Eta,"l25Eta/F" );
  l25tree->Branch("l25Phi", &l25Phi,"l25Phi/F" );
  l25tree->Branch("l25Et", &l25Et,"l25Et/F" );
  l25tree->Branch("l25Pt", &l25Pt,"l25Pt/F" );
  l25tree->Branch("l25Depth", &l25Depth,"l25Depth/I" );
}

void L25TauEfficiencyAnalyzer::fill(const edm::Event& iEvent, const reco::PFTau& tau) {
  fill(iEvent,tau.p4());
}

void L25TauEfficiencyAnalyzer::fill(const edm::Event& iEvent, const reco::CaloTau& tau) {
  //stub method to call the LorentzVector method
  fill(iEvent,tau.p4());
}

void L25TauEfficiencyAnalyzer::fill(const edm::Event& iEvent, const LorentzVector& tau) {
  // PF specific quantities are moved to TTEff analyzer.
  //this was originally the calotau method, but since it only uses direction from the calojet
  //make this method the LorentzVector one and add a CaloTau stub that calls this method - gfball
  Handle<IsolatedTauTagInfoCollection> tags;
  Handle<CaloJetCollection> ptJets;
  Handle<CaloJetCollection> isoJets;
  l25Et = 0;
  l25Phi = 0;
  l25Eta = 0;
  l25Pt = 0;
  l25Depth = 0;
  if(iEvent.getByLabel(l25JetSource, tags)){
    for(unsigned int j = 0; j < tags->size(); j++){ // bare L2 Taus
      if(deltaR(tau, *(tags->at(j).jet())) < l25MatchingCone){ // dr < l25MatchingCone
	if(l25Depth < 1) l25Depth = 1; // L2 match
	l25Eta  = (tags->at(j).jet()->eta());
	l25Phi  = (tags->at(j).jet()->phi());
	l25Et   = (tags->at(j).jet()->et());
    const TrackRef leadTrk = tags->at(j).leadingSignalTrack(0.2,1.0);// track finding cone = 0.2 
	if(leadTrk.isNonnull()){                                                        
	    l25Pt  =  (leadTrk->pt() );
	    }// good lead cand
      }// calo and l25 tau match dr < l25MatchingCone
    }// for jet loop
  }// non empty collection
  
  if(iEvent.getByLabel(l25PtCutSource, ptJets)){ // Leading Pt Cut > X GeV/c applied, check HLT Config file
    for(unsigned int j = 0; j < ptJets->size(); j++){
      if(deltaR(tau, ptJets->at(j) ) < l25MatchingCone){ // dr < l25MatchingCone
	if(l25Depth < 2){
	    l25Depth = 2; // lead pt cut match
        break;
	    }
      }// pf and l25 tau match dr < l25MatchingCone
    }// for jet loop
  }// non empty collection
  
  if(iEvent.getByLabel(l25IsoSource, isoJets)){
    for(unsigned int j = 0; j < isoJets->size(); j++){
      if(deltaR(tau, isoJets->at(j)) < l25MatchingCone){ // dr < l25MatchingCone
	if(l25Depth < 3){
        l25Depth = 3; // iso match
        break;
        }
      }
    }
  }
}// tau ends

void L25TauEfficiencyAnalyzer::beginJob(const edm::EventSetup&){} 
void L25TauEfficiencyAnalyzer::analyze(const edm::Event&, const edm::EventSetup&){}
void L25TauEfficiencyAnalyzer::endJob(){} 
