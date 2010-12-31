import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce all objects specific to W --> tau-jet + nu channel
# which do **not** get stored in PATTuple
#--------------------------------------------------------------------------------
#produce pat::tau collections
from TauAnalysis.RecoTools.patLeptonSelection_cff import *

#produce pat::jet collections (jets not overlapping with selected W-->tau nu analysis tuas)
from TauAnalysis.RecoTools.patJetSelection_cff import *
from TauAnalysis.RecoTools.patJetSelectionForWTauNu_cff import *
#produce collections for pat::caloMET objects
from TauAnalysis.RecoTools.patMetSelection_cff import *
#produce collections for pat::PFMET objects
from TauAnalysis.RecoTools.patPFMetSelection_cff import *
#produce collections for tau+met pairs
from TauAnalysis.CandidateTools.tauNuPairProduction_cff import *
from TauAnalysis.CandidateTools.tauNuPairSelection_cfi import *
#produce collections for tau recoil energy
from TauAnalysis.RecoTools.tauRecoilEnergy_cff import *
#produce collections for met topology
from TauAnalysis.CandidateTools.metTopology_cfi import *


producePatTupleWtoTauNuSpecific = cms.Sequence(
    selectPatTausForWTauNu
    +selectPatTausForWTauNuLooseIsolation
    +selectPatSelJetsForWTauNu
    +selectPatMETs
    +selectPatPFMETs
    +produceTauNuPairs
    +selectTauNuPairs
    +selectTauNuPairsLooseIsolation
    +produceTauRecoilEnergy
    +selectMEtTopology
    +patMuonsMuScleFitCorrectedMomentum 
    +selectPatMuons
    +selectPatElectrons    
    +selectPatTaus
    +selectPatJets
)
