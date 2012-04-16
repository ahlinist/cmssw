import FWCore.ParameterSet.Config as cms

process = cms.Process("printRunLumiSectionEventNumbers")

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 5000
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START42_V13::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/skims/TauFakeRate_WJets_RunB_fromArun/selEvents_Data_2011RunB_Wmunu_AOD.root'
    )
)

process.printRunLumiSectionEventNumbers = cms.EDAnalyzer("PrintRunLumiSectionEventNumber",
    output = cms.string('printRunLumiSectionEventNumbers.txt'),
    separator = cms.string('\t')
)

process.o = cms.EndPath(process.printRunLumiSectionEventNumbers)

