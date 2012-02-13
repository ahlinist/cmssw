VERSION=3
## Use golden JSON 166861 for pileup reweighing
## Add PromptReco upto golden JSON 166861.
## Data & Cert = 715 ipb

VERSION=4
## Use PMV tree format while keeping the tree maker label "tree"
## Add 2010A and 2010B
## Data & Cert = 750 ipb
## Added SC quantities for corrections:
##   E, Et, eta, raw E, phi- and eta widths, preshower E
## Added photon gen energy

VERSION=5
## Added crack corrections

VERSION=6
## Use global tag START42_V11

VERSION=7
## Modify the cluster correction calculations.

VERSION=8
## LP2011 datasates and JSON (May10ReReco + 5AugReReco + Prompt) with 2/fb
## Pileup reweighting based on the or of the golden JSONs 136033-166861
##+ and 160404-173244 (includes 2010 data)

VERSION=9
## Added full gen level kinematics

VERSION=10
## 0.875/fb dataset for Summer 11 Wgamma analysis: runs 160431-166967
## Corresponding PU reweighting
## Real data:
##   o May10ReReco (160431-163869)
##   o PromptReco-v4  (AOD:  165071-166967,
##                     RECO: 165071-166967,
##                     RAW:  165358-166967)
## Removed the photon pt cut
## Relaxed the mmgMass mass cut [50,130] GeV
## Relaxed the max dimuon mass cut to 85 GeV

VERSION=11
## Relaxed the max dimuon mass cut to 85 GeV

VERSION=12
## Relaxed the max dimuon mass cut to 85 GeV
## First iteration of the full 2011 Dataset using Nov 4 JSON
## PU reweighting for the full 2011 dataset (A+B)

##################### DATA #########################
JSON_FILE=Cert_160404-180252_5Aug-v3_Prompt_Golden.txt

# DATASET=Run2010B-ZMu-Apr21ReReco-v1
# DATASET=ZMu-May10ReReco-42X-v3
# DATASET=ZMu-PromptSkim-v4_42X-v5
# DATASET=PromptReco-v4_FNAL_42X-v3
# DATASET=05Jul2011ReReco-ECAL-v1_condor_Dimuon_RECO-42X-v9
# DATASET=DoubleMu_Dimuon_AOD_Aug5rereco
# DATASET=DoubleMu_Dimuon_AOD_Prompt_v6
# DATASET=DoubleMu_Run2011A-May10ReReco-v1_glite_Dimuon_RECO-42X-v9
# DATASET=DoubleMu_Run2011A-PromptReco-v4_glite_Dimuon_RECO-42X-v9

# DATASET=DoubleMu_Run2011A-05Jul2011ReReco-ECAL-v1_condor_Dimuon_RECO-42X-v9
# DATASET=DoubleMu_Run2011A-05Aug2011-v1_glite_Dimuon_AOD-42X-v9
# DATASET=DoubleMu_Run2011A-03Oct2011-v1_condor_Dimuon_AOD-42X-v9
## This is actually 2011B, although it says 2011A
DATASET=DoubleMu_Run2011A-PromptReco-v1_condor_Dimuon_AOD-42X-v9



TOTAL_SECTIONS=8
# for SECTION in 7; do
for SECTION in `seq $TOTAL_SECTIONS`; do
    COMMAND="nohup cmsRun energyScaleTree_cfg.py \
        inputFiles_clear \
        inputFiles_load=files_${DATASET}.dat \
        jsonFile=$JSONE_FILE \
        isMC=False \
        maxEvents=-1 \
        outputFile=/wntmp/veverka/esTree_V${VERSION}_${DATASET} \
        totalSections=$TOTAL_SECTIONS \
        section=$SECTION \
        >& /wntmp/veverka/es_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &"
    echo $COMMAND
    eval $COMMAND
done


