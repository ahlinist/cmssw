#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesis.h"

NSVfitSingleParticleHypothesis::NSVfitSingleParticleHypothesis(const NSVfitSingleParticleHypothesis& bluePrint)
  : NSVfitSingleParticleHypothesisBase(bluePrint),
    mother_(bluePrint.mother_),
    p4_(bluePrint.p4_),
    p4_fitted_(bluePrint.p4_fitted_),
    dp4_(bluePrint.dp4_)
{}

NSVfitSingleParticleHypothesis& NSVfitSingleParticleHypothesis::operator=(const NSVfitSingleParticleHypothesis& bluePrint)  
{  
  NSVfitSingleParticleHypothesisBase::operator=(bluePrint);
  mother_ = bluePrint.mother_;
  p4_ = bluePrint.p4_;
  p4_fitted_ = bluePrint.p4_fitted_;
  dp4_ = bluePrint.dp4_;
  return (*this);
}

