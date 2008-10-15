import FWCore.ParameterSet.Config as cms

# Examples for configurations of the trigger primitive producers.
#
# A list of valid HLT path names for CMSSW releases is found in: 
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideGlobalHLT
# Choose the path 'myPath' of your interest. Then you can find the filter modules by doing
# > grep -n "path [myPath]" /afs/cern.ch/cms/sw/slc4_ia32_gcc345/cms/cmssw/[CMSSW version]/src/HLTrigger/Configuration/data/HLT_{2E30,1E32}.cff
# 
# BEWARE: Use the list of the CMSSW version the data has been  p r o d u c e d  with,
#         not of that you are running with!
#         Use this version also in the 'grep' command -- not $CMSSW_VERSION!
# BEWARE: Only those parts of the paths starting with  l o w e r  case letters are modules!
#         Capital letters indicate sequences, which you also can 'grep' for then.
# BEWARE: Make sure to choose filter modules only, not e.g. prescaler modules!
# modules to produce pat::TriggerPrimitiveCollections,
# grouped by POG (corresponding to trigger config files)
# The module names without "pat" indicate the HLT path name.

# Egamma triggers

# Muon triggers
patHLT1Electron = cms.EDProducer("PATTrigProducer",
    triggerEvent = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
    filterName = cms.InputTag("hltL1IsoSingleElectronTrackIsolFilter","","HLT")
)

patHLT1ElectronRelaxed = cms.EDProducer("PATTrigProducer",
    triggerEvent = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
    filterName = cms.InputTag("hltL1NonIsoSingleElectronTrackIsolFilter","","HLT")
)

patHLT1MuonIso = cms.EDProducer("PATTrigProducer",
    triggerEvent = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
    filterName = cms.InputTag("hltSingleMuIsoL3IsoFiltered","","HLT")
)

patHLT1MuonNonIso = cms.EDProducer("PATTrigProducer",
    triggerEvent = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
    filterName = cms.InputTag("hltSingleMuNoIsoL3PreFiltered","","HLT")
)


patHLTXElectronTau = cms.EDProducer("PATTrigProducer",
    triggerEvent = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
    filterName = cms.InputTag("hltXElectronTau","","HLT")
)

patHLTXMuonTau = cms.EDProducer("PATTrigProducer",
    triggerEvent = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
    filterName = cms.InputTag("hltXMuonTau","","HLT")
)


patHLTXElectronMuon = cms.EDProducer("PATTrigProducer",
    triggerEvent = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
    filterName = cms.InputTag("hltXElectronMuon","","HLT")
)
