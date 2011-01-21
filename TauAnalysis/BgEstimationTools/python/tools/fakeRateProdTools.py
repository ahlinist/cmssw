import FWCore.ParameterSet.Config as cms
import os
import shutil
from CondCore.DBCommon.CondDBSetup_cfi import CondDBSetup
from RecoTauTag.RecoTau.TauDiscriminatorTools import noPrediscriminants
from RecoTauTag.RecoTau.PFRecoTauDiscriminationByLeadingTrackFinding_cfi import \
       pfRecoTauDiscriminationByLeadingTrackFinding

fr_config = {
    'hpsTancTaus' : {
        'ewkTauIdTaNCloose' : {
            'qcdMuEnrichedDataJet' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_ewkTauIdTaNCloose_PPmuX_data_jet/fakeRate.db',
                'tag' : 'FakeRate',
            },
            'qcdMuEnrichedSimJet' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_ewkTauIdTaNCloose_PPmuX_mcPU156bx_jet/fakeRate.db',
                'tag' : 'FakeRate',
            },
            'qcdDiJetFirstDataJet' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_ewkTauIdTaNCloose_QCDdiJet1st_data_jet20/fakeRate.db',
                'tag' : 'FakeRate',
            },
            'qcdDiJetFirstSimJet' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_ewkTauIdTaNCloose_QCDdiJet1st_mc_jet20/fakeRate.db',
                'tag' : 'FakeRate',
            },
            'qcdDiJetSecondDataJet' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_ewkTauIdTaNCloose_QCDdiJet2nd_data_jet20/fakeRate.db',
                'tag' : 'FakeRate',
            },
            'qcdDiJetSecondSimJet' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_ewkTauIdTaNCloose_QCDdiJet2nd_mc_jet20/fakeRate.db',
                'tag' : 'FakeRate',
            },
            'WplusJetsDataJet' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_ewkTauIdTaNCloose_WplusJets_data_jet/fakeRate.db',
                'tag' : 'FakeRate',
            },
            'WplusJetsDataJet20' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_ewkTauIdTaNCloose_WplusJets_data_jet20/fakeRate.db',
                'tag' : 'FakeRate',
            },
            'WplusJetsDataJetNoEta20' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_ewkTauIdTaNCloose_WplusJets_data_jetNoEta20/fakeRate.db',
                'tag' : 'FakeRate',
            },
            'WplusJetsSimJet' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_ewkTauIdTaNCloose_WplusJets_mcPU156bx_jet/fakeRate.db',
                'tag' : 'FakeRate',
            },

        },
        'ewkTauIdHPSloose' : {
            'qcdMuEnrichedDataJet' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_ewkTauIdHPSloose_PPmuX_data_jet/fakeRate.db',
                'tag' : 'FakeRate',
            },
            'qcdMuEnrichedSimJet' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_ewkTauIdHPSloose_PPmuX_mcPU156bx_jet/fakeRate.db',
                'tag' : 'FakeRate',
            },
            'WplusJetsDataJet' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_ewkTauIdHPSloose_WplusJets_data_jet/fakeRate.db',
                'tag' : 'FakeRate',
            },
            'WplusJetsSimJet' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_ewkTauIdHPSloose_WplusJets_mcPU156bx_jet/fakeRate.db',
                'tag' : 'FakeRate',
            },
        },
    'bgEstTemplateTaNCinverted' : {
            'WplusJetsData' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_bgEstTemplateTaNCinverted_WplusJets_data/fakeRate.db',
                'tag' : 'FakeRate',
            },
            'WplusJetsSim' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_bgEstTemplateTaNCinverted_WplusJets_mcPU156bx/fakeRate.db',
                'tag' : 'FakeRate',
            }
        }
    }
}

eff_config = {
    'hpsTancTaus' : {
        'ewkTauIdTaNCmedium' : {
            'ZTTsim' : {
                'db' : '/afs/cern.ch/user/f/friis/public/fakeRates/fakerate_zttEff/fakeRate.db',
                'tag' : 'FakeRate'
            }
        },
        'ewkTauIdTaNCloose' : {
            'ZTTsim' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_ewkTauIdTaNCloose_Ztautau_mcPU156bx_jet/fakeRate.db',
                'tag' : 'FakeRate'
            }
        },
        'ewkTauIdHPSloose' : {
            'ZTTsim' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_ewkTauIdHPSloose_Ztautau_mcPU156bx_jet/fakeRate.db',
                'tag' : 'FakeRate'
            }
        },
        'bgEstTemplateTaNCinverted' : {
            'ZTTsim' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_bgEstTemplateTaNCinverted_Ztautau_mcPU156bx/fakeRate.db',
                'tag' : 'FakeRate'
            }
        }
    }
}

data_directory = os.path.join(os.environ['CMSSW_BASE'], 'src', 'TauAnalysis', 'BgEstimationTools', 'data')

