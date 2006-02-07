#ifndef Framework_DelayedReader_h
#define Framework_DelayedReader_h

/*----------------------------------------------------------------------
  
DelayedReader: The abstract interface through which the EventPrincipal
uses input sources to retrieve EDProducts from external storage.

$Id: DelayedReader.h,v 1.3 2006/01/06 00:29:32 wmtan Exp $

----------------------------------------------------------------------*/

#include <memory>

#include "DataFormats/Common/interface/EDProduct.h"


namespace edm {
  class BranchKey;
  class EDProductGetter;
  class DelayedReader {
  public:
    virtual ~DelayedReader();

    virtual std::auto_ptr<EDProduct> get(BranchKey const& k, EDProductGetter const* ep) const = 0;
  };
}

#endif
