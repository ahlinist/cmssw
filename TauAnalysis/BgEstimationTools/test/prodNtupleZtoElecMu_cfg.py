import FWCore.ParameterSet.Config as cms

process = cms.Process("prodNtupleZtoElecMu")

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
process.load("TauAnalysis.BgEstimationTools.bgEstSelectLayer1Jets_cff")

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
        'file:/afs/cern.ch/user/v/veelken/scratch0/CMSSW_2_2_10/src/TauAnalysis/Configuration/test/muTauSkim.root'
    ),
    skipEvents = cms.untracked.uint32(0)            
)

# event print-out
process.eventDump = cms.EDAnalyzer("EventDumpAnalyzer",
    evtSelFlags = process.selectEventsByBoolEventSelFlags.flags,
    plugin = cms.PSet(
        pluginName = cms.string('elecMuEventDump'),
        pluginType = cms.string('ElecMuEventDump'),

        l1GtReadoutRecordSource = cms.InputTag('hltGtDigis::HLT'),
        l1GtObjectMapRecordSource = cms.InputTag('hltL1GtObjectMap::HLT'),
        l1BitsToPrint = cms.vstring('L1_SingleEG5', 'L1_SingleEG8', 'L1_SingleEG10', 'L1_SingleEG12', 'L1_SingleEG15',
                                    'L1_SingleIsoEG5', 'L1_SingleIsoEG8', 'L1_SingleIsoEG10', 'L1_SingleIsoEG12', 'L1_SingleIsoEG15',
                                    'L1_SingleMu3', 'L1_SingleMu5', 'L1_SingleMu7', 'L1_SingleMu10', 'L1_SingleMu14'),
        
        hltResultsSource = cms.InputTag('TriggerResults::HLT'),
        hltPathsToPrint = cms.vstring('HLT_IsoEle15_L1I', 'HLT_Mu15', 'HLT_IsoMu11', 'HLT_IsoEle8_IsoMu7'),
    
        genParticleSource = cms.InputTag('genParticles'),
        genTauJetSource = cms.InputTag('tauGenJets'),
        electronSource = cms.InputTag('electronsBgEstPreselection'),
        muonSource = cms.InputTag('muonsBgEstPreselection'),
        tauSource = cms.InputTag('cleanLayer1TausSel'),
        diTauCandidateSource = cms.InputTag('elecMuPairsBgEstPreselection'),
        metSource = cms.InputTag('layer1METs'),
        genMEtSource = cms.InputTag('genMETWithMu'),
        jetSource = cms.InputTag('selectedLayer1JetsEt20Cumulative'),
    
        #output = cms.string("elecMuEventDump.txt"),
        output = cms.string("std::cout"),
    )
)

