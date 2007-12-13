#include "FWCore/Framework/interface/MakerMacros.h"
#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoTauJetMcFilter.h"
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoTauMcInfo.h"
#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoHLTInfo.h"
#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoTauTagInfo.h"
#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoTauTagInfoFilter.h"
#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoTauTagProngFilter.h"
#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoTauTagHcalEtFilter.h"
#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoCollinearityFilter.h"
#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoTauJetEtFilter.h"
#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoLeptonEtFilter.h"
#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoElectronIdFilter.h"
#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoMassInfo.h"
#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoEfficiencyAnalyzer.h"
#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoSelectionEfficiencyAnalyzer.h"
#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoObjectQualityAnalyzer.h"
#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoTauTaggingEfficiencyCalculator.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(ZETauRecoTauJetMcFilter);
DEFINE_ANOTHER_FWK_MODULE(ZETauRecoTauMcInfo);
DEFINE_ANOTHER_FWK_MODULE(ZETauRecoHLTInfo);
DEFINE_ANOTHER_FWK_MODULE(ZETauRecoTauTagInfo);
DEFINE_ANOTHER_FWK_MODULE(ZETauRecoTauTagInfoFilter);
DEFINE_ANOTHER_FWK_MODULE(ZETauRecoTauTagProngFilter);
DEFINE_ANOTHER_FWK_MODULE(ZETauRecoTauTagHcalEtFilter);
DEFINE_ANOTHER_FWK_MODULE(ZETauRecoCollinearityFilter);
DEFINE_ANOTHER_FWK_MODULE(ZETauRecoTauJetEtFilter);
DEFINE_ANOTHER_FWK_MODULE(ZETauRecoLeptonEtFilter);
DEFINE_ANOTHER_FWK_MODULE(ZETauRecoElectronIdFilter);
DEFINE_ANOTHER_FWK_MODULE(ZETauRecoMassInfo);
DEFINE_ANOTHER_FWK_MODULE(ZETauRecoEfficiencyAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(ZETauRecoSelectionEfficiencyAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(ZETauRecoObjectQualityAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(ZETauRecoTauTaggingEfficiencyCalculator);
