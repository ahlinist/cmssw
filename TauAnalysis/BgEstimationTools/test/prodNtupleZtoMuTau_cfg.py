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
process.GlobalTag.globaltag = cms.string('MC_36Y_V7A::All')

#--------------------------------------------------------------------------------
# import sequences for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")
process.load("TauAnalysis.Configuration.producePatTupleZtoMuTauSpecific_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectZtoMuTau_cff")
process.load("TauAnalysis.BgEstimationTools.bgEstPreselZtoMuTau_cff")

# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_cfi import *
#from TauAnalysis.BgEstimationTools.bgEstSampleDefinitionsZtoMuTau_7TeV_cfi import *
#--------------------------------------------------------------------------------

# print event content 
process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'/store/relval/CMSSW_3_1_2/RelValZTT/GEN-SIM-RECO/STARTUP31X_V2-v1/0007/A4DD1FAE-B178-DE11-B608-001D09F24EAC.root',
        #'/store/relval/CMSSW_3_1_2/RelValZTT/GEN-SIM-RECO/STARTUP31X_V2-v1/0007/9408B54D-CB78-DE11-9AEB-001D09F2503C.root'
        'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_1.root',
        'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_2.root'
    ),
    skipEvents = cms.untracked.uint32(0)            
)

#--------------------------------------------------------------------------------
# produce muon + tau-jet pairs
from TauAnalysis.CandidateTools.muTauPairProduction_cff import *

