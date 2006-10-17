#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "Configuration/CSA06Skimming/interface/MCProcessFilter.h"
#include "Configuration/CSA06Skimming/interface/MCSingleParticleFilter.h"
#include "Configuration/CSA06Skimming/interface/MCZll.h" 
#include "Configuration/CSA06Skimming/interface/HLTCSA06Bits.h"
#include "Configuration/CSA06Skimming/interface/MCParticlePairFilter.h"
#include "Configuration/CSA06Skimming/interface/MinimumBiasFilter.h"
#include "Configuration/CSA06Skimming/interface/MCProcessRangeFilter.h"
#include "Configuration/CSA06Skimming/interface/MCDijetResonance.h" 
#include "Configuration/CSA06Skimming/interface/RecoDiMuon.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(MCProcessFilter);
DEFINE_ANOTHER_FWK_MODULE(MCSingleParticleFilter);
DEFINE_ANOTHER_FWK_MODULE(HLTCSA06Bits);
DEFINE_ANOTHER_FWK_MODULE(MCParticlePairFilter);
DEFINE_ANOTHER_FWK_MODULE(MinimumBiasFilter);
DEFINE_ANOTHER_FWK_MODULE(MCProcessRangeFilter);
DEFINE_ANOTHER_FWK_MODULE(MCZll);
DEFINE_ANOTHER_FWK_MODULE(MCDijetResonance);
DEFINE_ANOTHER_FWK_MODULE(RecoDiMuon);

  
