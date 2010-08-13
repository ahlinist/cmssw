#ifndef TauAnalysis_DQMTools_DQMHistErrorBandProducer_h
#define TauAnalysis_DQMTools_DQMHistErrorBandProducer_h

/** \class DQMHistErrorBandProducer
 *  
 *  Class to compute error bands (representing effects of systematic uncertainties) displayed on plots
 *  by summing bin-by-bin variations of histograms filled after shifting up/down and smearing
 *  the quantities filled into each histogram
 *
 *  $Date: 2010/02/04 16:11:38 $
 *  $Revision: 1.2 $
 *  \author Christian Veelken, UC Davis
 */

// framework & common header files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/DQMTools/interface/DQMErrorBandProducerBase.h"

#include <vector>
#include <string>

class DQMHistErrorBandProducer : public DQMErrorBandProducerBase
{
 public:
  explicit DQMHistErrorBandProducer(const edm::ParameterSet&);
  virtual ~DQMHistErrorBandProducer();

 private:
  void computeErrorBandImp(DQMStore&, const std::string&, const std::vector<std::string>&, 
			   const std::string&, const std::string&, int);
};

#endif


