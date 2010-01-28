import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.DQMTools.drawJobConfigurator import *
from TauAnalysis.Configuration.analyzeZtoMuTau_cfi import *
from TauAnalysis.Configuration.analyzeZtoElecMu_cfi import *
from TauAnalysis.Configuration.analyzeZtoElecTau_cfi import *
from TauAnalysis.Configuration.analyzeWtoTauNu_cfi import *

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

        analyzer.filters.remove(evtSel_tight)
        analyzer.filters.append(evtSel_loose)

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
        
        dqmSubDirectoryNames.append(drawJobConfigurator._composeSubDirectoryName(afterCut = afterCut, beforeCut = beforeCut))

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
        
        setattr(dqmHistScaler_plotsLooseEvtSel, "meNameNumerator", cms.string("".join([meName_numerator, meOptionsNumWeighted])))
        setattr(dqmHistScaler_plotsLooseEvtSel, "meNameDenominator", cms.string("".join([meName_denominator, meOptionsNumWeighted])))
        setattr(dqmHistScaler_plotsLooseEvtSel, "dqmDirectory_factorizedLooseSel", dqmDirectory_factorizedLooseSel)
        setattr(dqmHistScaler_plotsLooseEvtSel, "dqmDirectory_factorizedTightSel", dqmDirectory_factorizedTightSel)
        setattr(dqmHistScaler_plotsLooseEvtSel, "meType", meType)
        
        setattr(dqmHistScaler_filterStatLooseEvtSel, "meNameNumerator", cms.string("".join([meName_numerator, meOptionsNumWeighted])))
        setattr(dqmHistScaler_filterStatLooseEvtSel, "meNameDenominator", cms.string("".join([meName_denominator, meOptionsNumWeighted])))
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

