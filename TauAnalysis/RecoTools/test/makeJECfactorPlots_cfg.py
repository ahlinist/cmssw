import FWCore.ParameterSet.Config as cms

process = cms.Process("makeJECfactorPlots")

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.load('JetMETCorrections.Configuration.JetCorrectionServices_cff')

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring()
)

#isMC = True
isMC = False

if isMC:
    process.GlobalTag.globaltag = 'GR_R_42_V20::All'
    process.source.fileNames = cms.untracked.vstring(
        'file:/data1/veelken/CMSSW_4_2_x/skims/skim_data_SingleMu_Run2011A_PromptReco_v4_chunk_93_5651.root'
    )
else:
    process.GlobalTag.globaltag = 'START42_V13::All'
    process.source.fileNames = cms.untracked.vstring(
        'file:/data1/veelken/CMSSW_4_2_x/skims/goldenZmumuEvents_simDYtoMuMu_AOD_9_1_T1A.root'
    )

#--------------------------------------------------------------------------------
#
# configure Jet Energy Corrections
#
##process.load("CondCore.DBCommon.CondDBCommon_cfi")
##process.jec = cms.ESSource("PoolDBESSource",
##    DBParameters = cms.PSet(
##        messageLevel = cms.untracked.int32(0)
##    ),
##    timetype = cms.string('runnumber'),
##    toGet = cms.VPSet(
##        cms.PSet(
##            record = cms.string('JetCorrectionsRecord'),
##            tag    = cms.string('JetCorrectorParametersCollection_Jec11V2_AK5PF'),
##            label  = cms.untracked.string('AK5PF')
##        ),
##        cms.PSet(
##            record = cms.string('JetCorrectionsRecord'),
##            tag    = cms.string('JetCorrectorParametersCollection_Jec11V2_AK5Calo'),
##            label  = cms.untracked.string('AK5Calo')
##        )
##    ),
##    connect = cms.string('sqlite_fip:TauAnalysis/Configuration/data/Jec11V2.db')
##)
##process.es_prefer_jec = cms.ESPrefer('PoolDBESSource', 'jec')
#--------------------------------------------------------------------------------

# load tau-jet specific JEC parameters (from SQLlite file)
process.load("PhysicsTools.PatAlgos.recoLayer0.tauJetCorrections_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.makeJECfactorPlots = cms.EDAnalyzer('JECfactorAnalyzer',
    etaMin = cms.double(-5.0),
    etaMax = cms.double(+5.0),
    etaStepSize = cms.double(0.1),
    ptMin = cms.double(1.),
    ptMax = cms.double(100.),
    ptStepSize = cms.double(1.),
                                            
    levels = cms.vstring(
        'L1FastJet',
        'L2Relative',
        'L3Absolute',
        'L2L3Residual'
    ),

    payloads = cms.vstring(
        #'AK5Calo',
        'AK5PF',
        #'AK5tauHPSlooseCombDBcorr',
        #'AK5tauHPSlooseCombDBcor# load tau-jet specific JEC parameters from SQLlite filerOneProng0Pi0',
        #'AK5tauHPSlooseCombDBcorrOneProng1Pi0',
        #'AK5tauHPSlooseCombDBcorrOneProng2Pi0',
        #'AK5tauHPSlooseCombDBcorrThreeProng0Pi0'
    ),

    #yMin = cms.double(-0.5),
    #yMax = cms.double(+2.5),
    yMin = cms.double(+0.5),                                         
    yMax = cms.double(+1.5),

    outputFileName = cms.string('/data1/veelken/tmp/JECfactorPlots/makeJECfactorPlots.png')
)                                            

process.p = cms.Path(process.makeJECfactorPlots)

processDumpFile = open('makeJECfactorPlots.dump' , 'w')
print >> processDumpFile, process.dumpPython()


