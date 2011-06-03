#ifndef TauAnalysis_CandidateTools_RooAbsEstimatablePdf_h
#define TauAnalysis_CandidateTools_RooAbsEstimatablePdf_h

/*
 * RooAbsEstimatablePdf
 *
 * Abstract base class that provides an interface that allows the parameters of
 * a PDF to be estimated from an input RooAbsData dataset.
 *
 * Author: Evan K. Friis, UC Davis
 *
 */

#include "RooAbsData.h"
#include "RooAbsReal.h"
#include "RooArgSet.h"
#include "RooRealProxy.h"
#include "RooRealVar.h"

class RooAbsEstimatablePdf {
  public:
    RooAbsEstimatablePdf(){}
    virtual ~RooAbsEstimatablePdf(){}

    // Estimate all parameters of the distribution except [observable]
    // using the supplied data.  The updated parameters are returned in a
    // RooAggSet

    virtual RooArgSet estimateParameters(
        RooAbsData& data, double errorFactor=1.0) = 0;

  protected:
    // Convert a RooRealProxy to a real var, if possible.  Will return an null
    // pointer if not possible.
    RooRealVar* convertProxy(const RooRealProxy& proxy) const;

  private:
    ClassDef(RooAbsEstimatablePdf, 1);
};

#endif /* end of include guard: TauAnalysis_CandidateTools_RooAbsEstimatablePdf_h */
