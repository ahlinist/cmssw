import FWCore.ParameterSet.Config as cms

##### WMuNu group standard selector ####

#from ElectroWeakAnalysis.WMuNu.WMuNuSelection_cff import *


### HLT filter ###
import HLTrigger.HLTfilters.hltHighLevel_cfi
hltFilter = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
hltFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","REDIGI")
hltFilter.HLTPaths = ['HLT_Mu9']#,'HLT_Ele15_LW_L1R']


hltFilterEle = cms.EDFilter("HLTSummaryFilter",
                            summary = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
                            member = cms.InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt10HcalIsolFilter","","HLT"),
                            cut = cms.string("pt> 20"),
                            minN = cms.int32(1),
                                 )

#####Primary vertex filter
PVFilter = cms.EDFilter(
    'SingleTopVertexFilter',
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string('!isFake & position().Rho() < 2.0 & abs(z) < 24 & ndof > 4.'),
    filter = cms.bool(True)
    )

PVFilterProducer = cms.EDFilter(
    'VertexSelector',
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string('!isFake & position().Rho() < 2.0 & abs(z) < 24 & ndof > 4.'),
    filter = cms.bool(False)
    )


######### Part of selection: Particle counting ##########
countElectrons = cms.EDFilter("PATCandViewCountFilter",
                              src = cms.InputTag("tightElectrons"),
                              minNumber = cms.uint32(1),
                              maxNumber = cms.uint32(9999),
                              filter = cms.bool(True)
                              )

countMuons = cms.EDFilter("PATCandViewCountFilter",
                          src = cms.InputTag("tightMuons"),
                          minNumber = cms.uint32(1),
                          maxNumber = cms.uint32(9999),
                          filter = cms.bool(True)
                          )

countLeptonsNoOverlap = cms.EDFilter("CandOrCounterNoOverlap",
                            
                            src1 = cms.InputTag("tightMuons"),
                            src2 = cms.InputTag("tightElectrons"),

                            srcOverlap1 = cms.InputTag("looseMuons"),
                            srcOverlap2 = cms.InputTag("looseElectrons"),
                            
                            minNumberTight = cms.int32(1),
                            maxNumberTight = cms.int32(9999),

                            minNumberLoose = cms.int32(1),
                            maxNumberLoose = cms.int32(9999),
                            )


countLeptons = cms.EDFilter("CandOrCounter",
                            
                            src1 = cms.InputTag("tightMuons"),
                            src2 = cms.InputTag("tightElectrons"),
                            
                            minNumber = cms.int32(1),
                            maxNumber = cms.int32(9999),
                            )
