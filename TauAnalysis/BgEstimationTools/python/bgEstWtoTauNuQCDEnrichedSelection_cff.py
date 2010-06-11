import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *
from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *

#--------------------------------------------------------------------------------
# modify pat::Taus
#--------------------------------------------------------------------------------

from TauAnalysis.RecoTools.patPFTauSelection_cfi import *
from TauAnalysis.RecoTools.patPFTauSelectionForWTauNu_cfi import *
tausBgEstWtoTauNuqcdEnrichedLeadTrkPt = copy.deepcopy(selectedPatTausForWTauNuLeadTrkPt)
tausBgEstWtoTauNuqcdEnrichedLeadTrkPt.cut = cms.string('leadTrack().isNonnull() & leadTrack().pt() > 15 & leadTrack.pt() < 20')
tausBgEstWtoTauNuqcdEnrichedEcalIso = copy.deepcopy(selectedPatTausForWTauNuEcalIso)
tausBgEstWtoTauNuqcdEnrichedEcalIso.cut = cms.string('tauID("byIsolation") < 0.5')
tausBgEstWtoTauNuqcdEnrichedTrkIso = copy.deepcopy(selectedPatTausForWTauNuTrkIso)
tausBgEstWtoTauNuqcdEnrichedTrkIso.cut = cms.string('tauID("byIsolation") < 0.5')
tausBgEstWtoTauNuqcdEnrichedProng = copy.deepcopy(selectedPatTausForWTauNuProng)
tausBgEstWtoTauNuqcdEnrichedProng.cut = cms.string('tauID("byIsolation") < 0.5')
tausBgEstWtoTauNuqcdEnrichedCharge = copy.deepcopy(selectedPatTausForWTauNuCharge)
tausBgEstWtoTauNuqcdEnrichedCharge.cut = cms.string('tauID("byIsolation") < 0.5')
tausBgEstWtoTauNuqcdEnrichedMuonVeto = copy.deepcopy(selectedPatTausForWTauNuMuonVeto)
tausBgEstWtoTauNuqcdEnrichedElectronVeto = copy.deepcopy(selectedPatTausForWTauNuElectronVeto)
tausBgEstWtoTauNuqcdEnrichedEcalCrackVeto = copy.deepcopy(selectedPatTausForWTauNuEcalCrackVeto)


tauSelConfiguratorBgEstWtoTauNuqcdEnriched = objSelConfigurator(
        [
            tausBgEstWtoTauNuqcdEnrichedLeadTrkPt,
                    tausBgEstWtoTauNuqcdEnrichedEcalIso,
                    tausBgEstWtoTauNuqcdEnrichedTrkIso,
                    tausBgEstWtoTauNuqcdEnrichedProng,
                    tausBgEstWtoTauNuqcdEnrichedCharge,
                    tausBgEstWtoTauNuqcdEnrichedMuonVeto,
                    tausBgEstWtoTauNuqcdEnrichedElectronVeto,
                    tausBgEstWtoTauNuqcdEnrichedEcalCrackVeto ],
            src = "selectedPatTausForWTauNuLeadTrkCumulative",
            pyModuleName = __name__,
            doSelIndividual = False
        )

selectTausBgEstWtoTauNuqcdEnriched = tauSelConfiguratorBgEstWtoTauNuqcdEnriched.configure(pyNameSpace = locals())
#--------------------------------------------------------------------------------
# modify tau+MET collection
#--------------------------------------------------------------------------------

tauNuPairsBgEstqcdEnriched = cms.EDProducer("PATTauNuPairProducer",
                                                srcVisDecayProducts = cms.InputTag('tausBgEstWtoTauNuqcdEnrichedEcalCrackVetoCumulative'),
                                                srcMET = cms.InputTag('patPFMETs'),
                                                verbosity = cms.untracked.int32(0)
                                            )

selectTauNuPairsBgEstWtoTauNuqcdEnriched = cms.Sequence(tauNuPairsBgEstqcdEnriched)

