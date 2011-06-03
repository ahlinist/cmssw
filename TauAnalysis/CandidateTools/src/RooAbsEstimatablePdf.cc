#include "TauAnalysis/CandidateTools/interface/RooAbsEstimatablePdf.h"

RooRealVar*
RooAbsEstimatablePdf::convertProxy(const RooRealProxy& proxy) const {
  RooAbsArg* arg = proxy.absArg();
  RooRealVar* var = dynamic_cast<RooRealVar*>(arg);
  if (!var) {
    std::cerr << "WARN: convertProxy could not convert "
      << proxy.arg().GetName() << " into a RooRealVar" << std::endl;
  }
  return var;
}

ClassImp(RooAbsEstimatablePdf)
