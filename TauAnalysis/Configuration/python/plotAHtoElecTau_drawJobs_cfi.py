import FWCore.ParameterSet.Config as cms
import copy
import operator

from TauAnalysis.Configuration.analyzeAHtoElecTau_cfi import *
from TauAnalysis.Configuration.eventSelectionForAHtoElecTau_cfi import *
from TauAnalysis.DQMTools.tools.drawJobConfigurator import *


#--------------------------------------------------------------------------------
# import draw jobs already comfigured for Z -> e + tau-jet analysis
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.plotZtoElecTau_drawJobs_cfi import \
		drawJobConfigurator_ForElecTau as drawJobConfigurator_AHtoElecTau,\
		plots_ZtoElecTau as plots_AHtoElecTau



#--------------------------------------------------------------------------------
# define cut-flow control plots specific to "non-b-tag" analysis path
#--------------------------------------------------------------------------------

drawJobConfigurator_AHtoElecTau_woBtag = drawJobConfigurator(
    template = plots_AHtoElecTau,
    dqmDirectory = '#PROCESSDIR#/ahElecTauAnalyzerOS_woBtag/'
) 
drawJobConfigurator_AHtoElecTau_woBtag.add(
    afterCut = evtSelDiTauCandidateForAHtoElecTauAntiOverlapVeto,
    beforeCut = evtSelDiTauCandidateForAHtoElecTauPzetaDiff,
    plots = [
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/PzetaDiff',
            title = "P_{#zeta} - 1.5*P_{#zeta}^{vis} (after di-tau #DeltaR cut)",
            xAxis = 'GeV',
            name = "cutFlowControlPlots_PzetaDiff_afterDiTauDeltaR"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/VisMass',
            title = "M_{vis}(Electron + Tau) (after di-tau #DeltaR cut)",
            xAxis = 'Mass',
            name = "cutFlowControlPlots_visibleMass_afterDiTauDeltaR"
        )
    ]
)
drawJobConfigurator_AHtoElecTau_woBtag.add(
    afterCut = evtSelDiTauCandidateForAHtoElecTauPzetaDiff,
    beforeCut = evtSelDiElecPairZeeHypothesisVetoByLooseIsolation,
    plots = [
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/VisMass',
            title = "M_{vis}(Electron + Tau) (after P_{#zeta} Cut)",
            xAxis = 'Mass',
            name = "cutFlowControlPlots_visibleMass_afterPzetaDiff"
        ),
        drawJobConfigEntry(
            meName = 'DiElecZeeHypothesisByLooseIsolationQuantities/DiTauCandidateCharge',
            title = "Charge(iso. Elec. + iso. Elec.) (after P_{#zeta} Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_diElectronCharge_afterPzetaDiff"
        )
    ]
)
drawJobConfigurator_AHtoElecTau_woBtag.add(
    afterCut = evtSelDiElecPairZeeHypothesisVetoByLooseIsolation,
    beforeCut = evtSelJetEtCut,
    plot = drawJobConfigEntry(
    meName = 'JetQuantities/Jet#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Jet (after Z #rightarrow e^{+} e^{-} hypothesis Veto)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_jet_afterZeeHypothesisVeto"
    )
)

drawJobConfigurator_AHtoElecTau_woBtag.add(
    afterCut = evtSelJetEtCut,
    beforeCut = evtSelBtagVeto,
    plots = [
        drawJobConfigEntry(
            meName = 'JetQuantities/BtagDisc_trackCountingHighEffBJetTags',
            title = "TCHE B-tag discriminant (after jet E_{T} > 30 GeV cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_bTagDiscr_afterJetEtCut"
        ),
        drawJobConfigEntry(
            meName = 'JetQuantities/NumBtags_trackCountingHighEffBJetTags',
            title = "Num. Jets with b-Tag (after jet E_{T} > 30 GeV cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_numBtagJets_afterJetEtCut"
        )
    ]
)

#drawJobConfigurator_AHtoElecTau_woBtag.add(
#    afterCut = evtSelBtagVeto,
#    beforeCut = evtSelDiTauCandidateForAHtoElecTauZeroCharge,
#    plot = drawJobConfigEntry(
#        meName = 'DiTauCandidateQuantities/DiTauCandidateCharge',
#        title = "Charge(Electron + Tau) (after Jet no b-tag Cut)",
#        xAxis = 'unlabeled',
#        name = "cutFlowControlPlots_diTauCharge_afterJetNoBtag"
#    )
#)

