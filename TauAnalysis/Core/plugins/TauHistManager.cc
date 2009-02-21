#include "TauAnalysis/Core/plugins/TauHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/IsolationAlgos/interface/IsoDepositVetoFactory.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"

#include "PhysicsTools/Utilities/interface/deltaR.h"

#include <TMath.h>

#include <stdlib.h>

pat::IsolationKeys isoDepositKeyParticleFlowIso = pat::ParticleIso;
pat::IsolationKeys isoDepositKeyPFChargedHadronIso = pat::ChargedParticleIso;
pat::IsolationKeys isoDepositKeyPFNeutralHadronIso = pat::NeutralParticleIso;
pat::IsolationKeys isoDepositKeyPFGammaIso = pat::GammaParticleIso;

bool matchesGenTau(const pat::Tau& patTau)
{
  bool isGenTauMatched = false;
  for ( std::vector<reco::GenParticleRef>::const_iterator it = patTau.genParticleRefs().begin(); 
	it != patTau.genParticleRefs().end(); ++it ) {
    const reco::GenParticleRef& genParticle = (*it);
    if ( genParticle->pdgId() == -15 || genParticle->pdgId() == +15 ) isGenTauMatched = true;
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
  //std::cout << " tauSrc = " << tauSrc_ << std::endl;

  vertexSrc_ = ( cfg.exists("vertexSource") ) ? cfg.getParameter<edm::InputTag>("vertexSource") : edm::InputTag();
  if ( vertexSrc_.label() == "" ) {
    edm::LogWarning("TauHistManager") << " Configuration parameter 'vertexSource' not specified" 
				      << " --> Impact Parameter histograms will NOT be plotted !!";
  }
  //std::cout << " vertexSrc = " << vertexSrc_ << std::endl;

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
}

TauHistManager::~TauHistManager()
{
//--- nothing to be done yet...
}

void TauHistManager::bookHistograms(const edm::EventSetup& setup)
{
  //std::cout << "<TauHistManager::bookHistograms>:" << std::endl;

  if ( edm::Service<DQMStore>().isAvailable() ) {
    DQMStore& dqmStore = (*edm::Service<DQMStore>());

    dqmStore.setCurrentFolder(dqmDirectory_store_);

//--- book histograms for Pt, eta and phi distributions
//    of taus passing all id. and isolation selections
    bookTauHistograms(dqmStore, hTauPt_, hTauEta_, hTauPhi_, "Tau");
    hTauPtVsEta_ = dqmStore.book2D("TauPtVsEta", "TauPtVsEta", 24, -3., +3., 30, 0., 150.);

    hTauEnCompToGen_ = dqmStore.book1D("TauEnCompToGen", "TauEnCompToGen", 100, -2.50, +2.50);
    hTauThetaCompToGen_ = dqmStore.book1D("TauThetaCompToGen", "TauThetaCompToGen", 200, -0.050, +0.050);
    hTauPhiCompToGen_ = dqmStore.book1D("TauPhiCompToGen", "TauPhiCompToGen", 200, -0.050, +0.050);

    bookTauHistograms(dqmStore, hTauLeadTrkPt_, hTauLeadTrkEta_, hTauLeadTrkPhi_, "TauLeadTrk");
    hTauLeadTrkMatchDist_ = dqmStore.book1D("TauLeadTrkMatchDist", "TauLeadTrkMatchDist", 100, -0.500, 0.500);
    hTauLeadTrkIPxy_ = dqmStore.book1D("TauLeadTrkIPxy", "TauLeadTrkIPxy", 100, -0.100, 0.100);
    hTauLeadTrkIPz_ = dqmStore.book1D("TauLeadTrkIPz", "TauLeadTrkIPz", 100, -1.0, 1.0);

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

void TauHistManager::fillHistograms(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{  
  //std::cout << "<TauHistManager::fillHistograms>:" << std::endl; 

  edm::Handle<std::vector<pat::Tau> > patTaus;
  iEvent.getByLabel(tauSrc_, patTaus);

  //std::cout << " patTaus.size = " << patTaus->size() << std::endl;

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
  
  std::cout << " particleIso = " << patTau.particleIso() << std::endl;
  std::cout << " chargedParticleIso = " << patTau.chargedParticleIso() << std::endl;
  std::cout << " neutralParticleIso = " << patTau.neutralParticleIso() << std::endl;
  std::cout << " gammaParticleIso = " << patTau.gammaParticleIso() << std::endl;
  
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
      reco::isodeposit::AbsVetos tauTrkIsoParam;
      tauTrkIsoParam.push_back(IsoDepositVetoFactory::make("0.02"));
      tauTrkIsoParam.push_back(IsoDepositVetoFactory::make("Threshold(1.0)"));
      double tauTrkIsoDeposit_i = patTau.trackerIsoDeposit()->countWithin(isoConeSize_i, tauTrkIsoParam, false);
      hTauTrkIsoPtConeSizeDep_[iConeSize - 1]->Fill(tauTrkIsoDeposit_i);
    }
    
    reco::isodeposit::AbsVetos tauIsoParam;
    tauIsoParam.push_back(IsoDepositVetoFactory::make("0.0"));
    tauIsoParam.push_back(IsoDepositVetoFactory::make("0.0"));

    if ( patTau.ecalIsoDeposit() ) {
      double tauEcalIsoDeposit_i = patTau.ecalIsoDeposit()->countWithin(isoConeSize_i, tauIsoParam, false);
      hTauEcalIsoPtConeSizeDep_[iConeSize - 1]->Fill(tauEcalIsoDeposit_i);
    }
    
    if ( patTau.hcalIsoDeposit() ) {
      double tauHcalIsoDeposit_i = patTau.hcalIsoDeposit()->countWithin(isoConeSize_i, tauIsoParam, false);
      hTauHcalIsoPtConeSizeDep_[iConeSize - 1]->Fill(tauHcalIsoDeposit_i);
    }

    reco::isodeposit::AbsVetos tauParticleFlowIsoParam;
    tauParticleFlowIsoParam.push_back(IsoDepositVetoFactory::make("0.0"));
    tauParticleFlowIsoParam.push_back(IsoDepositVetoFactory::make("Threshold(0.5)"));

    if ( patTau.isoDeposit(isoDepositKeyParticleFlowIso) ) {
      double tauParticleFlowIsoDeposit_i 
	= patTau.isoDeposit(isoDepositKeyParticleFlowIso)->countWithin(isoConeSize_i, tauParticleFlowIsoParam, false);
      hTauParticleFlowIsoPtConeSizeDep_[iConeSize - 1]->Fill(tauParticleFlowIsoDeposit_i);
    }
    
    if ( patTau.isoDeposit(isoDepositKeyPFChargedHadronIso) ) {
      double tauPFChargedHadronIsoDeposit_i 
	= patTau.isoDeposit(isoDepositKeyPFChargedHadronIso)->countWithin(isoConeSize_i, tauParticleFlowIsoParam, false);
      hTauPFChargedHadronIsoPtConeSizeDep_[iConeSize - 1]->Fill(tauPFChargedHadronIsoDeposit_i);
    }
    
    if ( patTau.isoDeposit(isoDepositKeyPFNeutralHadronIso) ) {
      double tauPFNeutralHadronIsoDeposit_i 
	= patTau.isoDeposit(isoDepositKeyPFNeutralHadronIso)->countWithin(isoConeSize_i, tauParticleFlowIsoParam, false);
      hTauPFNeutralHadronIsoPtConeSizeDep_[iConeSize - 1]->Fill(tauPFNeutralHadronIsoDeposit_i);
    }

    if ( patTau.isoDeposit(isoDepositKeyPFGammaIso) ) {
      double tauPFGammaIsoDeposit_i 
	= patTau.isoDeposit(isoDepositKeyPFGammaIso)->countWithin(isoConeSize_i, tauParticleFlowIsoParam, false);
      hTauPFGammaIsoPtConeSizeDep_[iConeSize - 1]->Fill(tauPFGammaIsoDeposit_i);
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(HistManagerPluginFactory, TauHistManager, "TauHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<TauHistManager> TauAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(TauAnalyzer);
