import FWCore.ParameterSet.Config as cms
import sys
import copy
import random

class sysProdConfigurator(cms._ParameterTypeBase):

    def __init__(self, pyModuleName, pyNameSpace):
        self.pyModuleName = pyModuleName,
        self.pyNameSpace = pyNameSpace 
        self.sequence = None

    def createSequence(self, randomNumberService, modulePrefix, moduleType, src,
                       energyScaleMean, energyScaleShift, energyScaleSmearing,
                       ptShift, ptSmearing, etaShift, etaSmearing, phiShift, phiSmearing):
            module = cms.EDProducer(moduleType)
            module.src = cms.InputTag(src)
            #THE FileInPath should not be empty!!! Put a file that exists even if
            #it has no meaning... There is no trivial way to avoid that :-(
            module.fileName = cms.FileInPath('DataFormats/Candidate/interface/Candidate.h')
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
            module.gaussianSmearingSigmaEScale = cms.double(energyScaleSmearing)
            module.setLabel(modulePrefix)
            if moduleType == 'SmearedTauProducer': # Additional Tau default Config
                module.jetCorrPayloadName    = cms.string('AK5PF')
                module.jetCorrUncertaintyTag = cms.string('START38_V13')
                module.jecFlavorUncertainty  = cms.double(2.)
                module.shiftByJECuncertainty = cms.double(0.)

            randomPSet = cms.PSet(
                initialSeed = cms.untracked.uint32(987346),
                engineName = cms.untracked.string('TRandom3')
            )

            # ENERGY SCALE
            energyScaleValues = [ energyScaleMean - energyScaleShift, energyScaleMean, energyScaleMean + energyScaleShift ]
            energyScaleLabels = [ 'EnScaleDown', '', 'EnScaleUp' ]

            for label, eScale in zip(energyScaleLabels, energyScaleValues):
                m = module.clone()
                m.energyScale = cms.double(eScale)

                # Additional Tau default Config
                if label == 'EnScaleDown':
                    m.shiftByJECuncertainty = cms.double(-1.)
                if label == 'EnScaleUp':
                    m.shiftByJECuncertainty = cms.double(+1.)    
                
                #Register the filter in the namespace
                pyModule = sys.modules[self.pyModuleName[0]]
                if pyModule is None:
                   raise ValueError("'pyModuleName' Parameter invalid")
                setattr(pyModule,modulePrefix+label,m)
                if self.sequence == None:
                   self.sequence = m
                else:
                  self.sequence *= m

                #Create an entry in the random number service
                randomSet = randomPSet.clone()
                randomSet.initialSeed = random.randint(100000,300000)
                setattr(randomNumberService,modulePrefix+label,randomSet)

            # Pt
            ptValues = [ -ptShift, ptShift ]
            ptLabels = [ 'PtShiftDown', 'PtShiftUp' ]

            for label, value in zip(ptLabels, ptValues):
                m = module.clone()
                m.deltaPt = cms.double(value)                
                m.setLabel(modulePrefix+label)
                #Register the filter in the namespace
                pyModule = sys.modules[self.pyModuleName[0]]
                if pyModule is None:
                    raise ValueError("'pyModuleName' Parameter invalid")
                setattr(pyModule,modulePrefix+label,m)
                if self.sequence == None:
                    self.sequence = m
                else:
                    self.sequence *= m
                #Create an entry in the random number service
                randomSet = randomPSet.clone()
                randomSet.initialSeed = random.randint(100000,300000)
                setattr(randomNumberService,modulePrefix+label,randomSet)

            #eta
            etaValues = [ -etaShift, etaShift]
            etaLabels = [ 'EtaShiftDown', 'EtaShiftUp' ]

            for label, value in zip(etaLabels, etaValues):
                m = module.clone()
                m.deltaEta = cms.double(value)
                m.setLabel(modulePrefix+label)
                #Register the filter in the namespace
                pyModule = sys.modules[self.pyModuleName[0]]
                if pyModule is None:
                    raise ValueError("'pyModuleName' Parameter invalid")
                setattr(pyModule, modulePrefix + label, m)
                if self.sequence == None:
                    self.sequence = m
                else:
                    self.sequence *= m
                randomSet = randomPSet.clone()
                randomSet.initialSeed = random.randint(100000, 300000)
                setattr(randomNumberService, modulePrefix+label, randomSet)

            #phi
            phiValues = [ -phiShift, phiShift ]
            phiLabels = [ 'PhiShiftDown', 'PhiShiftUp' ]

            for label, value in zip(phiLabels, phiValues):
                m = module.clone()
                m.deltaPhi = cms.double(value)
                m.setLabel(modulePrefix+label)
                #Register the filter in the namespace
                pyModule = sys.modules[self.pyModuleName[0]]
                if pyModule is None:
                    raise ValueError("'pyModuleName' Parameter invalid")
                setattr(pyModule, modulePrefix + label, m)
                if self.sequence == None:
                    self.sequence = m
                else:
                    self.sequence *= m
                randomSet = randomPSet.clone()
                randomSet.initialSeed = random.randint(100000, 300000)
                setattr(randomNumberService, modulePrefix + label, randomSet)

            return cms.Sequence(self.sequence)
