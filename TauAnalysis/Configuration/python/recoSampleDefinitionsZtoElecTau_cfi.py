import FWCore.ParameterSet.Config as cms
import copy

# define configuration parameters for submission of Z --> e + tau-jet jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)

intLumiData = float(200.)

patTupleOutputDirectoryName = cms.string('/castor/cern.ch/user/v/veelken/CMSSW_2_2_7/')

#--------------------------------------------------------------------------------
# Z --> tau+ tau- sample generated with Pythia + Tauola (all decay modes)
#  integrated luminosity = 1135 pb^-1
# (to be corrected for missing files)
#
intLumiZtautau = float(1135.4)

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
#  integrated luminosity = 90 pb^-1
# (to be corrected for missing files)
#
intLumiZee = float(90.)

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
#
#intLumiQCD_EMenriched_Pt20to30 = float()

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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_20.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_21.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_22.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_23.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_24.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_25.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt20to30_part01 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_EMenriched_Pt20to30_part01 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt20to30_part01.root')

fileNamesQCD_EMenriched_Pt20to30_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_26.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_27.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_28.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_29.root',
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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_50.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt20to30_part02 = genPhaseSpaceCutQCD_EMenriched_Pt20to30_part01

outputFileNameQCD_EMenriched_Pt20to30_part02 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt20to30_part02.root')

fileNamesQCD_EMenriched_Pt20to30_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_51.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_52.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_53.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_54.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_55.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_56.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_57.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_58.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_59.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_50.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_61.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_62.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_63.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_64.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_65.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_66.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_67.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_68.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_69.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_70.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_71.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_72.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_73.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_74.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_75.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt20to30_part03 = genPhaseSpaceCutQCD_EMenriched_Pt20to30_part01

outputFileNameQCD_EMenriched_Pt20to30_part03 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt20to30_part03.root')

fileNamesQCD_EMenriched_Pt20to30_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_76.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_77.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_78.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_79.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_80.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_81.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_82.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_83.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_84.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_85.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_86.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_87.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_88.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_89.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_90.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_91.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_92.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_93.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_94.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_95.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_96.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_97.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_98.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_99.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_100.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt20to30_part04 = genPhaseSpaceCutQCD_EMenriched_Pt20to30_part01

outputFileNameQCD_EMenriched_Pt20to30_part04 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt20to30_part04.root')

fileNamesQCD_EMenriched_Pt20to30_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_101.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_102.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_103.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_104.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_105.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_106.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_107.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_108.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_109.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_110.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_111.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_112.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_113.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_114.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_115.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_116.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_117.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_118.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_119.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_110.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_111.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_112.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_113.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_114.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_125.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_126.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_127.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt20to30/skimElecTau_QCD_EMenriched_Pt20to30_128.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt20to30_part05 = genPhaseSpaceCutQCD_EMenriched_Pt20to30_part01

outputFileNameQCD_EMenriched_Pt20to30_part05 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt20to30_part05.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# electron enriched b/c --> e QCD sample generated with Pythia (30 GeV < Pt(hat) < 80 GeV)
#
#intLumiQCD_EMenriched_Pt30to80 = float()

fileNamesQCD_EMenriched_Pt30to80_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_2.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_4.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_6.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_8.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_9.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_10.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_EMenriched_Pt30to80_part01 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part01.root')

fileNamesQCD_EMenriched_Pt30to80_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_11.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_12.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_13.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_14.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_15.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_16.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_17.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_18.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_19.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_20.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt30to80_part02 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01

outputFileNameQCD_EMenriched_Pt30to80_part02 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part02.root')

fileNamesQCD_EMenriched_Pt30to80_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_21.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_22.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_23.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_24.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_25.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_26.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_27.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_28.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_29.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_30.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt30to80_part03 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_EMenriched_Pt30to80_part03 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part03.root')

fileNamesQCD_EMenriched_Pt30to80_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_31.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_32.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_33.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_34.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_35.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_36.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_37.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_38.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_39.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_40.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt30to80_part04 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_EMenriched_Pt30to80_part04 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part04.root')

