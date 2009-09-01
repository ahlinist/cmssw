
import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("L1Trigger.Configuration.L1Config_cff")
process.load("L1TriggerConfig.L1GtConfigProducers.Luminosity.lumi1x1032.L1Menu_CRUZET200805_gr7_muon_cff")
process.load("FWCore.MessageService.MessageLogger_cfi")


process.maxEvents = cms.untracked.PSet( 
     input = cms.untracked.int32(1000) 
)

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        '/store/data/Commissioning09/Calo/RECO/v1/000/096/893/FECF0964-5944-DE11-BEC7-000423D94E1C.root'
    )
)

process.demo = cms.EDAnalyzer('JetTrigAnalyzer',
                               TriggerTag = cms.untracked.InputTag( 'TriggerResults::HLT' ),
                               l1collections = cms.untracked.InputTag('hltL1extraParticles'),
                        	DoPrint = cms.untracked.bool(False)

)


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('testfile.root')
)

process.p = cms.Path(process.demo)
process.MessageLogger.cerr.FwkReport.reportEvery = 100
