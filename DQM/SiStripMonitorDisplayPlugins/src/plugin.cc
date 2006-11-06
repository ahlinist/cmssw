//<<<<<< INCLUDES                                                       >>>>>>

#include "DQM/SiStripMonitorDisplayPlugins/interface/TrackerMapDisplayPlugins.h"
#include "VisMonitoring/VisDQMBase/interface/VisDQMDisplayPluginsDB.h"
#include "PluginManager/ModuleDef.h"

DEFINE_SEAL_MODULE ();
DEFINE_SEAL_PLUGIN (VisDQMDisplayPluginsDB, TrackerMapDisplayPlugins, TrackerMapDisplayPlugins::catalogLabel ());
