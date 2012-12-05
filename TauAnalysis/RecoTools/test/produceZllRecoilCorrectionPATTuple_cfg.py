import FWCore.ParameterSet.Config as cms

process = cms.Process("produceZllRecoilCorrectionPATTuple")

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.load('Configuration/Geometry/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')

#--------------------------------------------------------------------------------
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/user/veelken/CMSSW_5_3_x/skims/simZplusJets_madgraph_AOD_1_1_txi.root'
        #'file:/afs/cern.ch/user/m/mmarionn/public/forChristian/pickevents_1.root',
        #'file:/afs/cern.ch/user/m/mmarionn/public/forChristian/pickevents_2.root',
        #'file:/afs/cern.ch/user/m/mmarionn/public/forChristian/pickevents_3.root',
        #'file:/afs/cern.ch/user/m/mmarionn/public/forChristian/pickevents_4.root'                                
    ),
    ##eventsToProcess = cms.untracked.VEventRange('1:41958:16769938'),
    skipEvents = cms.untracked.uint32(0)            
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

#--------------------------------------------------------------------------------
# define configuration parameter default values

isMC = True # use for MC
##isMC = False # use for Data
##HLTprocessName = "HLT" # use for 2012 Data
HLTprocessName = "HLT" # use for Spring'12 MC
#type1JetPtThreshold = 20.0 # increased jet Pt threshold to reduce sensitivity of Type 1 corrected MET to pile-up
type1JetPtThreshold = 10.0 # current default value recommended by JetMET POG
#jetCorrUncertaintyTag = "Total"
jetCorrUncertaintyTag = "SubTotalDataMC"
runPeriod = "2012RunABC" # use for MET sys. shift correction vs. Nvtx
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system/grid
#
#__isMC = #isMC#
#__HLTprocessName = #HLTprocessName#
#__runPeriod = #runPeriod#
#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define GlobalTag to be used for event reconstruction
if isMC:
    process.GlobalTag.globaltag = cms.string('START53_V11::All')
else:
    process.GlobalTag.globaltag = cms.string('GR_R_53_V13::All')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
# configure Jet Energy Corrections
#
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.jec = cms.ESSource("PoolDBESSource",
   DBParameters = cms.PSet(
       messageLevel = cms.untracked.int32(0)
   ),
   timetype = cms.string('runnumber'),
   toGet = cms.VPSet(
       cms.PSet(
           record = cms.string('JetCorrectionsRecord'),
           tag    = cms.string(''),
           label  = cms.untracked.string('AK5PF')
       ),
       cms.PSet(
           record = cms.string('JetCorrectionsRecord'),
           tag    = cms.string(''),
           label  = cms.untracked.string('AK5Calo')
       )
   ),
   connect = cms.string('')
)
process.es_prefer_jec = cms.ESPrefer('PoolDBESSource', 'jec')
if isMC:
   process.jec.connect = cms.string('sqlite_fip:TauAnalysis/Configuration/data/Fall12_V5Final_MC.db')
   process.jec.toGet[0].tag = cms.string('JetCorrectorParametersCollection_Fall12_V5_MC_AK5PF')
   process.jec.toGet[1].tag = cms.string('JetCorrectorParametersCollection_Fall12_V5_MC_AK5Calo')
else:
   process.jec.connect = cms.string('sqlite_fip:TauAnalysis/Configuration/data/Fall12_V5Final_DATA.db')
   process.jec.toGet[0].tag = cms.string('JetCorrectorParametersCollection_Fall12_V5_DATA_AK5PF')
   process.jec.toGet[1].tag = cms.string('JetCorrectorParametersCollection_Fall12_V5_DATA_AK5Calo')
#-------------------------------------------------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# print debug information whenever plugins get loaded dynamically from libraries
# (for debugging problems with plugin related dynamic library loading)
#process.add_(cms.Service("PrintLoadingPlugins"))
#--------------------------------------------------------------------------------

process.prePatProductionSequence = cms.Sequence()

