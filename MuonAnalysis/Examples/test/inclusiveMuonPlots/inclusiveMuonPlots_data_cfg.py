import FWCore.ParameterSet.Config as cms

process = cms.Process("MuonPlots")

# Messages
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.GlobalTag.globaltag = 'GR_R_36X_V12::All'

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        ## Produced with MuonAnalysis/Examples/test/patSkims/patMuons_data_cfg.py
        'root://pcmssd12.cern.ch//data/gpetrucc/7TeV/inclusiveMuons/inclusiveMuons_fromMay6th_v4/tupleData_42_1.root',
    )
)
for I in range(len(process.source.fileNames)): 
    process.source.fileNames[I] = process.source.fileNames[I].replace("root://pcmssd12.cern.ch//","file:/")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.TFileService = cms.Service('TFileService', fileName=cms.string('inclusiveMuonPlots_Data.root') )

from HLTrigger.HLTfilters.hltHighLevelDev_cfi import hltHighLevelDev
reskim  = hltHighLevelDev.clone(TriggerResultsTag = cms.InputTag('TriggerResults','',''))
process.recoMu     = reskim.clone(HLTPaths = ['skim_RecoMu'], HLTPathsPrescales = [1])
process.bscMinBias = hltHighLevelDev.clone(HLTPaths = ['HLT_L1_BscMinBiasOR_BptxPlusORMinus'], HLTPathsPrescales = [1])
process.preFilter = cms.Sequence(process.recoMu)

def addPlots(process, label, requireMB=True):
    module = getattr(process,label)
    ## prepare plots for full detector and for separate barrel/endcaps
    abseta = "abs(eta)"
    if label.find("standAlone") == 0: abseta = "abs(outerTrack.eta)";
    mbar   = module.clone(selection = module.selection.value() + " && "+abseta+" < 1.2");
    mec    = module.clone(selection = module.selection.value() + " && "+abseta+" > 1.2");
    setattr(process, label+"B", mbar);
    setattr(process, label+"E", mec );
    if requireMB:
        setattr(process, label+"_Plots", cms.Path(process.preFilter + process.bscMinBias + module + mbar + mec)) 
    else:
        setattr(process, label+"_Plots", cms.Path(process.preFilter + module + mbar + mec))

from MuonAnalysis.Examples.inclusiveMuonPlots_cfi import makeInclusiveMuonPlots;
commonInputs = cms.PSet(
    muons     = cms.InputTag('patMuonsWithTrigger'),
    onlyLeadingMuon = cms.bool(False),
    primaryVertices = cms.InputTag("offlinePrimaryVertices"),
)

process.trackerMuons = cms.EDAnalyzer("InclusiveMuonPlots",
    makeInclusiveMuonPlots(),
    commonInputs,
    selection = cms.string("isTrackerMuon && muonID('TMLastStationAngTight')"),
)
addPlots(process, "trackerMuons")

process.globalMuons = process.trackerMuons.clone(selection = "isGlobalMuon")
addPlots(process, "globalMuons")

process.standAloneMuons   = process.trackerMuons.clone(selection = "isStandAloneMuon")
process.standAloneMuonsVH = process.trackerMuons.clone(selection = "isStandAloneMuon && outerTrack.numberOfValidHits > 0")
addPlots(process, "standAloneMuons")
addPlots(process, "standAloneMuonsVH")

if True:
    process.globalMuonsPt3 = process.globalMuons.clone(
        selection = 'isGlobalMuon && pt > 3'
    )
    addPlots(process, "globalMuonsPt3")
    process.globalMuonsPt3HLT = process.globalMuons.clone(
        selection = 'isGlobalMuon && pt > 3 && !triggerObjectMatchesByFilter("hltSingleMu3L3Filtered3").empty()'
    )
    addPlots(process, "globalMuonsPt3HLT", requireMB=False)

    
