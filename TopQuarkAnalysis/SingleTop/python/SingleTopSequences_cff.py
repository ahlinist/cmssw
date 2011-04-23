import FWCore.ParameterSet.Config as cms

from TopQuarkAnalysis.SingleTop.SelectionCuts_Skim_cff import *

from PhysicsTools.HepMCCandAlgos.flavorHistoryPaths_cfi import *

from PhysicsTools.PatAlgos.patSequences_cff import *

from ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff import *

#from TopQuarkAnalysis.SingleTop.SingleTopNtuplizers_cff import nTupleTopJetsPF
#from TopQuarkAnalysis.SingleTop.SingleTopNtuplizers_cff import nTuplePatMETsPF
#from TopQuarkAnalysis.SingleTop.SingleTopNtuplizers_cff import nTupleElectrons
#from TopQuarkAnalysis.SingleTop.SingleTopNtuplizers_cff import nTupleMuons
#from TopQuarkAnalysis.SingleTop.SingleTopNtuplizers_cff import nTuplesSkim

from TopQuarkAnalysis.SingleTop.SingleTopNtuplizers_cff import nTupleTopJetsPF
from TopQuarkAnalysis.SingleTop.SingleTopNtuplizers_cff import nTuplePatMETsPF
from TopQuarkAnalysis.SingleTop.SingleTopNtuplizers_cff import nTupleElectrons
from TopQuarkAnalysis.SingleTop.SingleTopNtuplizers_cff import nTupleMuons
from TopQuarkAnalysis.SingleTop.SingleTopNtuplizers_cff import nTuplesSkim

from TopQuarkAnalysis.SingleTop.SingleTopNtuplizers_cff import nTupleAllElectrons
from TopQuarkAnalysis.SingleTop.SingleTopNtuplizers_cff import nTupleAllMuons

from TopQuarkAnalysis.SingleTop.SingleTopNtuplizers_cff import nTupleAllJets

from TopQuarkAnalysis.SingleTop.SingleTopNtuplizers_cff import nTupleLooseElectrons
from TopQuarkAnalysis.SingleTop.SingleTopNtuplizers_cff import nTupleLooseMuons


nTuplePatMETsPF.src = cms.InputTag('patMETs')


patElectronIDs = cms.Sequence(simpleEleIdSequence)
electronIDSources = cms.PSet(
    simpleEleId70cIso = cms.InputTag("simpleEleId70cIso"),
    simpleEleId95cIso = cms.InputTag("simpleEleId95cIso"),
    ) 

#cFlavorHistoryProducer.matchedSrc = cms.InputTag("antikt5GenJets")
#bFlavorHistoryProducer.matchedSrc = cms.InputTag("antikt5GenJets")

patElectrons.addElectronID = cms.bool(True)
patElectrons.electronIDSources = electronIDSources

patElectronIDs = cms.Sequence(simpleEleIdSequence)

#makeNewPatElectrons = cms.Sequence(patElectronIDs * patElectronIsolation * patElectrons)

patElectrons.usePV = cms.bool(False)
patMuons.usePV = cms.bool(False)


#In those paths the customized collections are produced

basePath = cms.Sequence(
   preselectedMETs +
   looseMuons +
   PVFilterProducer +
   looseElectrons +
   topJetsPF +
   UnclusteredMETPF +
   tightMuons +
   tightElectrons +
#  SingleTopMCProducer +
   PDFInfo 
   )

basePathData = cms.Sequence(
   preselectedMETs +
   looseMuons +
   PVFilterProducer +
   looseElectrons +
   topJetsPF +
   UnclusteredMETPF +
   tightMuons +
   tightElectrons 
#  SingleTopMCProducer +
   )

#Flavor history tools sequence
flavorHistorySequence = cms.Sequence(
    cFlavorHistoryProducer *
    bFlavorHistoryProducer
    )

#Selection step: require 1 high pt(threshold -10%) muon/electron and max 1 extra loose lepton, no isolation requirements.
preselection = cms.Sequence(
    hltFilter +
    PVFilter +
    countLeptons  
    )

#Selection step: require 1 high pt(threshold -10%) muon/electron and max 1 extra loose lepton, loose id requirements.
#process.preselection(
#    hltFilter +
#    PVFilter +
#    countLeptons  
#    )

#Ntuple production sequences:

#!!!Work in progress!!!#
