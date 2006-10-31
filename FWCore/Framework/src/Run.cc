#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/RunPrincipal.h"

namespace edm {

    Run::Run(RunPrincipal& dbk, ModuleDescription const& md) :
	DataViewImpl(dbk.impl(), md),
	aux_(dbk.aux()) {
    }
}
