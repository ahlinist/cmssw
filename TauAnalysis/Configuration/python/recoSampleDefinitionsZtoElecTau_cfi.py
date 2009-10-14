import FWCore.ParameterSet.Config as cms
import copy

# define configuration parameters for submission of Z --> e + tau-jet jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
#
# samples skimmed with loose E/p selection and the inclusion of conversion collections and all track extra collections
#
# Authors:
#	Jeff Kolb, Notre Dame
#   Christian Veelken, UC Davis
#

intLumiZtoElecTau_Data = float(200.)

#--------------------------------------------------------------------------------
# Z --> tau+ tau- sample generated with Pythia + Tauola (all decay modes)
#
#  /Ztautau/Summer08_IDEAL_V11_redigi_v2/GEN-SIM-RECO
#  generated events = 1245500
#  skimmed events = 197374
#  integrated luminosity = 1146.9 pb^-1
#
intLumiZtoElecTau_Ztautau = float(1146.9)
corrFactorZtoElecTau_Ztautau = float(1.)

patTupleOutputFileNameZtoElecTau_Ztautau = cms.untracked.string('patTupleZtoElecTau_Ztautau_partXX.root')
plotsOutputFileNameZtoElecTau_Ztautau = cms.string('plotsZtoElecTau_Ztautau_partXX.root')
genPhaseSpaceCutZtoElecTau_Ztautau = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

fileNamesZtoElecTau_Ztautau_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Ztautau/skimElecTau_Ztautau_1.root'
)
fileNamesZtoElecTau_Ztautau_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Ztautau/skimElecTau_Ztautau_2.root'
)	
fileNamesZtoElecTau_Ztautau_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Ztautau/skimElecTau_Ztautau_3.root'
)	
fileNamesZtoElecTau_Ztautau_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Ztautau/skimElecTau_Ztautau_4.root'
)
fileNamesZtoElecTau_Ztautau_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Ztautau/skimElecTau_Ztautau_5.root'
)
fileNamesZtoElecTau_Ztautau_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Ztautau/skimElecTau_Ztautau_6.root'
)
fileNamesZtoElecTau_Ztautau_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Ztautau/skimElecTau_Ztautau_7.root'
)
fileNamesZtoElecTau_Ztautau_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Ztautau/skimElecTau_Ztautau_8.root'
)
fileNamesZtoElecTau_Ztautau_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Ztautau/skimElecTau_Ztautau_9.root'
)
fileNamesZtoElecTau_Ztautau_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Ztautau/skimElecTau_Ztautau_10.root'
)
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# Z --> e+ e- sample generated with Pythia
#
#  /Zee/Summer08_IDEAL_V11_redigi_v2/GEN-SIM-RECO
#  generated events = 822598
#  skimmed events = 634453
#  integrated luminosity = 667 pb^-1
#
intLumiZtoElecTau_Zee = float(667.)
# 1/165 skim output files missing
corrFactorZtoElecTau_Zee = float(1.006)

patTupleOutputFileNameZtoElecTau_Zee = cms.untracked.string('patTupleZtoElecTau_Zee_partXX.root')

plotsOutputFileNameZtoElecTau_Zee = cms.string('plotsZtoElecTau_Zee_partXX.root')

genPhaseSpaceCutZtoElecTau_Zee = cms.PSet(
  pluginName = cms.string('genPhaseSpaceCut'),
  pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

fileNamesZtoElecTau_Zee_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_01.root'
)
fileNamesZtoElecTau_Zee_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_02.root'
)
fileNamesZtoElecTau_Zee_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_03.root'
)
fileNamesZtoElecTau_Zee_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_04.root'
)
fileNamesZtoElecTau_Zee_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_05.root'
)
fileNamesZtoElecTau_Zee_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_06.root'
)
fileNamesZtoElecTau_Zee_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_07.root'
)
fileNamesZtoElecTau_Zee_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_08.root'
)
fileNamesZtoElecTau_Zee_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_09.root'
)
fileNamesZtoElecTau_Zee_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_10.root'
)
fileNamesZtoElecTau_Zee_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_11.root'
)
fileNamesZtoElecTau_Zee_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_12.root'
)
fileNamesZtoElecTau_Zee_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_13.root'
)
fileNamesZtoElecTau_Zee_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_14.root'
)
fileNamesZtoElecTau_Zee_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_15.root'
)
fileNamesZtoElecTau_Zee_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_16.root'
)
fileNamesZtoElecTau_Zee_part17 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_17.root'
)
fileNamesZtoElecTau_Zee_part18 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_18.root'
)
fileNamesZtoElecTau_Zee_part19 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_19.root'
)
fileNamesZtoElecTau_Zee_part20 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_20.root'
)
fileNamesZtoElecTau_Zee_part21 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_21.root'
)
fileNamesZtoElecTau_Zee_part22 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_22.root'
)
fileNamesZtoElecTau_Zee_part23 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_23.root'
)
fileNamesZtoElecTau_Zee_part24 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zee/skimElecTau_Zee_24.root'
)
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# EM enriched QCD sample generated with Pythia (20 GeV < Pt(hat) < 30 GeV)
#
#  /QCD_EMenriched_Pt20to30/Summer08_IDEAL_V11_redigi_v2/GEN-SIM-RECO
#  generated events = 20359765
#  events passing skim = 944563
#  xc = 0.40mb
#  filter eff = 0.008
#  integrated lumi = 6.362 pb^-1
#
#  missing 2/522 skim output files
corrFactorZtoElecTau_QCD_EMenriched_Pt20to30 = float(1.004)
intLumiZtoElecTau_QCD_EMenriched_Pt20to30 = float(6.36)

