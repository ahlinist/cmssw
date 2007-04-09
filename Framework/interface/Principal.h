#ifndef Framework_Principal_h
#define Framework_Principal_h

/*----------------------------------------------------------------------
  
Principal: This is the implementation of the classes
responsible for management of EDProducts. It is not seen by reconstruction code;

The major internal component of the Principal is the Group, which
contains an EDProduct and its associated Provenance, along with
ancillary transient information regarding the two. Groups are handled
through shared pointers.

The Principal returns BasicHandle, rather than a shared
pointer to a Group, when queried.

$Id$

----------------------------------------------------------------------*/
#include <list>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

#include "boost/shared_ptr.hpp"

#include "DataFormats/Provenance/interface/BranchKey.h"
#include "DataFormats/Provenance/interface/ProductID.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "DataFormats/Common/interface/EDProductGetter.h"
#include "DataFormats/Provenance/interface/ProcessHistory.h"
#include "DataFormats/Provenance/interface/ProcessHistoryID.h"
#include "DataFormats/Common/interface/BasicHandle.h"
#include "FWCore/Framework/interface/NoDelayedReader.h"
#include "FWCore/Framework/interface/DelayedReader.h"
#include "FWCore/Framework/interface/SelectorBase.h"

#include "FWCore/Framework/interface/Group.h"
#include "FWCore/Utilities/interface/TypeID.h"

namespace edm {
  class ProductRegistry;
  class UnscheduledHandler;
  class Principal : public EDProductGetter {
  public:
    typedef std::vector<boost::shared_ptr<Group> > GroupVec;
    typedef GroupVec::const_iterator               const_iterator;
    typedef ProcessHistory::const_iterator         ProcessNameConstIterator;
    typedef boost::shared_ptr<const Group>         SharedConstGroupPtr;
    typedef std::vector<BasicHandle>               BasicHandleVec;
    typedef GroupVec::size_type                    size_type;

    typedef boost::shared_ptr<Group> SharedGroupPtr;
    typedef std::string ProcessName;
    typedef std::list<SharedConstGroupPtr> MatchingGroups;
    typedef std::map<ProcessName, MatchingGroups> MatchingGroupLookup;

    Principal(ProductRegistry const& reg,
		   ProcessConfiguration const& pc,
                   ProcessHistoryID const& hist = ProcessHistoryID(),
                   boost::shared_ptr<DelayedReader> rtrv = boost::shared_ptr<DelayedReader>(new NoDelayedReader));

    virtual ~Principal();
    size_t  size() const { return groups_.size(); }

    EDProductGetter const* prodGetter() const {return this;}

    Principal const& groupGetter() const {return *this;}

    Principal & groupGetter() {return *this;}

    // Return the number of EDProducts contained.
    size_type numEDProducts() const;
    
    void put(std::auto_ptr<EDProduct> edp,
	     std::auto_ptr<Provenance> prov);

    SharedConstGroupPtr const getGroup(ProductID const& oid, bool resolve = true) const;

    BasicHandle  get(ProductID const& oid) const;

    BasicHandle  getBySelector(TypeID const& tid, SelectorBase const& s) const;

    BasicHandle  getByLabel(TypeID const& tid,
			    std::string const& label,
                            std::string const& productInstanceName) const;

    BasicHandle  getByLabel(TypeID const& tid,
			    std::string const& label,
			    std::string const& productInstanceName,
			    std::string const& processName) const;

    void getMany(TypeID const& tid, 
		 SelectorBase const&,
		 BasicHandleVec& results) const;

    BasicHandle  getByType(TypeID const& tid) const;

    void getManyByType(TypeID const& tid, 
		 BasicHandleVec& results) const;

    // Return a BasicHandle to the product which:
    //   1. is a sequence,
    //   2. and has the nested type 'value_type'
    //   3. and for which valuetype is the same as or a public base of
    //      this value_type,
    //   4. and which matches teh given module label and product
    //      instance name.
    BasicHandle getMatchingSequence(std::type_info const& wantedElementType,
				    std::string const& moduleLabel,
				    std::string const& productInstanceName) const;

