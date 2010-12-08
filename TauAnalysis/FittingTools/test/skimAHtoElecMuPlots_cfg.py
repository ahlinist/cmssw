import FWCore.ParameterSet.Config as cms

import os

process = cms.Process('skimAHtoElecTauPlots')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

#--------------------------------------------------------------------------------
# Load analysis results for different channels,
# scale histograms to integrated luminosity of analyzed data sample
#--------------------------------------------------------------------------------

# Conversions to pico barns
_picobarns =  1.0

# Integrated luminosity to normalize
intLuminosityData = 35.39/_picobarns

inputFileNameData = 'plotsZtoElecMu_Data.root'

intLuminositiesMC = {   
    'Ztautau'    : 1198.69,
    'Zee'        : 1276.31,
    'Zmumu'      : 1373.67,
    'qcdSum'     :  348.43,
    'Wenu'       :  635.44,
    'Wmunu'      :  692.69,
    'Wtaunu'     :  506.38,
    'TTplusJets' : 6305.17
}

inputFileNamesMC = {
    'Ztautau'    : 'plotsZtoElecMu_Ztautau_POWHEG.root', 
    'Zee'        : 'plotsZtoElecMu_Zee.root',
    'Zmumu'      : 'plotsZtoElecMu_Zmumu.root',   
    'qcdSum'     : 'plotsZtoElecMu_QCDMuPt15.root',
    'Wenu'       : 'plotsZtoElecMu_Wenu.root',
    'Wmunu'      : 'plotsZtoElecMu_Wmunu.root',
    'Wtaunu'     : 'plotsZtoElecMu_Wtaunu.root', 
    'TTplusJets' : 'plotsZtoElecMu_TTbar.root'
    
}

inputFilePath = '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/ZtoElecMu_2010Nov23'

process.loadAnalysisResults = cms.EDAnalyzer("DQMFileLoader")

for processName, inputFileName in inputFileNamesMC.items():
    intLuminosityMC = intLuminositiesMC[processName]

    setattr(process.loadAnalysisResults, processName, cms.PSet(
        inputFileNames = cms.vstring(
            os.path.join(inputFilePath, inputFileName)
        ),
        scaleFactor = cms.double(intLuminosityData/intLuminosityMC),
        dqmDirectory_store = cms.string('harvested/%s' % processName)
    ))

setattr(process.loadAnalysisResults, "Data", cms.PSet(
    inputFileNames = cms.vstring(
        os.path.join(inputFilePath, inputFileNameData)
    ),
    scaleFactor = cms.double(1.),                          
    dqmDirectory_store = cms.string('/harvested/data')
))

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

#--------------------------------------------------------------------------------
# Drop MonitorElements not needed for exporting analysis results into ASCII files,
# in order to reduce memory consumption
#--------------------------------------------------------------------------------

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.saveAnalysisResults = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsZtoElecMu_skimmed.root'),
    outputCommands = cms.vstring(
        'drop harvested/*',
        'keep harvested/Ztautau/zElecMuAnalyzer/afterEvtSelDiMuZMass/*',
        'keep harvested/Ztautau/zElecMuAnalyzer/FilterStatistics/*',
        'keep harvested/Zee/zElecMuAnalyzer/afterEvtSelDiMuZMass/*',
        'keep harvested/Zee/zElecMuAnalyzer/FilterStatistics/*',
        'keep harvested/Zmumu/zElecMuAnalyzer/afterEvtSelDiMuZMass/*',
        'keep harvested/Zmumu/zElecMuAnalyzer/FilterStatistics/*',
        'keep harvested/qcdSum/zElecMuAnalyzer/afterEvtSelDiMuZMass/*',
        'keep harvested/qcdSum/zElecMuAnalyzer/FilterStatistics/*',
        'keep harvested/Wenu/zElecMuAnalyzer/afterEvtSelDiMuZMass/*',
        'keep harvested/Wenu/zElecMuAnalyzer/FilterStatistics/*',
        'keep harvested/Wmunu/zElecMuAnalyzer/afterEvtSelDiMuZMass/*',
        'keep harvested/Wmunu/zElecMuAnalyzer/FilterStatistics/*',
        'keep harvested/Wtaunu/zElecMuAnalyzer/afterEvtSelDiMuZMass/*',
        'keep harvested/Wtaunu/zElecMuAnalyzer/FilterStatistics/*',
        'keep harvested/TTplusJets/zElecMuAnalyzer/afterEvtSelDiMuZMass/*',
        'keep harvested/TTplusJets/zElecMuAnalyzer/FilterStatistics/*',
        'keep harvested/data/zElecMuAnalyzer/afterEvtSelDiMuZMass/*',
        'keep harvested/data/zElecMuAnalyzer/FilterStatistics/*'
    )                                   
)

process.p = cms.Path(
    process.loadAnalysisResults
   + process.dumpDQMStore 
   + process.saveAnalysisResults
)
