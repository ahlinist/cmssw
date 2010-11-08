import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.DQMTools.tools.composeSubDirectoryName import composeSubDirectoryName

#--------------------------------------------------------------------------------
# generic utility functions for factorization
# usable for all channels
#--------------------------------------------------------------------------------

def replaceEventSelections(analyzer, evtSel_replacements):
    # auxiliary function to replace in configuration of GenericAnalyzer
    # "tight" by "loose" cuts for factorization purposes

    for evtSel_replacement in evtSel_replacements:
        
        # check that all entries in evtSel_replacements list contain exactly two entries
        # (one for the "tight" cut to be replaced and one for the "loose" cut used as replacement)
        if len(evtSel_replacement) != 2:
            raise ValueError("Invalid 'evtSel_replacements' Parameter !!")

        evtSel_tight = evtSel_replacement[0]
        evtSel_loose = evtSel_replacement[1]

        for evtSel_i in analyzer.filters:
            if getattr(evtSel_i, "pluginName").value() == getattr(evtSel_tight, "pluginName").value():

                analyzer.filters.remove(evtSel_i)
                analyzer.filters.append(evtSel_loose)

                print("Replaced in " + getattr(analyzer, "name").value() + ": "
                      + getattr(evtSel_tight, "pluginName").value() + " by " + getattr(evtSel_loose, "pluginName").value()
                      + " (version with factorization enabled)")

def replaceAnalyzerModules(analyzer, analyzerModule_replacements):
    # auxiliary function to replace analyzer modules
    # in configuration of GenericAnalyzer

    for analyzerModule_replacement in analyzerModule_replacements:
        
        # check that all entries in analyzerModule_replacements list contain exactly two entries
        # (one for the "old" module to be replaced and one for the "new" module used as replacement)
        if len(analyzerModule_replacement) != 2:
            raise ValueError("Invalid 'analyzerModule_replacements' Parameter !!")

        analyzerModule_old = analyzerModule_replacement[0]
        analyzerModule_new = analyzerModule_replacement[1]

        for analyzerModule_i in analyzer.analyzers:
            if getattr(analyzerModule_i, "pluginName").value() == getattr(analyzerModule_old, "pluginName").value():

                analyzer.analyzers.remove(analyzerModule_i)
                analyzer.analyzers.append(analyzerModule_new)

                print("Replaced in " + getattr(analyzer, "name").value() + ": "
                      + getattr(analyzerModule_old, "pluginName").value() + " by " + getattr(analyzerModule_new, "pluginName").value()
                      + " (version with factorization enabled)")

#
#--------------------------------------------------------------------------------
#

def composeDirectoryName(dqmDirectory, factorizationLabel):
    if dqmDirectory.rfind("_") == -1:
        return dqmDirectory + '_' + factorizationLabel + '/'
    else:        
        return dqmDirectory[:dqmDirectory.rindex("_")] + '_' + factorizationLabel + dqmDirectory[dqmDirectory.rindex("_"):] + '/'

def composeSubDirectoryNames_plots(evtSelList):
    # auxiliary function to compose names of dqmSubDirectories
    # in which histograms are stored

    dqmSubDirectoryNames = []
    for iEvtSel in range(len(evtSelList) - 1):
        afterCut = evtSelList[iEvtSel]
        beforeCut = evtSelList[iEvtSel + 1]
        
        dqmSubDirectoryNames.append(composeSubDirectoryName(afterCut = afterCut, beforeCut = beforeCut))

    return dqmSubDirectoryNames

def composeSubDirectoryNames_filterStatistics(evtSelList):
    # auxiliary function to compose names of dqmSubDirectories
    # in which FilterStatistics objects are stored

    dqmSubDirectoryNames = []
    for evtSel in evtSelList:
        dqmSubDirectoryNames.append(getattr(evtSel, "pluginName").value())

    return dqmSubDirectoryNames    

def composeFactorizationSequence(process,
                                 processName,
                                 dqmDirectoryIn_factorizedTightEvtSel, evtSel_factorizedTight, 
                                 dqmDirectoryIn_factorizedLooseEvtSel, evtSel_factorizedLoose,
                                 meName_numerator, meName_denominator,
                                 dqmDirectoryOut,
                                 dropInputDirectories = True):
    # compose sequence applying factorization
    # to histograms and FilterStatistics objects
    
    # configure EDAnalyzer that copies histograms filled **before**
    # cuts used for factorization are applied
    dqmHistScaler_plotsTightEvtSel = cms.EDAnalyzer("DQMHistScaler",
        dqmDirectory_input = cms.string(dqmDirectoryIn_factorizedTightEvtSel),
        dqmSubDirectories_input = cms.vstring(
            composeSubDirectoryNames_plots([ None ] + evtSel_factorizedTight + [ evtSel_factorizedLoose[0] ])
        ),
        scaleFactor = cms.double(1.),
        dqmDirectory_output = cms.string(dqmDirectoryOut)
    )

    # configure EDAnalyzer that copies FilterStatistics objects filled **before**
    # cuts used for factorization are applied
    dqmHistScaler_filterStatTightEvtSel = cms.EDAnalyzer("DQMHistScaler",
        dqmDirectory_input = cms.string(dqmDirectoryIn_factorizedTightEvtSel + "FilterStatistics" + "/"),
        dqmSubDirectories_input = cms.vstring(
            composeSubDirectoryNames_filterStatistics(evtSel_factorizedTight)
        ),
        scaleFactor = cms.double(1.),
        dqmDirectory_output = cms.string(dqmDirectoryOut + "FilterStatistics" + "/")
    )

    # configure EDAnalyzer that copies histograms filled **after**
    # cuts used for factorization are applied
    dqmHistScaler_plotsLooseEvtSel = cms.EDAnalyzer("DQMHistScaler",
        dqmDirectory_input = cms.string(dqmDirectoryIn_factorizedLooseEvtSel),
        dqmSubDirectories_input = cms.vstring(
            composeSubDirectoryNames_plots(evtSel_factorizedLoose + [ None ])
        ),
        dqmDirectory_output = cms.string(dqmDirectoryOut)
    )

    # configure EDAnalyzer that copies FilterStatistics objects filled **after**
    # cuts used for factorization are applied
    dqmHistScaler_filterStatLooseEvtSel = cms.EDAnalyzer("DQMHistScaler",
        dqmDirectory_input = cms.string(dqmDirectoryIn_factorizedLooseEvtSel + "FilterStatistics" + "/"),
        dqmSubDirectories_input = cms.vstring(
            composeSubDirectoryNames_filterStatistics(evtSel_factorizedLoose)
        ),
        dqmDirectory_output = cms.string(dqmDirectoryOut + "FilterStatistics" + "/")
    )

    # automatically add to meNames
    # suffix indicating how MonitorElements of type float
    # need to get scaled and added
    #
    # NOTE: definitions of meOptionsSeparator and meOptionsNumWeighted
    #       need to match those in TauAnalysis/Core/src/FilterStatisticsService.cc 
    #
    meOptionsSeparator = "#"
    meOptionsNumWeighted = "".join([meOptionsSeparator, "a1", meOptionsSeparator, "s1"])

    if meName_numerator is not None and meName_denominator is not None:
        dqmDirectory_factorizedLooseSel = cms.string(dqmDirectoryIn_factorizedLooseEvtSel + "FilterStatistics" + "/")
        dqmDirectory_factorizedTightSel = cms.string(dqmDirectoryIn_factorizedTightEvtSel + "FilterStatistics" + "/")
        meType = cms.string("real")
        
        setattr(dqmHistScaler_plotsLooseEvtSel, "meName_numerator", cms.string("".join([meName_numerator, meOptionsNumWeighted])))
        setattr(dqmHistScaler_plotsLooseEvtSel, "meName_denominator", cms.string("".join([meName_denominator, meOptionsNumWeighted])))
        setattr(dqmHistScaler_plotsLooseEvtSel, "dqmDirectory_factorizedLooseSel", dqmDirectory_factorizedLooseSel)
        setattr(dqmHistScaler_plotsLooseEvtSel, "dqmDirectory_factorizedTightSel", dqmDirectory_factorizedTightSel)
        setattr(dqmHistScaler_plotsLooseEvtSel, "meType", meType)
        
        setattr(dqmHistScaler_filterStatLooseEvtSel, "meName_numerator", cms.string("".join([meName_numerator, meOptionsNumWeighted])))
        setattr(dqmHistScaler_filterStatLooseEvtSel, "meName_denominator", cms.string("".join([meName_denominator, meOptionsNumWeighted])))
        setattr(dqmHistScaler_filterStatLooseEvtSel, "dqmDirectory_factorizedLooseSel", dqmDirectory_factorizedLooseSel)
        setattr(dqmHistScaler_filterStatLooseEvtSel, "dqmDirectory_factorizedTightSel", dqmDirectory_factorizedTightSel)
        setattr(dqmHistScaler_filterStatLooseEvtSel, "meType", meType)
    else:
        setattr(dqmHistScaler_plotsLooseEvtSel, "scaleFactor", cms.double(1.))
        
        setattr(dqmHistScaler_filterStatLooseEvtSel, "scaleFactor", cms.double(1.))

    # delete original histograms and FilterStatistics objects
    # after applying factorization
    # (add drop command to last DQMHistScaler module put in sequence)
    if dropInputDirectories:
        setattr(dqmHistScaler_filterStatLooseEvtSel, "drop", cms.vstring(
            [ dqmDirectoryIn_factorizedLooseEvtSel,
              dqmDirectoryIn_factorizedTightEvtSel ] ))

    # add EDAnalyzers copying histograms and FilterStatistics objects
    # to process object
    setattr(process, "dqmHistScaler_plotsFactorizedTightEvtSel" + "_" + processName, dqmHistScaler_plotsTightEvtSel)
    setattr(process, "dqmHistScaler_filterStatFactorizedTightEvtSel" + "_" + processName, dqmHistScaler_filterStatTightEvtSel)
    setattr(process, "dqmHistScaler_plotsFactorizedLooseEvtSel" + "_" + processName, dqmHistScaler_plotsLooseEvtSel)
    setattr(process, "dqmHistScaler_filterStatFactorizedLooseEvtSel" + "_" + processName, dqmHistScaler_filterStatLooseEvtSel)

    # return sequence of all EDAnalyzers
    factorizationSequence = cms.Sequence(
        dqmHistScaler_plotsTightEvtSel + dqmHistScaler_filterStatTightEvtSel
       + dqmHistScaler_plotsLooseEvtSel + dqmHistScaler_filterStatLooseEvtSel
    )

    return factorizationSequence

