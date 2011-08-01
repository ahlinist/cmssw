import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce objects that are needed for TauAnalysis specific PAT layer 1 customizations
#--------------------------------------------------------------------------------

# produce particle flow based muon isolation quantities
from CommonTools.ParticleFlow.pfNoPileUp_cff import *
from CommonTools.ParticleFlow.ParticleSelectors.pfAllChargedHadrons_cfi import *
from CommonTools.ParticleFlow.ParticleSelectors.pfAllNeutralHadrons_cfi import *
from CommonTools.ParticleFlow.ParticleSelectors.pfAllPhotons_cfi import *
from TauAnalysis.RecoTools.recoElectronIsolation_cfi import *
from TauAnalysis.RecoTools.recoMuonIsolation_cfi import *

# compute electron and tau id. quantities specific to EWK-tau analyses
from TauAnalysis.RecoTools.recoElectronIdentification_cfi import *
from TauAnalysis.RecoTools.recoPFTauIdentification_cfi import *

# produce collections of kT PFJets for dR = 0.6 needed for rho (FastJet) pile-up corrections
from RecoJets.Configuration.RecoPFJets_cff import *
kt6PFJets.Rho_EtaMax = cms.double(2.5)
kt6PFJets.doRhoFastjet = True

#--------------------------------------------------------------------------------
#
# configure Jet Energy Corrections
#
from CondCore.DBCommon.CondDBCommon_cfi import *
jec = cms.ESSource("PoolDBESSource",
    DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(0)
    ),
    timetype = cms.string('runnumber'),
    toGet = cms.VPSet(
        cms.PSet(
            record = cms.string('JetCorrectionsRecord'),
            tag    = cms.string('JetCorrectorParametersCollection_Jec11V2_AK5PF'),
            label  = cms.untracked.string('AK5PF')
        ),
        cms.PSet(
            record = cms.string('JetCorrectionsRecord'),
            tag    = cms.string('JetCorrectorParametersCollection_Jec11V2_AK5Calo'),
            label  = cms.untracked.string('AK5Calo')
        )
    ),
    connect = cms.string('sqlite_fip:TauAnalysis/Configuration/data/Jec11V2.db')
)
es_prefer_jec = cms.ESPrefer('PoolDBESSource', 'jec')
#-------------------------------------------------------------------------------------------------------------------------

# produce collections of dR = 0.07 and dR = 0.15 fixed
# and dR = 5.0/Et shrinking signal cone taus using latest tags;
# produce tau id. discriminators (including TaNC = "Tau Neural Classifiers")
from RecoTauTag.Configuration.RecoPFTauTag_cff import *

# produce anti-kt collections of generator level jets
from RecoJets.Configuration.GenJetParticles_cff import *
from RecoJets.Configuration.RecoGenJets_cff import *

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
   + kt6PFJets 
   + PFTau
   # CV: discriminator against calo. muons currently disabled per default;
   #     add manually
   #+ recoTauHPSTancSequence
   #+ hpsTancTausDiscriminationAgainstCaloMuon
   #+ shrinkingConePFTauTancCVTransform
   + ewkTauId
   + genParticlesForJets + ak5GenJets 
   + metsignificance
   + produceEventShapeVars
)
