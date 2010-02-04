import FWCore.ParameterSet.Config as cms
import copy

# define configuration parameters for submission of Z --> e + tau-jet jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
#
# loosely skimmed samples:
#   gsfElectron with abs(eta) < 2.5 and pt > 8
#	PF or calo tau (discriminant by leading pion)
#   dR > 0.3 for electron and tau cand.
#
# Authors:
#	Jeff Kolb, Notre Dame
#   Christian Veelken, UC Davis
#

intLumiZtoElecTau_Data = float(200.)

#--------------------------------------------------------------------------------
# Z --> tau+ tau- sample generated with Pythia + Tauola (all decay modes)
#
#  10 TeV
#  /Ztautau/Summer09-MC_31X_V3-v1/GEN-SIM-RECO
#  generated events = 2068207
#  skimmed events = 246599
#
intLumiZtoElecTau_Ztautau_10TeV = float(1063.9)
corrFactorZtoElecTau_Ztautau_10TeV = float(1.)

patTupleOutputFileNameZtoElecTau_Ztautau_10TeV = cms.untracked.string('patTupleZtoElecTau_Ztautau_10TeV_partXX.root')
plotsOutputFileNameZtoElecTau_Ztautau_10TeV = cms.string('plotsZtoElecTau_Ztautau_10TeV_partXX.root')
genPhaseSpaceCutZtoElecTau_Ztautau_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

fileNamesZtoElecTau_Ztautau_10TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_01.root'
)
fileNamesZtoElecTau_Ztautau_10TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_02.root'
)	
fileNamesZtoElecTau_Ztautau_10TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_03.root'
)	
fileNamesZtoElecTau_Ztautau_10TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_04.root'
)
fileNamesZtoElecTau_Ztautau_10TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_05.root'
)
fileNamesZtoElecTau_Ztautau_10TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_06.root'
)
fileNamesZtoElecTau_Ztautau_10TeV_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_07.root'
)
fileNamesZtoElecTau_Ztautau_10TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_08.root'
)
fileNamesZtoElecTau_Ztautau_10TeV_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_09.root'
)
fileNamesZtoElecTau_Ztautau_10TeV_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_10.root'
)
fileNamesZtoElecTau_Ztautau_10TeV_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_11.root'
)
fileNamesZtoElecTau_Ztautau_10TeV_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_12.root'
)	
fileNamesZtoElecTau_Ztautau_10TeV_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_13.root'
)	
fileNamesZtoElecTau_Ztautau_10TeV_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_14.root'
)
fileNamesZtoElecTau_Ztautau_10TeV_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_15.root'
)
fileNamesZtoElecTau_Ztautau_10TeV_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_16.root'
)
fileNamesZtoElecTau_Ztautau_10TeV_part17 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_17.root'
)
fileNamesZtoElecTau_Ztautau_10TeV_part18 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_18.root'
)
fileNamesZtoElecTau_Ztautau_10TeV_part19 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_19.root'
)
fileNamesZtoElecTau_Ztautau_10TeV_part20 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_20.root'
)
fileNamesZtoElecTau_Ztautau_10TeV_part21 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_21.root'
)
fileNamesZtoElecTau_Ztautau_10TeV_part22 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_22.root'
)
fileNamesZtoElecTau_Ztautau_10TeV_part23 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_10TeV/skimElecTau_Ztautau_10TeV_23.root'
)
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# Z --> e+ e- sample generated with Pythia
#
#  10 TeV
#  /Zee/Summer09-MC_31X_V3-v1/GEN-SIM-RECO
#  generated events = 2682355
#  skimmed events = 1183702
#
intLumiZtoElecTau_Zee_10TeV = float(1379.8)
# 3/60 skim output files missing
corrFactorZtoElecTau_Zee_10TeV = float(1.053)

patTupleOutputFileNameZtoElecTau_Zee_10TeV = cms.untracked.string('patTupleZtoElecTau_Zee_10TeV_partXX.root')

plotsOutputFileNameZtoElecTau_Zee_10TeV = cms.string('plotsZtoElecTau_Zee_10TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_Zee_10TeV = cms.PSet(
  pluginName = cms.string('genPhaseSpaceCut'),
  pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

fileNamesZtoElecTau_Zee_10TeV_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_2.root'
)
fileNamesZtoElecTau_Zee_10TeV_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_3.root'
)
fileNamesZtoElecTau_Zee_10TeV_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_4.root'
)
fileNamesZtoElecTau_Zee_10TeV_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_5.root'
)
fileNamesZtoElecTau_Zee_10TeV_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_6.root'
)
fileNamesZtoElecTau_Zee_10TeV_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_7.root'
)
fileNamesZtoElecTau_Zee_10TeV_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_8.root'
)
fileNamesZtoElecTau_Zee_10TeV_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_9.root'
)
fileNamesZtoElecTau_Zee_10TeV_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_10.root'
)
fileNamesZtoElecTau_Zee_10TeV_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_11.root'
)
fileNamesZtoElecTau_Zee_10TeV_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_12.root'
)
fileNamesZtoElecTau_Zee_10TeV_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_13.root'
)
fileNamesZtoElecTau_Zee_10TeV_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_14.root'
)
fileNamesZtoElecTau_Zee_10TeV_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_15.root'
)
fileNamesZtoElecTau_Zee_10TeV_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_16.root'
)
fileNamesZtoElecTau_Zee_10TeV_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_17.root'
)
fileNamesZtoElecTau_Zee_10TeV_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_18.root'
)
fileNamesZtoElecTau_Zee_10TeV_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_19.root'
)
fileNamesZtoElecTau_Zee_10TeV_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_20.root'
)
fileNamesZtoElecTau_Zee_10TeV_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_21.root'
)
fileNamesZtoElecTau_Zee_10TeV_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_22.root'
)
fileNamesZtoElecTau_Zee_10TeV_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_23.root'
)
fileNamesZtoElecTau_Zee_10TeV_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_24.root'
)
fileNamesZtoElecTau_Zee_10TeV_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_25.root'
)
fileNamesZtoElecTau_Zee_10TeV_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_26.root'
)
fileNamesZtoElecTau_Zee_10TeV_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_27.root'
)
fileNamesZtoElecTau_Zee_10TeV_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_28.root'
)
fileNamesZtoElecTau_Zee_10TeV_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_29.root'
)
fileNamesZtoElecTau_Zee_10TeV_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_30.root'
)
fileNamesZtoElecTau_Zee_10TeV_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_31.root'
)
fileNamesZtoElecTau_Zee_10TeV_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_32.root'
)
fileNamesZtoElecTau_Zee_10TeV_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_33.root'
)
fileNamesZtoElecTau_Zee_10TeV_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_34.root'
)
fileNamesZtoElecTau_Zee_10TeV_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_35.root'
)
fileNamesZtoElecTau_Zee_10TeV_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_36.root'
)
fileNamesZtoElecTau_Zee_10TeV_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_37.root'
)
fileNamesZtoElecTau_Zee_10TeV_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_39.root'
)
fileNamesZtoElecTau_Zee_10TeV_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_41.root'
)
fileNamesZtoElecTau_Zee_10TeV_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_42.root'
)
fileNamesZtoElecTau_Zee_10TeV_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_43.root'
)
fileNamesZtoElecTau_Zee_10TeV_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_44.root'
)
fileNamesZtoElecTau_Zee_10TeV_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_45.root'
)
fileNamesZtoElecTau_Zee_10TeV_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_46.root'
)
fileNamesZtoElecTau_Zee_10TeV_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_47.root'
)
fileNamesZtoElecTau_Zee_10TeV_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_48.root'
)
fileNamesZtoElecTau_Zee_10TeV_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_49.root'
)
fileNamesZtoElecTau_Zee_10TeV_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_50.root'
)
fileNamesZtoElecTau_Zee_10TeV_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_51.root'
)
fileNamesZtoElecTau_Zee_10TeV_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_52.root'
)
fileNamesZtoElecTau_Zee_10TeV_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_53.root'
)
fileNamesZtoElecTau_Zee_10TeV_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_54.root'
)
fileNamesZtoElecTau_Zee_10TeV_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_55.root'
)
fileNamesZtoElecTau_Zee_10TeV_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_56.root'
)
fileNamesZtoElecTau_Zee_10TeV_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_57.root'
)
fileNamesZtoElecTau_Zee_10TeV_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_58.root'
)
fileNamesZtoElecTau_Zee_10TeV_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_59.root'
)
fileNamesZtoElecTau_Zee_10TeV_part57 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_10TeV/skimElecTau_60.root'
)
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# EM enriched QCD sample generated with Pythia (20 GeV < Pt(hat) < 30 GeV)
#
#  10 TeV
#  /QCD_EMenriched_Pt20to30/Summer09-MC_31X_V3-v1/GEN-SIM-RECO
#  generated events = 33709623
#  events passing skim = 2973018
#  integrated lumi = 10.534 pb^-1
intLumiZtoElecTau_QCD_EMenriched_Pt20to30_10TeV = float(10.534)
#  missing 36/700 skim output files
corrFactorZtoElecTau_QCD_EMenriched_Pt20to30_10TeV = float(1.054)

patTupleOutputFileNameZtoElecTau_QCD_EMenriched_Pt20to30_10TeV = cms.untracked.string('patTupleZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_EMenriched_Pt20to30_10TeV = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_EMenriched_Pt20to30_10TeV = cms.PSet(
  pluginName = cms.string('genPhaseSpaceCut'),
  pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_01.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_02.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_03.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_04.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_05.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_06.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_07.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_08.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_09.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_10.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_11.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_12.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_13.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_14.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_15.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_16.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part17 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_17.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part18 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_18.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part19 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_19.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part20 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_20.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part21 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_21.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part22 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_22.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part23 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_23.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part24 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_24.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part25 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_25.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part26 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_26.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part27 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_27.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part28 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_28.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part29 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_29.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part30 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_30.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part31 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_31.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part32 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_32.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part33 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_33.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part34 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_34.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part35 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_35.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part36 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_36.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part37 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_37.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part38 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_38.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part39 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_39.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part40 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_40.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part41 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_41.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part42 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_42.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part43 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_43.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part44 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_44.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part45 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_45.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part46 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_46.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part47 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_47.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part48 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_48.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part49 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_49.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part50 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_50.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part51 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_51.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part52 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_52.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part53 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_53.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part54 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_54.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part55 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_55.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part56 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_56.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part57 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_57.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part58 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_58.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part59 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_59.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part60 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_60.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part61 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_61.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part62 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_62.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part63 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_63.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part64 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_64.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part65 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_65.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part66 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_66.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part67 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_67.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part68 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_68.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part69 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_69.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part70 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_70.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part71 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_71.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part72 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_72.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part73 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_73.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part74 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_74.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part75 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_75.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part76 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_76.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part77 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_77.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part78 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_78.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part79 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_79.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part80 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_80.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part81 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_81.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part82 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_82.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part83 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_83.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part84 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_84.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part85 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_85.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part86 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_86.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part87 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_87.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part88 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_88.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part89 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_89.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part90 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_90.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part91 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_91.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part92 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_92.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part93 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_93.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part94 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_94.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part95 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_95.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part96 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_96.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part97 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_97.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part98 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_98.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part99 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_99.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part100 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_100.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part101 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_101.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part102 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_102.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part103 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_103.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part104 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_104.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part105 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_105.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part106 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_106.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part107 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_107.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part108 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_108.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part109 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_109.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part110 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_110.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part111 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_111.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part112 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_112.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part113 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_113.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part114 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_114.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part115 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_115.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part116 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_116.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part117 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_117.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part118 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_118.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part119 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_119.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part120 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_120.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part121 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_121.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part122 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_122.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part123 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_123.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part124 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_124.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part125 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_125.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part126 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_126.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part127 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_127.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part128 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_128.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part129 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_129.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part130 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_130.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part131 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_131.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part132 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_132.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part133 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_133.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part134 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_134.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part135 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_135.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part136 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_136.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part137 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_137.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part138 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_138.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part139 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_139.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part140 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_140.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part141 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_141.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part142 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_142.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part143 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_143.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part144 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_144.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part145 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_145.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part146 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_146.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part147 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_147.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part148 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_148.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part149 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_149.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part150 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_150.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part151 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_151.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part152 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_152.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part153 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_153.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part154 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_154.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part155 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_155.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part156 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_156.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part157 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_157.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part158 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_158.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part159 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_159.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part160 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_160.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part161 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_161.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part162 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_162.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part163 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_163.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part164 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_164.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part165 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_165.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part166 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_166.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part167 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_167.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part168 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_168.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part169 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_169.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part170 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_170.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part171 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_171.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part172 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_172.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part173 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_173.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part174 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_174.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part175 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_10TeV/skimElecTau_QCD_EMenriched_Pt20to30_10TeV_175.root'
)


