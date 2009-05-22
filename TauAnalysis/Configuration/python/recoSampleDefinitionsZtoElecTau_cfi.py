import FWCore.ParameterSet.Config as cms
import copy

# define configuration parameters for submission of Z --> e + tau-jet jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)

intLumiData = float(200.)

patTupleOutputDirectoryName = cms.string('/castor/cern.ch/user/v/veelken/CMSSW_2_2_7/')

#--------------------------------------------------------------------------------
# Z --> tau+ tau- sample generated with Pythia + Tauola (all decay modes)
#  /Ztautau/Summer08_IDEAL_V11_redigi_v2/GEN-SIM-RECO
#  generated events = 1245500
#  skimmed events = 131703
#  integrated luminosity = 1146.9 pb^-1
#
intLumiZtautau = float(1146.9)

fileNamesZtautau_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Ztautau/skimElecTau_Ztautau_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Ztautau/skimElecTau_Ztautau_2.root'
)

genPhaseSpaceCutZtautau_part01 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameZtautau_part01 = cms.string('plotsZtoElecTau_Ztautau_part01.root')

fileNamesZtautau_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Ztautau/skimElecTau_Ztautau_3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Ztautau/skimElecTau_Ztautau_4.root'
)

genPhaseSpaceCutZtautau_part02 = genPhaseSpaceCutZtautau_part01

outputFileNameZtautau_part02 = cms.string('plotsZtoElecTau_Ztautau_part02.root')

fileNamesZtautau_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Ztautau/skimElecTau_Ztautau_5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Ztautau/skimElecTau_Ztautau_6.root'
)

genPhaseSpaceCutZtautau_part03 = genPhaseSpaceCutZtautau_part01

outputFileNameZtautau_part03 = cms.string('plotsZtoElecTau_Ztautau_part03.root')

fileNamesZtautau_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Ztautau/skimElecTau_Ztautau_7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Ztautau/skimElecTau_Ztautau_8.root'
)

genPhaseSpaceCutZtautau_part04 = genPhaseSpaceCutZtautau_part01

outputFileNameZtautau_part04 = cms.string('plotsZtoElecTau_Ztautau_part04.root')

fileNamesZtautau_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Ztautau/skimElecTau_Ztautau_9.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Ztautau/skimElecTau_Ztautau_10.root'
)

genPhaseSpaceCutZtautau_part05 = genPhaseSpaceCutZtautau_part01

outputFileNameZtautau_part05 = cms.string('plotsZtoElecTau_Ztautau_part05.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# Z --> e+ e- sample generated with Pythia
#
#  /Zee/Summer08_IDEAL_V11_redigi_v2/GEN-SIM-RECO
#  generated events = 822598
#  skimmed events = 587512
#  integrated luminosity = 667 pb^-1
#
intLumiZee = float(667.)

fileNamesZee_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_2.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_3.root'
)

fileNamesZee_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_4.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_6.root'
)

fileNamesZee_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_8.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_9.root'
)

fileNamesZee_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_10.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_11.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_12.root'
)

fileNamesZee_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_13.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_14.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_15.root'
)

fileNamesZee_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_16.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_17.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_18.root'
)

fileNamesZee_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_19.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_20.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_21.root'
)

fileNamesZee_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_22.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_23.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_24.root'
)

fileNamesZee_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_25.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_26.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_27.root'
)

fileNamesZee_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_28.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_29.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_30.root'
)

fileNamesZee_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_31.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_32.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_33.root'
)

fileNamesZee_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_34.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_35.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_36.root'
)

fileNamesZee_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_37.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_38.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_39.root'
)

fileNamesZee_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_40.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_41.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_42.root'
)

fileNamesZee_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_43.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_44.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_45.root'
)

fileNamesZee_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_46.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_47.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_48.root'
)

fileNamesZee_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_49.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_50.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_51.root'
)

fileNamesZee_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_52.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_53.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_54.root'
)

fileNamesZee_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_55.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_56.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_57.root'
)

fileNamesZee_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_58.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_59.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_60.root'
)

fileNamesZee_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_61.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_62.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_63.root'
)
fileNamesZee_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_64.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_65.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_66.root'
)
fileNamesZee_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_67.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_68.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_69.root'
)
fileNamesZee_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_70.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_71.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_72.root'
)
fileNamesZee_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_73.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_74.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_75.root'
)
fileNamesZee_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_76.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_77.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_78.root'
)
fileNamesZee_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_79.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_80.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_81.root'
)

fileNamesZee_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_82.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zee/skimElecTau_83.root'
)

genPhaseSpaceCutZee_part01 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

genPhaseSpaceCutZee_part02 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part03 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part04 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part05 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part06 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part07 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part08 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part09 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part10 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part11 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part12 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part13 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part14 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part15 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part16 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part17 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part18 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part19 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part20 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part21 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part22 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part23 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part24 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part25 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part26 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part27 = genPhaseSpaceCutZee_part01  
genPhaseSpaceCutZee_part28 = genPhaseSpaceCutZee_part01 

outputFileNameZee_part01 = cms.string('plotsZtoElecTau_Zee_part01.root')
outputFileNameZee_part02 = cms.string('plotsZtoElecTau_Zee_part02.root')
outputFileNameZee_part03 = cms.string('plotsZtoElecTau_Zee_part03.root')
outputFileNameZee_part04 = cms.string('plotsZtoElecTau_Zee_part04.root')
outputFileNameZee_part05 = cms.string('plotsZtoElecTau_Zee_part05.root')
outputFileNameZee_part06 = cms.string('plotsZtoElecTau_Zee_part06.root')
outputFileNameZee_part07 = cms.string('plotsZtoElecTau_Zee_part07.root')
outputFileNameZee_part08 = cms.string('plotsZtoElecTau_Zee_part08.root')
outputFileNameZee_part09 = cms.string('plotsZtoElecTau_Zee_part09.root')
outputFileNameZee_part10 = cms.string('plotsZtoElecTau_Zee_part10.root')
outputFileNameZee_part11 = cms.string('plotsZtoElecTau_Zee_part11.root')
outputFileNameZee_part12 = cms.string('plotsZtoElecTau_Zee_part12.root')
outputFileNameZee_part13 = cms.string('plotsZtoElecTau_Zee_part13.root')
outputFileNameZee_part14 = cms.string('plotsZtoElecTau_Zee_part14.root')
outputFileNameZee_part15 = cms.string('plotsZtoElecTau_Zee_part15.root')
outputFileNameZee_part16 = cms.string('plotsZtoElecTau_Zee_part16.root')
outputFileNameZee_part17 = cms.string('plotsZtoElecTau_Zee_part17.root')
outputFileNameZee_part18 = cms.string('plotsZtoElecTau_Zee_part18.root')
outputFileNameZee_part19 = cms.string('plotsZtoElecTau_Zee_part19.root')
outputFileNameZee_part20 = cms.string('plotsZtoElecTau_Zee_part20.root')
outputFileNameZee_part21 = cms.string('plotsZtoElecTau_Zee_part21.root')
outputFileNameZee_part22 = cms.string('plotsZtoElecTau_Zee_part22.root')
outputFileNameZee_part23 = cms.string('plotsZtoElecTau_Zee_part23.root')
outputFileNameZee_part24 = cms.string('plotsZtoElecTau_Zee_part24.root')
outputFileNameZee_part25 = cms.string('plotsZtoElecTau_Zee_part25.root')
outputFileNameZee_part26 = cms.string('plotsZtoElecTau_Zee_part26.root')
outputFileNameZee_part27 = cms.string('plotsZtoElecTau_Zee_part27.root')
outputFileNameZee_part28 = cms.string('plotsZtoElecTau_Zee_part28.root')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# electron enriched b/c --> e QCD sample generated with Pythia (20 GeV < Pt(hat) < 30 GeV)
#  /QCD_EMenriched_Pt20to30/Summer08_IDEAL_V11_redigi_v2/GEN-SIM-RECO
#  generated events = 20359765
#  events passing skim = 351697
#  xc = 0.40mb
#  filter eff = 0.008
#  integrated lumi = 6.362 pb^-1
#

intLumiQCD_EMenriched_Pt20to30 = float(6.36)

