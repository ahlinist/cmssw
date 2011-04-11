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
            'ZtoMuTau'          : '2011Apr09_HPSloose',
            'ZtoMuTau_tauIdEff' : '2011Apr09_HPSloose'
        },
        'jobs' : {
            '2011Apr09_HPSloose' : {
                'ZtoMuTau' : {
                    'analysisFilePath' : "/castor/cern.ch/user/v/veelken/CMSSW_4_1_x/plots/ZtoMuTau/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_4_1_x/plots/ZtoMuTau/",
                    'tmpFilePath' : "/data2/veelken/tmp/ZtoMuTau/",
                    'jobId' : "2011Apr09_HPSloose",
                    'batchHarvest' : "/castor/cern.ch/user/v/veelken/CMSSW_4_1_x/harvesting/ZtoMuTau/2011Apr09_HPSloose"
                },
                'ZtoMuTau_tauIdEff' : {
                    'analysisFilePath' : "/castor/cern.ch/user/v/veelken/CMSSW_4_1_x/skims/TauIdEffMeas/",
                    'harvestingFilePath' : "/data2/veelken/CMSSW_4_1_x/skims/ZtoMuTau_tauIdEff/",
                    'tmpFilePath' : "/data1/veelken/tmp/ZtoMuTau_tauIdEff/",
                    # Directory containing selected events
                    'pickevents' : "/castor/cern.ch/user/v/veelken/CMSSW_4_1_x/skims/TauIdEffMeas/"
                }
            },
            '2011Apr05_HPSloose' : {
                'ZtoMuTau' : {
                    'analysisFilePath' : "/castor/cern.ch/user/v/veelken/CMSSW_4_1_x/plots/ZtoMuTau/",
                    'harvestingFilePath' : "/data1/veelken/CMSSW_4_1_x/plots/ZtoMuTau/",
                    'tmpFilePath' : "/data2/veelken/tmp/ZtoMuTau/",
                    'jobId' : "2011Apr05_HPSloose",
                    'batchHarvest' : "/castor/cern.ch/user/v/veelken/CMSSW_4_1_x/harvesting/ZtoMuTau/2011Apr05_HPSloose"
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
        'current' : 'Run15',
        'jobs' : {
			# fall10 MC with 156 BX PU, 2010 dataset (36/pb);  just run data; no electron iso
            'Run16' : {
                'ZtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/AHtoElecTau/Run16/",
                    'harvestingFilePath' : "/data/ndpc0/c/jkolb/TauResults",
                    'tmpFilePath' : "/data/ndpc0/c/jkolb/TauResults/tmp/"
                }
            },
			# fall10 MC with 156 BX PU, 2010 dataset (36/pb);  re-run data sample for match with Wisconsin
            'Run15' : {
                'ZtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/AHtoElecTau/Run15/",
                    'harvestingFilePath' : "/data/ndpc0/c/jkolb/TauResults",
                    'tmpFilePath' : "/data/ndpc0/c/jkolb/TauResults/tmp/"
                }
            },
			# fall10 MC with 156 BX PU, 2010 dataset (36/pb);  no Mt(e+MET) cut
            'Run14' : {
                'AHtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/AHtoElecTau/Run14/",
                    'harvestingFilePath' : "/data/ndpc0/c/jkolb/TauResults",
                    'tmpFilePath' : "/data/ndpc0/c/jkolb/TauResults/tmp/"
                }
            },
			# fall10 MC with PU, 2010 dataset (36/pb); same as Run12, but with systematics
            'Run13' : {
                'AHtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/AHtoElecTau/Run13/",
					'harvestingFilePath' : "/data/ndpc0/c/jkolb/TauResults/",
                    'tmpFilePath' : "/data/ndpc0/c/jkolb/TauResults/tmp/"
                },
                'ZtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/ZtoElecTau/Run13/",
                    'harvestingFilePath' : "/data/ndpc0/c/jkolb/TauResults",
                    'tmpFilePath' : "/data/ndpc0/c/jkolb/TauResults/tmp/"
                },
                'ZtoElecTau_bgEstTemplate' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/ZtoElecTau/Run13/",
                    'harvestingFilePath' : "/data/ndpc0/c/jkolb/TauResults",
                    'tmpFilePath' : "/data/ndpc0/c/jkolb/TauResults/tmp/"
                }
            },
			# fall10 MC with PU, 2010 dataset (36/pb); now using HPS loose
            'Run12' : {
                'AHtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/AHtoElecTau/Run12/",
					'harvestingFilePath' : "/data/ndpc0/c/jkolb/TauResults/",
                    'tmpFilePath' : "/data/ndpc0/c/jkolb/TauResults/tmp/"
                },
                'ZtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/ZtoElecTau/Run12/",
                    'harvestingFilePath' : "/data/ndpc0/c/jkolb/TauResults",
                    'tmpFilePath' : "/data/ndpc0/c/jkolb/TauResults/tmp/"
                },
                'ZtoElecTau_bgEstTemplate' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/ZtoElecTau/Run12/",
                    'harvestingFilePath' : "/data/ndpc0/c/jkolb/TauResults",
                    'tmpFilePath' : "/data/ndpc0/c/jkolb/TauResults/tmp/"
                }
            },
			# fall10 MC with PU, 2010 dataset (36/pb);  same as Run07, 
			# with no DOCA, deltaCotTheta conversion cuts, but keep missingInnerHits cut
            'Run11' : {
                'ZtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/ZtoElecTau/Run11/",
                    'harvestingFilePath' : "/data/ndpc0/c/jkolb/TauResults",
                    'tmpFilePath' : "/data/ndpc0/c/jkolb/TauResults/tmp/"
                }
            },
			# fall10 MC with PU, 2010 dataset (36/pb);  same as Run07, with no Mt(e+MET) cut
            'Run10' : {
                'AHtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/AHtoElecTau/Run10/",
                    'harvestingFilePath' : "/data/ndpc0/c/jkolb/TauResults",
                    'tmpFilePath' : "/data/ndpc0/c/jkolb/TauResults/tmp/"
                }
            },
			# fall10 MC with PU, 2010 dataset (36/pb);  same as Run07, with no conversion cut
            'Run09' : {
                'ZtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/ZtoElecTau/Run09/",
                    'harvestingFilePath' : "/data/ndpc0/c/jkolb/TauResults",
                    'tmpFilePath' : "/data/ndpc0/c/jkolb/TauResults/tmp/"
                }
            },
			# fall10 MC with PU, 2010 dataset (36/pb);  same as Run07, but allowing 1 missing inner hit
			#(conclusions: stick with zero missing inner hits (Run07) )
            'Run08' : {
                'ZtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/ZtoElecTau/Run08/",
                    'harvestingFilePath' : "/data/ndpc0/c/jkolb/TauResults",
                    'tmpFilePath' : "/data/ndpc0/c/jkolb/TauResults/tmp/"
                }
            },
			# fall10 MC with PU, 2010 dataset (36/pb);  new conversion veto, data event cleaning
			# AH files do not have full conversion plots
            'Run07' : {
                'AHtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/AHtoElecTau/Run07/",
					'harvestingFilePath' : "/data/ndpc0/c/jkolb/TauResults/",
                    'tmpFilePath' : "/data/ndpc0/c/jkolb/TauResults/tmp/"
                },
                'ZtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/ZtoElecTau/Run07/",
                    'harvestingFilePath' : "/data/ndpc0/c/jkolb/TauResults",
                    'tmpFilePath' : "/data/ndpc0/c/jkolb/TauResults/tmp/"
                }
            },
			# fall10 MC with PU, 2010 dataset (36/pb);  fixed bug in tau electron rejection, everything looks OK
            'Run06' : {
                'AHtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/AHtoElecTau/Run06/",
					'harvestingFilePath' : "/data/ndpc0/c/jkolb/TauResults/",
                    'tmpFilePath' : "/data/ndpc0/c/jkolb/TauResults/tmp/"
                },
                'ZtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/ZtoElecTau/Run06/",
                    'harvestingFilePath' : "/data/ndpc0/c/jkolb/TauResults",
                    'tmpFilePath' : "/data/ndpc0/c/jkolb/TauResults/tmp/"
                }
            },
			# fall10 MC with PU, 2010 dataset (36/pb);  fixing bug in tau electron/muon/ECAL-crack veto,
			# and reducing number of histograms
            'Run05' : {
                'AHtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/AHtoElecTau/Run05/",
					'harvestingFilePath' : "/data/ndpc0/c/jkolb/TauResults/",
                    'tmpFilePath' : "/data/ndpc0/c/jkolb/TauResults/tmp/"
                },
                'ZtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/ZtoElecTau/Run05/",
                    'harvestingFilePath' : "/data/ndpc0/c/jkolb/TauResults",
                    'tmpFilePath' : "/data/ndpc0/c/jkolb/TauResults/tmp/"
                }
            },
			# fall10 MC with PU, 2010 dataset (36/pb)
            'Run04' : {
                'AHtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/AHtoElecTau/Run04/",
					'harvestingFilePath' : "/data/ndpc0/c/jkolb/TauResults/",
                    'tmpFilePath' : "/data/ndpc0/c/jkolb/TauResults/tmp/"
                },
                'ZtoElecTau' : {
                    'analysisFilePath' : "/user/j/jkolb/elecTauAnalysis/fall10/ZtoElecTau/Run04/",
                    'harvestingFilePath' : "/data/ndpc0/c/jkolb/TauResults",
                    'tmpFilePath' : "/data/ndpc0/c/jkolb/TauResults/tmp/"
                }
            },
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
        },
        'lusito': {
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
                } 
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