patTupleOutputFileNameZtoElecTau_QCD_EMenriched_Pt20to30 = cms.untracked.string('patTupleZtoElecTau_QCD_EMenriched_Pt20to30_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_EMenriched_Pt20to30 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt20to30_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_EMenriched_Pt20to30 = cms.PSet(
  pluginName = cms.string('genPhaseSpaceCut'),
  pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_1.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_2.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_3.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_4.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_5.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_6.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_7.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_8.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_9.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_10.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_11.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_12.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_13.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_14.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_15.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_16.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part17 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_17.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part18 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_18.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part19 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_19.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part20 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_20.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part21 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_21.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part22 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_22.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part23 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_23.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part24 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_24.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part25 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_25.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part26 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_26.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part27 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_27.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part28 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_28.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part29 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_29.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part30 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_30.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part31 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_31.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part32 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_32.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part33 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_33.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part34 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_34.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part35 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_35.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part36 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_36.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part37 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_37.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part38 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_38.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part39 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_39.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part40 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_40.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part41 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_41.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part42 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_42.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part43 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_43.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part44 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_44.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part45 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_45.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part46 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_46.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part47 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_47.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part48 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_48.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part49 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_49.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part50 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_50.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part51 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_51.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part52 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_52.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt20to30_part53 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_53.root'
)


#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# EM enriched QCD sample generated with Pythia (30 GeV < Pt(hat) < 80 GeV)
#
#  /QCD_EMenriched_Pt30to80/Summer08_IDEAL_V11_redigi_v2/GEN-SIM-RECO
#  generated events = 38298918
#  events passing skim   = 3978190
#  cross section = 0.10mb
#  filter eff = 0.047
#  integrated lumi = 8.15 pb^-1
#
# 4/1096 skim output files missing
corrFactorZtoElecTau_QCD_EMenriched_Pt30to80 = float(1.004)
intLumiZtoElecTau_QCD_EMenriched_Pt30to80 = float(8.15)

