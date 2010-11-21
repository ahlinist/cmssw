#!/usr/bin/env python
from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi \
        import recoSampleDefinitionsAHtoMuTau_7TeV
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, \
        getJobId, getPickEventsPath, getHarvestingFilePath
import os

channel = 'AHtoMuTau'
configFile = 'runAHtoMuTau_cfg.py'
analysisFilePath = getAnalysisFilePath(channel)
jobId = getJobId(channel)

samplesToAnalyze = [
    # modify in case you want to submit crab jobs for some of the samples only...
    #'ZtautauPU156bx',
    #'Ztautau',
    #'PPmuXptGt20Mu10',
    #'data_Mu_Run2010B_Prompt',
]

#enableSystematics = True
enableSystematics = False

# Function that maps a sample to its pickevents file
def local_sample_mapper(sample):
    return 'file:' + os.path.join(
        getPickEventsPath(channel),
        "skim_%s_%s_%s.root" % (channel, sample, jobId)
    )

# Define what output plot file name a sample will have.  We emulate the final
# "harvested" results, but put it in the 'local' directory.
def output_mapper(sample):
    output_file = os.path.join(
        getHarvestingFilePath(channel),
        'local',
        "harvested_%s_%s_%s.root" % (channel, sample, jobId)
    )
    # Make the direcdtory if it doesn't exist
    if not os.path.exists(os.path.dirname(output_file)):
        os.makedirs(os.path.dirname(output_file))
    return output_file

submitAnalysisToGrid(configFile = configFile, channel = 'AHtoMuTau',
                     samples = recoSampleDefinitionsAHtoMuTau_7TeV,
                     outputFilePath = analysisFilePath, jobId = jobId,
                     samplesToAnalyze = samplesToAnalyze,
                     disableSysUncertainties = not enableSystematics,
                     # Options for local running
                     create=False, submit=False, cfgdir='local',
                     inputFileMap = local_sample_mapper,
                     outputFileMap = output_mapper,
                     processName = 'local',
                     saveFinalEvents = False)
