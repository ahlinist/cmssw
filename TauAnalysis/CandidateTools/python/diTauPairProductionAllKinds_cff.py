import FWCore.ParameterSet.Config as cms

from TauAnalysis.CandidateTools.elecMuPairProduction_cff import *
from TauAnalysis.CandidateTools.elecTauPairProduction_cff import *
from TauAnalysis.CandidateTools.muTauPairProduction_cff import *
from TauAnalysis.CandidateTools.diTauPairProduction_cff import *

produceDiTauPairsAllKinds = cms.Sequence( produceElecMuPairsAll
                                         +produceElecTauPairsAll
                                         +produceMuTauPairsAll
                                         +produceDiTauPairs )
