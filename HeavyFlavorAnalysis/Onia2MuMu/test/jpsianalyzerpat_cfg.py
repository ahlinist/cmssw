import FWCore.ParameterSet.Config as cms

process = cms.Process("ANAPAT")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
    fileNames = cms.untracked.vstring(
'rfio:/castor/cern.ch/user/c/covarell/temp/testNewCSCTF_data_1_1_SSn.root', 
'rfio:/castor/cern.ch/user/c/covarell/temp/testNewCSCTF_data_2_1_EMo.root', 
'rfio:/castor/cern.ch/user/c/covarell/temp/testNewCSCTF_data_2_1_LLj.root', 
'rfio:/castor/cern.ch/user/c/covarell/temp/testNewCSCTF_data_2_1_Xmn.root', 
'rfio:/castor/cern.ch/user/c/covarell/temp/testNewCSCTF_data_3_1_2LZ.root', 
'rfio:/castor/cern.ch/user/c/covarell/temp/testNewCSCTF_data_3_1_JHH.root', 
'rfio:/castor/cern.ch/user/c/covarell/temp/testNewCSCTF_data_3_1_OY8.root', 
'rfio:/castor/cern.ch/user/c/covarell/temp/testNewCSCTF_data_3_1_s8P.root', 
'rfio:/castor/cern.ch/user/c/covarell/temp/testNewCSCTF_data_4_1_ZY1.root', 
'rfio:/castor/cern.ch/user/c/covarell/temp/testNewCSCTF_data_4_1_kap.root', 
'rfio:/castor/cern.ch/user/c/covarell/temp/testNewCSCTF_data_4_1_tbk.root', 
'rfio:/castor/cern.ch/user/c/covarell/temp/testNewCSCTF_data_4_1_xcF.root', 
'rfio:/castor/cern.ch/user/c/covarell/temp/testNewCSCTF_data_5_1_dHS.root', 
'rfio:/castor/cern.ch/user/c/covarell/temp/testNewCSCTF_data_6_1_Lnu.root', 
'rfio:/castor/cern.ch/user/c/covarell/temp/testNewCSCTF_data_6_1_p3K.root', 
'rfio:/castor/cern.ch/user/c/covarell/temp/testNewCSCTF_data_7_1_gFb.root', 
'rfio:/castor/cern.ch/user/c/covarell/temp/testNewCSCTF_data_8_1_XJi.root' 
    )
)

# process.options = cms.untracked.PSet(
#    IgnoreCompletely = cms.untracked.vstring( "ProductNotFound" )
# )

process.hltMu3 = cms.EDFilter("HLTHighLevel",
                 TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                 HLTPaths = cms.vstring("HLT_L1DoubleMuOpen"),
                 eventSetupPathsKey = cms.string(''),
                 andOr = cms.bool(True),
                 throw = cms.bool(False)
)

process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 500

process.demo = cms.EDAnalyzer('JPsiAnalyzerPAT',

    src = cms.InputTag("onia2MuMuPatTrkTrk"),
    srcWithCaloMuons = cms.InputTag("onia2MuMuPatGlbCal"),

    histFileName = cms.string("Histos_362_data.root"),   
    dataSetName = cms.string("DataSet_362_data.root"),
                              
    pTBinRanges = cms.vdouble(0.0, 6.0, 8.0, 9.0, 10.0, 12.0, 15.0, 40.0),
    # pTBinRanges = cms.vdouble(0.0, 40.0),
    etaBinRanges = cms.vdouble(0.0, 1.3, 2.5),
    # etaBinRanges = cms.vdouble(0.0, 3.0),
    onlyTheBest = cms.bool(True),		
    applyCuts = cms.bool(True),			
    storeEfficiency = cms.bool(False),	
    useBeamSpot = cms.bool(True),
    useRapidity = cms.bool(True),
    useCaloMuons = cms.untracked.bool(False),
    removeSignalEvents = cms.untracked.bool(False),
    removeTrueMuons = cms.untracked.bool(False),
    storeWrongSign = cms.untracked.bool(True),
    writeOutCandidates = cms.untracked.bool(False),
    includePsiPrime = cms.untracked.bool(False)
)


process.p = cms.Path(process.hltMu3*process.demo)
