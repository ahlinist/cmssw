#!/bin/tcsh
###########################################################################
#  Usage:    as a cron job
#  Purpose:  check for Quaero submissions, and for completed Quaero requests
###########################################################################

alias ssh 'ssh -F ~/.ssh/config '
alias scp 'scp -F ~/.ssh/config '

cd QUAERO_CRON_DIR
source environmentVariables.tcsh
setenv QuaeroReviewCommittee "knuteson@fnal.gov"
setenv logLReviewThreshold 3

if ( $QUAERO_EDITION == "quaero" ) then
  setenv cronJobName cron_quaero.tcs
endif
if ( $QUAERO_EDITION == "quaero_development" ) then
  setenv cronJobName cron_quaero_dev
endif
if ( $QUAERO_EDITION == "quaero_mit" ) then
  setenv cronJobName cron_quaero_mit
endif

# Check to make sure this cron job is not still running from the previous time called
if ( `ps -u knuteson | grep -i $cronJobName | wc | awk '{print $1}'` > 1 ) then
  if ( `find ./ -name tmp_cron2\* -mmin +5 | wc -l` >= 1 ) then
    rm tmp_cron2*
    kill `ps -u knuteson | grep -i $cronJobName | awk '{print $1}'`
  else
    exit
  endif
endif

setenv processID $$

# Check for new Quaero submissions, and copy them to the local host
foreach newRequest (`(ssh ${USER}@${QUAERO_PY_HOST} "ls $QUAERO_PY_DIR/$QUAERO_EDITION/requests/[1-9]*/request.txt" > tmp_cron1_${processID}.txt) >& /dev/null; cat tmp_cron1_${processID}.txt | head -5`)
  cd requests/;
  (ssh ${USER}@${QUAERO_PY_HOST} "cd $newRequest:h/../; tar -zcf - $newRequest:h:t" | tar -zxf -) > /dev/null
  cd -;
# (scp -pr ${USER}@${QUAERO_PY_HOST}:$newRequest:h requests/) > /dev/null
  if ( -e requests/$newRequest:h:t ) then
    ssh ${USER}@$QUAERO_PY_HOST "rm -r $newRequest:h" >& /dev/null
  else
    echo 'failed to acquire '${USER}@${QUAERO_PY_HOST}:$newRequest:h
  endif
end
rm tmp_cron1_${processID}.txt

# Take each new request, now on the local host, and submit it to one of our Quaero batch clusters
foreach i ( 1 2 3 4 5 )
if (-e requests/`cat requests/currentID.txt`) then
  if (`cat requests/currentID.txt` == "") then
  else
   if (`ps -u knuteson | grep -i ' quaero ' | wc | awk '{print $1}'` <= 1) then 
      setenv id `cat requests/currentID.txt`
      if ( "$id" == "" ) then
        echo "Bruce, currentID is empty" | /usr/lib/sendmail knuteson@fnal.gov
        exit
      endif
      mv requests/currentID.txt requests/currentID.txt~

      setenv nf `echo $QUAERO_CC_PLATFORMS | awk '{print NF}'`
      echo $QUAERO_CC_PLATFORMS | awk -v z=`expr $id % $nf + 1` '{print $z}' > tmp_cron11_${processID}.txt
      setenv QUAERO_CC_PLATFORM `cat tmp_cron11_${processID}.txt`
      rm tmp_cron11_${processID}.txt
      source environmentVariables.tcsh

      cd requests/;
      (tar -zcf - $id | ssh ${QUAERO_CC_USER}@${QUAERO_CC_HOST} "cd $QUAERO_CC_DIR/$QUAERO_EDITION/requests/; tar -zxf - ") > /dev/null |& tee ../tmp_err1_${processID}.txt
      cd -;
#      (scp -pr requests/$id ${QUAERO_CC_USER}@${QUAERO_CC_HOST}:$QUAERO_CC_DIR/$QUAERO_EDITION/requests/) > /dev/null |& tee tmp_err1_${processID}.txt
      if ( `cat tmp_err1_${processID}.txt` == "" ) then
        rm -r requests/$id/
        @ ida =  $id + 1
        echo $ida > requests/currentID.txt
        echo $id $QUAERO_CC_HOST `date` >> requests/submissions.txt
	rm requests/currentID.txt~
        ssh ${QUAERO_CC_USER}@$QUAERO_CC_HOST "$QUAERO_CC_SETUP; cd $QUAERO_CC_DIR/$QUAERO_EDITION/; source environmentVariables.tcsh; ./qsub 'cd '$QUAERO_CC_LAUNCH_DIR/$QUAERO_EDITION/'; source environmentVariables.tcsh; source requests/'$id'/runcommand.tcsh;'" >& yo.txt