fileNamesQCD_EMenriched_Pt20to30_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_2.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_4.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_6.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_8.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_9.root',
)
fileNamesQCD_EMenriched_Pt20to30_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_10.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_11.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_12.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_13.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_14.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_15.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_16.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_17.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_18.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_19.root',
)
fileNamesQCD_EMenriched_Pt20to30_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_20.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_21.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_22.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_23.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_24.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_25.root'
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_26.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_27.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_28.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_29.root',
)
fileNamesQCD_EMenriched_Pt20to30_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_30.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_31.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_32.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_33.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_34.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_35.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_36.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_37.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_38.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_39.root',
)
fileNamesQCD_EMenriched_Pt20to30_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_40.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_41.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_42.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_43.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_44.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_45.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_46.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_47.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_48.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_49.root',
)
fileNamesQCD_EMenriched_Pt20to30_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_50.root'
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_51.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_52.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_53.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_54.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_55.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_56.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_57.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_58.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_59.root'
)
fileNamesQCD_EMenriched_Pt20to30_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_50.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_61.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_62.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_63.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_64.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_65.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_66.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_67.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_68.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_69.root'
)
fileNamesQCD_EMenriched_Pt20to30_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_70.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_71.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_72.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_73.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_74.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_75.root'
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_76.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_77.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_78.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_79.root'
)
fileNamesQCD_EMenriched_Pt20to30_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_80.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_81.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_82.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_83.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_84.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_85.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_86.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_87.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_88.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_89.root'
)
fileNamesQCD_EMenriched_Pt20to30_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_90.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_91.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_92.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_93.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_94.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_95.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_96.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_97.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_98.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_99.root'
)
fileNamesQCD_EMenriched_Pt20to30_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_100.root'
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_101.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_102.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_103.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_104.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_105.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_106.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_107.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_108.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_109.root'
)
fileNamesQCD_EMenriched_Pt20to30_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_110.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_111.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_112.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_113.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_114.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_115.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_116.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_117.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_118.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_119.root'
)
fileNamesQCD_EMenriched_Pt20to30_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_120.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_121.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_122.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_123.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_124.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_125.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_126.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_127.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_128.root'
)

outputFileNameQCD_EMenriched_Pt20to30_part01 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt20to30_part01.root')
outputFileNameQCD_EMenriched_Pt20to30_part02 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt20to30_part02.root')
outputFileNameQCD_EMenriched_Pt20to30_part03 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt20to30_part03.root')
outputFileNameQCD_EMenriched_Pt20to30_part04 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt20to30_part04.root')
outputFileNameQCD_EMenriched_Pt20to30_part05 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt20to30_part05.root')
outputFileNameQCD_EMenriched_Pt20to30_part06 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt20to30_part06.root')
outputFileNameQCD_EMenriched_Pt20to30_part07 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt20to30_part07.root')
outputFileNameQCD_EMenriched_Pt20to30_part08 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt20to30_part08.root')
outputFileNameQCD_EMenriched_Pt20to30_part09 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt20to30_part09.root')
outputFileNameQCD_EMenriched_Pt20to30_part10 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt20to30_part10.root')
outputFileNameQCD_EMenriched_Pt20to30_part11 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt20to30_part11.root')
outputFileNameQCD_EMenriched_Pt20to30_part12 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt20to30_part12.root')
outputFileNameQCD_EMenriched_Pt20to30_part13 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt20to30_part13.root')

genPhaseSpaceCutQCD_EMenriched_Pt20to30_part01 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

genPhaseSpaceCutQCD_EMenriched_Pt20to30_part02 = genPhaseSpaceCutQCD_EMenriched_Pt20to30_part01  
genPhaseSpaceCutQCD_EMenriched_Pt20to30_part03 = genPhaseSpaceCutQCD_EMenriched_Pt20to30_part01  
genPhaseSpaceCutQCD_EMenriched_Pt20to30_part04 = genPhaseSpaceCutQCD_EMenriched_Pt20to30_part01  
genPhaseSpaceCutQCD_EMenriched_Pt20to30_part05 = genPhaseSpaceCutQCD_EMenriched_Pt20to30_part01  
genPhaseSpaceCutQCD_EMenriched_Pt20to30_part06 = genPhaseSpaceCutQCD_EMenriched_Pt20to30_part01  
genPhaseSpaceCutQCD_EMenriched_Pt20to30_part07 = genPhaseSpaceCutQCD_EMenriched_Pt20to30_part01  
genPhaseSpaceCutQCD_EMenriched_Pt20to30_part08 = genPhaseSpaceCutQCD_EMenriched_Pt20to30_part01  
genPhaseSpaceCutQCD_EMenriched_Pt20to30_part09 = genPhaseSpaceCutQCD_EMenriched_Pt20to30_part01  
genPhaseSpaceCutQCD_EMenriched_Pt20to30_part10 = genPhaseSpaceCutQCD_EMenriched_Pt20to30_part01  
genPhaseSpaceCutQCD_EMenriched_Pt20to30_part11 = genPhaseSpaceCutQCD_EMenriched_Pt20to30_part01  
genPhaseSpaceCutQCD_EMenriched_Pt20to30_part12 = genPhaseSpaceCutQCD_EMenriched_Pt20to30_part01  
genPhaseSpaceCutQCD_EMenriched_Pt20to30_part13 = genPhaseSpaceCutQCD_EMenriched_Pt20to30_part01  

#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# electron enriched b/c --> e QCD sample generated with Pythia (30 GeV < Pt(hat) < 80 GeV)
#
#  /QCD_EMenriched_Pt30to80/Summer08_IDEAL_V11_redigi_v2/GEN-SIM-RECO
#  generated events = 38298918
#  events passing skim   = 1684837
#  cross section = 0.10mb
#  filter eff = 0.047
#  integrated lumi = 8.15 pb^-1
#
intLumiQCD_EMenriched_Pt30to80 = float(8.15)

fileNamesQCD_EMenriched_Pt30to80_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_2.root'
)
fileNamesQCD_EMenriched_Pt30to80_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_4.root'
)
fileNamesQCD_EMenriched_Pt30to80_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_6.root'
)
fileNamesQCD_EMenriched_Pt30to80_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_8.root'
)
fileNamesQCD_EMenriched_Pt30to80_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_9.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_10.root'
)
fileNamesQCD_EMenriched_Pt30to80_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_11.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_12.root'
)
fileNamesQCD_EMenriched_Pt30to80_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_13.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_14.root'
)
fileNamesQCD_EMenriched_Pt30to80_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_15.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_16.root'
)
fileNamesQCD_EMenriched_Pt30to80_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_17.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_18.root'
)
fileNamesQCD_EMenriched_Pt30to80_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_19.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_20.root'
)
fileNamesQCD_EMenriched_Pt30to80_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_21.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_22.root'
)
fileNamesQCD_EMenriched_Pt30to80_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_23.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_24.root'
)
fileNamesQCD_EMenriched_Pt30to80_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_25.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_26.root'
)
fileNamesQCD_EMenriched_Pt30to80_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_27.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_28.root'
)
fileNamesQCD_EMenriched_Pt30to80_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_29.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_30.root'
)
fileNamesQCD_EMenriched_Pt30to80_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_31.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_32.root'
)
fileNamesQCD_EMenriched_Pt30to80_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_33.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_34.root'
)
fileNamesQCD_EMenriched_Pt30to80_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_35.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_36.root'
)
fileNamesQCD_EMenriched_Pt30to80_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_37.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_38.root'
)
fileNamesQCD_EMenriched_Pt30to80_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_39.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_40.root'
)
fileNamesQCD_EMenriched_Pt30to80_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_41.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_42.root'
)
fileNamesQCD_EMenriched_Pt30to80_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_43.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_44.root'
)
fileNamesQCD_EMenriched_Pt30to80_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_45.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_46.root'
)
fileNamesQCD_EMenriched_Pt30to80_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_47.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_48.root'
)
fileNamesQCD_EMenriched_Pt30to80_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_49.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_50.root'
)
fileNamesQCD_EMenriched_Pt30to80_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_51.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_52.root'
)
fileNamesQCD_EMenriched_Pt30to80_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_53.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_54.root'
)
fileNamesQCD_EMenriched_Pt30to80_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_55.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_56.root'
)
fileNamesQCD_EMenriched_Pt30to80_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_57.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_58.root'
)
fileNamesQCD_EMenriched_Pt30to80_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_59.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_60.root'
)
fileNamesQCD_EMenriched_Pt30to80_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_61.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_62.root'
)
fileNamesQCD_EMenriched_Pt30to80_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_63.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_64.root'
)
fileNamesQCD_EMenriched_Pt30to80_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_65.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_66.root'
)
fileNamesQCD_EMenriched_Pt30to80_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_67.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_68.root'
)
fileNamesQCD_EMenriched_Pt30to80_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_69.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_70.root'
)
fileNamesQCD_EMenriched_Pt30to80_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_71.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_72.root'
)
fileNamesQCD_EMenriched_Pt30to80_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_73.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_74.root'
)
fileNamesQCD_EMenriched_Pt30to80_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_75.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_76.root'
)
fileNamesQCD_EMenriched_Pt30to80_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_77.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_78.root'
)
fileNamesQCD_EMenriched_Pt30to80_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_79.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_80.root'
)
fileNamesQCD_EMenriched_Pt30to80_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_81.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_82.root'
)
fileNamesQCD_EMenriched_Pt30to80_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_83.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_84.root'
)
fileNamesQCD_EMenriched_Pt30to80_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_85.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_86.root'
)
fileNamesQCD_EMenriched_Pt30to80_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_87.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_88.root'
)
fileNamesQCD_EMenriched_Pt30to80_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_89.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_90.root'
)
fileNamesQCD_EMenriched_Pt30to80_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_91.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_92.root'
)
fileNamesQCD_EMenriched_Pt30to80_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_93.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_94.root'
)
fileNamesQCD_EMenriched_Pt30to80_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_95.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_96.root'
)
fileNamesQCD_EMenriched_Pt30to80_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_97.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_98.root'
)
fileNamesQCD_EMenriched_Pt30to80_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_99.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_100.root'
)
fileNamesQCD_EMenriched_Pt30to80_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_101.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_102.root'
)
fileNamesQCD_EMenriched_Pt30to80_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_103.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_104.root'
)
fileNamesQCD_EMenriched_Pt30to80_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_105.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_106.root'
)
fileNamesQCD_EMenriched_Pt30to80_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_107.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_108.root'
)
fileNamesQCD_EMenriched_Pt30to80_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_109.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_110.root'
)
fileNamesQCD_EMenriched_Pt30to80_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_111.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_112.root'
)
fileNamesQCD_EMenriched_Pt30to80_part57 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_113.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_114.root'
)
fileNamesQCD_EMenriched_Pt30to80_part58 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_115.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_116.root'
)
fileNamesQCD_EMenriched_Pt30to80_part59 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_117.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_118.root'
)
fileNamesQCD_EMenriched_Pt30to80_part60 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_119.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_120.root'
)
fileNamesQCD_EMenriched_Pt30to80_part61 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_121.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_122.root'
)
fileNamesQCD_EMenriched_Pt30to80_part62 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_123.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_124.root'
)
fileNamesQCD_EMenriched_Pt30to80_part63 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_125.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_126.root'
)
fileNamesQCD_EMenriched_Pt30to80_part64 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_127.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_128.root'
)
fileNamesQCD_EMenriched_Pt30to80_part65 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_129.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_130.root'
)
fileNamesQCD_EMenriched_Pt30to80_part66 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_131.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_132.root'
)
fileNamesQCD_EMenriched_Pt30to80_part67 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_133.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_134.root'
)
fileNamesQCD_EMenriched_Pt30to80_part68 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_135.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_136.root'
)
fileNamesQCD_EMenriched_Pt30to80_part69 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_137.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_138.root'
)
fileNamesQCD_EMenriched_Pt30to80_part70 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_139.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_140.root'
)
fileNamesQCD_EMenriched_Pt30to80_part71 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_141.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_142.root'
)
fileNamesQCD_EMenriched_Pt30to80_part72 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_143.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_144.root'
)
fileNamesQCD_EMenriched_Pt30to80_part73 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_145.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_146.root'
)
fileNamesQCD_EMenriched_Pt30to80_part74 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_147.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_148.root'
)
fileNamesQCD_EMenriched_Pt30to80_part75 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_149.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_150.root'
)
fileNamesQCD_EMenriched_Pt30to80_part76 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_151.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_152.root'
)
fileNamesQCD_EMenriched_Pt30to80_part77 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_153.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_154.root'
)
fileNamesQCD_EMenriched_Pt30to80_part78 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_155.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_156.root'
)
fileNamesQCD_EMenriched_Pt30to80_part79 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_157.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_158.root'
)
fileNamesQCD_EMenriched_Pt30to80_part80 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_159.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_160.root'
)
fileNamesQCD_EMenriched_Pt30to80_part81 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_161.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_162.root'
)
fileNamesQCD_EMenriched_Pt30to80_part82 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_163.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_164.root'
)
fileNamesQCD_EMenriched_Pt30to80_part83 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_165.root'
)

