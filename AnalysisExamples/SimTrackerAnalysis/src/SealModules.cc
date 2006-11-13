#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "AnalysisExamples/SimTrackerAnalysis/interface/SimHitTrackerAnalyzer.h"
#include "AnalysisExamples/SimTrackerAnalysis/interface/SimTrackSimVertexAnalyzer.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(SimHitTrackerAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(SimTrackSimVertexAnalyzer);
