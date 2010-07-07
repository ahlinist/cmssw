import FWCore.ParameterSet.Config as cms

maxevents = 100  # specify number of events
isMC = False  # Specify whether file is Monte Carlo or real data
version = 10
# VERSION 10 is the currently specified version as of 7 July 2010

process = cms.Process('USER')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

# GLOBAL TAGS: REPLACE WITH WHATEVER IS APPROPRIATE FOR YOUR WORK!!!!!!!

# MC
if (isMC):
    process.GlobalTag.globaltag ='START3X_V26A::All'

# DATA
else:
    process.GlobalTag.globaltag ='GR10_P_V4::All'

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(maxevents)
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(False)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/601/F65A94F7-4141-DF11-9F4E-003048D47A80.root'
    #'file:/home/jtemple/TEST_HCAL_RECO_FILE.root'
    )

)

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.RECOEventContent.outputCommands,
    fileName = cms.untracked.string('output_file.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RECO'),
        filterName = cms.untracked.string('')
    )
)

# Other statements
process.MessageLogger.cerr.FwkReport.reportEvery = 1


import JetMETAnalysis.HcalReflagging.RemoveAddSevLevel as RemoveAddSevLevel
#process.hcalRecAlgos=RemoveAddSevLevel.RemoveFlag(process.hcalRecAlgos,"HFLongShort")
#process.hcalRecAlgos=RemoveAddSevLevel.AddFlag(process.hcalRecAlgos,"UserDefinedBit0",10)




# HF RecHit reflagger
process.load("JetMETAnalysis/HcalReflagging/HFrechitreflaggerJETMET_cff")
if version==1:
    process.hfrecoReflagged = process.HFrechitreflaggerJETMETv1.clone()
elif version==2:
    process.hfrecoReflagged = process.HFrechitreflaggerJETMETv2.clone()
elif version==3:
    process.hfrecoReflagged = process.HFrechitreflaggerJETMETv3.clone()
elif version==4:
    if (isMC==False):
        process.hfrecoReflagged = process.HFrechitreflaggerJETMETv4.clone()
    else:
        process.hfrecoReflagged = process.HFrechitreflaggerJETMETv2.clone()  
elif version==5:
    if (isMC==False):
        process.hfrecoReflagged = process.HFrechitreflaggerJETMETv5.clone()
    else:
        process.hfrecoReflagged = process.HFrechitreflaggerJETMETv3.clone()  
elif version==10:
    process.hfrecoReflagged = process.HFrechitreflaggerJETMETv10.clone()
    if (isMC==True):  # V10 cleaning uses results of prior flags when setting new flags
        process.hfrecoReflagged.PETstat.flagsToSkip =string.atoi('10',2)
        process.hfrecoReflagged.S8S1stat.flagsToSkip=string.atoi('10010',2)
        process.hfrecoReflagged.S9S1stat.flagsToSkip=string.atoi('11010',2)
        process.hfrecoReflagged.FlagsToSet=(4,3,0)  # set flag 4 (HFPET -- also sets HFLongShort), then flag 3 (HFS8S1 -- also sets HFLongShort), then flag 0 (HFLongShort -- set directly via S9S1)

# Add debugging here
process.hfrecoReflagged.debug=1


# Versions set the flag "HFLongShort", which needs to have a severity level > 9 to be excluded from CaloTowers.  Some versions (4 & 5) set the flag "HFInTimeWindow" -- (bit 2) you can turn on that if you wish.
# Starting with release 3_7, the HFLongShort and HFDigiTime flags should be active by default in data.  However, HFDigiTime is not turned on by default, because it does not provide useful results for MC.  It must be activated here.


import JetMETAnalysis.HcalReflagging.RemoveAddSevLevel as RemoveAddSevLevel
process.hcalRecAlgos=RemoveAddSevLevel.AddFlag(process.hcalRecAlgos,"HFLongShort",11)
if (isMC==False):  # Don't use HFDigiTime on MC !
    process.hcalRecAlgos=RemoveAddSevLevel.AddFlag(process.hcalRecAlgos,"HFDigiTime",11)
if process.hfrecoReflagged.debug>0:
    for i in process.hcalRecAlgos.SeverityLevels:
        print i



process.output.outputCommands.append("keep *_hfrecoReflagged_*_*")
#new rechit collection name is:  HFRecHitsSorted_hfrecoReflagged__USER

# Use the reflagged HF RecHits to make the CaloTowers
process.towerMaker.hfInput = "hfrecoReflagged"
process.towerMakerWithHO.hfInput = "hfrecoReflagged"

# Path and EndPath definitions
process.reflagging_step = cms.Path(process.hfrecoReflagged)
process.rereco_step = cms.Path(process.caloTowersRec
                               *(process.recoJets*process.recoJetIds+process.recoTrackJets)
                               *process.recoJetAssociations
                               *process.metreco
                               ) # re-reco jets and met
#process.rereco_step = cms.Path(process.towerMaker*process.ak5CaloJets*process.met) # a simpler use case
process.out_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule(process.reflagging_step,
                                process.rereco_step,
                                process.out_step)
