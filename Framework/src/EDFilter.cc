/*----------------------------------------------------------------------
  
$Id$

----------------------------------------------------------------------*/

#include "FWCore/Framework/interface/EDFilter.h"

namespace edm
{
  EDFilter::~EDFilter()
  { }

  void EDFilter::beginJob(EventSetup const&) 
  { }
   
  void EDFilter::endJob()
  { }
   
}
  
