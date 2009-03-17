/*
 *  plugin.cc
 *  CMSSW
 *
 *  Created by Chris Jones on 7/24/05.
 *
 */

#include "CondCore/PluginSystem/interface/registration_macros.h"

DEFINE_SEAL_MODULE();


#include "CondFormats/DataRecord/interface/SiStripCondDataRecords.h"


#include "CondFormats/SiStripObjects/interface/SiStripPedestals.h"
REGISTER_PLUGIN(SiStripPedestalsRcd,SiStripPedestals);

#include "CondFormats/SiStripObjects/interface/SiStripNoises.h"
REGISTER_PLUGIN(SiStripNoisesRcd,SiStripNoises);

#include "CondFormats/SiStripObjects/interface/SiStripFedCabling.h"
REGISTER_PLUGIN(SiStripFedCablingRcd,SiStripFedCabling);

#include "CondFormats/SiStripObjects/interface/SiStripLorentzAngle.h"
REGISTER_PLUGIN(SiStripLorentzAngleRcd,SiStripLorentzAngle);
REGISTER_PLUGIN(SiStripLorentzAngleSimRcd,SiStripLorentzAngle);

#include "CondFormats/SiStripObjects/interface/SiStripApvGain.h"
REGISTER_PLUGIN(SiStripApvGainRcd,SiStripApvGain);
REGISTER_PLUGIN(SiStripApvGainSimRcd,SiStripApvGain);

#include "CondFormats/SiStripObjects/interface/SiStripBadStrip.h"
REGISTER_PLUGIN(SiStripBadStripRcd,SiStripBadStrip);
REGISTER_PLUGIN(SiStripBadModuleRcd,SiStripBadStrip);
REGISTER_PLUGIN(SiStripBadFiberRcd,SiStripBadStrip);
REGISTER_PLUGIN(SiStripBadChannelRcd,SiStripBadStrip);
REGISTER_PLUGIN(SiStripDCSStatusRcd,SiStripBadStrip);

#include "CondFormats/SiStripObjects/interface/SiStripModuleHV.h"
REGISTER_PLUGIN(SiStripModuleHVRcd,SiStripModuleHV);

#include "CondFormats/SiStripObjects/interface/SiStripRunSummary.h"
REGISTER_PLUGIN(SiStripRunSummaryRcd,SiStripRunSummary);

#include "CondFormats/SiStripObjects/interface/SiStripPerformanceSummary.h"
REGISTER_PLUGIN(SiStripPerformanceSummaryRcd,SiStripPerformanceSummary);

#include "CondFormats/SiStripObjects/interface/SiStripThreshold.h"
REGISTER_PLUGIN(SiStripThresholdRcd,SiStripThreshold);

#include "CondFormats/SiStripObjects/interface/SiStripSummary.h"
REGISTER_PLUGIN(SiStripSummaryRcd,SiStripSummary);





