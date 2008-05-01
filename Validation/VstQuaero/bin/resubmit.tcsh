#!/bin/tcsh
################################################################################
# Usage:  Quaero/utilities/resubmit.tcsh <cluster> <quaero_edition> <id>
#
# Purpose:  Resubmits an already evaluated Quaero submission, to test changes
################################################################################

alias ssh 'ssh -F ~/.ssh/config '
alias scp 'scp -F ~/.ssh/config '

cd $RELEASE_DIR
setenv cluster $1
setenv quaero_edition $2
setenv id $3
setenv processID $$
if ( $id == "" ) then
  exit
endif
source Quaero/utilities/install.tcsh $quaero_edition
if ( $cluster == "any" ) then
  setenv nf `echo $QUAERO_CC_PLATFORMS | awk '{print NF}'`
  echo $QUAERO_CC_PLATFORMS | awk -v z=`expr $id % $nf + 1` '{print $z}' > tmp_cron11_${processID}.txt
  setenv cluster `cat tmp_cron11_${processID}.txt`
  echo $cluster
  rm tmp_cron11_${processID}.txt 
endif
if ( $cluster == "local" ) then
  setenv QUAERO_CC_PLATFORM `getQuaeroClusterName.tcsh`
else
  setenv QUAERO_CC_PLATFORM $cluster
endif
source Quaero/utilities/environmentVariables.tcsh

ssh ${USER}@${QUAERO_HTML_HOST} "cd $QUAERO_HTML_DIR/$QUAERO_EDITION/requests/$id/; mkdir old >& /dev/null; mv *.* old/; mv old/request.txt .request.txt >& /dev/null; echo 'To: Knuteson <knuteson@mit.edu>' > request.txt; tail +2 .request.txt >> request.txt; rm .request.txt >& /dev/null; mv old/runcommand.tcsh .runcommand.tcsh >& /dev/null; mv .runcommand.tcsh runcommand.tcsh; mkdir signalfiles >& /dev/null; mkdir signalfiles/hepg/ >& /dev/null; mkdir signalfiles/lep2/ >& /dev/null; mkdir signalfiles/tev1/ >& /dev/null; mkdir signalfiles/tev2/ >& /dev/null; mkdir signalfiles/hera/ >& /dev/null; rm signalfiles/*/*;"
#ls -l answer.txt | awk '{print $6 "_" $7 "_" $8}' | head -1; mkdir _$atime; 

ssh ${USER}@${QUAERO_HTML_HOST} "cd $QUAERO_HTML_DIR/$QUAERO_EDITION/requests/; scp -pr $id ${QUAERO_CC_USER}@${QUAERO_CC_HOST}:$QUAERO_CC_DIR/$QUAERO_EDITION/requests/; rm -r $id/;"
   
if ( $cluster == "local" ) then
  cd $QUAERO_CC_DIR/$QUAERO_EDITION/; 
  source environmentVariables.tcsh; 
  ./quaero $id -debugLevel5 |& tee requests/$id/logfile.txt
  ./cleanup ./ $id knuteson@fnal.gov > /dev/null
  scp -r requests/$id ${USER}@${QUAERO_HTML_HOST}:$QUAERO_HTML_DIR/$QUAERO_EDITION/requests/
  rm -r requests/$id;
  cd -;
else
  ssh ${QUAERO_CC_USER}@$QUAERO_CC_HOST "$QUAERO_CC_SETUP; cd $QUAERO_CC_DIR/$QUAERO_EDITION/; source environmentVariables.tcsh; ./qsub 'cd '$QUAERO_CC_DIR/$QUAERO_EDITION/'; source environmentVariables.tcsh; source requests/'$id'/runcommand.tcsh;'"
endif

setenv QUAERO_CC_PLATFORM `getQuaeroClusterName.tcsh`
source Quaero/utilities/environmentVariables.tcsh

echo 'done!'


