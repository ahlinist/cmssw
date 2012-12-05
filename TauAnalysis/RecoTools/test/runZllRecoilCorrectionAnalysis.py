#!/usr/bin/env python

from TauAnalysis.TauIdEfficiency.tools.buildConfigFilesTauIdEffAnalysis import buildConfigFile_hadd
from TauAnalysis.RecoTools.tools.buildConfigFilesZllRecoilCorrectionAnalysis import *
from TauAnalysis.TauIdEfficiency.tools.buildConfigFilesTauIdEffAnalysis import getStringRep_bool
from TauAnalysis.Skimming.recoSampleDefinitionsGoldenZmumu_7TeV_grid_cfi import RECO_SAMPLES

import os

version = 'v7_05wHCALlaserFilter'

inputFilePath = '/data2/veelken/CMSSW_5_3_x/PATtuples/ZllRecoilCorrection/%s/' % version
outputFilePath = '/data1/veelken/tmp/ZllRecoilCorrection/%s_wMEtShiftCorr_pfMEtCov_2' % version

samplesToAnalyze = {
    'Data_2012RunABC' : {
        'samples' : [
            'Data_runs190456to193621',
            'Data_runs193834to196531',
            'Data_runs190782to190949_recover',
            'Data_runs198022to198523',
            'Data_runs198934to202016v2',
            'Data_runs202044to203002v2'
        ],
        'isMC' : False
    },
    'ZplusJets_madgraph' : {
        'samples' : [
            'ZplusJets_madgraph'
        ],
        'allEvents_DBS' : RECO_SAMPLES['ZplusJets_madgraph']['events_processed'],
        'xSection' : RECO_SAMPLES['ZplusJets_madgraph']['x_sec'],
        'isMC' : True,
        'Type' : 'Signal',
        'applyRhoNeutralReweighting' : True,
        'scaleFactor' : 1.
    },
    'TTplusJets_madgraph' : {
        'samples' : [
            'TTplusJets_madgraph'
        ],
        'allEvents_DBS' : RECO_SAMPLES['TTplusJets_madgraph']['events_processed'],
        'xSection' : RECO_SAMPLES['TTplusJets_madgraph']['x_sec'],
        'isMC' : True,
        'Type' : 'Background',
        'applyRhoNeutralReweighting' : False,
        'scaleFactor' : 0.1
    },
    'Tbar_tW' : {
        'samples' : [
            'Tbar_tW'
        ],
        'allEvents_DBS' : RECO_SAMPLES['Tbar_tW']['events_processed'],
        'xSection' : RECO_SAMPLES['Tbar_tW']['x_sec'],
        'isMC' : True,
        'Type' : 'Background',
        'applyRhoNeutralReweighting' : False,
        'scaleFactor' : 1.
    },
    'T_tW' : {
        'samples' : [
            'T_tW'
        ],
        'allEvents_DBS' : RECO_SAMPLES['T_tW']['events_processed'],
        'xSection' : RECO_SAMPLES['T_tW']['x_sec'],
        'isMC' : True,
        'Type' : 'Background',
        'applyRhoNeutralReweighting' : False,
        'scaleFactor' : 1.
    },
    'WW' : {
       'samples' : [
           'WW'
       ],
       'allEvents_DBS' : RECO_SAMPLES['WW']['events_processed'],
       'xSection' : RECO_SAMPLES['WW']['x_sec'],
       'isMC' : True,
       'Type' : 'Background',
       'applyRhoNeutralReweighting' : False,
       'scaleFactor' : 1.
    },
    'WZ' : {
       'samples' : [
           'WZ'
       ],
       'allEvents_DBS' : RECO_SAMPLES['WZ']['events_processed'],
       'xSection' : RECO_SAMPLES['WZ']['x_sec'],
       'isMC' : True,
       'Type' : 'Background',
       'applyRhoNeutralReweighting' : False,
        'scaleFactor' : 1.
    },
    'ZZ' : {
       'samples' : [
           'ZZ'
       ],
       'allEvents_DBS' : RECO_SAMPLES['ZZ']['events_processed'],
       'xSection' : RECO_SAMPLES['ZZ']['x_sec'],
       'isMC' : True,
       'Type' : 'Background',
       'applyRhoNeutralReweighting' : False,
        'scaleFactor' : 1.
    },
    ##'QCD' : {
    ##    'samples' : [
    ##        'PPmuXptGt20Mu15'
    ##    ],
    ##    'allEvents_DBS' : RECO_SAMPLES['PPmuXptGt20Mu15']['events_processed'],
    ##    'xSection' : RECO_SAMPLES['PPmuXptGt20Mu15']['x_sec'],
    ##    'isMC' : True,
    ##    'Type' : 'Background',
    ##    'applyRhoNeutralReweighting' : False,
    ##    'scaleFactor' : 1.
    ##}
}

runPeriod = '2012RunABC'

#maxEvents = 25000
maxEvents = -1

intLumiData = None
hltPaths = None
srcWeights = None
if runPeriod == '2012RunABC':
    samplesToAnalyze['Data'] = samplesToAnalyze['Data_2012RunABC']
    #intLumiData = 10251 # runs 190456-202016
    intLumiData = 12063 # runs 190456-203002 (JSON v2)
    hltPaths = {
        'Data' : [
            'HLT_Mu17_Mu8_v16',
            'HLT_Mu17_Mu8_v17',
            'HLT_Mu17_Mu8_v18',
            'HLT_Mu17_Mu8_v19',
            'HLT_Mu17_Mu8_v21'
        ],
        'smMC' : [
            'HLT_Mu17_Mu8_v17'
        ]
    }
    srcWeights = {
        'Data' : [],
        'smMC' : [ 'vertexMultiplicityReweight3d2012RunABCruns190456to203002' ]
    }
else:
    raise ValueError("Invalid runPeriod = %s !!" % runPeriod)

for sampleName in samplesToAnalyze.keys():
    if not samplesToAnalyze[sampleName]['isMC'] and sampleName != 'Data':
        samplesToAnalyze.pop(sampleName)
        
#print samplesToAnalyze

