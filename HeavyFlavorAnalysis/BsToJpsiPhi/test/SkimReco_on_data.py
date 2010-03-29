import FWCore.ParameterSet.Config as cms

process = cms.Process("ANALYSIS")

process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/StandardSequences/Services_cff')
process.load("PhysicsTools.HepMCCandAlgos.allMuonsGenParticlesMatch_cfi")

process.GlobalTag.globaltag = 'GR09_R_35X_V4::All'
#process.GlobalTag.globaltag = 'GR09_R_V5::All'

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")

process.load("SimTracker.TrackAssociation.TrackAssociatorByChi2_cfi")
process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")
process.load("SimGeneral.TrackingAnalysis.trackingParticles_cfi")
process.load("SimGeneral.MixingModule.mixNoPU_cfi")

process.allKTracks = cms.EDProducer("ConcreteChargedCandidateProducer",
                                    src = cms.InputTag("generalTracks"),
                                    particleType = cms.string('K+')
                                    )

process.allPiTracks = cms.EDProducer("ConcreteChargedCandidateProducer",
                                    src = cms.InputTag("generalTracks"),
                                    particleType = cms.string('pi+')
                                    )

process.kTracks = cms.EDFilter("CandViewRefSelector",
                               src = cms.InputTag("allKTracks"),
                               cut = cms.string("pt > 0.3 & abs(eta) < 2.5")	
                               )

process.piTracks = cms.EDFilter("CandViewRefSelector",
                               src = cms.InputTag("allPiTracks"),
                               cut = cms.string("pt > 0.3 & abs(eta) < 2.5")	
                               )
process.bsVertexAnalysis = cms.EDAnalyzer("BsToJpsiPhiAnalysis",
                                          genParticlesLabel  = cms.InputTag(""),  # on DATA
#                                          genParticlesLabel  = cms.InputTag("genParticles"),
                                          TrackLabel_K = cms.InputTag("kTracks"),
                                          TrackLabel_pi = cms.InputTag("piTracks"),
                                          TriggerTag = cms.InputTag("TriggerResults::HLT"),
                                          MuonTag = cms.InputTag("muons"),
                                          StoreDeDxInfo = cms.bool( True ),
                                          saveL1DoubleMuOpenTriggeredOnly = cms.bool( False ), # on DATA
#                                          saveL1DoubleMuOpenTriggeredOnly = cms.bool( True ),
                                          JpsiMassWindowBeforeFit = cms.double(2), # on DATA
                                          JpsiMassWindowAfterFit = cms.double(2), # on DATA
#                                          JpsiMassWindowBeforeFit = cms.double(0.150),
#                                          JpsiMassWindowAfterFit = cms.double(0.150),
                                          JpsiPtCut      = cms.double(0.), # on DATA
#                                          JpsiPtCut      = cms.double(0.5),
                                          KaonTrackPtCut = cms.double(0.5),
                                          BdKaonTrackPtCut = cms.double(0.4),
                                          PhiMassWindowBeforeFit  = cms.double(0.2), # on DATA
                                          PhiMassWindowAfterFit  = cms.double(0.2), # on DATA
#                                          PhiMassWindowBeforeFit  = cms.double(0.05),
#                                          PhiMassWindowAfterFit  = cms.double(0.04),
                                          BsLowerMassCutBeforeFit = cms.double(4.5),
                                          BsUpperMassCutBeforeFit = cms.double(6),
                                          BsLowerMassCutAfterFit  = cms.double(5.2),
                                          BsUpperMassCutAfterFit  = cms.double(5.7),
                                          KstarMassWindowBeforeFit =cms.double(0.2),
                                          KstarMassWindowAfterFit =cms.double(0.2),
                                          BdLowerMassCutBeforeFit = cms.double(4.5),
                                          BdUpperMassCutBeforeFit = cms.double(6),
                                          BdLowerMassCutAfterFit = cms.double(4.9),
                                          BdUpperMassCutAfterFit = cms.double(5.7),

                                          verbose                = cms.bool( True ), # on DATA
#                                          verbose                = cms.bool( False ),
                                          outputFile = cms.untracked.string("/tmp/cerizza/outputfile_onDATA.root"),
                                          
                                         )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )



#-------------------------------------------------------------------------
#############REAL DATA######################
# this is for filtering on L1 technical trigger bit
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
# bsc minbias and veto on beam halo
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')

# this is for filtering on HLT physics bit
process.hltPhysicsDeclared = cms.EDFilter("HLTHighLevel",
                                 TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                                 HLTPaths = cms.vstring("HLT_PhysicsDeclared"
                                                        ),
                                 eventSetupPathsKey = cms.string(''),
                                 andOr = cms.bool(True),
                                 throw = cms.bool(True)
                                 )