fileNamesQCD_EMenriched_Pt30to80_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_41.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_42.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_43.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_44.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_45.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_46.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_47.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_48.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_49.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_50.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt30to80_part05 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01

outputFileNameQCD_EMenriched_Pt30to80_part05 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part05.root')

fileNamesQCD_EMenriched_Pt30to80_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_51.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_52.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_53.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_54.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_55.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_56.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_57.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_58.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_59.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_60.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt30to80_part06 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01

outputFileNameQCD_EMenriched_Pt30to80_part06 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part06.root')

fileNamesQCD_EMenriched_Pt30to80_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_61.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_62.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_63.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_64.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_65.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_66.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_67.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_68.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_69.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_70.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt30to80_part07 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01

outputFileNameQCD_EMenriched_Pt30to80_part07 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part07.root')

fileNamesQCD_EMenriched_Pt30to80_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_71.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_72.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_73.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_74.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_75.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_76.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_77.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_78.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_79.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_80.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt30to80_part08 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01

outputFileNameQCD_EMenriched_Pt30to80_part08 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part08.root')

fileNamesQCD_EMenriched_Pt30to80_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_81.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_82.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_83.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_84.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_85.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_86.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_87.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_88.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_89.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_90.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt30to80_part09 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01

outputFileNameQCD_EMenriched_Pt30to80_part09 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part09.root')

fileNamesQCD_EMenriched_Pt30to80_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_91.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_92.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_93.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_94.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_95.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_96.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_97.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_98.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_99.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_100.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt30to80_part10 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01

outputFileNameQCD_EMenriched_Pt30to80_part10 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part10.root')

fileNamesQCD_EMenriched_Pt30to80_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_101.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_102.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_103.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_104.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_105.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_106.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_107.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_108.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_109.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_110.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt30to80_part11 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01

outputFileNameQCD_EMenriched_Pt30to80_part11 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part11.root')

fileNamesQCD_EMenriched_Pt30to80_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_111.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_112.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_113.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_114.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_115.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_116.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_117.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_118.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_119.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_120.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt30to80_part12 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01

outputFileNameQCD_EMenriched_Pt30to80_part12 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part12.root')

fileNamesQCD_EMenriched_Pt30to80_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_121.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_122.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_123.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_124.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_125.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_126.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_127.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_128.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_129.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_130.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt30to80_part13 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01

outputFileNameQCD_EMenriched_Pt30to80_part13 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part13.root')

fileNamesQCD_EMenriched_Pt30to80_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_131.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_132.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_133.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_134.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_135.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_136.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_137.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_138.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_139.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_140.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt30to80_part14 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01

outputFileNameQCD_EMenriched_Pt30to80_part14 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part14.root')

fileNamesQCD_EMenriched_Pt30to80_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_141.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_142.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_143.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_144.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_145.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_146.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_147.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_148.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_149.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_150.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt30to80_part15 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01

outputFileNameQCD_EMenriched_Pt30to80_part15 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part15.root')

fileNamesQCD_EMenriched_Pt30to80_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_151.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_152.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_153.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_154.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_155.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_156.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_157.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_158.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_159.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_160.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_161.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_162.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_163.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_164.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt30to80/skimElecTau_QCD_EMenriched_Pt30to80_165.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt30to80_part16 = genPhaseSpaceCutQCD_EMenriched_Pt30to80_part01

outputFileNameQCD_EMenriched_Pt30to80_part16 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_part16.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# electron enriched b/c --> e QCD sample generated with Pythia (80 GeV < Pt(hat) < 170 GeV)
#
#intLumiQCD_EMenriched_Pt80to170 = float()

fileNamesQCD_EMenriched_Pt80to170_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_151.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_152.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_153.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_154.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_155.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_156.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_157.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_158.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_159.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_160.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_161.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_162.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_163.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_164.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_165.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_166.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_167.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_168.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_EMenriched_Pt80to170_part01 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part01.root')

