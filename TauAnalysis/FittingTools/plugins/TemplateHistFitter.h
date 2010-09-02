#ifndef TauAnalysis_FittingTools_TemplateHistFitter_h  
#define TauAnalysis_FittingTools_TemplateHistFitter_h

/** \class TemplateHistFitter
 *
 * Estimate contribution of signal and background processes to final event sample 
 * by fitting shape "templates" for different processes to distribution observed in data
 * (class implements "template" method for data-driven background estimation)
 *
 * NOTE: The TemplateHistFitter class is capable of fitting distributions of multiple different observables simultaneously,
 *       determining the contribution of signal and background processes as the normalization parameters of the templates
 *       that best fit the combination of all observed distributions.
 *
 *       Two different algorithms are supported to perform the actual fitting:
 *      o RooFit
 *      o TFractionFitter
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: TemplateHistFitter.h,v 1.2 2010/06/11 08:58:09 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/FittingTools/interface/TemplateFitAdapterBase.h"
#include "TauAnalysis/FittingTools/interface/RunningMean.h"
#include "TauAnalysis/FittingTools/interface/RunningMedian.h"
#include "TauAnalysis/FittingTools/interface/RunningCovMatrix.h"

#include <TVectorD.h>
#include <TMatrixD.h>

#include <vector>
#include <string>
#include <map>

class TemplateHistFitter : public edm::EDAnalyzer
{
 public:
  
  explicit TemplateHistFitter(const edm::ParameterSet&);
  ~TemplateHistFitter();
  
 private:

  void beginJob() {}
  void analyze(const edm::Event&, const edm::EventSetup&) {}
  void endJob();

//--- auxiliary functions
  void saveFitResults();

  void estimateUncertainties(bool, bool, int, double, const char*, int, bool);
  void saveEstUncertainties(const std::string&, const RunningMean&, const RunningMedian&, const RunningCovMatrix&);

  typedef std::vector<std::string> vstring;

//--- configuration parameters
//    and other data-members
  vstring processNames_;
  vstring varNames_;

  int printLevel_;
  bool printWarnings_;

  int statErrNumSamplings_;
  double statErrChi2redMax_;
  int statErrPrintLevel_;
  bool statErrPrintWarnings_;

  int sysErrNumSamplings_;
  double sysErrChi2redMax_;
  int sysErrPrintLevel_;
  bool sysErrPrintWarnings_;

  TemplateFitAdapterBase* fitAlgorithm_;
  const TemplateFitAdapterBase::fitResultType* fitResult_;

  std::string dqmDirectory_fitResult_;
  std::string controlPlotsFileName_;
};

#endif  


