#!/usr/bin/env python
from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi \
        import recoSampleDefinitionsAHtoMuTau_7TeV as samples
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, \
        getJobId, getPickEventsPath, getHarvestingFilePath, overrideJobId
import os
import copy
import glob

overrideJobId('AHtoMuTau', 'RunOnOursJan16')
#overrideJobId('AHtoMuTau', 'RunOnWeirdFile')

channel = 'AHtoMuTau'
configFile = 'runAHtoMuTau_cfg.py'
analysisFilePath = getAnalysisFilePath(channel)
jobId = getJobId(channel)

samplesToAnalyze = [
    #'data_Mu_Run2010A_Nov4ReReco',
    'data_Mu_Run2010B_Nov4ReReco'
]

add_prefix = lambda x: 'file:' + x

pickEventsMap = {
    'RunOnMikesJan13' : {
        'data_Mu_Run2010A_Nov4ReReco' : map(add_prefix,
                                            glob.glob('pickevents/2010A/*root')),
        'data_Mu_Run2010B_Nov4ReReco' : map(add_prefix,
                                            glob.glob('pickevents/2010B/*root')),
    },
    'RunOnOursJan16' : {
        'data_Mu_Run2010A_Nov4ReReco' : map(
            add_prefix,
            glob.glob('/data2/friis/HiggsDataSkimHPSLooseJan16/*2010A*root')),
        #'data_Mu_Run2010B_Nov4ReReco' : map(
            #add_prefix,
            #glob.glob('/data2/friis/HiggsDataSkimHPSLooseJan16/*2010B*root')),
        'data_Mu_Run2010B_Nov4ReReco' : map(
            add_prefix,
            glob.glob('/data1/veelken/CMSSW_3_8_x/skims/AHtoMuTau/selEvents_AHtoMuTau_HPSloose_friis_RECO.root')),
    },
    'RunOnChristiansSkim' : {
        'data_Mu_Run2010B_Nov4ReReco' : '/data1/veelken/CMSSW_3_8_x/skims/AHtoMuTau/selEvents_AHtoMuTau_HPSloose_friis_RECO.root'
    },
    'RunOnWeirdFile' : {
        'data_Mu_Run2010B_Nov4ReReco' : 'file:/data2/friis/DebuggingLostCrabEvents/BE4A558E-36EC-DF11-9264-90E6BA19A22D.root'
    },
}


enableSystematics = False
enableFakeRates = False

# Function that maps a sample to its pickevents file
def local_sample_mapper(sample):
    #return 'file:' + os.path.join(
        #getPickEventsPath(channel),
        #"skim_%s_%s_%s.root" % (channel, sample, 'Run26')
    #)
    return pickEventsMap[jobId][sample]

# Define what output plot file name a sample will have.  We emulate the final
# "harvested" results, but put it in the 'local' directory.
def output_mapper(channel, sample, jobId, label=''):
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
                     samples = samples,
                     outputFilePath = analysisFilePath, jobId = jobId,
                     samplesToAnalyze = samplesToAnalyze,
                     disableSysUncertainties = True,
                     enableFakeRates = False,
                     # Options for local running
                     create=False, submit=False, cfgdir='local',
                     inputFileMap = local_sample_mapper,
                     outputFileMap = lambda x,y,z: output_mapper(x,y,z, ''),
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
