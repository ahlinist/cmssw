import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

# run the input file through the end;
# for a limited number of events, replace -1 with the desired number 
#
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load( "SimGeneral.HepPDTESSource.pythiapdt_cfi" )

process.source = cms.Source( "PoolSource",
                             fileNames = cms.untracked.vstring(
			     "file:TestHZZ4tau_1.root",
                             "file:TestHZZ4tau_2.root",
                             "file:TestHZZ4tau_3.root"
			     )
                           )
	      
process.MCTest = cms.EDAnalyzer( "MCTesterCMS" )

process.p1 = cms.Path( process.MCTest )

