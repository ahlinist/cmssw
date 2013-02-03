import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.helpers import *

def addPFMet(process, correct = False):    
    process.load("JetMETCorrections.Type1MET.MetType1Corrections_cff")    
    process.metJESCorAK5PFJet.jetPTthreshold = cms.double(10.0)
    process.metJESCorAK5PFJet.useTypeII = cms.bool(True)

    process.patPFMETs = process.patMETs.clone()
    process.patPFMETs.addMuonCorrections = False
    process.patPFMETs.genMETSource = cms.InputTag('genMetTrue')

    if correct:
        process.patPFMETs.metSource = cms.InputTag('metJESCorAK5PFJet')
        process.makePatPFMETs = cms.Sequence(process.metJESCorAK5PFJet * process.patPFMETs)
    else:
        process.patPFMETs.metSource = cms.InputTag('pfMet')
        process.makePatPFMETs = cms.Sequence(process.patPFMETs)

    process.makePatMETs += process.makePatPFMETs
    
    return process.makePatMETs

def addCorrectedPFMet(process, isMC, doApplyType0corr, doApplySysShiftCorr, runPeriod, doSmearJets):

    process.load("PhysicsTools.PatUtils.patPFMETCorrections_cff")
    
    process.load("JetMETCorrections.METPUSubtraction.mvaPFMET_cff")
    if isMC:
        process.calibratedAK5PFJetsForPFMEtMVA.correctors = cms.vstring("ak5PFL1FastL2L3")
    else:
        process.calibratedAK5PFJetsForPFMEtMVA.correctors = cms.vstring("ak5PFL1FastL2L3Residual")
    process.pfMEtMVA.srcCorrJets = cms.InputTag('calibratedAK5PFJetsForPFMEtMVA')
    process.pfMEtMVA.srcLeptons = cms.VInputTag('goodMuons')
    process.pfMEtMVA.verbosity = cms.int32(0)
    process.patPFMetMVA = process.patMETs.clone(
        metSource = cms.InputTag('pfMEtMVA'),
        addMuonCorrections = cms.bool(False),
        genMETSource = cms.InputTag('genMetTrue')
    )
    process.patMEtMVAsequence = cms.Sequence(process.pfMEtMVAsequence + process.patPFMetMVA)

    process.load("JetMETCorrections.METPUSubtraction.noPileUpPFMET_cff")
    process.load("JetMETCorrections.METPUSubtraction.noPileUpPFchsMET_cff")
    if isMC:
        process.calibratedAK5PFJetsForNoPileUpPFMEt.correctors = cms.vstring("ak5PFL1FastL2L3")
        process.calibratedAK5PFchsJetsForNoPileUpPFchsMEt.correctors = cms.vstring("ak5PFchsL1FastL2L3")
    else:
        process.calibratedAK5PFJetsForNoPileUpPFMEt.correctors = cms.vstring("ak5PFL1FastL2L3Residual")
        process.calibratedAK5PFchsJetsForNoPileUpPFchsMEt.correctors = cms.vstring("ak5PFchsL1FastL2L3Residual")

    process.noPileUpPFMEt.srcLeptons = cms.VInputTag('patMuons')
    process.noPileUpPFMEtData.verbosity = cms.int32(0)
    process.noPileUpPFMEt.verbosity = cms.int32(0)
    process.patPFMetNoPileUp = process.patMETs.clone(
        metSource = cms.InputTag('noPileUpPFMEt'),
        addMuonCorrections = cms.bool(False),
        genMETSource = cms.InputTag('genMetTrue')
    )
    process.noPileUpPFchsMEt.srcLeptons = cms.VInputTag('patMuons')
    process.noPileUpPFchsMEtData.verbosity = cms.int32(0)
    process.noPileUpPFchsMEt.verbosity = cms.int32(0)
    process.patPFchsMetNoPileUp = process.patMETs.clone(
        metSource = cms.InputTag('noPileUpPFchsMEt'),
        addMuonCorrections = cms.bool(False),
        genMETSource = cms.InputTag('genMetTrue')
    )
    process.patMEtNoPileUpSequence = cms.Sequence(process.noPileUpPFMEtSequence + process.patPFMetNoPileUp + process.patPFchsMetNoPileUp)
    
    process.makeCorrectedPatMETs = cms.Sequence()

    if isMC:
        import PhysicsTools.PatAlgos.tools.helpers as configtools
        process.type0PFMEtCorrection.remove(process.type0PFMEtCorrectionPFCandToVertexAssociation)
        process.makeCorrectedPatMETs += process.type0PFMEtCorrectionPFCandToVertexAssociation
        configtools.cloneProcessingSnippet(process, process.producePatPFMETCorrections, "NoSmearing")
        process.selectedPatJetsForMETtype1p2CorrNoSmearing.src = cms.InputTag('patJetsNotOverlappingWithLeptonsForMEtUncertainty')
        process.selectedPatJetsForMETtype2CorrNoSmearing.src = process.selectedPatJetsForMETtype1p2CorrNoSmearing.src
        configtools.cloneProcessingSnippet(process, process.patMEtMVAsequence, "NoSmearing")
        process.patMEtNoPileUpSequence.remove(process.type0PFMEtCorrection)
        configtools.cloneProcessingSnippet(process, process.patMEtNoPileUpSequence, "NoSmearing")
    else:
        doSmearJets = False
        
    sysShiftCorrParameter = None
    if doApplySysShiftCorr:
        process.load("JetMETCorrections.Type1MET.pfMETsysShiftCorrections_cfi")
        if runPeriod == "2012RunABC":
            if isMC:
                sysShiftCorrParameter = process.pfMEtSysShiftCorrParameters_2012runABCvsNvtx_mc
            else:
                sysShiftCorrParameter = process.pfMEtSysShiftCorrParameters_2012runABCvsNvtx_data
        else:
            raise ValueError("Invalid runPeriod = %s !!" % runPeriod)

    from PhysicsTools.PatUtils.tools.runType1PFMEtUncertainties import runType1PFMEtUncertainties
    runType1PFMEtUncertainties(
        process,
        electronCollection = '',
        photonCollection = '',
        muonCollection = cms.InputTag('patMuons'),
        tauCollection = '',
        jetCollection = cms.InputTag('patJets'),        
        doSmearJets = doSmearJets,
        makeType1corrPFMEt = True,
        makeType1p2corrPFMEt = True,
        doApplyType0corr = doApplyType0corr,
        sysShiftCorrParameter = sysShiftCorrParameter,
        doApplySysShiftCorr = doApplySysShiftCorr,
        addToPatDefaultSequence = False
    )
    from PhysicsTools.PatUtils.tools.runType1PFMEtUncertainties import runMVAMEtUncertainties
    runMVAMEtUncertainties(
        process,
        electronCollection = '',
        photonCollection = '',
        muonCollection = cms.InputTag('patMuons'),
        tauCollection = '',
        doSmearJets = doSmearJets,
        addToPatDefaultSequence = False
    )
    from PhysicsTools.PatUtils.tools.runNoPileUpMEtUncertainties import runNoPileUpMEtUncertainties
    runNoPileUpMEtUncertainties(
        process,
        electronCollection = '',
        photonCollection = '',
        muonCollection = cms.InputTag('patMuons'),
        tauCollection = '',
        doApplyChargedHadronSubtraction = False,
        doSmearJets = doSmearJets,
        addToPatDefaultSequence = False
    )
    runNoPileUpMEtUncertainties(
        process,
        electronCollection = '',
        photonCollection = '',
        muonCollection = cms.InputTag('patMuons'),
        tauCollection = '',
        doApplyChargedHadronSubtraction = True,
        doSmearJets = doSmearJets,
        addToPatDefaultSequence = False
    )

    if isMC:
        process.patPFMet.addGenMET = cms.bool(True)
        process.patPFMetMVA.addGenMET = cms.bool(True)
        process.patPFJetMETtype1p2Corr.jetCorrLabel = cms.string("L3Absolute")
        process.patPFJetMETtype1p2CorrNoSmearing.jetCorrLabel = cms.string("L3Absolute")
    
        process.makeCorrectedPatMETs += process.metUncertaintySequence
        process.makeCorrectedPatMETs += process.producePatPFMETCorrectionsNoSmearing
        process.makeCorrectedPatMETs += process.patMEtMVAsequenceNoSmearing
        process.makeCorrectedPatMETs += process.patMEtNoPileUpSequenceNoSmearing
    else:
        process.patPFMet.addGenMET = cms.bool(False)
        process.patPFMetMVA.addGenMET = cms.bool(False)
        process.patPFJetMETtype1p2Corr.jetCorrLabel = cms.string("L2L3Residual")
    
        process.makeCorrectedPatMETs += process.patJetsNotOverlappingWithLeptonsForMEtUncertainty
        if hasattr(process, "pfMEtSysShiftCorrSequence"):
            process.makeCorrectedPatMETs += process.pfMEtSysShiftCorrSequence
        process.makeCorrectedPatMETs += process.producePatPFMETCorrections
        process.makeCorrectedPatMETs += process.patMEtMVAsequence
        process.makeCorrectedPatMETs += process.patMEtNoPileUpSequence

    # add MVA MEt with unity response training
    for moduleName in dir(process):
        if (moduleName.endswith("Up") or moduleName.endswith("Down")) and not isMC:
            continue
        module = getattr(process, moduleName)
        if isinstance(module, cms.EDProducer) and module.type_() == "PFMETProducerMVA":
            module_unity = module.clone(
                inputFileNames = cms.PSet(
                    DPhi = cms.FileInPath('JetMETCorrections/METPUSubtraction/data/gbrmetphi_53.root'), # CV: same for unity and non-unity response training
                    CovU2 = cms.FileInPath('JetMETCorrections/METPUSubtraction/data/gbru2cov_53_UnityResponse.root'),
                    U = cms.FileInPath('JetMETCorrections/METPUSubtraction/data/gbrmet_53_UnityResponse.root'),
                    CovU1 = cms.FileInPath('JetMETCorrections/METPUSubtraction/data/gbru1cov_53_UnityResponse.root')
                )
            )
            moduleName_unity = moduleName.replace("pfMEtMVA", "pfMEtMVAunityResponse")
            setattr(process, moduleName_unity, module_unity)
            process.makeCorrectedPatMETs += module_unity    
    for moduleName in dir(process):
        if (moduleName.endswith("Up") or moduleName.endswith("Down")) and not isMC:
            continue
        module = getattr(process, moduleName)
        if isinstance(module, cms.EDProducer) and module.type_() == "PATMETProducer" and moduleName.find("patPFMetMVA") != -1:
            module_unity = module.clone(
                metSource = cms.InputTag(module.metSource.value().replace("pfMEtMVA", "pfMEtMVAunityResponse"))
            )
            moduleName_unity = moduleName.replace("patPFMetMVA", "patPFMetMVAunityResponse")
            setattr(process, moduleName_unity, module_unity)
            process.makeCorrectedPatMETs += module_unity

    # add No-PU MEt without L1Fastjet jet energy corrections
    for moduleName in dir(process):
        if ((moduleName.endswith("Up") or moduleName.endswith("Down")) and not moduleName.endswith("NoPileUp")) and not isMC:
            continue
        module = getattr(process, moduleName)
        if isinstance(module, cms.EDProducer) and module.type_() == "NoPileUpPFMEtDataProducer":
            moduleName_calib_or_corrJets = module.srcJets.value()
            module_calib_or_corrJets = getattr(process, moduleName_calib_or_corrJets)
            moduleName_calibJets = None
            moduleName_corrJets = None
            moduleName_smearedJets = None
            ##print "%s: %s" % (moduleName_calib_or_corrJets, module_calib_or_corrJets.type_())
            if module_calib_or_corrJets.type_() == "ShiftedPFJetProducer":
                moduleName_corrJets = moduleName_calib_or_corrJets
                module_corrJets = getattr(process, moduleName_corrJets)
                moduleName_calibJets = module_corrJets.src.value()
            elif module_calib_or_corrJets.type_() == "PFJetCorrectionProducer":
                moduleName_calibJets = moduleName_calib_or_corrJets
            elif module_calib_or_corrJets.type_() == "SmearedPFJetProducer":
                moduleName_smearedJets = moduleName_calib_or_corrJets
                module_smearedJets = getattr(process, moduleName_smearedJets)
                moduleName_calibJets = module_smearedJets.src.value()
            else:
                raise ValueError("Module = %s is of unsupported type = %s !!" % (moduleName_calib_or_corrJets, module_calib_or_corrJets.type_()))
            if moduleName_calibJets:
                module_calibJets = getattr(process, moduleName_calibJets)
                module_calibJets_woL1FastJet = module_calibJets.clone(
                    correctors = cms.vstring(module_calibJets.correctors[0].replace("L1Fast", ""))
                )
                moduleName_calibJets_woL1FastJet = moduleName_calibJets.replace("ForNoPileUpPFMEt", "ForNoPileUpPFMEtWithoutL1FastJet")
                setattr(process, moduleName_calibJets_woL1FastJet, module_calibJets_woL1FastJet)
                ##print "adding module %s: %s" % (moduleName_calibJets_woL1FastJet, module_calibJets_woL1FastJet.type_())
                process.makeCorrectedPatMETs += module_calibJets_woL1FastJet
            if moduleName_corrJets:
                module_corrJets = getattr(process, moduleName_corrJets)
                module_corrJets_woL1FastJet = module_corrJets.clone(
                    src = cms.InputTag(module_corrJets.src.value().replace("ForNoPileUpPFMEt", "ForNoPileUpPFMEtWithoutL1FastJet")),
                    jetCorrLabelUpToL3Res = cms.string(module_corrJets.jetCorrLabelUpToL3Res.value().replace("L1Fast", "")),
                    jetCorrLabelUpToL3 = cms.string(module_corrJets.jetCorrLabelUpToL3.value().replace("L1Fast", ""))
                )
                moduleName_corrJets_woL1FastJet = moduleName_corrJets.replace("ForNoPileUpPFMEt", "ForNoPileUpPFMEtWithoutL1FastJet")
                setattr(process, moduleName_corrJets_woL1FastJet, module_corrJets_woL1FastJet)
                ##print "adding module %s: %s" % (moduleName_corrJets_woL1FastJet, module_corrJets_woL1FastJet.type_())
                process.makeCorrectedPatMETs += module_corrJets_woL1FastJet
            if moduleName_smearedJets:
                module_smearedJets = getattr(process, moduleName_smearedJets)
                module_smearedJets_woL1FastJet = module_smearedJets.clone(
                    src = cms.InputTag(module_smearedJets.src.value().replace("ForNoPileUpPFMEt", "ForNoPileUpPFMEtWithoutL1FastJet")),
                    jetCorrLabel = cms.string('ak5PFL1Fastjet')
                )
                moduleName_smearedJets_woL1FastJet = moduleName_smearedJets.replace("ForNoPileUpPFMEt", "ForNoPileUpPFMEtWithoutL1FastJet")
                setattr(process, moduleName_smearedJets_woL1FastJet, module_smearedJets_woL1FastJet)
                ##print "adding module %s: %s" % (moduleName_smearedJets_woL1FastJet, module_smearedJets_woL1FastJet.type_())
                process.makeCorrectedPatMETs += module_smearedJets_woL1FastJet
            moduleName_jetId = module.srcJetIds.getModuleLabel()
            module_jetId = getattr(process, moduleName_jetId)
            module_jetId_woL1FastJet = module_jetId.clone(
                jets = cms.InputTag(module_jetId.jets.value().replace("ForNoPileUpPFMEt", "ForNoPileUpPFMEtWithoutL1FastJet"))
            )
            moduleName_jetId_woL1FastJet = moduleName_jetId.replace("ForNoPileUpPFMEt", "ForNoPileUpPFMEtWithoutL1FastJet")
            setattr(process, moduleName_jetId_woL1FastJet, module_jetId_woL1FastJet)
            ##print "adding module %s: %s" % (moduleName_jetId_woL1FastJet, module_jetId_woL1FastJet.type_())
            process.makeCorrectedPatMETs += module_jetId_woL1FastJet            
            module_woL1FastJet = module.clone(
                jetEnOffsetCorrLabel = cms.string(""),
                srcJets = cms.InputTag(module.srcJets.value().replace("ForNoPileUpPFMEt", "ForNoPileUpPFMEtWithoutL1FastJet")),
                srcJetIds = cms.InputTag(module.srcJetIds.value().replace("ForNoPileUpPFMEt", "ForNoPileUpPFMEtWithoutL1FastJet"))
            )
            moduleName_woL1FastJet = moduleName.replace("noPileUpPFMEtData", "noPileUpPFMEtDataWithoutL1FastJet")
            setattr(process, moduleName_woL1FastJet, module_woL1FastJet)
            process.makeCorrectedPatMETs += module_woL1FastJet
    for moduleName in dir(process):
        if ((moduleName.endswith("Up") or moduleName.endswith("Down")) and not moduleName.endswith("NoPileUp")) and not isMC:
            continue
        module = getattr(process, moduleName)
        if isinstance(module, cms.EDProducer) and module.type_() == "NoPileUpPFMEtProducer":
            module_woL1FastJet = module.clone(
                srcMVAMEtData = cms.InputTag(module.srcMVAMEtData.value().replace("noPileUpPFMEtData", "noPileUpPFMEtDataWithoutL1FastJet"))
            )
            moduleName_woL1FastJet = moduleName.replace("noPileUpPFMEt", "noPileUpPFMEtWithoutL1FastJet")
            setattr(process, moduleName_woL1FastJet, module_woL1FastJet)
            process.makeCorrectedPatMETs += module_woL1FastJet
    for moduleName in dir(process):
        if ((moduleName.endswith("Up") or moduleName.endswith("Down")) and not moduleName.endswith("NoPileUp")) and not isMC:
            continue
        module = getattr(process, moduleName)
        if isinstance(module, cms.EDProducer) and module.type_() == "PATMETProducer" and moduleName.find("patPFMetNoPileUp") != -1:
            module_woL1FastJet = module.clone(
                metSource = cms.InputTag(module.metSource.value().replace("noPileUpPFMEt", "noPileUpPFMEtWithoutL1FastJet"))
            )
            moduleName_woL1FastJet = moduleName.replace("patPFMetNoPileUp", "patPFMetNoPileUpWithoutL1FastJet")
            setattr(process, moduleName_woL1FastJet, module_woL1FastJet)
            process.makeCorrectedPatMETs += module_woL1FastJet

    return process.makeCorrectedPatMETs

def addTCMet(process):
    process.layer1TCMETs = process.patMETs.clone()
    process.layer1TCMETs.addMuonCorrections = False
    process.layer1TCMETs.metSource = cms.InputTag('tcMet')
    process.layer1TCMETs.genMETSource = cms.InputTag('genMETWithMu')
    process.patCandidates.replace(process.patMETs,
                                  process.patMETs + process.layer1TCMETs)
    
def replaceMETforDiTaus(process,
                        oldMet = cms.InputTag('patMETs'),
                        newMet = cms.InputTag('patPFMETs') ):
    massSearchReplaceParam(process.produceDiTauPairsAllKinds,
                           'srcMET', oldMet, newMet)

def replaceMETforTauNu(process,
                       oldMet = cms.InputTag('patMETs'),
                       newMet = cms.InputTag('patPFMETs') ):
    massSearchReplaceParam(process.produceTauNuPairs,
                           'srcMET', oldMet, newMet)
def replaceMETforMet(process,
                     oldMet = cms.InputTag('Layer1METs'),
                     newMet = cms.InputTag('patPFMETs')):
    massSearchReplaceParam(process.selectLayer1METs,
                           'src', oldMet ,newMet)


