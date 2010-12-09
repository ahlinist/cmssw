from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi \
        import recoSampleDefinitionsAHtoMuTau_7TeV as samples

import TauAnalysis.Configuration.submitAnalysisToLXBatch as submit
import TauAnalysis.Configuration.userRegistry as reg
import TauAnalysis.Configuration.tools.castor as castor
import os

channel = 'AHtoMuTau'
configFile = 'runAHtoMuTau_cfg.py'
# Where we will send the output on castor
outputPath = reg.getAnalysisFilePath(channel)
jobId = reg.getJobId(channel)
skimPath = reg.getSkimEvents(channel)

# Get all the skim files from the castor directory
skim_files = [os.path.join(skimPath, file) for file in
    filter(lambda x: x.startswith('skim_'), (
    file_info['file'] for file_info in castor.nslsl(skimPath)))]

def inputFileMapper(channel, sample, jobId):
    for file in skim_files:
        if file.find('_' + sample + '_') != -1:
            yield file

def outputFileMapper(channel, sample, jobId):
    return "plots_%s_%s_%s.root" % (channel, sample, jobId)

enableSystematics = False

submit.submitAnalysisToLXBatch(
    configFile=configFile,
    channel=channel,
    samples=samples,
    outputDirectory = outputPath,
    jobId = jobId,
    disableSysUncertainties = not enableSystematics,
    inputFileMap = inputFileMapper,
    outputFileMap = outputFileMapper,
    processName = 'lxbatch',
)
