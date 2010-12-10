import FWCore.ParameterSet.Config as cms
import copy

from RecoJets.Configuration.RecoJets_cff import *

from RecoJets.Configuration.GenJetParticles_cff import *
from RecoJets.Configuration.RecoGenJets_cff import *

from PhysicsTools.PatAlgos.tools.coreTools import *
import PhysicsTools.PatAlgos.tools.helpers as patutils

from TauAnalysis.RecoTools.postPatProduction_cff import produceGenObjects

from TauAnalysis.Configuration.tools.analysisSequenceTools import removeAnalyzer

def _setattr_ifexists(obj, attrName, attrValue):
	if hasattr(obj, attrName):
		setattr(obj, attrName, attrValue)

def switchToData(process):

	# remove MC matching from standard PAT sequences
	removeMCMatching(process, ["All"], outputInProcess = False)
	process.patDefaultSequence.remove(process.patJetPartonMatch)

	# add data-quality cuts which work on "real" data only
	if hasattr(process, "dataQualityFilters"):
		process.dataQualityFilters._seq = process.dataQualityFilters._seq * process.hltPhysicsDeclared
		process.dataQualityFilters._seq = process.dataQualityFilters._seq * process.dcsstatus

	if hasattr(process, "patPFMETs"):
		process.patPFMETs.addGenMET = cms.bool(False)

	# remove modules from pre-PAT production running on genParticles
	if hasattr(process, "producePrePat"):
		process.producePrePat.remove(genParticlesForJets)
		process.producePrePat.remove(ak5GenJets)

	# remove modules from post-PAT production running on genParticles
	if hasattr(process, "producePostPat"):
		process.producePostPat.remove(produceGenObjects)

	# iterate over all sequences attached to process object and
	# recursively disable gen. matching for all modules in sequence
	for processAttrName in dir(process):
            processAttr = getattr(process, processAttrName)
	    if isinstance(processAttr, cms.Sequence):
	        print "--> Disabling gen. Matching for sequence:", processAttrName
	        # replace all InputTags referring to
		#  o genParticleSource = cms.InputTag('genParticles') --> ""
   	        #  o genJetSource = cms.InputTag('iterativeCone5GenJets') --> ""
		#  o genJetSource = cms.InputTag('ak5GenJets') --> ""
		#  o genTauJetSource = cms.InputTag('tauGenJets') --> ""
		#  o genMEtSource = cms.InputTag('genMetTrue') --> ""
		patutils.massSearchReplaceAnyInputTag(processAttr, cms.InputTag('genParticles'), cms.InputTag(''))
		patutils.massSearchReplaceAnyInputTag(processAttr, cms.InputTag('iterativeCone5GenJets'), cms.InputTag(''))
		patutils.massSearchReplaceAnyInputTag(processAttr, cms.InputTag('ak5GenJets'), cms.InputTag(''))
		patutils.massSearchReplaceAnyInputTag(processAttr, cms.InputTag('tauGenJets'), cms.InputTag(''))
		patutils.massSearchReplaceAnyInputTag(processAttr, cms.InputTag('genMetTrue'), cms.InputTag(''))

		# replace all InputTags referring to gen. Z decay products
		# in electron + tau-jet Zee (muon + tau-jet Zmumu) hypotheses
		patutils.massSearchReplaceAnyInputTag(processAttr, cms.InputTag('genElectronsFromZs'), cms.InputTag(''))
		patutils.massSearchReplaceAnyInputTag(processAttr, cms.InputTag('genMuonsFromZs'), cms.InputTag(''))

        # disable access to generator level information in all
	# histogram managers, binner and event-dump plugins of GenericAnalyzer module
	for processAttrName in dir(process):
	    processAttr = getattr(process, processAttrName)
            if isinstance(processAttr, cms.EDAnalyzer):
	        if processAttr.type_() == "GenericAnalyzer":
		    # disable matching between gen. and rec. particles
		    # in all histogram managers and binner plugins by setting
		    #  o genParticleSource = cms.InputTag('genParticles') --> ""
		    #  o genJetSource = cms.InputTag('iterativeCone5GenJets') --> ""
		    #  o genJetSource = cms.InputTag('ak5GenJets') --> ""
		    #  o genTauJetSource = cms.InputTag('tauGenJets') --> ""
		    #  o genMEtSource = cms.InputTag('genMetTrue') --> ""
		    if hasattr(processAttr, "analyzers"):
		        analyzerPlugins = getattr(processAttr, "analyzers")
			for analyzerPlugin in analyzerPlugins:
			    _setattr_ifexists(analyzerPlugin, "genParticleSource", cms.InputTag(''))
			    _setattr_ifexists(analyzerPlugin, "genJetSource", cms.InputTag(''))
			    _setattr_ifexists(analyzerPlugin, "genTauJetSource", cms.InputTag(''))
			    _setattr_ifexists(analyzerPlugin, "genMEtSource", cms.InputTag(''))

                    # remove from all analysis sequences the following objects:
		    #  o genPhaseSpaceInfoHistManager
		    #  o modelBinnerForMuTauGenTauLeptonPairAcc
		    #  o modelBinnerForMuTauWrtGenTauLeptonPairAcc
		    #  o modelBinnerForMuTauCentralJetVetoWrtGenTauLeptonPairAcc
		    #  o modelBinnerForMuTauCentralJetBtagWrtGenTauLeptonPairAcc
		    #  o sysUncertaintyBinnerForMuTau
		    #  o diTauCandidateCollinearApproxHistManagerBinnedForMuTau
		    analyzerPluginsToRemove = [
		       "genPhaseSpaceEventInfoHistManager",
		       "modelBinnerForMuTauGenTauLeptonPairAcc",
		       "modelBinnerForMuTauWrtGenTauLeptonPairAcc",
		       "modelBinnerForMuTauCentralJetVetoWrtGenTauLeptonPairAcc",
		       "modelBinnerForMuTauCentralJetBtagWrtGenTauLeptonPairAcc",
		       "sysUncertaintyBinnerForMuTau",
		       "diTauCandidateCollinearApproxHistManagerBinnedForMuTau"
		    ]
		    for analyzerPluginName in analyzerPluginsToRemove:
  		        if hasattr(process, analyzerPluginName):
			    analyzerPlugin = getattr(process, analyzerPluginName)
			    if processAttr.analyzers.count(analyzerPlugin) > 0:
			        processAttr.analyzers.remove(analyzerPlugin)
			    removeAnalyzer(processAttr.analysisSequence, analyzerPluginName)

	            # disable matching between gen. and rec. particles in all event-dump plugins by setting
		    #  o doGenInfo = cms.bool(True) --> cms.bool(False)
		    #  o genParticleSource = cms.InputTag('genParticles') --> ""
		    if hasattr(processAttr, "eventDumps"):
		        eventDumpPlugins = getattr(processAttr, "eventDumps")
			for eventDumpPlugin in eventDumpPlugins:
			    eventDumpPlugin.doGenInfo = cms.bool(False)
			    eventDumpPlugin.genParticleSource = cms.InputTag('')

		    # disable estimation of systematic uncertainties by setting
		    #  o analyzers_systematic = cms.VPSet(..) --> cms.VPSet()
		    processAttr.analyzers.analyzers_systematic = cms.VPSet()
