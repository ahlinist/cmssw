import FWCore.ParameterSet.Config as cms
import copy

# import utility function for changing cut values
from TauAnalysis.Configuration.tools.changeCut import changeCut

# import configuration parameters of histogram manager
# for validation of tau id. efficiencies/fake-rates
from TauAnalysis.BgEstimationTools.tauIdEffValidationHistManager_cfi import tauIdEffValidationHistManager

# import utility function for add histogram manager to analysis sequence
from TauAnalysis.Configuration.tools.analysisSequenceTools import addAnalyzer, setAnalyzerParameter, pruneAnalysisSequence

from TauAnalysis.BgEstimationTools.tools.fakeRateProdTools import configureFakeRateProductionPAT, getFakeRateConfigParameters

#--------------------------------------------------------------------------------
# auxiliary functions needed for reconfiguration of analysis sequences
#--------------------------------------------------------------------------------

def makeGenAnalyzerModule(process, genAnalyzerModule, label):

    analyzer = copy.deepcopy(genAnalyzerModule)

    analyzer.name = cms.string("".join([getattr(genAnalyzerModule, "name").value(), "_", label]))

    analyzerName = "".join([genAnalyzerModule.label(), "_", label])
    setattr(process, analyzerName, analyzer)
    analyzer = getattr(process, analyzerName)

    return analyzer

def addGenAnalyzerModule(process, genAnalyzerModule, label, analysisSequence):

    analyzer = makeGenAnalyzerModule(process, genAnalyzerModule, label)

    # add module to sequence
    analysisSequence += analyzer

    return analysisSequence

def getBoolEventSelFlagProducer_src(module):

    src_cumulative = None
    src = None

    if hasattr(module, "selectors"):
        for pset in getattr(module, "selectors"):
            if getattr(pset, "instanceName").value() == 'cumulative':
                src_cumulative = getattr(pset, "src")
            elif getattr(pset, "instanceName").value() == '':
                src = getattr(pset, "src")
    else:
        raise ValueError("BoolEventSelFlagProducer module has no 'selectors' Parameter !!")

    if src_cumulative is not None:
        return src_cumulative
    else:
        return src