# this is for filtering on HLT MinBiasBSC bit
process.hltMinBiasBSC = cms.EDFilter("HLTHighLevel",
                                     TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                                     HLTPaths = cms.vstring("HLT_MinBiasBSC"
                                                            ),
                                     eventSetupPathsKey = cms.string(''),
                                     andOr = cms.bool(True),
                                     throw = cms.bool(True)
                                     )


# filter on good vertex
process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                                           vertexCollection = cms.InputTag('offlinePrimaryVertices'),
                                           minimumNDOF = cms.uint32(4), ## starting from > 341
                                           #minimumNumberOfTracks = cms.uint32(3) ,
                                           maxAbsZ = cms.double(15),	
                                           maxd0 = cms.double(2)	
                                           )

#process.oneGoodVertexFilter = cms.EDFilter("VertexSelector",
#   		src = cms.InputTag("offlinePrimaryVertices"),
# 		 cut = cms.string("!isFake && ndof > 4 && abs(z) <= 15 && position.Rho <= 2"),
#   		filter = cms.bool(True),       # otherwise it won't filter the events, just produce an empty vertex collection.
#)


# filter to remove scraping ("monster") events
process.scrapingFilter = cms.EDFilter("FilterOutScraping",
                                      applyfilter = cms.untracked.bool(True),
                                      debugOn = cms.untracked.bool(False),
                                      numtrack = cms.untracked.uint32(10),
                                      thresh = cms.untracked.double(0.2)
                                       )

## ------  note (22 Feb 2010): 
# VertexSelector and GoodVertexSelector basically do the same thing.
#    VertexSelector mainly used by tracker DPG (see TRKPromptFeedBack twiki page )
#    GoodVertexSelector  used by DPGAnalysis/Skims
#    lateste PVT prescription:  ndf > 4, non tracks > 3. 
#    this can be achieved in VertexSelector  with "ndof > 4 "   or 
#    taking new GoodVertexSelector code by the HEAD of DPGAnalysis/Skims (the one in 3_4_2 is not good),
#       and putting "minimumNDOF = cms.uint32(4)" 
## ---------------------------------------------------------------------------------

###############################################


