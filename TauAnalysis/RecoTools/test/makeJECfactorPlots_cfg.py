import FWCore.ParameterSet.Config as cms

process = cms.Process("makeJECfactorPlots")

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.load('JetMETCorrections.Configuration.JetCorrectionServices_cff')

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring()
)

isMC = True
#isMC = False

#--------------------------------------------------------------------------------
#
# configure Jet Energy Corrections
#
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.jec = cms.ESSource("PoolDBESSource",
   DBParameters = cms.PSet(
       messageLevel = cms.untracked.int32(0)
   ),
   timetype = cms.string('runnumber'),
   toGet = cms.VPSet(
       cms.PSet(
           record = cms.string('JetCorrectionsRecord'),
           tag    = cms.string(''),
           label  = cms.untracked.string('AK5PF')
       ),
       cms.PSet(
           record = cms.string('JetCorrectionsRecord'),
           tag    = cms.string(''),
           label  = cms.untracked.string('AK5Calo')
       )
   ),
   connect = cms.string('')
)
process.es_prefer_jec = cms.ESPrefer('PoolDBESSource', 'jec')
if isMC:
   process.jec.connect = cms.string('sqlite_fip:TauAnalysis/Configuration/data/Fall12_V5Final_MC.db')
   process.jec.toGet[0].tag = cms.string('JetCorrectorParametersCollection_Fall12_V5_MC_AK5PF')
   process.jec.toGet[1].tag = cms.string('JetCorrectorParametersCollection_Fall12_V5_MC_AK5Calo')
else:
   process.jec.connect = cms.string('sqlite_fip:TauAnalysis/Configuration/data/Fall12_V5Final_DATA.db')
   process.jec.toGet[0].tag = cms.string('JetCorrectorParametersCollection_Fall12_V5_DATA_AK5PF')
   process.jec.toGet[1].tag = cms.string('JetCorrectorParametersCollection_Fall12_V5_DATA_AK5Calo')
#-------------------------------------------------------------------------------------------------------------------------

# load tau-jet specific JEC parameters (from SQLlite file)
#process.load("PhysicsTools.PatAlgos.recoLayer0.tauJetCorrections_cff")

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

    outputFileName = cms.string('')
)                                            

if isMC:
    process.GlobalTag.globaltag = 'START53_V11::All'
    process.source.fileNames = cms.untracked.vstring(
        '/store/user/veelken/CMSSW_5_3_x/skims/GoldenZmumu/2012Oct09/goldenZmumuEvents_ZplusJets_madgraph_2012Oct09_AOD_100_1_wSO.root'
    )
    process.makeJECfactorPlots.outputFileName = cms.string('/data1/veelken/tmp/JECfactorPlots/Jec12_V5_MC/makeJECfactorPlots.pdf')
else:    
    process.GlobalTag.globaltag = 'GR_R_53_V13::All'
    process.source.fileNames = cms.untracked.vstring(
        '/store/user/veelken/CMSSW_5_3_x/skims/GoldenZmumu/2012Oct09/goldenZmumuEvents_Data_runs202044to203002v2_2012Oct09_AOD_100_1_jKM.root'
    )
    process.makeJECfactorPlots.outputFileName = cms.string('/data1/veelken/tmp/JECfactorPlots/Jec12_V5_DATA/makeJECfactorPlots.pdf')

process.p = cms.Path(process.makeJECfactorPlots)

processDumpFile = open('makeJECfactorPlots.dump' , 'w')
print >> processDumpFile, process.dumpPython()


