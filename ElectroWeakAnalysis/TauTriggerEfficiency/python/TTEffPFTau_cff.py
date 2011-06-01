import FWCore.ParameterSet.Config as cms
import copy

from RecoTauTag.RecoTau.RecoTauPiZeroProducer_cfi import ak5PFJetsRecoTauPiZeros
#TTEffak5PFJetsRecoTauPiZeros = ak5PFJetsRecoTauPiZeros.clone()

from RecoTauTag.Configuration.RecoPFTauTag_cff import *

from RecoJets.JetAssociationProducers.ic5PFJetTracksAssociatorAtVertex_cfi import ic5PFJetTracksAssociatorAtVertex
TTEffak5PFJetTracksAssociatorAtVertex = ic5PFJetTracksAssociatorAtVertex.clone()
TTEffak5PFJetTracksAssociatorAtVertex.jets = cms.InputTag("ak5PFJets")
from RecoTauTag.RecoTau.PFRecoTauTagInfoProducer_cfi import pfRecoTauTagInfoProducer
TTEffPFTauTagInfoProducer = copy.deepcopy(pfRecoTauTagInfoProducer)
TTEffPFTauTagInfoProducer.tkminPt = cms.double(0.5)
TTEffPFTauTagInfoProducer.ChargedHadrCand_tkminPt = cms.double(0.5)
TTEffPFTauTagInfoProducer.PFJetTracksAssociatorProducer = cms.InputTag("TTEffak5PFJetTracksAssociatorAtVertex")

#from RecoTauTag.Configuration.FixedConePFTaus_cff import fixedConePFTauProducer
#TTEffFixedConePFTauProducer = copy.deepcopy(fixedConePFTauProducer)
#TTEffFixedConePFTauProducer.PFTauTagInfoProducer = cms.InputTag("TTEffPFTauTagInfoProducer")

from RecoTauTag.Configuration.ShrinkingConePFTaus_cff import shrinkingConePFTauProducer
TTEffShrinkingConePFTauProducer = copy.deepcopy(shrinkingConePFTauProducer)
TTEffShrinkingConePFTauProducer.PFTauTagInfoProducer = cms.InputTag("TTEffPFTauTagInfoProducer")

from RecoTauTag.RecoTau.PFRecoTauDiscriminationByLeadingPionPtCut_cfi import *
from RecoTauTag.RecoTau.TauDiscriminatorTools import noPrediscriminants
TTEffPFTauDiscriminationByLeadingPionPtCut = cms.EDProducer("PFRecoTauDiscriminationByLeadingObjectPtCut",

    # Tau collection to discriminate
    #PFTauProducer = cms.InputTag('TTEffFixedConePFTauProducer'),
    PFTauProducer = cms.InputTag('TTEffShrinkingConePFTauProducer'),

    # no pre-reqs for this cut
    Prediscriminants = noPrediscriminants,

    # Allow either charged or neutral PFCandidates to meet this requirement
    UseOnlyChargedHadrons = cms.bool(False),

    MinPtLeadingObject = cms.double(3.0)
)

TTEffPFTausSelected = cms.EDFilter("PFTauSelector",
    #src = cms.InputTag("TTEffFixedConePFTauProducer"),
    src = cms.InputTag("TTEffShrinkingConePFTauProducer"),
    discriminators = cms.VPSet(
        cms.PSet(
          discriminator=cms.InputTag("TTEffPFTauDiscriminationByLeadingPionPtCut"),
          selectionCut=cms.double(0.5)
        )
    ),
    cut = cms.string('et > 15. && abs(eta) < 2.5')
)


from RecoTauTag.RecoTau.PFRecoTauDiscriminationByLeadingTrackFinding_cfi import pfRecoTauDiscriminationByLeadingTrackFinding
TTEffPFTauDiscriminationByLeadingTrackFinding = copy.deepcopy(pfRecoTauDiscriminationByLeadingTrackFinding)
TTEffPFTauDiscriminationByLeadingTrackFinding.PFTauProducer = cms.InputTag('TTEffPFTausSelected')

from RecoTauTag.RecoTau.PFRecoTauDiscriminationByIsolationUsingLeadingPion_cfi import pfRecoTauDiscriminationByIsolationUsingLeadingPion
TTEffPFTauDiscriminationByIsolation = copy.deepcopy(pfRecoTauDiscriminationByIsolationUsingLeadingPion)
TTEffPFTauDiscriminationByIsolation.PFTauProducer = cms.InputTag('TTEffPFTausSelected')
TTEffPFTauDiscriminationByIsolation.MinPtLeadingPion = cms.double(3.0)
TTEffPFTauDiscriminationByIsolation.Prediscriminants.leadPion.Producer = cms.InputTag('TTEffPFTauDiscriminationByLeadingTrackFinding')

#copying the Discriminator against Muon
from RecoTauTag.RecoTau.PFRecoTauDiscriminationAgainstMuon_cfi import pfRecoTauDiscriminationAgainstMuon
TTEffPFTauDiscriminationAgainstMuon = copy.deepcopy(pfRecoTauDiscriminationAgainstMuon)
TTEffPFTauDiscriminationAgainstMuon.PFTauProducer = cms.InputTag('TTEffPFTausSelected')
TTEffPFTauDiscriminationAgainstMuon.Prediscriminants.leadTrack.Producer = cms.InputTag('TTEffPFTauDiscriminationByLeadingTrackFinding')

#copying the Discriminator against Electron
from RecoTauTag.RecoTau.PFRecoTauDiscriminationAgainstElectron_cfi import pfRecoTauDiscriminationAgainstElectron
TTEffPFTauDiscriminationAgainstElectron = copy.deepcopy(pfRecoTauDiscriminationAgainstElectron)
TTEffPFTauDiscriminationAgainstElectron.PFTauProducer = 'TTEffPFTausSelected'
TTEffPFTauDiscriminationAgainstElectron.Prediscriminants.leadTrack.Producer = cms.InputTag('TTEffPFTauDiscriminationByLeadingTrackFinding')

TTEffPFTau = cms.Sequence(
	PFTau *
        TTEffak5PFJetTracksAssociatorAtVertex *
        TTEffPFTauTagInfoProducer *
#	recoTauCommonSequence *
	ak5PFJetsRecoTauPiZeros *
#        TTEffFixedConePFTauProducer *
        TTEffShrinkingConePFTauProducer *
        TTEffPFTauDiscriminationByLeadingPionPtCut *
        TTEffPFTausSelected *
        TTEffPFTauDiscriminationByLeadingTrackFinding *
        TTEffPFTauDiscriminationByIsolation *
        TTEffPFTauDiscriminationAgainstMuon *
	TTEffPFTauDiscriminationAgainstElectron
)

