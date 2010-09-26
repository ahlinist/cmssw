from TauAnalysis.Configuration.submitToGrid2 import submitToGrid
import TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi as samples
from Configuration.PyReleaseValidation.autoCond import autoCond
import copy

CONFIG_FILE = 'runAHtoMuTau_cfg.py'
OUTPUT_FILE_PATH = "/user/f/friis/AHtoMuTau_grid/" 
JOB_ID = "Run7"
PLOT_FILES_PREFIX = 'plots'

SAMPLE_LIST_OVERRIDE = []

ENABLE_SYSTEMATICS = False

JOB_OPTIONS_DEFAULTS = [
    ('maxEvents', -1),
    ('inputFileType', 'RECO/AOD'),
    ('isBatchMode', True),
    ('plotsOutputFileName', PLOT_FILES_PREFIX),
]

def get_conditions(globalTag):
    """ Retrieve appropriate conditions 

    Conditions can be automatically retrieved using the autoCond
    utility if the input is in [mc, startup, com10, craft09, etc]
    """
    if globalTag in autoCond:
        return autoCond[globalTag]
    else:
        return globalTag


def number_of_jobs(sample_info, preferred=10000, max_jobs=300):
    """
    Try to run on <preferred> events per job, unless it would
    create to many jobs.
    """
    to_process = sample_info['events_processed']*sample_info['skim_eff']
    desired_njobs = to_process/preferred
    if desired_njobs < 0:
        return max_jobs
    output = (desired_njobs > max_jobs) and max_jobs or desired_njobs
    return int(output)

# Loop over the samples we want to analyze
for sample in samples.SAMPLES_TO_ANALYZE:
    # If the override list is defined list, only take those
    if SAMPLE_LIST_OVERRIDE:
        if sample not in SAMPLE_LIST_OVERRIDE: 
            print "Skipping", sample
            continue
    print "Submitting ", sample

    sample_info = samples.RECO_SAMPLES[sample]
    # Make job info
    jobInfo = {
        'channel' : 'AHtoMuTau',
        'sample' : sample,
        'id' : JOB_ID
    }
    jobOptions = copy.copy(JOB_OPTIONS_DEFAULTS)
    # Get the type and genPhase space cut
    jobOptions.append(('type', sample_info['type']))
    # For the genphase space cut, we need to do it for the two different
    jobOptions.append(('genPhaseSpaceCut', sample_info['genPhaseSpaceCut'], 
        {'suffix': 'woBtag'}))
    jobOptions.append(('genPhaseSpaceCut', sample_info['genPhaseSpaceCut'], 
        {'suffix': 'wBtag'}))

    # Enable factorization if necessary
    jobOptions.append(('enableFactorization', sample_info['factorize']))

    # Get the appropriate GlobalTag
    jobOptions.append(('globalTag', get_conditions(sample_info['conditions'])))

    # This must be done after the factorization step?
    jobOptions.append(('enableSysUncertainties', ENABLE_SYSTEMATICS))

    # Build crab options
    crabOptions = {
        'number_of_jobs' : number_of_jobs(sample_info),
        'datasetpath' : sample_info['datasetpath'],
        'dbs_url' : sample_info['dbs_url'],
        'user_remote_dir' : OUTPUT_FILE_PATH,
        'output_file' : "%s_%s_%s_%s.root" % (
            PLOT_FILES_PREFIX, jobInfo['channel'],
            jobInfo['sample'], jobInfo['id']),
        # Default MC info
        'split_type' : (sample_info['type'] == 'Data') and 'lumis' or 'events',
        'lumi_mask' : sample_info['lumi_mask'],
        'runselection' : sample_info['runselection'],
    }

    submitToGrid(CONFIG_FILE, jobInfo, jobOptions, crabOptions, submit=True)

