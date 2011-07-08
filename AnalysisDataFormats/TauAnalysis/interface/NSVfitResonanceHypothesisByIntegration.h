#ifndef AnalysisDataFormats_TauAnalysis_NSVfitResonanceHypothesisByIntegration_h
#define AnalysisDataFormats_TauAnalysis_NSVfitResonanceHypothesisByIntegration_h

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisBase.h"

#include <string>

class NSVfitEventHypothesis;

class NSVfitResonanceHypothesisByIntegration : public NSVfitResonanceHypothesisBase
{
 public:

  NSVfitResonanceHypothesisByIntegration() {}
  NSVfitResonanceHypothesisByIntegration(const NSVfitResonanceHypothesisByIntegration&);
  NSVfitResonanceHypothesisByIntegration(const NSVfitResonanceHypothesisBase&);
  virtual ~NSVfitResonanceHypothesisByIntegration() {}

  virtual NSVfitResonanceHypothesisByIntegration* clone() const { return new NSVfitResonanceHypothesisByIntegration(*this); }

  virtual NSVfitResonanceHypothesisByIntegration& operator=(const NSVfitResonanceHypothesisByIntegration&);

  /// mean and median reconstructed mass
  double mass_mean() const { return massMean_; }
  double mass_median() const { return massMedian_; }
  double mass_maximum() const { return massMaximum_; }
  double mass_maxInterpol() const { return massMaxInterpol_; }

  friend class NSVfitAlgorithmByIntegration;

 private:

  /// mean and median reconstructed mass
  double massMean_;
  double massMedian_;
  double massMaximum_;
  double massMaxInterpol_;
};

#endif
