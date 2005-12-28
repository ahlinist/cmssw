/*----------------------------------------------------------------------
  
$Id$

----------------------------------------------------------------------*/

#include "FWCore/Framework/interface/EDProducer.h"

namespace edm {
  EDProducer::~EDProducer() { }

  void EDProducer::beginJob(EventSetup const&) {
  }

  void EDProducer::endJob() {
  }

   boost::function<void(const BranchDescription&)> EDProducer::registrationCallback() const {
      return callWhenNewProductsRegistered_;
   }
}