metOptions = {
    ##'pfMEt' : {
    ##    'srcJets' : {
    ##        'Data' : {
    ##            'central'    : 'patJets'
    ##        },
    ##        'smMC' : {
    ##            'central'    : 'patJets'
    ##        }
    ##    },
    ##    'srcMEt' : {
    ##        'Data' : {
    ##            'central'    : 'patPFMet'
    ##        },
    ##        'smMC' : {
    ##            'central'    : 'patPFMetNoSmearing'
    ##        }
    ##    },
    ##    'srcMEtCov' : 'pfMEtSignCovMatrix',
    ##    'sfMEtCov' : 1.0,
    ##    'applyMEtShiftCorrection' : False,
    ##    'plotNoPileUpMEtInputs' : [],
    ##    'isCaloMEt' : False
    ##},
    ##'pfMEtTypeIcorrected' : {
    ##    'srcJets' : {
    ##        'Data' : {
    ##            'central'    : 'patJets'
    ##        },
    ##        'smMC' : {
    ##            'central'    : 'patJets'
    ##        }
    ##    },
    ##    'srcMEt' : {
    ##        'Data' : {
    ##            'central'    : 'patType1CorrectedPFMet'
    ##        },
    ##        'smMC' : {
    ##            'central'    : 'patType1CorrectedPFMetNoSmearing'
    ##        }
    ##    },
    ##    'srcMEtCov' : 'pfMEtSignCovMatrix',
    ##    'sfMEtCov' : 1.0,
    ##    'applyMEtShiftCorrection' : False,
    ##    'plotNoPileUpMEtInputs' : [],
    ##    'isCaloMEt' : False
    ##},
    ##'pfMEtTypeIpIIcorrected' : {
    ##    'srcJets' : {
    ##        'Data' : {
    ##            'central'    : 'patJets'
    ##        },
    ##        'smMC' : {
    ##            'central'    : 'patJets'
    ##        }
    ##    },
    ##    'srcMEt'  : {
    ##        'Data' : {
    ##            'central'    : 'patType1p2CorrectedPFMet'
    ##        },
    ##        'smMC' : {
    ##            'central'    : 'patType1p2CorrectedPFMetNoSmearing'
    ##        }
    ##    },
    ##    'srcMEtCov' : 'pfMEtSignCovMatrix',
    ##    'sfMEtCov' : 1.0,
    ##    'applyMEtShiftCorrection' : False,
    ##    'plotNoPileUpMEtInputs' : [],
    ##    'isCaloMEt' : False
    ##},
    ##'pfMEtMVA' : {
    ##    'srcJets' : {
    ##        'Data' : {
    ##            'central'    : 'patJets'
    ##        },
    ##        'smMC' : {
    ##            'central'    : 'patJets'
    ##        }
    ##    },
    ##    'srcMEt' : {
    ##        'Data' : {
    ##            'central'    : 'patPFMetMVA'
    ##        },
    ##        'smMC' : {
    ##            'central'    : 'patPFMetMVANoSmearing'
    ##        }
    ##    },
    ##    'srcMEtCov' : '', # CV: take PFMET significance matrix from reco::MET->getSignificanceMatrix()
    ##    'sfMEtCov' : 1.0,
    ##    'applyMEtShiftCorrection' : False,
    ##    'plotNoPileUpMEtInputs' : [],
    ##    'isCaloMEt' : False
    ##},
##     'pfMEtNoPileUp' : {
##         'srcJets' : {
##             'Data' : {
##                 'central'    : 'patJets'
##             },
##             'smMC' : {
##                 'central'    : 'patJets'
##             }
##         },
##         'srcMEt' : {
##             'Data' : {
##                 'central'    : 'patPFMetNoPileUp'
##             },
##             'smMC' : {
##                 'central'    : 'patPFMetNoPileUpNoSmearing'
##             }
##         },
##         'srcMEtCov' : 'pfMEtSignCovMatrix',
##         'sfMEtCov' : 1.0,   
##         'applyMEtShiftCorrection' : False,
##         'plotNoPileUpMEtInputs' : [],
##         'isCaloMEt' : False
##     },
    'pfMEtSmeared' : {
       'srcJets' : {
           'Data' : {
               'central'    : 'patJets'
           },
           'smMC' : {
               'central'    : 'smearedPatJets',
               #'jetEnUp'    : 'shiftedPatJetsEnUpForRawMEt',
               #'jetEnDown'  : 'shiftedPatJetsEnDownForRawMEt',
               'jetEnUp'    : 'smearedPatJets',
               'jetEnDown'  : 'smearedPatJets',
               'jetResUp'   : 'smearedPatJetsResUp',
               'jetResDown' : 'smearedPatJetsResDown',
               'unclEnUp'   : 'smearedPatJets',
               'unclEnDown' : 'smearedPatJets'
           }
       },
       'srcMEt' : {
           'Data' : {
               'central'    : 'patPFMet'
           },
           'smMC' : {
               'central'    : 'patPFMet',
               'jetEnUp'    : 'patPFMetJetEnUp',
               'jetEnDown'  : 'patPFMetJetEnDown',
               'jetResUp'   : 'patPFMetJetResUp',
               'jetResDown' : 'patPFMetJetResDown',
               'unclEnUp'   : 'patPFMetUnclusteredEnUp',
               'unclEnDown' : 'patPFMetUnclusteredEnDown'
           }
       },
       'srcMEtCov' : 'pfMEtSignCovMatrix',
       'sfMEtCov' : 1.0,
       'applyMEtShiftCorrection' : False,
       'plotNoPileUpMEtInputs' : [],
       'isCaloMEt' : False
    },
    'pfMEtTypeIcorrectedSmeared' : {
        'srcJets' : {
            'Data' : {
                'central'    : 'patJets'
            },
            'smMC' : {
                'central'    : 'smearedPatJets',
                #'jetEnUp'    : 'shiftedPatJetsEnUpForCorrMEt',
                #'jetEnDown'  : 'shiftedPatJetsEnDownForCorrMEt',
                'jetEnUp'    : 'smearedPatJets',
                'jetEnDown'  : 'smearedPatJets',
                'jetResUp'   : 'smearedPatJetsResUp',
                'jetResDown' : 'smearedPatJetsResDown',
                'unclEnUp'   : 'smearedPatJets',
                'unclEnDown' : 'smearedPatJets'
            }
        },
        'srcMEt' : {
            'Data' : {
                'central'    : 'patType1CorrectedPFMet'
            },
            'smMC' : {
                'central'    : 'patType1CorrectedPFMet',
                'jetEnUp'    : 'patType1CorrectedPFMetJetEnUp',
                'jetEnDown'  : 'patType1CorrectedPFMetJetEnDown',
                'jetResUp'   : 'patType1CorrectedPFMetJetResUp',
                'jetResDown' : 'patType1CorrectedPFMetJetResDown',
                'unclEnUp'   : 'patType1CorrectedPFMetUnclusteredEnUp',
                'unclEnDown' : 'patType1CorrectedPFMetUnclusteredEnDown'
            }
        },
        'srcMEtCov' : 'pfMEtSignCovMatrix',
        'sfMEtCov' : 1.0,
        'applyMEtShiftCorrection' : True,
        ##'applyMEtShiftCorrection' : False,
        'plotNoPileUpMEtInputs' : [],
        'isCaloMEt' : False
    },
    ##'pfMEtTypeIpIIcorrectedSmeared' : {
    ##    'srcJets' : {
    ##        'Data' : {
    ##            'central'    : 'patJets'
    ##        },                
    ##        'smMC' : {
    ##            'central'    : 'smearedPatJets',
    ##            #'jetEnUp'    : 'shiftedPatJetsEnUpForCorrMEt',
    ##            #'jetEnDown'  : 'shiftedPatJetsEnDownForCorrMEt',
    ##            'jetEnUp'    : 'smearedPatJets',
    ##            'jetEnDown'  : 'smearedPatJets',
    ##            'jetResUp'   : 'smearedPatJetsResUp',
    ##            'jetResDown' : 'smearedPatJetsResDown',
    ##            'unclEnUp'   : 'smearedPatJets',
    ##            'unclEnDown' : 'smearedPatJets'
    ##        }
    ##    },    
    ##    'srcMEt' : {
    ##        'Data' : {
    ##            'central'    : 'patType1p2CorrectedPFMet'
    ##        },
    ##        'smMC' : {
    ##            'central'    : 'patType1p2CorrectedPFMet',
    ##            'jetEnUp'    : 'patType1p2CorrectedPFMetJetEnUp',
    ##            'jetEnDown'  : 'patType1p2CorrectedPFMetJetEnDown',
    ##            'jetResUp'   : 'patType1p2CorrectedPFMetJetResUp',
    ##            'jetResDown' : 'patType1p2CorrectedPFMetJetResDown',
    ##            'unclEnUp'   : 'patType1p2CorrectedPFMetUnclusteredEnUp',
    ##            'unclEnDown' : 'patType1p2CorrectedPFMetUnclusteredEnDown'
    ##        }
    ##    },
    ##    'srcMEtCov' : 'pfMEtSignCovMatrix',
    ##    'sfMEtCov' : 1.0,
    ##    'applyMEtShiftCorrection' : False,
    ##    'plotNoPileUpMEtInputs' : [],
    ##    'isCaloMEt' : False
    ##},
    'pfMEtMVASmeared' : {
        'srcJets' : {
            'Data' : {
                'central'    : 'patJets'
            },
            'smMC' : {
                'central'    : 'smearedPatJets',
                #'jetEnUp'    : 'shiftedPatJetsEnUpForCorrMEt',
                #'jetEnDown'  : 'shiftedPatJetsEnDownForCorrMEt',
                'jetEnUp'    : 'smearedPatJets',
                'jetEnDown'  : 'smearedPatJets',
                'jetResUp'   : 'smearedPatJetsResUp',
                'jetResDown' : 'smearedPatJetsResDown',
                'unclEnUp'   : 'smearedPatJets',
                'unclEnDown' : 'smearedPatJets'
            }
        },
        'srcMEt' : {
            'Data' : {
                'central'    : 'patPFMetMVA'
            },
            'smMC' : {
                'central'    : 'patPFMetMVA',
                'jetEnUp'    : 'patPFMetMVAJetEnUp',
                'jetEnDown'  : 'patPFMetMVAJetEnDown',
                'jetResUp'   : 'patPFMetMVAJetResUp',
                'jetResDown' : 'patPFMetMVAJetResDown',
                'unclEnUp'   : 'patPFMetMVAUnclusteredEnUp',
                'unclEnDown' : 'patPFMetMVAUnclusteredEnDown'
            }
        },        
        'srcMEtCov' : 'pfMEtSignCovMatrix',
        'sfMEtCov' : 0.60,
        ##'srcMEtCov' : '', # CV: take PFMET significance matrix from reco::MET->getSignificanceMatrix()
        ##'sfMEtCov' : 1.0,
        'applyMEtShiftCorrection' : True,
        ##'applyMEtShiftCorrection' : False,
        'plotNoPileUpMEtInputs' : [],    
        'isCaloMEt' : False
    },
    'pfMEtMVAunityResponseSmeared' : {
        'srcJets' : {
            'Data' : {
                'central'    : 'patJets'
            },
            'smMC' : {
                'central'    : 'smearedPatJets',
                #'jetEnUp'    : 'shiftedPatJetsEnUpForCorrMEt',
                #'jetEnDown'  : 'shiftedPatJetsEnDownForCorrMEt',
                'jetEnUp'    : 'smearedPatJets',
                'jetEnDown'  : 'smearedPatJets',
                'jetResUp'   : 'smearedPatJetsResUp',
                'jetResDown' : 'smearedPatJetsResDown',
                'unclEnUp'   : 'smearedPatJets',
                'unclEnDown' : 'smearedPatJets'
            }
        },
        'srcMEt' : {
            'Data' : {
                'central'    : 'patPFMetMVAunityResponse'
            },
            'smMC' : {
                'central'    : 'patPFMetMVAunityResponse',
                'jetEnUp'    : 'patPFMetMVAunityResponseJetEnUp',
                'jetEnDown'  : 'patPFMetMVAunityResponseJetEnDown',
                'jetResUp'   : 'patPFMetMVAunityResponseJetResUp',
                'jetResDown' : 'patPFMetMVAunityResponseJetResDown',
                'unclEnUp'   : 'patPFMetMVAunityResponseUnclusteredEnUp',
                'unclEnDown' : 'patPFMetMVAunityResponseUnclusteredEnDown'
            }
        },        
        'srcMEtCov' : 'pfMEtSignCovMatrix',
        'sfMEtCov' : 0.75,
        ##'srcMEtCov' : '', # CV: take PFMET significance matrix from reco::MET->getSignificanceMatrix()
        ##'sfMEtCov' : 1.0,
        ##'applyMEtShiftCorrection' : True,
        'applyMEtShiftCorrection' : False,
        'plotNoPileUpMEtInputs' : [],    
        'isCaloMEt' : False
    },
    'pfMEtNoPileUpSmeared' : {
        'srcJets' : {
            'Data' : {
                'central'    : 'patJets'
            },
            'smMC' : {
                'central'    : 'smearedPatJets',
                'jetEnUp'    : 'smearedPatJets',
                'jetEnDown'  : 'smearedPatJets',
                'jetResUp'   : 'smearedPatJetsResUp',
                'jetResDown' : 'smearedPatJetsResDown',
                'unclEnUp'   : 'smearedPatJets',
                'unclEnDown' : 'smearedPatJets'
            }
        },
        'srcMEt' : {
            'Data' : {
                'central'    : 'patPFMetNoPileUp'
            },
            'smMC' : {
                'central'    : 'patPFMetNoPileUp',
                'jetEnUp'    : 'patPFMetNoPileUpJetEnUp',
                'jetEnDown'  : 'patPFMetNoPileUpJetEnDown',
                'jetResUp'   : 'patPFMetNoPileUpJetResUp',
                'jetResDown' : 'patPFMetNoPileUpJetResDown',
                'unclEnUp'   : 'patPFMetNoPileUpUnclusteredEnUp',
                'unclEnDown' : 'patPFMetNoPileUpUnclusteredEnDown'
            }
        },
        'srcMEtCov' : 'pfMEtSignCovMatrix',
        'sfMEtCov' : 0.70,
        ##'srcMEtCov' : '', # CV: take PFMET significance matrix from reco::MET->getSignificanceMatrix()
        ##'sfMEtCov' : 1.0,
        'applyMEtShiftCorrection' : True,
        ##'applyMEtShiftCorrection' : False,
        'srcNoPileUpMEtInputs' : {
            'Data' : {
                'central'    : 'noPileUpPFMEt'
            },
            'smMC' : {
                'central'    : 'noPileUpPFMEt',
                'jetEnUp'    : 'noPileUpPFMEtJetEnUp',
                'jetEnDown'  : 'noPileUpPFMEtJetEnDown',
                'jetResUp'   : 'noPileUpPFMEtJetResUp',
                'jetResDown' : 'noPileUpPFMEtJetResDown',
                'unclEnUp'   : 'noPileUpPFMEtUnclusteredEnUp',
                'unclEnDown' : 'noPileUpPFMEtUnclusteredEnDown'
            }
        },
        'plotNoPileUpMEtInputs' : [
            'sumLeptons',
            'sumNoPUjetOffsetEnCorr',
            'sumNoPUjets',
            'sumPUjets',
            'sumNoPUunclChargedCands',
            'sumPUunclChargedCands',
            'sumUnclNeutralCands'
        ],
        'isCaloMEt' : False # CV: only need to set 'isCaloMEt' to true in case muon corrections are not applied to CaloMEt
    },
##     'pfMinMEtSmeared' : {
##         'srcJets' : {
##             'Data' : {
##                 'central'    : 'patJets'
##             },
##             'smMC' : {
##                 'central'    : 'smearedPatJets',
##                 'jetEnUp'    : 'smearedPatJets',
##                 'jetEnDown'  : 'smearedPatJets',
##                 'jetResUp'   : 'smearedPatJetsResUp',
##                 'jetResDown' : 'smearedPatJetsResDown',
##                 'unclEnUp'   : 'smearedPatJets',
##                 'unclEnDown' : 'smearedPatJets'
##             }
##         },
##         'srcMEt' : {
##             'Data' : {
##                 'central'    : 'patMinMEt'
##             },
##             'smMC' : {
##                 'central'    : 'patMinMEt',
##                 'jetEnUp'    : 'patMinMEtJetEnUp',
##                 'jetEnDown'  : 'patMinMEtJetEnDown',
##                 'jetResUp'   : 'patMinMEtJetResUp',
##                 'jetResDown' : 'patMinMEtJetResDown',
##                 'unclEnUp'   : 'patMinMEtUnclusteredEnUp',
##                 'unclEnDown' : 'patMinMEtUnclusteredEnDown'
##             }
##         },
##         'srcMEtCov' : 'pfMEtSignCovMatrix',
##         'sfMEtCov' : 0.7,
##         'applyMEtShiftCorrection' : False,
##         'plotNoPileUpMEtInputs' : [],
##         'isCaloMEt' : True
##     },    
##     'caloMEt' : {
##         'srcJets' : {
##             'Data' : {
##                 'central'    : 'patJets'
##             },
##             'smMC' : {
##                 'central'    : 'patJets'
##             }
##         },
##         'srcMEt' : {
##             'Data' : {
##                 'central'    : 'patCaloMet'
##             },
##             'smMC' : {
##                 'central'    : 'patCaloMet'
##             }
##         },
##         'srcMEtCov' : '',
##         'sfMEtCov' : 1.0,
##         'applyMEtShiftCorrection' : False,
##         'plotNoPileUpMEtInputs' : [],
##         'isCaloMEt' : True
##     },
##     'caloMEtNoHF' : {
##         'srcJets' : {
##             'Data' : {
##                 'central'    : 'patJets'
##             },
##             'smMC' : {
##                 'central'    : 'patJets'
##             }
##         },
##         'srcMEt' : {
##             'Data' : {
##                 'central'    : 'patCaloMetNoHF'
##             },
##             'smMC' : {
##                 'central'    : 'patCaloMetNoHF'
##             }
##         },
##         'srcMEtCov' : '',
##         'sfMEtCov' : 1.0,   
##         'applyMEtShiftCorrection' : False,
##         'plotNoPileUpMEtInputs' : [],    
##         'isCaloMEt' : True
##     },
##     'caloMEtTypeIcorrected' : {
##         'srcJets' : {
##             'Data' : {
##                 'central'    : 'patJets'
##             },
##             'smMC' : {
##                 'central'    : 'patJets'
##             }
##         },
##         'srcMEt' : {
##             'Data' : {
##                 'central'    : 'patType1CorrectedCaloMet'
##             },
##             'smMC' : {
##                 'central'    : 'patType1CorrectedCaloMet'
##             }
##         },
##         'srcMEtCov' : '',
##         'sfMEtCov' : 1.0,    
##         'applyMEtShiftCorrection' : True,
##         ##'applyMEtShiftCorrection' : False,
##         'plotNoPileUpMEtInputs' : [],    
##         'isCaloMEt' : False
##     },
##     'caloMEtTypeIcorrectedNoHF' : {
##         'srcJets' : {
##             'Data' : {
##                 'central'    : 'patJets'
##             },
##             'smMC' : {
##                 'central'    : 'patJets'
##             }
##         },
##         'srcMEt' : {
##             'Data' : {
##                 'central'    : 'patType1CorrectedCaloMetNoHF'
##             },
##             'smMC' : {
##                 'central'    : 'patType1CorrectedCaloMetNoHF'
##             }
##         },
##         'srcMEtCov' : '',
##         'sfMEtCov' : 1.0,    
##         'applyMEtShiftCorrection' : False,
##         'plotNoPileUpMEtInputs' : [],    
##         'isCaloMEt' : True
##    }
}

