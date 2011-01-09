import FWCore.ParameterSet.Config as cms
import copy

# import utility function for changing cut values
from TauAnalysis.Configuration.tools.changeCut import changeCut

# import configuration parameters of histogram manager
# for validation of tau id. efficiencies/fake-rates
from TauAnalysis.BgEstimationTools.tauIdEffValidationHistManager_cfi import tauIdEffValidationHistManager

# import utility function for add histogram manager to analysis sequence
from TauAnalysis.Configuration.tools.analysisSequenceTools import addAnalyzer

from TauAnalysis.BgEstimationTools.tools.fakeRateProdTools import configureFakeRateProduction

#--------------------------------------------------------------------------------
# auxiliary functions needed for reconfiguration of analysis sequences
#--------------------------------------------------------------------------------

def setAnalyzerParameter(genAnalyzerModule, pluginName, parameterName, parameterValue):
    for analyzerPlugin in genAnalyzerModule.analyzers:
        if hasattr(analyzerPlugin, "pluginName"):
            analyzerPluginName = getattr(analyzerPlugin, "pluginName").value()
            if analyzerPluginName == pluginName:
                setattr(analyzerPlugin, parameterName, parameterValue)

def pruneAnalysisSequence(genAnalyzerModule):

    # disable filling of histograms after all stages of the event selection
    # except for the last occurence (after all cuts have been applied)

    lastEntry = {}
    for iAnalysisSequenceEntry in range(len(genAnalyzerModule.analysisSequence)):
        analysisSequenceEntry = genAnalyzerModule.analysisSequence[iAnalysisSequenceEntry]
        if hasattr(analysisSequenceEntry, "analyzers"):
            analyzerPlugins = getattr(analysisSequenceEntry, "analyzers")
            for analyzerPlugin in analyzerPlugins:
                analyzerPluginName = analyzerPlugin
                lastEntry[analyzerPluginName] = iAnalysisSequenceEntry

    prunedAnalysisSequence = []
    for iAnalysisSequenceEntry in range(len(genAnalyzerModule.analysisSequence)):
        analysisSequenceEntry = genAnalyzerModule.analysisSequence[iAnalysisSequenceEntry]
        if hasattr(analysisSequenceEntry, "analyzers"):
            # keep analyzer entry only in case it contains at least one histogram manager
            # not filled at a later stage of the event selection

            keepAnalyzers = []
            for lastEntryKey, lastEntryValue in lastEntry.items():
                if lastEntryValue == iAnalysisSequenceEntry:
                    keepAnalyzers.append(lastEntryKey)

            if len(keepAnalyzers) > 0:
                # keep analysis sequence entry, but fill only histograms
                # which are not filled at a later stage of the event selection
                analysisSequenceEntry.analyzers = cms.vstring(keepAnalyzers)

                # in all cases, disable storing run and events numbers
                setattr(genAnalyzerModule.analysisSequence[iAnalysisSequenceEntry], "saveRunLumiSectionEventNumbers", cms.vstring())

                prunedAnalysisSequence.append(analysisSequenceEntry)
        else:
            # keep all filter entries,
            # but disable saving of run and event numbers of events passing filter
            setattr(analysisSequenceEntry, "saveRunLumiSectionEventNumbers", cms.vstring(''))
            prunedAnalysisSequence.append(analysisSequenceEntry)

    genAnalyzerModule.analysisSequence = cms.VPSet(prunedAnalysisSequence)

def disableEventDump(genAnalyzerModule):
    # disable event print-out
    if not len(genAnalyzerModule.eventDumps):
        print "-- already disabled!"
        return
    disabledEventDump = copy.deepcopy(genAnalyzerModule.eventDumps[0])
    disabledEventDump.output = cms.string("std::cout")
    disabledEventDump.triggerConditions = cms.vstring()
    genAnalyzerModule.eventDumps[0] = disabledEventDump

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
    if analysisSequence is None:
        analysisSequence = analyzer
    else:
        analysisSequence *= analyzer

    return analysisSequence

