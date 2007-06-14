#ifndef FWCore_Sources_VectorInputSource_h
#define FWCore_Sources_VectorInputSource_h


/*----------------------------------------------------------------------
  
VectorInputSource: Abstract interface for vector input sources.
$Id$

----------------------------------------------------------------------*/

#include <memory>
#include <vector>
#include "boost/shared_ptr.hpp"

#include "FWCore/Sources/interface/EDInputSource.h"

namespace edm {
  class EventPrincipal;
  class InputSourceDescription;
  class ParameterSet;
  class VectorInputSource : public EDInputSource {
  public:
    typedef boost::shared_ptr<EventPrincipal> EventPrincipalVectorElement;
    typedef std::vector<EventPrincipalVectorElement> EventPrincipalVector;
    explicit VectorInputSource(ParameterSet const& pset, InputSourceDescription const& desc);
    virtual ~VectorInputSource();

    void readMany(int number, EventPrincipalVector& result);

  private:
    virtual void readMany_(int number, EventPrincipalVector& result) = 0;
  };
}

#endif
