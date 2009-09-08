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
#process.load("TauAnalysis.Configuration.selectZtoMuTau_cff")

# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
from TauAnalysis.Configuration.recoSampleDefinitionsbbAHtoElecTau_cfi import *
#from TauAnalysis.BgEstimationTools.bgEstSampleDefinitionsbbAHtoElecTau_cfi import *

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
        #'file:/localscratch/ah115bb_tautau.root'
        'file:/tmp/elecTauSkim_ah115_1.root'
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

process.trigger = cms.EDFilter("EventSelPluginFilter",
  selector = cms.PSet(
    pluginName = cms.string('trigger'),
    pluginType = cms.string('TriggerResultEventSelector'),
    src = cms.InputTag('TriggerResults::HLT'),
    triggerPaths = cms.vstring('HLT_Ele10_SW_L1R')
  )
)

process.vertexPrimary = cms.EDFilter("PATSingleVertexSelector",
  mode = cms.string('firstVertex'),
  vertices = cms.InputTag('offlinePrimaryVerticesWithBS'),
  filter = cms.bool(True)
)

process.vertexValid = cms.EDFilter("VertexSelector",
  src = cms.InputTag('vertexPrimary'),
  cut = cms.string('isValid & (chi2prob(chi2,ndof) > 0.01) & (tracksSize >= 2)'),
  filter = cms.bool(True)
)

process.vertexZ = cms.EDFilter("VertexSelector",
  src = cms.InputTag('vertexValid'),
  cut = cms.string('abs(z) < 25'),
  filter = cms.bool(True)
)

process.select_vertex = cms.Sequence(process.vertexPrimary+process.vertexValid+process.vertexZ)

process.elecPt = cms.EDFilter("PATElectronSelector",
  src = cms.InputTag('cleanLayer1Electrons'),
  cut = cms.string('pt > 20'),
  filter = cms.bool(True)
)

process.elecEta = cms.EDFilter("PATElectronSelector",
  src = cms.InputTag('elecPt'),
  cut = cms.string('abs(eta)<2.1'),
  filter = cms.bool(True)
)

process.elecCrack = cms.EDFilter("PATElectronSelector",
  src = cms.InputTag('elecEta'),
  cut = cms.string('abs(superCluster.eta) < 1.442 | abs(superCluster.eta) > 1.560'),
  filter = cms.bool(True)
)

process.elecTrack = cms.EDFilter("PATElectronSelector",
  src = cms.InputTag('elecCrack'),
  cut = cms.string('gsfTrack.isNonnull'),
  filter = cms.bool(True)
)

process.elecIP = cms.EDFilter("PATElectronIpSelector",
  src = cms.InputTag('elecTrack'),
  vertexSource = cms.InputTag('vertexZ'),
  IpMax = cms.double(0.05),
  filter = cms.bool(True)
)

process.select_electron = cms.Sequence(process.elecPt+process.elecEta+process.elecCrack+process.elecTrack+process.elecIP)

process.tauOverlap = cms.EDFilter("PATTauAntiOverlapSelector",
  src = cms.InputTag('cleanLayer1Taus'),
  srcNotToBeFiltered = cms.VInputTag('elecIP'),
  dRmin = cms.double(0.3),
  filter = cms.bool(True)
)

process.tauPt = cms.EDFilter("PATTauSelector",
  src = cms.InputTag('tauOverlap'),
  cut = cms.string('pt > 20'),
  filter = cms.bool(True)
)

process.tauEta = cms.EDFilter("PATTauSelector",
  src = cms.InputTag('tauPt'),
  cut = cms.string('abs(eta) < 2.1'),
  filter = cms.bool(True)
)

process.tauTracks = cms.EDFilter("PATTauSelector",
  src = cms.InputTag('tauEta'),
  cut = cms.string('signalTracks.size() = 1 | signalTracks.size() = 3'),
  filter = cms.bool(True)
)

process.select_tau = cms.Sequence(process.tauOverlap+process.tauPt+process.tauEta+process.tauTracks)

process.jetOverlap = cms.EDFilter("PATJetAntiOverlapSelector",
  src = cms.InputTag('cleanLayer1Jets'),
  srcNotToBeFiltered = cms.VInputTag('elecIP','tauTracks'),
  dRmin = cms.double(0.7),
  filter = cms.bool(True)
)

process.jetEt = cms.EDFilter("PATJetSelector",
  src = cms.InputTag('jetOverlap'),
  cut = cms.string('et > 15'),
  filter = cms.bool(True)
)

process.jetEta = cms.EDFilter("PATJetSelector",
  src = cms.InputTag('jetEt'),
  cut = cms.string('abs(eta) < 2.1'),
  filter = cms.bool(True)
)

process.select_jet = cms.Sequence(process.jetOverlap+process.jetEt+process.jetEta)

