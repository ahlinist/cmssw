import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce all objects specific to Z --> muon + tau-jet channel
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
#
# produce collection of (PF)MET objects with type-I jet corrections applied
#
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

#from PhysicsTools.PFCandProducer.pfType1MET_cff import *
#from JetMETCorrections.Configuration.JetCorrectionServices_cff import *
#pfType1MET.inputUncorMetLabel = cms.InputTag('pfRawMET')
#pfType1MET.inputUncorJetsTag = cms.InputTag('ak5PFJetsAntiOverlapWithLeptonsVeto')
#pfType1MET.corrector = cms.string("ak5PFL2L3")

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
    inputUncorJetsLabel = cms.string('ak5PFJets'),
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
# produce collections of muon + tau-jet pairs
# passing different selection criteria
#
from TauAnalysis.CandidateTools.muTauPairProduction_cff import *
from TauAnalysis.CandidateTools.diTauPairSelectionAllKinds_cff import *
#
# produce collections of event vertices matching selected muon + tau-jet pairs
#
from TauAnalysis.RecoTools.recoVertexSelectionForMuTau_cff import *
#
# produce data-formats providing information about compatibility
# of an muon + tau-jet pair with the hypothesis
# of being an muon pair resulting from a Z --> mu+ mu- decay
#
from TauAnalysis.RecoTools.muTauPairZmumuHypothesis_cff import *
#
# produce combinations of muon + muon pairs,
# the hypothesis being that the pair of muons results from a Z/gamma* --> mu+ mu- decay
#
from TauAnalysis.RecoTools.diMuPairZmumuHypothesis_cff import *
from TauAnalysis.RecoTools.diMuPairDYmumuHypothesis_cff import *
#
# produce collection of primary event vertex candidates
# selected by requiring sum(trackPt) > 10 GeV
#
from TauAnalysis.RecoTools.vertexMultiplicityReweight_cfi import selectedPrimaryVerticesTrackPtSumGt10

producePatTupleZtoMuTauSpecific = cms.Sequence(
    selectedPrimaryVerticesTrackPtSumGt10
   + patMuonsMuScleFitCorrectedMomentum + selectPatMuons + selectPatMuonsLooseIsolation
   + selectPatElectrons
   + selectPatTaus + selectPatTausForMuTau
   + selectPatJets
   + ak5PFJetsAntiOverlapWithLeptonsVeto + pfUnclusteredCandidates + pfMEtType1and2corrected + patPFtype1METs
   + produceMuTauPairsAll
   + selectMuTauPairs + selectMuTauPairsLooseMuonIsolation
   + selectPrimaryVertexForMuTauAll
   + produceMuTauPairZmumuHypotheses
   + produceDiMuPairsZmumuHypotheses + produceDiMuPairsDYmumuHypotheses
)
