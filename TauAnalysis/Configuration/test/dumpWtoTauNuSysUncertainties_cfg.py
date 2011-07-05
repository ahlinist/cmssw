import FWCore.ParameterSet.Config as cms

import os

process = cms.Process('dumpWtoTauNuSysUncertainties')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(0)
    )

process.source = cms.Source("EmptySource")

inputFilePath = '/afs/cern.ch/user/l/liis/RunSysOK'
inputFileNames = {
    'Wtaunu': 'harvested_WtoTauNu_Wtaunu_RunSysOK.root'
#    'Wtaunu' : 'plotsWtoTauNu_all.root'
 }
dqmDirectory_Wtaunu = '/wTauNuAnalyzer/beforeEvtSelTrigger'

genAccBinner = 'modelBinnerForWTauNuGenTauNuPairAcc'
recEffBinner = 'modelBinnerForWTauNuWrtGenTauNuPairAcc'

process.loadWtoTauNu = cms.EDAnalyzer("DQMFileLoader")

process.dumpWtoTauNuSequence = cms.Sequence(process.loadWtoTauNu)

for sample, inputFileName in inputFileNames.items():

    setattr(process.loadWtoTauNu,sample,cms.PSet(
        inputFileNames = cms.vstring(os.path.join(inputFilePath, inputFileName)),
        dqmDirectory_store = cms.string('/'+ sample + '/')
        ))
   
    dumpWtoTauNuAcceptanceModule = cms.EDAnalyzer("DQMDumpBinningResults",
                                                  binningService = cms.PSet(
        pluginType = cms.string("ModelBinningService"),
        dqmDirectories = cms.PSet(
           genAcc = cms.string('/'+ sample + dqmDirectory_Wtaunu + '/' + genAccBinner),
           recEff = cms.string('/' + sample + dqmDirectory_Wtaunu + '/' + recEffBinner)
           )
        )
                                                  )
    dumpWtoTauNuAcceptanceModuleName = "dumpWtoTauNuAcceptance%s" % sample
    setattr(process, dumpWtoTauNuAcceptanceModuleName, dumpWtoTauNuAcceptanceModule)
    process.dumpWtoTauNuSequence += dumpWtoTauNuAcceptanceModule


    expUncertainty = cms.PSet(
        sysNames = cms.vstring(""),
        sysTitle = cms.string(""),
        sysCentralValue = cms.string("CENTRAL_VALUE"),
        pluginType = cms.string("ModelBinningService")
        )

    dumpWtoTauNuEffUncertaintyModule = cms.EDAnalyzer("DQMDumpSysUncertaintyBinningResults",
                                                      config = cms.VPSet(
        expUncertainty.clone(
          sysNames = cms.vstring(
              "sysTauJetEnUp",
              "sysTauJetEnDown"
              ),
          sysTitle = cms.string("Tau-jet energy scale")
          ),
        expUncertainty.clone(
          sysNames = cms.vstring(
              "sysJetEnUp",
              "sysJetEnDown"
            ),
          sysTitle = cms.string("jet Energy scale")
          )
        ),
                                                      resultTypes = cms.vstring("acceptance"),
                                                      dqmDirectories = cms.PSet(
        Wtaunu = cms.string('/' + sample + dqmDirectory_Wtaunu + '/' + 'sysUncertaintyBinningResults' + '/' + recEffBinner)
        )
                                                      )
    dumpWtoTauNuEffUncertaintyModuleName = "dumpWtoTauNuEffUncertainties%s" % sample
    setattr(process, dumpWtoTauNuEffUncertaintyModuleName, dumpWtoTauNuEffUncertaintyModule)
    process.dumpWtoTauNuSequence += dumpWtoTauNuEffUncertaintyModule
    
process.p = cms.Path(process.dumpWtoTauNuSequence)

