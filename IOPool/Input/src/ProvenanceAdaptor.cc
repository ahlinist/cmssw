/*----------------------------------------------------------------------

ProvenanceAdaptor.cc

----------------------------------------------------------------------*/
  //------------------------------------------------------------
  // Class ProvenanceAdaptor: adapts old provenance (fileFormatVersion_.value() < 11) to new provenance.

#include "IOPool/Input/src/ProvenanceAdaptor.h"

#include <cassert>
#include <set>
#include <utility>
#include <string>
#include <memory>
#include "DataFormats/Provenance/interface/BranchID.h"
#include "DataFormats/Provenance/interface/ProcessConfiguration.h"
#include "DataFormats/Provenance/interface/ProcessHistory.h"
#include "DataFormats/Provenance/interface/ProductRegistry.h"
#include "FWCore/Utilities/interface/Algorithms.h"

namespace edm {
  
  void
  ProvenanceAdaptor::fixProcessHistory(ProcessHistoryMap& pHistMap,
				       ProcessHistoryVector& pHistVector) {
    assert (pHistMap.empty() != pHistVector.empty());
    for (ProcessHistoryVector::const_iterator i = pHistVector.begin(), e = pHistVector.end(); i != e; ++i) {
      pHistMap.insert(std::make_pair(i->id(), *i));
    }
    pHistVector.clear();
    for (ProcessHistoryMap::const_iterator i = pHistMap.begin(), e = pHistMap.end(); i != e; ++i) {
      ProcessHistory newHist;
      ProcessHistoryID const& oldphID = i->first;
      for (ProcessHistory::const_iterator it = i->second.begin(), et = i->second.end(); it != et; ++it) {
	ParameterSetID const& newPsetID = convertID(it->parameterSetID());
	newHist.push_back(ProcessConfiguration(it->processName(), newPsetID, it->releaseVersion(), it->passID()));
      }
      assert(newHist.size() == i->second.size());
      ProcessHistoryID newphID = newHist.id();
      pHistVector.push_back(newHist);
      if (newphID != oldphID) {
        processHistoryIdConverter_.insert(std::make_pair(oldphID, newphID));
      }
    }
    assert(pHistVector.size() == pHistMap.size());
  }

  namespace {
    typedef StringVector OneHistory;
    typedef std::set<OneHistory> Histories;
    typedef std::pair<std::string, BranchID> Product;
    typedef std::vector<Product> OrderedProducts;
    struct Sorter {
      explicit Sorter(Histories const& histories) : histories_(histories) {}
      bool operator()(Product const& a, Product const& b) const;
      Histories const histories_;
    };
    bool Sorter::operator()(Product const& a, Product const& b) const {
      assert (a != b);
      if (a.first == b.first) return false;
      bool mayBeTrue = false;
      for (Histories::const_iterator it = histories_.begin(), itEnd = histories_.end(); it != itEnd; ++it) {
	OneHistory::const_iterator itA = find_in_all(*it, a.first);
	if (itA == it->end()) continue;
	OneHistory::const_iterator itB = find_in_all(*it, b.first);
	if (itB == it->end()) continue;
	assert (itA != itB);
	if (itB < itA) {
	  // process b precedes process a;
	  return false;
	}
	// process a precedes process b;
	mayBeTrue = true;
      }
      return mayBeTrue;
    }

    void
    fillProcessConfiguration(ProcessHistoryVector const& pHistVec, ProcessConfigurationVector& procConfigVector) {
      procConfigVector.clear();
      std::set<ProcessConfiguration> pcset;
      for (ProcessHistoryVector::const_iterator it = pHistVec.begin(), itEnd = pHistVec.end();
	  it != itEnd; ++it) {
	for (ProcessConfigurationVector::const_iterator i = it->begin(), iEnd = it->end();
	    i != iEnd; ++i) {
	  if (pcset.insert(*i).second) {
	    procConfigVector.push_back(*i);
	  }
	}
      }
    }