#--------------------------------------------------------------------------------
# utility functions specific to factorization
# of muon isolation efficiencies in Z --> mu + tau-jet channel
#--------------------------------------------------------------------------------

def enableFactorization_runZtoMuTau(process):
    process.load("TauAnalysis.Configuration.selectZtoMuTau_factorized_cff")
    process.selectZtoMuTauEvents_factorized = cms.Sequence(
        process.selectZtoMuTauEvents
       * process.selectZtoMuTauEventsLooseMuonIsolation
    )
    process.p.replace(process.selectZtoMuTauEvents, process.selectZtoMuTauEvents_factorized)
    process.load("TauAnalysis.Configuration.analyzeZtoMuTau_factorized_cff")
    process.analyzeZtoMuTauEvents_factorized = cms.Sequence(
        process.analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation
       * process.analyzeZtoMuTauEvents_factorizedWithMuonIsolation
    )
    process.p.replace(process.analyzeZtoMuTauEvents, process.analyzeZtoMuTauEvents_factorized)

def enableFactorization_makeZtoMuTauPlots_grid(
    process,
    factorizationSequenceName = "loadAndFactorizeZtoMuTauSamples",
    samplesToFactorize = [ 'InclusivePPmuX', 'PPmuXptGt20Mu10', 'PPmuXptGt20Mu15' ],
    relevantMergedSamples = [ 'qcdSum', ],
    mergedToRecoSampleDict = {},
    mergedSampleAdderModule = lambda sample: 'addZtoMuTau_%s' % (sample),
    dqmDirectoryOut = 
    lambda sample:'/harvested/%s_factorized/zMuTauAnalyzer/'% (sample),
    dqmDirectoryOutUnfactorized = 
    lambda sample:'/harvested/%s/zMuTauAnalyzer/'% (sample),
    dqmDirectoryTight = 
    lambda sample:'/harvested/%s/zMuTauAnalyzer_factorizedWithMuonIsolation/' % (sample),
    dqmDirectoryLoose = 
    lambda sample:'/harvested/%s/zMuTauAnalyzer_factorizedWithoutMuonIsolation/' % (sample),
    pyObjectLabel = ""):

    process.load("TauAnalysis.Configuration.analyzeZtoMuTau_cfi")

    # define list of event selection criteria on "tight" muon isolation branch
    # of the analysis, **before** applying factorization of muon track + ECAL
    # isolation efficiencies
    evtSelZtoMuTau_factorizedTight = [
        process.evtSelGenPhaseSpace,
        process.evtSelTrigger,
        process.evtSelDataQuality,
        process.evtSelPrimaryEventVertex,
        process.evtSelPrimaryEventVertexQuality,
        process.evtSelPrimaryEventVertexPosition,
        process.evtSelGlobalMuon,
        process.evtSelMuonEta,
        process.evtSelMuonPt,
        process.evtSelTauAntiOverlapWithMuonsVeto,
        process.evtSelTauEta,
        process.evtSelTauPt,
        process.evtSelMuonVbTfId,
        process.evtSelMuonPFRelIso
    ]

    # define list of event selection criteria on "loose" muon isolation branch
    # of the analysis, **after** applying factorization of muon track + ECAL
    # isolation efficiencies
    evtSelZtoMuTau_factorizedLoose = [
        process.evtSelMuonAntiPion,
        process.evtSelMuonTrkIP,
        process.evtSelTauLeadTrk,
        process.evtSelTauLeadTrkPt,
        process.evtSelTauTaNCdiscr,
        process.evtSelTauTrkIso,
        process.evtSelTauEcalIso,
        process.evtSelTauProng,
        process.evtSelTauCharge,
        process.evtSelTauMuonVeto,
        process.evtSelTauElectronVeto,
        process.evtSelDiTauCandidateForMuTauAntiOverlapVeto,
        process.evtSelDiTauCandidateForMuTauZeroCharge,
        process.evtSelDiTauCandidateForMuTauAcoplanarity12,
        process.evtSelDiTauCandidateForMuTauMt1MET,
        process.evtSelDiTauCandidateForMuTauPzetaDiff,
        process.evtSelDiMuPairZmumuHypothesisVetoByMass,
        process.evtSelDiMuPairZmumuHypothesisVetoByLooseIsolationAndCharge
    ]

    # defines names of MonitorElements used as numerator and denominator
    # to compute factorization scale-factor
    meNameZtoMuTau_numerator = "evtSelMuonPFRelIso/passed_cumulative_numWeighted"
    meNameZtoMuTau_denominator = "evtSelMuonPFRelIso/processed_cumulative_numWeighted"
    
    # Loop over the samples and create sequences
    # for each of the factorization jobs and add them to the factorization
    # sequence
    factorizationSequence = getattr(process, factorizationSequenceName)
    for sample in samplesToFactorize:
        new_factorization_sequence = composeFactorizationSequence(
            process = process,
            processName = sample + "_" + pyObjectLabel,
            dqmDirectoryIn_factorizedTightEvtSel = dqmDirectoryTight(sample),
            evtSel_factorizedTight = evtSelZtoMuTau_factorizedTight,
            dqmDirectoryIn_factorizedLooseEvtSel = dqmDirectoryLoose(sample),
            evtSel_factorizedLoose = evtSelZtoMuTau_factorizedLoose,
            meName_numerator = meNameZtoMuTau_numerator,
            meName_denominator = meNameZtoMuTau_denominator,
            dqmDirectoryOut = dqmDirectoryOut(sample),
            dropInputDirectories = False
        )
        new_factorization_seq_name = "scaleZtoMuTau_%s_%s" % (sample, pyObjectLabel)
        setattr(process, new_factorization_seq_name, new_factorization_sequence)
        factorizationSequence += new_factorization_sequence

    # Now update any of the relevant mergers
    for mergedSample in relevantMergedSamples:
        # Get the module that is doing the merging, if it exists
        if not hasattr(process.mergeSamplesZtoMuTau, "merge_%s"%(mergedSample)): continue
        merger = getattr(process.mergeSamplesZtoMuTau, "merge_%s" % (mergedSample))
        
        # Get the subsamples associated with this merged sample
        subsamples = mergedToRecoSampleDict[mergedSample]['samples']
        # Set the adder to use our new factorized inputs
        def merge_directories(_list):
            for sample in _list:
                if sample in samplesToFactorize:
                    yield dqmDirectoryOut(sample)
                else:
                    yield dqmDirectoryOutUnfactorized(sample)
                    
        merger.dqmDirectories_input = cms.vstring(list(merge_directories(subsamples)))
    
    # Update the plot sources in the plot jobs.  Note that we don't need to do
    # this for the merged samples, since we have replaced the HistAdder sources
    for plotterModuleName in [ 'plotZtoMuTau', ]:
        plotterModuleProcesses = getattr(process, plotterModuleName).processes
        for sample in samplesToFactorize:
            if hasattr(plotterModuleProcesses, sample):
                getattr(plotterModuleProcesses, sample).dqmDirectory = \
                        cms.string("/harvested/%s_factorized" % sample)

