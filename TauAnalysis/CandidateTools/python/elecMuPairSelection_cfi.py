import FWCore.ParameterSet.Config as cms


# require electron and muon to be separated in space
selectedElecMuPairsAntiOverlapVeto = cms.EDFilter("PATElecMuPairSelector",
#     src = cms.InputTag("allElecMuPairs"),
     cut = cms.string('dR12 > 0.7'),
     filter = cms.bool(False)
)

# require muon and electron to form a zero-charge pair
selectedElecMuPairsZeroCharge = cms.EDFilter("PATElecMuPairSelector",
#     src = selectedElecMuPairsAntiOverlapVeto.src,
     cut = cms.string('charge = 0'),
     filter = cms.bool(False)
)
#require cut transverse mass of electron and MET
selectedElecMuPairsMt1MET = cms.EDFilter("PATElecMuPairSelector",
#    src = selectedElecMuPairsAntiOverlapVeto.src,
    cut = cms.string('mt1MET < 50.'),
    filter = cms.bool(False)
)

#require cut transverse mass of muon and MET
selectedElecMuPairsMt2MET = cms.EDFilter("PATElecMuPairSelector",    
#    src = selectedElecMuPairsAntiOverlapVeto.src,
    cut = cms.string('mt2MET < 50.'),
    filter = cms.bool(False)
)


selectElecMuPairs = cms.Sequence(
                                  selectedElecMuPairsAntiOverlapVeto         
	                         *selectedElecMuPairsZeroCharge
                                 *selectedElecMuPairsMt1MET
                                 *selectedElecMuPairsMt2MET
)
