#ifndef UtilAlgos_FunctionMinSelectorHeepSkim_h
#define UtilAlgos_FunctionMinSelectorHeepSkim_h


#include "PhysicsTools/UtilAlgos/interface/ParameterAdapter.h"
#include "SUSYBSMAnalysis/CSA07Skims/interface/FunctionMinSelectorHeepSkim.h"
#include "SUSYBSMAnalysis/CSA07Skims/interface/SuperClusterEtHeepSkim.h"

namespace reco {
   namespace modules {
     typedef FunctionMinSelectorHeepSkim<SuperClusterEtHeepSkim> EtMinSuperClusterSelectorHeepSkim;

     template<>
     struct ParameterAdapter<EtMinSuperClusterSelectorHeepSkim> {
       static EtMinSuperClusterSelectorHeepSkim make(
         const edm::ParameterSet & cfg ) {
	 return EtMinSuperClusterSelectorHeepSkim( cfg.getParameter<double>( "etMin" ) );
       }
     };

   }
}

#endif
