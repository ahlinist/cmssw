#ifndef TauAnalysis_FittingTools_SmoothLandau_x_GaussPdf_h
#define TauAnalysis_FittingTools_SmoothLandau_x_GaussPdf_h

/** \class SmoothLandau_x_GaussPdf
 *
 * Numerical implementation of Landau density convoluted with Gaussian
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: SmoothLandau_x_GaussPdf.h,v 1.1 2010/05/26 15:00:26 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class SmoothLandau_x_GaussPdf : public RooAbsPdf 
{
 public:
  // constructor(s)
  SmoothLandau_x_GaussPdf();   
  SmoothLandau_x_GaussPdf(const char*, const char*, RooAbsReal&, RooAbsReal&, RooAbsReal&, RooAbsReal&, RooAbsReal&);
  SmoothLandau_x_GaussPdf(const SmoothLandau_x_GaussPdf&, const char* = "");
  
  // destructor
  virtual ~SmoothLandau_x_GaussPdf();

  virtual TObject* clone(const char* newName) const { return new SmoothLandau_x_GaussPdf(*this, newName); }

 protected:
  Double_t evaluate() const;

  RooRealProxy x_;

  RooRealProxy width_;
  RooRealProxy mp_;
  RooRealProxy area_;
  RooRealProxy gsigma_;
};

#endif  

