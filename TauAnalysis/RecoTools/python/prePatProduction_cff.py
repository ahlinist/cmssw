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
from TauAnalysis.RecoTools.recoMuonIsolation_cfi import *

# compute electron and tau id. quantities specific to EWK-tau analyses
from TauAnalysis.RecoTools.recoElectronIdentification_cfi import *
from TauAnalysis.RecoTools.recoPFTauIdentification_cfi import *

# produce collections of dR = 0.07 and dR = 0.15 fixed
# and dR = 5.0/Et shrinking signal cone taus using latest tags;
# produce tau id. discriminators (including TaNC = "Tau Neural Classifiers")
from RecoTauTag.Configuration.RecoPFTauTag_cff import *
#from RecoTauTag.TauTagTools.TancCVTransform_cfi import *

# produce tau id. efficiencies & fake-rates
from RecoTauTag.TauAnalysisTools.PFTauEfficiencyAssociator_cfi import *

# produce anti-kt jet collections not present in Summer'09 samples yet
# disabled by default since Spring10 production
from RecoJets.JetAssociationProducers.trackExtrapolator_cfi import *
from RecoJets.Configuration.RecoJets_cff import *
from RecoJets.Configuration.RecoPFJets_cff import *
from RecoJets.Configuration.GenJetParticles_cff import *
from RecoJets.Configuration.RecoGenJets_cff import *
from RecoJets.Configuration.RecoJetAssociations_cff import *
from RecoJets.Configuration.JetIDProducers_cff import *
from RecoBTag.Configuration.RecoBTag_cff import *
redoAk5Jets = cms.Sequence(
    trackExtrapolator
   + ak5CaloJets + ak5CaloJetsPUCorr + ak5JetID + ak5JTA
   + ak5PFJets
   #+ btagging
)

# produce MET significance values
from RecoMET.METProducers.CaloMETSignif_cfi import *
metsignificance.METType = cms.string('CaloMET')
metsignificance.calculateSignificance = cms.bool(False)

# produce event-shape variables
from PhysicsTools.CandAlgos.EventShapeVars_cff import *

producePrePat = cms.Sequence(
    pfNoPileUpSequence
   + pfAllChargedHadrons + pfAllNeutralHadrons + pfAllPhotons
   + recoElectronIsolation
   + recoMuonIsolation
   + redoAk5Jets
   + PFTau
   #+ shrinkingConePFTauTancCVTransform
   + associateTauFakeRates
   + ewkTauId
   + genParticlesForJets + ak5GenJets
   + metsignificance
   + produceEventShapeVars
)
