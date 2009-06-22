import FWCore.ParameterSet.Config as cms

process = cms.Process("NTUPLE")


process.Timing =cms.Service("Timing")

process.MessageLogger = cms.Service("MessageLogger",
                                    ntu = cms.untracked.PSet(
    threshold = cms.untracked.string('Warning')
    ),
                                    destinations = cms.untracked.vstring('ntu')
                                    )





# Include PAT Layer 0 & 1 if not running on pattified data
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('IDEAL_V11::All')
process.load("Configuration.StandardSequences.MagneticField_cff")

# CaloTowerConstituentsMap needed for Electron/Photon-Jet cleaning
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
process.CaloTowerConstituentsMapBuilder = cms.ESProducer("CaloTowerConstituentsMapBuilder",
    MapFile = cms.untracked.string('Geometry/CaloTopology/data/CaloTowerEEGeometric.map.gz')
)

# Cross-cleaner setup
process.load("SusyAnalysis.PatCrossCleaner.patCrossCleaner_cfi")
# Switch on/off some components
process.patcrosscleaner.doMuonJetCC        = True
#process.patcrosscleaner.doElectronJetCC    = True
process.patcrosscleaner.doElectronJetCC    = True
process.patcrosscleaner.doPhotonJetCC      = True
process.patcrosscleaner.doElectronPhotonCC = True
# Change the jet energy corrections
process.patcrosscleaner.L1JetCorrector      = 'none'
process.patcrosscleaner.L2JetCorrector      = 'L2RelativeJetCorrectorIC5Calo'
process.patcrosscleaner.L3JetCorrector      = 'L3AbsoluteJetCorrectorIC5Calo'
process.patcrosscleaner.L4JetCorrector      = 'none'
process.patcrosscleaner.L5udsJetCorrector   = 'none'
process.patcrosscleaner.L5gluonJetCorrector = 'none'
process.patcrosscleaner.L5cJetCorrector     = 'none'
process.patcrosscleaner.L5bJetCorrector     = 'none'
process.patcrosscleaner.L6JetCorrector      = 'none'
process.patcrosscleaner.L7udsJetCorrector   = 'L7PartonJetCorrectorIC5qJet'
process.patcrosscleaner.L7gluonJetCorrector = 'L7PartonJetCorrectorIC5gJet'
process.patcrosscleaner.L7cJetCorrector     = 'L7PartonJetCorrectorIC5cJet'
process.patcrosscleaner.L7bJetCorrector     = 'L7PartonJetCorrectorIC5bJet'

# Parameters for electron-jet cross-cleaning
#process.patcrosscleaner.ElectronJetCrossCleaning.SusyAnalyzerCleaning = True
process.patcrosscleaner.ElectronJetCrossCleaning.deltaR_min        = 0.5
process.patcrosscleaner.ElectronJetCrossCleaning.SharedEtoJetE     = 0.7
#process.patcrosscleaner.ElectronJetCrossCleaning.SharedEForNIsoEle = -1.
#process.patcrosscleaner.ElectronJetCrossCleaning.IsolationKey  = 'TrackerIso'
#process.patcrosscleaner.ElectronJetCrossCleaning.IsoValueCut   = 1.
process.patcrosscleaner.ElectronJetCrossCleaning.IsolationKey = cms.string('CombRelIso')
process.patcrosscleaner.ElectronJetCrossCleaning.IsoValueCut = 0.2
process.patcrosscleaner.ElectronJetCrossCleaning.ecalIsoWeight = 1.
process.patcrosscleaner.ElectronJetCrossCleaning.hcalIsoWeight = 1.
process.patcrosscleaner.ElectronJetCrossCleaning.trkIsoWeight = 1.

