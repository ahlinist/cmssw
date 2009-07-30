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
        #'file:/afs/cern.ch/user/v/veelken/scratch0/CMSSW_2_2_10/src/TauAnalysis/Configuration/test/muTauSkim.root'
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_2_2_3/selEvents_ZtoMuTau_ZmumuPlusJets_part01.root',
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_2_2_3/selEvents_ZtoMuTau_ZmumuPlusJets_part02.root',
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_2_2_3/selEvents_ZtoMuTau_ZmumuPlusJets_part03.root'
    ),
    skipEvents = cms.untracked.uint32(0)            
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

process.tauProngCutBgEstPreselection = cms.EDFilter("BoolEventSelFlagProducer",
    pluginName = cms.string("tauProngCutBgEstPreselection"),
    pluginType = cms.string("PATCandViewMinEventSelector"),
    src = cms.InputTag('selectedLayer1TausProngCumulative'),
    minNumber = cms.uint32(1)                                                
)

process.muTauPairsBgEstPreselection = cms.EDProducer("PATMuTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('muonsBgEstPreselection'),
    srcLeg2 = cms.InputTag('selectedLayer1TausProngCumulative'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

process.muTauPairCutBgEstPreselection = cms.EDFilter("BoolEventSelFlagProducer",
    pluginName = cms.string("muTauPairCutBgEstPreselection"),
    pluginType = cms.string("PATCandViewMinEventSelector"),
    src = cms.InputTag('muTauPairsBgEstPreselection'),
    minNumber = cms.uint32(1)
)                                                                             

process.produceBoolEventSelFlags = cms.Sequence(
    process.muonsBgEstPreselection + process.muonTrkCutBgEstPreselection
   +process.tauProngCutBgEstPreselection
   +process.muTauPairsBgEstPreselection + process.muTauPairCutBgEstPreselection
)

process.selectEventsByBoolEventSelFlags = cms.EDFilter("MultiBoolEventSelFlagFilter",
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('primaryEventVertex'),
        cms.InputTag('primaryEventVertexQuality'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('muonTrkCutBgEstPreselection'),
        cms.InputTag('tauProngCutBgEstPreselection'),
        cms.InputTag('muTauPairCutBgEstPreselection')
    )
)
#--------------------------------------------------------------------------------

# event print-out
process.eventDump = cms.EDAnalyzer("EventDumpAnalyzer",
    evtSelFlags = process.selectEventsByBoolEventSelFlags.flags,
    plugin = cms.PSet(
        pluginName = cms.string('muTauEventDump'),
        pluginType = cms.string('MuTauEventDump'),

        l1GtReadoutRecordSource = cms.InputTag('hltGtDigis::HLT'),
        l1GtObjectMapRecordSource = cms.InputTag('hltL1GtObjectMap::HLT'),
        l1BitsToPrint = cms.vstring('L1_SingleMu3', 'L1_SingleMu5', 'L1_SingleMu7', 'L1_SingleMu10', 'L1_SingleMu14'),
    
        hltResultsSource = cms.InputTag('TriggerResults::HLT'),
        hltPathsToPrint = cms.vstring('HLT_Mu15', 'HLT_IsoMu11'),
    
        genParticleSource = cms.InputTag('genParticles'),
        genTauJetSource = cms.InputTag('tauGenJets'),
        electronSource = cms.InputTag('cleanLayer1ElectronsSel'),
        muonSource = cms.InputTag('muonsBgEstPreselection'),
        tauSource = cms.InputTag('selectedLayer1TausProngCumulative'),
        diTauCandidateSource = cms.InputTag('muTauPairsBgEstPreselection'),
        metSource = cms.InputTag('layer1METs'),
        genMEtSource = cms.InputTag('genMETWithMu'),
        jetSource = cms.InputTag('selectedLayer1JetsEt20Cumulative'),
        #recoTrackSource = cms.InputTag('generalTracks'),
        #pfChargedHadronSource = cms.InputTag('pfAllChargedHadrons'),
        #pfGammaSource = cms.InputTag('pfAllPhotons'),
        #pfNeutralHadronSource = cms.InputTag('pfAllNeutralHadrons'),
    
        #output = cms.string("muTauEventDump.txt"),
        output = cms.string("std::cout"),
    )
)



# produce ntuple
process.ntupleProducer = cms.EDAnalyzer("ObjValNtupleProducer",
    treeName = cms.string("bgEstEvents"),
    branches = cms.PSet(
        selMuonTrackIso = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            value = cms.string("leg1.trackIso"),
            indices = cms.vuint32(0,1)
        ),
        selMuonRelTrackIso = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            value = cms.string("leg1.trackIso/leg1.pt"),
            indices = cms.vuint32(0,1)
        ),
        selMuonEcalIso = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            value = cms.string("leg1.ecalIso"),
            indices = cms.vuint32(0,1)
        ),
        selMuonRelEcalIso = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            value = cms.string("leg1.ecalIso/leg1.pt"),
            indices = cms.vuint32(0,1)
        ),
        selMuonIso = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            value = cms.string("leg1.trackIso + leg1.ecalIso"),
            indices = cms.vuint32(0,1)
        ),
        selMuonRelIso = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            value = cms.string("(leg1.trackIso + leg1.ecalIso)/leg1.pt"),
            indices = cms.vuint32(0,1)
        ),
        selMuonCaloComp = cms.PSet(
            pluginType = cms.string("PATMuTauPairMuonAntiPionExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            CaloCompCoefficient = cms.double(1.),
            SegmCompCoefficient = cms.double(0.),
            indices = cms.vuint32(0,1)
        ),
        selMuonSegmComp = cms.PSet(
            pluginType = cms.string("PATMuTauPairMuonAntiPionExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            CaloCompCoefficient = cms.double(0.),
            SegmCompCoefficient = cms.double(1.),
            indices = cms.vuint32(0,1)
        ),
        selMuonComp = cms.PSet(
            pluginType = cms.string("PATMuTauPairMuonAntiPionExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            CaloCompCoefficient = cms.double(0.8),
            SegmCompCoefficient = cms.double(1.2),
            indices = cms.vuint32(0,1)
        ),
        selMuonTrkIP = cms.PSet(
            pluginType = cms.string("PATMuTauPairMuonIpExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            vertexSource = cms.InputTag("selectedPrimaryVertexPosition"),
            indices = cms.vuint32(0,1)
        ),
    
        selDiTauAbsCharge = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            value = cms.string("abs(charge)"),
            indices = cms.vuint32(0,1)
        ),
        selDiTauMt1MET = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            value = cms.string("mt1MET"),
            indices = cms.vuint32(0,1)
        ),
        selDiTauDPhi12 = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            value = cms.string("dPhi12"),
            indices = cms.vuint32(0,1)
        ),
        selDiTauPzetaDiff = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            value = cms.string("pZeta - 1.5*pZetaVis"),
            indices = cms.vuint32(0,1)
        ),
        selDiTauMvis12 = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            value = cms.string("p4Vis.mass"),
            indices = cms.vuint32(0,1)
        ),

        selTauDiscrAgainstMuons = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            value = cms.string("leg2.tauID('againstMuon')"),
            indices = cms.vuint32(0,1)
        ),

        numGlobalMuons = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('selectedLayer1MuonsGlobalIndividual')
        ),
        numSelMuons = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('selectedLayer1MuonsEcalIsoLooseIsolationCumulative')
        ),
        numSelTaus = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('selectedLayer1TausProngCumulative')
        ),
        numSelDiTaus = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection')
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
#__process.source.skipEvents = cms.untracked.uint32(#skipEvents#)
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
#                    * process.printEventContent    # uncomment to enable dump of event content after PAT-tuple production
                     * process.selectZtoMuTauEvents
                     * process.genPhaseSpaceFilter
                     * process.produceBoolEventSelFlags
#                    * process.eventDump 
                     * process.selectEventsByBoolEventSelFlags
                     * process.ntupleProducer )

# print-out all python configuration parameter information
#print process.dumpPython()
