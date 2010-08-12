#ifndef TauAnalysis_DQMTools_DQMDumpMonitorElement_h
#define TauAnalysis_DQMTools_DQMDumpMonitorElement_h

/** \class DQMDumpMonitorElement
 *  
 *  Class to print-out value of DQM monitoring element
 *
 *  $Date: 2010/08/11 10:35:49 $
 *  $Revision: 1.4 $
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

class DQMDumpMonitorElement : public edm::EDAnalyzer
{
  typedef std::vector<std::string> vstring;

  struct jobEntryType
  {
    jobEntryType(const edm::ParameterSet&);

    std::string meName_;
    std::string meName_err_;

    std::string label_;

    vstring processes_;
  };

 public:
  explicit DQMDumpMonitorElement(const edm::ParameterSet&);
  virtual ~DQMDumpMonitorElement();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

 private:
  std::vector<jobEntryType> dumpJobs_;
};

#endif