def addFakeRateGenAnalyzerModule(process, genAnalyzerModule, frType, frConfig, bgEstFakeRateAnalysisSequence):

    bgEstFakeRateAnalyzer = makeGenAnalyzerModule(process, genAnalyzerModule, "fr" + "_" + frType)

    if frConfig is not None:
        psetFakeRateEventWeight = cms.PSet(
            src = frConfig['srcEventWeight'],
            applyAfterFilter = cms.string("evtSelTauTaNCdiscr")
        )
        if hasattr(bgEstFakeRateAnalyzer, "eventWeights"):
            getattr(bgEstFakeRateAnalyzer, "eventWeights").append(psetFakeRateEventWeight)
        else:
            setattr(bgEstFakeRateAnalyzer, "eventWeights", cms.VPSet(psetFakeRateEventWeight))

        srcFakeRateJetWeight = cms.vstring(frConfig['patLabel'])
        setAnalyzerParameter(bgEstFakeRateAnalyzer,
                             "tauHistManager", "tauJetWeightSource", srcFakeRateJetWeight)
        setAnalyzerParameter(bgEstFakeRateAnalyzer,
                             "diTauCandidateZmumuHypothesisHistManagerForMuTau", "lepton2WeightSource", srcFakeRateJetWeight)
        setAnalyzerParameter(bgEstFakeRateAnalyzer,
                             "diTauCandidateHistManagerForMuTau", "diTauLeg2WeightSource", srcFakeRateJetWeight)
        setAnalyzerParameter(bgEstFakeRateAnalyzer,
                             "diTauCandidateSVfitHistManagerForMuTau", "diTauLeg2WeightSource", srcFakeRateJetWeight)

    # add module to sequence
    if bgEstFakeRateAnalysisSequence is None:
        bgEstFakeRateAnalysisSequence = bgEstFakeRateAnalyzer
    else:
        bgEstFakeRateAnalysisSequence *= bgEstFakeRateAnalyzer

    return bgEstFakeRateAnalysisSequence

def makeDataBinningDumpSequence(process, dqmDirectory, processSubDirectories, frSubDirectories, moduleLabel):

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

        moduleName = "".join(["dumpDataBinningBgEstFakeRateZtoMuTau", "_", processName, "_", moduleLabel])
        setattr(process, moduleName, module)

        module = getattr(process, moduleName)

        if dataBinningDumpAnalysisSequence is None:
            dataBinningDumpAnalysisSequence = module
        else:
            dataBinningDumpAnalysisSequence *= module

    return dataBinningDumpAnalysisSequence

def makeFilterStatTableDumpSequence(process, dqmDirectory, processSubDirectories, frSubDirectories, moduleLabel):

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

        moduleName = "".join(["dumpFilterStatTableBgEstFakeRateZtoMuTau", "_", processName, "_", moduleLabel])
        setattr(process, moduleName, module)

        module = getattr(process, moduleName)

        if filterStatTableDumpAnalysisSequence is None:
            filterStatTableDumpAnalysisSequence = module
        else:
            filterStatTableDumpAnalysisSequence *= module

    return filterStatTableDumpAnalysisSequence

def getPSetAttributes(object):

    attributes = []

    for attribute in dir(object):

        # check that "attribute" is not an internal attribute or method of cms.PSet
        isInternalAttribute = False

        for classAttribute in dir(cms.PSet):
            if attribute == classAttribute:
                isInternalAttribute = True
        if attribute.startswith("_"):
            isInternalAttribute = True

        if not isInternalAttribute:
            attributes.append(attribute)

    return attributes

def getFakeRateConfigParameters(process):

    frConfig = {}

    frTypes = getPSetAttributes(process.bgEstFakeRateJetWeights.frTypes)

    for frType in frTypes:
        frConfig[frType] = {}
        frConfig[frType]['srcJetWeight']   = cms.InputTag("bgEstFakeRateJetWeights",   frType)
        frConfig[frType]['srcEventWeight'] = cms.InputTag("bgEstFakeRateEventWeights", frType)
        frConfig[frType]['patLabel']       = "".join(["bgEstFakeRateJetWeight", "_", frType])

        for shift in [ "SysTauIdEffUp", "SysTauIdEffDown" ]:
            if hasattr(process, "bgEstFakeRateJetWeights" + shift):
                frConfig[frType + shift] = {}
                frConfig[frType + shift]['srcJetWeight']   = cms.InputTag("bgEstFakeRateJetWeights"   + shift, frType)
                frConfig[frType + shift]['srcEventWeight'] = cms.InputTag("bgEstFakeRateEventWeights" + shift, frType)
                frConfig[frType + shift]['patLabel']       = "".join(["bgEstFakeRateJetWeight", "_", frType + shift])

    return frConfig

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

