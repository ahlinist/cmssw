#include "CondCore/PluginSystem/interface/registration_macros.h"
#include "CondCore/IdealGeometryPlugins/interface/IdealGeometryProxy.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"

DEFINE_SEAL_MODULE();
REGISTER_PLUGIN(IdealGeometryRecord,DDCompactView);
