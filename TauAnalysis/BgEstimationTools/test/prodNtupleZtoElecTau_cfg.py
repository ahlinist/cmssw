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
# import sequences for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")
process.load("TauAnalysis.Configuration.producePatTupleZtoElecTauSpecific_cff")
process.load("TauAnalysis.BgEstimationTools.producePatTupleZtoElecTauSpecificForBgEst_cff")

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
        #'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/Ztautau/skimElecTau_Ztautau_1.root'
        'file:/afs/cern.ch/user/v/veelken/scratch0/CMSSW_2_2_10/src/TauAnalysis/BgEstimationTools/test/bgEstSample.root'
    ),
    skipEvents = cms.untracked.uint32(0)            
)

# produce ntuple
kineReweight_fileName = cms.string('rfio:/castor/cern.ch/user/v/veelken/bgEstKineReweights/bgEstKineEventReweightsZtoElecTau.root')
kineReweight_dqmDirectory = "DQMData/bgEstKineEventReweights"
kineReweight_meName = "diTauMvis"
kineVarExtractor_config = cms.PSet(
    pluginType = cms.string("PATElecTauPairValExtractor"),
    src = cms.InputTag('elecTauPairsBgEstPreselection'),
    value = cms.string("p4Vis.mass"),
    indices = cms.vuint32(0)
)