def configureFakeRateWeightProduction(
    process, method = None,
    preselPFTauJetSource = 'hpsTancTaus', frSet = 'ewkTauIdTaNCmedium',
    preselPatTauSource = 'selectedPatTausForMuTauLeadTrkPtCumulative',
    addPatTauPreselection = "tauID('againstElectron') > 0.5 & tauID('againstMuon') > 0.5",
    patTupleProdSequenceName = "producePatTupleZtoMuTauSpecific"):

    # check validity of method parameter
    if method is None:
        raise ValueError("Undefined method Parameter !!")
    else:
        if method != "simple" and method != "CDF":
            raise ValueError("Invalid method Parameter !!")

    # produce fake-rates
    frProdConfig = configureFakeRateProduction(process, preselPFTauJetSource, frSet)
    process.producePrePat += process.tauFakeRates

    # compute fake-rate weights
    #
    # NOTE: jet weights are computed for all (shrinking signal cone) reco::PFTaus,
    #       but only those tau-jet candidates passing preselection on PAT level
    #       must enter event weight computation !!
    #
    process.load("TauAnalysis.BgEstimationTools.fakeRateJetWeightProducer_cfi")
    process.bgEstFakeRateJetWeights.frTypes = cms.PSet()
    for frType, patLUT in frProdConfig.items():
        pset = cms.PSet(
            tauJetDiscriminators = cms.VPSet(cms.PSet(
                tauJetIdEffSource = cms.InputTag(frProdConfig['ZTTsim']),
            qcdJetFakeRateSource = cms.InputTag(patLUT),
                tauJetDiscrSource = cms.InputTag('ewkTauId')
            ))
        )
        setattr(process.bgEstFakeRateJetWeights.frTypes, frType, pset)
    process.bgEstFakeRateJetWeights.allTauJetSource = cms.InputTag(preselPFTauJetSource)
    process.bgEstFakeRateJetWeights.preselTauJetSource = cms.InputTag(preselPFTauJetSource)
    process.bgEstFakeRateJetWeights.method = method
    process.producePrePat += process.bgEstFakeRateJetWeights

    # add jet weights computed for tau id. efficiency shifted up/down by 30%
    if method == "CDF":
        process.bgEstFakeRateJetWeightsSysTauIdEffUp = copy.deepcopy(process.bgEstFakeRateJetWeights)
        process.bgEstFakeRateJetWeightsSysTauIdEffUp.shiftTauIdEff = cms.double(+0.30)
        process.producePrePat += process.bgEstFakeRateJetWeightsSysTauIdEffUp

        process.bgEstFakeRateJetWeightsSysTauIdEffDown = copy.deepcopy(process.bgEstFakeRateJetWeights)
        process.bgEstFakeRateJetWeightsSysTauIdEffDown.shiftTauIdEff = cms.double(-0.30)
        process.producePrePat += process.bgEstFakeRateJetWeightsSysTauIdEffDown

    process.tausForFakeRateEventWeights = cms.EDFilter("PATTauSelector",
        src = cms.InputTag(preselPatTauSource),
        cut = cms.string(addPatTauPreselection),
        filter = cms.bool(False)
    )

    process.load("TauAnalysis.BgEstimationTools.fakeRateEventWeightProducer_cfi")
    process.bgEstFakeRateEventWeights.allTauJetSource = cms.InputTag(preselPFTauJetSource)
    process.bgEstFakeRateEventWeights.preselTauJetSource = cms.InputTag('tausForFakeRateEventWeights')
    process.bgEstFakeRateEventWeights.method = method
    process.bgEstFakeRateEventWeights.frTypes = process.bgEstFakeRateJetWeights.frTypes
    process.produceFakeRateEventWeights = cms.Sequence(process.tausForFakeRateEventWeights + process.bgEstFakeRateEventWeights)
    patTupleProdSequence = getattr(process, patTupleProdSequenceName)
    patTupleProdSequence += process.produceFakeRateEventWeights

    # add event weights for tau id. efficiency shifted up/down by 30%
    for shift in [ "SysTauIdEffUp", "SysTauIdEffDown" ]:
        if hasattr(process, "bgEstFakeRateJetWeights" + shift):
            frEventWeightProducerModule = copy.deepcopy(process.bgEstFakeRateEventWeights)
            frEventWeightProducerModule.shiftTauIdEff = \
              getattr(process, "bgEstFakeRateJetWeights" + shift).shiftTauIdEff
            frEventWeightProducerModuleName = "bgEstFakeRateEventWeights" + shift
            setattr(process, frEventWeightProducerModuleName, frEventWeightProducerModule)
            process.produceFakeRateEventWeights += frEventWeightProducerModule

    # add fake-rates to pat::Tau
    frConfigParameters = getFakeRateConfigParameters(process)
    for frType, frConfig in frConfigParameters.items():
        setattr(process.patTaus.efficiencies, frConfig['patLabel'], frConfig['srcJetWeight'])
    process.patTaus.addEfficiencies = cms.bool(True)

    return frProdConfig

