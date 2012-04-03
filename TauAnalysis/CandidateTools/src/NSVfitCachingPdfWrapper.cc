#include "TauAnalysis/CandidateTools/interface/NSVfitCachingPdfWrapper.h"
#include <algorithm>
#include "TH2F.h"
#include "TMath.h"
#include "RooAbsPdf.h"
#include "RooRealVar.h"
#include "RooArgSet.h"
#include "RooMsgService.h"

NSVfitCachingPdfWrapper::NSVfitCachingPdfWrapper(RooAbsPdf* pdf,
    RooRealVar* x, RooRealVar* y,
    size_t nXBins, double xLow, double xHigh,
    size_t nYBins, double yLow, double yHigh) {
  assert(pdf);
  assert(x);
  assert(y);
  pdf_ = pdf;
  x_ = x;
  y_ = y;
  isValid_ = true;
  normalizationVariable_.reset(new RooArgSet(*x_, "toIntegrate"));
  initializeCache(nXBins, xLow, xHigh, nYBins, yLow, yHigh);
  RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL) ;
}

NSVfitCachingPdfWrapper::NSVfitCachingPdfWrapper(RooAbsPdf* pdf,
    RooRealVar* x, RooRealVar* y, size_t nXBins, size_t nYBins) {
  assert(pdf);
  assert(x);
  assert(y);
  pdf_ = pdf;
  x_ = x;
  y_ = y;
  isValid_ = true;
  normalizationVariable_.reset(new RooArgSet(*x_, "toIntegrate"));
  initializeCache(nXBins, x_->getMin(), x_->getMax(),
      nYBins, y_->getMin(), y_->getMax());
}

void NSVfitCachingPdfWrapper::initializeCache(
    size_t nXBins, double xLow, double xHigh,
    size_t nYBins, double yLow, double yHigh) {
  //std::cout << "<NSVfitCachingPdfWrapper::initializeCache>:" << std::endl;
  //std::cout << " nXBins = " << nXBins << std::endl;
  //std::cout << " nYBins = " << nYBins << std::endl;

  cache_.reset(new TH2F("cache", "cache",
        nXBins, xLow, xHigh, nYBins, yLow, yHigh));

  double xBinWidth = TMath::Abs((xHigh - xLow)/nXBins);
  xLow_ = cache_->GetXaxis()->GetXmin() + 0.5*xBinWidth;
  xHigh_ = cache_->GetXaxis()->GetXmax() - 0.5*xBinWidth;

  double yBinWidth = TMath::Abs((yHigh - yLow)/nYBins);
  yLow_ = cache_->GetYaxis()->GetXmin() + 0.5*yBinWidth;
  yHigh_ = cache_->GetYaxis()->GetXmax() - 0.5*yBinWidth;

  for (int xBin = 1; xBin < cache_->GetNbinsX()+1; ++xBin) {
    //std::cout << "processing xBin = " << xBin << std::endl;
    for (int yBin = 1; yBin < cache_->GetNbinsY()+1; ++yBin) {
      double xVal = cache_->GetXaxis()->GetBinCenter(xBin);
      double yVal = cache_->GetYaxis()->GetBinCenter(yBin);
      x_->setVal(xVal);
      y_->setVal(yVal);
      double pdfVal = pdf_->getVal(normalizationVariable_.get());
      cache_->SetBinContent(xBin, yBin, pdfVal);
    }
  }

  //std::cout << "done." << std::endl;
}

double NSVfitCachingPdfWrapper::getVal(double x, double y) const {
  // Dont' let nans get to the Interpolate function, it will crash.
  if (isnan(x)) {
    return x;
  }
  if (isnan(y))
    return y;

  // Don't actually go past limits
  x = std::max(x, xLow_);
  x = std::min(x, xHigh_);
  y = std::max(y, yLow_);
  y = std::min(y, yHigh_);

  double retVal = cache_->Interpolate(x, y);

  return retVal;
}
