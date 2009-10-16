#include "TauAnalysis/Core/plugins/TauHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/IsolationAlgos/interface/IsoDepositVetoFactory.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"
#include "DataFormats/TauReco/interface/PFTauDecayMode.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "PhysicsTools/Utilities/interface/deltaR.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

#include <stdlib.h>

bool matchesGenTau(const pat::Tau& patTau)
{
  bool isGenTauMatched = false;
  std::vector<reco::GenParticleRef> associatedGenParticles = patTau.genParticleRefs();
  for ( std::vector<reco::GenParticleRef>::const_iterator it = associatedGenParticles.begin(); 
	it != associatedGenParticles.end(); ++it ) {
    if ( it->isAvailable() ) {
      const reco::GenParticleRef& genParticle = (*it);
      if ( genParticle->pdgId() == -15 || genParticle->pdgId() == +15 ) isGenTauMatched = true;
    }
  }
  return isGenTauMatched;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

TauHistManager::TauHistManager(const edm::ParameterSet& cfg)
  : dqmError_(0)
{
  //std::cout << "<TauHistManager::TauHistManager>:" << std::endl;

  tauSrc_ = cfg.getParameter<edm::InputTag>("tauSource");
  //std::cout << " tauSrc = " << tauSrc_.label() << std::endl;

  vertexSrc_ = ( cfg.exists("vertexSource") ) ? cfg.getParameter<edm::InputTag>("vertexSource") : edm::InputTag();
  if ( vertexSrc_.label() == "" ) {
    edm::LogWarning("TauHistManager") << " Configuration parameter 'vertexSource' not specified" 
				      << " --> Impact Parameter histograms will NOT be plotted !!";
  }
  //std::cout << " vertexSrc = " << vertexSrc_.label() << std::endl;

  jetSrc_ = cfg.getParameter<edm::InputTag>("jetSource");
  //std::cout << " jetSrc = " << jetSrc_ << std::endl;

  genParticleSrc_ = ( cfg.exists("genParticleSource") ) ? cfg.getParameter<edm::InputTag>("genParticleSource") : edm::InputTag();
  if ( genParticleSrc_.label() == "" ) {
    edm::LogWarning("TauHistManager") << " Configuration parameter 'genParticleSource' not specified" 
				      << " --> matching gen. Particle PdgId histogram will NOT be plotted !!";
  }
  //std::cout << " genParticleSrc = " << genParticleSrc_ << std::endl;

  tauJetWeightExtractors_ = getTauJetWeightExtractors<pat::Tau>(cfg, "tauJetWeightSource");

  if ( cfg.exists("tauIndicesToPlot") ) {
    std::string tauIndicesToPlot_string = cfg.getParameter<std::string>("tauIndicesToPlot");
    if ( tauIndicesToPlot_string != "all" ) {
      size_t posTauIndexBegin = 0;
      size_t posTauIndexEnd = posTauIndexBegin;
      do {
	size_t posNextSeparator = tauIndicesToPlot_string.find(",", posTauIndexBegin);
	posTauIndexEnd = ( posNextSeparator < std::string::npos ) ? posNextSeparator : tauIndicesToPlot_string.length();
	
	std::string tauIndex_string(tauIndicesToPlot_string, posTauIndexBegin, posTauIndexEnd - posTauIndexBegin);
	int tauIndex_int = atoi(tauIndex_string.data());
	//std::cout << "--> adding tauIndex_int = " << tauIndex_int << std::endl;
	tauIndicesToPlot_.push_back(tauIndex_int);
	
	posTauIndexBegin = posTauIndexEnd + 1;
      } while ( posTauIndexEnd < tauIndicesToPlot_string.length() );
    }
  }

  dqmDirectory_store_ = cfg.getParameter<std::string>("dqmDirectory_store");
  //std::cout << " dqmDirectory_store = " << dqmDirectory_store_ << std::endl;

  requireGenTauMatch_ = cfg.getParameter<bool>("requireGenTauMatch");
  //std::cout << " requireGenTauMatch = " << requireGenTauMatch_ << std::endl;

  std::string normalization_string = cfg.getParameter<std::string>("normalization");
  normMethod_ = getNormMethod(normalization_string, "taus");

  makeIsoPtCtrlHistograms_ = ( cfg.exists("makeIsoPtCtrlHistograms") ) ? 
    cfg.getParameter<bool>("makeIsoPtCtrlHistograms") : false;

  makeIsoPtConeSizeDepHistograms_ = ( cfg.exists("makeIsoPtConeSizeDepHistograms") ) ? 
    cfg.getParameter<bool>("makeIsoPtConeSizeDepHistograms") : false;

  numTauIsoConeSizes_ = 15;
  tauIsoConeSizeIncr_ = 0.1;
  numTauIsoPtThresholds_ = 4;
  tauIsoPtThresholdIncr_ = 0.5;

//--- create "veto" objects for computation of IsoDeposit sums
  tauParticleFlowIsoParam_.push_back(IsoDepositVetoFactory::make("0.0"));
  tauParticleFlowIsoParam_.push_back(IsoDepositVetoFactory::make("Threshold(0.5)"));

  makeTauIdEfficiencyHistograms_ = ( cfg.exists("makeTauIdEfficiencyHistograms") ) ? 
    cfg.getParameter<vstring>("makeTauIdEfficiencyHistograms") : vstring();
  makeTauFakeRateHistograms_ = ( cfg.exists("makeTauFakeRateHistograms") ) ? 
    cfg.getParameter<vstring>("makeTauFakeRateHistograms") : vstring();
}

TauHistManager::~TauHistManager()
{
//--- delete "veto" objects for computation of IsoDeposit sums
  clearIsoParam(tauParticleFlowIsoParam_);

  for ( std::vector<FakeRateJetWeightExtractor<pat::Tau>*>::iterator it = tauJetWeightExtractors_.begin();
	it != tauJetWeightExtractors_.end(); ++it ) {
    delete (*it);
  }
}

void TauHistManager::bookHistograms()
{
  //std::cout << "<TauHistManager::bookHistograms>:" << std::endl;

  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("bookHistograms") << " Failed to access dqmStore --> histograms will NOT be booked !!";
    dqmError_ = 1;
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  dqmStore.setCurrentFolder(dqmDirectory_store_);

//--- book histogram for number of tau-jets in each event
  hNumTaus_ = dqmStore.book1D("NumTaus", "Tau Jets in Event", 10, -0.5, 9.5);

//--- book histograms for Pt, eta and phi distributions
//    of tau-jets passing all id. and isolation selections
  bookTauHistograms(dqmStore, hTauPt_, hTauEta_, hTauPhi_, "Tau");
  hTauPtVsEta_ = dqmStore.book2D("TauPtVsEta", "TauPtVsEta", 24, -3., +3., 30, 0., 150.);
  hTauCharge_ = dqmStore.book1D("TauCharge", "Tau Charge (#Sigma Tracks in Signal Cone)", 11, -5.5, +5.5);
  
  hTauEnCompToGen_ = dqmStore.book1D("TauEnCompToGen", "RECO-GEN #Delta E", 100, -2.50, +2.50);
  hTauThetaCompToGen_ = dqmStore.book1D("TauThetaCompToGen", "RECO-GEN #Delta#theta", 200, -0.050, +0.050);
  hTauPhiCompToGen_ = dqmStore.book1D("TauPhiCompToGen", "RECO-GEN #Delta#phi", 200, -0.050, +0.050);
  
  hTauMatchingGenParticlePdgId_ = dqmStore.book1D("TauMatchingGenParticlePdgId", "matching gen. Particle PdgId", 26, -1.5, 24.5);

  hTauNumTracksSignalCone_ = dqmStore.book1D("TauNumTracksSignalCone", "Tracks in Signal Cone", 10, -0.5, 9.5);
  hTauNumTracksIsoCone_ = dqmStore.book1D("TauNumTracksIsoCone", "Tracks in Isolation Cone", 20, -0.5, 19.5);
  
  bookTauHistograms(dqmStore, hTauLeadTrkPt_, hTauLeadTrkEta_, hTauLeadTrkPhi_, "TauLeadTrk");
  hTauLeadTrkMatchDist_ = dqmStore.book1D("TauLeadTrkMatchDist", "TauLeadTrkMatchDist", 100, -0.500, 0.500);
  hTauLeadTrkIPxy_ = dqmStore.book1D("TauLeadTrkIPxy", "Lead Track Impact Parameter (xy)", 100, -0.100, 0.100);
  hTauLeadTrkIPz_ = dqmStore.book1D("TauLeadTrkIPz", "Lead Track Impact Parameter (z)", 100, -1.0, 1.0);
  hTauLeadTrkNumHits_ = dqmStore.book1D("TauLeadTrkNumHits", "Lead Track Number of Pixel + Strip Hits", 25, -0.5, 24.5);
  hTauLeadTrkNumPixelHits_ = dqmStore.book1D("TauLeadTrkNumPixelHits", "Lead Track Number of Pixel Hits", 5, -0.5, 4.5);
  hTauLeadTrkNumStripHits_ = dqmStore.book1D("TauLeadTrkNumStripHits", "Lead Track Number of Strip Hits", 20, -0.5, 19.5);
  
  hTauDiscriminatorByIsolation_ = dqmStore.book1D("TauDiscriminatorByIsolation", 
						  "Discriminator by Isolation (Track and ECAL)", 2, -0.5, 1.5);
  hTauDiscriminatorByTrackIsolation_ = dqmStore.book1D("TauDiscriminatorByTrackIsolation", 
						       "Discriminator by Track Isolation", 2, -0.5, 1.5);
  hTauDiscriminatorByEcalIsolation_ = dqmStore.book1D("TauDiscriminatorByEcalIsolation", 
						      "Discriminator by ECAL Isolation", 2, -0.5, 1.5);
  
  hTauDiscriminatorAgainstElectrons_ = dqmStore.book1D("TauDiscriminatorAgainstElectrons", 
						       "Discriminator against Electrons", 102, -0.01, 1.01);
  hTauEmFraction_ = dqmStore.book1D("TauEmFraction", "TauEmFraction", 101, -0.01, 2.01);
  hTauHcalTotOverPLead_ = dqmStore.book1D("TauHcalTotOverPLead", "TauHcalTotOverPLead", 101, -0.01, 2.01);
  hTauHcalMaxOverPLead_ = dqmStore.book1D("TauHcalMaxOverPLead", "TauHcalMaxOverPLead", 101, -0.01, 2.01);
  hTauHcal3x3OverPLead_ = dqmStore.book1D("TauHcal3x3OverPLead", "TauHcal3x3OverPLead", 101, -0.01, 2.01);
  hTauEcalStripSumEOverPLead_ = dqmStore.book1D("TauEcalStripSumEOverPLead", "TauEcalStripSumEOverPLead", 101, -0.01, 2.01);
  hTauBremsRecoveryEOverPLead_ = dqmStore.book1D("TauBremsRecoveryEOverPLead", "TauBremsRecoveryEOverPLead", 101, -0.01, 2.01);
  hTauCaloEOverPLead_ = dqmStore.book1D("TauCaloEOverPLead", "TauCaloEOverPLead", 101, -0.01, 2.01);

  hTauDiscriminatorAgainstMuons_ = dqmStore.book1D("TauDiscriminatorAgainstMuons", 
						   "Discriminator against Muons", 102, -0.01, 1.01);
    
  hTauRecDecayMode_ = dqmStore.book1D("TauRecDecayMode", "decayMode()", 25, -0.5, 24.5);
  
  hTauTaNCoutputOneProngNoPi0s_ = dqmStore.book1D("TauTaNCoutputOneProngNoPi0s", 
						  "TauTaNCoutputOneProngNoPi0s", 102, -0.01, 1.01); 
  hTauTaNCoutputOneProngOnePi0_ = dqmStore.book1D("TauTaNCoutputOneProngOnePi0", 
						  "TauTaNCoutputOneProngOnePi0", 102, -0.01, 1.01);
  hTauTaNCoutputOneProngTwoPi0s_ = dqmStore.book1D("TauTaNCoutputOneProngTwoPi0s", 
						   "TauTaNCoutputOneProngTwoPi0s", 102, -0.01, 1.01);
  hTauTaNCoutputThreeProngNoPi0s_ = dqmStore.book1D("TauTaNCoutputThreeProngNoPi0s", 
						    "TauTaNCoutputThreeProngNoPi0s", 102, -0.01, 1.01);
  hTauTaNCoutputThreeProngOnePi0_ = dqmStore.book1D("TauTaNCoutputThreeProngOnePi0", 
						    "TauTaNCoutputThreeProngOnePi0", 102, -0.01, 1.01);
  hTauTaNCoutputTransform_ = dqmStore.book1D("TauTaNCoutputTransform",
					     "TauTaNCoutputTransform", 102, -0.01, 1.01);
  
  hTauDiscriminatorTaNCfrOnePercent_ = dqmStore.book1D("TauDiscriminatorTaNCfrOnePercent",
						       "TauDiscriminatorTaNCfrOnePercent", 102, -0.01, 1.01);
  hTauDiscriminatorTaNCfrHalfPercent_ = dqmStore.book1D("TauDiscriminatorTaNCfrHalfPercent",
							"TauDiscriminatorTaNCfrHalfPercent", 102, -0.01, 1.01);
  hTauDiscriminatorTaNCfrQuarterPercent_ = dqmStore.book1D("TauDiscriminatorTaNCfrQuarterPercent",
							   "TauDiscriminatorTaNCfrQuarterPercent", 102, -0.01, 1.01);
  hTauDiscriminatorTaNCfrTenthPercent_ = dqmStore.book1D("TauDiscriminatorTaNCfrTenthPercent",
							 "TauDiscriminatorTaNCfrTenthPercent", 102, -0.01, 1.01);
    
  hTauTrkIsoPt_ = dqmStore.book1D("TauTrkIsoPt", "Track Isolation P_{T}", 100, 0., 20.);    
  hTauEcalIsoPt_ = dqmStore.book1D("TauEcalIsoPt", "ECAL Isolation P_{T}", 100, 0., 20.);
  hTauHcalIsoPt_ = dqmStore.book1D("TauHcalIsoPt", "HCAL Isolation P_{T}", 100, 0., 20.);
  hTauIsoSumPt_ = dqmStore.book1D("TauIsoSumPt", "Isolation Sum(P_{T})", 100, 0., 20.);

  hTauDeltaRnearestJet_ = dqmStore.book1D("TauDeltaRnearestJet", "#DeltaR(nearest Jet)", 102, -0.1, 10.1);
  
  hTauParticleFlowIsoPt_ = dqmStore.book1D("TauParticleFlowIsoPt", "Particle Flow Isolation P_{T}", 100, 0., 20.);    
  hTauPFChargedHadronIsoPt_ = dqmStore.book1D("TauPFChargedHadronIsoPt", "Particle Flow (Charged Hadron) Isolation P_{T}", 100, 0., 20.);   
  hTauPFNeutralHadronIsoPt_ = dqmStore.book1D("TauPFNeutralHadronIsoPt", "Particle Flow (Neutral Hadron) Isolation P_{T}", 100, 0., 20.);   
  hTauPFGammaIsoPt_ = dqmStore.book1D("TauPFGammaIsoPt", "Particle Flow (Photon) Isolation P_{T}", 100, 0., 20.);  

//--- book "control" histograms to check agreement between tau isolation variables
//    computed by PAT-level IsoDeposits with the values computed by reco::PFTau producer
  if ( makeIsoPtCtrlHistograms_ ) {
    hTauPFChargedHadronIsoPtCtrl_ = dqmStore.book2D("TauPFChargedHadronIsoPtCtrl", "Particle Flow (Charged Hadron) Isolation P_{T} (reco::PFTau vs. IsoDeposit)", 40, 0., 20., 40, 0., 20.);
    hTauPFGammaIsoPtCtrl_ = dqmStore.book2D("TauPFGammaIsoPtCtrl", "Particle Flow (Photon) Isolation P_{T} (reco::PFTau vs. IsoDeposit)", 40, 0., 20., 40, 0., 20.);
  } else {
    hTauPFChargedHadronIsoPtCtrl_ = 0;
    hTauPFGammaIsoPtCtrl_ = 0;
  } 
  
  hTauTrkIsoEnProfile_ = dqmStore.book1D("TauTrkIsoEnProfile", "All Isolation Tracks #Delta P", 100, 0., 10.);
  hTauTrkIsoPtProfile_ = dqmStore.book1D("TauTrkIsoPtProfile", "All Isolation Tracks #Delta P_{T}", 100, 0., 10.);
  hTauTrkIsoEtaDistProfile_ = dqmStore.book1D("TauTrkIsoEtaDistProfile", "All Isolation Tracks |#Delta#eta|", 15, 0., 1.5);
  hTauTrkIsoPhiDistProfile_ = dqmStore.book1D("TauTrkIsoPhiDistProfile", "All Isolation Tracks |#Delta#phi|", 15, 0., 1.5);
  
  if ( makeIsoPtConeSizeDepHistograms_ ) bookTauIsoConeSizeDepHistograms(dqmStore);

//--- book "control" histograms to check parametrization of 
//    tau id. efficiency and fake-rate values stored in the pat::Tau
  bookTauIdEfficiencyHistograms(dqmStore, hTauIdEfficiencies_, "IdEfficiency", makeTauIdEfficiencyHistograms_);
  bookTauIdEfficiencyHistograms(dqmStore, hTauFakeRates_, "FakeRate", makeTauFakeRateHistograms_);
}

void TauHistManager::fillTauDiscriminatorHistogram(MonitorElement* h, const pat::Tau& patTau, const char* discrName,
						   std::map<std::string, bool>& discrAvailability_hasBeenChecked, double weight)
{
//--- tau id. discriminators not available for all kinds of taus
//    (in particular those based on TaNC are available only for shrinking signal cone PFTaus so far),
//    so need to check whether a given discriminator is available before filling histogram,
//    in order to avoid pat::Tau::tauID method from triggering an exception;
//    availability is checked only once and a warning is printed 
//    in case the discriminator given as function argument is unavailable
  if ( !discrAvailability_hasBeenChecked[discrName] ) {
    if ( !patTau.isTauIDAvailable(discrName) ) {
      edm::LogWarning("TauHistManager") << " Discriminator = " << discrName 
					<< " unavailable for pat::Tau collection = " << tauSrc_.label() 
					<< " --> skipping filling of histogram = " << h->getName() << " !!";
    }
    
    discrAvailability_hasBeenChecked[discrName] = true;
  }
  
  if ( patTau.isTauIDAvailable(discrName) ) h->Fill(patTau.tauID(discrName), weight);
}

bool isIndexed(int patTauIndex, const std::vector<int>& tauIndicesToPlot)
{
  bool isIndexed = true;
  
  if ( tauIndicesToPlot.size() > 0 ) {
    isIndexed = false;
    for ( std::vector<int>::const_iterator tauIndexToPlot = tauIndicesToPlot.begin();
	  tauIndexToPlot != tauIndicesToPlot.end(); ++tauIndexToPlot ) {
      if ( (*tauIndexToPlot) == patTauIndex ) isIndexed = true;
    }
  }

  return isIndexed;
}

void TauHistManager::fillHistograms(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{  
  //std::cout << "<TauHistManager::fillHistograms>:" << std::endl; 

  if ( dqmError_ ) {
    edm::LogError ("fillHistograms") << " Failed to access dqmStore --> histograms will NOT be filled !!";
    return;
  }
  
  edm::Handle<pat::TauCollection> patTaus;
  evt.getByLabel(tauSrc_, patTaus);

  edm::Handle<pat::JetCollection> patJets;
  evt.getByLabel(jetSrc_, patJets);

  edm::Handle<reco::GenParticleCollection> genParticles;
  evt.getByLabel(genParticleSrc_, genParticles);

  double tauJetWeightSum = 0.;
  int patTauIndex = 0;
  for ( std::vector<pat::Tau>::const_iterator patTau = patTaus->begin(); 
	patTau != patTaus->end(); ++patTau, ++patTauIndex ) {
    if ( tauIndicesToPlot_.size() > 0 && (!isIndexed(patTauIndex, tauIndicesToPlot_)) ) continue;

    if ( requireGenTauMatch_ && !matchesGenTau(*patTau) ) continue;

    tauJetWeightSum += getTauJetWeight<pat::Tau>(*patTau, tauJetWeightExtractors_);
  }

  //std::cout << " patTaus.size = " << patTaus->size() << std::endl;
  hNumTaus_->Fill(patTaus->size(), evtWeight);

  for ( std::vector<pat::Tau>::const_iterator patTau = patTaus->begin(); 
	patTau != patTaus->end(); ++patTau, ++patTauIndex ) {

    if ( tauIndicesToPlot_.size() > 0 && (!isIndexed(patTauIndex, tauIndicesToPlot_)) ) continue;

    //bool isGenTauMatched = matchesGenTau(*patTau);
    //std::cout << " Pt = " << patTau->pt() << ", eta = " << patTau->eta() << ", phi = " << patTau->phi() << std::endl;
    //std::cout << " isGenTauMatched = " << isGenTauMatched << std::endl;

    if ( requireGenTauMatch_ && !matchesGenTau(*patTau) ) continue;

    double tauJetWeight = getTauJetWeight<pat::Tau>(*patTau, tauJetWeightExtractors_);
    double weight = ( normMethod_ == kNormEvents ) ? evtWeight*(tauJetWeight/tauJetWeightSum) : tauJetWeight;

    fillTauHistograms(*patTau, hTauPt_, hTauEta_, hTauPhi_, weight);
    hTauPtVsEta_->Fill(patTau->eta(), patTau->pt(), weight);
    hTauCharge_->Fill(patTau->charge(), weight);

//--- compare reconstructed tau-jet 
//    to visible decay products on generator level;
//    normalize difference between reconstructed and generated energy
//    to expected energy dependence of resolution
    if ( patTau->genJet() ) {
      hTauEnCompToGen_->Fill((patTau->energy() - patTau->genJet()->energy())/patTau->genJet()->energy(), weight);
      hTauThetaCompToGen_->Fill(patTau->theta() - patTau->genJet()->theta(), weight);
      hTauPhiCompToGen_->Fill(patTau->phi() - patTau->genJet()->phi(), weight);
    }

    int matchingGenParticlePdgId = getMatchingGenParticlePdgId(patTau->p4(), genParticles);
    if ( matchingGenParticlePdgId == -1 ) {
      hTauMatchingGenParticlePdgId_->Fill(-1, weight);
    } else if ( abs(matchingGenParticlePdgId) > 22 ) {
      hTauMatchingGenParticlePdgId_->Fill(24, weight);
    } else {
      hTauMatchingGenParticlePdgId_->Fill(abs(matchingGenParticlePdgId), weight);
    }

    hTauNumTracksSignalCone_->Fill(patTau->signalTracks().size(), weight);
    hTauNumTracksIsoCone_->Fill(patTau->isolationTracks().size(), weight);

    if ( patTau->leadTrack().isAvailable() && patTau->leadTrack().isNonnull() ) {
      hTauLeadTrkPt_->Fill(patTau->leadTrack()->pt(), weight);
      hTauLeadTrkEta_->Fill(patTau->leadTrack()->eta(), weight);
      hTauLeadTrkPhi_->Fill(patTau->leadTrack()->phi(), weight);
      
      hTauLeadTrkMatchDist_->Fill(reco::deltaR(patTau->leadTrack()->momentum(), patTau->p4()), weight);

      if ( vertexSrc_.label() != "" ) {
	edm::Handle<std::vector<reco::Vertex> > recoVertices;
	evt.getByLabel(vertexSrc_, recoVertices);
	if ( recoVertices->size() >= 1 ) {
	  const reco::Vertex& thePrimaryEventVertex = (*recoVertices->begin());
	  hTauLeadTrkIPxy_->Fill(patTau->leadTrack()->dxy(thePrimaryEventVertex.position()), weight);
	  hTauLeadTrkIPz_->Fill(patTau->leadTrack()->dz(thePrimaryEventVertex.position()), weight);
	}
      }

      const reco::HitPattern& hitPattern = patTau->leadTrack()->hitPattern();
      hTauLeadTrkNumHits_->Fill(hitPattern.numberOfValidTrackerHits(), weight);
      hTauLeadTrkNumPixelHits_->Fill(hitPattern.numberOfValidPixelHits(), weight);
      hTauLeadTrkNumStripHits_->Fill(hitPattern.numberOfValidStripHits(), weight);
    }

    hTauDiscriminatorByIsolation_->Fill(patTau->tauID("byIsolation"), weight);
    hTauDiscriminatorByTrackIsolation_->Fill(patTau->tauID("trackIsolation"), weight);
    hTauDiscriminatorByEcalIsolation_->Fill(patTau->tauID("ecalIsolation"), weight);

    hTauDiscriminatorAgainstElectrons_->Fill(patTau->tauID("againstElectron"), weight);
    hTauEmFraction_->Fill(patTau->emFraction(), weight);
    hTauHcalTotOverPLead_->Fill(patTau->hcalTotOverPLead(), weight);
    hTauHcalMaxOverPLead_->Fill(patTau->hcalMaxOverPLead(), weight);
    hTauHcal3x3OverPLead_->Fill(patTau->hcal3x3OverPLead(), weight);
    hTauEcalStripSumEOverPLead_->Fill(patTau->ecalStripSumEOverPLead(), weight);
    hTauBremsRecoveryEOverPLead_->Fill(patTau->bremsRecoveryEOverPLead(), weight);
    hTauCaloEOverPLead_->Fill(patTau->ecalStripSumEOverPLead() + patTau->hcalTotOverPLead(), weight);

    hTauDiscriminatorAgainstMuons_->Fill(patTau->tauID("againstMuon"), weight);
  
    int tauDecayMode = patTau->decayMode();
    hTauRecDecayMode_->Fill(tauDecayMode, weight);

    static std::map<std::string, bool> discrAvailability_hasBeenChecked;
    fillTauDiscriminatorHistogram(hTauDiscriminatorTaNCfrOnePercent_, *patTau, "byTaNCfrOnePercent", 
				  discrAvailability_hasBeenChecked, weight);
    fillTauDiscriminatorHistogram(hTauDiscriminatorTaNCfrHalfPercent_, *patTau, "byTaNCfrHalfPercent",
				  discrAvailability_hasBeenChecked, weight);
    fillTauDiscriminatorHistogram(hTauDiscriminatorTaNCfrQuarterPercent_, *patTau, "byTaNCfrQuarterPercent",
				  discrAvailability_hasBeenChecked, weight);
    fillTauDiscriminatorHistogram(hTauDiscriminatorTaNCfrTenthPercent_, *patTau, "byTaNCfrTenthPercent",
				  discrAvailability_hasBeenChecked, weight);

    MonitorElement* hTauTaNCoutput = 0;
    if ( tauDecayMode == reco::PFTauDecayMode::tauDecay1ChargedPion0PiZero ) {
      hTauTaNCoutput = hTauTaNCoutputOneProngNoPi0s_;
    } else if ( tauDecayMode == reco::PFTauDecayMode::tauDecay1ChargedPion1PiZero ) {
      hTauTaNCoutput = hTauTaNCoutputOneProngOnePi0_;
    } else if ( tauDecayMode == reco::PFTauDecayMode::tauDecay1ChargedPion2PiZero ) {
      hTauTaNCoutput = hTauTaNCoutputOneProngTwoPi0s_;
    } else if ( tauDecayMode == reco::PFTauDecayMode::tauDecay3ChargedPion0PiZero ) {
      hTauTaNCoutput = hTauTaNCoutputThreeProngNoPi0s_;
    } else if ( tauDecayMode == reco::PFTauDecayMode::tauDecay3ChargedPion1PiZero ) {
      hTauTaNCoutput = hTauTaNCoutputThreeProngOnePi0_;
    } 
    if ( hTauTaNCoutput ) {
      fillTauDiscriminatorHistogram(hTauTaNCoutput, *patTau, "byTaNC", 
				    discrAvailability_hasBeenChecked, weight);
    }

    fillTauDiscriminatorHistogram(hTauTaNCoutputTransform_, *patTau, "byTaNCtransform",
				  discrAvailability_hasBeenChecked, weight);
 
    fillTauIsoHistograms(*patTau, weight);
    hTauDeltaRnearestJet_->Fill(getDeltaRnearestJet(patTau->p4(), patJets), weight);
    if ( makeIsoPtConeSizeDepHistograms_ ) fillTauIsoConeSizeDepHistograms(*patTau, weight);

    fillTauIdEfficiencyHistograms(*patTau, weight, hTauIdEfficiencies_, makeTauIdEfficiencyHistograms_);
    fillTauIdEfficiencyHistograms(*patTau, weight, hTauFakeRates_, makeTauFakeRateHistograms_);
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void TauHistManager::bookTauHistograms(DQMStore& dqmStore, MonitorElement*& hTauPt, MonitorElement*& hTauEta, MonitorElement*& hTauPhi, const char* histoSetName)
{
  std::string hTauPtName = std::string(histoSetName).append("Pt");
  hTauPt = dqmStore.book1D(hTauPtName, hTauPtName, 75, 0., 150.);
  
  std::string hTauEtaName = std::string(histoSetName).append("Eta");
  hTauEta = dqmStore.book1D(hTauEtaName, hTauEtaName, 60, -3., +3.);
  
  std::string hTauPhiName = std::string(histoSetName).append("Phi");
  hTauPhi = dqmStore.book1D(hTauPhiName, hTauPhiName, 36, -TMath::Pi(), +TMath::Pi());
}

void TauHistManager::bookTauIsoConeSizeDepHistograms(DQMStore& dqmStore)
{
  for ( unsigned iConeSize = 1; iConeSize <= numTauIsoConeSizes_; ++iConeSize ) {
    std::ostringstream iConeSizeString;
    iConeSizeString << std::setfill('0') << std::setw(2) << iConeSize;
    
    std::string hTauParticleFlowIsoPtConeSizeDepName_i 
      = std::string("TauParticleFlowIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
    hTauParticleFlowIsoPtConeSizeDep_.push_back(dqmStore.book1D(hTauParticleFlowIsoPtConeSizeDepName_i, 
								hTauParticleFlowIsoPtConeSizeDepName_i, 40, 0., 10.));
    std::string hTauPFChargedHadronIsoPtConeSizeDepName_i 
      = std::string("TauChargedHadronIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
    hTauPFChargedHadronIsoPtConeSizeDep_.push_back(dqmStore.book1D(hTauPFChargedHadronIsoPtConeSizeDepName_i, 
								   hTauPFChargedHadronIsoPtConeSizeDepName_i, 40, 0., 10.));
    std::string hTauPFNeutralHadronIsoPtConeSizeDepName_i 
      = std::string("TauPFNeutralHadronIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
    hTauPFNeutralHadronIsoPtConeSizeDep_.push_back(dqmStore.book1D(hTauPFNeutralHadronIsoPtConeSizeDepName_i, 
								   hTauPFNeutralHadronIsoPtConeSizeDepName_i, 40, 0., 10.));
    std::string hTauPFGammaIsoPtConeSizeDepName_i 
      = std::string("TauPFGammaIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
    hTauPFGammaIsoPtConeSizeDep_.push_back(dqmStore.book1D(hTauPFGammaIsoPtConeSizeDepName_i, 
							   hTauPFGammaIsoPtConeSizeDepName_i, 40, 0., 10.));
  }
}

void TauHistManager::bookTauIdEfficiencyHistograms(DQMStore& dqmStore, std::vector<MonitorElement*>& histograms,
						   const char* type, const vstring& labels)
{
  for ( vstring::const_iterator label = labels.begin();
	label != labels.end(); ++label ) {
    std::string histogramName = std::string("Tau").append(type).append(" (").append(*label).append(")");
    histograms.push_back(dqmStore.book1D(histogramName, histogramName, 102, -0.01, 1.01));
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void TauHistManager::fillTauHistograms(const pat::Tau& patTau, MonitorElement* hTauPt, MonitorElement* hTauEta, MonitorElement* hTauPhi, 
				       double weight)
{
  //std::cout << "<TauHistManager::fillTauHistograms>:" << std::endl;

  hTauPt->Fill(patTau.pt(), weight);
  hTauEta->Fill(patTau.eta(), weight);
  hTauPhi->Fill(patTau.phi(), weight);
}

void TauHistManager::fillTauIsoHistograms(const pat::Tau& patTau, double weight)
{
  //std::cout << "<TauHistManager::fillTauIsoHistograms>:" << std::endl;

  hTauTrkIsoPt_->Fill(patTau.trackIso(), weight);
  hTauEcalIsoPt_->Fill(patTau.ecalIso(), weight);
  hTauHcalIsoPt_->Fill(patTau.hcalIso(), weight);
  //hTauIsoSumPt_->Fill(patTau.trackIso() + patTau.ecalIso() + patTau.hcalIso(), weight);
  hTauIsoSumPt_->Fill(patTau.trackIso() + patTau.ecalIso(), weight);
  
  //std::cout << " particleIso = " << patTau.particleIso() << std::endl;
  //std::cout << " chargedParticleIso = " << patTau.chargedParticleIso() << std::endl;
  //std::cout << " neutralParticleIso = " << patTau.neutralParticleIso() << std::endl;
  //std::cout << " gammaParticleIso = " << patTau.gammaParticleIso() << std::endl;
  
  hTauParticleFlowIsoPt_->Fill(patTau.particleIso(), weight);
  hTauPFChargedHadronIsoPt_->Fill(patTau.chargedParticleIso(), weight);
  hTauPFNeutralHadronIsoPt_->Fill(patTau.neutralParticleIso(), weight);
  hTauPFGammaIsoPt_->Fill(patTau.gammaParticleIso(), weight);

  if ( makeIsoPtCtrlHistograms_ ) {
    double sumPtIsolationConePFChargedHadrons = 0.;
    for ( reco::PFCandidateRefVector::const_iterator pfChargedHadron = patTau.isolationPFChargedHadrCands().begin();
	  pfChargedHadron != patTau.isolationPFChargedHadrCands().end(); ++pfChargedHadron ) {
      if ( (*pfChargedHadron)->pt() > 1.0 ) sumPtIsolationConePFChargedHadrons += (*pfChargedHadron)->pt();
    }
    hTauPFChargedHadronIsoPtCtrl_->Fill(sumPtIsolationConePFChargedHadrons, patTau.chargedParticleIso(), weight); 
    
    double sumPtIsolationConePFGammas = 0.;
    for ( reco::PFCandidateRefVector::const_iterator pfGamma = patTau.isolationPFGammaCands().begin();
	  pfGamma != patTau.isolationPFGammaCands().end(); ++pfGamma ) {
      if ( (*pfGamma)->pt() > 1.5 ) sumPtIsolationConePFGammas += (*pfGamma)->pt();
    }
    hTauPFGammaIsoPtCtrl_->Fill(sumPtIsolationConePFGammas, patTau.gammaParticleIso(), weight);
  }

  for ( reco::TrackRefVector::const_iterator isolationTrack = patTau.isolationTracks().begin();
	isolationTrack != patTau.isolationTracks().end(); ++isolationTrack ) {	  
    hTauTrkIsoEnProfile_->Fill((*isolationTrack)->p(), weight);
    hTauTrkIsoPtProfile_->Fill((*isolationTrack)->pt(), weight);
    hTauTrkIsoEtaDistProfile_->Fill(TMath::Abs(patTau.eta() - (*isolationTrack)->eta()), weight);
    hTauTrkIsoPhiDistProfile_->Fill(TMath::Abs(patTau.phi() - (*isolationTrack)->phi()), weight);
  }
}

void TauHistManager::fillTauIsoConeSizeDepHistograms(const pat::Tau& patTau, double weight)
{
  //std::cout << "<TauHistManager::fillTauIsoConeSizeDepHistograms>:" << std::endl;

  for ( unsigned iConeSize = 1; iConeSize <= numTauIsoConeSizes_; ++iConeSize ) {
    double isoConeSize_i = iConeSize*tauIsoConeSizeIncr_;

    if ( patTau.isoDeposit(pat::ParticleIso) ) {
      double tauParticleFlowIsoDeposit_i 
	= patTau.isoDeposit(pat::ParticleIso)->countWithin(isoConeSize_i, tauParticleFlowIsoParam_, false);
      hTauParticleFlowIsoPtConeSizeDep_[iConeSize - 1]->Fill(tauParticleFlowIsoDeposit_i, weight);
    }
    
    if ( patTau.isoDeposit(pat::ChargedParticleIso) ) {
      double tauPFChargedHadronIsoDeposit_i 
	= patTau.isoDeposit(pat::ChargedParticleIso)->countWithin(isoConeSize_i, tauParticleFlowIsoParam_, false);
      hTauPFChargedHadronIsoPtConeSizeDep_[iConeSize - 1]->Fill(tauPFChargedHadronIsoDeposit_i, weight);
    }
    
    if ( patTau.isoDeposit(pat::NeutralParticleIso) ) {
      double tauPFNeutralHadronIsoDeposit_i 
	= patTau.isoDeposit(pat::NeutralParticleIso)->countWithin(isoConeSize_i, tauParticleFlowIsoParam_, false);
      hTauPFNeutralHadronIsoPtConeSizeDep_[iConeSize - 1]->Fill(tauPFNeutralHadronIsoDeposit_i, weight);
    }

    if ( patTau.isoDeposit(pat::GammaParticleIso) ) {
      double tauPFGammaIsoDeposit_i 
	= patTau.isoDeposit(pat::GammaParticleIso)->countWithin(isoConeSize_i, tauParticleFlowIsoParam_, false);
      hTauPFGammaIsoPtConeSizeDep_[iConeSize - 1]->Fill(tauPFGammaIsoDeposit_i, weight);
    }
  }
}

void TauHistManager::fillTauIdEfficiencyHistograms(const pat::Tau& patTau, double weight,
						   std::vector<MonitorElement*>& histograms, const vstring& labels)
{
  assert(histograms.size() == labels.size());

  unsigned numHistograms = histograms.size();
  for ( unsigned iHistogram = 0; iHistogram < numHistograms; ++iHistogram ) {
    histograms[iHistogram]->Fill(patTau.efficiency(labels[iHistogram].data()).value(), weight);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, TauHistManager, "TauHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, TauHistManager, "TauHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<TauHistManager> TauAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(TauAnalyzer);
