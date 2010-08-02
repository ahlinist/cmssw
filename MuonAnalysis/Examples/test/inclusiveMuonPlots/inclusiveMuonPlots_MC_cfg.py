import FWCore.ParameterSet.Config as cms

process = cms.Process("MuonPlots")

# Messages
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.GlobalTag.globaltag = 'START36_V10::All'

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        ## Produced with MuonAnalysis/Examples/test/patSkims/patMuons_mc_cfg.py
        'root://pcmssd12.cern.ch//data/gpetrucc/7TeV/inclusiveMuons/inclusiveMuons_MuHLT_MinBiasMC357_v5/tupleMC_1_1.root',
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.TFileService = cms.Service('TFileService', fileName=cms.string('inclusiveMuonPlots_MC.class.root') )

from HLTrigger.HLTfilters.hltHighLevelDev_cfi import hltHighLevelDev
reskim  = hltHighLevelDev.clone(TriggerResultsTag = cms.InputTag('TriggerResults','',''))
process.recoMu     = reskim.clone(HLTPaths = ['skim_RecoMu'], HLTPathsPrescales = [1])
process.bscMinBias = hltHighLevelDev.clone(HLTPaths = ['HLT_L1_BscMinBiasOR_BptxPlusORMinus'], HLTPathsPrescales = [1])
process.preFilter = cms.Sequence(process.recoMu)

## for some classifications which differ from the basic ones only for arbitration, we save only the code not the flavour
## so to get the flavour we have to use the basic one.
flavourlessClassifiers = { 'classByHitsTMA':'classByHitsTM', 'classByHitsGlbPT':'classByHitsGlb' }

def addClassAliases(process, mainlabel, classifier, requireMB=True):
    mainmodule = getattr(process,mainlabel)
    ## split also in barrel/endcap
    abseta = "abs(eta)"
    if mainlabel.find("standAlone") == 0: abseta = "abs(outerTrack.eta)";
    mbar   = mainmodule.clone(selection = mainmodule.selection.value() + " && "+abseta+" < 1.2");
    mec    = mainmodule.clone(selection = mainmodule.selection.value() + " && "+abseta+" > 1.2");
    setattr(process, mainlabel+"B", mbar);
    setattr(process, mainlabel+"E", mec );
    # now for each of the three regions, create all subclasses
    for label in [mainlabel, mainlabel+"B", mainlabel+"E"]:
        module = getattr(process,label)
        modules = [module]
        sel = module.selection.value()
        flavClassifier = classifier 
        if classifier in flavourlessClassifiers: flavClassifier = flavourlessClassifiers[classifier]
        classes = (("Ghost", "-5 <= userInt('%s') <= -1"), ("Punch", "0 <= userInt('%s') <= 1"), ("Light", "userInt('%s') == 2"), ("Heavy", "userInt('%s') >= 3"), ("Error","userInt('%s') == -99"),
                   ("Beauty", "userInt('%s') >= 3 && (userInt('%x:momFlav') == 5 || userInt('%x:gmomFlav') == 5)"),
                   ("Charm",  "userInt('%s') >= 3 && (userInt('%x:momFlav') < 5  && userInt('%x:gmomFlav')  < 5)"))
        for X,C in classes:
            setattr(process, label+X, module.clone(selection = sel + ' && ' + (C.replace("%s",classifier).replace("%x",flavClassifier))))
            modules.append( getattr(process, label+X) )
        if requireMB:
            setattr(process, label+"_Classes", cms.Path(sum(modules,process.preFilter+process.bscMinBias)))
        else:
            setattr(process, label+"_Classes", cms.Path(sum(modules,process.preFilter)))

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
addClassAliases(process, "trackerMuons", "classByHitsTMLSAT")

process.globalMuons = process.trackerMuons.clone(selection = "isGlobalMuon")
addClassAliases(process, "globalMuons", "classByHitsGlb")

process.standAloneMuons   = process.trackerMuons.clone(selection = "isStandAloneMuon")
process.standAloneMuonsVH = process.trackerMuons.clone(selection = "isStandAloneMuon && outerTrack.numberOfValidHits > 0")
addClassAliases(process, "standAloneMuons",   "classByHitsSta")
addClassAliases(process, "standAloneMuonsVH", "classByHitsSta")


if True:
    process.globalMuonsPt3 = process.globalMuons.clone(
        selection = 'isGlobalMuon && pt > 3'
    )
    addClassAliases(process, "globalMuonsPt3", "classByHitsGlb")
    process.globalMuonsPt3HLT = process.globalMuons.clone(
        selection = 'isGlobalMuon && pt > 3 && !triggerObjectMatchesByFilter("hltSingleMu3L3Filtered3").empty()'
    )
    addClassAliases(process, "globalMuonsPt3HLT", "classByHitsGlb", requireMB=False)

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
    addClassAliases(process, "globalMuonsEwkPt3", "classByHitsGlbPT")
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
    addClassAliases(process, "globalMuonsEwkPt3HLT", "classByHitsGlbPT", requireMB=False)
