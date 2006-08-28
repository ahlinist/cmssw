#ifndef L1Trigger_L1MuonParticleFwd_h
#define L1Trigger_L1MuonParticleFwd_h
// -*- C++ -*-
//
// Package:     L1Trigger
// Class  :     L1MuonParticleFwd
// 
/**\class L1MuonParticleFwd L1MuonParticleFwd.h DataFormats/L1Trigger/interface/L1MuonParticleFwd.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Original Author:  Werner Sun
//         Created:  Sat Jul 15 14:28:43 EDT 2006
// $Id: L1MuonParticleFwd.h,v 1.2 2006/07/26 00:05:39 wsun Exp $
//

// system include files

// user include files

// forward declarations
#include <vector>
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefVector.h"


namespace l1extra {

   class L1MuonParticle ;

   typedef std::vector< L1MuonParticle > L1MuonParticleCollection ;

   typedef edm::Ref< L1MuonParticleCollection > L1MuonParticleRef ;
   typedef edm::RefVector< L1MuonParticleCollection > L1MuonParticleRefVector ;
   typedef std::vector< L1MuonParticleRef > L1MuonParticleVectorRef ;
}

#endif