# ----------->>>> FINISH <<<< -------------------

fileNamesQCD_EMenriched_Pt80to170_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_151.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_152.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_153.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_154.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_155.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_156.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_157.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_158.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_159.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_160.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_161.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_162.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_163.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_164.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_165.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_166.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_167.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_168.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt80to170_part02 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01

outputFileNameQCD_EMenriched_Pt80to170_part02 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part02.root')

fileNamesQCD_EMenriched_Pt80to170_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_151.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_152.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_153.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_154.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_155.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_156.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_157.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_158.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_159.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_160.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_161.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_162.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_163.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_164.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_165.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_166.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_167.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_168.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt80to170_part03 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01

outputFileNameQCD_EMenriched_Pt80to170_part03 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part03.root')

fileNamesQCD_EMenriched_Pt80to170_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_151.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_152.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_153.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_154.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_155.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_156.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_157.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_158.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_159.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_160.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_161.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_162.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_163.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_164.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_165.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_166.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_167.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_EMenriched_Pt80to170/skimElecTau_QCD_EMenriched_Pt80to170_168.root'
)

genPhaseSpaceCutQCD_EMenriched_Pt80to170_part04 = genPhaseSpaceCutQCD_EMenriched_Pt80to170_part01

outputFileNameQCD_EMenriched_Pt80to170_part04 = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_part04.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# QCD b/c --> e sample generated with Pythia (20 GeV < Pt(hat) < 30 GeV)
#  integrated luminosity = 8.24 pb^-1
# (to be corrected by scale factor for missing files)
#
intLumiQCD_BCtoE_Pt20to30 = float(8.24)

fileNamesQCD_BCtoE_Pt20to30_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_2.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_4.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_6.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_8.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_9.root'
)

genPhaseSpaceCutQCD_BCtoE_Pt20to30_part01 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_BCtoE_Pt20to30_part01 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_part01.root')

fileNamesQCD_BCtoE_Pt20to30_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_10.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_11.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_12.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_13.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_14.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_15.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_16.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_17.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_18.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_19.root'
)

genPhaseSpaceCutQCD_BCtoE_Pt20to30_part02 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_BCtoE_Pt20to30_part02 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_part02.root')

fileNamesQCD_BCtoE_Pt20to30_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_20.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_21.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_22.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_23.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_24.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_25.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_26.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_27.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_28.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_29.root'
)

genPhaseSpaceCutQCD_BCtoE_Pt20to30_part03 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_BCtoE_Pt20to30_part03 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_part03.root')

fileNamesQCD_BCtoE_Pt20to30_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_30.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_31.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_32.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_33.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_34.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_35.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_36.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_37.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_38.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_39.root'
)

genPhaseSpaceCutQCD_BCtoE_Pt20to30_part04 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_BCtoE_Pt20to30_part04 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_part04.root')

fileNamesQCD_BCtoE_Pt20to30_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_40.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_41.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_42.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_43.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_44.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_45.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_46.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_47.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt20to30/skimElecTau_48.root'
)

genPhaseSpaceCutQCD_BCtoE_Pt20to30_part05 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_BCtoE_Pt20to30_part05 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_part05.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# QCD b/c -->e sample generated with Pythia (30 GeV < Pt(hat) < 80 GeV)
#  integrated luminosity = 3.67 pb^-1
# (to be corrected by scale factor for missing files)
#
intLumiQCD_BCtoE_Pt30to80 = float(3.67)

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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_14.root',
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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_29.root',
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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_44.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_45.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_46.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_47.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_48.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_49.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_50.root'
)

genPhaseSpaceCutQCD_BCtoE_Pt30to80_part01 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_BCtoE_Pt30to80_part01 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part01.root')

fileNamesQCD_BCtoE_Pt30to80_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_51.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_52.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_53.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_54.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_55.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_56.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_57.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_58.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_59.root',
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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_74.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_75.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_76.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_77.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_78.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_79.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_70.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_80.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_81.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_82.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_83.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_84.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_85.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_86.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_87.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_88.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_89.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_80.root',
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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_100.root'
)

