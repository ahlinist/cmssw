#ifndef AnalysisDataFormats_TauAnalysis_NSVfitTauDecayHypothesis_h
#define AnalysisDataFormats_TauAnalysis_NSVfitTauDecayHypothesis_h

/*
 * NSVfitTauDecayHypothesis
 *
 * Class which holds a representation of a decay of a tau lepton.
 *
 * Authors: Evan K. Friis, Christian Veelken, UC Davis
 *
 */

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

class NSVfitTauDecayHypothesis : public NSVfitSingleParticleHypothesisBase
{
 public:
  NSVfitTauDecayHypothesis() {}
  NSVfitTauDecayHypothesis(const edm::Ptr<reco::Candidate>& particle,
      const std::string& name, int barcode)
    : NSVfitSingleParticleHypothesisBase(particle, name, barcode) 
  {}
  ~NSVfitTauDecayHypothesis() {}
  // Copy constructors
  NSVfitTauDecayHypothesis(const NSVfitTauDecayHypothesis&);
  virtual NSVfitTauDecayHypothesis* clone() const { return new NSVfitTauDecayHypothesis(*this); }
  virtual NSVfitTauDecayHypothesis& operator=(const NSVfitTauDecayHypothesis&);

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

  /// tau lepton hadronic decay mode
  /// (as defined in DataFormats/TauReco/interface/PFTauDecayMode.h)
  int decayMode() const { return decayMode_; }

  /// tau lepton polarization
  double polarization() const { return polarization_; }

  friend class NSVfitTauDecayBuilderBase;

 protected:
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

  /// tau lepton hadronic decay mode
  int decayMode_;

  /// tau lepton polarization
  double polarization_;
};

#endif /* end of include guard: AnalysisDataFormats_TauAnalysis_NSVfitTauDecayHypothesis_h */
