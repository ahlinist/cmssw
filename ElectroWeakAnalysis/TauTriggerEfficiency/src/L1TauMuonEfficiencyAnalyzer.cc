// -*- C++ -*-
//
// Package:    TTEffAnalyzer
// Class:      L1TauMuonEfficiencyAnalyzer
// 
/**\class L1TauMuonEfficiencyAnalyzer L1TauMuonEfficiencyAnalyzer.cc ElectroWeakAnalysis/TTEffAnalyzer/src/L1TauMuonEfficiencyAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
//#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "TTree.h"
#include "TLorentzVector.h"

#include<vector>

using l1extra::L1JetParticleCollection;

//
class L1TauMuonEfficiencyAnalyzer : public edm::EDAnalyzer {
   public:
      explicit L1TauMuonEfficiencyAnalyzer(const edm::ParameterSet&);
      ~L1TauMuonEfficiencyAnalyzer();

   private:
      typedef math::XYZTLorentzVector LorentzVector;

      virtual void beginJob();
      virtual void analyze(const edm::Event&, const edm::EventSetup&);

  typedef std::pair<double, l1extra::L1JetParticleCollection::const_iterator> L1Pair;
  typedef std::pair<double, reco::GenJetCollection::const_iterator> GenPair;
  
  edm::InputTag MCParticles_;
  edm::InputTag genJets_;
  edm::InputTag L1extraTauJetSource;
  edm::InputTag L1extraCentralJetSource;

  TTree *tree;
  unsigned int b_event;
  unsigned int b_run;
  unsigned int b_lumi;
  TLorentzVector b_muon;
  std::vector<int> b_muonMothers;
  TLorentzVector b_matchL1Jet;
  TLorentzVector b_matchGenJet;
  int b_matchL1JetInd;
  int b_matchGenJetInd;
  float b_matchL1JetDR;
  float b_matchGenJetDR;
  bool b_matchL1IsTau;
  bool b_matchL1IsCen;

};

DEFINE_FWK_MODULE(L1TauMuonEfficiencyAnalyzer);


L1TauMuonEfficiencyAnalyzer::L1TauMuonEfficiencyAnalyzer(const edm::ParameterSet& iConfig):
  MCParticles_(iConfig.getParameter<edm::InputTag>("GenParticleCollection")),
  genJets_(iConfig.getParameter<edm::InputTag>("GenJetCollection")),
  L1extraTauJetSource(iConfig.getParameter<edm::InputTag>("L1extraTauJetSource")),
  L1extraCentralJetSource(iConfig.getParameter<edm::InputTag>("L1extraCentralJetSource"))
{}
L1TauMuonEfficiencyAnalyzer::~L1TauMuonEfficiencyAnalyzer() {}
void L1TauMuonEfficiencyAnalyzer::beginJob() {
  edm::Service<TFileService> fs;
  tree = fs->make<TTree>("tree", "L1 tau and MC muon tree");

  tree->Branch("event", &b_event);
  tree->Branch("run", &b_run);
  tree->Branch("lumi", &b_lumi);

  tree->Branch("muon", &b_muon);
  tree->Branch("muonMothers", &b_muonMothers);
  tree->Branch("matchL1Jet", &b_matchL1Jet);
  tree->Branch("matchL1JetIndex", &b_matchL1JetInd);
  tree->Branch("matchL1JetDR", &b_matchL1JetDR);
  tree->Branch("matchL1JetIsTau", &b_matchL1IsTau);
  tree->Branch("matchL1JetIsCen", &b_matchL1IsCen);
  tree->Branch("matchGenJet", &b_matchGenJet);
  tree->Branch("matchGenJetIndex", &b_matchGenJetInd);
  tree->Branch("matchGenJetDR", &b_matchGenJetDR);
}

template <typename T>
static
std::pair<double, typename T::const_iterator> match(const reco::GenParticle& muon, const T& coll) {
  double minDR = 1e10;
  typename T::const_iterator found = coll.end();
  for(typename T::const_iterator iJet = coll.begin(); iJet != coll.end(); ++iJet) {
    double DR = deltaR(muon, *iJet);
    if(DR < minDR) {
      found = iJet;
      minDR = DR;
    }
  }

  return make_pair(minDR, found);
}

void L1TauMuonEfficiencyAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::Handle<reco::GenParticleCollection> mcParticles;
  iEvent.getByLabel(MCParticles_, mcParticles);

  edm::Handle<reco::GenJetCollection> genJets;
  iEvent.getByLabel(genJets_, genJets);

  edm::Handle<L1JetParticleCollection> l1TauHandle;
  iEvent.getByLabel(L1extraTauJetSource, l1TauHandle);

  edm::Handle<L1JetParticleCollection> l1CentralJetHandle;
  iEvent.getByLabel(L1extraCentralJetSource, l1CentralJetHandle);

  b_event = iEvent.id().event();
  b_run = iEvent.id().run();
  b_lumi = iEvent.luminosityBlock();

  for(reco::GenParticleCollection::const_iterator iMcMuon = mcParticles->begin(); iMcMuon != mcParticles->end(); ++iMcMuon) {
    if(abs(iMcMuon->pdgId()) != 13)
      continue;

    const reco::Candidate *mother = iMcMuon->mother();
    if(mother && abs(mother->pdgId()) == 13) {
      continue;
    }

    //std::cout << "Muon with pt " << iMcMuon->pt() << std::endl;

    b_muonMothers.clear();
    int prevPdgId = iMcMuon->pdgId();
    while(mother) {
      if(prevPdgId == mother->pdgId()) {
        mother = mother->mother();
        continue;
      }

      //std::cout << "  mother " << mother->pdgId() << " pt " << mother->pt() << std::endl;
      
      prevPdgId = mother->pdgId();
      b_muonMothers.push_back(prevPdgId);
      mother = mother->mother();
    }

    b_muon.SetXYZT(iMcMuon->px(), iMcMuon->py(), iMcMuon->pz(), iMcMuon->energy());
    b_matchL1Jet.SetXYZT(0,0,0,0);
    b_matchL1JetDR = 1e10;
    b_matchL1JetInd = -1;
    b_matchL1IsTau = false;
    b_matchL1IsCen = false;

    b_matchGenJet.SetXYZT(0,0,0,0);
    b_matchGenJetDR = 1e10;
    b_matchGenJetInd = -1;

    L1Pair tauJet = match(*iMcMuon, *l1TauHandle);
    if(tauJet.second != l1TauHandle->end()) {
      b_matchL1IsTau = true;
      b_matchL1JetDR = tauJet.first;
      L1Pair::second_type iJet = tauJet.second;
      b_matchL1Jet.SetXYZT(iJet->px(), iJet->py(), iJet->pz(), iJet->energy());
      b_matchL1JetInd = iJet - l1TauHandle->begin();
    }

    L1Pair cenJet = match(*iMcMuon, *l1CentralJetHandle);
    if(cenJet.second != l1CentralJetHandle->end() && cenJet.first < b_matchL1JetDR) {
      b_matchL1IsCen = true;
      b_matchL1IsTau = false;
      b_matchL1JetDR = cenJet.first;
      L1Pair::second_type iJet = cenJet.second;
      b_matchL1Jet.SetXYZT(iJet->px(), iJet->py(), iJet->pz(), iJet->energy());
      b_matchL1JetInd = iJet - l1CentralJetHandle->begin();
    }

    GenPair genJet = match(*iMcMuon, *genJets);
    if(genJet.second != genJets->end()) {
      b_matchGenJetDR = genJet.first;
      GenPair::second_type iJet = genJet.second;
      b_matchGenJet.SetXYZT(iJet->px(), iJet->py(), iJet->pz(), iJet->energy());
      b_matchGenJetInd = iJet - genJets->begin();
    }

    tree->Fill();
  }
}
