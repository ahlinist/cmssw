import FWCore.ParameterSet.Config as cms

process = cms.Process("Ana")
process.load('FWCore.MessageService.MessageLogger_cfi')
##-------------------- Communicate with the DB -----------------------
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_R_311_V2::All'
##-------------------- Import the JEC services -----------------------
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
#process.ak5CaloL1Offset.useCondDB = False
#process.ak5PFL1Offset.useCondDB = False
#############   Set the number of events #############
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)
#############   Define the source file ###############
process.load('QCDAnalysis.HighPtJetAnalysis.SkimFileNames_cfi')
#############   Import the HLT filters ###############
process.load('QCDAnalysis.HighPtJetAnalysis.hltFilters_cff')
#############   Define the source file ###############
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/user/kkousour/2011/Jets/JetAOD_9_1_Bh3.root')
)
############# processed tree producer ##################
process.TFileService = cms.Service("TFileService",fileName = cms.string('ProcessedTree_data.root'))

process.ak5 = cms.EDAnalyzer('ProcessedTreeProducer',
    ## jet collections ###########################
    pfjets          = cms.string('ak5PFJets'),
    calojets        = cms.string('ak5CaloJets'),
    ## database entry for the uncertainties ######
    PFPayloadName   = cms.string('AK5PF'),
    CaloPayloadName = cms.string('AK5Calo'),
    ## calojet ID and extender for the JTA #######
    calojetID       = cms.string('ak5JetID'),
    calojetExtender = cms.string('ak5JetExtender'),
    ## set the conditions for good Vtx counting ##
    goodVtxNdof     = cms.double(4),
    goodVtxZ        = cms.double(24),
    ## activate the dijet preselection ###########
    preselDijet     = cms.bool(True),
    ## number of jets to be stored ###############
    nPFJets         = cms.int32(4),
    nCaloJets       = cms.int32(4),
    ## jec services ##############################
    pfjecService    = cms.string('ak5PFL1L2L3Residual'),
    calojecService  = cms.string('ak5CaloL1L2L3Residual')
)

process.ak7 = cms.EDAnalyzer('ProcessedTreeProducer',
    ## jet collections ###########################
    pfjets          = cms.string('ak7PFJets'),
    calojets        = cms.string('ak7CaloJets'),
    ## database entry for the uncertainties ######
    PFPayloadName   = cms.string('AK7PF'),
    CaloPayloadName = cms.string('AK7Calo'),
    ## calojet ID and extender for the JTA #######
    calojetID       = cms.string('ak7JetID'),
    calojetExtender = cms.string('ak7JetExtender'),
    ## set the conditions for bood Vtx counting ##
    goodVtxNdof     = cms.double(4), 
    goodVtxZ        = cms.double(24),
    ## activate the dijet preselection ###########
    preselDijet     = cms.bool(True),
    ## number of jets to be stored ###############
    nPFJets         = cms.int32(4),
    nCaloJets       = cms.int32(4),
    ## jec services ##############################
    pfjecService    = cms.string('ak7PFL1L2L3Residual'),
    calojecService  = cms.string('ak7CaloL1L2L3Residual')
)

process.path = cms.Path(process.hlt110v1 * process.ak5 * process.ak7)
#############   Format MessageLogger #################
process.MessageLogger.cerr.FwkReport.reportEvery = 100


