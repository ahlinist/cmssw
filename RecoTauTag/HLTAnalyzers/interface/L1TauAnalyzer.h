// system include files
#include <memory>
#include <string>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// HEP library include files
#include <Math/GenVector/VectorUtil.h>
//#include "CLHEP/HepMC/GenEvent.h"
//#include "CLHEP/HepMC/GenVertex.h"
//#include "CLHEP/HepMC/GenParticle.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "RecoTauTag/HLTAnalyzers/interface/MCTauCand.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "HepMC/GenEvent.h"

// L1 Trigger data formats
#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "DataFormats/L1Trigger/interface/L1EtMissParticle.h"
#include "DataFormats/L1Trigger/interface/L1EtMissParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1ParticleMap.h"

// Histogram containers
#include "RecoTauTag/HLTAnalyzers/interface/L1TauHistograms.h"
#include "RecoTauTag/HLTAnalyzers/interface/L1ElecHistograms.h"
#include "RecoTauTag/HLTAnalyzers/interface/L1JetMETHistograms.h"
#include "RecoTauTag/HLTAnalyzers/interface/L1Acceptance.h"

// reco
#include "DataFormats/JetReco/interface/BasicJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/METReco/interface/GenMET.h"
//#include "DataFormats/JetReco/interface/JetTracksAssociation.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"

//
// class decleration
//

class L1TauAnalyzer : public edm::EDAnalyzer {
public:
  explicit L1TauAnalyzer(const edm::ParameterSet&);
  ~L1TauAnalyzer();
  
  
private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  void getGenObjects(const edm::Event&, const edm::EventSetup&);
  void getRecoObjects(const edm::Event&, const edm::EventSetup&);
  void getFastL1Objects(const edm::Event&, const edm::EventSetup&);
  void getL1extraObjects(const edm::Event&, const edm::EventSetup&);
  void getTauJets(const edm::Event& iEvent);
  void getMuons(const edm::Event& iEvent);
  void getElecs(const edm::Event& iEvent);

  bool passTauCuts(const reco::Jet&);
  bool passElecCuts(const reco::PixelMatchGsfElectron&);
  bool passMuCuts(const reco::Muon&);
  bool passMETCuts(const reco::CaloMET&);

  void fillFastL1Histos();
  void makeFastL1Histos();
  void fillL1extraHistos();
  void makeL1extraHistos();
  void fillL1CompHistos();
  void fillL1GenCompHistos();

  void printGenInfo(const edm::Event&);
  
  // ----------member data ---------------------------

  // Config parameters
  bool _doGenObjects;
  bool _doFastL1Objects;
  bool _doL1extraObjects;
  bool _doRecoObjects;
  bool _doPrintGenInfo;
  bool _doRecoCuts;

  std::string _GeneratorSource;

  std::string _FastL1Source;
  std::string _FastL1metSource;
  std::string _FastL1taujetSource;
  std::string _FastL1cenjetSource;
  std::string _FastL1forjetSource;
  std::string _FastL1egammaSource;
  std::string _FastL1isoegammaSource;

  std::string _L1extraSource;
  std::string _L1extramuonSource;
  std::string _L1extrametSource;
  std::string _L1extrataujetSource;
  std::string _L1extracenjetSource;
  std::string _L1extraforjetSource;
  std::string _L1extraegammaSource;
  std::string _L1extraisoegammaSource;

  std::string _iCone5CaloJetSource;
  std::string _iCone5GenJetSource;
  std::string _CaloMETSource;
  std::string _GenMETSource;
  std::string _ConeIsoTauJetTagSource;
  std::string _GlobalMuonSource;
  std::string _PixelElecSource;

  // List of generated leptons from Z, W or H
  std::vector<HepMC::GenParticle> _GenBosons;
  std::vector<HepMC::GenParticle> _GenElecs;
  std::vector<HepMC::GenParticle> _GenMuons;
  std::vector<HepMC::GenParticle> _GenTauElecs;
  std::vector<HepMC::GenParticle> _GenTauMuons;
  std::vector<MCTauCand> _GenTaus;

  // FastL1 objects
  //l1extra::L1EtMissParticle _FastL1MET;
  l1extra::L1EtMissParticleCollection _FastL1MET;
  l1extra::L1JetParticleCollection _FastL1TauJets;
  l1extra::L1JetParticleCollection _FastL1CenJets;
  l1extra::L1JetParticleCollection _FastL1ForJets;
  l1extra::L1JetParticleCollection _FastL1JetsNoTau;
  l1extra::L1JetParticleCollection _FastL1InclJets;
  l1extra::L1EmParticleCollection _FastL1Egammas;
  l1extra::L1EmParticleCollection _FastL1isoEgammas;

  // L1extra objects
  //l1extra::L1EtMissParticle _L1extraMET;
  l1extra::L1EtMissParticleCollection _L1extraMET;
  l1extra::L1MuonParticleCollection _L1extraMuons;
  l1extra::L1JetParticleCollection _L1extraTauJets;
  l1extra::L1JetParticleCollection _L1extraCenJets;
  l1extra::L1JetParticleCollection _L1extraForJets;
  l1extra::L1JetParticleCollection _L1extraJetsNoTau;
  l1extra::L1JetParticleCollection _L1extraInclJets;
  l1extra::L1EmParticleCollection _L1extraEgammas;
  l1extra::L1EmParticleCollection _L1extraisoEgammas;

  // reco objects
  reco::CaloJetCollection _iCone5CaloJets;
  std::vector<reco::Jet> _iCone5TauJets;
  reco::GenJetCollection _iCone5GenJets;
  reco::METCollection _GenMET;
  reco::CaloMETCollection _CaloMET;
  
  reco::MuonCollection _Muons;
  reco::PixelMatchGsfElectronCollection _PixelElectrons;

  // Histogram containers
  L1TauHistograms* _FastL1TauHistos;
  L1TauHistograms* _L1extraTauHistos;
  L1ElecHistograms* _FastL1ElecHistos;
  L1ElecHistograms* _L1extraElecHistos;
  L1JetMETHistograms* _FastL1JetMETHistos;
  L1JetMETHistograms* _L1extraJetMETHistos;

  L1Acceptance* _FastL1Acceptance;
  L1Acceptance* _L1extraAcceptance;

};
