#include "TauAnalysis/Core/plugins/ElectronHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/IsolationAlgos/interface/IsoDepositVetoFactory.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

#include <stdlib.h>

bool matchesGenElectron(const pat::Electron& patElectron)
{
  //std::cout << "<matchesGenElectron>:" << std::endl;

  bool isGenElectronMatched = false;
  std::vector<reco::GenParticleRef> associatedGenParticles = patElectron.genParticleRefs();
  for ( std::vector<reco::GenParticleRef>::const_iterator it = associatedGenParticles.begin(); 
	it != associatedGenParticles.end(); ++it ) {
    if ( it->isAvailable() ) {
      const reco::GenParticleRef& genParticle = (*it);
      if ( genParticle->pdgId() == -11 || genParticle->pdgId() == +11 ) isGenElectronMatched = true;
    } else {
      edm::LogWarning("matchesGenElectron") << " edm::Ref of genParticle associated to pat::Electron is invalid !!";
    }
  }
  return isGenElectronMatched;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

ElectronHistManager::ElectronHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<ElectronHistManager::ElectronHistManager>:" << std::endl;

  electronSrc_ = cfg.getParameter<edm::InputTag>("electronSource");
  //std::cout << " electronSrc = " << electronSrc_ << std::endl;

  vertexSrc_ = ( cfg.exists("vertexSource") ) ? cfg.getParameter<edm::InputTag>("vertexSource") : edm::InputTag();
  if ( vertexSrc_.label() == "" ) {
    edm::LogWarning("ElectronHistManager") << " Configuration parameter 'vertexSource' not specified" 
					   << " --> Impact Parameter histograms will NOT be plotted !!";
  }
  //std::cout << " vertexSrc = " << vertexSrc_ << std::endl;

  jetSrc_ = cfg.getParameter<edm::InputTag>("jetSource");
  //std::cout << " jetSrc = " << jetSrc_ << std::endl;

  genParticleSrc_ = ( cfg.exists("genParticleSource") ) ? cfg.getParameter<edm::InputTag>("genParticleSource") : edm::InputTag();
  if ( genParticleSrc_.label() == "" ) {
    edm::LogWarning("ElectronHistManager") << " Configuration parameter 'genParticleSource' not specified" 
					   << " --> matching gen. Particle PdgId histogram will NOT be plotted !!";
  }
  //std::cout << " genParticleSrc = " << genParticleSrc_ << std::endl;

  requireGenElectronMatch_ = cfg.getParameter<bool>("requireGenElectronMatch");
  //std::cout << " requireGenElectronMatch = " << requireGenElectronMatch_ << std::endl;

  std::string normalization_string = cfg.getParameter<std::string>("normalization");
  normMethod_ = getNormMethod(normalization_string, "electrons");

  makeIsoPtCtrlHistograms_ = ( cfg.exists("makeIsoPtCtrlHistograms") ) ? 
    cfg.getParameter<bool>("makeIsoPtCtrlHistograms") : false;

  makeIsoPtConeSizeDepHistograms_ = ( cfg.exists("makeIsoPtConeSizeDepHistograms") ) ? 
    cfg.getParameter<bool>("makeIsoPtConeSizeDepHistograms") : false;

  numElectronIsoConeSizes_ = 15;
  electronIsoConeSizeIncr_ = 0.1;
  numElectronIsoPtThresholds_ = 4;
  electronIsoPtThresholdIncr_ = 0.5;

  // electrons in crack region included in histograms
  //electronEtaMaxBarrel = 1.479;
  //electronEtaMinEndcap = electronEtaMaxBarrel;

  // electrons in crack region excluded from histograms
  electronEtaMaxBarrel_ = 1.442;
  electronEtaMinEndcap_ = 1.560;

//--- create "veto" objects for computation of IsoDeposit sums
  electronTrkIsoParam_.push_back(IsoDepositVetoFactory::make("0.02"));
  electronTrkIsoParam_.push_back(IsoDepositVetoFactory::make("Threshold(1.0)"));

  electronEcalIsoParam_.push_back(IsoDepositVetoFactory::make("0.0"));
  electronEcalIsoParam_.push_back(IsoDepositVetoFactory::make("Threshold(0.0)"));

  electronHcalIsoParam_.push_back(IsoDepositVetoFactory::make("0.0"));
  electronHcalIsoParam_.push_back(IsoDepositVetoFactory::make("Threshold(0.0)"));

  electronParticleFlowIsoParam_.push_back(IsoDepositVetoFactory::make("0.0"));
  electronParticleFlowIsoParam_.push_back(IsoDepositVetoFactory::make("Threshold(0.5)"));
}

ElectronHistManager::~ElectronHistManager()
{
//--- delete "veto" objects for computation of IsoDeposit sums
  clearIsoParam(electronTrkIsoParam_);
  clearIsoParam(electronEcalIsoParam_);
  clearIsoParam(electronHcalIsoParam_);
  clearIsoParam(electronParticleFlowIsoParam_);
}

