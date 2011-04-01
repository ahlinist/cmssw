import FWCore.ParameterSet.Config as cms

process = cms.Process("copyToCastor")

import TauAnalysis.Configuration.tools.castor as castor

from TauAnalysis.Skimming.EventContent_cff import *

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 5000
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START38_V14::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010A_Nov4ReReco_chunk_5_871a.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010A_Nov4ReReco_chunk_0_c4da.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010A_Nov4ReReco_chunk_10_aa11.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010A_Nov4ReReco_chunk_11_4d3e.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010A_Nov4ReReco_chunk_1_28d5.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010A_Nov4ReReco_chunk_2_09d0.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010A_Nov4ReReco_chunk_7_a84a.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010A_Nov4ReReco_chunk_3_3cf9.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010A_Nov4ReReco_chunk_6_fc37.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010A_Nov4ReReco_chunk_4_567c.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010B_Nov4ReReco_chunk_0_8834.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010B_Nov4ReReco_chunk_14_b4b8.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010A_Nov4ReReco_chunk_8_7d54.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010B_Nov4ReReco_chunk_10_f872.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010A_Nov4ReReco_chunk_9_56dc.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010B_Nov4ReReco_chunk_13_677a.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010B_Nov4ReReco_chunk_11_1aaa.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010B_Nov4ReReco_chunk_12_facc.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010B_Nov4ReReco_chunk_1_8b16.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010B_Nov4ReReco_chunk_15_ccdf.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010B_Nov4ReReco_chunk_5_9649.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010B_Nov4ReReco_chunk_4_eae2.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010B_Nov4ReReco_chunk_3_c8f5.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010B_Nov4ReReco_chunk_2_c132.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010B_Nov4ReReco_chunk_6_9ecc.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010B_Nov4ReReco_chunk_7_817a.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010B_Nov4ReReco_chunk_9_809d.root',
        'file:/data1/veelken/tmp/skims/skim_data_Mu_Run2010B_Nov4ReReco_chunk_8_509d.root'
    )
)

# Get all the skim files from the castor directory
#sourceFilePath = "/castor/cern.ch/user/f/friis/Run50plainharvest/"
sourceFilePath = "/castor/cern.ch/user/v/veelken/CMSSW_3_8_x/skims/debug/"
source_files = [ 'rfio:%s' % file_info['path'] for file_info in castor.nslsl(sourceFilePath) ]

jobId = "2011Feb03bV2"

source_files_matched = []

for source_file in source_files:
    if source_file.find(jobId) != -1:
	source_files_matched.append(source_file)

#print "source_files_matched", source_files_matched

setattr(process.source, "fileNames", cms.untracked.vstring(source_files_matched))

dummyEventSelection = cms.untracked.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('')
    )
)

process.copyToCastorOutputModule = cms.OutputModule("PoolOutputModule",                                              
    #fileName = cms.untracked.string('/data1/veelken/CMSSW_3_8_x/skims/debug/selEvents_TauIdEffDebugging_2011Feb19_RECO.root')
    fileName = cms.untracked.string(
        #'/castor/cern.ch/user/v/veelken/CMSSW_3_8_x/skims/debug/debugTauIdEffMeasSample_data_all_2011Feb03bV2_RECO.root'
        '/data1/veelken/CMSSW_3_8_x/skims/debug/debugTauIdEffMeasSample_data_all_2011Feb03bV2_RECO.root'
    )
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath(process.copyToCastorOutputModule)

