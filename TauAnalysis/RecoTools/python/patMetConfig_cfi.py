import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.recoLayer0.jetMETCorrections_cff import *

from PhysicsTools.PatAlgos.producersLayer1.metProducer_cff import *

from JetMETCorrections.Type1MET.TauMetCorrections_cff import * 

#--------------------------------------------------------------------------------  
# PAT layer 0 MET configuration parameters
#--------------------------------------------------------------------------------

# enable CaloMET type-II corrections for "unclustered" energy
# ( see https://twiki.cern.ch/twiki/bin/view/Main/TypeIMETWithCollisionData2010#Extra_tag_for_Type_II_correc_AN1 ;
#       http://indico.cern.ch/getFile.py/access?contribId=6&sessionId=1&resId=0&materialId=slides&confId=93979 )
metJESCorAK5CaloJetMuons.useTypeII = cms.bool(True)
metJESCorAK5CaloJetMuons.hasMuonsCorr = cms.bool(True)

# apply tau-jet specific corrections to reco::MET per default
# (comment-out the next line in case you prefer to **not** apply the tau-jet specific MET corrections)
tauMetCorr.InputCaloJetsLabel = metJESCorAK5CaloJet.inputUncorJetsLabel
tauMetCorr.jetPTthreshold = metJESCorAK5CaloJet.jetPTthreshold
tauMetCorr.jetEMfracLimit = metJESCorAK5CaloJet.jetEMfracLimit
tauMetCorr.correctorLabel = metJESCorAK5CaloJet.corrector
tauMetCorr.InputMETLabel = cms.string('metJESCorAK5CaloJetMuons')

patMETCorrections._seq = patMETCorrections._seq * MetTauCorrections

#--------------------------------------------------------------------------------  
# PAT layer 1 MET configuration parameters
#--------------------------------------------------------------------------------

# change input for pat::MET production to reco::MET collection
# containing the tau-jet specific corrections
# (comment-out the next line in case you prefer to **not** apply the tau-jet specific MET corrections)
patMETs.metSource  = cms.InputTag("tauMetCorr")
