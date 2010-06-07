import FWCore.ParameterSet.Config as cms

process = cms.Process('prodBgEstTemplatesZtoMuTau')

process.load("TauAnalysis.Configuration.dumpZtoMuTau_cff")
process.load("TauAnalysis.Configuration.plotZtoMuTau_cff")

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

# define directory from which .root files containing the histograms get loaded
process.loadZtoMuTau.inputFilePath = cms.string("rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_6_x/bgEstPlots/ZtoMuTau_bgEstTemplates/7TeV/")

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.addZtoMuTauBgEstTemplates_qcdSum = cms.EDAnalyzer("DQMHistAdder",
    qcdSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            ##'harvested/InclusivePPmuX/',
            'harvested/PPmuXptGt20/'
        ),
        dqmDirectory_output = cms.string('harvested/qcdSum/')
    )                          
)

process.addZtoMuTauBgEstTemplates_smSum = cms.EDAnalyzer("DQMHistAdder",
    smSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'harvested/Ztautau/',
            'harvested/Zmumu/',
            'harvested/WplusJets/',
            'harvested/TTplusJets/',
            'harvested/qcdSum/'
        ),
        dqmDirectory_output = cms.string('harvested/smSum/')
    )
)

process.addZtoMuTauBgEstTemplates = cms.Sequence(process.addZtoMuTauBgEstTemplates_qcdSum + process.addZtoMuTauBgEstTemplates_smSum)

process.saveZtoMuTau.outputFileName = cms.string('bgEstTemplatesZtoMuTau.root')

#--------------------------------------------------------------------------------
# Print-out cut-flow information for background enriched samples
# from which template histograms are obtained
process.dumpZtoMuTau_bgEstZmumuJetMisIdEnriched = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('harvested/Ztautau/BgEstTemplateAnalyzer_ZmumuJetMisIdEnriched/FilterStatistics/'),
        Zmumu = cms.string('harvested/Zmumu/BgEstTemplateAnalyzer_ZmumuJetMisIdEnriched/FilterStatistics/'),
        WplusJets = cms.string('harvested/WplusJets/BgEstTemplateAnalyzer_ZmumuJetMisIdEnriched/FilterStatistics/'),
        QCD = cms.string('harvested/qcdSum/BgEstTemplateAnalyzer_ZmumuJetMisIdEnriched/FilterStatistics/'),
        TTplusJets = cms.string('harvested/TTplusJets/BgEstTemplateAnalyzer_ZmumuJetMisIdEnriched/FilterStatistics/'),
        smSum = cms.string('harvested/smSum/BgEstTemplateAnalyzer_ZmumuJetMisIdEnriched/FilterStatistics/')
    ),
    columnsSummaryTable = cms.vstring("Passed"),
    printSummaryTableOnly = cms.bool(True)                                                    
)

process.dumpZtoMuTau_bgEstZmumuMuonMisIdEnriched = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('harvested/Ztautau/BgEstTemplateAnalyzer_ZmumuMuonMisIdEnriched/FilterStatistics/'),
        Zmumu = cms.string('harvested/Zmumu/BgEstTemplateAnalyzer_ZmumuMuonMisIdEnriched/FilterStatistics/'),
        WplusJets = cms.string('harvested/WplusJets/BgEstTemplateAnalyzer_ZmumuMuonMisIdEnriched/FilterStatistics/'),
        QCD = cms.string('harvested/qcdSum/BgEstTemplateAnalyzer_ZmumuMuonMisIdEnriched/FilterStatistics/'),
        TTplusJets = cms.string('harvested/TTplusJets/BgEstTemplateAnalyzer_ZmumuMuonMisIdEnriched/FilterStatistics/'),
        smSum = cms.string('harvested/smSum/BgEstTemplateAnalyzer_ZmumuMuonMisIdEnriched/FilterStatistics/'),
    ),
    columnsSummaryTable = cms.vstring("Passed"),
    printSummaryTableOnly = cms.bool(True)                                                    
)

