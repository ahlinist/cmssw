#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "ElectroWeakAnalysis/ZTauTau_DoubleTauJet/interface/MyFilter.h"
#include "ElectroWeakAnalysis/ZTauTau_DoubleTauJet/interface/EventSkipFilter.h"
#include "ElectroWeakAnalysis/ZTauTau_DoubleTauJet/interface/EWKTauAnalyser.h"
DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE( MyFilter );
DEFINE_ANOTHER_FWK_MODULE( EventSkipFilter );
DEFINE_ANOTHER_FWK_MODULE( EWKTauAnalyser );
