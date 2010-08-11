#ifndef TauAnalysis_DQMTools_DQMHistAdder_h
#define TauAnalysis_DQMTools_DQMHistAdder_h

/** \class DQMHistAdder
 *  
 *  Class to add DQM monitoring elements stored in separate directories in DQMStore
 *  (in order to e.g. compute the total Standard Model expectation)
 *
 *  $Date: 2010/01/28 16:57:27 $
 *  $Revision: 1.3 $
 *  \author Christian Veelken, UC Davis
 */

// framework & common header files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/DQMDefinitions.h"

#include <vector>
#include <string>

class DQMHistAdder : public edm::EDAnalyzer
{
  typedef std::vector<std::string> vstring;

  struct cfgEntryAddJob
  {
    cfgEntryAddJob(const edm::ParameterSet&);

    void print() const;

    vstring meNames_input_;
    vstring meNames_inputErr_;
    std::string meName_output_;
    std::string meName_outputErr_;

    vstring dqmDirectories_input_;
    std::string dqmDirectory_output_;

    int cfgError_;
  };

 public:
  explicit DQMHistAdder(const edm::ParameterSet&);
  virtual ~DQMHistAdder();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

 private:
  std::vector<cfgEntryAddJob> addJobs_;
  int cfgError_;
};

#endif