patTupleOutputFileNameZtoElecTau_QCD_EMenriched_Pt30to80 = cms.untracked.string('patTupleZtoElecTau_QCD_EMenriched_Pt30to80_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_EMenriched_Pt30to80 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_EMenriched_Pt30to80 = cms.PSet(
  pluginName = cms.string('genPhaseSpaceCut'),
  pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_1.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_2.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_3.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_4.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_5.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_6.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_7.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_8.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_9.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_10.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_11.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_12.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_13.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_14.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_15.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_16.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part17 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_17.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part18 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_18.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part19 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_19.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part20 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_20.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part21 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_21.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part22 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_22.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part23 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_23.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part24 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_24.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part25 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_25.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part26 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_26.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part27 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_27.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part28 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_28.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part29 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_29.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part30 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_30.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part31 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_31.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part32 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_32.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part33 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_33.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part34 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_34.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part35 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_35.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part36 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_36.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part37 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_37.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part38 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_38.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part39 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_39.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part40 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_40.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part41 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_41.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part42 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_42.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part43 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_43.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part44 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_44.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part45 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_45.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part46 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_46.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part47 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_47.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part48 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_48.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part49 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_49.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part50 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_50.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part51 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_51.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part52 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_52.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part53 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_53.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part54 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_54.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part55 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_55.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part56 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_56.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part57 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_57.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part58 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_58.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part59 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_59.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part60 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_60.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part61 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_61.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part62 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_62.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part63 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_63.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part64 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_64.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part65 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_65.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part66 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_66.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part67 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_67.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part68 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_68.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part69 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_69.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part70 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_70.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part71 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_71.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part72 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_72.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part73 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_73.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part74 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_74.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part75 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_75.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part76 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_76.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part77 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_77.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part78 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_78.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part79 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_79.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part80 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_80.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part81 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_81.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part82 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_82.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part83 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_83.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part84 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_84.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part85 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_85.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part86 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_86.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part87 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_87.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part88 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_88.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part89 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_89.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part90 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_90.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part91 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_91.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part92 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_92.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part93 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_93.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part94 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_94.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part95 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_95.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part96 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_96.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part97 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_97.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part98 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_98.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part99 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_99.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part100 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_100.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part101 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_101.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part102 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_102.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part103 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_103.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part104 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_104.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part105 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_105.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part106 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_106.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part107 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_107.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part108 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_108.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part109 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_109.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part110 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_110.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part111 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_111.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part112 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_112.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part113 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_113.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part114 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_114.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part115 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_115.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part116 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_116.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part117 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_117.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part118 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_118.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part119 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_119.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part120 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_120.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part121 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_121.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part122 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_122.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part123 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_123.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part124 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_124.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part125 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_125.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part126 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_126.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part127 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_127.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part128 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_128.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part129 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_129.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part130 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_130.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part131 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_131.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part132 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_132.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part133 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_133.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part134 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_134.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part135 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_135.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part136 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_136.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part137 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_137.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part138 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_138.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part139 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_139.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part140 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_140.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part141 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_141.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part142 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_142.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part143 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_143.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part144 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_144.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part145 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_145.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part146 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_146.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part147 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_147.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part148 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_148.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part149 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_149.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part150 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_150.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part151 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_151.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part152 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_152.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part153 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_153.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part154 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_154.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part155 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_155.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part156 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_156.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part157 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_157.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part158 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_158.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part159 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_159.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part160 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_160.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part161 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_161.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part162 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_162.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part163 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_163.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part164 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_164.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part165 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_165.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part166 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_166.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part167 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_167.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part168 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_168.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part169 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_169.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part170 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_170.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part171 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_171.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part172 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_172.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part173 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_173.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part174 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_174.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part175 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_175.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part176 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_176.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part177 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_177.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part178 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_178.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part179 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_179.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part180 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_180.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part181 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_181.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part182 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_182.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part183 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_183.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part184 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_184.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part185 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_185.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part186 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_186.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part187 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_187.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part188 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_188.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part189 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_189.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part190 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_190.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part191 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_191.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part192 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_192.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part193 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_193.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part194 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_194.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part195 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_195.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part196 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_196.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part197 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_197.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part198 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_198.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part199 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_199.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part200 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_200.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part201 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_201.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part202 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_202.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part203 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_203.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part204 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_204.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part205 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_205.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part206 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_206.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part207 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_207.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part208 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_208.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part209 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_209.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part210 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_210.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part211 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_211.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part212 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_212.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part213 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_213.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part214 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_214.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part215 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_215.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part216 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_216.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part217 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_217.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part218 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_218.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part219 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_219.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt30to80_part220 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_220.root'
)
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# EM enriched QCD sample generated with Pythia (80 GeV < Pt(hat) < 170 GeV)
#
#  /QCD_EMenriched_Pt80to170/Summer08_IDEAL_V11_redigi_v1/GEN-SIM-RECO
#  generated events = 5970425
#  events passing skim = 1152618
#  cross section = 0.0019mb
#  filter eff = 0.15
#  integrated lumi = 20.95
#
#
intLumiZtoElecTau_QCD_EMenriched_Pt80to170 = float(20.95)
corrFactorZtoElecTau_QCD_EMenriched_Pt80to170 = float(1.)

patTupleOutputFileNameZtoElecTau_QCD_EMenriched_Pt80to170 = cms.untracked.string('patTupleZtoElecTau_QCD_EMenriched_Pt80to170_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_EMenriched_Pt80to170 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_EMenriched_Pt80to170 = cms.PSet(
  pluginName = cms.string('genPhaseSpaceCut'),
  pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_1.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_2.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_3.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_4.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_5.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_6.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_7.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_8.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_9.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_10.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_11.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_12.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_13.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_14.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_15.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_16.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part17 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_17.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part18 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_18.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part19 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_19.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part20 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_20.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part21 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_21.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part22 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_22.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part23 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_23.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part24 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_24.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part25 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_25.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part26 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_26.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part27 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_27.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part28 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_28.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part29 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_29.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part30 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_30.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part31 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_31.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part32 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_32.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part33 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_33.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part34 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_34.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part35 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_35.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part36 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_36.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part37 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_37.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part38 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_38.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part39 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_39.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part40 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_40.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part41 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_41.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part42 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_42.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part43 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_43.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part44 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_44.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part45 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_45.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part46 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_46.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part47 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_47.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part48 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_48.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part49 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_49.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part50 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_50.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part51 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_51.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part52 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_52.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part53 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_53.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part54 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_54.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part55 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_55.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part56 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_56.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part57 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_57.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part58 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_58.root'
)
fileNamesZtoElecTau_QCD_EMenriched_Pt80to170_part59 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_59.root'
)

