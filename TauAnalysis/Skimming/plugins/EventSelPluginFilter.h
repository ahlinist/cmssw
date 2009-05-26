#ifndef TauAnalysis_Skimming_EventSelPluginFilter_h
#define TauAnalysis_Skimming_EventSelPluginFilter_h

/** \class EventSelPluginFilter
 *
 * Provide classes derrived from EventSelectorBase with an EDFilter interface 
 * (similar to PhysicsTools/UtilAlgos/interface/EventSelectorAdapter.h
 *  except that event selector derived from EventSelectorBase 
 *  is created dynamically at run-time via plug-in mechanism
 *  instead of being a templated data-member)
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: EventSelPluginFilter.h,v 1.1 2009/01/22 14:02:08 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "PhysicsTools/UtilAlgos/interface/EventSelectorBase.h"

class EventSelPluginFilter : public edm::EDFilter 
{
 public:
  // constructor 
  explicit EventSelPluginFilter(const edm::ParameterSet&);
    
  // destructor
  virtual ~EventSelPluginFilter();
    
 private:
  bool filter(edm::Event&, const edm::EventSetup&);
  
  EventSelectorBase* eventSelector_;
};

#endif
