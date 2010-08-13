#ifndef TauAnalysis_DQMTools_DQMErrorBandProducerBase_h
#define TauAnalysis_DQMTools_DQMErrorBandProducerBase_h

/** \class DQMErrorBandProducerBase
 *  
 *  Base-class to compute variations of histograms, binning results,...
 *  (representing e.g. effects of systematic uncertainties) 
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

#include "DQMServices/Core/interface/DQMStore.h"

#include <vector>
#include <string>

class DQMErrorBandProducerBase : public edm::EDAnalyzer
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
  explicit DQMErrorBandProducerBase(const edm::ParameterSet&);
  virtual ~DQMErrorBandProducerBase();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

 protected:
  virtual void computeErrorBand(DQMStore&, const std::string&, const std::vector<std::string>&, 
				const std::string&, const std::string&, const std::string&, int);
  virtual void computeErrorBandImp(DQMStore&, const std::string&, const std::vector<std::string>&, 
				   const std::string&, const std::string&, int) = 0;

  std::vector<cfgEntryProduceJob> produceJobs_;

  static const std::string method_sqrt;
  static const std::string method_min_max;
  static const std::string defaultMethod;

  int verbosity_;

  int cfgError_;
};

#endif


