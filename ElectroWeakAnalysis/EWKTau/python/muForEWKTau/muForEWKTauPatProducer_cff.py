from ElectroWeakAnalysis.EWKTau.muForEWKTau.muForEWKTauIso_cff import *
from ElectroWeakAnalysis.EWKTau.muForEWKTau.muForEWKTauPatConfig_cfi import *
from ElectroWeakAnalysis.EWKTau.muForEWKTau.muForEWKTauPatSelector_cfi import *

from PhysicsTools.PatAlgos.triggerLayer0.trigMatchSequences_cff import *
muonTrigMatchHLT1MuonIso.src = "allLayer0MuForEWKTau"
muonTrigMatchHLT1MuonNonIso.src = "allLayer0MuForEWKTau"
patTrigMatchMuForEWKTau = cms.Sequence( (patHLT1MuonIso * muonTrigMatchHLT1MuonIso)
	                               +(patHLT1MuonNonIso * muonTrigMatchHLT1MuonNonIso) )

muonMatch.src = "allLayer0MuForEWKTau"

layer0MuForEWKTau = cms.Sequence(patAODMuonIsolation + allLayer0MuForEWKTau + patLayer0MuonIsolation + muonMatch + patTrigMatchMuForEWKTau)
layer1MuForEWKTau = cms.Sequence(allLayer1MuForEWKTau)

selectMuForEWKTau = cms.Sequence( selectKinMuForEWKTau
                                 *selectMchHLTMuForEWKTau
                                 *selectTrkIsoMuForEWKTau
                                 *selectEcalIsoMuForEWKTau
                                 *selectHcalIsoMuForEWKTau
                                 *selectIDIsoMuForEWKTau )

allLayer1MuSelForEWKTau = cms.EDProducer("PATMuonSelProducer",

  leptonSource = cms.InputTag("allLayer1MuForEWKTau"),

  selFlags = cms.PSet(
    selKineMuForEWKTau = cms.PSet(
      src = cms.InputTag('selectKinMuForEWKTau')
    ),
    selHLTmatchMuForEWKTau = cms.PSet(
      src = cms.InputTag('selectMchHLTMuForEWKTau')
    ),
    selTrkIsoMuForEWKTau = cms.PSet(
      src = cms.InputTag('selectTrkIsoMuForEWKTau')
    ),
    selEcalIsoMuForEWKTau = cms.PSet(
      src = cms.InputTag('selectEcalIsoMuForEWKTau')
    ),
    selHcalIsoMuForEWKTau = cms.PSet(
      src = cms.InputTag('selectHcalIsoMuForEWKTau')
    ),
    selIdMuForEWKTau = cms.PSet(
      src = cms.InputTag('selectIDIsoMuForEWKTau')
    )
    #selTrkIpMuForEWKTau = cms.PSet(
    #  src = cms.InputTag('')
    #)
  )
)

produceMuTau = cms.Sequence(layer0MuForEWKTau + layer1MuForEWKTau + selectMuForEWKTau + allLayer1MuSelForEWKTau)