def enableFactorization_makeZtoMuTauPlots(process,
        dqmDirectoryIn_InclusivePPmuX = 'harvested/InclusivePPmuX/zMuTauAnalyzer',
        dqmDirectoryOut_InclusivePPmuX = 'harvested/InclusivePPmuX_factorized/zMuTauAnalyzer',
        dqmDirectoryIn_PPmuXptGt20 = 'harvested/PPmuXptGt20/zMuTauAnalyzer',
        dqmDirectoryOut_PPmuXptGt20 = 'harvested/PPmuXptGt20_factorized/zMuTauAnalyzer',                                  
        modName_addZtoMuTau_qcdSum = "addZtoMuTau_qcdSum",
        modName_addZtoMuTau_smSum = "addZtoMuTau_smSum",                                
        seqName_addZtoMuTau = "addZtoMuTau",
        pyObjectLabel = ""):

    # define list of event selection criteria on "tight" muon isolation branch of the analysis,
    # **before** applying factorization of muon track + ECAL isolation efficiencies
    evtSelZtoMuTau_factorizedTight = [
        genPhaseSpaceCut,
        evtSelTrigger,
        evtSelPrimaryEventVertex,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,
        evtSelGlobalMuon,
        evtSelMuonEta,
        evtSelMuonPt,
        evtSelTauAntiOverlapWithMuonsVeto,
        evtSelTauEta,
        evtSelTauPt,
        evtSelMuonTrkIso,
        evtSelMuonEcalIso
    ]

    # define list of event selection criteria on "loose" muon isolation branch of the analysis,
    # **after** applying factorization of muon track + ECAL isolation efficiencies
    evtSelZtoMuTau_factorizedLoose = [
        evtSelMuonAntiPion,
        evtSelMuonTrkIP,
        evtSelTauLeadTrk,
        evtSelTauLeadTrkPt,
        evtSelTauTrkIso,
        evtSelTauEcalIso,
        evtSelTauProng,
        evtSelTauCharge,
        evtSelTauMuonVeto,
        evtSelDiTauCandidateForMuTauAntiOverlapVeto,
        evtSelDiTauCandidateForMuTauZeroCharge,
        evtSelDiTauCandidateForMuTauAcoplanarity12,
        evtSelDiTauCandidateForMuTauMt1MET,
        evtSelDiTauCandidateForMuTauPzetaDiff,
        evtSelDiMuPairZmumuHypothesisVeto
    ]

    # defines names of MonitorElements used as numerator and denominator
    # to compute factorization scale-factor
    meNameZtoMuTau_numerator = "evtSelMuonEcalIso/passed_cumulative_numWeighted"
    meNameZtoMuTau_denominator = "evtSelMuonTrkIso/processed_cumulative_numWeighted"

    # configure sequence for applying factorization to "InclusivePPmuX" process
    # (QCD background sample for Pt(hat) < 20 GeV region in phase-space)
    scaleZtoMuTau_InclusivePPmuX = composeFactorizationSequence(
        process = process,
        processName = "InclusivePPmuX" + "_" + pyObjectLabel,
        dqmDirectoryIn_factorizedTightEvtSel = composeDirectoryName(dqmDirectoryIn_InclusivePPmuX, "factorizedWithMuonIsolation"),
        evtSel_factorizedTight = evtSelZtoMuTau_factorizedTight,
        dqmDirectoryIn_factorizedLooseEvtSel = composeDirectoryName(dqmDirectoryIn_InclusivePPmuX, "factorizedWithoutMuonIsolation"),
        evtSel_factorizedLoose = evtSelZtoMuTau_factorizedLoose,
        meName_numerator = meNameZtoMuTau_numerator,
        meName_denominator = meNameZtoMuTau_denominator,
        dqmDirectoryOut = dqmDirectoryOut_InclusivePPmuX + '/'
    )

    scaleZtoMuTauName_InclusivePPmuX = "scaleZtoMuTau_InclusivePPmuX" + "_" + pyObjectLabel
    setattr(process, scaleZtoMuTauName_InclusivePPmuX, scaleZtoMuTau_InclusivePPmuX)
    
    # configure sequence for applying factorization to "PPmuXPPmuXptGt20" process
    # (QCD background sample for Pt(hat) > 20 GeV region in phase-space)
    scaleZtoMuTau_PPmuXptGt20 = composeFactorizationSequence(
        process = process,
        processName = "PPmuXptGt20" + "_" + pyObjectLabel,
        dqmDirectoryIn_factorizedTightEvtSel = composeDirectoryName(dqmDirectoryIn_PPmuXptGt20, "factorizedWithMuonIsolation"),
        evtSel_factorizedTight = evtSelZtoMuTau_factorizedTight,
        dqmDirectoryIn_factorizedLooseEvtSel = composeDirectoryName(dqmDirectoryIn_PPmuXptGt20, "factorizedWithoutMuonIsolation"),
        evtSel_factorizedLoose = evtSelZtoMuTau_factorizedLoose,
        meName_numerator = meNameZtoMuTau_numerator,
        meName_denominator = meNameZtoMuTau_denominator,
        dqmDirectoryOut = dqmDirectoryOut_PPmuXptGt20 + '/'
    )

    scaleZtoMuTauName_PPmuXptGt20 = "scaleZtoMuTau_PPmuXptGt20" + "_" + pyObjectLabel
    setattr(process, scaleZtoMuTauName_PPmuXptGt20, scaleZtoMuTau_PPmuXptGt20)
    
    # compute QCD background sum using factorized histograms and FilterStatistics objects
    addZtoMuTau_qcdSum = getattr(process, modName_addZtoMuTau_qcdSum)
    addZtoMuTau_qcdSum.qcdSum.dqmDirectories_input = cms.vstring(
        dqmDirectoryOut_InclusivePPmuX + '/',
        dqmDirectoryOut_PPmuXptGt20 + '/'
    )
    addZtoMuTau = cms.Sequence(
        getattr(process, scaleZtoMuTauName_InclusivePPmuX)
       + getattr(process, scaleZtoMuTauName_PPmuXptGt20)
    )
    addZtoMuTau._seq = addZtoMuTau._seq * getattr(process, modName_addZtoMuTau_qcdSum)
    if hasattr(process, modName_addZtoMuTau_smSum):
        addZtoMuTau._seq = addZtoMuTau._seq * getattr(process, modName_addZtoMuTau_smSum)
    setattr(process, seqName_addZtoMuTau, addZtoMuTau)

    if hasattr(process, "plotZtoMuTau"):
        process.plotZtoMuTau.processes.InclusivePPmuX.dqmDirectory = cms.string('harvested/InclusivePPmuX_factorized')
        process.plotZtoMuTau.processes.PPmuXptGt20.dqmDirectory = cms.string('harvested/PPmuXptGt20_factorized')