# produce ntuple
process.ntupleProducer = cms.EDAnalyzer("ObjValNtupleProducer",
    treeName = cms.string("bgEstEvents"),
    branches = cms.PSet(
        # variables specific to selection of Z --> e+ e- background enriched sample
        electronPtZee = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstZeeEnriched'),
            value = cms.string("leg1.pt"),
            indices = cms.vuint32(0,1)
        ),
        electronTrackIsoZee = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstZeeEnriched'),
            value = cms.string("leg1.trackIso"),
            indices = cms.vuint32(0,1)
        ),
        electronEcalIsoZee = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstZeeEnriched'),
            value = cms.string("leg1.ecalIso"),
            indices = cms.vuint32(0,1)
        ),
        tauDiscrAgainstElectronsZee = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstZeeEnriched'),
            value = cms.string("leg2.tauID('againstElectron')"),
            indices = cms.vuint32(0,1)
        ),
        diTauAbsChargeZee = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstZeeEnriched'),
            value = cms.string("abs(charge)"),
            indices = cms.vuint32(0,1)
        ),
        numDiTausZee = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstZeeEnriched')
        ),
        numJetsAlpha0point1Zee = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('jetsAlpha0point1ForElecTauBgEstZeeEnriched')
        ),

        # variables specific to selection of W + jets background enriched sample
        electronPtWplusJets = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstWplusJetsEnriched'),
            value = cms.string("leg1.pt"),
            indices = cms.vuint32(0,1)
        ),
        electronTrackIsoWplusJets = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstWplusJetsEnriched'),
            value = cms.string("leg1.trackIso"),
            indices = cms.vuint32(0,1)
        ),
        electronEcalIsoWplusJets = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstWplusJetsEnriched'),
            value = cms.string("leg1.ecalIso"),
            indices = cms.vuint32(0,1)
        ),
        tauTrackIsoWplusJets = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstWplusJetsEnriched'),
            value = cms.string("leg2.chargedParticleIso"),
            indices = cms.vuint32(0,1)
        ),
        tauTrackIsoDiscrWplusJets = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstWplusJetsEnriched'),
            value = cms.string("leg2.tauID('trackIsolation')"),
            indices = cms.vuint32(0,1)
        ),
        tauEcalIsoWplusJets = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstWplusJetsEnriched'),
            value = cms.string("leg2.gammaParticleIso"),
            indices = cms.vuint32(0,1)
        ),
        tauEcalIsoDiscrWplusJets = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstWplusJetsEnriched'),
            value = cms.string("leg2.tauID('ecalIsolation')"),
            indices = cms.vuint32(0,1)
        ),
        tauNumSignalConeTracksWplusJets = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstWplusJetsEnriched'),
            value = cms.string("leg2.signalTracks.size"),
            indices = cms.vuint32(0,1)
        ),
        tauDiscrAgainstElectronsWplusJets = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstWplusJetsEnriched'),
            value = cms.string("leg2.tauID('againstElectron')"),
            indices = cms.vuint32(0,1)
        ),
        diTauMt1MEtWplusJets = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstWplusJetsEnriched'),
            value = cms.string("mt1MET"),
            indices = cms.vuint32(0,1)
        ),
        diTauPzetaDiffWplusJets = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstWplusJetsEnriched'),
            value = cms.string("pZeta - 1.5*pZetaVis"),
            indices = cms.vuint32(0,1)
        ),
        numDiTausWplusJets = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstWplusJetsEnriched')
        ), 
        numJetsAlpha0point1WplusJets = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('jetsAlpha0point1ForElecTauBgEstWplusJetsEnriched')
        ),
        
        # variables specific to selection of ttbar + jets background enriched sample
        electronTrackIsoTTplusJets = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstTTplusJetsEnriched'),
            value = cms.string("leg1.trackIso"),
            indices = cms.vuint32(0,1)
        ),
        electronEcalIsoTTplusJets = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstTTplusJetsEnriched'),
            value = cms.string("leg1.ecalIso"),
            indices = cms.vuint32(0,1)
        ),        
        tauDiscrAgainstElectronsTTplusJets = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstTTplusJetsEnriched'),
            value = cms.string("leg2.tauID('againstElectron')"),
            indices = cms.vuint32(0,1)
        ),
        diTauAbsChargeTTplusJets = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstTTplusJetsEnriched'),
            value = cms.string("abs(charge)"),
            indices = cms.vuint32(0,1)
        ),
        numDiTausTTplusJets = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstTTplusJetsEnriched')
        ), 
        jetEt40bTaggingDiscrSimpleSecondaryVertexTTplusJets = cms.PSet(
            pluginType = cms.string("PATJetValExtractor"),
            src = cms.InputTag('jetsEt40ForElecTauBgEstTTplusJetsEnriched'),
            value = cms.string("bDiscriminator('simpleSecondaryVertex')"),
            indices = cms.vuint32(0,1,2)
        ),
        jetEt40bTaggingDiscrCombinedSecondaryVertexTTplusJets = cms.PSet(
            pluginType = cms.string("PATJetValExtractor"),
            src = cms.InputTag('jetsEt40ForElecTauBgEstTTplusJetsEnriched'),
            value = cms.string("bDiscriminator('combinedSecondaryVertex')"),
            indices = cms.vuint32(0,1,2)
        ),
        jetEt40bTaggingDiscrTrackCountingHighEffTTplusJets = cms.PSet(
            pluginType = cms.string("PATJetValExtractor"),
            src = cms.InputTag('jetsEt40ForElecTauBgEstTTplusJetsEnriched'),
            value = cms.string("bDiscriminator('trackCountingHighEffBJetTags')"),
            indices = cms.vuint32(0,1,2)
        ),
        jetEt40bTaggingDiscrSoftElectronTTplusJets = cms.PSet(
            pluginType = cms.string("PATJetValExtractor"),
            src = cms.InputTag('jetsEt40ForElecTauBgEstTTplusJetsEnriched'),
            value = cms.string("bDiscriminator('softElectronBJetTags')"),
            indices = cms.vuint32(0,1,2)
        ),
        jetEt40bTaggingDiscrSoftMuonTTplusJets = cms.PSet(
            pluginType = cms.string("PATJetValExtractor"),
            src = cms.InputTag('jetsEt40ForElecTauBgEstTTplusJetsEnriched'),
            value = cms.string("bDiscriminator('softMuonBJetTags')"),
            indices = cms.vuint32(0,1,2)
        ),
        numJetsEt40TTplusJets = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('jetsEt40ForElecTauBgEstTTplusJetsEnriched')
        ),
        numJetsEt60TTplusJets = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('jetsEt60ForElecTauBgEstTTplusJetsEnriched')
        ),

        # variables for selection of QCD background enriched sample
        electronTrackIsoQCD = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstQCDenriched'),
            value = cms.string("leg1.trackIso"),
            indices = cms.vuint32(0,1)
        ),
        electronEcalIsoQCD = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstQCDenriched'),
            value = cms.string("leg1.ecalIso"),
            indices = cms.vuint32(0,1)
        ),
        tauDiscrAgainstElectronsQCD = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstQCDenriched'),
            value = cms.string("leg2.tauID('againstElectron')"),
            indices = cms.vuint32(0,1)
        ),
        numDiTausQCD = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstQCDenriched')
        ), 

        # "global" variables not specific to any particular background
        metPt = cms.PSet(
            pluginType = cms.string("PATMetValExtractor"),
            src = cms.InputTag('layer1METs'),
            value = cms.string("pt"),
            indices = cms.vuint32(0)
        ),
        numLooseIdElectrons = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('electronsLooseIdForBgEst')
        ),
        numRobustIdElectrons = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('electronsRobustIdForBgEst')
        ),
        numTightIdElectrons = cms.PSet(
            pluginType = cms.string("NumCandidateExtractor"),
            src = cms.InputTag('selectedLayer1ElectronsTightIdIndividual')
        ),

        # variables used for template shape extraction
        diTauMvis12Zee = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstZeeEnriched'),
            value = cms.string("p4Vis.mass"),
            indices = cms.vuint32(0,1)
        ),
        diTauMvis12combinedHypothesesZee = cms.PSet(
            pluginType = cms.string("ZtautauVisMassHypothesisElecTauValExtractor"),
            src = cms.InputTag('elecTauPairVisMassHypothesesForBgEstZeeEnriched'),
            srcDiTauCandidates = cms.InputTag('elecTauPairsForBgEstZeeEnriched'),
            indices = cms.vuint32(0,1)
        ),
        diTauMvis12WplusJets = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstWplusJetsEnriched'),
            value = cms.string("p4Vis.mass"),
            indices = cms.vuint32(0,1)
        ),
        diTauMvis12TTplusJets = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstTTplusJetsEnriched'),
            value = cms.string("p4Vis.mass"),
            indices = cms.vuint32(0,1)
        ),
        diTauMvis12QCD = cms.PSet(
            pluginType = cms.string("PATElecTauPairValExtractor"),
            src = cms.InputTag('elecTauPairsForBgEstQCDenriched'),
            value = cms.string("p4Vis.mass"),
            indices = cms.vuint32(0,1)
        ),
       
        # event weight variables
        # (accounting for different simulated luminosities of background Monte Carlo samples)
        eventWeight = cms.PSet(
            pluginType = cms.string("ConstObjValExtractor"),
            value = cms.double(1.)
        ),

        # additional event weight variables for correcting "bias"
        # of visible invariant electron + tau-jet mass distribution
        # caused by Mt(electron + tau-jet) transverse mass cut
        # and cut on CDF (Pzeta - 1.5*PzetaVis) variable
        kineEventReweightWplusJets = cms.PSet(
            pluginType = cms.string("KineEventReweightExtractor"),
            weightLookupTable = cms.PSet(
                fileName = kineReweight_fileName,
                meName = cms.string(kineReweight_dqmDirectory + "/" + "WplusJets" + "/" + kineReweight_meName)
            ),
            kineVarExtractor = kineVarExtractor_config.clone(
                src = cms.InputTag('elecTauPairsForBgEstWplusJetsEnriched')
            )
        ),
        kineEventReweightTTplusJets = cms.PSet(
            pluginType = cms.string("KineEventReweightExtractor"),
            weightLookupTable = cms.PSet(
                fileName = kineReweight_fileName,
                meName = cms.string(kineReweight_dqmDirectory + "/" + "TTplusJets" + "/" + kineReweight_meName)
            ),
            kineVarExtractor = kineVarExtractor_config.clone(
                src = cms.InputTag('elecTauPairsForBgEstTTplusJetsEnriched')
            )
        ),
        kineEventReweightQCD = cms.PSet(
            pluginType = cms.string("KineEventReweightExtractor"),
            weightLookupTable = cms.PSet(
                fileName = kineReweight_fileName,
                meName = cms.string(kineReweight_dqmDirectory + "/" + "QCD" + "/" + kineReweight_meName)
            ),
            kineVarExtractor = kineVarExtractor_config.clone(
                src = cms.InputTag('elecTauPairsForBgEstQCDenriched')
            )
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
#__process.ntupleProducer.branches.eventWeight.value = cms.double(#corrFactor#*intLumiZtoElecTau_Data/#intLumi#)
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

process.p = cms.Path(
    process.producePatTuple
   + process.producePatTupleZtoElecTauSpecific
   + process.producePatTupleZtoElecTauSpecificForBgEst
#   + process.printEventContent   # uncomment to enable dump of event content after PAT-tuple production
   + process.selectZtoElecTauEvents
   + process.genPhaseSpaceFilter
   + process.produceBoolEventSelFlags
   + process.selectEventsByBoolEventSelFlags
   + process.ntupleProducer
)

# print-out all python configuration parameter information
#print process.dumpPython()
