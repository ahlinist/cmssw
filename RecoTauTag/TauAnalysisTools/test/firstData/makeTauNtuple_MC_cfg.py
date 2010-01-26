import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

options = VarParsing.VarParsing ('standard')

options.parseArguments()

process = cms.Process("USER")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

process.source = cms.Source (
    "PoolSource",
    fileNames = cms.untracked.vstring(options.files)
)

process.filteredJets = cms.EDProducer(
    # This should eventually be pf specific...  Anything the produces a vector
    # of PFJets compatibile w/ View<Candidate> is fine
    "CandViewSelector", 
    cut = cms.string("pt() > 5"),
    src = cms.InputTag("iterativeCone5PFJets")
)

# Skip event w/ no jets above threshold
process.atLeastOneJet = cms.EDFilter(
    "CandViewCountFilter",
    src = cms.InputTag("filteredJets"),
    minNumber = cms.uint32(1)
)

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = 'GR09_P_V7::All'
process.GlobalTag.globaltag = 'MC_31X_V3::All'
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("RecoTauTag.Configuration.RecoPFTauTag_cff")

# Build the skeleton.  This will create a new sequence called
# process.buildTauNtuple.  It will automatically build the matching option for you.
# The matching option is named matchingCollection2inputCollection - i.e. 
# process.filteredJets2shrinkingConePFTauProducer
from RecoTauTag.TauAnalysisTools.tools.ntupleDefinitions import *
from RecoTauTag.TauAnalysisTools.tools.ntupleTools import makeTauNtuple

makeTauNtuple(
    process,
    input_collection = "shrinkingConePFTauProducer",
    #The name of the created module is [tauType]NtupleProducer
    tauType="shrinkingConePFTau", 
    dataType="PFTau",
    # Expressions and discrimaintors are defined in ntupleDefinitions
    expressions = cms.PSet(common_expressions, pftau_expressions),
    discriminators = cms.PSet(pftau_discriminators, pftau_discriminators_extra),
    decayModeExpressions = pftau_decayModeExpressions,
    # Use only those taus which match our clean jets
    matchingOption = "matched",
    matchingCollection = "filteredJets"
)

# Now setup the truth expressions a bit more for our ntuple
process.shrinkingConePFTauNtupleProducer.matchedType = cms.string("PFJet")
process.shrinkingConePFTauNtupleProducer.matched_expressions = cms.PSet(
    common_expressions, # pt, eta, charge, mass
    emFraction = cms.string("neutralEmEnergyFraction()"),
)

makeTauNtuple(
    process,
    input_collection = "fixedConePFTauProducer",
    #The name of the created module is [tauType]NtupleProducer
    tauType="fixedConePFTau", 
    dataType="PFTau",
    # Expressions and discrimaintors are defined in ntupleDefinitions
    expressions = cms.PSet(common_expressions, pftau_expressions),
    discriminators = cms.PSet(pftau_discriminators),
    # Use only those taus which match our clean jets
    matchingOption = "matched",
    matchingCollection = "filteredJets"
)

# Now setup the truth expressions a bit more for our ntuple
process.fixedConePFTauNtupleProducer.matchedType = cms.string("PFJet")
process.fixedConePFTauNtupleProducer.matched_expressions = cms.PSet(
    common_expressions, # pt, eta, charge, mass
    emFraction = cms.string("neutralEmEnergyFraction()"),
)


# Main process path.  Note that all the ntuple matching/building is done
# in the sequence buildTauNtuple (created behind the scenes)

#process.load("RecoJets.Configuration.RecoPFJets_cff")
process.path = cms.Path(
    #process.iterativeCone5PFJets*
    process.PFTau*
    process.filteredJets*
    process.atLeastOneJet*
    process.buildTauNtuple
)

process.out = cms.OutputModule("PoolOutputModule",                                                                                                                                                        
      outputCommands = cms.untracked.vstring("drop *", "keep *_*NtupleProducer_*_*" ),
      verbose = cms.untracked.bool(False),
      fileName = cms.untracked.string("data_ntuple.root")      
      )

process.endpath = cms.EndPath(process.out)
