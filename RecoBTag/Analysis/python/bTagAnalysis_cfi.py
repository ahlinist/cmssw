import FWCore.ParameterSet.Config as cms

# BTagPerformanceAnalyzer configuration
from RecoBTag.Analysis.bTagCombinedSVVariables_cff import *
bTagAnalysis = cms.EDAnalyzer("BTagPerformanceAnalyzer",
    # output root file
    rootfile = cms.string('jetTagAnalysis.root'),
    tagConfig = cms.VPSet(cms.PSet(
        type = cms.string('TrackIP'),
        parameters = cms.PSet(
            endEffPur = cms.double(1.005),
            nBinEffPur = cms.int32(200),
            startEffPur = cms.double(0.005)
        ),
        label = cms.InputTag("impactParameterTagInfos")
    ), cms.PSet(
        ipTagInfos = cms.InputTag("impactParameterTagInfos"),
        type = cms.string('GenericMVA'),
        parameters = cms.PSet(
            categoryVariable = cms.string('vertexCategory'),
            categories = cms.VPSet(cms.PSet(
                combinedSVNoVertexVariables,
                combinedSVPseudoVertexVariables,
                combinedSVRecoVertexVariables
            ), cms.PSet(
                combinedSVNoVertexVariables,
                combinedSVPseudoVertexVariables,
                combinedSVRecoVertexVariables
            ), cms.PSet(
                combinedSVNoVertexVariables,
                combinedSVPseudoVertexVariables
            ), cms.PSet(
                combinedSVNoVertexVariables
            ))
        ),
        svTagInfos = cms.InputTag("secondaryVertexTagInfos"),
        label = cms.InputTag("combinedSecondaryVertex")
    ), cms.PSet(
        parameters = cms.PSet(
            discriminatorStart = cms.double(-10.0),
            nBinEffPur = cms.int32(200),
            effBConst = cms.double(0.5),
            endEffPur = cms.double(1.005),
            discriminatorEnd = cms.double(30.0),
            startEffPur = cms.double(0.005)
        ),
        label = cms.InputTag("trackCountingHighEffBJetTags")
    ), cms.PSet(
        parameters = cms.PSet(
            discriminatorStart = cms.double(-10.0),
            nBinEffPur = cms.int32(200),
            effBConst = cms.double(0.5),
            endEffPur = cms.double(1.005),
            discriminatorEnd = cms.double(30.0),
            startEffPur = cms.double(0.005)
        ),
        label = cms.InputTag("trackCountingHighPurBJetTags")
    ), cms.PSet(
        parameters = cms.PSet(
            discriminatorStart = cms.double(-0.1),
            nBinEffPur = cms.int32(200),
            effBConst = cms.double(0.5),
            endEffPur = cms.double(1.005),
            discriminatorEnd = cms.double(2.6),
            startEffPur = cms.double(0.005)
        ),
        label = cms.InputTag("jetProbabilityBJetTags")
    ), cms.PSet(
        parameters = cms.PSet(
            discriminatorStart = cms.double(-0.1),
            nBinEffPur = cms.int32(200),
            effBConst = cms.double(0.5),
            endEffPur = cms.double(1.005),
            discriminatorEnd = cms.double(8.1),
            startEffPur = cms.double(0.005)
        ),
        label = cms.InputTag("jetBProbabilityBJetTags")
    ), cms.PSet(
        parameters = cms.PSet(
            discriminatorStart = cms.double(-0.1),
            nBinEffPur = cms.int32(200),
            effBConst = cms.double(0.5),
            endEffPur = cms.double(1.005),
            discriminatorEnd = cms.double(1.1),
            startEffPur = cms.double(0.005)
        ),
        label = cms.InputTag("impactParameterMVABJetTags")
    ), cms.PSet(
        parameters = cms.PSet(
            discriminatorStart = cms.double(0.0),
            nBinEffPur = cms.int32(200),
            effBConst = cms.double(0.5),
            endEffPur = cms.double(1.005),
            discriminatorEnd = cms.double(8.0),
            startEffPur = cms.double(0.005)
        ),
        label = cms.InputTag("simpleSecondaryVertexBJetTags")
    ), cms.PSet(
        parameters = cms.PSet(
            discriminatorStart = cms.double(-0.1),
            nBinEffPur = cms.int32(200),
            effBConst = cms.double(0.5),
            endEffPur = cms.double(1.005),
            discriminatorEnd = cms.double(1.1),
            startEffPur = cms.double(0.005)
        ),
        label = cms.InputTag("combinedSecondaryVertexBJetTags")
    ), cms.PSet(
        parameters = cms.PSet(
            discriminatorStart = cms.double(-0.1),
            nBinEffPur = cms.int32(200),
            effBConst = cms.double(0.5),
            endEffPur = cms.double(1.005),
            discriminatorEnd = cms.double(1.1),
            startEffPur = cms.double(0.005)
        ),
        label = cms.InputTag("combinedSecondaryVertexMVABJetTags")
    ), cms.PSet(
        parameters = cms.PSet(
            discriminatorStart = cms.double(-0.1),
            nBinEffPur = cms.int32(200),
            effBConst = cms.double(0.05),
            endEffPur = cms.double(0.205),
            discriminatorEnd = cms.double(1.1),
            startEffPur = cms.double(0.005)
        ),
        label = cms.InputTag("softMuonBJetTags")
    ), cms.PSet(
        parameters = cms.PSet(
            discriminatorStart = cms.double(-0.1),
            nBinEffPur = cms.int32(200),
            effBConst = cms.double(0.05),
            endEffPur = cms.double(0.205),
            discriminatorEnd = cms.double(1.1),
            startEffPur = cms.double(0.005)
        ),
        label = cms.InputTag("softMuonNoIPBJetTags")
    ), cms.PSet(
        parameters = cms.PSet(
            discriminatorStart = cms.double(-0.1),
            nBinEffPur = cms.int32(200),
            effBConst = cms.double(0.05),
            endEffPur = cms.double(0.205),
            discriminatorEnd = cms.double(1.1),
            startEffPur = cms.double(0.005)
        ),
        label = cms.InputTag("softElectronBJetTags")
    )),
    epsBaseName = cms.string(''),
    produceEps = cms.bool(False),
    ptRecJetMax = cms.double(400.0),
    update = cms.bool(False),
    psBaseName = cms.string(''),
    etaMax = cms.double(2.4),
    etaMin = cms.double(0.0),
    ptPartonMin = cms.double(0.0),
    ptRecJetMin = cms.double(30.0),
    partonKinematics = cms.bool(True),
    inputfile = cms.string(''),
    jetIdParameters = cms.PSet(
        vetoFlavour = cms.vstring(),
        rejectBCSplitting = cms.bool(False),
        physicsDefinition = cms.bool(False),
        coneSizeToAssociate = cms.double(0.3),
        fillLeptons = cms.bool(False),
        fillHeavyHadrons = cms.bool(False),
        fillPartons = cms.bool(True),
        mcSource = cms.string('source')
    ),
    jetMCSrc = cms.InputTag("mcJetFlavour"),
    ptRanges = cms.vdouble(50.0, 80.0, 120.0),
    etaRanges = cms.vdouble(0.0, 1.4, 2.4),
    fastMC = cms.bool(False),
    ptPartonMax = cms.double(99999.0),
    producePs = cms.bool(False),
    allHistograms = cms.bool(False)
)


