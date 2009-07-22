import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.plotterProcessDefinitions_cfi import *
from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_cfi import *

plotDirectoryName = cms.string("rfio:/castor/cern.ch/user/j/jkolb/elecTauAnalysis/hists_factorized/")

#--------------------------------------------------------------------------------
# define for Z --> e + tau-jet analysis names of .root files containing histograms
#--------------------------------------------------------------------------------

processZtoElecTau_Ztautau = copy.deepcopy(process_Ztautau)
processZtoElecTau_Ztautau.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_Ztautau_part01.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Ztautau_part02.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Ztautau_part03.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Ztautau_part04.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Ztautau_part05.root'
)
processZtoElecTau_Ztautau.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtautau*intLumiData/intLumiZtautau)

processZtoElecTau_ZtautauSum = copy.deepcopy(process_Ztautau)
processZtoElecTau_ZtautauSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_ZtautauSum.root'
)
processZtoElecTau_ZtautauSum.config_dqmFileLoader.dqmDirectory_store = cms.string('')
processZtoElecTau_ZtautauSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processZtoElecTau_Zee = copy.deepcopy(process_Zee)
processZtoElecTau_Zee.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part01.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part02.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part03.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part04.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part05.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part06.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part07.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part08.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part09.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part10.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part11.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part12.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part13.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part14.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part15.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part16.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part17.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part18.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part19.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part20.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part21.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part22.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part23.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part24.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part25.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part26.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part27.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_Zee_part27.root'
)
processZtoElecTau_Zee.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZee*intLumiData/intLumiZee)

processZtoElecTau_ZeeSum = copy.deepcopy(process_Zee)
processZtoElecTau_ZeeSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_ZeeSum.root'
)
processZtoElecTau_ZeeSum.config_dqmFileLoader.dqmDirectory_store = cms.string('')
processZtoElecTau_ZeeSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processZtoElecTau_gammaPlusJets_Pt15to20 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_Pt15to20.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_PhotonJets_Pt15to20.root'
)
processZtoElecTau_gammaPlusJets_Pt15to20.config_dqmFileLoader.dqmDirectory_store = cms.string('gammaPlusJets_Pt15to20')
processZtoElecTau_gammaPlusJets_Pt15to20.config_dqmFileLoader.scaleFactor = cms.double(corrFactorPhotonJets_Pt15to20*intLumiData/intLumiPhotonJets_Pt15to20)

processZtoElecTau_gammaPlusJets_Pt20to25 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_Pt20to25.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_PhotonJets_Pt20to25.root'
)
processZtoElecTau_gammaPlusJets_Pt20to25.config_dqmFileLoader.dqmDirectory_store = cms.string('gammaPlusJets_Pt20to25')
processZtoElecTau_gammaPlusJets_Pt20to25.config_dqmFileLoader.scaleFactor = cms.double(corrFactorPhotonJets_Pt20to25*intLumiData/intLumiPhotonJets_Pt20to25)

processZtoElecTau_gammaPlusJets_Pt25to30 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_Pt25to30.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_PhotonJets_Pt25to30.root'
)
processZtoElecTau_gammaPlusJets_Pt25to30.config_dqmFileLoader.dqmDirectory_store = cms.string('gammaPlusJets_Pt25to30')
processZtoElecTau_gammaPlusJets_Pt25to30.config_dqmFileLoader.scaleFactor = cms.double(corrFactorPhotonJets_Pt25to30*intLumiData/intLumiPhotonJets_Pt25to30)

processZtoElecTau_gammaPlusJets_Pt30to35 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_Pt30to35.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_PhotonJets_Pt30to35.root'
)
processZtoElecTau_gammaPlusJets_Pt30to35.config_dqmFileLoader.dqmDirectory_store = cms.string('gammaPlusJets_Pt30to35')
processZtoElecTau_gammaPlusJets_Pt30to35.config_dqmFileLoader.scaleFactor = cms.double(corrFactorPhotonJets_Pt30to35*intLumiData/intLumiPhotonJets_Pt30to35)

