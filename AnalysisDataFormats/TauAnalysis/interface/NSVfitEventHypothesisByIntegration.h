#ifndef AnalysisDataFormats_TauAnalysis_NSVfitEventHypothesisByIntegration_h
#define AnalysisDataFormats_TauAnalysis_NSVfitEventHypothesisByIntegration_h

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisBase.h"

#include <TH1.h>

#include <string>

class NSVfitEventHypothesisByIntegration : public NSVfitEventHypothesisBase
{
 public:
  NSVfitEventHypothesisByIntegration() {}
  NSVfitEventHypothesisByIntegration(const edm::Ptr<reco::Candidate>& met) 
    : NSVfitEventHypothesisBase(met)
  {}
  NSVfitEventHypothesisByIntegration(const NSVfitEventHypothesisByIntegration&);
  virtual ~NSVfitEventHypothesisByIntegration() {}

  virtual NSVfitEventHypothesisByIntegration* clone() const { return new NSVfitEventHypothesisByIntegration(*this); }

  virtual NSVfitEventHypothesisByIntegration& operator=(const NSVfitEventHypothesisByIntegration&);

  const TH1* histMassResults() const { return histMassResults_.get(); }

 private:

  /// Nd-histogram containing full correlations of masses reconstructed for resonances
  std::auto_ptr<TH1> histMassResults_;
};

#endif


