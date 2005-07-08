/*----------------------------------------------------------------------
  
$Id$

----------------------------------------------------------------------*/

#include "FWCore/CoreFramework/interface/EDAnalyzer.h"

namespace edm
{
  EDAnalyzer::~EDAnalyzer()
  { }

  void EDAnalyzer::beginJob( EventSetup const& ) 
  { }
   
  void EDAnalyzer::endJob()
  { }
}
  
