#include "TauAnalysis/Core/interface/SysUncertaintyService.h"

SysUncertaintyService* SysUncertaintyService::gSysUncertaintyService = 0;

#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"
#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_SERVICE(SysUncertaintyService);
