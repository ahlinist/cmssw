#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h"

NSVfitEventHypothesis::NSVfitEventHypothesis(const NSVfitEventHypothesis& bluePrint)
  : NSVfitEventHypothesisBase(bluePrint),
    p4_(bluePrint.p4_),
    dp4_(bluePrint.dp4_),
    p4MEt_(bluePrint.p4MEt_)
{
  size_t numResonances = resonances_.size();
  for ( size_t iResonance = 0; iResonance < numResonances; ++iResonance ) {
    this->resonance(iResonance)->setEventHypothesis(this);	
  }
}

NSVfitEventHypothesis& NSVfitEventHypothesis::operator=(const NSVfitEventHypothesis& bluePrint)
{
  NSVfitEventHypothesisBase::operator=(bluePrint);
  p4_ = bluePrint.p4_;
  dp4_ = bluePrint.dp4_;
  p4MEt_ = bluePrint.p4MEt_;
  size_t numResonances = resonances_.size();
  for ( size_t iResonance = 0; iResonance < numResonances; ++iResonance ) {
    this->resonance(iResonance)->setEventHypothesis(this);
  }
  return (*this);
}
