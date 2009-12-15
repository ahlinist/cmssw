import FWCore.ParameterSet.Config as cms
import re
from RecoTauTag.TauAnalysisTools.TauNtupleProducer_cfi import *
from RecoTauTag.TauAnalysisTools.tools.ntupleDefinitions import *

def parametersOfType(type, pset):
   ''' Generator to retrive sub parameters of a given type from a PSet '''
   for name, value in pset.parameters_().iteritems():
      if isinstance(value, type):
         yield (name, getattr(pset, name))

def addToSequence(process, sequence, toAdd):
    ''' Add toAdd to sequence in process if it exists, otherwise create it '''
    if hasattr(process, sequence):
        my_seq = getattr(process, sequence)
        my_seq += (toAdd)
    else:
        setattr(process, sequence, cms.Sequence(toAdd))

def makeTauNtuple(
    process, 
    input_collection="shrinkingConePFTauProducer",
    tauType='shrinkingConePFTau',
    dataType='PFTau',
    expressions=common_expressions,
    discriminators=None,
    decayModeExpressions=None,
    matchingOption = "matched",
    matchingCollection = "trueHadronicTaus"):

    # Clone the prototype
    new_ntuple = protoRecoTauNtupleProducer.clone()
    # Set input collection
    new_ntuple.source = input_collection
    new_ntuple.dataType = dataType
    # Set alias from input collection
    new_ntuple.alias = tauType
    # Set up matching
    if matchingOption not in ['none', 'matched', 'unmatched']:
        raise InputError, " makeTauNtuple: bad matchingOption: ", matchingOption
    new_ntuple.matchingOption = matchingOption
    # Produce the matching if needed
    if matchingOption != 'none':
        new_matching = protoMatcher.clone()
        new_matching.matched = matchingCollection
        new_matching.src = input_collection
        matching_name = "%s2%s" % (matchingCollection, input_collection)
        # Register in process
        setattr(process, matching_name, new_matching)
        # Ensure truth production is in sequence
        addToSequence(process, "buildTauNtuple", 
                      getattr(process, matchingCollection))
        # Add matching to sequence
        addToSequence(process, "buildTauNtuple", new_matching)
        #process.buildTauNtuple += new_matching
        # Add in ntuple proudcer
        new_ntuple.matchingSource = matching_name

    # Add in our desired expressions
    new_ntuple.expressions = expressions
    # Add in and adapt our discriminators
    if discriminators is not None:
        new_ntuple.discriminators = discriminators
    # Search and replace to fix the input tags for the discriminators
    #  Note: all the prototypes use shrinkingConePFTau as the tau type
    discriminatorRegex = re.compile('shrinkingConePFTau')
    for name, discriminator in parametersOfType(cms.InputTag, new_ntuple.discriminators):
        oldProducerValue = discriminator.value()
        # Replace by new producer
        #discriminator = cms.InputTag(discriminatorRegex.sub(oldProducerValue, tauType))
        discriminator = cms.InputTag(discriminatorRegex.sub(tauType, oldProducerValue))
        setattr(new_ntuple.discriminators, name, discriminator)

    # Add in decaymode expressions
    if decayModeExpressions is not None:
        new_ntuple.decayModeExpressions = decayModeExpressions

    # Register in process & add to default sequence
    module_name = tauType + 'NtupleProducer'
    setattr(process, module_name, new_ntuple)
    addToSequence(process, "buildTauNtuple", new_ntuple)
    #process.buildTauNtuple += new_ntuple

def makeStandardTauNtuples(process, matchingCollection = "trueHadronicTaus"):
   # Shrinking cone
   makeTauNtuple(process, 
         input_collection = 'shrinkingConePFTauProducer',
         tauType = 'shrinkingConePFTau',
         expressions = cms.PSet(common_expressions, pftau_expressions),
         discriminators = cms.PSet(pftau_discriminators, pftau_discriminators_extra),
         matchingOption = "matched",
         matchingCollection = matchingCollection)
   makeTauNtuple(process, 
         input_collection = 'fixedConePFTauProducer',
         tauType = 'fixedConePFTau',
         expressions = cms.PSet(common_expressions, pftau_expressions),
         discriminators = cms.PSet(pftau_discriminators),
         matchingOption = "matched",
         matchingCollection = matchingCollection)

