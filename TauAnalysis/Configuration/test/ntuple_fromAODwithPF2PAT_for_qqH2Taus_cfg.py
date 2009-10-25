# This is an example PAT configuration showing the usage of PF2PAT

# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *

# Overriding source and various other things
#conditions, for global tag see: https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions
#process.GlobalTag.globaltag = cms.string('STARTUP31X_V3::All')
#process.GlobalTag.globaltag = cms.string('MC_31X_V4::All')
process.GlobalTag.globaltag = cms.string('DESIGN_31X_V3::All')
#magnetic field
#process.load("Configuration.StandardSequences.MagneticField_cff")#3.8T
process.load("Configuration.StandardSequences.MagneticField_40T_cff")

#run summary
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

#source
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

process.source.skipEvents = cms.untracked.uint32(0)
process.source.fileNames = cms.untracked.vstring()

dirName='./'
for i in range(0, 1):
    myFile = 'file:'+dirName+'aod_fastHLT.root'
    #print myFile
    process.source.fileNames.append(myFile)

#output
process.out.fileName = cms.untracked.string(
    dirName+'patLayer1_fromAOD_PF2PAT.root'
    )

# User configurable
# add caloMET
addCaloMet = True

# add tcMET
addTcMet = True

# add caloTaus
addCaloTaus = True

# add run data (run description)
addRunData = True

addPF2PATProducts = True
addAODExtraProducts = True
addTauExtraProducts = True

##########################################################################
# Don't modify anything below this line unless you know what you are doing.

# load the standard PAT config
process.load("PhysicsTools.PatAlgos.patSequences_cff")

# load the PF to PAT config
process.load("PhysicsTools.PFCandProducer.PF2PAT_cff")

# Configure PAT to use PF2PAT instead of AOD sources
from PhysicsTools.PatAlgos.tools.pfTools import *
usePF2PAT(process,runPF2PAT=True)  # or you can leave this to the default, False, and run PF2PAT before patDefaultSequence

# Let it run
process.p = cms.Path(
    process.patDefaultSequence  
)

## Modifications of the PAT production sequence
#use "correct" genMET
process.layer1METs.genMETSource = 'genMetTrue'
#produce tauGenJet collections accordingly to tau decay modes
process.load("TauAnalysis.GenSimTools.tauGenJetSelector_cfi")
process.makeAllLayer1Taus.replace(process.tauGenJets,
                                  process.tauGenJets *
                                  ( process.selectedGenTauDecaysToElectron +
                                    process.selectedGenTauDecaysToMuon +
                                    process.selectedGenTauDecaysToHadrons ) )
process.tauGenJetMatch.matched = cms.InputTag('selectedGenTauDecaysToHadrons')
# add caloMET
if addCaloMet:
    process.layer1CaloMETs = process.layer1METs.clone()
    process.layer1CaloMETs.metSource = cms.InputTag('metJESCorIC5CaloJetMuons')
    process.layer1CaloMETs.addMuonCorrections = False
    #process.layer1CaloMETs.genMETSource = cms.InputTag('genMetTrue')
    process.makeLayer1METs += (process.patMETCorrections*
                               process.layer1CaloMETs)
    process.allLayer1Summary.candidates += [cms.InputTag("layer1CaloMETs")]
# add tcMET
process.load("PhysicsTools.PatAlgos.tools.metTools")
if addTcMet:
    #addTcMET(process)
    # layer1METsTC.genMETSource = cms.InputTag('genMetTrue')
    process.layer1TCMETs = process.layer1METs.clone()
    process.layer1TCMETs.metSource = cms.InputTag('tcMet')
    process.layer1TCMETs.addMuonCorrections = False
    #process.layer1TCMETs.genMETSource = cms.InputTag('genMetTrue')
    process.makeLayer1METs += process.layer1TCMETs
    process.allLayer1Summary.candidates += [cms.InputTag("layer1TCMETs")]