# # ###################### SMALL MC ####################
# #
# # for DATASET in WToMuNu_TuneZ2_7TeV-pythia6_Summer11_RECO_42X-v4
# # do
# #     nohup cmsRun energyScaleTree_cfg.py \
# #         inputFiles_clear \
# #         inputFiles_load=files_${DATASET}.dat \
# #         isMC=True \
# #         maxEvents=-1 \
# #         outputFile=/wntmp/veverka/esTree_V${VERSION}_${DATASET} \
# #         >& /wntmp/veverka/es_${DATASET}.out &
# # done
#
#
# #G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6_Summer11_AOD_42X-v4
#
##################### LARGE MC ##################
# DATASET=DYToMuMu_pythia6_AOD-42X-v4
# DATASET=DYToMuMu_pythia6_v2_RECO-42X-v4
# TOTAL_SECTIONS=8
# for SECTION in `seq $TOTAL_SECTIONS`; do
#     COMMAND="nohup cmsRun energyScaleTree_cfg.py \
#         print \
#         inputFiles_clear \
#         inputFiles_load=files_${DATASET}.dat \
#         isMC=True \
#         maxEvents=-1 \
#         outputFile=/wntmp/veverka/esTree_V${VERSION}_${DATASET} \
#         globalTag=START42_V11::All \
#         totalSections=$TOTAL_SECTIONS \
#         section=$SECTION \
#         >& /wntmp/veverka/es_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &"
#     echo $COMMAND
#     eval $COMMAND
# done


############### VERY LARGE MC ##################
# DATASET=DYToMuMu_M-20-powheg-pythia_Winter10-v2
# DATASET=DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia_S4-v1_condor_Dimuon_AOD-42X-v9
# TOTAL_SECTIONS=40
# # TOTAL_SECTIONS=8
# BATCH=${1:-1}
# JOBS_PER_BATCH=8
# # for SECTION in 1; do
# for SECTION in `seq $((JOBS_PER_BATCH*(BATCH-1)+1)) $((JOBS_PER_BATCH*BATCH))`; do
#     COMMAND="nohup cmsRun energyScaleTree_cfg.py \
#         print \
#         inputFiles_clear \
#         inputFiles_load=files_${DATASET}.dat \
#         isMC=True \
#         maxEvents=-1 \
#         outputFile=/wntmp/veverka/esTree_V${VERSION}_${DATASET} \
#         globalTag=START42_V11::All \
#         totalSections=$TOTAL_SECTIONS \
#         section=$SECTION \
#         >& /wntmp/veverka/es_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &"
#     echo $COMMAND
#     eval $COMMAND
# done



# ################ Higaga ##################
# # DATASET=GluGluToHToGG_M-120_7TeV-powheg-pythia6_Summer11_42X-v4
# # DATASET=RelValSingleGammaPt10_RECO-42X-v4
# DATASET=G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6_Summer11_AOD_42X-v4
# # DATASET=RelValSingleGammaPt35_RECO-42X-v4
# # DATASET=VBF_HToGG_M-120_7TeV-powheg-pythia6_Summer11_42X-v4
# TOTAL_SECTIONS=8
# # SECTION=3
# #     python -i HigagaenergyScaleTree_cfg.py \
# for SECTION in `seq $TOTAL_SECTIONS`; do
#     nohup cmsRun HigagaenergyScaleTree_cfg.py \
#         print \
#         inputFiles_clear \
#         inputFiles_load=files_${DATASET}.dat \
#         isMC=True \
#         maxEvents=-1 \
#         outputFile=/wntmp/veverka/esTree_V${VERSION}_${DATASET} \
#         totalSections=$TOTAL_SECTIONS \
#         section=$SECTION \
#         >& /wntmp/veverka/es_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &
# done


# ################ Higaga ##################
# DATASET=G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6_Summer11_AOD_42X-v4
# TOTAL_SECTIONS=1
# # SECTION=3
# #     python -i HigagaenergyScaleTree_cfg.py \
# for SECTION in `seq $TOTAL_SECTIONS`; do
#     nohup cmsRun HigagaenergyScaleTree_cfg.py \
#         print \
#         inputFiles_clear \
#         inputFiles_load=files_${DATASET}.dat \
#         isMC=True \
#         maxEvents=100 \
#         outputFile=/wntmp/veverka/esTree_V${VERSION}_${DATASET} \
#         totalSections=$TOTAL_SECTIONS \
#         section=$SECTION \
#         >& /wntmp/veverka/es_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &
# done