#--------------------------------------------------------------------------------
# load definition of VBTF Z --> mu+ mu- event selection
# (with no isolation cuts applied on one of the two muons)
# load definitions of data-quality filters
process.load("TauAnalysis.TauIdEfficiency.filterDataQuality_cfi")
if isMC:
    process.dataQualityFilters.remove(process.hltPhysicsDeclared)
    process.dataQualityFilters.remove(process.dcsstatus)
    ##process.dataQualityFilters.remove(process.HBHENoiseFilter)
    process.dataQualityFilters.remove(process.hcalLaserEventFilter)
    process.dataQualityFilters.remove(process.hcallasereventfilter2012)
    process.dataQualityFilters.remove(process.ecalLaserCorrFilter)
process.prePatProductionSequence += process.dataQualityFilters

process.load("TauAnalysis.Skimming.goldenZmmSelectionVBTFnoMuonIsolation_cfi")
##process.goodMuons.filter = cms.bool(False) # CV: only for testing !!!
##process.goldenZmumuFilter.minNumber = cms.uint32(0) # CV: only for testing !!!
process.prePatProductionSequence += process.goldenZmumuSelectionSequence

process.uniqueGoodMuonPairFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("goodMuons"),
    minNumber = cms.uint32(2),
    maxNumber = cms.uint32(2)                          
)
process.prePatProductionSequence += process.uniqueGoodMuonPairFilter # CV: disable only for testing !!!
process.uniqueGoodIsoMuonPairFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("goodIsoMuons"),
    minNumber = cms.uint32(2),
    maxNumber = cms.uint32(2)                          
)
process.prePatProductionSequence += process.uniqueGoodIsoMuonPairFilter # CV: disable only for testing !!!
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# compute neutral particle density for out-of-time pile-up reweighting

process.load("TauAnalysis/RecoTools/recoVertexSelection_cff")

process.load("TauAnalysis/RecoTools/vertexMultiplicityVsRhoPFNeutralReweight_cfi")

process.pfChargedHadronPileUpCands = process.pfChargedHadronNoPileUpCands.clone(
    src = cms.InputTag('pfPileUp')
)

process.kt6PFChargedHadronPileUpJetsForVtxMultReweighting = process.kt6PFChargedHadronNoPileUpJetsForVtxMultReweighting.clone(
    src = cms.InputTag('pfChargedHadronPileUpCands')
)

process.pfChargedHadronCands = process.pfChargedHadronNoPileUpCands.clone(
    src = cms.InputTag('particleFlow')
)

process.kt6PFChargedHadronJetsForVtxMultReweighting = process.kt6PFChargedHadronNoPileUpJetsForVtxMultReweighting.clone(
    src = cms.InputTag('pfChargedHadronCands')
)

process.kt6PFNeutralHadronJetsForVtxMultReweighting = process.kt6PFNeutralJetsForVtxMultReweighting.clone(
    src = cms.InputTag('pfAllNeutralHadrons')
)

process.kt6PFGammaJetsForVtxMultReweighting = process.kt6PFNeutralJetsForVtxMultReweighting.clone(
    src = cms.InputTag('pfAllPhotons')
)


process.prePatProductionSequence += process.selectedPrimaryVertexQuality
process.prePatProductionSequence += process.selectedPrimaryVertexPosition
process.prePatProductionSequence += process.produceVertexMultiplicityVsRhoPFNeutralReweights
process.prePatProductionSequence += process.pfAllNeutralHadrons
process.prePatProductionSequence += process.kt6PFNeutralHadronJetsForVtxMultReweighting
process.prePatProductionSequence += process.pfAllPhotons
process.prePatProductionSequence += process.kt6PFGammaJetsForVtxMultReweighting
process.prePatProductionSequence += process.pfChargedHadronPileUpCands
process.prePatProductionSequence += process.kt6PFChargedHadronPileUpJetsForVtxMultReweighting
process.prePatProductionSequence += process.pfChargedHadronCands
process.prePatProductionSequence += process.kt6PFChargedHadronJetsForVtxMultReweighting
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# configure Jet Energy Corrections

process.load("TauAnalysis.Configuration.jetCorrectionParameters_cfi")
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# select reco::Vertex collections
process.load("TauAnalysis/RecoTools/recoVertexSelection_cff")

