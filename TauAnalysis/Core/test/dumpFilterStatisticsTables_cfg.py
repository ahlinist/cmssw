import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Print-out cut-flow information contained in FilterStatisticsTables
#--------------------------------------------------------------------------------

process = cms.Process('dumpFilterStatisticsTables')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

process.loadFilterStatistics = cms.EDAnalyzer("DQMFileLoader",
    tmp = cms.PSet(
        inputFileNames = cms.vstring('../../Configuration/test/plotsZtoMuTau.root'),
        dqmDirectory_store = cms.string('tmp')
    )
)

process.dumpFilterStatisticsTables = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('tmp/zMuTauAnalyzer/FilterStatistics')
        #Ztautau = cms.string('tmp/zMuTauAnalyzer/FilterStatistics/Ztautau/'),
        #Zmumu = cms.string('tmp/zMuTauAnalyzer/FilterStatistics/'),
        #WplusJets = cms.string('tmp/zMuTauAnalyzer/FilterStatistics/'),
        #InclusivePPmuX = cms.string('tmp/zMuTauAnalyzer/FilterStatistics/'),
        #PPmuXptGt20 cms.string('tmp/zMuTauAnalyzer/FilterStatistics/')
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "Efficiency")
)
 
process.p = cms.Path( process.loadFilterStatistics
                     +process.dumpFilterStatisticsTables )
