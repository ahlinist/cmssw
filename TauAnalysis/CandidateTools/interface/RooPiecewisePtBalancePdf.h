#ifndef TauAnalysis_CandidateTools_RooPiecewisePtBalancePdf_h
#define TauAnalysis_CandidateTools_RooPiecewisePtBalancePdf_h

#include <memory>
#include "RooAbsPdf.h"
#include "RooRealVar.h"
#include "RooRealProxy.h"
#include "RooSkewNormal.h"
#include "RooFunctor.h"
#include "RooBifurGauss.h"
#include "RooLandau.h"
#include "TauAnalysis/CandidateTools/interface/RooAbsEstimatablePdf.h"
#include "TF1.h"

class RooPiecewisePtBalancePdf : public RooAbsPdf, public RooAbsEstimatablePdf {
  public:
    RooPiecewisePtBalancePdf():RooAbsPdf(){}
    RooPiecewisePtBalancePdf(const char *name, const char *title,
        RooAbsReal& _x,
        // Skew normal turn on
        RooAbsReal& _sn_location, RooAbsReal& _sn_scale, RooAbsReal& _sn_skew,
        // End of skew normal
        RooAbsReal& _x0,
        // Gaussian cap
        RooAbsReal& _cap_location, RooAbsReal& _cap_sigmaL, RooAbsReal& _cap_sigmaR,
        // Total width of Gaussian cap piece
        RooAbsReal& _dx1,
        // Landau tail
        RooAbsReal& _tail_mp, RooAbsReal& _tail_width);

    RooPiecewisePtBalancePdf(const RooPiecewisePtBalancePdf& other, const char *name=0);

    virtual TObject* clone(const char* newname) const {
      return new RooPiecewisePtBalancePdf(*this, newname);
    }
    virtual ~RooPiecewisePtBalancePdf();

    /// Get value of PDF at current parameters (non--normalized)
    Double_t evaluate() const;
    Double_t evaluateCap(double xx) const;
    Double_t evaluateTail(double xx) const;
    Double_t evaluateTurnon(double xx) const;

    /// Identify supported analytical integrals
    Int_t getAnalyticalIntegral(
        RooArgSet& allVars, RooArgSet& analVars, const char *rangeName) const;

    /// Compute analytical integrals
    Double_t analyticalIntegral(Int_t code, const char *rangeName) const;

    /// Estimate parameters
    RooArgSet estimateParameters(RooAbsData& data, double errorFactor);

  private:
    Double_t getTurnonScaleFactor() const;
    Double_t getTailScaleFactor() const;
    Double_t capToTailTransitionPoint() const;

    Double_t turnonCDF(double xx) const;
    Double_t capCDF(double xx) const;
    Double_t tailCDF(double xx) const;

    RooRealProxy x;
    RooRealProxy sn_location;
    RooRealProxy sn_scale;
    RooRealProxy sn_skew;
    RooRealProxy x0;
    RooRealProxy cap_location;
    RooRealProxy cap_sigmaL;
    RooRealProxy cap_sigmaR;
    RooRealProxy dx1;
    RooRealProxy tail_mp;
    RooRealProxy tail_width;
    // PDFs
    /*
    RooSkewNormal turnonPDF;
    RooBifurGauss capPDF;
    RooLandau tailPDF;

    // Normalization variable set
    RooArgSet nset;

    mutable TF1* turnonPDF_func;
    mutable TF1* capPDF_func;
    mutable TF1* tailPDF_func;
    */

    // generate CInt dictionaries
    ClassDef(RooPiecewisePtBalancePdf,1);
};

#endif /* end of include guard: TauAnalysis_CandidateTools_RooPiecewisePtBalancePdf_h */