processZtoElecTau_gammaPlusJets_PtGt35 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_PtGt35.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_PhotonJets_PtGt35.root'
)
processZtoElecTau_gammaPlusJets_PtGt35.config_dqmFileLoader.dqmDirectory_store = cms.string('gammaPlusJets_PtGt35')
processZtoElecTau_gammaPlusJets_PtGt35.config_dqmFileLoader.scaleFactor = cms.double(corrFactorPhotonJets_PtGt35*intLumiData/intLumiPhotonJets_PtGt35)

#--------------------------------------------------------------------------------

processZtoElecTau_QCD_BCtoE_Pt20to30 = copy.deepcopy(process_QCD_BCtoE_Pt20to30)
processZtoElecTau_QCD_BCtoE_Pt20to30.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part01.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part02.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part03.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part04.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part05.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part06.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part07.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part08.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part09.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part10.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part11.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part12.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part13.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part14.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part15.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part16.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part17.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part18.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part19.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part20.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part21.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part22.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part23.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30_part24.root'
)
processZtoElecTau_QCD_BCtoE_Pt20to30.config_dqmFileLoader.scaleFactor = cms.double(corrFactorQCD_BCtoE_Pt20to30*intLumiData/intLumiQCD_BCtoE_Pt20to30)

processZtoElecTau_QCD_BCtoE_Pt20to30Sum = copy.deepcopy(process_QCD_BCtoE_Pt20to30)
processZtoElecTau_QCD_BCtoE_Pt20to30Sum.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30Sum.root'
)
processZtoElecTau_QCD_BCtoE_Pt20to30Sum.config_dqmFileLoader.dqmDirectory_store = cms.string('')
processZtoElecTau_QCD_BCtoE_Pt20to30Sum.config_dqmFileLoader.scaleFactor = cms.double(1.)

processZtoElecTau_QCD_BCtoE_Pt30to80 = copy.deepcopy(process_QCD_BCtoE_Pt30to80)
processZtoElecTau_QCD_BCtoE_Pt30to80.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part01.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part02.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part03.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part04.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part05.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part06.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part07.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part08.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part09.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part10.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part11.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part12.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part13.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part14.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part15.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part16.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part17.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part18.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part19.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part20.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part21.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part22.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part23.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part24.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part25.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part26.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80_part27.root'
)
processZtoElecTau_QCD_BCtoE_Pt30to80.config_dqmFileLoader.scaleFactor = cms.double(corrFactorQCD_BCtoE_Pt30to80*intLumiData/intLumiQCD_BCtoE_Pt30to80)

processZtoElecTau_QCD_BCtoE_Pt30to80Sum = copy.deepcopy(process_QCD_BCtoE_Pt30to80)
processZtoElecTau_QCD_BCtoE_Pt30to80Sum.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80Sum.root'
)
processZtoElecTau_QCD_BCtoE_Pt30to80Sum.config_dqmFileLoader.dqmDirectory_store = cms.string('')
processZtoElecTau_QCD_BCtoE_Pt30to80Sum.config_dqmFileLoader.scaleFactor = cms.double(1.)

processZtoElecTau_QCD_BCtoE_Pt80to170 = copy.deepcopy(process_QCD_BCtoE_Pt80to170)
processZtoElecTau_QCD_BCtoE_Pt80to170.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part01.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part02.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part03.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part04.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part05.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part06.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part07.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part08.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part09.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part10.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part11.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part12.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part13.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part14.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt80to170_part15.root'
)
processZtoElecTau_QCD_BCtoE_Pt80to170.config_dqmFileLoader.scaleFactor = cms.double(corrFactorQCD_BCtoE_Pt80to170*intLumiData/intLumiQCD_BCtoE_Pt80to170)

processZtoElecTau_QCD_BCtoE_Pt80to170Sum = copy.deepcopy(process_QCD_BCtoE_Pt80to170)
processZtoElecTau_QCD_BCtoE_Pt80to170Sum.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt80to170Sum.root'
)
processZtoElecTau_QCD_BCtoE_Pt80to170Sum.config_dqmFileLoader.dqmDirectory_store = cms.string('')
processZtoElecTau_QCD_BCtoE_Pt80to170Sum.config_dqmFileLoader.scaleFactor = cms.double(1.)

