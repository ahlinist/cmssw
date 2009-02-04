#include "TauAnalysis/Core/plugins/MuonHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/IsolationAlgos/interface/IsoDepositVetoFactory.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include <TMath.h>

bool matchesGenMuon(const pat::Muon& patMuon)
{
  //std::cout << "<matchesGenMuon>:" << std::endl;

  bool isGenMuonMatched = false;
  for ( std::vector<reco::GenParticleRef>::const_iterator it = patMuon.genParticleRefs().begin(); it != patMuon.genParticleRefs().end(); ++it ) {
    if ( it->ref().isNonnull() && it->ref().isValid() ) {
      const reco::GenParticleRef& genParticle = (*it);
      if ( genParticle->pdgId() == -13 || genParticle->pdgId() == +13 ) isGenMuonMatched = true;
    } else {
      edm::LogWarning("matchesGenMuon") << " edm::Ref of genParticle associated to pat::Muon is invalid !!";
    }
  }
  return isGenMuonMatched;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

MuonHistManager::MuonHistManager(const edm::ParameterSet& cfg)
{
  //std::cout << "<MuonHistManager::MuonHistManager>:" << std::endl;

  muonSrc_ = cfg.getParameter<edm::InputTag>("muonSource");
  //std::cout << " muonSrc = " << muonSrc_ << std::endl;

  dqmDirectory_store_ = cfg.getParameter<std::string>("dqmDirectory_store");
  //std::cout << " dqmDirectory_store = " << dqmDirectory_store_ << std::endl;

  requireGenMuonMatch_ = cfg.getParameter<bool>("requireGenMuonMatch");
  //std::cout << " requireGenMuonMatch = " << requireGenMuonMatch_ << std::endl;

  numMuonIsoConeSizes_ = 10;
  muonIsoConeSizeIncr_ = 0.1;
}

MuonHistManager::~MuonHistManager()
{
//--- nothing to be done yet...
}

void MuonHistManager::bookHistograms(const edm::EventSetup& setup)
{
  //std::cout << "<MuonHistManager::bookHistograms>:" << std::endl;

  if ( edm::Service<DQMStore>().isAvailable() ) {
    DQMStore& dqmStore = (*edm::Service<DQMStore>());

    dqmStore.setCurrentFolder(dqmDirectory_store_);

//--- book histograms for Pt, eta and phi distributions
//    of muons passing all id. and isolation selections
    bookMuonHistograms(dqmStore, hMuonPt_, hMuonEta_, hMuonPhi_, "Muon");
    hMuonPtVsEta_ = dqmStore.book2D("MuonPtVsEta", "MuonPtVsEta", 24, -3., +3., 30, 0., 150.);

    hMuonTrackIP_ = dqmStore.book1D("MuonTrackIP", "MuonTrackIP", 100, -0.100, 0.100);

    hMuonEcalDeposits_ = dqmStore.book1D("MuonEcalDeposits", "MuonEcalDeposits", 100, 0., 20.);
    hMuonHcalDeposits_ = dqmStore.book1D("MuonHcalDeposits", "MuonHcalDeposits", 100, 0., 20.);
    hMuonCaloDeposits_ = dqmStore.book1D("MuonCaloDeposits", "MuonCaloDeposits", 100, 0., 20.);
    hMuonCaloCompatibility_ = dqmStore.book1D("MuonCaloCompatibility", "MuonCaloCompatibility", 102, -0.01, 1.01);

    hMuonNumberOfChambers_ = dqmStore.book1D("MuonNumberOfChambers", "MuonNumberOfChambers", 25, -0.5, 24.5);
    hMuonSegmentCompatibility_ = dqmStore.book1D("MuonSegmentCompatibility", "MuonSegmentCompatibility", 102, -0.01, 1.01);

    hMuonTrkIsoPt_ = dqmStore.book1D("MuonTrkIsoPt", "MuonTrkIsoPt", 100, 0., 20.);    
    hMuonEcalIsoPt_ = dqmStore.book1D("MuonEcalIsoPt", "MuonEcalIsoPt", 100, 0., 20.);
    hMuonHcalIsoPt_ = dqmStore.book1D("MuonHcalIsoPt", "MuonHcalIsoPt", 100, 0., 20.);

    for ( unsigned iConeSize = 1; iConeSize <= numMuonIsoConeSizes_; ++iConeSize ) {
      std::ostringstream iConeSizeString;
      iConeSizeString << std::setfill('0') << std::setw(2) << iConeSize;

      std::string hMuonTrkIsoPtConeSizeDepName_i = std::string("MuonTrkIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
      hMuonTrkIsoPtConeSizeDep_.push_back(dqmStore.book1D(hMuonTrkIsoPtConeSizeDepName_i, hMuonTrkIsoPtConeSizeDepName_i, 100, 0., 20.));

      std::string hMuonEcalIsoPtConeSizeDepName_i = std::string("MuonEcalIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
      hMuonEcalIsoPtConeSizeDep_.push_back(dqmStore.book1D(hMuonEcalIsoPtConeSizeDepName_i, hMuonEcalIsoPtConeSizeDepName_i, 100, 0., 20.));

      std::string hMuonHcalIsoPtConeSizeDepName_i = std::string("MuonHcalIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
      hMuonHcalIsoPtConeSizeDep_.push_back(dqmStore.book1D(hMuonHcalIsoPtConeSizeDepName_i, hMuonHcalIsoPtConeSizeDepName_i, 100, 0., 20.));
    }
  }
}

void MuonHistManager::fillHistograms(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{  
  //std::cout << "<MuonHistManager::fillHistograms>:" << std::endl; 

  edm::Handle<std::vector<pat::Muon> > patMuons;
  iEvent.getByLabel(muonSrc_, patMuons);

  //std::cout << " patMuons.size = " << patMuons->size() << std::endl;

  fillMuonHistograms(*patMuons, hMuonPt_, hMuonEta_, hMuonPhi_);

  for ( std::vector<pat::Muon>::const_iterator patMuon = patMuons->begin(); 
	patMuon != patMuons->end(); ++patMuon ) {
  
    //bool isGenMuonMatched = matchesGenMuon(*patMuon);
    //std::cout << " Pt = " << patMuon->pt() << ", eta = " << patMuon->eta() << ", phi = " << patMuon->phi() << std::endl;
    //std::cout << " isGenMuonMatched = " << isGenMuonMatched << std::endl;

    if ( requireGenMuonMatch_ && !matchesGenMuon(*patMuon) ) continue;

    hMuonPtVsEta_->Fill(patMuon->eta(), patMuon->pt());

    if ( patMuon->innerTrack().isAvailable() && !patMuon->innerTrack().isNull() ) {
      hMuonTrackIP_->Fill(patMuon->innerTrack()->d0());
    }
  }

  for ( std::vector<pat::Muon>::const_iterator patMuon = patMuons->begin(); 
	patMuon != patMuons->end(); ++patMuon ) {
    const reco::Muon* recoMuon = dynamic_cast<const reco::Muon*>(patMuon->originalObject());

    if ( requireGenMuonMatch_ && (!matchesGenMuon(*patMuon)) ) continue;

    if ( recoMuon != NULL ) {
      if ( recoMuon->isEnergyValid() ) {
	hMuonEcalDeposits_->Fill(recoMuon->calEnergy().em);
	hMuonHcalDeposits_->Fill(recoMuon->calEnergy().had + recoMuon->calEnergy().ho);
	hMuonCaloDeposits_->Fill(recoMuon->calEnergy().em + recoMuon->calEnergy().had + recoMuon->calEnergy().ho);
      }
      if ( recoMuon->isCaloCompatibilityValid() ) hMuonCaloCompatibility_->Fill(recoMuon->caloCompatibility());
      
      hMuonNumberOfChambers_->Fill(recoMuon->numberOfChambers());
      float segmentCompatibility = muon::segmentCompatibility(*recoMuon);
      hMuonSegmentCompatibility_->Fill(segmentCompatibility);
    } else {
      edm::LogError("analyze") << " Failed to access reco::Muon linked to pat::Muon object --> some histograms will NOT be filled !!";
    }
  }

  fillMuonIsoHistograms(*patMuons);
  fillMuonIsoConeSizeDepHistograms(*patMuons);
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void MuonHistManager::bookMuonHistograms(DQMStore& dqmStore, MonitorElement*& hMuonPt, MonitorElement*& hMuonEta, MonitorElement*& hMuonPhi, const char* histoSetName)
{
  std::string hMuonPtName = std::string(histoSetName).append("Pt");
  hMuonPt = dqmStore.book1D(hMuonPtName, hMuonPtName, 75, 0., 150.);
  
  std::string hMuonEtaName = std::string(histoSetName).append("Eta");
  hMuonEta = dqmStore.book1D(hMuonEtaName, hMuonEtaName, 60, -3., +3.);
  
  std::string hMuonPhiName = std::string(histoSetName).append("Phi");
  hMuonPhi = dqmStore.book1D(hMuonPhiName, hMuonPhiName, 36, -TMath::Pi(), +TMath::Pi());
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void MuonHistManager::fillMuonHistograms(const std::vector<pat::Muon>& patMuons, MonitorElement* hMuonPt, MonitorElement* hMuonEta, MonitorElement* hMuonPhi)
{
  //std::cout << "<MuonHistManager::fillMuonHistograms>:" << std::endl;

  for ( std::vector<pat::Muon>::const_iterator patMuon = patMuons.begin(); 
	patMuon != patMuons.end(); ++patMuon ) {

    if ( requireGenMuonMatch_ && (!matchesGenMuon(*patMuon)) ) continue;

    hMuonPt->Fill(patMuon->pt());
    hMuonEta->Fill(patMuon->eta());
    hMuonPhi->Fill(patMuon->phi());
  }
}

void MuonHistManager::fillMuonIsoHistograms(const std::vector<pat::Muon>& patMuons)
{
  //std::cout << "<MuonHistManager::fillMuonIsoHistograms>:" << std::endl;

  for ( std::vector<pat::Muon>::const_iterator patMuon = patMuons.begin(); 
	patMuon != patMuons.end(); ++patMuon ) {

    if ( requireGenMuonMatch_ && (!matchesGenMuon(*patMuon)) ) continue;

    hMuonTrkIsoPt_->Fill(patMuon->trackIso());
    if ( patMuon->trackIso() == 0. ) {
      hMuonEcalIsoPt_->Fill(patMuon->ecalIso());
      if ( patMuon->ecalIso() < 1. ) {
        hMuonHcalIsoPt_->Fill(patMuon->hcalIso());
      }
    }
  }
}

void MuonHistManager::fillMuonIsoConeSizeDepHistograms(const std::vector<pat::Muon>& patMuons)
{
  //std::cout << "<MuonHistManager::fillMuonIsoConeSizeDepHistograms>:" << std::endl;

  for ( std::vector<pat::Muon>::const_iterator patMuon = patMuons.begin(); 
	patMuon != patMuons.end(); ++patMuon ) {
    
    if ( requireGenMuonMatch_ && (!matchesGenMuon(*patMuon)) ) continue;

    for ( unsigned iConeSize = 1; iConeSize <= numMuonIsoConeSizes_; ++iConeSize ) {
      float isoConeSize_i = iConeSize*muonIsoConeSizeIncr_;

      reco::isodeposit::AbsVetos muonTrkIsoParam;
      muonTrkIsoParam.push_back(IsoDepositVetoFactory::make("0.02"));
      muonTrkIsoParam.push_back(IsoDepositVetoFactory::make("Threshold(1.0)"));
      float muonTrkIsoDeposit_i = patMuon->trackerIsoDeposit()->countWithin(isoConeSize_i, muonTrkIsoParam, false);
      hMuonTrkIsoPtConeSizeDep_[iConeSize - 1]->Fill(muonTrkIsoDeposit_i);
      
      reco::isodeposit::AbsVetos muonEcalIsoParam;
      muonEcalIsoParam.push_back(IsoDepositVetoFactory::make("0.0"));
      muonEcalIsoParam.push_back(IsoDepositVetoFactory::make("0.0"));
      float muonEcalIsoDeposit_i = patMuon->ecalIsoDeposit()->countWithin(isoConeSize_i, muonEcalIsoParam, false);
      hMuonEcalIsoPtConeSizeDep_[iConeSize - 1]->Fill(muonEcalIsoDeposit_i);
      
      reco::isodeposit::AbsVetos muonHcalIsoParam;
      muonHcalIsoParam.push_back(IsoDepositVetoFactory::make("0.0"));
      muonHcalIsoParam.push_back(IsoDepositVetoFactory::make("0.0"));
      float muonHcalIsoDeposit_i = patMuon->hcalIsoDeposit()->countWithin(isoConeSize_i, muonHcalIsoParam, false);
      hMuonHcalIsoPtConeSizeDep_[iConeSize - 1]->Fill(muonHcalIsoDeposit_i);
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(HistManagerPluginFactory, MuonHistManager, "MuonHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<MuonHistManager> MuonAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(MuonAnalyzer);
