#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"

NSVfitResonanceHypothesis::NSVfitResonanceHypothesis(const NSVfitResonanceHypothesis& bluePrint)
  : eventHyp_(bluePrint.eventHyp_),
    name_(bluePrint.name_),
    barcode_(bluePrint.barcode_),
    p4_(bluePrint.p4_),
    dp4_(bluePrint.dp4_),
    daughters_(bluePrint.daughters_),
    mass_(bluePrint.mass_),
    massErrUp_(bluePrint.massErrUp_),
    massErrDown_(bluePrint.massErrDown_),
    massMean_(bluePrint.massMean_),
    massMedian_(bluePrint.massMedian_),
    massMaximum_(bluePrint.massMaximum_),
    massMaxInterpol_(bluePrint.massMaxInterpol_)
{
  for (unsigned int i = 0; i < daughters_.size(); i++) {
    daughters_[i].setMother(this);
  }
}

NSVfitResonanceHypothesis& NSVfitResonanceHypothesis::operator =(const NSVfitResonanceHypothesis& bluePrint)
{
  eventHyp_ = bluePrint.eventHyp_;
  name_ = bluePrint.name_;
  barcode_ = bluePrint.barcode_;
  p4_ = bluePrint.p4_;
  dp4_ = bluePrint.dp4_;
  daughters_ = bluePrint.daughters_;
  for (unsigned int i = 0; i < daughters_.size(); i++) {
    daughters_[i].setMother(this);
  }
  mass_ = bluePrint.mass_;
  massErrUp_ = bluePrint.massErrUp_;
  massErrDown_ = bluePrint.massErrDown_;
  massMean_ = bluePrint.massMean_;
  massMedian_ = bluePrint.massMedian_;
  massMaximum_ = bluePrint.massMaximum_;
  massMaxInterpol_ = bluePrint.massMaxInterpol_;
  return (*this);
}

//
//-------------------------------------------------------------------------------
//

bool operator<(const NSVfitResonanceHypothesis& hypothesis1, const NSVfitResonanceHypothesis& hypothesis2)
{
  return (hypothesis1.mass() < hypothesis2.mass());
}