#--------------------------------------------------------------------------------
# utility functions specific to factorization
# of muon isolation efficiencies in Z --> e + mu channel
#--------------------------------------------------------------------------------

def enableFactorization_runZtoElecMu(process):
    process.load("TauAnalysis.Configuration.selectZtoElecMu_factorized_cff")
    process.selectZtoElecMuEvents_factorized = cms.Sequence(
        process.selectZtoElecMuEvents
       * process.selectZtoElecMuEventsLooseElectronIsolation
    )
    process.p.replace(process.selectZtoElecMuEvents, process.selectZtoElecMuEvents_factorized)
    process.load("TauAnalysis.Configuration.analyzeZtoElecMu_factorized_cff")
    process.analyzeZtoElecMuEvents_factorized = cms.Sequence(
        process.analyzeZtoElecMuEvents_factorizedWithoutElectronIsolation
       * process.analyzeZtoElecMuEvents_factorizedWithElectronIsolation
    )
    process.p.replace(process.analyzeZtoElecMuEvents, process.analyzeZtoElecMuEvents_factorized)

def enableFactorization_makeZtoElecMuPlots(process,
        dqmDirectoryIn_InclusivePPmuX = 'harvested/InclusivePPmuX/zElecMuAnalyzer',
        dqmDirectoryOut_InclusivePPmuX = 'harvested/InclusivePPmuX_factorized/zElecMuAnalyzer',
        dqmDirectoryIn_PPmuXptGt20 = 'harvested/PPmuXptGt20/zElecMuAnalyzer',
        dqmDirectoryOut_PPmuXptGt20 = 'harvested/PPmuXptGt20_factorized/zElecMuAnalyzer',                                  
        modName_addZtoElecMu_qcdSum = "addZtoElecMu_qcdSum",
        modName_addZtoElecMu_smSum = "addZtoElecMu_smSum",                                
        seqName_addZtoElecMu = "addZtoElecMu",
        pyObjectLabel = ""):
    process.load("TauAnalysis.Configuration.analyzeZtoElecMu_cfi")
    
    # define list of event selection criteria on "tight" muon isolation branch of the analysis,
    # **before** applying factorization of muon track + ECAL isolation efficiencies
    evtSelZtoElecMu_factorizedTight = [
        process.genPhaseSpaceCut,
        process.evtSelTrigger,
        process.evtSelPrimaryEventVertex,
        process.evtSelPrimaryEventVertexQuality,
        process.evtSelPrimaryEventVertexPosition,
        process.evtSelTightElectronId,
        process.evtSelElectronAntiCrack,
        process.evtSelElectronEta,
        process.evtSelElectronPt,
        process.evtSelGlobalMuon,
        process.evtSelMuonEta,
        process.evtSelMuonPt,
        process.evtSelElectronTrkIso,
        process.evtSelElectronEcalIso
    ]

    # define list of event selection criteria on "loose" muon isolation branch of the analysis,
    # **after** applying factorization of muon track + ECAL isolation efficiencies
    evtSelZtoElecMu_factorizedLoose = [
        process.evtSelElectronTrk,
        process.evtSelElectronTrkIP,
        process.evtSelMuonPFRelIso,
        process.evtSelMuonAntiPion,
        process.evtSelMuonTrkIP,
        process.evtSelDiTauCandidateForElecMuAntiOverlapVeto,
        process.evtSelDiTauCandidateForElecMuZeroCharge,
        process.evtSelDiTauCandidateForElecMuAcoplanarity12,
        process.evtSelDiTauCandidateForElecMuMt1MET,
        process.evtSelDiTauCandidateForElecMuMt2MET,
        process.evtSelDiTauCandidateForElecMuPzetaDiff
    ]

    # defines names of MonitorElements used as numerator and denominator
    # to compute factorization scale-factor
    meNameZtoElecMu_numerator = "evtSelElectronEcalIso/passed_cumulative_numWeighted"
    meNameZtoElecMu_denominator = "evtSelElectronTrkIso/processed_cumulative_numWeighted"

    # configure sequence for applying factorization to "InclusivePPmuX" process
    # (QCD background sample for Pt(hat) < 20 GeV region in phase-space)
    scaleZtoElecMu_InclusivePPmuX = composeFactorizationSequence(
        process = process,
        processName = "InclusivePPmuX" + "_" + pyObjectLabel,
        dqmDirectoryIn_factorizedTightEvtSel = composeDirectoryName(dqmDirectoryIn_InclusivePPmuX, "factorizedWithElectronIsolation"),
        evtSel_factorizedTight = evtSelZtoElecMu_factorizedTight,
        dqmDirectoryIn_factorizedLooseEvtSel = composeDirectoryName(dqmDirectoryIn_InclusivePPmuX, "factorizedWithoutElectronIsolation"),
        evtSel_factorizedLoose = evtSelZtoElecMu_factorizedLoose,
        meName_numerator = meNameZtoElecMu_numerator,
        meName_denominator = meNameZtoElecMu_denominator,
        dqmDirectoryOut = dqmDirectoryOut_InclusivePPmuX + '/'
    )

    scaleZtoElecMuName_InclusivePPmuX = "scaleZtoElecMu_InclusivePPmuX" + "_" + pyObjectLabel
    setattr(process, scaleZtoElecMuName_InclusivePPmuX, scaleZtoElecMu_InclusivePPmuX)

    # configure sequence for applying factorization to "PPmuXPPmuXptGt20" process
    # (QCD background sample for Pt(hat) > 20 GeV region in phase-space)
    scaleZtoElecMu_PPmuXptGt20 = composeFactorizationSequence(
        process = process,
        processName = "PPmuXptGt20" + "_" + pyObjectLabel,
        dqmDirectoryIn_factorizedTightEvtSel = composeDirectoryName(dqmDirectoryIn_PPmuXptGt20, "factorizedWithElectronIsolation"),
        evtSel_factorizedTight = evtSelZtoElecMu_factorizedTight,
        dqmDirectoryIn_factorizedLooseEvtSel = composeDirectoryName(dqmDirectoryIn_PPmuXptGt20, "factorizedWithoutElectronIsolation"),
        evtSel_factorizedLoose = evtSelZtoElecMu_factorizedLoose,
        meName_numerator = meNameZtoElecMu_numerator,
        meName_denominator = meNameZtoElecMu_denominator,
        dqmDirectoryOut = dqmDirectoryOut_PPmuXptGt20 + '/'
    )

    scaleZtoElecMuName_PPmuXptGt20 = "scaleZtoElecMu_PPmuXptGt20" + "_" + pyObjectLabel
    setattr(process, scaleZtoElecMuName_PPmuXptGt20, scaleZtoElecMu_PPmuXptGt20)

    # compute QCD background sum using factorized histograms and FilterStatistics objects
    addZtoElecMu_qcdSum = getattr(process, modName_addZtoElecMu_qcdSum)
    addZtoElecMu_qcdSum.qcdSum.dqmDirectories_input = cms.vstring(
        dqmDirectoryOut_InclusivePPmuX + '/',
        dqmDirectoryOut_PPmuXptGt20 + '/'
    )
    addZtoElecMu = cms.Sequence(
        getattr(process, scaleZtoElecMuName_InclusivePPmuX)
       + getattr(process, scaleZtoElecMuName_PPmuXptGt20)
    )
    addZtoElecMu._seq = addZtoElecMu._seq * getattr(process, modName_addZtoElecMu_qcdSum)
    if hasattr(process, modName_addZtoElecMu_smSum):
        addZtoElecMu._seq = addZtoElecMu._seq * getattr(process, modName_addZtoElecMu_smSum)
    setattr(process, seqName_addZtoElecMu, addZtoElecMu)

    if hasattr(process, "plotZtoElecMu"):
        process.plotZtoElecMu.processes.InclusivePPmuX.dqmDirectory = cms.string('harvested/InclusivePPmuX_factorized')
        process.plotZtoElecMu.processes.PPmuXptGt20.dqmDirectory = cms.string('harvested/PPmuXptGt20_factorized')