def enableFakeRatesImpl(process, method=None, analyzers=None,
                        frSet = None, tau_ids=None,
                        patTupleProdSeq = None):
    # check validity of method parameter
    if method is None:
        raise ValueError("Undefined method Parameter !!")
    else:
        if method != "simple" and method != "CDF":
            raise ValueError("Invalid method Parameter !!")

    # compute fake-rate weights
    frProdConfig = configureFakeRateWeightProduction(
        process, method = method,
        frSet = frSet,
        patTupleProdSequenceName = patTupleProdSeq)

    # disable cuts on tau id. discriminators
    #
    # NOTE: tau lead. track finding and lead. track Pt discriminators
    #       must **not** be disabled, as these discriminators are already applied at the skimming stage !!
    #       Instead, need to apply TauAnalysis specific efficiency/fake-rate values,
    #       which represent the probability for a tau-jet candidate
    #       passing the lead. track finding and lead. track Pt discriminators
    #       to pass the track && ECAL isolation, 1||3 tracks in signal cone and charge = +/- 1 requirements as well
    #
    for source, new_cut in tau_ids:
        print "FR method: Disabling tauID cut: %s --> %s for fake rate method" % (
            source, new_cut)
        changeCut(process, source, new_cut)

    # get list of fake-rates types to be processed
    frConfigParameters = getFakeRateConfigParameters(process)
    bgEstFakeRateAnalysisSequence = None

    # Fill histograms only for events passing all event selection criteria
    for analyzer in analyzers:
        if not hasattr(process, analyzer):
            print "FR method: Can't add fake rate to analysis sequence %s" % analyzer, \
                    " it doesn't exist in the process!"
            continue
        analyzer_sequence = getattr(process, analyzer)
        print "FR method: Removing extra plots for analyzer: %s" % analyzer
        pruneAnalysisSequence(analyzer_sequence)
        print "FR method: Disabling event dumps for analyzer: %s" % analyzer
        disableEventDump(analyzer_sequence)

        # duplicate analysis sequence:
        #  1.) tau id. discriminators not applied
        #  2.) events weighted by fake-rate
        #
        # for each type of fake-rate weights given as function argument
        # Note: special care is needed to avoid double-counting in case there is
        # more than one (loosely selected) tau-jet candidate in the event when
        # filling histograms that are sensitive to the tau-jet multiplicity
        for frType, frConfig in frConfigParameters.items():
            print "FR method: Building frType:", frType
            # check if factorization is enabled; if so, apply fake-rate event
            # weights to analysis paths without/with muon isolation, else apply
            # fake-rate event weights to "standard" analysis path
            bgEstFakeRateAnalysisSequence = \
              addFakeRateGenAnalyzerModule(
                  process, analyzer_sequence,
                  frType, frConfig, bgEstFakeRateAnalysisSequence)

        # add analysis sequence: 1.) with tau id. discriminators not applied 2.)
        # events **not** weighted by fake-rate (for the purpose of making
        # control plots for the data sample from which contributions of
        # individual background processes are estimated via the fake-rate
        # technique)
        print "FR method: Adding unweighted sequence"
        bgEstFakeRateAnalysisSequence = \
          addGenAnalyzerModule(process, analyzer_sequence,
                               "frUnweighted", bgEstFakeRateAnalysisSequence)
        # if method is "simple", add one more analysis sequence:
        #  1.) with tau id. discriminators not applied
        #  2.) events weighted by tau id. efficiency
        # (for the purpose of checking the tau id. efficiency values
        #  which are used by the "CDF" method)
        if method == "simple":
            # The extra frType is stored in an unused PSet in the jet fake rate
            # weight producer
            process.bgEstFakeRateJetWeights.frTypes.tauIdEfficiency = cms.PSet(
                tauJetDiscriminators = cms.VPSet(cms.PSet(
                    tauJetIdEffSource = cms.InputTag(frProdConfig['ZTTsim']),
                    qcdJetFakeRateSource = cms.InputTag(frProdConfig['ZTTsim']),
                    tauJetDiscrSource = cms.InputTag("ewkTauId")
                ))
            )
            process.bgEstFakeRateEventWeights.frTypes.tauIdEfficiency = \
          process.bgEstFakeRateJetWeights.frTypes.tauIdEfficiency
            frConfig[frType] = {}
            frConfig[frType]['srcJetWeight']   = cms.InputTag("bgEstFakeRateJetWeights",   "tauIdEfficiency")
            frConfig[frType]['srcEventWeight'] = cms.InputTag("bgEstFakeRateEventWeights", "tauIdEfficiency")
            frConfig[frType]['patLabel']       = "".join(["bgEstFakeRateJetWeight", "_", "tauIdEfficiency"])
            print "FR method: Using simple weights, adding ZTT eff"
            bgEstFakeRateAnalysisSequence = \
              addFakeRateGenAnalyzerModule(
                  process, analyzer_sequence,
                  "tauIdEfficiency", frConfig, bgEstFakeRateAnalysisSequence)

        setattr(process, "bgEstFakeRateAnalysisSequence" + analyzer, cms.Sequence(bgEstFakeRateAnalysisSequence))
        new_sequence = getattr(process, "bgEstFakeRateAnalysisSequence" + analyzer)
        print "FR method: Replacing %s by the new sequence %s" % (analyzer, "bgEstFakeRateAnalysisSequence" + analyzer)
        process.p.replace(analyzer_sequence, new_sequence)

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
        'patTupleProdSeq' : 'producePatTupleZtoMuTauSpecific',
        'analyzers' : [
            "analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation",
            "analyzeZtoMuTauEvents_factorizedWithMuonIsolation",
            "analyzeZtoMuTauEvents",
        ],
        'tau_ids' : [
            ("selectedPatTausForMuTauTaNCdiscr", "tauID('byTaNCloose') > -1.e+3"),
            ("selectedPatTausForMuTauProng", "signalPFChargedHadrCands.size() > -1"),
            ("selectedPatTausForMuTauCharge", "abs(charge) > -1"),
            ("selectedMuTauPairsZeroCharge", "leg2.leadPFChargedHadrCand.isNonnull & (leg1.charge + leg2.leadPFChargedHadrCand.charge) = 0"),
        ],
        'frSet' : 'ewkTauIdTaNCloose',
    },
    'AHtoMuTau' : {
        'patTupleProdSeq' : 'producePatTupleAHtoMuTauSpecific',
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
            "analyzeAHtoMuTauEventsSS_wBtag",
        ],
        'tau_ids' : [
            ("selectedPatTausForMuTauTaNCdiscr", "tauID('byTaNCloose') > -1.e+3"),
            ("selectedPatTausForMuTauProng", "signalPFChargedHadrCands.size() > -1"),
            ("selectedPatTausForMuTauCharge", "abs(charge) > -1"),
            ("selectedMuTauPairsForAHtoMuTauZeroCharge", "leg2.leadPFChargedHadrCand.isNonnull & (leg1.charge + leg2.leadPFChargedHadrCand.charge) = 0"),
            ("selectedMuTauPairsForAHtoMuTauNonZeroCharge", "leg2.leadPFChargedHadrCand.isNonnull & (leg1.charge + leg2.leadPFChargedHadrCand.charge) != 0"),
        ],
        'frSet' : 'ewkTauIdHPSloose',
    }
}