#runZllRecoilCorrectionFits = True
runZllRecoilCorrectionFits = False

execDir = "%s/bin/%s/" % (os.environ['CMSSW_BASE'], os.environ['SCRAM_ARCH'])

executable_produceZllRecoilNtuples           = execDir + 'FWLiteZllRecoilCorrectionNtupleProducer'
executable_fitZllRecoilNtuples               = execDir + 'fitZllRecoilCorrection'
executable_FWLiteZllRecoilCorrectionAnalyzer = execDir + 'FWLiteZllRecoilCorrectionAnalyzer'
executable_hadd                              = 'hadd'
executable_makeZllRecoilCorrectionFinalPlots = execDir + 'makeZllRecoilCorrectionFinalPlots'
executable_shell                             = '/bin/csh'
executable_python                            = 'python'

nice = 'nice '

if not os.path.exists(outputFilePath):
    os.mkdir(outputFilePath)
    
outputFilePath = os.path.join(outputFilePath, runPeriod)
if not os.path.exists(outputFilePath):
    os.mkdir(outputFilePath)

#--------------------------------------------------------------------------------
#
# build config files for producing "plain" ROOT Ntuples
# needed for fitting Z-recoil correction parameters
#
fileNames_produceZllRecoilNtuples = {}
if runZllRecoilCorrectionFits:
    for metOptionName in metOptions.keys():
        fileNames_produceZllRecoilNtuples[metOptionName] = {}
        for sampleName in samplesToAnalyze.keys():
            if not (samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Background'):
                processType = None
                if samplesToAnalyze[sampleName]['isMC']:
                    processType = 'smMC'
                else:
                    processType = 'Data'
                srcJets = metOptions[metOptionName]['srcJets'][processType]
                srcMEt = metOptions[metOptionName]['srcMEt'][processType]            
                fileNames_produceZllRecoilNtuples[metOptionName][sampleName] = {}
                for central_or_shift in srcMEt.keys():
                    retVal_produceZllRecoilNtuples = \
                      buildConfigFile_produceZllRecoilNtuples(
                        maxEvents, sampleName, metOptionName, inputFilePath, outputFilePath, samplesToAnalyze,
                        central_or_shift, srcMEt[central_or_shift], srcJets[central_or_shift], 
                        hltPaths[processType], srcWeights[processType])

                    if retVal_produceZllRecoilNtuples is None:
                        print("No input files for MET type = %s, sample = %s, systematic = %s --> skipping !!" % \
                          (metOptionName, sampleName, central_or_shift))
                        continue

                    fileNames_produceZllRecoilNtuples[metOptionName][sampleName][central_or_shift] = \
                      retVal_produceZllRecoilNtuples
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
# build config files for fitting Z-recoil correction parameters 
#
fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp = {}
if runZllRecoilCorrectionFits:
    for metOptionName in metOptions.keys():
        fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName] = {}
        for sampleName in samplesToAnalyze.keys():
            if not (samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Background'):
                processType = None
                if samplesToAnalyze[sampleName]['isMC']:
                    processType = 'smMC'
                else:
                    processType = 'Data'
                fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName] = {}            
                for central_or_shift in metOptions[metOptionName]['srcMEt'][processType].keys():
                    retVal_fitZllRecoilNtuples = \
                      buildConfigFile_fitZllRecoilNtuples(
                        sampleName, metOptionName,
                        fileNames_produceZllRecoilNtuples[metOptionName][sampleName][central_or_shift]['outputFileName'], outputFilePath,
                        samplesToAnalyze, central_or_shift,
                        metOptions[metOptionName]['isCaloMEt'],
                        "qT", "uParl", "uPerp")

                    if retVal_fitZllRecoilNtuples is None:
                        continue

                    fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName][central_or_shift] = \
                      retVal_fitZllRecoilNtuples
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
# build config files for running FWLiteZllRecoilCorrectionAnalyzer 
#
fileNames_FWLiteZllRecoilCorrectionAnalyzer = {}
for metOptionName in metOptions.keys():
    fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName] = {}
    for sampleName in samplesToAnalyze.keys():
        processType = None
        if samplesToAnalyze[sampleName]['isMC']:
            processType = 'smMC'
        else:
            processType = 'Data'
        srcJets = metOptions[metOptionName]['srcJets'][processType]
        srcMEt = metOptions[metOptionName]['srcMEt'][processType]
        srcMEtCov = metOptions[metOptionName]['srcMEtCov']
        sfMEtCov = metOptions[metOptionName]['sfMEtCov']
        applyMEtShiftCorrection = metOptions[metOptionName]['applyMEtShiftCorrection']
        plotNoPileUpMEtInputs = metOptions[metOptionName]['plotNoPileUpMEtInputs']
        fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName] = {}
        for central_or_shift in srcMEt.keys():
            srcNoPileUpMEtInputs = None
            if len(plotNoPileUpMEtInputs) >= 1:
                srcNoPileUpMEtInputs = metOptions[metOptionName]['srcNoPileUpMEtInputs'][processType][central_or_shift]
            else:
                srcNoPileUpMEtInputs = ''
            retVal_FWLiteZllRecoilCorrectionAnalyzer = \
              buildConfigFile_FWLiteZllRecoilCorrectionAnalyzer(
                maxEvents, sampleName, runPeriod, metOptionName, inputFilePath, outputFilePath, samplesToAnalyze,
                central_or_shift,
                srcMEt[central_or_shift], applyMEtShiftCorrection, srcMEtCov, sfMEtCov, srcJets[central_or_shift],
                srcNoPileUpMEtInputs, plotNoPileUpMEtInputs,
                hltPaths[processType], srcWeights[processType],
                None, intLumiData)

            if retVal_FWLiteZllRecoilCorrectionAnalyzer is None:
                continue

            fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift] = \
              retVal_FWLiteZllRecoilCorrectionAnalyzer
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
# build shell script for running 'hadd' in order to "harvest" histograms
# produced by FWLiteZllRecoilCorrectionAnalyzer macro
#
fileNames_hadd = {}
for metOptionName in metOptions.keys():
    haddShellFileName  = os.path.join(outputFilePath, 'harvestZllRecoilCorrectionHistograms_%s.csh' % metOptionName)
    haddInputFileNames = []
    for sampleName in samplesToAnalyze.keys():
        if fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName].has_key(sampleName):
            for central_or_shift in metOptions[metOptionName]['srcJets']['smMC'].keys():
                if fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName].has_key(central_or_shift):
                    haddInputFileNames.append(
                      fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['outputFileName'])
    haddOutputFileName = os.path.join(outputFilePath, 'analyzeZllRecoilCorrectionHistograms_all_%s.root' % metOptionName)

    retVal_hadd = \
      buildConfigFile_hadd(executable_hadd, haddShellFileName, haddInputFileNames, haddOutputFileName)

    fileNames_hadd[metOptionName] = {}
    fileNames_hadd[metOptionName]['shellFileName']  = haddShellFileName
    fileNames_hadd[metOptionName]['inputFileNames'] = haddInputFileNames
    fileNames_hadd[metOptionName]['outputFileName'] = haddOutputFileName
    fileNames_hadd[metOptionName]['logFileName']    = retVal_hadd['logFileName']
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
# build config files for making final plots
#
sampleNameData           = None
sampleNameMC_signal      = None
sampleNameMCs_background = []
for sampleName in samplesToAnalyze.keys():
    if samplesToAnalyze[sampleName]['isMC']:
        if samplesToAnalyze[sampleName]['Type'] == 'Signal':
            if sampleNameMC_signal is not None:
                raise ValueError("'MC' sample must be defined only once !!")
            sampleNameMC_signal = sampleName
        elif samplesToAnalyze[sampleName]['Type'] == 'Background':
            sampleNameMCs_background.append(sampleName)
        else:
            raise ValueError("Invalid MC type = %s !!" % samplesToAnalyze[sampleName]['Type'])
    else:
        if sampleNameData is not None:
            raise ValueError("'Data' sample must be defined only once !!")
        sampleNameData = sampleName
        
