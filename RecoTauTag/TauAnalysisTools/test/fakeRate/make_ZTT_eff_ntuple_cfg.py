import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       '/store/relval/CMSSW_3_3_1/RelValQCD_FlatPt_15_3000/GEN-SIM-RECO/MC_31X_V9-v1/0002/4CDB3CFA-E7BF-DE11-89B7-0030487C6062.root',
       '/store/relval/CMSSW_3_3_1/RelValQCD_FlatPt_15_3000/GEN-SIM-RECO/MC_31X_V9-v1/0001/FAC01BC7-4EBF-DE11-B398-000423D98EA8.root',
       '/store/relval/CMSSW_3_3_1/RelValQCD_FlatPt_15_3000/GEN-SIM-RECO/MC_31X_V9-v1/0001/D61D9798-50BF-DE11-A345-000423D6BA18.root',
       '/store/relval/CMSSW_3_3_1/RelValQCD_FlatPt_15_3000/GEN-SIM-RECO/MC_31X_V9-v1/0001/9E1FEBD3-4CBF-DE11-A846-003048D3756A.root',
       '/store/relval/CMSSW_3_3_1/RelValQCD_FlatPt_15_3000/GEN-SIM-RECO/MC_31X_V9-v1/0001/90ECAD91-58BF-DE11-9864-000423D944F8.root',
       '/store/relval/CMSSW_3_3_1/RelValQCD_FlatPt_15_3000/GEN-SIM-RECO/MC_31X_V9-v1/0001/5687CDB7-4DBF-DE11-902B-0030487D1BCC.root',
       '/store/relval/CMSSW_3_3_1/RelValQCD_FlatPt_15_3000/GEN-SIM-RECO/MC_31X_V9-v1/0001/3E8D924E-4FBF-DE11-9094-0019DB29C614.root'
       )
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load("RecoTauTag.TauAnalysisTools.TauNtupleProducer_cfi")
from RecoTauTag.TauAnalysisTools.tools.ntupleTools import *

makeTauNtuple(process, 
      input_collection = 'shrinkingConePFTauProducer',
      tauType = 'shrinkingConePFTau',
      expressions = cms.PSet(common_expressions, pftau_expressions),
      discriminators = cms.PSet(pftau_discriminators, pftau_discriminators_extra),
      matchingOption = "matched",
      matchingCollection = "trueHadronicTaus")

process.path = cms.Path(process.buildTauNtuple)

process.out = cms.OutputModule("PoolOutputModule",                                                                                                                                                        
      outputCommands = cms.untracked.vstring("drop *", "keep *_*NtupleProducer_*_*" ),
      verbose = cms.untracked.bool(False),
      fileName = cms.untracked.string("ztt_ntuple.root")      
      )

# Get trigger report
process.options = cms.untracked.PSet(
        wantSummary = cms.untracked.bool(True)
        )

process.endpath = cms.EndPath(process.out)
