//<<<<<< INCLUDES                                                       >>>>>>

#include "DQM/SiStripMonitorDisplayPlugins/interface/TrackerMapDisplayPlugins.h"
#include "VisMonitoring/VisDQMBase/interface/VisDQMDisplayPluginsDB.h"
#include "FWCore/PluginManager/interface/ModuleDef.h"

DEFINE_SEAL_MODULE ();
DEFINE_SEAL_PLUGIN (VisDQMDisplayPluginsDB, TrackerMapDisplayPlugins, TrackerMapDisplayPlugins::catalogLabel ());
