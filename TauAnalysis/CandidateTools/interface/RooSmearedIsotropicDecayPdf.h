#ifndef TauAnalysis_CandidateTools_RooSmearedIsotropicDecayPdf_h
#define TauAnalysis_CandidateTools_RooSmearedIsotropicDecayPdf_h

#include "RooAbsPdf.h"
#include "RooAbsReal.h"
#include "RooRealProxy.h"

class RooSmearedIsotropicDecayPdf : public RooAbsPdf {
  public:
    RooSmearedIsotropicDecayPdf(const char *name, const char *title,
        RooAbsReal& _x, RooAbsReal& _smear, RooAbsReal& _location);

    RooSmearedIsotropicDecayPdf(const RooSmearedIsotropicDecayPdf& other,
        const char *name=0);
    virtual TObject* clone(const char* newname) const {
      return new RooSmearedIsotropicDecayPdf(*this, newname);
    };
    inline virtual ~RooSmearedIsotropicDecayPdf() {}

    /// Give the PDF value at the specified points
    Double_t evaluate() const;

    Int_t getAnalyticalIntegral(
        RooArgSet& allVars, RooArgSet& analVars, const char *rangeName) const;

    Double_t analyticalIntegral(Int_t code, const char *rangeName) const;

  private:
    RooRealProxy x;
    RooRealProxy smear;
    RooRealProxy location;
};

#endif /* end of include guard: TauAnalysis_CandidateTools_RooSmearedIsotropicDecayPdf_h */