#--------------------------------------------------------------------------------
# define cut-flow control plots specific to "b-tag" analysis path
#--------------------------------------------------------------------------------
drawJobConfigurator_AHtoElecTau_wBtag = drawJobConfigurator(
    template = plots_AHtoElecTau,
    dqmDirectory = '#PROCESSDIR#/ahElecTauAnalyzerOS_wBtag/'
) 

drawJobConfigurator_AHtoElecTau_wBtag.add(
    afterCut = evtSelDiTauCandidateForAHtoElecTauAntiOverlapVeto,
    beforeCut = evtSelDiTauCandidateForAHtoElecTauPzetaDiff,
    plots = [
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/PzetaDiff',
            title = "P_{#zeta} - 1.5*P_{#zeta}^{vis} (after di-tau #DeltaR cut)",
            xAxis = 'GeV',
            name = "cutFlowControlPlots_PzetaDiff_afterDiTauDeltaR"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/VisMass',
            title = "M_{vis}(Electron + Tau) (after di-tau #DeltaR cut)",
            xAxis = 'Mass',
            name = "cutFlowControlPlots_visibleMass_afterDiTauDeltaR"
        )
    ]
)
drawJobConfigurator_AHtoElecTau_wBtag.add(
    afterCut = evtSelDiTauCandidateForAHtoElecTauPzetaDiff,
    beforeCut = evtSelDiElecPairZeeHypothesisVetoByLooseIsolation,
    plots = [
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/VisMass',
            title = "M_{vis}(Electron + Tau) (after P_{#zeta} Cut)",
            xAxis = 'Mass',
            name = "cutFlowControlPlots_visibleMass_afterPzetaDiff"
        ),
        drawJobConfigEntry(
            meName = 'DiElecZeeHypothesisByLooseIsolationQuantities/DiTauCandidateCharge',
            title = "Charge(iso. Elec. + iso. Elec.) (after P_{#zeta} Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_diElectronCharge_afterPzetaDiff"
        )
    ]
)
drawJobConfigurator_AHtoElecTau_wBtag.add(
    afterCut = evtSelDiElecPairZeeHypothesisVetoByLooseIsolation,
    beforeCut = evtSelJetEtCut,
    plot = drawJobConfigEntry(
    meName = 'JetQuantities/Jet#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Jet (after Z #rightarrow e^{+} e^{-} hypothesis Veto)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_jet_afterZeeHypothesisVeto"
    )
)

drawJobConfigurator_AHtoElecTau_wBtag.add(
    afterCut = evtSelJetEtCut,
    beforeCut = evtSelBtagCut,
    plots = [
        drawJobConfigEntry(
            meName = 'JetQuantities/BtagDisc_trackCountingHighEffBJetTags',
            title = "TCHE B-tag discriminant (after jet E_{T} > 30 GeV cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_bTagDiscr_afterJetEtCut"
        ),
        drawJobConfigEntry(
            meName = 'JetQuantities/NumBtags_trackCountingHighEffBJetTags',
            title = "Num. Jets with b-Tag (after jet E_{T} > 30 GeV cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_numBtagJets_afterJetEtCut"
        )
    ]
)


#drawJobConfigurator_AHtoElecTau_wBtag.add(
#    afterCut = evtSelBtagCut,
#    beforeCut = evtSelDiTauCandidateForAHtoElecTauZeroCharge,
#    plot = drawJobConfigEntry(
#        meName = 'DiTauCandidateQuantities/DiTauCandidateCharge',
#        title = "Charge(Electron + Tau) (after Jet b-tag Cut)",
#        xAxis = 'unlabeled',
#        name = "cutFlowControlPlots_diTauCharge_afterJetBtag"
#    )
#)

#--------------------------------------------------------------------------------
# define cut-flow control plots specific to "VBF" analysis path
#--------------------------------------------------------------------------------

drawJobConfigurator_AHtoElecTau_VBF = copy.deepcopy(drawJobConfigurator_AHtoElecTau)
drawJobConfigurator_AHtoElecTau_VBF.setDQMdirectory('#PROCESSDIR#/ahElecTauAnalyzerOS_VBF/')