    Provenance const&
    getProvenance(ProductID const& oid) const;

    void
    getAllProvenance(std::vector<Provenance const *> & provenances) const;

    // ----- access to all products

    const_iterator begin() const { return groups_.begin(); }
    const_iterator end() const { return groups_.end(); }

    ProcessHistory const& processHistory() const;    

    ProcessHistoryID const& processHistoryID() const {
      return processHistoryID_;   
    }

    // ----- Add a new Group
    // *this takes ownership of the Group, which in turn owns its
    // data.
    void addGroup(std::auto_ptr<Group> g);

    ProductRegistry const& productRegistry() const {return *preg_;}

    boost::shared_ptr<DelayedReader> store() const {return store_;}

    virtual EDProduct const* getIt(ProductID const& oid) const;

    // ----- Mark this Principal as having been updated in the
    // current Process.
    void addToProcessHistory() const;

  protected:
    void setUnscheduled() {unscheduled_ = true;}

  private:

    // ----- manipulation of provenance

    virtual bool unscheduledFill(Group const& group) const = 0;

    virtual bool fillAndMatchSelector(Provenance& prov, SelectorBase const& selector) const = 0;

    SharedConstGroupPtr const getInactiveGroup(ProductID const& oid) const;

    void getAllMatches_(std::string const& moduleLabel,
			std::string const& productInstanceName,
			MatchingGroupLookup& matches) const;

    // Make my DelayedReader get the EDProduct for a Group.  The Group is
    // a cache, and so can be modified through the const reference.
    // We do not change the *number* of groups through this call, and so
    // *this is const.
    void resolve_(Group const& g, bool unconditional = false) const;

    void fillElementLookup(const ROOT::Reflex::Type & type, int slotNumber);

    mutable ProcessHistoryID processHistoryID_;

    boost::shared_ptr<ProcessHistory> processHistoryPtr_;

    ProcessConfiguration const& processConfiguration_;

    mutable bool processHistoryModified_;

    // A vector of active groups.
    GroupVec groups_; // products and provenances are persistent

    // users need to vary the info in the BranchKey object
    // to store the output of different code versions for the
    // same configured module (e.g. change processName_)

    // indices are to product/provenance slot
    typedef std::map<BranchKey, int> BranchDict;
    BranchDict branchDict_; // 1->1

    typedef std::map<ProductID, int> ProductDict;
    ProductDict productDict_; // 1->1

    typedef std::map<std::string, std::vector<int> > TypeDict;
    TypeDict productLookup_; // 1->many
    TypeDict elementLookup_; // 1->many

    // A vector of inactive groups (product not present or inaccessible).
    GroupVec inactiveGroups_; // products and provenances are persistent

    // indices are to product/provenance slot
    BranchDict inactiveBranchDict_; // 1->1
    ProductDict inactiveProductDict_; // 1->1
    TypeDict inactiveTypeDict_; // 1->many

    // it is probably straightforward to load the BranchKey
    // dictionary above with information from the input source - 
    // mostly because this is information from provenance.
    // The product provanance are likewise easily filled.
    // The typeid index is more of a problem. Here
    // the I/O subsystem will need to take the friendly product
    // name string from provenance and get back a TypeID object.
    // Getting the products loaded (from the file) is another
    // issue. Here we may need some sort of hook into the I/O
    // system unless we want to preload all products (probably
    // not a good idea).
    // At MiniBooNE, this products object was directly part of
    // ROOT and the "gets" caused things to load properly - and
    // this is where the reservation for an object came into
    // the picture i.e. the "maker" function of the event.
    // should eventprincipal be pure interface?
    // should ROOT just be present here?
    // What goes into the event header(s)? Which need to be persistent?

    // Pointer to the product registry. There is one entry in the registry
    // for each EDProduct in the event.
    ProductRegistry const* preg_;

    // Pointer to the 'source' that will be used to obtain EDProducts
    // from the persistent store.
    boost::shared_ptr<DelayedReader> store_;

    // Unscheduled handling supported?
    bool unscheduled_;
  };
}
#endif