process.muTauPairsZtoMuTauNtuple = allMuTauPairs.clone(
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedPatMuonsTrkIPcumulative'),
    srcLeg2 = cms.InputTag('selectedPatTausForMuTauLeadTrkPtCumulative'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('patMETs'),
    recoMode = cms.string(""),
    doSVreco = cms.bool(False)
)

process.muTauPairsZtoMuTauNtupleBackToBack = cms.EDFilter("PATMuTauPairSelector",
    src = cms.InputTag('muTauPairsZtoMuTauNtuple'),                                                        
    cut = cms.string('dPhi12 > 2.793'),
    filter = cms.bool(False)
)

process.muTauPairsZtoMuTauNtupleZeroCharge = cms.EDFilter("PATMuTauPairSelector",
    src = cms.InputTag('muTauPairsZtoMuTauNtupleBackToBack'),                                                     
    cut = cms.string('leg2.leadPFChargedHadrCand.isNonnull & (leg1.charge + leg2.leadPFChargedHadrCand.charge) = 0'),
    filter = cms.bool(False)                                                               
)

process.muTauPairsZtoMuTauNtupleMt1MET = cms.EDFilter("PATMuTauPairSelector",
    src = cms.InputTag('muTauPairsZtoMuTauNtupleZeroCharge'),                                  
    cut = cms.string('mt1MET < 50.'),
    filter = cms.bool(False)
)

process.muTauPairsZtoMuTauNtuplePzetaDiff = cms.EDFilter("PATMuTauPairSelector",
    src = cms.InputTag('muTauPairsZtoMuTauNtupleMt1MET'),                                                   
    cut = cms.string('(pZeta - 1.5*pZetaVis) > -20.'),
    filter = cms.bool(False)
)

process.produceMuTauPairsZtoMuTauNtuple = cms.Sequence(
    process.muTauPairsZtoMuTauNtuple
   * process.muTauPairsZtoMuTauNtupleBackToBack
   * process.muTauPairsZtoMuTauNtupleZeroCharge
   * process.muTauPairsZtoMuTauNtupleMt1MET
   * process.muTauPairsZtoMuTauNtuplePzetaDiff
)
#--------------------------------------------------------------------------------

extractor_template = cms.PSet(
    pluginType = cms.string("PATMuTauPairValExtractor"),
    src = cms.InputTag('muTauPairsZtoMuTauNtuplePzetaDiff'),
    value = cms.string(""),
    indices = cms.vuint32(0,1)
)

process.ntupleProducer = cms.EDAnalyzer("ObjValNtupleProducer",
    treeName = cms.string("ZtoMuTauNtuple"),
    branches = cms.PSet(
        # variables needed to determine signal and background contributions
        # via maximum-likelihood fit
        muonPt = extractor_template.clone(
            value = cms.string("leg1.pt")
        ),
        muonAbsEta = extractor_template.clone(
            value = cms.string("abs(leg1.eta)")
        ),
        diTauMt1MEt = extractor_template.clone(
            value = cms.string("mt1MET")
        ),
        diTauVisMass = extractor_template.clone(
            value = cms.string("p4Vis.mass")
        ),
        diTauInvMass = extractor_template.clone(
            value = cms.string("nSVfitSolution('psKine_MEt_logM_fit').resonance(0).mass")
        ),

        # tau. id. variables
        tauCharge = extractor_template.clone(
            value = cms.string("leg2.charge")
        ),
        tauLeadChargedParticlePt = extractor_template.clone(
            value = cms.string("? leg2.leadPFChargedHadrCand.isNonnull ? leg2.leadPFChargedHadrCand.pt : 0.")
        ),
        tauLeadNeutralParticlePt = extractor_template.clone(
            value = cms.string("? leg2.leadPFNeutralCand.isNonnull ? leg2.leadPFNeutralCand.pt : 0.")
        ),
        tauLeadParticlePt = extractor_template.clone(
            value = cms.string("? leg2.leadPFCand.isNonnull ? leg2.leadPFCand.pt : 0.")
        ),
        tauNumChargedParticlesSignalCone = extractor_template.clone(
            value = cms.string("leg2.signalPFChargedHadrCands.size")
        ),
        tauNumNeutralHadronsSignalCone = extractor_template.clone(
            value = cms.string("leg2.signalPFNeutrHadrCands.size")
        ),
        tauNumPhotonsSignalCone = extractor_template.clone(
            value = cms.string("leg2.signalPFGammaCands.size")
        ),
        tauNumParticlesSignalCone = extractor_template.clone(
            value = cms.string("leg2.signalPFCands.size")
        ),
        tauNumChargedParticlesIsoCone = extractor_template.clone(
            value = cms.string("leg2.isolationPFChargedHadrCands.size")
        ),
        tauNumNeutralHadronsIsoCone = extractor_template.clone(
            value = cms.string("leg2.isolationPFNeutrHadrCands.size")
        ),
        tauNumPhotonsIsoCone = extractor_template.clone(
            value = cms.string("leg2.isolationPFGammaCands.size")
        ),
        tauNumParticlesIsoCone = extractor_template.clone(
            value = cms.string("leg2.isolationPFCands.size")
        ),
        tauPtSumChargedParticlesIsoCone = extractor_template.clone(
            value = cms.string("leg2.isolationPFChargedHadrCandsPtSum")
        ),
        tauPtSumPhotonsIsoCone = extractor_template.clone(
            value = cms.string("leg2.isolationPFGammaCandsEtSum")
        ),
       
        # event weight variables
        # (accounting for different simulated luminosities of background Monte Carlo samples)
        eventWeight = cms.PSet(
            pluginType = cms.string("ConstObjValExtractor"),
            value = cms.double(1.)
        )
    )
)

process.TFileService = cms.Service("TFileService", 
    fileName = cms.string('ntupleZtoMuTau.root')
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
#__process.source.fileNames = #inputFileNames#
#__process.source.skipEvents = cms.untracked.uint32(#skipEvents#)
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.genPhaseSpaceFilter.selector = copy.deepcopy(#genPhaseSpaceCut#)
#__process.TFileService.fileName = #ntupleOutputFileName#
#__process.ntupleProducer.branches.eventWeight.value = cms.double(#corrFactor#*#intLumiData#/#intLumi#)
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

#--------------------------------------------------------------------------------
# import utility function for managing pat::Jets
from PhysicsTools.PatAlgos.tools.jetTools import *

# uncomment to replace caloJets by pfJets
switchJetCollection(process, jetCollection = cms.InputTag("ak5PFJets"))
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for managing pat::METs
from TauAnalysis.Configuration.tools.metTools import *

# uncomment to add pfMET
# set Boolean swich to true in order to apply type-1 corrections
addPFMet(process, correct = False)

# uncomment to replace caloMET by pfMET in all di-tau objects
process.load("TauAnalysis.CandidateTools.diTauPairProductionAllKinds_cff")
replaceMETforDiTaus(process, cms.InputTag('patMETs'), cms.InputTag('patPFMETs'))
#--------------------------------------------------------------------------------

process.p = cms.Path(
    process.producePatTuple
   + process.producePatTupleZtoMuTauSpecific
   + process.selectZtoMuTauEvents
   + process.genPhaseSpaceFilter
   + process.produceMuTauPairsZtoMuTauNtuple
   + process.produceBoolEventSelFlags
   + process.selectEventsByBoolEventSelFlags
   + process.ntupleProducer
)

#--------------------------------------------------------------------------------
# disable estimation of systematic uncertainties
from TauAnalysis.Configuration.tools.sysUncertaintyTools import disableSysUncertainties_runZtoMuTau
disableSysUncertainties_runZtoMuTau(process)
#--------------------------------------------------------------------------------

# print-out all python configuration parameter information
#del process.patJetMETCorrections
#print process.dumpPython()
