#ifndef TauAnalysis_Skimming_EventCountAnalyzer_h
#define TauAnalysis_Skimming_EventCountAnalyzer_h

/** \class EventCountAnalyzer
 *
 * Utility class to count the number of events 
 * contained in an EDM root file
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: EventCountAnalyzer.h,v 1.1 2009/01/22 16:30:02 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class EventCountAnalyzer : public edm::EDAnalyzer 
{
 public:
  // constructor 
  explicit EventCountAnalyzer(const edm::ParameterSet&);
    
  // destructor
  virtual ~EventCountAnalyzer();
    
 private:
  void beginJob(const edm::EventSetup&);
  void analyze(const edm::Event&, const edm::EventSetup&);
  void endJob();
  
  long numEventsProcessed_;
};

#endif   

