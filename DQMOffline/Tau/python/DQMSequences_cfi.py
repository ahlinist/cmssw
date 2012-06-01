from DQMOffline.Tau.dataTypes.RunDQMOnRealElectronsData_cff import *
from DQMOffline.Tau.dataTypes.RunDQMOnRealData_cff import *
from DQMOffline.Tau.dataTypes.RunDQMOnRealMuonsData_cff import *
from DQMOffline.Tau.dataTypes.RunDQMOnRealTausData_cff import *

dqmInfoTauV = cms.EDAnalyzer(
    "DQMEventInfo",
    subSystemFolder = cms.untracked.string('RecoTauV')
    )


produceDenoms = cms.Sequence(
    produceDenominatorRealData+
    produceDenominatorRealElectronsData+
    produceDenominatorRealMuonsData+
    produceDenominatorRealTausData
    )

seqModifier = ApplyFunctionToSequence( lambda module: setTrigger( module, cms.PSet( hltDBKey = cms.string('TauTriggerForALLQCDDataset'), hltPaths = cms.vstring('HLT_IsoMu24_eta2p1_v*') ) ) )
TauValNumeratorAndDenominatorRealData.visit(seqModifier)

seqModifier = ApplyFunctionToSequence( lambda module: setTrigger( module, cms.PSet( hltDBKey = cms.string('TauTriggerForALLEleDataset'), hltPaths = cms.vstring('HLT_Ele20_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC4_Mass50_v*') ) ) )
TauValNumeratorAndDenominatorRealElectronsData.visit(seqModifier)

seqModifier = ApplyFunctionToSequence( lambda module: setTrigger( module, cms.PSet( hltDBKey = cms.string('TauTriggerForALLMuDataset'), hltPaths = cms.vstring('HLT_IsoMu24_eta2p1_v*') ) ) )
TauValNumeratorAndDenominatorRealMuonsData.visit(seqModifier)

seqModifier = ApplyFunctionToSequence( lambda module: setTrigger( module, cms.PSet( hltDBKey = cms.string('TauTriggerForALLTauDataset'), hltPaths = cms.vstring('HLT_IsoMu15_eta2p1_L1ETM20_v*') ) ) )
TauValNumeratorAndDenominatorRealTausData.visit(seqModifier)

pfTauRunDQMValidation = cms.Sequence(
    TauValNumeratorAndDenominatorRealData+
    TauValNumeratorAndDenominatorRealElectronsData+
    TauValNumeratorAndDenominatorRealMuonsData+
    TauValNumeratorAndDenominatorRealTausData+
    dqmInfoTauV
    )

runTauEff = cms.Sequence(
    efficienciesRealData+
    efficienciesRealElectronsData+
    efficienciesRealMuonsData+
    efficienciesRealTausData+
    normalizePlotsRealMuonsData
    )

#----------------------------------------------------------------------------------------------------------------------------------------
#                                                  Denominators according to dataset
#----------------------------------------------------------------------------------------------------------------------------------------
## produceDenomsMu = cms.Sequence(
##     produceDenominatorRealData+
##     produceDenominatorRealMuonsData
##     )
produceDenomsSingleMu = cms.Sequence(
    produceDenominatorRealData+
    produceDenominatorRealMuonsData
    )
produceDenomsJet = produceDenominatorRealData
produceDenomsMultiJet = produceDenomsJet

produceDenomsDoubleElectron = produceDenominatorRealElectronsData
produceDenomsTauPlusX = produceDenomsDoubleElectron

#----------------------------------------------------------------------------------------------------------------------------------------
#                                                  Main modules according to dataset
#----------------------------------------------------------------------------------------------------------------------------------------
proc.GeneralMuSequence = cms.Sequence( proc.TauValNumeratorAndDenominatorRealData * proc.TauValNumeratorAndDenominatorRealMuonsData )