process.prePatProductionSequence += process.selectPrimaryVertex

process.load("TauAnalysis/RecoTools/vertexMultiplicityReweight_cfi")
process.vertexMultiplicityReweight3d2012RunABCruns190456to202016 = process.vertexMultiplicityReweight.clone(
    inputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/expPUpoissonMean_runs190456to202016_Mu17_Mu8.root"),
    type = cms.string("gen3d"),
    mcPeriod = cms.string("Summer12_S10")
)
process.vertexMultiplicityReweight1d2012RunABCruns190456to202016 = process.vertexMultiplicityReweight.clone(
    inputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/expPUpoissonDist_runs190456to202016_Mu17_Mu8.root"),
    type = cms.string("gen"),
    mcPeriod = cms.string("Summer12_S10")
)
process.vertexMultiplicityReweight3d2012RunABCruns190456to203002 = process.vertexMultiplicityReweight3d2012RunABCruns190456to202016.clone(
    inputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/expPUpoissonMean_runs190456to203002_Mu17_Mu8.root")
)
process.vertexMultiplicityReweight1d2012RunABCruns190456to203002 = process.vertexMultiplicityReweight1d2012RunABCruns190456to202016.clone(
    inputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/expPUpoissonDist_runs190456to203002_Mu17_Mu8.root")
)

process.selectedPrimaryVerticesTrackPtSumGt10 = cms.EDFilter("VertexByTrackPtSumSelector",
    src = cms.InputTag('selectedPrimaryVertexPosition'),
    trackPtSumMin = cms.double(10.)
)
process.selectedPrimaryVerticesTrackPtSumGt5 = process.selectedPrimaryVerticesTrackPtSumGt10.clone(
    trackPtSumMin = cms.double(5.)
)
process.prePatProductionSequence += process.selectedPrimaryVerticesTrackPtSumGt5
process.prePatProductionSequence += process.selectedPrimaryVerticesTrackPtSumGt10

# produce pile-up reweighting factors
if isMC:
    process.prePatProductionSequence += process.vertexMultiplicityReweight3d2012RunABCruns190456to202016
    process.prePatProductionSequence += process.vertexMultiplicityReweight1d2012RunABCruns190456to202016
    process.prePatProductionSequence += process.vertexMultiplicityReweight3d2012RunABCruns190456to203002
    process.prePatProductionSequence += process.vertexMultiplicityReweight1d2012RunABCruns190456to203002
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# produce PFMET significance cov. matrix

# CV: compute PFMET significance cov. matrix for uncorrected jets
#     in order to include pile-up jets
#    (which to a significant fraction get killed by L1Fastjet corrections)
process.ak5PFJetsNotOverlappingWithLeptons = cms.EDFilter("PFJetAntiOverlapSelector",
    src = cms.InputTag('ak5PFJets'),
    srcNotToBeFiltered = cms.VInputTag('goodMuons'),
    dRmin = cms.double(0.5),
    invert = cms.bool(False),
    filter = cms.bool(False)                                                          
)
process.prePatProductionSequence += process.ak5PFJetsNotOverlappingWithLeptons

from JetMETCorrections.Type1MET.pfMETCorrections_cff import pfCandsNotInJet
process.pfCandsNotInJetForPFMEtSignCovMatrix = pfCandsNotInJet.clone()
process.prePatProductionSequence += process.pfCandsNotInJetForPFMEtSignCovMatrix

from RecoMET.METProducers.METSigParams_cfi import *
process.pfMEtSignCovMatrix = cms.EDProducer("PFMEtSignCovMatrixProducer",
    METSignificance_params,                     
    src = cms.VInputTag(
        'goodMuons',
        'ak5PFJetsNotOverlappingWithLeptons',                                        
        'pfCandsNotInJetForPFMEtSignCovMatrix'
    ),
    addJERcorr = cms.PSet(
        inputFileName = cms.FileInPath('PhysicsTools/PatUtils/data/pfJetResolutionMCtoDataCorrLUT.root'),
        lutName = cms.string('pfJetResolutionMCtoDataCorrLUT')
    )
)
process.prePatProductionSequence += process.pfMEtSignCovMatrix
#--------------------------------------------------------------------------------

