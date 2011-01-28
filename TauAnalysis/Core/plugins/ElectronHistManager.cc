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
#include "TauAnalysis/Core/interface/eventAuxFunctions.h"
#include "TauAnalysis/GenSimTools/interface/genParticleAuxFunctions.h"

#include <TMath.h>

#include <stdlib.h>

bool matchesGenElectron(const pat::Electron& patElectron)
{
  //std::cout << "<matchesGenElectron>:" << std::endl;

  bool isGenElectronMatched = false;
  std::vector<reco::GenParticleRef> associatedGenParticles = patElectron.genParticleRefs();
  for ( std::vector<reco::GenParticleRef>::const_iterator it = associatedGenParticles.begin(); 
	it != associatedGenParticles.end(); ++it ) {
    if ( isValidRef(*it) ) {
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
: HistManagerBase(cfg),
  pfCombIsoExtractor_(0)
{
  //std::cout << "<ElectronHistManager::ElectronHistManager>:" << std::endl;
  
  electronSrc_ = cfg.getParameter<edm::InputTag>("electronSource");
  //std::cout << " electronSrc = " << electronSrc_ << std::endl;
  
  vertexSrc_ = ( cfg.exists("vertexSource") ) ? cfg.getParameter<edm::InputTag>("vertexSource") : edm::InputTag();
  if ( vertexSrc_.label() == "" ) {
    edm::LogWarning("ElectronHistManager") 
      << " Configuration parameter 'vertexSource' not specified" 
      << " --> Impact Parameter histograms will NOT be plotted !!";
  }
  //std::cout << " vertexSrc = " << vertexSrc_ << std::endl;
  
  jetSrc_ = cfg.getParameter<edm::InputTag>("jetSource");
  //std::cout << " jetSrc = " << jetSrc_ << std::endl;
   
  pfCandidateSrc_ = cfg.getParameter<edm::InputTag>("pfCandidateSource");
  //std::cout << " pfCandidateSrc = " << pfCandidateSrc_ << std::endl;

  genParticleSrc_ = ( cfg.exists("genParticleSource") ) ? cfg.getParameter<edm::InputTag>("genParticleSource") : edm::InputTag();
  //std::cout << " genParticleSrc = " << genParticleSrc_ << std::endl;
  
  requireGenElectronMatch_ = cfg.getParameter<bool>("requireGenElectronMatch");
  //std::cout << " requireGenElectronMatch = " << requireGenElectronMatch_ << std::endl;
  
  skipPdgIdsGenParticleMatch_ = cfg.getParameter<vint>("skipPdgIdsGenParticleMatch");
 
  if ( cfg.exists("pfCombIsoExtractor") ) {
	edm::ParameterSet cfgPFCompIsoExtractor = cfg.getParameter<edm::ParameterSet>("pfCombIsoExtractor");
	pfCombIsoExtractor_ = new PATElectronPFIsolationExtractor(cfgPFCompIsoExtractor);
  }

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

  delete pfCombIsoExtractor_;
}

void ElectronHistManager::bookHistogramsImp()
{
  //std::cout << "<ElectronHistManager::bookHistogramsImp>:" << std::endl;
  
//--- book histogram for number of electrons in each event
  hNumElectrons_ = book1D("NumElectrons", "Electron Count", 10, -0.5, 9.5);

//--- book histograms for Pt, eta and phi distributions
//    of electrons passing all id. and isolation selections
  bookElectronHistograms(hElectronPt_, hElectronEta_, hElectronPhi_, "Electron");
  hElectronPtVsEta_ = book2D("ElectronPtVsEta", "Electron #eta vs P_{T}", 24, -3., +3., 30, 0., 150.);
  hElectronCharge_ = book1D("ElectronCharge", "Electron Charge", 3, -1.5, +1.5);
  
  bookWeightHistograms(*dqmStore_, "ElectronWeight", "Electron Weight", 
		       hElectronWeightPosLog_, hElectronWeightNegLog_, hElectronWeightZero_,
		       hElectronWeightLinear_);
  
  bookElectronHistograms(hGenElectronPt_, hGenElectronEta_, hGenElectronPhi_, "GenElectron");	

  hElectronEnCompToGen_ = book1D("ElectronEnCompToGen", "Electron RECO-GEN #Delta E", 100, -0.50, +0.50);
  hElectronThetaCompToGen_ = book1D("ElectronThetaCompToGen", "Electron RECO-GEN #Delta#theta", 200, -0.025, +0.025);
  hElectronPhiCompToGen_ = book1D("ElectronPhiCompToGen", "Electron RECO-GEN #Delta#phi", 200, -0.010, +0.010);
  
  hElectronMatchingGenParticlePdgId_ = book1D("ElectronMatchingGenParticlePdgId", "matching gen. Particle PdgId", 26, -1.5, 24.5);
  hElectronMatchingFinalStateGenParticlePdgId_ = book1D("ElectronMatchingFinalStateGenParticlePdgId", "matching final-state gen. Particle PdgId", 26, -1.5, 24.5);
  
  hElectronTrackPt_ = book1D("ElectronTrackPt", "Electron Track P_{T}", 75, 0., 150.);
  hElectronTrackIPxy_ = book1D("ElectronTrackIPxy", "Electron Track Impact Parameter (xy)", 100, -0.100, 0.100);
  hElectronTrackIPz_ = book1D("ElectronTrackIPz", "Electron Track Impact Parameter (z)", 100, -1.0, 1.0);
  
  hElectronSuperclEnOverTrackMomBarrel_ = book1D("ElectronSuperclEnOverTrackMomBarrel", "Electron (Barrel) #frac{E_{Supercluster}}{P_{Track}}", 50, 0., 5.);
  hElectronSuperclEnOverTrackMomEndcap_ = book1D("ElectronSuperclEnOverTrackMomEndcap", "Electron (Endcap) #frac{E_{Supercluster}}{P_{Track}}", 50, 0., 5.);
  hElectronSeedClEnOverTrackMomBarrel_ = book1D("ElectronSeedClEnOverTrackMomBarrel", "Electron (Barrel) #frac{E_{Seed-Cluster}}{P_{Track}}", 50, 0., 5.);
  hElectronSeedClEnOverTrackMomEndcap_ = book1D("ElectronSeedClEnOverTrackMomEndcap", "Electron (Endcap) #frac{E_{Seed-Cluster}}{P_{Track}}", 50, 0., 5.);
  hElectronSuperclShapeSigmaEtaEta_ = book1D("ElectronSuperclShapeSigmaEtaEta", "Electron #sigma_{#eta#eta}^{Supercluster}", 102, -0.001, 0.101);
  hElectronBremsFraction_ = book1D("ElectronBremsFraction", "Electron f_{Brems}", 80, -2., +2.);
  hElectronHadEnOverEmEnBarrel_ = book1D("ElectronHadEnOverEmEnBarrel", "Electron #frac{E_{had}}{E_{em}} Barrel", 100, 0., 0.15);
  hElectronSuperclShapeSigmaIetaIetaBarrel_ = book1D("ElectronSuperclShapeSigmaIetaIetaBarrel", "Electron #sigma_{i#eta i#eta}^{Supercluster} Barrel", 100, 0., 0.035);
  hElectronDeltaPhiSuperclToExtrapolTrackBarrel_ = book1D("ElectronDeltaPhiSuperclToExtrapolTrackBarrel", "Electron #Delta#phi_{in} Barrel", 100, -0.25, 0.25);
  hElectronDeltaEtaSuperclToExtrapolTrackBarrel_ = book1D("ElectronDeltaEtaSuperclToExtrapolTrackBarrel", "Electron #Delta#eta_{in} Barrel", 100, -0.075, 0.075);
  hElectronHadEnOverEmEnEndcap_ = book1D("ElectronHadEnOverEmEnEndcap", "Electron #frac{E_{had}}{E_{em}} Endcap", 100, 0., 0.15);
  hElectronSuperclShapeSigmaIetaIetaEndcap_ = book1D("ElectronSuperclShapeSigmaIetaIetaEndcap", "Electron #sigma_{i#eta i#eta}^{Supercluster} Endcap", 100, 0., 0.035);
  hElectronDeltaPhiSuperclToExtrapolTrackEndcap_ = book1D("ElectronDeltaPhiSuperclToExtrapolTrackEndcap", "Electron #Delta#phi_{in} Endcap", 100, -0.25, 0.25);
  hElectronDeltaEtaSuperclToExtrapolTrackEndcap_ = book1D("ElectronDeltaEtaSuperclToExtrapolTrackEndcap", "Electron #Delta#eta_{in} Endcap", 100, -0.075, 0.075);
  
  hElectronTrkIsoPt_ = book1D("ElectronTrkIsoPt", "Electron Track Isolation P_{T}", 100, 0., 10.);    
  hElectronTrkIsoPtVsElectronPt_ = book2D("ElectronTrkIsoPtVsElectronPt", "Electron Track Isolation P_{T} vs Electron P_{T}", 20, 0., 100., 20, 0., 10.);    
  hElectronEcalIsoPt_ = book1D("ElectronEcalIsoPt", "Electron ECAL Isolation P_{T}", 100, 0., 10.);
  hElectronEcalIsoPtBarrel_ = book1D("ElectronEcalIsoPtBarrel", "Electron (Barrel) ECAL Isolation P_{T}", 100, 0., 10.);
  hElectronEcalIsoPtEndcap_ = book1D("ElectronEcalIsoPtEndcap", "Electron (Endcap) ECAL Isolation P_{T}", 100, 0., 10.);
  hElectronEcalIsoPtVsElectronPt_ = book2D("ElectronEcalIsoPtVsElectronPt", "Electron ECAL Isolation P_{T} vs Electron P_{T}", 20, 0., 100., 20, 0., 10.);   
  hElectronHcalIsoPt_ = book1D("ElectronHcalIsoPt", "Electron HCAL Isolation P_{T}", 100, 0., 10.);
  hElectronIsoSumPt_ = book1D("ElectronIsoSumPt", "Electron Isolation Sum(P_{T})", 100, 0., 10.);
  hElectronIsoSumPtVsElectronPt_ = book2D("hElectronIsoSumPtVsElectronPt", "Electron Isolation Sum(P_{T}) vs Electron P_{T}", 20, 0., 100., 20, 0., 10.);   
  hElectronTrkIsoPtRel_ = book1D("ElectronTrkIsoPtRel", "ElectronTrkIsoPtRel", 100, 0., 2.);    
  hElectronEcalIsoPtRel_ = book1D("ElectronEcalIsoPtRel", "ElectronEcalIsoPtRel", 100, 0., 2.);
  hElectronEcalIsoPtBarrelRel_ = book1D("ElectronEcalIsoPtBarrelRel", "Electron (Barrel) #frac{P_{T ECAL isolation}}{P_{T track}}", 100, 0., 1.);
  hElectronEcalIsoPtEndcapRel_ = book1D("ElectronEcalIsoPtEndcapRel", "Electron (Endcap) #frac{P_{T ECAL isolation}}{P_{T track}}", 100, 0., 1.);
  hElectronHcalIsoPtRel_ = book1D("ElectronHcalIsoPtRel", "Electron #frac{P_{T HCAL isolation}}{P_{T track}}", 100, 0., 1.);
  hElectronIsoSumPtRel_ = book1D("ElectronIsoSumPtRel", "Electron #frac{#sum P_{T isolation}}{P_{T track}}", 100, 0., 1.);
  
  hElectronDeltaRnearestJet_ = book1D("ElectronDeltaRnearestJet", "#DeltaR(nearest Jet)", 102, -0.1, 10.1);
  
  hElectronParticleFlowIsoPt_ = book1D("ElectronParticleFlowIsoPt", "Electron Particle Flow Isolation P_{T}", 100, 0., 10.);    
  hElectronParticleFlowIsoPtRel_ = book1D("ElectronParticleFlowIsoPtRel", "Relative Electron Particle Flow Isolation P_{T}", 100, 0., 10.);    
  hElectronPFChargedHadronIsoPt_ = book1D("ElectronPFChargedHadronIsoPt", "Electron Particle Flow (Charged Hadrons) Isolation P_{T}", 100, 0., 10.);   
  hElectronPFChargedHadronIsoPtRel_ = book1D("ElectronPFChargedHadronIsoPtRel", "Relative Electron Particle Flow (Charged Hadrons) Isolation P_{T}", 100, 0., 10.);   
  hElectronPFNeutralHadronIsoPt_ = book1D("ElectronPFNeutralHadronIsoPt", "Electron Particle Flow (Neutral Hadrons) Isolation P_{T}", 100, 0., 10.);   
  hElectronPFNeutralHadronIsoPtRel_ = book1D("ElectronPFNeutralHadronIsoPtRel", "Relative Electron Particle Flow (Neutral Hadrons) Isolation P_{T}", 100, 0., 10.);   
  hElectronPFGammaIsoPt_ = book1D("ElectronPFGammaIsoPt", "Electron Particle Flow (Photons) Isolation P_{T}", 100, 0., 10.);  
  hElectronPFGammaIsoPtRel_ = book1D("ElectronPFGammaIsoPtRel", "Relative Electron Particle Flow (Photons) Isolation P_{T}", 100, 0., 10.);  
 
  hElectronPFCombIsoPt_ = book1D("ElectronPFCombIsoPt", "ElectronPFCombIsoPt", 101, -0.05, 10.05);
  hElectronPFCombIsoPtBarrel_ = book1D("ElectronPFCombIsoPtBarrel", "ElectronPFCombIsoPtBarrel", 101, -0.05, 10.05);
  hElectronPFCombIsoPtEndcap_ = book1D("ElectronPFCombIsoPtEndcap", "ElectronPFCombIsoPtEndcap", 101, -0.05, 10.05);
  hElectronPFCombIsoPtRel_ = book1D("ElectronPFCombIsoPtRel", "ElectronPFCombIsoPtRel", 101, -0.005, 1.005);
  hElectronPFCombIsoPtRelBarrel_ = book1D("ElectronPFCombIsoPtRelBarrel", "ElectronPFCombIsoPtRelBarrel", 101, -0.005, 1.005);
  hElectronPFCombIsoPtRelEndcap_ = book1D("ElectronPFCombIsoPtRelEndcap", "ElectronPFCombIsoPtRelEndcap", 101, -0.005, 1.005);

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
  getCollection(evt, electronSrc_, patElectrons);
  
  edm::Handle<pat::JetCollection> patJets;
  getCollection(evt, jetSrc_, patJets);

  edm::Handle<reco::PFCandidateCollection> pfCandidates;
  evt.getByLabel(pfCandidateSrc_, pfCandidates);

  edm::Handle<reco::GenParticleCollection> genParticles;
  if ( genParticleSrc_.label() != "" ) evt.getByLabel(genParticleSrc_, genParticles);

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
    
    fillWeightHistograms(hElectronWeightPosLog_, hElectronWeightNegLog_, hElectronWeightZero_, 
			 hElectronWeightLinear_, electronWeight);
    
//--- compare reconstructed electron to generator level one;
//    normalize difference between reconstructed and generated energy
//    to expected energy dependence of resolution
    if ( patElectron->genLepton() ) {
      hGenElectronPt_->Fill(patElectron->genLepton()->pt(), weight);
      hGenElectronEta_->Fill(patElectron->genLepton()->eta(), weight);
      hGenElectronPhi_->Fill(patElectron->genLepton()->phi(), weight);

      hElectronEnCompToGen_->Fill((patElectron->energy() - patElectron->genLepton()->energy())
				  /TMath::Sqrt(patElectron->genLepton()->energy()), weight);
      hElectronThetaCompToGen_->Fill(patElectron->theta() - patElectron->genLepton()->theta(), weight);
      hElectronPhiCompToGen_->Fill(patElectron->phi() - patElectron->genLepton()->phi(), weight);
    }
    
    int matchingGenParticlePdgId = -1;
    int matchingFinalStateGenParticlePdgId = -1;
    if( genParticles.isValid() ) { 
      matchingGenParticlePdgId = getMatchingGenParticlePdgId(patElectron->p4(), *genParticles, &skipPdgIdsGenParticleMatch_, true);
      matchingFinalStateGenParticlePdgId = getMatchingGenParticlePdgId(patElectron->p4(), *genParticles, &skipPdgIdsGenParticleMatch_, false);
    }
    
    if ( matchingFinalStateGenParticlePdgId == -1 ) {
      hElectronMatchingFinalStateGenParticlePdgId_->Fill(-1, weight);
    } else if ( abs(matchingFinalStateGenParticlePdgId) > 22 ) {
      hElectronMatchingFinalStateGenParticlePdgId_->Fill(24, weight);
    } else {
      hElectronMatchingFinalStateGenParticlePdgId_->Fill(abs(matchingFinalStateGenParticlePdgId), weight);
    }
    
    if ( matchingGenParticlePdgId == -1 ) {
      hElectronMatchingGenParticlePdgId_->Fill(-1, weight);
    } else if ( abs(matchingGenParticlePdgId) > 22 ) {
      hElectronMatchingGenParticlePdgId_->Fill(24, weight);
    } else {
      hElectronMatchingGenParticlePdgId_->Fill(abs(matchingGenParticlePdgId), weight);
    }
    
    if ( isValidRef(patElectron->gsfTrack()) ) {
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
      hElectronHadEnOverEmEnBarrel_->Fill(patElectron->hcalOverEcal(), weight); 
      hElectronSuperclShapeSigmaIetaIetaBarrel_->Fill(patElectron->sigmaIetaIeta(), weight); 
      hElectronDeltaPhiSuperclToExtrapolTrackBarrel_->Fill(patElectron->deltaPhiSuperClusterTrackAtVtx(), weight);
      hElectronDeltaEtaSuperclToExtrapolTrackBarrel_->Fill(patElectron->deltaEtaSuperClusterTrackAtVtx(), weight);    
    }
    if ( TMath::Abs(patElectron->eta()) > electronEtaMinEndcap_ ) {
      hElectronSuperclEnOverTrackMomEndcap_->Fill(patElectron->eSuperClusterOverP(), weight);
      hElectronSeedClEnOverTrackMomEndcap_->Fill(patElectron->eSeedClusterOverP(), weight); 
      hElectronHadEnOverEmEnEndcap_->Fill(patElectron->hcalOverEcal(), weight); 
      hElectronSuperclShapeSigmaIetaIetaEndcap_->Fill(patElectron->sigmaIetaIeta(), weight); 
      hElectronDeltaPhiSuperclToExtrapolTrackEndcap_->Fill(patElectron->deltaPhiSuperClusterTrackAtVtx(), weight);
      hElectronDeltaEtaSuperclToExtrapolTrackEndcap_->Fill(patElectron->deltaEtaSuperClusterTrackAtVtx(), weight);
    }
    hElectronSuperclShapeSigmaEtaEta_->Fill(patElectron->sigmaEtaEta(), weight); 
    if ( isValidRef(patElectron->gsfTrack()) ) {
      double pIn  = patElectron->trackMomentumAtVtx().R(); 
      double pOut = patElectron->trackMomentumOut().R(); 
      if ( pIn > 0. ) hElectronBremsFraction_->Fill((pIn - pOut)/pIn, weight);
    }
    
    fillElectronIsoHistograms(*patElectron, *pfCandidates, weight);
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

void ElectronHistManager::fillElectronIsoHistograms(const pat::Electron& patElectron, const reco::PFCandidateCollection& pfCandidates, double weight)
{
  //std::cout << "<ElectronHistManager::fillElectronIsoHistograms>:" << std::endl;

  double electronTrackIso = patElectron.dr03TkSumPt();
  double electronEcalIso = patElectron.dr03EcalRecHitSumEt();
  double electronHcalIso = patElectron.userIsolation(pat::HcalIso);
  double electronTrackEcalHcalIsoSum = electronTrackIso + electronEcalIso + electronHcalIso; 
  double electronTrackEcalIsoSum = electronTrackIso + electronEcalIso; 

  hElectronTrkIsoPt_->Fill(electronTrackIso, weight);
  hElectronTrkIsoPtRel_->Fill(electronTrackIso/patElectron.pt(), weight);
  hElectronTrkIsoPtVsElectronPt_->Fill(patElectron.pt(), electronTrackIso, weight);
  
  hElectronEcalIsoPt_->Fill(electronEcalIso, weight);
  hElectronEcalIsoPtRel_->Fill(electronEcalIso/patElectron.pt(), weight);
  hElectronEcalIsoPtVsElectronPt_->Fill(patElectron.pt(), electronEcalIso, weight);
  
  if ( isValidRef(patElectron.superCluster()) ) {
    if ( TMath::Abs(patElectron.superCluster()->eta()) < electronEtaMaxBarrel_ ) { 
      hElectronEcalIsoPtBarrel_->Fill(electronEcalIso, weight);
      hElectronEcalIsoPtBarrelRel_->Fill(electronEcalIso/patElectron.pt(), weight);
	}
    if ( TMath::Abs(patElectron.superCluster()->eta()) > electronEtaMinEndcap_ ) {
      hElectronEcalIsoPtEndcap_->Fill(electronEcalIso, weight);
      hElectronEcalIsoPtEndcapRel_->Fill(electronEcalIso/patElectron.pt(), weight);
	}
  }
  
  hElectronHcalIsoPt_->Fill(electronHcalIso, weight);
  hElectronHcalIsoPtRel_->Fill(electronHcalIso/patElectron.pt(), weight);
  
  hElectronIsoSumPt_->Fill(electronTrackIso + electronEcalIso + electronHcalIso, weight);
  hElectronIsoSumPtVsElectronPt_->Fill(patElectron.pt(), electronTrackIso + electronEcalIso, weight);
  hElectronIsoSumPtRel_->Fill((electronTrackIso + electronEcalIso + electronHcalIso)/patElectron.pt(), weight);
  //hElectronIsoSumPt_->Fill(electronTrackIso + electronEcalIso, weight);
  //hElectronIsoSumPtRel_->Fill((electronTrackIso + electronEcalIso)/patElectron.pt(), weight);
  
  //std::cout << " particleIso = " << patElectron.particleIso() << std::endl;
  //std::cout << " chargedHadronIso = " << patElectron.chargedHadronIso() << std::endl;
  //std::cout << " neutralHadronIso = " << patElectron.neutralHadronIso() << std::endl;
  //std::cout << " photonIso = " << patElectron.photonIso() << std::endl;
  
  hElectronParticleFlowIsoPt_->Fill(patElectron.particleIso(), weight);
  hElectronParticleFlowIsoPtRel_->Fill(patElectron.particleIso()/patElectron.pt(), weight);
  hElectronPFChargedHadronIsoPt_->Fill(patElectron.chargedHadronIso(), weight);
  hElectronPFChargedHadronIsoPtRel_->Fill(patElectron.chargedHadronIso()/patElectron.pt(), weight);
  hElectronPFNeutralHadronIsoPt_->Fill(patElectron.neutralHadronIso(), weight);
  hElectronPFNeutralHadronIsoPtRel_->Fill(patElectron.neutralHadronIso()/patElectron.pt(), weight);
  hElectronPFGammaIsoPt_->Fill(patElectron.photonIso(), weight);
  hElectronPFGammaIsoPtRel_->Fill(patElectron.photonIso()/patElectron.pt(), weight);
 
  if ( pfCombIsoExtractor_ ) {
	  double pfCombIso = (*pfCombIsoExtractor_)(patElectron, pfCandidates);
	  hElectronPFCombIsoPt_->Fill(pfCombIso, weight);
	  hElectronPFCombIsoPtRel_->Fill(pfCombIso/patElectron.pt(), weight);

	  if ( isValidRef(patElectron.superCluster()) ) {
		  if ( TMath::Abs(patElectron.superCluster()->eta()) < electronEtaMaxBarrel_ ) { 
			  hElectronPFCombIsoPtBarrel_->Fill(pfCombIso, weight);
			  hElectronPFCombIsoPtRelBarrel_->Fill(pfCombIso/patElectron.pt(), weight);
		  }
		  if ( TMath::Abs(patElectron.superCluster()->eta()) > electronEtaMinEndcap_ ) {
			  hElectronPFCombIsoPtEndcap_->Fill(pfCombIso, weight);
			  hElectronPFCombIsoPtRelEndcap_->Fill(pfCombIso/patElectron.pt(), weight);
		  }
	  }
  }
 
  if ( makeIsoPtCtrlHistograms_ ) {
    hElectronPFChargedHadronIsoPtCtrl_->Fill(electronTrackIso, patElectron.chargedHadronIso(), weight);
    hElectronPFGammaIsoPtCtrl_->Fill(electronEcalIso, patElectron.photonIso(), weight);
  } 
  
  fillLeptonIsoDepositHistograms(patElectron.trackIsoDeposit(), 
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
    
    float electronTrkIsoDeposit_i = patElectron.trackIsoDeposit()->countWithin(isoConeSize_i, electronTrkIsoParam_, false);
    hElectronTrkIsoPtConeSizeDep_[iConeSize - 1]->Fill(electronTrkIsoDeposit_i, weight);
    
    float electronEcalIsoDeposit_i = patElectron.ecalIsoDeposit()->countWithin(isoConeSize_i, electronEcalIsoParam_, false);
    hElectronEcalIsoPtConeSizeDep_[iConeSize - 1]->Fill(electronEcalIsoDeposit_i, weight);
    
    float electronHcalIsoDeposit_i = patElectron.hcalIsoDeposit()->countWithin(isoConeSize_i, electronHcalIsoParam_, false);
    hElectronHcalIsoPtConeSizeDep_[iConeSize - 1]->Fill(electronHcalIsoDeposit_i, weight);
    
    if ( patElectron.isoDeposit(pat::PfAllParticleIso) ) {
      double electronParticleFlowIsoDeposit_i 
	= patElectron.isoDeposit(pat::PfAllParticleIso)->countWithin(isoConeSize_i, electronParticleFlowIsoParam_, false);
      hElectronParticleFlowIsoPtConeSizeDep_[iConeSize - 1]->Fill(electronParticleFlowIsoDeposit_i, weight);
    }
    
    if ( patElectron.isoDeposit(pat::PfChargedHadronIso) ) {
      double electronPFChargedHadronIsoDeposit_i 
	= patElectron.isoDeposit(pat::PfChargedHadronIso)->countWithin(isoConeSize_i, electronParticleFlowIsoParam_, false);
      hElectronPFChargedHadronIsoPtConeSizeDep_[iConeSize - 1]->Fill(electronPFChargedHadronIsoDeposit_i, weight);
    }
    
    if ( patElectron.isoDeposit(pat::PfNeutralHadronIso) ) {
      double electronPFNeutralHadronIsoDeposit_i 
	= patElectron.isoDeposit(pat::PfNeutralHadronIso)->countWithin(isoConeSize_i, electronParticleFlowIsoParam_, false);
      hElectronPFNeutralHadronIsoPtConeSizeDep_[iConeSize - 1]->Fill(electronPFNeutralHadronIsoDeposit_i, weight);
    }
    
    if ( patElectron.isoDeposit(pat::PfGammaIso) ) {
      double electronPFGammaIsoDeposit_i 
	= patElectron.isoDeposit(pat::PfGammaIso)->countWithin(isoConeSize_i, electronParticleFlowIsoParam_, false);
      hElectronPFGammaIsoPtConeSizeDep_[iConeSize - 1]->Fill(electronPFGammaIsoDeposit_i, weight);
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, ElectronHistManager, "ElectronHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, ElectronHistManager, "ElectronHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<ElectronHistManager> ElectronAnalyzer;

DEFINE_FWK_MODULE(ElectronAnalyzer);
