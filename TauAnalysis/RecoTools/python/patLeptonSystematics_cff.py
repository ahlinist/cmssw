import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.tools.sysProdConfigurator import *
from TauAnalysis.RecoTools.randomNumberGenerator_cfi import *

from TauAnalysis.RecoTools.patMuonMomentumCorrection_cfi import *

#--------------------------------------------------------------------------------
# shift/smear electron collection
#--------------------------------------------------------------------------------

sysConfiguratorElectrons = sysProdConfigurator(pyModuleName = __name__, pyNameSpace = locals())
prodSmearedElectrons = sysConfiguratorElectrons.createSequence(
    randomNumberService   = RandomNumberGeneratorService,
    modulePrefix          = 'smearedElectrons',
    moduleType            = 'SmearedElectronProducer',
    src                   = 'patElectrons',
    energyScaleMean       = 1.0,
    energyScaleShift      = 0.01,
    energyScaleSmearing   = 0.03,
    ptShift               = 1.0,
    ptSmearing            = 0.5,
    etaShift              = 0.025,
    etaSmearing           = 0.01,
    phiShift              = 0.025,
    phiSmearing           = 0.025
)

#--------------------------------------------------------------------------------
# shift/smear muon collection
#--------------------------------------------------------------------------------

poolDBESSourceMuScleFitShiftUp = poolDBESSourceMuScleFitCentralValue.clone(
    connect = cms.string('sqlite_fip:TauAnalysis/RecoTools/data/Z_20_invNb_innerTrack_plusError.db'),
    appendToDataLabel = cms.string("shiftUp"),
    toGet = cms.VPSet(
        cms.PSet(
            record = cms.string('MuScleFitDBobjectRcd'),
            tag = cms.string('Z_20_invNb_innerTrack')
        )
    )
)

patMuonsMuScleFitCorrectedMomentumShiftUp = patMuonsMuScleFitCorrectedMomentum.clone(
    DbObjectLabel = cms.untracked.string("shiftUp")
)

poolDBESSourceMuScleFitShiftDown = poolDBESSourceMuScleFitCentralValue.clone(
    connect = cms.string('sqlite_fip:TauAnalysis/RecoTools/data/Z_20_invNb_innerTrack_minusError.db'),
    appendToDataLabel = cms.string("shiftDown"),
    toGet = cms.VPSet(
        cms.PSet(
            record = cms.string('MuScleFitDBobjectRcd'),
            tag = cms.string('Z_20_invNb_innerTrack')
        )
    )
)

patMuonsMuScleFitCorrectedMomentumShiftDown = patMuonsMuScleFitCorrectedMomentum.clone(
    DbObjectLabel = cms.untracked.string("shiftDown")
)

prodSmearedMuons = cms.Sequence(patMuonsMuScleFitCorrectedMomentumShiftUp * patMuonsMuScleFitCorrectedMomentumShiftDown)

#--------------------------------------------------------------------------------
# shift/smear tau-jet collection
#--------------------------------------------------------------------------------

patTausJECshiftUp = cms.EDProducer("SmearedTauProducer",
    src                         = cms.InputTag('cleanPatTaus'),
    ##jecUncertaintyInputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/START38_V13_AK5PF_Uncertainty.txt"),
    jetCorrPayloadName          = cms.string('AK5PF'),
    jetCorrUncertaintyTag       = cms.string('Uncertainty'),
    ##jecFlavorUncertainty        = cms.double(2.),
    jecFlavorUncertainty        = cms.double(0.),
    shiftByJECuncertainty       = cms.double(+1.)
)

patTausJECshiftDown = patTausJECshiftUp.clone(
    shiftByJECuncertainty = cms.double(-1.)
)

prodSmearedTaus = cms.Sequence(patTausJECshiftUp * patTausJECshiftDown)



