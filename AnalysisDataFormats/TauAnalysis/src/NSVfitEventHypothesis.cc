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
{}

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
  histMassResults_ = std::auto_ptr<TH1>((TH1*)bluePrint.histMassResults_->Clone());
  return (*this);
}
