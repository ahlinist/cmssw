#ifndef TauAnalysis_RecoTools_ZllHypothesisT1T2Algorithm_h
#define TauAnalysis_RecoTools_ZllHypothesisT1T2Algorithm_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Candidate/interface/Particle.h" 
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include "AnalysisDataFormats/TauAnalysis/interface/ZllHypothesisT1T2.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

#include <TMath.h>

edm::InputTag ZllHypothesisT1T2_getInputTag(const edm::ParameterSet& cfg, const char* label)
{
  return cfg.exists(label) ? cfg.getParameter<edm::InputTag>(label) : edm::InputTag();
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

template<typename T1, typename T2>
class ZllHypothesisT1T2Algorithm
{
  typedef edm::Ptr<reco::GenParticle> GenParticlePtr;
  typedef edm::Ptr<reco::CaloJet> CaloJetPtr;
  typedef edm::Ptr<reco::PFJet> PFJetPtr;
  typedef edm::Ptr<reco::Track> TrackPtr;
  typedef edm::Ptr<reco::GsfElectron> GsfElectronPtr;
  typedef edm::Ptr<reco::GsfTrack> GsfTrackPtr;

 public:

  ZllHypothesisT1T2Algorithm(const edm::ParameterSet& cfg)
  {
    srcGenLeptons_ = ZllHypothesisT1T2_getInputTag(cfg, "genLeptonsFromZsSource");
    srcCaloJets_ = ZllHypothesisT1T2_getInputTag(cfg, "caloJetSource");
    srcPFJets_ = ZllHypothesisT1T2_getInputTag(cfg, "pfJetSource");
    srcTracks_ = ZllHypothesisT1T2_getInputTag(cfg, "trackSource");
    srcGsfElectrons_ = ZllHypothesisT1T2_getInputTag(cfg, "gsfElectronSource");
    srcGsfTracks_ = ZllHypothesisT1T2_getInputTag(cfg, "gsfTrackSource");

    tkminPixelHits_ = cfg.getParameter<int>("tkminPixelHits");
    tkminTrackerHits_ = cfg.getParameter<int>("tkminTrackerHits");
    tkmaxChi2_ = cfg.getParameter<double>("tkmaxChi2");
    
    dRmatch_ = cfg.getParameter<double>("dRmatch");
    
    verbosity_ = cfg.getUntrackedParameter<int>("verbosity", 0);
  }

  ~ZllHypothesisT1T2Algorithm() {}

  ZllHypothesisT1T2<T1,T2> buildZllHypothesis(edm::Ptr<CompositePtrCandidateT1T2MEt<T1,T2> > diCandidatePair,
					      const edm::Event& evt, const edm::EventSetup& es)
  {
    ZllHypothesisT1T2<T1,T2> ZllHypothesis(diCandidatePair);

    const reco::Particle::LorentzVector& direction1 = diCandidatePair->leg1()->p4();
    const reco::Particle::LorentzVector& direction2 = diCandidatePair->leg2()->p4();
    
    ZllHypothesis.genLepton1_ = getBestMatchParticle<reco::GenParticle>(evt, srcGenLeptons_, direction1);
    ZllHypothesis.genLepton2_ = getBestMatchParticle<reco::GenParticle>(evt, srcGenLeptons_, direction2);

    ZllHypothesis.lepton1matchedCaloJet_ = getBestMatchParticle<reco::CaloJet>(evt, srcCaloJets_, direction1);
    ZllHypothesis.lepton1matchedPFJet_ = getBestMatchParticle<reco::PFJet>(evt, srcPFJets_, direction1);
    ZllHypothesis.lepton1matchedTrack_ = getBestMatchTrack<reco::Track>(evt, srcTracks_, direction1);
    ZllHypothesis.lepton1matchedGsfElectron_ = getBestMatchParticle<reco::GsfElectron>(evt, srcGsfElectrons_, direction1);
    ZllHypothesis.lepton1matchedGsfTrack_ = getBestMatchTrack<reco::GsfTrack>(evt, srcGsfTracks_, direction1);

    ZllHypothesis.lepton2matchedCaloJet_ = getBestMatchParticle<reco::CaloJet>(evt, srcCaloJets_, direction2);
    ZllHypothesis.lepton2matchedPFJet_ = getBestMatchParticle<reco::PFJet>(evt, srcPFJets_, direction2);
    ZllHypothesis.lepton2matchedTrack_ = getBestMatchTrack<reco::Track>(evt, srcTracks_, direction2);
    ZllHypothesis.lepton2matchedGsfElectron_ = getBestMatchParticle<reco::GsfElectron>(evt, srcGsfElectrons_, direction2);
    ZllHypothesis.lepton2matchedGsfTrack_ = getBestMatchTrack<reco::GsfTrack>(evt, srcGsfTracks_, direction2);

    //edm::ESHandle<HepPDT::ParticleDataTable> pdt;
    //es.getData(pdt);
    //const double nominalZmass = pdt->particle(23)->mass();
    const double nominalZmass = 91.188;

    double visMassBestMatch = 1.e+3;
    reco::Particle::LorentzVector p4Lepton1bestMatch;
    int lepton1bestMatchType = ZllHypothesis::kUndefined;
    reco::Particle::LorentzVector p4Lepton2bestMatch;
    int lepton2bestMatchType = ZllHypothesis::kUndefined;
    for ( int lepton1Type = ZllHypothesis::kCaloJet;
	  lepton1Type <= ZllHypothesis::kGsfTrack; ++lepton1Type ) {
    
      bool isValid1;
      reco::Particle::LorentzVector lepton1Momentum 
	= getP4(ZllHypothesis.lepton1matchedCaloJet_, ZllHypothesis.lepton1matchedPFJet_, ZllHypothesis.lepton1matchedTrack_,
		ZllHypothesis.lepton1matchedGsfElectron_, ZllHypothesis.lepton1matchedGsfTrack_, lepton1Type, isValid1);
      if ( !isValid1 ) continue;
      
      for ( int lepton2Type = ZllHypothesis::kCaloJet;
	    lepton2Type <= ZllHypothesis::kGsfTrack; ++lepton2Type ) {
	
	bool isValid2;
	reco::Particle::LorentzVector lepton2Momentum = 
	  getP4(ZllHypothesis.lepton2matchedCaloJet_, ZllHypothesis.lepton2matchedPFJet_, ZllHypothesis.lepton2matchedTrack_,
		ZllHypothesis.lepton2matchedGsfElectron_, ZllHypothesis.lepton2matchedGsfTrack_, lepton2Type, isValid2);
	if ( !isValid2 ) continue;
	
	double visMass = (lepton1Momentum + lepton2Momentum).mass();

	if ( verbosity_ ) {
	  std::cout << "<ZllHypothesisT1T2Algorithm::buildZllHypothesis>:" << std::endl;
	  std::cout << "lepton1Type = " << lepton1Type << ", lepton1Pt = " << lepton1Momentum.pt() << ";"
		    << " lepton2Type = " << lepton2Type << ", lepton2Pt = " << lepton2Momentum.pt() << ":"
		    << " visMass = " << visMass << std::endl;
	}

	if ( TMath::Abs(visMass - nominalZmass) < TMath::Abs(visMassBestMatch - nominalZmass) ) {
	  p4Lepton1bestMatch = lepton1Momentum;
	  lepton1bestMatchType = lepton1Type;
	  p4Lepton2bestMatch = lepton2Momentum;
	  lepton2bestMatchType = lepton2Type;
	  visMassBestMatch = visMass;
	}
      }
    }
    
    ZllHypothesis.p4Lepton1bestMatch_ = p4Lepton1bestMatch;
    ZllHypothesis.typeLepton1bestMatch_ = lepton1bestMatchType;
    ZllHypothesis.p4Lepton2bestMatch_ = p4Lepton2bestMatch;
    ZllHypothesis.typeLepton2bestMatch_ = lepton2bestMatchType;
    
    if ( verbosity_ ) {
      if ( ZllHypothesis.genLepton1_.isAvailable() &&
	   ZllHypothesis.genLepton2_.isAvailable() ) {
	std::cout << "<ZllHypothesisT1T2Algorithm::buildZllHypothesis>:" << std::endl;
	std::cout << " DiCandidatePair matches generated Z --> e+ e- / Z --> mu+ mu- decay," 
		  << " visMassBestMatch = " << visMassBestMatch << std::endl;
      }
    }
    
    return ZllHypothesis;
  }

 private:

  template <typename T>
  edm::Ptr<T> getBestMatchParticle(const edm::Event& evt, const edm::InputTag& src,
				   const reco::Particle::LorentzVector& direction)
  {
    if ( src.label() == "" ) return edm::Ptr<T>();

    edm::Handle<edm::View<T> > particles;
    evt.getByLabel(src, particles);

    edm::Ptr<T> particle_bestMatch;

    double dRmin = dRmatch_;
    for ( unsigned idx = 0, numParticles = particles->size(); 
	  idx < numParticles; ++idx ) {
      edm::Ptr<T> particle = particles->ptrAt(idx);
      if ( reco::deltaR(particle->p4(), direction) < dRmin ) {
	particle_bestMatch = particle;
      }
    }
  
    return particle_bestMatch;
  }

  template <typename T>
  edm::Ptr<T> getBestMatchTrack(const edm::Event& evt, const edm::InputTag& src,
				const reco::Particle::LorentzVector& direction)
  {
    if ( src.label() == "" ) return edm::Ptr<T>();

    edm::Handle<edm::View<T> > tracks;
    evt.getByLabel(src, tracks);

    edm::Ptr<T> track_bestMatch;
    
    double ptMax = -1.;
    for ( unsigned idx = 0, numTracks = tracks->size(); 
	  idx < numTracks; ++idx ) {
      edm::Ptr<T> track = tracks->ptrAt(idx);

      if ( track->hitPattern().numberOfValidPixelHits() >= tkminPixelHits_ &&
	   track->numberOfValidHits() >= tkminTrackerHits_ &&
	   track->normalizedChi2() < tkmaxChi2_ ) {
	double pt = track->pt();
	if ( reco::deltaR(track->eta(), track->phi(), direction.eta(), direction.phi()) < dRmatch_ && pt > ptMax ) {
	  track_bestMatch = track;
	  ptMax = pt;
	}
      }
    }
  
    return track_bestMatch;
  }

  reco::Particle::LorentzVector getP4(const CaloJetPtr& caloJet, const PFJetPtr& pfJet, const TrackPtr& track,
				      const GsfElectronPtr& gsfElectron, const GsfTrackPtr& gsfTrack,
				      int type, bool& isValid)
  {
    assert(type >= ZllHypothesis::kCaloJet && type <= ZllHypothesis::kGsfTrack);
    
    isValid = true;
    
    if ( type == ZllHypothesis::kCaloJet && caloJet.isAvailable() ) {
      return caloJet->p4();
    } else if ( type == ZllHypothesis::kPFJet && pfJet.isAvailable() ) {
      return pfJet->p4();
    } else if ( type == ZllHypothesis::kRecoTrack && track.isAvailable() ) {
      return reco::Particle::LorentzVector(track->px(), track->py(), track->pz(), track->p());
    } else if ( type == ZllHypothesis::kGsfElecron && gsfElectron.isAvailable() ) {
      return gsfElectron->p4();
    } else if ( type == ZllHypothesis::kGsfTrack && gsfTrack.isAvailable() ) {
      return reco::Particle::LorentzVector(gsfTrack->px(), gsfTrack->py(), gsfTrack->pz(), gsfTrack->p());
    }
    
    isValid = false;
    
    return reco::Particle::LorentzVector(0,0,0,0);
  }

  edm::InputTag srcGenLeptons_;
  edm::InputTag srcCaloJets_;
  edm::InputTag srcPFJets_;
  edm::InputTag srcTracks_;
  edm::InputTag srcGsfElectrons_;
  edm::InputTag srcGsfTracks_;

  int tkminPixelHits_;
  int tkminTrackerHits_;
  double tkmaxChi2_;

  double dRmatch_;

  int verbosity_;  
};

#endif 

