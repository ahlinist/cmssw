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
#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"

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
  hDiTauVisMassFromJetP4_ = book1D("DiTauVisMassFromJetP4", "DiTauVisMassFromJetP4", 40, 0., 200.);
  hDiTauVisMassFromTauJetP4_ = book1D("DiTauVisMassFromTauJetP4", "DiTauVisMassFromTauJetP4", 40, 0., 200.);
  hDiTauCollinearApproxMassFromJetP4_ = book1D("DiTauCollinearApproxMassFromJetP4", "DiTauCollinearApproxMassFromJetP4", 50, 0., 250.);
  hDiTauCollinearApproxMassFromTauJetP4_ = book1D("DiTauCollinearApproxMassFromTauJetP4", "DiTauCollinearApproxMassFromTauJetP4", 50, 0., 250.);

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

void fillCollinearApproxMassHistogram(MonitorElement* h, 
				      const reco::Candidate::LorentzVector& leg1, 
				      const reco::Candidate::LorentzVector& leg2,
				      double metPx, double metPy, 
				      double weight)
{
  double x1_numerator = leg1.px()*leg2.py() - leg2.px()*leg1.py();
  double x1_denominator = leg2.py()*(leg1.px() + metPx) - leg2.px()*(leg1.py() + metPy);
  double x1 = ( x1_denominator != 0. ) ? x1_numerator/x1_denominator : -1.;
  bool isX1withinPhysRange = true;
  double x1phys = getPhysX(x1, isX1withinPhysRange);
  
  double x2_numerator = x1_numerator;
  double x2_denominator = leg1.px()*(leg2.py() + metPy) - leg1.py()*(leg2.px() + metPx);
  double x2 = ( x2_denominator != 0. ) ? x2_numerator/x2_denominator : -1.;
  bool isX2withinPhysRange = true;
  double x2phys = getPhysX(x2, isX2withinPhysRange);
  
  if ( x1phys != 0. && x2phys != 0. ) {
    reco::Candidate::LorentzVector p4 = leg1/x1phys + leg2/x2phys;
    h->Fill(p4.mass(), weight);
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

    hMuonExtTrkIsoPt_->Fill(getMuonExtIso(*patMuon, pat::TrackIso), evtWeight);
    hMuonExtEcalIsoPt_->Fill(getMuonExtIso(*patMuon, pat::EcalIso), evtWeight);
    hMuonExtHcalIsoPt_->Fill(getMuonExtIso(*patMuon, pat::HcalIso), evtWeight);
    double muonExtIsoSum = getMuonExtIso(*patMuon, pat::TrackIso) + getMuonExtIso(*patMuon, pat::EcalIso) + getMuonExtIso(*patMuon, pat::HcalIso);
    hMuonExtIsoSumPt_->Fill(muonExtIsoSum, evtWeight);
  }
  
  for ( std::vector<pat::Tau>::const_iterator patTau = patTaus->begin(); 
	patTau != patTaus->end(); ++patTau ) {
    hTauDiscriminatorByEwkTauId_->Fill(patTau->tauID(tauIdDiscriminator_), evtWeight);

    hTauExtParticleFlowIsoPt_->Fill(getTauExtIso(*patTau, pat::PfAllParticleIso, tauIsolationVetos_), evtWeight);
    hTauExtPFChargedHadronIsoPt_->Fill(getTauExtIso(*patTau, pat::PfChargedHadronIso, tauIsolationVetos_), evtWeight);
    hTauExtPFNeutralHadronIsoPt_->Fill(getTauExtIso(*patTau, pat::PfNeutralHadronIso, tauIsolationVetos_), evtWeight);
    hTauExtPFGammaIsoPt_->Fill(getTauExtIso(*patTau, pat::PfGammaIso, tauIsolationVetos_), evtWeight);
  }

  double diTauChargeSign = (*diTauChargeSignExtractor_)(evt);
  hDiTauChargeSign_->Fill(diTauChargeSign, evtWeight);
  
  for ( PATMuTauPairCollection::const_iterator diTau = diTaus->begin(); 
	diTau != diTaus->end(); ++diTau ) {
    double diTauPtProj = computeDiTauProj(diTau->p4Vis(), diTau->leg1()->p4());
    hDiTauPtProj_->Fill(diTauPtProj, evtWeight);

    double diTauMEtProj = computeDiTauProj(diTau->met()->p4(), diTau->leg1()->p4());
    hDiTauMEtProj_->Fill(diTauMEtProj, evtWeight);

    hDiTauVisMassFromJetP4_->Fill((diTau->leg1()->p4() + diTau->leg2()->pfTauTagInfoRef()->pfjetRef()->p4()).mass(), evtWeight);
    hDiTauVisMassFromTauJetP4_->Fill((diTau->leg1()->p4() + diTau->leg2()->p4()).mass(), evtWeight);
    //if ( (diTau->dPhi12()*180./TMath::Pi()) < 160. ) {
      fillCollinearApproxMassHistogram(hDiTauCollinearApproxMassFromJetP4_,
				       diTau->leg1()->p4(), diTau->leg2()->pfTauTagInfoRef()->pfjetRef()->p4(),
				       diTau->met()->px(), diTau->met()->py(), evtWeight);
      fillCollinearApproxMassHistogram(hDiTauCollinearApproxMassFromTauJetP4_,
				       diTau->leg1()->p4(), diTau->leg2()->p4(),
				       diTau->met()->px(), diTau->met()->py(), evtWeight);
    //}
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

DEFINE_FWK_MODULE(TauIdEffZtoMuTauAnalyzer);
