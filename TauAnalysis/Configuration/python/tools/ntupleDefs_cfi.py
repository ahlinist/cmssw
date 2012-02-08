import FWCore.ParameterSet.Config as cms

def doNtupleAHtoElecTau(process, channel, sample, id):

    outputFileName = 'diTauNtuple_%s_%s_%s.root' % (
            channel, sample, id)

    process.isRecElecTau = cms.EDProducer("BoolEventSelFlagProducer",
            pluginName = cms.string('isRecElecTau'),
            pluginType = cms.string('MultiBoolEventSelFlagSelector'),
            flags = cms.VInputTag(
                cms.InputTag('Trigger'),
                cms.InputTag('genPhaseSpaceCut'),
                cms.InputTag('primaryEventVertexPosition'),
                cms.InputTag('electronTrkIPcut', 'cumulative'),
                cms.InputTag('tauMuonVeto', 'cumulative'),
                cms.InputTag('diElecPairZeeHypothesisVetoByLooseIsolation'),
            )
    )
    process.p += process.isRecElecTau

    process.filterFinalEvents = cms.EDFilter("BoolEventFilter",
            src = cms.InputTag("isRecElecTau")
    )
    process.p += process.filterFinalEvents

    process.diTauNtuple = cms.EDProducer("CandViewNtpProducer",
            src = cms.InputTag("allElecTauPairs"),
            lazyParser = cms.untracked.bool(True),
            eventInfo = cms.untracked.bool(True),
            variables = cms.VPSet(
                cms.PSet(
                    tag = cms.untracked.string("visMass"),
                    quantity = cms.untracked.string("p4Vis.mass")
                    ),
                cms.PSet(
                    tag = cms.untracked.string("fullMassFit"),
                    quantity = cms.untracked.string("nSVfitSolution('psKine_MEt_logM_fit').mass")
                    ),
                cms.PSet(
                    tag = cms.untracked.string("fullMassInt"),
                    quantity = cms.untracked.string("nSVfitSolution('psKine_MEt_logM_int').mass")
                    ),
                cms.PSet(
                    tag = cms.untracked.string("mt1MET"),
                    quantity = cms.untracked.string("mt1MET")
                    ),
                cms.PSet(
                    tag = cms.untracked.string("pZetaDiff"),
                    quantity = cms.untracked.string("pZeta - 1.5*pZetaVis")
                    )
            )
    )
    process.p += process.diTauNtuple 
    
    nTupleEventContent = cms.PSet(
        outputCommands = cms.untracked.vstring('drop *')
    )
    nTupleEventContent.outputCommands.extend(
        ['keep *_diTauNtuple_*_*']
    )
    process.saveNtuple = cms.OutputModule("PoolOutputModule",
        nTupleEventContent,
        fileName = cms.untracked.string(outputFileName)
    )
    process.endtasks += process.saveNtuple