process.patTupleProductionSequence = cms.Sequence()

#--------------------------------------------------------------------------------
# produce PAT objects

process.load("PhysicsTools/PatAlgos/patSequences_cff")

# CV: disable pat::Electron and pat::Photon sequences,
#     in order to prevent run-time exception caused by dropping
#     SuperCluster information from event content at skimming stage
#    (in order to reduce size of skim files stored on EOS)
process.patDefaultSequence.remove(process.makePatElectrons)
process.selectedPatCandidates.remove(process.selectedPatElectrons)
process.cleanPatCandidates.remove(process.cleanPatElectrons)
process.patDefaultSequence.remove(process.makePatPhotons)
process.selectedPatCandidates.remove(process.selectedPatPhotons)
process.cleanPatCandidates.remove(process.cleanPatPhotons)
# CV: disable sequences for selecting and cleaning of PAT objects
#     in order to prevent run-time exception caused by missing electrons/photons
process.patDefaultSequence.remove(process.selectedPatCandidates)
process.patDefaultSequence.remove(process.cleanPatCandidates)
process.patDefaultSequence.remove(process.countPatCandidates)

process.patMuons.muonSource = cms.InputTag('goodMuons')

# disable all pat::Muon embedding
for objSelAttrName in dir(process.patMuons):
    objSelAttr = getattr(process.patMuons, objSelAttrName)
    if isinstance(objSelAttr, cms.bool) and (objSelAttrName.startswith("embed") or objSelAttrName.startswith("add")):
        setattr(process.patMuons, objSelAttrName, cms.bool(False))

if not isMC:
    # remove MC matching from standard PAT sequences
    from PhysicsTools.PatAlgos.tools.coreTools import *
    removeMCMatching(process, ["All"], outputModules = [])

process.patIsoMuons = process.patMuons.clone(
    muonSource = cms.InputTag('goodIsoMuons')
)
process.patDefaultSequence += process.patIsoMuons

# configure pat::Jet production
# (enable L2L3Residual corrections in case running on Data)
jetCorrections = [ 'L1FastJet', 'L2Relative', 'L3Absolute' ]
if not isMC:
    jetCorrections.append('L2L3Residual')

from PhysicsTools.PatAlgos.tools.jetTools import *
switchJetCollection(
    process,
    cms.InputTag('ak5PFJets'),
    doJTA = True,
    doBTagging = True,
    jetCorrLabel = ( 'AK5PF', cms.vstring(jetCorrections) ),
    doType1MET = False,
    doJetID = True,
    jetIdLabel = "ak5",
    outputModules = []
)

process.patTupleProductionSequence += process.patDefaultSequence

# configure pat::MET production
from TauAnalysis.Configuration.tools.metTools import addCorrectedPFMet
doApplyType0corr = True
doApplySysShiftCorr = False
doSmearJets = None
if isMC:
    doSmearJets = True
else:
    doSmearJets = False
addCorrectedPFMet(process, isMC, doApplyType0corr, doApplySysShiftCorr, runPeriod, doSmearJets)
process.patTupleProductionSequence += process.makeCorrectedPatMETs

# set jet Pt threshold used for computing Type 1 MET corrections
for processAttrName in dir(process):
    processAttr = getattr(process, processAttrName)
    if isinstance(processAttr, cms.EDProducer) and \
      (processAttr.type_() == "PATPFJetMETcorrInputProducer" or processAttr.type_() == "PFJetMETcorrInputProducer"):
        print "--> Setting Type 1 MET correction threshold to %1.1f GeV for module: %s" % (type1JetPtThreshold, processAttrName)
        setattr(processAttr, "type1JetPtThreshold", cms.double(type1JetPtThreshold))

# add minMET(Type 1 corrected PFMET, no-PU MET)
central_or_shifts = [ "" ]
if isMC:
    central_or_shifts.extend(
        ["JetEnUp", "JetEnDown",
         "JetResUp", "JetResDown",
         "UnclusteredEnUp", "UnclusteredEnDown" ])
