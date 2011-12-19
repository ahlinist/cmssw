#include "TauAnalysis/Core/plugins/PATElectronDump.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "TauAnalysis/Core/interface/eventDumpAuxFunctions.h"
#include "TauAnalysis/Core/interface/eventAuxFunctions.h"
#include "TauAnalysis/GenSimTools/interface/genParticleAuxFunctions.h"

#include "RecoEgamma/EgammaTools/interface/ConversionFinder.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

#include <TMath.h>

PATElectronDump::PATElectronDump(const edm::ParameterSet& cfg)
  : ObjectDumpBase(cfg),
    patElectronSource_(cfg.getParameter<edm::InputTag>("electronSource")),
    genParticleSource_(cfg.getParameter<edm::InputTag>("genParticleSource")),
    pfCandidateSrc_(cfg.getParameter<edm::InputTag>("pfCandidateSource")),
    pfCombIsoExtractor_(0),
    pfChargedHadronIsoExtractor_(0),
    pfNeutralHadronIsoExtractor_(0),
    pfPhotonIsoExtractor_(0)
{
  typedef std::vector<int> vint;
  skipPdgIdsGenParticleMatch_ = ( cfg.exists("skipPdgIdsGenParticleMatch") ) ?
    cfg.getParameter<vint>("skipPdgIdsGenParticleMatch") : vint();
    
  if ( pfCandidateSrc_.label() != "") {
    std::cout << "Will print PF iso sums" << std::endl;
    if ( cfg.exists("pfChargedHadronIsoExtractor") ) {
      edm::ParameterSet cfgPFChargedHadronIsoExtractor = cfg.getParameter<edm::ParameterSet>("pfChargedHadronIsoExtractor");
      pfChargedHadronIsoExtractor_ = new PATElectronPFIsolationExtractor(cfgPFChargedHadronIsoExtractor);
      pfNoPileUpCandidateSrc_ = cfg.getParameter<edm::InputTag>("pfNoPileUpCandidateSource");
    }
    if ( cfg.exists("pfNeutralHadronIsoExtractor") ) {
      edm::ParameterSet cfgPFNeutralHadronIsoExtractor = cfg.getParameter<edm::ParameterSet>("pfNeutralHadronIsoExtractor");
      pfNeutralHadronIsoExtractor_ = new PATElectronPFIsolationExtractor(cfgPFNeutralHadronIsoExtractor);
      pfNoPileUpCandidateSrc_ = cfg.getParameter<edm::InputTag>("pfNoPileUpCandidateSource");
    }
    if ( cfg.exists("pfPhotonIsoExtractor") ) {
      edm::ParameterSet cfgPFPhotonIsoExtractor = cfg.getParameter<edm::ParameterSet>("pfPhotonIsoExtractor");
      pfPhotonIsoExtractor_ = new PATElectronPFIsolationExtractor(cfgPFPhotonIsoExtractor);
      pfNoPileUpCandidateSrc_ = cfg.getParameter<edm::InputTag>("pfNoPileUpCandidateSource");
    }
    if ( cfg.exists("pfCombIsoExtractor") ) {
      edm::ParameterSet cfgPFCombIsoExtractor = cfg.getParameter<edm::ParameterSet>("pfCombIsoExtractor");
      pfCombIsoExtractor_ = new PATElectronPFIsolationExtractor(cfgPFCombIsoExtractor);
      pfNoPileUpCandidateSrc_ = cfg.getParameter<edm::InputTag>("pfNoPileUpCandidateSource");
    }
  }
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

    edm::Handle<reco::PFCandidateCollection> pfCandidates;
    if( pfCandidateSrc_.label() != "" ) evt.getByLabel(pfCandidateSrc_, pfCandidates);

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
            *outputStream_ << "  theta = " << patElectron->superCluster()->position().theta() 
                << " (eta = " << patElectron->superCluster()->position().eta() << ")" << std::endl;
        } else {
            *outputStream_ << "  none." << std::endl;
        }
        *outputStream_ << " Track" << std::endl;
        printTrackInfo(patElectron->track(), patElectron->vertex(), true, false, outputStream_);
        *outputStream_ << " gsf Track" << std::endl;
        printTrackInfo(patElectron->gsfTrack(), patElectron->vertex(), true, true, outputStream_);
        *outputStream_ << " Supercluster Energy/Track Momentum = " << patElectron->eSuperClusterOverP() << std::endl;
        //*outputStream_ << " electronID('eidRobustTight') = " << patElectron->electronID("eidRobustTight") << std::endl;
        //*outputStream_ << " electronID('eidRobustLoose') = " << patElectron->electronID("eidRobustLoose") << std::endl;
        *outputStream_ << " deltaEta (trk/cluster) = " << patElectron->deltaEtaSuperClusterTrackAtVtx() << std::endl;
        *outputStream_ << " deltaPhi (trk/cluster) = " << patElectron->deltaPhiSuperClusterTrackAtVtx() << std::endl;
        *outputStream_ << " H/E = " << patElectron->hcalOverEcal() << std::endl;
        *outputStream_ << " sigmaIetaIeta = " << patElectron->sigmaIetaIeta() << std::endl;
        //*outputStream_ << " trackIso = " << patElectron->trackIso() << std::endl;
        //*outputStream_ << " ecalIso = " << patElectron->ecalIso() << std::endl;
        //*outputStream_ << " hcalIso = " << patElectron->hcalIso() << std::endl;

	edm::Handle<reco::PFCandidateCollection> pfNoPileUpCandidates;
	if ( pfNoPileUpCandidateSrc_.label() != "" ) evt.getByLabel(pfNoPileUpCandidateSrc_, pfNoPileUpCandidates);

        // print PF isolation info, if requested
        if ( pfChargedHadronIsoExtractor_ ) {
	  double pfChargedHadronIso = 
	    (*pfChargedHadronIsoExtractor_)(*patElectron, ParticlePFIsolationExtractor<pat::Electron>::kDirP4,
					    *pfCandidates, *pfNoPileUpCandidates);
	  *outputStream_ << " pfChargedHadronIsoSum/pt = " << pfChargedHadronIso << "/" << patElectron->pt() 
			 << " = " << pfChargedHadronIso/patElectron->pt() << std::endl;
        }
        if ( pfNeutralHadronIsoExtractor_ ) {
	  double pfNeutralHadronIso = 
	    (*pfNeutralHadronIsoExtractor_)(*patElectron, ParticlePFIsolationExtractor<pat::Electron>::kDirP4,
					    *pfCandidates, *pfNoPileUpCandidates);
	  *outputStream_ << " pfNeutralHadronIsoSum/pt = " << pfNeutralHadronIso << "/" << patElectron->pt()
			 << " = " << pfNeutralHadronIso/patElectron->pt() << std::endl;
        }
        if ( pfPhotonIsoExtractor_ ) {
	  double pfPhotonIso = 
	    (*pfPhotonIsoExtractor_)(*patElectron, ParticlePFIsolationExtractor<pat::Electron>::kDirP4,
				     *pfCandidates, *pfNoPileUpCandidates);
	  *outputStream_ << " pfPhotonIsoSum/pt = " << pfPhotonIso << "/" << patElectron->pt() 
			 << " = " << pfPhotonIso/patElectron->pt() << std::endl;
        }
        if ( pfCombIsoExtractor_ ) {
	  double pfCombIso = 
	    (*pfCombIsoExtractor_)(*patElectron, ParticlePFIsolationExtractor<pat::Electron>::kDirP4,
				   *pfCandidates, *pfNoPileUpCandidates);
	  *outputStream_ << " pfCombIsoSum/pt = " << pfCombIso << "/" << patElectron->pt() 
			 << " = " << pfCombIso/patElectron->pt() << std::endl;
        }
	
        // JK temporarily disabled; breaks something in 4_2_X 
        //if( pfCandidateSrc_.label() != "" ) 
        //    printPFCandidateIsolationInfo(pfCandidates,"pfNoPileUp",patElectron->momentum(),0,0.4,-1,outputStream_);


        *outputStream_ << " vertex from electron" << std::endl;
        printVertexInfo(patElectron->vertex(), outputStream_);
        //*outputStream_ << "" << std::endl;
        //if( vertexHandle->size() > 0) {
        //    *outputStream_ << patElectron->gsfTrack()->dxy(vertexHandle->at(0).position()) <<std::endl;
        //    *outputStream_ << patElectron->gsfTrack()->dz(vertexHandle->at(0).position()) << std::endl;
        //}
        if ( genParticleSource_.label() != "" ) 
            *outputStream_ << "* matching gen. pdgId = " 
                << getMatchingGenParticlePdgId(patElectron->p4(), *genParticles, &skipPdgIdsGenParticleMatch_) << std::endl;
        ++iElectron;


        // do conversion info
        edm::Handle<reco::BeamSpot> bsHandle;
        evt.getByLabel("offlineBeamSpot", bsHandle);
        const reco::BeamSpot &theBS = *bsHandle.product();

        edm::Handle<reco::ConversionCollection> hConversions;
        evt.getByLabel("allConversions", hConversions);

        //  calculate compatibility with conversion
        bool hasMatchedConversion = ConversionTools::hasMatchedConversion(*patElectron,hConversions,theBS.position(), true, 2.0, 0.000001, 0);
        
        //  calculate number of missing inner pixel hits
        const reco::Track *elec_track = (const reco::Track*)(patElectron->gsfTrack().get());  
        const reco::HitPattern& p_inner = elec_track->trackerExpectedHitsInner(); 

         *outputStream_ << " conversion info: " << std::endl
         << "    has matched conversion = " << hasMatchedConversion << std::endl
         << "    # missing inner pixel hits = " << p_inner.numberOfHits() << std::endl;
    }

    *outputStream_ << std::endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATElectronDump, "PATElectronDump");
DEFINE_EDM_PLUGIN(ObjectDumpPluginFactory, PATElectronDump, "PATElectronDump");
