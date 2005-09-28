#ifndef Framework_DelayedReader_h
#define Framework_DelayedReader_h

/*----------------------------------------------------------------------
  
DelayedReader: The abstract interface through which the EventPrincipal
uses input sources to retrieve EDProducts from external storage.

$Id$

----------------------------------------------------------------------*/

#include <memory>

#include "FWCore/Framework/interface/BranchKey.h"
#include "FWCore/EDProduct/interface/EDProduct.h"


namespace edm
{
  class DelayedReader
  {
  public:
    virtual ~DelayedReader();

    virtual std::auto_ptr<EDProduct> get(BranchKey const& k) const = 0;
  };
}

#endif
