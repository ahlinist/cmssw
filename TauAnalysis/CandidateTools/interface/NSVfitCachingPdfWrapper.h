#ifndef TauAnalysis_CandidateTools_NSVfitCachingPdfWrapper_h
#define TauAnalysis_CandidateTools_NSVfitCachingPdfWrapper_h

/*
 * A caching PDF which wraps a 2D RooAbsPdf.  The PDF is assumed to be
 * normalized over the x variable.
 *
 * Author: Evan K. Friis, Christian Veelken, UC Davis
 *
 */

#include <boost/shared_ptr.hpp>

// Forward declarations
class RooAbsPdf;
class RooRealVar;
class RooArgSet;
class TH2;

class NSVfitCachingPdfWrapper {
  public:
    NSVfitCachingPdfWrapper() { isValid_ = false; }

    // Default constructor
    NSVfitCachingPdfWrapper(RooAbsPdf* pdf,
        RooRealVar* x, RooRealVar* y,
        size_t nXBins, double xLow, double xHigh,
        size_t nYBins, double yLow, double yHigh);

    // Constructor where the x and y ranges are taken from the RooRealVars
    NSVfitCachingPdfWrapper(RooAbsPdf* pdf,
        RooRealVar* x, RooRealVar* y, size_t nXBins, size_t nYBins);

    double getVal(double x, double y) const;

    bool isValid() const { return isValid_; }

  private:

    void initializeCache(size_t nXBins, double xLow, double xHigh,
        size_t nYBins, double yLow, double yHigh);

    RooAbsPdf* pdf_;
    RooRealVar* x_;
    RooRealVar* y_;
    bool isValid_;

    double xLow_;
    double xHigh_;
    double yLow_;
    double yHigh_;

    boost::shared_ptr<TH2> cache_;
    boost::shared_ptr<RooArgSet> normalizationVariable_;
};

#endif /* end of include guard: TauAnalysis_CandidateTools_NSVfitCachingPdfWrapper_h */