#>& /dev/null
        rm tmp_err1_${processID}.txt
      else
	mv requests/currentID.txt~ requests/currentID.txt
      endif
    endif
  endif
endif
end

foreach platform ( $QUAERO_CC_PLATFORMS )
  setenv QUAERO_CC_PLATFORM $platform
  source environmentVariables.tcsh

  foreach done (`(ssh ${QUAERO_CC_USER}@$QUAERO_CC_HOST "$QUAERO_CC_SETUP; ls $QUAERO_CC_DIR/$QUAERO_EDITION/requests/*/done" > tmp_cron2_${processID}.txt) >& /dev/null; cat tmp_cron2_${processID}.txt | head -1`)
#    cd requests/; (ssh ${QUAERO_CC_USER}@${QUAERO_CC_HOST} "cd $done:h:h/; tar -zcf - $done:h:t" | tar -zxf -) > /dev/null |& tee tmp_err2_${processID}.txt; cd -;
    (scp -pr ${QUAERO_CC_USER}@${QUAERO_CC_HOST}:$done:h requests/) > /dev/null |& tee tmp_err2_${processID}.txt
    if ( `cat tmp_err2_${processID}.txt` == "" ) then
      ssh ${QUAERO_CC_USER}@$QUAERO_CC_HOST "rm -rf $done:h" >& /dev/null
      rm requests/$done:h:t/done
      mv requests/$done:h:t/runcommand.tcsh requests/$done:h:t/.runcommand.tcsh      

      echo "AuthUserFile /home/knuteson/Public/private/.htpasswd" > requests/$done:h:t/.htaccess
      echo "AuthGroupFile /dev/null" >> requests/$done:h:t/.htaccess
      echo "AuthName Quaero" >> requests/$done:h:t/.htaccess
      echo "AuthType Basic" >> requests/$done:h:t/.htaccess
      echo "<Limit GET>" >> requests/$done:h:t/.htaccess
      echo "require user mit" >> requests/$done:h:t/.htaccess
      echo "</Limit>" >> requests/$done:h:t/.htaccess

      (scp -pr requests/$done:h:t ${USER}@${QUAERO_HTML_HOST}:$QUAERO_HTML_DIR/$QUAERO_EDITION/requests/) > /dev/null |& tee tmp_err3_${processID}.txt
      if ( `cat tmp_err3_${processID}.txt` == "" ) then
        if ( `ssh ${USER}@${QUAERO_HTML_HOST} "ls $QUAERO_HTML_DIR/$QUAERO_EDITION/requests/$done:h:t/answer.txt" |& grep -i 'answer\.txt' | grep -v 'No such file' | wc | awk '{print $1}'` == 1 ) then
          setenv logL `grep -i 'log10' requests/${done:h:t}/answer.txt | awk '{print $13}'`
          if ( "$logL" == "" ) setenv logL 0
          if ( ( $QUAERO_EDITION == "quaero" ) && ( `echo "print $logL>$logLReviewThreshold" | python` == 1 ) ) then
            /usr/lib/sendmail "$QuaeroReviewCommittee" < requests/${done:h:t}/answer.txt
            ssh ${USER}@${QUAERO_HTML_HOST} "cd $QUAERO_HTML_DIR/$QUAERO_EDITION/; mv requests/$done:h:t underReview/;"
          else
#bktemp            /usr/lib/sendmail -t < requests/${done:h:t}/answer.txt
          endif
          if ( `grep -i 'knuteson@fnal.gov' requests/${done:h:t}/answer.txt` == "" ) then
            /usr/lib/sendmail knuteson@fnal.gov < requests/${done:h:t}/answer.txt
          endif
          if ( "$done:h:t" != "" ) rm -r requests/$done:h:t
        else
          echo "Bruce, please clean up Quaero's HTML area to make room for the results of additional requests.  Error file is "$done".  Then move files from the cron "$QUAERO_EDITION"/request/ directory to the html request result repository.  Thanks, Quaero" | /usr/lib/sendmail knuteson@fnal.gov
        endif
      endif
      rm tmp_err3_${processID}.txt
    endif
    rm tmp_err2_${processID}.txt
  end
  rm tmp_cron2_${processID}.txt
end  
