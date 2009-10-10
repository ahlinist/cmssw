#ifndef __AnalysisDataFormats_TauAnalysis_ZllHypothesisT1T2_h__
#define __AnalysisDataFormats_TauAnalysis_ZllHypothesisT1T2_h__

/** \class ZllHypothesisT1T2
 *
 * Information about compatibility of an 
 *  elecron + tau-jet, muon + tau-jet or electron + muon pair
 * with the hypothesis of being an 
 *  electron + electron or muon + muon pair
 * resulting from a 
 *  Z --> e+ e- or Z --> mu+ mu-
 * decay
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: ZllHypothesisT1T2.h,v 1.2 2009/07/29 13:10:30 veelken Exp $
 *
 */

#include "DataFormats/Common/interface/Ptr.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"

#include "DataFormats/Candidate/interface/Particle.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

namespace ZllHypothesis 
{
  enum { kCaloJet, kPFJet, kRecoTrack, kGsfElecron, kGsfTrack, kUndefined };
}

template<typename T1, typename T2>
class ZllHypothesisT1T2
{
  typedef edm::Ptr<CompositePtrCandidateT1T2MEt<T1,T2> > diCandidatePairPtr;

  typedef edm::Ptr<reco::GenParticle> GenParticlePtr;
  typedef edm::Ptr<reco::CaloJet> CaloJetPtr;
  typedef edm::Ptr<reco::PFJet> PFJetPtr;
  typedef edm::Ptr<reco::Track> TrackPtr;
  typedef edm::Ptr<reco::GsfElectron> GsfElectronPtr;
  typedef edm::Ptr<reco::GsfTrack> GsfTrackPtr;

 public:
  /// default constructor 
  ZllHypothesisT1T2() {}

  /// constructor 
  ZllHypothesisT1T2(const diCandidatePairPtr& diCandidatePair)
    : diCandidatePair_(diCandidatePair) {}

  /// destructor
  ~ZllHypothesisT1T2() {}

  /// access to reconstructed di-tau pair
  diCandidatePairPtr diCandidatePair() const { return diCandidatePair_; }
  
  /// access to generated electrons/muons
  /// (if matching reconstructed di-tau pair) 
  const GenParticlePtr genLepton1() const { return genLepton1_; }
  const GenParticlePtr genLepton2() const { return genLepton2_; }

  /// get combination of lepton1 and lepton2 four-vectors 
  /// that best match nominal Z0 mass
  const reco::Particle::LorentzVector& p4Lepton1bestMatch() const { return p4Lepton1bestMatch_; }
  int typeLepton1bestMatch() const { return typeLepton1bestMatch_; }
  const reco::Particle::LorentzVector& p4Lepton2bestMatch() const { return p4Lepton2bestMatch_; }
  int typeLepton2bestMatch() const { return typeLepton2bestMatch_; }

  /// get four-momentum of hypothetic Z0
  const reco::Candidate::LorentzVector p4ZbestMatch() const { return (p4Lepton1bestMatch_ + p4Lepton2bestMatch_); }

  /// access to CaloJet, PFJet, recoTrack, gsfElectron, gsfTrack
  /// matched to first electron/muon
  const CaloJetPtr lepton1matchedCaloJet() const { return lepton1matchedCaloJet_; }
  const PFJetPtr lepton1matchedPFJet() const { return lepton1matchedPFJet_; }
  const TrackPtr lepton1matchedTrack() const { return lepton1matchedTrack_; }
  const GsfElectronPtr lepton1matchedGsfElectron() const { return lepton1matchedGsfElectron_; }
  const GsfTrackPtr lepton1matchedGsfTrack() const { return lepton1matchedGsfTrack_; }

  /// access to CaloJet, PFJet, recoTrack, gsfElectron, gsfTrack
  /// matched to second electron/muon
  const CaloJetPtr lepton2matchedCaloJet() const { return lepton2matchedCaloJet_; }
  const PFJetPtr lepton2matchedPFJet() const { return lepton2matchedPFJet_; }
  const TrackPtr lepton2matchedTrack() const { return lepton2matchedTrack_; }
  const GsfElectronPtr lepton2matchedGsfElectron() const { return lepton2matchedGsfElectron_; }
  const GsfTrackPtr lepton2matchedGsfTrack() const { return lepton2matchedGsfTrack_; }

 private:

  /// allow only ZllHypothesisT1T2Algorithm to change values of data-members
  template<typename U1, typename U2> friend class ZllHypothesisT1T2Algorithm;

  /// reference to di-tau pair object
  diCandidatePairPtr diCandidatePair_;

  /// generated electrons/muons
  /// (if matching reconstructed di-tau pair) 
  GenParticlePtr genLepton1_;
  GenParticlePtr genLepton2_;

  /// combination of lepton1 and lepton2 four-vectors 
  /// that best match nominal Z0 mass
  reco::Particle::LorentzVector p4Lepton1bestMatch_; 
  int typeLepton1bestMatch_;
  reco::Particle::LorentzVector p4Lepton2bestMatch_; 
  int typeLepton2bestMatch_;

  /// references to CaloJet, PFJet, recoTrack, gsfElectron, gsfTrack
  /// matched to first (hypothetic) electron/muon
  CaloJetPtr lepton1matchedCaloJet_;
  PFJetPtr lepton1matchedPFJet_;
  TrackPtr lepton1matchedTrack_;
  GsfElectronPtr lepton1matchedGsfElectron_;
  GsfTrackPtr lepton1matchedGsfTrack_;

  /// references to CaloJet, PFJet, recoTrack, gsfElectron, gsfTrack
  /// matched to second (hypothetic) electron/muon
  CaloJetPtr lepton2matchedCaloJet_;
  PFJetPtr lepton2matchedPFJet_;
  TrackPtr lepton2matchedTrack_;
  GsfElectronPtr lepton2matchedGsfElectron_;
  GsfTrackPtr lepton2matchedGsfTrack_;
};

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef ZllHypothesisT1T2<pat::Electron, pat::Tau> ZllHypothesisElecTau;
typedef ZllHypothesisT1T2<pat::Muon, pat::Tau> ZllHypothesisMuTau;
typedef ZllHypothesisT1T2<pat::Tau, pat::Tau> ZllHypothesisDiTau;
typedef ZllHypothesisT1T2<pat::Electron, pat::Muon> ZllHypothesisElecMu;

#endif