#-----------------------------------------------------------------------------------
# modify jet collection
#----------------------------------------------------------------------------------
jetsBgEstqcdEnrichedAntiOverlapWithTausVeto = cms.EDFilter("PATJetAntiOverlapSelector",
                                                           src = cms.InputTag("selectedPatJetsEta21Cumulative"),
                                                           srcNotToBeFiltered = cms.VInputTag(
    "tausBgEstWtoTauNuqcdEnrichedEcalCrackVetoCumulative"
    ),
                                                           dRmin = cms.double(0.7),
                                                           filter = cms.bool(False)
                                                           )

jetsBgEstqcdEnrichedPt15 = cms.EDFilter("PATJetSelector",
                                        src = cms.InputTag("jetsBgEstqcdEnrichedAntiOverlapWithTausVeto"),
                                        cut = cms.string('pt > 15'),
                                        filter = cms.bool(False)
                                        )

selectJetsBgEstWplusJetsEnriched = cms.Sequence(
    jetsBgEstqcdEnrichedAntiOverlapWithTausVeto
    *jetsBgEstqcdEnrichedPt15
    )
#-----------------------------------------------------------------------------------
# modify tau recoil energy collection
#----------------------------------------------------------------------------------

tauRecoilEnergyFromCaloTowersBgEstqcdEnriched = cms.EDProducer("PATTauRecoilEnergyFromCaloTowersProducer",
                                                               srcLeptons = cms.InputTag('tausBgEstWtoTauNuqcdEnrichedEcalCrackVetoCumulative'),
                                                               srcEnergyObjects = cms.InputTag('towerMaker'),
                                                               etaMin = cms.double(0.),
                                                               etaMax = cms.double(+5.),
                                                               etMin = cms.double(0.5),
                                                               dPhiMin = cms.double(2.64),
                                                               verbosity = cms.untracked.int32(0)
                                                               )

tauRecoilEnergyFromCaloTowersPt5BgEstqcdEnriched = cms.EDFilter("PATTauRecoilEnergyFromCaloTowersSelector",
                                                                src = cms.InputTag("tauRecoilEnergyFromCaloTowersBgEstqcdEnriched"),
                                                                cut = cms.string('etSum < 10.'),
                                                                filter = cms.bool(False)
                                                                )


selectTauRecoilEnergyBgEstqcdEnriched = cms.Sequence(
    tauRecoilEnergyFromCaloTowersBgEstqcdEnriched
    *tauRecoilEnergyFromCaloTowersPt5BgEstqcdEnriched
    )

#-----------------------------------------------------------------------------------
# modify MET-topology collection
#----------------------------------------------------------------------------------
MEtTopology04BgEstqcdEnriched = cms.EDFilter("MEtTopologySelector",
                                             src = cms.InputTag('metTopologies'),
                                             cut = cms.string('Vanti()/Vparallel() < 0.4'),
                                             filter = cms.bool(False)
                                             )
selectMEtTopologyBgEstqcdEnriched = cms.Sequence(MEtTopology04BgEstqcdEnriched)

#---------------------Select events------------------------------------------------------
from TauAnalysis.Configuration.selectWtoTauNu_cff import *
cfgTauLeadTrkPtBgEstqcdEnriched = copy.deepcopy(cfgTauLeadTrkPt)
cfgTauLeadTrkPtBgEstqcdEnriched.pluginName = cms.string('tauLeadTrkPtCutBgEstqcdEnriched')
cfgTauLeadTrkPtBgEstqcdEnriched.src_cumulative = cms.InputTag('tausBgEstWtoTauNuqcdEnrichedLeadTrkPtCumulative')
cfgTauLeadTrkPtBgEstqcdEnriched.systematics = cms.vstring()

