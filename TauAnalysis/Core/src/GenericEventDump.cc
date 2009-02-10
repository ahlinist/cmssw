#include "TauAnalysis/Core/interface/GenericEventDump.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/TriggerNames.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "TauAnalysis/Core/interface/eventDumpAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include <TMath.h>

#include <iostream>
#include <fstream>

edm::InputTag getInputTag(const edm::ParameterSet& cfg, const char* parameter)
{
  return ( cfg.exists(parameter) ) ? cfg.getParameter<edm::InputTag>(parameter) : edm::InputTag();
}

GenericEventDump::GenericEventDump(const edm::ParameterSet& cfg)
  : EventDumpBase(cfg)
{
  //std::cout << "<GenericEventDump::GenericEventDump>:" << std::endl;

  triggerResultsSource_ = getInputTag(cfg, "triggerResultsSource");
  triggerPathsToPrint_ = ( cfg.exists("triggerPathsToPrint") ) ? 
    cfg.getParameter<vstring>("triggerPathsToPrint") : vstring();

  genParticleSource_ = getInputTag(cfg, "genParticleSource");
  genTauJetSource_ = getInputTag(cfg, "genTauJetSource");

  patElectronSource_ = getInputTag(cfg, "electronSource");
  patMuonSource_ = getInputTag(cfg, "muonSource");
  patTauSource_ = getInputTag(cfg, "tauSource");

  patMEtSource_ = getInputTag(cfg, "patMEtSource");

  recoTrackSource_ = getInputTag(cfg, "recoTrackSource");
}

