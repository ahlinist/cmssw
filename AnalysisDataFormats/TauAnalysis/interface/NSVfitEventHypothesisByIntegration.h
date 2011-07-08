#ifndef AnalysisDataFormats_TauAnalysis_NSVfitEventHypothesisByIntegration_h
#define AnalysisDataFormats_TauAnalysis_NSVfitEventHypothesisByIntegration_h

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisBase.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisByIntegration.h"

#include "DataFormats/Common/interface/OwnVector.h"
#include "DataFormats/Common/interface/ClonePolicy.h"

#include <TH1.h>

#include <string>

struct TH1ClonePolicy;

class NSVfitEventHypothesisByIntegration : public NSVfitEventHypothesisBase
{
 public:

  NSVfitEventHypothesisByIntegration() {}
  NSVfitEventHypothesisByIntegration(const edm::Ptr<reco::Candidate>& met) 
    : NSVfitEventHypothesisBase(met)
  {}
  NSVfitEventHypothesisByIntegration(const NSVfitEventHypothesisByIntegration&);
  NSVfitEventHypothesisByIntegration(const NSVfitEventHypothesisBase&);
  virtual ~NSVfitEventHypothesisByIntegration() {}

  virtual NSVfitEventHypothesisByIntegration* clone() const { return new NSVfitEventHypothesisByIntegration(*this); }

  virtual NSVfitEventHypothesisByIntegration& operator=(const NSVfitEventHypothesisByIntegration&);

  /// fit hypotheses of lepton resonances
  NSVfitResonanceHypothesisByIntegration* resonance(size_t idx) { 
    return dynamic_cast<NSVfitResonanceHypothesisByIntegration*>(&resonances_[idx]); 
  }
  const NSVfitResonanceHypothesisByIntegration* resonance(size_t idx) const { 
    return dynamic_cast<const NSVfitResonanceHypothesisByIntegration*>(&resonances_[idx]); 
  }

  const TH1* histMassResults() const { return histMassResults_.get(); }

  friend class NSVfitAlgorithmByIntegration;

 private:

  /// Nd-histogram containing full correlations of masses reconstructed for resonances
  std::auto_ptr<TH1> histMassResults_;
};

#endif


