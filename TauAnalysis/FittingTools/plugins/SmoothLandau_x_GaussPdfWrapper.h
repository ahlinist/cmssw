#ifndef TauAnalysis_FittingTools_SmoothLandau_x_GaussPdfWrapper_h
#define TauAnalysis_FittingTools_SmoothLandau_x_GaussPdfWrapper_h

/** \class SmoothLandau_x_GaussPdfWrapper
 *
 * Wrapper class needed in order to use SmoothLandau_x_GaussPdf as a plugin
 * (implementing numerically the convolution of a Landau with a Gaussian probability density function)
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: SmoothLandau_x_GaussPdfWrapper.h,v 1.1 2010/05/26 15:00:27 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/FittingTools/interface/SmoothPdfWrapperBase.h"
#include "TauAnalysis/FittingTools/interface/SmoothLandau_x_GaussPdf.h"

class SmoothLandau_x_GaussPdfWrapper : public SmoothPdfWrapperBase
{
 public:
  // constructor 
  explicit SmoothLandau_x_GaussPdfWrapper(const edm::ParameterSet&);   
  
  // destructor
  ~SmoothLandau_x_GaussPdfWrapper();
  
  void initialize();

 private:
  edm::ParameterSet cfgParameter_;

  RooRealVar* width_;
  RooRealVar* mp_;
  RooRealVar* area_;
  RooRealVar* gsigma_;
};

#endif  

