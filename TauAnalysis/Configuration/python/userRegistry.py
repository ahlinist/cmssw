import os

userSettings = {
    'friis' : {
        # The job ID to use per default.
        'current' : 'Run31',
        # List of runs
        'jobs' : {
            'Run31' : {
                'AHtoMuTau' : {
                    'analysisFilePath' : '/user/f/friis/Run31/',
                    'harvestingFilePath' : "/data1/friis/",
                    'tmpFilePath' : "/data2/friis/tmp/",
                    'pickevents' : '/data1/friis/Run26'
                }
            },
            'Run32' : {
                'AHtoMuTau' : {
                    # The output directory on castor
                    'analysisFilePath' : '/user/f/friis/Run32/',
                    # The output directory for the plots
                    'harvestingFilePath' : "/data1/friis/",
                    'tmpFilePath' : "/data2/friis/tmp/",
                    'pickevents' : '/data1/friis/Run26'
                }
            },
        },
        'global' : {
            'drawOptions' : {
                'canvasSizeX' : 640,
                'canvasSizeY' : 800
            }
        }
    },
    'veelken': {
        'current' : {
            'AHtoMuTau' : '2010Dec01',
            'ZtoMuTau' : '2010Nov26ii',
            'ZtoMuTau_bgEstTemplate' : '2010Nov26ii',
            'ZtoMuTau_tauIdEff' : '2010Dec01iii'
        },
        'jobs' : {
            '2010Dec01' : {
                'AHtoMuTau' : {
                    'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/plots/AHtoMuTau/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/AHtoMuTau/",
                    'tmpFilePath' : "/data1/veelken/tmp/",
                    'jobId' : "2010Dec01",
                    # Directory containing selected events
                    'pickevents' : '/data1/friis/Run26'
                },
            },
            '2010Nov26ii' : {
                'ZtoMuTau' : {
                    'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/plots/ZtoMuTau/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau/",
                    'tmpFilePath' : "/data1/veelken/tmp/",
                },
                'ZtoMuTau_bgEstTemplate' : {
                    'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/",
                    'tmpFilePath' : "/data1/veelken/tmp/ZtoMuTau_bgEstTemplate/",
                    'jobId' : "2010Nov26ii"
                },
            },
            '2010Dec01iii': {
                'ZtoMuTau_tauIdEff' : {
                    'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/plots/ZtoMuTau_tauIdEff/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau_tauIdEff/",
                    'tmpFilePath' : "/data1/veelken/tmp/ZtoMuTau_tauIdEff/",
                    'jobId' : "2010Dec01ii"
                },
            },
            '2010Nov19' : {
                'ZtoDiTau' : {
                    'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/plots/ZtoDiTau/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/ZtoDiTau/",
                    'tmpFilePath' : "/data1/veelken/tmp/",
                },
            },
        },
        'global' : {
            'drawOptions' : {
                'canvasSizeX' : 800,
                'canvasSizeY' : 640
            }
        }
    },
    'jkolb': {
        'current' : {
            'AHtoElecTau' : '7TeV',
            'ZtoElecTau' : '2010Nov13',
            'ZtoElecTau_stdCuts' : '2010Nov24',
        },
        'jobs' : {
            '7TeV' : {
                'AHtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/HtoElecTau/",
                    'harvestingFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/HtoElecTau/",
                    'tmpFilePath' : "/tmp/jkolb/",
                },
            },
            '2010Nov13' : {
                'ZtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/current/",
                    'harvestingFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/current/",
                    'tmpFilePath' : "/tmp/jkolb/",
                },
            },
            '2010Nov24' : {
                'ZtoElecTau_stdCuts' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/stdCuts/",
                    'harvestingFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/stdCuts/",
                    'tmpFilePath' : "/tmp/jkolb/",
                },
            },
        },
        'global' : {
            'drawOptions' : {
                'canvasSizeX' : 800,
                'canvasSizeY' : 640
            }
        }
    }
}

def mine():
    return userSettings[os.environ['LOGNAME']]

def getJobId(channel):
    # Get the current job id for this channel
    jobIdInfo = mine()['current']
    # Check if we need to remap the job ids for different channels.
    if isinstance(jobIdInfo, dict):
        return jobIdInfo[channel]
    else:
        return jobIdInfo


def getInfo(channel):
    # Get the current job id for this channel
    jobid = getJobId(channel)
    # Get the channel information for the jobId
    return mine()['jobs'][jobid][channel]

def overrideJobId(channel, jobId):
    # Set the job ID to something else temporarily
    mine()['current'] = jobId

def getPickEventsPath(channel):
    return  getInfo(channel)['pickevents']

def getAnalysisFilePath(channel):
    return getInfo(channel)['analysisFilePath']

def getHarvestingFilePath(channel):
    user_settings = getInfo(channel)
    harvestingFilePath = os.path.join(
        user_settings['harvestingFilePath'], getJobId(channel))
    if not os.path.exists(harvestingFilePath):
        os.makedirs(harvestingFilePath)
    if not os.path.isdir(harvestingFilePath):
        print "WARNING: Harvesting file path %s is not a directory!"
    return harvestingFilePath

def getLocalHarvestingFilePath(channel):
    return os.path.join(getHarvestingFilePath(channel), 'local')

def getTmpFilePath(channel):
    return getInfo(channel)['tmpFilePath']

def getConfigFileName(channel):
    return getInfo(channel)['configFileName']
