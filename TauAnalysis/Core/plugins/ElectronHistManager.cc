#include "TauAnalysis/Core/plugins/ElectronHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/IsolationAlgos/interface/IsoDepositVetoFactory.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

bool matchesGenElectron(const pat::Electron& patElectron)
{
  //std::cout << "<matchesGenElectron>:" << std::endl;

  bool isGenElectronMatched = false;
  for ( std::vector<reco::GenParticleRef>::const_iterator it = patElectron.genParticleRefs().begin(); 
	it != patElectron.genParticleRefs().end(); ++it ) {
    if ( it->ref().isNonnull() && it->ref().isValid() ) {
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

  dqmDirectory_store_ = cfg.getParameter<std::string>("dqmDirectory_store");
  //std::cout << " dqmDirectory_store = " << dqmDirectory_store_ << std::endl;

  requireGenElectronMatch_ = cfg.getParameter<bool>("requireGenElectronMatch");
  //std::cout << " requireGenElectronMatch = " << requireGenElectronMatch_ << std::endl;

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
}

ElectronHistManager::~ElectronHistManager()
{
//--- nothing to be done yet...
}

void ElectronHistManager::bookHistograms(const edm::EventSetup& setup)
{
  //std::cout << "<ElectronHistManager::bookHistograms>:" << std::endl;

  if ( edm::Service<DQMStore>().isAvailable() ) {
    DQMStore& dqmStore = (*edm::Service<DQMStore>());

    dqmStore.setCurrentFolder(dqmDirectory_store_);

//--- book histograms for Pt, eta and phi distributions
//    of electrons passing all id. and isolation selections
    bookElectronHistograms(dqmStore, hElectronPt_, hElectronEta_, hElectronPhi_, "Electron");
    hElectronPtVsEta_ = dqmStore.book2D("ElectronPtVsEta", "ElectronPtVsEta", 24, -3., +3., 30, 0., 150.);

    hElectronEnCompToGen_ = dqmStore.book1D("ElectronEnCompToGen", "ElectronEnCompToGen", 100, -0.50, +0.50);
    hElectronThetaCompToGen_ = dqmStore.book1D("ElectronThetaCompToGen", "ElectronThetaCompToGen", 200, -0.010, +0.010);
    hElectronPhiCompToGen_ = dqmStore.book1D("ElectronPhiCompToGen", "ElectronPhiCompToGen", 200, -0.010, +0.010);

    hElectronTrackPt_ = dqmStore.book1D("ElectronTrackPt", "ElectronTrackPt", 75, 0., 150.);
    hElectronTrackIPxy_ = dqmStore.book1D("ElectronTrackIPxy", "ElectronTrackIPxy", 100, -0.100, 0.100);
    hElectronTrackIPz_ = dqmStore.book1D("ElectronTrackIPz", "ElectronTrackIPz", 100, -1.0, 1.0);

    hElectronSuperclEnOverTrackMomBarrel_ = dqmStore.book1D("ElectronSuperclEnOverTrackMomBarrel", "ElectronSuperclEnOverTrackMomBarrel", 50, 0., 5.);
    hElectronSuperclEnOverTrackMomEndcap_ = dqmStore.book1D("ElectronSuperclEnOverTrackMomEndcap", "ElectronSuperclEnOverTrackMomEndcap", 50, 0., 5.);

    hElectronIdRobust_ = dqmStore.book1D("ElectronIdRobust", "ElectronIdRobust", 2, -0.5, 1.5);

    hElectronTrkIsoPt_ = dqmStore.book1D("ElectronTrkIsoPt", "ElectronTrkIsoPt", 100, 0., 20.);    
    hElectronEcalIsoPt_ = dqmStore.book1D("ElectronEcalIsoPt", "ElectronEcalIsoPt", 100, 0., 20.);
    hElectronEcalIsoPtBarrel_ = dqmStore.book1D("ElectronEcalIsoPtBarrel", "ElectronEcalIsoPtBarrel", 100, 0., 20.);
    hElectronEcalIsoPtEndcap_ = dqmStore.book1D("ElectronEcalIsoPtEndcap", "ElectronEcalIsoPtEndcap", 100, 0., 20.);
    hElectronHcalIsoPt_ = dqmStore.book1D("ElectronHcalIsoPt", "ElectronHcalIsoPt", 100, 0., 20.);
    hElectronIsoSumPt_ = dqmStore.book1D("ElectronIsoSumPt", "ElectronIsoSumPt", 100, 0., 20.);

    hElectronTrkIsoValProfile_ = dqmStore.book1D("ElectronTrkIsoValProfile", "ElectronTrkIsoValProfile", 100, 0., 10.);
    hElectronTrkIsoEtaDistProfile_ = dqmStore.book1D("ElectronTrkIsoEtaDistProfile", "ElectronTrkIsoEtaDistProfile", 15, 0., 1.5);
    hElectronTrkIsoPhiDistProfile_ = dqmStore.book1D("ElectronTrkIsoPhiDistProfile", "ElectronTrkIsoPhiDistProfile", 15, 0., 1.5);
    
    hElectronEcalIsoValProfile_ = dqmStore.book1D("ElectronEcalIsoValProfile", "ElectronEcalIsoValProfile", 100, 0., 10.);
    hElectronEcalIsoEtaDistProfile_ = dqmStore.book1D("ElectronEcalIsoEtaDistProfile", "ElectronEcalIsoEtaDistProfile", 15, 0., 1.5);
    hElectronEcalIsoPhiDistProfile_ = dqmStore.book1D("ElectronEcalIsoPhiDistProfile", "ElectronEcalIsoPhiDistProfile", 15, 0., 1.5);
    
    hElectronHcalIsoValProfile_ = dqmStore.book1D("ElectronHcalIsoValProfile", "ElectronHcalIsoValProfile", 100, 0., 10.);
    hElectronHcalIsoEtaDistProfile_ = dqmStore.book1D("ElectronHcalIsoEtaDistProfile", "ElectronHcalIsoEtaDistProfile", 15, 0., 1.5);
    hElectronHcalIsoPhiDistProfile_  = dqmStore.book1D("ElectronHcalIsoPhiDistProfile", "ElectronHcalIsoPhiDistProfile", 15, 0., 1.5);
    
    for ( unsigned iConeSize = 1; iConeSize <= numElectronIsoConeSizes_; ++iConeSize ) {
      std::ostringstream iConeSizeString;
      iConeSizeString << std::setfill('0') << std::setw(2) << iConeSize;

      std::string hElectronTrkIsoPtConeSizeDepName_i = std::string("ElectronTrkIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
      hElectronTrkIsoPtConeSizeDep_.push_back(dqmStore.book1D(hElectronTrkIsoPtConeSizeDepName_i, hElectronTrkIsoPtConeSizeDepName_i, 
							      100, 0., 20.));
      std::string hElectronEcalIsoPtConeSizeDepName_i = std::string("ElectronEcalIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
      hElectronEcalIsoPtConeSizeDep_.push_back(dqmStore.book1D(hElectronEcalIsoPtConeSizeDepName_i, hElectronEcalIsoPtConeSizeDepName_i, 
							       100, 0., 20.));
      std::string hElectronHcalIsoPtConeSizeDepName_i = std::string("ElectronHcalIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
      hElectronHcalIsoPtConeSizeDep_.push_back(dqmStore.book1D(hElectronHcalIsoPtConeSizeDepName_i, hElectronHcalIsoPtConeSizeDepName_i, 
							       100, 0., 20.));
    }
  }
}



void ElectronHistManager::fillHistograms(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{  
  //std::cout << "<ElectronHistManager::fillHistograms>:" << std::endl; 

  edm::Handle<std::vector<pat::Electron> > patElectrons;
  iEvent.getByLabel(electronSrc_, patElectrons);

  //std::cout << " patElectrons.size = " << patElectrons->size() << std::endl;

  fillElectronHistograms(*patElectrons, hElectronPt_, hElectronEta_, hElectronPhi_);

  for ( std::vector<pat::Electron>::const_iterator patElectron = patElectrons->begin(); 
	patElectron != patElectrons->end(); ++patElectron ) {
  
    //bool isGenElectronMatched = matchesGenElectron(*patElectron);
    //std::cout << " Pt = " << patElectron->pt() << ", eta = " << patElectron->eta() << ", phi = " << patElectron->phi() << std::endl;
    //std::cout << " isGenElectronMatched = " << isGenElectronMatched << std::endl;

    if ( requireGenElectronMatch_ && !matchesGenElectron(*patElectron) ) continue;

    hElectronPtVsEta_->Fill(patElectron->eta(), patElectron->pt());

//--- compare reconstructed electron to generator level one;
//    normalize difference between reconstructed and generated energy
//    to expected energy dependence of resolution
     if ( patElectron->genLepton() ) {
       hElectronEnCompToGen_->Fill((patElectron->energy() - patElectron->genLepton()->energy())
				   /TMath::Sqrt(patElectron->genLepton()->energy()));
       hElectronThetaCompToGen_->Fill(patElectron->theta() - patElectron->genLepton()->theta());
       hElectronPhiCompToGen_->Fill(patElectron->phi() - patElectron->genLepton()->phi());
    }
    
    if ( patElectron->gsfTrack().isAvailable() && !patElectron->gsfTrack().isNull() ) {
      hElectronTrackPt_->Fill(patElectron->gsfTrack()->pt());
      if ( vertexSrc_.label() != "" ) {
	edm::Handle<std::vector<reco::Vertex> > recoVertices;
	iEvent.getByLabel(vertexSrc_, recoVertices);
	if ( recoVertices->size() >= 1 ) {
	  const reco::Vertex& thePrimaryEventVertex = (*recoVertices->begin());
	  hElectronTrackIPxy_->Fill(patElectron->gsfTrack()->dxy(thePrimaryEventVertex.position()));
	  hElectronTrackIPz_->Fill(patElectron->gsfTrack()->dz(thePrimaryEventVertex.position()));
	}
      }
    }

    if ( patElectron->superCluster().isAvailable() && patElectron->superCluster().isNonnull() ) {
      if ( TMath::Abs(patElectron->superCluster()->eta()) < electronEtaMaxBarrel_ ) 
	hElectronSuperclEnOverTrackMomBarrel_->Fill(patElectron->eSuperClusterOverP());
      if ( TMath::Abs(patElectron->superCluster()->eta()) > electronEtaMinEndcap_ ) 
      hElectronSuperclEnOverTrackMomEndcap_->Fill(patElectron->eSuperClusterOverP());
    }

    hElectronIdRobust_->Fill(patElectron->electronID("robust"));
  }

  fillElectronIsoHistograms(*patElectrons);
  fillElectronIsoConeSizeDepHistograms(*patElectrons);
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void ElectronHistManager::bookElectronHistograms(DQMStore& dqmStore, MonitorElement*& hElectronPt, MonitorElement*& hElectronEta, MonitorElement*& hElectronPhi, const char* histoSetName)
{
  std::string hElectronPtName = std::string(histoSetName).append("Pt");
  hElectronPt = dqmStore.book1D(hElectronPtName, hElectronPtName, 75, 0., 150.);
  
  std::string hElectronEtaName = std::string(histoSetName).append("Eta");
  hElectronEta = dqmStore.book1D(hElectronEtaName, hElectronEtaName, 60, -3., +3.);
  
  std::string hElectronPhiName = std::string(histoSetName).append("Phi");
  hElectronPhi = dqmStore.book1D(hElectronPhiName, hElectronPhiName, 36, -TMath::Pi(), +TMath::Pi());
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void ElectronHistManager::fillElectronHistograms(const std::vector<pat::Electron>& patElectrons, 
						 MonitorElement* hElectronPt, MonitorElement* hElectronEta, MonitorElement* hElectronPhi)
{
  //std::cout << "<ElectronHistManager::fillElectronHistograms>:" << std::endl;

  for ( std::vector<pat::Electron>::const_iterator patElectron = patElectrons.begin(); patElectron != patElectrons.end(); ++patElectron ) {

    if ( requireGenElectronMatch_ && (!matchesGenElectron(*patElectron)) ) continue;

    hElectronPt->Fill(patElectron->pt());
    hElectronEta->Fill(patElectron->eta());
    hElectronPhi->Fill(patElectron->phi());
  }
}

void ElectronHistManager::fillElectronIsoHistograms(const std::vector<pat::Electron>& patElectrons)
{
  //std::cout << "<ElectronHistManager::fillElectronIsoHistograms>:" << std::endl;

  for ( std::vector<pat::Electron>::const_iterator patElectron = patElectrons.begin(); 
	patElectron != patElectrons.end(); ++patElectron ) {

    if ( requireGenElectronMatch_ && (!matchesGenElectron(*patElectron)) ) continue;

    hElectronTrkIsoPt_->Fill(patElectron->trackIso());
    hElectronEcalIsoPt_->Fill(patElectron->ecalIso());
    if ( patElectron->superCluster().isAvailable() && patElectron->superCluster().isNonnull() ) {
      if ( TMath::Abs(patElectron->superCluster()->eta()) < electronEtaMaxBarrel_ ) 
	hElectronEcalIsoPtBarrel_->Fill(patElectron->ecalIso());
      if ( TMath::Abs(patElectron->superCluster()->eta()) > electronEtaMinEndcap_ ) 
	hElectronEcalIsoPtEndcap_->Fill(patElectron->ecalIso());
    }
    hElectronHcalIsoPt_->Fill(patElectron->hcalIso());
    hElectronIsoSumPt_->Fill(patElectron->trackIso() + patElectron->ecalIso() + patElectron->hcalIso());

    fillLeptonIsoDepositHistograms(patElectron->trackerIsoDeposit(), 
				   hElectronTrkIsoValProfile_, hElectronTrkIsoEtaDistProfile_, hElectronTrkIsoPhiDistProfile_);
    fillLeptonIsoDepositHistograms(patElectron->ecalIsoDeposit(), 
				   hElectronEcalIsoValProfile_, hElectronEcalIsoEtaDistProfile_, hElectronEcalIsoPhiDistProfile_);
    fillLeptonIsoDepositHistograms(patElectron->hcalIsoDeposit(), 
				   hElectronHcalIsoValProfile_, hElectronHcalIsoEtaDistProfile_, hElectronHcalIsoPhiDistProfile_);
  }
}

void ElectronHistManager::fillElectronIsoConeSizeDepHistograms(const std::vector<pat::Electron>& patElectrons)
{
  //std::cout << "<ElectronHistManager::fillElectronIsoConeSizeDepHistograms>:" << std::endl;

  for ( std::vector<pat::Electron>::const_iterator patElectron = patElectrons.begin(); 
	patElectron != patElectrons.end(); ++patElectron ) {
    
    if ( requireGenElectronMatch_ && (!matchesGenElectron(*patElectron)) ) continue;

    for ( unsigned iConeSize = 1; iConeSize <= numElectronIsoConeSizes_; ++iConeSize ) {
      float isoConeSize_i = iConeSize*electronIsoConeSizeIncr_;

      reco::isodeposit::AbsVetos electronTrkIsoParam;
      electronTrkIsoParam.push_back(IsoDepositVetoFactory::make("0.02"));
      electronTrkIsoParam.push_back(IsoDepositVetoFactory::make("Threshold(1.0)"));
      float electronTrkIsoDeposit_i = patElectron->trackerIsoDeposit()->countWithin(isoConeSize_i, electronTrkIsoParam, false);
      hElectronTrkIsoPtConeSizeDep_[iConeSize - 1]->Fill(electronTrkIsoDeposit_i);
      
      reco::isodeposit::AbsVetos electronEcalIsoParam;
      electronEcalIsoParam.push_back(IsoDepositVetoFactory::make("0.0"));
      electronEcalIsoParam.push_back(IsoDepositVetoFactory::make("0.0"));
      float electronEcalIsoDeposit_i = patElectron->ecalIsoDeposit()->countWithin(isoConeSize_i, electronEcalIsoParam, false);
      hElectronEcalIsoPtConeSizeDep_[iConeSize - 1]->Fill(electronEcalIsoDeposit_i);
      
      reco::isodeposit::AbsVetos electronHcalIsoParam;
      electronHcalIsoParam.push_back(IsoDepositVetoFactory::make("0.0"));
      electronHcalIsoParam.push_back(IsoDepositVetoFactory::make("0.0"));
      float electronHcalIsoDeposit_i = patElectron->hcalIsoDeposit()->countWithin(isoConeSize_i, electronHcalIsoParam, false);
      hElectronHcalIsoPtConeSizeDep_[iConeSize - 1]->Fill(electronHcalIsoDeposit_i);
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(HistManagerPluginFactory, ElectronHistManager, "ElectronHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<ElectronHistManager> ElectronAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(ElectronAnalyzer);