#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# QCD b/c --> e sample generated with Pythia (20 GeV < Pt(hat) < 30 GeV)
#
#  /QCD_BCtoE_Pt20to30/Summer08_IDEAL_V11_redigi_v1/GEN-SIM-RECO
#  generated events = 1997072
#  events passing skim = 516997
#  cross section = 0.04mb
#  filter eff = 0.00048
#  integrated luminosity = 10.4 pb^-1
# 
#
intLumiZtoElecTau_QCD_BCtoE_Pt20to30 = float(10.4)
corrFactorZtoElecTau_QCD_BCtoE_Pt20to30 = float(1.0)

patTupleOutputFileNameZtoElecTau_QCD_BCtoE_Pt20to30 = cms.untracked.string('patTupleZtoElecTau_QCD_BCtoE_Pt20to30_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_BCtoE_Pt20to30 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_BCtoE_Pt20to30 = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_01.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_02.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_03.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_04.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_05.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_06.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_07.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_08.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_09.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_10.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_11.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_12.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_13.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_14.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_15.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_16.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part17 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_17.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part18 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_18.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part19 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_19.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part20 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_20.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part21 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_21.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part22 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_22.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part23 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_23.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part24 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_24.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part25 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_25.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part26 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_26.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part27 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_27.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part28 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt20to30/skimElecTau_QCD_BCtoE_Pt20to30_28.root'
)
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# QCD b/c -->e sample generated with Pythia (30 GeV < Pt(hat) < 80 GeV)
#
#  /QCD_BCtoE_Pt30to80/Summer08_IDEAL_V11_redigi_v1/GEN-SIM-RECO
#  generated events = 2016487
#  events passing skim = 842837
#  cross section = 0.10mb
#  filter eff = 0.0024
#  integrated luminosity = 8.40 pb^-1
#
#
corrFactorZtoElecTau_QCD_BCtoE_Pt30to80 = float(1.0)
intLumiZtoElecTau_QCD_BCtoE_Pt30to80 = float(8.40)

patTupleOutputFileNameZtoElecTau_QCD_BCtoE_Pt30to80 = cms.untracked.string('patTupleZtoElecTau_QCD_BCtoE_Pt30to80_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_BCtoE_Pt30to80 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_BCtoE_Pt30to80 = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_01.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_02.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_03.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_04.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_05.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_06.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_07.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_08.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_09.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_10.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_11.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_12.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_13.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_14.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_15.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_16.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part17 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_17.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part18 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_18.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part19 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_19.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part20 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_20.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part21 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_21.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part22 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_22.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part23 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_23.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part24 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_24.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part25 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_25.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part26 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_26.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part27 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_27.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part28 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_28.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part29 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_29.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part30 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_30.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part31 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_31.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part32 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_32.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part33 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_33.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part34 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_34.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part35 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_35.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part36 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_36.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part37 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_37.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part38 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_38.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part39 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_39.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part40 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_40.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part41 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_41.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part42 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_42.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part43 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_43.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part44 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_44.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part45 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_45.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part46 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt30to80/skimElecTau_QCD_BCtoE_Pt30to80_46.root'
)
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
#  QCD b/c -->e sample generated with Pythia (80 GeV < Pt(hat) < 170 GeV)
#
#  dataset = /QCD_BCtoE_Pt80to170/Summer08_IDEAL_V11_redigi_v1/GEN-SIM-RECO
#  generated events = 1075822
#  events passing skim = 480399
#  cross section = 0.0019mb
#  filter eff = 0.012
#  integrated luminosity = 47.2 pb^-1
#
corrFactorZtoElecTau_QCD_BCtoE_Pt80to170 = float(1.0)
intLumiZtoElecTau_QCD_BCtoE_Pt80to170 = float(47.2)

patTupleOutputFileNameZtoElecTau_QCD_BCtoE_Pt80to170 = cms.untracked.string('patTupleZtoElecTau_QCD_BCtoE_Pt80to170_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_BCtoE_Pt80to170 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt80to170_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_BCtoE_Pt80to170 = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_01.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_02.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_03.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_04.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_05.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_06.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_07.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_08.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_09.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_10.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_11.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_12.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_13.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_14.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_15.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_16.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part17 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_17.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part18 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_18.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part19 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_19.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part20 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_20.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part21 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_21.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part22 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_22.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part23 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_23.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part24 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_24.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part25 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_25.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part26 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_26.root'
)
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# photon + jets samples (PYTHIA8)
# 500k events total, PDF set is CTEQ5L, filter on photons in central region (-2.4 < eta(gamma) < 2.4, ET(gamma) > 15 GeV/c)
#
# datasets = /PYTHIA8PhotonJetPtXXtoYY/Summer08_IDEAL_V11_redigi_v1/GEN-SIM-RECO
#
fileNamesZtoElecTau_PhotonJets_Pt15to20 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/PhotonJets_Pt15to20/skimElecTau_PhotonJets_Pt15to20_1.root'
)
fileNamesZtoElecTau_PhotonJets_Pt20to25 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/PhotonJets_Pt20to25/skimElecTau_PhotonJets_Pt20to25_1.root'
)
fileNamesZtoElecTau_PhotonJets_Pt25to30 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/PhotonJets_Pt25to30/skimElecTau_PhotonJets_Pt25to30_1.root'
)
fileNamesZtoElecTau_PhotonJets_Pt30to35 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/PhotonJets_Pt30to35/skimElecTau_PhotonJets_Pt30to35_1.root'
)
fileNamesZtoElecTau_PhotonJets_PtGt35 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/PhotonJets_PtGt35/skimElecTau_PhotonJets_PtGt35_1.root'
)

