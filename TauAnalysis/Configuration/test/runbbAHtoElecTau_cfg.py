import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('runbbAHtoElecTau')

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryIdeal_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_noesprefer_cff')
process.GlobalTag.globaltag = 'IDEAL_V9::All'

process.load("TauAnalysis.Configuration.producePatTuple_cff")

from TauAnalysis.Configuration.recoSampleDefinitionsbbAHtoElecTau_cfi import *

#--------------------------------------------------------------------------------

process.DQMStore = cms.Service("DQMStore")

process.savebbAHtoElecTauPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsbbAHtoElecTau.root')
)

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(-1)    
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/localscratch/ah115bb_tautau.root'
    ),
    skipBadFiles = cms.untracked.bool(True)    
)


from PhysicsTools.PatAlgos.tools.tauTools import * 
switchToPFTauFixedCone(process)

#--------------------------------------------------------------------------------
process.load("PhysicsTools.PFCandProducer.pfMET_cfi")
process.patAODExtraReco.replace( process.patJetMETCorrections, process.patJetMETCorrections + process.pfMET )
process.layer1PFMETs = process.layer1METs.clone(
  metSource = cms.InputTag("pfMET"),
  addMuonCorrections = cms.bool(False),
  addTrigMatch = cms.bool(False)
)
process.allLayer1Objects.replace( process.layer1METs, process.layer1METs + process.layer1PFMETs)

from TauAnalysis.Configuration.tools.tauAnalysisMaker import *

options = {
  'name':'bbAHtoElecTau',
  'object_order':['vertex','electron','tau','jet','elecTauPair']
}

cuts = [
   GenPhaseSpaceCut(),
   TriggerCut(triggerPaths=cms.vstring('HLT_IsoEle15_L1I')),
   VertexCut(cppclass='PATSingleVertexSelector',mode=cms.string('firstVertex'), vertices=cms.InputTag('$last(vertex)'),filter=cms.bool(False),title='Primary Vertex'),
   VertexCut(cppclass='VertexSelector',cut=cms.string('isValid & (chi2prob(chi2,ndof) > 0.01) & (tracksSize >= 2)'),filter=cms.bool(False)),
   VertexCut(cppclass='VertexSelector',cut=cms.string('z > -25 & z < +25'),filter=cms.bool(False)),
   ElectronCut(cut=cms.string('pt > 15')),
   ElectronCut(cut=cms.string('abs(eta) < 2.1')),
   ElectronCut(cut=cms.string('abs(superCluster.eta) < 1.442 | abs(superCluster.eta) > 1.560')),
   ElectronCut(cut=cms.string('(abs(superCluster.eta) < 1.479 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)')),
   ElectronCut(cut=cms.string('electronID("robust")>0')),
   ElectronCut(cut=cms.string('trackIso < 1.')),
   ElectronCut(cut=cms.string('(abs(superCluster.eta) < 1.479 & ecalIso < 1.0) | (abs(superCluster.eta) > 1.479 & ecalIso < 2.5)')),
   ElectronCut(cut=cms.string('gsfTrack.isNonnull')),
   ElectronCut(cppclass='PATElectronIpSelector',vertexSource=cms.InputTag('$last(vertex)'),IpMax=cms.double(0.05),filter=cms.bool(False),title='Electron IP'),
   TauCut(cppclass='PATTauAntiOverlapSelector',srcNotToBeFiltered=cms.VInputTag('$last(electron)'),dRmin=cms.double(0.3),filter=cms.bool(False),title='Tau Anti-overlap'),
   TauCut(cut=cms.string('abs(eta) < 2.1')),
   TauCut(cut=cms.string('pt > 20')),
   TauCut(cut=cms.string('tauID("leadingTrackFinding")>0.5')),
   TauCut(cut=cms.string('tauID("leadingTrackPtCut")>0.5')),
   TauCut(cut=cms.string('tauID("trackIsolation")>0.5')),
   TauCut(cut=cms.string('tauID("ecalIsolation")>0.5')),
   TauCut(cut=cms.string('signalTracks.size() = 1 | signalTracks.size() = 3')),
   TauCut(cut=cms.string('tauID("againstElectron") > 0.5')),
   ElecTauPairCut(cut=cms.string('dR12 > 0.7')),
   ElecTauPairCut(cut=cms.string('charge = 0')),
   JetCut(cppclass='PATJetAntiOverlapSelector',srcNotToBeFiltered=cms.VInputTag('$last(electron)','$last(tau)','$last(muon)'),dRmin=cms.double(0.7),filter=cms.bool(False),title='Jet Anti-overlap'),
   JetCut(cut=cms.string('abs(eta) < 2.1')),
   JetCut(cut=cms.string('et > 20')),
   JetCut(cut=cms.string('bDiscriminator("simpleSecondaryVertexBJetTags")>2 | bDiscriminator("combinedSecondaryVertexBJetTags")>0.4')),
   JetCut(cut=cms.string('bDiscriminator("trackCountingHighEffBJetTags")>2.5')),
   ElecTauPairCut(cut=cms.string('mt1MET < 60'))
]

maker = TauAnalysisMaker(cuts,options,process)



process.p = cms.Path( 
  process.producePrePat+
  process.patDefaultSequence+
  process.producePostPat+
  maker.createObjects()+
  process.savebbAHtoElecTauPlots
)                                          

print maker