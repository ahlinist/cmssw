#include "TauAnalysis/BgEstimationTools/plugins/TauIdEffZtoMuTauHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/IsolationAlgos/interface/IsoDepositVetoFactory.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEtFwd.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

#include <stdlib.h>

TauIdEffZtoMuTauHistManager::TauIdEffZtoMuTauHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<TauIdEffZtoMuTauHistManager::TauIdEffZtoMuTauHistManager>:" << std::endl;

  muonSrc_ = cfg.getParameter<edm::InputTag>("muonSource");
  tauSrc_ = cfg.getParameter<edm::InputTag>("tauSource");
  diTauSrc_ = cfg.getParameter<edm::InputTag>("diTauSource");
  centralJetSrc_ = cfg.getParameter<edm::InputTag>("centralJetSource");

  tauIdDiscriminator_ = cfg.getParameter<std::string>("tauIdDiscriminator");
  
  edm::ParameterSet cfgDiTauChargeSignExtractor = cfg.getParameter<edm::ParameterSet>("diTauChargeSignExtractor");
  std::string pluginType = cfgDiTauChargeSignExtractor.getParameter<std::string>("pluginType");
  diTauChargeSignExtractor_ = ObjValExtractorPluginFactory::get()->create(pluginType, cfgDiTauChargeSignExtractor);

  tauIsolationVetos_.push_back(IsoDepositVetoFactory::make("0.5"));
}

TauIdEffZtoMuTauHistManager::~TauIdEffZtoMuTauHistManager()
{
  delete diTauChargeSignExtractor_;

  for ( std::vector<reco::isodeposit::AbsVeto*>::iterator it = tauIsolationVetos_.begin();
	it != tauIsolationVetos_.end(); ++it ) {
    delete (*it);
  }
}

void TauIdEffZtoMuTauHistManager::bookHistogramsImp()
{
  //std::cout << "<TauIdEffZtoMuTauHistManager::bookHistogramsImp>:" << std::endl;

//--- book histogram for tau id. discriminator the efficiency of which is to be measured
//    and for isolation variables computed in the annulus dRmin = 0.5 to dRmax = 1.0 around jet-axis
  hTauDiscriminatorByEwkTauId_ = book1D("TauDiscriminatorByEwkTauId", "TauDiscriminatorByEwkTauId", 2, -0.5, 1.5);
  hTauExtParticleFlowIsoPt_ = book1D("TauExtParticleFlowIsoPt", "TauExtParticleFlowIsoPt", 100, 0., 25.);   
  hTauExtPFChargedHadronIsoPt_ = book1D("TauExtPFChargedHadronIsoPt", "TauExtPFChargedHadronIsoPt", 100, 0., 25.);  
  hTauExtPFNeutralHadronIsoPt_ = book1D("TauExtPFNeutralHadronIsoPt", "TauExtPFNeutralHadronIsoPt", 100, 0., 25.);  
  hTauExtPFGammaIsoPt_ = book1D("TauExtPFGammaIsoPt", "TauExtPFGammaIsoPt", 100, 0., 25.);  

  float muonPtBinning1d[] = { 15., 20., 25., 30., 35., 40., 50., 60., 80., 120. };
  hMuonPt_ = book1D("MuonPt", "MuonPt", 9, muonPtBinning1d);  
  hMuonAbsEta_ = book1D("MuonAbsEta", "MuonAbsEta", 14, 0., 2.1);  
  float muonAbsEtaBinning2d[] = { 0., 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1 };
  float muonPtBinning2d[] = { 15., 20., 25., 30., 40., 60., 120. };
  hMuonPtVsAbsEta_ = book2D("MuonPtVsAbsEta", "MuonPtVsAbsEta", 7, muonAbsEtaBinning2d, 6, muonPtBinning2d);
  hMuonExtTrkIsoPt_ = book1D("MuonExtTrkIsoPt", "MuonExtTrkIsoPt", 100, 0., 25.); 
  hMuonExtEcalIsoPt_ = book1D("MuonExtEcalIsoPt", "MuonExtEcalIsoPt", 100, 0., 25.); 
  hMuonExtHcalIsoPt_ = book1D("MuonExtHcalIsoPt", "MuonExtHcalIsoPt", 100, 0., 25.); 
  hMuonExtIsoSumPt_ = book1D("MuonExtIsoSumPt", "MuonExtIsoSumPt", 100, 0., 25.); 

  hDiTauChargeSign_ = book1D("DiTauChargeSign", "DiTauChargeSign", 3, -1.5, +1.5);  
  hDiTauPtProj_ = book1D("DiTauPtProj", "DiTauPtProj", 200, -100., +100.);
  hDiTauMEtProj_ = book1D("DiTauMEtProj", "DiTauMEtProj", 200, -100., +100.);

  hNumCentralJets_ = book1D("NumCentralJets", "NumCentralJets", 5, -0.5, 4.5);
  hCentralJetPt_ = book1D("CentralJetPt", "CentralJetPt", 75, 0., 150.);
  hCentralJetEta_ = book1D("CentralJetEta", "CentralJetEta", 60, -3., +3.);
  hCentralJetPhi_ = book1D("CentralJetPhi", "CentralJetPhi", 36, -TMath::Pi(), +TMath::Pi());
}

