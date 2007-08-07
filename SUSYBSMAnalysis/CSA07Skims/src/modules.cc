#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "SUSYBSMAnalysis/CSA07Skims/interface/LepSUSYSkim.h"
#include "SUSYBSMAnalysis/CSA07Skims/interface/HadSUSYQCDControlMETSkim.h"
#include "SUSYBSMAnalysis/CSA07Skims/interface/HadSUSYQCDSkim.h"
#include "SUSYBSMAnalysis/CSA07Skims/interface/HadSUSYdiMuonSkim.h"
#include "SUSYBSMAnalysis/CSA07Skims/interface/HadSUSYdiElecSkim.h"
#include "SUSYBSMAnalysis/CSA07Skims/interface/SUSYHighPtPhotonSkim.h"
#include "SUSYBSMAnalysis/CSA07Skims/interface/HadSUSYSMBackgrSkim.h"
#include "SUSYBSMAnalysis/CSA07Skims/interface/HadSUSYTopSkim.h"
#include "SUSYBSMAnalysis/CSA07Skims/interface/TeVdiMuonSkim.h"


DEFINE_FWK_MODULE( LepSUSYSkim );
DEFINE_FWK_MODULE( HadSUSYQCDControlMETSkim );
DEFINE_FWK_MODULE( HadSUSYQCDSkim );
DEFINE_FWK_MODULE( HadSUSYdiMuonSkim );
DEFINE_FWK_MODULE( HadSUSYdiElecSkim );
DEFINE_FWK_MODULE( SUSYHighPtPhotonSkim );
DEFINE_FWK_MODULE( HadSUSYSMBackgrSkim );
DEFINE_FWK_MODULE( HadSUSYTopSkim );
DEFINE_FWK_MODULE( TeVdiMuonSkim );
