#ifndef Framework_EventPrincipal_h
#define Framework_EventPrincipal_h

/*----------------------------------------------------------------------
  
EventPrincipal: This is the class responsible for management of
per event EDProducts. It is not seen by reconstruction code;
such code sees the Event class, which is a proxy for EventPrincipal.

The major internal component of the EventPrincipal
is the DataBlock.

$Id: EventPrincipal.h,v 1.46 2006/12/29 20:55:07 wmtan Exp $

----------------------------------------------------------------------*/

#include "DataFormats/Common/interface/EventAux.h"
#include "FWCore/Framework/interface/DataBlockImpl.h"
#include "FWCore/Framework/interface/EPEventProvenanceFiller.h"
#include "FWCore/Framework/interface/UnscheduledHandler.h"

#include "boost/shared_ptr.hpp"

namespace edm {
  class EventID;
  class LuminosityBlockPrincipal;
  class RunPrincipal;
  class EventPrincipal : private DataBlockImpl {
    typedef DataBlockImpl Base;
  public:
    typedef Base::const_iterator const_iterator;
    typedef Base::SharedConstGroupPtr SharedConstGroupPtr;
    EventPrincipal(EventID const& id,
	Timestamp const& time,
	ProductRegistry const& reg,
        boost::shared_ptr<LuminosityBlockPrincipal const> lbp,
        ProcessConfiguration const& pc,
	ProcessHistoryID const& hist = ProcessHistoryID(),
	boost::shared_ptr<DelayedReader> rtrv = boost::shared_ptr<DelayedReader>(new NoDelayedReader));
    EventPrincipal(EventID const& id,
	Timestamp const& time,
	ProductRegistry const& reg,
        ProcessConfiguration const& pc,
	ProcessHistoryID const& hist = ProcessHistoryID(),
	boost::shared_ptr<DelayedReader> rtrv = boost::shared_ptr<DelayedReader>(new NoDelayedReader));
    ~EventPrincipal() {}

    LuminosityBlockPrincipal const& luminosityBlockPrincipal() const {
      return *luminosityBlockPrincipal_;
    }

    boost::shared_ptr<LuminosityBlockPrincipal const> const
    luminosityBlockPrincipalConstSharedPtr() const {
      return luminosityBlockPrincipal_;
    }

    EventID const& id() const {
      return aux().id();
    }

    Timestamp const& time() const {
      return aux().time();
    }

    EventAux const& aux() const {
      return aux_;
    }

    LuminosityBlockID const& luminosityBlockID() const {
      return aux().luminosityBlockID();
    }

    RunNumber_t runNumber() const {
      return id().run();
    }

    RunPrincipal const& runPrincipal() const;

    using Base::addGroup;
    using Base::begin;
    using Base::end;
    using Base::getAllProvenance;
    using Base::getByLabel;
    using Base::get;
    using Base::getBySelector;
    using Base::getByType;
    using Base::getGroup;
    using Base::getIt;
    using Base::getMany;
    using Base::getManyByType;
    using Base::getProvenance;
    using Base::groupGetter;
    using Base::numEDProducts;
    using Base::processHistory;
    using Base::processHistoryID;
    using Base::prodGetter;
    using Base::productRegistry;
    using Base::put;
    using Base::size;
    using Base::store;

    void setUnscheduledHandler(boost::shared_ptr<UnscheduledHandler> iHandler);

  private:

    virtual bool unscheduledFill(Group const& group) const;

    virtual bool fillAndMatchSelector(Provenance& prov, SelectorBase const& selector) const;

    EventAux aux_;
    boost::shared_ptr<LuminosityBlockPrincipal const> const luminosityBlockPrincipal_;
    // Handler for unscheduled modules
    boost::shared_ptr<UnscheduledHandler> unscheduledHandler_;
    // Provenance filler for unscheduled modules
    boost::shared_ptr<EPEventProvenanceFiller> provenanceFiller_;
  };

  inline
  bool
  isSameRun(EventPrincipal const* a, EventPrincipal const* b) {
    return(a != 0 && b != 0 && a->runNumber() == b->runNumber());
  }

  inline
  bool
  isSameLumi(EventPrincipal const* a, EventPrincipal const* b) {
    return(isSameRun(a, b) && a->luminosityBlockID() == b->luminosityBlockID());
  }
}
#endif