process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    
    ##################  /MinimumBias/BeamCommissioning09-Jan29ReReco-v2/RECO  ##########################
    
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/FEF6AAC7-760E-DF11-8AA3-002618943861.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/F87D1FB9-760E-DF11-A3E5-003048D15E2C.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/F6C9A2B4-760E-DF11-8F86-003048678AFA.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/F22F7BA2-760E-DF11-A78B-00304867BFAA.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/F08EB5BA-760E-DF11-88A7-003048678BAC.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/E8D607A1-760E-DF11-AC70-00304867C04E.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/E6EA04BA-760E-DF11-AD5C-003048679236.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/E6C2D2B4-760E-DF11-A442-003048D15E02.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/DC787FBB-760E-DF11-9145-002618943875.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/D4E8E4C5-760E-DF11-B32A-003048678A78.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/D22B25A4-760E-DF11-A7E2-003048679236.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/CEA661A0-760E-DF11-BB3C-003048678BAE.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/CC31E2B9-760E-DF11-8EBB-003048678B34.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/C455E3C4-760E-DF11-8F30-00261894393E.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/B8042BB7-760E-DF11-A721-00261894398B.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/B45DFCB3-760E-DF11-8F9E-003048678BB2.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/B43CBCA1-760E-DF11-BE06-002618FDA27F.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/B05339BC-760E-DF11-A53C-003048678E92.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/A8069CC2-760E-DF11-8AA6-003048678B18.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/A6C64FC4-760E-DF11-9F43-00304867908C.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/A6024BB5-760E-DF11-A546-003048678B18.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/A01A5BC5-760E-DF11-AED2-003048678B34.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/9C9DB4B5-760E-DF11-B50A-002618943911.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/9489CDC2-760E-DF11-BF69-003048678BC6.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/904FD9C2-760E-DF11-8743-003048678BB2.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/8AB1D9B1-760E-DF11-92CE-003048678AC8.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/86B1AAB2-760E-DF11-85FB-002618943976.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/7EC380BE-760E-DF11-AAE4-002618FDA25B.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/7CDA85A4-760E-DF11-BD3E-003048678D52.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/7AAFE0AD-760E-DF11-9E27-002618943842.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/766846C5-760E-DF11-8777-0030486791DC.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/66C037C3-760E-DF11-872E-003048678B0A.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/66220ABC-760E-DF11-8829-00304867C04E.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/647537B1-760E-DF11-AC39-0030486792F0.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/5C54D3C5-760E-DF11-95B7-002618943842.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/566758BB-760E-DF11-9B58-003048678FE0.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/5278F39F-760E-DF11-8D68-002618943842.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/4A7771BC-760E-DF11-93FC-0030486791AA.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/446C8BA4-760E-DF11-A814-00304867C1BC.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/3C6C01C3-760E-DF11-B0C4-003048678B04.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/30951DC1-760E-DF11-BFA4-00304867BFAA.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/300E02C3-760E-DF11-A4AE-003048678BAA.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/2A229DBF-760E-DF11-98E8-003048678B08.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/24F66DA3-760E-DF11-ACA8-003048D15D04.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/1E0AB3BE-760E-DF11-B5B8-00304867918A.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/1CD26CC6-760E-DF11-B673-002618943908.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/18A6ACBB-760E-DF11-A102-003048678BAE.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/103486C0-760E-DF11-A5F8-003048678B38.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/0A9A11C6-760E-DF11-B631-003048678AFA.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/0A521CB8-760E-DF11-AF9F-002618943976.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/02C6BAB7-760E-DF11-9703-003048D15D04.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/F8290B8C-200E-DF11-B0C8-0018F3D09612.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/F690D271-1D0E-DF11-887E-001A92810AD2.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/F661817A-2B0E-DF11-A464-001A92810AE0.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/F40B408B-270E-DF11-B5F6-001BFCDBD19E.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/E80BCCAA-280E-DF11-AE91-001A92971B0E.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/E6606F77-1F0E-DF11-B04B-0018F3D09612.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/E617006A-260E-DF11-BAE8-00304866C398.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/DA9878B3-210E-DF11-9B08-001A92810ACA.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/D0323386-270E-DF11-B726-003048678F8C.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/CEA0807E-200E-DF11-BF85-003048678FB2.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/CAEFDB6D-1C0E-DF11-AF88-0018F3D09612.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/CADAB5BF-320E-DF11-935B-0018F3D09690.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/C0DD755D-1D0E-DF11-8988-0018F3D09680.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/BA2F827B-1F0E-DF11-B0FE-001A92810AA4.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/B6CC7272-2B0E-DF11-A9A6-003048678F8E.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/B4E19491-2C0E-DF11-B66B-00304867C1B0.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/AC9A0ED7-250E-DF11-BA28-001A92971B0E.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/AC1A2375-200E-DF11-98F6-0018F3D09612.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/A6996BB8-2A0E-DF11-9EEE-001A92971AAA.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/A4F1AA9D-1E0E-DF11-85DB-001A92810AE4.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/A0F52A9B-230E-DF11-84E2-001A92971B0E.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/A0D765B7-210E-DF11-B999-0018F3D0967E.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/9C8AF480-1F0E-DF11-A4B1-001A9281173E.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/9C828C9B-2E0E-DF11-A6A3-001A92971B7E.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/92452F8D-270E-DF11-B5B8-00304867C136.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/84C6A98A-1E0E-DF11-BE76-001A92810AD2.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/8431D353-1B0E-DF11-8863-001A92971B0C.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/762D93B4-250E-DF11-8DE8-0018F3D09700.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/745C56C6-2D0E-DF11-8FCE-001A92971BD8.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/70237F63-1D0E-DF11-86A7-001A9281173E.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/6CCE33A7-280E-DF11-A831-003048678FA0.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/6A8A6FB3-210E-DF11-9D6A-001A92810ACA.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/68440381-2B0E-DF11-9124-001A92971B08.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/66571285-2A0E-DF11-A901-001A92971B7E.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/62963EBB-2D0E-DF11-9EA5-003048678F78.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/5C12547A-1F0E-DF11-9409-001A92811748.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/58C23895-1E0E-DF11-83BC-001A92810ACA.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/4AA6348D-270E-DF11-9F7D-001A92971AEC.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/46A1008B-200E-DF11-98C0-001BFCDBD19E.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/3A47F4A9-280E-DF11-9054-001BFCDBD19E.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/2A5F1D87-1F0E-DF11-BB59-001A92971B0E.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/287E77D2-250E-DF11-8D38-003048678D6C.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/26539B8A-350E-DF11-AE34-003048678F02.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/1C69D8D3-250E-DF11-8A8A-001A9281173E.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/12F6779D-390E-DF11-BB34-001A9281171E.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/0E2D9FB5-210E-DF11-9A81-001A9281173E.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0020/ECF7843B-F90D-DF11-B798-001BFCDBD19E.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0020/38C92F05-EF0D-DF11-AAE6-00304867BEE4.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0020/120CB3FD-F60D-DF11-8566-001A92971B08.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/FE6765FA-EB0D-DF11-9AC3-001A92971AEC.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/F6877BC6-DD0D-DF11-B7F4-002618FDA211.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/F2C49E2B-E50D-DF11-BDDA-003048678FF4.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/F0F311CE-E80D-DF11-A688-0026189438C4.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/EC0BC135-E60D-DF11-9BA0-001A928116FC.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/E6A1CD32-E30D-DF11-9729-001A928116D2.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/E60B251D-EC0D-DF11-B52A-001A92810AD8.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/E00FE9E5-DE0D-DF11-A0A6-001A92810AE6.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/D61AD9C3-DD0D-DF11-B34C-001A928116EC.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/D211AFF0-DD0D-DF11-A839-001A92810A94.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/D0A0770F-EC0D-DF11-BA4B-00304867C0F6.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/CABBC52C-E40D-DF11-9F95-001A92810AEC.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/C4B87932-E30D-DF11-9F72-001A92810AE6.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/C490874E-E60D-DF11-9EBD-001A92971B72.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/BEBFDA37-E50D-DF11-85BC-001A92810AC8.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/B48B10FC-E20D-DF11-A4F0-001A9281171C.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/ACEE9B8E-DD0D-DF11-84B6-0018F3D096DE.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/AC7C84EB-E10D-DF11-96FF-001A928116D2.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/AC0C5C35-E50D-DF11-B616-001A92971B74.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/AACEB375-E70D-DF11-8CA8-0018F3D096B4.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/9C2D3DEC-E10D-DF11-AA12-001A92810AAA.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/9622F3C0-EA0D-DF11-B54A-0018F3D09692.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/92FECE8D-E70D-DF11-8BD9-001A92810ACA.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/8E958B61-E70D-DF11-A123-001A9281173C.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/8E581290-DD0D-DF11-BDE4-001A928116C4.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/86A9D1E5-DE0D-DF11-83DE-001A92810AA0.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/7AE7D960-E30D-DF11-9FC2-001A92810A94.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/726A7466-E60D-DF11-99B3-002618FDA20E.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/70101BE2-DE0D-DF11-AF20-001A9281170C.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/6E575989-DC0D-DF11-93C7-001A92810ACE.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/66F2BBF2-E10D-DF11-A5B5-001A928116D0.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/665E07EB-E10D-DF11-8A17-001A928116EC.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/568EF468-E80D-DF11-9180-0030486790BE.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/54F9F8C7-DF0D-DF11-9902-00261894396B.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/483D3EE4-E10D-DF11-B17F-001A92971B20.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/38C94496-E90D-DF11-8AB9-001A92810AF4.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/3800DE38-E50D-DF11-AC6C-00304867902C.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/36E1B516-EC0D-DF11-93BE-00304867924E.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/341BF3ED-E00D-DF11-B48D-001A92810AA2.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/2C9B9B9F-EB0D-DF11-8034-001A92971BDC.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/2C725039-E40D-DF11-8317-001A92810AD8.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/28BBE433-E30D-DF11-8FE6-001A92971B84.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/2695492C-E50D-DF11-8C6F-001A92971B30.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/2425BB8A-DC0D-DF11-848B-001A92811746.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/22F8B3E8-E10D-DF11-8AF3-001A92971B28.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/1CA1AB40-E50D-DF11-AA28-0018F3D095F6.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/1A3E6486-E70D-DF11-B125-003048678B1C.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/14A1BAE5-DE0D-DF11-B2FC-001A92810AE4.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/0E2EB433-E30D-DF11-B3C1-001A928116F8.root',
    '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/0ACCA1F9-E00D-DF11-860C-0018F3D095F6.root'
    
    )
                            )
	
	
#-------------------------------------------------------------------------
# filter on lumisections
from HeavyFlavorAnalysis.BsToJpsiPhi.goodLumiSectionList_cfi import *
process.source.lumisToProcess = goodLumisToProcess
#-------------------------------------------------------------------------


### path
process.p = cms.Path(
    process.hltLevel1GTSeed +
    process.hltPhysicsDeclared +
    process.hltMinBiasBSC +
    process.primaryVertexFilter +
    process.scrapingFilter +
    process.allPiTracks +
    process.allKTracks +
    process.kTracks +
    process.piTracks +
    process.bsVertexAnalysis
    )



