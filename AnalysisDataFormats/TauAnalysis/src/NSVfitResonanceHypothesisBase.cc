#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisBase.h"

NSVfitResonanceHypothesisBase::NSVfitResonanceHypothesisBase(const NSVfitResonanceHypothesisBase& bluePrint)
  : name_(bluePrint.name_),
    barcode_(bluePrint.barcode_),
    daughters_(bluePrint.daughters_),
    mass_(bluePrint.mass_),
    massErrUp_(bluePrint.massErrUp_),
    massErrDown_(bluePrint.massErrDown_),
    isValidSolution_(bluePrint.isValidSolution_)
{}

NSVfitResonanceHypothesisBase& NSVfitResonanceHypothesisBase::operator=(const NSVfitResonanceHypothesisBase& bluePrint)
{  
  name_ = bluePrint.name_;
  barcode_ = bluePrint.barcode_;
  daughters_ = bluePrint.daughters_;
  mass_ = bluePrint.mass_;
  massErrUp_ = bluePrint.massErrUp_;
  massErrDown_ = bluePrint.massErrDown_;
  isValidSolution_ = bluePrint.isValidSolution_;
  return (*this);
}

//
//-------------------------------------------------------------------------------
//

bool operator<(const NSVfitResonanceHypothesisBase& hypothesis1, const NSVfitResonanceHypothesisBase& hypothesis2)
{
  return (hypothesis1.barcode() < hypothesis2.barcode());
}
