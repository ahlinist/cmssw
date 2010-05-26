#ifndef TauAnalysis_FittingTools_SmoothGenericPdfWrapper_h
#define TauAnalysis_FittingTools_SmoothGenericPdfWrapper_h

/** \class SmoothGenericPdfWrapper
 *
 * Wrapper class needed in order to use RooGenericPdf as a plugin
 * (implementing arbitrary analytic probability density functions that can be defined via strings)
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: SmoothGenericPdfWrapper.h,v 1.1 2009/11/27 15:46:33 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/FittingTools/interface/SmoothPdfWrapperBase.h"

#include <string>
#include <map>

class SmoothGenericPdfWrapper : public SmoothPdfWrapperBase
{
 public:
  // constructor 
  explicit SmoothGenericPdfWrapper(const edm::ParameterSet& cfg);
  
  // destructor
  virtual ~SmoothGenericPdfWrapper();

  void initialize();

 private:
  std::string formula_;

  std::map<std::string, RooRealVar*> parameter_;
  std::map<std::string, double> parameter0_;
};

#endif  

