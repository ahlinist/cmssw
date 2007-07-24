#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "CalibTracker/SiStripRunSummary/interface/SiStripRunSummaryWrite.h"
#include "CalibTracker/SiStripRunSummary/interface/SiStripRunSummaryRead.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(SiStripRunSummaryWrite);
DEFINE_ANOTHER_FWK_MODULE(SiStripRunSummaryRead);
