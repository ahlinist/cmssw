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

#--------------------------------------------------------------------------------
#
# define DataBase source for TaNC configurations
#
# NOTE: only needed temporary for CMSSW_2_2_x !!
#
from CondCore.DBCommon.CondDBSetup_cfi import *
TauMVAFromDB = cms.ESSource("PoolDBESSource",
    CondDBSetup,
    timetype = cms.untracked.string('runnumber'),
    toGet = cms.VPSet(
        cms.PSet(
	    #record = cms.string('TauTagMVAComputerRcd'),
            record = cms.string('BTauGenericMVAJetTagComputerRcd'),
	    tag = cms.string('MyTestMVATag')
	)
    ),
    connect = cms.string('sqlite_file:/afs/cern.ch/user/f/friis/scratch0/Example.db'),
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService')
)
es_prefer_TauMVAFromDB = cms.ESPrefer("PoolDBESSource", "TauMVAFromDB")
#--------------------------------------------------------------------------------

producePrePat = cms.Sequence( pfAllChargedHadrons + pfAllNeutralHadrons + pfAllPhotons
                             + electronIdCutBased + recoElectronIsolation
                             + recoMuonIsolation
                             + PFTau
                             + shrinkingConePFTauDiscriminationByTaNC + RunTanc )

