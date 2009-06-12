#include "TauAnalysis/Core/plugins/GenPhaseSpaceEventInfoHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "AnalysisDataFormats/TauAnalysis/interface/GenPhaseSpaceEventInfo.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

GenPhaseSpaceEventInfoHistManager::GenPhaseSpaceEventInfoHistManager(const edm::ParameterSet& cfg)
  : dqmError_(0)
{
  //std::cout << "<GenPhaseSpaceEventInfoHistManager::GenPhaseSpaceEventInfoHistManager>:" << std::endl;

  genPhaseSpaceEventInfoSource_ = cfg.getParameter<edm::InputTag>("genPhaseSpaceEventInfoSource");
  //std::cout << " genPhaseSpaceEventInfoSource = " << genPhaseSpaceEventInfoSource_ << std::endl;

  genJetSource_ = cfg.getParameter<edm::InputTag>("genJetSource");
  //std::cout << " genJetSource = " << genJetSource_ << std::endl;

  genJetPtMin_ = ( cfg.exists("genJetPtMin") ) ? cfg.getParameter<double>("genJetPtMin") : -1.;
  genJetPtMax_ = ( cfg.exists("genJetPtMax") ) ? cfg.getParameter<double>("genJetPtMax") : 1.e+9;
  genJetEtaMin_ = ( cfg.exists("genJetEtaMin") ) ? cfg.getParameter<double>("genJetEtaMin") : -5.;
  genJetEtaMax_ = ( cfg.exists("genJetEtaMax") ) ? cfg.getParameter<double>("genJetEtaMax") : +5.;

  genParticlesFromZsSource_ = cfg.getParameter<edm::InputTag>("genParticlesFromZsSource");
  //std::cout << " genParticlesFromZsSource = " << genParticlesFromZsSource_ << std::endl;

  dqmDirectory_store_ = cfg.getParameter<std::string>("dqmDirectory_store");
  //std::cout << " dqmDirectory_store = " << dqmDirectory_store_ << std::endl;
}

GenPhaseSpaceEventInfoHistManager::~GenPhaseSpaceEventInfoHistManager()
{
//--- nothing to be done yet...
}

void GenPhaseSpaceEventInfoHistManager::bookHistograms()
{
  //std::cout << "<GenPhaseSpaceEventInfoHistManager::bookHistograms>:" << std::endl;

  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("bookHistograms") << " Failed to access dqmStore --> histograms will NOT be booked !!";
    dqmError_ = 1;
    return;
  }
  
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
  
  hNumGenJets_ = dqmStore.book1D("NumGenJets", "NumGenJets", 21, -0.5, +20.5);
  
  hGenZsPt_ = dqmStore.book1D("GenZsPt", "GenZsPt", 101, -0.5, +100.5);
  hGenZsEta_ = dqmStore.book1D("GenZsEta", "GenZsEta", 200, -10., +10.);
  hGenZsMass_ = dqmStore.book1D("GenZsMass", "GenZsMass", 151, -0.5, +150.5);
  
  hGenParticlesFromZsPt_ = dqmStore.book1D("GenParticlesFromZsPt", "GenParticlesFromZsPt", 101, -0.5, +100.5);
  hGenParticlesFromZsEta_ = dqmStore.book1D("GenParticlesFromZsEta", "GenParticlesFromZsEta", 200, -10., +10.);
  hGenParticlesFromZsPdgId_ = dqmStore.book1D("GenParticlesFromZsPdgId", "GenParticlesFromZsPdgId", 49, -24.5, +24.5);
}

void GenPhaseSpaceEventInfoHistManager::fillHistograms(const edm::Event& evt, const edm::EventSetup& es)

{  
  //std::cout << "<GenPhaseSpaceEventInfoHistManager::fillHistograms>:" << std::endl; 

  if ( dqmError_ ) {
    edm::LogError ("fillHistograms") << " Failed to access dqmStore --> histograms will NOT be filled !!";
    return;
  }

  edm::Handle<GenPhaseSpaceEventInfo> genPhaseSpaceEventInfo;
  evt.getByLabel(genPhaseSpaceEventInfoSource_, genPhaseSpaceEventInfo);

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

  edm::Handle<reco::GenJetCollection> genJets;
  evt.getByLabel(genJetSource_, genJets);

  unsigned numGenJets = 0;

  for ( reco::GenJetCollection::const_iterator genJet = genJets->begin();
	genJet != genJets->end(); ++genJet ) {
    if ( genJet->pt()  > genJetPtMin_  && genJet->pt()  < genJetPtMax_ &&
	 genJet->eta() > genJetEtaMin_ && genJet->eta() < genJetEtaMax_ ) {
      ++numGenJets;
    }
  }

  hNumGenJets_->Fill(numGenJets);

  edm::Handle<reco::GenParticleCollection> genParticlesFromZs;
  evt.getByLabel(genParticlesFromZsSource_, genParticlesFromZs);

  if ( genParticlesFromZs->size() == 2 ) {
    reco::Particle::LorentzVector genZ(0,0,0,0);
    for ( reco::GenParticleCollection::const_iterator genParticle = genParticlesFromZs->begin();
	  genParticle != genParticlesFromZs->end(); ++genParticle ) {
      genZ += genParticle->p4();
      
      hGenParticlesFromZsPt_->Fill(genParticle->pt());
      hGenParticlesFromZsEta_->Fill(genParticle->eta());

      int pdgId = genParticle->pdgId();
      if ( pdgId >= -22 && pdgId <= +22 )
	hGenParticlesFromZsPdgId_->Fill(pdgId);
      else if ( pdgId < -22 )
	hGenParticlesFromZsPdgId_->Fill(-24);
      else if ( pdgId > +22 )
	hGenParticlesFromZsPdgId_->Fill(+24);
    }

    hGenZsPt_->Fill(genZ.pt());
    hGenZsEta_->Fill(genZ.eta());
    hGenZsMass_->Fill(genZ.mass());
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, GenPhaseSpaceEventInfoHistManager, "GenPhaseSpaceEventInfoHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, GenPhaseSpaceEventInfoHistManager, "GenPhaseSpaceEventInfoHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<GenPhaseSpaceEventInfoHistManager> GenPhaseSpaceEventInfoAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(GenPhaseSpaceEventInfoAnalyzer);
