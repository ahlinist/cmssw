#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CSA07anl/Analyzer/interface/EMU_Filter.h"
#include "CSA07anl/Analyzer/interface/Analyzer.h"
#include "CSA07anl/Analyzer/interface/Z2TAUEMUFILTER.h"
#include "CSA07anl/Analyzer/interface/Analyzer_fam.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(Analyzer);
DEFINE_ANOTHER_FWK_MODULE(Analyzer_fam);
DEFINE_ANOTHER_FWK_MODULE(EMU_Filter);
DEFINE_ANOTHER_FWK_MODULE(Z2TAUEMUFILTER);

