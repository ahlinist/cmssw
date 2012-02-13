#! /bin/bash
# VERSION=8
## Use golden JSON 166502 for pileup reweighing
## Add PromptReco upto golden JSON 166502.
## Add more data files that were extended with previously failed
##+ jobs (ACDC)
## Data & Cert = 499 ipb

# VERSION=9
## Use golden JSON 166861 for pileup reweighing
## Add 2010B and PromptReco upto golden JSON 166861.
## Data & Cert = 750 ipb

# VERSION=10
## Added crack correction factors

# VERSION=11
## Move to Summer11 S4 MC samples.
## Use MC pileup distribution from S4 gamma + jet.
## Loosen the invariant mass window from [60,120] to [50,130]

# VERSION=12
## Fixed a bug with MC pileup distribution wrong binning
## Use golden JSON for runs 160404-166861

# VERSION=13
## Use the Lepton Photon 2011 goleden JSON (runs 160404-173xxx)

## VERSION=14
## Use the 28 Oct 2011 golden JSON

# VERSION=15
## First iteration of the full 2011 Dataset using Nov 4 JSON
## PU reweighting for the full 2011 dataset (A+B)

# VERSION=16
## PU weights for 2011A only

VERSION=17
## PU weights for 2011B only



##################### DATA #########################
# DATASET=ZMu-PromptSkim-v4_42X-v5
# DATASET=PromptReco-v4_FNAL_42X-v3
# DATASET=ZMu-May10ReReco-42X-v3
# DATASET=Run2010B-ZMu-Apr21ReReco-v1
# DATASET=05Jul2011ReReco-ECAL-v1_condor_Dimuon_RECO-42X-v9
# DATASET=DoubleMu_Dimuon_AOD_Aug5rereco
# DATASET=DoubleMu_Dimuon_AOD_Prompt_v5
# DATASET=DoubleMu_Dimuon_AOD_Prompt_v6

# DATASET=DoubleMu_Run2011A-05Aug2011-v1_glite_Dimuon_AOD-42X-v9
# DATASET=DoubleMu_Run2011A-03Oct2011-v1_condor_Dimuon_AOD-42X-v9
# DATASET=DoubleMu_Run2011A-PromptReco-v6_glite_Dimuon_AOD-42X-v9
## This is actually 2011B, although it says 2011A
# DATASET=DoubleMu_Run2011A-PromptReco-v1_condor_Dimuon_AOD-42X-v9

# DATASET=DoubleMu_Run2011A-05Jul2011ReReco-ECAL-v1_condor_Dimuon_RECO-42X-v9
# DATASET=DoubleMu_Run2011A-05Aug2011-v1_glite_Dimuon_RECO-42X-v9
# DATASET=DoubleMu_Run2011A-03Oct2011-v1_condor_Dimuon_RECO-42X-v9
# DATASET=DoubleMu_Run2011A-PromptReco-v6_glite_Dimuon_RECO-42X-v9
# DATASET=DoubleMu_Run2011B-PromptReco-v1_condor_Dimuon_RECO-42X-v9

# JSON_FILE=Cert_136033-166861_Apr21_May10_Prompt-v4_Golden.json
# JSON_FILE=Cert_170249-172619_7TeV_ReReco5Aug_Collisions11_JSON.txt
# JSON_FILE=Cert_160404-179431_7TeV_PromptReco_Collisions11_JSON.txt
# JSON_FILE=Cert_Golden_new.json
# JSON_FILE=Cert_160404-180252_5Aug-v3_Prompt_Golden.txt
# TOTAL_SECTIONS=8
# for SECTION in `seq $TOTAL_SECTIONS`; do
#     nohup cmsRun PmvTreeMaker_cfg.py \
#         inputFiles_clear \
#         inputFiles_load=files_${DATASET}.dat \
#         jsonFile=$JSON_FILE \
#         isMC=False \
#         maxEvents=-1 \
#         outputFile=/wntmp/veverka/pmvTree_V${VERSION}_${DATASET} \
#         totalSections=$TOTAL_SECTIONS \
#         section=$SECTION \
#         >& /wntmp/veverka/pmv_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &
# done



###################### SMALL MC ####################
# DATASETS='WJetsToLNu_TuneZ2_7TeV-madgraph-tauola_Summer11-PU_S4_START42_V11-v1_condor_Dimuon_AOD-42X-v9
# TTJets_TuneZ2_7TeV-madgraph-tauola_S4-v2_condor_Dimuon_AOD-42X-v9
# QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6_S4-v1_condor_Dimuon_AOD-42X-v9'
# DATASETS='
# WJetsToLNu_TuneZ2_7TeV-madgraph-tauola_Summer11-PU_S4_START42_V11-v1_condor_Dimuon_RECO-42X-v9
# TTJets_TuneZ2_7TeV-madgraph-tauola_Summer11-PU_S4_START42_V11-v2_condor_Dimuon_RECO-42X-v9
# QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6_Summer11-PU_S4_START42_V11-v1_condor_Dimuon_RECO-42X-v9
# '
# for DATASET in $DATASETS
# do
#     nohup cmsRun PmvTreeMaker_cfg.py \
#         inputFiles_clear \
#         inputFiles_load=files_${DATASET}.dat \
#         isMC=True \
#         maxEvents=-1 \
#         outputFile=/wntmp/veverka/pmvTree_V${VERSION}_${DATASET} \
#         >& /wntmp/veverka/pmv_${DATASET}.out &
# done
#
#G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6_Summer11_AOD_42X-v4