cfgTauTrkIsoBgEstqcdEnriched = copy.deepcopy(cfgTauTrkIso)
cfgTauTrkIsoBgEstqcdEnriched.pluginName = cms.string('tauTrkIsoCutBgEstqcdEnriched')
cfgTauTrkIsoBgEstqcdEnriched.src_cumulative = cms.InputTag('tausBgEstWtoTauNuqcdEnrichedTrkIsoCumulative')
cfgTauTrkIsoBgEstqcdEnriched.systematics = cms.vstring()

cfgTauEcalIsoBgEstqcdEnriched = copy.deepcopy(cfgTauEcalIso)
cfgTauEcalIsoBgEstqcdEnriched.pluginName = cms.string('tauEcalIsoCutBgEstqcdEnriched')
cfgTauEcalIsoBgEstqcdEnriched.src_cumulative = cms.InputTag('tausBgEstWtoTauNuqcdEnrichedEcalIsoCumulative')
cfgTauEcalIsoBgEstqcdEnriched.systematics = cms.vstring()

cfgTauProngBgEstqcdEnriched = copy.deepcopy(cfgTauProngCut)
cfgTauProngBgEstqcdEnriched.pluginName = cms.string('tauProngCutBgEstqcdEnriched')
cfgTauProngBgEstqcdEnriched.src_cumulative = cms.InputTag('tausBgEstWtoTauNuqcdEnrichedProngCumulative')
cfgTauProngBgEstqcdEnriched.systematics = cms.vstring()

cfgTauChargeBgEstqcdEnriched = copy.deepcopy(cfgTauChargeCut)
cfgTauChargeBgEstqcdEnriched.pluginName = cms.string('tauChargeCutBgEstqcdEnriched')
cfgTauChargeBgEstqcdEnriched.src_cumulative = cms.InputTag('tausBgEstWtoTauNuqcdEnrichedChargeCumulative')
cfgTauChargeBgEstqcdEnriched.systematics = cms.vstring()

cfgTauMuonVetoBgEstqcdEnriched = copy.deepcopy(cfgTauMuonVeto)
cfgTauMuonVetoBgEstqcdEnriched.pluginName = cms.string('tauMuonVetoCutBgEstqcdEnriched')
cfgTauMuonVetoBgEstqcdEnriched.src_cumulative = cms.InputTag('tausBgEstWtoTauNuqcdEnrichedMuonVetoCumulative')
cfgTauMuonVetoBgEstqcdEnriched.systematics = cms.vstring()

cfgTauElectronVetoBgEstqcdEnriched = copy.deepcopy(cfgTauElectronVeto)
cfgTauElectronVetoBgEstqcdEnriched.pluginName = cms.string('tauElectronVetoCutBgEstqcdEnriched')
cfgTauElectronVetoBgEstqcdEnriched.src_cumulative = cms.InputTag('tausBgEstWtoTauNuqcdEnrichedElectronVetoCumulative')
cfgTauElectronVetoBgEstqcdEnriched.systematics = cms.vstring()

cfgTauEcalCrackVetoBgEstqcdEnriched = copy.deepcopy(cfgTauEcalCrackVeto)
cfgTauEcalCrackVetoBgEstqcdEnriched.pluginName = cms.string('tauEcalCrackVetoCutBgEstqcdEnriched')
cfgTauEcalCrackVetoBgEstqcdEnriched.src_cumulative = cms.InputTag('tausBgEstWtoTauNuqcdEnrichedEcalCrackVetoCumulative')
cfgTauEcalCrackVetoBgEstqcdEnriched.systematics = cms.vstring()

