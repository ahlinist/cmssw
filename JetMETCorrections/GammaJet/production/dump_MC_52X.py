#
# $Id: dump_MC_52X.py,v 1.1 2012/05/30 14:22:24 meridian Exp $
#
#  configuration to dump ntuples in MC
#   the only diff should be for jetmet corrections
#
import FWCore.ParameterSet.Config as cms
import sys
import os
import imp

# contains all details of what to run and defines the process and the path
#from  JetMETCorrections.GammaJet.dumper_process_cff import process

# read in process from file
filename  = 'common_dump_config.py'
handle = open(filename, 'r')
cfo = imp.load_source("pycfg", filename, handle)
process = cfo.process
handle.close()

process.p = cms.Path(process.analysisSequence)

## DO NOT CHANGE THE PATH HERE! New modules should be added ONLY in the common configuration 
#  only paramaters should be changes for data and MC
process.source.fileNames = cms.untracked.vstring(
#    '/store/mc/Summer11/GluGluToHToGG_M-115_7TeV-powheg-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/842C8A8B-967F-E011-90C4-E0CB4E55363A.root'
#    'root://pccmsrm27///cms/local/meridian/data/Summer12/vbf1208tev.root'
#'/store/group/phys_higgs/meridian/HGGProd/GluGlu_HToGG_M-120_8TeV-powheg-pythia6-TuneZ2Star-PU_S7_START52_V9/meridian/GluGlu_HToGG_M-120_8TeV-powheg-LHE_v1/MinBias_TuneZ2_7TeV_START42_V12_38T_GluGlu_HToGG_M-120_8TeV-powheg-pythia6-TuneZ2Star-PU_S7_START52_V9/02bbbcabd5cce5a11b868478ebd549ea/POWHEG_PYTHIA6_Tauola_H_2gamma_8TeV_cff_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO_PU_275_1_hS5.root',
#'/store/group/phys_higgs/meridian/HGGProd/GluGlu_HToGG_M-120_8TeV-powheg-pythia6-TuneZ2Star-PU_S7_START52_V9/meridian/GluGlu_HToGG_M-120_8TeV-powheg-LHE_v1/MinBias_TuneZ2_7TeV_START42_V12_38T_GluGlu_HToGG_M-120_8TeV-powheg-pythia6-TuneZ2Star-PU_S7_START52_V9/02bbbcabd5cce5a11b868478ebd549ea/POWHEG_PYTHIA6_Tauola_H_2gamma_8TeV_cff_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO_PU_281_1_cBW.root',
#'/store/group/phys_higgs/meridian/HGGProd/GluGlu_HToGG_M-120_8TeV-powheg-pythia6-TuneZ2Star-PU_S7_START52_V9/meridian/GluGlu_HToGG_M-120_8TeV-powheg-LHE_v1/MinBias_TuneZ2_7TeV_START42_V12_38T_GluGlu_HToGG_M-120_8TeV-powheg-pythia6-TuneZ2Star-PU_S7_START52_V9/02bbbcabd5cce5a11b868478ebd549ea/POWHEG_PYTHIA6_Tauola_H_2gamma_8TeV_cff_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO_PU_364_1_egd.root'
'/store/group/phys_higgs/meridian/HGGProd/VBF_HToGG_M-120_8TeV-powheg-pythia6-TuneZ2Star-PU_S7_START52_V9/meridian/VBF_HToGG_M-120_8TeV-powheg-LHE_v1/MinBias_TuneZ2_7TeV_START42_V12_38T_VBF_HToGG_M-120_8TeV-powheg-pythia6-TuneZ2Star-PU_S7_START52_V9/02bbbcabd5cce5a11b868478ebd549ea/POWHEG_PYTHIA6_Tauola_H_2gamma_8TeV_cff_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO_PU_236_1_vQn.root',
'/store/group/phys_higgs/meridian/HGGProd/VBF_HToGG_M-120_8TeV-powheg-pythia6-TuneZ2Star-PU_S7_START52_V9/meridian/VBF_HToGG_M-120_8TeV-powheg-LHE_v1/MinBias_TuneZ2_7TeV_START42_V12_38T_VBF_HToGG_M-120_8TeV-powheg-pythia6-TuneZ2Star-PU_S7_START52_V9/02bbbcabd5cce5a11b868478ebd549ea/POWHEG_PYTHIA6_Tauola_H_2gamma_8TeV_cff_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO_PU_237_1_2xS.root',
'/store/group/phys_higgs/meridian/HGGProd/VBF_HToGG_M-120_8TeV-powheg-pythia6-TuneZ2Star-PU_S7_START52_V9/meridian/VBF_HToGG_M-120_8TeV-powheg-LHE_v1/MinBias_TuneZ2_7TeV_START42_V12_38T_VBF_HToGG_M-120_8TeV-powheg-pythia6-TuneZ2Star-PU_S7_START52_V9/02bbbcabd5cce5a11b868478ebd549ea/POWHEG_PYTHIA6_Tauola_H_2gamma_8TeV_cff_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO_PU_256_1_SNm.root'
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Global tag#
# Remember to change it and adapt to your needs #
process.GlobalTag.globaltag = cms.string('START52_V9::All')

##  apply only L2 and L3 jet corrections in MC
process.load("CondCore.DBCommon.CondDBCommon_cfi")
from CondCore.DBCommon.CondDBSetup_cfi import *
process.jec = cms.ESSource("PoolDBESSource",
                           DBParameters = cms.PSet(
    messageLevel = cms.untracked.int32(0)
    ),
                           timetype = cms.string('runnumber'),
                           toGet = cms.VPSet(
    cms.PSet(
    record = cms.string('JetCorrectionsRecord'),
    tag    = cms.string('JetCorrectorParametersCollection_Summer12_V7_MC_AK5PF'),
    label  = cms.untracked.string('AK5PF')
    ),
    ## here you add as many jet types as you need
    ## note that the tag name is specific for the particular sqlite file
    ),
                           connect = cms.string('sqlite:Summer12_V7_MC.db')
                           )
## add an es_prefer statement to resolve a possible conflict from simultaneous connection to a global tag
process.es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')

process.myanalysis.JetCorrectionService_akt5 = cms.string('ak5CaloL1L2L3')
process.myanalysis.JetCorrectionService_akt7 = cms.string('ak7CaloL1L2L3')
process.myanalysis.JetCorrectionService_jptak5 = cms.string('ak5JPTL1L2L3')
process.myanalysis.JetCorrectionService_jptak7 = cms.string('ak7JPTL1L2L3')
process.myanalysis.JetCorrectionService_pfakt5 = cms.string('ak5PFL1FastL2L3')
process.myanalysis.JetCorrectionService_pfakt7 = cms.string('ak7PFL1FastL2L3')
#process.myanalysis.TriggerTag = cms.untracked.InputTag("TriggerResults::REDIGI311X")

#print process.dumpPython()