corrLevelsMC = [
    ##'beforeGenPUreweight',
    'beforeAddPUreweight',
    ##'beforeZllRecoilCorr',
    ##'afterZllRecoilMCtoDataCorr',
    ##'afterZllRecoilAbsCalib',
]

fileNames_makeZllRecoilCorrectionFinalPlots = {}
for metOptionName in metOptions.keys():
    fileNames_makeZllRecoilCorrectionFinalPlots[metOptionName] = {}
    for corrLevelMC in corrLevelsMC:
        retVal_makeZllRecoilCorrectionFinalPlots = \
          buildConfigFile_makeZllRecoilCorrectionFinalPlots(
            sampleNameData, sampleNameMC_signal, sampleNameMCs_background, runPeriod, metOptionName,
            fileNames_hadd[metOptionName]['outputFileName'], outputFilePath, samplesToAnalyze, corrLevelMC,
            metOptions[metOptionName]['srcJets']['smMC'].keys(),
            metOptions[metOptionName]['isCaloMEt'],
            metOptions[metOptionName]['plotNoPileUpMEtInputs'])
            
        fileNames_makeZllRecoilCorrectionFinalPlots[metOptionName][corrLevelMC] = retVal_makeZllRecoilCorrectionFinalPlots
#--------------------------------------------------------------------------------