#Mu Dataset
## procAttributes = dir(proc) #Takes a snapshot of what there in the process
## helpers.cloneProcessingSnippet( proc, proc.GeneralMuSequence, 'AtMu') #clones the sequence inside the process with AtMu postfix
## seqModifier = ApplyFunctionToSequence( lambda module: setTrigger( module, cms.PSet( hltDBKey = cms.string('TauTriggerForMuDataset'), hltPaths = cms.vstring('HLT_IsoMu24_eta2p1_v*') ) ) )
## proc.GeneralMuSequenceAtMu.visit(seqModifier)
## #checks what's new in the process (the cloned sequences and modules in them)
## newProcAttributes = filter( lambda x: (x not in procAttributes) and (x.find('AtMu') != -1), dir(proc) )
## #spawns a local variable with the same name as the proc attribute, needed for future process.load
## for newAttr in newProcAttributes:
##     locals()[newAttr] = getattr(proc,newAttr)
 
## pfTauRunDQMValidationMu = cms.Sequence(
##     TauValNumeratorAndDenominatorRealDataAtMu+
##     TauValNumeratorAndDenominatorRealMuonsDataAtMu+
##     dqmInfoTauV
##     )

#SingleMu Dataset
procAttributes = dir(proc) #Takes a snapshot of what there in the process
helpers.cloneProcessingSnippet( proc, proc.GeneralMuSequence, 'AtSingleMu') #clones the sequence inside the process with AtSingleMu postfix
seqModifier = ApplyFunctionToSequence( lambda module: setTrigger( module, cms.PSet( hltDBKey = cms.string('TauTriggerForSingleMuDataset'), hltPaths = cms.vstring('HLT_IsoMu24_eta2p1_v*') ) ) )
proc.GeneralMuSequenceAtSingleMu.visit(seqModifier)
#checks what's new in the process (the cloned sequences and modules in them)
newProcAttributes = filter( lambda x: (x not in procAttributes) and (x.find('AtSingleMu') != -1), dir(proc) )
#spawns a local variable with the same name as the proc attribute, needed for future process.load
for newAttr in newProcAttributes:
    locals()[newAttr] = getattr(proc,newAttr)

pfTauRunDQMValidationSingleMu = cms.Sequence(
    TauValNumeratorAndDenominatorRealDataAtSingleMu+
    TauValNumeratorAndDenominatorRealMuonsDataAtSingleMu+
    dqmInfoTauV
    )

#Jet Dataset
procAttributes = dir(proc) #Takes a snapshot of what there in the process
helpers.cloneProcessingSnippet( proc, proc.TauValNumeratorAndDenominatorRealData, 'AtJet') #clones the sequence inside the process with AtJet postfix
seqModifier = ApplyFunctionToSequence( lambda module: setTrigger( module, cms.PSet( hltDBKey = cms.string('TauTriggerForJetDataset'), hltPaths = cms.vstring('HLT_Jet30_L1FastJet_v*') ) ) )
proc.TauValNumeratorAndDenominatorRealDataAtJet.visit(seqModifier)
#checks what's new in the process (the cloned sequences and modules in them)
newProcAttributes = filter( lambda x: (x not in procAttributes) and (x.find('AtJet') != -1), dir(proc) )
#spawns a local variable with the same name as the proc attribute, needed for future process.load
for newAttr in newProcAttributes:
    locals()[newAttr] = getattr(proc,newAttr)

pfTauRunDQMValidationJet = cms.Sequence(
    TauValNumeratorAndDenominatorRealDataAtJet+
    dqmInfoTauV
    )

#MultiJet Dataset
procAttributes = dir(proc) #Takes a snapshot of what there in the process
helpers.cloneProcessingSnippet( proc, proc.TauValNumeratorAndDenominatorRealData, 'AtMultiJet') #clones the sequence inside the process with AtMultiJet postfix
seqModifier = ApplyFunctionToSequence( lambda module: setTrigger( module, cms.PSet( hltDBKey = cms.string('TauTriggerForMultiJetDataset'), hltPaths = cms.vstring('OUR_HLT_FALLBACK_PATH') ) ) )
proc.TauValNumeratorAndDenominatorRealDataAtMultiJet.visit(seqModifier)
#checks what's new in the process (the cloned sequences and modules in them)
newProcAttributes = filter( lambda x: (x not in procAttributes) and (x.find('AtMultiJet') != -1), dir(proc) )
#spawns a local variable with the same name as the proc attribute, needed for future process.load
for newAttr in newProcAttributes:
    locals()[newAttr] = getattr(proc,newAttr)

