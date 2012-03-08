import FWCore.ParameterSet.Config as cms

def debug(process):
    label=''
    for o in process.outputModules_():
        om=process.outputModules_()[o]
        label=om.fileName.value().replace('.root','')
        print label
        break
    process.load('logger_cfi')
    process.options.wantSummary = cms.untracked.bool(True)

    process.MessageLogger._categoryCanTalk('Collect')
    process.MessageLogger._categoryCanTalk('PeakFinder')
    #process.MessageLogger._categoryCanTalk('Equilibrate')
    #process.MessageLogger._categoryCanTalk('Truncate')
    #process.MessageLogger._categoryCanTalk('PhiInHelix')
    #process.MessageLogger._categoryCanTalk('SlopeCheck')
    #process.MessageLogger._categoryCanTalk('CollectPeak')

    process.MessageLogger._moduleCanTalk('loopersMask')
    process.TFileService = cms.Service("TFileService",
                                       fileName=cms.string(label+'_TF.root')
                                       )

    process.MessageLogger._changeDestination(label)
    process.maxEvents.input = 1 # dont' flowed debug
    return (process)
