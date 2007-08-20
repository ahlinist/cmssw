/* 
 *
 *
 */
#include "FWCore/Framework/interface/MakerMacros.h"
#include "PhysicsTools/UtilAlgos/interface/ObjectCountFilter.h"
#include "SUSYBSMAnalysis/CSA07Skims/interface/EtMinSuperClusterSelectorHeepSkim.h"


typedef ObjectCountFilter<
          reco::SuperClusterCollection, 
          reco::modules::EtMinSuperClusterSelectorHeepSkim
        > EtMinSuperClusterCountFilterHeepSkim;

DEFINE_FWK_MODULE( EtMinSuperClusterCountFilterHeepSkim );