def addFakeRateGenAnalyzerModule(process, genAnalyzerModule, histManagers = [],
                                 tauCuts = None, diTauCuts = [], selTauByDiTauModuleType = None,
                                 frType = None, frConfig = None,
                                 bgEstFakeRateAnalysisSequence = None):

    # check that 'tauCuts', 'frType', 'frConfig'
    # and 'bgEstFakeRateAnalysisSequence' parameters are defined
    if tauCuts is None:
        raise ValueError("Undefined 'tauCuts' Parameter !!")
    if frType is None:
        raise ValueError("Undefined 'frType' Parameter !!")
    if frConfig is None:
        raise ValueError("Undefined 'frConfig' Parameter !!")
    if bgEstFakeRateAnalysisSequence is None:
        raise ValueError("Undefined 'bgEstFakeRateAnalysisSequence' Parameter !!")
    # check that 'selTauByDiTauModuleType' is defined
    if selTauByDiTauModuleType is None and len(diTauCuts) > 0:
        raise ValueError("Undefined 'selTauByDiTauModuleType' Parameter !!")

    bgEstFakeRateAnalyzerModule = makeGenAnalyzerModule(process, genAnalyzerModule, "fr" + "_" + frType)
    bgEstFakeRateAnalyzerName = getattr(bgEstFakeRateAnalyzerModule, "name").value()
    bgEstFakeRateAnalyzerName = bgEstFakeRateAnalyzerName.replace('_', 'X')
   
    cutsToUpdate = []
    cutsToUpdate.extend(tauCuts)
    cutsToUpdate.extend(diTauCuts)

    lastSelTauCollection = None

    if frConfig is not None:
        
        # check that cut is applied in analysis sequence of GenericAnalyzer module
        cutsToUpdate_applied = []
        for cutToUpdate in cutsToUpdate:
            isApplied = False
            analysisSequence = getattr(bgEstFakeRateAnalyzerModule, "analysisSequence")
            for pset in analysisSequence:
                if hasattr(pset, "filter") and getattr(pset, "filter").value() == cutToUpdate:
                    isApplied = True
            if isApplied:
                cutsToUpdate_applied.append(cutToUpdate)
            else:
                print("Cut = %s not applied in GenericAnalyzer module = %s --> skipping !!" % (cutToUpdate, bgEstFakeRateAnalyzerName))
            
        for cutIdx, cutToUpdate in enumerate(cutsToUpdate_applied):

            selTauCollectionName = None
            
            if cutToUpdate in tauCuts:
                evtSelPSet = getattr(process, cutToUpdate)
                boolEventFlagModuleName = None
                if hasattr(evtSelPSet, "src_cumulative"):
                    boolEventFlagModuleName = getattr(evtSelPSet, "src_cumulative").getModuleLabel()
                else:
                    boolEventFlagModuleName = getattr(evtSelPSet, "src").getModuleLabel()
                boolEventFlagModule = getattr(process, boolEventFlagModuleName)
                selTauCollectionName = getBoolEventSelFlagProducer_src(boolEventFlagModule).getModuleLabel()
                lastSelTauCollectionName = selTauCollectionName
              
            if cutToUpdate in diTauCuts:
                evtSelPSet = getattr(process, cutToUpdate)
                boolEventFlagModuleName = None
                if hasattr(evtSelPSet, "src_cumulative"):
                    boolEventFlagModuleName = getattr(evtSelPSet, "src_cumulative").getModuleLabel()
                else:
                    boolEventFlagModuleName = getattr(evtSelPSet, "src").getModuleLabel()
                boolEventFlagModule = getattr(process, boolEventFlagModuleName)
                selDiTauCollectionName = getBoolEventSelFlagProducer_src(boolEventFlagModule).getModuleLabel()
                
                selTauByDiTauModule = cms.EDProducer(selTauByDiTauModuleType,
                    srcDiTau = cms.InputTag(selDiTauCollectionName),                               
                    srcLeg2 = cms.InputTag(lastSelTauCollectionName),
                    dRmatchLeg2 = cms.double(0.3)                                          
                )
                selTauByDiTauModuleName = "selTauByDiTauX%sX%s" % (bgEstFakeRateAnalyzerName, cutToUpdate)
                setattr(process, selTauByDiTauModuleName, selTauByDiTauModule)
                bgEstFakeRateAnalysisSequence += selTauByDiTauModule

                selTauCollectionName = selTauByDiTauModuleName

            process.load("TauAnalysis.BgEstimationTools.fakeRateEventWeightProducer_cfi")
            bgEstFakeRateEventWeightModule = process.bgEstFakeRateEventWeights.clone(
                allTauJetSource = process.bgEstFakeRateJetWeights.allTauJetSource,
                preselTauJetSource = cms.InputTag(selTauCollectionName),
                method = process.bgEstFakeRateJetWeights.method,
                frTypes = process.bgEstFakeRateJetWeights.frTypes
            )
            bgEstFakeRateEventWeightModuleName = "bgEstFakeRateEventWeightsX%sX%s" % (bgEstFakeRateAnalyzerName, cutToUpdate)
            setattr(process, bgEstFakeRateEventWeightModuleName, bgEstFakeRateEventWeightModule)
            bgEstFakeRateAnalysisSequence += bgEstFakeRateEventWeightModule

            psetFakeRateEventWeight = cms.PSet(
                src = cms.InputTag(bgEstFakeRateEventWeightModuleName, frConfig['srcEventWeight'].getProductInstanceLabel()),
                applyAfterFilter = cms.string(cutToUpdate)
            )
            if cutIdx < (len(cutsToUpdate_applied) - 1):
                setattr(psetFakeRateEventWeight, "applyBeforeFilter", cms.string(cutsToUpdate_applied[cutIdx + 1]))
            if hasattr(bgEstFakeRateAnalyzerModule, "eventWeights"):
                getattr(bgEstFakeRateAnalyzerModule, "eventWeights").append(psetFakeRateEventWeight)
            else:
                setattr(bgEstFakeRateAnalyzerModule, "eventWeights", cms.VPSet(psetFakeRateEventWeight))

            # add event weights for tau id. efficiency shifted up/down by one sigma
            for shift in [ "SysTauIdEffUp", "SysTauIdEffDown" ]:
                if hasattr(process, "bgEstFakeRateJetWeights" + shift):
                    bgEstFakeRateEventWeightModuleSysTauIdEff = bgEstFakeRateEventWeightModule.clone(
                        shiftTauIdEff = getattr(process, "bgEstFakeRateJetWeights" + shift).shiftTauIdEff
                    )
                    bgEstFakeRateEventWeightModuleNameSysTauIdEff = "%s%s" % (bgEstFakeRateEventWeightModuleName, shift)
                    setattr(process, bgEstFakeRateEventWeightModuleNameSysTauIdEff, bgEstFakeRateEventWeightModuleSysTauIdEff)
                    bgEstFakeRateAnalysisSequence += bgEstFakeRateEventWeightModuleSysTauIdEff

        srcFakeRateJetWeight = cms.vstring(frConfig['patLabel'])
        for histManagerName, srcParamName in histManagers.items():
            setAnalyzerParameter(bgEstFakeRateAnalyzerModule, histManagerName, srcParamName, srcFakeRateJetWeight)

    srcFakeRateJetWeight = cms.vstring(frConfig['patLabel'])
    for histManagerName, srcParamName in histManagers.items():
        setAnalyzerParameter(bgEstFakeRateAnalyzerModule, histManagerName, srcParamName, srcFakeRateJetWeight)        

    # add module to sequence
    bgEstFakeRateAnalysisSequence += bgEstFakeRateAnalyzerModule

