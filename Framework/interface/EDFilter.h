#ifndef Framework_EDFilter_h
#define Framework_EDFilter_h

/*----------------------------------------------------------------------
  
EDFilter: The base class of all "modules" used to control the flow of
processing in a processing path.
Filters can also insert products into the event.
These products should be informational products about the filter decision.

$Id$

----------------------------------------------------------------------*/

#include "FWCore/Framework/interface/ProducerBase.h"

namespace edm {
  class Event;
  class EventSetup;
  class EDFilter : public ProducerBase {
    public:
      typedef EDFilter ModuleType;

      EDFilter() : ProducerBase() {}
      virtual ~EDFilter();
      virtual bool filter(Event& e, EventSetup const& c) = 0;
      virtual void beginJob(EventSetup const&) ;
      virtual void endJob() ;
  };
}

#endif