drawJobConfigurator_AHtoElecTau_VBF.add(
    afterCut = evtSelDiTauCandidateForAHtoElecTauAntiOverlapVeto,
    beforeCut = evtSelDiTauCandidateForElecTauMt1MET,
    plots = [
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/Mt1MET',
            title = "M_{T}(Electron + MET) (after di-tau #DeltaR Cut)",
            xAxis = 'Mt',
            name = "cutFlowControlPlots_mtElectronMET_afterDiTauDeltaR"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/VisMass',
            title = "M_{vis}(Electron + Tau) (after di-tau #DeltaR Cut)",
            xAxis = 'Mass',
            name = "cutFlowControlPlots_visibleMass_afterDiTauDeltaR"
        )
    ]
)

drawJobConfigurator_AHtoElecTau_VBF.add(
    afterCut = evtSelDiTauCandidateForElecTauMt1MET,
    beforeCut = evtSelDiElecPairZeeHypothesisVetoByLooseIsolation,
    plots = [
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/VisMass',
            title = "M_{vis}(Electron + Tau) (after transverse mass Cut)",
            xAxis = 'Mass',
            name = "cutFlowControlPlots_visibleMass_afterMt1MET"
        ),
        drawJobConfigEntry(
            meName = 'DiElecZeeHypothesisByLooseIsolationQuantities/DiTauCandidateCharge',
            title = "Charge(iso. Elec. + iso. Elec.) (after transverse Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_diElectronCharge_afterMt1MET"
        )
    ]
)
#drawJobConfigurator_AHtoElecTau_VBF.add(
#    afterCut = evtSelXXX,
#    beforeCut = evtSelDiTauCandidateForElecTauZeroCharge,
#    plot = drawJobConfigEntry(
#        meName = 'DiTauCandidateQuantities/DiTauCandidateCharge',
#        title = "Charge(Electron + Tau) (after VBF ??? Cut)",
#        xAxis = 'unlabeled',
#        name = "cutFlowControlPlots_diTauCharge_afterVBF"
#    )
#)

#--------------------------------------------------------------------------------
# define distributions to be plotted
# for events passing all event selection criteria
#--------------------------------------------------------------------------------


