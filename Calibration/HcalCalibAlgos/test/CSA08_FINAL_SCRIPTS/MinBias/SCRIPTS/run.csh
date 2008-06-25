#!/bin/csh

setenv SRC /afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/MinBias/CMSSW_2_0_7/src/Calibration/HcalCalibAlgos/test
setenv WORK /afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/MinBias/CMSSW_2_0_7/src/Calibration/HcalCalibAlgos/test/WORK_CAF_S43_NS
setenv LOGDIR /castor/cern.ch/user/k/kodolova/CSA08/MINBIAS_CAF_207/NS_30May1900
setenv SCRIPTS /afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/MinBias/SCRIPTS

set j=1
foreach i (`cat ${WORK}/TMP/list050`)
echo ${i} ${j}
### For CAF
bsub -q cmscaf ${SCRIPTS}/analysis_minbias_new.csh ${i} ${j} ${SRC} ${WORK} ${LOGDIR}
###sleep 60
@ j = ${j} + "1"
end
