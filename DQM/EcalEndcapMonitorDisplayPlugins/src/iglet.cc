
#include "DQM/EcalEndcapMonitorDisplayPlugins/interface/EEMDisplayPlugins.h"
#include "VisMonitoring/VisDQMBase/interface/VisDQMDisplayPluginsDB.h"

DEFINE_IGUANA_MODULE();
DEFINE_IGUANA_PLUGIN( VisDQMDisplayPluginsDB, EEMDisplayPlugins, EEMDisplayPlugins::catalogLabel() );
