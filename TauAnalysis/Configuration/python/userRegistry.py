import FWCore.ParameterSet.Config as cms

import os

userSettings = {
    'friis' : {
        'AHtoMuTau' : {
            'analysisFilePath' : '/user/f/friis/AHtoMuTauNov21/',
            'harvestingFilePath' : "/data1/friis/",
            'tmpFilePath' : "/tmp/friis/",
            'jobId' : 'Run24',
            # Directory containing selected events
            'pickevents' : '/data1/friis/Run24/',
        },
        'global' : {
            'drawOptions' : {
                'canvasSizeX' : 640,
                'canvasSizeY' : 800
            }
        }
    },
    'veelken': {
        'AHtoMuTau' : {
            'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/plots/AHtoMuTau/",
            'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/AHtoMuTau/",
            'tmpFilePath' : "/tmp/veelken/",
            'jobId' : "7TeV"
        },
        'ZtoMuTau' : {
            'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/plots/ZtoMuTau/",
            'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau/",
            'tmpFilePath' : "/tmp/veelken/",
            'jobId' : "2010Nov13"
        },
        'ZtoMuTau_bgEstTemplate' : {
            'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/",
            'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/",
            'tmpFilePath' : "/tmp/veelken/ZtoMuTau_bgEstTemplate/",
            'jobId' : "2010Nov14"
        },
        'ZtoMuTau_tauIdEff' : {
            'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/plots/ZtoMuTau_tauIdEff/",
            'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau_tauIdEff/",
            'tmpFilePath' : "/tmp/veelken/ZtoMuTau_tauIdEff/",
            'jobId' : "2010Nov01"
        },
        'ZtoDiTau' : {
            'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/plots/ZtoDiTau/",
            'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/ZtoDiTau/",
            'tmpFilePath' : "/tmp/veelken/",
            'jobId' : "2010Nov13"
        },
        'global' : {
            'drawOptions' : {
                'canvasSizeX' : 800,
                'canvasSizeY' : 640
            }
        }
    }
}

def overrideJobId(channel, jobId):
    # Set the job ID to something else temporarily
    userSettings[os.environ['LOGNAME']][channel]['jobId'] = jobId

def getPickEventsPath(channel):
    return  userSettings[os.environ['LOGNAME']][channel]['pickevents']

def getAnalysisFilePath(channel):
    return userSettings[os.environ['LOGNAME']][channel]['analysisFilePath']

def getHarvestingFilePath(channel):
    user_settings = userSettings[os.environ['LOGNAME']][channel]
    harvestingFilePath = os.path.join(
        user_settings['harvestingFilePath'], user_settings['jobId'])
    if not os.path.exists(harvestingFilePath):
        os.makedirs(harvestingFilePath)
    if not os.path.isdir(harvestingFilePath):
        print "WARNING: Harvesting file path %s is not a directory!"
    return harvestingFilePath

def getTmpFilePath(channel):
    return userSettings[os.environ['LOGNAME']][channel]['tmpFilePath']

def getJobId(channel):
    return userSettings[os.environ['LOGNAME']][channel]['jobId']


