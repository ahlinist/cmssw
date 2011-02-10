#ifndef TauAnalysis_DQMTools_DQMEventCounter_h
#define TauAnalysis_DQMTools_DQMEventCounter_h

/** \class DQMEventCounter
 *  
 *  Class to count number of events processed
 *
 *  $Date: 2010/08/11 10:35:49 $
 *  $Revision: 1.4 $
 *  \author Christian Veelken, UC Davis
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include <string>

class DQMEventCounter : public edm::EDAnalyzer
{
 public:
  explicit DQMEventCounter(const edm::ParameterSet&);
  virtual ~DQMEventCounter();
  virtual void beginJob(); 
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

 private:
  std::string moduleLabel_;

  std::string dqmDirectory_;

  std::string meName_;

  MonitorElement* me_;
};

#endif


