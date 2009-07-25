import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce objects that are needed for TauAnalysis specific PAT layer 1 customizations
#--------------------------------------------------------------------------------

# produce particle flow based muon isolation quantities
from PhysicsTools.PFCandProducer.pfAllChargedHadrons_cfi import *
from PhysicsTools.PFCandProducer.pfAllNeutralHadrons_cfi import *
from PhysicsTools.PFCandProducer.pfAllPhotons_cfi import *
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

# define DataBase source for TaNC configurations
# (need esprefer statement in order to prevent conflict with Fake BTau conditions)
from RecoTauTag.TauTagTools.TancConditions_cff import *

producePrePat = cms.Sequence( pfAllChargedHadrons + pfAllNeutralHadrons + pfAllPhotons
                             + electronIdCutBased + recoElectronIsolation
                             + recoMuonIsolation
                             + PFTau
                             + shrinkingConePFTauDiscriminationByTaNC + RunTanc + shrinkingConePFTauTancCVTransform )