cfgCentralJetVetoBgEstqcdEnriched = cms.PSet(
    pluginName = cms.string('centralJetVetoBgEstqcdEnriched'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('jetsBgEstqcdEnrichedPt15'),
    maxNumber = cms.uint32(0)
    )

cfgRecoilEnergyFromCaloTowersCutBgEstqcdEnriched = cms.PSet(
    pluginName = cms.string('recoilEnergyCutBgEstqcdEnriched'),
    pluginType = cms.string('PATTauRecoilEnergyFromCaloTowersMinEventSelector'),
    src = cms.InputTag('tauRecoilEnergyFromCaloTowersPt5BgEstqcdEnriched'),
    minNumber = cms.uint32(1)
    )

cfgMetTopologyCutBgEstqcdEnriched = cms.PSet(
    pluginName = cms.string('metTopologyCutBgEstqcdEnriched'),
    pluginType = cms.string('MEtTopologyMinEventSelector'),
    src = cms.InputTag('MEtTopology04BgEstqcdEnriched'),
    minNumber = cms.uint32(1)
    )

evtSelConfiguratorBgEstWtoTauNuqcdEnriched = eventSelFlagProdConfigurator(
    [
    cfgTauLeadTrkPtBgEstqcdEnriched,
    cfgTauTrkIsoBgEstqcdEnriched,
    cfgTauEcalIsoBgEstqcdEnriched,
    cfgTauProngBgEstqcdEnriched,
    cfgTauChargeBgEstqcdEnriched,
    cfgTauMuonVetoBgEstqcdEnriched,
    cfgTauElectronVetoBgEstqcdEnriched,
    cfgTauEcalCrackVetoBgEstqcdEnriched,
    cfgCentralJetVetoBgEstqcdEnriched,
    cfgRecoilEnergyFromCaloTowersCutBgEstqcdEnriched,
    cfgMetTopologyCutBgEstqcdEnriched
    ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
    )

selectEventsBgEstWtoTauNuqcdEnriched = evtSelConfiguratorBgEstWtoTauNuqcdEnriched.configure()

#-------------------Analyze events------------------------------------------------------------------------------
from TauAnalysis.Configuration.analyzeWtoTauNu_cff import *

tauHistManagerBgEstqcdEnriched = copy.deepcopy(tauHistManager)
tauHistManagerBgEstqcdEnriched.pluginName = cms.string('tauHistManagerBgEstqcdEnriched')
tauHistManagerBgEstqcdEnriched.tauSource = cms.InputTag('tausBgEstWtoTauNuqcdEnrichedEcalCrackVetoCumulative')

jetHistManagerBgEstqcdEnriched = copy.deepcopy(jetHistManager)
jetHistManagerBgEstqcdEnriched.pluginName = cms.string('jetHistManagerBgEstqcdEnriched')
tauHistManagerBgEstqcdEnriched.jetSource = cms.InputTag('jetsBgEstqcdEnrichedPt15')

tauNuCandidateHistManagerBgEstqcdEnriched = copy.deepcopy(tauNuCandidateHistManager)
tauNuCandidateHistManagerBgEstqcdEnriched.pluginName = cms.string('tauNuCandidateHistManagerBgEstqcdEnriched')
tauNuCandidateHistManagerBgEstqcdEnriched.tauNuCandidateSource = cms.InputTag('tauNuPairsBgEstqcdEnriched')

tauRecoilEnergyFromCaloTowersHistManagerBgEstqcdEnriched = copy.deepcopy(tauRecoilEnergyFromCaloTowersHistManager)
tauRecoilEnergyFromCaloTowersHistManagerBgEstqcdEnriched.pluginName = cms.string('tauRecoilEnergyFromCaloTowersHistManagerBgEstqcdEnriched')
tauRecoilEnergyFromCaloTowersHistManagerBgEstqcdEnriched.leptonRecoilEnergySource = cms.InputTag('tauRecoilEnergyFromCaloTowersBgEstqcdEnriched')

analyzeEventsBgEstWtoTauNuqcdEnriched = cms.EDAnalyzer("GenericAnalyzer",
                                                       name = cms.string('BgEstTemplateAnalyzer_qcdEnriched'),
                                                       
                                                       filters = cms.VPSet(
    evtSelPrimaryEventVertex,
    evtSelPrimaryEventVertexQuality,
    evtSelPrimaryEventVertexPosition,
    
    evtSelTauEta,
    evtSelTauPt,
    evtSelPFMetPt,
    evtSelTauLeadTrk,
    cms.PSet(
    pluginName = cms.string('tauLeadTrkPtCutBgEstqcdEnriched'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('tauLeadTrkPtCutBgEstqcdEnriched', 'cumulative')
    ),
    cms.PSet(
    pluginName = cms.string('tauTrkIsoCutBgEstqcdEnriched'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('tauTrkIsoCutBgEstqcdEnriched', 'cumulative')
    ),
    cms.PSet(
    pluginName = cms.string('tauEcalIsoCutBgEstqcdEnriched'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('tauEcalIsoCutBgEstqcdEnriched', 'cumulative')
    ),
    cms.PSet(
    pluginName = cms.string('tauProngCutBgEstqcdEnriched'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('tauProngCutBgEstqcdEnriched', 'cumulative')
    ),
    cms.PSet(
    pluginName = cms.string('tauChargeCutBgEstqcdEnriched'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('tauChargeCutBgEstqcdEnriched', 'cumulative')
    ),
    cms.PSet(
    pluginName = cms.string('tauMuonVetoCutBgEstqcdEnriched'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('tauMuonVetoCutBgEstqcdEnriched', 'cumulative')
    ),
    cms.PSet(
    pluginName = cms.string('tauElectronVetoCutBgEstqcdEnriched'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('tauElectronVetoCutBgEstqcdEnriched', 'cumulative')
    ),
    cms.PSet(
    pluginName = cms.string('tauEcalCrackVetoCutBgEstqcdEnriched'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('tauEcalCrackVetoCutBgEstqcdEnriched', 'cumulative')
    ),
    cms.PSet(
    pluginName = cms.string('centralJetVetoBgEstqcdEnriched'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('centralJetVetoBgEstqcdEnriched')
    ),
    cms.PSet(
    pluginName = cms.string('recoilEnergyCutBgEstqcdEnriched'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('recoilEnergyCutBgEstqcdEnriched')
    ),
    cms.PSet(
    pluginName = cms.string('metTopologyCutBgEstqcdEnriched'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('metTopologyCutBgEstqcdEnriched')
    )
    ),
                                                       analyzers = cms.VPSet(
    tauHistManagerBgEstqcdEnriched,
    jetHistManagerBgEstqcdEnriched,
    tauNuCandidateHistManagerBgEstqcdEnriched,
    tauRecoilEnergyFromCaloTowersHistManagerBgEstqcdEnriched,
    caloMEtHistManager,
    pfMEtHistManager,
    metTopologyHistManager
    ),
                                                       eventDumps = cms.VPSet(),
                                                       
                                                       analysisSequence = cms.VPSet(
    # vertex selection
    cms.PSet(
    filter = cms.string('evtSelPrimaryEventVertex'),
    title = cms.string('Vertex'),
    saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    filter = cms.string('evtSelPrimaryEventVertexQuality'),
    title = cms.string('p(chi2Vertex) > 0.01'),
    saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    filter = cms.string('evtSelPrimaryEventVertexPosition'),
    title = cms.string('-50 < Vertex < +50 cm'),
    saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    filter = cms.string('evtSelTauEta'),
    title = cms.string('-2.1 < eta(Tau) < +2.1'),
    saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    filter = cms.string('evtSelTauPt'),
    title = cms.string('25< Pt(Tau) < 60 GeV'),
    saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    filter = cms.string('evtSelPFMetPt'),
    title = cms.string('PF-MET > 30 GeV'),
    saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    filter = cms.string('evtSelTauLeadTrk'),
    title = cms.string('with leadtrk'),
    saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    filter = cms.string('tauLeadTrkPtCutBgEstqcdEnriched'),
    title = cms.string('15 < leadtrk pt < 20 GeV'),
    saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    filter = cms.string('tauEcalIsoCutBgEstqcdEnriched'),
    title = cms.string('inverse isolation'),
    saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    filter = cms.string('tauTrkIsoCutBgEstqcdEnriched'),
    title = cms.string('none'),
    saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    filter = cms.string('tauProngCutBgEstqcdEnriched'),
    title = cms.string('none'),
    saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    filter = cms.string('tauChargeCutBgEstqcdEnriched'),
    title = cms.string('none'),
    saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    filter = cms.string('tauMuonVetoCutBgEstqcdEnriched'),
    title = cms.string('Muon veto'),
    saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    filter = cms.string('tauElectronVetoCutBgEstqcdEnriched'),
    title = cms.string('Electron veto'),
    saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    filter = cms.string('tauEcalCrackVetoCutBgEstqcdEnriched'),
    title = cms.string('Ecal Crack veto'),
    saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    filter = cms.string('centralJetVetoBgEstqcdEnriched'),
    title = cms.string('central jet veto'),
    saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    analyzers = cms.vstring(
    'tauHistManagerBgEstqcdEnriched',
    'jetHistManagerBgEstqcdEnriched',
    'caloMEtHistManager',
    'pfMEtHistManager',
    'metTopologyHistManager',
    'tauNuCandidateHistManagerBgEstqcdEnriched',
    'tauRecoilEnergyFromCaloTowersHistManagerBgEstqcdEnriched'
    ),
    replace = cms.vstring('tauHistManagerBgEstqcdEnriched.tauSource = tausBgEstWtoTauNuqcdEnrichedEcalCrackVetoCumulative',
                          'tauRecoilEnergyFromCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCaloTowersBgEstqcdEnriched',
                          'tauNuCandidateHistManager.tauNuCandidateSource = tauNuPairsBgEstqcdEnriched')
    ),
    cms.PSet(
    filter = cms.string('recoilEnergyCutBgEstqcdEnriched'),
    title = cms.string('recoil energy (calotowers) < 10'),
    saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    analyzers = cms.vstring(
    'tauHistManagerBgEstqcdEnriched',
    'jetHistManagerBgEstqcdEnriched',
    'caloMEtHistManager',
    'pfMEtHistManager',
    'metTopologyHistManager',
    'tauNuCandidateHistManagerBgEstqcdEnriched',
    'tauRecoilEnergyFromCaloTowersHistManagerBgEstqcdEnriched'
    ),
    replace = cms.vstring('tauHistManagerBgEstqcdEnriched.tauSource = tausBgEstWtoTauNuqcdEnrichedEcalCrackVetoCumulative',
                          'tauRecoilEnergyFromCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCaloTowersBgEstqcdEnriched'
                          )
    ),
    cms.PSet(
    filter = cms.string('metTopologyCutBgEstqcdEnriched'),
    title = cms.string('MET-topology < 0.4'),
    saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    analyzers = cms.vstring(
    'tauHistManagerBgEstqcdEnriched',
    'jetHistManagerBgEstqcdEnriched',
    'caloMEtHistManager',
    'pfMEtHistManager',
    'metTopologyHistManager',
    'tauNuCandidateHistManagerBgEstqcdEnriched',
    'tauRecoilEnergyFromCaloTowersHistManagerBgEstqcdEnriched'
    ),
    replace = cms.vstring('tauHistManagerBgEstqcdEnriched.tauSource = tausBgEstWtoTauNuqcdEnrichedEcalCrackVetoCumulative',
                          'tauRecoilEnergyFromCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCaloTowersBgEstqcdEnriched',
                          'tauNuCandidateHistManager.tauNuCandidateSource = tauNuPairsBgEstqcdEnriched')
    )
    )
                                                       
                                                       )


bgEstqcdEnrichedAnalysisSequence = cms.Sequence(
    selectTausBgEstWtoTauNuqcdEnriched
    + selectJetsBgEstWplusJetsEnriched
    + selectTauNuPairsBgEstWtoTauNuqcdEnriched
    )
