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
  : HistManagerBase(cfg)
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

  makeLeptonPtVsPtHatHistograms_ = ( cfg.exists("makeLeptonPtVsPtHatHistograms") ) ? 
    cfg.getParameter<bool>("makeLeptonPtVsPtHatHistograms") : false;
}

GenPhaseSpaceEventInfoHistManager::~GenPhaseSpaceEventInfoHistManager()
{
//--- nothing to be done yet...
}

void GenPhaseSpaceEventInfoHistManager::bookHistogramsImp()
{
  //std::cout << "<GenPhaseSpaceEventInfoHistManager::bookHistogramsImp>:" << std::endl;
    
  hPtHat_ = book1D("PtHat", "PtHat", 101, -0.5, +100.5);
  
  hLeadingElectronPt_ = book1D("LeadingElectronPt", "LeadingElectronPt", 101, -0.5, +100.5);
  hLeadingMuonPt_ = book1D("LeadingMuonPt", "LeadingMuonPt", 101, -0.5, +100.5);
  hLeadingTauLeptonPt_ = book1D("LeadingTauLeptonPt", "LeadingTauLeptonPt", 101, -0.5, +100.5);
  
  if ( makeLeptonPtVsPtHatHistograms_ ) {
    hLeadingElectronPtVsPtHat_ = book2D("LeadingElectronPtVsPtHat", 
					"LeadingElectronPtVsPtHat", 101, -0.5, +100.5, 101, -0.5, +100.5);
    hLeadingMuonPtVsPtHat_ = book2D("LeadingMuonPtVsPtHat", 
				    "LeadingMuonPtVsPtHat", 101, -0.5, +100.5, 101, -0.5, +100.5);
    hLeadingTauLeptonPtVsPtHat_ = book2D("LeadingTauLeptonPtVsPtHat", 
					 "LeadingTauLeptonPtVsPtHat", 101, -0.5, +100.5, 101, -0.5, +100.5);
  }

  hNumGenJets_ = book1D("NumGenJets", "NumGenJets", 21, -0.5, +20.5);
  
  hGenZsPt_ = book1D("GenZsPt", "GenZsPt", 101, -0.5, +100.5);
  hGenZsEta_ = book1D("GenZsEta", "GenZsEta", 200, -10., +10.);
  hGenZsMass_ = book1D("GenZsMass", "GenZsMass", 151, -0.5, +150.5);
  
  hGenParticlesFromZsPt_ = book1D("GenParticlesFromZsPt", "GenParticlesFromZsPt", 101, -0.5, +100.5);
  hGenParticlesFromZsEta_ = book1D("GenParticlesFromZsEta", "GenParticlesFromZsEta", 200, -10., +10.);
  hGenParticlesFromZsPdgId_ = book1D("GenParticlesFromZsPdgId", "GenParticlesFromZsPdgId", 49, -24.5, +24.5);
}

void GenPhaseSpaceEventInfoHistManager::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)

{  
  //std::cout << "<GenPhaseSpaceEventInfoHistManager::fillHistogramsImp>:" << std::endl; 

  edm::Handle<GenPhaseSpaceEventInfo> genPhaseSpaceEventInfo;
  evt.getByLabel(genPhaseSpaceEventInfoSource_, genPhaseSpaceEventInfo);

  hPtHat_->Fill(genPhaseSpaceEventInfo->ptHat(), evtWeight);

  hLeadingElectronPt_->Fill(genPhaseSpaceEventInfo->leadingGenElectron().pt(), evtWeight);
  hLeadingMuonPt_->Fill(genPhaseSpaceEventInfo->leadingGenMuon().pt(), evtWeight);
  hLeadingTauLeptonPt_->Fill(genPhaseSpaceEventInfo->leadingGenTauLepton().pt(), evtWeight);

  if ( makeLeptonPtVsPtHatHistograms_ ) {
    hLeadingElectronPtVsPtHat_->Fill(genPhaseSpaceEventInfo->ptHat(), 
				     genPhaseSpaceEventInfo->leadingGenElectron().pt(), evtWeight);
    hLeadingMuonPtVsPtHat_->Fill(genPhaseSpaceEventInfo->ptHat(), 
				 genPhaseSpaceEventInfo->leadingGenMuon().pt(), evtWeight);
    hLeadingTauLeptonPtVsPtHat_->Fill(genPhaseSpaceEventInfo->ptHat(), 
				      genPhaseSpaceEventInfo->leadingGenTauLepton().pt(), evtWeight);
  }

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

  hNumGenJets_->Fill(numGenJets, evtWeight);

  edm::Handle<reco::GenParticleCollection> genParticlesFromZs;
  evt.getByLabel(genParticlesFromZsSource_, genParticlesFromZs);

  if ( genParticlesFromZs->size() == 2 ) {
    reco::Particle::LorentzVector genZ(0,0,0,0);
    for ( reco::GenParticleCollection::const_iterator genParticle = genParticlesFromZs->begin();
	  genParticle != genParticlesFromZs->end(); ++genParticle ) {
      genZ += genParticle->p4();
      
      hGenParticlesFromZsPt_->Fill(genParticle->pt(), evtWeight);
      hGenParticlesFromZsEta_->Fill(genParticle->eta(), evtWeight);

      int pdgId = genParticle->pdgId();
      if ( pdgId >= -22 && pdgId <= +22 )
	hGenParticlesFromZsPdgId_->Fill(pdgId, evtWeight);
      else if ( pdgId < -22 )
	hGenParticlesFromZsPdgId_->Fill(-24, evtWeight);
      else if ( pdgId > +22 )
	hGenParticlesFromZsPdgId_->Fill(+24, evtWeight);
    }

    hGenZsPt_->Fill(genZ.pt(), evtWeight);
    hGenZsEta_->Fill(genZ.eta(), evtWeight);
    hGenZsMass_->Fill(genZ.mass(), evtWeight);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, GenPhaseSpaceEventInfoHistManager, "GenPhaseSpaceEventInfoHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, GenPhaseSpaceEventInfoHistManager, "GenPhaseSpaceEventInfoHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<GenPhaseSpaceEventInfoHistManager> GenPhaseSpaceEventInfoAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(GenPhaseSpaceEventInfoAnalyzer);