processZtoElecTau_QCD_EMenriched_Pt20to30 = copy.deepcopy(process_QCD_EMenriched_Pt20to30)
processZtoElecTau_QCD_EMenriched_Pt20to30.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part01.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part02.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part03.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part04.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part05.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part06.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part07.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part08.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part09.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part10.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part11.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part12.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part13.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part14.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part15.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part16.root'
)
processZtoElecTau_QCD_EMenriched_Pt20to30.config_dqmFileLoader.scaleFactor = cms.double(corrFactorQCD_EMenriched_Pt20to30*intLumiData/intLumiQCD_EMenriched_Pt20to30)

processZtoElecTau_QCD_EMenriched_Pt20to30Sum = copy.deepcopy(process_QCD_EMenriched_Pt20to30)
processZtoElecTau_QCD_EMenriched_Pt20to30Sum.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt20to30Sum.root'
)
processZtoElecTau_QCD_EMenriched_Pt20to30Sum.config_dqmFileLoader.dqmDirectory_store = cms.string('')
processZtoElecTau_QCD_EMenriched_Pt20to30Sum.config_dqmFileLoader.scaleFactor = cms.double(1.)

processZtoElecTau_QCD_EMenriched_Pt30to80 = copy.deepcopy(process_QCD_EMenriched_Pt30to80)
processZtoElecTau_QCD_EMenriched_Pt30to80.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part01.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part02.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part03.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part04.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part05.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part06.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part07.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part08.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part09.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part10.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part11.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part12.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part13.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part14.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part15.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part16.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part17.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part18.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part19.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part20.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part21.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part22.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part23.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part24.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part25.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part26.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part27.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part28.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part29.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part30.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part31.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part32.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part33.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part34.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part35.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part36.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part37.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part38.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part39.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part40.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part41.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part42.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part43.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part44.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part45.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part46.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part47.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part48.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part49.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part50.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part51.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part52.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part53.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part54.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part55.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part56.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part57.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part58.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part59.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part60.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part61.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part62.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part63.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part64.root',
    #plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part65.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part66.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part67.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part68.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part69.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part70.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part71.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part72.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part73.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part74.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part75.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part76.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part77.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part78.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part79.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part80.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part81.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part82.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part83.root'
)
processZtoElecTau_QCD_EMenriched_Pt30to80.config_dqmFileLoader.scaleFactor = cms.double(corrFactorQCD_EMenriched_Pt30to80*intLumiData/intLumiQCD_EMenriched_Pt30to80)

processZtoElecTau_QCD_EMenriched_Pt30to80Sum = copy.deepcopy(process_QCD_EMenriched_Pt30to80)
processZtoElecTau_QCD_EMenriched_Pt30to80Sum.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80Sum.root'
)
processZtoElecTau_QCD_EMenriched_Pt30to80Sum.config_dqmFileLoader.dqmDirectory_store = cms.string('')
processZtoElecTau_QCD_EMenriched_Pt30to80Sum.config_dqmFileLoader.scaleFactor = cms.double(1.)

processZtoElecTau_QCD_EMenriched_Pt80to170 = copy.deepcopy(process_QCD_EMenriched_Pt80to170)
processZtoElecTau_QCD_EMenriched_Pt80to170.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part01.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part02.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part03.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part04.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part05.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part06.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part07.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part08.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part09.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part10.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part11.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part12.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part13.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part14.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part15.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part16.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part17.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part18.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part19.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part20.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part21.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part22.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part23.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part24.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part25.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part26.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part27.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part28.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part29.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170_part30.root'
)
processZtoElecTau_QCD_EMenriched_Pt80to170.config_dqmFileLoader.scaleFactor = cms.double(corrFactorQCD_EMenriched_Pt80to170*intLumiData/intLumiQCD_EMenriched_Pt80to170)

processZtoElecTau_QCD_EMenriched_Pt80to170Sum = copy.deepcopy(process_QCD_EMenriched_Pt80to170)
processZtoElecTau_QCD_EMenriched_Pt80to170Sum.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170Sum.root'
)
processZtoElecTau_QCD_EMenriched_Pt80to170Sum.config_dqmFileLoader.dqmDirectory_store = cms.string('')
processZtoElecTau_QCD_EMenriched_Pt80to170Sum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processZtoElecTau_WplusJets = copy.deepcopy(process_WplusJets)
processZtoElecTau_WplusJets.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_WplusJets_part01.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_WplusJets_part02.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_WplusJets_part03.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_WplusJets_part04.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_WplusJets_part05.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_WplusJets_part06.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_WplusJets_part07.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_WplusJets_part08.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_WplusJets_part09.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_WplusJets_part10.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_WplusJets_part11.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_WplusJets_part12.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_WplusJets_part13.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_WplusJets_part14.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_WplusJets_part15.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_WplusJets_part16.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_WplusJets_part17.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_WplusJets_part18.root'
)
processZtoElecTau_WplusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorWplusJets*intLumiData/intLumiWplusJets)