def enableFakeRates(process, channel, method = None):
    print channel
    enableFakeRatesImpl(process, method, **_FAKE_RATE_CONFIGS[channel])

def enableFakeRates_makeZtoMuTauPlots(process, enableFactorization = True):

    # get list of fake-rates types to be processed
    process.load("TauAnalysis.BgEstimationTools.fakeRateJetWeightProducer_cfi")
    frTypes = getPSetAttributes(process.bgEstFakeRateJetWeights.frTypes)
    frTypes.append("frUnweighted")

    seq_isFirstModule = True

    for frType in frTypes:

        mod_addZtoMuTau_qcdSum = copy.deepcopy(process.addBgEstFakeRateZtoMuTau_qcdSum_tauFakeRate)
        modInputDir_addZtoMuTau_qcdSum = cms.vstring(
            "".join(['tauFakeRate/harvested/InclusivePPmuX/zMuTauAnalyzer', '_fr_', frType]),
            "".join(['tauFakeRate/harvested/PPmuXptGt20/zMuTauAnalyzer', '_fr_', frType])
        )
        setattr(mod_addZtoMuTau_qcdSum.qcdSum, "dqmDirectories_input", modInputDir_addZtoMuTau_qcdSum)
        modOutputDir_addZtoMuTau_qcdSum = cms.string("".join(['tauFakeRate/harvested/qcdSum/zMuTauAnalyzer', '_fr_', frType]))
        setattr(mod_addZtoMuTau_qcdSum.qcdSum, "dqmDirectory_output", modOutputDir_addZtoMuTau_qcdSum)
        modName_addZtoMuTau_qcdSum = "".join(["addBgEstFakeRateZtoMuTau_qcdSum_tauFakeRate", "_", frType])
        setattr(process, modName_addZtoMuTau_qcdSum, mod_addZtoMuTau_qcdSum)

        seq_addZtoMuTau = cms.Sequence(getattr(process, modName_addZtoMuTau_qcdSum))

        modName_addZtoMuTau_smBgSum = "undefined"
        if hasattr(process, "addBgEstFakeRateZtoMuTau_smBgSum_tauFakeRate"):
            mod_addZtoMuTau_smBgSum = copy.deepcopy(process.addBgEstFakeRateZtoMuTau_smBgSum_tauFakeRate)
            modInputDir_addZtoMuTau_smBgSum = cms.vstring(
                "".join(['tauFakeRate/harvested/Zmumu/zMuTauAnalyzer', '_fr_', frType]),
                "".join(['tauFakeRate/harvested/WplusJets/zMuTauAnalyzer', '_fr_', frType]),
                "".join(['tauFakeRate/harvested/TTplusJets/zMuTauAnalyzer', '_fr_', frType]),
                "".join(['tauFakeRate/harvested/qcdSum/zMuTauAnalyzer', '_fr_', frType])
            )
            setattr(mod_addZtoMuTau_smBgSum.smBgSum, "dqmDirectories_input", modInputDir_addZtoMuTau_smBgSum)
            modOutputDir_addZtoMuTau_smBgSum = cms.string("".join(['tauFakeRate/harvested/smBgSum/zMuTauAnalyzer', '_fr_', frType]))
            setattr(mod_addZtoMuTau_smBgSum.smBgSum, "dqmDirectory_output", modOutputDir_addZtoMuTau_smBgSum)
            modName_addZtoMuTau_smBgSum = "".join(["addBgEstFakeRateZtoMuTau_smBgSum_tauFakeRate", "_", frType])
            setattr(process, modName_addZtoMuTau_smBgSum, mod_addZtoMuTau_smBgSum)

            seq_addZtoMuTau._seq = seq_addZtoMuTau._seq * getattr(process, modName_addZtoMuTau_smBgSum)

        modName_addZtoMuTau_smSum = "undefined"
        if hasattr(process, "addBgEstFakeRateZtoMuTau_smSum_tauFakeRate"):
            mod_addZtoMuTau_smSum = copy.deepcopy(process.addBgEstFakeRateZtoMuTau_smSum_tauFakeRate)
            modInputDir_addZtoMuTau_smSum = cms.vstring(
                "".join(['tauFakeRate/harvested/Ztautau/zMuTauAnalyzer', '_fr_', frType]),
                "".join(['tauFakeRate/harvested/smBgSum/zMuTauAnalyzer', '_fr_', frType])
            )
            setattr(mod_addZtoMuTau_smSum.smSum, "dqmDirectories_input", modInputDir_addZtoMuTau_smSum)
            modOutputDir_addZtoMuTau_smSum = cms.string("".join(['tauFakeRate/harvested/smSum/zMuTauAnalyzer', '_fr_', frType]))
            setattr(mod_addZtoMuTau_smSum.smSum, "dqmDirectory_output", modOutputDir_addZtoMuTau_smSum)
            modName_addZtoMuTau_smSum = "".join(["addBgEstFakeRateZtoMuTau_smSum_tauFakeRate", "_", frType])
            setattr(process, modName_addZtoMuTau_smSum, mod_addZtoMuTau_smSum)

            seq_addZtoMuTau._seq = seq_addZtoMuTau._seq * getattr(process, modName_addZtoMuTau_smSum)

        seqName_addZtoMuTau = "".join(["addBgEstFakeRateZtoMuTau_tauFakeRate", "_", frType])
        setattr(process, seqName_addZtoMuTau, seq_addZtoMuTau)

        ##if enableFactorization:
        ##    enableFactorization_makeZtoMuTauPlots_grid(process,
        ##      dqmDirectoryIn_InclusivePPmuX = \
        ##        "".join(['tauFakeRate/harvested/InclusivePPmuX/zMuTauAnalyzer', '_fr_', frType]),
        ##      dqmDirectoryOut_InclusivePPmuX = \
        ##        "".join(['tauFakeRate/harvested/InclusivePPmuX_factorized/zMuTauAnalyzer', '_fr_', frType]),
        ##      dqmDirectoryIn_PPmuXptGt20 = \
        ##        "".join(['tauFakeRate/harvested/PPmuXptGt20/zMuTauAnalyzer', '_fr_', frType]),
        ##      dqmDirectoryOut_PPmuXptGt20 = \
        ##        "".join(['tauFakeRate/harvested/PPmuXptGt20_factorized/zMuTauAnalyzer', '_fr_', frType]),
        ##      modName_addZtoMuTau_qcdSum = modName_addZtoMuTau_qcdSum,
        ##      modName_addZtoMuTau_smBgSum = modName_addZtoMuTau_smBgSum,
        ##      modName_addZtoMuTau_smSum = modName_addZtoMuTau_smSum,
        ##      seqName_addZtoMuTau = seqName_addZtoMuTau,
        ##      pyObjectLabel = frType)

        if seq_isFirstModule:
            setattr(process, "addBgEstFakeRateZtoMuTau_tauFakeRate", cms.Sequence(getattr(process, seqName_addZtoMuTau)))
            seq_isFirstModule = False
        else:
            process.addBgEstFakeRateZtoMuTau_tauFakeRate._seq = \
              process.addBgEstFakeRateZtoMuTau_tauFakeRate._seq * getattr(process, seqName_addZtoMuTau)

