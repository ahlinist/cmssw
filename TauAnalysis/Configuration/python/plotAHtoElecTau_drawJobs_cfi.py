import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.analyzeAHtoElecTau_cfi import *
from TauAnalysis.DQMTools.tools.drawJobConfigurator import *


#--------------------------------------------------------------------------------
# import draw jobs already comfigured for Z -> e + tau-jet analysis
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.plotZtoElecTau_drawJobs_cfi import \
		drawJobConfigurator_ForElecTau as drawJobConfigurator_AHtoElecTau,\
		plots_ZtoElecTau as plots_AHtoElecTau

#--------------------------------------------------------------------------------
# define jobs specific to A/H -> e + tau-jet
#--------------------------------------------------------------------------------

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelPrimaryEventVertexForElecTau,
    beforeCut = evtSelPrimaryEventVertexQualityForElecTau,
    plot = drawJobConfigEntry(
        meName = 'VertexQuantities/VertexChi2Prob',
        title = "P(#Chi^{2}_{vtx}) (after Common Electron+Tau Vertex)",
        xAxis = 'prob',
        name = "cutFlowControlPlots_vertexChi2Prob_afterPrimaryEventVertexForElecTau"
    )
)

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelPrimaryEventVertexQualityForElecTau,
    beforeCut = evtSelPrimaryEventVertexPositionForElecTau,
    plot = drawJobConfigEntry(
        meName = 'VertexQuantities/VertexZ',
        title = "z_{vtx} (after primary Event Vertex quality Cut)",
        xAxis = 'posZ',
        name = "cutFlowControlPlots_vertexZ_afterPrimaryEventVertexQualityForElecTau"
    )
)

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelPrimaryEventVertexPositionForElecTau,
    beforeCut = evtSelDiTauCandidateForAHtoElecTauMt1MET,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/Mt1MET',
        title = "M_{T}(Electron + MET) (after primary Event Vertex position Cut)",
        xAxis = 'Mt',
        name = "cutFlowControlPlots_mtElectronMET_afterPrimaryEventVertexPositionForElecTau"
    )
)

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelDiTauCandidateForAHtoElecTauMt1MET,
    beforeCut = evtSelDiTauCandidateForAHtoElecTauPzetaDiff,
    plots = [
            drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/PzetaDiff',
        title = "P_{#zeta} - 1.5*P_{#zeta}^{vis} (after transverse Mass Cut)",
        xAxis = 'GeV',
        name = "cutFlowControlPlots_PzetaDiff_afterMt1MET"
            ),
      drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/VisMass',
            title = "M_{vis}(Electron + Tau) (after transverse Mass Cut)",
            xAxis = 'Mass',
            name = "cutFlowControlPlots_visibleMass_afterMt1MET"
       )
        ]
)

drawJobConfigurator_AHtoElecTau.add(
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


#--------------------------------------------------------------------------------
# define cut-flow control plots specific to "non-b-tag" analysis path
#--------------------------------------------------------------------------------

drawJobConfigurator_AHtoElecTau_woBtag = copy.deepcopy(drawJobConfigurator_AHtoElecTau)
drawJobConfigurator_AHtoElecTau_woBtag.setDQMdirectory('#PROCESSDIR#/ahElecTauAnalyzerOS_woBtag/')

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

drawJobConfigurator_AHtoElecTau_woBtag.add(
    afterCut = evtSelBtagVeto,
    beforeCut = evtSelDiTauCandidateForAHtoElecTauZeroCharge,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/DiTauCandidateCharge',
        title = "Charge(Electron + Tau) (after Jet no b-tag Cut)",
        xAxis = 'unlabeled',
        name = "cutFlowControlPlots_diTauCharge_afterJetNoBtag"
    )
)

#--------------------------------------------------------------------------------
# define cut-flow control plots specific to "b-tag" analysis path
#--------------------------------------------------------------------------------

drawJobConfigurator_AHtoElecTau_wBtag = copy.deepcopy(drawJobConfigurator_AHtoElecTau)
drawJobConfigurator_AHtoElecTau_wBtag.setDQMdirectory('#PROCESSDIR#/ahElecTauAnalyzerOS_wBtag/')

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


drawJobConfigurator_AHtoElecTau_wBtag.add(
    afterCut = evtSelBtagCut,
    beforeCut = evtSelDiTauCandidateForAHtoElecTauZeroCharge,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/DiTauCandidateCharge',
        title = "Charge(Electron + Tau) (after Jet b-tag Cut)",
        xAxis = 'unlabeled',
        name = "cutFlowControlPlots_diTauCharge_afterJetBtag"
    )
)


#--------------------------------------------------------------------------------
# define distributions to be plotted
# for events passing all event selection criteria
#--------------------------------------------------------------------------------


