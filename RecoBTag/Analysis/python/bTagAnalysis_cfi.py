import FWCore.ParameterSet.Config as cms

# BTagPerformanceAnalyzer configuration
#includes added because of block refactoring replacing simple includes by using statements
from RecoBTag.Analysis.bTagTrackIPAnalysis_cff import *
from RecoBTag.Analysis.bTagCombinedSVAnalysis_cff import *
from RecoBTag.Analysis.bTagTrackCountingAnalysis_cff import *
from RecoBTag.Analysis.bTagTrackProbabilityAnalysis_cff import *
from RecoBTag.Analysis.bTagTrackBProbabilityAnalysis_cff import *
from RecoBTag.Analysis.bTagGenericAnalysis_cff import *
from RecoBTag.Analysis.bTagSimpleSVAnalysis_cff import *
from RecoBTag.Analysis.bTagSoftLeptonAnalysis_cff import *
from RecoBTag.Analysis.bTagSoftLeptonByPtAnalysis_cff import *
from RecoBTag.Analysis.bTagSoftLeptonByIPAnalysis_cff import *
from RecoBTag.Analysis.bTagCommon_cff import *

bTagAnalysis = cms.EDAnalyzer("BTagPerformanceAnalyzer",
    # parameters common to all tagger algorithms
    bTagCommonBlock,
    
    # output root file
    rootfile = cms.string('jetTagAnalysis.root'),           

    # TagInfos and JetTags to plot
    tagConfig = cms.VPSet(
        cms.PSet(
            bTagTrackIPAnalysisBlock,
            type = cms.string('TrackIP'),
            label = cms.InputTag("impactParameterTagInfos")
        ), 
        cms.PSet(
            bTagCombinedSVAnalysisBlock,
            type = cms.string('GenericMVA'),
            ipTagInfos = cms.InputTag("impactParameterTagInfos"),
            svTagInfos = cms.InputTag("secondaryVertexTagInfos"),
            label = cms.InputTag("combinedSecondaryVertex")
        ), 
        cms.PSet(
            bTagTrackCountingAnalysisBlock,
            label = cms.InputTag("trackCountingHighEffBJetTags")
        ), 
        cms.PSet(
            bTagTrackCountingAnalysisBlock,
            label = cms.InputTag("trackCountingHighPurBJetTags")
        ), 
        cms.PSet(
            bTagProbabilityAnalysisBlock,
            label = cms.InputTag("jetProbabilityBJetTags")
        ), 
        cms.PSet(
            bTagBProbabilityAnalysisBlock,
            label = cms.InputTag("jetBProbabilityBJetTags")
        ), 
        cms.PSet(
            bTagGenericAnalysisBlock,
            label = cms.InputTag("impactParameterMVABJetTags")
        ), 
        cms.PSet(
            bTagSimpleSVAnalysisBlock,
            label = cms.InputTag("simpleSecondaryVertexBJetTags")
        ), 
        cms.PSet(
            bTagGenericAnalysisBlock,
            label = cms.InputTag("combinedSecondaryVertexBJetTags")
        ), 
        cms.PSet(
            bTagGenericAnalysisBlock,
            label = cms.InputTag("combinedSecondaryVertexMVABJetTags")
        ), 
        cms.PSet(
            parameters = cms.PSet(),
            type = cms.string('SoftLepton'),
            label = cms.InputTag("softMuonTagInfos")
        ),
        cms.PSet(
            bTagSoftLeptonAnalysisBlock,
            label = cms.InputTag("softMuonBJetTags")
        ), 
        cms.PSet(
            bTagSoftLeptonAnalysisBlock,
            label = cms.InputTag("softMuonNoIPBJetTags")
        ), 
        cms.PSet(
            bTagSoftLeptonByPtAnalysisBlock,
            label = cms.InputTag("softMuonByPtBJetTags")
        ), 
        cms.PSet(
            bTagSoftLeptonByIPAnalysisBlock,
            label = cms.InputTag("softMuonByIP3dBJetTags")
        ), 
        cms.PSet(
            parameters = cms.PSet(),
            type = cms.string('SoftLepton'),
            label = cms.InputTag("softElectronTagInfos")
        ),
        cms.PSet(
            bTagSoftLeptonAnalysisBlock,
            label = cms.InputTag("softElectronBJetTags")
        )
    )

)