def make_MakeFile_vstring(list_of_strings):
    retVal = ""
    for i, string_i in enumerate(list_of_strings):
        if i > 0:
            retVal += " "
        retVal += string_i
    return retVal

# done building config files, now build Makefile...
makeFileName = "Makefile_ZllRecoilCorrectionAnalysis_%s_%s" % (version, runPeriod)
makeFile = open(makeFileName, "w")
makeFile.write("\n")
outputFileNames_makeZllRecoilCorrectionFinalPlots_and_Numbers = []
for metOptionName in metOptions.keys():
    for corrLevelMC in corrLevelsMC:
        outputFileNames_makeZllRecoilCorrectionFinalPlots_and_Numbers.append(
          fileNames_makeZllRecoilCorrectionFinalPlots[metOptionName][corrLevelMC]['outputFileName'])
    if runZllRecoilCorrectionFits:    
        for sampleName in samplesToAnalyze.keys():
            if not (samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Background'):
                if fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName].has_key(sampleName):
                    for central_or_shift in metOptions[metOptionName]['srcMEt']['smMC'].keys():
                        if fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName].has_key(central_or_shift):
                            outputFileNames_makeZllRecoilCorrectionFinalPlots_and_Numbers.append(
                              fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName][central_or_shift]['outputFileName'])