#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# EM enriched QCD sample generated with Pythia (30 GeV < Pt(hat) < 80 GeV)
#
#  10 TeV
#  /QCD_EMenriched_Pt30to80/Summer09-MC_31X_V3-v1/GEN-SIM-RECO
#  generated events = 39998653
#  events passing skim = 6317642
#  integrated lumi = 8.510 pb^-1
intLumiZtoElecTau_QCD_EMenriched_Pt30to80_10TeV = float(8.510)
#
# 125/1014 skim output files missing
corrFactorZtoElecTau_QCD_EMenriched_Pt30to80_10TeV = float(1.141)

patTupleOutputFileNameZtoElecTau_QCD_EMenriched_Pt30to80_10TeV = cms.untracked.string('patTupleZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_EMenriched_Pt30to80_10TeV = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_EMenriched_Pt30to80_10TeV = cms.PSet(
  pluginName = cms.string('genPhaseSpaceCut'),
  pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_01.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_02.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_03.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_04.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_05.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_06.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_07.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_08.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_09.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_10.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_11.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_12.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_13.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_14.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_15.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_16.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_17.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_18.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_19.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_20.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_21.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_22.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_23.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_24.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_25.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_26.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_27.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_28.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_29.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_30.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_31.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_32.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_33.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_34.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_35.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_36.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_37.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_38.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_39.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_40.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_41.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_42.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_43.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_44.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_45.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_46.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_47.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_48.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_49.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_50.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_51.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_52.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_53.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_54.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_55.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_56.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part57 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_57.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part58 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_58.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part59 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_59.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part60 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_60.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part61 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_61.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part62 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_62.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part63 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_63.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part64 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_64.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part65 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_65.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part66 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_66.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part67 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_67.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part68 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_68.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part69 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_69.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part70 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_70.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part71 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_71.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part72 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_72.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part73 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_73.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part74 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_74.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part75 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_75.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part76 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_76.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part77 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_77.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part78 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_78.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part79 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_79.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part80 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_80.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part81 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_81.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part82 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_82.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part83 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_83.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part84 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_84.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part85 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_85.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part86 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_86.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part87 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_87.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part88 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_88.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part89 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_89.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part90 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_90.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part91 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_91.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part92 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_92.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part93 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_93.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part94 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_94.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part95 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_95.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part96 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_96.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part97 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_97.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part98 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_98.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part99 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_99.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part100 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_100.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part101 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_101.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part102 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_102.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part103 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_103.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part104 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_104.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part105 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_105.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part106 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_106.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part107 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_107.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part108 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_108.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part109 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_109.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part110 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_110.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part111 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_111.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part112 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_112.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part113 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_113.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part114 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_114.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part115 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_115.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part116 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_116.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part117 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_117.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part118 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_118.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part119 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_119.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part120 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_120.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part121 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_121.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part122 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_122.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part123 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_123.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part124 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_124.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part125 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_125.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part126 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_126.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part127 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_127.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part128 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_128.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part129 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_129.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part130 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_130.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part131 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_131.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part132 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_132.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part133 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_133.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part134 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_134.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part135 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_135.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part136 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_136.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part137 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_137.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part138 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_138.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part139 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_139.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part140 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_140.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part141 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_141.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part142 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_142.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part143 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_143.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part144 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_144.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part145 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_145.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part146 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_146.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part147 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_147.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part148 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_148.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part149 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_149.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part150 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_150.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part151 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_151.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part152 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_152.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part153 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_153.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part154 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_154.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part155 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_155.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part156 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_156.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part157 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_157.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part158 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_158.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part159 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_159.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part160 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_160.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part161 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_161.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part162 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_162.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part163 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_163.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part164 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_164.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part165 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_165.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part166 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_166.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part167 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_167.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part168 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_168.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part169 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_169.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part170 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_170.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part171 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_171.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part172 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_172.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part173 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_173.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part174 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_174.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part175 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_175.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part176 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_176.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part177 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_177.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part178 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_178.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part179 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_179.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part180 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_180.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part181 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_181.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part182 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_182.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part183 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_183.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part184 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_184.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part185 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_185.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part186 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_186.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part187 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_187.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part188 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_188.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part189 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_189.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part190 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_190.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part191 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_191.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part192 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_192.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part193 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_193.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part194 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_194.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part195 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_195.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part196 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_196.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part197 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_197.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part198 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_198.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part199 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_199.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part200 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_200.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part201 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_201.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part202 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_202.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part203 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_203.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part204 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_204.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part205 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_205.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part206 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_206.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part207 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_207.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part208 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_208.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part209 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_209.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part210 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_210.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part211 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_211.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part212 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_212.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part213 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_213.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part214 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_214.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part215 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_215.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part216 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_216.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part217 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_217.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part218 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_218.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part219 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_219.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part220 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_220.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part221 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_221.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part222 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_222.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part223 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_223.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part224 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_224.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part225 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_225.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part226 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_226.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part227 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_227.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part228 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_228.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part229 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_229.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part230 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_230.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part231 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_231.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part232 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_232.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part233 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_233.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part234 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_234.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part235 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_235.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part236 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_236.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part237 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_237.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part238 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_238.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part239 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_239.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part240 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_240.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part241 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_241.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part242 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_242.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part243 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_243.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part244 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_244.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part245 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_245.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part246 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_246.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part247 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_247.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part248 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_248.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part249 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_249.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part250 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_250.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part251 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_251.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part252 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_252.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part253 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_253.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part254 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_10TeV/skimElecTau_QCD_EMenriched_Pt30to80_10TeV_254.root'
)

#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# EM enriched QCD sample generated with Pythia (80 GeV < Pt(hat) < 170 GeV)
#
#  10TeV
#  /QCD_EMenriched_Pt80to170/Summer09-MC_31X_V3-v1/GEN-SIM-RECO
#  generated events = 5735561
#  events passing skim = 1929570
#  cross section = 0.0019mb
#  filter eff = 0.15
#  integrated lumi = 20.125
intLumiZtoElecTau_QCD_EMenriched_Pt80to170_10TeV = float(20.125)
#
# 1/122 files missing
corrFactorZtoElecTau_QCD_EMenriched_Pt80to170_10TeV = float(1.008)

patTupleOutputFileNameZtoElecTau_QCD_EMenriched_Pt80to170_10TeV = cms.untracked.string('patTupleZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_EMenriched_Pt80to170_10TeV = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_EMenriched_Pt80to170_10TeV = cms.PSet(
  pluginName = cms.string('genPhaseSpaceCut'),
  pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_1.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_2.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_3.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_4.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_5.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_6.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_7.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_8.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_9.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_10.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_11.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_12.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_13.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_14.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_15.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_16.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_17.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_18.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_19.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_20.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_21.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_22.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_23.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_24.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_25.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_26.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_27.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_28.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_29.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_30.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_31.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_32.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_33.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_34.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_35.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_36.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_37.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_38.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_39.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_40.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_41.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_42.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_43.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_44.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_45.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_46.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_47.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_48.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_49.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_50.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_51.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_52.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_53.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_54.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_55.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_56.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part57 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_57.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part58 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_58.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part59 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_59.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part60 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_60.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part61 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_61.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part62 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_62.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part63 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_63.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part64 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_64.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part65 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_65.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part66 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_66.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part67 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_67.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part68 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_68.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part69 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_69.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part70 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_70.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part71 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_71.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part72 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_72.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part73 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_73.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part74 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_74.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part75 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_75.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part76 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_76.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part77 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_77.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part78 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_78.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part79 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_79.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part80 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_80.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part81 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_81.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part82 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_82.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part83 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_83.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part84 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_84.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part85 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_85.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part86 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_86.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part87 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_87.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part88 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_88.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part89 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_89.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part90 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_91.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part91 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_92.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part92 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_93.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part93 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_94.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part94 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_95.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part95 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_96.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part96 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_97.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part97 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_98.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part98 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_99.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part99 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_100.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part100 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_101.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part101 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_102.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part102 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_103.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part103 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_104.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part104 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_105.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part105 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_106.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part106 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_107.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part107 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_108.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part108 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_109.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part109 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_110.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part110 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_111.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part111 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_112.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part112 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_113.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part113 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_114.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part114 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_115.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part115 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_116.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part116 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_117.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part117 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_118.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part118 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_119.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part119 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_120.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part120 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_121.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part121 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_10TeV/skimElecTau_122.root'
)

#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# QCD b/c --> e sample generated with Pythia (20 GeV < Pt(hat) < 30 GeV)
#
#  10 TeV
#  /QCD_BCtoE_Pt20to30/Summer09-MC_31X_V3-v1/GEN-SIM-RECO
#  generated events = 2383833
#  events passing skim = 614403
#  cross section = 0.04mb
#  filter eff = 0.00048
#  integrated luminosity = 12.416 pb^-1
# 
intLumiZtoElecTau_QCD_BCtoE_Pt20to30_10TeV = float(12.416)
# 5/58 files missing
corrFactorZtoElecTau_QCD_BCtoE_Pt20to30_10TeV = float(1.094)

patTupleOutputFileNameZtoElecTau_QCD_BCtoE_Pt20to30_10TeV = cms.untracked.string('patTupleZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_BCtoE_Pt20to30_10TeV = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_BCtoE_Pt20to30_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_1.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_2.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_3.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_4.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_5.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_6.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_7.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_8.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_10.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_11.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_12.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_13.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_14.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_16.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_17.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_18.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part17 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_19.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part18 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_20.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part19 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_21.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part20 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_22.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part21 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_23.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part22 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_24.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part23 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_25.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part24 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_26.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part25 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_27.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part26 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_28.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part27 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_29.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part28 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_30.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part29 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_31.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part30 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_32.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part31 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_33.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part32 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_34.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part33 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_35.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part34 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_36.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part35 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_37.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part36 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_38.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part37 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_39.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part38 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_40.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part39 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_41.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part40 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_42.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part41 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_43.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part42 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_46.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part43 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_47.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part44 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_49.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part45 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_50.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part46 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_51.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part47 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_52.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part48 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_53.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part49 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_54.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part50 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_55.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part51 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_56.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part52 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_57.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part53 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_10TeV/skimElecTau_58.root'
)
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# QCD b/c -->e sample generated with Pythia (30 GeV < Pt(hat) < 80 GeV)
#
#  10 TeV
#  /QCD_BCtoE_Pt30to80/Summer09-MC_31X_V3-v1/GEN-SIM-RECO
#  generated events = 2035108
#  events passing skim = 1073352
#  cross section = 0.10mb
#  filter eff = 0.0024
#  integrated luminosity = 8.48 pb^-1
intLumiZtoElecTau_QCD_BCtoE_Pt30to80_10TeV = float(8.48)
#
# 1/69 files missing
corrFactorZtoElecTau_QCD_BCtoE_Pt30to80_10TeV = float(1.015)

patTupleOutputFileNameZtoElecTau_QCD_BCtoE_Pt30to80_10TeV = cms.untracked.string('patTupleZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_BCtoE_Pt30to80_10TeV = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_BCtoE_Pt30to80_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_1.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_2.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_3.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_4.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_5.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_6.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_7.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_8.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_9.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_10.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_11.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_12.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_13.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_14.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_15.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_16.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_17.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_18.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_19.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_20.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_21.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_22.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_23.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_24.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_25.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_26.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_27.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_28.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_29.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_30.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_31.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_32.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_33.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_34.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_35.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_36.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_37.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_38.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_39.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_40.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_41.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_42.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_43.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_44.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_45.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_46.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_47.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_48.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_49.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_50.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_51.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_52.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_53.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_54.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_55.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_56.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part57 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_58.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part58 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_59.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part59 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_60.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part60 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_61.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part61 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_62.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part62 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_63.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part63 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_64.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part64 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_65.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part65 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_66.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part66 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_67.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part67 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_68.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part68 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_10TeV/skimElecTau_69.root'
)