outputFileNameQCD_EMenriched_Pt30to80_part01 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part01.root')
outputFileNameQCD_EMenriched_Pt30to80_part02 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part02.root')
outputFileNameQCD_EMenriched_Pt30to80_part03 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part03.root')
outputFileNameQCD_EMenriched_Pt30to80_part04 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part04.root')
outputFileNameQCD_EMenriched_Pt30to80_part05 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part05.root')
outputFileNameQCD_EMenriched_Pt30to80_part06 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part06.root')
outputFileNameQCD_EMenriched_Pt30to80_part07 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part07.root')
outputFileNameQCD_EMenriched_Pt30to80_part08 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part08.root')
outputFileNameQCD_EMenriched_Pt30to80_part09 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part09.root')
outputFileNameQCD_EMenriched_Pt30to80_part10 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part10.root')
outputFileNameQCD_EMenriched_Pt30to80_part11 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part11.root')
outputFileNameQCD_EMenriched_Pt30to80_part12 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part12.root')
outputFileNameQCD_EMenriched_Pt30to80_part13 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part13.root')
outputFileNameQCD_EMenriched_Pt30to80_part14 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part14.root')
outputFileNameQCD_EMenriched_Pt30to80_part15 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part15.root')
outputFileNameQCD_EMenriched_Pt30to80_part16 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part16.root')
outputFileNameQCD_EMenriched_Pt30to80_part17 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part17.root')
outputFileNameQCD_EMenriched_Pt30to80_part18 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part18.root')
outputFileNameQCD_EMenriched_Pt30to80_part19 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part19.root')
outputFileNameQCD_EMenriched_Pt30to80_part20 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part20.root')
outputFileNameQCD_EMenriched_Pt30to80_part21 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part21.root')
outputFileNameQCD_EMenriched_Pt30to80_part22 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part22.root')
outputFileNameQCD_EMenriched_Pt30to80_part23 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part23.root')
outputFileNameQCD_EMenriched_Pt30to80_part24 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part24.root')
outputFileNameQCD_EMenriched_Pt30to80_part25 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part25.root')
outputFileNameQCD_EMenriched_Pt30to80_part26 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part26.root')
outputFileNameQCD_EMenriched_Pt30to80_part27 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part27.root')
outputFileNameQCD_EMenriched_Pt30to80_part28 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part28.root')
outputFileNameQCD_EMenriched_Pt30to80_part29 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part29.root')
outputFileNameQCD_EMenriched_Pt30to80_part30 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part30.root')
outputFileNameQCD_EMenriched_Pt30to80_part31 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part31.root')
outputFileNameQCD_EMenriched_Pt30to80_part32 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part32.root')
outputFileNameQCD_EMenriched_Pt30to80_part33 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part33.root')
outputFileNameQCD_EMenriched_Pt30to80_part34 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part34.root')
outputFileNameQCD_EMenriched_Pt30to80_part35 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part35.root')
outputFileNameQCD_EMenriched_Pt30to80_part36 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part36.root')
outputFileNameQCD_EMenriched_Pt30to80_part37 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part37.root')
outputFileNameQCD_EMenriched_Pt30to80_part38 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part38.root')
outputFileNameQCD_EMenriched_Pt30to80_part39 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part39.root')
outputFileNameQCD_EMenriched_Pt30to80_part40 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part40.root')
outputFileNameQCD_EMenriched_Pt30to80_part41 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part41.root')
outputFileNameQCD_EMenriched_Pt30to80_part42 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part42.root')
outputFileNameQCD_EMenriched_Pt30to80_part43 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part43.root')
outputFileNameQCD_EMenriched_Pt30to80_part44 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part44.root')
outputFileNameQCD_EMenriched_Pt30to80_part45 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part45.root')
outputFileNameQCD_EMenriched_Pt30to80_part46 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part46.root')
outputFileNameQCD_EMenriched_Pt30to80_part47 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part47.root')
outputFileNameQCD_EMenriched_Pt30to80_part48 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part48.root')
outputFileNameQCD_EMenriched_Pt30to80_part49 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part49.root')
outputFileNameQCD_EMenriched_Pt30to80_part50 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part50.root')
outputFileNameQCD_EMenriched_Pt30to80_part51 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part51.root')
outputFileNameQCD_EMenriched_Pt30to80_part52 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part52.root')
outputFileNameQCD_EMenriched_Pt30to80_part53 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part53.root')
outputFileNameQCD_EMenriched_Pt30to80_part54 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part54.root')
outputFileNameQCD_EMenriched_Pt30to80_part55 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part55.root')
outputFileNameQCD_EMenriched_Pt30to80_part56 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part56.root')
outputFileNameQCD_EMenriched_Pt30to80_part57 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part57.root')
outputFileNameQCD_EMenriched_Pt30to80_part58 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part58.root')
outputFileNameQCD_EMenriched_Pt30to80_part59 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part59.root')
outputFileNameQCD_EMenriched_Pt30to80_part60 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part60.root')
outputFileNameQCD_EMenriched_Pt30to80_part61 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part61.root')
outputFileNameQCD_EMenriched_Pt30to80_part62 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part62.root')
outputFileNameQCD_EMenriched_Pt30to80_part63 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part63.root')
outputFileNameQCD_EMenriched_Pt30to80_part64 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part64.root')
outputFileNameQCD_EMenriched_Pt30to80_part65 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part65.root')
outputFileNameQCD_EMenriched_Pt30to80_part66 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part66.root')
outputFileNameQCD_EMenriched_Pt30to80_part67 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part67.root')
outputFileNameQCD_EMenriched_Pt30to80_part68 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part68.root')
outputFileNameQCD_EMenriched_Pt30to80_part69 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part69.root')
outputFileNameQCD_EMenriched_Pt30to80_part70 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part70.root')
outputFileNameQCD_EMenriched_Pt30to80_part71 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part71.root')
outputFileNameQCD_EMenriched_Pt30to80_part72 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part72.root')
outputFileNameQCD_EMenriched_Pt30to80_part73 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part73.root')
outputFileNameQCD_EMenriched_Pt30to80_part74 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part74.root')
outputFileNameQCD_EMenriched_Pt30to80_part75 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part75.root')
outputFileNameQCD_EMenriched_Pt30to80_part76 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part76.root')
outputFileNameQCD_EMenriched_Pt30to80_part77 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part77.root')
outputFileNameQCD_EMenriched_Pt30to80_part78 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part78.root')
outputFileNameQCD_EMenriched_Pt30to80_part79 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part79.root')
outputFileNameQCD_EMenriched_Pt30to80_part80 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part80.root')
outputFileNameQCD_EMenriched_Pt30to80_part81 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part81.root')
outputFileNameQCD_EMenriched_Pt30to80_part82 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part82.root')
outputFileNameQCD_EMenriched_Pt30to80_part83 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part83.root')

genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

genPhaseSpaceCutQCD_EMenriched_Pt30to80_part02 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part03 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part04 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part05 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part06 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part07 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part08 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part09 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part10 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part11 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part12 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part13 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part14 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part15 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part16 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part17 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part18 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part19 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part20 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part21 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part22 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part23 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part24 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part25 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part26 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part27 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part28 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part29 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part30 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part31 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part32 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part33 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part34 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part35 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part36 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part37 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part38 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part39 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part40 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part41 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part42 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part43 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part44 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part45 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part46 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part47 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part48 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part49 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part50 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part51 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part52 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part53 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part54 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part55 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part56 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part57 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part58 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part59 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part60 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part61 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part62 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part63 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part64 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part65 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part66 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part67 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part68 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part69 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part70 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part71 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part72 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part73 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part74 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part75 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part76 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part77 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part78 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part79 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part80 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part81 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part82 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  
genPhaseSpaceCutQCD_EMenriched_Pt30to80_part83 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01  

#--------------------------------------------------------------------------------




#--------------------------------------------------------------------------------
# electron enriched b/c --> e QCD sample generated with Pythia (80 GeV < Pt(hat) < 170 GeV)
#
#  /QCD_EMenriched_Pt80to170/Summer08_IDEAL_V11_redigi_v1/GEN-SIM-RECO
#  generated events = 5970425
#  events passing skim = 518076 
#  cross section = 0.0019mb
#  filter eff = 0.15
#  raw integrated lumi = 20.95
#  6/90 bad skim output files ( 13,25,32,34,77,90 ) - ~420000 ev in bad files 
#  correction factor for missing files = 0.930
#  corrected integrated luminosity = 19.5 pb^-1
#
#  84 good skim output files: 518k ev, 23 jobs, 22k ev/job, 
#
intLumiQCD_EMenriched_Pt80to170 = float(19.5)

fileNamesQCD_EMenriched_Pt80to170_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_2.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_4.root',
)
fileNamesQCD_EMenriched_Pt80to170_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_6.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_8.root',
)
fileNamesQCD_EMenriched_Pt80to170_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_9.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_10.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_11.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_12.root',
)
fileNamesQCD_EMenriched_Pt80to170_part04 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_13.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_14.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_14.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_16.root',
)
fileNamesQCD_EMenriched_Pt80to170_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_17.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_18.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_19.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_20.root',
)
fileNamesQCD_EMenriched_Pt80to170_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_21.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_22.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_23.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_24.root',
)
fileNamesQCD_EMenriched_Pt80to170_part07 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_25.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_26.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_27.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_28.root',
)
fileNamesQCD_EMenriched_Pt80to170_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_29.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_20.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_31.root'
#	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_32.root',
)
fileNamesQCD_EMenriched_Pt80to170_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_33.root',
#	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_34.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_35.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_36.root',
)
fileNamesQCD_EMenriched_Pt80to170_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_37.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_38.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_39.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_40.root',
)
fileNamesQCD_EMenriched_Pt80to170_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_41.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_42.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_43.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_44.root',
)
fileNamesQCD_EMenriched_Pt80to170_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_45.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_46.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_47.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_48.root',
)
fileNamesQCD_EMenriched_Pt80to170_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_49.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_50.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_51.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_52.root',
)
fileNamesQCD_EMenriched_Pt80to170_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_53.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_54.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_55.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_56.root',
)
fileNamesQCD_EMenriched_Pt80to170_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_57.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_58.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_59.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_60.root',
)
fileNamesQCD_EMenriched_Pt80to170_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_61.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_62.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_63.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_64.root',
)
fileNamesQCD_EMenriched_Pt80to170_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_65.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_66.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_67.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_68.root',
)
fileNamesQCD_EMenriched_Pt80to170_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_69.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_70.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_71.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_72.root',
)
fileNamesQCD_EMenriched_Pt80to170_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_73.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_74.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_75.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_76.root',
)
fileNamesQCD_EMenriched_Pt80to170_part20 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_77.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_78.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_79.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_80.root',
)
fileNamesQCD_EMenriched_Pt80to170_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_81.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_82.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_83.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_84.root',
)
fileNamesQCD_EMenriched_Pt80to170_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_85.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_86.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_87.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_88.root',
)
fileNamesQCD_EMenriched_Pt80to170_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_89.root'
#	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_90.root'
)

outputFileNameQCD_EMenriched_Pt80to170_part01 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part01.root')
outputFileNameQCD_EMenriched_Pt80to170_part02 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part02.root')
outputFileNameQCD_EMenriched_Pt80to170_part03 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part03.root')
outputFileNameQCD_EMenriched_Pt80to170_part04 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part04.root')
outputFileNameQCD_EMenriched_Pt80to170_part05 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part05.root')
outputFileNameQCD_EMenriched_Pt80to170_part06 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part06.root')
outputFileNameQCD_EMenriched_Pt80to170_part07 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part07.root')
outputFileNameQCD_EMenriched_Pt80to170_part08 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part08.root')
outputFileNameQCD_EMenriched_Pt80to170_part09 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part09.root')
outputFileNameQCD_EMenriched_Pt80to170_part10 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part10.root')
outputFileNameQCD_EMenriched_Pt80to170_part11 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part11.root')
outputFileNameQCD_EMenriched_Pt80to170_part12 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part12.root')
outputFileNameQCD_EMenriched_Pt80to170_part13 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part13.root')
outputFileNameQCD_EMenriched_Pt80to170_part14 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part14.root')
outputFileNameQCD_EMenriched_Pt80to170_part15 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part15.root')
outputFileNameQCD_EMenriched_Pt80to170_part16 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part16.root')
outputFileNameQCD_EMenriched_Pt80to170_part17 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part17.root')
outputFileNameQCD_EMenriched_Pt80to170_part18 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part18.root')
outputFileNameQCD_EMenriched_Pt80to170_part19 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part19.root')
outputFileNameQCD_EMenriched_Pt80to170_part20 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part20.root')
outputFileNameQCD_EMenriched_Pt80to170_part21 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part21.root')
outputFileNameQCD_EMenriched_Pt80to170_part22 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part22.root')
outputFileNameQCD_EMenriched_Pt80to170_part23 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part23.root')

genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

genPhaseSpaceCutQCD_EMenriched_Pt80to170_part02 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  
genPhaseSpaceCutQCD_EMenriched_Pt80to170_part03 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  
genPhaseSpaceCutQCD_EMenriched_Pt80to170_part04 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  
genPhaseSpaceCutQCD_EMenriched_Pt80to170_part05 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  
genPhaseSpaceCutQCD_EMenriched_Pt80to170_part06 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  
genPhaseSpaceCutQCD_EMenriched_Pt80to170_part07 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  
genPhaseSpaceCutQCD_EMenriched_Pt80to170_part08 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  
genPhaseSpaceCutQCD_EMenriched_Pt80to170_part09 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  
genPhaseSpaceCutQCD_EMenriched_Pt80to170_part10 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  
genPhaseSpaceCutQCD_EMenriched_Pt80to170_part11 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  
genPhaseSpaceCutQCD_EMenriched_Pt80to170_part12 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  
genPhaseSpaceCutQCD_EMenriched_Pt80to170_part13 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  
genPhaseSpaceCutQCD_EMenriched_Pt80to170_part14 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  
genPhaseSpaceCutQCD_EMenriched_Pt80to170_part15 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  
genPhaseSpaceCutQCD_EMenriched_Pt80to170_part16 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  
genPhaseSpaceCutQCD_EMenriched_Pt80to170_part17 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  
genPhaseSpaceCutQCD_EMenriched_Pt80to170_part18 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  
genPhaseSpaceCutQCD_EMenriched_Pt80to170_part19 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  
genPhaseSpaceCutQCD_EMenriched_Pt80to170_part20 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  
genPhaseSpaceCutQCD_EMenriched_Pt80to170_part21 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  
genPhaseSpaceCutQCD_EMenriched_Pt80to170_part22 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  
genPhaseSpaceCutQCD_EMenriched_Pt80to170_part23 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  
genPhaseSpaceCutQCD_EMenriched_Pt80to170_part24 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01  

#--------------------------------------------------------------------------------




