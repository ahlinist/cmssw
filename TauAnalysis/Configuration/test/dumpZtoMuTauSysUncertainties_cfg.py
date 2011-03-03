import FWCore.ParameterSet.Config as cms

import os

#--------------------------------------------------------------------------------
# Print-out systematic uncertainties on Z --> tau+ tau- 
# signal acceptance and efficiency
#--------------------------------------------------------------------------------

process = cms.Process('dumpZtoMuTauSysUncertainties')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

inputFilePath = '/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau/2011Feb23_HPSloose/'
inputFileNames = {
    'ZtautauPU156bx'          : 'harvested_ZtoMuTau_ZtautauPU156bx_2011Feb23_HPSloose.root',
    'ZtautauPU156bx_pythiaZ2' : 'harvested_ZtoMuTau_ZtautauPU156bx_pythiaZ2_2011Feb23_HPSloose.root',
    'Ztautau_powhegZ2'        : 'harvested_ZtoMuTau_Ztautau_powhegZ2_2011Feb23_HPSloose.root',
    'Ztautau_powheg'          : 'harvested_ZtoMuTau_Ztautau_powheg_2011Feb23_HPSloose.root'
}    

dqmDirectory_Ztautau = '/zMuTauAnalyzerOS/afterGenPhaseSpaceCut_beforeEvtSelTrigger'

genAccBinner = 'modelBinnerForMuTauGenTauLeptonPairAcc'
recEffBinner = 'modelBinnerForMuTauWrtGenTauLeptonPairAcc'

genAccBinner3mZbins = 'modelBinnerForMuTauGenTauLeptonPairAcc3mZbins'
recEffBinner3mZbins = 'modelBinnerForMuTauWrtGenTauLeptonPairAcc3mZbins'

process.loadZtoMuTau = cms.EDAnalyzer("DQMFileLoader")

process.dumpZtoMuTauSequence = cms.Sequence(process.loadZtoMuTau)

