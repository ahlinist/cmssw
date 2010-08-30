##   #########################################################
##
##   Configuration for the production of the ICHEP VBTF ntuple
##   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
##   MC, spring10
##
##   Nikolaos Rompotis - Imperial College London
##   22 June 2010
##
##   #########################################################
import FWCore.ParameterSet.Config as cms

process = cms.Process("PAT")



process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

#process.MessageLogger = cms.Service(
#        "MessageLogger",
#            categories = cms.untracked.vstring('info', 'debug','cout')
#            )


process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = cms.untracked.string("INFO")
process.MessageLogger.cerr.FwkSummary = cms.untracked.PSet(
       reportEvery = cms.untracked.int32(1000000),
          limit = cms.untracked.int32(10000000)
       )
process.MessageLogger.cerr.FwkReport = cms.untracked.PSet(
      reportEvery = cms.untracked.int32(100000),
         limit = cms.untracked.int32(10000000)
      )
process.options = cms.untracked.PSet(
       wantSummary = cms.untracked.bool(True)
       )




# source
process.source = cms.Source("PoolSource", 
     fileNames = cms.untracked.vstring(
    # SOME DATA FILE TO BE PUT HERE
    #'rfio:/castor/cern.ch/user/r/rompotis/DATA_STUDIES/Spring10/sample_WminusToENu-CTEQ66-powheg_Spring10-START3X_V26_AODSIM-v2.root',
    #'file:rfio:/castor/cern.ch/user/r/rompotis/DATA_STUDIES/Spring10/sample_WenuSpring10START3X_V26_S09-v1_AODSIM.root',
#    '/store/data/Commissioning10/MinimumBias/RECO/SD_EG-Jun14thSkim_v1/0149/FEDC34AB-7F84-DF11-A007-00261894385A.root'
#    '/store/data/Run2010A/EG/RECO/Jun14thReReco_v1/0001/FE96AFA9-C678-DF11-9E4C-003048F0E7FC.root',
#    '/store/data/Run2010A/EG/RECO/Jun14thReReco_v1/0001/F266F2FA-BE78-DF11-BEB4-003048F0E81E.root',
#    '/store/data/Run2010A/EG/RECO/Jun14thReReco_v1/0001/EA166B12-C378-DF11-9BEC-003048CF6334.root',
#    '/store/data/Run2010A/EG/RECO/Jun14thReReco_v1/0001/E8CCAFFA-BE78-DF11-8D0B-003048F0E81E.root',
#    '/store/data/Run2010A/EG/RECO/Jun14thReReco_v1/0001/E2B13F9F-C978-DF11-B42A-003048F0E81E.root',
#    '/store/data/Run2010A/EG/RECO/Jun14thReReco_v1/0001/DC224012-C378-DF11-B414-003048CF6334.root',
#    '/store/data/Run2010A/EG/RECO/Jun14thReReco_v1/0001/D449BF0F-C378-DF11-AB42-003048F0E81E.root',
#    '/store/data/Run2010A/EG/RECO/Jun14thReReco_v1/0001/D084A5EF-CA78-DF11-B000-003048F0E186.root',
#    '/store/data/Run2010A/EG/RECO/Jun14thReReco_v1/0001/C892F555-C378-DF11-919F-003048F0E81E.root',
#    '/store/data/Run2010A/EG/RECO/Jun14thReReco_v1/0001/C842530C-C378-DF11-BD97-003048CF632E.root',
#    '/store/data/Run2010A/EG/RECO/Jun14thReReco_v1/0001/C828CC0C-C378-DF11-B72D-003048CF632E.root',
#    '/store/data/Run2010A/EG/RECO/Jun14thReReco_v1/0001/BE8B579A-C978-DF11-BB94-003048CF6334.root',
#    '/store/data/Run2010A/EG/RECO/Jun14thReReco_v1/0001/B0A78AFA-BE78-DF11-88A2-003048CF6334.root',
#    '/store/data/Run2010A/EG/RECO/Jun14thReReco_v1/0001/AC3549F8-BE78-DF11-B07B-003048F0E186.root',
#    'rfio:/castor/cern.ch/cms/store/data/Run2010A/EG/RECO/Jun14thReReco_v1/0001/FE96AFA9-C678-DF11-9E4C-003048F0E7FC.root'
    'rfio:/castor/cern.ch/cms/store/data/Run2010A/EG/RECO/v4/000/140/359/B84453AC-1C92-DF11-B1F8-001617E30D52.root',
    )
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

## Load additional processes
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
## global tags:
process.GlobalTag.globaltag = cms.string('GR_R_36X_V12A::All') # GLOBAL TAG FOR DATA

process.load("Configuration.StandardSequences.MagneticField_cff")


################################################################################################
###    P r e p a r a t i o n      o f    t h e    P A T    O b j e c t s   f r o m    A O D  ###
################################################################################################

## pat sequences to be loaded:
#process.load("PhysicsTools.PFCandProducer.PF2PAT_cff")
process.load("PhysicsTools.PatAlgos.patSequences_cff")
#process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")
##
#
## %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
## MET creation     <=== WARNING: YOU MAY WANT TO MODIFY THIS PART OF THE CODE       %%%%%%%%%%%%%
##                                specify the names of the MET collections that you need here %%%%
##                                                                                             #%%
## if you don't specify anything the default MET is the raw Calo MET                           #%%
process.caloMET = process.patMETs.clone(                                                       #%%
    metSource = cms.InputTag("met","","RECO"),
    addTrigMatch = cms.bool(False),
    addMuonCorrections = cms.bool(False),
    addGenMET = cms.bool(False),
)
process.tcMET = process.patMETs.clone(                                                         #%%
    metSource = cms.InputTag("tcMet","","RECO"),
    addTrigMatch = cms.bool(False),
    addMuonCorrections = cms.bool(False),
    addGenMET = cms.bool(False),
)
process.pfMET = process.patMETs.clone(                                                         #%%
    metSource = cms.InputTag("pfMet","","RECO"),
    addTrigMatch = cms.bool(False),
    addMuonCorrections = cms.bool(False),
    addGenMET = cms.bool(False),
)
## specify here what you want to have on the plots! <===== MET THAT YOU WANT ON THE PLOTS  %%%%%%%
myMetCollection   = 'caloMET'
myPfMetCollection =   'pfMET'
myTcMetCollection =   'tcMET'
## modify the sequence of the MET creation:                                                    #%%
process.makePatMETs = cms.Sequence(process.caloMET*process.tcMET*process.pfMET)
## %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
## modify the final pat sequence: keep only electrons + METS (muons are needed for met corrections)
process.load("RecoEgamma.EgammaIsolationAlgos.egammaIsolationSequence_cff")
#process.patElectronIsolation = cms.Sequence(process.egammaIsolationSequence)

process.patElectrons.isoDeposits = cms.PSet()
process.patElectrons.userIsolation = cms.PSet()
process.patElectrons.addElectronID = cms.bool(True)
process.patElectrons.electronIDSources = cms.PSet(
    simpleEleId95relIso= cms.InputTag("simpleEleId95relIso"),
    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
    simpleEleId85relIso= cms.InputTag("simpleEleId85relIso"),
    simpleEleId80relIso= cms.InputTag("simpleEleId80relIso"),
    simpleEleId70relIso= cms.InputTag("simpleEleId70relIso"),
    simpleEleId60relIso= cms.InputTag("simpleEleId60relIso"),
    simpleEleId95cIso= cms.InputTag("simpleEleId95cIso"),
    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
    simpleEleId85cIso= cms.InputTag("simpleEleId85cIso"),
    simpleEleId80cIso= cms.InputTag("simpleEleId80cIso"),
    simpleEleId70cIso= cms.InputTag("simpleEleId70cIso"),
    simpleEleId60cIso= cms.InputTag("simpleEleId60cIso"),    
    )
##
process.patElectrons.addGenMatch = cms.bool(False)
process.patElectrons.embedGenMatch = cms.bool(False)
process.patElectrons.usePV = cms.bool(False)
##
process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff")
# you have to tell the ID that it is data
process.simpleEleId95relIso.dataMagneticFieldSetUp = cms.bool(True)
process.simpleEleId90relIso.dataMagneticFieldSetUp = cms.bool(True)
process.simpleEleId85relIso.dataMagneticFieldSetUp = cms.bool(True)
process.simpleEleId80relIso.dataMagneticFieldSetUp = cms.bool(True)
process.simpleEleId70relIso.dataMagneticFieldSetUp = cms.bool(True)
process.simpleEleId60relIso.dataMagneticFieldSetUp = cms.bool(True)
process.simpleEleId95cIso.dataMagneticFieldSetUp = cms.bool(True)
process.simpleEleId90cIso.dataMagneticFieldSetUp = cms.bool(True)
process.simpleEleId85cIso.dataMagneticFieldSetUp = cms.bool(True)
process.simpleEleId80cIso.dataMagneticFieldSetUp = cms.bool(True)
process.simpleEleId70cIso.dataMagneticFieldSetUp = cms.bool(True)
process.simpleEleId60cIso.dataMagneticFieldSetUp = cms.bool(True)
#
process.patElectronIDs = cms.Sequence(process.simpleEleIdSequence)
process.makePatElectrons = cms.Sequence(process.patElectronIDs*process.patElectrons)
# process.makePatMuons may be needed depending on how you calculate the MET
process.makePatCandidates = cms.Sequence(process.makePatElectrons+process.makePatMETs)
process.patDefaultSequence = cms.Sequence(process.makePatCandidates)
##
##  ################################################################################
##
##  the filter to select the candidates from the data samples
##
##
## WARNING: you may want to modify this item:
HLT_process_name = "HLT"   # REDIGI for the Spring10 production traditional MC / HLT for the powheg samples or data
# trigger path selection
HLT_path_name     = "HLT_Photon10_L1R" #= "HLT_Ele15_LW_L1R" #
# trigger filter name
HLT_filter_name  =  "hltL1NonIsoHLTNonIsoSinglePhotonEt10HcalIsolFilter"
#
HLT_path_name_extra0   = "HLT_Photon15_L1R"
HLT_filter_name_extra0 = cms.untracked.InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt15HcalIsolFilter","",HLT_process_name)
HLT_path_name_extra1   = "HLT_Photon15_Cleaned_L1R"
HLT_filter_name_extra1 = cms.untracked.InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt15CleanedHcalIsolFilter","",HLT_process_name)

#HLT_path_name_extra2   = "HLT_Ele15_LW_L1R"
#HLT_filter_name_extra2 = cms.untracked.InputTag("hltL1NonIsoHLTNonIsoSingleElectronLWEt15PixelMatchFilter","",HLT_process_name)

HLT_path_name_extra2   = "HLT_Ele15_SW_L1R"
HLT_filter_name_extra2 = cms.untracked.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15PixelMatchFilter","",HLT_process_name)

process.wenuFilter = cms.EDFilter('WenuCandidateFilter',
                                  ### the input collections needed:
                                  electronCollectionTag = cms.untracked.InputTag("patElectrons","","PAT"),
                                  metCollectionTag = cms.untracked.InputTag(myMetCollection,"","PAT"),
                                  pfMetCollectionTag = cms.untracked.InputTag(myPfMetCollection,"","PAT"),
                                  tcMetCollectionTag = cms.untracked.InputTag(myTcMetCollection,"","PAT"),
                                  triggerCollectionTag = cms.untracked.InputTag("TriggerResults","",HLT_process_name),
                                  triggerEventTag = cms.untracked.InputTag("hltTriggerSummaryAOD","",HLT_process_name),
                                  hltpath = cms.untracked.string(HLT_path_name), 
                                  hltpathFilter = cms.untracked.InputTag(HLT_filter_name,"",HLT_process_name),
                                  ebRecHits = cms.untracked.InputTag("reducedEcalRecHitsEB"),
                                  eeRecHits = cms.untracked.InputTag("reducedEcalRecHitsEE"),
                                  PrimaryVerticesCollection = cms.untracked.InputTag("offlinePrimaryVertices"),
                                  ### here the preselection is applied
                                  # fiducial cuts:
                                  BarrelMaxEta = cms.untracked.double(1.4442),
                                  EndCapMinEta = cms.untracked.double(1.566),
                                  EndCapMaxEta = cms.untracked.double(2.5),
                                  # demand ecal driven electron:
                                  useEcalDrivenElectrons = cms.untracked.bool(True),
                                  # demand offline spike cleaning with the Swiss Cross criterion:
                                  useSpikeRejection = cms.untracked.bool(True),
                                  spikeCleaningSwissCrossCut = cms.untracked.double(0.95),
                                  # demand geometrically matched to an HLT object with ET>15GeV
                                  useTriggerInfo = cms.untracked.bool(True),
                                  electronMatched2HLT = cms.untracked.bool(True),
                                  electronMatched2HLT_DR = cms.untracked.double(0.1),
                                  useHLTObjectETCut = cms.untracked.bool(True),
                                  hltObjectETCut = cms.untracked.double(15.),
                                  useExtraTrigger = cms.untracked.bool(True),
                                  vHltpathExtra = cms.untracked.vstring(HLT_path_name_extra0,HLT_path_name_extra1,HLT_path_name_extra2),
                                  vHltpathFilterExtra = cms.untracked.VInputTag(HLT_filter_name_extra0, HLT_filter_name_extra1, HLT_filter_name_extra2),
                                  # ET Cut in the SC
                                  ETCut = cms.untracked.double(20.),                                  
                                  METCut = cms.untracked.double(0.),
                                  # reject events with a 2nd electron with ET > 20 that passes the WP95%
                                  vetoSecondElectronEvents = cms.untracked.bool(False),
                                  storeSecondElectron = cms.untracked.bool(True),
                                  ETCut2ndEle = cms.untracked.double(20.),
                                  vetoSecondElectronIDType = cms.untracked.string("simpleEleId95relIso"),
                                  vetoSecondElectronIDSign = cms.untracked.string("="),
                                  vetoSecondElectronIDValue = cms.untracked.double(7.),
                                  # Other parameters of the code - leave them as they are
                                  useValidFirstPXBHit = cms.untracked.bool(False),
                                  useConversionRejection = cms.untracked.bool(False),
                                  useExpectedMissingHits = cms.untracked.bool(False),
                                  maxNumberOfExpectedMissingHits = cms.untracked.int32(1),
                                  # calculate some new cuts
                                  calculateValidFirstPXBHit = cms.untracked.bool(True),
                                  calculateConversionRejection = cms.untracked.bool(True),
                                  calculateExpectedMissingHits = cms.untracked.bool(True),
                                  # we are dealing with DATA
                                  dataMagneticFieldSetUp = cms.untracked.bool(True),
                                  dcsTag = cms.untracked.InputTag("scalersRawToDigi"),
                                  )
####################################################################################
##
## the W selection that you prefer included in another cfg
from ElectroWeakAnalysis.WENu.simpleCutBasedSpring10SelectionBlocks_cfi import *

selection_inverse = cms.PSet (
    deta_EB_inv = cms.untracked.bool(True),
    deta_EE_inv = cms.untracked.bool(True)
    )

####################################################################################
#
# we need to store jet information, hence we have to produce the jets:
process.load("JetMETCorrections.Configuration.DefaultJEC_cff")

process.jetSequence = cms.Sequence( process.ak5CaloJetsL2L3  )
process.pfjetAK5Sequence = cms.Sequence( process.ak5PFJetsL2L3 )

process.ourJetSequence = cms.Sequence( process.jetSequence * process.pfjetAK5Sequence )



##
## and the plot creator
process.plotter = cms.EDAnalyzer('WenuPlots',
                                 # selection in use: wont be used - we have usePrecalcID true later
                                 selection_80relIso,
                                 selection_inverse,
                                 # The selection to be used here:
                                 usePrecalcID = cms.untracked.bool(True),
                                 usePrecalcIDType = cms.untracked.string('simpleEleId80relIso'),
                                 usePrecalcIDSign = cms.untracked.string('='),
                                 usePrecalcIDValue = cms.untracked.double(7),
                                 # some extra information on the ntuple production:
                                 includeJetInformationInNtuples = cms.untracked.bool(True),
                                 caloJetCollectionTag = cms.untracked.InputTag('ak5CaloJetsL2L3'),
                                 pfJetCollectionTag = cms.untracked.InputTag('ak5PFJetsL2L3'),
                                 DRJetFromElectron = cms.untracked.double(0.3),
                                 #
                                 wenuCollectionTag = cms.untracked.InputTag("wenuFilter","selectedWenuCandidates","PAT"),
                                 WENU_VBTFselectionFileName = cms.untracked.string("WENU_VBTFselection.root"),
                                 WENU_VBTFpreseleFileName = cms.untracked.string("WENU_VBTFpreselection.root"),
                                 DatasetTag =  cms.untracked.int32(100),
                                 storeExtraInformation = cms.untracked.bool(True),
                                 storeAllSecondElectronVariables = cms.untracked.bool(True),
                                 )
#
# if you run on data then you have to do misalignment  corrections first!!!
# not to be used with MC!!
process.load("RecoEgamma.EgammaTools.correctedElectronsProducer_cfi")
process.p = cms.Path( process.gsfElectrons*process.ourJetSequence*
                      process.patDefaultSequence*process.wenuFilter*process.plotter)
#process.p = cms.Path( process.ourJetSequence * process.patDefaultSequence +process.wenuFilter + process.plotter)