#--------------------------------------------------------------------------------
# QCD b/c --> e sample generated with Pythia (20 GeV < Pt(hat) < 30 GeV)
#
#  /QCD_BCtoE_Pt20to30/Summer08_IDEAL_V11_redigi_v1/GEN-SIM-RECO
#  generated events = 1997072
#  events passing skim = 434810
#  cross section = 0.04mb
#  filter eff = 0.00048
#  integrated luminosity = 10.4 pb^-1
# 
#  435k events, 48 files, 3 files/job, 16 jobs, 27k ev/job
#
intLumiQCD_BCtoE_Pt20to30 = float(10.4)

fileNamesQCD_BCtoE_Pt20to30_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_2.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_3.root'
)
fileNamesQCD_BCtoE_Pt20to30_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_4.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_6.root'
)
fileNamesQCD_BCtoE_Pt20to30_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_8.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_9.root'
)
fileNamesQCD_BCtoE_Pt20to30_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_10.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_11.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_12.root'
)
fileNamesQCD_BCtoE_Pt20to30_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_13.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_14.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_15.root'
)
fileNamesQCD_BCtoE_Pt20to30_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_16.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_17.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_18.root'
)
fileNamesQCD_BCtoE_Pt20to30_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_19.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_20.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_21.root'
)
fileNamesQCD_BCtoE_Pt20to30_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_22.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_23.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_24.root'
)
fileNamesQCD_BCtoE_Pt20to30_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_25.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_26.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_27.root'
)
fileNamesQCD_BCtoE_Pt20to30_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_28.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_29.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_30.root'
)
fileNamesQCD_BCtoE_Pt20to30_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_31.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_32.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_33.root'
)
fileNamesQCD_BCtoE_Pt20to30_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_34.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_35.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_36.root'
)
fileNamesQCD_BCtoE_Pt20to30_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_37.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_38.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_39.root'
)
fileNamesQCD_BCtoE_Pt20to30_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_40.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_41.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_42.root'
)
fileNamesQCD_BCtoE_Pt20to30_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_43.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_44.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_45.root'
)
fileNamesQCD_BCtoE_Pt20to30_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_46.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_47.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_48.root'
)

genPhaseSpaceCutQCD_BCtoE_Pt20to30_part01 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_BCtoE_Pt20to30_part01 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_part01.root')
outputFileNameQCD_BCtoE_Pt20to30_part02 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_part02.root')
outputFileNameQCD_BCtoE_Pt20to30_part03 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_part03.root')
outputFileNameQCD_BCtoE_Pt20to30_part04 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_part04.root')
outputFileNameQCD_BCtoE_Pt20to30_part05 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_part05.root')
outputFileNameQCD_BCtoE_Pt20to30_part06 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_part06.root')
outputFileNameQCD_BCtoE_Pt20to30_part07 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_part07.root')
outputFileNameQCD_BCtoE_Pt20to30_part08 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_part08.root')
outputFileNameQCD_BCtoE_Pt20to30_part09 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_part09.root')
outputFileNameQCD_BCtoE_Pt20to30_part10 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_part10.root')
outputFileNameQCD_BCtoE_Pt20to30_part11 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_part11.root')
outputFileNameQCD_BCtoE_Pt20to30_part12 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_part12.root')
outputFileNameQCD_BCtoE_Pt20to30_part13 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_part13.root')
outputFileNameQCD_BCtoE_Pt20to30_part14 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_part14.root')
outputFileNameQCD_BCtoE_Pt20to30_part15 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_part15.root')
outputFileNameQCD_BCtoE_Pt20to30_part16 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_part16.root')

genPhaseSpaceCutQCD_BCtoE_Pt20to30_part02 = genPhaseSpaceCutQCD_BCtoE_Pt20to30_part01  
genPhaseSpaceCutQCD_BCtoE_Pt20to30_part03 = genPhaseSpaceCutQCD_BCtoE_Pt20to30_part01  
genPhaseSpaceCutQCD_BCtoE_Pt20to30_part04 = genPhaseSpaceCutQCD_BCtoE_Pt20to30_part01  
genPhaseSpaceCutQCD_BCtoE_Pt20to30_part05 = genPhaseSpaceCutQCD_BCtoE_Pt20to30_part01  
genPhaseSpaceCutQCD_BCtoE_Pt20to30_part06 = genPhaseSpaceCutQCD_BCtoE_Pt20to30_part01  
genPhaseSpaceCutQCD_BCtoE_Pt20to30_part07 = genPhaseSpaceCutQCD_BCtoE_Pt20to30_part01  
genPhaseSpaceCutQCD_BCtoE_Pt20to30_part08 = genPhaseSpaceCutQCD_BCtoE_Pt20to30_part01  
genPhaseSpaceCutQCD_BCtoE_Pt20to30_part09 = genPhaseSpaceCutQCD_BCtoE_Pt20to30_part01  
genPhaseSpaceCutQCD_BCtoE_Pt20to30_part10 = genPhaseSpaceCutQCD_BCtoE_Pt20to30_part01  
genPhaseSpaceCutQCD_BCtoE_Pt20to30_part11 = genPhaseSpaceCutQCD_BCtoE_Pt20to30_part01  
genPhaseSpaceCutQCD_BCtoE_Pt20to30_part12 = genPhaseSpaceCutQCD_BCtoE_Pt20to30_part01  
genPhaseSpaceCutQCD_BCtoE_Pt20to30_part13 = genPhaseSpaceCutQCD_BCtoE_Pt20to30_part01  
genPhaseSpaceCutQCD_BCtoE_Pt20to30_part14 = genPhaseSpaceCutQCD_BCtoE_Pt20to30_part01  
genPhaseSpaceCutQCD_BCtoE_Pt20to30_part15 = genPhaseSpaceCutQCD_BCtoE_Pt20to30_part01  
genPhaseSpaceCutQCD_BCtoE_Pt20to30_part16 = genPhaseSpaceCutQCD_BCtoE_Pt20to30_part01  

#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# QCD b/c -->e sample generated with Pythia (30 GeV < Pt(hat) < 80 GeV)
#
#  /QCD_BCtoE_Pt30to80/Summer08_IDEAL_V11_redigi_v1/GEN-SIM-RECO
#  generated events = 2016487
#  events passing skim = 653284
#  cross section = 0.10mb
#  filter eff = 0.0024
#  integrated luminosity = 8.40 pb^-1
#
#  654k ev, 406 files, 27 jobs, 24k ev/job, 15 files/job
#
intLumiQCD_BCtoE_Pt30to80 = float(8.40)

