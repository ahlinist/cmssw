#include "TauAnalysis/Core/plugins/GenPhaseSpaceEventInfoHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "AnalysisDataFormats/TauAnalysis/interface/GenPhaseSpaceEventInfo.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

GenPhaseSpaceEventInfoHistManager::GenPhaseSpaceEventInfoHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<GenPhaseSpaceEventInfoHistManager::GenPhaseSpaceEventInfoHistManager>:" << std::endl;

  generatorInfoSource_ = cfg.getParameter<edm::InputTag>("generatorInfoSource");
  //std::cout << " generatorInfoSource = " << generatorInfoSource_ << std::endl;

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

  genParticlesFromHsSource_ = cfg.getParameter<edm::InputTag>("genParticlesFromHsSource");
  //std::cout << " genParticlesFromZsSource = " << genParticlesFromHsSource_ << std::endl;

  makeBjorkenXratioHistogram_ = ( cfg.exists("makeBjorkenXratioHistogram") ) ? 
    cfg.getParameter<bool>("makeBjorkenXratioHistogram") : false;

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
    
  hProcessId_ = book1D("ProcessId", "ProcessId", 201, -100.5, +100.5);
  hPtHat_ = book1D("PtHat", "PtHat", 101, -0.5, +100.5);
  hBjorkenX1_ = book1D("hBjorkenX1", "hBjorkenX1", 101, -0.05, +1.05);
  hBjorkenX2_ = book1D("hBjorkenX2", "hBjorkenX2", 101, -0.05, +1.05);
  if ( makeBjorkenXratioHistogram_ ) hBjorkenXratio_ = book1D("hBjorkenXratio", "hBjorkenXratio", 10001, -0.00005, +1.00005);
  
  hLeadingElectronPt_ = book1D("LeadingElectronPt", "LeadingElectronPt", 101, -0.5, +100.5);
  hLeadingElectronEta_ = book1D("LeadingElectronEta", "LeadingElectronEta", 150, -7.5, +7.5);
  hLeadingMuonPt_ = book1D("LeadingMuonPt", "LeadingMuonPt", 101, -0.5, +100.5);
  hLeadingMuonEta_ = book1D("LeadingMuonEta", "LeadingMuonEta", 150, -7.5, +7.5);
  hLeadingTauLeptonPt_ = book1D("LeadingTauLeptonPt", "LeadingTauLeptonPt", 101, -0.5, +100.5);
  hLeadingTauLeptonEta_ = book1D("LeadingTauLeptonEta", "LeadingTauLeptonEta", 150, -7.5, +7.5);
  
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

  hGenHsPt_ = book1D("GenHsPt", "GenHsPt", 101, -0.5, +100.5);
  hGenHsEta_ = book1D("GenHsEta", "GenHsEta", 200, -10., +10.);
  hGenHsMass_ = book1D("GenHsMass", "GenHsMass", 501, -0.5, +500.5);
  
  hGenParticlesFromHsPt_ = book1D("GenParticlesFromHsPt", "GenParticlesFromHsPt", 501, -0.5, +500.5);
  hGenParticlesFromHsEta_ = book1D("GenParticlesFromHsEta", "GenParticlesFromHsEta", 200, -10., +10.);
  hGenParticlesFromHsPdgId_ = book1D("GenParticlesFromHsPdgId", "GenParticlesFromHsPdgId", 49, -24.5, +24.5);
}