#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
#  QCD b/c -->e sample generated with Pythia (80 GeV < Pt(hat) < 170 GeV)
#
#  10 TeV
#  dataset = /QCD_BCtoE_Pt80to170/Summer09-MC_31X_V3-v1/GEN-SIM-RECO
#  generated events = 5735561
#  events passing skim = 772812
#  cross section = 0.0019mb
#  filter eff = 0.012
#  integrated luminosity = 25.16 pb^-1
intLumiZtoElecTau_QCD_BCtoE_Pt80to170_10TeV = float(25.16)
# 
corrFactorZtoElecTau_QCD_BCtoE_Pt80to170_10TeV = float(1.0)

patTupleOutputFileNameZtoElecTau_QCD_BCtoE_Pt80to170_10TeV = cms.untracked.string('patTupleZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_BCtoE_Pt80to170_10TeV = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_BCtoE_Pt80to170_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_1.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_2.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_3.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_4.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_5.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_6.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_7.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_8.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_9.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_10.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_11.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_12.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_13.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_14.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_15.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_16.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_17.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_18.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_19.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_20.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_21.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_22.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_23.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_24.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_25.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_26.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_27.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_28.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_29.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_30.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_31.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_32.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_33.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_34.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_35.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_36.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_37.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_38.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_39.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_40.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_41.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_42.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_43.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_44.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_10TeV/skimElecTau_45.root'
)


#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# photon + jets samples (PYTHIA)
#
# datasets = /PhotonJet_PtXXtoYY/Summer09-MC_31X_V3-v1/GEN-SIM-RECO
#
fileNamesZtoElecTau_PhotonPlusJets_Pt15to20_10TeV = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/PhotonPlusJets_Pt15to20_10TeV/skimElecTau_PhotonPlusJets_Pt15to20_10TeV_01.root'
)
fileNamesZtoElecTau_PhotonPlusJets_Pt20to30_10TeV = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/PhotonPlusJets_Pt20to30_10TeV/skimElecTau_PhotonPlusJets_Pt20to30_10TeV_01.root'
)
fileNamesZtoElecTau_PhotonPlusJets_Pt30to50_10TeV = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/PhotonPlusJets_Pt30to50_10TeV/skimElecTau_PhotonPlusJets_Pt30to50_10TeV_01.root'
)

corrFactorZtoElecTau_PhotonPlusJets_Pt15to20_10TeV = float(1.0)
corrFactorZtoElecTau_PhotonPlusJets_Pt20to30_10TeV = float(1.0)
corrFactorZtoElecTau_PhotonPlusJets_Pt30to50_10TeV = float(1.0)

intLumiZtoElecTau_PhotonPlusJets_Pt15to20_10TeV = float(0.6398)
intLumiZtoElecTau_PhotonPlusJets_Pt20to30_10TeV = float(1.251)
intLumiZtoElecTau_PhotonPlusJets_Pt30to50_10TeV = float(4.555)

genPhaseSpaceCutZtoElecTau_PhotonPlusJets_Pt15to20_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)
genPhaseSpaceCutZtoElecTau_PhotonPlusJets_Pt20to30_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)
genPhaseSpaceCutZtoElecTau_PhotonPlusJets_Pt30to50_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)
plotsOutputFileNameZtoElecTau_PhotonPlusJets_Pt15to20_10TeV = cms.string('plotsZtoElecTau_PhotonPlusJets_Pt15to20_10TeV.root')
plotsOutputFileNameZtoElecTau_PhotonPlusJets_Pt20to30_10TeV = cms.string('plotsZtoElecTau_PhotonPlusJets_Pt20to30_10TeV.root')
plotsOutputFileNameZtoElecTau_PhotonPlusJets_Pt30to50_10TeV = cms.string('plotsZtoElecTau_PhotonPlusJets_Pt30to50_10TeV.root')

patTupleOutputFileNameZtoElecTau_PhotonPlusJets_Pt15to20_10TeV = cms.untracked.string('patTupleZtoElecTau_PhotonPlusJets_Pt15to20_10TeV.root')
patTupleOutputFileNameZtoElecTau_PhotonPlusJets_Pt20to30_10TeV = cms.untracked.string('patTupleZtoElecTau_PhotonPlusJets_Pt20to30_10TeV.root')
patTupleOutputFileNameZtoElecTau_PhotonPlusJets_Pt30to50_10TeV = cms.untracked.string('patTupleZtoElecTau_PhotonPlusJets_Pt30to50_10TeV.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
#  W + jets sample generated with Madgraph
#  dataset = /WJets-madgraph/Summer09-MC_31X_V3-v1/GEN-SIM-RECO	
#  generated events = 11024540
#  events passing skim = 788334
#  cross section = 30nb 
#  int lumi = 368 pb^-1
# 
# (NOTE: for Monte Carlo samples generated by Madgraph,
#        the filter efficiency is already included in the cross-sections
#        listed at https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2008 !!)
#
intLumiZtoElecTau_WplusJets_10TeV = float(367.49)
#  2/557 skim output files missing
corrFactorZtoElecTau_WplusJets_10TeV = float(1.004)

fileNamesZtoElecTau_WplusJets_10TeV_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_01.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_02.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_03.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_04.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_05.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_06.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_07.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_08.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_09.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_10.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_11.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_12.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_13.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_14.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_15.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_16.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_17.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_18.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_19.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_20.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_21.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_22.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_23.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_24.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_25.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_26.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_27.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_28.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_29.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_30.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_31.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_32.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_33.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_34.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_35.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_36.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_37.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_38.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_39.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_40.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_41.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_42.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_43.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_44.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_45.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_46.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_47.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_48.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_49.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_50.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_51.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_52.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_53.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_54.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_55.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_56.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part57 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_57.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part58 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_58.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part59 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_59.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part60 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_60.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part61 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_61.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part62 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_62.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part63 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_63.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part64 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_64.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part65 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_65.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part66 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_66.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part67 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_67.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part68 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_68.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part69 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_69.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part70 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_70.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part71 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_71.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part72 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_72.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part73 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_73.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part74 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_74.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part75 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_75.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part76 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_76.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part77 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_77.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part78 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_78.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part79 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_79.root'
)
fileNamesZtoElecTau_WplusJets_10TeV_part80 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_10TeV/skimElecTau_WJets_10TeV_80.root'
)

patTupleOutputFileNameZtoElecTau_WplusJets_10TeV = cms.untracked.string('patTupleZtoElecTau_WplusJets_10TeV_partXX.root')

plotsOutputFileNameZtoElecTau_WplusJets_10TeV = cms.string('plotsZtoElecTau_WplusJets_10TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_WplusJets_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
#  Z + jets sample generated with Madgraph (excluding Z --> tau+ tau- decays)
#
#  dataset = /ZJets-madgraph/Summer09-MC_31X_V3-v1/GEN-SIM-RECO
#  generated events = 932532
#  events passing skim = 255827
#  cross section = 3.6nb
#  integrated luminosity = 259.04 pb^-1
#
# (NOTE: for Monte Carlo samples generated by Madgraph,
#        the filter efficiency is already included in the cross-sections
#        listed at https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2008 !!)
#
intLumiZtoElecTau_ZJets_10TeV = float(259.04)
corrFactorZtoElecTau_ZJets_10TeV = float(1.0)

fileNamesZtoElecTau_ZJets_10TeV_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_1.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_2.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_3.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_4.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_5.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_6.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_7.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_8.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_9.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_10.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_11.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_12.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_13.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_14.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_15.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_16.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_17.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_18.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_19.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_20.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_21.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_22.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_23.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_24.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_25.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_26.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_27.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_28.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_29.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_30.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_31.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_32.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_33.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_34.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_35.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_36.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_37.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_38.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_39.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_40.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_41.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_42.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_43.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_44.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_45.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_46.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_47.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_48.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_49.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_50.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_51.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_52.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_53.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_54.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_55.root'
)
fileNamesZtoElecTau_ZJets_10TeV_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_10TeV/skimElecTau_56.root'
)

corrFactorZtoElecTau_ZeePlusJets_10TeV = corrFactorZtoElecTau_ZJets_10TeV
intLumiZtoElecTau_ZeePlusJets_10TeV = intLumiZtoElecTau_ZJets_10TeV