double getMuonExtIso(const pat::Muon& patMuon, const pat::IsolationKeys& pfIsoType)
{
  const pat::IsoDeposit* muonIsoDeposit = patMuon.isoDeposit(pfIsoType);
  if ( muonIsoDeposit ) 
    return muonIsoDeposit->depositWithin(1.0);
  else
    return -1.;
}

double getTauExtIso(const pat::Tau& patTau, const pat::IsolationKeys& pfIsoType, 
		  const std::vector<reco::isodeposit::AbsVeto*>& tauIsolationVetos)
{
  const pat::IsoDeposit* tauIsoDeposit = patTau.isoDeposit(pfIsoType);
  if ( tauIsoDeposit ) 
    return tauIsoDeposit->depositWithin(1.0, tauIsolationVetos);
  else 
    return -1.;
}

double computeDiTauProj(const reco::Particle::LorentzVector& vProj, const reco::Particle::LorentzVector& vRef)
{
  if ( vRef.pt() > 0. ) {
    double scalarProduct = vProj.px()*vRef.px() + vProj.py()*vRef.py();
    return (scalarProduct/vRef.pt());
  } else {
    return 0.;
  }
}

void TauIdEffZtoMuTauHistManager::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{  
  //std::cout << "<TauIdEffZtoMuTauHistManager::fillHistogramsImp>:" << std::endl; 

  edm::Handle<pat::MuonCollection> patMuons;
  evt.getByLabel(muonSrc_, patMuons);
  
  edm::Handle<pat::TauCollection> patTaus;
  evt.getByLabel(tauSrc_, patTaus);

  edm::Handle<PATMuTauPairCollection> diTaus;
  evt.getByLabel(diTauSrc_, diTaus);
  
  edm::Handle<pat::JetCollection> patCentralJets;
  evt.getByLabel(centralJetSrc_, patCentralJets);

  for ( std::vector<pat::Muon>::const_iterator patMuon = patMuons->begin(); 
	patMuon != patMuons->end(); ++patMuon ) {
    hMuonPt_->Fill(patMuon->pt(), evtWeight); 
    hMuonAbsEta_->Fill(TMath::Abs(patMuon->eta()), evtWeight); 
    hMuonPtVsAbsEta_->Fill(TMath::Abs(patMuon->eta()), patMuon->pt(), evtWeight); 

    hMuonExtTrkIsoPt_->Fill(getMuonExtIso(*patMuon, pat::TrackerIso), evtWeight);
    hMuonExtEcalIsoPt_->Fill(getMuonExtIso(*patMuon, pat::ECalIso), evtWeight);
    hMuonExtHcalIsoPt_->Fill(getMuonExtIso(*patMuon, pat::HCalIso), evtWeight);
    double muonExtIsoSum = getMuonExtIso(*patMuon, pat::TrackerIso) + getMuonExtIso(*patMuon, pat::ECalIso) + getMuonExtIso(*patMuon, pat::HCalIso);
    hMuonExtIsoSumPt_->Fill(muonExtIsoSum, evtWeight);
  }
  
  for ( std::vector<pat::Tau>::const_iterator patTau = patTaus->begin(); 
	patTau != patTaus->end(); ++patTau ) {
    hTauDiscriminatorByEwkTauId_->Fill(patTau->tauID(tauIdDiscriminator_), evtWeight);

    hTauExtParticleFlowIsoPt_->Fill(getTauExtIso(*patTau, pat::ParticleIso, tauIsolationVetos_), evtWeight);
    hTauExtPFChargedHadronIsoPt_->Fill(getTauExtIso(*patTau, pat::ChargedHadronIso, tauIsolationVetos_), evtWeight);
    hTauExtPFNeutralHadronIsoPt_->Fill(getTauExtIso(*patTau, pat::NeutralHadronIso, tauIsolationVetos_), evtWeight);
    hTauExtPFGammaIsoPt_->Fill(getTauExtIso(*patTau, pat::PhotonIso, tauIsolationVetos_), evtWeight);
  }

  double diTauChargeSign = (*diTauChargeSignExtractor_)(evt);
  hDiTauChargeSign_->Fill(diTauChargeSign, evtWeight);
  
  for ( PATMuTauPairCollection::const_iterator diTau = diTaus->begin(); 
	diTau != diTaus->end(); ++diTau ) {
    double diTauPtProj = computeDiTauProj(diTau->p4Vis(), diTau->leg1()->p4());
    hDiTauPtProj_->Fill(diTauPtProj, evtWeight);

    double diTauMEtProj = computeDiTauProj(diTau->met()->p4(), diTau->leg1()->p4());
    hDiTauMEtProj_->Fill(diTauMEtProj, evtWeight);
  }

  hNumCentralJets_->Fill(patCentralJets->size(), evtWeight);

  for ( std::vector<pat::Jet>::const_iterator patCentralJet = patCentralJets->begin(); 
	patCentralJet != patCentralJets->end(); ++patCentralJet ) {
    hCentralJetPt_->Fill(patCentralJet->pt(), evtWeight);
    hCentralJetEta_->Fill(patCentralJet->eta(), evtWeight);
    hCentralJetPhi_->Fill(patCentralJet->phi(), evtWeight);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, TauIdEffZtoMuTauHistManager, "TauIdEffZtoMuTauHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, TauIdEffZtoMuTauHistManager, "TauIdEffZtoMuTauHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<TauIdEffZtoMuTauHistManager> TauIdEffZtoMuTauAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(TauIdEffZtoMuTauAnalyzer);
