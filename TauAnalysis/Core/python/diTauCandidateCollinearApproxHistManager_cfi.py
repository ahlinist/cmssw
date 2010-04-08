import FWCore.ParameterSet.Config as cms

from TauAnalysis.Core.diTauCandidateHistManager_cfi import *
from TauAnalysis.Core.caloMEtHistManager_cfi import *
from TauAnalysis.Core.pfMEtHistManager_cfi import *

collinearApproxBinning = cms.PSet(
    name = cms.string("collinearApproxBinning"),
    config = cms.VPSet(
        cms.PSet(
            extractor = cms.PSet(
                pluginType = cms.string("PATMuTauPairValExtractor"),
                src = cms.InputTag('selectedMuTauPairsPzetaDiffCumulative'),
                value = cms.string("dPhi12")
            ),
            branchName = cms.string('dPhi12'),
            binning = cms.PSet(
                boundaries = cms.vdouble(2.7925),
                min = cms.double(-0.01),
                max = cms.double(3.15)
            )
        ),
        cms.PSet(
            extractor = cms.PSet(
                pluginType = cms.string("PATMuTauPairValExtractor"),
                src = cms.InputTag('selectedMuTauPairsPzetaDiffCumulative'),
                value = cms.string("collinearApproxIsValid")
            ),
            branchName = cms.string('collinearApproxIsValid'),
            binning = cms.PSet(
                boundaries = cms.vdouble(+0.5),
                min = cms.double(-0.01),
                max = cms.double(+1.01)
            )
        )
    )
)

diTauCandidateCollinearApproxHistManager = cms.PSet(    
    pluginName = cms.string('diTauCandidateCollinearApproxHistManager'),
    pluginType = cms.string('DiCandidatePairCollinearApproxHistManager'),
      
    diTauCandidateSource = cms.InputTag('selectedMuTauPairsPzetaDiffCumulative'),

    genParticleSource = cms.InputTag('genParticles'),
    genJetSource = cms.InputTag('iterativeCone5GenJets'),
    genMEtSource = cms.InputTag('genMetTrue'),
    recoJetSource = cms.InputTag('iterativeCone5PFJets'),

    collinearApproxMassCompatibilities = cms.vstring(
        "mZ",
        "mAH120"
    ),

    dqmDirectory_store = cms.string('DiTauCandidateCollinearApproxQuantities'),

    #requireGenMatch = cms.bool(True),
    requireGenMatch = cms.bool(False),

    #normalization = cms.string("diTauCandidates"),
    normalization = cms.string("events"),

    makeCollinearApproxX1X2histograms = cms.bool(True),
    makeCollinearApproxMassDepHistograms = cms.bool(True)
)

diTauCandidateCollinearApproxHistManagerBinned = cms.PSet(
    pluginName = cms.string('diTauCandidateCollinearApproxHistManagerBinned'),
    pluginType = cms.string('BinGridHistManager'),
    binning = collinearApproxBinning,
    histManagers = cms.VPSet(),
    dqmDirectory_store = cms.string('collinearApproxHistograms4regions')
)
