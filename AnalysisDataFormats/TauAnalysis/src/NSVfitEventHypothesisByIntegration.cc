#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisByIntegration.h"

NSVfitEventHypothesisByIntegration::NSVfitEventHypothesisByIntegration(const NSVfitEventHypothesisByIntegration& bluePrint)
  : NSVfitEventHypothesisBase(bluePrint)
{
  histMassResults_.reset((TH1*)bluePrint.histMassResults_->Clone());
}

NSVfitEventHypothesisByIntegration::NSVfitEventHypothesisByIntegration(const NSVfitEventHypothesisBase& bluePrint)
  : NSVfitEventHypothesisBase(bluePrint)
{
  resonances_.clear();
  size_t numResonances = bluePrint.numResonances();
  for ( size_t iResonance = 0; iResonance < numResonances; ++iResonance ) {
    std::auto_ptr<NSVfitResonanceHypothesisBase> resonance_byIntegration(
      new NSVfitResonanceHypothesisByIntegration(*bluePrint.resonance(iResonance)));
    resonances_.push_back(resonance_byIntegration);
  }
}

NSVfitEventHypothesisByIntegration& NSVfitEventHypothesisByIntegration::operator=(const NSVfitEventHypothesisByIntegration& bluePrint)
{
  NSVfitEventHypothesisBase::operator=(bluePrint);
  histMassResults_.reset((TH1*)bluePrint.histMassResults_->Clone());
  return (*this);
}