# add caloTaus
if addCaloTaus:
    process.load("RecoTauTag.RecoTau.CaloRecoTauDiscriminationByIsolation_cfi")
    process.allLayer0CaloTausDiscrimination = process.caloRecoTauDiscriminationByIsolation.clone()
    process.allLayer0CaloTaus = cms.EDFilter("CaloTauSelector",
        src = cms.InputTag('caloRecoTauProducer'),
        discriminators = cms.VPSet(
            cms.PSet(discriminator = cms.InputTag("allLayer0CaloTausDiscrimination"),
                     selectionCut = cms.double(0.5) )
            )
    )
    caloTauLabel = cms.InputTag('allLayer0CaloTaus')
    process.caloTauMatch = process.tauMatch.clone()
    process.caloTauMatch.src = caloTauLabel
    process.caloTauGenJetMatch = process.tauGenJetMatch.clone()
    process.caloTauGenJetMatch.src = caloTauLabel
    process.allLayer1CaloTaus = process.allLayer1Taus.clone()
    process.allLayer1CaloTaus.tauSource = caloTauLabel
    #process.allLayer1CaloTaus.addGenMatch = False
    process.allLayer1CaloTaus.genParticleMatch = 'caloTauMatch'
    process.allLayer1CaloTaus.genJetMatch = 'caloTauGenJetMatch'
    process.allLayer1CaloTaus.isolation = cms.PSet()
    process.allLayer1CaloTaus.isoDeposits = cms.PSet()
    process.allLayer1CaloTaus.tauIDSources = cms.PSet(
        leadingTrackFinding = cms.InputTag("caloRecoTauDiscriminationByLeadingTrackFinding"),
        leadingTrackPtCut   = cms.InputTag("caloRecoTauDiscriminationByLeadingTrackPtCut"),
        byIsolation         = cms.InputTag("caloRecoTauDiscriminationByIsolation"),
        #againstElectron     = cms.InputTag("caloRecoTauDiscriminationAgainstElectron")  
    )
    process.allLayer1CaloTaus.addDecayMode = False
    process.allLayer1CaloTaus.decayModeSrc = ''
    process.makeAllCaloLayer1Taus = cms.Sequence(
        # reco pre-production
        process.allLayer0CaloTausDiscrimination *
        process.allLayer0CaloTaus *
        process.patCaloTauDiscrimination *
        ##patPFCandidateIsoDepositSelection *
        ##patPFTauIsolation *
        # pat specifics
        process.caloTauMatch *
        process.caloTauGenJetMatch *
        # object production
        process.allLayer1CaloTaus
    )
    massSearchReplaceParam(process.patCaloTauDiscrimination, 'CaloTauProducer',
                           cms.InputTag('caloRecoTauProducer'), caloTauLabel)

    process.makeAllLayer1Taus += process.makeAllCaloLayer1Taus
    process.allLayer1Summary.candidates += [cms.InputTag("allLayer1CaloTaus")]
    process.selectedLayer1CaloTaus = process.selectedLayer1Taus.clone() 
    process.selectedLayer1CaloTaus.src = cms.InputTag("allLayer1CaloTaus")
    process.pfTauMatchedLayer1CaloTaus = cms.EDFilter(
        "PATTauAntiOverlapSelector",
        src = cms.InputTag("selectedLayer1CaloTaus"),
        srcNotToBeFiltered = cms.VInputTag("selectedLayer1Taus"),
        dRmin = cms.double(0.3),
        invert = cms.untracked.bool(True),
        filter = cms.bool(False)                                           
    )

    process.selectedLayer1Objects.replace(process.selectedLayer1Taus,
                                          process.selectedLayer1Taus +
                                          process.selectedLayer1CaloTaus +
                                          process.pfTauMatchedLayer1CaloTaus)
    process.selectedLayer1Summary.candidates += [cms.InputTag("selectedLayer1CaloTaus"),
                                                 cms.InputTag("pfTauMatchedLayer1CaloTaus")]

#######

## Modifications of the output file content
# Add PF2PAT output to the created file
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoLayer1Cleaning
process.load("PhysicsTools.PFCandProducer.PF2PAT_EventContent_cff")
process.out.outputCommands =  cms.untracked.vstring('drop *', *patEventContentNoLayer1Cleaning )
if addPF2PATProducts:
    process.out.outputCommands.extend( process.PF2PATEventContent.outputCommands)
if addAODExtraProducts:
    from PhysicsTools.PatAlgos.patEventContent_cff import patExtraAodEventContent
    process.out.outputCommands.extend(cms.untracked.vstring( *patExtraAodEventContent ) )
if addTauExtraProducts:
    tauOutputList = cms.untracked.vstring('keep *_tauGenJets_*_*',
                                          'keep *_selectedGenTauDecaysToElectron_*_*',
                                          'keep *_selectedGenTauDecaysToMuon_*_*',
                                          'keep *_selectedGenTauDecaysToHadrons_*_*',
                                          'keep *_genMetTrue_*_*'
                                          )
    process.out.outputCommands.extend( tauOutputList )
if addCaloMet:
    process.out.outputCommands.extend(cms.untracked.vstring('keep *_layer1CaloMETs_*_*') )
if addTcMet:
    process.out.outputCommands.extend(cms.untracked.vstring('keep *_layer1TCMETs_*_*') )
if addCaloTaus:
    process.out.outputCommands.extend(cms.untracked.vstring('keep *_selectedLayer1CaloTaus_*_*',
                                                            'keep *_pfTauMatchedLayer1CaloTaus_*_*') )
if addRunData:
    process.out.outputCommands.extend(cms.untracked.vstring('keep LHERunInfoProduct_*_*_*',
                                                            'keep GenRunInfoProduct_*_*_*') )
        
