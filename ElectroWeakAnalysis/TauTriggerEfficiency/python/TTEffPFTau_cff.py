import FWCore.ParameterSet.Config as cms
import copy

from RecoJets.JetAssociationProducers.ic5PFJetTracksAssociatorAtVertex_cfi import *
ak5PFJetTracksAssociatorAtVertex = ic5PFJetTracksAssociatorAtVertex.clone()
ak5PFJetTracksAssociatorAtVertex.jets = cms.InputTag("ak5PFJets")
from RecoTauTag.RecoTau.PFRecoTauTagInfoProducer_cfi import *
myPFTauTagInfoProducer = copy.deepcopy(pfRecoTauTagInfoProducer)
myPFTauTagInfoProducer.tkminPt = cms.double(0.5)
myPFTauTagInfoProducer.ChargedHadrCand_tkminPt = cms.double(0.5)
myPFTauTagInfoProducer.PFJetTracksAssociatorProducer = cms.InputTag("ak5PFJetTracksAssociatorAtVertex")

from RecoTauTag.Configuration.FixedConePFTaus_cfi import *
myConeProducer = copy.deepcopy(fixedConePFTauProducer)
myConeProducer.PFTauTagInfoProducer = cms.InputTag("myPFTauTagInfoProducer")

from RecoTauTag.RecoTau.PFRecoTauDiscriminationByLeadingPionPtCut_cfi import *
from RecoTauTag.RecoTau.TauDiscriminatorTools import noPrediscriminants
thisPFTauDiscriminationByLeadingPionPtCut = cms.EDProducer("PFRecoTauDiscriminationByLeadingObjectPtCut",

    # Tau collection to discriminate
    PFTauProducer = cms.InputTag('myConeProducer'),

    # no pre-reqs for this cut
    Prediscriminants = noPrediscriminants,

    # Allow either charged or neutral PFCandidates to meet this requirement
    UseOnlyChargedHadrons = cms.bool(False),

    MinPtLeadingObject = cms.double(3.0)
)

PFTausSelected = cms.EDFilter("PFTauSelector",
    src = cms.InputTag("myConeProducer"),
    discriminators = cms.VPSet(
        cms.PSet(
          discriminator=cms.InputTag("thisPFTauDiscriminationByLeadingPionPtCut"),
          selectionCut=cms.double(0.5)
        )
    )
)

from RecoTauTag.RecoTau.PFRecoTauDiscriminationByLeadingTrackFinding_cfi import *
thisPFTauDiscriminationByLeadingTrackFinding = copy.deepcopy(pfRecoTauDiscriminationByLeadingTrackFinding)
thisPFTauDiscriminationByLeadingTrackFinding.PFTauProducer = 'PFTausSelected'

from RecoTauTag.RecoTau.PFRecoTauDiscriminationByIsolationUsingLeadingPion_cfi import *
thisPFTauDiscriminationByIsolation = copy.deepcopy(pfRecoTauDiscriminationByIsolationUsingLeadingPion)
thisPFTauDiscriminationByIsolation.PFTauProducer = 'PFTausSelected'
thisPFTauDiscriminationByIsolation.MinPtLeadingPion = cms.double(3.0)
thisPFTauDiscriminationByIsolation.Prediscriminants.leadPion.Producer = cms.InputTag('thisPFTauDiscriminationByLeadingTrackFinding')

#copying the Discriminator against Muon
from RecoTauTag.RecoTau.PFRecoTauDiscriminationAgainstMuon_cfi import *
thisPFTauDiscriminationAgainstMuon = copy.deepcopy(pfRecoTauDiscriminationAgainstMuon)
thisPFTauDiscriminationAgainstMuon.PFTauProducer = 'PFTausSelected'
thisPFTauDiscriminationAgainstMuon.Prediscriminants.leadPion.Producer = cms.InputTag('thisPFTauDiscriminationByLeadingTrackFinding')

TTEffPFTau = cms.Sequence(
        ak5PFJetTracksAssociatorAtVertex *
        myPFTauTagInfoProducer *
        myConeProducer *
        thisPFTauDiscriminationByLeadingPionPtCut *
        PFTausSelected *
        thisPFTauDiscriminationByLeadingTrackFinding *
        thisPFTauDiscriminationByIsolation *
        thisPFTauDiscriminationAgainstMuon
)