def makeDataBinningDumpSequence(process, channel,
                                dqmDirectory, processSubDirectories, frSubDirectories, moduleLabel):

    dataBinningDumpAnalysisSequence = None

    for processName, processSubDirectory in processSubDirectories.items():

        module = cms.EDAnalyzer("DQMDumpBinningResults",
            binningService = cms.PSet(
                pluginType = cms.string("DataBinningService"),
                dqmDirectories = cms.PSet()
            )
        )

        for frType, frSubDirectory in frSubDirectories.items():

            dqmDirectory_i = dqmDirectory
            dqmDirectory_i = dqmDirectory_i.replace("#PROCESSDIR#", processSubDirectory.value())
            dqmDirectory_i = dqmDirectory_i.replace("#FAKERATEDIR#", frSubDirectory)

            setattr(module.binningService.dqmDirectories, frType, cms.string(dqmDirectory_i))

        moduleName = "".join(["dumpDataBinningBgEstFakeRate", channel, "_", processName, "_", moduleLabel])
        setattr(process, moduleName, module)

        module = getattr(process, moduleName)

        if dataBinningDumpAnalysisSequence is None:
            dataBinningDumpAnalysisSequence = module
        else:
            dataBinningDumpAnalysisSequence *= module

    return dataBinningDumpAnalysisSequence

def makeFilterStatTableDumpSequence(process, channel,
                                    dqmDirectory, processSubDirectories, frSubDirectories, moduleLabel):

    filterStatTableDumpAnalysisSequence = None

    for processName, processSubDirectory in processSubDirectories.items():

        module = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
            dqmDirectories = cms.PSet(),
            #columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency"),
            columnsSummaryTable = cms.vstring("Passed"),
            printSummaryTableOnly = cms.bool(True)
        )

        for frType, frSubDirectory in frSubDirectories.items():

            dqmDirectory_i = dqmDirectory
            dqmDirectory_i = dqmDirectory_i.replace("#PROCESSDIR#", processSubDirectory.value())
            dqmDirectory_i = dqmDirectory_i.replace("#FAKERATEDIR#", frSubDirectory)

            setattr(module.dqmDirectories, frType, cms.string(dqmDirectory_i))

        moduleName = "".join(["dumpFilterStatTableBgEstFakeRate", channel, "_", processName, "_", moduleLabel])
        setattr(process, moduleName, module)

        module = getattr(process, moduleName)

        if filterStatTableDumpAnalysisSequence is None:
            filterStatTableDumpAnalysisSequence = module
        else:
            filterStatTableDumpAnalysisSequence *= module

    return filterStatTableDumpAnalysisSequence

#--------------------------------------------------------------------------------
# auxiliary functions needed for reconfiguration of DQM file loader modules
#--------------------------------------------------------------------------------

def reconfigDQMFileLoader(dqmFileLoaderConfig, dqmDirectory):

    # configure attributes of DQMFileLoader module
    # corresponding to different signal/background processes
    for processName in dir(dqmFileLoaderConfig):
        processConfigEntry = getattr(dqmFileLoaderConfig, processName)

        if isinstance(processConfigEntry, cms.PSet):

            if hasattr(processConfigEntry, "dqmDirectory_store"):
                if not dqmDirectory.endswith("/"):
                    dqmDirectory += "/"

                dqmDirectory_old = getattr(processConfigEntry, "dqmDirectory_store").value()
                dqmDirectory_new = dqmDirectory.replace("#PROCESSDIR#", dqmDirectory_old)

                setattr(processConfigEntry, "dqmDirectory_store", dqmDirectory_new)

