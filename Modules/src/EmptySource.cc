/*----------------------------------------------------------------------
$Id$
----------------------------------------------------------------------*/

#include <stdexcept>
#include <memory>


#include "FWCore/Modules/src/EmptySource.h"

namespace edm {
  EmptySource::EmptySource(ParameterSet const& pset,
				       InputSourceDescription const& desc) :
    GeneratedInputSource(pset, desc)
  { }

  EmptySource::~EmptySource() {
  }

  bool
  EmptySource::produce(edm::Event &) {
    return true;
  }
}
