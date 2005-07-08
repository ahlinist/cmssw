/*----------------------------------------------------------------------
  
$Id$

----------------------------------------------------------------------*/

#include "FWCore/CoreFramework/interface/EDProducer.h"

namespace edm
{
  EDProducer::~EDProducer()
  { }

  void EDProducer::beginJob( EventSetup const& ) 
  { }

  void EDProducer::endJob()
  { }
}
  
