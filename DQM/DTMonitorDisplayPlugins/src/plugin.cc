
#include "DQM/DTMonitorDisplayPlugins/interface/DTMDisplayPlugins.h"
#include "VisMonitoring/VisDQMBase/interface/VisDQMDisplayPluginsDB.h"


DEFINE_IGUANA_MODULE();
DEFINE_IGUANA_PLUGIN( VisDQMDisplayPluginsDB, DTMDisplayPlugins, DTMDisplayPlugins::catalogLabel() );