    void
    fillListsAndIndexes(ProductRegistry const& productRegistry,
			ProcessHistoryMap const& pHistMap,
			boost::shared_ptr<BranchIDLists const>& branchIDLists,
			std::vector<BranchListIndex>& branchListIndexes) {
      OrderedProducts orderedProducts;
      std::set<std::string> processNamesThatProduced;
      ProductRegistry::ProductList const& prodList = productRegistry.productList();
      for (ProductRegistry::ProductList::const_iterator it = prodList.begin(), itEnd = prodList.end();
	  it != itEnd; ++it) {
        if (it->second.branchType() == InEvent) {
	  it->second.init();
	  processNamesThatProduced.insert(it->second.processName());
	  orderedProducts.push_back(std::make_pair(it->second.processName(), it->second.branchID()));
        }
      }
      assert (!orderedProducts.empty());
      Histories processHistories;
      size_t max = 0;
      for(ProcessHistoryMap::const_iterator it = pHistMap.begin(), itEnd = pHistMap.end(); it != itEnd; ++it) {
        ProcessHistory const& pHist = it->second;
        OneHistory processHistory;
        for(ProcessHistory::const_iterator i = pHist.begin(), iEnd = pHist.end(); i != iEnd; ++i) {
	  if (processNamesThatProduced.find(i->processName()) != processNamesThatProduced.end()) {
	    processHistory.push_back(i->processName());
	  }
        }
        max = (processHistory.size() > max ? processHistory.size() : max);
        assert(max <= processNamesThatProduced.size());
        if (processHistory.size() > 1) {
          processHistories.insert(processHistory);
        }
      }
      stable_sort_all(orderedProducts, Sorter(processHistories));

      std::auto_ptr<BranchIDLists> pv(new BranchIDLists);
      std::auto_ptr<BranchIDList> p(new BranchIDList);
      std::string processName;
      BranchListIndex blix = 0;
      for (OrderedProducts::const_iterator it = orderedProducts.begin(), itEnd = orderedProducts.end(); it != itEnd; ++it) {
        if (it->first != processName) {
	  if (!processName.empty()) {
	    pv->push_back(*p);
	    branchListIndexes.push_back(blix);
	    ++blix;
	    p.reset(new BranchIDList);
          }
	  processName = it->first;
        }
        p->push_back(it->second.id());
      }
      pv->push_back(*p);
      branchListIndexes.push_back(blix);
      branchIDLists.reset(pv.release());
    }
  }

  ProvenanceAdaptor::ProvenanceAdaptor(
	     ProductRegistry& productRegistry,
	     ProcessHistoryMap& pHistMap,
	     ProcessHistoryVector& pHistVector,
	     ProcessConfigurationVector& procConfigVector,
	     ParameterSetIdConverter const& parameterSetIdConverter,
	     bool fullConversion) :
	        parameterSetIdConverter_(parameterSetIdConverter),
	        processHistoryIdConverter_(),
		branchIDLists_(),
		branchListIndexes_() {
    fixProcessHistory(pHistMap, pHistVector);
    fillProcessConfiguration(pHistVector, procConfigVector);
    if (fullConversion) {
      fillListsAndIndexes(productRegistry, pHistMap, branchIDLists_, branchListIndexes_);
    }
  }

  ProvenanceAdaptor::~ProvenanceAdaptor() {}

  ParameterSetID const&
  ProvenanceAdaptor::convertID(ParameterSetID const& oldID) const {
    ParameterSetIdConverter::const_iterator it = parameterSetIdConverter_.find(oldID);
    if (it == parameterSetIdConverter_.end()) {
      return oldID;
    }
    return it->second; 
  }

  ProcessHistoryID const&
  ProvenanceAdaptor::convertID(ProcessHistoryID const& oldID) const {
    ProcessHistoryIdConverter::const_iterator it = processHistoryIdConverter_.find(oldID);
    if (it == processHistoryIdConverter_.end()) {
      return oldID;
    }
    return it->second; 
  }

  boost::shared_ptr<BranchIDLists const>
  ProvenanceAdaptor::branchIDLists() const {
    return branchIDLists_;
  }

  void
  ProvenanceAdaptor::branchListIndexes(BranchListIndexes & indexes)  const {
    indexes = branchListIndexes_;
  }
}
