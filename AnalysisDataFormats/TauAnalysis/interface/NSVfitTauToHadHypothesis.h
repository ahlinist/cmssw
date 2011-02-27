#ifndef AnalysisDataFormats_TauAnalysis_NSVfitTauToHadHypothesis_h
#define AnalysisDataFormats_TauAnalysis_NSVfitTauToHadHypothesis_h

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

#include "DataFormats/PatCandidates/interface/Tau.h"

class NSVfitTauToHadHypothesis : public NSVfitSingleParticleHypothesisBase
{
 public:

  /// decay angle in tau lepton rest-frame
  double decay_angle_rf() const { return decay_angle_rf_; }

  friend class NSVfitTauToHadBuilder;

 private:

  /// decay angle in tau lepton rest-frame
  double decay_angle_rf_;
};

#endif