corrFactorZtoElecTau_PhotonJets_Pt15to20 = float(1.0)
corrFactorZtoElecTau_PhotonJets_Pt20to25 = float(1.0)
corrFactorZtoElecTau_PhotonJets_Pt25to30 = float(1.0)
corrFactorZtoElecTau_PhotonJets_Pt30to35 = float(1.0)
corrFactorZtoElecTau_PhotonJets_PtGt35 = float(1.0)

intLumiZtoElecTau_PhotonJets_Pt15to20 = float(1.075)
intLumiZtoElecTau_PhotonJets_Pt20to25 = float(3.38)
intLumiZtoElecTau_PhotonJets_Pt25to30 = float(8.95)
intLumiZtoElecTau_PhotonJets_Pt30to35 = float(21.89)
intLumiZtoElecTau_PhotonJets_PtGt35   = float(13.36)

genPhaseSpaceCutZtoElecTau_PhotonJets_Pt15to20 = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)
genPhaseSpaceCutZtoElecTau_PhotonJets_Pt20to25 = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)
genPhaseSpaceCutZtoElecTau_PhotonJets_Pt25to30 = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)
genPhaseSpaceCutZtoElecTau_PhotonJets_Pt30to35 = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)
genPhaseSpaceCutZtoElecTau_PhotonJets_PtGt35 = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

plotsOutputFileNameZtoElecTau_PhotonJets_Pt15to20 = cms.string('plotsZtoElecTau_PhotonJets_Pt15to20.root')
plotsOutputFileNameZtoElecTau_PhotonJets_Pt20to25 = cms.string('plotsZtoElecTau_PhotonJets_Pt20to25.root')
plotsOutputFileNameZtoElecTau_PhotonJets_Pt25to30 = cms.string('plotsZtoElecTau_PhotonJets_Pt25to30.root')
plotsOutputFileNameZtoElecTau_PhotonJets_Pt30to35 = cms.string('plotsZtoElecTau_PhotonJets_Pt30to35.root')
plotsOutputFileNameZtoElecTau_PhotonJets_PtGt35   = cms.string('plotsZtoElecTau_PhotonJets_PtGt35.root')