process.patcrosscleaner.ElectronJetCrossCleaning.ElectronID   = 'eidRobustLoose'
# Parameters for photon-jet cross-cleaning
#process.patcrosscleaner.patPhotons = cms.InputTag("patPhotonIDProducer")
#process.patcrosscleaner.PhotonJetCrossCleaning.deltaR_min   = 0.4
#process.patcrosscleaner.PhotonJetCrossCleaning.IsoValueCut  = 1000000.  # ie, no limit
#process.patcrosscleaner.PhotonJetCrossCleaning.IsolationKey = 'CaloIso' # effectively not used
#process.patcrosscleaner.PhotonJetCrossCleaning.SharedEtoJetE = 0.7 # TW added to drop jets
#SSprocess.patcrosscleaner.PhotonJetCrossCleaning.PhotonID = 'TightPhoton'
# Parameters for muon-jet cross-cleaning
process.patcrosscleaner.MuonJetCrossCleaning.deltaR_min   = 0.2
#process.patcrosscleaner.MuonJetCrossCleaning.caloIso_max  = 10.0 # ntuple 2 and older
#process.patcrosscleaner.MuonJetCrossCleaning.trackIso_max = 10.0 # ntuple 2 and older
process.patcrosscleaner.MuonJetCrossCleaning.useCombRelIso = True # ntuple 3 + // V+jets recommodation
process.patcrosscleaner.MuonJetCrossCleaning.IsoValueCut = 0.2
process.patcrosscleaner.MuonJetCrossCleaning.ecalIsoWeight = 1.
process.patcrosscleaner.MuonJetCrossCleaning.hcalIsoWeight = 1.
process.patcrosscleaner.MuonJetCrossCleaning.trkIsoWeight = 1.
process.patcrosscleaner.MuonJetCrossCleaning.MuonID = 'AllGlobalMuons'

# clone for JPT corrections
#process.load("JetMETCorrections.JetPlusTrack.L1L3Corrections_JPT_cff")
#process.patcrosscleanerJPT = process.patcrosscleaner.clone()
#process.patcrosscleanerJPT.L1JetCorrector      = 'L1OffsetJetCorrectorZSP'
#process.patcrosscleanerJPT.L2JetCorrector      = 'none'
#process.patcrosscleanerJPT.L3JetCorrector      = 'L3AbsoluteJetCorrectorJPT'
#process.patcrosscleanerJPT.L7udsJetCorrector   = 'none'
#process.patcrosscleanerJPT.L7gluonJetCorrector = 'none'
#process.patcrosscleanerJPT.L7cJetCorrector     = 'none'
#process.patcrosscleanerJPT.L7bJetCorrector     = 'none'
#process.patcrosscleanerJPT.MuonJetCrossCleaning.modifyJetEnergy = False

process.load("PhysicsTools.PatAlgos.patLayer0_cff")
process.load("PhysicsTools.PatAlgos.patLayer1_cff")
process.load("PF.Susy.patFromPF2PAT_cff")
process.load("PF.Susy.PF2PAT_cff")
process.load("PF.Susy.patLayer1_EventContent_cff")

#process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer08_cff")
#process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer08Redigi_cff")
process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer08Redigi_cff")

#process.load("JetMETCorrections.Configuration.L2L3Corrections_Winter09_cff")

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('bbjets_100_250_new.root')
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    
#'file:pat.root'
   'file:patLayer1.root'

    )

)



process.maxEvents = cms.untracked.PSet(   input = cms.untracked.int32(5) )
aFile = ""
if len(process.source.fileNames)>0: aFile       = process.source.fileNames[0].lower()
isFastSim   = ("fastsim" in aFile)
doPhotonID  = (not isFastSim and "aodsim" not in aFile)

isFastSim = False
doPhotonID = True


#process.genParticles.abortOnUnknownPDGCode = False

# Re-generates PhotonID used same variables provided by RECO, but ECAL/HCAL iso replaced using IsoDeposit values
#process.load("bainbrid.Test.patPhotonIDProducer_cfi")



#-- ECAL Saturation bug fix ---------------------------------------------------
### Make new ECal RecHit collection ###
if not isFastSim:
      process.load("SUSYBSMAnalysis.CorrectedECalRecHitProducer.correctedecalrechitproducer_cfi")
      process.CorrectedECalRecHitProducer.recHitsEB = cms.InputTag("ecalRecHit","EcalRecHitsEB")
      process.CorrectedECalRecHitProducer.recHitsEE = cms.InputTag("ecalRecHit","EcalRecHitsEE")

### Change ECal input collection for reconstruction ###
      process.load("Configuration.StandardSequences.Reconstruction_cff")
      process.towerMaker.ecalInputs = cms.VInputTag(cms.InputTag("CorrectedECalRecHitProducer","EcalRecHitsEBcorr",""),
                                                    cms.InputTag("CorrectedECalRecHitProducer","EcalRecHitsEEcorr",""))
