import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce objects that are needed for TauAnalysis specific PAT layer 1 customizations
#--------------------------------------------------------------------------------

# produce particle flow based muon isolation quantities
from PhysicsTools.PFCandProducer.pfNoPileUp_cff import *
from PhysicsTools.PFCandProducer.ParticleSelectors.pfAllChargedHadrons_cfi import *
from PhysicsTools.PFCandProducer.ParticleSelectors.pfAllNeutralHadrons_cfi import *
from PhysicsTools.PFCandProducer.ParticleSelectors.pfAllPhotons_cfi import *
from TauAnalysis.RecoTools.recoElectronIsolation_cfi import *
from TauAnalysis.RecoTools.recoElectronIdentification_cfi import *
from TauAnalysis.RecoTools.recoMuonIsolation_cfi import *

# produce collections of dR = 0.07 and dR = 0.15 fixed
# and dR = 5.0/Et shrinking signal cone taus using latest tags
from RecoTauTag.Configuration.RecoPFTauTag_cff import *

# compute TaNC ("tau neural classifiers") discriminators
# for shrinking cone PFTau collection
from RecoTauTag.TauTagTools.TauMVADiscriminator_cfi import *

# compute discrimanators based on TaNC output
# for different tau-jet efficiency vs. QCD-jet fake rate "working-points"
from RecoTauTag.TauTagTools.TauNeuralClassifiers_cfi import *
#from RecoTauTag.TauTagTools.TancCVTransform_cfi import *

# produce tau id. efficiencies & fake-rates
#from RecoTauTag.TauTagTools.PFTauEfficiencyAssociator_cfi import *

# produce MET significance values
from RecoMET.METProducers.CaloMETSignif_cfi import *
metsignificance.METType = cms.string('CaloMET')

producePrePat = cms.Sequence(
    pfNoPileUpSequence
   + pfAllChargedHadrons + pfAllNeutralHadrons + pfAllPhotons
   + recoElectronIsolation
   + recoMuonIsolation
   + PFTau
   + shrinkingConePFTauDiscriminationByTaNC + RunTanc
   #+ shrinkingConePFTauTancCVTransform
   #+ shrinkingConeEfficienciesProducerFromFile
   + metsignificance
)
