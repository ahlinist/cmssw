import FWCore.ParameterSet.Config as cms

process = cms.Process("SingleTop")


process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    FailPath = cms.untracked.vstring('ProductNotFound','Type Mismatch')
    )


# conditions ------------------------------------------------------------------

process.load("PhysicsTools.PFCandProducer.PF2PAT_cff")

print "test "

#process.load("Configuration.StandardSequences.MixingNoPileUp_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff") ### real data

process.GlobalTag.globaltag = cms.string('GR_R_311_V2::All')

#from Configuration.PyReleaseValidation.autoCond import autoCond
#process.GlobalTag.globaltag = autoCond['startup']
process.load("TopQuarkAnalysis.SingleTop.SingleTopSequences_cff") 
process.load("SelectionCuts_Skim_cff");


# set the dB to the beamspot
process.patMuons.usePV = cms.bool(False)
process.patElectrons.usePV = cms.bool(False)

# require physics declared
process.load('HLTrigger.special.hltPhysicsDeclared_cfi')
process.hltPhysicsDeclared.L1GtReadoutRecordTag = 'gtDigis'

#dummy output

process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('dummy.root'),
                               outputCommands = cms.untracked.vstring(""),
                               )

process.load("PhysicsTools.HepMCCandAlgos.flavorHistoryPaths_cfi")

#mytrigs=["HLT_Mu9"]
mytrigs=["*"]

from HLTrigger.HLTfilters.hltHighLevel_cfi import *
if mytrigs is not None :
    process.hltSelection = hltHighLevel.clone(TriggerResultsTag = 'TriggerResults::HLT', HLTPaths = mytrigs)
    process.hltSelection.throw = False




from PhysicsTools.PatAlgos.tools.pfTools import *

postfix = ""
jetAlgo="AK5"


usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo, runOnMC=False, postfix=postfix)

process.pathPreselection = cms.Path(
    process.patElectronIDs +
    getattr(process,"patPF2PATSequence"+postfix)
    )

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring (

#    'file:/tmp/oiorio/F81B1889-AF4B-DF11-85D3-001A64789DF4.root'
'file:/tmp/oiorio/EC0EE286-FA55-E011-B99B-003048F024F6.root'
#'file:/tmp/oiorio/D0B32FD9-6D87-E011-8572-003048678098.root'
),
duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)


process.demo = cms.EDAnalyzer('SimpleEventDumper',                              
                              verticesSource = cms.InputTag("PVFilterProducer"),
                              electronSource = cms.InputTag("cleanPatElectrons"),
                              muonSource     = cms.InputTag("patMuons"),
                              patmetSource = cms.InputTag("patMETs"),
                              calometSource = cms.InputTag("met"), #uncorrected
                              pfmetSource = cms.InputTag("pfMet"),
                              tcmetSource = cms.InputTag("tcMet"),
                              patjetSource = cms.InputTag("patJets"),
                              pfjetSource = cms.InputTag("ak5PFJets"), #uncorrected
                              pfpatjetSource = cms.InputTag("patJetsAK5PF"), #corrected, and possibility to access b-tagging for it
                              jptjetSource = cms.InputTag("patJetsAK5JPT"),
                              lep_pt_min = cms.double(10),
                              mt_min = cms.double(20),
                              jet_pt_min = cms.double(20),
                              useL5corr = cms.bool(False),
                              useL5corr_including_gluons = cms.bool(False),#

                              imgSolStrategy = cms.int32(1), #0: ignore Img part; 1: adjust MT to MW (as TOP-09-005)
                             )



process.WLightFilter = process.flavorHistoryFilter.clone(pathToSelect = cms.int32(11))
process.WccFlter = process.flavorHistoryFilter.clone(pathToSelect = cms.int32(6))
process.WbbFilter = process.flavorHistoryFilter.clone(pathToSelect = cms.int32(5))

#process.hltFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","REDIGI38X")
#process.hltFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","REDIGI37X")
#process.hltFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","REDIGI")
process.hltFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.hltFilter.HLTPaths = mytrigs


process.baseLeptonSequence = cms.Path(
    process.basePathData 
    )

process.selection = cms.Path (
    process.preselection +
    process.nTuplesSkim
    )


from TopQuarkAnalysis.SingleTop.SingleTopNtuplizers_cff import saveNTuplesSkimLoose
from TopQuarkAnalysis.SingleTop.SingleTopNtuplizers_cff import saveNTuplesSkimMu

savePatTupleSkimLoose = cms.untracked.vstring(
    'drop *',

    'keep patMuons_selectedPatMuons_*_*',
    'keep patElectrons_selectedPatElectrons_*_*',
    'keep patJets_selectedPatJets_*_*',
    'keep patMETs_patMETs_*_*',
    'keep *_PVFilterProducer_*_*',

    'keep patJets_topJetsPF_*_*',
    'keep patMuons_looseMuons_*_*',
    'keep patElectrons_looseElectrons_*_*',
    'keep patMuons_tightMuons_*_*',
    'keep patElectrons_tightElectrons_*_*',

    'keep *_PDFInfo_*_*',
  
    'keep *_cFlavorHistoryProducer_*_*',
    'keep *_bFlavorHistoryProducer_*_*',
    )

## Output module configuration
process.singleTopNTuple = cms.OutputModule("PoolOutputModule",
#                                fileName = cms.untracked.string('rfio:/CST/cern.ch/user/o/oiorio/SingleTop/SubSkims/WControlSamples1.root'),
#                   fileName = cms.untracked.Bstring('/tmp/oiorio/edmntuple_tchannel_big.root'),
                   fileName = cms.untracked.string('edmntuple_DataMu_v1.root'),
                                             
                   SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('selection')),
                   outputCommands = saveNTuplesSkimLoose,
)

process.singleTopPatTuple = cms.OutputModule("PoolOutputModule",
#                                fileName = cms.untracked.string('rfio:/CST/cern.ch/user/o/oiorio/SingleTop/SubSkims/WControlSamples1.root'),
                   fileName = cms.untracked.string('pattuple_tchannel.root'),


                   SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('selection')),
                   outputCommands = savePatTupleSkimLoose
)
process.singleTopNTuple.dropMetaData = cms.untracked.string("ALL")

process.outpath = cms.EndPath(
    process.singleTopNTuple #+
    #process.singleTopPatTuple
    )

