#include "DataFormats/Streamer/interface/StreamedProducts.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/Utilities/interface/WrappedClassName.h"

namespace edm {
    StreamedProduct::StreamedProduct(void const* prod,
		    BranchDescription const& desc,
		    ProductStatus status,
		    std::vector<BranchID> const* parents) :
      desc_(&desc), status_(status), parents_(parents), prod_(const_cast<void*>(prod)), classRef_() {
      if(productstatus::present(status_) && prod == 0) {
	desc.init();
        throw edm::Exception(edm::errors::LogicError, "StreamedProduct::StreamedProduct\n")
           << "A product with a status of 'present' is not actually present.\n"
           << "The branch name is " << desc.branchName() << "\n"
           << "Contact a framework developer.\n";
      }
    }

    void
    StreamedProduct::allocateForReading() {
      setNewClassType();
      prod_ = classRef_->New();
    }

    void
    StreamedProduct::setNewClassType() {
      classRef_.SetName(wrappedClassName(desc_->fullClassName()).c_str());
    }

    void
    StreamedProduct::clearClassType() {
      if(prod_ != 0) {
        classRef_->Destructor(prod_);
        prod_ = 0;
      }
    }
}

