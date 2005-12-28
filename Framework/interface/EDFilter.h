#ifndef Framework_EDFilter_h
#define Framework_EDFilter_h

/*----------------------------------------------------------------------
  
EDFilter: The base class of all "modules" used to control the flow of
processing in a processing path.

$Id$

----------------------------------------------------------------------*/

namespace edm {
  class Event;
  class EventSetup;
  class ParameterSet;
  class EDFilter {
    public:
      typedef EDFilter ModuleType;

      virtual ~EDFilter();
      virtual bool filter(Event const& e, EventSetup const& c) = 0;
      virtual void beginJob(EventSetup const&) ;
      virtual void endJob() ;
  };
}

#endif
