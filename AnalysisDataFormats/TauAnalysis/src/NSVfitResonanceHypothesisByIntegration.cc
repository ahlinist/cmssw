#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisByIntegration.h"

NSVfitResonanceHypothesisByIntegration::NSVfitResonanceHypothesisByIntegration(const NSVfitResonanceHypothesisByIntegration& bluePrint)
  : NSVfitResonanceHypothesisBase(bluePrint),
    massMean_(bluePrint.massMean_),
    massMedian_(bluePrint.massMedian_),
    massMaximum_(bluePrint.massMaximum_),
    massMaxInterpol_(bluePrint.massMaxInterpol_)
{}

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
