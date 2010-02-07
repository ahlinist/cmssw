import FWCore.ParameterSet.Config as cms
import sys
import copy

class sysProdConfigurator(cms._ParameterTypeBase):
    def __init__(self,pyModuleName,pyNameSpace):
        self.pyModuleName = pyModuleName,
        self.pyNameSpace = pyNameSpace 
        self.sequence = None

    def createSequence(self,modulePrefix,moduleType,src,energyScaleMean,energyScaleShift,energyScaleSmearing,ptShift,ptSmearing,etaShift,etaSmearing,phiShift,phiSmearing):
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
            module.energyScale = cms.double(energyScaleMean)
            module.gaussianSmearingSigmaPt = cms.double(ptSmearing)
            module.gaussianSmearingSigmaEta = cms.double(etaSmearing)
            module.gaussianSmearingSigmaPhi = cms.double(phiSmearing)
            module.gaussianSmearingEScale   = cms.double(energyScaleSmearing)
            module.setLabel(modulePrefix)
            if moduleType =='smearedTauProducer': #Additional Tau default Config
                module.smearConstituents = cms.bool(False),
                hadronEnergyScale        = cms.double(1.0),
                gammaEnergyScale         = cms.double(1.0)  


            #ENERGY SCALE
            energyScaleValues = [energyScaleMean-energyScaleShift, energyScaleMean ,energyScaleMean+energyScaleShift]
            energyScaleLabels = ['EScaleDown' ,'','EScaleUp']

            for label,eScale in zip(energyScaleLabels,energyScaleValues):
                m =module.clone()
                m.energyScale = cms.double(eScale)
              #Register the filter in the namespace
                pyModule = sys.modules[self.pyModuleName[0]]
                if pyModule is None:
                   raise ValueError("'pyModuleName' Parameter invalid")
                setattr(pyModule,modulePrefix+label,m)
                if self.sequence == None:
                   self.sequence=m
                else:
                  self.sequence*=m


            #PT
            ptValues = [-ptShift,ptShift]
            ptLabels = ['PtShiftDown' ,'PtShiftUp']

            for label,value in zip(ptLabels,ptValues):
                m =module.clone()
                m.deltaPt = cms.double(value)
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

            #eta
            etaValues = [-etaShift,etaShift]
            etaLabels = ['EtaShiftDown' ,'EtaShiftUp']

            for label,value in zip(etaLabels,etaValues):
                m =module.clone()
                m.deltaEta = cms.double(value)
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

            #phi
            phiValues = [-phiShift,phiShift]
            phiLabels = ['PhiShiftDown' ,'PhiShiftUp']

            for label,value in zip(phiLabels,phiValues):
                m =module.clone()
                m.deltaPhi = cms.double(value)
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
