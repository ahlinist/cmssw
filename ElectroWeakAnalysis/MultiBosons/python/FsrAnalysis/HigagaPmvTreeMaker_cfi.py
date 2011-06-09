import FWCore.ParameterSet.Config as cms

pmvTree = cms.EDAnalyzer("CandViewTreeMaker",
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
    phoPhi           phi
    phoR9            r9
    phoIsEB          isEB
    phoHoE           hadronicOverEm
    scEt             superCluster.energy*sin(superCluster.position.theta)
    phoDeltaRToTrack  userFloat('conversionTools:deltaRToTrack')
    phoPassElectronVeto userInt('conversionTools:passElectronVeto')
    phoHasMatchedConversion userInt('conversionTools:hasMatchedConversion')
    phoHasPixelMatch hasPixelSeed
    phoSigmaIetaIeta sigmaIetaIeta
    phoTrackIso      trackIso
    phoEcalIso       ecalIso
    phoHcalIso       hcalIso
    phoMomPdgId      userInt('photonGenMatch:motherPdgId')
    phoMomStatus     userInt('photonGenMatch:motherStatus')
    phoGMomPdgId     userInt('photonGenMatch:grandMotherPdgId')
    phoGMomStatus    userInt('photonGenMatch:grandMotherStatus')""".split("\n")

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
            conditionalQuantity = cms.untracked.PSet(
                ifCondition = cms.untracked.string(
                    'genParticlesSize > 0'
                ),
                thenQuantity = cms.untracked.string(var),
                elseQuantity = cms.untracked.string("0")
            )
        )
    )