if isFastSim:
      process.moreRecoJetMET = cms.Sequence( process.muonMETValueMapProducer
                                             * process.muonTCMETValueMapProducer
                                             * process.tcMet
                                             )
else:
      process.load("Configuration.StandardSequences.Reconstruction_cff")
      process.towerMaker.ecalInputs = cms.VInputTag(cms.InputTag("CorrectedECalRecHitProducer","EcalRecHitsEBcorr",""),
                                                    cms.InputTag("CorrectedECalRecHitProducer","EcalRecHitsEEcorr",""))
      process.moreRecoJetMET = cms.Sequence( process.CorrectedECalRecHitProducer
                                             * process.caloTowersRec
                                             * process.recoJets
                                             * process.metreco
                                             )
      process.patAODJetMETCorrections.remove(process.muonMETValueMapProducer) # otherwise this module is duplicated in process.metreco
            
#--to fix photon cleaning -----------------------------------------------------
from PhysicsTools.PatAlgos.cleaningLayer0.photonCleaner_cfi import *
process.allLayer0Photons.removeDuplicates    = cms.string('none')
process.allLayer0Photons.removeElectrons     = cms.string('none')

# Remove embedding of superClusters, will keep entire superCluster collection
process.allLayer1Electrons.embedSuperCluster = cms.bool(False)
process.allLayer1Photons.embedSuperCluster   = cms.bool(False)

#-- redo PhotonID  --------------------------------------------------------------
if doPhotonID:
      process.load("RecoEgamma.PhotonIdentification.photonId_cff")
      if isFastSim: process.PhotonIDProd.barrelEcalRecHitProducer = cms.string('caloRecHits')
      else:         process.PhotonIDProd.barrelEcalRecHitProducer = cms.string('ecalRecHit')
      if isFastSim: process.PhotonIDProd.endcapEcalRecHitProducer = cms.string('caloRecHits')
      else:         process.PhotonIDProd.endcapEcalRecHitProducer = cms.string('ecalRecHit')
      
#-- JetPlusTrack --------------------------------------------------------------
process.load("JetMETCorrections.Configuration.ZSPJetCorrections219_cff")
process.load("JetMETCorrections.Configuration.JetPlusTrackCorrections_cff")
process.JPT = cms.Sequence( process.ZSPJetCorrections + process.JetPlusTrackCorrections )
        

