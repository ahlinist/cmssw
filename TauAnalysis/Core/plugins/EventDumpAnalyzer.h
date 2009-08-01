#ifndef TauAnalysis_Core_EventDumpAnalyzer_h
#define TauAnalysis_Core_EventDumpAnalyzer_h

/** \class EventDumpAnalyzer
 *
 * Provide classes derrived from EventDumpBase with an EDAnalyzer interface 
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: EventDumpAnalyzer.h,v 1.1 2009/06/09 13:30:20 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "TauAnalysis/Core/interface/EventDumpBase.h"

#include <vector>

class EventDumpAnalyzer : public edm::EDAnalyzer
{
 public:
  // constructor 
  explicit EventDumpAnalyzer(const edm::ParameterSet&);
    
  // destructor
  virtual ~EventDumpAnalyzer();
  
 private:
  void beginJob() {}
  void analyze(const edm::Event&, const edm::EventSetup&);
  void endJob() {}

//--- names of boolean flags to be evaluated
  typedef std::vector<edm::InputTag> vInputTag;
  vInputTag evtSelFlags_;

  EventDumpBase* plugin_;
};

#endif   
