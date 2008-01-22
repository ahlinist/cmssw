#include "CondCore/PluginSystem/interface/registration_macros.h"
#include "CondFormats/IdealGeometryObjects/interface/PGeometricDet.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "Geometry/Records/interface/PGeometricDetRcd.h"

DEFINE_SEAL_MODULE();
REGISTER_PLUGIN(IdealGeometryRecord,DDCompactView);
REGISTER_PLUGIN(PGeometricDetRcd,PGeometricDet);