fileNamesQCD_BCtoE_Pt30to80_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_2.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_4.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_6.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_8.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_9.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_10.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_11.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_12.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_13.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_14.root'
)
fileNamesQCD_BCtoE_Pt30to80_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_15.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_16.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_17.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_18.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_19.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_20.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_21.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_22.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_23.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_24.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_25.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_26.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_27.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_28.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_29.root'
)
fileNamesQCD_BCtoE_Pt30to80_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_30.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_31.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_32.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_33.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_34.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_35.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_36.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_37.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_38.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_39.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_40.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_41.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_42.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_43.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_44.root'
)
fileNamesQCD_BCtoE_Pt30to80_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_45.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_46.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_47.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_48.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_49.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_50.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_51.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_52.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_53.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_54.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_55.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_56.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_57.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_58.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_59.root'
)
fileNamesQCD_BCtoE_Pt30to80_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_60.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_61.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_62.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_63.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_64.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_65.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_66.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_67.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_68.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_69.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_70.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_71.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_72.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_73.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_74.root'
)
fileNamesQCD_BCtoE_Pt30to80_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_75.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_76.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_77.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_78.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_79.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_80.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_81.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_82.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_83.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_84.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_85.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_86.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_87.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_88.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_89.root'
)
fileNamesQCD_BCtoE_Pt30to80_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_90.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_91.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_92.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_93.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_94.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_95.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_96.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_97.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_98.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_99.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_100.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_101.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_102.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_103.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_104.root'
)
fileNamesQCD_BCtoE_Pt30to80_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_105.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_106.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_107.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_108.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_109.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_110.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_111.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_112.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_113.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_114.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_115.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_116.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_117.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_118.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_119.root'
)
fileNamesQCD_BCtoE_Pt30to80_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_120.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_121.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_122.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_123.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_124.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_125.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_126.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_127.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_128.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_129.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_130.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_131.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_132.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_133.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_134.root'
)
fileNamesQCD_BCtoE_Pt30to80_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_135.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_136.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_137.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_138.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_139.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_140.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_141.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_142.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_143.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_144.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_145.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_146.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_147.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_148.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_149.root'
)
fileNamesQCD_BCtoE_Pt30to80_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_150.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_151.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_152.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_153.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_154.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_155.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_156.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_157.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_158.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_159.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_160.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_161.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_162.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_163.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_164.root'
)
fileNamesQCD_BCtoE_Pt30to80_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_165.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_166.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_167.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_168.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_169.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_170.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_171.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_172.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_173.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_174.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_175.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_176.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_177.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_178.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_179.root'
)
fileNamesQCD_BCtoE_Pt30to80_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_180.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_181.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_182.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_183.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_184.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_185.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_186.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_187.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_188.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_189.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_190.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_191.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_192.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_193.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_194.root'
)
fileNamesQCD_BCtoE_Pt30to80_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_195.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_196.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_197.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_198.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_199.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_200.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_201.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_202.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_203.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_204.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_205.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_206.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_207.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_208.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_209.root'
)
fileNamesQCD_BCtoE_Pt30to80_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_210.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_211.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_212.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_213.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_214.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_215.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_216.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_217.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_218.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_219.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_220.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_221.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_222.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_223.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_224.root'
)
fileNamesQCD_BCtoE_Pt30to80_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_225.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_226.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_227.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_228.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_229.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_230.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_231.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_232.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_233.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_234.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_235.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_236.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_237.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_238.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_239.root'
)
fileNamesQCD_BCtoE_Pt30to80_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_240.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_241.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_242.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_243.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_244.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_245.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_246.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_247.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_248.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_249.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_250.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_251.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_252.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_253.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_254.root'
)
fileNamesQCD_BCtoE_Pt30to80_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_255.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_256.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_257.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_258.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_259.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_260.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_261.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_262.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_263.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_264.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_265.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_266.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_267.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_268.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_269.root'
)
fileNamesQCD_BCtoE_Pt30to80_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_270.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_271.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_272.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_273.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_274.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_275.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_276.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_277.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_278.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_279.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_280.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_281.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_282.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_283.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_284.root'
)
fileNamesQCD_BCtoE_Pt30to80_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_285.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_286.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_287.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_288.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_289.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_290.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_291.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_292.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_293.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_294.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_295.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_296.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_297.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_298.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_299.root'
)
fileNamesQCD_BCtoE_Pt30to80_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_300.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_301.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_302.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_303.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_304.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_305.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_306.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_307.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_308.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_309.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_310.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_311.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_312.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_313.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_314.root'
)
fileNamesQCD_BCtoE_Pt30to80_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_315.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_316.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_317.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_318.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_319.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_320.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_321.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_322.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_323.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_324.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_325.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_326.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_327.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_328.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_329.root'
)
fileNamesQCD_BCtoE_Pt30to80_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_330.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_331.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_332.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_333.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_334.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_335.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_336.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_337.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_338.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_339.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_340.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_341.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_342.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_343.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_344.root'
)
fileNamesQCD_BCtoE_Pt30to80_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_345.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_346.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_347.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_348.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_349.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_350.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_351.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_352.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_353.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_354.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_355.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_356.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_357.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_358.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_359.root'
)
fileNamesQCD_BCtoE_Pt30to80_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_360.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_361.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_362.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_363.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_364.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_365.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_366.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_367.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_368.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_369.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_370.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_371.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_372.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_373.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_374.root'
)
fileNamesQCD_BCtoE_Pt30to80_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_375.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_376.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_377.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_378.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_379.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_380.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_381.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_382.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_383.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_384.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_385.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_386.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_387.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_388.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_389.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_390.root'
)
fileNamesQCD_BCtoE_Pt30to80_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_391.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_392.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_393.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_394.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_395.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_396.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_397.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_398.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_399.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_400.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_401.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_402.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_403.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_404.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_405.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_406.root'
)

outputFileNameQCD_BCtoE_Pt30to80_part01 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part01.root')
outputFileNameQCD_BCtoE_Pt30to80_part02 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part02.root')
outputFileNameQCD_BCtoE_Pt30to80_part03 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part03.root')
outputFileNameQCD_BCtoE_Pt30to80_part04 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part04.root')
outputFileNameQCD_BCtoE_Pt30to80_part05 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part05.root')
outputFileNameQCD_BCtoE_Pt30to80_part06 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part06.root')
outputFileNameQCD_BCtoE_Pt30to80_part07 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part07.root')
outputFileNameQCD_BCtoE_Pt30to80_part08 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part08.root')
outputFileNameQCD_BCtoE_Pt30to80_part09 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part09.root')
outputFileNameQCD_BCtoE_Pt30to80_part10 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part10.root')
outputFileNameQCD_BCtoE_Pt30to80_part11 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part11.root')
outputFileNameQCD_BCtoE_Pt30to80_part12 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part12.root')
outputFileNameQCD_BCtoE_Pt30to80_part13 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part13.root')
outputFileNameQCD_BCtoE_Pt30to80_part14 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part14.root')
outputFileNameQCD_BCtoE_Pt30to80_part15 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part15.root')
outputFileNameQCD_BCtoE_Pt30to80_part16 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part16.root')
outputFileNameQCD_BCtoE_Pt30to80_part17 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part17.root')
outputFileNameQCD_BCtoE_Pt30to80_part18 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part18.root')
outputFileNameQCD_BCtoE_Pt30to80_part19 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part19.root')
outputFileNameQCD_BCtoE_Pt30to80_part20 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part20.root')
outputFileNameQCD_BCtoE_Pt30to80_part21 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part21.root')
outputFileNameQCD_BCtoE_Pt30to80_part22 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part22.root')
outputFileNameQCD_BCtoE_Pt30to80_part23 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part23.root')
outputFileNameQCD_BCtoE_Pt30to80_part24 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part24.root')
outputFileNameQCD_BCtoE_Pt30to80_part25 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part25.root')
outputFileNameQCD_BCtoE_Pt30to80_part26 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part26.root')
outputFileNameQCD_BCtoE_Pt30to80_part27 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part27.root')

genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

genPhaseSpaceCutQCD_BCtoE_Pt30to80_part02 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part03 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part04 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part05 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part06 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part07 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part08 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part09 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part10 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part11 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part12 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part13 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part14 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part15 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part16 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part17 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part18 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part19 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part20 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part21 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part22 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part23 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part24 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part25 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part26 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  
genPhaseSpaceCutQCD_BCtoE_Pt30to80_part27 = genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01  

#--------------------------------------------------------------------------------





#--------------------------------------------------------------------------------
#  QCD b/c -->e sample generated with Pythia (80 GeV < Pt(hat) < 170 GeV)
#
#  dataset = /QCD_BCtoE_Pt80to170/Summer08_IDEAL_V11_redigi_v1/GEN-SIM-RECO
#  generated events = 1075822
#  events passing skim = 319959
#  cross section = 0.0019mb
#  filter eff = 0.012
#  integrated luminosity = 47.2 pb^-1
#
#  320k ev, 114 files, 10 files/job, 12 jobs
#
intLumiQCD_BCtoE_Pt30to80 = float(47.2)

fileNamesQCD_BCtoE_Pt80to170_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_2.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_4.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_6.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_8.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_9.root'
)
fileNamesQCD_BCtoE_Pt80to170_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_10.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_11.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_12.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_13.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_14.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_15.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_16.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_17.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_18.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_19.root'
)
fileNamesQCD_BCtoE_Pt80to170_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_20.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_21.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_22.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_23.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_24.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_25.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_26.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_27.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_28.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_29.root'
)
fileNamesQCD_BCtoE_Pt80to170_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_30.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_31.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_32.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_33.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_34.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_35.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_36.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_37.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_38.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_39.root'
)
fileNamesQCD_BCtoE_Pt80to170_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_40.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_41.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_42.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_43.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_44.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_45.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_46.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_47.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_48.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_49.root'
)
fileNamesQCD_BCtoE_Pt80to170_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_50.root'
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_51.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_52.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_53.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_54.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_55.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_56.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_57.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_58.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_59.root'
)
fileNamesQCD_BCtoE_Pt80to170_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_60.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_61.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_62.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_63.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_64.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_65.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_66.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_67.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_68.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_69.root'
)
fileNamesQCD_BCtoE_Pt80to170_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_70.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_71.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_72.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_73.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_74.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_75.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_76.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_77.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_78.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_79.root'
)
fileNamesQCD_BCtoE_Pt80to170_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_80.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_81.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_82.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_83.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_84.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_85.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_86.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_87.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_88.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_89.root'
)
fileNamesQCD_BCtoE_Pt80to170_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_90.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_91.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_92.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_93.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_94.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_95.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_96.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_97.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_98.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_99.root'
)
fileNamesQCD_BCtoE_Pt80to170_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_100.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_101.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_102.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_103.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_104.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_105.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_106.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_107.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_108.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_109.root'
)
fileNamesQCD_BCtoE_Pt80to170_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_110.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_111.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_112.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_113.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_114.root'
)

outputFileNameQCD_BCtoE_Pt80to170_part01 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt80to170_part01.root')
outputFileNameQCD_BCtoE_Pt80to170_part02 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt80to170_part02.root')
outputFileNameQCD_BCtoE_Pt80to170_part03 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt80to170_part03.root')
outputFileNameQCD_BCtoE_Pt80to170_part04 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt80to170_part04.root')
outputFileNameQCD_BCtoE_Pt80to170_part05 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt80to170_part05.root')
outputFileNameQCD_BCtoE_Pt80to170_part06 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt80to170_part06.root')
outputFileNameQCD_BCtoE_Pt80to170_part07 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt80to170_part07.root')
outputFileNameQCD_BCtoE_Pt80to170_part08 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt80to170_part08.root')
outputFileNameQCD_BCtoE_Pt80to170_part09 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt80to170_part09.root')
outputFileNameQCD_BCtoE_Pt80to170_part10 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt80to170_part10.root')
outputFileNameQCD_BCtoE_Pt80to170_part11 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt80to170_part11.root')
outputFileNameQCD_BCtoE_Pt80to170_part12 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt80to170_part12.root')

