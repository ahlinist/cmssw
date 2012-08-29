#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisBase.h"

NSVfitEventHypothesisBase::NSVfitEventHypothesisBase(const NSVfitEventHypothesisBase& bluePrint)
  : name_(bluePrint.name_),
    barcode_(bluePrint.barcode_),
    nll_(bluePrint.nll_),
    met_(bluePrint.met_),
    eventVertexPos_(bluePrint.eventVertexPos_),
    eventVertexCov_(bluePrint.eventVertexCov_),
    eventVertexShift_(bluePrint.eventVertexShift_),
    eventVertexIsValid_(bluePrint.eventVertexIsValid_),
    resonances_(bluePrint.resonances_),
    mass_(bluePrint.mass_),
    massErrUp_(bluePrint.massErrUp_),
    massErrDown_(bluePrint.massErrDown_),
    isValidSolution_(bluePrint.isValidSolution_)
{}

NSVfitEventHypothesisBase& NSVfitEventHypothesisBase::operator=(const NSVfitEventHypothesisBase& bluePrint)
{
  name_ = bluePrint.name_;
  barcode_ = bluePrint.barcode_;
  nll_ = bluePrint.nll_;
  met_ = bluePrint.met_;
  eventVertexPos_ = bluePrint.eventVertexPos_;
  eventVertexCov_ = bluePrint.eventVertexCov_;
  eventVertexShift_ = bluePrint.eventVertexShift_;
  eventVertexIsValid_ = bluePrint.eventVertexIsValid_;
  resonances_ = bluePrint.resonances_;
  mass_ = bluePrint.mass_;
  massErrUp_ = bluePrint.massErrUp_;
  massErrDown_ = bluePrint.massErrDown_;
  isValidSolution_ = bluePrint.isValidSolution_;
  return (*this);
}

//
//-------------------------------------------------------------------------------
//

bool operator<(const NSVfitEventHypothesisBase& hypothesis1, const NSVfitEventHypothesisBase& hypothesis2)
{
  return (hypothesis1.barcode() < hypothesis2.barcode());
}

