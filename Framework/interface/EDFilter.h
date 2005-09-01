#ifndef Framework_EDFilter_h
#define Framework_EDFilter_h

/*----------------------------------------------------------------------
  
EDFilter: The base class of all "modules" used to control the flow of
processing in a processing path.

$Id$

----------------------------------------------------------------------*/
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/Framework/interface/EventSetup.h"

namespace edm
  {
  class EDFilter
    {
    public:
      typedef EDFilter ModuleType;

      virtual ~EDFilter();
      virtual bool filter(Event const& e, EventSetup const& c) = 0;
      virtual void beginJob(EventSetup const&) ;
      virtual void endJob() ;
      
      
    };
}

#endif
