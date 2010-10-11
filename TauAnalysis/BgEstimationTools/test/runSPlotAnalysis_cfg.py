import FWCore.ParameterSet.Config as cms

process = cms.Process("runSPlotAnalysis")

process.load('FWCore/MessageService/MessageLogger_cfi')
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

#--------------------------------------------------------------------------------
dqmDirectory_Ztautau_template = 'template/Ztautau'
dqmDirectory_QCD_template = 'template/QCD'
dqmDirectory_WplusJets_template = 'template/WplusJets'

dqmDirectory_data = 'data'

meName_muonPt_vs_muonAbsEta = 'muonPt_vs_muonAbsEta'

ntuplePath = "/data1/veelken/CMSSW_3_6_x/ntuples"
treeName = "ntupleProducer/ZtoMuTauNtuple"

branchName_muonPt = 'muonPt_0'
branchName_muonAbsEta = 'muonAbsEta_0'

branchName_eventWeight = 'eventWeight'
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
process.prodTemplateHist_Ztautau = cms.EDAnalyzer("TemplateHistProducer",
    fileNames = cms.vstring(
        ntuplePath + '/' + 'ntupleZtoMuTau_Ztautau_7TeV_part*'
    ),
                                                  
    treeName = cms.string(treeName),
                                                  
    treeSelection = cms.string(""),
                                                  
    config = cms.VPSet(
        cms.PSet(
            variables = cms.VPSet(
                cms.PSet(
                    branchName = cms.string(branchName_muonPt),
                    numBins = cms.uint32(8),
                    binEdges = cms.vdouble(15., 20., 25., 30., 40., 50., 60., 80., 100.)
                ),
                cms.PSet(
                    branchName = cms.string(branchName_muonAbsEta),
                    numBins = cms.uint32(7),
                    min = cms.double(0.),
                    max = cms.double(2.1)
                )
            ),
            meName = cms.string(dqmDirectory_Ztautau_template + "/" + meName_muonPt_vs_muonAbsEta)
        )
    ),
                                                  
    branchNamesEventWeight = cms.vstring(branchName_eventWeight),

    norm = cms.double(1.)
)

process.prodTemplateHist_QCD = process.prodTemplateHist_Ztautau.clone()
process.prodTemplateHist_QCD.fileNames = cms.vstring(
    ntuplePath + '/' + 'ntupleZtoMuTau_PPmuXptGt20_7TeV_part*'
)
process.prodTemplateHist_QCD.config[0].meName = cms.string(dqmDirectory_QCD_template + "/" + meName_muonPt_vs_muonAbsEta)

process.prodTemplateHist_WplusJets = process.prodTemplateHist_Ztautau.clone()
process.prodTemplateHist_WplusJets.fileNames = cms.vstring(
    ntuplePath + '/' + 'ntupleZtoMuTau_WplusJets_7TeV_part*'
)
process.prodTemplateHist_WplusJets.config[0].meName = cms.string(dqmDirectory_WplusJets_template + "/" + meName_muonPt_vs_muonAbsEta)

