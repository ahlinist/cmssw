#include "TauAnalysis/Core/plugins/TauHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/IsolationAlgos/interface/IsoDepositVetoFactory.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"
#include "DataFormats/TauReco/interface/PFTauDecayMode.h"

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
    if ( it->ref().isNonnull() && it->ref().isValid() ) {
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

  numTauIsoConeSizes_ = 15;
  tauIsoConeSizeIncr_ = 0.1;
  numTauIsoPtThresholds_ = 4;
  tauIsoPtThresholdIncr_ = 0.5;

//--- create "veto" objects for computation of IsoDeposit sums
  tauTrkIsoParam_.push_back(IsoDepositVetoFactory::make("0.02"));
  tauTrkIsoParam_.push_back(IsoDepositVetoFactory::make("Threshold(1.0)"));

  tauEcalIsoParam_.push_back(IsoDepositVetoFactory::make("0.0"));
  tauEcalIsoParam_.push_back(IsoDepositVetoFactory::make("Threshold(0.0)"));

  tauHcalIsoParam_.push_back(IsoDepositVetoFactory::make("0.0"));
  tauHcalIsoParam_.push_back(IsoDepositVetoFactory::make("Threshold(0.0)"));

  tauParticleFlowIsoParam_.push_back(IsoDepositVetoFactory::make("0.0"));
  tauParticleFlowIsoParam_.push_back(IsoDepositVetoFactory::make("Threshold(0.5)"));
}

TauHistManager::~TauHistManager()
{
//--- delete "veto" objects for computation of IsoDeposit sums
  clearIsoParam(tauTrkIsoParam_);
  clearIsoParam(tauEcalIsoParam_);
  clearIsoParam(tauHcalIsoParam_);
  clearIsoParam(tauParticleFlowIsoParam_);
}