process.elecTaus = cms.EDProducer("PATElecTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('elecIP'),
    srcLeg2 = cms.InputTag('tauTracks'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

process.elecTausCharge = cms.EDFilter("PATElecTauPairSelector",
  src = cms.InputTag('elecTaus'),
  cut = cms.string('charge = 0'),
  filter = cms.bool(True)
)

process.select_electau = cms.Sequence(process.elecTaus+process.elecTausCharge)

process.select_all = cms.Sequence(process.select_vertex+process.select_electron+process.select_tau+process.select_jet+process.select_electau)
#--------------------------------------------------------------------------------

# produce ntuple
process.ntupleProducer = cms.EDAnalyzer("ObjValNtupleProducer",
    treeName = cms.string("bbAHCutTree"),
    branches = cms.PSet(
        pairEta = cms.PSet(
          pluginType = cms.string("PATElecTauPairVectorValExtractor"),
          src = cms.InputTag("elecTausCharge"),
          value = cms.string("eta"),
          vector = cms.bool(True)
        ),
        pairPhi = cms.PSet(
          pluginType = cms.string("PATElecTauPairVectorValExtractor"),
          src = cms.InputTag("elecTausCharge"),
          value = cms.string("phi"),
          vector = cms.bool(True)
        ),
        pairEt = cms.PSet(
          pluginType = cms.string("PATElecTauPairVectorValExtractor"),
          src = cms.InputTag("elecTausCharge"),
          value = cms.string("et"),
          vector = cms.bool(True)
        ),
        pairDR12 = cms.PSet(
          pluginType = cms.string("PATElecTauPairVectorValExtractor"),
          src = cms.InputTag("elecTausCharge"),
          value = cms.string("dR12"),
          vector = cms.bool(True)
        ),
        pairDPhi12 = cms.PSet(
          pluginType = cms.string("PATElecTauPairVectorValExtractor"),
          src = cms.InputTag("elecTausCharge"),
          value = cms.string("dPhi12"),
          vector = cms.bool(True)
        ),
        pairMt1MET = cms.PSet(
          pluginType = cms.string("PATElecTauPairVectorValExtractor"),
          src = cms.InputTag("elecTausCharge"),
          value = cms.string("mt1MET"),
          vector = cms.bool(True)
        ),
        #tauLeadPt = cms.PSet(
        #  pluginType = cms.string("PATTauVectorValExtractor"),
        #  src = cms.InputTag("tauTracks"),
        #  value = cms.string("leadTrack.outerPt"),
        #  vector = cms.bool(True)
        #),
        tauEta = cms.PSet(
          pluginType = cms.string("PATTauVectorValExtractor"),
          src = cms.InputTag("tauTracks"),
          value = cms.string('eta'),
          vector = cms.bool(True)
        ),
        tauPhi = cms.PSet(
          pluginType = cms.string("PATTauVectorValExtractor"),
          src = cms.InputTag("tauTracks"),
          value = cms.string('phi'),
          vector = cms.bool(True)
        ),
        tauEt = cms.PSet(
          pluginType = cms.string("PATTauVectorValExtractor"),
          src = cms.InputTag("tauTracks"),
          value = cms.string('et'),
          vector = cms.bool(True)
        ),
        tauTrackIso = cms.PSet(
          pluginType = cms.string("PATTauVectorValExtractor"),
          src = cms.InputTag("tauTracks"),
          value = cms.string('tauID("trackIsolation")'),
          vector = cms.bool(True)
        ),
        tauECALIso = cms.PSet(
          pluginType = cms.string("PATTauVectorValExtractor"),
          src = cms.InputTag("tauTracks"),
          value = cms.string('tauID("ecalIsolation")'),
          vector = cms.bool(True)
        ),
        tauElectron = cms.PSet(
          pluginType = cms.string("PATTauVectorValExtractor"),
          src = cms.InputTag("tauTracks"),
          value = cms.string('tauID("againstElectron")'),
          vector = cms.bool(True)
        ),
        jetEta = cms.PSet(
          pluginType = cms.string("PATJetVectorValExtractor"),
          src = cms.InputTag("jetEta"),
          value = cms.string('eta'),
          vector = cms.bool(True)
        ),
        jetPhi = cms.PSet(
          pluginType = cms.string("PATJetVectorValExtractor"),
          src = cms.InputTag("jetEta"),
          value = cms.string('phi'),
          vector = cms.bool(True)
        ),
        jetEt = cms.PSet(
          pluginType = cms.string("PATJetVectorValExtractor"),
          src = cms.InputTag("jetEta"),
          value = cms.string('et'),
          vector = cms.bool(True)
        ),
        jetBTagTrackCount = cms.PSet(
          pluginType = cms.string("PATJetVectorValExtractor"),
          src = cms.InputTag("jetEta"),
          value = cms.string('bDiscriminator("trackCountingHighEffBJetTags")'),
          vector = cms.bool(True)
        ),
        jetBTagSimpleSecondary = cms.PSet(
          pluginType = cms.string("PATJetVectorValExtractor"),
          src = cms.InputTag("jetEta"),
          value = cms.string('bDiscriminator("simpleSecondaryVertexBJetTags")'),
          vector = cms.bool(True)
        ),
        jetBTagCombinedSecondary = cms.PSet(
          pluginType = cms.string("PATJetVectorValExtractor"),
          src = cms.InputTag("jetEta"),
          value = cms.string('bDiscriminator("combinedSecondaryVertexBJetTags")'),
          vector = cms.bool(True)
        ),
        electronEta = cms.PSet(
          pluginType = cms.string("PATElectronVectorValExtractor"),
          src = cms.InputTag("elecIP"),
          value = cms.string('eta'),
          vector = cms.bool(True)
        ),
        electronPhi = cms.PSet(
          pluginType = cms.string("PATElectronVectorValExtractor"),
          src = cms.InputTag("elecIP"),
          value = cms.string('phi'),
          vector = cms.bool(True)
        ),
        electronEt = cms.PSet(
          pluginType = cms.string("PATElectronVectorValExtractor"),
          src = cms.InputTag("elecIP"),
          value = cms.string('et'),
          vector = cms.bool(True)
        ),
        electronRobust = cms.PSet(
          pluginType = cms.string("PATElectronVectorValExtractor"),
          src = cms.InputTag("elecIP"),
          value = cms.string('electronID("robust")'),
          vector = cms.bool(True)
        ),
        electronLoose = cms.PSet(
          pluginType = cms.string("PATElectronVectorValExtractor"),
          src = cms.InputTag("elecIP"),
          value = cms.string('electronID("loose")'),
          vector = cms.bool(True)
        ),
        electronTight = cms.PSet(
          pluginType = cms.string("PATElectronVectorValExtractor"),
          src = cms.InputTag("elecIP"),
          value = cms.string('electronID("tight")'),
          vector = cms.bool(True)
        ),
        electronTrackIso = cms.PSet(
          pluginType = cms.string("PATElectronVectorValExtractor"),
          src = cms.InputTag("elecIP"),
          value = cms.string('trackIso'),
          vector = cms.bool(True)
        ),
        electronSuperEta = cms.PSet(
          pluginType = cms.string("PATElectronVectorValExtractor"),
          src = cms.InputTag("elecIP"),
          value = cms.string('abs(superCluster.eta)'),
          vector = cms.bool(True)
        ),
        electronEcalIso = cms.PSet(
          pluginType = cms.string("PATElectronVectorValExtractor"),
          src = cms.InputTag("elecIP"),
          value = cms.string('ecalIso'),
          vector = cms.bool(True)
        ),
        electronESuperOverP = cms.PSet(
          pluginType = cms.string("PATElectronVectorValExtractor"),
          src = cms.InputTag("elecIP"),
          value = cms.string('eSuperClusterOverP'),
          vector = cms.bool(True)
        ),
        electronCombinedIso = cms.PSet(
          pluginType = cms.string("PATElectronVectorValExtractor"),
          src = cms.InputTag("elecIP"),
          value = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt'),
          vector = cms.bool(True)
        ),
        electronFBrem = cms.PSet(
          pluginType = cms.string("PATElectronVectorValExtractor"),
          src = cms.InputTag("elecIP"),
          value = cms.string('(trackMomentumAtVtx.R-trackMomentumOut.R)/trackMomentumAtVtx.R'),
          vector = cms.bool(True)
        ),
        electronHOverE = cms.PSet(
          pluginType = cms.string("PATElectronVectorValExtractor"),
          src = cms.InputTag("elecIP"),
          value = cms.string('hadronicOverEm'),
          vector = cms.bool(True)
        ),
        electronSigmaIEtaIEta = cms.PSet(
          pluginType = cms.string("PATElectronVectorValExtractor"),
          src = cms.InputTag("elecIP"),
          value = cms.string('scSigmaIEtaIEta'),
          vector = cms.bool(True)
        ),
        numElectrons = cms.PSet(
          pluginType = cms.string("NumCandidateExtractor"),
          src = cms.InputTag("elecIP")
        ),
        numTaus = cms.PSet(
          pluginType = cms.string("NumCandidateExtractor"),
          src = cms.InputTag("tauTracks")
        ),
        numJets = cms.PSet(
          pluginType = cms.string("NumCandidateExtractor"),
          src = cms.InputTag("jetEta")
        ),
        numPairs = cms.PSet(
          pluginType = cms.string("NumCandidateExtractor"),
          src = cms.InputTag("elecTausCharge")
        ),
        eventWeight = cms.PSet(
          pluginType = cms.string("ConstObjValExtractor"),
          value = cms.double(1.)
        )
    )
)

process.TFileService = cms.Service("TFileService", 
    fileName = cms.string('/localscratch/bbahtuple.root')
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
                     * process.genPhaseSpaceFilter
                     * process.trigger
                     * process.select_all
                     * process.ntupleProducer )

# print-out all python configuration parameter information
#print process.dumpPython()
