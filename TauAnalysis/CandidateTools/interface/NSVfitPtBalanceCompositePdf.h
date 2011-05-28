#ifndef TauAnalysis_CandidateTools_NSVfitPtBalanceCompositePdf_h
#define TauAnalysis_CandidateTools_NSVfitPtBalanceCompositePdf_h

/*
 * NSVfitPtBalanceCompositePdf
 *
 * Author: Evan Friis (UC Davis)
 *
 * Interface to a set of RooAbsPdf objects stored in a set of RooWorkspaces.
 * Each RooWorkspace corresponds to a slice in phi, the opening angle between
 * the visible decay product legs.
 */

#include <vector>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "TauAnalysis/CandidateTools/interface/NSVfitCachingPdfWrapper.h"

namespace edm {
  class ParameterSet;
}

class RooAbsPdf;
class RooRealVar;
class RooBinning;
class RooArgSet;
class TH2F;

class NSVfitPtBalanceCompositePdf {
  public:

    NSVfitPtBalanceCompositePdf(const edm::ParameterSet&);
    ~NSVfitPtBalanceCompositePdf();

    double operator()(double scaledPt, double mass, double deltaPhi) const;

  private:

    /*
    struct PdfInfo {
      RooAbsPdf* pdf_;
      RooRealVar* xVar_;
      RooRealVar* mass_;
      boost::shared_ptr<RooArgSet> normalizationVariable_;
      double getVal(double scaledPt, double mass) const;
      void print(std::ostream& stream) const;
    };
    */

    RooBinning* phiBinning_;
    std::vector<NSVfitCachingPdfWrapper> pdfs_;

    int verbosity_;
};


#endif /* end of include guard: TauAnalysis_CandidateTools_NSVfitPtBalanceCompositePdf_h */
