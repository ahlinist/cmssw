#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

bool operator<(const NSVfitSingleParticleHypothesisBase& hypothesis1, const NSVfitSingleParticleHypothesisBase& hypothesis2)
{
  return (hypothesis1.barcode() < hypothesis2.barcode());
}

