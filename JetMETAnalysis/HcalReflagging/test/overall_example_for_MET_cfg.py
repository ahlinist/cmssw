import FWCore.ParameterSet.Config as cms

maxevents = 100
isMC = False

# Specify whether or not to apply HBHE cleaning at RecHit level
# Don't run with both the 'useHBHEcleaning' and 'useHBHEfilter' flags set to 'True'
# unless you know what you're doing!

useHBHEcleaning = False

# Specify whether to use HBHE event filter
# Need to do the following if this filter is true:
#addpkg CommonTools/RecoAlgos
#cvs update -r V00-02-12 CommonTools/RecoAlgos/src/HBHENoiseFilter.cc
#cvs update -r V00-02-12 CommonTools/RecoAlgos/interface/HBHENoiseFilter.h
#cvs update -r V00-02-12 CommonTools/RecoAlgos/python/HBHENoiseFilter_cfi.py
#cvs update -r V00-02-12 CommonTools/RecoAlgos/BuildFile

useHBHEfilter = True



# Specify HF cleaning version here.
# (version 2 is recommended default)
version = 2   # version 1 = default (loose), version 2 = (medium), version 3 = (tight)
# VERSION 2 is the currently recommended version, as of 22 April 2010.

process = cms.Process('USER')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

process.MessageLogger.cerr.FwkReport.reportEvery = 1


###########################################################################################

# GLOBAL TAGS: REPLACE WITH WHATEVER IS APPROPRIATE FOR YOUR WORK!
# Find the list of global tags at:  
#    https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions

# MC
if (isMC):
    process.GlobalTag.globaltag ='START3X_V26A::All'

# DATA
else:
    process.GlobalTag.globaltag ='GR10_P_V4::All'


############################################################################################
    
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(maxevents)
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(False)
)

# SPECIFY YOUR SOURCE FILE HERE!
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/601/F65A94F7-4141-DF11-9F4E-003048D47A80.root'
    )
)

# Output definition -- produces revised root file with new collections

process.output = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.RECOEventContent.outputCommands,
    fileName = cms.untracked.string('output_file.root'), # change to whatever output name you'd like
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RECO'),
        filterName = cms.untracked.string('')
    )
)

if useHBHEfilter == True:
    process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')
    process.p = cms.Path(process.HBHENoiseFilter)
    process.output.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('p') )




# New SeverityLevelComputer that forces RecHits with UserDefinedBit0 set to be excluded from new rechit collection
import JetMETAnalysis.HcalReflagging.RemoveAddSevLevel as RemoveAddSevLevel
process.hcalRecAlgos=RemoveAddSevLevel.RemoveFlag(process.hcalRecAlgos,"HFLongShort")

# UserDefinedBit0 is used by both the HF and HBHE reflaggers
process.hcalRecAlgos=RemoveAddSevLevel.AddFlag(process.hcalRecAlgos,"UserDefinedBit0",10)

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


# HBHE RecHit reflagger
process.load("JetMETAnalysis/HcalReflagging/hbherechitreflaggerJETMET_cfi")
process.hbherecoReflagged = process.hbherechitreflaggerJETMET.clone()
process.hbherecoReflagged.debug=0

# Best current cuts (as of 12 May 2010):  require at least 14 hits with E > 0.5 GeV

# Set energy threshold for identifying noise
process.hbherecoReflagged.Ethresh=0.5
# Set number of channels/hpd that must be above threshold in order for the HPD to be marked noisy
process.hbherecoReflagged.Nhits=14


process.output.outputCommands.append("keep *_hfrecoReflagged_*_*")
#new HF rechit collection name is:  HFRecHitsSorted_hfrecoReflagged__USER

# Use the reflagged HF RecHits to make the CaloTowers
process.towerMaker.hfInput = "hfrecoReflagged"
process.towerMakerWithHO.hfInput = "hfrecoReflagged"

# Path and EndPath definitions

if (useHBHEcleaning==False):
    process.reflagging_step = cms.Path(process.hfrecoReflagged)
else:
    process.reflagging_step = cms.Path(process.hfrecoReflagged+process.hbherecoReflagged)
    # Need to specify that new HBHE collection should be fed to calotower maker
    process.towerMaker.hbheInput = "hbherecoReflagged"
    process.towerMakerWithHO.hbheInput = "hbherecoReflagged"
    process.output.outputCommands.append("keep *_hbherecoReflagged_*_*")




process.rereco_step = cms.Path(process.caloTowersRec*(process.recoJets*process.recoJetIds+process.recoTrackJets)*process.recoJetAssociations*process.metreco) # re-reco jets and met
process.out_step = cms.EndPath(process.output)

# Schedule definition
if useHBHEfilter == True:
    process.schedule = cms.Schedule(process.p, process.reflagging_step,process.rereco_step,process.out_step)
else:
    process.schedule = cms.Schedule(process.reflagging_step,process.rereco_step,process.out_step)
