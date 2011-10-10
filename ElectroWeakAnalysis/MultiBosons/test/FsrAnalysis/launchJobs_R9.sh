VERSION=1
## Added cluster-correction and gen-level variables

# ###################### SMALL MC ####################
# DATASET=g93p01
# DATASET=g94cms
DATASET=g94p02
TOTAL_SECTIONS=1
for SECTION in `seq $TOTAL_SECTIONS`; do
    nohup cmsRun r9Tree_cfg.py \
        print \
        inputFiles_clear \
        inputFiles_load=files_${DATASET}.dat \
        maxEvents=-1 \
        outputFile=/tmp/veverka/r9Tree_V${VERSION}_${DATASET} \
        totalSections=$TOTAL_SECTIONS \
        section=$SECTION \
        >& /tmp/veverka/r9_${DATASET}_${SECTION}of${TOTAL_SECTIONS}.out &
done