#--------------------------------------------------------------------------------
# utility functions specific to application of fake-rate weights
# to tau id. efficiency measurement analysis
#--------------------------------------------------------------------------------

def enableFakeRates_runTauIdEffAnalysisZtoMuTau(process):

    method = "simple"

    # compute fake-rate weights
    configureFakeRateWeightProduction(process, method = method)

    # enable checking of fake-rates and tau id. efficiencies
    # with event weights in tau-jet histogram manager
    setattr(process.tauHistManager, "checkWeightConsistency", cms.bool(True))

    # get list of fake-rates types to be processed
    frTypes = getPSetAttributes(process.bgEstFakeRateJetWeights.frTypes)

    fakeRateAnalysisSequence = None

    # duplicate analysis sequence:
    #  1.) tau id. discriminators not applied
    #  2.) events weighted by fake-rate
    # for each type of fake-rate weights given as function argument
    #
    # Note: special care is needed to avoid double-counting
    #       in case there is more than one (loosely selected) tau-jet candidate in the event
    #       when filling histograms that are sensitive to the tau-jet multiplicity
    #
    for frType in frTypes:
        fakeRateAnalysisSequence = \
          addFakeRateGenAnalyzerModule(process, process.analyzeEventsTauIdEffZtoMuTauCombinedFit,
                                       frType, fakeRateAnalysisSequence)

    setattr(process, "fakeRateAnalysisSequence", cms.Sequence(fakeRateAnalysisSequence))

    process.bgEstTauIdEffZtoMuTauCombinedFitAnalysisSequence._seq = \
      process.bgEstTauIdEffZtoMuTauCombinedFitAnalysisSequence._seq * process.fakeRateAnalysisSequence