genPhaseSpaceCutQCD_BCtoE_Pt80to170_part01 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

genPhaseSpaceCutQCD_BCtoE_Pt80to170_part02 = genPhaseSpaceCutQCD_BCtoE_Pt80to170_part01  
genPhaseSpaceCutQCD_BCtoE_Pt80to170_part03 = genPhaseSpaceCutQCD_BCtoE_Pt80to170_part01  
genPhaseSpaceCutQCD_BCtoE_Pt80to170_part04 = genPhaseSpaceCutQCD_BCtoE_Pt80to170_part01  
genPhaseSpaceCutQCD_BCtoE_Pt80to170_part05 = genPhaseSpaceCutQCD_BCtoE_Pt80to170_part01  
genPhaseSpaceCutQCD_BCtoE_Pt80to170_part06 = genPhaseSpaceCutQCD_BCtoE_Pt80to170_part01  
genPhaseSpaceCutQCD_BCtoE_Pt80to170_part07 = genPhaseSpaceCutQCD_BCtoE_Pt80to170_part01  
genPhaseSpaceCutQCD_BCtoE_Pt80to170_part08 = genPhaseSpaceCutQCD_BCtoE_Pt80to170_part01  
genPhaseSpaceCutQCD_BCtoE_Pt80to170_part09 = genPhaseSpaceCutQCD_BCtoE_Pt80to170_part01  
genPhaseSpaceCutQCD_BCtoE_Pt80to170_part10 = genPhaseSpaceCutQCD_BCtoE_Pt80to170_part01  
genPhaseSpaceCutQCD_BCtoE_Pt80to170_part11 = genPhaseSpaceCutQCD_BCtoE_Pt80to170_part01  
genPhaseSpaceCutQCD_BCtoE_Pt80to170_part12 = genPhaseSpaceCutQCD_BCtoE_Pt80to170_part01  

#--------------------------------------------------------------------------------






#--------------------------------------------------------------------------------
# photon + jets samples
# 500k events total, PDF set is CTEQ5L, filter on photons in central region (-2.4 < eta(gamma) < 2.4, ET(gamma) > 15 GeV/c)
#
# datasets = /PYTHIA8PhotonJetPtXXtoYY/Summer08_IDEAL_V11_redigi_v1/GEN-SIM-RECO
#
fileNamesPhotonJets_Pt15to20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/PhotonJets_Pt15to20/skimElecTau_PhotonJets_Pt15to20_1.root'
)
fileNamesPhotonJets_Pt20to25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/PhotonJets_Pt20to25/skimElecTau_PhotonJets_Pt20to25_1.root'
)
fileNamesPhotonJets_Pt25to30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/PhotonJets_Pt25to30/skimElecTau_PhotonJets_Pt25to30_1.root'
)
fileNamesPhotonJets_Pt30to35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/PhotonJets_Pt30to35/skimElecTau_PhotonJets_Pt30to35_1.root'
)
fileNamesPhotonJets_PtGt35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/PhotonJets_Pt35/skimElecTau_PhotonJets_Pt35_1.root'
)

intLumiPhotonJets_Pt15to20 = float(1.075)
intLumiPhotonJets_Pt20to25 = float(3.38)
intLumiPhotonJets_Pt25to30 = float(8.95)
intLumiPhotonJets_Pt30to35 = float(21.89)
intLumiPhotonJets_PtGt35     = float(13.36)

genPhaseSpaceCutPhotonJets_Pt15to20 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)
genPhaseSpaceCutPhotonJets_Pt20to25 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)
genPhaseSpaceCutPhotonJets_Pt25to30 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)
genPhaseSpaceCutPhotonJets_Pt30to35 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)
genPhaseSpaceCutPhotonJets_PtGt35 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNamePhotonJets_Pt15to20 = cms.string('plotsZtoElecTau_PhotonJets_Pt15to20.root')
outputFileNamePhotonJets_Pt20to25 = cms.string('plotsZtoElecTau_PhotonJets_Pt20to25.root')
outputFileNamePhotonJets_Pt25to30 = cms.string('plotsZtoElecTau_PhotonJets_Pt25to30.root')
outputFileNamePhotonJets_Pt30to35 = cms.string('plotsZtoElecTau_PhotonJets_Pt30to35.root')
outputFileNamePhotonJets_PtGt35   = cms.string('plotsZtoElecTau_PhotonJets_PtGt35.root')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#  W + jets sample generated with Madgraph
#  dataset = /WJets-madgraph/Summer08_IDEAL_V11_redigi_v1/GEN-SIM-RECO
#  generated events = 9745661
#  events passing skim = 416085
#  cross section = 40nb
#  raw int lumi = 243 pb^-1
#  3/979 skim jobs fail - ~30000 events missing
#  correction = 0.997
#  corrected integrated luminosity = 242 pb^-1
# 
#
# (NOTE: for Monte Carlo samples generated by Madgraph,
#        the filter efficiency is already included in the cross-sections
#        listed at https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2008 !!)
#
#  4.7kev/file, 5 files/job, 23kev/job, 18 jobs, 89 files 

intLumiWplusJets = float(242.)

fileNamesWjets_madgraph_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_2.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_4.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_5.root'
)
fileNamesWjets_madgraph_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_6.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_8.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_9.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_10.root'
)
fileNamesWjets_madgraph_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_11.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_12.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_13.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_14.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_15.root'
)
fileNamesWjets_madgraph_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_16.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_17.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_18.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_19.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_20.root'
)
fileNamesWjets_madgraph_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_21.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_22.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_23.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_24.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_25.root'
)
fileNamesWjets_madgraph_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_26.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_27.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_28.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_29.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_30.root'
)
fileNamesWjets_madgraph_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_31.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_32.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_33.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_34.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_35.root'
)
fileNamesWjets_madgraph_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_36.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_37.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_38.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_39.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_40.root'
)
fileNamesWjets_madgraph_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_41.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_42.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_43.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_44.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_45.root'
)
fileNamesWjets_madgraph_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_46.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_47.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_48.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_49.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_50.root'
)
fileNamesWjets_madgraph_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_51.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_52.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_53.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_54.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_55.root'
)
fileNamesWjets_madgraph_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_56.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_57.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_58.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_59.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_60.root'
)
fileNamesWjets_madgraph_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_61.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_62.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_63.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_64.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_65.root'
)
fileNamesWjets_madgraph_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_66.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_67.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_68.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_69.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_70.root'
)
fileNamesWjets_madgraph_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_71.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_72.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_73.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_74.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_75.root'
)
fileNamesWjets_madgraph_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_76.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_77.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_78.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_79.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_80.root'
)
fileNamesWjets_madgraph_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_81.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_82.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_83.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_84.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_85.root'
)
fileNamesWjets_madgraph_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_86.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_87.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_88.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_89.root'
)

genPhaseSpaceCutWjets_madgraph_part01 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameWjets_madgraph_part01 = cms.string('plotsZtoElecTau_Wjets_madgraph_part01.root')
outputFileNameWjets_madgraph_part02 = cms.string('plotsZtoElecTau_Wjets_madgraph_part02.root')
outputFileNameWjets_madgraph_part03 = cms.string('plotsZtoElecTau_Wjets_madgraph_part03.root')
outputFileNameWjets_madgraph_part04 = cms.string('plotsZtoElecTau_Wjets_madgraph_part04.root')
outputFileNameWjets_madgraph_part05 = cms.string('plotsZtoElecTau_Wjets_madgraph_part05.root')
outputFileNameWjets_madgraph_part06 = cms.string('plotsZtoElecTau_Wjets_madgraph_part06.root')
outputFileNameWjets_madgraph_part07 = cms.string('plotsZtoElecTau_Wjets_madgraph_part07.root')
outputFileNameWjets_madgraph_part08 = cms.string('plotsZtoElecTau_Wjets_madgraph_part08.root')
outputFileNameWjets_madgraph_part09 = cms.string('plotsZtoElecTau_Wjets_madgraph_part09.root')
outputFileNameWjets_madgraph_part10 = cms.string('plotsZtoElecTau_Wjets_madgraph_part10.root')
outputFileNameWjets_madgraph_part11 = cms.string('plotsZtoElecTau_Wjets_madgraph_part11.root')
outputFileNameWjets_madgraph_part12 = cms.string('plotsZtoElecTau_Wjets_madgraph_part12.root')
outputFileNameWjets_madgraph_part13 = cms.string('plotsZtoElecTau_Wjets_madgraph_part13.root')
outputFileNameWjets_madgraph_part14 = cms.string('plotsZtoElecTau_Wjets_madgraph_part14.root')
outputFileNameWjets_madgraph_part15 = cms.string('plotsZtoElecTau_Wjets_madgraph_part15.root')
outputFileNameWjets_madgraph_part16 = cms.string('plotsZtoElecTau_Wjets_madgraph_part16.root')
outputFileNameWjets_madgraph_part17 = cms.string('plotsZtoElecTau_Wjets_madgraph_part17.root')
outputFileNameWjets_madgraph_part18 = cms.string('plotsZtoElecTau_Wjets_madgraph_part18.root')

