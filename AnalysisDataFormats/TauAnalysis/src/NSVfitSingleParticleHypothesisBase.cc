#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

NSVfitSingleParticleHypothesisBase::NSVfitSingleParticleHypothesisBase(const NSVfitSingleParticleHypothesisBase& bluePrint)
  : name_(bluePrint.name_),
    barcode_(bluePrint.barcode_),
    particle_(bluePrint.particle_)
{}

NSVfitSingleParticleHypothesisBase& NSVfitSingleParticleHypothesisBase::operator =(const NSVfitSingleParticleHypothesisBase& bluePrint)
{
  name_ = bluePrint.name_;
  barcode_ = bluePrint.barcode_;
  particle_ = bluePrint.particle_;
  return (*this);
}

//
//-------------------------------------------------------------------------------
//

bool operator<(const NSVfitSingleParticleHypothesisBase& hypothesis1, const NSVfitSingleParticleHypothesisBase& hypothesis2)
{
  return (hypothesis1.barcode() < hypothesis2.barcode());
}

