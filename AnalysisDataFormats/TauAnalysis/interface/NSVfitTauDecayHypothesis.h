#ifndef AnalysisDataFormats_TauAnalysis_NSVfitTauDecayHypothesis_h
#define AnalysisDataFormats_TauAnalysis_NSVfitTauDecayHypothesis_h

/*
 * NSVfitTauDecayHypothesis
 *
 * Base class representing tau lepton decay to electrons, muons or into hadrons.
 *
 * NOTE: this base-class is used to make persistent solutions of NSVfitAlgorithmByLikelihoodMaximization;
 *       solutions of NSVfitAlgorithmByIntegration are of type NSVfitTauDecayHypothesisBase
 *
 * Authors: Evan K. Friis, Christian Veelken, UC Davis
 *
 */

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/Ptr.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauDecayHypothesisBaseT.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

class NSVfitTauDecayHypothesis : public NSVfitTauDecayHypothesisBaseT<NSVfitSingleParticleHypothesis>
{
 public:

  NSVfitTauDecayHypothesis() {}
  NSVfitTauDecayHypothesis(const edm::Ptr<reco::Candidate>& particle, const std::string& name, int barcode)
    : NSVfitTauDecayHypothesisBaseT<NSVfitSingleParticleHypothesis>(particle, name, barcode)
  {
    p4_ = particle->p4();
  }
  NSVfitTauDecayHypothesis(const NSVfitTauDecayHypothesis& bluePrint)
    : NSVfitTauDecayHypothesisBaseT<NSVfitSingleParticleHypothesis>(bluePrint),
      tracks_(bluePrint.tracks_),
      p4invis_rf_(bluePrint.p4invis_rf_),
      p4vis_rf_(bluePrint.p4vis_rf_),
      p3Vis_unit_(bluePrint.p3Vis_unit_),
      visMass_(bluePrint.visMass_),
      visEnFracX_(bluePrint.visEnFracX_),
      decay_angle_rf_(bluePrint.decay_angle_rf_),
      decayVertexPos_(bluePrint.decayVertexPos_),
      flightPath_(bluePrint.flightPath_),
      decayDistance_(bluePrint.decayDistance_)
  {}

  ~NSVfitTauDecayHypothesis() {}

  virtual NSVfitTauDecayHypothesis& operator=(const NSVfitTauDecayHypothesis& bluePrint)
  {
    NSVfitTauDecayHypothesisBaseT<NSVfitSingleParticleHypothesis>::operator=(bluePrint);
    tracks_ = bluePrint.tracks_;
    p4invis_rf_ = bluePrint.p4invis_rf_;
    p4vis_rf_ = bluePrint.p4vis_rf_;
    p3Vis_unit_ = bluePrint.p3Vis_unit_;
    visMass_ = bluePrint.visMass_;
    visEnFracX_ = bluePrint.visEnFracX_;
    decay_angle_rf_ = bluePrint.decay_angle_rf_;
    decayVertexPos_ = bluePrint.decayVertexPos_;
    flightPath_ = bluePrint.flightPath_;
    decayDistance_ = bluePrint.decayDistance_;
    return (*this);
  }

  /// collection of tracks associated to reco::Candidate
  virtual const std::vector<const reco::Track*>& tracks() const { return tracks_; }

  /// momenta of visible (electron/muon) and invisible (neutrinos)
  /// decay products in tau lepton rest-frame
  const reco::Candidate::LorentzVector& p4vis_rf()   const { return p4vis_rf_;   }
  const reco::Candidate::LorentzVector& p4invis_rf() const { return p4invis_rf_; }

  /// direction and mass of visible decay products
  const reco::Candidate::Vector& p3Vis_unit() const { return p3Vis_unit_; }
  double visMass() const { return visMass_; }

  /// energy ratio of visible decay products/tau lepton energy
  double visEnFracX() const { return visEnFracX_; }

  /// decay angle in tau lepton rest-frame
  double decay_angle_rf() const { return decay_angle_rf_; }

  /// access to position of secondary vertex (tau lepton decay vertex)
  bool hasDecayVertex() const { return true; }
  const AlgebraicVector3& decayVertexPos() const { return decayVertexPos_; }
  const reco::Candidate::Vector& flightPath() const { return flightPath_; }
  double decayDistance() const { return decayDistance_; }

  friend class NSVfitTauDecayBuilder;
  template<typename T1, typename T2> friend class CompositePtrCandidateT1T2MEt;

 protected:

  /// collection of tracks associated to reco::Candidate
  std::vector<const reco::Track*> tracks_; // transient data-member

  /// momenta of visible (electron/muon) and invisible (neutrinos)
  /// decay products in tau lepton rest-frame
  reco::Candidate::LorentzVector p4invis_rf_;
  reco::Candidate::LorentzVector p4vis_rf_;

  /// direction and mass of visible decay products
  reco::Candidate::Vector p3Vis_unit_;
  double visMass_;

  /// energy ratio of visible decay products/tau lepton energy
  double visEnFracX_;

  /// decay angle in tau lepton rest-frame
  double decay_angle_rf_;

  /// position of reconstructed tau lepton decay vertex
  AlgebraicVector3 decayVertexPos_;
  reco::Candidate::Vector flightPath_;
  double decayDistance_;
};

#endif /* end of include guard: AnalysisDataFormats_TauAnalysis_NSVfitTauDecayHypothesis_h */