# produce ntuple
process.ntupleProducer = cms.EDAnalyzer("ObjValNtupleProducer",
    treeName = cms.string("bgEstEvents"),
    branches = cms.PSet(
        selElectronTrackIso = cms.PSet(
            pluginType = cms.string("PATElecMuPairValExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            value = cms.string("leg1.trackIso"),
            indices = cms.vuint32(0,1)
        ),
        selElectronRelTrackIso = cms.PSet(
            pluginType = cms.string("PATElecMuPairValExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            value = cms.string("leg1.trackIso/leg1.pt"),
            indices = cms.vuint32(0,1)
        ),
        selElectronEcalIso = cms.PSet(
            pluginType = cms.string("PATElecMuPairValExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            value = cms.string("leg1.ecalIso"),
            indices = cms.vuint32(0,1)
        ),
        selElectronRelEcalIso = cms.PSet(
            pluginType = cms.string("PATElecMuPairValExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            value = cms.string("leg1.ecalIso/leg1.pt"),
            indices = cms.vuint32(0,1)
        ),
        selElectronIso = cms.PSet(
            pluginType = cms.string("PATElecMuPairValExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            value = cms.string("leg1.trackIso + leg1.ecalIso"),
            indices = cms.vuint32(0,1)
        ),
        selElectronRelIso = cms.PSet(
            pluginType = cms.string("PATElecMuPairValExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            value = cms.string("(leg1.trackIso + leg1.ecalIso)/leg1.pt"),
            indices = cms.vuint32(0,1)
        ),
        selElectronTrkIP = cms.PSet(
            pluginType = cms.string("PATElecMuPairElectronIpExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            vertexSource = cms.InputTag("selectedPrimaryVertexPosition"),
            indices = cms.vuint32(0,1)
        ),
    
        selMuonTrackIso = cms.PSet(
            pluginType = cms.string("PATElecMuPairValExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            value = cms.string("leg1.trackIso"),
            indices = cms.vuint32(0,1)
        ),
        selMuonRelTrackIso = cms.PSet(
            pluginType = cms.string("PATElecMuPairValExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            value = cms.string("leg1.trackIso/leg1.pt"),
            indices = cms.vuint32(0,1)
        ),
        selMuonEcalIso = cms.PSet(
            pluginType = cms.string("PATElecMuPairValExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            value = cms.string("leg1.ecalIso"),
            indices = cms.vuint32(0,1)
        ),
        selMuonRelEcalIso = cms.PSet(
            pluginType = cms.string("PATElecMuPairValExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            value = cms.string("leg1.ecalIso/leg1.pt"),
            indices = cms.vuint32(0,1)
        ),
        selMuonIso = cms.PSet(
            pluginType = cms.string("PATElecMuPairValExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            value = cms.string("leg1.trackIso + leg1.ecalIso"),
            indices = cms.vuint32(0,1)
        ),
        selMuonRelIso = cms.PSet(
            pluginType = cms.string("PATElecMuPairValExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            value = cms.string("(leg1.trackIso + leg1.ecalIso)/leg1.pt"),
            indices = cms.vuint32(0,1)
        ),
        selMuonCaloComp = cms.PSet(
            pluginType = cms.string("PATElecMuPairMuonAntiPionExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            CaloCompCoefficient = cms.double(1.),
            SegmCompCoefficient = cms.double(0.),
            indices = cms.vuint32(0,1)
        ),
        selMuonSegmComp = cms.PSet(
            pluginType = cms.string("PATElecMuPairMuonAntiPionExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            CaloCompCoefficient = cms.double(0.),
            SegmCompCoefficient = cms.double(1.),
            indices = cms.vuint32(0,1)
        ),
        selMuonComp = cms.PSet(
            pluginType = cms.string("PATElecMuPairMuonAntiPionExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            CaloCompCoefficient = cms.double(0.8),
            SegmCompCoefficient = cms.double(1.2),
            indices = cms.vuint32(0,1)
        ),
        selMuonTrkIP = cms.PSet(
            pluginType = cms.string("PATElecMuPairMuonIpExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            vertexSource = cms.InputTag("selectedPrimaryVertexPosition"),
            indices = cms.vuint32(0,1)
        ),
    
        selDiTauAbsCharge = cms.PSet(
            pluginType = cms.string("PATElecMuPairValExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            value = cms.string("abs(charge)"),
            indices = cms.vuint32(0,1)
        ),
        selDiTauMt1MET = cms.PSet(
            pluginType = cms.string("PATElecMuPairValExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            value = cms.string("mt1MET"),
            indices = cms.vuint32(0,1)
        ),
        selDiTauMt2MET = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            value = cms.string("mt2MET"),
            indices = cms.vuint32(0,1)
        ),
        selDiTauDPhi12 = cms.PSet(
            pluginType = cms.string("PATElecMuPairValExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            value = cms.string("dPhi12"),
            indices = cms.vuint32(0,1)
        ),
        selDiTauPzetaDiff = cms.PSet(
            pluginType = cms.string("PATElecMuPairValExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            value = cms.string("pZeta - 1.5*pZetaVis"),
            indices = cms.vuint32(0,1)
        ),
        selDiTauMvis12 = cms.PSet(
            pluginType = cms.string("PATElecMuPairValExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection'),
            value = cms.string("p4Vis.mass"),
            indices = cms.vuint32(0,1)
        ),

        selCentralJetEt40bTaggingDiscrSimpleSecondaryVertex = cms.PSet(
            pluginType = cms.string("PATJetValExtractor"),
            src = cms.InputTag('selectedLayer1JetsEt40Cumulative'),
            value = cms.string("bDiscriminator('simpleSecondaryVertex')"),
            indices = cms.vuint32(0,1,2)
        ),
        selCentralJetEt40bTaggingDiscrCombinedSecondaryVertex = cms.PSet(
            pluginType = cms.string("PATJetValExtractor"),
            src = cms.InputTag('selectedLayer1JetsEt40Cumulative'),
            value = cms.string("bDiscriminator('combinedSecondaryVertex')"),
            indices = cms.vuint32(0,1,2)
        ),
        selCentralJetEt40bTaggingDiscrTrackCountingHighEff = cms.PSet(
            pluginType = cms.string("PATJetValExtractor"),
            src = cms.InputTag('selectedLayer1JetsEt40Cumulative'),
            value = cms.string("bDiscriminator('trackCountingHighEffBJetTags')"),
            indices = cms.vuint32(0,1,2)
        ),
        selCentralJetEt40bTaggingDiscrSoftElectron = cms.PSet(
            pluginType = cms.string("PATJetValExtractor"),
            src = cms.InputTag('selectedLayer1JetsEt40Cumulative'),
            value = cms.string("bDiscriminator('softElectronBJetTags')"),
            indices = cms.vuint32(0,1,2)
        ),
        selCentralJetEt40bTaggingDiscrSoftMuon = cms.PSet(
            pluginType = cms.string("PATJetValExtractor"),
            src = cms.InputTag('selectedLayer1JetsEt40Cumulative'),
            value = cms.string("bDiscriminator('softMuonBJetTags')"),
            indices = cms.vuint32(0,1,2)
        ),

        numTightElectrons = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('selectedLayer1ElectronsTightIdIndividual')
        ),
        numSelElectrons = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('selectedLayer1ElectronsEcalIsoLooseIsolationCumulative')
        ),
        numGlobalMuons = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('selectedLayer1MuonsGlobalIndividual')
        ),
        numSelMuons = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('selectedLayer1MuonsEcalIsoLooseIsolationCumulative')
        ),
        numSelDiTaus = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('elecMuPairsBgEstPreselection')
        ),        
        numCentralJetsEt20 = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('selectedLayer1JetsEt20Cumulative')
        ),
        numCentralJetsAlpha0point1 = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('selectedLayer1JetsAlpha0point1Cumulative')
        ),
        numCentralJetsAlpha0point3 = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('selectedLayer1JetsAlpha0point3Cumulative')
        ),
        numCentralJetsEt40 = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('selectedLayer1JetsEt40Cumulative')
        ),
        numCentralJetsEt60 = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('selectedLayer1JetsEt60Cumulative')
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
# import utility function for managing pat::METs
from TauAnalysis.Configuration.tools.metTools import *

# uncomment to add pfMET
# first Boolean swich on genMET with mu's production
# second Boolean swich on type-1 corrections
addPFMet(process, True, False)

# uncomment to replce caloMET by pfMET in all di-tau objects
replaceMETforDiTaus(process,
                    cms.InputTag('layer1METs'),
                    cms.InputTag('layer1PFMETs'))
#--------------------------------------------------------------------------------

from PhysicsTools.PatAlgos.tools.tauTools import * 
switchToPFTauFixedCone(process)

process.p = cms.Path( process.producePatTuple
                     * process.bgEstSelectLayer1Jets
#                    * process.printEventContent    # uncomment to enable dump of event content after PAT-tuple production
                     * process.selectZtoElecMuEvents
                     * process.genPhaseSpaceFilter
                     * process.produceBoolEventSelFlags
#                    * process.eventDump 
                     * process.selectEventsByBoolEventSelFlags
                     * process.ntupleProducer )

# print-out all python configuration parameter information
#print process.dumpPython()
