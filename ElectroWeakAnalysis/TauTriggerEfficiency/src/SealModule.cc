#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/PluginManager/interface/ModuleDef.h"
//#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L1TauEfficiencyAnalyzer.h"
//#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L2TauEfficiencyAnalyzer.h"
//#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L25and3TauEfficiencyAnalyzer.h"
#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/TTEffAnalyzer.h"
#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/EventCounter.h"

DEFINE_SEAL_MODULE();
//DEFINE_ANOTHER_FWK_MODULE(L1TauEfficiencyAnalyzer);
//DEFINE_ANOTHER_FWK_MODULE(L2TauEfficiencyAnalyzer);
//DEFINE_ANOTHER_FWK_MODULE(L25and3TauEfficiencyAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(TTEffAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(EventCounter);

