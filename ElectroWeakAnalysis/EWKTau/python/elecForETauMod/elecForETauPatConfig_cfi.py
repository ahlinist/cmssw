import FWCore.ParameterSet.Config as cms
import copy

from PhysicsTools.PatAlgos.cleaningLayer0.electronCleaner_cfi import *
from PhysicsTools.PatAlgos.producersLayer1.electronProducer_cfi import *

allLayer0ElecForETau = copy.deepcopy(allLayer0Electrons)
allLayer0ElecForETau.electronSource = cms.InputTag("pixelMatchGsfElectrons")
allLayer0ElecForETau.removeDuplicates = cms.bool(True)
allLayer0ElecForETau.selection = cms.PSet(type = cms.string('none'))
allLayer0ElecForETau.isolation = cms.PSet()
allLayer0ElecForETau.markItems = cms.bool(True)
allLayer0ElecForETau.bitsToIgnore = cms.vstring('')
allLayer0ElecForETau.saveRejected = cms.string('')
allLayer0ElecForETau.saveAll = cms.string('')
     
allLayer1ElecForETau = copy.deepcopy(allLayer1Electrons)
allLayer1ElecForETau.electronSource = cms.InputTag("allLayer0ElecForETau")
allLayer1ElecForETau.embedTrack = cms.bool(False)
allLayer1ElecForETau.embedGsfTrack = cms.bool(False)
allLayer1ElecForETau.embedSuperCluster = cms.bool(False)
allLayer1ElecForETau.isolation = cms.PSet(
   tracker = cms.PSet(
      src = cms.InputTag("layer0ElectronIsolations","eleIsoDepositForETauTk"),
      deltaR = cms.double(0.6),
      vetos = cms.vstring(
         '0.02', # inner radius veto cone (was 0.015)
         'Threshold(1.0)' # threshold on individual track pt
      ),       
      skipDefaultVeto = cms.bool(True),
   ),
   ecal = cms.PSet(
      src = cms.InputTag("layer0ElectronIsolations","eleIsoDepositForETauEcalFromHits"),
      deltaR = cms.double(0.6),
      vetos = cms.vstring(
         'EcalBarrel:0.040', 
         'EcalBarrel:RectangularEtaPhiVeto(-0.02,0.02,-0.5,0.5)',
         'EcalEndcaps:0.070',
         'EcalEndcaps:RectangularEtaPhiVeto(-0.02,0.02,-0.5,0.5)',
         'EcalBarrel:ThresholdFromTransverse(0.08)',
         'EcalEndcaps:ThresholdFromTransverse(0.3)'
      ),
      skipDefaultVeto = cms.bool(True),
   ),
   hcal = cms.PSet(
      src = cms.InputTag("layer0ElectronIsolations","eleIsoDepositForETauHcalFromTowers"),
      deltaR = cms.double(0.6),
      skipDefaultVeto = cms.bool(True),
   ),
)
allLayer1ElecForETau.isoDeposits = cms.PSet(
   tracker = cms.InputTag("layer0ElectronIsolations","eleIsoDepositForETauTk"),
   ecal    = cms.InputTag("layer0ElectronIsolations","eleIsoDepositForETauEcalFromHits"),
   hcal    = cms.InputTag("layer0ElectronIsolations","eleIsoDepositForETauHcalFromTowers"),
)
allLayer1ElecForETau.addElectronID = cms.bool(True)
allLayer1ElecForETau.electronIDSources = cms.PSet(
   robust     = cms.InputTag("elecIdForETauCutBasedRobust"),
   loose      = cms.InputTag("elecIdForETauCutBasedLoose"),
   tight      = cms.InputTag("elecIdForETauCutBasedTight"),        
)
allLayer1ElecForETau.addTrigMatch = cms.bool(True)
allLayer1ElecForETau.trigPrimMatch = cms.VInputTag(
   cms.InputTag("electronTrigMatchHLT1Electron"),
   #cms.InputTag("electronTrigMatchHLT1ElectronRelaxed"), 
)
allLayer1ElecForETau.addGenMatch = cms.bool(False)
allLayer1ElecForETau.embedGenMatch    = cms.bool(False)
allLayer1ElecForETau.genParticleMatch = cms.InputTag("electronMatch")






## allLayer0ElecForETau = cms.EDFilter("PATElectronCleaner",
##     electronSource = cms.InputTag("pixelMatchGsfElectrons"), 
## 
##     # remove or flag duplicate (same track or same supercluster seed)
##     removeDuplicates = cms.bool(True),
## 
##     # selection (e.g. ID)
##     selection = cms.PSet(
##         type = cms.string('none')
##     )
##)