patTupleOutputFileNameZtoElecTau_PhotonJets_Pt15to20 = cms.string('patTupleZtoElecTau_PhotonJets_Pt15to20.root')
patTupleOutputFileNameZtoElecTau_PhotonJets_Pt20to25 = cms.string('patTupleZtoElecTau_PhotonJets_Pt20to25.root')
patTupleOutputFileNameZtoElecTau_PhotonJets_Pt25to30 = cms.string('patTupleZtoElecTau_PhotonJets_Pt25to30.root')
patTupleOutputFileNameZtoElecTau_PhotonJets_Pt30to35 = cms.string('patTupleZtoElecTau_PhotonJets_Pt30to35.root')
patTupleOutputFileNameZtoElecTau_PhotonJets_PtGt35   = cms.string('patTupleZtoElecTau_PhotonJets_PtGt35.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
#  W + jets sample generated with Madgraph
#  dataset = /WJets-madgraph/Summer08_IDEAL_V11_redigi_v1/GEN-SIM-RECO
#  generated events = 9745661
#  events passing skim = 584160
#  cross section = 40nb
#  int lumi = 243 pb^-1
#
# 
# (NOTE: for Monte Carlo samples generated by Madgraph,
#        the filter efficiency is already included in the cross-sections
#        listed at https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2008 !!)
#
intLumiZtoElecTau_WplusJets = float(242.)
#  1/979 skim output files missing
corrFactorZtoElecTau_WplusJets = float(1.001)

fileNamesZtoElecTau_WplusJets_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_01.root'
)
fileNamesZtoElecTau_WplusJets_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_02.root'
)
fileNamesZtoElecTau_WplusJets_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_03.root'
)
fileNamesZtoElecTau_WplusJets_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_04.root'
)
fileNamesZtoElecTau_WplusJets_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_05.root'
)
fileNamesZtoElecTau_WplusJets_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_06.root'
)
fileNamesZtoElecTau_WplusJets_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_07.root'
)
fileNamesZtoElecTau_WplusJets_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_08.root'
)
fileNamesZtoElecTau_WplusJets_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_09.root'
)
fileNamesZtoElecTau_WplusJets_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_10.root'
)
fileNamesZtoElecTau_WplusJets_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_11.root'
)
fileNamesZtoElecTau_WplusJets_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_12.root'
)
fileNamesZtoElecTau_WplusJets_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_13.root'
)
fileNamesZtoElecTau_WplusJets_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_14.root'
)
fileNamesZtoElecTau_WplusJets_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_15.root'
)
fileNamesZtoElecTau_WplusJets_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_16.root'
)
fileNamesZtoElecTau_WplusJets_part17 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_17.root'
)
fileNamesZtoElecTau_WplusJets_part18 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_18.root'
)
fileNamesZtoElecTau_WplusJets_part19 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_19.root'
)
fileNamesZtoElecTau_WplusJets_part20 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_20.root'
)
fileNamesZtoElecTau_WplusJets_part21 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_21.root'
)
fileNamesZtoElecTau_WplusJets_part22 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_22.root'
)
fileNamesZtoElecTau_WplusJets_part23 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_23.root'
)
fileNamesZtoElecTau_WplusJets_part24 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_24.root'
)
fileNamesZtoElecTau_WplusJets_part25 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_25.root'
)
fileNamesZtoElecTau_WplusJets_part26 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_26.root'
)
fileNamesZtoElecTau_WplusJets_part27 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_27.root'
)
fileNamesZtoElecTau_WplusJets_part28 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_28.root'
)
fileNamesZtoElecTau_WplusJets_part29 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_29.root'
)
fileNamesZtoElecTau_WplusJets_part30 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_30.root'
)
fileNamesZtoElecTau_WplusJets_part31 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_31.root'
)
fileNamesZtoElecTau_WplusJets_part32 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_32.root'
)
fileNamesZtoElecTau_WplusJets_part33 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_33.root'
)
fileNamesZtoElecTau_WplusJets_part34 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Wjets_madgraph/skimElecTau_Wjets_madgraph_34.root'
)

patTupleOutputFileNameZtoElecTau_WplusJets = cms.untracked.string('patTupleZtoElecTau_WplusJets_partXX.root')

plotsOutputFileNameZtoElecTau_WplusJets = cms.string('plotsZtoElecTau_WplusJets_partXX.root')

genPhaseSpaceCutZtoElecTau_WplusJets = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
#  Z + jets sample generated with Madgraph (excluding Z --> tau+ tau- decays)
#
#  dataset = /ZJets-madgraph/Summer08_IDEAL_V11_redigi_v1/GEN-SIM-RECO
#  generated events = 1262816
#  events passing skim = 319321
#  cross section = 3.7nb
#  integrated luminosity = 341 pb^-1
#
# (NOTE: for Monte Carlo samples generated by Madgraph,
#        the filter efficiency is already included in the cross-sections
#        listed at https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2008 !!)
#
intLumiZtoElecTau_ZplusJets = float(341.)
corrFactorZtoElecTau_ZplusJets = float(1.0)

fileNamesZtoElecTau_ZplusJets_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zjets_madgraph/skimElecTau_Zjets_madgraph_01.root'
)
fileNamesZtoElecTau_ZplusJets_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zjets_madgraph/skimElecTau_Zjets_madgraph_02.root'
)
fileNamesZtoElecTau_ZplusJets_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zjets_madgraph/skimElecTau_Zjets_madgraph_03.root'
)
fileNamesZtoElecTau_ZplusJets_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zjets_madgraph/skimElecTau_Zjets_madgraph_04.root'
)
fileNamesZtoElecTau_ZplusJets_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zjets_madgraph/skimElecTau_Zjets_madgraph_05.root'
)
fileNamesZtoElecTau_ZplusJets_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zjets_madgraph/skimElecTau_Zjets_madgraph_06.root'
)
fileNamesZtoElecTau_ZplusJets_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zjets_madgraph/skimElecTau_Zjets_madgraph_07.root'
)
fileNamesZtoElecTau_ZplusJets_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zjets_madgraph/skimElecTau_Zjets_madgraph_08.root'
)
fileNamesZtoElecTau_ZplusJets_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zjets_madgraph/skimElecTau_Zjets_madgraph_09.root'
)
fileNamesZtoElecTau_ZplusJets_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zjets_madgraph/skimElecTau_Zjets_madgraph_10.root'
)
fileNamesZtoElecTau_ZplusJets_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zjets_madgraph/skimElecTau_Zjets_madgraph_11.root'
)
fileNamesZtoElecTau_ZplusJets_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zjets_madgraph/skimElecTau_Zjets_madgraph_12.root'
)
fileNamesZtoElecTau_ZplusJets_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zjets_madgraph/skimElecTau_Zjets_madgraph_13.root'
)
fileNamesZtoElecTau_ZplusJets_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zjets_madgraph/skimElecTau_Zjets_madgraph_14.root'
)
fileNamesZtoElecTau_ZplusJets_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zjets_madgraph/skimElecTau_Zjets_madgraph_15.root'
)
fileNamesZtoElecTau_ZplusJets_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Zjets_madgraph/skimElecTau_Zjets_madgraph_16.root'
)

