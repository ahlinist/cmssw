import FWCore.ParameterSet.Config as cms
## TODO: check that filters work (they don't)
## TODO: check that lepton1 is the leading lepton
## TODO: move to selectors in filter mode

WENuGammaFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("WENuGammaCands"),
    minNumber = cms.uint32(1),
    cut = cms.string(""), # dummy to be set below
)

WMuNuGammaFilter = WENuGammaFilter.clone(src = "WMuNuGammaCands")
ZEEGammaFilter   = WENuGammaFilter.clone(src = "ZEEGammaCands")
ZMuMuGammaFilter = WENuGammaFilter.clone(src = "ZMuMuGammaCands")
ZNuNuGammaFilter = WENuGammaFilter.clone(src = "ZNuNuGammaCands")

## Define some auxiliary dictionaries to make the rest neater
## (cms.string doesn't support `operator+=(str)' or equivalent)
filters = {
  "WENuGamma" : WENuGammaFilter,
  "WMuNuGamma": WMuNuGammaFilter,
  "ZEEGamma"  : ZEEGammaFilter,
  "ZMuMuGamma": ZMuMuGammaFilter,
  "ZNuNuGamma": ZNuNuGammaFilter,
}

## Dictionary to contain python strings describing cuts
cuts = {}

## Initialize the cut strings by requiring a photon pt > 10 GeV
for key in filters.keys(): cuts[key] = "daughter('photon').pt > 20"

## Add additional requirement of leading lepton pt > 15 GeV
cuts["WENuGamma"]  += " & daugher('electronPlusMET').daughter('electron').pt > 15"
cuts["WMuNuGamma"] += " & daugher('muonPlusMET').daughter('muon').pt > 15"
cuts["ZEEGamma"]   += " & daugher('dielectron').daughter('electron').pt > 15"
cuts["ZMuMuGamma"] += " & daugher('dimuon').daughter('muon').pt > 15"

## Add additional MET > 40 GeV cut for the ZNuNuGamma
cuts["ZNuNuGamma"] += " & daughter('MET').pt > 40"

## Set the filters' cut paramets
for key in cuts.keys():  filters[key].cut = cuts[key]
