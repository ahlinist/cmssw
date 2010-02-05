import FWCore.ParameterSet.Config as cms
import sys
import copy

class sysProdConfigurator(cms._ParameterTypeBase):
    def __init__(self,pyModuleName,pyNameSpace):
        self.pyModuleName = pyModuleName,
        self.pyNameSpace = pyNameSpace 
        self.sequence = None

    def createSequence(self,modulePrefix,moduleType,src,energyScaleShift,energyScaleSmearing,ptShift,ptSmearing,etaShift,etaSmearing,phiShift,phiSmearing):
            module =cms.EDProducer(moduleType)
            module.src = cms.InputTag(src)
            module.fileName = cms.string('')
            module.smearMCParticle = cms.bool(False)
            module.smearFromPtHistogram = cms.bool(False)
            module.smearFromEtaHistogram = cms.bool(False)
            module.ptHistogram = cms.string('')
            module.etaHistogram = cms.string('')
            module.deltaEta = cms.double(0)
            module.deltaPt = cms.double(0)
            module.deltaPhi = cms.double(0)
            module.energyScale = cms.double(1.0)
            module.gaussianSmearingSigmaPt = cms.double(ptSmearing)
            module.gaussianSmearingSigmaEta = cms.double(etaSmearing)
            module.gaussianSmearingSigmaPhi = cms.double(phiSmearing)
            module.gaussianSmearingEScale   = cms.double(energyScaleSmearing)
            module.setLabel(modulePrefix)


            energyScaleValues = [1.0-energyScaleShift, 1.0 ,1.0+energyScaleShift]
            energyScaleLabels = ['EScaleDown' ,'','EScaleUp']

            for label,eScale in zip(energyScaleLabels,energyScaleValues):
                m =module.clone()
                m.src = cms.InputTag(src)
                m.fileName = cms.string('')
                m.smearMCParticle = cms.bool(False)
                m.smearFromPtHistogram = cms.bool(False)
                m.smearFromEtaHistogram = cms.bool(False)
                m.ptHistogram = cms.string('')
                m.etaHistogram = cms.string('')
                m.deltaEta = cms.double(0)
                m.deltaPt = cms.double(0)
                m.deltaPhi = cms.double(0)
                m.energyScale = cms.double(eScale)
                m.gaussianSmearingSigmaPt = cms.double(ptSmearing)
                m.gaussianSmearingSigmaEta = cms.double(etaSmearing)
                m.gaussianSmearingSigmaPhi = cms.double(phiSmearing)
                m.gaussianSmearingEScale   = cms.double(energyScaleSmearing)
                m.setLabel(modulePrefix+label)
              #Register the filter in the namespace
                pyModule = sys.modules[self.pyModuleName[0]]
                if pyModule is None:
                   raise ValueError("'pyModuleName' Parameter invalid")
                setattr(pyModule,modulePrefix+label,m)
                if self.sequence == None:
                   self.sequence=m
                else:
                  self.sequence*=m
                  
            return cms.Sequence(self.sequence)
