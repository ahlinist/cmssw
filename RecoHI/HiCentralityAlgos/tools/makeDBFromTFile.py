import FWCore.ParameterSet.VarParsing as VarParsing

ivars = VarParsing.VarParsing('standard')

ivars.register ('outputTag',
                mult=ivars.multiplicity.singleton,
                mytype=ivars.varType.string,
                info="for testing")
ivars.outputTag="demo"

ivars.register ('inputFile',
                mult=ivars.multiplicity.singleton,
                mytype=ivars.varType.string,
                info="for testing")
ivars.input="./input.root"
ivars.output="./output.root"

ivars.parseArguments()

import FWCore.ParameterSet.Config as cms

process = cms.Process('DUMMY')
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))
process.source = cms.Source("EmptyIOVSource",
                            timetype = cms.string("runnumber"),
                            firstValue = cms.uint64(1),
                            lastValue = cms.uint64(1),
                            interval = cms.uint64(1)
                            )

process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.CondDBCommon.connect = "sqlite_file:" + ivars.output

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
                                          process.CondDBCommon,
                                          timetype = cms.untracked.string("runnumber"),
                                          toPut = cms.VPSet(cms.PSet(record = cms.string('HeavyIonRcd'),
                                                                     tag = cms.string(ivars.outputTag)
                                                                     )
                                                            )
                                          )

process.makeCentralityTableDB = cms.EDAnalyzer('CentralityTableProducer',
                                               makeDBFromTFile = cms.untracked.bool(True),
                                               inputTFile = cms.string(ivars.input),
                                               rootTag = cms.string(ivars.outputTag),
                                               nBins = cms.int32(20)
                                               )

process.step  = cms.Path(process.makeCentralityTableDB)
    