fileNamesZtoElecTau_ZeePlusJets_10TeV_part01 = fileNamesZtoElecTau_ZJets_10TeV_part01
fileNamesZtoElecTau_ZeePlusJets_10TeV_part02 = fileNamesZtoElecTau_ZJets_10TeV_part02
fileNamesZtoElecTau_ZeePlusJets_10TeV_part03 = fileNamesZtoElecTau_ZJets_10TeV_part03
fileNamesZtoElecTau_ZeePlusJets_10TeV_part04 = fileNamesZtoElecTau_ZJets_10TeV_part04
fileNamesZtoElecTau_ZeePlusJets_10TeV_part05 = fileNamesZtoElecTau_ZJets_10TeV_part05
fileNamesZtoElecTau_ZeePlusJets_10TeV_part06 = fileNamesZtoElecTau_ZJets_10TeV_part06
fileNamesZtoElecTau_ZeePlusJets_10TeV_part07 = fileNamesZtoElecTau_ZJets_10TeV_part07
fileNamesZtoElecTau_ZeePlusJets_10TeV_part08 = fileNamesZtoElecTau_ZJets_10TeV_part08
fileNamesZtoElecTau_ZeePlusJets_10TeV_part09 = fileNamesZtoElecTau_ZJets_10TeV_part09
fileNamesZtoElecTau_ZeePlusJets_10TeV_part10 = fileNamesZtoElecTau_ZJets_10TeV_part10
fileNamesZtoElecTau_ZeePlusJets_10TeV_part11 = fileNamesZtoElecTau_ZJets_10TeV_part11
fileNamesZtoElecTau_ZeePlusJets_10TeV_part12 = fileNamesZtoElecTau_ZJets_10TeV_part12
fileNamesZtoElecTau_ZeePlusJets_10TeV_part13 = fileNamesZtoElecTau_ZJets_10TeV_part13
fileNamesZtoElecTau_ZeePlusJets_10TeV_part14 = fileNamesZtoElecTau_ZJets_10TeV_part14
fileNamesZtoElecTau_ZeePlusJets_10TeV_part15 = fileNamesZtoElecTau_ZJets_10TeV_part15
fileNamesZtoElecTau_ZeePlusJets_10TeV_part16 = fileNamesZtoElecTau_ZJets_10TeV_part16
fileNamesZtoElecTau_ZeePlusJets_10TeV_part17 = fileNamesZtoElecTau_ZJets_10TeV_part17
fileNamesZtoElecTau_ZeePlusJets_10TeV_part18 = fileNamesZtoElecTau_ZJets_10TeV_part18
fileNamesZtoElecTau_ZeePlusJets_10TeV_part19 = fileNamesZtoElecTau_ZJets_10TeV_part19
fileNamesZtoElecTau_ZeePlusJets_10TeV_part20 = fileNamesZtoElecTau_ZJets_10TeV_part20
fileNamesZtoElecTau_ZeePlusJets_10TeV_part21 = fileNamesZtoElecTau_ZJets_10TeV_part21
fileNamesZtoElecTau_ZeePlusJets_10TeV_part22 = fileNamesZtoElecTau_ZJets_10TeV_part22
fileNamesZtoElecTau_ZeePlusJets_10TeV_part23 = fileNamesZtoElecTau_ZJets_10TeV_part23
fileNamesZtoElecTau_ZeePlusJets_10TeV_part24 = fileNamesZtoElecTau_ZJets_10TeV_part24
fileNamesZtoElecTau_ZeePlusJets_10TeV_part25 = fileNamesZtoElecTau_ZJets_10TeV_part25
fileNamesZtoElecTau_ZeePlusJets_10TeV_part26 = fileNamesZtoElecTau_ZJets_10TeV_part26
fileNamesZtoElecTau_ZeePlusJets_10TeV_part27 = fileNamesZtoElecTau_ZJets_10TeV_part27
fileNamesZtoElecTau_ZeePlusJets_10TeV_part28 = fileNamesZtoElecTau_ZJets_10TeV_part28
fileNamesZtoElecTau_ZeePlusJets_10TeV_part29 = fileNamesZtoElecTau_ZJets_10TeV_part29
fileNamesZtoElecTau_ZeePlusJets_10TeV_part30 = fileNamesZtoElecTau_ZJets_10TeV_part30
fileNamesZtoElecTau_ZeePlusJets_10TeV_part31 = fileNamesZtoElecTau_ZJets_10TeV_part31
fileNamesZtoElecTau_ZeePlusJets_10TeV_part32 = fileNamesZtoElecTau_ZJets_10TeV_part32
fileNamesZtoElecTau_ZeePlusJets_10TeV_part33 = fileNamesZtoElecTau_ZJets_10TeV_part33
fileNamesZtoElecTau_ZeePlusJets_10TeV_part34 = fileNamesZtoElecTau_ZJets_10TeV_part34
fileNamesZtoElecTau_ZeePlusJets_10TeV_part35 = fileNamesZtoElecTau_ZJets_10TeV_part35
fileNamesZtoElecTau_ZeePlusJets_10TeV_part36 = fileNamesZtoElecTau_ZJets_10TeV_part36
fileNamesZtoElecTau_ZeePlusJets_10TeV_part37 = fileNamesZtoElecTau_ZJets_10TeV_part37
fileNamesZtoElecTau_ZeePlusJets_10TeV_part38 = fileNamesZtoElecTau_ZJets_10TeV_part38
fileNamesZtoElecTau_ZeePlusJets_10TeV_part39 = fileNamesZtoElecTau_ZJets_10TeV_part39
fileNamesZtoElecTau_ZeePlusJets_10TeV_part40 = fileNamesZtoElecTau_ZJets_10TeV_part40
fileNamesZtoElecTau_ZeePlusJets_10TeV_part41 = fileNamesZtoElecTau_ZJets_10TeV_part41
fileNamesZtoElecTau_ZeePlusJets_10TeV_part42 = fileNamesZtoElecTau_ZJets_10TeV_part42
fileNamesZtoElecTau_ZeePlusJets_10TeV_part43 = fileNamesZtoElecTau_ZJets_10TeV_part43
fileNamesZtoElecTau_ZeePlusJets_10TeV_part44 = fileNamesZtoElecTau_ZJets_10TeV_part44
fileNamesZtoElecTau_ZeePlusJets_10TeV_part45 = fileNamesZtoElecTau_ZJets_10TeV_part45
fileNamesZtoElecTau_ZeePlusJets_10TeV_part46 = fileNamesZtoElecTau_ZJets_10TeV_part46
fileNamesZtoElecTau_ZeePlusJets_10TeV_part47 = fileNamesZtoElecTau_ZJets_10TeV_part47
fileNamesZtoElecTau_ZeePlusJets_10TeV_part48 = fileNamesZtoElecTau_ZJets_10TeV_part48
fileNamesZtoElecTau_ZeePlusJets_10TeV_part49 = fileNamesZtoElecTau_ZJets_10TeV_part49
fileNamesZtoElecTau_ZeePlusJets_10TeV_part50 = fileNamesZtoElecTau_ZJets_10TeV_part50
fileNamesZtoElecTau_ZeePlusJets_10TeV_part51 = fileNamesZtoElecTau_ZJets_10TeV_part51
fileNamesZtoElecTau_ZeePlusJets_10TeV_part52 = fileNamesZtoElecTau_ZJets_10TeV_part52
fileNamesZtoElecTau_ZeePlusJets_10TeV_part53 = fileNamesZtoElecTau_ZJets_10TeV_part53
fileNamesZtoElecTau_ZeePlusJets_10TeV_part54 = fileNamesZtoElecTau_ZJets_10TeV_part54
fileNamesZtoElecTau_ZeePlusJets_10TeV_part55 = fileNamesZtoElecTau_ZJets_10TeV_part55
fileNamesZtoElecTau_ZeePlusJets_10TeV_part56 = fileNamesZtoElecTau_ZJets_10TeV_part56

patTupleOutputFileNameZtoElecTau_ZeePlusJets_10TeV = cms.untracked.string('patTupleZtoElecTau_ZeePlusJets_10TeV_partXX.root')

plotsOutputFileNameZtoElecTau_ZeePlusJets_10TeV = cms.string('plotsZtoElecTau_ZeePlusJets_10TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_ZeePlusJets_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genElectronsFromZs'),
    minNumber = cms.uint32(2)
)

corrFactorZtoElecTau_ZtautauPlusJets_10TeV = corrFactorZtoElecTau_ZJets_10TeV
intLumiZtoElecTau_ZtautauPlusJets_10TeV = intLumiZtoElecTau_ZJets_10TeV

fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part01 = fileNamesZtoElecTau_ZJets_10TeV_part01
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part02 = fileNamesZtoElecTau_ZJets_10TeV_part02
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part03 = fileNamesZtoElecTau_ZJets_10TeV_part03
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part04 = fileNamesZtoElecTau_ZJets_10TeV_part04
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part05 = fileNamesZtoElecTau_ZJets_10TeV_part05
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part06 = fileNamesZtoElecTau_ZJets_10TeV_part06
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part07 = fileNamesZtoElecTau_ZJets_10TeV_part07
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part08 = fileNamesZtoElecTau_ZJets_10TeV_part08
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part09 = fileNamesZtoElecTau_ZJets_10TeV_part09
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part10 = fileNamesZtoElecTau_ZJets_10TeV_part10
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part11 = fileNamesZtoElecTau_ZJets_10TeV_part11
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part12 = fileNamesZtoElecTau_ZJets_10TeV_part12
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part13 = fileNamesZtoElecTau_ZJets_10TeV_part13
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part14 = fileNamesZtoElecTau_ZJets_10TeV_part14
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part15 = fileNamesZtoElecTau_ZJets_10TeV_part15
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part16 = fileNamesZtoElecTau_ZJets_10TeV_part16
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part17 = fileNamesZtoElecTau_ZJets_10TeV_part17
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part18 = fileNamesZtoElecTau_ZJets_10TeV_part18
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part19 = fileNamesZtoElecTau_ZJets_10TeV_part19
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part20 = fileNamesZtoElecTau_ZJets_10TeV_part20
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part21 = fileNamesZtoElecTau_ZJets_10TeV_part21
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part22 = fileNamesZtoElecTau_ZJets_10TeV_part22
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part23 = fileNamesZtoElecTau_ZJets_10TeV_part23
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part24 = fileNamesZtoElecTau_ZJets_10TeV_part24
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part25 = fileNamesZtoElecTau_ZJets_10TeV_part25
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part26 = fileNamesZtoElecTau_ZJets_10TeV_part26
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part27 = fileNamesZtoElecTau_ZJets_10TeV_part27
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part28 = fileNamesZtoElecTau_ZJets_10TeV_part28
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part29 = fileNamesZtoElecTau_ZJets_10TeV_part29
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part30 = fileNamesZtoElecTau_ZJets_10TeV_part30
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part31 = fileNamesZtoElecTau_ZJets_10TeV_part31
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part32 = fileNamesZtoElecTau_ZJets_10TeV_part32
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part33 = fileNamesZtoElecTau_ZJets_10TeV_part33
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part34 = fileNamesZtoElecTau_ZJets_10TeV_part34
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part35 = fileNamesZtoElecTau_ZJets_10TeV_part35
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part36 = fileNamesZtoElecTau_ZJets_10TeV_part36
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part37 = fileNamesZtoElecTau_ZJets_10TeV_part37
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part38 = fileNamesZtoElecTau_ZJets_10TeV_part38
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part39 = fileNamesZtoElecTau_ZJets_10TeV_part39
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part40 = fileNamesZtoElecTau_ZJets_10TeV_part40
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part41 = fileNamesZtoElecTau_ZJets_10TeV_part41
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part42 = fileNamesZtoElecTau_ZJets_10TeV_part42
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part43 = fileNamesZtoElecTau_ZJets_10TeV_part43
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part44 = fileNamesZtoElecTau_ZJets_10TeV_part44
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part45 = fileNamesZtoElecTau_ZJets_10TeV_part45
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part46 = fileNamesZtoElecTau_ZJets_10TeV_part46
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part47 = fileNamesZtoElecTau_ZJets_10TeV_part47
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part48 = fileNamesZtoElecTau_ZJets_10TeV_part48
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part49 = fileNamesZtoElecTau_ZJets_10TeV_part49
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part50 = fileNamesZtoElecTau_ZJets_10TeV_part50
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part51 = fileNamesZtoElecTau_ZJets_10TeV_part51
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part52 = fileNamesZtoElecTau_ZJets_10TeV_part52
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part53 = fileNamesZtoElecTau_ZJets_10TeV_part53
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part54 = fileNamesZtoElecTau_ZJets_10TeV_part54
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part55 = fileNamesZtoElecTau_ZJets_10TeV_part55
fileNamesZtoElecTau_ZtautauPlusJets_10TeV_part56 = fileNamesZtoElecTau_ZJets_10TeV_part56

patTupleOutputFileNameZtoElecTau_ZtautauPlusJets_10TeV = cms.untracked.string('patTupleZtoElecTau_ZtautauPlusJets_10TeV_partXX.root')

plotsOutputFileNameZtoElecTau_ZtautauPlusJets_10TeV = cms.string('plotsZtoElecTau_ZtautauPlusJets_10TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_ZtautauPlusJets_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genTausFromZs'),
    minNumber = cms.uint32(2)
)
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# TTbar + Jets (madgraph, summer09) sample
#  
#  dataset = /TTbarJets-madgraph/Summer09-MC_31X_V3-v2/GEN-SIM-RECO	
#  generated events = 1075985
#  events passing skim = 705506
#  cross section = 292 pb
#  integrated luminosity = 3684.9 pb^-1
#
# (NOTE: for Monte Carlo samples generated by Madgraph,
#        the filter efficiency is already included in the cross-sections
#        listed at https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2008 !!)
#
intLumiZtoElecTau_TTplusJets_10TeV = float(3684.9)
corrFactorZtoElecTau_TTplusJets_10TeV = float(1.0)

genPhaseSpaceCutZtoElecTau_TTplusJets_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

patTupleOutputFileNameZtoElecTau_TTplusJets_10TeV = cms.untracked.string('patTupleZtoElecTau_TTplusJets_10TeV_partXX.root')

plotsOutputFileNameZtoElecTau_TTplusJets_10TeV = cms.string('plotsZtoElecTau_TTplusJets_10TeV_partXX.root')

fileNamesZtoElecTau_TTplusJets_10TeV_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_1.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_2.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_3.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_4.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_5.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_6.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_7.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_8.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_9.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_10.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_11.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_12.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_13.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_14.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_15.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_16.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_17.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_18.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_19.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_20.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_21.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_22.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_23.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_24.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_25.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_26.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_27.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_28.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_29.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_30.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_31.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_32.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_33.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_34.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_35.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_36.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_37.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_38.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_39.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_40.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_41.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_42.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_43.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_44.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_45.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_46.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_47.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_48.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_49.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_50.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_51.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_52.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_53.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_54.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_55.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_56.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part57 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_57.root'
)
fileNamesZtoElecTau_TTplusJets_10TeV_part58 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_10TeV/skimElecTau_58.root'
)

#--------------------------------------------------------------------------------

# 7 TeV samples

#--------------------------------------------------------------------------------
# Z --> tau+ tau- sample generated with Pythia + Tauola (all decay modes)
#
#  7 TeV
#  dataset = /Ztautau/Summer09-MC_31X_V3_7TeV-v1/GEN-SIM-RECO	
#  generated events = 2193025
#  skimmed events = 261654
#
intLumiZtoElecTau_Ztautau_7TeV = float(1686.9)
corrFactorZtoElecTau_Ztautau_7TeV = float(1.)