pfTauRunDQMValidationMultiJet = cms.Sequence(
    TauValNumeratorAndDenominatorRealDataAtMultiJet+
    dqmInfoTauV
    )

#DoubleElectron Dataset
procAttributes = dir(proc) #Takes a snapshot of what there in the process
helpers.cloneProcessingSnippet( proc, proc.TauValNumeratorAndDenominatorRealElectronsData, 'AtDoubleElectron') #clones the sequence inside the process with AtDoubleElectron postfix
seqModifier = ApplyFunctionToSequence( lambda module: setTrigger( module, cms.PSet( hltDBKey = cms.string('TauTriggerForDoubleElectronDataset'), hltPaths = cms.vstring('HLT_Ele20_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC4_Mass50_v*') ) ) ) 
proc.TauValNumeratorAndDenominatorRealElectronsDataAtDoubleElectron.visit(seqModifier)
#checks what's new in the process (the cloned sequences and modules in them)
newProcAttributes = filter( lambda x: (x not in procAttributes) and (x.find('AtDoubleElectron') != -1), dir(proc) )
#spawns a local variable with the same name as the proc attribute, needed for future process.load
for newAttr in newProcAttributes:
    locals()[newAttr] = getattr(proc,newAttr)

pfTauRunDQMValidationDoubleElectron = cms.Sequence(
    TauValNumeratorAndDenominatorRealElectronsDataAtDoubleElectron+
    dqmInfoTauV
    )

#TauPlusX Dataset
procAttributes = dir(proc) #Takes a snapshot of what there in the process
helpers.cloneProcessingSnippet( proc, proc.TauValNumeratorAndDenominatorRealElectronsData, 'TauPlusX') #clones the sequence inside the process with TauPlusX postfix
seqModifier = ApplyFunctionToSequence( lambda module: setTrigger( module, cms.PSet( hltDBKey = cms.string('TauTriggerForTauPlusXDataset'), hltPaths = cms.vstring('HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v*') ) ) )
proc.TauValNumeratorAndDenominatorRealElectronsDataTauPlusX.visit(seqModifier)
#checks what's new in the process (the cloned sequences and modules in them)
newProcAttributes = filter( lambda x: (x not in procAttributes) and (x.find('TauPlusX') != -1), dir(proc) )
#spawns a local variable with the same name as the proc attribute, needed for future process.load
for newAttr in newProcAttributes:
    locals()[newAttr] = getattr(proc,newAttr)

pfTauRunDQMValidationTauPlusX = cms.Sequence(
    TauValNumeratorAndDenominatorRealElectronsDataTauPlusX+
    dqmInfoTauV
    )

#----------------------------------------------------------------------------------------------------------------------------------------
#                                                      Efficiencies production according to dataset
#----------------------------------------------------------------------------------------------------------------------------------------
## runTauEffMu = cms.Sequence(
##     efficienciesRealMuonsData+
##     efficienciesRealData+
##     normalizePlotsRealMuonsData
##     )
runTauEffSingleMu =  cms.Sequence(
    efficienciesRealMuonsData+
    efficienciesRealData+
    normalizePlotsRealMuonsData
    )       

runTauEffJet = TauEfficienciesRealData
runTauEffMutiJet = runTauEffJet

runTauEffDoubleElectron = produceDenominatorRealElectronsData
runTauEffTauPlusX = runTauEffDoubleElectron

##Full sequences, including normalizations
## TauEfficienciesRealData+
## TauEfficienciesRealElectronsData+
## TauEfficienciesRealMuonsData