void ElectronHistManager::bookHistogramsImp()
{
  //std::cout << "<ElectronHistManager::bookHistogramsImp>:" << std::endl;

//--- book histogram for number of electrons in each event
  hNumElectrons_ = book1D("NumElectrons", "Electron Count", 10, -0.5, 9.5);

//--- book histograms for Pt, eta and phi distributions
//    of electrons passing all id. and isolation selections
<<<<<<< ElectronHistManager.cc
  bookElectronHistograms(hElectronPt_, hElectronEta_, hElectronPhi_, "Electron");
  hElectronPtVsEta_ = book2D("ElectronPtVsEta", "Electron #eta vs P_{T}", 24, -3., +3., 30, 0., 150.);
  hElectronCharge_ = book1D("ElectronCharge", "Electron Charge", 3, -1.5, +1.5);
  
  bookWeightHistograms(*dqmStore_, "ElectronWeight", "Electron Weight", 
		       hElectronWeightPosUnweighted_, hElectronWeightPosWeighted_, 
		       hElectronWeightNegUnweighted_, hElectronWeightNegWeighted_);

  hElectronEnCompToGen_ = book1D("ElectronEnCompToGen", "Electron RECO-GEN #Delta E", 100, -0.50, +0.50);
  hElectronThetaCompToGen_ = book1D("ElectronThetaCompToGen", "Electron RECO-GEN #Delta#theta", 200, -0.010, +0.010);
  hElectronPhiCompToGen_ = book1D("ElectronPhiCompToGen", "Electron RECO-GEN #Delta#phi", 200, -0.010, +0.010);
  
  hElectronMatchingGenParticlePdgId_ = book1D("ElectronMatchingGenParticlePdgId", "matching gen. Particle PdgId", 26, -1.5, 24.5);

  hElectronTrackPt_ = book1D("ElectronTrackPt", "Electron Track P_{T}", 75, 0., 150.);
  hElectronTrackIPxy_ = book1D("ElectronTrackIPxy", "Electron Track Impact Parameter (xy)", 100, -0.100, 0.100);
  hElectronTrackIPz_ = book1D("ElectronTrackIPz", "Electron Track Impact Parameter (z)", 100, -1.0, 1.0);
  
  hElectronSuperclEnOverTrackMomBarrel_ = book1D("ElectronSuperclEnOverTrackMomBarrel", "Electron (Barrel) #frac{E_{Supercluster}}{P_{Track}}", 50, 0., 5.);
  hElectronSuperclEnOverTrackMomEndcap_ = book1D("ElectronSuperclEnOverTrackMomEndcap", "Electron (Endcap) #frac{E_{Supercluster}}{P_{Track}}", 50, 0., 5.);
  hElectronHadEnOverEmEn_ = book1D("ElectronHadEnOverEmEn", "Electron #frac{E_{had}}{E_{em}}", 102, -0.01, 1.01);
  hElectronSuperclShapeSigmaEtaEta_ = book1D("ElectronSuperclShapeSigmaEtaEta", "Electron #sigma_{#eta#eta}^{Supercluster}", 102, -0.001, 0.101);
  hElectronDeltaPhiSuperclToExtrapolTrack_ = book1D("ElectronDeltaPhiSuperclToExtrapolTrack", "Electron #Delta#phi_{in}", 22, -0.001, 0.021);
  hElectronDeltaEtaSuperclToExtrapolTrack_ = book1D("ElectronDeltaEtaSuperclToExtrapolTrack", "Electron #Delta#eta_{in}", 102, -0.001, 0.101);
  hElectronBremsFraction_ = book1D("ElectronBremsFraction", "Electron f_{Brems}", 80, -2., +2.);
  
  hElectronIdRobust_ = book1D("ElectronIdRobust", "Electron Id (Robust)", 2, -0.5, 1.5);
  
  hElectronTrkIsoPt_ = book1D("ElectronTrkIsoPt", "Electron Track Isolation P_{T}", 100, 0., 20.);    
  hElectronTrkIsoPtVsElectronPt_ = book2D("ElectronTrkIsoPtVsElectronPt", "Electron Track Isolation P_{T} vs Electron P_{T}", 20, 0., 100., 20, 0., 10.);    
  hElectronEcalIsoPt_ = book1D("ElectronEcalIsoPt", "Electron ECAL Isolation P_{T}", 100, 0., 20.);
  hElectronEcalIsoPtBarrel_ = book1D("ElectronEcalIsoPtBarrel", "Electron (Barrel) ECAL Isolation P_{T}", 100, 0., 20.);
  hElectronEcalIsoPtEndcap_ = book1D("ElectronEcalIsoPtEndcap", "Electron (Endcap) ECAL Isolation P_{T}", 100, 0., 20.);
  hElectronEcalIsoPtVsElectronPt_ = book2D("ElectronTrkIsoPtVsElectronPt", "Electron ECAL Isolation P_{T} vs Electron P_{T}", 20, 0., 100., 20, 0., 10.);   
  hElectronHcalIsoPt_ = book1D("ElectronHcalIsoPt", "Electron HCAL Isolation P_{T}", 100, 0., 20.);
  hElectronIsoSumPt_ = book1D("ElectronIsoSumPt", "Electron Isolation Sum(P_{T})", 100, 0., 20.);
  hElectronIsoSumPtVsElectronPt_ = book2D("hElectronIsoSumPtVsElectronPt", "Electron Isolation Sum(P_{T}) vs Electron P_{T}", 20, 0., 100., 20, 0., 10.);   
  hElectronTrkIsoPtRel_ = book1D("ElectronTrkIsoPtRel", "ElectronTrkIsoPtRel", 200, 0., 2.);    
  hElectronEcalIsoPtRel_ = book1D("ElectronEcalIsoPtRel", "ElectronEcalIsoPtRel", 200, 0., 2.);
  hElectronEcalIsoPtBarrelRel_ = book1D("ElectronEcalIsoPtBarrelRel", "Electron (Barrel) #frac{P_{T ECAL isolation}}{P_{T track}}", 200, 0., 2.);
  hElectronEcalIsoPtEndcapRel_ = book1D("ElectronEcalIsoPtEndcapRel", "Electron (Endcap) #frac{P_{T ECAL isolation}}{P_{T track}}", 200, 0., 2.);
  hElectronHcalIsoPtRel_ = book1D("ElectronHcalIsoPtRel", "Electron #frac{P_{T HCAL isolation}}{P_{T track}}", 200, 0., 2.);
  hElectronIsoSumPtRel_ = book1D("ElectronIsoSumPtRel", "Electron #frac{#sum P_{T isolation}}{P_{T track}}", 200, 0., 2.);
  
  hElectronDeltaRnearestJet_ = book1D("ElectronDeltaRnearestJet", "#DeltaR(nearest Jet)", 102, -0.1, 10.1);

  hElectronParticleFlowIsoPt_ = book1D("ElectronParticleFlowIsoPt", "Electron Particle Flow Isolation P_{T}", 100, 0., 20.);    
  hElectronPFChargedHadronIsoPt_ = book1D("ElectronPFChargedHadronIsoPt", "Electron Particle Flow (Charged Hadrons) Isolation P_{T}", 100, 0., 20.);   
  hElectronPFNeutralHadronIsoPt_ = book1D("ElectronPFNeutralHadronIsoPt", "Electron Particle Flow (Neutral Hadrons) Isolation P_{T}", 100, 0., 20.);   
  hElectronPFGammaIsoPt_ = book1D("ElectronPFGammaIsoPt", "Electron Particle Flow (Photons) Isolation P_{T}", 100, 0., 20.);  
=======
  bookElectronHistograms(dqmStore, hElectronPt_, hElectronEta_, hElectronPhi_, "Electron");
  hElectronPtVsEta_ = dqmStore.book2D("ElectronPtVsEta", "Electron #eta vs P_{T}", 24, -3., +3., 30, 0., 150.);
  hElectronCharge_ = dqmStore.book1D("ElectronCharge", "Electron Charge", 3, -1.5, +1.5);
  
  hElectronEnCompToGen_ = dqmStore.book1D("ElectronEnCompToGen", "Electron RECO-GEN #Delta E", 100, -0.50, +0.50);
  hElectronThetaCompToGen_ = dqmStore.book1D("ElectronThetaCompToGen", "Electron RECO-GEN #Delta#theta", 200, -0.010, +0.010);
  hElectronPhiCompToGen_ = dqmStore.book1D("ElectronPhiCompToGen", "Electron RECO-GEN #Delta#phi", 200, -0.010, +0.010);
  
  hElectronMatchingGenParticlePdgId_ = dqmStore.book1D("ElectronMatchingGenParticlePdgId", "matching gen. Particle PdgId", 26, -1.5, 24.5);

  hElectronTrackPt_ = dqmStore.book1D("ElectronTrackPt", "Electron Track P_{T}", 75, 0., 150.);
  hElectronTrackIPxy_ = dqmStore.book1D("ElectronTrackIPxy", "Electron Track Impact Parameter (xy)", 100, -0.100, 0.100);
  hElectronTrackIPz_ = dqmStore.book1D("ElectronTrackIPz", "Electron Track Impact Parameter (z)", 100, -1.0, 1.0);
  
  hElectronSuperclEnOverTrackMomBarrel_ = dqmStore.book1D("ElectronSuperclEnOverTrackMomBarrel", "Electron (Barrel) #frac{E_{Supercluster}}{P_{Track}}", 50, 0., 5.);
  hElectronSuperclEnOverTrackMomEndcap_ = dqmStore.book1D("ElectronSuperclEnOverTrackMomEndcap", "Electron (Endcap) #frac{E_{Supercluster}}{P_{Track}}", 50, 0., 5.);
  hElectronSeedClEnOverTrackMomBarrel_ = dqmStore.book1D("ElectronSeedClEnOverTrackMomBarrel", "Electron (Barrel) #frac{E_{Seed-Cluster}}{P_{Track}}", 50, 0., 5.);
  hElectronSeedClEnOverTrackMomEndcap_ = dqmStore.book1D("ElectronSeedClEnOverTrackMomEndcap", "Electron (Endcap) #frac{E_{Seed-Cluster}}{P_{Track}}", 50, 0., 5.);
  hElectronHadEnOverEmEn_ = dqmStore.book1D("ElectronHadEnOverEmEn", "Electron #frac{E_{had}}{E_{em}}", 102, -0.01, 1.01);
  hElectronSuperclShapeSigmaEtaEta_ = dqmStore.book1D("ElectronSuperclShapeSigmaEtaEta", "Electron #sigma_{#eta#eta}^{Supercluster}", 102, -0.001, 0.101);
  hElectronDeltaPhiSuperclToExtrapolTrack_ = dqmStore.book1D("ElectronDeltaPhiSuperclToExtrapolTrack", "Electron #Delta#phi_{in}", 22, -0.001, 0.021);
  hElectronDeltaEtaSuperclToExtrapolTrack_ = dqmStore.book1D("ElectronDeltaEtaSuperclToExtrapolTrack", "Electron #Delta#eta_{in}", 102, -0.001, 0.101);
  hElectronBremsFraction_ = dqmStore.book1D("ElectronBremsFraction", "Electron f_{Brems}", 80, -2., +2.);
  
  hElectronTrkIsoPt_ = dqmStore.book1D("ElectronTrkIsoPt", "Electron Track Isolation P_{T}", 100, 0., 20.);    
  hElectronTrkIsoPtVsElectronPt_ = dqmStore.book2D("ElectronTrkIsoPtVsElectronPt", "Electron Track Isolation P_{T} vs Electron P_{T}", 20, 0., 100., 20, 0., 10.);    
  hElectronEcalIsoPt_ = dqmStore.book1D("ElectronEcalIsoPt", "Electron ECAL Isolation P_{T}", 100, 0., 20.);
  hElectronEcalIsoPtBarrel_ = dqmStore.book1D("ElectronEcalIsoPtBarrel", "Electron (Barrel) ECAL Isolation P_{T}", 100, 0., 20.);
  hElectronEcalIsoPtEndcap_ = dqmStore.book1D("ElectronEcalIsoPtEndcap", "Electron (Endcap) ECAL Isolation P_{T}", 100, 0., 20.);
  hElectronEcalIsoPtVsElectronPt_ = dqmStore.book2D("ElectronTrkIsoPtVsElectronPt", "Electron ECAL Isolation P_{T} vs Electron P_{T}", 20, 0., 100., 20, 0., 10.);   
  hElectronHcalIsoPt_ = dqmStore.book1D("ElectronHcalIsoPt", "Electron HCAL Isolation P_{T}", 100, 0., 20.);
  hElectronIsoSumPt_ = dqmStore.book1D("ElectronIsoSumPt", "Electron Isolation Sum(P_{T})", 100, 0., 20.);
  hElectronIsoSumPtVsElectronPt_ = dqmStore.book2D("hElectronIsoSumPtVsElectronPt", "Electron Isolation Sum(P_{T}) vs Electron P_{T}", 20, 0., 100., 20, 0., 10.);   
  hElectronTrkIsoPtRel_ = dqmStore.book1D("ElectronTrkIsoPtRel", "ElectronTrkIsoPtRel", 200, 0., 2.);    
  hElectronEcalIsoPtRel_ = dqmStore.book1D("ElectronEcalIsoPtRel", "ElectronEcalIsoPtRel", 200, 0., 2.);
  hElectronEcalIsoPtBarrelRel_ = dqmStore.book1D("ElectronEcalIsoPtBarrelRel", "Electron (Barrel) #frac{P_{T ECAL isolation}}{P_{T track}}", 200, 0., 2.);
  hElectronEcalIsoPtEndcapRel_ = dqmStore.book1D("ElectronEcalIsoPtEndcapRel", "Electron (Endcap) #frac{P_{T ECAL isolation}}{P_{T track}}", 200, 0., 2.);
  hElectronHcalIsoPtRel_ = dqmStore.book1D("ElectronHcalIsoPtRel", "Electron #frac{P_{T HCAL isolation}}{P_{T track}}", 200, 0., 2.);
  hElectronIsoSumPtRel_ = dqmStore.book1D("ElectronIsoSumPtRel", "Electron #frac{#sum P_{T isolation}}{P_{T track}}", 200, 0., 2.);
  
  hElectronDeltaRnearestJet_ = dqmStore.book1D("ElectronDeltaRnearestJet", "#DeltaR(nearest Jet)", 102, -0.1, 10.1);

  hElectronParticleFlowIsoPt_ = dqmStore.book1D("ElectronParticleFlowIsoPt", "Electron Particle Flow Isolation P_{T}", 100, 0., 20.);    
  hElectronPFChargedHadronIsoPt_ = dqmStore.book1D("ElectronPFChargedHadronIsoPt", "Electron Particle Flow (Charged Hadrons) Isolation P_{T}", 100, 0., 20.);   
  hElectronPFNeutralHadronIsoPt_ = dqmStore.book1D("ElectronPFNeutralHadronIsoPt", "Electron Particle Flow (Neutral Hadrons) Isolation P_{T}", 100, 0., 20.);   
  hElectronPFGammaIsoPt_ = dqmStore.book1D("ElectronPFGammaIsoPt", "Electron Particle Flow (Photons) Isolation P_{T}", 100, 0., 20.);  
>>>>>>> 1.22
  
//--- book "control" histograms to check agreement between electron isolation variables
//    computed by PAT-level IsoDeposits based on particle flow 
//    with values computed on AOD level, based on ECAL recHits and reco::Tracks
  if ( makeIsoPtCtrlHistograms_ ) {
    hElectronPFChargedHadronIsoPtCtrl_ = book2D("ElectronPFChargedHadronIsoPtCtrl", "PFChargedHadron vs. reco::Track Isolation P_{T}", 40, 0., 20., 40, 0., 20.);
    hElectronPFGammaIsoPtCtrl_ = book2D("ElectronPFGammaIsoPtCtrl", "PFGamma vs. ECAL recHit Isolation P_{T}", 40, 0., 20., 40, 0., 20.);
  } else {
    hElectronPFChargedHadronIsoPtCtrl_ = 0;
    hElectronPFGammaIsoPtCtrl_ = 0;
  } 

  hElectronTrkIsoValProfile_ = book1D("ElectronTrkIsoValProfile", "ElectronTrkIsoValProfile", 100, 0., 10.);
  hElectronTrkIsoEtaDistProfile_ = book1D("ElectronTrkIsoEtaDistProfile", "ElectronTrkIsoEtaDistProfile", 15, 0., 1.5);
  hElectronTrkIsoPhiDistProfile_ = book1D("ElectronTrkIsoPhiDistProfile", "ElectronTrkIsoPhiDistProfile", 15, 0., 1.5);
  
  hElectronEcalIsoValProfile_ = book1D("ElectronEcalIsoValProfile", "ElectronEcalIsoValProfile", 100, 0., 10.);
  hElectronEcalIsoEtaDistProfile_ = book1D("ElectronEcalIsoEtaDistProfile", "ElectronEcalIsoEtaDistProfile", 15, 0., 1.5);
  hElectronEcalIsoPhiDistProfile_ = book1D("ElectronEcalIsoPhiDistProfile", "ElectronEcalIsoPhiDistProfile", 15, 0., 1.5);
  
  hElectronHcalIsoValProfile_ = book1D("ElectronHcalIsoValProfile", "ElectronHcalIsoValProfile", 100, 0., 10.);
  hElectronHcalIsoEtaDistProfile_ = book1D("ElectronHcalIsoEtaDistProfile", "ElectronHcalIsoEtaDistProfile", 15, 0., 1.5);
  hElectronHcalIsoPhiDistProfile_  = book1D("ElectronHcalIsoPhiDistProfile", "ElectronHcalIsoPhiDistProfile", 15, 0., 1.5);
  
  if ( makeIsoPtConeSizeDepHistograms_ ) bookElectronIsoConeSizeDepHistograms();
}