makeFile.write("all: %s\n" % make_MakeFile_vstring(outputFileNames_makeZllRecoilCorrectionFinalPlots_and_Numbers))
makeFile.write("\techo 'Finished running ZllRecoilCorrectionAnalysis.'\n")
makeFile.write("\n")
if runZllRecoilCorrectionFits:
    for metOptionName in metOptions.keys():
        for sampleName in samplesToAnalyze.keys():
            if not (samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Background'):
                if fileNames_produceZllRecoilNtuples[metOptionName].has_key(sampleName):
                    for central_or_shift in metOptions[metOptionName]['srcJets']['smMC'].keys():
                        if fileNames_produceZllRecoilNtuples[metOptionName][sampleName].has_key(central_or_shift):
                            makeFile.write("%s: %s\n" %
                              (fileNames_produceZllRecoilNtuples[metOptionName][sampleName][central_or_shift]['outputFileName'],
                               #executable_produceZllRecoilNtuples,
                               ""))
                            makeFile.write("\t%s%s %s &> %s\n" %
                              (nice, executable_produceZllRecoilNtuples,
                               fileNames_produceZllRecoilNtuples[metOptionName][sampleName][central_or_shift]['configFileName'],
                               fileNames_produceZllRecoilNtuples[metOptionName][sampleName][central_or_shift]['logFileName']))
makeFile.write("\n")
if runZllRecoilCorrectionFits:
    for metOptionName in metOptions.keys():
        for sampleName in samplesToAnalyze.keys():
            if not (samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Background'):
                if fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName].has_key(sampleName) and \
                   fileNames_produceZllRecoilNtuples[metOptionName].has_key(sampleName):
                    for central_or_shift in metOptions[metOptionName]['srcJets']['smMC'].keys():
                        if fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName].has_key(central_or_shift) and \
                           fileNames_produceZllRecoilNtuples[metOptionName][sampleName].has_key(central_or_shift):
                            makeFile.write("%s: %s %s\n" %
                              (fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName][central_or_shift]['outputFileName'],
                               fileNames_produceZllRecoilNtuples[metOptionName][sampleName][central_or_shift]['outputFileName'],
                               #executable_fitZllRecoilNtuples,
                               ""))
                            makeFile.write("\t%s%s %s &> %s\n" %
                              (nice, executable_fitZllRecoilNtuples,
                               fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName][central_or_shift]['configFileName'],
                               fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName][central_or_shift]['logFileName']))
