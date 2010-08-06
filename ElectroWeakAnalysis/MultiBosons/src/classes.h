#include "PhysicsTools/SelectorUtils/interface/Selector.h"
#include "PhysicsTools/SelectorUtils/interface/EventSelector.h"

#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaMuonSelector.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaElectronSelector.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaMETSelector.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaPhotonSelector.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaLeptonMETEventSelector.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaDiLeptonEventSelector.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaEventSelector.h"

namespace {
  struct dictionary {    
    pat::strbitset _bs;
    
    VGammaMuonSelector _vgms;
    VGammaElectronSelector _vges;
    VGammaMETSelector _vgmets;
    VGammaPhotonSelector _vgps;
    VGammaDiLeptonEventSelector _vgdles;
    VGammaLeptonMETEventSelector _vglmes;
    VGammaEventSelector _vgevs;
  };
}
