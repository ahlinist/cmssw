import FWCore.ParameterSet.Config as cms

#from TauAnalysis.Configuration.producePatTupleZtoElecTauSpecific_cff import *

#--------------------------------------------------------------------------------
# produce all objects specific to MSSM Higgs A/H --> electron + tau-jet channel
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
        "selectedPatTausMuonVetoCumulative"
    ),
    dRmin = cms.double(0.5),
    filter = cms.bool(False)
)

pfUnclusteredCandidates = cms.EDFilter("PFCandidateAntiOverlapSelector",
    src = cms.InputTag('particleFlow'),
    srcNotToBeFiltered = cms.VInputTag(
        "selectedPatElectronsTrkIPcumulative",
        "selectedPatMuonsTrkIPcumulative",
        "selectedPatTausMuonVetoCumulative",
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
# passing SM Higgs selection criteria 
#
from TauAnalysis.CandidateTools.elecTauPairProduction_cff import *
from TauAnalysis.CandidateTools.elecTauPairSelection_cfi import *
from TauAnalysis.CandidateTools.vbfEventProductionForAHtoElecTau_cff import *
from TauAnalysis.CandidateTools.vbfEventSelectionForAHtoElecTau_cff import *
# produce collections of electron + tau-jet pairs
# passing MSSM Higgs selection criteria
#
from TauAnalysis.CandidateTools.elecTauPairSelectionForAHtoElecTau_cff import *
#
# produce collections of event vertices matching selected electron + tau-jet pairs
#
from TauAnalysis.RecoTools.recoVertexSelectionForElecTau_cff import *
#
# produce collections of central jets
# passing b-tagging discriminator
#
from TauAnalysis.RecoTools.patJetSelectionForAHtoElecTau_cff import *
#
# produce and select VBF event objects
#from TauAnalysis.CandidateTools.vbfEventProductionForAHtoElecTau_cff import produceVBFEventHypothesesForAHtoElecTau
#from TauAnalysis.CandidateTools.vbfEventSelectionForAHtoElecTau_cff import selectVBFEventHypothesesForAHtoElecTau
#
# produce combinations of electron + electron pairs,
# the hypothesis being that the pair of electrons results from a Z --> e+ e- decay
#
from TauAnalysis.RecoTools.diElecPairZeeHypothesis_cff import *

producePatTupleAHtoElecTauSpecific = cms.Sequence(
     selectPatElectrons 
    + selectPatElectronsForElecTau 
    + selectPatTaus 
    + selectPatTausForElecTau 
    + selectPrimaryVertexForElecTau 
    + selectPatMuons 
    + selectPatJets 
    #  + ak5PFJetsAntiOverlapWithLeptonsVeto + pfUnclusteredCandidates + pfMEtType1and2corrected + patPFtype1METs 
    + produceElecTauPairsAll
    + selectElecTauPairs # SM Higgs selection
	+ selectElecTauPairsForAHtoElecTau # MSSM Higgs selection
	+ selectPatJetsForAHtoElecTau 
    + produceVBFEventHypothesesForAHtoElecTau + selectVBFEventHypothesesForAHtoElecTau
    + produceDiElecPairs
)