patTupleOutputFileNameZtoElecTau_Ztautau_7TeV = cms.untracked.string('patTupleZtoElecTau_Ztautau_7TeV_partXX.root')
plotsOutputFileNameZtoElecTau_Ztautau_7TeV = cms.string('plotsZtoElecTau_Ztautau_7TeV_partXX.root')
genPhaseSpaceCutZtoElecTau_Ztautau_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

fileNamesZtoElecTau_Ztautau_7TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_7TeV/skimElecTau_Ztautau_7TeV_01.root'
)
fileNamesZtoElecTau_Ztautau_7TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_7TeV/skimElecTau_Ztautau_7TeV_02.root'
)	
fileNamesZtoElecTau_Ztautau_7TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_7TeV/skimElecTau_Ztautau_7TeV_03.root'
)	
fileNamesZtoElecTau_Ztautau_7TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_7TeV/skimElecTau_Ztautau_7TeV_04.root'
)
fileNamesZtoElecTau_Ztautau_7TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_7TeV/skimElecTau_Ztautau_7TeV_05.root'
)
fileNamesZtoElecTau_Ztautau_7TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_7TeV/skimElecTau_Ztautau_7TeV_06.root'
)
fileNamesZtoElecTau_Ztautau_7TeV_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_7TeV/skimElecTau_Ztautau_7TeV_07.root'
)
fileNamesZtoElecTau_Ztautau_7TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_7TeV/skimElecTau_Ztautau_7TeV_08.root'
)
fileNamesZtoElecTau_Ztautau_7TeV_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_7TeV/skimElecTau_Ztautau_7TeV_09.root'
)
fileNamesZtoElecTau_Ztautau_7TeV_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_7TeV/skimElecTau_Ztautau_7TeV_10.root'
)
fileNamesZtoElecTau_Ztautau_7TeV_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_7TeV/skimElecTau_Ztautau_7TeV_11.root'
)
fileNamesZtoElecTau_Ztautau_7TeV_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_7TeV/skimElecTau_Ztautau_7TeV_12.root'
)	
fileNamesZtoElecTau_Ztautau_7TeV_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_7TeV/skimElecTau_Ztautau_7TeV_13.root'
)	
fileNamesZtoElecTau_Ztautau_7TeV_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_7TeV/skimElecTau_Ztautau_7TeV_14.root'
)
fileNamesZtoElecTau_Ztautau_7TeV_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_7TeV/skimElecTau_Ztautau_7TeV_15.root'
)
fileNamesZtoElecTau_Ztautau_7TeV_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_7TeV/skimElecTau_Ztautau_7TeV_16.root'
)
fileNamesZtoElecTau_Ztautau_7TeV_part17 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_7TeV/skimElecTau_Ztautau_7TeV_17.root'
)
fileNamesZtoElecTau_Ztautau_7TeV_part18 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_7TeV/skimElecTau_Ztautau_7TeV_18.root'
)
fileNamesZtoElecTau_Ztautau_7TeV_part19 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_7TeV/skimElecTau_Ztautau_7TeV_19.root'
)
fileNamesZtoElecTau_Ztautau_7TeV_part20 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Ztautau_7TeV/skimElecTau_Ztautau_7TeV_20.root'
)
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# Z --> e+ e- sample generated with Pythia
#
#  7 TeV
#  dataset = /Zee/Summer09-MC_31X_V3_7TeV_TrackingParticles-v1/GEN-SIM-RECO	
#  generated events = 2538855
#  skimmed events = 1204636
#
intLumiZtoElecTau_Zee_7TeV = float(1953.0)
# 1/53 skim output files missing
corrFactorZtoElecTau_Zee_7TeV = float(1.019)

patTupleOutputFileNameZtoElecTau_Zee_7TeV = cms.untracked.string('patTupleZtoElecTau_Zee_7TeV_partXX.root')

plotsOutputFileNameZtoElecTau_Zee_7TeV = cms.string('plotsZtoElecTau_Zee_7TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_Zee_7TeV = cms.PSet(
  pluginName = cms.string('genPhaseSpaceCut'),
  pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

fileNamesZtoElecTau_Zee_7TeV_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_1.root'
)
fileNamesZtoElecTau_Zee_7TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_2.root'
)
fileNamesZtoElecTau_Zee_7TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_3.root'
)
fileNamesZtoElecTau_Zee_7TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_4.root'
)
fileNamesZtoElecTau_Zee_7TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_5.root'
)
fileNamesZtoElecTau_Zee_7TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_6.root'
)
fileNamesZtoElecTau_Zee_7TeV_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_7.root'
)
fileNamesZtoElecTau_Zee_7TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_8.root'
)
fileNamesZtoElecTau_Zee_7TeV_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_9.root'
)
fileNamesZtoElecTau_Zee_7TeV_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_10.root'
)
fileNamesZtoElecTau_Zee_7TeV_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_11.root'
)
fileNamesZtoElecTau_Zee_7TeV_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_12.root'
)
fileNamesZtoElecTau_Zee_7TeV_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_13.root'
)
fileNamesZtoElecTau_Zee_7TeV_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_14.root'
)
fileNamesZtoElecTau_Zee_7TeV_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_15.root'
)
fileNamesZtoElecTau_Zee_7TeV_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_16.root'
)
fileNamesZtoElecTau_Zee_7TeV_part17 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_17.root'
)
fileNamesZtoElecTau_Zee_7TeV_part18 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_18.root'
)
fileNamesZtoElecTau_Zee_7TeV_part19 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_19.root'
)
fileNamesZtoElecTau_Zee_7TeV_part20 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_20.root'
)
fileNamesZtoElecTau_Zee_7TeV_part21 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_21.root'
)
fileNamesZtoElecTau_Zee_7TeV_part22 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_22.root'
)
fileNamesZtoElecTau_Zee_7TeV_part23 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_23.root'
)
fileNamesZtoElecTau_Zee_7TeV_part24 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_24.root'
)
fileNamesZtoElecTau_Zee_7TeV_part25 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_25.root'
)
fileNamesZtoElecTau_Zee_7TeV_part26 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_26.root'
)
fileNamesZtoElecTau_Zee_7TeV_part27 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_27.root'
)
fileNamesZtoElecTau_Zee_7TeV_part28 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_28.root'
)
fileNamesZtoElecTau_Zee_7TeV_part29 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_29.root'
)
fileNamesZtoElecTau_Zee_7TeV_part30 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_30.root'
)
fileNamesZtoElecTau_Zee_7TeV_part31 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_31.root'
)
fileNamesZtoElecTau_Zee_7TeV_part32 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_32.root'
)
fileNamesZtoElecTau_Zee_7TeV_part33 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_33.root'
)
fileNamesZtoElecTau_Zee_7TeV_part34 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_34.root'
)
fileNamesZtoElecTau_Zee_7TeV_part35 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_35.root'
)
fileNamesZtoElecTau_Zee_7TeV_part36 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_36.root'
)
fileNamesZtoElecTau_Zee_7TeV_part37 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_37.root'
)
fileNamesZtoElecTau_Zee_7TeV_part38 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_38.root'
)
fileNamesZtoElecTau_Zee_7TeV_part39 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_39.root'
)
fileNamesZtoElecTau_Zee_7TeV_part40 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_40.root'
)
fileNamesZtoElecTau_Zee_7TeV_part41 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_42.root'
)
fileNamesZtoElecTau_Zee_7TeV_part42 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_43.root'
)
fileNamesZtoElecTau_Zee_7TeV_part43 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_44.root'
)
fileNamesZtoElecTau_Zee_7TeV_part44 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_45.root'
)
fileNamesZtoElecTau_Zee_7TeV_part45 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_46.root'
)
fileNamesZtoElecTau_Zee_7TeV_part46 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_47.root'
)
fileNamesZtoElecTau_Zee_7TeV_part47 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_48.root'
)
fileNamesZtoElecTau_Zee_7TeV_part48 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_49.root'
)
fileNamesZtoElecTau_Zee_7TeV_part49 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_50.root'
)
fileNamesZtoElecTau_Zee_7TeV_part50 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_51.root'
)
fileNamesZtoElecTau_Zee_7TeV_part51 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_52.root'
)
fileNamesZtoElecTau_Zee_7TeV_part52 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/Zee_7TeV/skimElecTau_53.root'
)
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# EM enriched QCD sample generated with Pythia (20 GeV < Pt(hat) < 30 GeV)
#
#  7 TeV
#  dataset = /QCD_EMenriched_Pt20to30/Summer09-MC_31X_V3_7TeV-v1/GEN-SIM-RECO	
#  generated events = 33880096
#  events passing skim = 2973018
#  integrated lumi = 19.71 pb^-1
intLumiZtoElecTau_QCD_EMenriched_Pt20to30_7TeV = float(19.71)
#  missing 10/700 skim output files
corrFactorZtoElecTau_QCD_EMenriched_Pt20to30_7TeV = float(1.014)

patTupleOutputFileNameZtoElecTau_QCD_EMenriched_Pt20to30_7TeV = cms.untracked.string('patTupleZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_EMenriched_Pt20to30_7TeV = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_EMenriched_Pt20to30_7TeV = cms.PSet(
  pluginName = cms.string('genPhaseSpaceCut'),
  pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_01.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_02.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_03.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_04.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_05.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_06.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_07.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_08.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_09.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_10.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_11.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_12.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_13.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_14.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_15.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_16.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part17 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_17.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part18 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_18.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part19 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_19.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part20 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_20.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part21 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_21.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part22 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_22.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part23 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_23.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part24 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_24.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part25 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_25.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part26 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_26.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part27 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_27.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part28 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_28.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part29 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_29.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part30 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_30.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part31 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_31.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part32 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_32.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part33 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_33.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part34 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_34.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part35 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_35.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part36 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_36.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part37 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_37.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part38 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_38.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part39 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_39.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part40 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_40.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part41 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_41.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part42 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_42.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part43 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_43.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part44 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_44.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part45 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_45.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part46 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_46.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part47 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_47.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part48 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_48.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part49 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_49.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part50 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_50.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part51 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_51.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part52 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_52.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part53 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_53.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part54 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_54.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part55 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_55.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part56 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_56.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part57 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_57.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part58 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_58.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part59 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_59.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part60 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_60.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part61 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_61.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part62 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_62.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part63 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_63.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part64 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_64.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part65 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_65.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part66 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_66.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part67 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_67.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part68 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_68.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part69 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_69.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part70 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_70.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part71 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_71.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part72 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_72.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part73 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_73.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part74 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_74.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part75 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_75.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part76 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_76.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part77 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_77.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part78 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_78.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part79 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_79.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part80 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_80.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part81 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_81.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part82 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_82.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part83 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_83.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part84 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_84.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part85 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_85.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part86 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_86.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part87 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_87.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part88 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_88.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part89 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_89.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part90 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_90.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part91 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_91.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part92 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_92.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part93 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_93.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part94 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_94.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part95 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_95.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part96 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_96.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part97 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_97.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part98 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_98.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part99 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_99.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part100 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_100.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part101 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_101.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part102 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_102.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part103 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_103.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part104 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_104.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part105 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_105.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part106 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_106.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part107 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_107.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part108 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_108.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part109 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_109.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part110 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_110.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part111 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_111.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part112 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_112.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part113 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_113.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part114 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_114.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part115 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_115.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part116 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_116.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part117 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_117.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part118 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_118.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part119 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_119.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part120 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_120.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part121 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_121.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part122 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_122.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part123 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_123.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part124 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_124.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part125 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_125.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part126 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_126.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part127 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_127.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part128 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_128.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part129 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_129.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part130 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_130.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part131 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_131.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part132 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_132.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part133 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_133.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part134 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_134.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part135 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_135.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part136 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_136.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part137 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_137.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part138 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_138.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part139 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_139.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part140 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_140.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part141 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_141.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part142 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_142.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part143 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_143.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part144 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_144.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part145 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_145.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part146 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_146.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part147 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_147.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part148 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_149.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part149 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_150.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part150 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_151.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part151 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_152.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part152 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_153.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part153 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_154.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part154 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_155.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part155 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_156.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part156 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_157.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part157 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_158.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part158 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_159.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part159 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_160.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part160 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_161.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part161 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_162.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part162 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_163.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part163 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_164.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part164 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_165.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part165 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_166.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part166 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_167.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part167 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_168.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part168 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_169.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part169 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_170.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part170 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_171.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part171 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_172.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part172 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_173.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part173 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_174.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part174 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt20to30_7TeV/skimElecTau_QCD_EMenriched_Pt20to30_7TeV_175.root'
)

