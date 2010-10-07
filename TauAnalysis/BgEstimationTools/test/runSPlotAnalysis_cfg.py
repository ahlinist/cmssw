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

meName_muonPt = 'muonPt'
meName_muonAbsEta = 'muonAbsEta'
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
process.prodTemplateHist_Ztautau = cms.EDAnalyzer("TemplateHistProducer",
    fileNames = cms.vstring(
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_6_x/ntuples/ZtoMuTau/7TeV/ntupleZtoMuTau_Ztautau_7TeV_part*'
    ),
                                                  
    treeName = cms.string("ZtoMuTauNtuple"),
                                                  
    treeSelection = cms.string(""),
                                                  
    config = cms.VPSet(
        cms.PSet(
            variables = cms.VPSet(
                cms.PSet(
                    branchName = cms.string('muonPt_0'),
                    numBins = cms.uint32(20),
                    min = cms.double(0.),
                    max = cms.double(100.)
                )
            ),
            meName = cms.string(dqmDirectory_Ztautau_template + "/" + meName_muonPt)
	),
        cms.PSet(
            variables = cms.VPSet(
                cms.PSet(
                    branchName = cms.string('muonAbsEta_0'),
                    numBins = cms.uint32(14),
                    min = cms.double(0.),
                    max = cms.double(2.1)
                )
            ),
            meName = cms.string(dqmDirectory_Ztautau_template + "/" + meName_muonAbsEta)
        )
    ),
                                                  
    branchNamesEventWeight = cms.vstring('eventWeight'),

    norm = cms.double(1.)
)

process.prodTemplateHist_QCD = process.prodTemplateHist_Ztautau.clone()
process.prodTemplateHist_QCD.fileNames = cms.vstring(
    'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_6_x/ntuples/ZtoMuTau/7TeV/ntupleZtoMuTau_PPmuXptGt20_7TeV_part*'
)
process.prodTemplateHist_QCD.config[0].meName = cms.string(dqmDirectory_QCD_template + "/" + meName_muonPt)
process.prodTemplateHist_QCD.config[1].meName = cms.string(dqmDirectory_QCD_template + "/" + meName_muonAbsEta)

process.prodTemplateHist_WplusJets = process.prodTemplateHist_Ztautau.clone()
process.prodTemplateHist_WplusJets.fileNames = cms.vstring(
    'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_6_x/ntuples/ZtoMuTau/7TeV/ntupleZtoMuTau_WplusJets_7TeV_part*'
)
process.prodTemplateHist_WplusJets.config[0].meName = cms.string(dqmDirectory_WplusJets_template + "/" + meName_muonPt)
process.prodTemplateHist_WplusJets.config[1].meName = cms.string(dqmDirectory_WplusJets_template + "/" + meName_muonAbsEta)

process.prodTemplateHist = cms.Sequence(
    process.prodTemplateHist_Ztautau
   * process.prodTemplateHist_QCD
   * process.prodTemplateHist_WplusJets
)    
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
process.prodDistribution_data = process.prodTemplateHist_Ztautau.clone()
process.prodDistribution_data.fileNames = cms.vstring(
    'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_6_x/ntuples/ZtoMuTau/7TeV/ntupleZtoMuTau_Ztautau_7TeV_part*',
    'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_6_x/ntuples/ZtoMuTau/7TeV/ntupleZtoMuTau_PPmuXptGt20_7TeV_part*',
    'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_6_x/ntuples/ZtoMuTau/7TeV/ntupleZtoMuTau_WplusJets_7TeV_part*'
)
process.prodDistribution_data.config[0].meName = cms.string(dqmDirectory_data + "/" + meName_muonPt)
process.prodDistribution_data.config[1].meName = cms.string(dqmDirectory_data + "/" + meName_muonAbsEta)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
process.runSPlotAnalysis = cms.EDAnalyzer("SPlotAnalyzer",
    processes = cms.PSet(
        Ztautau = cms.PSet(
            templates = cms.PSet(
                muonPt = cms.PSet(
                    meName = cms.string(dqmDirectory_Ztautau_template + '/' + meName_muonPt)
                ),
                muonAbsEta = cms.PSet(
                    meName = cms.string(dqmDirectory_Ztautau_template + '/' + meName_muonAbsEta)
                )
            )
        ),
        QCD = cms.PSet(
            templates = cms.PSet(
                muonPt = cms.PSet(
                    meName = cms.string(dqmDirectory_QCD_template + '/' + meName_muonPt)
                ),
                muonAbsEta = cms.PSet(
                    meName = cms.string(dqmDirectory_QCD_template + '/' + meName_muonAbsEta)
                )
            )
        ),
        WplusJets = cms.PSet(
            templates = cms.PSet(
                muonPt = cms.PSet(
                    meName = cms.string(dqmDirectory_WplusJets_template + '/' + meName_muonPt)
                ),
                muonAbsEta = cms.PSet(
                    meName = cms.string(dqmDirectory_WplusJets_template + '/' + meName_muonAbsEta)
                )
            )
        )
    ),

    data = cms.PSet(
        distributions = cms.PSet(
            muonPt = cms.PSet(
                meName = cms.string(dqmDirectory_data + '/' + meName_muonPt)
            ),
            muonAbsEta = cms.PSet(
                meName = cms.string(dqmDirectory_data + '/' + meName_muonAbsEta)
            )
        )
    ),

    sPlot = cms.PSet(
        fileNames = cms.vstring(
            'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_6_x/ntuples/ZtoMuTau/7TeV/ntupleZtoMuTau_Ztautau_7TeV_part*',
            'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_6_x/ntuples/ZtoMuTau/7TeV/ntupleZtoMuTau_PPmuXptGt20_7TeV_part*',
            'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_6_x/ntuples/ZtoMuTau/7TeV/ntupleZtoMuTau_WplusJets_7TeV_part*'
        ),

        treeName = cms.string("ZtoMuTauNtuple"),
            
        branchNames = cms.vstring(
            'diTauVisMass',
            'tauCharge',
            'tauLeadChargedParticlePt',
            'tauNumChargedParticlesSignalCone'
        )
    ),    

    outputFilePath = cms.string('./plots/'),
    #outputFileName = cms.string('plotsSplotAnalysis.ps')
    indOutputFileName = cms.string('plotSPlotAnalysis_#PLOT#.png')
)
#--------------------------------------------------------------------------------

process.p = cms.Path(
    process.prodTemplateHist
   + process.prodDistribution_data
   + process.runSPlotAnalysis
)
