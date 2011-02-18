#include "TauAnalysis/Core/plugins/PATMuonDump.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "TauAnalysis/Core/interface/eventDumpAuxFunctions.h"
#include "TauAnalysis/Core/interface/eventAuxFunctions.h"
#include "TauAnalysis/GenSimTools/interface/genParticleAuxFunctions.h"

#include <TMath.h>

PATMuonDump::PATMuonDump(const edm::ParameterSet& cfg)
  : ObjectDumpBase(cfg),
    patMuonSource_(cfg.getParameter<edm::InputTag>("muonSource")),
    genParticleSource_(cfg.getParameter<edm::InputTag>("genParticleSource")),
    pfIsolationExtractor_(0)
{
  typedef std::vector<int> vint;
  skipPdgIdsGenParticleMatch_ = ( cfg.exists("skipPdgIdsGenParticleMatch") ) ?
    cfg.getParameter<vint>("skipPdgIdsGenParticleMatch") : vint();

  if ( cfg.exists("muonPFIsoExtractor") ) {
    edm::ParameterSet cfgPFIsolationExtractor = cfg.getParameter<edm::ParameterSet>("muonPFIsoExtractor");
    pfIsolationExtractor_ = new ParticlePFIsolationExtractor<pat::Muon>(cfgPFIsolationExtractor);
    pfIsoCandSource_ = cfg.getParameter<edm::InputTag>("muonPFIsoCandSource");
  }
}

PATMuonDump::~PATMuonDump()
{
  delete pfIsolationExtractor_;
}

void PATMuonDump::print(const edm::Event& evt, const edm::EventSetup& es) const
{
  if ( !outputStream_ ) {
    edm::LogError ("print") << " Data-member outputStream undefined --> skipping !!";
    return;
  }

  std::cout << "<PATMuonDump::print>:" << std::endl;
  std::cout << " src = " << patMuonSource_.label() << std::endl;

  edm::Handle<pat::MuonCollection> patMuons;
  evt.getByLabel(patMuonSource_, patMuons);

  edm::Handle<reco::GenParticleCollection> genParticles;
  if( genParticleSource_.label() != "") evt.getByLabel(genParticleSource_, genParticles);

  unsigned iMuon = 0;
  for ( pat::MuonCollection::const_iterator patMuon = patMuons->begin(); 
	patMuon != patMuons->end(); ++patMuon ) {
    *outputStream_ << "Muon(" << iMuon << "):" << std::endl;
    *outputStream_ << " Pt = " << patMuon->pt() << std::endl;
    *outputStream_ << " theta = " << patMuon->theta()*180./TMath::Pi() 
		   << " (eta = " << patMuon->eta() << ")" << std::endl;
    *outputStream_ << " phi = " << patMuon->phi()*180./TMath::Pi() << std::endl;
    *outputStream_ << " charge = " << patMuon->charge() << std::endl;
    if ( patMuon->isTimeValid() ) {
      *outputStream_ << " time = " << patMuon->time().timeAtIpInOut << " +/- " << patMuon->time().timeAtIpInOutErr << std::endl;
    }
    *outputStream_ << " isGlobalMuon = " << patMuon->isGlobalMuon() << std::endl;
    *outputStream_ << " isStandAloneMuon = " << patMuon->isStandAloneMuon() << std::endl;
    *outputStream_ << " inner Track" << std::endl;
    printTrackInfo(patMuon->innerTrack(), patMuon->vertex(), true, false, outputStream_);
    //printTrackHitPatternInfo(patMuon->innerTrack(), outputStream_);
    *outputStream_ << " outer Track" << std::endl;
    printTrackInfo(patMuon->outerTrack(), patMuon->vertex(), true, false, outputStream_);
    //printTrackHitPatternInfo(patMuon->outerTrack(), outputStream_);
    *outputStream_ << " global Track" << std::endl;
    printTrackInfo(patMuon->globalTrack(), patMuon->vertex(), true, false, outputStream_);
    *outputStream_ << " caloCompatibility = " << patMuon->caloCompatibility() << std::endl;
    *outputStream_ << "  isCaloCompatibilityValid = " << patMuon->isCaloCompatibilityValid() << std::endl;
    *outputStream_ << " segmentCompatibility = " << muon::segmentCompatibility(*patMuon) << std::endl;
    *outputStream_ << " trackIso = " << patMuon->trackIso() << std::endl;
    *outputStream_ << " caloIso = " << patMuon->caloIso() << std::endl;
    *outputStream_ << " ecalIso = " << patMuon->ecalIso() << std::endl;
    *outputStream_ << " hcalIso = " << patMuon->hcalIso() << std::endl;
    if ( pfIsolationExtractor_ ) {
      edm::Handle<reco::PFCandidateCollection> pfCandidates;
      evt.getByLabel(pfIsoCandSource_, pfCandidates);
      *outputStream_ << " pfIso = " << (*pfIsolationExtractor_)(*patMuon, *pfCandidates) << std::endl;
    }
    *outputStream_ << " vertex" << std::endl;
    printVertexInfo(patMuon->vertex(), outputStream_);
    if ( isValidRef(patMuon->track()) ) *outputStream_ << " dIP = " << patMuon->track()->dxy(patMuon->vertex()) << std::endl;
    if ( genParticleSource_.label() != "" ) 
      *outputStream_ << "* matching gen. pdgId = " 
		     << getMatchingGenParticlePdgId(patMuon->p4(), *genParticles, &skipPdgIdsGenParticleMatch_) << std::endl;
    ++iMuon; 
  }
  
  *outputStream_ << std::endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATMuonDump, "PATMuonDump");
DEFINE_EDM_PLUGIN(ObjectDumpPluginFactory, PATMuonDump, "PATMuonDump");
