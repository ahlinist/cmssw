#
# $Id: dump_MC.py,v 1.2 2011/05/22 12:57:08 meridian Exp $
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

is41X = True
    
process.p = cms.Path(process.analysisSequence)

## DO NOT CHANGE THE PATH HERE! New modules should be added ONLY in the common configuration 
#  only paramaters should be changes for data and MC
process.source.fileNames = cms.untracked.vstring(
#    'file:/cmsrm/pc24_2/meridian/data/GluGluToHToGG_M-115_7TeV-powheg-pythia6_AODSIM.root'
#   '/store/mc/Spring11/DiPhotonJets_7TeV-madgraph/AODSIM/PU_S1_START311_V1G1-v1/0000/0013CD5B-186F-E011-A93E-00A0D1EEF4F8.root'
#'file:MC-41xv10/90E5F7D6-1E56-E011-9D93-001D096D8916_DESY.root'
#'/store/mc/Summer11/GluGluToHToGG_M-115_7TeV-powheg-pythia6/AODSIM/PU_S3_START42_V11-v2/0000/842C8A8B-967F-E011-90C4-E0CB4E55363A.root'
'file:/cmshome/rahatlou/414p4-Hgg/src/JetMETCorrections/PaoloGammaJet/test/MC-41xv10/90E5F7D6-1E56-E011-9D93-001D096D8916_DESY.root'
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
)

# Global tag#
# Remember to change it and adapt to your needs #
process.GlobalTag.globaltag = cms.string('START311_V2::All')

##  apply only L2 and L3 jet corrections in MC
process.metMuonJESCorAK5.corrector = "ak5CaloL2L3"

process.myanalysis.JetCorrectionService_akt5 = cms.string('ak5CaloL1L2L3')
process.myanalysis.JetCorrectionService_akt7 = cms.string('ak7CaloL2L3')
process.myanalysis.JetCorrectionService_jptak5 = cms.string('ak5JPTL1L2L3')
process.myanalysis.JetCorrectionService_jptak7 = cms.string('ak7JPTL2L3')
process.myanalysis.JetCorrectionService_pfakt5 = cms.string('ak5PFL1FastL2L3')
process.myanalysis.JetCorrectionService_pfakt7 = cms.string('ak7PFL2L3')
process.myanalysis.TriggerTag = cms.untracked.InputTag("TriggerResults::REDIGI311X")

#print process.dumpPython()
