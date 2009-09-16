import FWCore.ParameterSet.Config as cms

ElectronProducer = cms.EDProducer("ElectronProducer",
                                  ElectronCollection=cms.InputTag("pixelMatchGsfElectrons"),
                                  IdCollection=cms.InputTag("eidCutBasedExt"),
                                  TrackCollection=cms.InputTag("generalTracks"),
                                  ptMin=cms.double(15),
                                  doID=cms.bool(True),
                                  doTrackIso=cms.bool(True),
                                  ptMinTrack=cms.double(1.5),
                                  lipMinTrack=cms.double(1.5),
                                  InnerConeDR=cms.double(0.02),
                                  OuterConeDR=cms.double(0.6),
                                  MaxIsoVar=cms.double(0.02),
                                  doElecFromZ=cms.bool(True),
                                  MinZwindow=cms.double(70),
                                  MaxZwindow=cms.double(110),
                                  ElecEtFromZcut=cms.double(15),
                                  EtaMax=cms.double(2.5)
                                  )

from RecoEcal.Configuration.RecoEcal_cff import *
from Configuration.StandardSequences.Geometry_cff import *
####from RecoEcal.EgammaClusterProducers.geometryForClustering_cff import *
from RecoEgamma.ElectronIdentification.electronIdCutBasedExt_cfi import eidCutBasedExt
eidCutBasedExt.electronQuality='robust'

ElectronProductionSequence = cms.Sequence(eidCutBasedExt+ElectronProducer)
