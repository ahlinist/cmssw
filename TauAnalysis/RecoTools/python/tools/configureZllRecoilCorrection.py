import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
# utility functions to apply Z-recoil corrections to MEt
#--------------------------------------------------------------------------------

from TauAnalysis.CandidateTools.tools.composeModuleName import composeModuleName

def configureZllRecoilCorrection(process, diTauProducerModuleName,
                                 ZllRecoilCorrectionType, shiftByUncertainty = 0., label = ""):
    
    #print("<configureZllRecoilCorrection>:")

    process.load("TauAnalysis.RecoTools.recoZllRecoilCorrection_cfi")

    retVal = {}

    patPFMETsZllRecoilCorrectionModule = cms.EDProducer(ZllRecoilCorrectionType,
        process.recoZllRecoilCorrectionParameter,                                         
        src = cms.InputTag(diTauProducerModuleName)
    )
    patPFMETsZllRecoilCorrectionModule.shiftByUncertainty = cms.double(shiftByUncertainty)
    retVal['patPFMETsZllRecoilCorrectionModule'] = patPFMETsZllRecoilCorrectionModule
    patPFMETsZllRecoilCorrectionModuleName = composeModuleName([ "patPFMETsZllRecoilCorrected", diTauProducerModuleName, label ])
    setattr(process, patPFMETsZllRecoilCorrectionModuleName, patPFMETsZllRecoilCorrectionModule)
    retVal['patPFMETsZllRecoilCorrectionModuleName'] = patPFMETsZllRecoilCorrectionModuleName

    diTauProducerModule = getattr(process, diTauProducerModuleName)

    diTauProducerModuleZllRecoilCorrected = diTauProducerModule.clone()
    diTauProducerModuleZllRecoilCorrected.srcMET = \
      cms.InputTag(patPFMETsZllRecoilCorrectionModuleName, 'met')
    diTauProducerModuleZllRecoilCorrected.srcReRecoDiTauObjects = \
      cms.InputTag(diTauProducerModuleName)
    diTauProducerModuleZllRecoilCorrected.srcReRecoDiTauToMEtAssociations = \
      cms.InputTag(patPFMETsZllRecoilCorrectionModuleName, 'diTauToMEtAssociations')
    retVal['diTauProducerModuleZllRecoilCorrected'] = diTauProducerModuleZllRecoilCorrected
    diTauProducerModuleZllRecoilCorrectedName = composeModuleName([ diTauProducerModuleName, "ZllRecoilCorrected", label ])
    setattr(process, diTauProducerModuleZllRecoilCorrectedName, diTauProducerModuleZllRecoilCorrected)
    retVal['diTauProducerModuleZllRecoilCorrectedName'] = diTauProducerModuleZllRecoilCorrectedName

    patPFMETsZllRecoilCorrectionSequence = cms.Sequence(
        patPFMETsZllRecoilCorrectionModule
       * diTauProducerModuleZllRecoilCorrected
    )
    retVal['patPFMETsZllRecoilCorrectionSequence'] = patPFMETsZllRecoilCorrectionSequence
    patPFMETsZllRecoilCorrectionSequenceName = \
      composeModuleName([ "patPFMETsZllRecoilCorrectionSequence", diTauProducerModuleName, label ])
    setattr(process, patPFMETsZllRecoilCorrectionSequenceName, patPFMETsZllRecoilCorrectionSequence)
    retVal['patPFMETsZllRecoilCorrectionSequenceName'] = patPFMETsZllRecoilCorrectionSequenceName

    return retVal
