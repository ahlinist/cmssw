#ifndef AnalysisDataFormats_TauAnalysis_NSVfitTauToHadHypothesis_h
#define AnalysisDataFormats_TauAnalysis_NSVfitTauToHadHypothesis_h

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauDecayHypothesis.h"

#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/CLHEP/interface/AlgebraicObjects.h"

class NSVfitTauToHadHypothesis : public NSVfitTauDecayHypothesis
{
 public:
  NSVfitTauToHadHypothesis() {}
  NSVfitTauToHadHypothesis(const edm::Ptr<reco::Candidate>& particle, const std::string& name, int barcode)
    : NSVfitTauDecayHypothesis(particle, name, barcode)
  {}
  ~NSVfitTauToHadHypothesis() {}

  NSVfitTauToHadHypothesis(const NSVfitTauToHadHypothesis&);
  virtual NSVfitTauDecayHypothesis* clone() const { return new NSVfitTauToHadHypothesis(*this); }
  virtual NSVfitTauDecayHypothesis& operator=(const NSVfitTauToHadHypothesis&);

  /// decay angles and masses of intermediate vector-meson resonances
  double decay_angle_VMrho() const { return decay_angle_VMrho_; }
  double mass2_VMrho() const { return mass2_VMrho_; }
  double decay_angle_VMa1() const { return decay_angle_VMa1_; }
  double decay_angle_VMa1r_theta() const { return decay_angle_VMa1r_theta_; }
  double decay_angle_VMa1r_phi() const { return decay_angle_VMa1r_phi_; }
  double mass2_VMa1() const { return mass2_VMa1_; }

  bool hasFittedVertex() const { return fittedVertex_.isValid(); }
  virtual const reco::Vertex& fittedVertex() const {
    return fittedVertex_;
  }

  friend class NSVfitTauToHadBuilder;

 private:
  /// decay angles and masses of intermediate vector-meson resonances
  double decay_angle_VMrho_;
  double mass2_VMrho_;
  double decay_angle_VMa1_;
  double decay_angle_VMa1r_theta_;
  double decay_angle_VMa1r_phi_;
  double mass2_VMa1_;
  reco::Vertex fittedVertex_;
};

#endif