finalSamplePlotsBase = [
        drawJobConfigEntry(
            meName = 'ElectronQuantities/Electron#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Electron (final Event sample)",
            xAxis = '#PAR#',
            name = "finalSamplePlots_electron"
            ),
        drawJobConfigEntry(
            meName = 'ElectronQuantities/ElectronMatchingGenParticlePdgId',
            title = "PdgId of gen. Particle matching Electron (final Event sample)",
            xAxis = 'PdgId',
            name = "finalSamplePlots_pdgIdGenParticleMatchingElectron"
            ),
        drawJobConfigEntry(
            meName = 'ElectronQuantities/ElectronMatchingFinalStateGenParticlePdgId',
            title = "PdgId of final-state gen. Particle matching Electron (final Event sample)",
            xAxis = 'PdgId',
            name = "finalSamplePlots_pdgIdFinalStateGenParticleMatchingElectron"
            ),
        drawJobConfigEntry(
            meName = 'ElectronQuantities/ElectronPFCombIsoPtRelBarrel',
            title = "Electron comb. rel. PF iso., Barrel (final Event sample)",
            xAxis = 'Pt',
            name = "finalSamplePlots_electronCombPFIsoRelBarrel"
            ),
        drawJobConfigEntry(
            meName = 'ElectronQuantities/ElectronPFCombIsoPtRelEndcap',
            title = "Electron comb. rel. PF iso., Endcap (final Event sample)",
            xAxis = 'Pt',
            name = "finalSamplePlots_electronCombPFIsoRelEndcap"
            ),
        #drawJobConfigEntry( 
        #    meName = 'ElectronQuantities/ElectronConversionRadius', 
        #    title = "Vertex radius of best conv. pair (final event sample)", 
        #    xAxis = 'posX', 
        #    name = "finalSamplePlots_electronConvRadius" 
        #    ), 
        #drawJobConfigEntry( 
        #    meName = 'ElectronQuantities/ElectronConversionDoca', 
        #    title = "DOCA of best conv. pair (final event sample)", 
        #    xAxis = 'posX', 
        #    name = "finalSamplePlots_electronConvDOCA" 
        #    ), 
        #drawJobConfigEntry( 
        #    meName = 'ElectronQuantities/ElectronConversionDeltaCotTheta', 
        #    title = "#DeltaCot(#theta) of best conv. pair (final event sample)", 
        #    xAxis = 'unlabeled', 
        #    name = "finalSamplePlots_electronConvDeltaCotTheta" 
        #    ), 
        #drawJobConfigEntry( 
        #    meName = 'ElectronQuantities/ElectronMissingExpInnerHits', 
        #    title = "Electron missing inner hits (final event sample)", 
        #    xAxis = 'unlabeled', 
        #    name = "finalSamplePlots_electronMissingInnerHits" 
        #    ), 
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (final Event sample)",
            xAxis = '#PAR#',
            name = "finalSamplePlots_tau"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauMatchingGenParticlePdgId',
            title = "PdgId of gen. Particle matching Tau (final Event sample)",
            xAxis = 'PdgId',
            name = "finalSamplePlots_pdgIdGenParticleMatchingTau"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauMatchingFinalStateGenParticlePdgId',
            title = "PdgId of final-state gen. Particle matching Tau (final Event sample)",
            xAxis = 'PdgId',
            name = "finalSamplePlots_pdgIdFinalStateGenParticleMatchingTau"
            ),
        drawJobConfigEntry(
                meName = 'TauQuantities/TauLeadTrkPt',
                title = "Tau lead. Track Pt (final Event sample)",
                xAxis = 'Pt',
                name = "finalSamplePlots_tauLeadTrkPt"
                ),
        drawJobConfigEntry(
                meName = 'TauQuantities/TauNumTracksSignalCone',
                title = "Tau Tracks in Signal Cone (final Event sample)",
                xAxis = 'unlabeled',
                name = "finalSamplePlots_tauNumTracksSignalCone"
                ),
        drawJobConfigEntry(
                meName = 'TauQuantities/TauJetRadius',
                title = "Tau Jet Radius (final Event sample)",
                xAxis = 'unlabeled',
                name = "finalSamplePlots_tauJetRadius"
                ),
        drawJobConfigEntry(
                meName = 'DiTauCandidateQuantities/DPhi12',
                title = "#Delta#phi(Electron-Tau) (final Event sample)",
                xAxis = 'dPhi',
                name = "finalSamplePlots_dPhiElectronTau"
                ),
        #drawJobConfigEntry(
        #        meName = 'DiTauCandidateQuantities/PzetaDiff',
        #        title = "P_{#zeta} - 1.5*P_{#zeta}^{vis} (final Event sample)",
        #        xAxis = 'GeV',
        #        name = "finalSamplePlots_PzetaDiff"
        #        ),
        drawJobConfigEntry(
                meName = 'DiTauCandidateQuantities/Mt1MET',
                title = "M_{T}(Electron + MET) (final Event sample)",
                xAxis = 'Mt',
                name = "finalSamplePlots_mtElectronMET"
                ),
        drawJobConfigEntry(
                meName = 'DiTauCandidateQuantities/Mt2MET',
                title = "M_{T}(Tau + MET) (final Event sample)",
                xAxis = 'Mt',
                name = "finalSamplePlots_mtTauMET"
                ),
        drawJobConfigEntry(
                meName = 'DiTauCandidateQuantities/Mt12MET',
                title = "M_{T}(Electron + Tau + MET) (final Event sample)",
                xAxis = 'Mt',
                name = "finalSamplePlots_mtElectronTauMET"
                ),
        drawJobConfigEntry(
                meName = 'DiTauCandidateQuantities/VisMass',
                title = "M_{vis}(Electron + Tau) (final Event sample)",
                xAxis = 'Mass',
                name = "finalSamplePlots_mVisible"
                ),
        drawJobConfigEntry(
                meName = 'DiTauCandidateQuantities/CDFmethodMass',
                title = "M(Electron + Tau), CDF method (final Event sample)",
                xAxis = 'Mass',
                name = "finalSamplePlots_mCDFmethod"
                ),
        drawJobConfigEntry(
                meName = 'DiTauCandidateQuantities/CollinearApproxMass',
                title = "M(Electron + Tau), collinear Approx. (final Event sample)",
                xAxis = 'Mass',
                name = "finalSamplePlots_mCollApprox"
                ),
        drawJobConfigEntry(
                meName = 'DiTauCandidateNSVfitQuantities/psKine_MEt_logM_fit/Mass',
                title = "M(Electron + Tau), SVfit method, fit. (final Event sample)",
                xAxis = 'Mass',
                name = "finalSamplePlots_mSVmethod"
                ),
        #drawJobConfigEntry(
        #        meName = 'DiTauCandidateNSVfitQuantities/psKine_MEt_logM_int/Mass',
        #        title = "M(Electron + Tau), SVfit method, int. (final Event sample)",
        #        xAxis = 'Mass',
        #        name = "finalSamplePlots_mSVmethod"
        #        ),
        drawJobConfigEntry(
                meName = 'DiTauCandidateQuantities/Ht12MET',
                title = "#Sigma H_{T}(Electron + Tau + MET) (final Event sample)",
                xAxis = 'Mass',
                name = "finalSamplePlots_ht"
                )
]

