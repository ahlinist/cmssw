#ifndef TauAnalysis_CandidateTools_RooBifurGeneralizedExp_h
#define TauAnalysis_CandidateTools_RooBifurGeneralizedExp_h

#include "RooAbsPdf.h"
#include "RooRealVar.h"
#include "RooRealProxy.h"

class RooBifurGeneralizedExp : public RooAbsPdf {
  public:
    RooBifurGeneralizedExp() {}
    RooBifurGeneralizedExp(const char *name, const char *title,
        RooAbsReal& _x, RooAbsReal& _pivot,
        RooAbsReal& _leftExp, RooAbsReal& _leftGauss,
        RooAbsReal& _rightExp, RooAbsReal& _rightGauss);

    RooBifurGeneralizedExp(const RooBifurGeneralizedExp& other, const char *name=0);
    virtual TObject* clone(const char* newname) const {
      return new RooBifurGeneralizedExp(*this, newname);
    }
    virtual ~RooBifurGeneralizedExp() {}

    /// Get value of PDF at current parameters (non--normalized)
    Double_t evaluate() const;

    /// Identify supported analytical integrals
    Int_t getAnalyticalIntegral(
        RooArgSet& allVars, RooArgSet& analVars, const char *rangeName) const;

    /// Compute analytical integrals
    Double_t analyticalIntegral(Int_t code, const char *rangeName) const;

  private:
    RooRealProxy x;
    RooRealProxy pivot;
    RooRealProxy leftExp;
    RooRealProxy leftGauss;
    RooRealProxy rightExp;
    RooRealProxy rightGauss;
};

#endif /* end of include guard: TauAnalysis_CandidateTools_RooBifurGeneralizedExp_h */