#--------------------------------------------------------------------------------
# auxiliary functions needed for adding fake-rate producer modules
# to PAT production sequence (**not** channel specific)
#--------------------------------------------------------------------------------

def enableFakeRatesImpl(process, method = None,
                        preselPatTauSource = None, tauIds = None,
                        patTupleProdSequenceName = None,
                        analyzers = None, histManagers = [], tauCuts = None, diTauCuts = [],
                        selTauByDiTauModuleType = None,
                        frSet = 'ewkTauIdHPSloose', 
                        recoTauProducerName = 'hpsPFTauProducer', recoTauPreselFlag = 'ewkTauId',
                        patTauProducerName = 'patTaus',
                        prePatProdSequenceName = "producePrePat"):

    # check validity of method parameter
    if method is None:
        raise ValueError("Undefined method Parameter !!")
    else:
        if method != "simple" and method != "CDF":
            raise ValueError("Invalid method Parameter !!")

    # compute fake-rates weights and add computed values to pat::Taus
    configureFakeRateProductionPAT(process, recoTauProducerName, recoTauPreselFlag,
                                   patTauProducerName, frSet, method, prePatProdSequenceName)

    # disable cuts on tau id. discriminators
    #
    # NOTE: tau lead. track finding and lead. track Pt discriminators
    #       must **not** be disabled, as these discriminators are already applied at the skimming stage !!
    #       Instead, need to apply TauAnalysis specific efficiency/fake-rate values,
    #       which represent the probability for a tau-jet candidate
    #       passing the lead. track finding and lead. track Pt discriminators
    #       to pass the track && ECAL isolation, 1||3 tracks in signal cone and charge = +/- 1 requirements as well
    #
    for source, new_cut in tauIds.items():
        print "FR method: Disabling tauID cut: %s --> %s for fake rate method" % (
            source, new_cut)
        changeCut(process, source, new_cut)

    # get list of fake-rates types to be processed
    frConfigParameters = getFakeRateConfigParameters(process)

    # Fill histograms only for events passing all event selection criteria
    for genAnalyzerName in analyzers:
        if not hasattr(process, genAnalyzerName):
            print "FR method: Can't add fake rate to analysis sequence %s" % genAnalyzerName, \
                  " it does not exist in the process !!"
            continue
        genAnalyzerModule = getattr(process, genAnalyzerName)
        print "FR method: Removing extra plots for analyzer: %s" % genAnalyzerName
        pruneAnalysisSequence(genAnalyzerModule)
        print "FR method: Disabling event dumps"
        if len(genAnalyzerModule.eventDumps):
            disabledEventDump = copy.deepcopy(genAnalyzerModule.eventDumps[0])
            disabledEventDump.output = cms.string("std::cout")
            disabledEventDump.triggerConditions = cms.vstring()
            genAnalyzerModule.eventDumps[0] = disabledEventDump

        bgEstFakeRateAnalysisSequence = cms.Sequence()

        # for each type of fake-rate weights given as function argument,
        # duplicate analysis sequence with:
        #
        #  1.) tau id. discriminators not applied
        #  2.) events weighted by fake-rate instead
        #
        # NOTE: special care is needed to avoid double-counting in case there is
        #       more than one (loosely selected) tau-jet candidate in the event when
        #       filling histograms that are sensitive to the tau-jet multiplicity
        #
        for frType, frConfig in frConfigParameters.items():
            print "FR method: Building frType:", frType
            # check if factorization is enabled; if so, apply fake-rate event
            # weights to analysis paths without/with muon isolation, else apply
            # fake-rate event weights to "standard" analysis path
            addFakeRateGenAnalyzerModule(process, genAnalyzerModule, histManagers, tauCuts, diTauCuts, selTauByDiTauModuleType,
                                         frType, frConfig, bgEstFakeRateAnalysisSequence)

        # for the purpose of making control plots for the data sample
        # selected in case no tau id. criteria are applied,
        # add analysis sequence with:
        #
        #  1.) tau id. discriminators not applied
        #  2.) events **not** weighted by fake-rate instead
        #
        print "FR method: Adding unweighted sequence"
        addGenAnalyzerModule(process, genAnalyzerModule, "frUnweighted", bgEstFakeRateAnalysisSequence)

        # in case method is "simple",
        # add analysis sequence with:
        #
        #  1.) with tau id. discriminators not applied
        #  2.) events weighted by tau id. efficiency
        #
        # for the purpose of checking the tau id. efficiency values
        # which are used by the "CDF" method.
        if method == "simple":
            process.bgEstFakeRateJetWeights.frTypes.tauIdEfficiency = cms.PSet(
                tauJetDiscriminators = cms.VPSet(cms.PSet(
                    tauJetIdEffSource = cms.InputTag(frProdConfig['ZTTsim']),
                    qcdJetFakeRateSource = cms.InputTag(frProdConfig['ZTTsim']),
                    tauJetDiscrSource = cms.InputTag("ewkTauId")
                ))
            )
            frConfig[frType] = {}
            frConfig[frType]['srcJetWeight']   = cms.InputTag("bgEstFakeRateJetWeights",   "tauIdEfficiency")
            frConfig[frType]['patLabel']       = "".join(["bgEstFakeRateJetWeight", "_", "tauIdEfficiency"])
            print "FR method: Using simple weights, adding ZTT eff"
            addFakeRateGenAnalyzerModule(process, genAnalyzerModule, histManagers, tauCuts, diTauCuts, selTauByDiTauModuleType,
                                         "tauIdEfficiency", frConfig, bgEstFakeRateAnalysisSequence)

        bgEstFakeRateAnalysisSequenceName = "bgEstFakeRateAnalysisSequenceX%s" % genAnalyzerName
        setattr(process, bgEstFakeRateAnalysisSequenceName, bgEstFakeRateAnalysisSequence)
        print "FR method: Replacing %s by the new sequence %s" % (genAnalyzerName, bgEstFakeRateAnalysisSequenceName)
        process.p.replace(genAnalyzerModule, bgEstFakeRateAnalysisSequence)

    # enable checking of fake-rates and tau id. efficiencies
    # with event weights in tau-jet histogram manager
    print "FR method: Enabling weight consistency check"
    setattr(process.tauHistManager, "checkWeightConsistency", cms.bool(True))