finalSamplePlots_OS = copy.deepcopy(finalSamplePlotsBase)
finalSamplePlots_OS.extend([ 
        drawJobConfigEntry(
                meName = 'PFMEtQuantities/MEtPt',
                title = "PFMET (final Event sample)",
                xAxis = 'Pt',
                name = "finalSamplePlots_pfMEt"
        )])
        #drawJobConfigEntry(
        #        meName = 'JetQuantities/BtagDisc_trackCountingHighEffBJetTags',
        #        title = "Jet b-Tag Discr. (final Event sample)",
        #        xAxis = 'unlabeled',
        #        name = "finalSamplePlots_jetBtagDiscr"
        #        ),
        #drawJobConfigEntry(
        #        meName = 'JetQuantities/NumJets',
        #        title = "Num. Jets with b-Tag (final Event sample)",
        #        xAxis = 'unlabeled',
        #        name = "finalSamplePlots_numBtagJets"
        #        )

drawJobConfigurator_AHtoElecTau_woBtag.add(
    afterCut = evtSelDiTauCandidateForAHtoElecTauZeroCharge,
    plots = finalSamplePlotsBase
)

drawJobConfigurator_AHtoElecTau_wBtag.add(
    afterCut = evtSelDiTauCandidateForAHtoElecTauZeroCharge,
    plots = finalSamplePlotsBase
)

drawJobConfigurator_AHtoElecTau_VBF.add(
    afterCut = evtSelDiTauCandidateForElecTauZeroCharge,
    plots = finalSamplePlotsBase
)


# Build draw job configurations for the same sign final event plots
drawJobConfigurator_AHtoElecTau_woBtagSS = drawJobConfigurator(
    template = plots_AHtoElecTau,
    dqmDirectory = ''
)
drawJobConfigurator_AHtoElecTau_wBtagSS = drawJobConfigurator(
    template = plots_AHtoElecTau,
    dqmDirectory = ''
)
drawJobConfigurator_AHtoElecTau_VBF_SS = drawJobConfigurator(
    template = plots_AHtoElecTau,
    dqmDirectory = ''
)
drawJobConfigurator_AHtoElecTau_woBtagSS.setDQMdirectory('#PROCESSDIR#/ahElecTauAnalyzerSS_woBtag/')
drawJobConfigurator_AHtoElecTau_wBtagSS.setDQMdirectory('#PROCESSDIR#/ahElecTauAnalyzerSS_wBtag/')
drawJobConfigurator_AHtoElecTau_VBF_SS.setDQMdirectory('#PROCESSDIR#/ahElecTauAnalyzerSS_VBF/')

drawJobConfigurator_AHtoElecTau_woBtagSS.add(
    afterCut = evtSelDiTauCandidateForAHtoElecTauNonZeroCharge,
    plots = finalSamplePlotsBase
)
drawJobConfigurator_AHtoElecTau_wBtagSS.add(
    afterCut = evtSelDiTauCandidateForAHtoElecTauNonZeroCharge,
    plots = finalSamplePlotsBase
)
drawJobConfigurator_AHtoElecTau_VBF_SS.add(
    afterCut = evtSelDiTauCandidateForElecTauNonZeroCharge,
    plots = finalSamplePlotsBase
)

