#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

NSVfitSingleParticleHypothesisBase::NSVfitSingleParticleHypothesisBase(const NSVfitSingleParticleHypothesisBase& bluePrint)
  : mother_(bluePrint.mother_),
    name_(bluePrint.name_),
    barcode_(bluePrint.barcode_),
    particle_(bluePrint.particle_),
    p4_(bluePrint.p4_),
    p4_fitted_(bluePrint.p4_fitted_),
    dp4_(bluePrint.dp4_),
    tracks_(bluePrint.tracks_)
{}

NSVfitSingleParticleHypothesisBase& NSVfitSingleParticleHypothesisBase::operator =(const NSVfitSingleParticleHypothesisBase& bluePrint)
{
  mother_ = bluePrint.mother_;
  name_ = bluePrint.name_;
  barcode_ = bluePrint.barcode_;
  particle_ = bluePrint.particle_;
  p4_ = bluePrint.p4_;
  p4_fitted_ = bluePrint.p4_fitted_;
  dp4_ = bluePrint.dp4_;
  tracks_ = bluePrint.tracks_;
  return (*this);
}

//
//-------------------------------------------------------------------------------
//

bool operator<(const NSVfitSingleParticleHypothesisBase& hypothesis1, const NSVfitSingleParticleHypothesisBase& hypothesis2)
{
  return (hypothesis1.p4().pt() < hypothesis2.p4().pt());
}