#--------------------------------------------------------------------------------
# utility functions specific to factorization
# of muon isolation efficiencies in Z --> e + tau channel
#--------------------------------------------------------------------------------

def enableFactorization_runZtoElecTau(process):
    process.load("TauAnalysis.Configuration.selectZtoElecTau_factorized_cff")
    process.selectZtoElecTauEvents_factorized = cms.Sequence( process.selectZtoElecTauEvents
                                                            *process.selectZtoElecTauEventsLooseElectronIsolation )
    process.p.replace(process.selectZtoElecTauEvents, process.selectZtoElecTauEvents_factorized)
    process.load("TauAnalysis.Configuration.analyzeZtoElecTau_factorized_cff")
    process.analyzeZtoElecTauEvents_factorized = cms.Sequence( process.analyzeZtoElecTauEvents_factorizedWithoutElectronIsolation
                                                             *process.analyzeZtoElecTauEvents_factorizedWithElectronIsolation )
    process.p.replace(process.analyzeZtoElecTauEvents, process.analyzeZtoElecTauEvents_factorized)

def enableFactorization_makeZtoElecTauPlots(process):
	process.load("TauAnalysis.Configuration.analyzeZtoElecTau_cfi")

	# define list of event selection criteria on "tight" electron isolation branch of the analysis,
	# **before** applying factorization of electron track + ECAL isolation efficiencies
	evtSelZtoElecTau_factorizedTight = [
		process.genPhaseSpaceCut,
		process.evtSelTrigger,
		process.evtSelPrimaryEventVertex,
		process.evtSelPrimaryEventVertexQuality,
		process.evtSelPrimaryEventVertexPosition,
		process.evtSelElectronId,
		process.evtSelElectronAntiCrack,
		process.evtSelElectronEta,
		process.evtSelElectronPt,
		process.evtSelTauAntiOverlapWithElectronsVeto,
		process.evtSelTauEta,
		process.evtSelTauPt,
		process.evtSelElectronTrkIso,
		process.evtSelElectronEcalIso,
		process.evtSelElectronConversionVeto
	]

	# define list of event selection criteria on "loose" electron isolation branch of the analysis,
	# **after** applying factorization of electron track + ECAL isolation efficiencies
	evtSelZtoElecTau_factorizedLoose = [
		process.evtSelElectronTrkIP,
		process.evtSelTauLeadTrk,
		process.evtSelTauLeadTrkPt,
		process.evtSelTauTaNCdiscr,
		process.evtSelTauTrkIso,
		process.evtSelTauEcalIso,
		process.evtSelTauProng,
		process.evtSelTauCharge,
		process.evtSelTauElectronVeto,
		process.evtSelTauEcalCrackVeto,
		process.evtSelTauMuonVeto,
		process.evtSelDiTauCandidateForElecTauAntiOverlapVeto,
		process.evtSelDiTauCandidateForElecTauZeroCharge,
		process.evtSelDiTauCandidateForElecTauAcoplanarity12,
		process.evtSelDiTauCandidateForElecTauMt1MET,
		process.evtSelDiTauCandidateForElecTauPzetaDiff,
		process.evtSelElecTauPairZeeHypothesisVeto
	]

	# defines names of MonitorElements used as numerator and denominator
	# to compute factorization scale-factor
	meNameZtoElecTau_numerator = "evtSelElectronConversionVeto/passed_cumulative_numWeighted"
	meNameZtoElecTau_denominator = "evtSelElectronTrkIso/processed_cumulative_numWeighted"

	# configure sequence for applying factorization to "QCD_BCtoE_PT20to30" process
	# (QCD background sample with b/c quark -> electron and 20 < Pt/GeV < 30)
	process.scaleZtoElecTau_QCD_BCtoE_Pt20to30 = composeFactorizationSequence(
		process = process,
		processName = "QCD_BCtoE_Pt20to30",
		dqmDirectoryIn_factorizedTightEvtSel = \
                         'harvested/QCD_BCtoE_Pt20to30/zElecTauAnalyzer_factorizedWithElectronIsolation/',
		evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
		dqmDirectoryIn_factorizedLooseEvtSel = \
                         'harvested/QCD_BCtoE_Pt20to30/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
		evtSel_factorizedLoose = evtSelZtoElecTau_factorizedLoose,
		meName_numerator = meNameZtoElecTau_numerator,
		meName_denominator = meNameZtoElecTau_denominator,
		dqmDirectoryOut = 'harvested/QCD_BCtoE_Pt20to30_factorized/zElecTauAnalyzer/'
	)

	# configure sequence for applying factorization to "QCD_BCtoE_PT30to80" process
	# (QCD background sample with b/c quark -> electron and 30 < Pt/GeV < 80)
	process.scaleZtoElecTau_QCD_BCtoE_Pt30to80 = composeFactorizationSequence(
		process = process,
		processName = "QCD_BCtoE_Pt30to80",
		dqmDirectoryIn_factorizedTightEvtSel = \
                         'harvested/QCD_BCtoE_Pt30to80/zElecTauAnalyzer_factorizedWithElectronIsolation/',
		evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
		dqmDirectoryIn_factorizedLooseEvtSel = \
                         'harvested/QCD_BCtoE_Pt30to80/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
		evtSel_factorizedLoose = evtSelZtoElecTau_factorizedLoose,
		meName_numerator = meNameZtoElecTau_numerator,
		meName_denominator = meNameZtoElecTau_denominator,
		dqmDirectoryOut = 'harvested/QCD_BCtoE_Pt30to80_factorized/zElecTauAnalyzer/'
	)

	# configure sequence for applying factorization to "QCD_BCtoE_PT80to170" process
	# (QCD background sample with b/c quark -> electron and 80 < Pt/GeV < 180)
	process.scaleZtoElecTau_QCD_BCtoE_Pt80to170 = composeFactorizationSequence(
		process = process,
		processName = "QCD_BCtoE_Pt80to170",
		dqmDirectoryIn_factorizedTightEvtSel = \
                         'harvested/QCD_BCtoE_Pt80to170/zElecTauAnalyzer_factorizedWithElectronIsolation/',
		evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
		dqmDirectoryIn_factorizedLooseEvtSel = \
                         'harvested/QCD_BCtoE_Pt80to170/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
		evtSel_factorizedLoose = evtSelZtoElecTau_factorizedLoose,
		meName_numerator = meNameZtoElecTau_numerator,
		meName_denominator = meNameZtoElecTau_denominator,
		dqmDirectoryOut = 'harvested/QCD_BCtoE_Pt80to170_factorized/zElecTauAnalyzer/'
	)

	# configure sequence for applying factorization to "QCD_EMenriched_PT20to30" process
	# (QCD background sample with EM enrichment and 20 < Pt/GeV < 30)
	process.scaleZtoElecTau_QCD_EMenriched_Pt20to30 = composeFactorizationSequence(
		process = process,
		processName = "QCD_EMenriched_Pt20to30",
		dqmDirectoryIn_factorizedTightEvtSel = \
                         'harvested/QCD_EMenriched_Pt20to30/zElecTauAnalyzer_factorizedWithElectronIsolation/',
		evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
		dqmDirectoryIn_factorizedLooseEvtSel = \
                         'harvested/QCD_EMenriched_Pt20to30/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
		evtSel_factorizedLoose = evtSelZtoElecTau_factorizedLoose,
		meName_numerator = meNameZtoElecTau_numerator,
		meName_denominator = meNameZtoElecTau_denominator,
		dqmDirectoryOut = 'harvested/QCD_EMenriched_Pt20to30_factorized/zElecTauAnalyzer/'
	)

	# configure sequence for applying factorization to "QCD_EMenriched_PT30to80" process
	# (QCD background sample with EM enrichment and 30 < Pt/GeV < 80)
	process.scaleZtoElecTau_QCD_EMenriched_Pt30to80 = composeFactorizationSequence(
		process = process,
		processName = "QCD_EMenriched_Pt30to80",
		dqmDirectoryIn_factorizedTightEvtSel = \
                         'harvested/QCD_EMenriched_Pt30to80/zElecTauAnalyzer_factorizedWithElectronIsolation/',
		evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
		dqmDirectoryIn_factorizedLooseEvtSel = \
                         'harvested/QCD_EMenriched_Pt30to80/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
		evtSel_factorizedLoose = evtSelZtoElecTau_factorizedLoose,
		meName_numerator = meNameZtoElecTau_numerator,
		meName_denominator = meNameZtoElecTau_denominator,
		dqmDirectoryOut = 'harvested/QCD_EMenriched_Pt30to80_factorized/zElecTauAnalyzer/'
	)

	# configure sequence for applying factorization to "QCD_EMenriched_PT80to170" process
	# (QCD background sample with EM enrichment and 80 < Pt/GeV < 170)
	process.scaleZtoElecTau_QCD_EMenriched_Pt80to170 = composeFactorizationSequence(
		process = process,
		processName = "QCD_EMenriched_Pt80to170",
		dqmDirectoryIn_factorizedTightEvtSel = \
                         'harvested/QCD_EMenriched_Pt80to170/zElecTauAnalyzer_factorizedWithElectronIsolation/',
		evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
		dqmDirectoryIn_factorizedLooseEvtSel = \
                         'harvested/QCD_EMenriched_Pt80to170/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
		evtSel_factorizedLoose = evtSelZtoElecTau_factorizedLoose,
		meName_numerator = meNameZtoElecTau_numerator,
		meName_denominator = meNameZtoElecTau_denominator,
		dqmDirectoryOut = 'harvested/QCD_EMenriched_Pt80to170_factorized/zElecTauAnalyzer/'
	)

	# configure sequence for applying factorization to "PhotonJets_Pt15to20" process
	process.scaleZtoElecTau_gammaPlusJets_Pt15to20 = composeFactorizationSequence(
		process = process,
		processName = "gammaPlusJets_Pt15to20",
		dqmDirectoryIn_factorizedTightEvtSel = \
                         'harvested/gammaPlusJets_Pt15to20/zElecTauAnalyzer_factorizedWithElectronIsolation/',
		evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
		dqmDirectoryIn_factorizedLooseEvtSel = \
                         'harvested/gammaPlusJets_Pt15to20/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
		evtSel_factorizedLoose = evtSelZtoElecTau_factorizedLoose,
		meName_numerator = meNameZtoElecTau_numerator,
		meName_denominator = meNameZtoElecTau_denominator,
		dqmDirectoryOut = 'harvested/gammaPlusJets_Pt15to20_factorized/zElecTauAnalyzer/'
	)

	# configure sequence for applying factorization to "PhotonJets_Pt20to30" process
	process.scaleZtoElecTau_gammaPlusJets_Pt20to30 = composeFactorizationSequence(
		process = process,
		processName = "gammaPlusJets_Pt20to30",
		dqmDirectoryIn_factorizedTightEvtSel = \
                         'harvested/gammaPlusJets_Pt20to30/zElecTauAnalyzer_factorizedWithElectronIsolation/',
		evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
		dqmDirectoryIn_factorizedLooseEvtSel = \
                         'harvested/gammaPlusJets_Pt20to30/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
		evtSel_factorizedLoose = evtSelZtoElecTau_factorizedLoose,
		meName_numerator = meNameZtoElecTau_numerator,
		meName_denominator = meNameZtoElecTau_denominator,
		dqmDirectoryOut = 'harvested/gammaPlusJets_Pt20to30_factorized/zElecTauAnalyzer/'
	)

	# configure sequence for applying factorization to "PhotonJets_Pt30to50" process
	process.scaleZtoElecTau_gammaPlusJets_Pt30to50 = composeFactorizationSequence(
		process = process,
		processName = "gammaPlusJets_Pt30to50",
		dqmDirectoryIn_factorizedTightEvtSel = \
                         'harvested/gammaPlusJets_Pt30to50/zElecTauAnalyzer_factorizedWithElectronIsolation/',
		evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
		dqmDirectoryIn_factorizedLooseEvtSel = \
                         'harvested/gammaPlusJets_Pt30to50/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
		evtSel_factorizedLoose = evtSelZtoElecTau_factorizedLoose,
		meName_numerator = meNameZtoElecTau_numerator,
		meName_denominator = meNameZtoElecTau_denominator,
		dqmDirectoryOut = 'harvested/gammaPlusJets_Pt30to50_factorized/zElecTauAnalyzer/'
	)

	# configure sequence for applying factorization to "PhotonJets_Pt50to80" process
	process.scaleZtoElecTau_gammaPlusJets_Pt50to80 = composeFactorizationSequence(
		process = process,
		processName = "gammaPlusJets_Pt50to80",
		dqmDirectoryIn_factorizedTightEvtSel = \
                         'harvested/gammaPlusJets_Pt50to80/zElecTauAnalyzer_factorizedWithElectronIsolation/',
		evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
		dqmDirectoryIn_factorizedLooseEvtSel = \
                         'harvested/gammaPlusJets_Pt50to80/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
		evtSel_factorizedLoose = evtSelZtoElecTau_factorizedLoose,
		meName_numerator = meNameZtoElecTau_numerator,
		meName_denominator = meNameZtoElecTau_denominator,
		dqmDirectoryOut = 'harvested/gammaPlusJets_Pt50to80_factorized/zElecTauAnalyzer/'
	)

	# configure sequence for applying factorization to "PhotonJets_Pt80to120" process
	process.scaleZtoElecTau_gammaPlusJets_Pt80to120 = composeFactorizationSequence(
		process = process,
		processName = "gammaPlusJets_Pt80to120",
		dqmDirectoryIn_factorizedTightEvtSel = \
                         'harvested/gammaPlusJets_Pt80to120/zElecTauAnalyzer_factorizedWithElectronIsolation/',
		evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
		dqmDirectoryIn_factorizedLooseEvtSel = \
                         'harvested/gammaPlusJets_Pt80to120/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
		evtSel_factorizedLoose = evtSelZtoElecTau_factorizedLoose,
		meName_numerator = meNameZtoElecTau_numerator,
		meName_denominator = meNameZtoElecTau_denominator,
		dqmDirectoryOut = 'harvested/gammaPlusJets_Pt80to120_factorized/zElecTauAnalyzer/'
	)

	# configure sequence for applying factorization to "PhotonJets_Pt30" process
	process.scaleZtoElecTau_gammaPlusJets_Pt30 = composeFactorizationSequence(
		process = process,
		processName = "gammaPlusJets_Pt30",
		dqmDirectoryIn_factorizedTightEvtSel = \
                         'harvested/gammaPlusJets_Pt30/zElecTauAnalyzer_factorizedWithElectronIsolation/',
		evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
		dqmDirectoryIn_factorizedLooseEvtSel = \
                         'harvested/gammaPlusJets_Pt30/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
		evtSel_factorizedLoose = evtSelZtoElecTau_factorizedLoose,
		meName_numerator = meNameZtoElecTau_numerator,
		meName_denominator = meNameZtoElecTau_denominator,
		dqmDirectoryOut = 'harvested/gammaPlusJets_Pt30_factorized/zElecTauAnalyzer/'
	)

	# compute gamma+jets background sum using factorized histograms and FilterStatistics objects
	process.addZtoElecTau_gammaPlusJetsSum.gammaPlusJetsSum.dqmDirectories_input = cms.vstring(
		'harvested/gammaPlusJets_Pt15to20_factorized',
		'harvested/gammaPlusJets_Pt20to30_factorized',
		'harvested/gammaPlusJets_Pt30to50_factorized',
		'harvested/gammaPlusJets_Pt50to80_factorized',
		'harvested/gammaPlusJets_Pt80to120_factorized'
    )

	# compute QCD background sum using factorized histograms and FilterStatistics objects
	process.addZtoElecTau_qcdSum.qcdSum.dqmDirectories_input = cms.vstring(
		'harvested/QCD_BCtoE_Pt20to30_factorized',
		'harvested/QCD_BCtoE_Pt30to80_factorized',
		'harvested/QCD_BCtoE_Pt80to170_factorized',
		'harvested/QCD_EMenriched_Pt20to30_factorized',
		'harvested/QCD_EMenriched_Pt30to80_factorized',
		'harvested/QCD_EMenriched_Pt80to170_factorized'
	)
	process.addZtoElecTau_qcdEMenrichedSum.qcdEMenrichedSum.dqmDirectories_input = cms.vstring(
		'harvested/QCD_EMenriched_Pt20to30_factorized',
		'harvested/QCD_EMenriched_Pt30to80_factorized',
		'harvested/QCD_EMenriched_Pt80to170_factorized'
	)
	process.addZtoElecTau_qcdBCtoESum.qcdBCtoESum.dqmDirectories_input = cms.vstring(
		'harvested/QCD_BCtoE_Pt20to30_factorized',
		'harvested/QCD_BCtoE_Pt30to80_factorized',
		'harvested/QCD_BCtoE_Pt80to170_factorized'
	)
	process.addZtoElecTau = cms.Sequence(
		process.scaleZtoElecTau_QCD_EMenriched_Pt20to30
		+ process.scaleZtoElecTau_QCD_EMenriched_Pt30to80
		+ process.scaleZtoElecTau_QCD_EMenriched_Pt80to170
		+ process.scaleZtoElecTau_QCD_BCtoE_Pt20to30
		+ process.scaleZtoElecTau_QCD_BCtoE_Pt30to80
		+ process.scaleZtoElecTau_QCD_BCtoE_Pt80to170
		#+ process.scaleZtoElecTau_gammaPlusJets_Pt15to20
		#+ process.scaleZtoElecTau_gammaPlusJets_Pt20to30
		#+ process.scaleZtoElecTau_gammaPlusJets_Pt30to50
		#+ process.scaleZtoElecTau_gammaPlusJets_Pt50to80
		#+ process.scaleZtoElecTau_gammaPlusJets_Pt80to120
		+ process.scaleZtoElecTau_gammaPlusJets_Pt30
		#			+ process.addZtoElecTau_qcdSum 
		+ process.addZtoElecTau_qcdBCtoESum 
		+ process.addZtoElecTau_qcdEMenrichedSum 
		#+ process.addZtoElecTau_gammaPlusJetsSum 
		#			+ process.addZtoElecTau_smSum
	)

