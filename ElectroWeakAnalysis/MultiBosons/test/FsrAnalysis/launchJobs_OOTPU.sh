## comment from vim @ home
VERSION=1
# DATASET=PU_noOOTPU
# DATASET=m50
# DATASET=p50
DATASET=noPU_noOOTPU
PREFIX=rfio:/castor/cern.ch/cms/store/caf/user/apresyan/user/a/apresyan/SingleGammaFlatPt10To100
# INPUT_FILE=PU_noOOTPU/SingleGammaFlatPt10To100_cfi_RAW2DIGI_L1Reco_RECO_PU_10int_PU_noOOTPU_1_1_Kou.root
# INPUT_FILE=m50/SingleGammaFlatPt10To100_cfi_RAW2DIGI_L1Reco_RECO_PU_10int_m50ns_1_1_6hP.root
INPUT_FILE=noPU_noOOTPU/SingleGammaFlatPt10To100_cfi_RAW2DIGI_L1Reco_RECO_PU_10int_noPU_noOOTPU_1_1_Q9X.root
# INPUT_FILE=p50/SingleGammaFlatPt10To100_cfi_RAW2DIGI_L1Reco_RECO_PU_10int_p50ns_1_1_8PD.root
#    isMC=True \
nohup cmsRun r9Tree_cfg.py \
    print \
    inputFiles_clear \
    inputFiles=$PREFIX/$INPUT_FILE \
    maxEvents=-1 \
    outputFile=/tmp/veverka/r9Tree_V${VERSION}_${DATASET} \
    >& /tmp/veverka/r9Tree_V${VERSION}_${DATASET}.out &
