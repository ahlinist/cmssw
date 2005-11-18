/*
 *  plugin.cc
 *  CMSSW
 *
 *  Created by Paolo Ronchese, 2005 Nov 14,
 *    based on template by Chris Jones on 7/24/05.
 *
 */

#include "CondCore/PluginSystem/interface/registration_macros.h"
#include "CondFormats/DTObjects/interface/DTReadOutMapping.h"
#include "CondFormats/DataRecord/interface/DTReadOutMappingRcd.h"
#include "CondFormats/DTObjects/interface/DTT0.h"
#include "CondFormats/DataRecord/interface/DTT0Rcd.h"


DEFINE_SEAL_MODULE();
REGISTER_PLUGIN(DTReadOutMappingRcd,DTReadOutMapping);
REGISTER_PLUGIN(DTT0Rcd,DTT0);
