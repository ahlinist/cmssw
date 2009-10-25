
import FWCore.ParameterSet.Config as cms

process = cms.Process("ANA")

##########################################################################

# source: 
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

dirName = './'

process.source = cms.Source("PoolSource",
    skipEvents = cms.untracked.uint32(0),
    fileNames = cms.untracked.vstring()
)
for i in range(0, 1):
    myFile = 'file:'+dirName+'patLayer1_fromAOD_PF2PAT.root'
    #myFile = 'file:'+dirName+'patLayer1_fromAOD_PF2PAT_'+str(i)+'.root'
    #print myFile
    process.source.fileNames.append(myFile)

# redo gen collections?
redoGen = False

# only leading taus
useLeadingTausOnly = True

# verbosity:
setVerbose = False

# add run data (run description)
addRunData = True

# output:
outputFile = dirName+'analysis_DiTau_PF2PAT.root'

##########################################################################
# Don't modify anything below this line unless you know what you are doing.

# gen level analysis
process.load("TauAnalysis.GenSimTools.gen_cff")
process.load("TauAnalysis.GenSimTools.genDiTauReconstruction_cff")
process.load("TauAnalysis.GenSimTools.genMETFromNeutrals_cfi")
#process.genMETFromNeutrals.neutrinosFromTaus = True
#process.genDiTau.srcMET = 'genMETFromNeutrals'

process.particleListDrawer.maxEventsToPrint = cms.untracked.int32(10)
process.particleListDrawer.printOnlyHardInteraction = True


# reco level analysis 
process.load("TauAnalysis.RecoTools.recoDiTauReconstruction_cff")
process.recoDiTau.srcLeg1 = 'selectedLayer1Muons'
process.recoDiTau.srcLeg2 = 'selectedLayer1Taus'
process.recoCaloTauDiTau = process.recoDiTau.clone()
process.recoCaloTauDiTau.srcLeg2 = 'selectedLayer1CaloTaus'
process.recoCaloMetDiTau = process.recoDiTau.clone()
process.recoCaloMetDiTau.srcMET = 'layer1CaloMETs'
process.recoTcMetDiTau = process.recoDiTau.clone()
process.recoTcMetDiTau.srcMET = 'layer1TCMETs'

# mixed level analysis (reco leptons, diffrent METs)
process.recoHTauDiTau = process.genDiTau.clone()
process.recoHTauDiTau.srcLeg2 = process.recoDiTau.srcLeg2
process.recoTausDiTau = process.genDiTau.clone()
process.recoTausDiTau.srcLeg1 = process.recoDiTau.srcLeg1
process.recoTausDiTau.srcLeg2 = process.recoDiTau.srcLeg2

if setVerbose:
    process.genDiTau.verbosity = 1
    process.recoDiTau.verbosity = 1

if not redoGen:
    process.genDiTauReconstruction.remove(process.tauGenJets)
    process.genDiTauReconstruction.remove(process.selectedGenTauDecaysToMuon)
    process.genDiTauReconstruction.remove(process.selectedGenTauDecaysToHadrons)

process.genDiTau.useLeadingTausOnly = useLeadingTausOnly
process.recoDiTau.useLeadingTausOnly = useLeadingTausOnly

process.p1 = cms.Path(
    #process.genParticlesPrint +
    #process.genMETFromNeutrals +
    process.genDiTauReconstruction +  
    process.recoDiTauReconstruction +
    process.recoCaloTauDiTau +
    process.recoCaloMetDiTau +
    process.recoTcMetDiTau +
    process.recoHTauDiTau +
    process.recoTausDiTau
    )


process.out = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *'),
    fileName = cms.untracked.string( outputFile ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring("p1")
        )
)

process.load("TauAnalysis.RecoTools.recoEventContent_cff")
process.out.outputCommands.extend( process.TauAnalysisRecoToolsEC.outputCommands )
process.load("TauAnalysis.GenSimTools.genEventContent_cff")
process.out.outputCommands.extend( process.TauAnalysisGenSimToolsEC.outputCommands )
#process.out.outputCommands.extend(cms.untracked.vstring('keep *_genEventAnalysisProducer_*_*') )
process.out.outputCommands.extend(cms.untracked.vstring('keep *_selectedLayer1Jets_*_*') )
process.out.outputCommands.extend(cms.untracked.vstring('keep *_selectedLayer1CaloTaus_*_*',
                                                        'keep *_pfTauMatchedLayer1CaloTaus_*_*',
                                                        'keep *_layer1CaloMETs_*_*',
                                                        'keep *_layer1TCMETs_*_*') )
process.out.outputCommands.extend(cms.untracked.vstring('keep *_recoHTauDiTau_*_*',
                                                        'keep *_recoTausDiTau_*_*') )
process.out.outputCommands.extend(cms.untracked.vstring('keep *_recoCaloTauDiTau_*_*',
                                                        'keep *_recoCaloMetDiTau_*_*',
                                                        'keep *_recoTcMetDiTau_*_*') )
if addRunData:
    process.out.outputCommands.extend(cms.untracked.vstring('keep LHERunInfoProduct_*_*_*',
                                                            'keep GenRunInfoProduct_*_*_*') )

process.outpath = cms.EndPath( process.out )


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
