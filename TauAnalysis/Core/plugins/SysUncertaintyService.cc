#include "TauAnalysis/Core/interface/SysUncertaintyService.h"

const double SysUncertaintyService_namespace::defaultEvtReweight_error = -1.;
const char* SysUncertaintyService_namespace::regexpParserExpr_array_entry = "[[:alnum:]]+\\([[:digit:]]+\\)";
const char* SysUncertaintyService_namespace::regexpParserExpr_array_elements = "([[:alnum:]]+)\\(([[:digit:]]+)\\)";

SysUncertaintyService* SysUncertaintyService::gSysUncertaintyService = 0;

#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"
#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_SERVICE(SysUncertaintyService);