#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# EM enriched QCD sample generated with Pythia (30 GeV < Pt(hat) < 80 GeV)
#
#  7 TeV
#  dataset = /QCD_EMenriched_Pt30to80/Summer09-MC_31X_V3_7TeV-v1/GEN-SIM-RECO	
#  generated events = 42299325
#  events passing skim = 7280086
#  integrated lumi = 12.09 pb^-1
intLumiZtoElecTau_QCD_EMenriched_Pt30to80_7TeV = float(12.09)
#
# 69/858 skim output files missing
corrFactorZtoElecTau_QCD_EMenriched_Pt30to80_7TeV = float(1.088)

patTupleOutputFileNameZtoElecTau_QCD_EMenriched_Pt30to80_7TeV = cms.untracked.string('patTupleZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_EMenriched_Pt30to80_7TeV = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_EMenriched_Pt30to80_7TeV = cms.PSet(
  pluginName = cms.string('genPhaseSpaceCut'),
  pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_01.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_02.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_03.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_04.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_05.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_06.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_07.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_08.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_09.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_10.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_11.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_12.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_13.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_14.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_15.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_16.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_17.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_18.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_19.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_20.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_21.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_22.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_23.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_24.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_25.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_26.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_27.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_28.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_29.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_30.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_31.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_32.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_33.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_34.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_35.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_36.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_37.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_38.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_39.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_40.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_41.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_42.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_43.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_44.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_45.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_46.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_47.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_48.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_49.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_50.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_51.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_52.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_53.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_54.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_55.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_56.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part57 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_57.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part58 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_58.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part59 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_59.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part60 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_60.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part61 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_61.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part62 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_62.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part63 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_63.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part64 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_64.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part65 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_65.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part66 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_66.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part67 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_67.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part68 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_68.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part69 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_69.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part70 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_70.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part71 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_71.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part72 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_72.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part73 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_73.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part74 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_74.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part75 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_75.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part76 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_76.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part77 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_77.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part78 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_78.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part79 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_79.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part80 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_80.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part81 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_81.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part82 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_82.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part83 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_83.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part84 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_84.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part85 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_85.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part86 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_86.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part87 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_87.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part88 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_88.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part89 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_89.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part90 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_90.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part91 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_91.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part92 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_92.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part93 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_93.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part94 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_94.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part95 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_95.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part96 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_96.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part97 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_97.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part98 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_98.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part99 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_99.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part100 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_100.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part101 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_101.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part102 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_102.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part103 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_103.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part104 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_104.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part105 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_105.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part106 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_106.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part107 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_107.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part108 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_108.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part109 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_109.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part110 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_110.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part111 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_111.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part112 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_112.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part113 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_113.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part114 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_114.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part115 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_115.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part116 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_116.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part117 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_117.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part118 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_118.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part119 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_119.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part120 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_120.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part121 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_121.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part122 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_122.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part123 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_123.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part124 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_124.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part125 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_125.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part126 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_126.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part127 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_127.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part128 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_128.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part129 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_129.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part130 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_130.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part131 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_131.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part132 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_132.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part133 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_133.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part134 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_140.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part135 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_141.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part136 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_142.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part137 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_143.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part138 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_144.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part139 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_145.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part140 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_146.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part141 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_147.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part142 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_148.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part143 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_149.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part144 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_150.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part145 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_151.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part146 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_152.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part147 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_153.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part148 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_154.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part149 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_155.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part150 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_156.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part151 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_157.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part152 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_158.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part153 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_159.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part154 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_160.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part155 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_161.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part156 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_162.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part157 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_163.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part158 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_164.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part159 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_165.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part160 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_166.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part161 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_167.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part162 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_168.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part163 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_169.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part164 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_170.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part165 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_171.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part166 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_172.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part167 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_173.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part168 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_190.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part169 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_191.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part170 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_192.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part171 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_193.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part172 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_194.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part173 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_195.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part174 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_196.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part175 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_197.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part176 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_198.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part177 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_199.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part178 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_200.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part179 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_201.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part180 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_202.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part181 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_203.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part182 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_204.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part183 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_205.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part184 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_206.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part185 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_207.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part186 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_208.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part187 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_209.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part188 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_210.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part189 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_211.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part190 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_212.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part191 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_213.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part192 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_214.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part193 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_215.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part194 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_216.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part195 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_217.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part196 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_218.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part197 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_219.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part198 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_220.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part199 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_221.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part200 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_222.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part201 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_223.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part202 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_224.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part203 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_225.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part204 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_226.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part205 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_227.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part206 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_228.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part207 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_229.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part208 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_230.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part209 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_231.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part210 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_232.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part211 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_233.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part212 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_234.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part213 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_235.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part214 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_236.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part215 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_237.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part216 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_238.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part217 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_239.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part218 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_240.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part219 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_241.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part220 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_242.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part221 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_243.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part222 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_244.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part223 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_245.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part224 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_246.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part225 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_247.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part226 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_248.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part227 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_249.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part228 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_250.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part229 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_251.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part230 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_252.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part231 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_253.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part232 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_254.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part233 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_255.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part234 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_256.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part235 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_257.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part236 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_258.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part237 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_259.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part238 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_260.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part239 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_261.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part240 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_262.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part241 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_263.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part242 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_264.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part243 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_265.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part244 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_266.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part245 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_267.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part246 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_268.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part247 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_269.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part248 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_270.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part249 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_271.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part250 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_272.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part251 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_273.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part252 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_274.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part253 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_275.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part254 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_276.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part255 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_277.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part256 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_278.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part257 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_279.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part258 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_280.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part259 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_281.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part260 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_282.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part261 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_283.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part262 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_284.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part263 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_285.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part264 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt30to80_7TeV/skimElecTau_QCD_EMenriched_Pt30to80_7TeV_286.root'
)

#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# EM enriched QCD sample generated with Pythia (80 GeV < Pt(hat) < 170 GeV)
#
#  7TeV
#  dataset = /QCD_EMenriched_Pt80to170/Summer09-MC_31X_V3_7TeV-v1/GEN-SIM-RECO	
#  generated events = 5551386
#  events passing skim = 1915812
#  integrated lumi = 41.40
intLumiZtoElecTau_QCD_EMenriched_Pt80to170_7TeV = float(41.40)
#
# 1/158 files missing
corrFactorZtoElecTau_QCD_EMenriched_Pt80to170_7TeV = float(1.006)

patTupleOutputFileNameZtoElecTau_QCD_EMenriched_Pt80to170_7TeV = cms.untracked.string('patTupleZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_EMenriched_Pt80to170_7TeV = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_EMenriched_Pt80to170_7TeV = cms.PSet(
  pluginName = cms.string('genPhaseSpaceCut'),
  pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_1.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_2.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_3.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_4.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_5.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_6.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_7.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_8.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_9.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_10.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_11.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_12.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_13.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_14.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_15.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_16.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_17.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_18.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_19.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_20.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_21.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_22.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_23.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_24.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_25.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_26.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_27.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_28.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_29.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_30.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_31.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_32.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_33.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_34.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_35.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_36.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_37.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_38.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_39.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_40.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_41.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_42.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_43.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_44.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_45.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_46.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_47.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_48.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_49.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_50.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_51.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_52.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_53.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_54.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_55.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_56.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part57 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_57.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part58 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_58.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part59 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_59.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part60 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_60.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part61 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_61.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part62 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_62.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part63 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_63.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part64 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_64.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part65 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_65.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part66 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_66.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part67 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_67.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part68 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_68.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part69 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_69.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part70 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_70.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part71 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_71.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part72 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_72.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part73 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_73.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part74 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_74.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part75 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_75.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part76 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_76.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part77 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_77.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part78 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_78.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part79 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_79.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part80 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_80.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part81 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_82.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part82 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_83.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part83 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_84.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part84 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_85.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part85 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_86.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part86 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_87.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part87 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_88.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part88 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_89.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part89 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_90.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part90 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_91.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part91 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_92.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part92 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_93.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part93 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_94.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part94 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_95.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part95 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_96.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part96 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_97.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part97 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_98.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part98 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_99.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part99 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_100.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part100 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_101.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part101 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_102.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part102 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_103.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part103 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_104.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part104 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_105.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part105 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_106.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part106 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_107.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part107 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_108.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part108 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_109.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part109 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_110.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part110 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_111.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part111 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_112.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part112 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_113.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part113 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_114.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part114 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_115.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part115 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_116.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part116 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_117.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part117 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_118.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part118 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_119.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part119 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_120.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part120 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_121.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part121 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_122.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part122 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_123.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part123 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_124.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part124 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_125.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part125 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_126.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part126 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_127.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part127 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_128.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part128 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_129.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part129 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_130.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part130 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_131.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part131 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_132.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part132 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_133.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part133 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_134.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part134 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_135.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part135 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_136.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part136 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_137.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part137 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_138.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part138 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_139.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part139 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_140.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part140 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_141.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part141 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_142.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part142 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_143.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part143 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_144.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part144 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_145.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part145 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_146.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part146 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_147.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part147 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_148.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part148 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_149.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part149 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_150.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part150 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_151.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part151 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_152.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part152 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_153.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part153 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_154.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part154 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_155.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part155 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_156.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part156 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_157.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part157 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_EMenriched_Pt80to170_7TeV/skimElecTau_158.root'
)

#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# QCD b/c --> e sample generated with Pythia (20 GeV < Pt(hat) < 30 GeV)
#
#  7 TeV
#  dataset = /QCD_BCtoE_Pt20to30/Summer09-MC_31X_V3_7TeV-v1/GEN-SIM-RECO	
#  generated events = 2752942
#  events passing skim = 757875
#  integrated luminosity = 25.41 pb^-1
# 
intLumiZtoElecTau_QCD_BCtoE_Pt20to30_7TeV = float(25.41)
# 0/57 files missing
corrFactorZtoElecTau_QCD_BCtoE_Pt20to30_7TeV = float(1.)

patTupleOutputFileNameZtoElecTau_QCD_BCtoE_Pt20to30_7TeV = cms.untracked.string('patTupleZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_BCtoE_Pt20to30_7TeV = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_BCtoE_Pt20to30_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_1.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_2.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_3.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_4.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_5.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_6.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_7.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_8.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_9.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_10.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_11.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_12.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_13.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_14.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_15.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_16.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_17.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_18.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_19.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_20.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_21.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_22.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_23.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_24.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_25.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_26.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_27.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_28.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_29.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_30.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_31.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_32.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_33.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_34.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_35.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_36.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_37.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_38.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_39.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_40.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_41.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_42.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_43.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_44.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_45.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_46.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_47.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_48.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_49.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_50.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_51.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_52.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_53.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_54.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_55.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_56.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part57 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt20to30_7TeV/skimElecTau_57.root'
)

#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# QCD b/c -->e sample generated with Pythia (30 GeV < Pt(hat) < 80 GeV)
#
#  7 TeV
#  dataset = /QCD_BCtoE_Pt30to80/Summer09-MC_31X_V3_7TeV-v1/GEN-SIM-RECO	
#  generated events = 2447285
#  events passing skim = 1225335
#  integrated luminosity = 17.64 pb^-1
intLumiZtoElecTau_QCD_BCtoE_Pt30to80_7TeV = float(17.64)
#
# 5/64 files missing
corrFactorZtoElecTau_QCD_BCtoE_Pt30to80_7TeV = float(1.085)

