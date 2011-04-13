import FWCore.ParameterSet.Config as cms

from TopQuarkAnalysis.SingleTop.SingleTopProducers_New_cff import *
from TopQuarkAnalysis.SingleTop.SingleTopSelectors_New_cff import *

#### Set the cuts values ####

### Cuts for the kinematic variables, and ID and b-tagging###

#Muons, electrons and jets kinematic and ID cuts
muLooseCut = cms.string("isGlobalMuon & pt > 9 & abs(eta) < 2.5 & (isolationR03.sumPt + isolationR03.emEt + isolationR03.hadEt)/pt > -0.2")#RelIso < 0.2
eleLooseCut = cms.string("et > 13.5 & abs(eta) < 2.5 & (dr03TkSumPt + dr03EcalRecHitSumEt + dr03HcalTowerSumEt)/et > -0.2 ")#RelIso < 0.2

#Cuts on the isolation of the leptons
eleTightCut = cms.string("et>27  && abs(eta)<2.5  & (gsfTrack().trackerExpectedHitsInner.numberOfHits == 0) & (dr03TkSumPt + dr03EcalRecHitSumEt + dr03HcalTowerSumEt)/et > -0.1  & dB < 0.02 & ( abs(superCluster.eta)> 1.5660 || abs(superCluster.eta)<1.4442) & (electronID('simpleEleId70cIso')==5 || electronID('simpleEleId70cIso')==7)")#RelIso < 0.1
#Legenda for eleId : 0 fail, 1 ID only, 2 iso Only, 3 ID iso only, 4 conv rej, 5 conv rej and ID, 6 conv rej and iso, 7 all  

#innerTrack.numberOfValidHits
muTightCut = cms.string("pt > 18 & isGlobalMuon && isTrackerMuon & abs(eta) < 2.1 && muonID('GlobalMuonPromptTight') > 0 & (isolationR03.sumPt + isolationR03.emEt + isolationR03.hadEt)/pt > -0.05 & dB < 0.02 & innerTrack.numberOfValidHits > 10")#RelIso < 0.05 

#Jet definition
jetLooseCut = cms.string("numberOfDaughters()>1 & pt()> 20 && abs(eta())<5 & ((abs(eta())>2.4) || ( chargedHadronEnergyFraction() > 0 & chargedMultiplicity()>0 & neutralEmEnergyFraction() < 0.99 & neutralHadronEnergyFraction() < 0.99 & chargedEmEnergyFraction()<0.99))")

#Number of leptons that survive all the cuts ( kin + iso + ID )
minLooseLeptons = cms.int32(0)
maxLooseLeptons = cms.int32(1)

#Number of leptons that survive all the cuts ( kin + iso + ID )
minTightLeptons = cms.int32(1)
maxTightLeptons = cms.int32(1)

#Detailed request for number of leptons ( not used in standard analysis )
minMuons = cms.uint32(1)
maxMuons = cms.uint32(1)

minElectrons = cms.uint32(1)
maxElectrons = cms.uint32(1)




### MC details that do not influence the selection  ###

#Check to see if it is signal channel ( MC only )
isMCSingleTopTChannel = cms.untracked.bool(False)

#definition: Leptons Loose
looseElectrons.cut =  eleLooseCut
looseMuons.cut = muLooseCut

#definition: Leptons Tight
tightElectrons.cut =  eleTightCut
tightMuons.cut = muTightCut

#definition: Jets Loose
topJetsPF.cut = jetLooseCut

countLeptons.minNumberLoose = minLooseLeptons
countLeptons.maxNumberLoose = maxLooseLeptons

countLeptons.minNumberTight = minTightLeptons
countLeptons.maxNumberTight = maxTightLeptons