## allLayer1ElecForETau = cms.EDProducer("PATElectronProducer",
##     # General configurables
##     electronSource = cms.InputTag("allLayer0ElecForETau"),

##     # user data to add
##     userData = cms.PSet(
##       # add custom classes here
##       userClasses = cms.PSet(
##         src = cms.VInputTag('')
##       ),
##       # add doubles here
##       userFloats = cms.PSet(
##         src = cms.VInputTag('')
##       ),
##       # add ints here
##       userInts = cms.PSet(
##         src = cms.VInputTag('')
##       ),
##       # add "inline" functions here
##       userFunctions = cms.vstring(""),
##       userFunctionLabels = cms.vstring("")
##     ),                                      

##     # Embedding of AOD items
##     embedTrack        = cms.bool(False), ## whether to embed in AOD externally stored track (note: gsf electrons don't have a track)
##     embedGsfTrack     = cms.bool(False), ## whether to embed in AOD externally stored gsf track
##     embedSuperCluster = cms.bool(False), ## whether to embed in AOD externally stored supercluster

##     # resolution configurables
##     addResolutions   = cms.bool(True),
##     electronResoFile = cms.string('PhysicsTools/PatUtils/data/Resolutions_electron.root'), # input root file for the resolution functions
##     useNNResolutions = cms.bool(False), ## use the neural network approach?

##     # Store isolation values
##     isolation = cms.PSet(
##         tracker = cms.PSet(
##             src = cms.InputTag("layer0ElectronIsolations","eleIsoDepositForETauTk"),
##             deltaR = cms.double(0.8),
##             vetos = cms.vstring('0.02',                  # inner radius veto cone (was 0.015)
##                                 'Threshold(1.0)'),       # threshold on individual track pt
##             skipDefaultVeto = cms.bool(True),
##           ),
        
##         ecal = cms.PSet(
##             src = cms.InputTag("layer0ElectronIsolations","eleIsoDepositForETauEcalFromHits"), #For future PAT
##          #  src = cms.InputTag("layer0ElectronIsolations","eleIsoDepositForETauEcalFromClusts"),
##            deltaR = cms.double(0.4),
##            vetos = cms.vstring('EcalBarrel:0.040', 
##                               'EcalBarrel:RectangularEtaPhiVeto(-0.02,0.02,-0.5,0.5)',
##                               'EcalEndcaps:0.070',
##                               'EcalEndcaps:RectangularEtaPhiVeto(-0.02,0.02,-0.5,0.5)',
##                               'EcalBarrel:ThresholdFromTransverse(0.08)', #For future PAT
##                               'EcalEndcaps:ThresholdFromTransverse(0.3)'  #For future PAT
##                              ),
##            skipDefaultVeto = cms.bool(True),
##           ),

##          hcal = cms.PSet(
##              src = cms.InputTag("layer0ElectronIsolations","eleIsoDepositForETauHcalFromTowers"),
##              deltaR = cms.double(0.4),
##              skipDefaultVeto = cms.bool(True),
##          ),
##          user = cms.VPSet(),
##      ),
##     # Store IsoDeposits
##     isoDeposits = cms.PSet(
##         tracker = cms.InputTag("layer0ElectronIsolations","eleIsoDepositForETauTk"),
##         #ecal    = cms.InputTag("layer0ElectronIsolations","eleIsoDepositForETauEcalFromClusts"),
##         ecal    = cms.InputTag("layer0ElectronIsolations","eleIsoDepositForETauEcalFromHits"),  #For Future PAT
##         hcal    = cms.InputTag("layer0ElectronIsolations","eleIsoDepositForETauHcalFromTowers"),
##     ),

##     # electron ID configurables
##     addElectronID = cms.bool(True),
##     electronIDSources = cms.PSet(
##         # configure many IDs as InputTag <someName> = <someTag>
##         robust     = cms.InputTag("elecIdForETauCutBasedRobust"),
##         loose      = cms.InputTag("elecIdForETauCutBasedLoose"),
##         tight      = cms.InputTag("elecIdForETauCutBasedTight"),        
##     ),

##     # Trigger matching configurables
##     addTrigMatch = cms.bool(True),
##     # trigger primitive sources to be used for the matching
##     trigPrimMatch = cms.VInputTag(
##             cms.InputTag("electronTrigMatchHLT1Electron"),
##             #cms.InputTag("electronTrigMatchHLT1ElectronRelaxed"), 
##     ),

##     # MC matching configurables
##     addGenMatch      = cms.bool(False),
##     embedGenMatch    = cms.bool(False),
##     genParticleMatch = cms.InputTag("electronMatch"), ## Association between electrons and generator particles

##     # Efficiencies
##     addEfficiencies = cms.bool(False),
##     efficiencies    = cms.PSet(),

## )
