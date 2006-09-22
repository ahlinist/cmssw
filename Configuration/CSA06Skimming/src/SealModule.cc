#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "Configuration/CSA06Skimming/interface/MCProcessFilter.h"
#include "Configuration/CSA06Skimming/interface/MCSingleParticleFilter.h"
#include "Configuration/CSA06Skimming/interface/HLTCSA06Bits.h"
#include "Configuration/CSA06Skimming/interface/MCParticlePairFilter.h"
 

  DEFINE_SEAL_MODULE();
  DEFINE_ANOTHER_FWK_MODULE(MCProcessFilter);
  DEFINE_ANOTHER_FWK_MODULE(MCSingleParticleFilter);
  DEFINE_ANOTHER_FWK_MODULE(HLTCSA06Bits);
  DEFINE_ANOTHER_FWK_MODULE(MCParticlePairFilter)
