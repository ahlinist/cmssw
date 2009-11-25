'''
        PFTauEficiencyAssociator_cfi

        Author: Evan K. Friis, UC Davis 

        The PFTauEfficiencyAssociator produces ValueMap<pat::LookupTableRecord>s
        associating an expected efficiency (or fake rate, from QCD) for a reco::PFTau
        given its kinematics.  The default configuration parameterizes the eff/fake rate
        by pt, eta and jet width and stores the information in a TH3.

'''
import FWCore.ParameterSet.Config as cms
from RecoTauTag.TauAnalysisTools.tools.ntupleDefinitions import pftau_expressions
from RecoTauTag.TauAnalysisTools.fakeRate.histogramConfiguration import cuts as disc_configs
from RecoTauTag.TauAnalysisTools.fakeRate.associatorTools import *


protoEffciencyAssociator = cms.EDProducer("PFTauEfficiencyAssociatorFromTH3",
       PFTauProducer = cms.InputTag("shrinkingConePFTauProducer"),
       xAxisFunction = pftau_expressions.jetPt,
       yAxisFunction = pftau_expressions.jetEta,
       zAxisFunction = pftau_expressions.jetWidth,
       efficiencySources = cms.PSet(
           filename = cms.string("/afs/cern.ch/user/f/friis/public/TauPeformance_QCD_BCtoMu.root"),
           # each efficiency source needs to be defined as a separate PSet
       )
)

# Build the list of efficiency sources from the histogram production
# configuration disc_configs
MuEnrichedQCDEffSources = add_eff_sources(prefix="fr",
        disc_configs=disc_configs.keys(), suffix="MuEnrichedQCDsim")

shrinkingConeMuEnrichedQCDAssociator = protoEffciencyAssociator.clone()
shrinkingConeMuEnrichedQCDAssociator.efficiencySources = cms.PSet(
        MuEnrichedQCDEffSources,
        filename=cms.string("/afs/cern.ch/user/f/friis/public/tauFakeRateHistograms/ppMuX_histograms.root")
)
   
if __name__ == '__main__':
   testProcess = cms.Process("FAKE")
   testProcess.fakeTest = shrinkingConeMuEnrichedQCDAssociator 
   print testProcess.fakeTest
   print build_pat_efficiency_loader(testProcess.fakeTest)

