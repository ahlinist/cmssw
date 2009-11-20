import FWCore.ParameterSet.Config as cms
import sys

'''
        PFTauEficiencyAssociator_cfi

        Author: Evan K. Friis, UC Davis 

        The PFTauEfficiencyAssociator produces ValueMap<pat::LookupTableRecord>s
        associating an expected efficiency (or fake rate, from QCD) for a reco::PFTau
        given its kinematics.  The default configuration parameterizes the eff/fake rate
        by pt, eta and jet width and stores the information in a TH3.

'''

# Definitions of standard naming and binning conventions
# for the efficiency association/production.
# See TauAnalysis/BgEstimationTools/python/fakeRateConfiguration
# for the histogram production routines

standardEtaAxis = cms.PSet(
      nBins         = cms.uint32(11),
      lowValue      = cms.double(0),
      highValue     = cms.double(+2.2),
      varName       = cms.string("eta"),
      niceName      = cms.string("#eta")
)

standardPtAxis = cms.PSet(
      variableBins = cms.vdouble(0., 10., 20., 30., 40., 50., 65., 80., 120.),
      varName      = cms.string("pt"),
      niceName     = cms.string("p_{T}")
)     

standardJetWidthAxis = cms.PSet(
      nBins        = cms.uint32(10),
      lowValue     = cms.double(0),
      highValue    = cms.double(0.5),
      varName      = cms.string("width"),
      niceName     = cms.string("#DeltaR_{jet}")
)

standardAxesDefinition = cms.PSet(
      xAxis = standardEtaAxis,
      yAxis = standardPtAxis,
      zAxis = standardJetWidthAxis
)

shrinkingConeEfficienciesProducerFromFile = cms.EDProducer("PFTauEfficiencyAssociatorFromTH3",
      PFTauProducer = cms.InputTag("shrinkingConePFTauProducer"),
      efficiencySources = cms.PSet(
         filename = cms.string("/afs/cern.ch/user/f/friis/public/TauPeformance_QCD_BCtoMu.root"),
         # each efficiency source needs to be defined as a separate PSet
         QCDFakeRateTrkIsolation = cms.PSet(
            standardAxesDefinition,
            location = cms.string("makeFakeRateHistograms/TrkIso_efficiency"),
         ),
         QCDFakeRateECALIsolation = cms.PSet(
            standardAxesDefinition,
            location = cms.string("makeFakeRateHistograms/EcalIso_efficiency"),
         ),
         QCDFakeRateTancFrOnePercent = cms.PSet(
            standardAxesDefinition,
            location = cms.string("makeFakeRateHistograms/TaNCOnePercent_efficiency"),
         ),
         QCDFakeRateTancFrHalfPercent = cms.PSet(
            standardAxesDefinition,
            location = cms.string("makeFakeRateHistograms/TaNCHalfPercent_efficiency"),
         ),
         QCDFakeRateTancFrQuarterPercent = cms.PSet(
            standardAxesDefinition,
            location = cms.string("makeFakeRateHistograms/TaNCQuarterPercent_efficiency"),
         ),
         QCDFakeRateTancFrTenthPercent = cms.PSet(
            standardAxesDefinition,
            location = cms.string("makeFakeRateHistograms/TaNCTenthPercent_efficiency"),
         ),
      )
)

shrinkingConePPMuXFakeRateProducer = cms.EDProducer("PFTauEfficiencyAssociatorFromTH3",
      PFTauProducer = cms.InputTag("shrinkingConePFTauProducer"),
      efficiencySources = cms.PSet(
         filename = cms.string("/afs/cern.ch/user/f/friis/public/tauFakeRateHistograms/ppMuX_histograms.root"),
         QCDFakeRateTrkIsolation = cms.PSet(
            standardAxesDefinition,
            location = cms.string("makeFakeRateHistograms/TrkIso_efficiency"),
         ),



def build_pat_efficiency_loader(producer_module, namespace=None, append_to=None):
   '''  Convert PFTauAssociator PSet config to a pat::Object.setEfficiency format 

   Builds a PSet appropriate for loading efficiencies into a pat::Object from a
   PFTauAssociator module configuration. The user can pass the optional
   parameter append_to to add efficiency sources to an existing PSet. Note that
   the input format for PAT efficiencies is 

   cms.PSet(
        effName1 = cms.InputTag("effName1Producer"),
        effName2 = cms.InputTag("effName2Producer")
   )

   '''
   output = append_to
   if output is None:
      output = cms.PSet()
   efficiency_sources_raw = producer_module.efficiencySources.parameters_()
   for source_name, source in efficiency_sources_raw.iteritems():
      if isinstance(source, cms.PSet):
         # Add to the pat configuration.
         moduleName = None
         if namespace is not None:
            for name, ref in namespace.items():
               if ref is producer_module : moduleName = name
         else:
            for pyModule in sys.modules.values():
               if pyModule is not None:                  
                  for name, ref in pyModule.__dict__.items():
                     if ref is producer_module : moduleName = name
         if moduleName is None:
            raise ValueError("Failed to determine moduleName !!")          
         setattr(output, source_name, cms.InputTag(moduleName, source_name))
   return output
   
if __name__ == '__main__':
   testProcess = cms.Process("FAKE")
   testProcess.fakeTest = shrinkingConeEfficienciesProducerFromFile
   print build_pat_efficiency_loader(testProcess.fakeTest)

