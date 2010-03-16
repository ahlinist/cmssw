import FWCore.ParameterSet.Config as cms

process = cms.Process("skimZtoElecMu")

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'IDEAL_V12::All'

#--------------------------------------------------------------------------------
# import sequence for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectZtoElecMu_cff")
process.load("TauAnalysis.BgEstimationTools.bgEstPreselZtoElecMu_cff")

# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecMu_cfi import *
from TauAnalysis.BgEstimationTools.bgEstSampleDefinitionsZtoElecMu_cfi import *

# import event-content definition of products to be stored in patTuple
from TauAnalysis.Configuration.patTupleEventContent_cff import *
from TauAnalysis.Skimming.EventContent_cff import *
#--------------------------------------------------------------------------------

# print event content 
process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/tmp/sunil/bgEstSampleZtoElecMu_Zmumu.root'
    )
)

#--------------------------------------------------------------------------------
# preselect events considered for "template" and "generalized matrix"
# background estimation methods
process.genPhaseSpaceFilter = cms.EDFilter("EventSelPluginFilter",
    selector = cms.PSet(
        pluginName = cms.string('genPhaseSpaceCut'),
        pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
        src = cms.InputTag('genPhaseSpaceEventInfo'),
        cut = cms.string('')
    )
)

process.electronsBgEstPreselection = cms.EDFilter("PATElectronSelector",
    src = cms.InputTag('selectedLayer1ElectronsEcalIsoLooseIsolationCumulative'),                                        
    cut = cms.string('gsfTrack.isNonnull'),
    filter = cms.bool(False)
)

process.electronTrkCutBgEstPreselection = cms.EDFilter("BoolEventSelFlagProducer",
    pluginName = cms.string("electronTrkCutBgEstPreselection"),
    pluginType = cms.string("PATCandViewMinEventSelector"),
    src = cms.InputTag('electronsBgEstPreselection'),
    minNumber = cms.uint32(1)
)

process.muonsBgEstPreselection = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag('selectedLayer1MuonsEcalIsoLooseIsolationCumulative'),                                        
    cut = cms.string('innerTrack.isNonnull'),
    filter = cms.bool(False)
)

process.muonTrkCutBgEstPreselection = cms.EDFilter("BoolEventSelFlagProducer",
    pluginName = cms.string("muonTrkCutBgEstPreselection"),
    pluginType = cms.string("PATCandViewMinEventSelector"),
    src = cms.InputTag('muonsBgEstPreselection'),
    minNumber = cms.uint32(1)
)

process.elecMuPairsBgEstPreselection = cms.EDProducer("PATElecMuPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('electronsBgEstPreselection'),
    srcLeg2 = cms.InputTag('muonsBgEstPreselection'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                                                  
    verbosity = cms.untracked.int32(0)
)

process.elecMuPairCutBgEstPreselection = cms.EDFilter("BoolEventSelFlagProducer",
    pluginName = cms.string("elecMuPairCutBgEstPreselection"),
    pluginType = cms.string("PATCandViewMinEventSelector"),
    src = cms.InputTag('elecMuPairsBgEstPreselection'),
    minNumber = cms.uint32(1)
)                                                                             

process.produceBoolEventSelFlags = cms.Sequence(
    process.electronsBgEstPreselection + process.electronTrkCutBgEstPreselection
   +process.muonsBgEstPreselection + process.muonTrkCutBgEstPreselection
   +process.elecMuPairsBgEstPreselection + process.elecMuPairCutBgEstPreselection
)

process.selectEventsByBoolEventSelFlags = cms.EDFilter("MultiBoolEventSelFlagFilter",
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('primaryEventVertex'),
        cms.InputTag('primaryEventVertexQuality'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('electronTrkCutBgEstPreselection'),
        cms.InputTag('muonTrkCutBgEstPreselection'),
        cms.InputTag('elecMuPairCutBgEstPreselection')
    )
)
#--------------------------------------------------------------------------------

process.saveBgEstSample = cms.OutputModule("PoolOutputModule",
    tauAnalysisEventContent,                                        
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('bgEstSkimPath')
    ),
    fileName = cms.untracked.string('bgEstSample.root')
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
#__process.source.fileNames = #inputFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.genPhaseSpaceFilter.selector = copy.deepcopy(#genPhaseSpaceCut#)
#__process.saveBgEstSample.fileName = #bgEstSampleOutputFileName#
#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for managing pat::METs
from TauAnalysis.Configuration.tools.metTools import *

# uncomment to add pfMET
# first Boolean swich on genMET with mu's production
# second Boolean swich on type-1 corrections
addPFMet(process, True, False)

# uncomment to replce caloMET by pfMET in all di-tau objects
#replaceMETforDiTaus(process,
#                    cms.InputTag('layer1METs'),
#                    cms.InputTag('layer1PFMETs'))
#--------------------------------------------------------------------------------

from PhysicsTools.PatAlgos.tools.tauTools import * 
switchToPFTauFixedCone(process)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.bgEstSkimPath = cms.Path(
    process.producePatTuple
#  * process.printEventContent    # uncomment to enable dump of event content after PAT-tuple production
   * process.selectZtoElecMuEvents
   * process.genPhaseSpaceFilter
   * process.produceBoolEventSelFlags
   * process.selectEventsByBoolEventSelFlags
)

process.o = cms.EndPath( process.saveBgEstSample )

# print-out all python configuration parameter information
#print process.dumpPython()
