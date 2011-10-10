import FWCore.ParameterSet.Config as cms

r9Tree = cms.EDAnalyzer("CandViewTreeMaker",
    name = cms.untracked.string("tree"),
    title = cms.untracked.string("photon tree test"),
    src = cms.InputTag("photons"),
    prefix = cms.untracked.string(""),
    sizeName = cms.untracked.string("size"),
    variables = cms.VPSet()
) # end of r9Tree

recoPhotonTagQuantityPairs = []
for tq in '''
    pt  pt
    eta eta
    phi phi
    r9  r9
    isEB          isEB
    isEE          isEE
    isEBEtaGap    isEBEtaGap
    isEBPhiGap    isEBPhiGap
    isEERingGap   isEERingGap
    isEEDeeGap    isEEDeeGap
    isEBEEGap     isEBEEGap
    hoe           hadronicOverEm
    sigmaIetaIeta sigmaIetaIeta
    scEt          superCluster.energy*sin(superCluster.position.theta)
    scE           superCluster.energy
    scRawE        superCluster.rawEnergy
    scEta         superCluster.eta
    scEtaWidth    superCluster.etaWidth
    scPhiWidth    superCluster.phiWidth
    '''.split('\n'):
    if not tq.strip():
        continue
    recoPhotonTagQuantityPairs.append(tq.split())

patPhotonTagQuantityPairs = []

patPhotonTagConditionalQuantityPairs = [
    ('pdgId' , ('genParticlesSize > 0', 'genParticle(0).pdgId', '0')),
    ('genE'  , ('genParticlesSize > 0', 'genParticle(0).energy', '0')),
    ('genEt' , ('genParticlesSize > 0', 'genParticle(0).pt', '0')),
    ('genEta', ('genParticlesSize > 0', 'genParticle(0).eta', '-99')),
    ('genPhi', ('genParticlesSize > 0', 'genParticle(0).phi', '-99')),
    ]

tagQuantityPairs = recoPhotonTagQuantityPairs
tagConditionalQuantityPairs = []

## Comment this out if you run on RECO photons
## Keep this if you run on PAT photons
tagQuantityPairs.extend(patPhotonTagQuantityPairs)
tagConditionalQuantityPairs.extend(patPhotonTagConditionalQuantityPairs)

for t,q in tagQuantityPairs:
    r9Tree.variables.append(
        cms.PSet(tag      = cms.untracked.string(t),
                 quantity = cms.untracked.string(q))
        )

for t, (cond, thenq, elseq) in tagConditionalQuantityPairs:
    r9Tree.variables.append(
        cms.PSet(
            tag = cms.untracked.string(t),
            conditionalQuantity = cms.untracked.PSet(
                ifCondition = cms.untracked.string(cond),
                thenQuantity = cms.untracked.string(thenq),
                elseQuantity = cms.untracked.string(elseq)
                ) # end of conditionalQuantity
            ) # end of cms.PSet
        ) # end of r9Tree.variables.append

