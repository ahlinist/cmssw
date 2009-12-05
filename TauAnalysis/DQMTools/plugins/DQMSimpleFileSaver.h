#ifndef TauAnalysis_DQMTools_DQMSimpleFileSaver_h
#define TauAnalysis_DQMTools_DQMSimpleFileSaver_h

/** \class DQMSimpleFileSaver
 *  
 *  Class to write all monitor elements registered in DQMStore into ROOT file
 *  (without any naming restrictions imposed by "regular" DQMFileSaver)
 *
 *  $Date: 2009/08/28 13:44:38 $
 *  $Revision: 1.2 $
 *  \author Christian Veelken, UC Davis
 */

// framework & common header files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

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
  std::vector<outputCommandEntry>* outputCommands_;

  std::string outputFileName_;

  int cfgError_;
};

#endif


