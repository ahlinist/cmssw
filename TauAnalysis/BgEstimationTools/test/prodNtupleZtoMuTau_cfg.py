import FWCore.ParameterSet.Config as cms

process = cms.Process("prodNtupleZtoMuTau")

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
process.load("TauAnalysis.Configuration.selectZtoMuTau_cff")

# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_cfi import *
from TauAnalysis.BgEstimationTools.bgEstSampleDefinitionsZtoMuTau_cfi import *

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
        #'rfio:/castor/cern.ch/user/v/veelken/CMSSW_2_2_3/muTauSkim.root'
        'file:/afs/cern.ch/user/v/veelken/scratch0/CMSSW_2_2_10/src/TauAnalysis/Configuration/test/muTauSkim.root'
    )
)

#--------------------------------------------------------------------------------
# preselect events entering ntuple
process.genPhaseSpaceFilter = cms.EDFilter("EventSelPluginFilter",
    selector = cms.PSet(
        pluginName = cms.string('genPhaseSpaceCut'),
        pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
        src = cms.InputTag('genPhaseSpaceEventInfo'),
        cut = cms.string('')
    )
)

process.muonEcalIsoCutLooseIsolation = cms.EDFilter("BoolEventSelFlagProducer",
    selectors = cms.VPSet(
        cms.PSet(
            pluginName = cms.string("muonEcalIsoCutLooseIsolation"),
            pluginType = cms.string("PATCandViewMinEventSelector"),
            src = cms.InputTag('selectedLayer1MuonsEcalIsoLooseIsolationCumulative'),
            minNumber = cms.uint32(1),
            instanceName = cms.string('prodNtupleZtoMuTau')
        )
    )
)

