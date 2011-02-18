import FWCore.ParameterSet.Config as cms

import copy
import os
import shutil

from CondCore.DBCommon.CondDBSetup_cfi import CondDBSetup

from RecoTauTag.RecoTau.TauDiscriminatorTools import noPrediscriminants
from RecoTauTag.RecoTau.PFRecoTauDiscriminationByLeadingTrackFinding_cfi import \
       pfRecoTauDiscriminationByLeadingTrackFinding

from TauAnalysis.BgEstimationTools.tools.getPSetAttributes import getPSetAttributes

fr_config = {
    'hpsPFTauProducer' : {
        'ewkTauIdHPSloose' : {
            'qcdMuEnrichedDataJet' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_ewkTauIdHPSloose_PPmuX_data_jet20/fakeRate.db',
                'tag' : 'FakeRate',
            },
            'qcdMuEnrichedSimJet' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_ewkTauIdHPSloose_PPmuX_mcPU156bx_jet20/fakeRate.db',
                'tag' : 'FakeRate',
            },
            'WplusJetsDataJet' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_ewkTauIdHPSloose_WplusJets_data_jetNoEta20/fakeRate.db',
                'tag' : 'FakeRate',
            },
            'WplusJetsSimJet' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_ewkTauIdHPSloose_WplusJets_mcPU156bx_jetNoEta20/fakeRate.db',
                'tag' : 'FakeRate',
            }
        }
    }
}