if True:
    process.globalMuonsEwkPt3 = process.globalMuons.clone(
        selection = ('isGlobalMuon && isTrackerMuon && '+
                     'track.hitPattern.numberOfValidPixelHits > 0 && track.hitPattern.numberOfValidTrackerHits > 10 && '+
                     'numberOfMatches > 1 && '+
                     'abs(dB) < 0.2 && '+
                     'globalTrack.normalizedChi2 < 10 && '+
                     'globalTrack.hitPattern.numberOfValidMuonHits > 0 && '+
                     'pt > 3')
    )
    addPlots(process, "globalMuonsEwkPt3")
    process.globalMuonsEwkPt3HLT = process.globalMuons.clone(
        selection = ('isGlobalMuon && isTrackerMuon && '+
                     'track.hitPattern.numberOfValidPixelHits > 0 && track.hitPattern.numberOfValidTrackerHits > 10 && '+
                     'numberOfMatches > 1 && '+
                     'abs(dB) < 0.2 && '+
                     'globalTrack.normalizedChi2 < 10 && '+
                     'globalTrack.hitPattern.numberOfValidMuonHits > 0 && '+
                     'pt > 3 && '+
                     '!triggerObjectMatchesByFilter("hltSingleMu3L3Filtered3").empty()')
    )
    addPlots(process, "globalMuonsEwkPt3HLT", requireMB=False)


    process.globalMuonsEwkPt3HLT = process.globalMuons.clone(
        selection = ('isGlobalMuon && isTrackerMuon && '+
                     'track.hitPattern.numberOfValidPixelHits > 0 && track.hitPattern.numberOfValidTrackerHits > 10 && '+
                     'numberOfMatches > 1 && '+
                     'abs(dB) < 0.2 && '+
                     'globalTrack.normalizedChi2 < 10 && '+
                     'globalTrack.hitPattern.numberOfValidMuonHits > 0 && '+
                     'pt > 3 && '+
                     '!triggerObjectMatchesByFilter("hltSingleMu3L3Filtered3").empty()')
    )
    addPlots(process, "globalMuonsEwkPt3HLT", requireMB=False)