makeFile.write("\n")
if runZllRecoilCorrectionFits:
    for metOptionName in metOptions.keys():
        for sampleName in samplesToAnalyze.keys():
            if fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName].has_key(sampleName) and \
               fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName].has_key(sampleName):          
                for central_or_shift in metOptions[metOptionName]['srcMEt']['smMC'].keys():
                    if fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName].has_key(central_or_shift):
                        if central_or_shift == 'central':
                            processType = None
                            if samplesToAnalyze[sampleName]['isMC']:
                                processType = 'smMC'
                            else:
                                processType = 'Data'
                            srcJets = metOptions[metOptionName]['srcJets'][processType]
                            srcMEt = metOptions[metOptionName]['srcMEt'][processType]
                            srcMEtCov = metOptions[metOptionName]['srcMEtCov']
                            sfMEtCov = metOptions[metOptionName]['sfMEtCov']
                            applyMEtShiftCorrection = metOptions[metOptionName]['applyMEtShiftCorrection']
                            plotNoPileUpMEtInputs = metOptions[metOptionName]['plotNoPileUpMEtInputs']
                            srcNoPileUpMEtInputs = None
                            if plotNoPileUpMEtInputs:
                                srcNoPileUpMEtInputs = metOptions[metOptionName]['srcNoPileUpMEtInputs'][central_or_shift]
                            else:
                                srcNoPileUpMEtInputs = ''
                            makeFile.write("%s: %s %s %s\n" %
                             (fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['outputFileName'],
                              fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleNameData]['central']['outputFileName'],
                              fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleNameMC_signal][central_or_shift]['outputFileName'],
                              #executable_FWLiteZllRecoilCorrectionAnalyzer,
                              ""))
                            # rebuild config file to run FWLiteZllRecoilCorrectionAnalyzer macro
                            # using actual Z-recoil correction parameter values determind by fit
                            tmpConfig = \
"""
#!/usr/bin/env python

from TauAnalysis.RecoTools.tools.buildConfigFilesZllRecoilCorrectionAnalysis import *

samplesToAnalyze = %s

metOptions = %s

buildConfigFile_FWLiteZllRecoilCorrectionAnalyzer(
  %i, '%s', '%s', '%s', '%s', '%s', samplesToAnalyze, '%s', '%s', %s, '%s', %f, '%s', %s, %s, %s, { 'data' : '%s', 'mc' : '%s' }, %f)
""" % (str(samplesToAnalyze),
       str(metOptions),
       maxEvents, sampleName, runPeriod, metOptionName, inputFilePath, outputFilePath,
       central_or_shift, srcMEt[central_or_shift], getStringRep_bool(applyMEtShiftCorrection), srcMEtCov, sfMEtCov, 
       srcJets[central_or_shift],
       srcNoPileUpMEtInputs, plotNoPileUpMEtInputs,
       hltPaths[processType], srcWeights[processType],       
       fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleNameData]['central']['outputFileName'],
       fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleNameMC_signal][central_or_shift]['outputFileName'],
       intLumiData)
                            tmpConfigFileName = "makeTMPconfigFile_%s_%s.py" % (sampleName, metOptionName)
                            tmpConfigFileName_full = os.path.join(outputFilePath, tmpConfigFileName)    
                            tmpConfigFile = open(tmpConfigFileName_full, "w")
                            tmpConfigFile.write(tmpConfig)
                            tmpConfigFile.close()            
                            makeFile.write("\t%s%s %s\n" %
                              (nice, executable_python, tmpConfigFileName_full))
                            makeFile.write("\t%s%s %s &> %s\n" %
                              (nice, executable_FWLiteZllRecoilCorrectionAnalyzer,
                               fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['configFileName'],
                               fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['logFileName']))
                        elif samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Signal':
                            makeFile.write("%s: %s\n" %
                              (fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['outputFileName'],
                               #executable_FWLiteZllRecoilCorrectionAnalyzer,
                               ""))
                            makeFile.write("\t%s%s %s &> %s\n" %
                              (nice, executable_FWLiteZllRecoilCorrectionAnalyzer,
                               fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['configFileName'],
                               fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['logFileName']))
            elif samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Background':
                for central_or_shift in metOptions[metOptionName]['srcMEt']['smMC'].keys():
                    if fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName].has_key(central_or_shift):
                        makeFile.write("%s: %s\n" %
                          (fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['outputFileName'],
                           #executable_FWLiteZllRecoilCorrectionAnalyzer,
                           ""))
                        makeFile.write("\t%s%s %s &> %s\n" %
                          (nice, executable_FWLiteZllRecoilCorrectionAnalyzer,
                           fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['configFileName'],
                           fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['logFileName']))