process.dumpZtoMuTau_bgEstWplusJetsEnriched = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('harvested/Ztautau/BgEstTemplateAnalyzer_WplusJetsEnriched/FilterStatistics/'),
        Zmumu = cms.string('harvested/Zmumu/BgEstTemplateAnalyzer_WplusJetsEnriched/FilterStatistics/'),
        WplusJets = cms.string('harvested/WplusJets/BgEstTemplateAnalyzer_WplusJetsEnriched/FilterStatistics/'),
        QCD = cms.string('harvested/qcdSum/BgEstTemplateAnalyzer_WplusJetsEnriched/FilterStatistics/'),
        TTplusJets = cms.string('harvested/TTplusJets/BgEstTemplateAnalyzer_WplusJetsEnriched/FilterStatistics/'),
        smSum = cms.string('harvested/smSum/BgEstTemplateAnalyzer_WplusJetsEnriched/FilterStatistics/')
    ),
    columnsSummaryTable = cms.vstring("Passed"),
    printSummaryTableOnly = cms.bool(True)       
)

process.dumpZtoMuTau_bgEstTTplusJetsEnriched = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('harvested/Ztautau/BgEstTemplateAnalyzer_TTplusJetsEnriched/FilterStatistics/'),
        Zmumu = cms.string('harvested/Zmumu/BgEstTemplateAnalyzer_TTplusJetsEnriched/FilterStatistics/'),
        WplusJets = cms.string('harvested/WplusJets/BgEstTemplateAnalyzer_TTplusJetsEnriched/FilterStatistics/'),
        QCD = cms.string('harvested/qcdSum/BgEstTemplateAnalyzer_TTplusJetsEnriched/FilterStatistics/'),
        TTplusJets = cms.string('harvested/TTplusJets/BgEstTemplateAnalyzer_TTplusJetsEnriched/FilterStatistics/'),
        smSum = cms.string('harvested/smSum/BgEstTemplateAnalyzer_TTplusJetsEnriched/FilterStatistics/')
    ),
    columnsSummaryTable = cms.vstring("Passed"),
    printSummaryTableOnly = cms.bool(True)       
)

process.dumpZtoMuTau_bgEstQCDenriched = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('harvested/Ztautau/BgEstTemplateAnalyzer_QCDenriched/FilterStatistics/'),
        Zmumu = cms.string('harvested/Zmumu/BgEstTemplateAnalyzer_QCDenriched/FilterStatistics/'),
        WplusJets = cms.string('harvested/WplusJets/BgEstTemplateAnalyzer_QCDenriched/FilterStatistics/'),
        QCD = cms.string('harvested/qcdSum/BgEstTemplateAnalyzer_QCDenriched/FilterStatistics/'),
        TTplusJets = cms.string('harvested/TTplusJets/BgEstTemplateAnalyzer_QCDenriched/FilterStatistics/'),
        smSum = cms.string('harvested/smSum/BgEstTemplateAnalyzer_QCDenriched/FilterStatistics/')
    ),
    columnsSummaryTable = cms.vstring("Passed"),
    printSummaryTableOnly = cms.bool(True)       
)

process.dumpZtoMuTauBgEstTemplates = cms.Sequence(
    process.dumpZtoMuTau_bgEstZmumuJetMisIdEnriched + process.dumpZtoMuTau_bgEstZmumuMuonMisIdEnriched
   + process.dumpZtoMuTau_bgEstWplusJetsEnriched
   + process.dumpZtoMuTau_bgEstTTplusJetsEnriched
   + process.dumpZtoMuTau_bgEstQCDenriched
)    
#--------------------------------------------------------------------------------

process.p = cms.Path(
    process.loadZtoMuTau
  #+ process.dumpDQMStore
   + process.addZtoMuTauBgEstTemplates
   + process.dumpZtoMuTauBgEstTemplates 
   + process.saveZtoMuTau
)

# print-out all python configuration parameter information
#print process.dumpPython()