finalSamplePlots = [
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
        drawJobConfigEntry(
                meName = 'DiTauCandidateQuantities/PzetaDiff',
                title = "P_{#zeta} - 1.5*P_{#zeta}^{vis} (final Event sample)",
                xAxis = 'GeV',
                name = "finalSamplePlots_PzetaDiff"
                ),
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
                title = "M(Electron + Tau), SVfit method (final Event sample)",
                xAxis = 'Mass',
                name = "finalSamplePlots_mSVmethod"
                ),
        drawJobConfigEntry(
                meName = 'DiTauCandidateQuantities/Ht12MET',
                title = "#Sigma H_{T}(Electron + Tau + MET) (final Event sample)",
                xAxis = 'Mass',
                name = "finalSamplePlots_ht"
                ),
        drawJobConfigEntry(
                meName = 'CaloMEtQuantities/MEtPt',
                title = "CaloMET (final Event sample)",
                xAxis = 'Pt',
                name = "finalSamplePlots_caloMEt"
                ),  
        drawJobConfigEntry(
                meName = 'PFMEtQuantities/MEtPt',
                title = "PFMET (final Event sample)",
                xAxis = 'Pt',
                name = "finalSamplePlots_pfMEt"
                ), 
        drawJobConfigEntry(
                meName = 'JetQuantities/BtagDisc_trackCountingHighEffBJetTags',
                title = "Jet b-Tag Discr. (final Event sample)",
                xAxis = 'unlabeled',
                name = "finalSamplePlots_jetBtagDiscr"
                ),
        drawJobConfigEntry(
                meName = 'JetQuantities/NumJets',
                title = "Num. Jets with b-Tag (final Event sample)",
                xAxis = 'unlabeled',
                name = "finalSamplePlots_numBtagJets"
                )
        ]

drawJobConfigurator_AHtoElecTau_woBtag.add(
    afterCut = evtSelDiTauCandidateForAHtoElecTauZeroCharge,
    plots = finalSamplePlots
)

drawJobConfigurator_AHtoElecTau_wBtag.add(
    afterCut = evtSelDiTauCandidateForAHtoElecTauZeroCharge,
    plots = finalSamplePlots
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
drawJobConfigurator_AHtoElecTau_woBtagSS.setDQMdirectory('#PROCESSDIR#/ahElecTauAnalyzerSS_woBtag/')
drawJobConfigurator_AHtoElecTau_wBtagSS.setDQMdirectory('#PROCESSDIR#/ahElecTauAnalyzerSS_wBtag/')

drawJobConfigurator_AHtoElecTau_woBtagSS.add(
    afterCut = evtSelDiTauCandidateForAHtoElecTauNonZeroCharge,
    plots = finalSamplePlots
)
drawJobConfigurator_AHtoElecTau_wBtagSS.add(
    afterCut = evtSelDiTauCandidateForAHtoElecTauNonZeroCharge,
    plots = finalSamplePlots
)

def useSSdataForQCD(process, samples, channel, jobId, backgroundYields):
    

    # import plot configurations
    plotConfigs = []
    for job in finalSamplePlots:
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

    # get data input samples 
    for sampleName in samples['FLATTENED_SAMPLES_TO_PLOT']:
        if 'data' in sampleName:
            sampleNameFull = sampleName + '_dataForQCD'
            setattr(process.loadAHtoElecTauSamples,sampleNameFull,cms.PSet() )
            sampleSet = getattr(process.loadAHtoElecTauSamples,sampleNameFull)
            sampleSet.inputFileNames = cms.vstring("harvested_%s_%s_%s.root" % (channel, sampleName, jobId))
            sampleSet.dqmDirectory_store = cms.string('/harvested/' + sampleNameFull)
            sampleSet.scaleFactor = cms.double(7665./20384*3.37)

            dqmDirectories_input.append('/harvested/' + sampleNameFull)
   
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
                'WplusJets_madgraph_skim',
                'ZeeSum',
                'ZtautauSum'),
            title = cms.string(plotCfg['title']),
            plots = cms.VPSet()
        )
        for sample in samples['SAMPLES_TO_PLOT']:
            drawJob.plots.append(cms.PSet(
                dqmMonitorElements = cms.vstring('#PROCESSDIR#/ahElecTauAnalyzerOS_woBtag/afterEvtSelDiTauCandidateForAHtoElecTauZeroCharge/' + plotCfg['meName']),
                drawOptionEntry = cms.string('default#.#' + sample),
                process = cms.string(sample)
            ))
        drawJob.plots.append(cms.PSet(
            dqmMonitorElements = cms.vstring('#PROCESSDIR#/ahElecTauAnalyzerSS_woBtag/afterEvtSelDiTauCandidateForAHtoElecTauNonZeroCharge/' + plotCfg['meName']),
            drawOptionEntry = cms.string('default#.#qcdSum'),
            process = cms.string('qcdSum')
        ))
        setattr(process.plotahElecTauAnalyzerOS_woBtag_linear.drawJobs, plotCfg['plotName'], drawJob)

        setattr(process.plotahElecTauAnalyzerOS_woBtag_linear.drawOptionSets.default, "qcdSum", cms.PSet(
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


