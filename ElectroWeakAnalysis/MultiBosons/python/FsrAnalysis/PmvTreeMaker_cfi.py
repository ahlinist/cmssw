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
    phoPhi           daughter("photon").phi
    phoR9            daughter("photon").masterClonePtr.r9
    phoIsEB          daughter("photon").masterClonePtr.isEB
    phoDeltaRToTrack  daughter("photon").masterClonePtr.userFloat('conversionTools:deltaRToTrack')
    phoPassElectronVeto daughter("photon").masterClonePtr.userInt('conversionTools:passElectronVeto')
    phoHasMatchedConversion daughter("photon").masterClonePtr.userInt('conversionTools:hasMatchedConversion')
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
            conditionalQuantity = cms.untracked.PSet(
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

var = lambda t, q: cms.PSet( tag      = cms.untracked.string(t),
                             quantity = cms.untracked.string(q) )

def condVar (iTag, iIf, iThen, iElse):
    return cms.PSet( tag = cms.untracked.string(iTag),
                     conditionalQuantity = cms.untracked.PSet(
                          ifCondition = cms.untracked.string(iIf),
                          thenQuantity = cms.untracked.string(iThen),
                          elseQuantity = cms.untracked.string(iElse),
                      ) )

def muVar(i, iTag, iQuantity):
    return var( 'mu%i%s' % (i, iTag),
                muon(i) + iQuantity   )

def muCondVar(i, iTag, iIf, iThen, iElse):
    return condVar(
        'mu%i%s' % (i, iTag),
        iIf,
        muon(i) + iThen,
        iElse
    )

def muCalEnergy(i, iTag, iQuantity):
    return muCondVar( i,
                      "CalEnergy" + iTag,
                      muon(i) + "isEnergyValid",
                      'calEnergy.' + iQuantity,
                      '-1' )


#muonCalEnergyBranches = """Em em
    #EmMax emMax
    #Had had""".split("\n")

#for line in muonCalEnergyBranches:
    #tag, var = line.split()
    #pmvTree.variables.extend([
        #cms.PSet(
            #tag = cms.untracked.string("mu%dCalEnergy%s" % (i, tag)),
            #conditionalQuantity = cms.untracked.PSet(
                #ifCondition = cms.untracked.string(muon(i) + "isEnergyValid"),
                #thenQuantity = cms.untracked.string(muon(i) + "calEnergy." + \
                                                    #var),
                #elseQuantity = cms.untracked.string("-1"),
            #)
        #) for i in range(1,3)
    #])

pmvTree.variables.extend([
    muVar( 1, 'Pt' , 'pt'  ),
    muVar( 2, 'Pt' , 'pt'  ),
    muVar( 1, 'Eta', 'eta' ),
    muVar( 2, 'Eta', 'eta' ),
    muVar( 1, 'Phi', 'phi' ),
    muVar( 2, 'Phi', 'phi' ),
    muVar( 1, 'TrackChi2', 'track.chi2' ),
    muVar( 2, 'TrackChi2', 'track.chi2' ),
    muVar( 1, 'TrackNormalizedChi2', 'track.normalizedChi2' ),
    muVar( 2, 'TrackNormalizedChi2', 'track.normalizedChi2' ),
    var( 'mu1DeltaR',
         '''deltaR( daughter("dimuon").masterClonePtr.daughter("muon1").eta,
                    daughter("dimuon").masterClonePtr.daughter("muon1").phi,
                    daughter("photon").eta,
                    daughter("photon").phi )''' ),
    var( 'mu2DeltaR',
         '''deltaR( daughter("dimuon").masterClonePtr.daughter("muon1").eta,
                    daughter("dimuon").masterClonePtr.daughter("muon2").phi,
                    daughter("photon").eta,
                    daughter("photon").phi )''' ),
    muCalEnergy( 1, 'Em'   , 'em'    ),
    muCalEnergy( 2, 'Em'   , 'em'    ),
    muCalEnergy( 1, 'EmMax', 'emMax' ),
    muCalEnergy( 2, 'EmMax', 'emMax' ),
    muCalEnergy( 1, 'Had'  , 'had'   ),
    muCalEnergy( 2, 'Had'  , 'had'   ),
    
    var( "minDEta",
         """
          min(abs(daughter("dimuon").daughter(0).eta-daughter("photon").eta),
              abs(daughter("dimuon").daughter(1).eta-daughter("photon").eta))
         """
    ),

    var( "minDeltaR",
         """
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
         """
    ),

    var( "minDPhi",
         """
            min( abs( deltaPhi( daughter("dimuon").daughter(0).phi,
                                daughter("photon").phi
                                )
                      ),
                 abs( deltaPhi( daughter("dimuon").daughter(1).phi,
                                daughter("photon").phi
                                )
                      )
                 )
        """
    ),

    var( 'kRatio',
         '''( 91.1876 * 91.1876 -
                daughter("dimuon").mass * daughter("dimuon").mass ) /
            ( mass * mass -
                daughter("dimuon").mass * daughter("dimuon").mass )
         ''' 
    )
])
