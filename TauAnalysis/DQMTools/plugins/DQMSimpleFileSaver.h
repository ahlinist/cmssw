#ifndef TauAnalysis_DQMTools_DQMSimpleFileSaver_h
#define TauAnalysis_DQMTools_DQMSimpleFileSaver_h

/** \class DQMSimpleFileSaver
 *  
 *  Class to write all monitor elements registered in DQMStore into ROOT file
 *  (without any naming restrictions imposed by "regular" DQMFileSaver)
 *
 *  $Date: 2009/01/21 17:34:57 $
 *  $Revision: 1.1 $
 *  \author Christian Veelken, UC Davis
 */

// framework & common header files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <string>
#include <vector>

class DQMSimpleFileSaver : public edm::EDAnalyzer
{
 public:
  explicit DQMSimpleFileSaver(const edm::ParameterSet&);
  virtual ~DQMSimpleFileSaver();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

private:
  typedef std::vector<std::string> vstring;
  vstring dqmDirectories_drop_;

  std::string outputFileName_;

  int cfgError_;
};

#endif