# File-in-path version
data_directory_fip = os.path.join('TauAnalysis', 'BgEstimationTools', 'data')

def addConfigItems(target_dict, source_dict):
    for patTauProducer in source_dict.keys():
        if target_dict.get(patTauProducer) is None:
	    target_dict[patTauProducer] = {}
	for frSet in source_dict[patTauProducer].keys():
            if target_dict[patTauProducer].get(frSet) is None:
  	        target_dict[patTauProducer][frSet] = {}
	    target_dict[patTauProducer][frSet].update(source_dict[patTauProducer][frSet])

def configureFakeRateProduction(process, patTauProducer, frSet = 'ewkTauIdTaNCloose'):

    fr_and_efficiency_config = {}
    addConfigItems(fr_and_efficiency_config, fr_config)
    addConfigItems(fr_and_efficiency_config, eff_config)

    retVal = {}

    process.tauFakeRates = cms.Sequence()

    for frType, frConfig in fr_and_efficiency_config[patTauProducer][frSet].items():
        print "Making DB loader: %s fake rates for %s" % (frType, frSet)

        # copy the SQL-lite file to working area
        db_source_file = frConfig['db']
        local_db_file_name = frSet + frType + os.path.basename(db_source_file)
        local_db_copy_name = os.path.join(data_directory, local_db_file_name)
        if not os.path.exists(data_directory):
            os.makedirs(data_directory)
        do_copy = False
        if not os.path.exists(local_db_copy_name):
            do_copy = True
        else:
            if not os.path.exists(db_source_file):
                print "WARNING!!! Can't find source file %s" % db_source_file
                print "I can't check if it's outdated!"
            else:
                if os.stat(local_db_copy_name).st_mtime < os.stat(db_source_file).st_mtime:
                    print " DB file is outdated!"
                    do_copy = True
        if do_copy:
            print "Copying fakerate db:", db_source_file, \
                    "->", local_db_copy_name
            shutil.copy(db_source_file, local_db_copy_name)

        local_db_file_in_path = os.path.join(data_directory_fip, local_db_file_name)

        # create PoolDBESSource module
        dbLoaderModule = cms.ESSource("PoolDBESSource",
            CondDBSetup,
            timetype = cms.string('runnumber'),
            toGet = cms.VPSet(cms.PSet(
                record = cms.string('TauTagMVAComputerRcd'),
                tag = cms.string(frConfig['tag'])
            )),
            connect = cms.string('sqlite_fip:%s' % local_db_file_in_path),
            BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
            # Give it a unique name in the EventSetup
            appendToDataLabel = cms.string("%s_%s" % (frSet, frType)),
        )
        dbLoaderModuleName = frType + "LoadDB"
        setattr(process, dbLoaderModuleName, dbLoaderModule)

        # create "predisciminator" module
        preDiscrModule = pfRecoTauDiscriminationByLeadingTrackFinding.clone(
            PFTauProducer = cms.InputTag(patTauProducer),
            Prediscriminants = noPrediscriminants,
        )
        preDiscrModuleName = frType + "PreDiscr"
        setattr(process, preDiscrModuleName, preDiscrModule)
	process.tauFakeRates += preDiscrModule

	# create "discriminator" module
        # extracting fake-rate from k-NN tree
        mvaDiscrModule = cms.EDProducer("RecoTauMVADiscriminator",
            PFTauProducer = cms.InputTag(patTauProducer),
            Prediscriminants = cms.PSet(
                BooleanOperator = cms.string("and"),
                leadTrack = cms.PSet(
                    Producer = cms.InputTag(preDiscrModuleName),
                    cut = cms.double(0.5)
                )
            ),
            # point it to the correct DB instance
            dbLabel = cms.string("%s_%s" % (frSet, frType)),
            # use identical k-NN extraction for all tau decay modes
            mvas = cms.VPSet(),
            # set name of MVA computer container.
            #
            # NOTE: needs to match name defined in trainTauFakeRate_cfg
	    #
            defaultMVA = cms.string("train"),
            # store "raw" k-NN output in discriminator
            # (no remapping [-1,+1] --> [0,1] required)
            remapOutput = cms.bool(False)
        )
        mvaDiscrModuleName = frType + "MVAdiscr"
        setattr(process, mvaDiscrModuleName, mvaDiscrModule)
	process.tauFakeRates += mvaDiscrModule

        # convert fake-rate from discriminator to pat::LookupTableRecord format
        patLUTconverterModule = cms.EDProducer("PFTauDiscriminatorToPatEfficiencies",
            discSrc = cms.InputTag(mvaDiscrModuleName),
            tauSrc = cms.InputTag(patTauProducer)
        )
        patLUTconverterModuleName = frType + "PATlutConverter"
	setattr(process, patLUTconverterModuleName, patLUTconverterModule)
        process.tauFakeRates += patLUTconverterModule

        retVal[frType] = patLUTconverterModuleName

    return retVal