genPhaseSpaceCutQCD_BCtoE_Pt30to80_part02 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_BCtoE_Pt30to80_part02 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part02.root')

fileNamesQCD_BCtoE_Pt30to80_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_101.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_102.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_103.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_104.root',
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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_119.root',
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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_134.root',
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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_149.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_150.root'
)

genPhaseSpaceCutQCD_BCtoE_Pt30to80_part03 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_BCtoE_Pt30to80_part03 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part03.root')

fileNamesQCD_BCtoE_Pt30to80_part04 = cms.untracked.vstring(
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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_164.root',
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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_179.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_170.root',
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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_180.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_190.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_191.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_192.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_193.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_194.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_195.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_196.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_197.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_198.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_199.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_200.root'
)

genPhaseSpaceCutQCD_BCtoE_Pt30to80_part04 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_BCtoE_Pt30to80_part04 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part04.root')

fileNamesQCD_BCtoE_Pt30to80_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_201.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_202.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_203.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_204.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_205.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_206.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_207.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_208.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_209.root',
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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_224.root',
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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_239.root',
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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_250.root'
)

genPhaseSpaceCutQCD_BCtoE_Pt30to80_part05 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_BCtoE_Pt30to80_part05 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part05.root')

fileNamesQCD_BCtoE_Pt30to80_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_251.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_252.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_253.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_254.root',
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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_269.root',
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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_270.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_280.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_281.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_282.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_283.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_284.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_285.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_286.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_287.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_288.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_289.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_280.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_290.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_291.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_292.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_293.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_294.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_295.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_296.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_297.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_298.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_299.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_300.root'
)

genPhaseSpaceCutQCD_BCtoE_Pt30to80_part06 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_BCtoE_Pt30to80_part06 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part06.root')

fileNamesQCD_BCtoE_Pt30to80_part07 = cms.untracked.vstring(
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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_314.root',
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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_329.root',
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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_344.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_345.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_346.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_347.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_348.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_349.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_350.root'
)

genPhaseSpaceCutQCD_BCtoE_Pt30to80_part07 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_BCtoE_Pt30to80_part07 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part07.root')

fileNamesQCD_BCtoE_Pt30to80_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_351.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_352.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_353.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_354.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_355.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_356.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_357.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_358.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_359.root',
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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_374.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_375.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_376.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_377.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_378.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_379.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_370.root',
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
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_380.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt30to80/skimElecTau_390.root',
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

genPhaseSpaceCutQCD_BCtoE_Pt30to80_part08 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_BCtoE_Pt30to80_part08 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_part08.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# QCD b/c -->e sample generated with Pythia (80 GeV < Pt(hat) < 170 GeV)
#  integrated luminosity = 3.67 pb^-1
# (to be corrected by scale factor for missing files)
#
#intLumiQCD_BCtoE_Pt30to80 = float()

fileNamesQCD_BCtoE_Pt80to170_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_2.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_4.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_6.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_8.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_9.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_10.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_11.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_12.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_13.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_14.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_15.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_16.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_17.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_18.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_19.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_20.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_21.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_22.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_23.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_24.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_25.root'
)

genPhaseSpaceCutQCD_BCtoE_Pt80to170_part01 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_BCtoE_Pt80to170_part01 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt80to170_part01.root')

fileNamesQCD_BCtoE_Pt80to170_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_26.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_27.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_28.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_29.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_30.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_31.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_32.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_33.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_34.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_35.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_36.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_37.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_38.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_39.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_40.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_41.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_42.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_43.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_44.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_45.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_46.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_47.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_48.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_49.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_50.root'
)

genPhaseSpaceCutQCD_BCtoE_Pt80to170_part02 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_BCtoE_Pt80to170_part02 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt80to170_part02.root')

