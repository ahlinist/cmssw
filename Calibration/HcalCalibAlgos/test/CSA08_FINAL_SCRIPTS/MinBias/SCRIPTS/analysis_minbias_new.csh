#!/bin/csh
date
setenv SRC ${3}
setenv ROOTDIR ${4}/ROOT
setenv LOGDIR ${5}

#### for work at CAF
setenv STAGE_SVCCLASS cmscaf
setenv STAGER_TRACE 3
########

setenv WORKDIR `pwd`/${1}
mkdir ${WORKDIR}

cd ${SRC}
eval `scramv1 runtime -csh`

cp ${4}/TMP/${1}.cfg ${WORKDIR}

cd ${WORKDIR}
ls ${WORKDIR}

cmsRun ${1}.cfg>&analysis_minbias.log

rfcp analysis_minbias.root ${ROOTDIR}/${1}.root
rfcp analysis_minbias.log ${LOGDIR}/${1}.log
