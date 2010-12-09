import FWCore.ParameterSet.Config as cms
from TauAnalysis.Skimming.EventContent_cff import tauAnalysisEventContent
from TauAnalysis.Skimming.ewkElecTauValHistManager_cfi import *


def addElecTauSkimm(process):

  #--------------------------------------------------------------------------------
  # select electrons and tau-jets
  #--------------------------------------------------------------------------------

  #  WP95 electron ID
	process.selectedElectrons = cms.EDFilter("GsfElectronSelector",
		  src = cms.InputTag("gsfElectrons"),
		  cut = cms.string("abs(eta) < 2.5 & pt > 12. &"
			  " ((abs(superCluster.eta) < 1.479 & abs(deltaEtaSuperClusterTrackAtVtx) < 0.007 & abs(deltaPhiSuperClusterTrackAtVtx) < 0.8 & hcalOverEcal < 0.12 & sigmaIetaIeta < 0.01) | (abs(superCluster.eta) > 1.479 & abs(deltaEtaSuperClusterTrackAtVtx) < 0.009 & abs(deltaPhiSuperClusterTrackAtVtx) <0.7 & hcalOverEcal < 0.05 & sigmaIetaIeta < 0.03))"),
		  filter = cms.bool(True)
		  )

	process.selectedTaNCtaus = cms.EDFilter("PFTauSelector",
		  src = cms.InputTag('hpsTancTaus'),
		  discriminators = cms.VPSet(
			  cms.PSet(
				  discriminator = cms.InputTag("hpsTancTausDiscriminationByVLooseIsolation"),
				  selectionCut = cms.double(0.5)
				  )
			  ),
		  filter = cms.bool(True)
		  )

	process.selectedHPStaus = cms.EDFilter("PFTauSelector",
		  src = cms.InputTag('hpsTancTaus'),
		  discriminators = cms.VPSet(
			  cms.PSet(
				  discriminator = cms.InputTag("hpsTancTausDiscriminationByTancVLoose"),
				  selectionCut = cms.double(0.5)
				  )
			  ),
		  filter = cms.bool(True)
		  )

  #--------------------------------------------------------------------------------
  # combine selected electrons and tau-jets into pairs;
  # DO NOT require electrons and tau-jets to be separated by dR > 0.3 in eta-phi,
  # in order to reject events in which the tau-jet refers to the same physical
  # particle as the electron (note that almost all electrons get selected as tau-jets !!)
  #--------------------------------------------------------------------------------

	process.elecTaNCtauPairs = cms.EDProducer("DiCandidatePairProducer",
		useLeadingTausOnly = cms.bool(False),
		srcLeg1 = cms.InputTag('selectedTaNCtaus'),
		srcLeg2 = cms.InputTag('selectedElectrons'),
		dRmin12 = cms.double(0.3),
		srcMET = cms.InputTag(''),
		recoMode = cms.string(""),
		verbosity = cms.untracked.int32(0)
		)

	process.selectedElecTaNCtauPairs = cms.EDFilter("DiCandidatePairSelector",
		src = cms.InputTag('elecTaNCtauPairs'),
		cut = cms.string("dR12 > -0.3"),
		filter = cms.bool(True)
		)

	process.elecHPStauPairs = cms.EDProducer("DiCandidatePairProducer",
		useLeadingTausOnly = cms.bool(False),
		srcLeg1 = cms.InputTag('selectedHPStaus'),
		srcLeg2 = cms.InputTag('selectedElectrons'),
		dRmin12 = cms.double(0.3),
		srcMET = cms.InputTag(''),
		recoMode = cms.string(""),
		verbosity = cms.untracked.int32(0)
		)

	process.selectedElecHPStauPairs = cms.EDFilter("DiCandidatePairSelector",
		src = cms.InputTag('elecHPStauPairs'),
		cut = cms.string("dR12 > -0.3"),
		filter = cms.bool(True)
		)

#--------------------------------------------------------------------------------
  # keep event in case it passed either the electron + pfTau or electron + caloTau selection
  #--------------------------------------------------------------------------------

	process.elecTaNCtauSkimPath = cms.Path(
		process.PFTau
		* (process.selectedTaNCtaus + process.selectedElectrons)
		* process.elecTaNCtauPairs
		* process.selectedElecTaNCtauPairs
		)

	process.elecHPStauSkimPath = cms.Path(
		process.PFTau
		* (process.selectedHPStaus + process.selectedElectrons)
		* process.elecHPStauPairs
		* process.selectedElecHPStauPairs
		)

	elecTauEventSelection = cms.untracked.PSet(
		SelectEvents = cms.untracked.PSet(
			SelectEvents = cms.vstring('elecTaNCtauSkimPath', 'elecHPStauSkimPath')
			)
		)

#--------------------------------------------------------------------------------
  # use HLT to count events passing OR.
#--------------------------------------------------------------------------------
	process.elecTauFilter = cms.EDFilter("HLTHighLevel",
		  TriggerResultsTag = cms.InputTag('TriggerResults','',process.name_()),
		  HLTPaths = elecTauEventSelection.SelectEvents.SelectEvents,
		  eventSetupPathsKey = cms.string(''), 
		  andOr = cms.bool(True),   
		  throw = cms.bool(True)
		  )


  #--------------------------------------------------------------------------------
  # fill validation histograms for events passing either the electron + pfTau or electron + caloTau selection
  #--------------------------------------------------------------------------------

	process.DQMStore = cms.Service("DQMStore")

	process.fillElecTauValPlots = cms.EDAnalyzer("EwkTauValidation",

		  # list of individual channels                           
		  channels = cms.VPSet(
			  ewkElecTauValHistManager
			  ),

		  # disable all warnings
		  maxNumWarnings = cms.int32(1)                      
		  )

	process.saveElecTauValPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
		  outputFileName = cms.string('elecTauValPlots.root')
		  )

  #--------------------------------------------------------------------------------
  # save events passing either the electron + pfTau or electron + caloTau selection
  #--------------------------------------------------------------------------------

	process.elecTauSkimOutputModule = cms.OutputModule("PoolOutputModule",                                 
		  tauAnalysisEventContent,
		  elecTauEventSelection,
		  dataset = cms.untracked.PSet(
			  filterName = cms.untracked.string('elecTauSkim')
			  ),
		  fileName = cms.untracked.string('skimElecTau.root')
		  )


	process.elecTauCounter = cms.EndPath(process.elecTauSkimOutputModule + process.elecTauFilter) 