#-- Extra PAT collections -----------------------------------------------------
#include "PhysicsTools/PatAlgos/python/tools/jetTools.py"
from PhysicsTools.PatAlgos.tools.jetTools import *
if isFastSim:   switchJECSet(process, newName='Winter09', oldName='Summer08Redigi')
if True:
      addJetCollection(process, "iterativeCone5CaloJets", "IC5"
                                                                           , doJTA             = True   # Run Jet-Track association & JetCharge
                                                                           , doBTagging        = True   # Run b-tagging
                                                                           , jetCorrLabel      = ("IC5", "Calo")
                                                                           , doType1MET        = True
                                                                           , genJetCollection  = cms.InputTag("iterativeCone5GenJets")
                                                                           )
      addJetCollection  (process, "iterativeCone5PFJets"  , 'IC5PF'
                                                                             , runCleaner        = "PFJet"
                                                                             , doJTA             = False  # Run Jet-Track association & JetCharge
                                                                             , doBTagging        = True   # Run b-tagging
                                                                             , jetCorrLabel      = ("IC5", "PF")
                                                                             , doType1MET        = False
                                                                             , genJetCollection  = cms.InputTag("iterativeCone5GenJets")
                                                                             )
      switchJetCollection(process, "sisCone5CaloJets"     , runCleaner        = "CaloJet"
                                                                              , doJTA             = True   # Run Jet-Track association & JetCharge
                                                                              , doBTagging        = True   # Run b-tagging
                                                                              , jetCorrLabel      = ("SC5", "Calo")
                                                                              , doType1MET        = True
                                                                              , genJetCollection  = cms.InputTag("sisCone5GenJets")

                                                                              )

      #-- Jet Plus Track  -----------------------------------------------------
      addJetCollection(process, "JetPlusTrackZSPCorJetIcone5", "IC5JPT"
                                                                           , doJTA             = True   # Run Jet-Track association & JetCharge
                                                                           , doBTagging        = True   # Run b-tagging
                                                                           , jetCorrLabel      = None
                                                                           , doType1MET        = True
                                                                           , genJetCollection  = cms.InputTag("iterativeCone5GenJets")
                                                                           )


      #-- Particle flow MET is different from other MET collections  -------------------------------------
      process.load("PhysicsTools.PFCandProducer.pfMET_cfi")
      process.allLayer1METsPF = process.allLayer1METs.clone(
            metSource = cms.InputTag("pfMET"),
              addTrigMatch = cms.bool(False),
              addMuonCorrections = cms.bool(False),
            )
      process.patLayer0.replace( process.allLayer0METs, process.allLayer0METs + process.pfMET )
      process.patLayer1.replace( process.allLayer1METs, process.allLayer1METs + process.allLayer1METsPF )

      #-- tcMET ---------------------------------------------------------------------
      process.load("RecoMET.METProducers.TCMET_cfi")
      process.allLayer0METstcMET = cms.EDFilter("PATBaseMETCleaner",
                                                    metSource = cms.InputTag('tcMet'), ## met corrected for jets and for muons
                                                    markItems = cms.bool(True),    ## write the status flags in the output items
                                                    bitsToIgnore = cms.vstring(),  ## You can specify some bit names,e.g. "Overflow/User1", "Core/Duplicate", "Isolation/All".
                                                    saveRejected = cms.string(''), ## set this to a non empty label tosave the list of items which fail
                                                    saveAll = cms.string(''),      ## set this to a non empty label tosave a list of all items both passing and failing
                                                )
      process.metTrigMatchHLT1MET65tcMET  = process.metTrigMatchHLT1MET65.clone (src       = cms.InputTag('allLayer0METstcMET'))
      process.allLayer1METstcMET          = process.allLayer1METs.clone         (metSource = cms.InputTag('allLayer0METstcMET'), trigPrimMatch = cms.VInputTag(cms.InputTag("metTrigMatchHLT1MET65tcMET")))
      process.selectedLayer1METstcMET     = process.selectedLayer1METs.clone    (src       = cms.InputTag('allLayer1METstcMET'))

      process.tcMetSequence = cms.Sequence( process.allLayer0METstcMET
                                            * process.metTrigMatchHLT1MET65tcMET
                                            * process.allLayer1METstcMET
                                            * process.selectedLayer1METstcMET
                                            )
      # Re-name collections for uniformity
      process.allLayer1JetsSC5        = process.allLayer1Jets
      process.allLayer1METsSC5        = process.allLayer1METs
      process.selectedLayer1Jets.src  = cms.InputTag("allLayer1JetsSC5")
      process.selectedLayer1METs.src  = cms.InputTag("allLayer1METsSC5")
      process.layer1Jets.replace(process.allLayer1Jets, process.allLayer1JetsSC5)
      process.layer1METs.replace(process.allLayer1METs, process.allLayer1METsSC5)
                                            
      #process.patify    =  cms.Path( process.moreRecoJetMET )
      #if doPhotonID:
        #process.patify *=  process.photonIDSequence
        # process.patify   *=( process.JPT
        #                                        * process.patLayer0
        #                                       * process.patLayer1
          #                                      * process.tcMetSequence
          #                                     )
          
      



#process.genParticles.abortOnUnknownPDGCode = False

# Re-generates PhotonID used same variables provided by RECO, but ECAL/HCAL iso replaced using IsoDeposit values
#process.load("bainbrid.Test.patPhotonIDProducer_cfi")

process.load("PF.Susy.hemisphere_cfi")
process.load("dijet_cfi")
#process.dijet.photTag = cms.InputTag("patPhotonIDProducer")
process.load("PF.Susy.pfhemisphere_cfi")
process.load("pfdijet_cfi")
#process.pfdijet.photTag = cms.InputTag("patPhotonIDProducer")

process.p = cms.Path(
    #process.moreRecoJetMET
    #* process.JPT
    #* process.patLayer0
    #* process.patLayer1
    #* process.tcMetSequence *
    #process.patPhotonIDProducer *
    process.patcrosscleaner *
    #process.patcrosscleanerJPT * 
    process.selectedLayer2Hemispheres *
    process.dijet 
    #process.pfemispheres *
    #process.pfdijet
    )
