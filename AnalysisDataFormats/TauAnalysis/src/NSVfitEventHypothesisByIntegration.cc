#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisByIntegration.h"

NSVfitEventHypothesisByIntegration::NSVfitEventHypothesisByIntegration(const NSVfitEventHypothesisByIntegration& bluePrint)
  : NSVfitEventHypothesisBase(bluePrint)
{
  if ( bluePrint.histMassResults_.get() != 0 ) histMassResults_ = std::auto_ptr<TH1>((TH1*)bluePrint.histMassResults_->Clone());
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
  if ( bluePrint.histMassResults_.get() != 0 ) histMassResults_ = std::auto_ptr<TH1>((TH1*)bluePrint.histMassResults_->Clone());
  return (*this);
}
