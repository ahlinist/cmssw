//#define MC_ENABLER      (comment if you want to drom MC dependencies)
#include "DQMOffline/Tau/interface/TauTagValidation.h"

#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DQMOffline/Tau/interface/DQMTau.h"

typedef TauTagValidation<DQMTau>  TauTagDQM;

DEFINE_FWK_MODULE( TauTagDQM );

