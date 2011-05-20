import FWCore.ParameterSet.Config as cms

pmvTree = cms.EDAnalyzer("PmvTreeMaker",
  name  = cms.untracked.string("pmv"),
  title = cms.untracked.string("pixel match veto TreeMaker tree"),
  src              = cms.InputTag("selectedZMuMuGammas"),
  primaryVertexSrc = cms.InputTag("offlinePrimaryVertices"),
  genParticleSrc   = cms.InputTag("prunedGenParticles"),
  isMC = cms.bool(True),
  prefix = cms.untracked.string(""),
  sizeName = cms.untracked.string("mmgSize"),
  variables = cms.VPSet(),
)

branches = """mmgMass mass
    mmMass           daughter("dimuon").mass
    phoPt            daughter("photon").pt
    phoEta           daughter("photon").eta
    phoIsEB          daughter("photon").masterClonePtr.isEB
    phoHasPixelMatch daughter("photon").masterClonePtr.hasPixelSeed
    phoSigmaIetaIeta daughter("photon").masterClonePtr.sigmaIetaIeta""".split("\n")

genBranches =  """phoPdgId daughter("photon").masterClonePtr.genParticle.pdgId""".split("\n")

for line in  branches:
    tag, var = line.split()
    pmvTree.variables.append(
        cms.PSet(
            tag = cms.untracked.string(tag),
            quantity = cms.untracked.string(var)
        )
    )

for line in genBranches:
    tag, var = line.split()
    pmvTree.variables.append(
        cms.PSet(
            tag = cms.untracked.string(tag),
            quantity = cms.untracked.PSet(
                ifCondition = cms.untracked.string(
                    'daughter("photon").masterClonePtr.genParticlesSize > 0'
                ),
                thenQuantity = cms.untracked.string(var),
                elseQuantity = cms.untracked.string("0")
            )
        )
    )

## shorthand for the muon expression
muon = lambda i: "daughter('dimuon').masterClonePtr." +\
                 "daughter('muon%d').masterClonePtr." % i

muonCalEnergyBranches = """Em em
    EmMax emMax
    Had had""".split("\n")

for line in muonCalEnergyBranches:
    tag, var = line.split()
    pmvTree.variables.extend([
        cms.PSet(
            tag = cms.untracked.string("mu%dCalEnergy%s" % (i, tag)),
            quantity = cms.untracked.PSet(
                ifCondition = cms.untracked.string(muon(i) + "isEnergyValid"),
                thenQuantity = cms.untracked.string(muon(i) + "calEnergy." + \
                                                    var),
                elseQuantity = cms.untracked.string("-1"),
            )
        ) for i in range(1,3)
    ])
pmvTree.variables.extend([
    cms.PSet(
        tag = cms.untracked.string("minDEta"),
        quantity = cms.untracked.string("""
            min(abs(daughter("dimuon").daughter(0).eta-daughter("photon").eta),
                abs(daughter("dimuon").daughter(1).eta-daughter("photon").eta))
        """)
    ),

    cms.PSet(
        tag = cms.untracked.string("minDeltaR"),
        quantity = cms.untracked.string("""
            min( deltaR( daughter("dimuon").daughter(0).eta,
                         daughter("dimuon").daughter(0).phi,
                         daughter("photon").eta,
                         daughter("photon").phi
                         ),
                 deltaR( daughter("dimuon").daughter(1).eta,
                         daughter("dimuon").daughter(1).phi,
                         daughter("photon").eta,
                         daughter("photon").phi
                         )
                 )
        """)
    ),

    cms.PSet(
        tag = cms.untracked.string("minDPhi"),
        quantity = cms.untracked.string("""
            min( abs( deltaPhi( daughter("dimuon").daughter(0).phi,
                                daughter("photon").phi
                                )
                      ),
                 abs( deltaPhi( daughter("dimuon").daughter(1).phi,
                                daughter("photon").phi
                                )
                      )
                 )
        """)
    ),


    #cms.PSet(
        #tag = cms.untracked.string("iphiy"),
        #quantity = cms.untracked.PSet(
            #ifCondition = cms.untracked.string("daughter('photon').masterClonePtr.isEB"),
            #thenQuantity = cms.untracked.string(
                #"daughter('photon').masterClonePtr.superCluster.seed.seed.iphi"
            #),
            #elseQuantity = cms.untracked.string(
                #"daughter('photon').masterClonePtr.superCluster.seed.seed.iy"
            #),
        #)
    #),

])
