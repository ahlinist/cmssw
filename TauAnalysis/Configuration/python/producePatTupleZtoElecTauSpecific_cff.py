import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce all objects specific to Z --> electron + tau-jet channel
# which do **not** get stored in PATTuple
#--------------------------------------------------------------------------------

#
# produce collections of pat::Electrons, pat::Muons and
# pat::(PF)Taus passing different selection criteria
#
from TauAnalysis.RecoTools.patLeptonSelection_cff import *
#
# produce collection of pat::Jets passing Et threshold and
# Eta acceptance cuts and not overlapping with any object
# passing selection criteria for pat::Electron, pat::Muon or pat::(PF)Tau
# (pat::Jet collection to be considered for central-jet veto)
#
from TauAnalysis.RecoTools.patJetSelection_cff import *
ak5PFJetsAntiOverlapWithLeptonsVeto = cms.EDFilter("PFJetAntiOverlapSelector",
    src = cms.InputTag('ak5PFJets'),
    srcNotToBeFiltered = cms.VInputTag(
        "selectedPatElectronsTrkIPcumulative",
        "selectedPatMuonsTrkIPcumulative",
        "selectedPatTausProngCumulative"
    ),
    dRmin = cms.double(0.5),
    filter = cms.bool(False)
)

pfUnclusteredCandidates = cms.EDFilter("PFCandidateAntiOverlapSelector",
    src = cms.InputTag('particleFlow'),
    srcNotToBeFiltered = cms.VInputTag(
        "selectedPatElectronsTrkIPcumulative",
        "selectedPatMuonsTrkIPcumulative",
        "selectedPatTausProngCumulative",
        "ak5PFJetsAntiOverlapWithLeptonsVeto"
    ),
    dRmin = cms.double(0.5),
    filter = cms.bool(False)
)

from JetMETCorrections.Type1MET.MetType1Corrections_cff import metJESCorAK5PFJet
from JetMETCorrections.Configuration.JetCorrectionServices_cff import *
pfMEtType1and2corrected = metJESCorAK5PFJet.clone(
    inputUncorJetsLabel = cms.string('ak5PFJetsAntiOverlapWithLeptonsVeto'),
    metType = cms.string("PFMET"),
    inputUncorMetLabel = cms.string('pfMet'),
    jetPTthreshold = cms.double(10.0),
    useTypeII = cms.bool(True),
    UscaleA = cms.double(1.5),
    UscaleB = cms.double(0.0),
    UscaleC = cms.double(0.0),
    inputUncorUnlusteredLabel = cms.untracked.InputTag('pfUnclusteredCandidates'),
    corrector = cms.string('ak5PFL2L3')
)

from PhysicsTools.PatAlgos.producersLayer1.metProducer_cfi import *
patPFtype1METs = patMETs.clone(
    metSource = cms.InputTag('pfMEtType1and2corrected'),
    addMuonCorrections = cms.bool(False),
    genMETSource = cms.InputTag('genMetTrue')
)
#
# produce collections of pat::(Calo)MET objects
# passing different selection criteria
#
from TauAnalysis.RecoTools.patMetSelection_cff import *
#
# produce collections of electron + tau-jet pairs
# passing different selection criteria
#
from TauAnalysis.CandidateTools.elecTauPairProduction_cff import *
from TauAnalysis.CandidateTools.diTauPairSelectionAllKinds_cff import *
#
# produce collections of event vertices matching selected muon + tau-jet pairs
#
from TauAnalysis.RecoTools.recoVertexSelectionForElecTau_cff import *

#
# produce data-formats providing information about compatibility
# of an electron + tau-jet pair with the hypothesis
# of being an electron pair resulting from a Z --> e+ e- decay
#
from TauAnalysis.RecoTools.elecTauPairZeeHypothesis_cff import *
#
# produce combinations of electron + electron pairs,
# the hypothesis being that the pair of electrons results from a Z --> e+ e- decay
#
from TauAnalysis.RecoTools.diElecPairZeeHypothesis_cff import *
#
# produce collection of primary event vertex candidates
# selected by requiring sum(trackPt) > 10 GeV
#
from TauAnalysis.RecoTools.vertexMultiplicityReweight_cfi import selectedPrimaryVerticesTrackPtSumGt10

producePatTupleZtoElecTauSpecific = cms.Sequence(
    #selectedPrimaryVerticesTrackPtSumGt10 +
	selectPatElectrons +
	selectPatElectronsLooseIsolation +
	selectPatElectronsForElecTau + selectPatElectronsForElecTauLooseIsolation +
	selectPatTaus +
	selectPatTausForElecTau +
    selectPrimaryVertexForElecTauAll + 
	selectPatMuons +
	selectPatJets +
    ak5PFJetsAntiOverlapWithLeptonsVeto + pfUnclusteredCandidates + pfMEtType1and2corrected + patPFtype1METs +
	produceElecTauPairsAll+
	selectElecTauPairs + selectElecTauPairsLooseElectronIsolation +
	produceElecTauPairZeeHypotheses +
	produceDiElecPairs
)