patTupleOutputFileNameZtoElecTau_QCD_BCtoE_Pt30to80_7TeV = cms.untracked.string('patTupleZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_BCtoE_Pt30to80_7TeV = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_BCtoE_Pt30to80_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_1.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_3.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_4.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_5.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_6.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_7.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_8.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_9.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_10.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_11.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_12.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_13.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_14.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_16.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_17.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_18.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_19.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_20.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_21.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_22.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_23.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_24.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_25.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_26.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_27.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_28.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_29.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_30.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_31.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_32.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_34.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_35.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_36.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_37.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_38.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_39.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_40.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_42.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_43.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_44.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_45.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_46.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_47.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_48.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_49.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_50.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_51.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_52.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_53.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_54.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_55.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_56.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_57.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_58.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_59.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_61.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part57 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_62.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part58 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_63.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part59 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt30to80_7TeV/skimElecTau_64.root'
)


#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
#  QCD b/c -->e sample generated with Pythia (80 GeV < Pt(hat) < 170 GeV)
#
#  7 TeV
#  dataset = /QCD_BCtoE_Pt80to170/Summer09-MC_31X_V3_7TeV-v1/GEN-SIM-RECO	
#  generated events = 1195625
#  events passing skim = 893480
#  integrated luminosity = 126.89 pb^-1
intLumiZtoElecTau_QCD_BCtoE_Pt80to170_7TeV = float(126.89)
# 
corrFactorZtoElecTau_QCD_BCtoE_Pt80to170_7TeV = float(1.030)

patTupleOutputFileNameZtoElecTau_QCD_BCtoE_Pt80to170_7TeV = cms.untracked.string('patTupleZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_BCtoE_Pt80to170_7TeV = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_BCtoE_Pt80to170_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_1.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_2.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_3.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_4.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_5.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_6.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_7.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_9.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_10.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_11.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_12.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_13.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_14.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_15.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_16.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_17.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_18.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_19.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_20.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_21.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_22.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_23.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_24.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_25.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_26.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_27.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_28.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_29.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_30.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_31.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_32.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_33.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/QCD_BCtoE_Pt80to170_7TeV/skimElecTau_34.root'
)


#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# photon + jets samples (PYTHIA)
#
# datasets = /PhotonJet_PtXXtoYY/Summer09-MC_31X_V3_7TeV-v1/GEN-SIM-RECO	
#
fileNamesZtoElecTau_PhotonPlusJets_Pt15to20_7TeV = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/PhotonPlusJets_Pt15to20_7TeV/skimElecTau_PhotonPlusJets_Pt15to20_7TeV_01.root'
)
fileNamesZtoElecTau_PhotonPlusJets_Pt20to30_7TeV = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/PhotonPlusJets_Pt20to30_7TeV/skimElecTau_PhotonPlusJets_Pt20to30_7TeV_01.root'
)
fileNamesZtoElecTau_PhotonPlusJets_Pt30to50_7TeV = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/PhotonPlusJets_Pt30to50_7TeV/skimElecTau_PhotonPlusJets_Pt30to50_7TeV_01.root'
)

corrFactorZtoElecTau_PhotonPlusJets_Pt15to20_7TeV = float(1.0)
corrFactorZtoElecTau_PhotonPlusJets_Pt20to30_7TeV = float(1.0)
corrFactorZtoElecTau_PhotonPlusJets_Pt30to50_7TeV = float(1.0)

intLumiZtoElecTau_PhotonPlusJets_Pt15to20_7TeV = float(0.9465)
intLumiZtoElecTau_PhotonPlusJets_Pt20to30_7TeV = float(1.924)
intLumiZtoElecTau_PhotonPlusJets_Pt30to50_7TeV = float(6.300)

genPhaseSpaceCutZtoElecTau_PhotonPlusJets_Pt15to20_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)
genPhaseSpaceCutZtoElecTau_PhotonPlusJets_Pt20to30_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)
genPhaseSpaceCutZtoElecTau_PhotonPlusJets_Pt30to50_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)
plotsOutputFileNameZtoElecTau_PhotonPlusJets_Pt15to20_7TeV = cms.string('plotsZtoElecTau_PhotonPlusJets_Pt15to20_7TeV.root')
plotsOutputFileNameZtoElecTau_PhotonPlusJets_Pt20to30_7TeV = cms.string('plotsZtoElecTau_PhotonPlusJets_Pt20to30_7TeV.root')
plotsOutputFileNameZtoElecTau_PhotonPlusJets_Pt30to50_7TeV = cms.string('plotsZtoElecTau_PhotonPlusJets_Pt30to50_7TeV.root')

patTupleOutputFileNameZtoElecTau_PhotonPlusJets_Pt15to20_7TeV = cms.untracked.string('patTupleZtoElecTau_PhotonPlusJets_Pt15to20_7TeV.root')
patTupleOutputFileNameZtoElecTau_PhotonPlusJets_Pt20to30_7TeV = cms.untracked.string('patTupleZtoElecTau_PhotonPlusJets_Pt20to30_7TeV.root')
patTupleOutputFileNameZtoElecTau_PhotonPlusJets_Pt30to50_7TeV = cms.untracked.string('patTupleZtoElecTau_PhotonPlusJets_Pt30to50_7TeV.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
#  W + jets sample generated with Madgraph
# 
#  7 TeV
#  dataset = /WJets-madgraph/Summer09-MC_31X_V3_7TeV-v1/GEN-SIM-RECO	
#  generated events = 11377412
#  events passing skim = 806193
#  int lumi = 638.1 pb^-1
#
# 
# (NOTE: for Monte Carlo samples generated by Madgraph,
#        the filter efficiency is already included in the cross-sections
#        listed at https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2008 !!)
#
intLumiZtoElecTau_WplusJets_7TeV = float(638.1)
#  9/571 skim output files missing
corrFactorZtoElecTau_WplusJets_7TeV = float(1.002)

patTupleOutputFileNameZtoElecTau_WplusJets_7TeV = cms.untracked.string('patTupleZtoElecTau_WplusJets_7TeV_partXX.root')

plotsOutputFileNameZtoElecTau_WplusJets_7TeV = cms.string('plotsZtoElecTau_WplusJets_7TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_WplusJets_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

fileNamesZtoElecTau_WplusJets_7TeV_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_01.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_02.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_03.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_04.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_05.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_06.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_07.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_08.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_09.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_10.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_11.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_12.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_13.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_14.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_15.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_16.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_17.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_18.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_19.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_20.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_21.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_22.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_23.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_24.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_25.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_26.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_27.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_28.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_29.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_30.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_31.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_32.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_33.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_34.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_35.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_36.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_37.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_38.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_39.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_40.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_41.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_42.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_43.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_44.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_45.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_46.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_47.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_48.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_49.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_50.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_51.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_52.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_53.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_54.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_55.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_56.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part57 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_57.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part58 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_58.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part59 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_59.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part60 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_60.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part61 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_61.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part62 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_62.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part63 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_63.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part64 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_64.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part65 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_65.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part66 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_66.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part67 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_67.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part68 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_68.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part69 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_69.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part70 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_70.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part71 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_71.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part72 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_72.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part73 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_73.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part74 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_74.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part75 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_75.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part76 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_76.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part77 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_77.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part78 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_78.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part79 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_79.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part80 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_80.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part81 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_81.root'
)
fileNamesZtoElecTau_WplusJets_7TeV_part82 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/WJets_7TeV/skimElecTau_WJets_7TeV_82.root'
)

#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
#  Z + jets sample generated with Madgraph (excluding Z --> tau+ tau- decays)
#
#  dataset = /ZJets-madgraph/Summer09-MC_31X_V3_7TeV-v2/GEN-SIM-RECO	
#  generated events = 1068735
#  events passing skim = 293565
#  integrated luminosity = 454.8 pb^-1
#
# (NOTE: for Monte Carlo samples generated by Madgraph,
#        the filter efficiency is already included in the cross-sections
#        listed at https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2008 !!)
#
intLumiZtoElecTau_ZJets_7TeV = float(454.8)
corrFactorZtoElecTau_ZJets_7TeV = float(1.0)

corrFactorZtoElecTau_ZeePlusJets_7TeV = corrFactorZtoElecTau_ZJets_7TeV
intLumiZtoElecTau_ZeePlusJets_7TeV = intLumiZtoElecTau_ZJets_7TeV

corrFactorZtoElecTau_ZtautauPlusJets_7TeV = corrFactorZtoElecTau_ZJets_7TeV
intLumiZtoElecTau_ZtautauPlusJets_7TeV = intLumiZtoElecTau_ZJets_7TeV

fileNamesZtoElecTau_ZJets_7TeV_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_1.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_2.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_3.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_4.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_5.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_6.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_7.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_8.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_9.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_10.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_11.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_12.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_13.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_14.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_15.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_16.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_17.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_18.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_19.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_20.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_21.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_22.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_23.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_24.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_25.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_26.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_27.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_28.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_29.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_30.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_31.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_32.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_33.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_34.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_35.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_36.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_37.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_38.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_39.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_40.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_41.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_42.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_43.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_44.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_45.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_46.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_47.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_48.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_49.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_50.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_51.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_52.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_53.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_54.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_55.root'
)
fileNamesZtoElecTau_ZJets_7TeV_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/ZJets_7TeV/skimElecTau_56.root'
)