#--------------------------------------------------------------------------------
# utility functions specific to factorization
# of tau id. efficiencies in Z --> tau-jet + tau-jet channel
#--------------------------------------------------------------------------------

def enableFactorization_runZtoDiTau(process):
    process.load("TauAnalysis.Configuration.selectZtoDiTau_factorized_cff")
    process.selectZtoDiTauEvents_factorized = cms.Sequence(
        process.selectZtoDiTauEvents
       * process.selectZtoDiTauEventsLoose2ndTau
    )
    process.p.replace(process.selectZtoDiTauEvents, process.selectZtoDiTauEvents_factorized)
    process.load("TauAnalysis.Configuration.analyzeZtoDiTau_factorized_cff")
    process.analyzeZtoDiTauEvents_factorized = cms.Sequence(
        process.analyzeZtoDiTauEvents_factorizedLoose2ndTau
       * process.analyzeZtoDiTauEvents_factorizedTight2ndTau
    )
    process.p.replace(process.analyzeZtoDiTauEvents, process.analyzeZtoDiTauEvents_factorized)

def enableFactorization_makeZtoDiTauPlots_grid(
    process,
    factorizationSequenceName = "loadAndFactorizeZtoDiTauSamples",
    samplesToFactorize = [ 'qcdDiJet', ],
    relevantMergedSamples = [ 'qcdSum', ],
    mergedToRecoSampleDict = {},
    mergedSampleAdderModule = lambda sample: 'addZtoDiTau_%s' % (sample),
    dqmDirectoryOut = 
    lambda sample:'/harvested/%s_factorized/zDiTauAnalyzer/'% (sample),
    dqmDirectoryOutUnfactorized = 
    lambda sample:'/harvested/%s/zDiTauAnalyzer/'% (sample),
    dqmDirectoryTight = 
    lambda sample:'/harvested/%s/zDiTauAnalyzer_factorizedTight2ndTau/' % (sample),
    dqmDirectoryLoose = 
    lambda sample:'/harvested/%s/zDiTauAnalyzer_factorizedLoose2ndTau/' % (sample),
    pyObjectLabel = ""):

    process.load("TauAnalysis.Configuration.analyzeZtoDiTau_cfi")

    # define list of event selection criteria on "tight" tau id. branch
    # of the analysis, **before** applying factorization of
    # lead. track Pt, track isolation and ECAL isolation efficiencies
    evtSelZtoDiTau_factorizedTight = [
        process.evtSelGenPhaseSpace,
        #process.evtSelTrigger,
        process.evtSelDataQuality,
        process.evtSelPrimaryEventVertex,
        process.evtSelPrimaryEventVertexQuality,
        process.evtSelPrimaryEventVertexPosition,
        process.evtSelFirstTauEta,
        process.evtSelFirstTauPt,
        process.evtSelSecondTauEta,
        process.evtSelSecondTauPt,
        process.evtSelFirstTauLeadTrk,
        process.evtSelFirstTauLeadTrkPt,
        process.evtSelFirstTauTaNCdiscr,
        process.evtSelFirstTauTrkIso,
        process.evtSelFirstTauEcalIso,
        process.evtSelFirstTauProng,
        process.evtSelFirstTauCharge,
        process.evtSelFirstTauMuonVeto,
        process.evtSelFirstTauElectronVeto,
        process.evtSelSecondTauLeadTrk,
        process.evtSelSecondTauLeadTrkPt,
        process.evtSelSecondTauTaNCdiscr,
        process.evtSelSecondTauTrkIso,
        process.evtSelSecondTauEcalIso,
        process.evtSelSecondTauProng,
        process.evtSelSecondTauCharge
    ]

    # define list of event selection criteria on "loose" tau id. branch
    # of the analysis, **after** applying factorization of
    # lead. track Pt, track isolation and ECAL isolation efficiencies
    evtSelZtoDiTau_factorizedLoose = [
        process.evtSelSecondTauMuonVeto,
        process.evtSelSecondTauElectronVeto,
        process.evtSelDiTauCandidateForDiTauAntiOverlapVeto,
        process.evtSelDiTauCandidateForDiTauZeroCharge,
        process.evtSelDiTauCandidateForDiTauAcoplanarity,
        process.evtSelDiTauCandidateForDiTauPzetaDiff
        #process.evtSelCentralJetVeto
    ]

    # defines names of MonitorElements used as numerator and denominator
    # to compute factorization scale-factor
    meNameZtoDiTau_numerator = "evtSelSecondTauCharge/passed_cumulative_numWeighted"
    meNameZtoDiTau_denominator = "evtSelSecondTauLeadTrkPt/processed_cumulative_numWeighted"

    # Loop over the samples and create sequences
    # for each of the factorization jobs and add them to the factorization
    # sequence
    factorizationSequence = getattr(process, factorizationSequenceName)
    for sample in samplesToFactorize:
        new_factorization_sequence = composeFactorizationSequence(
            process = process,
            processName = sample + "_" + pyObjectLabel,
            dqmDirectoryIn_factorizedTightEvtSel = dqmDirectoryTight(sample),
            evtSel_factorizedTight = evtSelZtoDiTau_factorizedTight,
            dqmDirectoryIn_factorizedLooseEvtSel = dqmDirectoryLoose(sample),
            evtSel_factorizedLoose = evtSelZtoDiTau_factorizedLoose,
            meName_numerator = meNameZtoDiTau_numerator,
            meName_denominator = meNameZtoDiTau_denominator,
            dqmDirectoryOut = dqmDirectoryOut(sample),
            dropInputDirectories = False
        )
        new_factorization_seq_name = "scaleZtoDiTau_%s_%s" % (sample, pyObjectLabel)
        setattr(process, new_factorization_seq_name, new_factorization_sequence)
        factorizationSequence += new_factorization_sequence

    # Now update any of the relevant mergers
    for mergedSample in relevantMergedSamples:
        # Get the module that is doing the merging, if it exists
        if not hasattr(process.mergeSamplesZtoDiTau, "merge_%s"%(mergedSample)): continue
        merger = getattr(process.mergeSamplesZtoDiTau, "merge_%s" % (mergedSample))
        
        # Get the subsamples associated with this merged sample
        subsamples = mergedToRecoSampleDict[mergedSample]['samples']
        # Set the adder to use our new factorized inputs
        def merge_directories(_list):
            for sample in _list:
                if sample in samplesToFactorize:
                    yield dqmDirectoryOut(sample)
                else:
                    yield dqmDirectoryOutUnfactorized(sample)
                    
        merger.dqmDirectories_input = cms.vstring(list(merge_directories(subsamples)))
    
    # Update the plot sources in the plot jobs.  Note that we don't need to do
    # this for the merged samples, since we have replaced the HistAdder sources
    for plotterModuleName in [ 'plotZtoDiTau', ]:
        plotterModuleProcesses = getattr(process, plotterModuleName).processes
        for sample in samplesToFactorize:
            if hasattr(plotterModuleProcesses, sample):
                getattr(plotterModuleProcesses, sample).dqmDirectory = \
                        cms.string("/harvested/%s_factorized" % sample)