corrFactorZtoElecTau_ZeePlusJets = corrFactorZtoElecTau_ZplusJets
intLumiZtoElecTau_ZeePlusJets = intLumiZtoElecTau_ZplusJets

fileNamesZtoElecTau_ZeePlusJets_part01 = fileNamesZtoElecTau_ZplusJets_part01
fileNamesZtoElecTau_ZeePlusJets_part02 = fileNamesZtoElecTau_ZplusJets_part02
fileNamesZtoElecTau_ZeePlusJets_part03 = fileNamesZtoElecTau_ZplusJets_part03
fileNamesZtoElecTau_ZeePlusJets_part04 = fileNamesZtoElecTau_ZplusJets_part04
fileNamesZtoElecTau_ZeePlusJets_part05 = fileNamesZtoElecTau_ZplusJets_part05
fileNamesZtoElecTau_ZeePlusJets_part06 = fileNamesZtoElecTau_ZplusJets_part06
fileNamesZtoElecTau_ZeePlusJets_part07 = fileNamesZtoElecTau_ZplusJets_part07
fileNamesZtoElecTau_ZeePlusJets_part08 = fileNamesZtoElecTau_ZplusJets_part08
fileNamesZtoElecTau_ZeePlusJets_part09 = fileNamesZtoElecTau_ZplusJets_part09
fileNamesZtoElecTau_ZeePlusJets_part10 = fileNamesZtoElecTau_ZplusJets_part10
fileNamesZtoElecTau_ZeePlusJets_part11 = fileNamesZtoElecTau_ZplusJets_part11
fileNamesZtoElecTau_ZeePlusJets_part12 = fileNamesZtoElecTau_ZplusJets_part12
fileNamesZtoElecTau_ZeePlusJets_part13 = fileNamesZtoElecTau_ZplusJets_part13
fileNamesZtoElecTau_ZeePlusJets_part14 = fileNamesZtoElecTau_ZplusJets_part14
fileNamesZtoElecTau_ZeePlusJets_part15 = fileNamesZtoElecTau_ZplusJets_part15
fileNamesZtoElecTau_ZeePlusJets_part16 = fileNamesZtoElecTau_ZplusJets_part16

patTupleOutputFileNameZtoElecTau_ZeePlusJets = cms.untracked.string('patTupleZtoElecTau_ZeePlusJets_partXX.root')

plotsOutputFileNameZtoElecTau_ZeePlusJets = cms.string('plotsZtoElecTau_ZeePlusJets_partXX.root')

genPhaseSpaceCutZtoElecTau_ZeePlusJets = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genElectronsFromZs'),
    minNumber = cms.uint32(2)
)

corrFactorZtoElecTau_ZtautauPlusJets = corrFactorZtoElecTau_ZplusJets
intLumiZtoElecTau_ZtautauPlusJets = intLumiZtoElecTau_ZplusJets

fileNamesZtoElecTau_ZtautauPlusJets_part01 = fileNamesZtoElecTau_ZplusJets_part01
fileNamesZtoElecTau_ZtautauPlusJets_part02 = fileNamesZtoElecTau_ZplusJets_part02
fileNamesZtoElecTau_ZtautauPlusJets_part03 = fileNamesZtoElecTau_ZplusJets_part03
fileNamesZtoElecTau_ZtautauPlusJets_part04 = fileNamesZtoElecTau_ZplusJets_part04
fileNamesZtoElecTau_ZtautauPlusJets_part05 = fileNamesZtoElecTau_ZplusJets_part05
fileNamesZtoElecTau_ZtautauPlusJets_part06 = fileNamesZtoElecTau_ZplusJets_part06
fileNamesZtoElecTau_ZtautauPlusJets_part07 = fileNamesZtoElecTau_ZplusJets_part07
fileNamesZtoElecTau_ZtautauPlusJets_part08 = fileNamesZtoElecTau_ZplusJets_part08
fileNamesZtoElecTau_ZtautauPlusJets_part09 = fileNamesZtoElecTau_ZplusJets_part09
fileNamesZtoElecTau_ZtautauPlusJets_part10 = fileNamesZtoElecTau_ZplusJets_part10
fileNamesZtoElecTau_ZtautauPlusJets_part11 = fileNamesZtoElecTau_ZplusJets_part11
fileNamesZtoElecTau_ZtautauPlusJets_part12 = fileNamesZtoElecTau_ZplusJets_part12
fileNamesZtoElecTau_ZtautauPlusJets_part13 = fileNamesZtoElecTau_ZplusJets_part13
fileNamesZtoElecTau_ZtautauPlusJets_part14 = fileNamesZtoElecTau_ZplusJets_part14
fileNamesZtoElecTau_ZtautauPlusJets_part15 = fileNamesZtoElecTau_ZplusJets_part15
fileNamesZtoElecTau_ZtautauPlusJets_part16 = fileNamesZtoElecTau_ZplusJets_part16

