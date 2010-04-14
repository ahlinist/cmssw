import FWCore.ParameterSet.Config as cms
import copy

def modifyPlots(process):
	TZtZeWGQ = cms.vstring(
			'TTplusJets', 
            'Ztautau',
			'Zee', 
			'WplusJets', 
            'gammaPlusJetsSum',
			'qcdSum'
	)
	ZtTZeWGQ = cms.vstring(
            'Ztautau',
			'TTplusJets', 
			'Zee', 
			'WplusJets', 
            'gammaPlusJetsSum',
			'qcdSum'
	)
	TZtWGQZe = cms.vstring(
			'TTplusJets', 
            'Ztautau',
			'WplusJets', 
            'gammaPlusJetsSum',
			'qcdSum',
			'Zee'
	)
	ZtTWGQZe = cms.vstring(
            'Ztautau',
			'TTplusJets', 
			'WplusJets', 
            'gammaPlusJetsSum',
			'qcdSum',
			'Zee'
	)
	TZtGWZeQ = cms.vstring(
			'TTplusJets', 
            'Ztautau',
            'gammaPlusJetsSum',
			'WplusJets', 
			'Zee', 
			'qcdSum'
	)
	TGQZtWZe = cms.vstring(
			'TTplusJets', 
            'gammaPlusJetsSum',
			'qcdSum',
            'Ztautau',
			'WplusJets', 
			'Zee'
	)
	ZtTQGZeW = cms.vstring(
            'Ztautau',
			'TTplusJets', 
			'qcdSum',
            'gammaPlusJetsSum',
			'Zee',
			'WplusJets'
	)
	ZtTGWZeQ = cms.vstring(
            'Ztautau',
			'TTplusJets', 
            'gammaPlusJetsSum',
			'WplusJets',
			'Zee',
			'qcdSum'
	)
	TZtGQZeW = cms.vstring(
			'TTplusJets', 
            'Ztautau',
            'gammaPlusJetsSum',
			'qcdSum',
			'Zee',
			'WplusJets'
	)
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electron_afterElectronEta.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electron_afterElectronEta.plots.processes = ZtTZeWGQ
	
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electron_afterLooseElectronId.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electron_afterLooseElectronId.plots.processes = ZtTZeWGQ
	
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electron_afterElectronEcalIso.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electron_afterElectronEcalIso.plots.processes = ZtTWGQZe
	
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electronTrkIso_afterTauPt.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electronTrkIso_afterTauPt.plots.processes = ZtTZeWGQ
	
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electronTrkIP_afterElectronTrk.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electronTrkIP_afterElectronTrk.plots.processes = ZtTWGQZe
	
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electronEcalIso_afterElectronTrkIso.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electronEcalIso_afterElectronTrkIso.plots.processes = ZtTWGQZe
	
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electronEcalIsoBarrel_afterElectronTrkIso.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electronEcalIsoBarrel_afterElectronTrkIso.plots.processes = ZtTWGQZe
	
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electronEcalIsoEndcap_afterElectronTrkIso.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electronEcalIsoEndcap_afterElectronTrkIso.plots.processes = ZtTWGQZe
	
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electron_afterElectronAntiCrack.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electron_afterElectronAntiCrack.plots.processes = ZtTZeWGQ
	
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electron_afterElectronTrkIP.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electron_afterElectronTrkIP.plots.processes = ZtTWGQZe
	
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electron_afterPrimaryEventVertexPosition.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electron_afterPrimaryEventVertexPosition.plots.processes = ZtTZeWGQ
			
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_tauLeadTrkPt_afterElectronConversionVeto.plots.processes = ZtTQGZeW
	
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_tauLeadTrkPt_afterTauAntiOverlapWithElectronsVeto.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_tauLeadTrkPt_afterTauAntiOverlapWithElectronsVeto.plots.processes = ZtTGWZeQ
	
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_tauLeadTrkPt_afterTauEta.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_tauLeadTrkPt_afterTauEta.plots.processes = ZtTGWZeQ
	
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_tauLeadTrkPt_afterTauLeadTrk.plots.processes = ZtTQGZeW
	
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_tauTrkIso_afterTauLeadTrkPt.plots.processes = ZtTWGQZe
	
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_tau_afterElectronConversionVeto.plots.processes = ZtTQGZeW

	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_tau_afterElectronPt.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_tau_afterElectronPt.plots.processes = ZtTZeWGQ

	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_tau_afterTauAntiOverlapWithElectronsVeto.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_tau_afterTauAntiOverlapWithElectronsVeto.plots.processes = ZtTZeWGQ

	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_tau_afterTauCharge.plots.processes = TGQZtWZe
	
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_tau_afterTauEta.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_tau_afterTauEta.plots.processes = ZtTZeWGQ

	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_vertexChi2Prob_afterPrimaryEventVertex.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_vertexChi2Prob_afterPrimaryEventVertex.plots.processes = ZtTZeWGQ

	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_vertexZ_afterPrimaryEventVertexQuality.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_vertexZ_afterPrimaryEventVertexQuality.plots.processes = ZtTZeWGQ

	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electronSigmaIetaIeta_afterPrimaryEventVertexPosition.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electronSigmaIetaIeta_afterPrimaryEventVertexPosition.plots.processes = ZtTZeWGQ
			
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electronHadronicOverE_afterPrimaryEventVertexPosition.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electronHadronicOverE_afterPrimaryEventVertexPosition.plots.processes = ZtTZeWGQ
			
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electronDeltaPhiSuperClusterTrkAtVertex_afterPrimaryEventVertexPosition.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electronDeltaPhiSuperClusterTrkAtVertex_afterPrimaryEventVertexPosition.plots.processes = ZtTZeWGQ
			
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electronDeltaEtaSuperClusterTrkAtVertex_afterPrimaryEventVertexPosition.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_electronDeltaEtaSuperClusterTrkAtVertex_afterPrimaryEventVertexPosition.plots.processes = ZtTZeWGQ
			
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_tauNumTracksSignalCone_afterTauEcalIso.yAxis = cms.string('numEntries_log')
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_tauNumTracksSignalCone_afterTauEcalIso.plots.processes = ZtQGTZeW
			
	process.plotZtoElecTau.drawJobs.cutFlowControlPlots_tau_afterTauLeadTrk.plots.processes = TZtGQZeW
			
