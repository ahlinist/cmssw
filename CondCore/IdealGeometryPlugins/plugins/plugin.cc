#include "CondCore/PluginSystem/interface/registration_macros.h"
#include "CondCore/IdealGeometryPlugins/interface/IdealGeometryProxy.h"
#include "CondFormats/IdealGeometryObjects/interface/PGeometricDet.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "CondFormats/DataRecord/interface/PGeometricDetRcd.h"

DEFINE_SEAL_MODULE();
REGISTER_PLUGIN(IdealGeometryRecord,DDCompactView);
REGISTER_PLUGIN(PGeometricDetRcd,PGeometricDet);