process.prodTemplateHist = cms.Sequence(
    process.prodTemplateHist_Ztautau
   * process.prodTemplateHist_QCD
   * process.prodTemplateHist_WplusJets
)    
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
process.prodDistribution_data = process.prodTemplateHist_Ztautau.clone()
process.prodDistribution_data.fileNames = cms.vstring(
    '/data1/veelken/CMSSW_3_6_x/ntuples/ntupleZtoMuTau_Ztautau_7TeV_part*',
    '/data1/veelken/CMSSW_3_6_x/ntuples/ntupleZtoMuTau_PPmuXptGt20_7TeV_part*',
    '/data1/veelken/CMSSW_3_6_x/ntuples/ntupleZtoMuTau_WplusJets_7TeV_part*'
)
process.prodDistribution_data.config[0].meName = cms.string(dqmDirectory_data + "/" + meName_muonPt_vs_muonAbsEta)
process.prodDistribution_data.norm = cms.double(-1.) # do not normalize data
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
process.runSPlotAnalysis = cms.EDAnalyzer("SPlotAnalyzer",
    processes = cms.PSet(
        Ztautau = cms.PSet(
            templates = cms.PSet(
                muonPt_vs_muonAbsEta = cms.PSet(
                    meName = cms.string(dqmDirectory_Ztautau_template + '/' + meName_muonPt_vs_muonAbsEta)
                )
            ),
        
            fileNames = cms.vstring(
                ntuplePath + '/' + 'ntupleZtoMuTau_Ztautau_7TeV_part*'
            ),
        
            treeName = cms.string(treeName),

            branchNameEventWeight = cms.string(branchName_eventWeight)
        ),
        QCD = cms.PSet(
            templates = cms.PSet(
                muonPt_vs_muonAbsEta = cms.PSet(
                    meName = cms.string(dqmDirectory_QCD_template + '/' + meName_muonPt_vs_muonAbsEta)
                )
            ),
        
            fileNames = cms.vstring(
                ntuplePath + '/' + 'ntupleZtoMuTau_PPmuXptGt20_7TeV_part*'
            ),
        
            treeName = cms.string(treeName),

            branchNameEventWeight = cms.string(branchName_eventWeight)
        ),
        WplusJets = cms.PSet(
            templates = cms.PSet(
                muonPt_vs_muonAbsEta = cms.PSet(
                    meName = cms.string(dqmDirectory_WplusJets_template + '/' + meName_muonPt_vs_muonAbsEta)
                )
            ),
        
            fileNames = cms.vstring(
                ntuplePath + '/' + 'ntupleZtoMuTau_WplusJets_7TeV_part*'
            ),
        
            treeName = cms.string(treeName),

            branchNameEventWeight = cms.string(branchName_eventWeight)
        )
    ),

    data = cms.PSet(
        distributions = cms.PSet(
            muonPt_vs_muonAbsEta = cms.PSet(
                meName = cms.string(dqmDirectory_data + '/' + meName_muonPt_vs_muonAbsEta)
            )
        ),
        
        fileNames = cms.vstring(
            ntuplePath + '/' + 'ntupleZtoMuTau_Ztautau_7TeV_part*',
            ntuplePath + '/' + 'ntupleZtoMuTau_PPmuXptGt20_7TeV_part*',
            ntuplePath + '/' + 'ntupleZtoMuTau_WplusJets_7TeV_part*'
        ),
        
        treeName = cms.string(treeName),

        branchNameEventWeight = cms.string(branchName_eventWeight)
    ),

    fitVariables = cms.vstring(
        branchName_muonPt,
        branchName_muonAbsEta
    ),                                    

    sPlots = cms.PSet(
        diTauVisMass = cms.PSet(
            branchName = cms.string('diTauVisMass_0'),
            title = cms.string("muon + tau-jet visible Mass"),
            xMin = cms.double(0.),
            xMax = cms.double(200.),
            selProcesses = cms.vstring('Ztautau')
        ),
        tauCharge = cms.PSet(
            branchName = cms.string('tauCharge_0'),
            title = cms.string("tau-jet charge"),
            xMin = cms.double(-5.5),
            xMax = cms.double(+5.5),
            selProcesses = cms.vstring('Ztautau')
        ),
        tauLeadChargedParticlePt = cms.PSet(
            branchName = cms.string('tauLeadChargedParticlePt_0'),
            title = cms.string("tau-jet lead. charged hadron P_{T}"),
            xMin = cms.double(0.),
            xMax = cms.double(150.),
            selProcesses = cms.vstring('Ztautau')
        ),
        tauNumChargedParticlesSignalCone = cms.PSet(
            branchName = cms.string('tauNumChargedParticlesSignalCone_0'),
            title = cms.string("tau-jet num. charged hadrons in signal cone"),
            xMin = cms.double(-0.5),
            xMax = cms.double(+9.5),
            selProcesses = cms.vstring('Ztautau')
        )
    ),    

    outputFilePath = cms.string('./plots/'),
    #outputFileName = cms.string('plotsSplotAnalysis.ps')
    indOutputFileName = cms.string('plotSPlotAnalysis_#PLOT#.png')
)
#--------------------------------------------------------------------------------

process.saveHistograms = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('runSPlotAnalysis_histograms.root'),
    outputCommands = cms.vstring()
)

process.p = cms.Path(
    process.prodTemplateHist
   + process.prodDistribution_data
   + process.saveHistograms
   + process.runSPlotAnalysis
)