else:
    for metOptionName in metOptions.keys():
        for sampleName in samplesToAnalyze.keys():
            if fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName].has_key(sampleName):
                for central_or_shift in metOptions[metOptionName]['srcMEt']['smMC'].keys():
                    if fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName].has_key(central_or_shift):
                        makeFile.write("%s: %s\n" %
                          (fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['outputFileName'],
                           #executable_FWLiteZllRecoilCorrectionAnalyzer,
                           ""))
                        makeFile.write("\t%s%s %s &> %s\n" %
                          (nice, executable_FWLiteZllRecoilCorrectionAnalyzer,
                           fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['configFileName'],
                           fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['logFileName']))
makeFile.write("\n")
for metOptionName in metOptions.keys():
    if len(fileNames_hadd[metOptionName]['inputFileNames']) > 0:
        makeFile.write("%s: %s\n" %
          (fileNames_hadd[metOptionName]['outputFileName'],
           make_MakeFile_vstring(fileNames_hadd[metOptionName]['inputFileNames'])))
        makeFile.write("\t%s%s %s &> %s\n" %
          (nice, executable_shell,
           fileNames_hadd[metOptionName]['shellFileName'],
           fileNames_hadd[metOptionName]['logFileName']))
makeFile.write("\n")
for metOptionName in metOptions.keys():
    for corrLevelMC in corrLevelsMC:
        makeFile.write("%s: %s %s\n" %
          (fileNames_makeZllRecoilCorrectionFinalPlots[metOptionName][corrLevelMC]['outputFileName'],
           fileNames_hadd[metOptionName]['outputFileName'],
           executable_makeZllRecoilCorrectionFinalPlots))
        makeFile.write("\t%s%s %s &> %s\n" %
          (nice, executable_makeZllRecoilCorrectionFinalPlots,
           fileNames_makeZllRecoilCorrectionFinalPlots[metOptionName][corrLevelMC]['configFileName'],
           fileNames_makeZllRecoilCorrectionFinalPlots[metOptionName][corrLevelMC]['logFileName']))
makeFile.write("\n")
makeFile.write(".PHONY: clean\n")
makeFile.write("clean:\n")
outputFileNames_produceZllRecoilNtuples = []
if runZllRecoilCorrectionFits:    
    for metOptionName in metOptions.keys():
        for sampleName in samplesToAnalyze.keys():
            if not (samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Background'):
                if fileNames_produceZllRecoilNtuples[metOptionName].has_key(sampleName):
                    for central_or_shift in metOptions[metOptionName]['srcMEt']['smMC'].keys():
                        if fileNames_produceZllRecoilNtuples[metOptionName][sampleName].has_key(central_or_shift):
                            outputFileNames_produceZllRecoilNtuples.append(
                              fileNames_produceZllRecoilNtuples[metOptionName][sampleName][central_or_shift]['outputFileName'])
makeFile.write("\trm -f %s\n" % make_MakeFile_vstring(outputFileNames_produceZllRecoilNtuples))
outputFileNames_fitZllRecoilNtuples = []
if runZllRecoilCorrectionFits:    
    for metOptionName in metOptions.keys():
        for sampleName in samplesToAnalyze.keys():
            if not (samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Background'):
                if fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName].has_key(sampleName):
                    for central_or_shift in metOptions[metOptionName]['srcMEt']['smMC'].keys():
                        if fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName].has_key(central_or_shift):
                            outputFileNames_fitZllRecoilNtuples.append(
                              fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName][central_or_shift]['outputFileName'])
makeFile.write("\trm -f %s\n" % make_MakeFile_vstring(outputFileNames_fitZllRecoilNtuples))
outputFileNames_FWLiteZllRecoilCorrectionAnalyzer = []
for metOptionName in metOptions.keys():
    for sampleName in samplesToAnalyze.keys():
        if fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName].has_key(sampleName):
            for central_or_shift in metOptions[metOptionName]['srcMEt']['smMC'].keys():
                if fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName].has_key(central_or_shift):
                    outputFileNames_FWLiteZllRecoilCorrectionAnalyzer.append(
                      fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['outputFileName'])
makeFile.write("\trm -f %s\n" % make_MakeFile_vstring(outputFileNames_FWLiteZllRecoilCorrectionAnalyzer))
outputFileNames_hadd = []
for metOptionName in metOptions.keys():
    outputFileNames_hadd.append(
      fileNames_hadd[metOptionName]['outputFileName'])
makeFile.write("\trm -f %s\n" % make_MakeFile_vstring(outputFileNames_hadd))
outputFileNames_makeZllRecoilCorrectionFinalPlots = []
for metOptionName in metOptions.keys():
    for corrLevelMC in corrLevelsMC:
        outputFileNames_makeZllRecoilCorrectionFinalPlots.append(
          fileNames_makeZllRecoilCorrectionFinalPlots[metOptionName][corrLevelMC]['outputFileName'])
makeFile.write("\trm -f %s\n" % make_MakeFile_vstring(outputFileNames_makeZllRecoilCorrectionFinalPlots))
makeFile.write("\techo 'Finished deleting old files.'\n")
makeFile.write("\n")
makeFile.close()

print("Finished building Makefile. Now execute 'make -j 8 -f %s'." % makeFileName)
