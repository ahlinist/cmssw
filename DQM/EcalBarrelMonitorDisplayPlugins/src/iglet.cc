
#include "DQM/EcalBarrelMonitorDisplayPlugins/interface/EBMDisplayPlugins.h"
#include "VisMonitoring/VisDQMBase/interface/VisDQMDisplayPluginsDB.h"

DEFINE_IGUANA_MODULE();
DEFINE_IGUANA_PLUGIN( VisDQMDisplayPluginsDB, EBMDisplayPlugins, EBMDisplayPlugins::catalogLabel() );