GenericEventDump::~GenericEventDump()
{
// nothing to be done yet...
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void GenericEventDump::printEventHeaderInfo(const edm::Event& evt, double eventWeight) const
{
  if ( !outputStream_ ) {
    edm::LogError ("printEventHeaderInfo") << " outputStream = NULL --> skipping !!";
    return;
  }

  edm::RunNumber_t runNumber = evt.id().run();
  edm::EventNumber_t eventNumber = evt.id().event();

  *outputStream_ << "Run: " << runNumber << std::endl;
  *outputStream_ << "Event: " << eventNumber << std::endl;
  *outputStream_ << std::endl;
  *outputStream_ << " weight = " << eventWeight << std::endl;
  *outputStream_ << std::endl;
}

void GenericEventDump::printEventTriggerInfo(const edm::Event& evt) const
{
  if ( !outputStream_ ) {
    edm::LogError ("printEventTriggerInfo") << " outputStream = NULL --> skipping !!";
    return;
  }

  if ( triggerResultsSource_.label() != "" ) {
    edm::Handle<edm::TriggerResults> triggerResults;
    evt.getByLabel(triggerResultsSource_, triggerResults);

    edm::TriggerNames triggerNames;
    triggerNames.init(*triggerResults);

    //for ( edm::TriggerNames::Strings::const_iterator triggerName = triggerNames.triggerNames().begin();
    //	  triggerName != triggerNames.triggerNames().end(); ++triggerName ) {
    //  unsigned int index = triggerNames.triggerIndex(*triggerName);
    //  if ( index < triggerNames.size() ) {
    //    std::string triggerDecision = ( triggerResults->accept(index) ) ? "passed" : "failed";
    //    
    //    std::cout << " triggerName = " << (*triggerName) << " " << triggerDecision << std::endl;
    //  }
    //}
    
    *outputStream_ << "triggerDecisions:" << std::endl;
    
    for ( std::vector<std::string>::const_iterator triggerPath = triggerPathsToPrint_.begin();
	  triggerPath != triggerPathsToPrint_.end(); ++triggerPath ) {
      unsigned int index = triggerNames.triggerIndex(*triggerPath);
      if ( index < triggerNames.size() ) {
	std::string triggerDecision = ( triggerResults->accept(index) ) ? "passed" : "failed";
	
	*outputStream_ << " " << (*triggerPath) << " " << triggerDecision << std::endl;
      } else {
	edm::LogError ("printEventTriggerInfo") << " Undefined trigger Path = " << (*triggerPath) << " --> skipping !!";
	continue;
      }
    }
    
    *outputStream_ << std::endl;
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void GenericEventDump::printElectronInfo(const edm::Event& evt) const
{
  if ( !outputStream_ ) {
    edm::LogError ("printElectronInfo") << " outputStream = NULL --> skipping !!";
    return;
  }

  if ( patElectronSource_.label() != "" ) {
    edm::Handle<pat::ElectronCollection> patElectrons;
    evt.getByLabel(patElectronSource_, patElectrons);

    unsigned iElectron = 0;
    for ( pat::ElectronCollection::const_iterator patElectron = patElectrons->begin(); 
	  patElectron != patElectrons->end(); ++patElectron ) {
      *outputStream_ << "Electron(" << iElectron << "):" << std::endl;
      *outputStream_ << " Pt = " << patElectron->pt() << std::endl;
      *outputStream_ << " theta = " << patElectron->theta()*180./TMath::Pi() << " (eta = " << patElectron->eta() << ")" << std::endl;
      *outputStream_ << " phi = " << patElectron->phi()*180./TMath::Pi() << std::endl;
      *outputStream_ << " Track" << std::endl;
      printTrackInfo(edm::RefToBase<reco::Track>(patElectron->track()), outputStream_);
      *outputStream_ << " gsf Track" << std::endl;
      printTrackInfo(edm::RefToBase<reco::Track>(patElectron->gsfTrack()), outputStream_);
      *outputStream_ << " vertex" << std::endl;
      printVertexInfo(patElectron->vertex(), outputStream_);
      ++iElectron;
    }
    
    *outputStream_ << std::endl;
  }
}

void GenericEventDump::printMuonInfo(const edm::Event& evt) const
{
  if ( !outputStream_ ) {
    edm::LogError ("printMuonInfo") << " outputStream = NULL --> skipping !!";
    return;
  }

  if ( patMuonSource_.label() != "" ) {
    edm::Handle<pat::MuonCollection> patMuons;
    evt.getByLabel(patMuonSource_, patMuons);

    unsigned iMuon = 0;
    for ( pat::MuonCollection::const_iterator patMuon = patMuons->begin(); 
	  patMuon != patMuons->end(); ++patMuon ) {
      *outputStream_ << "Muon(" << iMuon << "):" << std::endl;
      *outputStream_ << " Pt = " << patMuon->pt() << std::endl;
      *outputStream_ << " theta = " << patMuon->theta()*180./TMath::Pi() << " (eta = " << patMuon->eta() << ")" << std::endl;
      *outputStream_ << " phi = " << patMuon->phi()*180./TMath::Pi() << std::endl;
      *outputStream_ << " inner Track" << std::endl;
      printTrackInfo(edm::RefToBase<reco::Track>(patMuon->innerTrack()), outputStream_);
      *outputStream_ << " outer Track" << std::endl;
      printTrackInfo(edm::RefToBase<reco::Track>(patMuon->outerTrack()), outputStream_);
      *outputStream_ << " global Track" << std::endl;
      printTrackInfo(edm::RefToBase<reco::Track>(patMuon->globalTrack()), outputStream_);
      *outputStream_ << " trackIso = " << patMuon->trackIso() << std::endl;
      if ( recoTrackSource_.label() != "" ) {
	edm::Handle<reco::TrackCollection> recoTracks;
	evt.getByLabel(recoTrackSource_, recoTracks);
	printTrackIsolationInfo(recoTracks, patMuon->momentum(), 1.0, outputStream_);
      }
      *outputStream_ << " caloIso = " << patMuon->caloIso() << std::endl;
      *outputStream_ << " ecalIso = " << patMuon->ecalIso() << std::endl;
      *outputStream_ << " hcalIso = " << patMuon->hcalIso() << std::endl;
      *outputStream_ << " vertex" << std::endl;
      printVertexInfo(patMuon->vertex(), outputStream_);
      ++iMuon; 
    }
    
    *outputStream_ << std::endl;
  }
}

void GenericEventDump::printTauInfo(const edm::Event& evt) const
{
  if ( !outputStream_ ) {
    edm::LogError ("printTauInfo") << " outputStream = NULL --> skipping !!";
    return;
  }

  if ( patTauSource_.label() != "" ) {
    edm::Handle<pat::TauCollection> patTaus;
    evt.getByLabel(patTauSource_, patTaus);

    unsigned iTau = 0;
    for ( pat::TauCollection::const_iterator patTau = patTaus->begin(); 
	  patTau != patTaus->end(); ++patTau ) {
      *outputStream_ << "Tau(" << iTau << "):" << std::endl;
      *outputStream_ << " Et = " << patTau->et() << std::endl;
      *outputStream_ << " theta = " << patTau->theta()*180./TMath::Pi() << " (eta = " << patTau->eta() << ")" << std::endl;
      *outputStream_ << " phi = " << patTau->phi()*180./TMath::Pi() << std::endl;
      *outputStream_ << " leading Track" << std::endl;
      printTrackInfo(edm::RefToBase<reco::Track>(patTau->leadTrack()), outputStream_);
      *outputStream_ << " #signal Tracks = " << patTau->signalTracks().size() << std::endl;
      *outputStream_ << " tauId" << std::endl;
      *outputStream_ << "  leadingTrackFinding = " << patTau->tauID("leadingTrackFinding") << std::endl;
      *outputStream_ << "  leadingTrackPtCut = " << patTau->tauID("leadingTrackPtCut") << std::endl;
      *outputStream_ << "  trackIsolation = " << patTau->tauID("trackIsolation") << std::endl;
      *outputStream_ << "  ecalIsolation = " << patTau->tauID("ecalIsolation") << std::endl;
      *outputStream_ << " eVeto = " << patTau->tauID("againstElectron") << std::endl;
      *outputStream_ << " muVeto = " << patTau->tauID("againstMuon") << std::endl;
      *outputStream_ << " vertex" << std::endl;
      printVertexInfo(patTau->vertex(), outputStream_);
      ++iTau;
    }

    *outputStream_ << std::endl;
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void GenericEventDump::printMissingEtInfo(const edm::Event& evt) const
{
//--- print-out PAT/reco missing Et information
  
  if ( !outputStream_ ) {
    edm::LogError ("printMissingEtInfo") << " outputStream = NULL --> skipping !!";
    return;
  }

  if ( patMEtSource_.label() != "" ) {
    edm::Handle<pat::METCollection> patMETs;
    evt.getByLabel(patMEtSource_, patMETs);

    for ( pat::METCollection::const_iterator patMET = patMETs->begin(); 
	  patMET != patMETs->end(); ++patMET ) {
      
      *outputStream_ << "PAT MET:" 
		     << " Et = " << patMET->pt() << "," 
		     << " phi = " <<  patMET->phi() << std::endl;
      *outputStream_ << " isCaloMET = " << patMET->isCaloMET() << std::endl;
      
      if ( patMET->genMET() != NULL ) {
	const reco::GenMET* genMET = patMET->genMET();
	
	*outputStream_ << "genMET:" 
		       << " Et = " << genMET->pt() << "," 
		       << " phi = " <<  genMET->phi() << std::endl;
	
      } else {
	*outputStream_ << "no genMET associated to PAT MET !!" << std::endl;
      }
    }

    *outputStream_ << std::endl;
  }
}

