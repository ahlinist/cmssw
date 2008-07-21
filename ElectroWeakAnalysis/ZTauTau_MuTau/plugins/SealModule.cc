#include <FWCore/PluginManager/interface/ModuleDef.h>
#include <FWCore/Framework/interface/MakerMacros.h>

#include <UserAnalysisCode/ZTauTauAnalysis/plugins/ZTauTauAnalysis.h>
#include <UserAnalysisCode/ZTauTauAnalysis/plugins/MuTkIsolation.h>                                             
#include <UserAnalysisCode/ZTauTauAnalysis/plugins/Filtro.h>
#include <UserAnalysisCode/ZTauTauAnalysis/plugins/BtaggingModule.h>
/*#include <UserAnalysisCode/ZTauTauAnalysis/plugins/MuonCalIso.h>*/

DEFINE_ANOTHER_FWK_MODULE(Filtro);
DEFINE_ANOTHER_FWK_MODULE(ZTauTauAnalysis);