###################### LARGE MC ##################
# DATASET=DYToMuMu_pythia6_AOD-42X-v4
# DATASET=DYToMuMu_pythia6_v1_RECO-42X-v4
# DATASET=DYToMuMu_pythia6_v2_RECO-42X-v4
# DATASET=DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia_S4-v1_condor_Dimuon_AOD-42X-v9
# TOTAL_SECTIONS=8
# for SECTION in `seq $TOTAL_SECTIONS`; do
#     COMMAND="nohup cmsRun PmvTreeMaker_cfg.py \
#         print \
#         inputFiles_clear \
#         inputFiles_load=files_${DATASET}.dat \
#         isMC=True \
#         maxEvents=-1 \
#         outputFile=/wntmp/veverka/pmvTree_V${VERSION}_${DATASET} \
#         globalTag=START42_V11::All \
#         totalSections=$TOTAL_SECTIONS \
#         section=$SECTION \
#         >& /wntmp/veverka/pmv_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &"
#     echo $COMMAND
#     eval $COMMAND
# done

# ################ VERY LARGE MC ##################
# DATASET=DYToMuMu_M-20-powheg-pythia_Winter10-v2
# DATASET=DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia_S4-v1_condor_Dimuon_AOD-42X-v9
DATASET=DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia_Summer11-PU_S4_START42_V11-v1_glidein_Dimuon_RECO-42X-v9
# DATASET=DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola_Summer11-PU_S4_START42_V11-v1_condor_Dimuon_AOD-42X-v9

#TOTAL_BATCHES=8
TOTAL_BATCHES=2
JOBS_PER_BATCH=8
((TOTAL_SECTIONS=JOBS_PER_BATCH*TOTAL_BATCHES))
BATCH=${1:-1}

FIRST_SECTION=$((JOBS_PER_BATCH*(BATCH-1)+1))
LAST_SECTION=$((JOBS_PER_BATCH*BATCH))
for SECTION in $(seq $FIRST_SECTION $LAST_SECTION); do
    nohup cmsRun PmvTreeMaker_cfg.py \
        print \
        inputFiles_clear \
        inputFiles_load=files_${DATASET}.dat \
        isMC=True \
        maxEvents=-1 \
        outputFile=/wntmp/veverka/pmvTree_V${VERSION}_${DATASET} \
        totalSections=$TOTAL_SECTIONS \
        section=$SECTION \
        >& /wntmp/veverka/pmv_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &
done


################ Higaga ##################
# DATASET=GluGluToHToGG_M-120_7TeV-powheg-pythia6_Summer11_42X-v4
# DATASET=RelValSingleGammaPt10_RECO-42X-v4
# DATASET=G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6_Summer11_AOD_42X-v4
# DATASET=RelValSingleGammaPt35_RECO-42X-v4
# DATASET=VBF_HToGG_M-120_7TeV-powheg-pythia6_Summer11_42X-v4
# DATASET=G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6_S4-v1_condor_Inclusive_AOD-42X-v9
# TOTAL_SECTIONS=8
# SECTION=3
#     python -i HigagaPmvTreeMaker_cfg.py \
# for SECTION in `seq $TOTAL_SECTIONS`; do
#     nohup cmsRun HigagaPmvTreeMaker_cfg.py \
#         print \
#         inputFiles_clear \
#         inputFiles_load=files_${DATASET}.dat \
#         isMC=True \
#         maxEvents=-1 \
#         outputFile=/wntmp/veverka/pmvTree_V${VERSION}_${DATASET} \
#         totalSections=$TOTAL_SECTIONS \
#         section=$SECTION \
#         >& /wntmp/veverka/pmv_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &
# done


# ################ Higaga ##################
# DATASET=G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6_Summer11_AOD_42X-v4
# TOTAL_SECTIONS=1
# # SECTION=3
# #     python -i HigagaPmvTreeMaker_cfg.py \
# for SECTION in `seq $TOTAL_SECTIONS`; do
#     nohup cmsRun HigagaPmvTreeMaker_cfg.py \
#         print \
#         inputFiles_clear \
#         inputFiles_load=files_${DATASET}.dat \
#         isMC=True \
#         maxEvents=100 \
#         outputFile=/wntmp/veverka/pmvTree_V${VERSION}_${DATASET} \
#         totalSections=$TOTAL_SECTIONS \
#         section=$SECTION \
#         >& /wntmp/veverka/pmv_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &
# done
