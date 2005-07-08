#ifndef EDM_EDPRODUCER_INCLUDED
#define EDM_EDPRODUCER_INCLUDED

/*----------------------------------------------------------------------
  
EDProducer: The base class of all "modules" that will insert new
EDProducts into an Event.

$Id$

----------------------------------------------------------------------*/

#include "FWCore/CoreFramework/interface/CoreFrameworkfwd.h"

namespace edm
{
  class EDProducer
  {
  public:
    typedef EDProducer ModuleType;

    virtual ~EDProducer();
    virtual void produce(Event& e, EventSetup const& c) = 0;
    virtual void beginJob( EventSetup const& ) ;
    virtual void endJob() ;
    
  };
}

#endif // EDM_EDPRODUCER_INCLUDED