for central_or_shift in central_or_shifts:
    patMinMEtModule = cms.EDProducer("MinPatMETProducer",
        src = cms.VInputTag(
            'patType1CorrectedPFMet%s' % central_or_shift,
            'patPFMetNoPileUp%s' % central_or_shift
        )
    )
    patMinMEtModuleName = "patMinMEt%s" % central_or_shift
    setattr(process, patMinMEtModuleName, patMinMEtModule)
    process.patTupleProductionSequence += patMinMEtModule

# add CaloMET
process.patCaloMet = process.patMETs.clone(
    metSource = cms.InputTag('corMetGlobalMuons'),
    addMuonCorrections = cms.bool(False),
    genMETSource = cms.InputTag('genMetCalo')
)
process.patTupleProductionSequence += process.patCaloMet

process.load("RecoMET/METProducers/MetMuonCorrections_cff")
process.corMetGlobalMuonsNoHF = process.corMetGlobalMuons.clone(
    uncorMETInputTag = cms.InputTag('metNoHF')
)
process.patTupleProductionSequence += process.corMetGlobalMuonsNoHF
process.patCaloMetNoHF = process.patCaloMet.clone(
    metSource = cms.InputTag('corMetGlobalMuonsNoHF')
)
process.patTupleProductionSequence += process.patCaloMetNoHF

process.load("JetMETCorrections/Type1MET/caloMETCorrections_cff")
process.caloJetMETcorr.srcMET = cms.InputTag('met')
if isMC:
    process.caloJetMETcorr.jetCorrLabel = cms.string("ak5CaloL2L3")
else:
    process.caloJetMETcorr.jetCorrLabel = cms.string("ak5CaloL2L3Residual")
process.caloType1CorrectedMet.src = cms.InputTag('corMetGlobalMuons')
process.patTupleProductionSequence += process.caloJetMETcorr
process.patTupleProductionSequence += process.caloType1CorrectedMet
process.patType1CorrectedCaloMet = process.patMETs.clone(
    metSource = cms.InputTag('caloType1CorrectedMet'),
    addMuonCorrections = cms.bool(False),
    genMETSource = cms.InputTag('genMetCalo')
)
process.patTupleProductionSequence += process.patType1CorrectedCaloMet

process.caloType1CorrectedMetNoHF = process.caloType1CorrectedMet.clone(
    src = cms.InputTag('corMetGlobalMuonsNoHF')
)
process.patTupleProductionSequence += process.caloType1CorrectedMetNoHF
process.patType1CorrectedCaloMetNoHF = process.patType1CorrectedCaloMet.clone(
    metSource = cms.InputTag('caloType1CorrectedMetNoHF')
)
process.patTupleProductionSequence += process.patType1CorrectedCaloMetNoHF

#process.patL1ETM = cms.EDProducer("L1ExtraMEtToPATMEtConverter",
#    src = cms.InputTag('l1extraParticles', 'MET')                  
#)
#process.patTupleProductionSequence += process.patL1ETM
#--------------------------------------------------------------------------------    

#--------------------------------------------------------------------------------
# add event counter for Mauro's "self baby-sitting" technology
process.processedEventsPATtupleProduction = cms.EDProducer("EventCountProducer")
process.eventCounterPath = cms.Path(process.processedEventsPATtupleProduction)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# save PAT-tuple

