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

    ##### debugging from DataDumper.cc
    #### category unique for collecting hits in histoset : Collect
    #process.MessageLogger._categoryCanTalk('Collect')
    #### category with alternative for finding the helix loopers
    ### main category : PeakFinder
    #process.MessageLogger._categoryCanTalk('PeakFinder')
    ### sub-categories
    ## for when a histoset is rejected or accepted
    #process.MessageLogger._categoryCanTalk('CollectPeak')
    ## single categories of algo step
    #process.MessageLogger._categoryCanTalk('Truncate')
    #process.MessageLogger._categoryCanTalk('Equilibrate')
    #process.MessageLogger._categoryCanTalk('PhiInHelix')
    #process.MessageLogger._categoryCanTalk('SlopeCheck')
    #process.MessageLogger._categoryCanTalk('DoubleHits')
    #process.MessageLogger._categoryCanTalk('RadiusCheck')
    process.MessageLogger._categoryCanTalk('Kinematics')
    process.MessageLogger._categoryCanTalk('Summary')
    process.MessageLogger._categoryCanTalk('Plots')

    ##### category for handling the looper set from Method.cc
    process.MessageLogger._categoryCanTalk('LooperMethod')

    
    process.MessageLogger._moduleCanTalk('loopersMask')
    process.TFileService = cms.Service("TFileService",
                                       fileName=cms.string(label+'_TF.root')
                                       )

    process.MessageLogger._changeDestination(label)
    process.maxEvents.input = 1 # dont' flowed debug
    return (process)

def flagTracks(process):
    process.load('SimTracker/TrackAssociation/TrackMCQuality_cfi')
    process.generalTracksOldFlag = process.TrackMCQuality.clone(
        label_tr = cms.InputTag('generalTracks','','RECO')
        )
    process.generalTracksNewFlag = process.TrackMCQuality.clone(
        label_tr = cms.InputTag('generalTracks','','LOOPER')
        )
    process.validation+=process.generalTracksOldFlag
    process.validation+=process.generalTracksNewFlag

    for o in process.outputModules_():
        om=process.outputModules_()[o]
        om.outputCommands.append('keep *_generalTracksOldFlag_*_*')
        om.outputCommands.append('keep *_generalTracksNewFlag_*_*')
    return (process)
