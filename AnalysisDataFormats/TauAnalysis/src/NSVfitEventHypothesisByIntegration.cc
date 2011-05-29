#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisByIntegration.h"

NSVfitEventHypothesisByIntegration::NSVfitEventHypothesisByIntegration(const NSVfitEventHypothesisByIntegration& bluePrint)
  : NSVfitEventHypothesisBase(bluePrint)
{
  if ( bluePrint.histMassResults_.get() != 0 ) histMassResults_ = std::auto_ptr<TH1>((TH1*)bluePrint.histMassResults_->Clone());
}

NSVfitEventHypothesisByIntegration& NSVfitEventHypothesisByIntegration::operator=(const NSVfitEventHypothesisByIntegration& bluePrint)
{
  NSVfitEventHypothesisBase::operator=(bluePrint);
  if ( bluePrint.histMassResults_.get() != 0 ) histMassResults_ = std::auto_ptr<TH1>((TH1*)bluePrint.histMassResults_->Clone());
  return (*this);
}
