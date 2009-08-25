import FWCore.ParameterSet.Config as cms

process = cms.Process("prodNtupleZtoElecTau")

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
process.load("TauAnalysis.Configuration.selectZtoElecTau_cff")
process.load("TauAnalysis.BgEstimationTools.bgEstPreselZtoElecTau_cff")

# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_cfi import *
from TauAnalysis.BgEstimationTools.bgEstSampleDefinitionsZtoElecTau_cfi import *

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
        pluginName = cms.string('elecTauEventDump'),
        pluginType = cms.string('ElecTauEventDump'),

        l1GtReadoutRecordSource = cms.InputTag('hltGtDigis::HLT'),
        l1GtObjectMapRecordSource = cms.InputTag('hltL1GtObjectMap::HLT'),
        l1BitsToPrint = cms.vstring('L1_SingleEG5', 'L1_SingleEG8', 'L1_SingleEG10', 'L1_SingleEG12', 'L1_SingleEG15',
                                    'L1_SingleIsoEG5', 'L1_SingleIsoEG8', 'L1_SingleIsoEG10', 'L1_SingleIsoEG12', 'L1_SingleIsoEG15'),
    
        hltResultsSource = cms.InputTag('TriggerResults::HLT'),
        hltPathsToPrint = cms.vstring('HLT_IsoEle15_L1I'),
    
        genParticleSource = cms.InputTag('genParticles'),
        genTauJetSource = cms.InputTag('tauGenJets'),
        electronSource = cms.InputTag('electronsBgEstPreselection'),
        muonSource = cms.InputTag('cleanLayer1MuonsSel'),
        tauSource = cms.InputTag('selectedLayer1TausProngCumulative'),
        diTauCandidateSource = cms.InputTag('elecTauPairsBgEstPreselection'),
        metSource = cms.InputTag('layer1METs'),
        genMEtSource = cms.InputTag('genMETWithMu'),
        jetSource = cms.InputTag('selectedLayer1JetsEt20Cumulative'),
    
        #output = cms.string("elecTauEventDump.txt"),
        output = cms.string("std::cout"),
    )
)



# produce ntuple
process.ntupleProducer = cms.EDAnalyzer("ObjValNtupleProducer",
    treeName = cms.string("bgEstEvents"),
    branches = cms.PSet(
        selElectronTrackIso = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsBgEstPreselection'),
            value = cms.string("leg1.trackIso"),
            indices = cms.vuint32(0,1)
        ),
        selElectronRelTrackIso = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsBgEstPreselection'),
            value = cms.string("leg1.trackIso/leg1.pt"),
            indices = cms.vuint32(0,1)
        ),
        selElectronEcalIso = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsBgEstPreselection'),
            value = cms.string("leg1.ecalIso"),
            indices = cms.vuint32(0,1)
        ),
        selElectronRelEcalIso = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsBgEstPreselection'),
            value = cms.string("leg1.ecalIso/leg1.pt"),
            indices = cms.vuint32(0,1)
        ),
        selElectronIso = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsBgEstPreselection'),
            value = cms.string("leg1.trackIso + leg1.ecalIso"),
            indices = cms.vuint32(0,1)
        ),
        selElectronRelIso = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsBgEstPreselection'),
            value = cms.string("(leg1.trackIso + leg1.ecalIso)/leg1.pt"),
            indices = cms.vuint32(0,1)
        ),
        selElectronTrkIP = cms.PSet(
            pluginType = cms.string("PATElecTauPairElectronIpExtractor"),
            src = cms.InputTag('elecTauPairsBgEstPreselection'),
            vertexSource = cms.InputTag("selectedPrimaryVertexPosition"),
            indices = cms.vuint32(0,1)
        ),
    
        selDiTauAbsCharge = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsBgEstPreselection'),
            value = cms.string("abs(charge)"),
            indices = cms.vuint32(0,1)
        ),
        selDiTauMt1MET = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsBgEstPreselection'),
            value = cms.string("mt1MET"),
            indices = cms.vuint32(0,1)
        ),
        selDiTauDPhi12 = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsBgEstPreselection'),
            value = cms.string("dPhi12"),
            indices = cms.vuint32(0,1)
        ),
        selDiTauPzetaDiff = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsBgEstPreselection'),
            value = cms.string("pZeta - 1.5*pZetaVis"),
            indices = cms.vuint32(0,1)
        ),
        selDiTauMvis12 = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsBgEstPreselection'),
            value = cms.string("p4Vis.mass"),
            indices = cms.vuint32(0,1)
        ),

        selTauDiscrAgainstElectrons = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsBgEstPreselection'),
            value = cms.string("leg2.tauID('againstElectron')"),
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
        numSelTaus = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('selectedLayer1TausProngCumulative')
        ),
        numSelDiTaus = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('elecTauPairsBgEstPreselection')
        ),        
        numCentralJetsEt20 = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('selectedLayer1JetsEt20Cumulative')
        ),
        numCentralJetsAlpha0_1 = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('selectedLayer1JetsAlpha0_1Cumulative')
        ),
        numCentralJetsAlpha0_3 = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('selectedLayer1JetsAlpha0_3Cumulative')
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
# import utility function for switching pat::Tau input
# to different reco::Tau collection stored on AOD
from PhysicsTools.PatAlgos.tools.tauTools import * 

# comment-out to take reco::CaloTaus instead of reco::PFTaus
# as input for pat::Tau production
#switchToCaloTau(process)

# comment-out to take shrinking dR = 5.0/Et(PFTau) signal cone
# instead of fixed dR = 0.07 signal cone reco::PFTaus
# as input for pat::Tau production
switchToPFTauShrinkingCone(process)
#switchToPFTauFixedCone(process)
#--------------------------------------------------------------------------------

process.p = cms.Path( process.producePatTuple
                     * process.bgEstSelectLayer1Jets
#                    * process.printEventContent    # uncomment to enable dump of event content after PAT-tuple production
                     * process.selectZtoElecTauEvents
                     * process.genPhaseSpaceFilter
                     * process.produceBoolEventSelFlags
#                    * process.eventDump 
                     * process.selectEventsByBoolEventSelFlags
                     * process.ntupleProducer )

# print-out all python configuration parameter information
#print process.dumpPython()