def useSSdataForQCD(process, samples, channel, jobId, bgYieldCorrections):
    

    # import plot configurations
    plotConfigs = []
    for job in finalSamplePlotsBase:
        if getattr(job,"PAR",None) is not None:
            for var in job.PAR:
                plotConfigs.append( { 'plotName' : job.name + var ,
                    'meName' : job.meName[:len(job.meName)-5] + var,
                    'xAxis' : var,
                    'title' : job.title} )
        else:
            plotConfigs.append( { 'plotName' : job.name ,
                'meName' : job.meName,
                'xAxis' : job.xAxis,
                'title' : job.title} )

    dqmDirectories_input = cms.vstring()

    for sampleName in samples['FLATTENED_SAMPLES_TO_PLOT']:
        # get data input samples 
        if 'data' in sampleName:
            sampleNameFull = sampleName + '_dataForQCD'
            setattr(process.loadAHtoElecTauSamples,sampleNameFull,cms.PSet() )
            sampleSet = getattr(process.loadAHtoElecTauSamples,sampleNameFull)
            sampleSet.inputFileNames = cms.vstring("harvested_%s_%s_%s.root" % (channel, sampleName, jobId))
            sampleSet.dqmDirectory_store = cms.string('/harvested/' + sampleNameFull)
            sampleSet.scaleFactor = cms.double(bgYieldCorrections['qcdSum'])

            dqmDirectories_input.append('/harvested/' + sampleNameFull)
        # modify scale factors for other background samples
        for bgSampleName in bgYieldCorrections.keys():
            if sampleName is bgSampleName:
                sampleSet = getattr(process.loadAHtoElecTauSamples,sampleName)
                setattr(sampleSet,"scaleFactor",cms.double(1.0))
                if sampleSet.autoscale is not None:
                    sampleSet.autoscale = cms.bool(False)
                    sampleSet.scaleFactor = sampleSet.xSection.value() * sampleSet.targetIntLumi.value() / sampleSet.totalExpectedEventsBeforeSkim.value()
                print 'Luminosity normalization of %s is %f' % (sampleName, sampleSet.scaleFactor.value())
                setattr(sampleSet, "scaleFactor", cms.double(bgYieldCorrections[bgSampleName]*sampleSet.scaleFactor.value()) )
                print ' --> scaling by factor of %f' % bgYieldCorrections[bgSampleName]
   
    process.mergeSamplesAHtoElecTau.merge_qcdSum = cms.PSet(
        dqmDirectory_output = cms.string('/harvested/dataQCD'),
        dqmDirectories_input = dqmDirectories_input
    )
    
    process.plotahElecTauAnalyzerOS_woBtag_linear.processes.qcdSum = cms.PSet(
        dqmDirectory = cms.string('/harvested/dataQCD'),
        type = cms.string('smMC'),
        legendEntry = cms.string('QCD')
    )
    
    #  create new draw job for each element in plot configuration list
    for plotCfg in plotConfigs:
        drawJob = cms.PSet( 
            drawOptionSet = cms.string('default'),
            yAxis = cms.string('numEntries_linear'),
            labels = cms.vstring('mcNormScale'),
            legend = cms.string('regular'),
            xAxis = cms.string(plotCfg['xAxis']),
            stack = cms.vstring('qcdSum',
                'TTplusJets_madgraph_skim',
                #'EWsum',
                'WplusJets_madgraph_skim',
                'ZeeSum',
                'ZtautauSum'),
            title = cms.string(plotCfg['title']),
            plots = cms.VPSet()
        )
        drawJob.plots.append(cms.PSet(
            dqmMonitorElements = cms.vstring('#PROCESSDIR#/ahElecTauAnalyzerSS_woBtag/afterEvtSelDiTauCandidateForAHtoElecTauNonZeroCharge/' + plotCfg['meName']),
            drawOptionEntry = cms.string('default#.#qcdSum'),
            process = cms.string('qcdSum')
        ))
        for sample in samples['SAMPLES_TO_PLOT']:
            drawJob.plots.append(cms.PSet(
                dqmMonitorElements = cms.vstring('#PROCESSDIR#/ahElecTauAnalyzerOS_woBtag/afterEvtSelDiTauCandidateForAHtoElecTauZeroCharge/' + plotCfg['meName']),
                drawOptionEntry = cms.string('default#.#' + sample),
                process = cms.string(sample)
            ))
        for category in ['OS_woBtag_linear']:
            plotter = getattr(process,"plotahElecTauAnalyzer" + category)
            setattr(plotter.drawJobs, plotCfg['plotName'], drawJob)

            setattr(plotter.drawOptionSets.default, "qcdSum", cms.PSet(
                drawOptionLegend = cms.string('f'),
                fillStyle = cms.int32(1001),
                fillColor = cms.int32(797),
                lineColor = cms.int32(797),
                drawOption = cms.string('hist'),
                lineWidth = cms.int32(1),
                lineStyle = cms.int32(1)
                )
            )

    process.saveAHtoElecTau.outputCommands.append('keep harvested/dataQCD/*')


