from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *
from ElectroWeakAnalysis.EWKTau.muForEWKTau.muForEWKTauPatConfig_cfi import *
from ElectroWeakAnalysis.EWKTau.muForEWKTau.muForEWKTauPatSelector_cfi import *
from ElectroWeakAnalysis.EWKTau.muForEWKTau.muForEWKTauIso_cff import *
from PhysicsTools.PatAlgos.mcMatchLayer0.mcMatchSequences_cff import *
from PhysicsTools.PatAlgos.triggerLayer0.trigMatchSequences_cff import *
muonTrigMatchHLT1MuonIso.src = "allLayer0MuForEWKTau"
muonTrigMatchHLT1MuonNonIso.src = "allLayer0MuForEWKTau"

patTrigMatchMuForEWKTau = cms.Sequence(
                                        (patHLT1MuonIso * muonTrigMatchHLT1MuonIso)
	                               +(patHLT1MuonNonIso * muonTrigMatchHLT1MuonNonIso)
                                       
                                      )

patHighLevelRecoMuForEWKTau = cms.Sequence( patAODMuonIsolation * patLayer0MuonIsolation)
layer0MuForEWKTau = cms.Sequence(allLayer0MuForEWKTau * patTrigMatchMuForEWKTau * patHighLevelRecoMuForEWKTau)
layer1MuForEWKTau = cms.Sequence(allLayer1MuForEWKTau * selectKinMuForEWKTau)


