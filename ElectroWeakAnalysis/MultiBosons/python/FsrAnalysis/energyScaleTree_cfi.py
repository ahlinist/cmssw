import FWCore.ParameterSet.Config as cms

tree = cms.EDAnalyzer("CandViewTreeMaker",
  name  = cms.untracked.string("es"),
  title = cms.untracked.string("Z -> uuy Energy Scale Tree"),
  src              = cms.InputTag("selectedZMuMuGammas"),
  prefix = cms.untracked.string(""),
  sizeName = cms.untracked.string("size"),
  variables = cms.VPSet(),
)

branches = """
    isEB          daughter("photon").masterClonePtr.isEB
    pt            daughter("photon").pt
    eta           daughter("photon").eta
    phi           daughter("photon").phi
    r9            daughter("photon").masterClonePtr.r9
    m2            daughter("dimuon").mass
    m3            mass
    momPid        daughter("photon").masterClonePtr.userInt('photonUserData:genMomPdgId')
    """.split("\n")

genBranches =  """phoPdgId daughter("photon").masterClonePtr.genParticle.pdgId""".split("\n")

# muonBranches = '''
#     Pt    pt
#     Eta   eta
#     Phi   phi
#     '''.split('\n')

for line in  branches:
    if line.split() == []: continue
    tag, var = line.split()
    tree.variables.append(
        cms.PSet(
            tag = cms.untracked.string(tag),
            quantity = cms.untracked.string(var)
        )
    )

for line in genBranches:
    tag, var = line.split()
    tree.variables.append(
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

###############################################################################
## shorthand for the muon expression
muon = lambda i: "daughter('dimuon').masterClonePtr." +\
                 "daughter('muon%d').masterClonePtr." % i

###############################################################################
var = lambda t, q: cms.PSet( tag      = cms.untracked.string(t),
                             quantity = cms.untracked.string(q) )

###############################################################################
def condVar (iTag, iIf, iThen, iElse):
    return cms.PSet( tag = cms.untracked.string(iTag),
                     conditionalQuantity = cms.untracked.PSet(
                          ifCondition = cms.untracked.string(iIf),
                          thenQuantity = cms.untracked.string(iThen),
                          elseQuantity = cms.untracked.string(iElse),
                      ) )

###############################################################################
def muVar(i, iTag, iQuantity):
    return var( 'mu%i%s' % (i, iTag),
                muon(i) + iQuantity   )

###############################################################################
def muCondVar(i, iTag, iIf, iThen, iElse):
    return condVar(
        'mu%i%s' % (i, iTag),
        iIf,
        muon(i) + iThen,
        iElse
    )


###############################################################################
def deltaR(i):
    return '''deltaR( %seta,
                      %sphi,
                      daughter("photon").eta,
                      daughter("photon").phi )
           ''' % ( muon(i), muon(i) )

###############################################################################
def muNearVar(iTag, iQuantity):
    return condVar(
        iTag  = 'muNear' + iTag,
        iIf   = deltaR(1) + ' < ' + deltaR(2),
        iThen = muon(1) + iQuantity,
        iElse = muon(2) + iQuantity,
    )

###############################################################################
def muFarVar(iTag, iQuantity):
    return condVar(
        iTag  = 'muFar' + iTag,
        iIf   = deltaR(1) + ' > ' + deltaR(2),
        iThen = muon(1) + iQuantity,
        iElse = muon(2) + iQuantity,
    )


###############################################################################
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
    #tree.variables.extend([
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

tree.variables.extend([
    muNearVar( 'Pt' , 'pt'  ),
#     muFarVar ( 'Pt' , 'pt'  ),
#     muNearVar( 'Eta', 'eta' ),
#     muFarVar ( 'Eta', 'eta' ),
#     muNearVar( 'Phi', 'phi' ),
#     muFarVar ( 'Phi', 'phi' ),
#     var( "dr", 'min(%s, %s)' % ( deltaR(1), deltaR(2) ) ),
    var( 'k',
         '''( 91.1876 * 91.1876 -
                daughter("dimuon").mass * daughter("dimuon").mass ) /
            ( mass * mass -
                daughter("dimuon").mass * daughter("dimuon").mass )
         '''
    ),
###############################################################################
#     muVar( 1, 'Pt' , 'pt'  ),
#     muVar( 2, 'Pt' , 'pt'  ),
#     muVar( 1, 'Eta', 'eta' ),
#     muVar( 2, 'Eta', 'eta' ),
#     muVar( 1, 'Phi', 'phi' ),
#     muVar( 2, 'Phi', 'phi' ),
#     muVar( 1, 'TrackChi2', 'track.chi2' ),
#     muVar( 2, 'TrackChi2', 'track.chi2' ),
#     muVar( 1, 'TrackNormalizedChi2', 'track.normalizedChi2' ),
#     muVar( 2, 'TrackNormalizedChi2', 'track.normalizedChi2' ),
#     var( 'mu1DeltaR', deltaR(1) ),
#     var( 'mu2DeltaR', deltaR(2) ),
#     muCalEnergy( 1, 'Em'   , 'em'    ),
#     muCalEnergy( 2, 'Em'   , 'em'    ),
#     muCalEnergy( 1, 'EmMax', 'emMax' ),
#     muCalEnergy( 2, 'EmMax', 'emMax' ),
#     muCalEnergy( 1, 'Had'  , 'had'   ),
#     muCalEnergy( 2, 'Had'  , 'had'   ),

#     var( "minDEta",
#          """
#           min(abs(daughter("dimuon").daughter(0).eta-daughter("photon").eta),
#               abs(daughter("dimuon").daughter(1).eta-daughter("photon").eta))
#          """
#     ),

#     var( "maxDeltaR", 'max(%s, %s)' % ( deltaR(1), deltaR(2) ) ),

#     var( "minDPhi",
#          """
#             min( abs( deltaPhi( daughter("dimuon").daughter(0).phi,
#                                 daughter("photon").phi
#                                 )
#                       ),
#                  abs( deltaPhi( daughter("dimuon").daughter(1).phi,
#                                 daughter("photon").phi
#                                 )
#                       )
#                  )
#         """
#     ),

])
