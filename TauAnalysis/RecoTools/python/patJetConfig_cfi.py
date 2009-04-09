import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.cleaningLayer1.jetCleaner_cfi import *

#--------------------------------------------------------------------------------  
# PAT layer 1 jet configuration parameters
#--------------------------------------------------------------------------------

# do not remove jets overlapping with (loosely selected) electrons
# (instead, leave overlap removal to the subsequent selector stage)
cleanLayer1Jets.checkOverlaps = cms.PSet()
