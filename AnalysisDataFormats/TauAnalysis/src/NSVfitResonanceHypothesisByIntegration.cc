#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisByIntegration.h"

NSVfitResonanceHypothesisByIntegration::NSVfitResonanceHypothesisByIntegration(const NSVfitResonanceHypothesisByIntegration& bluePrint)
  : NSVfitResonanceHypothesisBase(bluePrint),
    massMean_(bluePrint.massMean_),
    massMedian_(bluePrint.massMedian_),
    massMaximum_(bluePrint.massMaximum_),
    massMaxInterpol_(bluePrint.massMaxInterpol_)
{}

NSVfitResonanceHypothesisByIntegration::NSVfitResonanceHypothesisByIntegration(const NSVfitResonanceHypothesisBase& bluePrint)
  : NSVfitResonanceHypothesisBase(bluePrint)
{
  daughters_.clear();
  size_t numDaughters = bluePrint.numDaughters();
  for ( size_t iDaughter = 0; iDaughter < numDaughters; ++iDaughter ) {
    std::auto_ptr<NSVfitSingleParticleHypothesisBase> daughter_byIntegration(bluePrint.daughter(iDaughter)->reduceToBase());
    daughters_.push_back(daughter_byIntegration);
  }
}

NSVfitResonanceHypothesisByIntegration& NSVfitResonanceHypothesisByIntegration::operator=(
  const NSVfitResonanceHypothesisByIntegration& bluePrint)
{
  NSVfitResonanceHypothesisBase::operator=(bluePrint);
  massMean_ = bluePrint.massMean_;
  massMedian_ = bluePrint.massMedian_;
  massMaximum_ = bluePrint.massMaximum_;
  massMaxInterpol_ = bluePrint.massMaxInterpol_;
  return (*this);
}
