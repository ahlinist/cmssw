# files_Mu_Run2010A-Dec22ReReco_v1.dat
# files_Mu_Run2010B-DiLeptonMu-Dec22ReReco_v2.dat
#
# files_QCD_Pt-20_MuEnrichedPt-15_Winter10.dat
# files_TTJets_TuneZ2-madgraph-Winter10.dat
# files_WJetsToLNu_TuneZ2_7TeV-madgraph_Winter10.dat
#
# files_DYToMuMu_M-20-powheg-pythia_Winter10-v1.dat
# files_DYToMuMu_M-20-powheg-pythia_Winter10-v2.dat
#
# files_GJets_TuneD6T_HT-40To100-madgraph_Winter10.dat
# files_GluGluToHToGG_M-120_7TeV-powheg-pythia6_Winter10.dat

# ##################### DATA #########################
# for DATASET in Mu_Run2010A-Dec22ReReco_v1 \
#     Mu_Run2010B-DiLeptonMu-Dec22ReReco_v2
# do
#     nohup cmsRun PmvTreeMaker_cfg.py \
#         inputFiles_clear \
#         inputFiles_load=files_${DATASET}.dat \
#         isMC=False \
#         maxEvents=-1 \
#         outputFile=/wntmp/veverka/pmvTree_${DATASET}_V3 \
#         >& /wntmp/veverka/pmv_${DATASET}_json.out &
# done
#
#
#
# ###################### SMALL MC ####################
# for DATASET in QCD_Pt-20_MuEnrichedPt-15_Winter10 \
#     TTJets_TuneZ2-madgraph-Winter10 \
#     WJetsToLNu_TuneZ2_7TeV-madgraph_Winter10;
# do
#     nohup cmsRun PmvTreeMaker_cfg.py \
#         inputFiles_clear \
#         inputFiles_load=files_${DATASET}.dat \
#         isMC=True \
#         maxEvents=-1 \
#         outputFile=/wntmp/veverka/pmvTree_${DATASET}_V3 \
#         >& /wntmp/veverka/pmv_${DATASET}.out &
# done
#


# ###################### LARGE MC ##################
# DATASET=DYToMuMu_M-20-powheg-pythia_Winter10-v1
# TOTAL_SECTIONS=8
# for SECTION in `seq $TOTAL_SECTIONS`; do
#     nohup cmsRun PmvTreeMaker_cfg.py \
#         print \
#         inputFiles_clear \
#         inputFiles_load=files_${DATASET}.dat \
#         isMC=True \
#         maxEvents=-1 \
#         outputFile=/wntmp/veverka/pmvTree_${DATASET}_V3 \
#         totalSections=$TOTAL_SECTIONS \
#         section=$SECTION \
#         >& /wntmp/veverka/pmv_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &
# done
#

# ################ VERY LARGE MC ##################
# DATASET=DYToMuMu_M-20-powheg-pythia_Winter10-v2
# TOTAL_SECTIONS=40
# BATCH=${1:-1}
# TOTAL_BATCHES=8
# for SECTION in `seq $((8*(BATCH-1)+1)) $((8*BATCH))`; do
#     nohup cmsRun PmvTreeMaker_cfg.py \
#         print \
#         inputFiles_clear \
#         inputFiles_load=files_${DATASET}.dat \
#         isMC=True \
#         maxEvents=-1 \
#         outputFile=/wntmp/veverka/pmvTree_${DATASET}_V3 \
#         totalSections=$TOTAL_SECTIONS \
#         section=$SECTION \
#         >& /wntmp/veverka/pmv_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &
# done


# ################ Higaga ##################
# DATASET=GluGluToHToGG_M-120_7TeV-powheg-pythia6_Winter10
# TOTAL_SECTIONS=4
# # SECTION=3
# #     python -i HigagaPmvTreeMaker_cfg.py \
# for SECTION in `seq $TOTAL_SECTIONS`; do
#     nohup cmsRun HigagaPmvTreeMaker_cfg.py \
#         print \
#         inputFiles_clear \
#         inputFiles_load=files_${DATASET}.dat \
#         isMC=True \
#         maxEvents=-1 \
#         outputFile=/wntmp/veverka/pmvTree_${DATASET}_V3 \
#         totalSections=$TOTAL_SECTIONS \
#         section=$SECTION \
#         >& /wntmp/veverka/pmv_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &
# done


################ Higaga ##################
# DATASET=GJets_TuneD6T_HT-40To100-madgraph_Winter10
DATASET=GluGluToHToGG_M-120_7TeV-powheg-pythia6_Winter10
TOTAL_SECTIONS=8
# SECTION=3
#     python -i HigagaPmvTreeMaker_cfg.py \
for SECTION in `seq $TOTAL_SECTIONS`; do
    nohup cmsRun HigagaPmvTreeMaker_cfg.py \
        print \
        inputFiles_clear \
        inputFiles_load=files_${DATASET}.dat \
        isMC=True \
        maxEvents=-1 \
        outputFile=/wntmp/veverka/pmvTree_${DATASET}_V4 \
        totalSections=$TOTAL_SECTIONS \
        section=$SECTION \
        >& /wntmp/veverka/pmv_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &
done