process.muTauPairsLooseSelection = cms.EDProducer("PATMuTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedLayer1MuonsEcalIsoLooseIsolationCumulative'),
    srcLeg2 = cms.InputTag('selectedLayer1TausForMuTauMuonVetoCumulative'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

process.muTauPairCutLooseSelection = cms.EDProducer("BoolEventSelFlagProducer",
    selectors = cms.VPSet(
        cms.PSet(
            pluginName = cms.string("muTauPairCutLooseSelection"),
            pluginType = cms.string("PATCandViewMinEventSelector"),
            src = cms.InputTag('muTauPairsLooseSelection'),
            minNumber = cms.uint32(1),
            instanceName = cms.string('prodNtupleZtoMuTau')
        )
    )
)                                                                             

process.produceBoolEventSelFlags = cms.Sequence( process.muonEcalIsoCutLooseIsolation
                                                +process.muTauPairsLooseSelection + process.muTauPairCutLooseSelection )

process.selectEventsByBoolEventSelFlags = cms.EDFilter("MultiBoolEventSelFlagFilter",
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('primaryEventVertex'),
        cms.InputTag('primaryEventVertexQuality'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('muonEcalIsoCutLooseIsolation', 'prodNtupleZtoMuTau'),
        cms.InputTag('tauMuonVeto', 'cumulative'),                                                        
        cms.InputTag('muTauPairCutLooseSelection', 'prodNtupleZtoMuTau')
    )
)
#--------------------------------------------------------------------------------

# produce ntuple
process.ntupleProducer = cms.EDAnalyzer("ObjValNtupleProducer",
    treeName = cms.string("bgEstEvents"),
    branches = cms.PSet(
        muonTrackIso = cms.PSet(
            pluginType = cms.string("PATMuonValExtractor"),
            src = cms.InputTag('selectedLayer1MuonsEcalIsoLooseIsolationCumulative'),
            value = cms.string("trackIso")
        ),
        muonRelTrackIso = cms.PSet(
            pluginType = cms.string("PATMuonValExtractor"),
            src = cms.InputTag('selectedLayer1MuonsEcalIsoLooseIsolationCumulative'),
            value = cms.string("trackIso/pt")
        ),
        muonEcalIso = cms.PSet(
            pluginType = cms.string("PATMuonValExtractor"),
            src = cms.InputTag('selectedLayer1MuonsEcalIsoLooseIsolationCumulative'),
            value = cms.string("ecalIso")
        ),
        muonRelEcalIso = cms.PSet(
            pluginType = cms.string("PATMuonValExtractor"),
            src = cms.InputTag('selectedLayer1MuonsEcalIsoLooseIsolationCumulative'),
            value = cms.string("ecalIso/pt")
        ),
        muonIso = cms.PSet(
            pluginType = cms.string("PATMuonValExtractor"),
            src = cms.InputTag('selectedLayer1MuonsEcalIsoLooseIsolationCumulative'),
            value = cms.string("trackIso + ecalIso")
        ),
        muonRelIso = cms.PSet(
            pluginType = cms.string("PATMuonValExtractor"),
            src = cms.InputTag('selectedLayer1MuonsEcalIsoLooseIsolationCumulative'),
            value = cms.string("(trackIso + ecalIso)/pt")
        ),
        muonCaloComp = cms.PSet(
            pluginType = cms.string("PATMuonAntiPionExtractor"),
            src = cms.InputTag('selectedLayer1MuonsEcalIsoLooseIsolationCumulative'),
            CaloCompCoefficient = cms.double(1.),
            SegmCompCoefficient = cms.double(0.)
        ),
        muonSegmComp = cms.PSet(
            pluginType = cms.string("PATMuonAntiPionExtractor"),
            src = cms.InputTag('selectedLayer1MuonsEcalIsoLooseIsolationCumulative'),
            CaloCompCoefficient = cms.double(0.),
            SegmCompCoefficient = cms.double(1.)
        ),
        muonComp = cms.PSet(
            pluginType = cms.string("PATMuonAntiPionExtractor"),
            src = cms.InputTag('selectedLayer1MuonsEcalIsoLooseIsolationCumulative'),
            CaloCompCoefficient = cms.double(0.8),
            SegmCompCoefficient = cms.double(1.2)
        ),
        muonTrkIP = cms.PSet(
            pluginType = cms.string("PATMuonIpExtractor"),
            src = cms.InputTag('selectedLayer1MuonsEcalIsoLooseIsolationCumulative'),
            vertexSource = cms.InputTag("selectedPrimaryVertexPosition")
        ),
    
        diTauAbsCharge = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsLooseSelection'),
            value = cms.string("abs(charge)")
        ),
        diTauMt1MET = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsLooseSelection'),
            value = cms.string("mt1MET")
        ),
        diTauDPhi12 = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsLooseSelection'),
            value = cms.string("dPhi12")
        ),
        diTauPzetaDiff = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsLooseSelection'),
            value = cms.string("pZeta - 1.5*pZetaVis")
        ),
        diTauMvis12 = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsLooseSelection'),
            value = cms.string("p4Vis.mass")
        ),

        tauDiscrAgainstMuons = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsLooseSelection'),
            value = cms.string("leg2.tauID('againstMuon')")
        ),

        numGlobalMuons = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('selectedLayer1MuonsGlobalIndividual')
        ),
        numTaus = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('selectedLayer1TausProngCumulative')
        ),
        numCentralJets = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('selectedLayer1JetsEt20Cumulative')
        ),

        eventWeight = cms.PSet(
            pluginType = cms.string("ConstObjValExtractor"),
            value = cms.double(1.)
        )
    )
)

process.TFileService = cms.Service("TFileService", 
    fileName = cms.string('bgEstNtuple.root')
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
#__process.source.fileNames = #bgEstSampleFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.genPhaseSpaceFilter.selector = copy.deepcopy(#genPhaseSpaceCut#)
#__process.TFileService.fileName = #bgEstNtupleOutputFileName#
#__process.ntupleProducer.branches.eventWeight.value = cms.double(#corrFactor#*intLumiData/#intLumi#)
#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for switching pat::Tau input
# to different reco::Tau collection stored on AOD
from PhysicsTools.PatAlgos.tools.tauTools import * 

# comment-out to take reco::CaloTaus instead of reco::PFTaus
# as input for pat::Tau production
#switchToCaloTau(process)

# comment-out to take shrinking dR = 5.0/Et(PFTau) signal cone
# instead of fixed dR = 0.07 signal cone reco::PFTaus
# as input for pat::Tau production
#switchToPFTauShrinkingCone(process)
switchToPFTauFixedCone(process)
#--------------------------------------------------------------------------------

process.p = cms.Path( process.producePatTuple
#                    +process.printEventContent    # uncomment to enable dump of event content after PAT-tuple production
                     +process.selectZtoMuTauEvents
                     +process.genPhaseSpaceFilter
                     +process.produceBoolEventSelFlags
                     +process.selectEventsByBoolEventSelFlags
                     +process.ntupleProducer )

# print-out all python configuration parameter information
#print process.dumpPython()