process.patTupleOutputModule = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring(
        'drop *',
        'keep EventAux_*_*_*',
        'keep LumiSummary_*_*_*',                       
        'keep edmMergeableCounter_*_*_*',
        'keep *_gtDigis_*_*',
        'keep *_hltL1GtObjectMap_*_*',
        'keep *_TriggerResults_*_*',
        'keep *_goodMuons_*_*',
        'keep *_goodIsoMuons_*_*',
        'keep *_patMuons_*_*',                                            
        'keep *_goldenZmumuCandidatesGe0IsoMuons_*_*',
        'keep *_goldenZmumuCandidatesGe1IsoMuons_*_*',
        'keep *_goldenZmumuCandidatesGe2IsoMuons_*_*',                                                        
        ##'keep *_offlinePrimaryVertices_*_*',
        ##'keep *_offlinePrimaryVerticesWithBS_*_*',
        'keep *_selectedPrimaryVertexPosition_*_*',                                         
        ##'keep *_selectedPrimaryVertexHighestPtTrackSum_*_*',
        ##'keep *_selectedPrimaryVerticesTrackPtSumGt5_*_*',
        ##'keep *_selectedPrimaryVerticesTrackPtSumGt10_*_*',                                            
        'keep *_patJets_*_*',
        'keep *_smearedPatJets_*_*',
        'keep *_shiftedPatJetsEnUp*_*_*',                                
        'keep *_shiftedPatJetsEnDown*_*_*',
        'keep *_smearedPatJetsResDown_*_*',
        'keep *_smearedPatJetsResUp_*_*',
        'keep *_selectedPatJetsAntiOverlapWithMuonsVeto_*_*',
        'keep *_pfCandsNotInJet_*_*',
        ##'keep *_particleFlow_*_*',                      
        ##'keep *_ak5PFJets_*_*',                             
        'keep *_patPFMet*_*_*',
        'keep *_patType1CorrectedPFMet*_*_*',
        'keep *_patType1p2CorrectedPFMet*_*_*',
        'keep *_pfMEtSignCovMatrix*_*_*',                      
        'keep *_patPFMetMVA*_*_*',
        'keep *_patPFMetMVA2*_*_*',
        'keep *_patPFMetNoPileUp*_*_*',
        'keep CommonMETData_noPileUpPFMEt*_*_*',
        'keep *_patMinMEt*_*_*',                            
        'keep *_patCaloMet*_*_*',
        'keep *_patType1CorrectedCaloMet*_*_*',
        'keep *_patL1ETM*_*_*',                                       
        'keep *_kt6PFNeutralJetsForVtxMultReweighting_rho_*',
        'keep *_kt6PFChargedHadronNoPileUpJetsForVtxMultReweighting_rho_*',
        'keep *_kt6PFChargedHadronPileUpJetsForVtxMultReweighting_rho_*',
        'keep *_kt6PFChargedHadronJetsForVtxMultReweighting_rho_*'                                            
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p')
    ),                                            
    fileName = cms.untracked.string("/data1/veelken/tmp/ZllRecoilCorrectionPATtuple.root")
)

if isMC:
    process.patTupleOutputModule.outputCommands.extend(
        cms.untracked.vstring(
            'keep *_addPileupInfo_*_*',
            'keep *_vertexMultiplicityReweight*_*_*',
            'keep *_genParticles_*_*'
        )
    )
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# produce & save histograms for in-time & out-of-time pile-up reweighting

process.DQMStore = cms.Service("DQMStore")

process.producePUreweightHistogramsKt6PFNeutralJets = cms.EDAnalyzer("PUreweightHistogramProducer",
    srcVertices = cms.InputTag('selectedPrimaryVertexPosition'),                                                 
    srcRho = cms.InputTag('kt6PFNeutralJetsForVtxMultReweighting', 'rho'),
    rhoMax = cms.double(10.)                            
)

process.rhoNeutralAnalyzer = cms.EDAnalyzer("RhoNeutralCorrAnalyzer",
    srcVertices = cms.InputTag('selectedPrimaryVertexPosition'),
    srcRhoNeutral = cms.InputTag('kt6PFNeutralJetsForVtxMultReweighting', 'rho'),                             
    srcRhoChargedHadronsNoPileUp = cms.InputTag('kt6PFChargedHadronNoPileUpJetsForVtxMultReweighting', 'rho'),
    srcRhoChargedHadronsPileUp = cms.InputTag('kt6PFChargedHadronPileUpJetsForVtxMultReweighting', 'rho'),
    srcRhoChargedHadrons = cms.InputTag('kt6PFChargedHadronJetsForVtxMultReweighting', 'rho'),
    srcRhoNeutralHadrons = cms.InputTag('kt6PFNeutralHadronJetsForVtxMultReweighting', 'rho'),
    srcRhoPhotons = cms.InputTag('kt6PFGammaJetsForVtxMultReweighting', 'rho'),                                        
    srcWeights = cms.VInputTag()
)

