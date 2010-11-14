#ifndef TauAnalysis_Skimming_EventCountFilter_h
#define TauAnalysis_Skimming_EventCountFilter_h

/** \class EventCountFilter
 *
 * Select total number of specified events,
 * starting from the i-th processed event
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: EventCountFilter.h,v 1.2 2010/11/12 16:59:47 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <string>
#include <map>

class EventCountFilter : public edm::EDFilter
{
 public:
  // constructor 
  explicit EventCountFilter(const edm::ParameterSet&);
    
  // destructor
  virtual ~EventCountFilter();
    
 private:
  bool filter(edm::Event&, const edm::EventSetup&);

  long numEventsToBeSelected_;
  long firstEventToBeSelected_;

  long numEventsProcessed_;
};

#endif   
