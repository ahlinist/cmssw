import FWCore.ParameterSet.Config as cms
import copy

from RecoJets.Configuration.RecoJets_cff import *

from RecoJets.Configuration.GenJetParticles_cff import *
from RecoJets.Configuration.RecoGenJets_cff import *

from PhysicsTools.PatAlgos.tools.coreTools import *

from TauAnalysis.RecoTools.postPatProduction_cff import produceGenObjects

def switchToData(process):
	
	# correct global conditions tag
	process.GlobalTag.globaltag = 'GR10_P_V4::All'

	# remove MC matching from standard PAT sequences
	removeMCMatching(process, ["All"])

	# remove modules from pre-PAT production running on genParticles
	process.producePrePat.remove(ak5CaloJets)
	process.producePrePat.remove(ak5CaloJetsPUCorr)

	process.producePrePat.remove(genParticlesForJets)
	process.producePrePat.remove(ak5GenJets)

	# remove modules from post-PAT production running on genParticles
	process.producePostPat.remove(produceGenObjects)