#--------------------------------------------------------------------------------
# utility functions specific to factorization
# of tau isolation efficiencies in W --> tau-jet + nu channel
#--------------------------------------------------------------------------------

def enableFactorization_runWtoTauNu(process):
    process.load("TauAnalysis.Configuration.selectWtoTauNu_factorized_cff")
    process.selectWtoTauNuEvents_factorized = cms.Sequence( process.selectWtoTauNuEvents
                                                           *process.selectWtoTauNuEventsLooseTauIsolation )
    process.p.replace(process.selectWtoTauNuEvents, process.selectWtoTauNuEvents_factorized)
    process.load("TauAnalysis.Configuration.analyzeWtoTauNu_factorized_cff")
    process.analyzeWtoTauNuEvents_factorized = cms.Sequence( process.analyzeWtoTauNuEvents_factorizedWithoutTauIsolation
                                                            *process.analyzeWtoTauNuEvents_factorizedWithTauIsolation )
    process.p.replace(process.analyzeWtoTauNuEvents, process.analyzeWtoTauNuEvents_factorized)

def enableFactorization_makeWtoTauNuPlots(process,
      dqmDirectoryIn_qcd_W = 'harvested/qcd_W/wTauNuAnalyzer',
      dqmDirectoryOut_qcd_W = 'harvested/qcd_W_factorized/wTauNuAnalyzer',
      modName_addWtoTauNu_qcd = "addWtoTauNu_qcd",
     # modName_addWtoTauNu_smSum = "addWtoTauNu_smSum",
      seqName_addWtoTauNu = "addWtoTauNu",
      pyObjectLabel = ""):
    process.load("TauAnalysis.Configuration.analyzeWtoTauNu_cfi")
    # define list of event selection criteria on "tight" tau isolation branch of the analysis,
    # **before** applying factorization of tauTaNC+prong+charge efficiencies
    evtSelWtoTauNu_factorizedTight = [
        process.evtSelPrimaryEventVertex,
        process.evtSelPrimaryEventVertexQuality,
        process.evtSelPrimaryEventVertexPosition,
        process.evtSelTauEta,
        process.evtSelTauPt,
        process.evtSelPFMetPt,
        process.evtSelMetPt,
        process.evtSelTauLeadTrk,
        process.evtSelTauLeadTrkPt,
        process.evtSelTauIso,
        process.evtSelTauTaNC,
        process.evtSelTauProng,
        process.evtSelTauCharge,
        process.evtSelTauMuonVeto,
        process.evtSelTauElectronVeto,
        process.evtSelTauEcalCrackVeto
        ]

    # define list of event selection criteria on "loose" muon isolation branch of the analysis,
    # **after** applying factorization of muon track + ECAL isolation efficiencies
    evtSelWtoTauNu_factorizedLoose = [
        process.evtSelCentralJetVeto,
        process.evtSelRecoilEnergyFromCaloTowers,
        process.evtSelMetTopology
    ]

  # defines names of MonitorElements used as numerator and denominator
    # to compute factorization scale-factor
    meNameWtoTauNu_numerator = "evtSelTauEcalCrackVeto/passed_cumulative_numWeighted"
    meNameWtoTauNu_denominator = "evtSelTauLeadTrkPt/processed_cumulative_numWeighted"


   # configure sequence for applying factorization to "qcd_W" process (QCD background sample for Pt(hat) > 15 GeV)
    scaleWtoTauNu_qcd_W = composeFactorizationSequence(
        process = process,
        processName = "qcd_W" + "_" + pyObjectLabel,
        dqmDirectoryIn_factorizedTightEvtSel = "harvested/qcd_W/wTauNuAnalyzer_factorizedWithTauIsolation/",
        evtSel_factorizedTight = evtSelWtoTauNu_factorizedTight,
        dqmDirectoryIn_factorizedLooseEvtSel = "harvested/qcd_W/wTauNuAnalyzer_factorizedWithoutTauIsolation/",
        evtSel_factorizedLoose = evtSelWtoTauNu_factorizedLoose,
        meName_numerator = meNameWtoTauNu_numerator,
        meName_denominator = meNameWtoTauNu_denominator,
        dqmDirectoryOut = dqmDirectoryOut_qcd_W + '/'
    )

    scaleWtoTauNuName_qcd_W = "scaleWtoTauNu_qcd_W" + "_" + pyObjectLabel
    setattr(process,scaleWtoTauNuName_qcd_W, scaleWtoTauNu_qcd_W)

    addWtoTauNu_qcd = getattr(process, modName_addWtoTauNu_qcd)
    addWtoTauNu_qcd.qcd.dqmDirectories_input = cms.vstring(
        dqmDirectoryOut_qcd_W + '/'
        )
    addWtoTauNu = cms.Sequence(
        getattr(process, scaleWtoTauNuName_qcd_W)
        )
    addWtoTauNu._seq = addWtoTauNu._seq * getattr(process, modName_addWtoTauNu_qcd)
