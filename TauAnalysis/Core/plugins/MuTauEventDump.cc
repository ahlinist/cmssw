#include "TauAnalysis/Core/plugins/MuTauEventDump.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Common/interface/Handle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "AnalysisDataFormats/TauAnalysis/interface/ZllHypothesisT1T2.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

#include "TauAnalysis/Core/interface/eventDumpAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include <TMath.h>

edm::InputTag getInputTag(const edm::ParameterSet& cfg, const char* parameter)
{
  return ( cfg.exists(parameter) ) ? cfg.getParameter<edm::InputTag>(parameter) : edm::InputTag();
}

MuTauEventDump::MuTauEventDump(const edm::ParameterSet& cfg)
  : GenericEventDump(cfg)
{
  //std::cout << "<MuTauEventDump::MuTauEventDump>:" << std::endl;

  electronDump_ = makeObjectDump(cfg, "PATElectronDump");
  muonDump_ = makeObjectDump(cfg, "PATMuonDump");
  tauDump_ = makeObjectDump(cfg, "PATTauDump");
  muTauDump_ = makeObjectDump(cfg, "PATMuTauPairDump");

  muTauZmumuHypothesisSource_ = getInputTag(cfg, "muTauZmumuHypothesisSource");
  diMuZmumuHypothesisSource_ = getInputTag(cfg, "diMuZmumuHypothesisSource");
}

MuTauEventDump::~MuTauEventDump()
{
//--- nothing to be done yet...
}

void MuTauEventDump::print(const edm::Event& iEvent, const edm::EventSetup& iSetup, 
			   const GenericAnalyzer_namespace::filterResults_type& filterResults_cumulative, 
			   const GenericAnalyzer_namespace::filterResults_type& filterResults_individual, 
			   double eventWeight) const
{  
  //std::cout << "<MuTauEventDump::print>:" << std::endl; 

  printEventHeaderInfo(iEvent, eventWeight);

  printEventTriggerInfo(iEvent);

  printEventSelectionInfo(filterResults_cumulative, filterResults_individual, outputStream_);

  if( doGenInfo_ ) {
    *outputStream_ << ">>GENERATOR LEVEL INFORMATION<<" << std::endl;
    
    edm::Handle<GenEventInfoProduct> genEventInfo;
    iEvent.getByLabel(genEventInfoSource_, genEventInfo);
    if ( genEventInfo.isValid() && genEventInfo->hasBinningValues() ) {
      std::cout << "Pt(hat) = " << genEventInfo->binningValues()[0] << std::endl;
    }

    edm::Handle<reco::GenParticleCollection> genParticleCollection;
    iEvent.getByLabel(genParticleSource_, genParticleCollection);
    edm::Handle<reco::GenJetCollection> genTauJetCollection;
    iEvent.getByLabel(genTauJetSource_, genTauJetCollection);
    printGenParticleInfo(*genParticleCollection, *genTauJetCollection, outputStream_);
  }

  *outputStream_ << ">>RECONSTRUCTION LEVEL INFORMATION<<" << std::endl;

  electronDump_->print(iEvent, iSetup);
  muonDump_->print(iEvent, iSetup);
  tauDump_->print(iEvent, iSetup);

  muTauDump_->print(iEvent, iSetup);
  printMuTauZmumuHypothesisInfo(iEvent);
  printDiMuZmumuHypothesisInfo(iEvent);

  printMissingEtInfo(iEvent);

  printJetInfo(iEvent);
}

void MuTauEventDump::printMuTauZmumuHypothesisInfo(const edm::Event& iEvent) const
{
  if ( muTauZmumuHypothesisSource_.label() == "" ) return;

  typedef std::vector<ZllHypothesisT1T2<pat::Muon, pat::Tau> > ZmumuHypothesisCollection;
  edm::Handle<ZmumuHypothesisCollection> ZmumuHypotheses;
  iEvent.getByLabel(muTauZmumuHypothesisSource_, ZmumuHypotheses);

  for ( ZmumuHypothesisCollection::const_iterator ZmumuHypothesis = ZmumuHypotheses->begin();
	ZmumuHypothesis != ZmumuHypotheses->end(); ++ZmumuHypothesis ) {
    *outputStream_ << "best matching muon:"
		   << " Pt = " << ZmumuHypothesis->p4Lepton1bestMatch().pt() << "," 
		   << " eta = " << ZmumuHypothesis->p4Lepton1bestMatch().eta() << ","
		   << " phi = " << ZmumuHypothesis->p4Lepton1bestMatch().phi()*180./TMath::Pi() << std::endl;
    *outputStream_ << "best matching tau-jet:"
		   << " Pt = " << ZmumuHypothesis->p4Lepton2bestMatch().pt() << "," 
		   << " eta = " << ZmumuHypothesis->p4Lepton2bestMatch().eta() << ","
		   << " phi = " << ZmumuHypothesis->p4Lepton2bestMatch().phi()*180./TMath::Pi() << std::endl;
    reco::Particle::LorentzVector hypotheticZmumuMomentum = ZmumuHypothesis->p4Lepton1bestMatch() + ZmumuHypothesis->p4Lepton2bestMatch();
    *outputStream_ << "hypothetic Z mass of muon + tau-jet combination = " << hypotheticZmumuMomentum.mass() << std::endl;
  }
}

void MuTauEventDump::printDiMuZmumuHypothesisInfo(const edm::Event& iEvent) const
{
  if ( diMuZmumuHypothesisSource_.label() == "" ) return;

  typedef std::vector<PATDiMuPair> ZmumuHypothesisCollection;
  edm::Handle<ZmumuHypothesisCollection> ZmumuHypotheses;
  iEvent.getByLabel(diMuZmumuHypothesisSource_, ZmumuHypotheses);

  for ( ZmumuHypothesisCollection::const_iterator ZmumuHypothesis = ZmumuHypotheses->begin();
	ZmumuHypothesis != ZmumuHypotheses->end(); ++ZmumuHypothesis ) {
    *outputStream_ << "muon(1):"
		   << " Pt = " << ZmumuHypothesis->leg1()->pt() << "," 
		   << " eta = " << ZmumuHypothesis->leg1()->eta() << ","
		   << " phi = " << ZmumuHypothesis->leg1()->phi()*180./TMath::Pi() << std::endl;
    *outputStream_ << "muon(2):"
		   << " Pt = " << ZmumuHypothesis->leg2()->pt() << "," 
		   << " eta = " << ZmumuHypothesis->leg2()->eta() << ","
		   << " phi = " << ZmumuHypothesis->leg2()->phi()*180./TMath::Pi() << std::endl;
    *outputStream_ << "hypothetic Z mass of muon + muon combination = " << ZmumuHypothesis->p4Vis().mass() << std::endl;
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(EventDumpPluginFactory, MuTauEventDump, "MuTauEventDump");
