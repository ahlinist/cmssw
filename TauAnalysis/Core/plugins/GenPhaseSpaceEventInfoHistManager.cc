#include "TauAnalysis/Core/plugins/GenPhaseSpaceEventInfoHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "AnalysisDataFormats/TauAnalysis/interface/GenPhaseSpaceEventInfo.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

GenPhaseSpaceEventInfoHistManager::GenPhaseSpaceEventInfoHistManager(const edm::ParameterSet& cfg)
{
  //std::cout << "<GenPhaseSpaceEventInfoHistManager::GenPhaseSpaceEventInfoHistManager>:" << std::endl;

  src_ = cfg.getParameter<edm::InputTag>("source");
  //std::cout << " src = " << src_ << std::endl;

  dqmDirectory_store_ = cfg.getParameter<std::string>("dqmDirectory_store");
  //std::cout << " dqmDirectory_store = " << dqmDirectory_store_ << std::endl;
}

GenPhaseSpaceEventInfoHistManager::~GenPhaseSpaceEventInfoHistManager()
{
//--- nothing to be done yet...
}

void GenPhaseSpaceEventInfoHistManager::bookHistograms(const edm::EventSetup& setup)
{
  //std::cout << "<GenPhaseSpaceEventInfoHistManager::bookHistograms>:" << std::endl;

  if ( edm::Service<DQMStore>().isAvailable() ) {
    DQMStore& dqmStore = (*edm::Service<DQMStore>());

    dqmStore.setCurrentFolder(dqmDirectory_store_);
    
    hPtHat_ = dqmStore.book1D("PtHat", "PtHat", 101, -0.5, +100.5);
    hLeadingElectronPt_ = dqmStore.book1D("LeadingElectronPt", "LeadingElectronPt", 101, -0.5, +100.5);
    hLeadingMuonPt_ = dqmStore.book1D("LeadingMuonPt", "LeadingMuonPt", 101, -0.5, +100.5);
    hLeadingTauLeptonPt_ = dqmStore.book1D("LeadingTauLeptonPt", "LeadingTauLeptonPt", 101, -0.5, +100.5);
    hLeadingElectronPtVsPtHat_ = dqmStore.book2D("LeadingElectronPtVsPtHat", "LeadingElectronPtVsPtHat", 
						 101, -0.5, +100.5, 101, -0.5, +100.5);
    hLeadingMuonPtVsPtHat_ = dqmStore.book2D("LeadingMuonPtVsPtHat", "LeadingMuonPtVsPtHat", 
					     101, -0.5, +100.5, 101, -0.5, +100.5);
    hLeadingTauLeptonPtVsPtHat_ = dqmStore.book2D("LeadingTauLeptonPtVsPtHat", "LeadingTauLeptonPtVsPtHat", 
						  101, -0.5, +100.5, 101, -0.5, +100.5);
  }
}

void GenPhaseSpaceEventInfoHistManager::fillHistograms(const edm::Event& iEvent, const edm::EventSetup& iSetup)

{  
  //std::cout << "<GenPhaseSpaceEventInfoHistManager::fillHistograms>:" << std::endl; 

  edm::Handle<GenPhaseSpaceEventInfo> genPhaseSpaceEventInfo;
  iEvent.getByLabel(src_, genPhaseSpaceEventInfo);
  hPtHat_->Fill(genPhaseSpaceEventInfo->ptHat());
  hLeadingElectronPt_->Fill(genPhaseSpaceEventInfo->leadingGenElectron().pt());
  hLeadingMuonPt_->Fill(genPhaseSpaceEventInfo->leadingGenMuon().pt());
  hLeadingTauLeptonPt_->Fill(genPhaseSpaceEventInfo->leadingGenTauLepton().pt());
  hLeadingElectronPtVsPtHat_->Fill(genPhaseSpaceEventInfo->ptHat(), 
				   genPhaseSpaceEventInfo->leadingGenElectron().pt());
  hLeadingMuonPtVsPtHat_->Fill(genPhaseSpaceEventInfo->ptHat(), 
			       genPhaseSpaceEventInfo->leadingGenMuon().pt());
  hLeadingTauLeptonPtVsPtHat_->Fill(genPhaseSpaceEventInfo->ptHat(), 
				    genPhaseSpaceEventInfo->leadingGenTauLepton().pt());
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(HistManagerPluginFactory, GenPhaseSpaceEventInfoHistManager, "GenPhaseSpaceEventInfoHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<GenPhaseSpaceEventInfoHistManager> GenPhaseSpaceEventInfoAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(GenPhaseSpaceEventInfoAnalyzer);
