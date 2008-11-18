import FWCore.ParameterSet.Config as cms

# import config for muon analyzer
from ElectroWeakAnalysis.EWKTau.muForEWKTau.muForEWKTauAnalyzer_cfi import *

# import config for muon efficiency histogram producer
from ElectroWeakAnalysis.EWKTau.muForEWKTau.muForEWKTauHistEff_cfi import *

# import config for tau analyzer
#from ElectroWeakAnalysis.EWKTau.tauForEWKTau.tauForEWKTauAnalyzer_cfi import *

saveMuTau = cms.EDAnalyzer("DQMFileAdder",
  outputFileName = cms.string('ewkTauMuAnalyzer_zMuMu.root')
)

analyzeMuTau = cms.Sequence(analyzeMuon * compEffMuon * saveMuTau)
