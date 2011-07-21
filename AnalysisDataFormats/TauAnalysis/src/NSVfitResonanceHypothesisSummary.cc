#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisSummary.h"

NSVfitResonanceHypothesisSummary::NSVfitResonanceHypothesisSummary(const NSVfitResonanceHypothesisSummary& bluePrint)
  : name_(bluePrint.name_),
    mass_(bluePrint.mass_),
    massErrUp_(bluePrint.massErrUp_),
    massErrDown_(bluePrint.massErrDown_),
    isValidSolution_(bluePrint.isValidSolution_)
{}

NSVfitResonanceHypothesisSummary::NSVfitResonanceHypothesisSummary(const NSVfitResonanceHypothesisBase& bluePrint)
  : name_(bluePrint.name()),
    mass_(bluePrint.mass()),
    massErrUp_(bluePrint.massErrUp()),
    massErrDown_(bluePrint.massErrDown()),
    isValidSolution_(bluePrint.isValidSolution())
{}

NSVfitResonanceHypothesisSummary& NSVfitResonanceHypothesisSummary::operator=(const NSVfitResonanceHypothesisSummary& bluePrint)
{   
  name_ = bluePrint.name_;
  mass_ = bluePrint.mass_;
  massErrUp_ = bluePrint.massErrUp_;
  massErrDown_ = bluePrint.massErrDown_;
  isValidSolution_ = bluePrint.isValidSolution_;
  return (*this);
}

