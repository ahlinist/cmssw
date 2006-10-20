#ifndef EgammaCandidates_ElectronIsolationAssociation_h
#define EgammaCandidates_ElectronIsolationAssociation_h
// \class ElectronIsolationAssociation
// 
// \short association of Isolation to an Electron
// $Id: $
//

#include "DataFormats/Common/interface/AssociationMap.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include <vector>

namespace reco {
  typedef edm::AssociationMap<edm::OneToValue<std::vector<reco::Electron>, float > > ElectronIsolationMap;
}
#endif
