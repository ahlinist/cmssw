import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
#
# Compute reweighting parameters needed to correct for shape distortion
# of visible electron + tau-jet mass distribution in background enriched samples
# caused by cuts on transverse mass and CDF Pzeta variable
# in final event selection criteria of Z --> elec + tau-jet channel
# (in case no reweighting is done to correct for the distortion,
#  the normalization factors determined by the "template" method
#  are systematically wrong (in particular for the W + jets background))
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

process = cms.Process('prodKineEventReweightsZtoElecTau')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

process.loadZtoElecTau = cms.EDAnalyzer("DQMFileLoader",
    all = cms.PSet(
        inputFileNames = cms.vstring('rfio:/castor/cern.ch/user/v/veelken/bgEstPlots/ZtoElecTau/plotsZtoElecTau_all.root'),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('')
    )
)

meName = 'DiTauCandidateQuantities/VisMass'
meName_numerator = 'zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/' + meName
meName_denominator = 'zElecTauAnalyzer/afterEvtSelDiTauCandidateForElecTauAcoplanarity12_beforeEvtSelDiTauCandidateForElecTauMt1MET/' + meName

process.prodKineEventReweightsZtoElecTau = cms.EDAnalyzer("DQMHistEffProducer",
    plots = cms.PSet(
        diTauDPhi12Reweight_Ztautau = cms.PSet(
            numerator = cms.string('harvested/Ztautau/' + meName_numerator),
            denominator = cms.string('harvested/Ztautau/' + meName_denominator),
            efficiency = cms.string("bgEstKineEventReweights/Ztautau/diTauMvis")
        ),
        diTauDPhi12Reweight_WplusJets = cms.PSet(
            numerator = cms.string('harvested/WplusJets/' + meName_numerator),
            denominator = cms.string('harvested/WplusJets/' + meName_denominator),
            efficiency = cms.string("bgEstKineEventReweights/WplusJets/diTauMvis")
        ),
        diTauDPhi12Reweight_TTplusJets = cms.PSet(
            numerator = cms.string('harvested/TTplusJets/' + meName_numerator),
            denominator = cms.string('harvested/TTplusJets/' + meName_denominator),
            efficiency = cms.string("bgEstKineEventReweights/TTplusJets/diTauMvis")
        ),
        diTauDPhi12Reweight_QCD = cms.PSet(
            numerator = cms.string('harvested/qcdSum/' + meName_numerator),
            denominator = cms.string('harvested/qcdSum/' + meName_denominator),
            efficiency = cms.string("bgEstKineEventReweights/QCD/diTauMvis")
        )
    )                                                     
)


process.saveKineEventReweightsZtoElecTau = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('bgEstKineEventReweightsZtoElecTau.root'),
    drop = cms.vstring('harvested')
)

process.p = cms.Path(
    process.loadZtoElecTau
   +process.prodKineEventReweightsZtoElecTau
   +process.saveKineEventReweightsZtoElecTau
)

# print-out all python configuration parameter information
#print process.dumpPython()


  