fileNamesQCD_BCtoE_Pt80to170_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_51.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_52.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_53.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_54.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_55.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_56.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_57.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_58.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_59.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_60.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_61.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_62.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_63.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_64.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_65.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_66.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_67.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_68.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_69.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_70.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_71.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_72.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_73.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_74.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_75.root'
)

genPhaseSpaceCutQCD_BCtoE_Pt80to170_part03 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_BCtoE_Pt80to170_part03 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt80to170_part03.root')

fileNamesQCD_BCtoE_Pt80to170_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_76.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_77.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_78.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_79.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_80.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_81.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_82.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_83.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_84.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_85.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_86.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_87.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_88.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_89.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_90.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_91.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_92.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_93.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_94.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_95.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_96.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_97.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_98.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_99.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_100.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_101.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_102.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_103.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_104.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_105.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_106.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_107.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_108.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_109.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_110.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_111.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_112.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_113.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/QCD_BCtoE_Pt80to170/skimElecTau_QCD_BCtoE_Pt80to170_114.root'
)

genPhaseSpaceCutQCD_BCtoE_Pt80to170_part04 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameQCD_BCtoE_Pt80to170_part04 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt80to170_part04.root')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# photon + jets samples
# 500k events total, PDF set is CTEQ5L, filter on photons in central region (-2.4 < eta(gamma) < 2.4, ET(gamma) > 15 GeV/c)
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
fileNamesPhotonJets_Pt35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/PhotonJets_Pt35/skimElecTau_PhotonJets_Pt35_1.root'
)

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
genPhaseSpaceCutPhotonJets_Pt35 = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNamePhotonJets_Pt15to20 = cms.string('plotsZtoElecTau_PhotonJets_Pt15to20.root')
outputFileNamePhotonJets_Pt20to25 = cms.string('plotsZtoElecTau_PhotonJets_Pt20to25.root')
outputFileNamePhotonJets_Pt25to30 = cms.string('plotsZtoElecTau_PhotonJets_Pt25to30.root')
outputFileNamePhotonJets_Pt30to35 = cms.string('plotsZtoElecTau_PhotonJets_Pt30to35.root')
outputFileNamePhotonJets_Pt35     = cms.string('plotsZtoElecTau_PhotonJets_Pt35.root')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# W + jets sample generated with Madgraph
#  integrated luminosity = 297 pb^-1
# (to be corrected for missing files)
#
# (NOTE: for Monte Carlo samples generated by Madgraph,
#        the filter efficiency is already included in the cross-sections
#        listed at https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2008 !!)
#
intLumiWplusJets = float(297.)

fileNamesWjets_madgraph = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_2.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_4.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_6.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_8.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_9.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_10.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_11.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_12.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Wjets_madgraph/skimElecTau_Wjets_madgraph_13.root'
)

genPhaseSpaceCutWjets_madgraph = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameWjets_madgraph = cms.string('plotsZtoElecTau_Wjets_madgraph.root')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# Z + jets sample generated with Madgraph (exclusing Z --> tau+ tau- decays)
#  integrated luminosity = 197 pb^-1
# (to be corrected for missing files)
#
# (NOTE: for Monte Carlo samples generated by Madgraph,
#        the filter efficiency is already included in the cross-sections
#        listed at https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2008 !!)
#
intLumiZplusJets = float(197.)

fileNamesZjets_madgraph = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_Zjets_madgraph_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_Zjets_madgraph_2.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_Zjets_madgraph_3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_Zjets_madgraph_4.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_Zjets_madgraph_5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_Zjets_madgraph_6.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_Zjets_madgraph_7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_Zjets_madgraph_8.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_Zjets_madgraph_9.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_Zjets_madgraph_10.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_Zjets_madgraph_11.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_Zjets_madgraph_12.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/Zjets_madgraph/skimElecTau_Zjets_madgraph_13.root'
)

genPhaseSpaceCutZjets_madgraph = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

outputFileNameZjets_madgraph = cms.string('plotsZtoElecTau_Zjets_madgraph.root')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# TT + jets (madgraph) sample
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

