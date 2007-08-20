#ifndef RecoAlgos_SuperclusteretHeepSkim_h
#define RecoAlgos_SuperclusteretHeepSkim_h

#include "DataFormats/EgammaReco/interface/SuperCluster.h"

   struct SuperClusterEtHeepSkim {
     typedef reco::SuperCluster type;
     double operator()( const reco::SuperCluster & c ) const {
       return c.energy() * sin( c.position().theta() );
     }
   };



#endif