## GET LUMIS FROM https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions10/7TeV/
## OR FROM /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange(
	'132440:157-132440:378',
	'132596:382-132596:382',
	'132596:447-132596:447',
	'132598:174-132598:176',
	'132599:1-132599:379',
	'132599:381-132599:437',
	'132601:1-132601:207',
	'132601:209-132601:259',
	'132601:261-132601:1107',
	'132602:1-132602:70',
	'132605:1-132605:444',
	'132605:446-132605:522',
	'132605:526-132605:622',
	'132605:624-132605:814',
	'132605:816-132605:829',
	'132605:831-132605:867',
	'132605:896-132605:942',
	'132606:1-132606:26',
	'132656:1-132656:111',
	'132658:1-132658:51',
	'132658:56-132658:120',
	'132658:127-132658:148',
	'132659:1-132659:76',
	'132661:1-132661:116',
	'132662:1-132662:9',
	'132662:25-132662:74',
	'132716:220-132716:436',
	'132716:440-132716:487',
	'132716:491-132716:586',
	'132959:326-132959:334',
	'132960:1-132960:124',
	'132961:1-132961:222',
	'132961:226-132961:230',
	'132961:237-132961:381',
	'132965:1-132965:68',
	'132968:1-132968:67',
	'132968:75-132968:169',
	'133029:101-133029:115',
	'133029:129-133029:332',
	'133031:1-133031:18',
	'133034:132-133034:287',
	'133035:1-133035:63',
	'133035:67-133035:302',
	'133036:1-133036:222',
	'133046:1-133046:43',
	'133046:45-133046:210',
	'133046:213-133046:227',
	'133046:229-133046:323',
	'133158:65-133158:786',
	'133321:1-133321:383',
	'133446:105-133446:266',
	'133448:1-133448:484',
	'133450:1-133450:329',
	'133450:332-133450:658',
	'133474:1-133474:95',
	'133483:94-133483:159',
	'133483:161-133483:591',
	'133874:166-133874:297',
	'133874:299-133874:721',
	'133874:724-133874:814',
	'133874:817-133874:864',
	'133875:1-133875:20',
	'133875:22-133875:37',
	'133876:1-133876:315',
	'133877:1-133877:77',
	'133877:82-133877:104',
	'133877:113-133877:231',
	'133877:236-133877:294',
	'133877:297-133877:437',
	'133877:439-133877:622',
	'133877:624-133877:853',
	'133877:857-133877:1472',
	'133877:1474-133877:1640',
	'133877:1643-133877:1931',
	'133881:1-133881:71',
	'133881:74-133881:223',
	'133881:225-133881:551',
	'133885:1-133885:132',
	'133885:134-133885:728',
	'133927:1-133927:44',
	'133928:1-133928:645',
	'135059:59-135059:67',
	'135149:297-135149:337',
	'135149:339-135149:754',
	'135149:756-135149:932',
	'135149:934-135149:937',
	'135149:942-135149:993',
	'135149:995-135149:1031',
	'135149:1033-135149:1098',
	'135149:1102-135149:1808',
	'135149:1811-135149:2269',
	'135149:2274-135149:2524',
	'135149:2528-135149:2713',
	'135149:2715-135149:3098',
	'135149:3100-135149:3102',
	'135149:3105-135149:3179',
	'135149:3182-135149:3303',
	'135149:3305-135149:3381',
	'135175:55-135175:545',
	'135175:548-135175:561',
	'135175:563-135175:790',
	'135175:792-135175:1046',
	'135445:997-135445:1067',
	'135445:1069-135445:1329',
	'135445:1332-135445:1388',
	'135445:1391-135445:1629',
	'135445:1631-135445:1827',
	'135521:60-135521:108',
	'135521:110-135521:359',
	'135521:361-135521:440',
	'135521:442-135521:488',
	'135523:1-135523:64',
	'135523:66-135523:109',
	'135523:113-135523:124',
	'135523:126-135523:211',
	'135525:1-135525:3',
	'135525:6-135525:143',
	'135525:145-135525:381',
	'135525:384-135525:435',
	'135525:437-135525:452',
	'135528:1-135528:91',
	'135528:94-135528:95',
	'135528:98-135528:142',
	'135528:145-135528:147',
	'135528:149-135528:308',
	'135528:310-135528:454',
	'135528:456-135528:606',
	'135528:608-135528:609',
	'135528:611-135528:770',
	'135528:773-135528:776',
	'135528:779-135528:813',
	'135528:816-135528:912',
	'135528:915-135528:924',
	'135528:926-135528:1082',
	'135528:1084-135528:1213',
	'135528:1215-135528:1436',
	'135535:75-135535:167',
	'135535:169-135535:232',
	'135537:39-135537:69',
	'135573:102-135573:110',
	'135573:113-135573:118',
	'135573:120-135573:155',
	'135575:2-135575:210',
	'135575:213-135575:241',
	'135575:243-135575:264',
	'135575:266-135575:381',
	'135575:384-135575:638',
	'135575:645-135575:1161',
	'135575:1163-135575:1253',
	'135735:31-135735:42',
	'135735:44-135735:149',
	'135735:151-135735:234',
	'135735:236-135735:320',
	'136066:181-136066:297',
	'136066:299-136066:319',
	'136066:321-136066:321',
	'136066:323-136066:336',
	'136066:338-136066:348',
	'136066:350-136066:366',
	'136066:368-136066:529',
	'136066:532-136066:544',
	'136066:546-136066:595',
	'136066:597-136066:719',
	'136066:721-136066:1004',
	'136066:1006-136066:1184',
	'136080:249-136080:250',
	'136080:252-136080:256',
	'136082:1-136082:173',
	'136082:175-136082:422',
	'136082:477-136082:504',
	'136082:506-136082:506',
	'136087:250-136087:265',
	'136087:267-136087:277',
	'136087:279-136087:310',
	'136087:312-136087:315',
	'136088:1-136088:3',
	'136088:6-136088:17',
	'136088:19-136088:39',
	'136088:41-136088:47',
	'136088:50-136088:50',
	'136088:52-136088:52',
	'136088:54-136088:63',
	'136088:65-136088:65',
	'136088:67-136088:89',
	'136088:91-136088:113',
	'136088:115-136088:131',
	'136088:133-136088:137',
	'136088:139-136088:184',
	'136088:187-136088:191',
	'136088:193-136088:194',
	'136088:196-136088:206',
	'136088:208-136088:208',
	'136088:210-136088:262',
	'136097:1-136097:1',
	'136097:3-136097:3',
	'136097:5-136097:5',
	'136097:7-136097:7',
	'136097:9-136097:9',
	'136097:11-136097:12',
	'136097:14-136097:79',
	'136098:1-136098:11',
	'136119:1-136119:36',
)