process.produceAndSavePUreweightHistograms = cms.Sequence()
process.produceAndSavePUreweightHistograms += process.producePUreweightHistogramsKt6PFNeutralJets
process.produceAndSavePUreweightHistograms += process.rhoNeutralAnalyzer

process.pfChargedHadronNoPileUpAnalyzer = cms.EDAnalyzer("PFCandidateAnalyzer",
    pfCandidateSource = cms.InputTag('pfChargedHadronNoPileUpCands'),
    dqmDirectory_store = cms.string('pfChargedHadronNoPileUpAnalyzer'),
    normalization = cms.string('pfCandidates'),
    srcWeights = cms.VInputTag()                                                       
)

process.pfChargedHadronPileUpAnalyzer = process.pfChargedHadronNoPileUpAnalyzer.clone(
    pfCandidateSource = cms.InputTag('pfChargedHadronPileUpCands'),
    dqmDirectory_store = cms.string('pfChargedHadronPileUpAnalyzer')
)                                                       

process.pfNeutralHadronAnalyzer = process.pfChargedHadronNoPileUpAnalyzer.clone(
    pfCandidateSource = cms.InputTag('pfAllNeutralHadrons'),
    dqmDirectory_store = cms.string('pfNeutralHadronAnalyzer')
)

process.pfPhotonAnalyzer = process.pfChargedHadronNoPileUpAnalyzer.clone(
    pfCandidateSource = cms.InputTag('pfAllPhotons'),
    dqmDirectory_store = cms.string('pfPhotonAnalyzer')
)

process.produceAndSavePUreweightHistograms += process.pfChargedHadronNoPileUpAnalyzer
process.produceAndSavePUreweightHistograms += process.pfChargedHadronPileUpAnalyzer
process.produceAndSavePUreweightHistograms += process.pfNeutralHadronAnalyzer
process.produceAndSavePUreweightHistograms += process.pfPhotonAnalyzer
                           
if isMC:
    process.producePUreweightHistogramsKt6PFNeutralJets2012RunABCruns190456to203002 = process.producePUreweightHistogramsKt6PFNeutralJets.clone(
        srcWeight = cms.InputTag('vertexMultiplicityReweight3d2012RunABCruns190456to203002')
    )
    process.produceAndSavePUreweightHistograms += process.producePUreweightHistogramsKt6PFNeutralJets2012RunABCruns190456to203002

    process.rhoNeutralAnalyzer2012RunABCruns190456to203002 = process.rhoNeutralAnalyzer.clone(
        srcGenPileUp = cms.InputTag('addPileupInfo'),
        srcWeights = cms.VInputTag('vertexMultiplicityReweight3d2012RunABCruns190456to203002')
    )
    process.produceAndSavePUreweightHistograms += process.rhoNeutralAnalyzer2012RunABCruns190456to203002

process.savePUreweightHistograms = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('/data1/veelken/tmp/ZllRecoilCorrectionPUreweightHistograms.root')
)
process.produceAndSavePUreweightHistograms += process.savePUreweightHistograms
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# CV: add DEBUGging module
##process.debugMEtSystematics = cms.EDAnalyzer("MEtSysDEBUGGER",
##    srcMEt_central = cms.InputTag('patPFMetNoPileUpNoSmearing'),
##    srcMEt_shifted = cms.InputTag('patPFMetNoPileUp'),
##    srcZllCandidates = cms.InputTag('goldenZmumuCandidatesGe1IsoMuons')
##)
##process.patTupleProductionSequence += process.debugMEtSystematics
#--------------------------------------------------------------------------------

# before starting to process 1st event, print event content
process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")
process.filterFirstEvent = cms.EDFilter("EventCountFilter",
    numEvents = cms.int32(1)
)
process.printFirstEventContentPath = cms.Path(process.filterFirstEvent + process.printEventContent)

process.p = cms.Path(
    process.prePatProductionSequence
   + process.patTupleProductionSequence
   + process.produceAndSavePUreweightHistograms
)

process.o = cms.EndPath(process.patTupleOutputModule)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

processDumpFile = open('produceZllRecoilCorrectionPATTuple.dump' , 'w')
print >> processDumpFile, process.dumpPython()
