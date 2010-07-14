#include "TauAnalysis/Core/plugins/PATElectronDump.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "TauAnalysis/Core/interface/eventDumpAuxFunctions.h"
#include "TauAnalysis/Core/interface/eventAuxFunctions.h"
#include "TauAnalysis/GenSimTools/interface/genParticleAuxFunctions.h"

#include <TMath.h>

PATElectronDump::PATElectronDump(const edm::ParameterSet& cfg)
  : ObjectDumpBase(cfg),
    patElectronSource_(cfg.getParameter<edm::InputTag>("electronSource")),
    genParticleSource_(cfg.getParameter<edm::InputTag>("genParticleSource"))
{
  typedef std::vector<int> vint;
  skipPdgIdsGenParticleMatch_ = ( cfg.exists("skipPdgIdsGenParticleMatch") ) ?
    cfg.getParameter<vint>("skipPdgIdsGenParticleMatch") : vint();
}

PATElectronDump::~PATElectronDump()
{
//--- nothing to be done yet...
}

void PATElectronDump::print(const edm::Event& evt, const edm::EventSetup& es) const
{
  if ( !outputStream_ ) {
    edm::LogError ("print") << " Data-member outputStream undefined --> skipping !!";
    return;
  }

  std::cout << "<PATElectronDump::print>:" << std::endl;
  std::cout << " src = " << patElectronSource_.label() << std::endl;

  edm::Handle<pat::ElectronCollection> patElectrons;
  evt.getByLabel(patElectronSource_, patElectrons);

  edm::Handle<reco::GenParticleCollection> genParticles;
  if( genParticleSource_.label() != "") evt.getByLabel(genParticleSource_, genParticles);
  
  unsigned iElectron = 0;
  for ( pat::ElectronCollection::const_iterator patElectron = patElectrons->begin(); 
	patElectron != patElectrons->end(); ++patElectron ) {
    *outputStream_ << "Electron(" << iElectron << "):" << std::endl;
    *outputStream_ << " Pt = " << patElectron->pt() << std::endl;
    *outputStream_ << " theta = " << patElectron->theta()*180./TMath::Pi() 
		   << " (eta = " << patElectron->eta() << ")" << std::endl;
    *outputStream_ << " phi = " << patElectron->phi()*180./TMath::Pi() << std::endl;
    *outputStream_ << " Supercluster" << std::endl;
    if ( isValidRef(patElectron->superCluster()) ) {
      double et = patElectron->superCluster()->energy()*TMath::Sin(patElectron->superCluster()->position().theta());
      *outputStream_ << "  Et = " << et << std::endl;
    } else {
      *outputStream_ << "  none." << std::endl;
    }
    *outputStream_ << " Track" << std::endl;
    printTrackInfo(patElectron->track(), patElectron->vertex(), true, false, outputStream_);
    *outputStream_ << " gsf Track" << std::endl;
    printTrackInfo(patElectron->gsfTrack(), patElectron->vertex(), true, false, outputStream_);
    *outputStream_ << " Supercluster Energy/Track Momentum = " << patElectron->eSuperClusterOverP() << std::endl;
    *outputStream_ << " electronID('eidRobustTight') = " << patElectron->electronID("eidRobustTight") << std::endl;
    *outputStream_ << " electronID('eidRobustLoose') = " << patElectron->electronID("eidRobustLoose") << std::endl;
    *outputStream_ << " trackIso = " << patElectron->trackIso() << std::endl;
    *outputStream_ << " ecalIso = " << patElectron->ecalIso() << std::endl;
    *outputStream_ << " hcalIso = " << patElectron->hcalIso() << std::endl;
    *outputStream_ << " vertex" << std::endl;
    printVertexInfo(patElectron->vertex(), outputStream_);
    if( genParticleSource_.label() != "") 
		*outputStream_ << "* matching gen. pdgId = " 
		   << getMatchingGenParticlePdgId(patElectron->p4(), genParticles, &skipPdgIdsGenParticleMatch_) << std::endl;
    ++iElectron;
  }
  
  *outputStream_ << std::endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATElectronDump, "PATElectronDump");
DEFINE_EDM_PLUGIN(ObjectDumpPluginFactory, PATElectronDump, "PATElectronDump");
