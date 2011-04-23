import FWCore.ParameterSet.Config as cms

pmvTree = cms.EDAnalyzer("TreeMaker",
  name  = cms.untracked.string("pmv"),
  title = cms.untracked.string("pixel match veto TreeMaker tree"),
  src   = cms.InputTag("selectedPhotons"),
  isMC = cms.bool(True),
  prefix = cms.untracked.string(""),
  sizeName = cms.untracked.string("nPhotons"),
  variables = cms.VPSet(),
)

branches = """phoPt            pt
    phoEta           eta
    phoIsEB          isEB
    phoHasPixelMatch hasPixelSeed
    phoSigmaIetaIeta sigmaIetaIeta
    phoHoE           hadronicOverEm
    phoTrackIso      trackIso
    phoEcalIso       ecalIso
    phoHcalIso       hcalIso""".split("\n")

genBranches =  """phoPdgId genParticle.pdgId""".split("\n")

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
                    'genParticlesSize > 0'
                ),
                thenQuantity = cms.untracked.string(var),
                elseQuantity = cms.untracked.string("0")
            )
        )
    )
