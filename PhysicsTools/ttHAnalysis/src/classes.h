#include "PhysicsTools/ttHAnalysis/interface/NtupleData.h"
#include "PhysicsTools/ttHAnalysis/interface/ElectronVariables.h"
#include "PhysicsTools/ttHAnalysis/interface/JetVariables.h"
#include "PhysicsTools/ttHAnalysis/interface/MissingEtVariables.h"
#include "FWCore/EDProduct/interface/Wrapper.h"
#include <vector>

namespace {
  namespace {
    edm::Wrapper<NtupleData> dummy;
    std::vector<ElectronVariables> ev;
    edm::Wrapper<std::vector<ElectronVariables> > wev;
    std::vector<JetVariables> jv;
    edm::Wrapper<std::vector<JetVariables> > wjv;
    std::vector<MissingEtVariables> mv;
    edm::Wrapper<std::vector<MissingEtVariables> > wmv;
  }
}
	