for sample, inputFileName in inputFileNames.items():

    setattr(process.loadZtoMuTau, sample, cms.PSet(
        inputFileNames = cms.vstring(os.path.join(inputFilePath, inputFileName)),
        dqmDirectory_store = cms.string('/' + sample + '/')
    ))

    dumpZtoMuTauAcceptanceModule = cms.EDAnalyzer("DQMDumpBinningResults",
        binningService = cms.PSet(
            pluginType = cms.string("ModelBinningService"),
            dqmDirectories = cms.PSet(
                genAcc = cms.string('/' + sample + dqmDirectory_Ztautau + '/' + genAccBinner),
                recEff = cms.string('/' + sample + dqmDirectory_Ztautau + '/' + recEffBinner)
            )
        )
    )
    dumpZtoMuTauAcceptanceModuleName = "dumpZtoMuTauAcceptance%s" % sample
    setattr(process, dumpZtoMuTauAcceptanceModuleName, dumpZtoMuTauAcceptanceModule)
    process.dumpZtoMuTauSequence += dumpZtoMuTauAcceptanceModule

    dumpZtoMuTauAcceptance3mZbinsModule = cms.EDAnalyzer("DQMDumpBinningResults",
        binningService = cms.PSet(
            pluginType = cms.string("ModelBinningService"),
            dqmDirectories = cms.PSet(
                genAcc = cms.string('/' + sample + dqmDirectory_Ztautau + '/' + genAccBinner3mZbins),
                recEff = cms.string('/' + sample + dqmDirectory_Ztautau + '/' + recEffBinner3mZbins)
            )
        )
    )
    dumpZtoMuTauAcceptance3mZbinsModuleName = "dumpZtoMuTauAcceptance3mZbins%s" % sample
    setattr(process, dumpZtoMuTauAcceptance3mZbinsModuleName, dumpZtoMuTauAcceptance3mZbinsModule)
    process.dumpZtoMuTauSequence += dumpZtoMuTauAcceptance3mZbinsModule

    theoryUncertainty = cms.PSet(
        sysNames = cms.vstring(""),
        sysTitle = cms.string(""),
        sysCentralValue = cms.string("CENTRAL_VALUE"),
        pluginType = cms.string("ModelBinningService"),
        method = cms.string("simple")
    )

    dumpZtoMuTauAccUncertaintyModule = cms.EDAnalyzer("DQMDumpSysUncertaintyBinningResults",
        config = cms.VPSet(
            cms.PSet(
                pdfSets = cms.VPSet(
                    cms.PSet(
                        sysNames = cms.vstring("sysPdfWeightsCTEQ66(45)"),
                        sysCentralValues = cms.vstring("sysPdfWeightsCTEQ66(0)")
                    ),
                    cms.PSet(
                        sysNames = cms.vstring("sysPdfWeightsMSTW2008nlo68cl(41)"),
                        sysCentralValues = cms.vstring("sysPdfWeightsMSTW2008nlo68cl(0)")
                    ),
                    cms.PSet(
                        sysNames = cms.vstring("sysPdfWeightsNNPDF20(100)"),
                        sysCentralValues = cms.vstring("sysPdfWeightsNNPDF20(0)", "sysPdfWeightsNNPDF20(1)")
                    )
                ),
                sysTitle = cms.string("PDF"),
                pluginType = cms.string("ModelBinningService"),                                               
                method = cms.string("pdf")
            ),
            theoryUncertainty.clone(
                sysNames = cms.vstring("sysIsrWeight"),
                sysTitle = cms.string("ISR")
            ),
            theoryUncertainty.clone(            
                sysNames = cms.vstring("sysFsrWeight"),
                sysTitle = cms.string("FSR")
            )
        ),
        resultTypes = cms.vstring("acceptance"),  
        dqmDirectories = cms.PSet(
            Ztautau = cms.string('/' + sample + dqmDirectory_Ztautau + '/' + 'sysUncertaintyBinningResults' + '/' + genAccBinner)
        )
    )
    dumpZtoMuTauAccUncertaintyModuleName = "dumpZtoMuTauAccUncertainties%s" % sample
    setattr(process, dumpZtoMuTauAccUncertaintyModuleName, dumpZtoMuTauAccUncertaintyModule)
    ##process.dumpZtoMuTauSequence += dumpZtoMuTauAccUncertaintyModule

    expUncertainty = cms.PSet(
        sysNames = cms.vstring(""),
        sysTitle = cms.string(""),
        sysCentralValue = cms.string("CENTRAL_VALUE"),
        pluginType = cms.string("ModelBinningService")
    )

    dumpZtoMuTauEffUncertaintyModule = cms.EDAnalyzer("DQMDumpSysUncertaintyBinningResults",
        config = cms.VPSet(
            expUncertainty.clone(
                sysNames = cms.vstring(
                    "sysMuonPtUp",
                    "sysMuonPtDown"
                ),
                sysTitle = cms.string("Muon Momentum scale")
            ),
            expUncertainty.clone(
                sysNames = cms.vstring(
                    "sysTauJetEnUp",
                    "sysTauJetEnDown"
                ),
                sysTitle = cms.string("Tau-jet Energy scale")
            ),
        	expUncertainty.clone(
                sysNames = cms.vstring(
                    "sysJetEnUp",
                    "sysJetEnDown"
                ),
                sysTitle = cms.string("Jet Energy scale")
            ),
            expUncertainty.clone(
                sysNames = cms.vstring(
                    "sysZllRecoilCorrectionUp",
                    "sysZllRecoilCorrectionDown"
                ),
                sysTitle = cms.string("MEt Z-recoil correction")
            )
        ),
        resultTypes = cms.vstring("acceptance"),                                                  
        dqmDirectories = cms.PSet(
            Ztautau = cms.string('/' + sample + dqmDirectory_Ztautau + '/' + 'sysUncertaintyBinningResults' + '/' + recEffBinner)
        )
    )
    dumpZtoMuTauEffUncertaintyModuleName = "dumpZtoMuTauEffUncertainties%s" % sample
    setattr(process, dumpZtoMuTauEffUncertaintyModuleName, dumpZtoMuTauEffUncertaintyModule)
    ##process.dumpZtoMuTauSequence += dumpZtoMuTauEffUncertaintyModule

process.p = cms.Path(process.dumpZtoMuTauSequence)

# print-out all python configuration parameter information
print process.dumpPython()

