#ifndef AnalysisDataFormats_TauAnalysis_NSVfitTauToHadHypothesis_h
#define AnalysisDataFormats_TauAnalysis_NSVfitTauToHadHypothesis_h

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/CLHEP/interface/AlgebraicObjects.h"

class NSVfitTauToHadHypothesis : public NSVfitSingleParticleHypothesisBase
{
 public:
  NSVfitTauToHadHypothesis() {}
  NSVfitTauToHadHypothesis(const edm::Ptr<reco::Candidate>& particle, const std::string& name, int barcode) 
    : NSVfitSingleParticleHypothesisBase(particle, name, barcode)
  {}
  NSVfitTauToHadHypothesis(const NSVfitTauToHadHypothesis&);
  ~NSVfitTauToHadHypothesis() {}

  NSVfitSingleParticleHypothesisBase* clone() const { return new NSVfitTauToHadHypothesis(*this); }

  virtual NSVfitSingleParticleHypothesisBase& operator=(const NSVfitTauToHadHypothesis&);

  /// direction and mass of visible decay products
  const reco::Candidate::Vector& p3Vis_unit() const { return p3Vis_unit_; }
  double visMass() const { return visMass_; }

  /// energy ratio of visible decay products/tau lepton energy
  double visEnFracX() const { return visEnFracX_; }

  /// decay angle in tau lepton rest-frame
  double decay_angle_rf() const { return decay_angle_rf_; }

  /// decay angles and masses of intermediate vector-meson resonances
  double decay_angle_VMrho() const { return decay_angle_VMrho_; }
  double mass2_VMrho() const { return mass2_VMrho_; }
  double decay_angle_VMa1() const { return decay_angle_VMa1_; }
  double decay_angle_VMa1r_theta() const { return decay_angle_VMa1r_theta_; }
  double decay_angle_VMa1r_phi() const { return decay_angle_VMa1r_phi_; }
  double mass2_VMa1() const { return mass2_VMa1_; }

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

  friend class NSVfitTauToHadBuilder;

 private:
  /// direction and mass of visible decay products
  reco::Candidate::Vector p3Vis_unit_;
  double visMass_;

  /// energy ratio of visible decay products/tau lepton energy
  double visEnFracX_;

  /// decay angle in tau lepton rest-frame
  double decay_angle_rf_;

  /// decay angles and masses of intermediate vector-meson resonances
  double decay_angle_VMrho_;
  double mass2_VMrho_;
  double decay_angle_VMa1_;
  double decay_angle_VMa1r_theta_;
  double decay_angle_VMa1r_phi_;
  double mass2_VMa1_;

  /// position of reconstructed tau lepton decay vertex
  AlgebraicVector3 decayVertexPos_;
  reco::Candidate::Vector flightPath_;
  double decayDistance_;

  /// tau lepton hadronic decay mode
  int decayMode_;

  /// tau lepton polarization
  double polarization_;
};

#endif
