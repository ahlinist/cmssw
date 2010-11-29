#!/usr/bin/env python
from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi \
        import recoSampleDefinitionsAHtoMuTau_7TeV
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, \
        getJobId, getPickEventsPath, getHarvestingFilePath, overrideJobId
import os
import copy
import glob

overrideJobId('AHtoMuTau', 'Run26')

channel = 'AHtoMuTau'
configFile = 'runAHtoMuTau_cfg.py'
analysisFilePath = getAnalysisFilePath(channel)
jobId = getJobId(channel)

samplesToAnalyze = []

enableSystematics = False
enableFakeRates = False

# Function that maps a sample to its pickevents file
def local_sample_mapper(sample):
    return 'file:' + os.path.join(
        getPickEventsPath(channel),
        "skim_%s_%s_%s.root" % (channel, sample, 'Run26')
    )

# Define what output plot file name a sample will have.  We emulate the final
# "harvested" results, but put it in the 'local' directory.
def output_mapper(sample, label=''):
    output_file = os.path.join(
        getHarvestingFilePath(channel),
        'local',
        "harvested_%s_%s_%s.root" % (channel, sample, jobId)
    )
    if label:
        output_file = output_file.replace('.root', "_" + label + ".root")
    # Make the direcdtory if it doesn't exist
    if not os.path.exists(os.path.dirname(output_file)):
        os.makedirs(os.path.dirname(output_file))
    return output_file

print "Building regular"
submitAnalysisToGrid(configFile = configFile, channel = 'AHtoMuTau',
                     samples = recoSampleDefinitionsAHtoMuTau_7TeV,
                     outputFilePath = analysisFilePath, jobId = jobId,
                     samplesToAnalyze = samplesToAnalyze,
                     disableSysUncertainties = True,
                     enableFakeRates = False,
                     # Options for local running
                     create=False, submit=False, cfgdir='local',
                     inputFileMap = local_sample_mapper,
                     outputFileMap = lambda x: output_mapper(x, ''),
                     processName = 'local',
                     saveFinalEvents = False)

# Make systematics jobs
jobIdSys = jobId + "sys"
samplesToAnalyze = [
    sample for sample in recoSampleDefinitionsAHtoMuTau_7TeV['SAMPLES_TO_ANALYZE']
    if recoSampleDefinitionsAHtoMuTau_7TeV['RECO_SAMPLES'][sample][
        'enableSysUncertainties'] ]

print "Building systematics"
submitAnalysisToGrid(configFile = configFile, channel = 'AHtoMuTau',
                     samples = recoSampleDefinitionsAHtoMuTau_7TeV,
                     outputFilePath = analysisFilePath, jobId = jobIdSys,
                     samplesToAnalyze = samplesToAnalyze,
                     disableSysUncertainties = False,
                     enableFakeRates = False,
                     # Options for local running
                     create=False, submit=False, cfgdir='local',
                     inputFileMap = local_sample_mapper,
                     outputFileMap = lambda x: output_mapper(x, 'sys'),
                     processName = 'local',
                     saveFinalEvents = False)

jobIdFr = jobId + "fr"
# Then do fake rates
print "Building fake rates"
samplesToAnalyze = [
    sample for sample in recoSampleDefinitionsAHtoMuTau_7TeV['SAMPLES_TO_ANALYZE']
    if recoSampleDefinitionsAHtoMuTau_7TeV['RECO_SAMPLES'][sample][
        'enableFakeRates'] ]

# Turn off factorization
sampleDefsNoFactorization = copy.deepcopy(recoSampleDefinitionsAHtoMuTau_7TeV)
for key, info in sampleDefsNoFactorization['RECO_SAMPLES'].iteritems():
    if info['factorize']:
        print "Disabling factorization for", key
        info['factorize'] = False

submitAnalysisToGrid(configFile = configFile, channel = 'AHtoMuTau',
                     samples = sampleDefsNoFactorization,
                     outputFilePath = analysisFilePath, jobId = jobIdFr,
                     samplesToAnalyze = samplesToAnalyze,
                     disableSysUncertainties = True,
                     enableFakeRates = True,
                     # Options for local running
                     create=False, submit=False, cfgdir='local',
                     inputFileMap = local_sample_mapper,
                     outputFileMap = lambda x: output_mapper(x, 'fr'),
                     processName = 'local',
                     saveFinalEvents = False)

# Build a script to run all jobs locally

script = '''
#!/usr/bin/bash

cd local

'''

cfg_files = glob.glob('local/*%ssys@Grid_cfg.py' % jobId)
for cfg_file in cfg_files:
    file_name = os.path.basename(cfg_file)
    log_file_name = file_name.replace('_cfg.py', '_cfg.log')
    script += 'echo Running %s\n' % file_name
    script += 'rm -f %s\n' % log_file_name
    script += 'nice cmsRun %s >& /data1/friis/logs/%s &\n\n' % (file_name, log_file_name)

script_file = open('run_local.sh', 'w')
script_file.write(script)

print "Built run_local.sh script"