#--------------------------------------------------------------------------------
# utility functions specific to factorization
# of muon isolation efficiencies in Z --> e + mu channel
#--------------------------------------------------------------------------------

def enableFactorization_runZtoElecMu(process):
    process.load("TauAnalysis.Configuration.selectZtoElecMu_factorized_cff")
    process.selectZtoElecMuEvents_factorized = cms.Sequence( process.selectZtoElecMuEvents
                                                            *process.selectZtoElecMuEventsLooseElectronIsolation )
    process.p.replace(process.selectZtoElecMuEvents, process.selectZtoElecMuEvents_factorized)
    process.load("TauAnalysis.Configuration.analyzeZtoElecMu_factorized_cff")
    process.analyzeZtoElecMuEvents_factorized = cms.Sequence( process.analyzeZtoElecMuEvents_factorizedWithoutElectronIsolation
                                                             *process.analyzeZtoElecMuEvents_factorizedWithElectronIsolation )
    process.p.replace(process.analyzeZtoElecMuEvents, process.analyzeZtoElecMuEvents_factorized)

def enableFactorization_makeZtoElecMuPlots(process):

    # define list of event selection criteria on "tight" muon isolation branch of the analysis,
    # **before** applying factorization of muon track + ECAL isolation efficiencies
    evtSelZtoElecMu_factorizedTight = [
        genPhaseSpaceCut,
        evtSelTrigger,
        evtSelPrimaryEventVertex,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,
        evtSelTightElectronId,
        evtSelElectronAntiCrack,
        evtSelElectronEta,
        evtSelElectronPt,
        evtSelGlobalMuon,
        evtSelMuonEta,
        evtSelMuonPt,
        evtSelElectronTrkIso,
        evtSelElectronEcalIso
    ]

    # define list of event selection criteria on "loose" muon isolation branch of the analysis,
    # **after** applying factorization of muon track + ECAL isolation efficiencies
    evtSelZtoElecMu_factorizedLoose = [
        evtSelElectronTrk,
        evtSelElectronTrkIP,
        evtSelMuonTrkIso,
        evtSelMuonEcalIso,
        evtSelMuonAntiPion,
        evtSelMuonTrkIP,
        evtSelDiTauCandidateForElecMuAntiOverlapVeto,
        evtSelDiTauCandidateForElecMuZeroCharge,
        evtSelDiTauCandidateForElecMuAcoplanarity12,
        evtSelDiTauCandidateForElecMuMt1MET,
        evtSelDiTauCandidateForElecMuMt2MET,
        evtSelDiTauCandidateForElecMuPzetaDiff
    ]

    # defines names of MonitorElements used as numerator and denominator
    # to compute factorization scale-factor
    meNameZtoElecMu_numerator = "evtSelElectronEcalIso/passed_cumulative_numWeighted"
    meNameZtoElecMu_denominator = "evtSelElectronTrkIso/processed_cumulative_numWeighted"

    # configure sequence for applying factorization to "InclusivePPmuX" process
    # (QCD background sample for Pt(hat) < 20 GeV region in phase-space)
    process.scaleZtoElecMu_InclusivePPmuX = composeFactorizationSequence(
        process = process,
        processName = "InclusivePPmuX",
        dqmDirectoryIn_factorizedTightEvtSel = 'harvested/InclusivePPmuX/zElecMuAnalyzer_factorizedWithElectronIsolation/',
        evtSel_factorizedTight = evtSelZtoElecMu_factorizedTight,
        dqmDirectoryIn_factorizedLooseEvtSel = 'harvested/InclusivePPmuX/zElecMuAnalyzer_factorizedWithoutElectronIsolation/',
        evtSel_factorizedLoose = evtSelZtoElecMu_factorizedLoose,
        meName_numerator = meNameZtoElecMu_numerator,
        meName_denominator = meNameZtoElecMu_denominator,
        dqmDirectoryOut = 'harvested/InclusivePPmuX_factorized/zElecMuAnalyzer/'
    )

    # configure sequence for applying factorization to "PPmuXPPmuXptGt20" process
    # (QCD background sample for Pt(hat) > 20 GeV region in phase-space)
    process.scaleZtoElecMu_PPmuXptGt20 = composeFactorizationSequence(
        process = process,
        processName = "PPmuXptGt20",
        dqmDirectoryIn_factorizedTightEvtSel = 'harvested/PPmuXptGt20/zElecMuAnalyzer_factorizedWithElectronIsolation/',
        evtSel_factorizedTight = evtSelZtoElecMu_factorizedTight,
        dqmDirectoryIn_factorizedLooseEvtSel = 'harvested/PPmuXptGt20/zElecMuAnalyzer_factorizedWithoutElectronIsolation/',
        evtSel_factorizedLoose = evtSelZtoElecMu_factorizedLoose,
        meName_numerator = meNameZtoElecMu_numerator,
        meName_denominator = meNameZtoElecMu_denominator,
        dqmDirectoryOut = 'harvested/PPmuXptGt20_factorized/zElecMuAnalyzer/'
    )

    # compute QCD background sum using factorized histograms and FilterStatistics objects
    process.addZtoElecMu_qcdSum.qcdSum.dqmDirectories_input = cms.vstring(
        'harvested/InclusivePPmuX_factorized',
        'harvested/PPmuXptGt20_factorized'
    )

    process.addZtoElecMu = cms.Sequence( process.scaleZtoElecMu_InclusivePPmuX + process.scaleZtoElecMu_PPmuXptGt20
                                        +process.addZtoElecMu_qcdSum + process.addZtoElecMu_smSum )

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

    # define list of event selection criteria on "tight" electron isolation branch of the analysis,
    # **before** applying factorization of electron track + ECAL isolation efficiencies
    evtSelZtoElecTau_factorizedTight = [
        genPhaseSpaceCut,
        evtSelTrigger,
        evtSelPrimaryEventVertex,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,
        evtSelTightElectronId,
        evtSelElectronAntiCrack,
        evtSelElectronEta,
        evtSelElectronPt,
        evtSelTauAntiOverlapWithElectronsVeto,
        evtSelTauEta,
        evtSelTauPt,
        evtSelElectronTrkIso,
        evtSelElectronEcalIso
    ]

    # define list of event selection criteria on "loose" electron isolation branch of the analysis,
    # **after** applying factorization of electron track + ECAL isolation efficiencies
    evtSelZtoElecTau_factorizedLoose = [
        evtSelElectronTrk,
        evtSelElectronTrkIP,
        evtSelElectronConversionVeto,
        evtSelTauLeadTrk,
        evtSelTauLeadTrkPt,
        evtSelTauTrkIso,
        evtSelTauEcalIso,
        evtSelTauProng,
        evtSelTauCharge,
        evtSelTauElectronVeto,
        evtSelTauEcalCrackVeto,
        evtSelDiTauCandidateForElecTauAntiOverlapVeto,
        evtSelDiTauCandidateForElecTauZeroCharge,
        evtSelDiTauCandidateForElecTauAcoplanarity12,
        evtSelDiTauCandidateForElecTauMt1MET,
        evtSelDiTauCandidateForElecTauPzetaDiff,
        evtSelElecTauPairZeeHypothesisVeto
    ]

    # defines names of MonitorElements used as numerator and denominator
    # to compute factorization scale-factor
    meNameZtoElecTau_numerator = "evtSelElectronEcalIso/passed_cumulative_numWeighted"
    meNameZtoElecTau_denominator = "evtSelElectronTrkIso/processed_cumulative_numWeighted"

    # configure sequence for applying factorization to "QCD_BCtoE_PT20to30" process
    # (QCD background sample with b/c quark -> electron and 20 < Pt/GeV < 30)
    process.scaleZtoElecTau_QCD_BCtoE_Pt20to30 = composeFactorizationSequence(
        process = process,
        processName = "QCD_BCtoE_Pt20to30",
        dqmDirectoryIn_factorizedTightEvtSel = 'harvested/QCD_BCtoE_Pt20to30/zElecTauAnalyzer_factorizedWithElectronIsolation/',
        evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
        dqmDirectoryIn_factorizedLooseEvtSel = 'harvested/QCD_BCtoE_Pt20to30/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
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
        dqmDirectoryIn_factorizedTightEvtSel = 'harvested/QCD_BCtoE_Pt30to80/zElecTauAnalyzer_factorizedWithElectronIsolation/',
        evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
        dqmDirectoryIn_factorizedLooseEvtSel = 'harvested/QCD_BCtoE_Pt30to80/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
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
        dqmDirectoryIn_factorizedTightEvtSel = 'harvested/QCD_BCtoE_Pt80to170/zElecTauAnalyzer_factorizedWithElectronIsolation/',
        evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
        dqmDirectoryIn_factorizedLooseEvtSel = 'harvested/QCD_BCtoE_Pt80to170/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
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
        dqmDirectoryIn_factorizedTightEvtSel = 'harvested/QCD_EMenriched_Pt20to30/zElecTauAnalyzer_factorizedWithElectronIsolation/',
        evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
        dqmDirectoryIn_factorizedLooseEvtSel = 'harvested/QCD_EMenriched_Pt20to30/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
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
        dqmDirectoryIn_factorizedTightEvtSel = 'harvested/QCD_EMenriched_Pt30to80/zElecTauAnalyzer_factorizedWithElectronIsolation/',
        evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
        dqmDirectoryIn_factorizedLooseEvtSel = 'harvested/QCD_EMenriched_Pt30to80/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
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
        dqmDirectoryIn_factorizedTightEvtSel = 'harvested/QCD_EMenriched_Pt80to170/zElecTauAnalyzer_factorizedWithElectronIsolation/',
        evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
        dqmDirectoryIn_factorizedLooseEvtSel = 'harvested/QCD_EMenriched_Pt80to170/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
        evtSel_factorizedLoose = evtSelZtoElecTau_factorizedLoose,
        meName_numerator = meNameZtoElecTau_numerator,
        meName_denominator = meNameZtoElecTau_denominator,
        dqmDirectoryOut = 'harvested/QCD_EMenriched_Pt80to170_factorized/zElecTauAnalyzer/'
    )

    # configure sequence for applying factorization to "PhotonJets_Pt15to20" process
    process.scaleZtoElecTau_gammaPlusJets_Pt15to20 = composeFactorizationSequence(
        process = process,
        processName = "gammaPlusJets_Pt15to20",
        dqmDirectoryIn_factorizedTightEvtSel = 'harvested/gammaPlusJets_Pt15to20/zElecTauAnalyzer_factorizedWithElectronIsolation/',
        evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
        dqmDirectoryIn_factorizedLooseEvtSel = 'harvested/gammaPlusJets_Pt15to20/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
        evtSel_factorizedLoose = evtSelZtoElecTau_factorizedLoose,
        meName_numerator = meNameZtoElecTau_numerator,
        meName_denominator = meNameZtoElecTau_denominator,
        dqmDirectoryOut = 'harvested/gammaPlusJets_Pt15to20_factorized/zElecTauAnalyzer/'
    )

    # configure sequence for applying factorization to "PhotonJets_Pt20to25" process
    process.scaleZtoElecTau_gammaPlusJets_Pt20to25 = composeFactorizationSequence(
        process = process,
        processName = "gammaPlusJets_Pt20to25",
        dqmDirectoryIn_factorizedTightEvtSel = 'harvested/gammaPlusJets_Pt20to25/zElecTauAnalyzer_factorizedWithElectronIsolation/',
        evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
        dqmDirectoryIn_factorizedLooseEvtSel = 'harvested/gammaPlusJets_Pt20to25/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
        evtSel_factorizedLoose = evtSelZtoElecTau_factorizedLoose,
        meName_numerator = meNameZtoElecTau_numerator,
        meName_denominator = meNameZtoElecTau_denominator,
        dqmDirectoryOut = 'harvested/gammaPlusJets_Pt20to25_factorized/zElecTauAnalyzer/'
    )

    # configure sequence for applying factorization to "PhotonJets_Pt25to30" process
    process.scaleZtoElecTau_gammaPlusJets_Pt25to30 = composeFactorizationSequence(
        process = process,
        processName = "gammaPlusJets_Pt25to30",
        dqmDirectoryIn_factorizedTightEvtSel = 'harvested/gammaPlusJets_Pt25to30/zElecTauAnalyzer_factorizedWithElectronIsolation/',
        evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
        dqmDirectoryIn_factorizedLooseEvtSel = 'harvested/gammaPlusJets_Pt25to30/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
        evtSel_factorizedLoose = evtSelZtoElecTau_factorizedLoose,
        meName_numerator = meNameZtoElecTau_numerator,
        meName_denominator = meNameZtoElecTau_denominator,
        dqmDirectoryOut = 'harvested/gammaPlusJets_Pt25to30_factorized/zElecTauAnalyzer/'
    )

    # configure sequence for applying factorization to "PhotonJets_Pt30to35" process
    process.scaleZtoElecTau_gammaPlusJets_Pt30to35 = composeFactorizationSequence(
        process = process,
        processName = "gammaPlusJets_Pt30to35",
        dqmDirectoryIn_factorizedTightEvtSel = 'harvested/gammaPlusJets_Pt30to35/zElecTauAnalyzer_factorizedWithElectronIsolation/',
        evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
        dqmDirectoryIn_factorizedLooseEvtSel = 'harvested/gammaPlusJets_Pt30to35/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
        evtSel_factorizedLoose = evtSelZtoElecTau_factorizedLoose,
        meName_numerator = meNameZtoElecTau_numerator,
        meName_denominator = meNameZtoElecTau_denominator,
        dqmDirectoryOut = 'harvested/gammaPlusJets_Pt30to35_factorized/zElecTauAnalyzer/'
    )

    # configure sequence for applying factorization to "PhotonJets_PtGt35" process
    process.scaleZtoElecTau_gammaPlusJets_PtGt35 = composeFactorizationSequence(
        process = process,
        processName = "gammaPlusJets_PtGt35",
        dqmDirectoryIn_factorizedTightEvtSel = 'harvested/gammaPlusJets_PtGt35/zElecTauAnalyzer_factorizedWithElectronIsolation/',
        evtSel_factorizedTight = evtSelZtoElecTau_factorizedTight,
        dqmDirectoryIn_factorizedLooseEvtSel = 'harvested/gammaPlusJets_PtGt35/zElecTauAnalyzer_factorizedWithoutElectronIsolation/',
        evtSel_factorizedLoose = evtSelZtoElecTau_factorizedLoose,
        meName_numerator = meNameZtoElecTau_numerator,
        meName_denominator = meNameZtoElecTau_denominator,
        dqmDirectoryOut = 'harvested/gammaPlusJets_PtGt35_factorized/zElecTauAnalyzer/'
    )

    # compute gamma+jets background sum using factorized histograms and FilterStatistics objects
    process.addZtoElecTau_gammaPlusJetsSum.gammaPlusJetsSum.dqmDirectories_input = cms.vstring(
        'harvested/gammaPlusJets_Pt15to20_factorized',
        'harvested/gammaPlusJets_Pt20to25_factorized',
        'harvested/gammaPlusJets_Pt25to30_factorized',
        'harvested/gammaPlusJets_Pt30to35_factorized',
        'harvested/gammaPlusJets_PtGt35_factorized'
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
    process.addZtoElecTau = cms.Sequence( process.scaleZtoElecTau_QCD_EMenriched_Pt20to30
					 + process.scaleZtoElecTau_QCD_EMenriched_Pt30to80
					 + process.scaleZtoElecTau_QCD_EMenriched_Pt80to170
					 + process.scaleZtoElecTau_QCD_BCtoE_Pt20to30
					 + process.scaleZtoElecTau_QCD_BCtoE_Pt30to80
					 + process.scaleZtoElecTau_QCD_BCtoE_Pt80to170
					 + process.scaleZtoElecTau_gammaPlusJets_Pt15to20
					 + process.scaleZtoElecTau_gammaPlusJets_Pt20to25
					 + process.scaleZtoElecTau_gammaPlusJets_Pt25to30
					 + process.scaleZtoElecTau_gammaPlusJets_Pt30to35
					 + process.scaleZtoElecTau_gammaPlusJets_PtGt35
                                         + process.addZtoElecTau_qcdSum 
					 + process.addZtoElecTau_gammaPlusJetsSum 
					 + process.addZtoElecTau_smSum
                                          )
	
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

def enableFactorization_makeWtoTauNuPlots(process):

    # define list of event selection criteria on "tight" tau isolation branch of the analysis,
    # **before** applying factorization of tauTaNC+prong+charge efficiencies
    evtSelWtoTauNu_factorizedTight = [
        evtSelPrimaryEventVertex,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,
        evtSelTauEta,
        evtSelTauPt,
        evtSelMetPt,
        evtSelTauLeadTrk,
        evtSelTauLeadTrkPt,
        evtSelTauTaNC,
        evtSelTauProng,
        evtSelTauCharge
        ]

    # define list of event selection criteria on "loose" muon isolation branch of the analysis,
    # **after** applying factorization of muon track + ECAL isolation efficiencies
    evtSelWtoTauNu_factorizedLoose = [
        evtSelTauMuonVeto,
        evtSelTauElectronVeto,
        evtSelTauEcalCrackVeto,
        evtSelCentralJetVeto,
        evtSelExplicitElectronVeto,
        evtSelRecoilEnergyFromCaloTowers
    ]

    # defines names of MonitorElements used as numerator and denominator
    # to compute factorization scale-factor
    meNameWtoTauNu_numerator = "evtSelTauProng/passed_cumulative_numWeighted"
    meNameWtoTauNu_denominator = "evtSelTauTaNC/processed_cumulative_numWeighted"

   # configure sequence for applying factorization to "qcd_W" process (QCD background sample for Pt(hat) > 15 GeV)
    process.scaleWtoTauNu_qcd_W = composeFactorizationSequence(
        process = process,
        processName = "qcd_W",
        dqmDirectoryIn_factorizedTightEvtSel = 'harvested/qcd_W/wTauNuAnalyzer_factorizedWithTauIsolation/',
        evtSel_factorizedTight = evtSelWtoTauNu_factorizedTight,
        dqmDirectoryIn_factorizedLooseEvtSel = 'harvested/qcd_W/wTauNuAnalyzer_factorizedWithoutTauIsolation/',
        evtSel_factorizedLoose = evtSelWtoTauNu_factorizedLoose,
        meName_numerator = meNameWtoTauNu_numerator,
        meName_denominator = meNameWtoTauNu_denominator,
        dqmDirectoryOut = 'harvested/qcd_W_factorized/wTauNuAnalyzer/'
    )

    process.addWtoTauNu_qcd.qcd.dqmDirectories_input = cms.vstring(
        'harvested/qcd_W_factorized'
        )

    process.addWtoTauNu = cms.Sequence( process.scaleWtoTauNu_qcd_W + process.addWtoTauNu_qcd + process.addWtoTauNu_smSum  )
    process.plotWtoTauNu.processes.qcd_W.dqmDirectory = cms.string('harvested/qcd_W_factorized')
