#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h"

NSVfitEventHypothesis::NSVfitEventHypothesis(const NSVfitEventHypothesis& bluePrint)
  : name_(bluePrint.name_),
    barcode_(bluePrint.barcode_),
    met_(bluePrint.met_),
    p4_(bluePrint.p4_),
    dp4_(bluePrint.dp4_),
    p4MEt_(bluePrint.p4MEt_),
    eventVertexPosition_(bluePrint.eventVertexPosition_),
    eventVertexPositionErr_(bluePrint.eventVertexPositionErr_),
    eventVertexPositionShift_(bluePrint.eventVertexPositionShift_),
    eventVertexIsValid_(bluePrint.eventVertexIsValid_),
    resonances_(bluePrint.resonances_),
    histMassResults_((TH1*)bluePrint.histMassResults_->Clone())
{
  for (unsigned int i = 0; i < resonances_.size(); i++) {
    resonances_[i].setEventHypothesis(this);
  }
}

NSVfitEventHypothesis& NSVfitEventHypothesis::operator =(const NSVfitEventHypothesis& bluePrint)
{
  name_ = bluePrint.name_;
  barcode_ = bluePrint.barcode_;
  met_ = bluePrint.met_;
  p4_ = bluePrint.p4_;
  dp4_ = bluePrint.dp4_;
  p4MEt_ = bluePrint.p4MEt_;
  eventVertexPosition_ = bluePrint.eventVertexPosition_;
  eventVertexPositionErr_ = bluePrint.eventVertexPositionErr_;
  eventVertexPositionShift_ = bluePrint.eventVertexPositionShift_;
  eventVertexIsValid_ = bluePrint.eventVertexIsValid_;
  resonances_ = bluePrint.resonances_;
  for (unsigned int i = 0; i < resonances_.size(); i++) {
    resonances_[i].setEventHypothesis(this);
  }
  histMassResults_ = std::auto_ptr<TH1>((TH1*)bluePrint.histMassResults_->Clone());
  return (*this);
}