patTupleOutputFileNameZtoElecTau_ZtautauPlusJets = cms.untracked.string('patTupleZtoElecTau_ZtautauPlusJets_partXX.root')

plotsOutputFileNameZtoElecTau_ZtautauPlusJets = cms.string('plotsZtoElecTau_ZtautauPlusJets_partXX.root')

genPhaseSpaceCutZtoElecTau_ZtautauPlusJets = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genTausFromZs'),
    minNumber = cms.uint32(2)
)
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# TTbar + Jets (madgraph, Fall 08) sample
#  
#  dataset = /TTJets-madgraph/Fall08_IDEAL_V11_redigi_v10/GEN-SIM-RECO
#  generated events = 946644
#  events passing skim = 470062
#  cross section = 317 pb
#  integrated luminosity = 2986 pb^-1
#
# (NOTE: for Monte Carlo samples generated by Madgraph,
#        the filter efficiency is already included in the cross-sections
#        listed at https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2008 !!)
#
intLumiZtoElecTau_TTplusJets = float(2986)
corrFactorZtoElecTau_TTplusJets = float(1.0)

fileNamesZtoElecTau_TTplusJets_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_01.root'
)
fileNamesZtoElecTau_TTplusJets_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_02.root'
)
fileNamesZtoElecTau_TTplusJets_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_03.root'
)
fileNamesZtoElecTau_TTplusJets_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_04.root'
)
fileNamesZtoElecTau_TTplusJets_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_05.root'
)
fileNamesZtoElecTau_TTplusJets_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_06.root'
)
fileNamesZtoElecTau_TTplusJets_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_07.root'
)
fileNamesZtoElecTau_TTplusJets_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_08.root'
)
fileNamesZtoElecTau_TTplusJets_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_09.root'
)
fileNamesZtoElecTau_TTplusJets_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_10.root'
)
fileNamesZtoElecTau_TTplusJets_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_11.root'
)
fileNamesZtoElecTau_TTplusJets_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_12.root'
)
fileNamesZtoElecTau_TTplusJets_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_13.root'
)
fileNamesZtoElecTau_TTplusJets_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_14.root'
)
fileNamesZtoElecTau_TTplusJets_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_15.root'
)
fileNamesZtoElecTau_TTplusJets_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_16.root'
)
fileNamesZtoElecTau_TTplusJets_part17 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_17.root'
)
fileNamesZtoElecTau_TTplusJets_part18 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_18.root'
)
fileNamesZtoElecTau_TTplusJets_part19 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_19.root'
)
fileNamesZtoElecTau_TTplusJets_part20 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_20.root'
)
fileNamesZtoElecTau_TTplusJets_part21 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_21.root'
)
fileNamesZtoElecTau_TTplusJets_part22 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_22.root'
)
fileNamesZtoElecTau_TTplusJets_part23 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_23.root'
)
fileNamesZtoElecTau_TTplusJets_part24 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_24.root'
)
fileNamesZtoElecTau_TTplusJets_part25 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_25.root'
)
fileNamesZtoElecTau_TTplusJets_part26 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_26.root'
)
fileNamesZtoElecTau_TTplusJets_part27 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_27.root'
)
fileNamesZtoElecTau_TTplusJets_part28 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_28.root'
)
fileNamesZtoElecTau_TTplusJets_part29 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_29.root'
)
fileNamesZtoElecTau_TTplusJets_part30 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_30.root'
)
fileNamesZtoElecTau_TTplusJets_part31 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_31.root'
)
fileNamesZtoElecTau_TTplusJets_part32 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/TTjets_madgraph/skimElecTau_TTjets_madgraph_32.root'
)

genPhaseSpaceCutZtoElecTau_TTplusJets = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

patTupleOutputFileNameZtoElecTau_TTplusJets = cms.untracked.string('patTupleZtoElecTau_TTplusJets_partXX.root')

plotsOutputFileNameZtoElecTau_TTplusJets = cms.string('plotsZtoElecTau_TTplusJets_partXX.root')
#--------------------------------------------------------------------------------