eff_config = {
    'hpsPFTauProducer' : {
        'ewkTauIdHPSloose' : {
            'ZTTsim' : {
                'db' : '/afs/cern.ch/user/f/friis/scratch0/fakerates/fakerate_ewkTauIdHPSloose_Ztautau_mcPU156bx_jet20/fakeRate.db',
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

def configureFakeRateProductionRECO(process, recoTauProducer = 'hpsPFTauProducer', frSet = 'ewkTauIdTaNCloose'):

    fr_and_efficiency_config = {}
    addConfigItems(fr_and_efficiency_config, fr_config)
    addConfigItems(fr_and_efficiency_config, eff_config)

    retVal = {}
    retVal["frConfig"] = {}

    process.tauFakeRates = cms.Sequence()

    for frType, frConfig in fr_and_efficiency_config[recoTauProducer][frSet].items():
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
            PFTauProducer = cms.InputTag(recoTauProducer),
            Prediscriminants = noPrediscriminants,
        )
        preDiscrModuleName = frType + "PreDiscr"
        setattr(process, preDiscrModuleName, preDiscrModule)
	process.tauFakeRates += preDiscrModule

	# create "discriminator" module
        # extracting fake-rate from k-NN tree
        mvaDiscrModule = cms.EDProducer("RecoTauMVADiscriminator",
            PFTauProducer = cms.InputTag(recoTauProducer),
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
            tauSrc = cms.InputTag(recoTauProducer)
        )
        patLUTconverterModuleName = frType + "PATlutConverter"
	setattr(process, patLUTconverterModuleName, patLUTconverterModule)
        process.tauFakeRates += patLUTconverterModule

        retVal['frConfig'][frType] = patLUTconverterModuleName
        
    retVal['sequence'] = process.tauFakeRates

    return retVal

def getFakeRateConfigParameters(process):

    frConfig = {}

    frTypes = getPSetAttributes(process.bgEstFakeRateJetWeights.frTypes)

    for frType in frTypes:
        frConfig[frType] = {}
        frConfig[frType]['srcJetWeight']   = cms.InputTag("bgEstFakeRateJetWeights",   frType)
        frConfig[frType]['srcEventWeight'] = cms.InputTag("bgEstFakeRateEventWeights", frType)
        frConfig[frType]['patLabel']       = "".join(["bgEstFakeRateJetWeight", "_", frType])

        for shift in [ "SysTauIdEffUp", "SysTauIdEffDown" ]:
            if hasattr(process, "bgEstFakeRateJetWeights" + shift):
                frConfig[frType + shift] = {}
                frConfig[frType + shift]['srcJetWeight']   = cms.InputTag("bgEstFakeRateJetWeights"   + shift, frType)
                frConfig[frType + shift]['srcEventWeight'] = cms.InputTag("bgEstFakeRateEventWeights" + shift, frType)
                frConfig[frType + shift]['patLabel']       = "".join(["bgEstFakeRateJetWeight", "_", frType + shift])

    return frConfig

def configureFakeRateProductionPAT(process, recoTauProducerName = 'hpsPFTauProducer', recoTauPreselFlag = 'ewkTauId',
                                   patTauProducerName = 'patTaus', frSet = 'ewkTauIdHPSloose', method = None,
                                   prePatProdSequenceName = "producePrePat"):

    # check validity of method parameter
    if method is None:
        raise ValueError("Undefined method Parameter !!")
    else:
        if method != "simple" and method != "CDF":
            raise ValueError("Invalid method Parameter !!")

    prePatProdSequence = getattr(process, prePatProdSequenceName)

    # produce fake-rates
    frProdConfigRECO = configureFakeRateProductionRECO(process, recoTauProducerName, frSet)
    prePatProdSequence += frProdConfigRECO['sequence']

    # compute fake-rate weights
    #
    # NOTE: jet weights are computed for all (shrinking signal cone) reco::PFTaus,
    #       but only those tau-jet candidates passing preselection on PAT level
    #       must enter event weight computation on analysis level !!
    #
    process.load("TauAnalysis.BgEstimationTools.fakeRateJetWeightProducer_cfi")
    process.bgEstFakeRateJetWeights.frTypes = cms.PSet()
    for frType, patLUT in frProdConfigRECO['frConfig'].items():
        pset = cms.PSet(
            tauJetDiscriminators = cms.VPSet(cms.PSet(
                tauJetIdEffSource = cms.InputTag(frProdConfigRECO['frConfig']['ZTTsim']),
            qcdJetFakeRateSource = cms.InputTag(patLUT),
                tauJetDiscrSource = cms.InputTag(recoTauPreselFlag)
            ))
        )
        setattr(process.bgEstFakeRateJetWeights.frTypes, frType, pset)
    process.bgEstFakeRateJetWeights.allTauJetSource = cms.InputTag(recoTauProducerName)    
    process.bgEstFakeRateJetWeights.preselTauJetSource = cms.InputTag(recoTauProducerName)
    process.bgEstFakeRateJetWeights.method = method
    # EK: put a minPt cut on the taus so we don't get a ton of errors.
    process.bgEstFakeRateJetWeights.minJetPt = 17.0
    prePatProdSequence += process.bgEstFakeRateJetWeights

    # add jet weights computed for tau id. efficiency shifted up/down by one sigma
    if method == "CDF":
        process.bgEstFakeRateJetWeightsSysTauIdEffUp = copy.deepcopy(process.bgEstFakeRateJetWeights)
        process.bgEstFakeRateJetWeightsSysTauIdEffUp.shiftTauIdEff = cms.double(+0.23)
        prePatProdSequence += process.bgEstFakeRateJetWeightsSysTauIdEffUp

        process.bgEstFakeRateJetWeightsSysTauIdEffDown = copy.deepcopy(process.bgEstFakeRateJetWeights)
        process.bgEstFakeRateJetWeightsSysTauIdEffDown.shiftTauIdEff = cms.double(-0.23)
        prePatProdSequence += process.bgEstFakeRateJetWeightsSysTauIdEffDown

    # add fake-rates to pat::Tau
    frConfigParameters = getFakeRateConfigParameters(process)
    patTauProducer = getattr(process, patTauProducerName)
    for frType, frConfig in frConfigParameters.items():
        setattr(patTauProducer.efficiencies, frConfig['patLabel'], frConfig['srcJetWeight'])
    patTauProducer.addEfficiencies = cms.bool(True)

    return frProdConfigRECO