#--------------------------------------------------------------------------------
# utility functions specific to application of fake-rate technique
# for data-driven background estimation to Z --> mu + tau-jet channel
#
# NOTE: in case factorization of the muon isolation efficiency
#       is used in order to improve the statistical precision of Monte Carlo estimates,
#       the function enableFakeRates_runZtoMuTau needs to be called **after**
#       the function enableFactorization_runZtoMuTau has been called
#
#--------------------------------------------------------------------------------

_FAKE_RATE_CONFIGS = {
    'ZtoMuTau' : {
        'preselPatTauSource' : 'selectedPatTausForMuTauLeadTrkPtCumulative',
        'tauIds' : {
            "selectedPatTausForMuTauTaNCdiscr" : \
              "tauID('leadingTrackFinding') > -1.",
            "selectedPatTausForMuTauProng"     : \
              "signalPFChargedHadrCands.size() > -1",
            "selectedPatTausForMuTauCharge"    : \
              "abs(charge) > -1",
            "selectedMuTauPairsZeroCharge"     : \
              "leg2.leadPFChargedHadrCand.isNonnull & (leg1.charge + leg2.leadPFChargedHadrCand.charge) = 0",
        },
        'patTupleProdSequenceName' : 'producePatTupleZtoMuTauSpecific',
        'analyzers' : [
            "analyzeZtoMuTauEventsOS_factorizedWithoutMuonIsolation",
            "analyzeZtoMuTauEventsOS_factorizedWithMuonIsolation",
            "analyzeZtoMuTauEventsOS",
            "analyzeZtoMuTauEventsSS_factorizedWithoutMuonIsolation",
            "analyzeZtoMuTauEventsSS_factorizedWithMuonIsolation",
            "analyzeZtoMuTauEventsSS"
        ],
        'histManagers' : {
            "tauHistManager" : "tauJetWeightSource",
            "diTauCandidateZmumuHypothesisHistManagerForMuTau" : "lepton2WeightSource", 
            "diTauCandidateHistManagerForMuTau" : "diTauLeg2WeightSource",
            "diTauCandidateSVfitHistManagerForMuTau" : "diTauLeg2WeightSource"
        },
        'tauCuts' : [
            'evtSelTauTaNCdiscr',
            'evtSelTauProng',
            'evtSelTauCharge',
            'evtSelTauMuonVeto',
            'evtSelTauElectronVeto'

        ],
        'diTauCuts' : [
            'evtSelDiTauCandidateForMuTauAntiOverlapVeto',
            'evtSelDiTauCandidateForMuTauMt1MET',
            'evtSelDiTauCandidateForMuTauPzetaDiff',
            'evtSelDiTauCandidateForMuTauZeroCharge',
            'evtSelDiTauCandidateForMuTauNonZeroCharge'

        ],
        'selTauByDiTauModuleType' : 'PATLeptonByMuTauPairMatchSelector',
        'frSet' : 'ewkTauIdHPSloose'
        #'frSet' : 'ewkTauIdTaNCloose'
    },
    'AHtoMuTau' : {
        'preselPatTauSource' : 'selectedPatTausForMuTauLeadTrkPtCumulative',
        'tauIds' : {
            "selectedPatTausForMuTauTaNCdiscr"            : \
              "tauID('leadingTrackFinding') > -1.",
            "selectedPatTausForMuTauProng"                : \
              "signalPFChargedHadrCands.size() > -1",
            "selectedPatTausForMuTauCharge"               : \
              "abs(charge) > -1",
            "selectedMuTauPairsForAHtoMuTauZeroCharge"    : \
              "leg2.leadPFChargedHadrCand.isNonnull & (leg1.charge + leg2.leadPFChargedHadrCand.charge) = 0",
            "selectedMuTauPairsForAHtoMuTauNonZeroCharge" : \
              "leg2.leadPFChargedHadrCand.isNonnull & (leg1.charge + leg2.leadPFChargedHadrCand.charge) != 0"
        },
        'patTupleProdSequenceName' : 'producePatTupleAHtoMuTauSpecific',
        'analyzers' : [
            "analyzeAHtoMuTauEventsOS_woBtag_factorizedWithoutMuonIsolation",
            "analyzeAHtoMuTauEventsOS_woBtag_factorizedWithMuonIsolation",
            "analyzeAHtoMuTauEventsOS_woBtag",
            "analyzeAHtoMuTauEventsOS_wBtag_factorizedWithoutMuonIsolation",
            "analyzeAHtoMuTauEventsOS_wBtag_factorizedWithMuonIsolation",
            "analyzeAHtoMuTauEventsOS_wBtag",
            "analyzeAHtoMuTauEventsSS_woBtag_factorizedWithoutMuonIsolation",
            "analyzeAHtoMuTauEventsSS_woBtag_factorizedWithMuonIsolation",
            "analyzeAHtoMuTauEventsSS_woBtag",
            "analyzeAHtoMuTauEventsSS_wBtag_factorizedWithoutMuonIsolation",
            "analyzeAHtoMuTauEventsSS_wBtag_factorizedWithMuonIsolation",
            "analyzeAHtoMuTauEventsSS_wBtag"
        ],
        'histManagers' : {
            "tauHistManager" : "tauJetWeightSource",
            "diTauCandidateZmumuHypothesisHistManagerForMuTau" : "lepton2WeightSource", 
            "diTauCandidateHistManagerForMuTau" : "diTauLeg2WeightSource",
            "diTauCandidateSVfitHistManagerForMuTau" : "diTauLeg2WeightSource"
        },
        'tauCuts' : [
            'evtSelTauTaNCdiscr',
            'evtSelTauProng',
            'evtSelTauCharge',
            'evtSelTauMuonVeto',
            'evtSelTauElectronVeto'

        ],
        'diTauCuts' : [
            'evtSelDiTauCandidateForAHtoMuTauAntiOverlapVeto',
            'evtSelDiTauCandidateForAHtoMuTauMt1MET',
            'evtSelDiTauCandidateForAHtoMuTauPzetaDiff',
            'evtSelDiTauCandidateForAHtoMuTauZeroCharge',
            'evtSelDiTauCandidateForAHtoMuTauNonZeroCharge'

        ],
        'selTauByDiTauModuleType' : 'PATLeptonByMuTauPairMatchSelector',
        'frSet' : 'ewkTauIdHPSloose'
        #'frSet' : 'ewkTauIdTaNCloose'
    }
}

def enableFakeRates(process, channel, method = None):
    enableFakeRatesImpl(process, method, **_FAKE_RATE_CONFIGS[channel])
