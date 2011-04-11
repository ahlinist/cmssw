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
process.MessageLogger.debugModules = ["selectedZMuMuGammas"]
process.MessageLogger.cerr.threshold = "DEBUG"



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
    import FsrApr082011_selection as muonSelection

from ElectroWeakAnalysis.MultiBosons.Selectors.diLeptonSelector_cfi \
    import diMuonSelection_Fsr2011Apr11 as diMuonSelection

from ElectroWeakAnalysis.MultiBosons.Selectors.photonSelector_cfi \
    import Fsr2011Apr11_selection as photonSelection

from ElectroWeakAnalysis.MultiBosons.Selectors.ZMuMuGammaSelector_cfi \
    import ZMuMuGammaSelection_Fsr2011Apr11 as ZMuMuGammaSelection

#from ElectroWeakAnalysis.MultiBosons.Selectors.muonSelector_cfi \
    #import Sep062010_selection as muonSelection

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

#process.selectedMuonPtrs = cms.EDFilter("VGammaPatMuonPtrSelector",
    #muonSelection,
    #src = cms.InputTag("cleanPatMuonsTriggerMatch","","PAT"),
    #filter = cms.bool(False)
#)

process.countsBefore = cms.EDAnalyzer("CandViewCountAnalyzer",
    nbins = cms.untracked.uint32(10),
    histograms = cms.untracked.VPSet(
        cms.PSet( src = cms.untracked.InputTag("cleanPatMuonsTriggerMatch") ),
        cms.PSet( src = cms.untracked.InputTag("cleanPatPhotonsTriggerMatch") ),
    )
)

process.countsAfter = cms.EDAnalyzer("CandViewCountAnalyzer",
    nbins = cms.untracked.uint32(10),
    histograms = cms.untracked.VPSet(
        cms.PSet( src = cms.untracked.InputTag("selectedMuons") ),
        cms.PSet( src = cms.untracked.InputTag("selectedPhotons") ),
        cms.PSet( src = cms.untracked.InputTag("selectedDiMuons") ),
        cms.PSet( src = cms.untracked.InputTag("selectedZMuMuGammas") ),
    )
)


from ElectroWeakAnalysis.MultiBosons.Histogramming.muonHistos_cfi import muonHistos
from ElectroWeakAnalysis.MultiBosons.Histogramming.photonHistos_cfi import photonHistos
from ElectroWeakAnalysis.MultiBosons.Histogramming.leafKineHistos_cff import *
from ElectroWeakAnalysis.MultiBosons.Histogramming.isoHistos_cff import *

process.allMuonsHistos = muonHistos.clone(
    src = "cleanPatMuonsTriggerMatch",
    #histograms = (leafKineHistos + isoHistos).copy(),
    histograms = (leafKineHistos).copy(),
)

process.selectedMuonsHistos = process.allMuonsHistos.clone(src = "selectedMuons")

process.allPhotonsHistos = photonHistos.clone(
    src = "cleanPatPhotonsTriggerMatch",
    histograms = (leafKineHistos).copy(),
)

process.selectedPhotonsHistos = process.allPhotonsHistos.clone(src = "selectedPhotons")

process.p = cms.Path(
    process.allMuonsHistos +
    process.allPhotonsHistos +
    process.countsBefore +
    process.selectedMuons * process.selectedMuonsHistos *
    process.goodDiMuons * process.selectedDiMuons +
    process.goodPhotons * process.selectedPhotons * process.selectedPhotonsHistos *
    process.goodZMuMuGammas * process.selectedZMuMuGammas *
    process.countsAfter
)

process.options.wantSummary = True

if __name__ == "__main__": import user