#    if hasattr(process, modName_addWtoTauNu_smSum):
#        addWtoTauNu._seq = addWtoTauNu._seq * getattr(process,modName_addWtoTauNu_smSum)
    setattr(process,seqName_addWtoTauNu, addWtoTauNu)
  
    process.plotWtoTauNu.processes.qcd_W.dqmDirectory = cms.string('harvested/qcd_W_factorized')

#--------------------------------------------------------------------------------
# utility functions specific to factorization
# of muon isolation efficiencies in MSSM Higgs A/H --> mu + tau-jet channel
#--------------------------------------------------------------------------------

def enableFactorization_runAHtoMuTau(process):
    process.load("TauAnalysis.Configuration.selectAHtoMuTau_factorized_cff")
    process.selectAHtoMuTauEvents_factorized = cms.Sequence(
        process.selectAHtoMuTauEvents
       * process.selectAHtoMuTauEventsLooseMuonIsolation
    )
    process.p.replace(process.selectAHtoMuTauEvents, process.selectAHtoMuTauEvents_factorized)
    process.load("TauAnalysis.Configuration.analyzeAHtoMuTau_factorized_cff")
    process.analyzeAHtoMuTauEvents_factorized = cms.Sequence(
        process.analyzeAHtoMuTauEvents_factorizedWithoutMuonIsolation
       * process.analyzeAHtoMuTauEvents_factorizedWithMuonIsolation
    )
    process.p.replace(process.analyzeAHtoMuTauEvents, process.analyzeAHtoMuTauEvents_factorized)

