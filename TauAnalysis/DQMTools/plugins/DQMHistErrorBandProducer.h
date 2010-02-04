#ifndef TauAnalysis_DQMTools_DQMHistErrorBandProducer_h
#define TauAnalysis_DQMTools_DQMHistErrorBandProducer_h

/** \class DQMHistErrorBandProducer
 *  
 *  Class to compute error bands (representing effects of systematic uncertainties) displayed on plots
 *  by summing bin-by-bin variations of histograms filled after shifting up/down and smearing
 *  the quantities filled into each histogram
 *
 *  $Date: 2010/01/28 16:58:46 $
 *  $Revision: 1.1 $
 *  \author Christian Veelken, UC Davis
 */

// framework & common header files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>
#include <string>

class DQMHistErrorBandProducer : public edm::EDAnalyzer
{
  typedef std::vector<std::string> vstring;

  struct cfgEntryProduceJob
  {
    cfgEntryProduceJob(const edm::ParameterSet&);
    void print() const;
    std::string dqmDirectory_inputCentralValue_;
    vstring dqmDirectories_inputVariance_;
    std::string dqmDirectory_output_;
    std::string method_;
  };	

 public:
  explicit DQMHistErrorBandProducer(const edm::ParameterSet&);
  virtual ~DQMHistErrorBandProducer();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

 private:
  std::vector<cfgEntryProduceJob> produceJobs_;

  int cfgError_;
};

#endif


