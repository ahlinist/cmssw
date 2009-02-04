#include "TauAnalysis/Core/plugins/MuTauEventDump.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "TauAnalysis/Core/interface/eventDumpAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include <TMath.h>

MuTauEventDump::MuTauEventDump(const edm::ParameterSet& cfg)
  : EventDumpBase(cfg)
{
  //std::cout << "<MuTauEventDump::MuTauEventDump>:" << std::endl;

  triggerResultsSrc_ = cfg.getParameter<edm::InputTag>("triggerResultsSource");
  //std::cout << " triggerResultsSrc = " << triggerResultsSrc_ << std::endl;

  triggerPathsToPrint_ = cfg.getParameter<vstring>("triggerPathsToPrint");
  //std::cout << " triggerPathsToPrint = " << format_vstring(triggerPathsToPrint_) << std::endl;

  genParticleSrc_ = cfg.getParameter<edm::InputTag>("genParticleSource");
  //std::cout << " genParticleSrc = " << genParticleSrc_ << std::endl;

  genTauJetSrc_ = cfg.getParameter<edm::InputTag>("genTauJetSource");
  //std::cout << " genTauJetSrc = " << genTauJetSrc_ << std::endl;

  electronSrc_ = cfg.getParameter<edm::InputTag>("electronSource");
  //std::cout << " electronSrc = " << electronSrc_ << std::endl;

  muonSrc_ = cfg.getParameter<edm::InputTag>("muonSource");
  //std::cout << " muonSrc = " << muonSrc_ << std::endl;

  tauSrc_ = cfg.getParameter<edm::InputTag>("tauSource");
  //std::cout << " tauSrc = " << tauSrc_ << std::endl;

  metSrc_ = cfg.getParameter<edm::InputTag>("metSource");
  //std::cout << " metSrc = " << metSrc_ << std::endl;
}

MuTauEventDump::~MuTauEventDump()
{
//--- nothing to be done yet...
}

void MuTauEventDump::print(const edm::Event& iEvent, const edm::EventSetup& iSetup, 
			   const std::vector<std::pair<std::string, bool> >& filterResults_cumulative, 
			   const std::vector<std::pair<std::string, bool> >& filterResults_individual, 
			   double eventWeight) const
{  
  //std::cout << "<MuTauEventDump::print>:" << std::endl; 

  printEventHeaderInfo(iEvent, eventWeight, outputStream_);

  edm::Handle<edm::TriggerResults> triggerResults;
  iEvent.getByLabel(triggerResultsSrc_, triggerResults);
  printEventTriggerInfo(triggerResults, triggerPathsToPrint_, outputStream_);

  printEventSelectionInfo(filterResults_cumulative, filterResults_individual, outputStream_);

  *outputStream_ << ">>GENERATOR LEVEL INFORMATION<<" << std::endl;

  edm::Handle<edm::View<reco::GenParticle> > genParticleCollection;
  iEvent.getByLabel(genParticleSrc_, genParticleCollection);
  edm::Handle<edm::View<reco::GenJet> > genTauJetCollection;
  iEvent.getByLabel(genTauJetSrc_, genTauJetCollection);
  printGenParticleInfo(genParticleCollection, genTauJetCollection, outputStream_);

  *outputStream_ << ">>RECONSTRUCTION LEVEL INFORMATION<<" << std::endl;

  edm::Handle<pat::ElectronCollection> electronCollection;
  iEvent.getByLabel(electronSrc_, electronCollection);
  printElectronInfo(electronCollection, outputStream_);

  edm::Handle<pat::MuonCollection> muonCollection;
  iEvent.getByLabel(muonSrc_, muonCollection);
  printMuonInfo(muonCollection, outputStream_);

  edm::Handle<pat::TauCollection> tauCollection;
  iEvent.getByLabel(tauSrc_, tauCollection);
  printTauInfo(tauCollection, outputStream_);

  edm::Handle<pat::METCollection> metCollection;
  iEvent.getByLabel(metSrc_, metCollection);
  printMissingEtInfo(metCollection, outputStream_);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(EventDumpPluginFactory, MuTauEventDump, "MuTauEventDump");
