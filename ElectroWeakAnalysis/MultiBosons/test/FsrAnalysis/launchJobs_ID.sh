VERSION=1
## Use relaxed selection similar to Otto's vg talk on 24 June
## No isolations, no eta restrictions, large mass window.
## Data & Cert = 750 ipb


##################### DATA #########################
# DATASET=Run2010B-ZMu-Apr21ReReco-v1
# # DATASET=ZMu-May10ReReco-42X-v3
# # DATASET=ZMu-PromptSkim-v4_42X-v5
# # DATASET=PromptReco-v4_FNAL_42X-v3
# TOTAL_SECTIONS=8
# for SECTION in `seq $TOTAL_SECTIONS`; do
#     COMMAND="nohup cmsRun photonIdTree_cfg.py \
#         inputFiles_clear \
#         inputFiles_load=files_${DATASET}.dat \
#         jsonFile=tmp.json \
#         isMC=False \
#         maxEvents=-1 \
#         outputFile=/wntmp/veverka/idTree_V${VERSION}_${DATASET} \
#         totalSections=$TOTAL_SECTIONS \
#         section=$SECTION \
#         >& /wntmp/veverka/id_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &"
#     echo $COMMAND | sed 's/ \+/ /g'
#     eval $COMMAND
# done


# ###################### SMALL MC ####################
#
# for DATASET in WToMuNu_TuneZ2_7TeV-pythia6_Summer11_RECO_42X-v4
# do
#     nohup cmsRun photonIdTree_cfg.py \
#         inputFiles_clear \
#         inputFiles_load=files_${DATASET}.dat \
#         isMC=True \
#         maxEvents=-1 \
#         outputFile=/wntmp/veverka/idTree_V${VERSION}_${DATASET} \
#         >& /wntmp/veverka/es_${DATASET}.out &
# done


#G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6_Summer11_AOD_42X-v4

##################### LARGE MC ##################
# DATASET=DYToMuMu_pythia6_AOD-42X-v4
DATASET=DYToMuMu_pythia6_v2_RECO-42X-v4
TOTAL_SECTIONS=8
for SECTION in `seq $TOTAL_SECTIONS`; do
    COMMAND="nohup cmsRun photonIdTree_cfg.py \
        print \
        inputFiles_clear \
        inputFiles_load=files_${DATASET}.dat \
        isMC=True \
        maxEvents=-1 \
        outputFile=/wntmp/veverka/idTree_V${VERSION}_${DATASET} \
        totalSections=$TOTAL_SECTIONS \
        section=$SECTION \
        >& /wntmp/veverka/id_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &"
    echo $COMMAND
    eval $COMMAND
done


# ################ VERY LARGE MC ##################
# DATASET=DYToMuMu_M-20-powheg-pythia_Winter10-v2
# TOTAL_SECTIONS=40
# BATCH=${1:-1}
# TOTAL_BATCHES=8
# for SECTION in `seq $((8*(BATCH-1)+1)) $((8*BATCH))`; do
#     nohup cmsRun photonIdTree_cfg.py \
#         print \
#         inputFiles_clear \
#         inputFiles_load=files_${DATASET}.dat \
#         isMC=True \
#         maxEvents=-1 \
#         outputFile=/wntmp/veverka/idTree_V${VERSION}_${DATASET} \
#         totalSections=$TOTAL_SECTIONS \
#         section=$SECTION \
#         >& /wntmp/veverka/id_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &
# done


# ################ Higaga ##################
# # DATASET=GluGluToHToGG_M-120_7TeV-powheg-pythia6_Summer11_42X-v4
# # DATASET=RelValSingleGammaPt10_RECO-42X-v4
# DATASET=G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6_Summer11_AOD_42X-v4
# # DATASET=RelValSingleGammaPt35_RECO-42X-v4
# # DATASET=VBF_HToGG_M-120_7TeV-powheg-pythia6_Summer11_42X-v4
# TOTAL_SECTIONS=8
# # SECTION=3
# #     python -i HigagaphotonIdTree_cfg.py \
# for SECTION in `seq $TOTAL_SECTIONS`; do
#     nohup cmsRun HigagaphotonIdTree_cfg.py \
#         print \
#         inputFiles_clear \
#         inputFiles_load=files_${DATASET}.dat \
#         isMC=True \
#         maxEvents=-1 \
#         outputFile=/wntmp/veverka/idTree_V${VERSION}_${DATASET} \
#         totalSections=$TOTAL_SECTIONS \
#         section=$SECTION \
#         >& /wntmp/veverka/id_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &
# done


# ################ Higaga ##################
# DATASET=G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6_Summer11_AOD_42X-v4
# TOTAL_SECTIONS=1
# # SECTION=3
# #     python -i HigagaphotonIdTree_cfg.py \
# for SECTION in `seq $TOTAL_SECTIONS`; do
#     nohup cmsRun HigagaphotonIdTree_cfg.py \
#         print \
#         inputFiles_clear \
#         inputFiles_load=files_${DATASET}.dat \
#         isMC=True \
#         maxEvents=100 \
#         outputFile=/wntmp/veverka/idTree_V${VERSION}_${DATASET} \
#         totalSections=$TOTAL_SECTIONS \
#         section=$SECTION \
#         >& /wntmp/veverka/id_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &
# done

