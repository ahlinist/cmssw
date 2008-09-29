import FWCore.ParameterSet.Config as cms
import copy

from ElectroWeakAnalysis.EWKTau.tauForETauMod.tauForETauPFRecoTauProducer_cff import *
from ElectroWeakAnalysis.EWKTau.tauForETauMod.tauForETauPatConfig_cfi import *

PFTauForETauPat = cms.Sequence(
                                PFTauForETau

                               *ldgTrkDiscrForETauKin
                               *allLayer0TausForETauKin
                               *layer1TausForETauKin

                               *ldgTrkDiscrForETauLdgTrk
                               *allLayer0TausForETauLdgTrk
                               *layer1TausForETauLdgTrk

                               *ldgTrkDiscrForETauLdgTrkCut
                               *allLayer0TausForETauLdgTrkPt
                               *layer1TausForETauLdgTrkPt

                               *pfRecoTauTrkIsoDiscrForETau
                               *allLayer0TausForETauTrkIsod
                               *layer1TausForETauTrkIsod

                               *pfRecoTauEclIsoDiscrForETau
                               *allLayer0TausForETauEclIsod
                               *layer1TausForETauEclIsod

                               *pfRecoTauElecRejDiscrForETau
                               *allLayer0TausForETauElecRej
                               *layer1TausForETauElecRej
                              
                               *select1Or3SigTrackTausForETau
                               *select1Or3SigChargedHadTausForETau

                              )
PFTauForETauEffPat = cms.Sequence(

                                PFTauForETauEff

                               *ldgTrkDiscrForETauKinEff
                               *allLayer0TausForETauKinEff 
                               *layer1TausForETauKinEff

                               *ldgTrkDiscrForETauLdgTrkEff
                               *allLayer0TausForETauLdgTrkEff
                               *layer1TausForETauLdgTrkEff

                               *ldgTrkDiscrForETauLdgTrkCut
                               *allLayer0TausForETauLdgTrkPtEff
                               *layer1TausForETauLdgTrkPtEff

                               *pfRecoTauTrkIsoDiscrForETauEff 
                               *allLayer0TausForETauTrkIsodEff
                               *layer1TausForETauTrkIsodEff

                               *pfRecoTauEclIsoDiscrForETauEff 
                               *allLayer0TausForETauEclIsodEff
                               *layer1TausForETauEclIsodEff

                               *pfRecoTauElecRejDiscrForETauEff
                               *allLayer0TausForETauElecRejEff
                               *layer1TausForETauElecRejEff

                               *select1Or3SigTrackTausForETauEff
                               *select1Or3SigChargedHadTausForETauEff
)






