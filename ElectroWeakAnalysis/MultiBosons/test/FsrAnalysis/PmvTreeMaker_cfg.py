import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

## setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

## setup any defaults you want
options.maxEvents = 10 # -1 means all events
options.inputFiles = ["file:/uscmst1b_scratch/lpc1/3DayLifetime/veverka/mu/VGammaSkim_LyonSyncTest_Dec22ReReco_v2_DimuonSkim_1_of_4.root"]
options.outputFile = "testSelector.root"

## get and parse the command line arguments
options.parseArguments()


## define the process
process = cms.Process("TEST")

## Message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1
## Enable LogInfo
process.MessageLogger.cerr.INFO.limit = 100
#process.MessageLogger.debugModules = ["selectedZMuMuGammas"]
#process.MessageLogger.cerr.threshold = "DEBUG"

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring() + options.inputFiles
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(options.outputFile)
)


from ElectroWeakAnalysis.MultiBosons.Selectors.muonSelector_cfi \
    import muonSelection_FsrApr082011_PixelMatchVeto as muonSelection

from ElectroWeakAnalysis.MultiBosons.Selectors.diLeptonSelector_cfi \
    import diMuonSelection_Fsr2011Apr11_PixelMatchVeto as diMuonSelection

from ElectroWeakAnalysis.MultiBosons.Selectors.photonSelector_cfi \
    import photonSelection_Fsr2011Apr11_PixelMatchVeto as photonSelection

from ElectroWeakAnalysis.MultiBosons.Selectors.ZMuMuGammaSelector_cfi \
    import ZMuMuGammaSelection_Fsr2011Apr11_PixelMatchVeto as ZMuMuGammaSelection

process.selectedMuons = cms.EDFilter("VGammaMuonFilter",
    filterParams = muonSelection,
    src = cms.InputTag("cleanPatMuonsTriggerMatch","","PAT"),
    filter = cms.bool(True),
    verbosity = cms.untracked.uint32(2)
)

process.goodDiMuons = cms.EDProducer("CandViewShallowClonePtrCombiner",
    checkCharge = cms.bool(False),
    cut = cms.string("mass > 0"), ## dummy cut
    decay = cms.string("selectedMuons selectedMuons"),
    roles = cms.vstring("muon1", "muon2")
)

process.selectedDiMuons = cms.EDFilter("VGammaDiLeptonFilter",
    filterParams = diMuonSelection,
    src = cms.InputTag("goodDiMuons"),
    filter = cms.bool(True),
    verbosity = cms.untracked.uint32(2)
)

process.goodPhotons = cms.EDFilter("PATPhotonSelector",
    cut = cms.string("""
        superCluster.energy * sin(superCluster.position.theta) > 10 &&
        hadronicOverEm < 0.5
        """),
    src = cms.InputTag("cleanPatPhotonsTriggerMatch"),
    filter = cms.bool(True)
)

process.selectedPhotons = cms.EDFilter("VGammaPhotonFilter",
    filterParams = photonSelection,
    src = cms.InputTag("goodPhotons"),
    filter = cms.bool(True),
    verbosity = cms.untracked.uint32(2)
)

process.goodZMuMuGammas = cms.EDProducer("CandViewShallowClonePtrCombiner",
    checkCharge = cms.bool(False),
    cut = cms.string("mass > 0"), ## dummy cut
    decay = cms.string("selectedDiMuons selectedPhotons"),
    roles = cms.vstring("dimuon", "photon")
)

process.selectedZMuMuGammas = cms.EDFilter("ZMuMuGammaFilter",
    filterParams = ZMuMuGammaSelection,
    src = cms.InputTag("goodZMuMuGammas"),
    filter = cms.bool(True),
    verbosity = cms.untracked.uint32(2)
)

process.selectionSequence = cms.Sequence(
    process.selectedMuons *
    process.goodDiMuons *
    process.selectedDiMuons *
    process.goodPhotons *
    process.selectedPhotons *
    process.goodZMuMuGammas *
    process.selectedZMuMuGammas
)

process.p = cms.Path(
    process.selectionSequence
)

process.options.wantSummary = False

ZMuMuGammaSelection.cutsToIgnore = cms.vstring("photonTrackIso")

if __name__ == "__main__": import user