double ElectronHistManager::getElectronWeight(const pat::Electron& patElectron)
{
  return 1.;
}

void ElectronHistManager::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{  
  //std::cout << "<ElectronHistManager::fillHistogramsImp>:" << std::endl; 

  edm::Handle<pat::ElectronCollection> patElectrons;
  evt.getByLabel(electronSrc_, patElectrons);

  edm::Handle<pat::JetCollection> patJets;
  evt.getByLabel(jetSrc_, patJets);

  edm::Handle<reco::GenParticleCollection> genParticles;
  evt.getByLabel(genParticleSrc_, genParticles);

  //std::cout << " patElectrons.size = " << patElectrons->size() << std::endl;
  hNumElectrons_->Fill(patElectrons->size(), evtWeight);

  double electronWeightSum = 0.;
  for ( std::vector<pat::Electron>::const_iterator patElectron = patElectrons->begin(); 
	patElectron != patElectrons->end(); ++patElectron ) {
    if ( requireGenElectronMatch_ && !matchesGenElectron(*patElectron) ) continue;

    electronWeightSum += getElectronWeight(*patElectron);
  }

  for ( std::vector<pat::Electron>::const_iterator patElectron = patElectrons->begin(); 
	patElectron != patElectrons->end(); ++patElectron ) {
  
    //bool isGenElectronMatched = matchesGenElectron(*patElectron);
    //std::cout << " Pt = " << patElectron->pt() << ", eta = " << patElectron->eta() << ", phi = " << patElectron->phi() << std::endl;
    //std::cout << " isGenElectronMatched = " << isGenElectronMatched << std::endl;
    
    if ( requireGenElectronMatch_ && !matchesGenElectron(*patElectron) ) continue;
    
    double electronWeight = getElectronWeight(*patElectron);
    double weight = getWeight(evtWeight, electronWeight, electronWeightSum);

    fillElectronHistograms(*patElectron, hElectronPt_, hElectronEta_, hElectronPhi_, weight);
    hElectronPtVsEta_->Fill(patElectron->eta(), patElectron->pt(), weight);
    hElectronCharge_->Fill(patElectron->charge(), weight);

    fillWeightHistograms(hElectronWeightPosUnweighted_, hElectronWeightPosWeighted_, 
			 hElectronWeightNegUnweighted_, hElectronWeightNegWeighted_, electronWeight);

//--- compare reconstructed electron to generator level one;
//    normalize difference between reconstructed and generated energy
//    to expected energy dependence of resolution
    if ( patElectron->genLepton() ) {
      hElectronEnCompToGen_->Fill((patElectron->energy() - patElectron->genLepton()->energy())
				  /TMath::Sqrt(patElectron->genLepton()->energy()), weight);
      hElectronThetaCompToGen_->Fill(patElectron->theta() - patElectron->genLepton()->theta(), weight);
      hElectronPhiCompToGen_->Fill(patElectron->phi() - patElectron->genLepton()->phi(), weight);
    }

    int matchingGenParticlePdgId = getMatchingGenParticlePdgId(patElectron->p4(), genParticles);
    if ( matchingGenParticlePdgId == -1 ) {
      hElectronMatchingGenParticlePdgId_->Fill(-1, weight);
    } else if ( abs(matchingGenParticlePdgId) > 22 ) {
      hElectronMatchingGenParticlePdgId_->Fill(24, weight);
    } else {
      hElectronMatchingGenParticlePdgId_->Fill(abs(matchingGenParticlePdgId), weight);
    }

    if ( patElectron->gsfTrack().isAvailable() && !patElectron->gsfTrack().isNull() ) {
      hElectronTrackPt_->Fill(patElectron->gsfTrack()->pt(), weight);
      if ( vertexSrc_.label() != "" ) {
	edm::Handle<std::vector<reco::Vertex> > recoVertices;
	evt.getByLabel(vertexSrc_, recoVertices);
	if ( recoVertices->size() >= 1 ) {
	  const reco::Vertex& thePrimaryEventVertex = (*recoVertices->begin());
	  hElectronTrackIPxy_->Fill(patElectron->gsfTrack()->dxy(thePrimaryEventVertex.position()), weight);
	  hElectronTrackIPz_->Fill(patElectron->gsfTrack()->dz(thePrimaryEventVertex.position()), weight);
	}
      }
    }

    if ( TMath::Abs(patElectron->eta()) < electronEtaMaxBarrel_ ) {
      hElectronSuperclEnOverTrackMomBarrel_->Fill(patElectron->eSuperClusterOverP(), weight);
      hElectronSeedClEnOverTrackMomBarrel_->Fill(patElectron->eSeedClusterOverP(), weight);
    }
    if ( TMath::Abs(patElectron->eta()) > electronEtaMinEndcap_ ) {
      hElectronSuperclEnOverTrackMomEndcap_->Fill(patElectron->eSuperClusterOverP(), weight);
      hElectronSeedClEnOverTrackMomEndcap_->Fill(patElectron->eSeedClusterOverP(), weight); 
    }
    hElectronHadEnOverEmEn_->Fill(patElectron->hcalOverEcal(), weight); 
    hElectronSuperclShapeSigmaEtaEta_->Fill(patElectron->sigmaEtaEta(), weight); 
    hElectronDeltaPhiSuperclToExtrapolTrack_->Fill(patElectron->deltaPhiSuperClusterTrackAtVtx(), weight);
    hElectronDeltaEtaSuperclToExtrapolTrack_->Fill(patElectron->deltaEtaSuperClusterTrackAtVtx(), weight);
    if ( patElectron->gsfTrack().isAvailable() ) {
      double pIn  = patElectron->trackMomentumAtVtx().R(); 
      double pOut = patElectron->trackMomentumOut().R(); 
      if ( pIn > 0. ) hElectronBremsFraction_->Fill((pIn - pOut)/pIn, weight);
    }

    fillElectronIsoHistograms(*patElectron, weight);
    hElectronDeltaRnearestJet_->Fill(getDeltaRnearestJet(patElectron->p4(), patJets), weight);
    if ( makeIsoPtConeSizeDepHistograms_ ) fillElectronIsoConeSizeDepHistograms(*patElectron, weight);
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void ElectronHistManager::bookElectronHistograms(MonitorElement*& hElectronPt, MonitorElement*& hElectronEta, MonitorElement*& hElectronPhi, const char* histoSetName)
{
  std::string hElectronPtName = std::string(histoSetName).append("Pt");
  hElectronPt = book1D(hElectronPtName, hElectronPtName, 75, 0., 150.);
  
  std::string hElectronEtaName = std::string(histoSetName).append("Eta");
  hElectronEta = book1D(hElectronEtaName, hElectronEtaName, 60, -3., +3.);
  
  std::string hElectronPhiName = std::string(histoSetName).append("Phi");
  hElectronPhi = book1D(hElectronPhiName, hElectronPhiName, 36, -TMath::Pi(), +TMath::Pi());
}

void ElectronHistManager::bookElectronIsoConeSizeDepHistograms()
{
  for ( unsigned iConeSize = 1; iConeSize <= numElectronIsoConeSizes_; ++iConeSize ) {
    std::ostringstream iConeSizeString;
    iConeSizeString << std::setfill('0') << std::setw(2) << iConeSize;
    
    std::string hElectronTrkIsoPtConeSizeDepName_i 
      = std::string("ElectronTrkIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
    hElectronTrkIsoPtConeSizeDep_.push_back(book1D(hElectronTrkIsoPtConeSizeDepName_i, 
						   hElectronTrkIsoPtConeSizeDepName_i, 40, 0., 10.));
    std::string hElectronEcalIsoPtConeSizeDepName_i 
      = std::string("ElectronEcalIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
    hElectronEcalIsoPtConeSizeDep_.push_back(book1D(hElectronEcalIsoPtConeSizeDepName_i, 
						    hElectronEcalIsoPtConeSizeDepName_i, 40, 0., 10.));
    std::string hElectronHcalIsoPtConeSizeDepName_i 
      = std::string("ElectronHcalIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
    hElectronHcalIsoPtConeSizeDep_.push_back(book1D(hElectronHcalIsoPtConeSizeDepName_i, 
						    hElectronHcalIsoPtConeSizeDepName_i, 40, 0., 10.));
    
    std::string hElectronParticleFlowIsoPtConeSizeDepName_i 
      = std::string("ElectronParticleFlowIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
    hElectronParticleFlowIsoPtConeSizeDep_.push_back(book1D(hElectronParticleFlowIsoPtConeSizeDepName_i, 
							    hElectronParticleFlowIsoPtConeSizeDepName_i, 40, 0., 10.));
    std::string hElectronPFChargedHadronIsoPtConeSizeDepName_i 
      = std::string("ElectronChargedHadronIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
    hElectronPFChargedHadronIsoPtConeSizeDep_.push_back(book1D(hElectronPFChargedHadronIsoPtConeSizeDepName_i, 
							       hElectronPFChargedHadronIsoPtConeSizeDepName_i, 40, 0., 10.));
    std::string hElectronPFNeutralHadronIsoPtConeSizeDepName_i 
      = std::string("ElectronPFNeutralHadronIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
    hElectronPFNeutralHadronIsoPtConeSizeDep_.push_back(book1D(hElectronPFNeutralHadronIsoPtConeSizeDepName_i, 
							       hElectronPFNeutralHadronIsoPtConeSizeDepName_i, 40, 0., 10.));
    std::string hElectronPFGammaIsoPtConeSizeDepName_i 
      = std::string("ElectronPFGammaIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
    hElectronPFGammaIsoPtConeSizeDep_.push_back(book1D(hElectronPFGammaIsoPtConeSizeDepName_i, 
						       hElectronPFGammaIsoPtConeSizeDepName_i, 40, 0., 10.));
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void ElectronHistManager::fillElectronHistograms(const pat::Electron& patElectron, 
						 MonitorElement* hElectronPt, MonitorElement* hElectronEta, MonitorElement* hElectronPhi,
						 double weight)
{
  //std::cout << "<ElectronHistManager::fillElectronHistograms>:" << std::endl;

  hElectronPt->Fill(patElectron.pt(), weight);
  hElectronEta->Fill(patElectron.eta(), weight);
  hElectronPhi->Fill(patElectron.phi(), weight);
}

void ElectronHistManager::fillElectronIsoHistograms(const pat::Electron& patElectron, double weight)
{
  //std::cout << "<ElectronHistManager::fillElectronIsoHistograms>:" << std::endl;

  hElectronTrkIsoPt_->Fill(patElectron.trackIso(), weight);
  hElectronTrkIsoPtVsElectronPt_->Fill(patElectron.pt(), patElectron.trackIso(), weight);
  hElectronEcalIsoPt_->Fill(patElectron.ecalIso(), weight);

  if ( patElectron.superCluster().isAvailable() && patElectron.superCluster().isNonnull() ) {
    if ( TMath::Abs(patElectron.superCluster()->eta()) < electronEtaMaxBarrel_ ) 
      hElectronEcalIsoPtBarrel_->Fill(patElectron.ecalIso(), weight);
    if ( TMath::Abs(patElectron.superCluster()->eta()) > electronEtaMinEndcap_ ) 
      hElectronEcalIsoPtEndcap_->Fill(patElectron.ecalIso(), weight);
  }
  hElectronEcalIsoPtVsElectronPt_->Fill(patElectron.pt(), patElectron.ecalIso(), weight);
  hElectronHcalIsoPt_->Fill(patElectron.hcalIso(), weight);
  //hElectronIsoSumPt_->Fill(patElectron.trackIso() + patElectron.ecalIso() + patElectron.hcalIso(), weight);
  hElectronIsoSumPt_->Fill(patElectron.trackIso() + patElectron.ecalIso(), weight);
  hElectronIsoSumPtVsElectronPt_->Fill(patElectron.pt(), patElectron.trackIso() + patElectron.ecalIso(), weight);
  hElectronTrkIsoPtRel_->Fill(patElectron.trackIso()/patElectron.pt(), weight);
  hElectronEcalIsoPtRel_->Fill(patElectron.ecalIso()/patElectron.pt(), weight);
  if ( patElectron.superCluster().isAvailable() && patElectron.superCluster().isNonnull() ) {
    if ( TMath::Abs(patElectron.superCluster()->eta()) < electronEtaMaxBarrel_ ) 
      hElectronEcalIsoPtBarrelRel_->Fill(patElectron.ecalIso()/patElectron.pt(), weight);
    if ( TMath::Abs(patElectron.superCluster()->eta()) > electronEtaMinEndcap_ ) 
      hElectronEcalIsoPtEndcapRel_->Fill(patElectron.ecalIso()/patElectron.pt(), weight);
  }
  hElectronHcalIsoPtRel_->Fill(patElectron.hcalIso()/patElectron.pt(), weight);
  //hElectronIsoSumPtRel_->Fill((patElectron.trackIso() + patElectron.ecalIso() + patElectron.hcalIso())/patElectron.pt(), weight);
  hElectronIsoSumPtRel_->Fill((patElectron.trackIso() + patElectron.ecalIso())/patElectron.pt(), weight);
    
  //std::cout << " particleIso = " << patElectron.particleIso() << std::endl;
  //std::cout << " chargedHadronIso = " << patElectron.chargedHadronIso() << std::endl;
  //std::cout << " neutralHadronIso = " << patElectron.neutralHadronIso() << std::endl;
  //std::cout << " photonIso = " << patElectron.photonIso() << std::endl;
  
  hElectronParticleFlowIsoPt_->Fill(patElectron.particleIso(), weight);
  hElectronPFChargedHadronIsoPt_->Fill(patElectron.chargedHadronIso(), weight);
  hElectronPFNeutralHadronIsoPt_->Fill(patElectron.neutralHadronIso(), weight);
  hElectronPFGammaIsoPt_->Fill(patElectron.photonIso(), weight);

  if ( makeIsoPtCtrlHistograms_ ) {
    hElectronPFChargedHadronIsoPtCtrl_->Fill(patElectron.trackIso(), patElectron.chargedHadronIso(), weight);
    hElectronPFGammaIsoPtCtrl_->Fill(patElectron.ecalIso(), patElectron.photonIso(), weight);
  } 

  fillLeptonIsoDepositHistograms(patElectron.trackerIsoDeposit(), 
				 hElectronTrkIsoValProfile_, hElectronTrkIsoEtaDistProfile_, hElectronTrkIsoPhiDistProfile_, weight);
  fillLeptonIsoDepositHistograms(patElectron.ecalIsoDeposit(), 
				 hElectronEcalIsoValProfile_, hElectronEcalIsoEtaDistProfile_, hElectronEcalIsoPhiDistProfile_, weight);
  fillLeptonIsoDepositHistograms(patElectron.hcalIsoDeposit(), 
				 hElectronHcalIsoValProfile_, hElectronHcalIsoEtaDistProfile_, hElectronHcalIsoPhiDistProfile_, weight);
}

void ElectronHistManager::fillElectronIsoConeSizeDepHistograms(const pat::Electron& patElectron, double weight)
{
  //std::cout << "<ElectronHistManager::fillElectronIsoConeSizeDepHistograms>:" << std::endl;

  for ( unsigned iConeSize = 1; iConeSize <= numElectronIsoConeSizes_; ++iConeSize ) {
    float isoConeSize_i = iConeSize*electronIsoConeSizeIncr_;
    
    float electronTrkIsoDeposit_i = patElectron.trackerIsoDeposit()->countWithin(isoConeSize_i, electronTrkIsoParam_, false);
    hElectronTrkIsoPtConeSizeDep_[iConeSize - 1]->Fill(electronTrkIsoDeposit_i, weight);
    
    float electronEcalIsoDeposit_i = patElectron.ecalIsoDeposit()->countWithin(isoConeSize_i, electronEcalIsoParam_, false);
    hElectronEcalIsoPtConeSizeDep_[iConeSize - 1]->Fill(electronEcalIsoDeposit_i, weight);

    float electronHcalIsoDeposit_i = patElectron.hcalIsoDeposit()->countWithin(isoConeSize_i, electronHcalIsoParam_, false);
    hElectronHcalIsoPtConeSizeDep_[iConeSize - 1]->Fill(electronHcalIsoDeposit_i, weight);

    if ( patElectron.isoDeposit(pat::ParticleIso) ) {
      double electronParticleFlowIsoDeposit_i 
	= patElectron.isoDeposit(pat::ParticleIso)->countWithin(isoConeSize_i, electronParticleFlowIsoParam_, false);
      hElectronParticleFlowIsoPtConeSizeDep_[iConeSize - 1]->Fill(electronParticleFlowIsoDeposit_i, weight);
    }
    
    if ( patElectron.isoDeposit(pat::ChargedHadronIso) ) {
      double electronPFChargedHadronIsoDeposit_i 
	= patElectron.isoDeposit(pat::ChargedHadronIso)->countWithin(isoConeSize_i, electronParticleFlowIsoParam_, false);
      hElectronPFChargedHadronIsoPtConeSizeDep_[iConeSize - 1]->Fill(electronPFChargedHadronIsoDeposit_i, weight);
    }
    
    if ( patElectron.isoDeposit(pat::NeutralHadronIso) ) {
      double electronPFNeutralHadronIsoDeposit_i 
	= patElectron.isoDeposit(pat::NeutralHadronIso)->countWithin(isoConeSize_i, electronParticleFlowIsoParam_, false);
      hElectronPFNeutralHadronIsoPtConeSizeDep_[iConeSize - 1]->Fill(electronPFNeutralHadronIsoDeposit_i, weight);
    }

    if ( patElectron.isoDeposit(pat::PhotonIso) ) {
      double electronPFGammaIsoDeposit_i 
	= patElectron.isoDeposit(pat::PhotonIso)->countWithin(isoConeSize_i, electronParticleFlowIsoParam_, false);
      hElectronPFGammaIsoPtConeSizeDep_[iConeSize - 1]->Fill(electronPFGammaIsoDeposit_i, weight);
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, ElectronHistManager, "ElectronHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, ElectronHistManager, "ElectronHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<ElectronHistManager> ElectronAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(ElectronAnalyzer);
