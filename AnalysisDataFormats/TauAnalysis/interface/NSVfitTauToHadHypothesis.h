#ifndef AnalysisDataFormats_TauAnalysis_NSVfitTauToHadHypothesis_h
#define AnalysisDataFormats_TauAnalysis_NSVfitTauToHadHypothesis_h

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

#include "DataFormats/PatCandidates/interface/Tau.h"

class NSVfitTauToHadHypothesis : public NSVfitSingleParticleHypothesisBase
{
 public:
  NSVfitTauToHadHypothesis() {}
  NSVfitTauToHadHypothesis(const edm::Ptr<reco::Candidate>& particle, const std::string& name, int barcode) 
    : NSVfitSingleParticleHypothesisBase(particle, name, barcode)
  {}
  ~NSVfitTauToHadHypothesis() {}

  /// direction and mass of visible decay products
  const reco::Candidate::Vector& p3Vis_unit() const { return p3Vis_unit_; }
  double visMass() const { return visMass_; }

  /// energy ratio of visible decay products/tau lepton energy
  double visEnFracX() const { return visEnFracX_; }

  /// decay angle in tau lepton rest-frame
  double decay_angle_rf() const { return decay_angle_rf_; }

  friend class NSVfitTauToHadBuilder;

 private:
  /// direction and mass of visible decay products
  reco::Candidate::Vector p3Vis_unit_;
  double visMass_;

  /// energy ratio of visible decay products/tau lepton energy
  double visEnFracX_;

  /// decay angle in tau lepton rest-frame
  double decay_angle_rf_;
};

#endif
