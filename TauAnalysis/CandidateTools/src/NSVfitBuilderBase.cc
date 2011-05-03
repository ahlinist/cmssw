#include "TauAnalysis/CandidateTools/interface/NSVfitBuilderBase.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"

int NSVfitBuilderBase::getFitParameterIdx(NSVfitAlgorithmBase* algorithm, const std::string& name, int type, bool isOptional)
{
  NSVfitParameter* fitParameter = algorithm->getFitParameter(name, type);
  if ( fitParameter ) {
    return fitParameter->index();
  } else {
    if ( isOptional ) return -1;
    else throw cms::Exception("NSVfitBuilderBase::getFitParameterIdx")
      << " No fitParameter = " << get_name_incl_type(name, type) << " defined !!\n"
      << "--> Please check if there is a likelihood which constrains this fitParameter.\n";
  }
}
