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
            # Loose skim
            'Run32' : {
                'AHtoMuTau' : {
                    # The output directory on castor
                    'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run32/',
                    #'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run32/',
                    # The output directory for the plots
                    'harvestingFilePath' : "/data1/friis/",
                    'tmpFilePath' : "/data2/friis/tmp/",
                    # Where to store the harvested histograms on lxbatch
                    'batchHarvest' : "/castor/cern.ch/user/f/friis/Run32harvest/",
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
                    # Just for testing
                    'pickevents' : '/data1/friis/Run26',
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
            # Another skim, only with Loose TaNC and Muon Iso turned off
            'Run35' : {
                'AHtoMuTau' : {
                    # The output directory on castor
                    'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run35/',
                    #'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run32/',
                    # The output directory for the plots
                    'harvestingFilePath' : "/data1/friis/",
                    'tmpFilePath' : "/data2/friis/tmp/",
                    # Where to store the harvested histograms on lxbatch
                    'batchHarvest' : "/castor/cern.ch/user/f/friis/Run35harvest/",
                }
            },
            # Rerunning the merging in a separate job
            'Run35A' : {
                'AHtoMuTau' : {
                    # The output directory on castor
                    'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run35/',
                    #'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run32/',
                    # The output directory for the plots
                    'harvestingFilePath' : "/data1/friis/",
                    'tmpFilePath' : "/data2/friis/tmp/",
                    # Where to store the harvested histograms on lxbatch
                    'batchHarvest' : "/castor/cern.ch/user/f/friis/Run35Aharvest/",
                }
            },
            'Run35SYS' : {
                'AHtoMuTau' : {
                    # The output directory on castor
                    'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run35SYS/',
                    #'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run32/',
                    # The output directory for the plots
                    'harvestingFilePath' : "/data2/friis/",
                    'tmpFilePath' : "/data2/friis/tmp/",
                    # Where to store the harvested histograms on lxbatch
                    'batchHarvest' : "/castor/cern.ch/user/f/friis/Run35SYSharvest/",
                    'skimSource' : 'Run35',
                }
            },
            'Run35SYSloose' : {
                'AHtoMuTau' : {
                    # The output directory on castor
                    'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run35SYSloose/',
                    #'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run32/',
                    # The output directory for the plots
                    'harvestingFilePath' : "/data2/friis/",
                    'tmpFilePath' : "/data2/friis/tmp/",
                    # Where to store the harvested histograms on lxbatch
                    'batchHarvest' : "/castor/cern.ch/user/f/friis/Run35SYSlooseharvest/",
                    'skimSource' : 'Run35',
                }
            },
            'Run35SYStight' : {
                'AHtoMuTau' : {
                    # The output directory on castor
                    'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run35SYStight/',
                    #'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run32/',
                    # The output directory for the plots
                    'harvestingFilePath' : "/data2/friis/",
                    'tmpFilePath' : "/data2/friis/tmp/",
                    # Where to store the harvested histograms on lxbatch
                    'batchHarvest' : "/castor/cern.ch/user/f/friis/Run35SYStightharvest/",
                    'skimSource' : 'Run35',
                }
            },
            'Run35test' : {
                'AHtoMuTau' : {
                    # The output directory on castor
                    'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run35test/',
                    #'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run32/',
                    # The output directory for the plots
                    'harvestingFilePath' : "/data2/friis/",
                    'tmpFilePath' : "/data2/friis/tmp/",
                    # Where to store the harvested histograms on lxbatch
                    'batchHarvest' : "/castor/cern.ch/user/f/friis/Run35testharvest/",
                    'skimSource' : 'Run35',
                }
            },
            # Rerun, with tau ID | or'red with loose and lead track pt off
            'Run36' : {
                'AHtoMuTau' : {
                    # The output directory on castor
                    'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run36/',
                    #'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run32/',
                    # The output directory for the plots
                    'harvestingFilePath' : "/data2/friis/",
                    'tmpFilePath' : "/data2/friis/tmp/",
                    # Where to store the harvested histograms on lxbatch
                    'batchHarvest' : "/castor/cern.ch/user/f/friis/Run36harvest/",
                }
            },
            # Rerun final?, with MT cut loosened to work with Mike's
            'Run37' : {
                'AHtoMuTau' : {
                    # The output directory on castor
                    'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run37/',
                    #'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run32/',
                    # The output directory for the plots
                    'harvestingFilePath' : "/data2/friis/",
                    'tmpFilePath' : "/data2/friis/tmp/",
                    # Where to store the harvested histograms on lxbatch
                    'batchHarvest' : "/castor/cern.ch/user/f/friis/Run37harvest/",
                }
            },
            # Run with final? analysis cuts, but with tau ID turned off for fake
            # rate.
            'Run39' : {
                'AHtoMuTau' : {
                    # The output directory on castor
                    'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run39/',
                    #'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run32/',
                    # The output directory for the plots
                    'harvestingFilePath' : "/data2/friis/",
                    'tmpFilePath' : "/data2/friis/tmp/",
                    # Where to store the harvested histograms on lxbatch
                    'batchHarvest' : "/castor/cern.ch/user/f/friis/Run39harvest/",
                }
            },
        },
        'global' : {
            'drawOptions' : {
                'canvasSizeX' : 640,
                'canvasSizeY' : 800
            },
            'harvestScripts' : '/tmp/friis/harvest_scripts',
        }
    },
    'veelken': {
        'current' : {
            'AHtoMuTau'              : '2010Dec23',
            'ZtoMuTau'               : '2011Jan07',
            'ZtoMuTau_bgEstTemplate' : '2011Jan10',
            'ZtoMuTau_tauIdEff'      : '2011Jan17'
        },
        'jobs' : {
            '2011Jan17' : {
                'ZtoMuTau_tauIdEff' : {
                    'analysisFilePath' : "/castor/cern.ch/user/v/veelken/CMSSW_3_8_x/skims/TauIdEffMeas/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/skims/ZtoMuTau_tauIdEff/",
                    'tmpFilePath' : "/data1/veelken/tmp/ZtoMuTau_tauIdEff/",
                     # Directory containing selected events
                    'pickevents' : '/castor/cern.ch/user/f/friis/fixme_delete/'
                }
            }, 
            '2011Jan12_lxbatch' : {
                'ZtoMuTau_tauIdEff' : {
                    'analysisFilePath' : "/castor/cern.ch/user/v/veelken/CMSSW_3_8_x/skims/TauIdEffMeas/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/skims/ZtoMuTau_tauIdEff/",
                    'tmpFilePath' : "/data1/veelken/tmp/ZtoMuTau_tauIdEff/",
                     # Directory containing selected events
                    'pickevents' : '/castor/cern.ch/user/f/friis/fixme_delete/'
                }
            },
            '2011Jan10' : {
                'ZtoMuTau_bgEstTemplate' : {
                    'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/",
                    'tmpFilePath' : "/data1/veelken/tmp/ZtoMuTau_bgEstTemplate/",
                    'jobId' : "2011Jan10"
                }
            },
	    '2011Jan09_lxbatch' : {
                'ZtoMuTau_tauIdEff' : {
                    'analysisFilePath' : "/castor/cern.ch/user/v/veelken/CMSSW_3_8_x/skims/TauIdEffMeas/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/skims/ZtoMuTau_tauIdEff/",
                    'tmpFilePath' : "/data1/veelken/tmp/ZtoMuTau_tauIdEff/",
                     # Directory containing selected events
                    'pickevents' : '/castor/cern.ch/user/f/friis/fixme_delete/'
                }
            },
            '2011Jan07' : {
                'ZtoMuTau' : {
                    'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/plots/ZtoMuTau/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau/",
                    'tmpFilePath' : "/data1/veelken/tmp/ZtoMuTau/",
                    'jobId' : "2011Jan07"
                }
            },
            '2011Jan06_lxbatch' : {
                'ZtoMuTau_tauIdEff' : {
                    'analysisFilePath' : "/castor/cern.ch/user/v/veelken/CMSSW_3_8_x/skims/TauIdEffMeas/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/skims/ZtoMuTau_tauIdEff/",
                    'tmpFilePath' : "/data1/veelken/tmp/ZtoMuTau_tauIdEff/",
                    # Directory containing selected events
                    'pickevents' : '/castor/cern.ch/user/f/friis/fixme_delete/'
                }
            }, 
            '2011Jan05ii' : {
                'ZtoMuTau' : {
                    'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/plots/ZtoMuTau/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau/",
                    'tmpFilePath' : "/data1/veelken/tmp/ZtoMuTau/",
                    'jobId' : "2011Jan05ii"
                }
            },
            '2010Dec31' : {
                'ZtoMuTau_tauIdEff' : {
                    'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/skims/TauIdEffMeas/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/skims/ZtoMuTau_tauIdEff/",
                    'tmpFilePath' : "/data1/veelken/tmp/ZtoMuTau_tauIdEff/",
                    'jobId' : "2010Dec31"
                }
            },
            '2010Dec27' : {
                'AHtoMuTau' : {
                    'analysisFilePath' : "/castor/cern.ch/user/v/veelken/CMSSW_3_8_x/plots/AHtoMuTau/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/AHtoMuTau/",
                    'tmpFilePath' : "/data1/veelken/tmp/AHtoMuTau/",
                    'jobId' : "2010Dec27",
                    # Where to store the harvested histograms on lxbatch
                    'batchHarvest' : "/castor/cern.ch/user/v/veelken/CMSSW_3_8_x/plots/AHtoMuTau_lxbatch/"
                }
            },
            '2010Dec23_lxbatch' : {
                'AHtoMuTau' : {
                    'analysisFilePath' : "/castor/cern.ch/user/v/veelken/CMSSW_3_8_x/plots/AHtoMuTau/2010Dec23_lxbatch/analysis/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/AHtoMuTau/",
                    'tmpFilePath' : "/data1/veelken/tmp/AHtoMuTau/",
                    'jobId' : "2010Dec23_lxbatch",
                    # Where to store the harvested histograms on lxbatch
                    'batchHarvest' : "/castor/cern.ch/user/v/veelken/CMSSW_3_8_x/plots/AHtoMuTau/2010Dec23_lxbatch/harvesting/",
                    'skimSource' : '2010Dec27'
                }
            },
            '2010Dec23' : {
                'AHtoMuTau' : {
                    'analysisFilePath' : "/castor/cern.ch/user/v/veelken/CMSSW_3_8_x/plots/AHtoMuTau/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/AHtoMuTau/",
                    'tmpFilePath' : "/data1/veelken/tmp/AHtoMuTau/",
                    'jobId' : "2010Dec23",
                    # Where to store the harvested histograms on lxbatch
                    'batchHarvest' : "/castor/cern.ch/user/v/veelken/CMSSW_3_8_x/plots/AHtoMuTau_lxbatch/"
                }
            },
            '2010Dec22' : {
                'ZtoMuTau_bgEstTemplate' : {
                    'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/",
                    'tmpFilePath' : "/data1/veelken/tmp/ZtoMuTau_bgEstTemplate/",
                    'jobId' : "2010Dec22"
                }
            },
            '2010Dec17' : {
                'ZtoMuTau_bgEstTemplate' : {
                    'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/",
                    'tmpFilePath' : "/data1/veelken/tmp/ZtoMuTau_bgEstTemplate/",
                    'jobId' : "2010Dec17"
                }
            },
            '2010Dec14ii' : {
                'ZtoMuTau' : {
                    'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/plots/ZtoMuTau/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau/",
                    'tmpFilePath' : "/data1/veelken/tmp/ZtoMuTau/",
                    'jobId' : "2010Dec14ii"
                },
                'ZtoMuTau_bgEstTemplate' : {
                    'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/",
                    'tmpFilePath' : "/data1/veelken/tmp/ZtoMuTau_bgEstTemplate/",
                    'jobId' : "2010Dec14ii"
                }
            },
	    '2010Dec04' : {
                'ZtoMuTau_tauIdEff' : {
                    'analysisFilePath' : "/user/v/veelken/CMSSW_3_8_x/skims/TauIdEffMeas/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_3_8_x/skims/ZtoMuTau_tauIdEff/",
                    'tmpFilePath' : "/data1/veelken/tmp/ZtoMuTau_tauIdEff/",
                    'jobId' : "2010Dec04",
                    # Directory containing selected events
                    #'pickevents' : '/data1/veelken/CMSSW_3_8_x/skims/ZtoMuTau_tauIdEff/2010Dec04'
                }
            },
            # References to skims run by Evan
            'Run35test' : {
                'AHtoMuTau' : {
                    # The output directory on castor
                    'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run35test/',
                    #'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run32/',
                    # The output directory for the plots
                    'harvestingFilePath' : "/data2/friis/",
                    'tmpFilePath' : "/data2/friis/tmp/",
                    # Where to store the harvested histograms on lxbatch
                    'batchHarvest' : "/castor/cern.ch/user/f/friis/Run35testharvest/",
                    'skimSource' : 'Run35',
                }
            },
            'Run35' : {
                'AHtoMuTau' : {
                    # The output directory on castor
                    'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run35/',
                    #'analysisFilePath' : '/castor/cern.ch/user/f/friis/Run32/',
                    # The output directory for the plots
                    'harvestingFilePath' : "/data1/friis/",
                    'tmpFilePath' : "/data2/friis/tmp/",
                    # Where to store the harvested histograms on lxbatch
                    'batchHarvest' : "/castor/cern.ch/user/f/friis/Run35harvest/",
                }
            }
        },
        'global' : {
            'drawOptions' : {
                'canvasSizeX' : 800,
                'canvasSizeY' : 640
            },
            'harvestScripts' : '/tmp/veelken/harvest_scripts',
        }
    },
    'jkolb': {
        'current' : {
            'AHtoElecTau' : 'Run03',
            'ZtoElecTau' : 'Run03',
        },
        'jobs' : {
            'Run03' : {
                'AHtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/AHtoElecTau/Run03/",
                    'harvestingFilePath' : "/data/ndpc2/b/jkolb/ZtoElecTauAnalysis/fall10/AHtoElecTau/harvested/Run03/",
                    'tmpFilePath' : "/data/ndpc2/b/jkolb/ZtoElecTauAnalysis/fall10/AHtoElecTau/harvested/Run03/tmp/"
                },
                'ZtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/ZtoElecTau/Run03/",
                    'harvestingFilePath' : "/data/ndpc2/b/jkolb/ZtoElecTauAnalysis/fall10/ZtoElecTau/Run03/",
                    'tmpFilePath' : "/data/ndpc2/b/jkolb/ZtoElecTauAnalysis/fall10/ZtoElecTau/Run03/tmp/"
                }
            },
            'Run02' : {
                'AHtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/AHtoElecTau/Run02",
                    'harvestingFilePath' : "/data/ndpc2/b/jkolb/ZtoElecTauAnalysis/fall10/analysis/harvested/",
                    'tmpFilePath' : "/data/ndpc2/b/jkolb/ZtoElecTauAnalysis/fall10/analysis/harvested/tmp"
                },
                'ZtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/stdCuts/",
                    'harvestingFilePath' : "/data/ndpc2/b/jkolb/ZtoElecTauAnalysis/fall10/analysis/harvested/stdCuts",
                    'tmpFilePath' : "/data/ndpc2/b/jkolb/ZtoElecTauAnalysis/fall10/analysis/harvested/stdCuts"
                }
            },
            'Run01' : {
                'AHtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/AHtoElecTau/",
                    'harvestingFilePath' : "/data/ndpc2/b/jkolb/ZtoElecTauAnalysis/fall10/analysis/harvested/stdCuts",
                    'tmpFilePath' : "/data/ndpc2/b/jkolb/ZtoElecTauAnalysis/fall10/analysis/harvested/stdCuts",
                },
                'ZtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/stdCuts/",
                    'harvestingFilePath' : "/data/ndpc2/b/jkolb/ZtoElecTauAnalysis/fall10/analysis/harvested/stdCuts",
                    'tmpFilePath' : "/data/ndpc2/b/jkolb/ZtoElecTauAnalysis/fall10/analysis/harvested/stdCuts",
                }
            }
        },
        'global' : {
            'drawOptions' : {
                'canvasSizeX' : 800,
                'canvasSizeY' : 640
            },
            'harvestScripts' : '/tmp/jkolb/harvest_scripts',
        }
    },
    'liis': {
        'current' : 'RunPileup',
        'jobs' : {
            'RunPileup' : {
                'WtoTauNu' : {
                    'analysisFilePath' : "/castor/cern.ch/user/l/liis/CMSSW_38X/Histograms/Pileup/run2",
                    'harvestingFilePath' : "/afs/cern.ch/user/l/liis/scratch0",
                    'tmpFilePath' : "/tmp/liis",
                    'jobId' : "RunTest",
                    }
                },
            'Run01' : {
                'WtoTauNu' : {
                    'analysisFilePath' : "/castor/cern.ch/user/l/liis/CMSSW_38X/Histograms/HPSTau",
                    'harvestingFilePath' : "/tmp/liis",
                    'tmpFilePath' : "/tmp/liis",
                    'jobID' : "Run01",
                    }
                }
            },
        'global' : {
            'drawOptions' : {
                'canvasSizeX' : 800,
                'canvasSizeY' : 640
                },
#            'harvestScripts' : 'tmp/liis/harvest_scripts'
            }
        },'
        lusito': {
        'current' : {
            'ZtoMuTau'               : '2011Feb15',
            'ZtoMuTau_bgEstTemplate' : '2011Feb15'
        },
        'jobs' : {
            '2011Feb15' : {
                'ZtoMuTau' : {
                    'analysisFilePath' : "/castor/cern.ch/user/l/lusito/CMSSW_3_8_x/plots15feb/ZtoMuTau/",
                    'harvestingFilePath' : "/tmp/lusito/CMSSW_3_8_x/plots15feb/ZtoMuTau_tmp/",
                    'tmpFilePath' : "/tmp/lusito/CMSSW_3_8_x/plots15feb/ZtoMuTau/",
                     # Directory containing selected events
                    #'pickevents' : '/castor/cern.ch/user/f/friis/fixme_delete/'
                },
               'ZtoMuTau_bgEstTemplate' : {
                    'analysisFilePath' : "/castor/cern.ch/user/l/lusito/CMSSW_3_8_x/plots15feb/ZtoMuTau_bgEstTemplate/",
                    'harvestingFilePath' : "/tmp/lusito/CMSSW_3_8_x/plots15feb/ZtoMuTau_bgEstTemplate_tmp/",
                    'tmpFilePath' : "/tmp/lusito/CMSSW_3_8_x/plots15feb/ZtoMuTau_bgEstTemplate/",
                     # Directory containing selected events
                    #'pickevents' : '/castor/cern.ch/user/f/friis/fixme_delete/'
                }, 
            }, 
    
    
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

def check_slash(dir):
    " Make sure a directory has a trailing slash "
    if not dir.endswith('/'):
        dir += '/'
    return dir

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
    return getInfo(channel)['pickevents']

def getHarvestScriptLocation():
    " Get location of harvest scripts "
    return mine()['global']['harvestScripts']

def getSkimEvents(channel, jobid = None):
    " Get the directory on castor that containing the skimmed files "
    skimJobId = getInfo(channel, jobid)['skimSource']
    # Get the location of the harvested & merged skims
    #output_path = os.path.normpath(
    #    '/castor/cern.ch/' + getInfo(channel, skimJobId)['batchHarvest'])
    output_path = os.path.normpath(
         getInfo(channel, skimJobId)['batchHarvest'])
    return check_slash(output_path)

def getAnalysisFilePath(channel):
    return getInfo(channel)['analysisFilePath']

def getHarvestingFilePath(channel, jobid=None):
    user_settings = getInfo(channel, jobid)
    if jobid is None:
        jobid = getJobId(channel)
    harvestingFilePath = os.path.join(
        user_settings['harvestingFilePath'], jobid)
    if not os.path.exists(harvestingFilePath):
        try:
            os.makedirs(harvestingFilePath)
        except:
            print "Failed to make harvesting file path: %s" % harvestingFilePath
    if not os.path.isdir(harvestingFilePath):
        print "WARNING: Harvesting file path %s is not a directory!" % harvestingFilePath
    return check_slash(harvestingFilePath)

def makeSkimStatFileMapper(channel, jobid=None):
    skimJobId = getInfo(channel, jobid)['skimSource']
    print skimJobId
    skim_path = getHarvestingFilePath(channel, skimJobId)
    print skim_path
    def mapper(sample):
        return os.path.join(skim_path, 'harvested_%s_%s_%s.root' %
                            (channel, sample, skimJobId))
    return mapper

def getLocalHarvestingFilePath(channel):
    return os.path.join(getHarvestingFilePath(channel), 'local')

def getTmpFilePath(channel):
    return getInfo(channel)['tmpFilePath']

def getBatchHarvestLocation(channel):
    " Where to store the output histograms when harvesting on LXBatch "
    output = getInfo(channel)['batchHarvest']
    # Add a trailing slash if we don't have one
    return check_slash(output)

def getConfigFileName(channel):
    return getInfo(channel)['configFileName']
