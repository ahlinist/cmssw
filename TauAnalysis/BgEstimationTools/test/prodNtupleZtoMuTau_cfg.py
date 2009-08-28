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
process.load("TauAnalysis.BgEstimationTools.bgEstPatJetSelection_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectZtoMuTau_cff")
process.load("TauAnalysis.BgEstimationTools.bgEstPreselZtoMuTau_cff")

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
        'file:/afs/cern.ch/user/v/veelken/scratch0/CMSSW_2_2_10/src/TauAnalysis/Configuration/test/muTauSkim.root'
    ),
    skipEvents = cms.untracked.uint32(0)            
)

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
    
        #output = cms.string("muTauEventDump.txt"),
        output = cms.string("std::cout"),
    )
)

# produce ntuple
kineReweight_fileName = cms.string('rfio:/castor/cern.ch/user/v/veelken/bgEstKineReweights/bgEstKineEventReweightsZtoMuTau.root')
kineReweight_dqmDirectory = "bgEstKineEventReweights"
kineReweight_meName = "diTauDPhi12"
kineVarExtractor_config = cms.PSet(
    pluginType = cms.string("PATMuTauPairValExtractor"),
    src = cms.InputTag('muTauPairsBgEstPreselection'),
    value = cms.string("dPhi12"),
    indices = cms.vuint32(0)
)
process.ntupleProducer = cms.EDAnalyzer("ObjValNtupleProducer",
    treeName = cms.string("bgEstEvents"),
    branches = cms.PSet(
        selMuonTrackIso = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            value = cms.string("leg1.trackIso"),
            indices = cms.vuint32(0,1)
        ),
        selMuonEcalIso = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            value = cms.string("leg1.ecalIso"),
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

        selTauTrackIso = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            value = cms.string("leg2.chargedParticleIso()"),
            indices = cms.vuint32(0,1)
        ),
        selTauTrackIsoDiscr = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            value = cms.string("leg2.tauID('trackIsolation')"),
            indices = cms.vuint32(0,1)
        ),
        selTauEcalIso = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            value = cms.string("leg2.gammaParticleIso()"),
            indices = cms.vuint32(0,1)
        ),
        selTauEcalIsoDiscr = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            value = cms.string("leg2.tauID('ecalIsolation')"),
            indices = cms.vuint32(0,1)
        ),
        selTauDiscrAgainstMuons = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection'),
            value = cms.string("leg2.tauID('againstMuon')"),
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

        numGlobalMuons = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('selectedLayer1MuonsGlobalIndividual')
        ),
        numSelMuons = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('muonsBgEstPreselection')
        ),
        numSelTaus = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('tausBgEstPreselection')
        ),
        numSelDiTaus = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('muTauPairsBgEstPreselection')
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
        ),

        kineEventReweight_ZtautauEnriched = cms.PSet(
            pluginType = cms.string("KineEventReweightExtractor"),
            weightLookupTable = cms.PSet(
                fileName = kineReweight_fileName,
                meName = cms.string(kineReweight_dqmDirectory + "/" + "Ztautau" + "/" + kineReweight_meName)
            ),
            kineVarExtractor = kineVarExtractor_config
        ),
        kineEventReweight_WplusJetsEnriched = cms.PSet(
            pluginType = cms.string("KineEventReweightExtractor"),
            weightLookupTable = cms.PSet(
                fileName = kineReweight_fileName,
                meName = cms.string(kineReweight_dqmDirectory + "/" + "WplusJets" + "/" + kineReweight_meName)
            ),
            kineVarExtractor = kineVarExtractor_config
        ),
        kineEventReweight_TTplusJetsEnriched = cms.PSet(
            pluginType = cms.string("KineEventReweightExtractor"),
            weightLookupTable = cms.PSet(
                fileName = kineReweight_fileName,
                meName = cms.string(kineReweight_dqmDirectory + "/" + "TTplusJets" + "/" + kineReweight_meName)
            ),
            kineVarExtractor = kineVarExtractor_config
        ),
        kineEventReweight_QCDenriched = cms.PSet(
            pluginType = cms.string("KineEventReweightExtractor"),
            weightLookupTable = cms.PSet(
                fileName = kineReweight_fileName,
                meName = cms.string(kineReweight_dqmDirectory + "/" + "QCD" + "/" + kineReweight_meName)
            ),
            kineVarExtractor = kineVarExtractor_config
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
                     * process.selectZtoMuTauEvents
                     * process.genPhaseSpaceFilter
                     * process.produceBoolEventSelFlags
#                    * process.eventDump 
                     * process.selectEventsByBoolEventSelFlags
                     * process.ntupleProducer )

# print-out all python configuration parameter information
print process.dumpPython()
