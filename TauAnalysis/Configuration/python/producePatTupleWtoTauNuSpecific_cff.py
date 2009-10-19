import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce all objects specific to W --> tau-jet + nu channel
# which do **not** get stored in PATTuple
#--------------------------------------------------------------------------------
#produce pat::tau collections
from TauAnalysis.RecoTools.patLeptonSelection_cff import *

#produce pat::jet collections
from TauAnalysis.RecoTools.patJetSelectionForWTauNu_cff import *
#produce collections for pat::(calo/PF)MET objects
from TauAnalysis.RecoTools.patMetSelection_cff import *
#produce collections for tau+met pairs
from TauAnalysis.CandidateTools.tauNuPairProduction_cff import *
#produce collections for tau recoil energy
from TauAnalysis.RecoTools.tauRecoilEnergy_cff import *

producePatTupleWtoTauNuSpecific = cms.Sequence(
    selectLayer1TausForWTauNu+
    selectLayer1TausForWTauNuLooseIsolation+
    selectLayer1JetsForWTauNu+
    selectLayer1METs+
    produceTauNuPairs+
    produceTauRecoilEnergy
)