fileNamesZtoElecTau_ZeePlusJets_7TeV_part01 = fileNamesZtoElecTau_ZJets_7TeV_part01
fileNamesZtoElecTau_ZeePlusJets_7TeV_part02 = fileNamesZtoElecTau_ZJets_7TeV_part02
fileNamesZtoElecTau_ZeePlusJets_7TeV_part03 = fileNamesZtoElecTau_ZJets_7TeV_part03
fileNamesZtoElecTau_ZeePlusJets_7TeV_part04 = fileNamesZtoElecTau_ZJets_7TeV_part04
fileNamesZtoElecTau_ZeePlusJets_7TeV_part05 = fileNamesZtoElecTau_ZJets_7TeV_part05
fileNamesZtoElecTau_ZeePlusJets_7TeV_part06 = fileNamesZtoElecTau_ZJets_7TeV_part06
fileNamesZtoElecTau_ZeePlusJets_7TeV_part07 = fileNamesZtoElecTau_ZJets_7TeV_part07
fileNamesZtoElecTau_ZeePlusJets_7TeV_part08 = fileNamesZtoElecTau_ZJets_7TeV_part08
fileNamesZtoElecTau_ZeePlusJets_7TeV_part09 = fileNamesZtoElecTau_ZJets_7TeV_part09
fileNamesZtoElecTau_ZeePlusJets_7TeV_part10 = fileNamesZtoElecTau_ZJets_7TeV_part10
fileNamesZtoElecTau_ZeePlusJets_7TeV_part11 = fileNamesZtoElecTau_ZJets_7TeV_part11
fileNamesZtoElecTau_ZeePlusJets_7TeV_part12 = fileNamesZtoElecTau_ZJets_7TeV_part12
fileNamesZtoElecTau_ZeePlusJets_7TeV_part13 = fileNamesZtoElecTau_ZJets_7TeV_part13
fileNamesZtoElecTau_ZeePlusJets_7TeV_part14 = fileNamesZtoElecTau_ZJets_7TeV_part14
fileNamesZtoElecTau_ZeePlusJets_7TeV_part15 = fileNamesZtoElecTau_ZJets_7TeV_part15
fileNamesZtoElecTau_ZeePlusJets_7TeV_part16 = fileNamesZtoElecTau_ZJets_7TeV_part16
fileNamesZtoElecTau_ZeePlusJets_7TeV_part17 = fileNamesZtoElecTau_ZJets_7TeV_part17
fileNamesZtoElecTau_ZeePlusJets_7TeV_part18 = fileNamesZtoElecTau_ZJets_7TeV_part18
fileNamesZtoElecTau_ZeePlusJets_7TeV_part19 = fileNamesZtoElecTau_ZJets_7TeV_part19
fileNamesZtoElecTau_ZeePlusJets_7TeV_part20 = fileNamesZtoElecTau_ZJets_7TeV_part20
fileNamesZtoElecTau_ZeePlusJets_7TeV_part21 = fileNamesZtoElecTau_ZJets_7TeV_part21
fileNamesZtoElecTau_ZeePlusJets_7TeV_part22 = fileNamesZtoElecTau_ZJets_7TeV_part22
fileNamesZtoElecTau_ZeePlusJets_7TeV_part23 = fileNamesZtoElecTau_ZJets_7TeV_part23
fileNamesZtoElecTau_ZeePlusJets_7TeV_part24 = fileNamesZtoElecTau_ZJets_7TeV_part24
fileNamesZtoElecTau_ZeePlusJets_7TeV_part25 = fileNamesZtoElecTau_ZJets_7TeV_part25
fileNamesZtoElecTau_ZeePlusJets_7TeV_part26 = fileNamesZtoElecTau_ZJets_7TeV_part26
fileNamesZtoElecTau_ZeePlusJets_7TeV_part27 = fileNamesZtoElecTau_ZJets_7TeV_part27
fileNamesZtoElecTau_ZeePlusJets_7TeV_part28 = fileNamesZtoElecTau_ZJets_7TeV_part28
fileNamesZtoElecTau_ZeePlusJets_7TeV_part29 = fileNamesZtoElecTau_ZJets_7TeV_part29
fileNamesZtoElecTau_ZeePlusJets_7TeV_part30 = fileNamesZtoElecTau_ZJets_7TeV_part30
fileNamesZtoElecTau_ZeePlusJets_7TeV_part31 = fileNamesZtoElecTau_ZJets_7TeV_part31
fileNamesZtoElecTau_ZeePlusJets_7TeV_part32 = fileNamesZtoElecTau_ZJets_7TeV_part32
fileNamesZtoElecTau_ZeePlusJets_7TeV_part33 = fileNamesZtoElecTau_ZJets_7TeV_part33
fileNamesZtoElecTau_ZeePlusJets_7TeV_part34 = fileNamesZtoElecTau_ZJets_7TeV_part34
fileNamesZtoElecTau_ZeePlusJets_7TeV_part35 = fileNamesZtoElecTau_ZJets_7TeV_part35
fileNamesZtoElecTau_ZeePlusJets_7TeV_part36 = fileNamesZtoElecTau_ZJets_7TeV_part36
fileNamesZtoElecTau_ZeePlusJets_7TeV_part37 = fileNamesZtoElecTau_ZJets_7TeV_part37
fileNamesZtoElecTau_ZeePlusJets_7TeV_part38 = fileNamesZtoElecTau_ZJets_7TeV_part38
fileNamesZtoElecTau_ZeePlusJets_7TeV_part39 = fileNamesZtoElecTau_ZJets_7TeV_part39
fileNamesZtoElecTau_ZeePlusJets_7TeV_part40 = fileNamesZtoElecTau_ZJets_7TeV_part40
fileNamesZtoElecTau_ZeePlusJets_7TeV_part41 = fileNamesZtoElecTau_ZJets_7TeV_part41
fileNamesZtoElecTau_ZeePlusJets_7TeV_part42 = fileNamesZtoElecTau_ZJets_7TeV_part42
fileNamesZtoElecTau_ZeePlusJets_7TeV_part43 = fileNamesZtoElecTau_ZJets_7TeV_part43
fileNamesZtoElecTau_ZeePlusJets_7TeV_part44 = fileNamesZtoElecTau_ZJets_7TeV_part44
fileNamesZtoElecTau_ZeePlusJets_7TeV_part45 = fileNamesZtoElecTau_ZJets_7TeV_part45
fileNamesZtoElecTau_ZeePlusJets_7TeV_part46 = fileNamesZtoElecTau_ZJets_7TeV_part46
fileNamesZtoElecTau_ZeePlusJets_7TeV_part47 = fileNamesZtoElecTau_ZJets_7TeV_part47
fileNamesZtoElecTau_ZeePlusJets_7TeV_part48 = fileNamesZtoElecTau_ZJets_7TeV_part48
fileNamesZtoElecTau_ZeePlusJets_7TeV_part49 = fileNamesZtoElecTau_ZJets_7TeV_part49
fileNamesZtoElecTau_ZeePlusJets_7TeV_part50 = fileNamesZtoElecTau_ZJets_7TeV_part50
fileNamesZtoElecTau_ZeePlusJets_7TeV_part51 = fileNamesZtoElecTau_ZJets_7TeV_part51
fileNamesZtoElecTau_ZeePlusJets_7TeV_part52 = fileNamesZtoElecTau_ZJets_7TeV_part52
fileNamesZtoElecTau_ZeePlusJets_7TeV_part53 = fileNamesZtoElecTau_ZJets_7TeV_part53
fileNamesZtoElecTau_ZeePlusJets_7TeV_part54 = fileNamesZtoElecTau_ZJets_7TeV_part54
fileNamesZtoElecTau_ZeePlusJets_7TeV_part55 = fileNamesZtoElecTau_ZJets_7TeV_part55
fileNamesZtoElecTau_ZeePlusJets_7TeV_part56 = fileNamesZtoElecTau_ZJets_7TeV_part56

patTupleOutputFileNameZtoElecTau_ZeePlusJets_7TeV = cms.untracked.string('patTupleZtoElecTau_ZeePlusJets_7TeV_partXX.root')

plotsOutputFileNameZtoElecTau_ZeePlusJets_7TeV = cms.string('plotsZtoElecTau_ZeePlusJets_7TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_ZeePlusJets_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genElectronsFromZs'),
    minNumber = cms.uint32(2)
)

fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part01 = fileNamesZtoElecTau_ZJets_7TeV_part01
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part02 = fileNamesZtoElecTau_ZJets_7TeV_part02
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part03 = fileNamesZtoElecTau_ZJets_7TeV_part03
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part04 = fileNamesZtoElecTau_ZJets_7TeV_part04
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part05 = fileNamesZtoElecTau_ZJets_7TeV_part05
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part06 = fileNamesZtoElecTau_ZJets_7TeV_part06
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part07 = fileNamesZtoElecTau_ZJets_7TeV_part07
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part08 = fileNamesZtoElecTau_ZJets_7TeV_part08
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part09 = fileNamesZtoElecTau_ZJets_7TeV_part09
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part10 = fileNamesZtoElecTau_ZJets_7TeV_part10
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part11 = fileNamesZtoElecTau_ZJets_7TeV_part11
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part12 = fileNamesZtoElecTau_ZJets_7TeV_part12
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part13 = fileNamesZtoElecTau_ZJets_7TeV_part13
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part14 = fileNamesZtoElecTau_ZJets_7TeV_part14
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part15 = fileNamesZtoElecTau_ZJets_7TeV_part15
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part16 = fileNamesZtoElecTau_ZJets_7TeV_part16
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part17 = fileNamesZtoElecTau_ZJets_7TeV_part17
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part18 = fileNamesZtoElecTau_ZJets_7TeV_part18
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part19 = fileNamesZtoElecTau_ZJets_7TeV_part19
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part20 = fileNamesZtoElecTau_ZJets_7TeV_part20
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part21 = fileNamesZtoElecTau_ZJets_7TeV_part21
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part22 = fileNamesZtoElecTau_ZJets_7TeV_part22
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part23 = fileNamesZtoElecTau_ZJets_7TeV_part23
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part24 = fileNamesZtoElecTau_ZJets_7TeV_part24
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part25 = fileNamesZtoElecTau_ZJets_7TeV_part25
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part26 = fileNamesZtoElecTau_ZJets_7TeV_part26
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part27 = fileNamesZtoElecTau_ZJets_7TeV_part27
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part28 = fileNamesZtoElecTau_ZJets_7TeV_part28
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part29 = fileNamesZtoElecTau_ZJets_7TeV_part29
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part30 = fileNamesZtoElecTau_ZJets_7TeV_part30
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part31 = fileNamesZtoElecTau_ZJets_7TeV_part31
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part32 = fileNamesZtoElecTau_ZJets_7TeV_part32
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part33 = fileNamesZtoElecTau_ZJets_7TeV_part33
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part34 = fileNamesZtoElecTau_ZJets_7TeV_part34
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part35 = fileNamesZtoElecTau_ZJets_7TeV_part35
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part36 = fileNamesZtoElecTau_ZJets_7TeV_part36
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part37 = fileNamesZtoElecTau_ZJets_7TeV_part37
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part38 = fileNamesZtoElecTau_ZJets_7TeV_part38
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part39 = fileNamesZtoElecTau_ZJets_7TeV_part39
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part40 = fileNamesZtoElecTau_ZJets_7TeV_part40
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part41 = fileNamesZtoElecTau_ZJets_7TeV_part41
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part42 = fileNamesZtoElecTau_ZJets_7TeV_part42
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part43 = fileNamesZtoElecTau_ZJets_7TeV_part43
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part44 = fileNamesZtoElecTau_ZJets_7TeV_part44
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part45 = fileNamesZtoElecTau_ZJets_7TeV_part45
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part46 = fileNamesZtoElecTau_ZJets_7TeV_part46
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part47 = fileNamesZtoElecTau_ZJets_7TeV_part47
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part48 = fileNamesZtoElecTau_ZJets_7TeV_part48
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part49 = fileNamesZtoElecTau_ZJets_7TeV_part49
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part50 = fileNamesZtoElecTau_ZJets_7TeV_part50
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part51 = fileNamesZtoElecTau_ZJets_7TeV_part51
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part52 = fileNamesZtoElecTau_ZJets_7TeV_part52
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part53 = fileNamesZtoElecTau_ZJets_7TeV_part53
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part54 = fileNamesZtoElecTau_ZJets_7TeV_part54
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part55 = fileNamesZtoElecTau_ZJets_7TeV_part55
fileNamesZtoElecTau_ZtautauPlusJets_7TeV_part56 = fileNamesZtoElecTau_ZJets_7TeV_part56

patTupleOutputFileNameZtoElecTau_ZtautauPlusJets_7TeV = cms.untracked.string('patTupleZtoElecTau_ZtautauPlusJets_7TeV_partXX.root')

plotsOutputFileNameZtoElecTau_ZtautauPlusJets_7TeV = cms.string('plotsZtoElecTau_ZtautauPlusJets_7TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_ZtautauPlusJets_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genTausFromZs'),
    minNumber = cms.uint32(2)
)
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# TTbar + Jets (madgraph, Fall 09) sample
#  
#  dataset = /TTbarJets-madgraph/Summer09-MC_31X_V3_7TeV-v2/GEN-SIM-RECO	
#  generated events = 1062730
#  events passing skim = 701424
#  integrated luminosity = 11808 pb^-1
#
# (NOTE: for Monte Carlo samples generated by Madgraph,
#        the filter efficiency is already included in the cross-sections
#        listed at https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2008 !!)
#
intLumiZtoElecTau_TTplusJets_7TeV = float(11808)
corrFactorZtoElecTau_TTplusJets_7TeV = float(1.0)

genPhaseSpaceCutZtoElecTau_TTplusJets_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

patTupleOutputFileNameZtoElecTau_TTplusJets_7TeV = cms.untracked.string('patTupleZtoElecTau_TTplusJets_7TeV_partXX.root')

plotsOutputFileNameZtoElecTau_TTplusJets_7TeV = cms.string('plotsZtoElecTau_TTplusJets_7TeV_partXX.root')

fileNamesZtoElecTau_TTplusJets_7TeV_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_1.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_2.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_3.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_4.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_5.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_6.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_7.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_8.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_9.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_10.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_11.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_12.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_13.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_14.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_15.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_16.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_17.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_18.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_19.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_20.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_21.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_22.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_23.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_24.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_25.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_26.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_27.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_28.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_29.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_30.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_31.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_32.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_33.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_34.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_35.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_36.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_37.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_38.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_39.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_40.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_41.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_42.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_43.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_44.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_45.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_46.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_47.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_48.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_49.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_50.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_51.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_52.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_53.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_54.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_55.root'
)
fileNamesZtoElecTau_TTplusJets_7TeV_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Summer09_CMSSW_3_1_4/TTJets_7TeV/skimElecTau_56.root'
)

#--------------------------------------------------------------------------------

