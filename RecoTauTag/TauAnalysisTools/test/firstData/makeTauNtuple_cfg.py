import FWCore.ParameterSet.Config as cms

process = cms.Process("USER")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

#Edit this file so that each line contains each file to use
my_files = "file_list.txt"

def inputFilesFromFile(input_file):
    ''' Generator to get around 255 list limit '''
    my_file = open(input_file, 'r')
    for file in my_file.readlines():
        yield file

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
        [file for file in inputFilesFromFile(my_files)]
    )
)

process.filteredJets = cms.EDProducer(
    # This should eventually be pf specific...  Anything the produces a vector
    # of PFJets compatibile w/ View<Candidate> is fine
    "CandViewSelector", 
    cut = cms.string("pt() > 5 && abs(eta()) < 2.1"),
    src = cms.InputTag("iterativeCone5PFJets")
)

# Skip event w/ no jets above threshold
process.atLeastOneJet = cms.EDFilter(
    "CandViewCountFilter",
    src = cms.InputTag("filteredJets"),
    minNumber = cms.uint32(1)
)

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
    # Use only those taus which match our clean jets
    matchingOption = "matched",
    matchingCollection = "filteredJets"
)

# Now setup the truth expressions a bit more for our ntuple
process.shrinkingConePFTauNtupleProducer.matchedType = cms.string("PFJet")
process.shrinkingConePFTauNtupleProducer.matched_expressions = cms.PSet(
    common_expressions, # pt, eta, charge, mass
    emFraction = cms.string("neutralHadronEnergyFraction()"),
)

# Main process path.  Note that all the ntuple matching/building is done
# in the sequence buildTauNtuple (created behind the scenes)

#process.load("RecoJets.Configuration.RecoPFJets_cff")
process.path = cms.Path(
    #process.iterativeCone5PFJets*
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

dump_file = open("ntupleCfgDump.py", "w")
dump_file.write(process.dumpPython())