processZtoElecTau_WplusJetsSum = copy.deepcopy(process_WplusJets)
processZtoElecTau_WplusJetsSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_WplusJetsSum.root'
)
processZtoElecTau_WplusJetsSum.config_dqmFileLoader.dqmDirectory_store = cms.string('')
processZtoElecTau_WplusJetsSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processZtoElecTau_ZeePlusJets = copy.deepcopy(process_ZeePlusJets)
processZtoElecTau_ZeePlusJets.config_dqmFileLoader.inputFileNames = cms.vstring(
    #plotDirectoryName.value() + 'plotsZtoElecTau_ZeePlusJets_part01.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZeePlusJets_part02.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZeePlusJets_part03.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZeePlusJets_part04.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZeePlusJets_part05.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZeePlusJets_part06.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZeePlusJets_part07.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZeePlusJets_part08.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZeePlusJets_part09.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZeePlusJets_part10.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZeePlusJets_part11.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZeePlusJets_part12.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZeePlusJets_part13.root'
)
processZtoElecTau_ZeePlusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZeePlusJets*intLumiData/intLumiZeePlusJets)

processZtoElecTau_ZeePlusJetsSum = copy.deepcopy(process_ZeePlusJets)
processZtoElecTau_ZeePlusJetsSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_ZeePlusJetsSum.root'
)
processZtoElecTau_ZeePlusJetsSum.config_dqmFileLoader.dqmDirectory_store = cms.string('')
processZtoElecTau_ZeePlusJetsSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

processZtoElecTau_ZtautauPlusJets = copy.deepcopy(process_ZtautauPlusJets)
processZtoElecTau_ZtautauPlusJets.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_ZtautauPlusJets_part01.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZtautauPlusJets_part02.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZtautauPlusJets_part03.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZtautauPlusJets_part04.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZtautauPlusJets_part05.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZtautauPlusJets_part06.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZtautauPlusJets_part07.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZtautauPlusJets_part08.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZtautauPlusJets_part09.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZtautauPlusJets_part10.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZtautauPlusJets_part11.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZtautauPlusJets_part12.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_ZtautauPlusJets_part13.root'
)
processZtoElecTau_ZtautauPlusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtautauPlusJets*intLumiData/intLumiZtautauPlusJets)

processZtoElecTau_ZtautauPlusJetsSum = copy.deepcopy(process_ZtautauPlusJets)
processZtoElecTau_ZtautauPlusJetsSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_ZtautauPlusJetsSum.root'
)
processZtoElecTau_ZtautauPlusJetsSum.config_dqmFileLoader.dqmDirectory_store = cms.string('')
processZtoElecTau_ZtautauPlusJetsSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processZtoElecTau_TTplusJets = copy.deepcopy(process_TTplusJets)
processZtoElecTau_TTplusJets.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_TTplusJets_part01.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_TTplusJets_part02.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_TTplusJets_part03.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_TTplusJets_part04.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_TTplusJets_part05.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_TTplusJets_part06.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_TTplusJets_part07.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_TTplusJets_part08.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_TTplusJets_part09.root',
    plotDirectoryName.value() + 'plotsZtoElecTau_TTplusJets_part10.root'
)
processZtoElecTau_TTplusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorTTplusJets*intLumiData/intLumiTTplusJets)

processZtoElecTau_TTplusJetsSum = copy.deepcopy(process_TTplusJets)
processZtoElecTau_TTplusJetsSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsZtoElecTau_TTplusJetsSum.root'
)
processZtoElecTau_TTplusJetsSum.config_dqmFileLoader.dqmDirectory_store = cms.string('')
processZtoElecTau_TTplusJetsSum.config_dqmFileLoader.scaleFactor = cms.double(1.)
