import os

userSettings = {
    'squires' : {
        # The job ID to use per default.
        'current' : 'Run29',
        # List of runs
        'jobs' : {
            'Run29' : {
                'AHtoMuTau' : {
                    'analysisFilePath' : '/user/s/squires/tau_3_8_7/',
                    'harvestingFilePath' : "/data2/squires/",
                    'tmpFilePath' : "/data2/squires/tmp/",
                    'pickevents' : '/data2/squires/Run29'
                }
            },
            'Run32' : {
                'AHtoMuTau' : {
                    # The output directory on castor
                    'analysisFilePath' : '/user/f/squires/Run32/',
                    # The output directory for the plots
                    'harvestingFilePath' : "/data1/squires/",
                    'tmpFilePath' : "/data2/squires/tmp/",
                    'pickevents' : '/data1/squires/Run26',
                }
            },
            'Run32onskim' : {
                'AHtoMuTau' : {
                    # The output directory on castor
                    'analysisFilePath' : '/castor/cern.ch/user/f/squires/Run32onskim/',
                    # The output directory for the plots
                    'harvestingFilePath' : "/data1/squires/",
                    'tmpFilePath' : "/data2/squires/tmp/",
                    'pickevents' : '/castor/cern.ch/user/f/squires/Run32harvest/',
                    'skimPath' : '/castor/cern.ch/user/f/squires/Run32harvest/',
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
    'friis' : {
        # The job ID to use per default.
        'current' : 'Run32onskim',
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
                    #'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run32/',
                    # The output directory for the plots
                    'harvestingFilePath' : "/data1/friis/",
                    'tmpFilePath' : "/data2/friis/tmp/",
                    # Where to store the harvested histograms on lxbatch
                    'batchHarvest' : "/castor/cern.ch/user/f/friis/Run32harvest",
                    'pickevents' : '/data1/friis/Run26',
                }
            },
            'Run32onskim' : {
                'AHtoMuTau' : {
                    # The output directory on castor
                    'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run32onskim/',
                    # The output directory for the plots
                    'harvestingFilePath' : "/data1/friis/",
                    'tmpFilePath' : "/data2/friis/tmp/",
                    'batchHarvest' : '/castor/cern.ch/user/f/friis/Run32onskimTmp/',
                    # The source that paroduced the skim
                    'skimSource' : 'Run32',
                }
            },
            'Run33' : {
                'AHtoMuTau' : {
                    # The output directory on castor
                    'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run33/',
                    # The output directory for the plots
                    'harvestingFilePath' : "/data1/friis/",
                    'tmpFilePath' : "/data2/friis/tmp/",
                    'batchHarvest' :  '/castor/cern.ch/user/f/friis/Run33harvest/',
                    # The source that paroduced the skim
                    'skimSource' : 'Run32',
                }
            },
            'Run33FR' : {
                'AHtoMuTau' : {
                    # The output directory on castor
                    'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run33FR/',
                    # The output directory for the plots
                    'harvestingFilePath' : "/data1/friis/",
                    'tmpFilePath' : "/data2/friis/tmp/",
                    'batchHarvest' :  '/castor/cern.ch/user/f/friis/Run33FRharvest/',
                    # The source that paroduced the skim
                    'skimSource' : 'Run32',
                }
            },
            'Run33SYS' : {
                'AHtoMuTau' : {
                    # The output directory on castor
                    'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run33SYS/',
                    # The output directory for the plots
                    'harvestingFilePath' : "/data1/friis/",
                    #'harvestingFilePath' : "/tmp/friis/",
                    'tmpFilePath' : "/data2/friis/tmp/",
                    'batchHarvest' : '/castor/cern.ch/user/f/friis/Run33SYSharvest/',
                    # The job ID of the skim producer
                    'skimSource' : 'Run32',
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
            'ZtoMuTau_bgEstTemplate' : '2010Dec08',
            'ZtoMuTau_tauIdEff' : '2010Dec01iii'
        },
        'jobs' : {
            '2010Dec09' : {
                'AHtoMuTau' : {
                    'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/plots/AHtoMuTau/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/AHtoMuTau/",
                    'tmpFilePath' : "/data1/veelken/tmp/",
                    'jobId' : "2010Dec09"
                }
            },
            '2010Dec08' : {
                'ZtoMuTau_bgEstTemplate' : {
                    'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/",
                    'tmpFilePath' : "/data1/veelken/tmp/ZtoMuTau_bgEstTemplate/",
                    'jobId' : "2010Dec08"
                }
            },
            '2010Dec01' : {
                'AHtoMuTau' : {
                    'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/plots/AHtoMuTau/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/AHtoMuTau/",
                    'tmpFilePath' : "/data1/veelken/tmp/",
                    'jobId' : "2010Dec01",
                    # Directory containing selected events
                    'pickevents' : '/data1/friis/Run26'
                }
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
                }
            },
            '2010Dec01iii': {
                'ZtoMuTau_tauIdEff' : {
                    'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/plots/ZtoMuTau_tauIdEff/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau_tauIdEff/",
                    'tmpFilePath' : "/data1/veelken/tmp/ZtoMuTau_tauIdEff/",
                    'jobId' : "2010Dec01ii"
                }
            },
            '2010Nov19' : {
                'ZtoDiTau' : {
                    'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/plots/ZtoDiTau/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/ZtoDiTau/",
                    'tmpFilePath' : "/data1/veelken/tmp/",
                }
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
            'AHtoElecTau' : 'Run01',
            'ZtoElecTau' : '2010Nov13',
            'ZtoElecTau_stdCuts' : '2010Nov24',
        },
        'jobs' : {
            'Run01' : {
                'AHtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/HtoElecTau/",
                    'harvestingFilePath' : "/data/ndpc2/b/jkolb/ZtoElecTauAnalysis/fall10/analysis/harvested/stdCuts",
                    'tmpFilePath' : "/data/ndpc2/b/jkolb/ZtoElecTauAnalysis/fall10/analysis/harvested/stdCuts",
                },
                'ZtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/stdCuts/",
                    'harvestingFilePath' : "/data/ndpc2/b/jkolb/ZtoElecTauAnalysis/fall10/analysis/harvested/stdCuts",
                    'tmpFilePath' : "/data/ndpc2/b/jkolb/ZtoElecTauAnalysis/fall10/analysis/harvested/stdCuts",
                }
            },
            '2010Nov13' : {
                'ZtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/current/",
                    'harvestingFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/current/",
                    'tmpFilePath' : "/tmp/jkolb/",
                }
            },
            '2010Nov24' : {
                'ZtoElecTau_stdCuts' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/stdCuts/",
                    'harvestingFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/stdCuts/",
                    'tmpFilePath' : "/tmp/jkolb/",
                }
            }
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


def getInfo(channel, jobid = None):
    # Get the current job id for this channel
    if jobid is None:
        jobid = getJobId(channel)
    # Get the channel information for the jobId
    return mine()['jobs'][jobid][channel]

def overrideJobId(channel, jobId):
    # Set the job ID to something else temporarily
    mine()['current'] = jobId

def getPickEventsPath(channel):
    return  getInfo(channel)['pickevents']

def getSkimEvents(channel, jobid = None):
    " Get the directory on castor that containing the skimmed files "
    skimJobId = getInfo(channel, jobid)['skimSource']
    # Get the location of the harvested & merged skims
    #output_path = os.path.normpath(
    #    '/castor/cern.ch/' + getInfo(channel, skimJobId)['batchHarvest'])
    output_path = os.path.normpath(
         getInfo(channel, skimJobId)['batchHarvest'])
    return output_path

def getAnalysisFilePath(channel):
    return getInfo(channel)['analysisFilePath']

def getHarvestingFilePath(channel, jobid=None):
    user_settings = getInfo(channel)
    harvestingFilePath = os.path.join(
        user_settings['harvestingFilePath'], getJobId(channel))
    if not os.path.exists(harvestingFilePath):
        try:
            os.makedirs(harvestingFilePath)
        except:
            print "Failed to make harvesting file path: %s" % harvestingFilePath
    if not os.path.isdir(harvestingFilePath):
        print "WARNING: Harvesting file path %s is not a directory!"
    return harvestingFilePath

def getLocalHarvestingFilePath(channel):
    return os.path.join(getHarvestingFilePath(channel), 'local')

def getTmpFilePath(channel):
    return getInfo(channel)['tmpFilePath']

def getBatchHarvestLocation(channel):
    " Where to store the output histograms when harvesting on LXBatch "
    return getInfo(channel)['batchHarvest']

def getConfigFileName(channel):
    return getInfo(channel)['configFileName']