void TauHistManager::bookHistograms(const edm::EventSetup& setup)
{
  //std::cout << "<TauHistManager::bookHistograms>:" << std::endl;

  if ( edm::Service<DQMStore>().isAvailable() ) {
    DQMStore& dqmStore = (*edm::Service<DQMStore>());

    dqmStore.setCurrentFolder(dqmDirectory_store_);

//--- book histogram for number of tau-jets in each event
    hNumTaus_ = dqmStore.book1D("NumTaus", "NumTaus", 10, -0.5, 9.5);

//--- book histograms for Pt, eta and phi distributions
//    of tau-jets passing all id. and isolation selections
    bookTauHistograms(dqmStore, hTauPt_, hTauEta_, hTauPhi_, "Tau");
    hTauPtVsEta_ = dqmStore.book2D("TauPtVsEta", "TauPtVsEta", 24, -3., +3., 30, 0., 150.);

    hTauEnCompToGen_ = dqmStore.book1D("TauEnCompToGen", "TauEnCompToGen", 100, -2.50, +2.50);
    hTauThetaCompToGen_ = dqmStore.book1D("TauThetaCompToGen", "TauThetaCompToGen", 200, -0.050, +0.050);
    hTauPhiCompToGen_ = dqmStore.book1D("TauPhiCompToGen", "TauPhiCompToGen", 200, -0.050, +0.050);

    hTauNumTracksSignalCone_ = dqmStore.book1D("TauNumTracksSignalCone", "TauNumTracksSignalCone", 10, -0.5, 9.5);
    hTauNumTracksIsoCone_ = dqmStore.book1D("TauNumTracksIsoCone", "TauNumTracksIsoCone", 20, -0.5, 19.5);

    bookTauHistograms(dqmStore, hTauLeadTrkPt_, hTauLeadTrkEta_, hTauLeadTrkPhi_, "TauLeadTrk");
    hTauLeadTrkMatchDist_ = dqmStore.book1D("TauLeadTrkMatchDist", "TauLeadTrkMatchDist", 100, -0.500, 0.500);
    hTauLeadTrkIPxy_ = dqmStore.book1D("TauLeadTrkIPxy", "TauLeadTrkIPxy", 100, -0.100, 0.100);
    hTauLeadTrkIPz_ = dqmStore.book1D("TauLeadTrkIPz", "TauLeadTrkIPz", 100, -1.0, 1.0);

    hTauDiscriminatorAgainstElectrons_ = dqmStore.book1D("TauDiscriminatorAgainstElectrons", 
							 "TauDiscriminatorAgainstElectrons", 102, -0.01, 1.01);
    hTauDiscriminatorAgainstMuons_ = dqmStore.book1D("TauDiscriminatorAgainstMuons", 
						     "TauDiscriminatorAgainstMuons", 102, -0.01, 1.01);

/*
  add histograms of:
   o TauGenJet decay mode
   o reco TauDecayMode
   o reco TauDecayMode (y-ayis) vs. TauGenJet decay mode (x-axis)
   o TauGenJet mass
   o pat::Tau mass
   o reco TauDecayMode mass
   o reco TauDecayMode mass separately for different decay modes
 */
    
    hTauRecDecayMode_ = dqmStore.book1D("TauRecDecayMode", "TauRecDecayMode", 25, -0.5, 24.5);

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

    hTauDiscriminatorTaNCfrOnePercent_ = dqmStore.book1D("TauDiscriminatorTaNCfrOnePercent",
							 "TauDiscriminatorTaNCfrOnePercent", 102, -0.01, 1.01);
    hTauDiscriminatorTaNCfrHalfPercent_ = dqmStore.book1D("TauDiscriminatorTaNCfrHalfPercent",
							  "TauDiscriminatorTaNCfrHalfPercent", 102, -0.01, 1.01);
    hTauDiscriminatorTaNCfrQuarterPercent_ = dqmStore.book1D("TauDiscriminatorTaNCfrQuarterPercent",
							     "TauDiscriminatorTaNCfrQuarterPercent", 102, -0.01, 1.01);
    hTauDiscriminatorTaNCfrTenthPercent_ = dqmStore.book1D("TauDiscriminatorTaNCfrTenthPercent",
							   "TauDiscriminatorTaNCfrTenthPercent", 102, -0.01, 1.01);
    
    hTauTrkIsoPt_ = dqmStore.book1D("TauTrkIsoPt", "TauTrkIsoPt", 100, 0., 20.);    
    hTauEcalIsoPt_ = dqmStore.book1D("TauEcalIsoPt", "TauEcalIsoPt", 100, 0., 20.);
    hTauHcalIsoPt_ = dqmStore.book1D("TauHcalIsoPt", "TauHcalIsoPt", 100, 0., 20.);
    hTauIsoSumPt_ = dqmStore.book1D("TauIsoSumPt", "TauIsoSumPt", 100, 0., 20.);

    hTauParticleFlowIsoPt_ = dqmStore.book1D("TauParticleFlowIsoPt", "TauParticleFlowIsoPt", 100, 0., 20.);    
    hTauPFChargedHadronIsoPt_ = dqmStore.book1D("TauPFChargedHadronIsoPt", "TauPFChargedHadronIsoPt", 100, 0., 20.);   
    hTauPFNeutralHadronIsoPt_ = dqmStore.book1D("TauPFNeutralHadronIsoPt", "TauPFNeutralHadronIsoPt", 100, 0., 20.);   
    hTauPFGammaIsoPt_ = dqmStore.book1D("TauPFGammaIsoPt", "TauPFGammaIsoPt", 100, 0., 20.);  

    hTauTrkIsoEnProfile_ = dqmStore.book1D("TauTrkIsoEnProfile", "TauTrkIsoEnProfile", 100, 0., 10.);
    hTauTrkIsoPtProfile_ = dqmStore.book1D("TauTrkIsoPtProfile", "TauTrkIsoPtProfile", 100, 0., 10.);
    hTauTrkIsoEtaDistProfile_ = dqmStore.book1D("TauTrkIsoEtaDistProfile", "TauTrkIsoEtaDistProfile", 15, 0., 1.5);
    hTauTrkIsoPhiDistProfile_ = dqmStore.book1D("TauTrkIsoPhiDistProfile", "TauTrkIsoPhiDistProfile", 15, 0., 1.5);

    for ( unsigned iConeSize = 1; iConeSize <= numTauIsoConeSizes_; ++iConeSize ) {
      std::ostringstream iConeSizeString;
      iConeSizeString << std::setfill('0') << std::setw(2) << iConeSize;

      std::string hTauTrkIsoPtConeSizeDepName_i = std::string("TauTrkIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
      hTauTrkIsoPtConeSizeDep_.push_back(dqmStore.book1D(hTauTrkIsoPtConeSizeDepName_i, 
							 hTauTrkIsoPtConeSizeDepName_i, 100, 0., 20.));
      std::string hTauEcalIsoPtConeSizeDepName_i = std::string("TauEcalIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
      hTauEcalIsoPtConeSizeDep_.push_back(dqmStore.book1D(hTauEcalIsoPtConeSizeDepName_i, 
							  hTauEcalIsoPtConeSizeDepName_i, 100, 0., 20.));
      std::string hTauHcalIsoPtConeSizeDepName_i = std::string("TauHcalIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
      hTauHcalIsoPtConeSizeDep_.push_back(dqmStore.book1D(hTauHcalIsoPtConeSizeDepName_i, 
							  hTauHcalIsoPtConeSizeDepName_i, 100, 0., 20.));

      std::string hTauParticleFlowIsoPtConeSizeDepName_i 
	= std::string("TauParticleFlowIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
      hTauParticleFlowIsoPtConeSizeDep_.push_back(dqmStore.book1D(hTauParticleFlowIsoPtConeSizeDepName_i, 
								  hTauParticleFlowIsoPtConeSizeDepName_i, 100, 0., 20.));
      std::string hTauPFChargedHadronIsoPtConeSizeDepName_i 
	= std::string("TauChargedHadronIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
      hTauPFChargedHadronIsoPtConeSizeDep_.push_back(dqmStore.book1D(hTauPFChargedHadronIsoPtConeSizeDepName_i, 
								     hTauPFChargedHadronIsoPtConeSizeDepName_i, 100, 0., 20.));
      std::string hTauPFNeutralHadronIsoPtConeSizeDepName_i 
	= std::string("TauPFNeutralHadronIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
      hTauPFNeutralHadronIsoPtConeSizeDep_.push_back(dqmStore.book1D(hTauPFNeutralHadronIsoPtConeSizeDepName_i, 
								     hTauPFNeutralHadronIsoPtConeSizeDepName_i, 100, 0., 20.));
      std::string hTauPFGammaIsoPtConeSizeDepName_i 
	= std::string("TauPFGammaIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
      hTauPFGammaIsoPtConeSizeDep_.push_back(dqmStore.book1D(hTauPFGammaIsoPtConeSizeDepName_i, 
							     hTauPFGammaIsoPtConeSizeDepName_i, 100, 0., 20.));
    }
  }
}

void TauHistManager::fillTauDiscriminatorHistogram(MonitorElement* h, const pat::Tau& patTau, const char* discrName,
						   std::map<std::string, bool>& discrAvailability_hasBeenChecked)
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
  
  if ( patTau.isTauIDAvailable(discrName) ) h->Fill(patTau.tauID(discrName));
}

void TauHistManager::fillHistograms(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{  
  //std::cout << "<TauHistManager::fillHistograms>:" << std::endl; 

  edm::Handle<std::vector<pat::Tau> > patTaus;
  iEvent.getByLabel(tauSrc_, patTaus);

  //std::cout << " patTaus.size = " << patTaus->size() << std::endl;
  hNumTaus_->Fill(patTaus->size());

  int patTauIndex = 0;
  for ( std::vector<pat::Tau>::const_iterator patTau = patTaus->begin(); 
	patTau != patTaus->end(); ++patTau, ++patTauIndex ) {

    if ( tauIndicesToPlot_.size() > 0 ) {
      bool isIndexed = false;
      for ( vint::const_iterator it = tauIndicesToPlot_.begin();
	    it != tauIndicesToPlot_.end(); ++it ) {
	if ( (*it) == patTauIndex ) isIndexed = true;
      }
      if ( !isIndexed ) continue;
    }

    //bool isGenTauMatched = matchesGenTau(*patTau);
    //std::cout << " Pt = " << patTau->pt() << ", eta = " << patTau->eta() << ", phi = " << patTau->phi() << std::endl;
    //std::cout << " isGenTauMatched = " << isGenTauMatched << std::endl;

    if ( requireGenTauMatch_ && !matchesGenTau(*patTau) ) continue;

    fillTauHistograms(*patTau, hTauPt_, hTauEta_, hTauPhi_);
    hTauPtVsEta_->Fill(patTau->eta(), patTau->pt());

//--- compare reconstructed tau-jet 
//    to visible decay products on generator level;
//    normalize difference between reconstructed and generated energy
//    to expected energy dependence of resolution
    if ( patTau->genJet() ) {
      hTauEnCompToGen_->Fill((patTau->energy() - patTau->genJet()->energy())/patTau->genJet()->energy());
      hTauThetaCompToGen_->Fill(patTau->theta() - patTau->genJet()->theta());
      hTauPhiCompToGen_->Fill(patTau->phi() - patTau->genJet()->phi());
    }

    hTauNumTracksSignalCone_->Fill(patTau->signalTracks().size());
    hTauNumTracksIsoCone_->Fill(patTau->isolationTracks().size());

    if ( patTau->leadTrack().isAvailable() && patTau->leadTrack().isNonnull() ) {
      hTauLeadTrkPt_->Fill(patTau->leadTrack()->pt());
      hTauLeadTrkEta_->Fill(patTau->leadTrack()->eta());
      hTauLeadTrkPhi_->Fill(patTau->leadTrack()->phi());
      
      hTauLeadTrkMatchDist_->Fill(reco::deltaR(patTau->leadTrack()->momentum(), patTau->p4()));

      if ( vertexSrc_.label() != "" ) {
	edm::Handle<std::vector<reco::Vertex> > recoVertices;
	iEvent.getByLabel(vertexSrc_, recoVertices);
	if ( recoVertices->size() >= 1 ) {
	  const reco::Vertex& thePrimaryEventVertex = (*recoVertices->begin());
	  hTauLeadTrkIPxy_->Fill(patTau->leadTrack()->dxy(thePrimaryEventVertex.position()));
	  hTauLeadTrkIPz_->Fill(patTau->leadTrack()->dz(thePrimaryEventVertex.position()));
	}
      }
    }

    hTauDiscriminatorAgainstElectrons_->Fill(patTau->tauID("againstElectron"));
    hTauDiscriminatorAgainstMuons_->Fill(patTau->tauID("againstMuon"));
  
    int tauDecayMode = patTau->decayMode();
    hTauRecDecayMode_->Fill(tauDecayMode);

    static std::map<std::string, bool> discrAvailability_hasBeenChecked;
    fillTauDiscriminatorHistogram(hTauDiscriminatorTaNCfrOnePercent_, *patTau, "byTaNCfrOnePercent", 
				  discrAvailability_hasBeenChecked);
    fillTauDiscriminatorHistogram(hTauDiscriminatorTaNCfrHalfPercent_, *patTau, "byTaNCfrHalfPercent",
				  discrAvailability_hasBeenChecked);
    fillTauDiscriminatorHistogram(hTauDiscriminatorTaNCfrQuarterPercent_, *patTau, "byTaNCfrQuarterPercent",
				  discrAvailability_hasBeenChecked);
    fillTauDiscriminatorHistogram(hTauDiscriminatorTaNCfrTenthPercent_, *patTau, "byTaNCfrTenthPercent",
				  discrAvailability_hasBeenChecked);

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
				    discrAvailability_hasBeenChecked);
    }
 
    fillTauIsoHistograms(*patTau);
    fillTauIsoConeSizeDepHistograms(*patTau);
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

//
//-----------------------------------------------------------------------------------------------------------------------
//

void TauHistManager::fillTauHistograms(const pat::Tau& patTau, MonitorElement* hTauPt, MonitorElement* hTauEta, MonitorElement* hTauPhi)
{
  //std::cout << "<TauHistManager::fillTauHistograms>:" << std::endl;

  hTauPt->Fill(patTau.pt());
  hTauEta->Fill(patTau.eta());
  hTauPhi->Fill(patTau.phi());
}

void TauHistManager::fillTauIsoHistograms(const pat::Tau& patTau)
{
  //std::cout << "<TauHistManager::fillTauIsoHistograms>:" << std::endl;
    
  hTauTrkIsoPt_->Fill(patTau.trackIso());
  hTauEcalIsoPt_->Fill(patTau.ecalIso());
  hTauHcalIsoPt_->Fill(patTau.hcalIso());
  hTauIsoSumPt_->Fill(patTau.trackIso() + patTau.ecalIso() + patTau.hcalIso());
  
  //std::cout << " particleIso = " << patTau.particleIso() << std::endl;
  //std::cout << " chargedParticleIso = " << patTau.chargedParticleIso() << std::endl;
  //std::cout << " neutralParticleIso = " << patTau.neutralParticleIso() << std::endl;
  //std::cout << " gammaParticleIso = " << patTau.gammaParticleIso() << std::endl;
  
  hTauParticleFlowIsoPt_->Fill(patTau.particleIso());
  hTauPFChargedHadronIsoPt_->Fill(patTau.chargedParticleIso());
  hTauPFNeutralHadronIsoPt_->Fill(patTau.neutralParticleIso());
  hTauPFGammaIsoPt_->Fill(patTau.gammaParticleIso());

  for ( reco::TrackRefVector::const_iterator isolationTrack = patTau.isolationTracks().begin();
	isolationTrack != patTau.isolationTracks().end(); ++isolationTrack ) {	  
    hTauTrkIsoEnProfile_->Fill((*isolationTrack)->p());
    hTauTrkIsoPtProfile_->Fill((*isolationTrack)->pt());
    hTauTrkIsoEtaDistProfile_->Fill(TMath::Abs(patTau.eta() - (*isolationTrack)->eta()));
    hTauTrkIsoPhiDistProfile_->Fill(TMath::Abs(patTau.phi() - (*isolationTrack)->phi()));
  }
}

void TauHistManager::fillTauIsoConeSizeDepHistograms(const pat::Tau& patTau)
{
  //std::cout << "<TauHistManager::fillTauIsoConeSizeDepHistograms>:" << std::endl;

  for ( unsigned iConeSize = 1; iConeSize <= numTauIsoConeSizes_; ++iConeSize ) {
    double isoConeSize_i = iConeSize*tauIsoConeSizeIncr_;
    
    if ( patTau.trackerIsoDeposit() ) {
      double tauTrkIsoDeposit_i = patTau.trackerIsoDeposit()->countWithin(isoConeSize_i, tauTrkIsoParam_, false);
      hTauTrkIsoPtConeSizeDep_[iConeSize - 1]->Fill(tauTrkIsoDeposit_i);
    }

    if ( patTau.ecalIsoDeposit() ) {
      double tauEcalIsoDeposit_i = patTau.ecalIsoDeposit()->countWithin(isoConeSize_i, tauEcalIsoParam_, false);
      hTauEcalIsoPtConeSizeDep_[iConeSize - 1]->Fill(tauEcalIsoDeposit_i);
    }
    
    if ( patTau.hcalIsoDeposit() ) {
      double tauHcalIsoDeposit_i = patTau.hcalIsoDeposit()->countWithin(isoConeSize_i, tauHcalIsoParam_, false);
      hTauHcalIsoPtConeSizeDep_[iConeSize - 1]->Fill(tauHcalIsoDeposit_i);
    }

    if ( patTau.isoDeposit(pat::ParticleIso) ) {
      double tauParticleFlowIsoDeposit_i 
	= patTau.isoDeposit(pat::ParticleIso)->countWithin(isoConeSize_i, tauParticleFlowIsoParam_, false);
      hTauParticleFlowIsoPtConeSizeDep_[iConeSize - 1]->Fill(tauParticleFlowIsoDeposit_i);
    }
    
    if ( patTau.isoDeposit(pat::ChargedParticleIso) ) {
      double tauPFChargedHadronIsoDeposit_i 
	= patTau.isoDeposit(pat::ChargedParticleIso)->countWithin(isoConeSize_i, tauParticleFlowIsoParam_, false);
      hTauPFChargedHadronIsoPtConeSizeDep_[iConeSize - 1]->Fill(tauPFChargedHadronIsoDeposit_i);
    }
    
    if ( patTau.isoDeposit(pat::NeutralParticleIso) ) {
      double tauPFNeutralHadronIsoDeposit_i 
	= patTau.isoDeposit(pat::NeutralParticleIso)->countWithin(isoConeSize_i, tauParticleFlowIsoParam_, false);
      hTauPFNeutralHadronIsoPtConeSizeDep_[iConeSize - 1]->Fill(tauPFNeutralHadronIsoDeposit_i);
    }

    if ( patTau.isoDeposit(pat::GammaParticleIso) ) {
      double tauPFGammaIsoDeposit_i 
	= patTau.isoDeposit(pat::GammaParticleIso)->countWithin(isoConeSize_i, tauParticleFlowIsoParam_, false);
      hTauPFGammaIsoPtConeSizeDep_[iConeSize - 1]->Fill(tauPFGammaIsoDeposit_i);
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(HistManagerPluginFactory, TauHistManager, "TauHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<TauHistManager> TauAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(TauAnalyzer);
