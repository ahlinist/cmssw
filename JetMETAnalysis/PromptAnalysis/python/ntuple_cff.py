import FWCore.ParameterSet.Config as cms

from JetMETAnalysis.PromptAnalysis.theBigNtuple_cfi import *

#Schedule
ntuplize_step = cms.Path( theBigNtuple )
schedule = cms.Schedule( ntuplize_step )
