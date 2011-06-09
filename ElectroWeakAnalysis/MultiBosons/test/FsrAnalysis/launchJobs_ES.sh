# ##################### DATA #########################
# # for DATASET in ZMu-May10ReReco-42X-v3
# for DATASET in ZMu-May10ReReco-42X-v3\
#     ZMu-PromptSkim-v4_42X-v5 \
#     PromptReco-v4_FNAL_42X-v3
# do
#     nohup cmsRun energyScaleTree_cfg.py \
#         inputFiles_clear \
#         inputFiles_load=files_${DATASET}.dat \
#         jsonFile=Cert_160404-165970_7TeV_PromptReco_Collisions11_JSON.txt \
#         isMC=False \
#         maxEvents=-1 \
#         outputFile=/wntmp/veverka/esTree_${DATASET}_V2 \
#         >& /wntmp/veverka/es_${DATASET}.out &
# done



###################### SMALL MC ####################

for DATASET in WToMuNu_TuneZ2_7TeV-pythia6_Summer11_RECO_42X-v4
do
    nohup cmsRun energyScaleTree_cfg.py \
        inputFiles_clear \
        inputFiles_load=files_${DATASET}.dat \
        isMC=True \
        maxEvents=-1 \
        outputFile=/wntmp/veverka/esTree_${DATASET}_V2 \
        >& /wntmp/veverka/es_${DATASET}.out &
done


#G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6_Summer11_AOD_42X-v4

# ###################### LARGE MC ##################
# DATASET=DYToMuMu_pythia6_AOD-42X-v4
# TOTAL_SECTIONS=8
# for SECTION in `seq $TOTAL_SECTIONS`; do
#     nohup cmsRun energyScaleTree_cfg.py \
#         print \
#         inputFiles_clear \
#         inputFiles_load=files_${DATASET}.dat \
#         isMC=True \
#         maxEvents=-1 \
#         outputFile=/wntmp/veverka/esTree_${DATASET}_V2 \
#         totalSections=$TOTAL_SECTIONS \
#         section=$SECTION \
#         >& /wntmp/veverka/es_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &
# done


# ################ VERY LARGE MC ##################
# DATASET=DYToMuMu_M-20-powheg-pythia_Winter10-v2
# TOTAL_SECTIONS=40
# BATCH=${1:-1}
# TOTAL_BATCHES=8
# for SECTION in `seq $((8*(BATCH-1)+1)) $((8*BATCH))`; do
#     nohup cmsRun energyScaleTree_cfg.py \
#         print \
#         inputFiles_clear \
#         inputFiles_load=files_${DATASET}.dat \
#         isMC=True \
#         maxEvents=-1 \
#         outputFile=/wntmp/veverka/esTree_${DATASET}_V2 \
#         totalSections=$TOTAL_SECTIONS \
#         section=$SECTION \
#         >& /wntmp/veverka/es_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &
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
#         outputFile=/wntmp/veverka/esTree_${DATASET}_V2 \
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
#         outputFile=/wntmp/veverka/esTree_${DATASET}_V2 \
#         totalSections=$TOTAL_SECTIONS \
#         section=$SECTION \
#         >& /wntmp/veverka/es_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &
# done

