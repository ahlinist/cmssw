# The following comments couldn't be translated into the new config version:

# histograms

import FWCore.ParameterSet.Config as cms

singleTopFilter = cms.EDFilter("STFilter",
    produceHistos = cms.bool(False),
    TH1bEtaFiltered = cms.PSet(
        xmin = cms.double(-8.0),
        Nbinx = cms.int32(100),
        xmax = cms.double(8.0)
    ),
    histOutFile = cms.untracked.string('filter_histos.root'),
    TH1bPt = cms.PSet(
        xmin = cms.double(0.0),
        Nbinx = cms.int32(150),
        xmax = cms.double(150.0)
    ),
    debuglvl = cms.untracked.int32(0), ## debug level (0=none, 1=small, 2=extended, 3 = very extended)

    TH1bEta = cms.PSet(
        xmin = cms.double(-8.0),
        Nbinx = cms.int32(100),
        xmax = cms.double(8.0)
    ),
    TH1bPtFiltered = cms.PSet(
        xmin = cms.double(0.0),
        Nbinx = cms.int32(150),
        xmax = cms.double(150.0)
    ),
    pTMax = cms.double(999.0)
)