def enableFakeRates_makeTauIdEffZtoMuTauPlots(process):

    # get list of fake-rates types to be processed
    process.load("TauAnalysis.BgEstimationTools.fakeRateJetWeightProducer_cfi")
    frTypes = getPSetAttributes(process.bgEstFakeRateJetWeights.frTypes)

    for frType in frTypes:

        mod_addZtoMuTau_qcdSum = copy.deepcopy(process.addTauIdEffZtoMuTau_qcdSum)
        modInputDir_addZtoMuTau_qcdSum = cms.vstring(
            "".join(['harvested/InclusivePPmuX/TauIdEffAnalyzerZtoMuTauCombinedFit', '_fr_', frType]),
            "".join(['harvested/PPmuXptGt20/TauIdEffAnalyzerZtoMuTauCombinedFit', '_fr_', frType])
        )
        setattr(mod_addZtoMuTau_qcdSum.qcdSum, "dqmDirectories_input", modInputDir_addZtoMuTau_qcdSum)
        modOutputDir_addZtoMuTau_qcdSum = \
          cms.string("".join(['tauFakeRate/harvested/qcdSum/TauIdEffAnalyzerZtoMuTauCombinedFit', '_fr_', frType]))
        setattr(mod_addZtoMuTau_qcdSum.qcdSum, "dqmDirectory_output", modOutputDir_addZtoMuTau_qcdSum)
        modName_addZtoMuTau_qcdSum = "".join(["addTauIdEffZtoMuTau_qcdSum", "_", frType])
        setattr(process, modName_addZtoMuTau_qcdSum, mod_addZtoMuTau_qcdSum)

        process.addTauIdEffZtoMuTau._seq = process.addTauIdEffZtoMuTau._seq * mod_addZtoMuTau_qcdSum

        mod_addZtoMuTau_smSum = copy.deepcopy(process.addTauIdEffZtoMuTau_smSum)
        modInputDir_addZtoMuTau_smSum = cms.vstring(
            "".join(['harvested/Ztautau/TauIdEffAnalyzerZtoMuTauCombinedFit', '_fr_', frType]),
            "".join(['harvested/Zmumu/TauIdEffAnalyzerZtoMuTauCombinedFit', '_fr_', frType]),
            "".join(['harvested/WplusJets/TauIdEffAnalyzerZtoMuTauCombinedFit', '_fr_', frType]),
            "".join(['harvested/TTplusJets/TauIdEffAnalyzerZtoMuTauCombinedFit', '_fr_', frType]),
            "".join(['harvested/qcdSum/TauIdEffAnalyzerZtoMuTauCombinedFit', '_fr_', frType])
        )
        setattr(mod_addZtoMuTau_smSum.smSum, "dqmDirectories_input", modInputDir_addZtoMuTau_smSum)
        modOutputDir_addZtoMuTau_smSum = \
          cms.string("".join(['tauFakeRate/harvested/smSum/TauIdEffAnalyzerZtoMuTauCombinedFit', '_fr_', frType]))
        setattr(mod_addZtoMuTau_smSum.smSum, "dqmDirectory_output", modOutputDir_addZtoMuTau_smSum)
        modName_addZtoMuTau_smSum = "".join(["addTauIdEffZtoMuTau_smSum", "_", frType])
        setattr(process, modName_addZtoMuTau_smSum, mod_addZtoMuTau_smSum)

        process.addTauIdEffZtoMuTau._seq = process.addTauIdEffZtoMuTau._seq * mod_addZtoMuTau_smSum
