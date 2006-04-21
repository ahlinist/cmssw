#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4Alignment.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4Crystal.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4DBEntry.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4DBManager.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4FEConfig.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4FenixStripConfig.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4FenixTcpConfig.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4Gains.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4Geom.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4Intercalibration.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4PedestalMgr.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4PedestalUtil.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4PnLinearity.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4PNPedestals.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4Run.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4ShapeParam.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4SM10Config.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4TdcRange.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4TowerPedestals.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/TPNsPeds.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/TTowerPeds.h"

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ nestedclass ;
#pragma link C++ class H4Alignment+;
#pragma link C++ class H4Crystal+;
#pragma link C++ class H4DBEntry+;
#pragma link C++ class H4DBManager+;
#pragma link C++ global gH4RootDBVersion+;
#pragma link C++ class H4FEConfig+;
#pragma link C++ class H4FenixStripConfig+;
#pragma link C++ class H4FenixTcpConfig+;
#pragma link C++ class H4Gains+;
#pragma link C++ class H4Geom+;
#pragma link C++ class H4Intercalibration+;
#pragma link C++ class H4PedestalMgr+;
#pragma link C++ class H4PedestalUtil+;
#pragma link C++ class H4PnLinearity+;
#pragma link C++ class H4PNPedestals+;
#pragma link C++ class H4Run+;
#pragma link C++ class H4ShapeParam::data+;
#pragma link C++ class H4ShapeParam+;
#pragma link C++ class H4SM10Config+;
#pragma link C++ class H4TdcRange+;
#pragma link C++ class H4TowerPedestals+;
#pragma link C++ class TPNsPeds+;
#pragma link C++ class TTowerPeds+;

#endif
