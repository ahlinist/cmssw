import FWCore.ParameterSet.Config as cms
from GeneratorInterface.ThePEGInterface.herwigDefaults_cff import *

configurationMetadata = cms.untracked.PSet(
	version = cms.untracked.string('$Revision: 1.1 $'),
	name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/GeneratorInterface/ThePEGInterface/test/testThePEGHadronisation.py,v $'),
	annotation = cms.untracked.string('LHE example - ttbar events, MRST2001 used')
)

source = cms.Source("LHESource",
	fileNames = cms.untracked.vstring('file:ttbar.lhe')
)

generator = cms.EDProducer("LHEProducer",
	eventsToPrint = cms.untracked.uint32(1),

	hadronisation = cms.PSet(
		herwigDefaultsBlock,

		generator = cms.string('ThePEG'),

		configFiles = cms.vstring(),
		dumpConfig  = cms.untracked.string(""),
		dumpEvents  = cms.untracked.string(""),

		parameterSets = cms.vstring(
			'pdfCTEQ5L',
			'basicSetup',
			'cm10TeV',
			'setParticlesStableForDetector',
			'lheDefaults', 
			'lheDefaultPDFs'
		)
	)
)

ProducerSourceSequence = cms.Sequence(generator)