def enableFactorization_makeAHtoMuTauPlots_grid(
    process,
    factorizationSequenceName = "loadAndFactorizeAHtoMuTauSamples",
    samplesToFactorize = [ 'InclusivePPmuX', 'PPmuXptGt20Mu10', 'PPmuXptGt20Mu15' ],
    relevantMergedSamples = [ 'qcdSum', ],
    mergedToRecoSampleDict = {},
    mergedSampleAdderModule = lambda sample, btag: 'addAHtoMuTau_%s_%s' % (btag, sample),
    dqmDirectoryOut = 
    lambda sample, btag:'/harvested/%s_factorized/ahMuTauAnalyzer_%s/'% (sample, btag),
    dqmDirectoryOutUnfactorized = 
    lambda sample, btag:'/harvested/%s/ahMuTauAnalyzer_%s/'% (sample, btag),
    dqmDirectoryTight = 
    lambda sample, btag:'/harvested/%s/ahMuTauAnalyzer_%s_factorizedWithMuonIsolation/' % (sample, btag),
    dqmDirectoryLoose = 
    lambda sample, btag:'/harvested/%s/ahMuTauAnalyzer_%s_factorizedWithoutMuonIsolation/' % (sample, btag),
    pyObjectLabel = ""):

    process.load("TauAnalysis.Configuration.analyzeAHtoMuTau_cfi")

    # define list of event selection criteria on "tight" muon isolation branch
    # of the analysis, **before** applying factorization of muon track + ECAL
    # isolation efficiencies
    evtSelAHtoMuTau_factorizedTight = [
        process.evtSelGenPhaseSpace,
        process.evtSelTrigger,
        process.evtSelDataQuality,
        process.evtSelPrimaryEventVertex,
        process.evtSelPrimaryEventVertexQuality,
        process.evtSelPrimaryEventVertexPosition,
        process.evtSelGlobalMuon,
        process.evtSelMuonEta,
        process.evtSelMuonPt,
        process.evtSelTauAntiOverlapWithMuonsVeto,
        process.evtSelTauEta,
        process.evtSelTauPt,
        process.evtSelMuonVbTfId,
        process.evtSelMuonPFRelIso
    ]

    # define list of event selection criteria on "loose" muon isolation branch
    # of the analysis, **after** applying factorization of muon track + ECAL
    # isolation efficiencies
    evtSelAHtoMuTau_factorizedLoose = [
        process.evtSelMuonAntiPion,
        process.evtSelMuonTrkIP,
        process.evtSelTauLeadTrk,
        process.evtSelTauLeadTrkPt,
        process.evtSelTauTaNCdiscr,
        process.evtSelTauTrkIso,
        process.evtSelTauEcalIso,
        process.evtSelTauProng,
        process.evtSelTauCharge,
        process.evtSelTauMuonVeto,
        process.evtSelTauElectronVeto,
        process.evtSelDiTauCandidateForAHtoMuTauAntiOverlapVeto,
        process.evtSelDiTauCandidateForAHtoMuTauZeroCharge,
        process.evtSelDiTauCandidateForAHtoMuTauMt1MET,
        process.evtSelDiTauCandidateForAHtoMuTauPzetaDiff,
        process.evtSelDiMuPairZmumuHypothesisVetoByMass,
        process.evtSelDiMuPairZmumuHypothesisVetoByLooseIsolationAndCharge
    ]

    # Make specialized cases for the w/ w/o btag cases
    evtSelAHtoMuTau_factorizedLoose_specialized = {
        'woBtag' : copy.deepcopy(evtSelAHtoMuTau_factorizedLoose),
        'wBtag' : copy.deepcopy(evtSelAHtoMuTau_factorizedLoose)
    }
    evtSelAHtoMuTau_factorizedLoose_specialized['woBtag'].append(
        process.evtSelNonCentralJetEt20bTag)
    evtSelAHtoMuTau_factorizedLoose_specialized['wBtag'].append(
        process.evtSelCentralJetEt20)
    evtSelAHtoMuTau_factorizedLoose_specialized['wBtag'].append(
        process.evtSelCentralJetEt20bTag)

    # defines names of MonitorElements used as numerator and denominator
    # to compute factorization scale-factor
    meNameAHtoMuTau_numerator = "evtSelMuonPFRelIso/passed_cumulative_numWeighted"
    meNameAHtoMuTau_denominator = "evtSelMuonPFRelIso/processed_cumulative_numWeighted"

    # Loop over the samples and btag options and create sequences
    # for each of the factorization jobs and add them to the factorization
    # sequence
    factorizationSequence = getattr(process, factorizationSequenceName)
    for sample, bTagOption in [(sample, bTagOption) 
                               for bTagOption in ['woBtag', 'wBtag'] 
                               for sample in samplesToFactorize]:
        new_factorization_sequence = composeFactorizationSequence(
            process = process,
            processName = sample + "_" + bTagOption + "_" + pyObjectLabel,
            dqmDirectoryIn_factorizedTightEvtSel = dqmDirectoryTight(
                sample, bTagOption),
            evtSel_factorizedTight = evtSelAHtoMuTau_factorizedTight,
            dqmDirectoryIn_factorizedLooseEvtSel = dqmDirectoryLoose(
                sample, bTagOption),
            evtSel_factorizedLoose = evtSelAHtoMuTau_factorizedLoose_specialized[bTagOption],
            meName_numerator = meNameAHtoMuTau_numerator,
            meName_denominator = meNameAHtoMuTau_denominator,
            dqmDirectoryOut = dqmDirectoryOut(sample, bTagOption),
            dropInputDirectories = False
        )
        new_factorization_seq_name = "scaleAHtoMuTau_%s_%s_%s" % (
            bTagOption, sample, pyObjectLabel)
        setattr(process, new_factorization_seq_name, new_factorization_sequence)
        factorizationSequence += new_factorization_sequence

    # Now update any of the relevant mergers
    for btag in ['woBtag', 'wBtag']:
        for mergedSample in relevantMergedSamples:
            # Get the module that is doing the merging, if it exists
            if not hasattr(process.mergeSamplesAHtoMuTau, 
                           "merge_%s_%s"%(mergedSample,btag)):
                continue
            merger = getattr(process.mergeSamplesAHtoMuTau, 
                                    "merge_%s_%s" % (mergedSample, btag))

            # Get the subsamples associated with this merged sample
            subsamples = mergedToRecoSampleDict[mergedSample]['samples']
            # Set the adder to use our new factorized inputs
            def merge_directories(_list):
                for sample in _list:
                    if sample in samplesToFactorize:
                        yield dqmDirectoryOut(sample, btag)
                    else:
                        yield dqmDirectoryOutUnfactorized(sample, btag)

            merger.dqmDirectories_input = cms.vstring(list(merge_directories(subsamples)))
    
    # Update the plot sources in the plot jobs.  Note that we don't need to do
    # this for the merged samples, since we have replaced the HistAdder sources
    for plotterModuleName in ['plotAHtoMuTau_woBtag', 'plotAHtoMuTau_wBtag']:
        if hasattr(process, plotterModuleName):
            plotterModuleProcesses = getattr(process, plotterModuleName).processes
            for sample in samplesToFactorize:
                if hasattr(plotterModuleProcesses, sample):
                    getattr(plotterModuleProcesses, sample).dqmDirectory = \
                      cms.string("/harvested/%s_factorized" % sample)

