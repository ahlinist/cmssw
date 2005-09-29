/*
 *  plugin.cc
 *  CMSSW
 *
 *  Created by Chris Jones on 7/24/05.
 *
 */

#include "CondCore/PluginSystem/interface/registration_macros.h"
#include "CondFormats/CSCObjects/interface/CSCPedestals.h"
#include "CondFormats/DataRecord/interface/CSCPedestalsRcd.h"
#include "CondFormats/CSCObjects/interface/CSCGains.h"
#include "CondFormats/DataRecord/interface/CSCGainsRcd.h"
#include "CondFormats/CSCObjects/interface/CSCXtalk.h"
#include "CondFormats/DataRecord/interface/CSCXtalkRcd.h"
DEFINE_SEAL_MODULE();
REGISTER_PLUGIN(CSCPedestalsRcd,CSCPedestals);
REGISTER_PLUGIN(CSCGainsRcd,CSCGains);
REGISTER_PLUGIN(CSCXtalkRcd,CSCXtalk);
