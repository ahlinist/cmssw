import FWCore.ParameterSet.Config as cms

from JetMETAnalysis.PromptAnalysis.PromptAna_Event_cfi import *
from JetMETAnalysis.PromptAnalysis.PromptAna_MET_cfi import *
from JetMETAnalysis.PromptAnalysis.PromptAna_Jet_cfi import *

promptanaTree = cms.EDAnalyzer("PromptAnaTree",
    outputCommands = cms.untracked.vstring(
    'drop *',
    'keep *_promptanaevent_*_*',
    'keep *_promptanamet_*_*',
    'keep *_promptananohf_*_*',
    'keep *_promptanaic5calojet_*_*',
    'keep *_promptanakt4calojet_*_*'
    ))

theBigNtuple = cms.Sequence( (
    promptanaevent +
    promptanamet   +
    promptananohf  +
    promptanaic5calojet +
    promptanakt4calojet
    ) * promptanaTree )