genPhaseSpaceCutWjets_madgraph_part02 = genPhaseSpaceCutWjets_madgraph_part01
genPhaseSpaceCutWjets_madgraph_part03 = genPhaseSpaceCutWjets_madgraph_part01
genPhaseSpaceCutWjets_madgraph_part04 = genPhaseSpaceCutWjets_madgraph_part01
genPhaseSpaceCutWjets_madgraph_part05 = genPhaseSpaceCutWjets_madgraph_part01
genPhaseSpaceCutWjets_madgraph_part06 = genPhaseSpaceCutWjets_madgraph_part01
genPhaseSpaceCutWjets_madgraph_part07 = genPhaseSpaceCutWjets_madgraph_part01
genPhaseSpaceCutWjets_madgraph_part08 = genPhaseSpaceCutWjets_madgraph_part01
genPhaseSpaceCutWjets_madgraph_part09 = genPhaseSpaceCutWjets_madgraph_part01
genPhaseSpaceCutWjets_madgraph_part10 = genPhaseSpaceCutWjets_madgraph_part01
genPhaseSpaceCutWjets_madgraph_part11 = genPhaseSpaceCutWjets_madgraph_part01
genPhaseSpaceCutWjets_madgraph_part12 = genPhaseSpaceCutWjets_madgraph_part01
genPhaseSpaceCutWjets_madgraph_part13 = genPhaseSpaceCutWjets_madgraph_part01
genPhaseSpaceCutWjets_madgraph_part14 = genPhaseSpaceCutWjets_madgraph_part01
genPhaseSpaceCutWjets_madgraph_part15 = genPhaseSpaceCutWjets_madgraph_part01
genPhaseSpaceCutWjets_madgraph_part16 = genPhaseSpaceCutWjets_madgraph_part01
genPhaseSpaceCutWjets_madgraph_part17 = genPhaseSpaceCutWjets_madgraph_part01
genPhaseSpaceCutWjets_madgraph_part18 = genPhaseSpaceCutWjets_madgraph_part01

#--------------------------------------------------------------------------------




#--------------------------------------------------------------------------------
#  Z + jets sample generated with Madgraph (exclusing Z --> tau+ tau- decays)
#
#  dataset = /ZJets-madgraph/Summer08_IDEAL_V11_redigi_v1/GEN-SIM-RECO
#  generated events = 1262816
#  events passing skim = 272465
#  cross section = 3.7nb
#  integrated luminosity = 341 pb^-1
# 
#
# (NOTE: for Monte Carlo samples generated by Madgraph,
#        the filter efficiency is already included in the cross-sections
#        listed at https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2008 !!)
#
intLumiZplusJets = float(341.)

# 13 jobs; 10 files/job; 128 files; ~21k ev/job
#
#
fileNamesZjets_madgraph_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_2.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_4.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_6.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_8.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_9.root'
)
fileNamesZjets_madgraph_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_10.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_11.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_12.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_13.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_14.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_15.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_16.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_17.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_18.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_19.root'
)
fileNamesZjets_madgraph_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_20.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_21.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_22.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_23.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_24.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_25.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_26.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_27.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_28.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_29.root'
)
fileNamesZjets_madgraph_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_30.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_31.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_32.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_33.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_34.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_35.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_36.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_37.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_38.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_39.root'
)
fileNamesZjets_madgraph_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_40.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_41.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_42.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_43.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_44.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_45.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_46.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_47.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_48.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_49.root'
)
fileNamesZjets_madgraph_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_50.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_51.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_52.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_53.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_54.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_55.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_56.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_57.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_58.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_59.root'
)
fileNamesZjets_madgraph_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_60.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_61.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_62.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_63.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_64.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_65.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_66.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_67.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_68.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_69.root'
)
fileNamesZjets_madgraph_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_70.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_71.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_72.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_73.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_74.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_75.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_76.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_77.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_78.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_79.root'
)
fileNamesZjets_madgraph_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_80.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_81.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_82.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_83.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_84.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_85.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_86.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_87.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_88.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_89.root'
)
fileNamesZjets_madgraph_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_90.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_91.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_92.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_93.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_94.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_95.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_96.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_97.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_98.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_99.root'
)
fileNamesZjets_madgraph_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_100.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_101.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_102.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_103.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_104.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_105.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_106.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_107.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_108.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_109.root'
)
fileNamesZjets_madgraph_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_110.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_111.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_112.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_113.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_114.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_115.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_116.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_117.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_118.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_119.root'
)
fileNamesZjets_madgraph_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_120.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_121.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_122.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_123.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_124.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_125.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_126.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_127.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_128.root'
)

genPhaseSpaceCutZjets_madgraph_part01 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameZjets_madgraph_part01 = cms.string('plotsZtoElecTau_Zjets_madgraph_part01.root')
outputFileNameZjets_madgraph_part02 = cms.string('plotsZtoElecTau_Zjets_madgraph_part02.root')
outputFileNameZjets_madgraph_part03 = cms.string('plotsZtoElecTau_Zjets_madgraph_part03.root')
outputFileNameZjets_madgraph_part04 = cms.string('plotsZtoElecTau_Zjets_madgraph_part04.root')
outputFileNameZjets_madgraph_part05 = cms.string('plotsZtoElecTau_Zjets_madgraph_part05.root')
outputFileNameZjets_madgraph_part06 = cms.string('plotsZtoElecTau_Zjets_madgraph_part06.root')
outputFileNameZjets_madgraph_part07 = cms.string('plotsZtoElecTau_Zjets_madgraph_part07.root')
outputFileNameZjets_madgraph_part08 = cms.string('plotsZtoElecTau_Zjets_madgraph_part08.root')
outputFileNameZjets_madgraph_part09 = cms.string('plotsZtoElecTau_Zjets_madgraph_part09.root')
outputFileNameZjets_madgraph_part10 = cms.string('plotsZtoElecTau_Zjets_madgraph_part10.root')
outputFileNameZjets_madgraph_part11 = cms.string('plotsZtoElecTau_Zjets_madgraph_part11.root')
outputFileNameZjets_madgraph_part12 = cms.string('plotsZtoElecTau_Zjets_madgraph_part12.root')
outputFileNameZjets_madgraph_part13 = cms.string('plotsZtoElecTau_Zjets_madgraph_part13.root')

genPhaseSpaceCutZjets_madgraph_part02 = genPhaseSpaceCutZjets_madgraph_part01
genPhaseSpaceCutZjets_madgraph_part03 = genPhaseSpaceCutZjets_madgraph_part01
genPhaseSpaceCutZjets_madgraph_part04 = genPhaseSpaceCutZjets_madgraph_part01
genPhaseSpaceCutZjets_madgraph_part05 = genPhaseSpaceCutZjets_madgraph_part01
genPhaseSpaceCutZjets_madgraph_part06 = genPhaseSpaceCutZjets_madgraph_part01
genPhaseSpaceCutZjets_madgraph_part07 = genPhaseSpaceCutZjets_madgraph_part01
genPhaseSpaceCutZjets_madgraph_part08 = genPhaseSpaceCutZjets_madgraph_part01
genPhaseSpaceCutZjets_madgraph_part09 = genPhaseSpaceCutZjets_madgraph_part01
genPhaseSpaceCutZjets_madgraph_part10 = genPhaseSpaceCutZjets_madgraph_part01
genPhaseSpaceCutZjets_madgraph_part11 = genPhaseSpaceCutZjets_madgraph_part01
genPhaseSpaceCutZjets_madgraph_part12 = genPhaseSpaceCutZjets_madgraph_part01
genPhaseSpaceCutZjets_madgraph_part13 = genPhaseSpaceCutZjets_madgraph_part01

#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# TT + jets (madgraph) sample
#
#   >>>>>>>>>>>>>>>>>>>  sample not skimmed!!  <<<<<<<<<<<<<<<<<<<<<<<<<
#
#intLumiTTplusJets = float()

fileNamesTTjets_madgraph = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/TTjets_madgraph/skimElecTau_TTjets_madgraph_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/TTjets_madgraph/skimElecTau_TTjets_madgraph_2.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/TTjets_madgraph/skimElecTau_TTjets_madgraph_3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/TTjets_madgraph/skimElecTau_TTjets_madgraph_4.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/TTjets_madgraph/skimElecTau_TTjets_madgraph_5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/TTjets_madgraph/skimElecTau_TTjets_madgraph_6.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/TTjets_madgraph/skimElecTau_TTjets_madgraph_7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/TTjets_madgraph/skimElecTau_TTjets_madgraph_8.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/TTjets_madgraph/skimElecTau_TTjets_madgraph_9.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/TTjets_madgraph/skimElecTau_TTjets_madgraph_10.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/TTjets_madgraph/skimElecTau_TTjets_madgraph_11.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/TTjets_madgraph/skimElecTau_TTjets_madgraph_12.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/TTjets_madgraph/skimElecTau_TTjets_madgraph_13.root'
)

genPhaseSpaceCutTTjets_madgraph = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameTTjets_madgraph = cms.string('plotsZtoElecTau_TTjets_madgraph.root')
#--------------------------------------------------------------------------------

