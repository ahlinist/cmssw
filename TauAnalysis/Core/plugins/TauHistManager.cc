#include "TauAnalysis/Core/plugins/TauHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/IsolationAlgos/interface/IsoDepositVetoFactory.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include <TMath.h>

bool matchesGenTau(const pat::Tau& patTau)
{
  bool isGenTauMatched = false;
  for ( std::vector<reco::GenParticleRef>::const_iterator it = patTau.genParticleRefs().begin(); it != patTau.genParticleRefs().end(); ++it ) {
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

  dqmDirectory_store_ = cfg.getParameter<std::string>("dqmDirectory_store");
  //std::cout << " dqmDirectory_store = " << dqmDirectory_store_ << std::endl;

  requireGenTauMatch_ = cfg.getParameter<bool>("requireGenTauMatch");
  //std::cout << " requireGenTauMatch = " << requireGenTauMatch_ << std::endl;

  numTauIsoConeSizes_ = 10;
  tauIsoConeSizeIncr_ = 0.1;
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

    bookTauHistograms(dqmStore, hTauLdgTrkPt_, hTauLdgTrkEta_, hTauLdgTrkPhi_, "Tau");

    hTauTrkIsoPt_ = dqmStore.book1D("TauTrkIsoPt", "TauTrkIsoPt", 100, 0., 20.);    
    hTauEcalIsoPt_ = dqmStore.book1D("TauEcalIsoPt", "TauEcalIsoPt", 100, 0., 20.);
    hTauHcalIsoPt_ = dqmStore.book1D("TauHcalIsoPt", "TauHcalIsoPt", 100, 0., 20.);

    for ( unsigned iConeSize = 1; iConeSize <= numTauIsoConeSizes_; ++iConeSize ) {
      std::ostringstream iConeSizeString;
      iConeSizeString << std::setfill('0') << std::setw(2) << iConeSize;

      std::string hTauTrkIsoPtConeSizeDepName_i = std::string("TauTrkIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
      hTauTrkIsoPtConeSizeDep_.push_back(dqmStore.book1D(hTauTrkIsoPtConeSizeDepName_i, hTauTrkIsoPtConeSizeDepName_i, 100, 0., 20.));

      std::string hTauEcalIsoPtConeSizeDepName_i = std::string("TauEcalIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
      hTauEcalIsoPtConeSizeDep_.push_back(dqmStore.book1D(hTauEcalIsoPtConeSizeDepName_i, hTauEcalIsoPtConeSizeDepName_i, 100, 0., 20.));

      std::string hTauHcalIsoPtConeSizeDepName_i = std::string("TauHcalIsoPtConeSizeDep").append("_").append(iConeSizeString.str());
      hTauHcalIsoPtConeSizeDep_.push_back(dqmStore.book1D(hTauHcalIsoPtConeSizeDepName_i, hTauHcalIsoPtConeSizeDepName_i, 100, 0., 20.));
    }
  }
}

void TauHistManager::fillHistograms(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{  
  //std::cout << "<TauHistManager::fillHistograms>:" << std::endl; 

  edm::Handle<std::vector<pat::Tau> > patTaus;
  iEvent.getByLabel(tauSrc_, patTaus);

  //std::cout << " patTaus.size = " << patTaus->size() << std::endl;

  fillTauHistograms(*patTaus, hTauPt_, hTauEta_, hTauPhi_);
  for ( std::vector<pat::Tau>::const_iterator patTau = patTaus->begin(); 
	patTau != patTaus->end(); ++patTau ) {

    //bool isGenTauMatched = matchesGenTau(*patTau);
    //std::cout << " Pt = " << patTau->pt() << ", eta = " << patTau->eta() << ", phi = " << patTau->phi() << std::endl;
    //std::cout << " isGenTauMatched = " << isGenTauMatched << std::endl;

    if ( requireGenTauMatch_ && !matchesGenTau(*patTau) ) continue;

    hTauPtVsEta_->Fill(patTau->eta(), patTau->pt());
  }

  fillTauIsoHistograms(*patTaus);
  fillTauIsoConeSizeDepHistograms(*patTaus);
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

void TauHistManager::fillTauHistograms(const std::vector<pat::Tau>& patTaus, MonitorElement* hTauPt, MonitorElement* hTauEta, MonitorElement* hTauPhi)
{
  //std::cout << "<TauHistManager::fillTauHistograms>:" << std::endl;
  //if ( selVar != "" ) std::cout << " selVar = " << selVar << std::endl;

  for ( std::vector<pat::Tau>::const_iterator patTau = patTaus.begin(); 
	patTau != patTaus.end(); ++patTau ) {

    if ( requireGenTauMatch_ && (!matchesGenTau(*patTau)) ) continue;

    hTauPt->Fill(patTau->pt());
    hTauEta->Fill(patTau->eta());
    hTauPhi->Fill(patTau->phi());
  }
}

void TauHistManager::fillTauIsoHistograms(const std::vector<pat::Tau>& patTaus)
{
  //std::cout << "<TauHistManager::fillTauIsoHistograms>:" << std::endl;

  for ( std::vector<pat::Tau>::const_iterator patTau = patTaus.begin(); 
	patTau != patTaus.end(); ++patTau ) {

    if ( requireGenTauMatch_ && (!matchesGenTau(*patTau)) ) continue;

    hTauTrkIsoPt_->Fill(patTau->trackIso());
    hTauEcalIsoPt_->Fill(patTau->ecalIso());
    hTauHcalIsoPt_->Fill(patTau->hcalIso());
  }
}

void TauHistManager::fillTauIsoConeSizeDepHistograms(const std::vector<pat::Tau>& patTaus)
{
  //std::cout << "<TauHistManager::fillTauIsoConeSizeDepHistograms>:" << std::endl;

  for ( std::vector<pat::Tau>::const_iterator patTau = patTaus.begin(); 
	patTau != patTaus.end(); ++patTau ) {
    
    if ( requireGenTauMatch_ && (!matchesGenTau(*patTau)) ) continue;

    if ( !patTau->trackerIsoDeposit() ||
	 !patTau->ecalIsoDeposit()    ||
	 !patTau->hcalIsoDeposit() ) {
/*
 CV: IsoDeposits are not computed for pat::Taus in CMSSW_2_2_3 yet
   --> no need to print-out this warning for each event !!

      edm::LogError ("TauHistManager::fillTauIsoConeSizeDepHistograms") << " No IsoDeposits associated to pat::Tau with"
									<< " E = " << patTau->energy() << ","
									<< " theta = " << patTau->theta()*180./TMath::Pi() << "," 
									<< " phi = " << patTau->phi()*180./TMath::Pi() << " !!";
 */
      continue;
    }

    for ( unsigned iConeSize = 1; iConeSize <= numTauIsoConeSizes_; ++iConeSize ) {
      float isoConeSize_i = iConeSize*tauIsoConeSizeIncr_;

      reco::isodeposit::AbsVetos tauTrkIsoParam;
      tauTrkIsoParam.push_back(IsoDepositVetoFactory::make("0.02"));
      tauTrkIsoParam.push_back(IsoDepositVetoFactory::make("Threshold(1.0)"));
      float tauTrkIsoDeposit_i = patTau->trackerIsoDeposit()->countWithin(isoConeSize_i, tauTrkIsoParam, false);
      hTauTrkIsoPtConeSizeDep_[iConeSize - 1]->Fill(tauTrkIsoDeposit_i);
      
      reco::isodeposit::AbsVetos tauEcalIsoParam;
      tauEcalIsoParam.push_back(IsoDepositVetoFactory::make("0.0"));
      tauEcalIsoParam.push_back(IsoDepositVetoFactory::make("0.0"));
      float tauEcalIsoDeposit_i = patTau->ecalIsoDeposit()->countWithin(isoConeSize_i, tauEcalIsoParam, false);
      hTauEcalIsoPtConeSizeDep_[iConeSize - 1]->Fill(tauEcalIsoDeposit_i);
      
      reco::isodeposit::AbsVetos tauHcalIsoParam;
      tauHcalIsoParam.push_back(IsoDepositVetoFactory::make("0.0"));
      tauHcalIsoParam.push_back(IsoDepositVetoFactory::make("0.0"));
      float tauHcalIsoDeposit_i = patTau->hcalIsoDeposit()->countWithin(isoConeSize_i, tauHcalIsoParam, false);
      hTauHcalIsoPtConeSizeDep_[iConeSize - 1]->Fill(tauHcalIsoDeposit_i);
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(HistManagerPluginFactory, TauHistManager, "TauHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<TauHistManager> TauAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(TauAnalyzer);