void fillGenParticleHistograms(
       const reco::GenParticleCollection& genParticlesFromDecays,
       MonitorElement* histoGenMotherPt, MonitorElement* histoGenMotherEta, MonitorElement* histoGenMotherMass,  
       MonitorElement* histoGenDaughterPt, MonitorElement* histoGenDaughterEta, MonitorElement* histoGenDaughterPdgId,
       double evtWeight)
{
  if ( genParticlesFromDecays.size() == 2 ) {
    reco::Particle::LorentzVector genMother(0,0,0,0);
    for ( reco::GenParticleCollection::const_iterator genParticle = genParticlesFromDecays.begin();
	  genParticle != genParticlesFromDecays.end(); ++genParticle ) {
      genMother += genParticle->p4();
      
      histoGenDaughterPt->Fill(genParticle->pt(), evtWeight);
      histoGenDaughterEta->Fill(genParticle->eta(), evtWeight);

      int pdgId = genParticle->pdgId();
      if ( pdgId >= -22 && pdgId <= +22 )
	histoGenDaughterPdgId->Fill(pdgId, evtWeight);
      else if ( pdgId < -22 )
	histoGenDaughterPdgId->Fill(-24, evtWeight);
      else if ( pdgId > +22 )
	histoGenDaughterPdgId->Fill(+24, evtWeight);
    }
    
    histoGenMotherPt->Fill(genMother.pt(), evtWeight);
    histoGenMotherEta->Fill(genMother.eta(), evtWeight);
    histoGenMotherMass->Fill(genMother.mass(), evtWeight);
  }
}

void GenPhaseSpaceEventInfoHistManager::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)

{  
  //std::cout << "<GenPhaseSpaceEventInfoHistManager::fillHistogramsImp>:" << std::endl; 

  edm::Handle<GenEventInfoProduct> generatorInfo;
  evt.getByLabel(generatorInfoSource_, generatorInfo);

  edm::Handle<GenPhaseSpaceEventInfo> genPhaseSpaceEventInfo;
  evt.getByLabel(genPhaseSpaceEventInfoSource_, genPhaseSpaceEventInfo);

  hProcessId_->Fill(generatorInfo->signalProcessID(), evtWeight);
  hPtHat_->Fill(genPhaseSpaceEventInfo->ptHat(), evtWeight);
  if ( generatorInfo->hasPDF() ) {
    double x1 = generatorInfo->pdf()->x.first;
    double x2 = generatorInfo->pdf()->x.second;
    hBjorkenX1_->Fill(x1, evtWeight);
    hBjorkenX2_->Fill(x2, evtWeight);
    if ( makeBjorkenXratioHistogram_ && (x1 + x2) != 0. ) hBjorkenXratio_->Fill(x1/(x1 + x2), evtWeight);
  }

  hLeadingElectronPt_->Fill(genPhaseSpaceEventInfo->leadingGenElectron().pt(), evtWeight);
  hLeadingElectronEta_->Fill(genPhaseSpaceEventInfo->leadingGenElectron().eta(), evtWeight);
  hLeadingMuonPt_->Fill(genPhaseSpaceEventInfo->leadingGenMuon().pt(), evtWeight);
  hLeadingMuonEta_->Fill(genPhaseSpaceEventInfo->leadingGenMuon().eta(), evtWeight);
  hLeadingTauLeptonPt_->Fill(genPhaseSpaceEventInfo->leadingGenTauLepton().pt(), evtWeight);
  hLeadingTauLeptonEta_->Fill(genPhaseSpaceEventInfo->leadingGenTauLepton().eta(), evtWeight);

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

  fillGenParticleHistograms(*genParticlesFromZs, 
			    hGenZsPt_, hGenZsEta_, hGenZsMass_,
			    hGenParticlesFromZsPt_, hGenParticlesFromZsEta_, hGenParticlesFromZsPdgId_,
			    evtWeight);

  edm::Handle<reco::GenParticleCollection> genParticlesFromHs;
  evt.getByLabel(genParticlesFromHsSource_, genParticlesFromHs);
  
  fillGenParticleHistograms(*genParticlesFromHs, 
			    hGenHsPt_, hGenHsEta_, hGenHsMass_,
			    hGenParticlesFromHsPt_, hGenParticlesFromHsEta_, hGenParticlesFromHsPdgId_,
			    evtWeight);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, GenPhaseSpaceEventInfoHistManager, "GenPhaseSpaceEventInfoHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, GenPhaseSpaceEventInfoHistManager, "GenPhaseSpaceEventInfoHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<GenPhaseSpaceEventInfoHistManager> GenPhaseSpaceEventInfoAnalyzer;

DEFINE_FWK_MODULE(GenPhaseSpaceEventInfoAnalyzer);
