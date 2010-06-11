#ifndef TauAnalysis_FittingTools_SmoothGenericPdfWrapper_h
#define TauAnalysis_FittingTools_SmoothGenericPdfWrapper_h

/** \class SmoothGenericPdfWrapper
 *
 * Wrapper class needed in order to use RooGenericPdf as a plugin
 * (implementing arbitrary analytic probability density functions that can be defined via strings)
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: SmoothGenericPdfWrapper.h,v 1.2 2010/05/26 15:26:07 veelken Exp $
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

  edm::ParameterSet cfgParameter_;
};

#endif  

