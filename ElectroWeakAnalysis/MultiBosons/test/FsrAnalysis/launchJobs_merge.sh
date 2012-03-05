#! /bin/bash

# ################ VERY LARGE MC ##################
# DATASET=DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia_Fall11-PU_S6_START42_V14B-v1_glite_Dimuon_AOD-42X-v10_10Feb
# DATASET=DoubleMu_Run2011A-16Jan2012-v1_condor_Dimuon_AOD-42X-v10
# DATASET=DoubleMu_Run2011B-16Jan2012-v1_condor_Dimuon_AOD-42X-v10
# DATASET=DoubleMu_Run2011A-16Jan2012-v1_glite_Dimuon_AOD-42X-v10
# DATASET=DoubleMu_Run2011B-16Jan2012-v1_glite_Dimuon_AOD-42X-v10
# DATASET=DoubleMu_Run2011A-30Nov2011-v1_condor_Dimuon_AOD-42X-v10
# DATASET=DoubleMu_Run2011B-30Nov2011-v1_condor_Dimuon_AOD-42X-v10
# DATASET=DoubleMu_Run2011A-30Nov2011-v1_condor_Dimuon_AOD-42X-v10_DBS
DATASET=DoubleMu_Run2011B-30Nov2011-v1_condor_Dimuon_AOD-42X-v10_DBS

OUTPUT_PATH=/wntmp/veverka/$DATASET
if [[ ! -d $OUTPUT_PATH ]]; then
    mkdir $OUTPUT_PATH || exit 1
fi

TOTAL_BATCHES=1
JOBS_PER_BATCH=8
((TOTAL_SECTIONS=JOBS_PER_BATCH*TOTAL_BATCHES))
BATCH=${1:-1}

FIRST_SECTION=$((JOBS_PER_BATCH*(BATCH-1)+1))
LAST_SECTION=$((JOBS_PER_BATCH*BATCH))
for SECTION in $(seq $FIRST_SECTION $LAST_SECTION); do
    nohup cmsRun merge_cfg.py \
        print \
        inputFiles_clear \
        inputFiles_load=files_${DATASET}.dat \
        maxEvents=-1 \
        outputFile=$OUTPUT_PATH/VGammaPAT_DimuonSkim_part${SECTION}of${TOTAL_SECTIONS}_ \
        totalSections=$TOTAL_SECTIONS \
        section=$SECTION \
        >& $OUTPUT_PATH/merge_part${SECTION}of${TOTAL_SECTIONS}.out &
done
