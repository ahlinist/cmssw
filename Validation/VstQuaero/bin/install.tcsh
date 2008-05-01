#!/bin/tcsh
##################################################
# Install script for Quaero package
# Usage:  source Quaero/utilities/install.tcsh <quaero_edition> code [clean]
#    or   source Quaero/utilities/install.tcsh <quaero_edition> cc_clean
#    or   source Quaero/utilities/install.tcsh <quaero_edition> web [code [clean]]
#    or   source Quaero/utilities/install.tcsh <quaero_edition> doc
#    or   source Quaero/utilities/install.tcsh <quaero_edition> exp <colliderRun>/<experiment> [full]
#    or   source Quaero/utilities/install.tcsh <quaero_edition> exp sys
#
# Bruce Knuteson
# March 2004
##################################################

date

##################################################
#  Check for proper quaero_edition
##################################################

setenv RELEASE_DIR `pwd`
setenv QUAERO_CC_PLATFORM mitcdf
source $RELEASE_DIR/Quaero/utilities/environmentVariables.tcsh
if(($1 == "quaero") || ($1 == "quaero_development") || ($1 == "quaero_mit")) then
else
  echo "Usage:  source Quaero/utilities/install.tcsh <quaero_mit | quaero_development | quaero> [code [clean] | web | doc]"
  echo "   or   source Quaero/utilities/install.tcsh exp <colliderRun>/<experiment>"
  exit
endif
setenv QUAERO_EDITION $1


##################################################
#  Install Experiment
##################################################

if ($2 == "exp") then
  setenv experiment $3
  if ($3 == "") then
    echo "please provide <colliderRun>/<experiment>"
    exit
  endif

#  Loop over platforms on which Quaero has batch queue access

  foreach platform ( $QUAERO_CC_PLATFORMS )
    echo '\n\n\n\n\n\n\n\n' $platform
    setenv QUAERO_CC_PLATFORM $platform
    source $RELEASE_DIR/Quaero/utilities/environmentVariables.tcsh   
    setenv QUAERO_EXP_DIR $QUAERO_CC_RELEASE_DIR/Quaero/runDir/$QUAERO_EDITION/experiments/
    (ssh -q ${QUAERO_CC_USER}@$QUAERO_CC_COMPILE_HOST "$QUAERO_CC_SETUP; mkdir $QUAERO_EXP_DIR/; mkdir $QUAERO_EXP_DIR/sys/; mkdir $QUAERO_EXP_DIR/tev1/; mkdir $QUAERO_EXP_DIR/tev1/; mkdir $QUAERO_EXP_DIR/tev2/; mkdir $QUAERO_EXP_DIR/lep2/; mkdir $QUAERO_EXP_DIR/hera/; mkdir $QUAERO_EXP_DIR/lhc/; mkdir $QUAERO_EXP_DIR/tev1/d0/; mkdir $QUAERO_EXP_DIR/tev2/cdf/; mkdir $QUAERO_EXP_DIR/tev2/d0/; mkdir $QUAERO_EXP_DIR/tev2/hint/; mkdir $QUAERO_EXP_DIR/lep2/aleph/; mkdir $QUAERO_EXP_DIR/lep2/l3/; mkdir $QUAERO_EXP_DIR/lep2/delphi/; mkdir $QUAERO_EXP_DIR/lep2/opal/; mkdir $QUAERO_EXP_DIR/lep2/hint/; mkdir $QUAERO_EXP_DIR/hera/h1/; mkdir $QUAERO_EXP_DIR/hera/zeus/; mkdir $QUAERO_EXP_DIR/hera/hint/; mkdir $QUAERO_EXP_DIR/lhc/atlas/; mkdir $QUAERO_EXP_DIR/lhc/cms/; mkdir $QUAERO_EXP_DIR/lhc/hint/;") >& /dev/null

#  Upload systematic sources and correlations if "sys"

    if ( $experiment == "sys" ) then
      cd $RELEASE_DIR/Quaero/experiments/sys/; scp -rp systematic*.txt ${QUAERO_CC_USER}@${QUAERO_CC_COMPILE_HOST}:$QUAERO_EXP_DIR/sys/; cd -;
    else

#  Copy important files for each experiment, including assignSystematicUncertainties, normalizeProperly, simulate, and bkgListing.dat

	ssh -q ${QUAERO_CC_USER}@$QUAERO_CC_COMPILE_HOST "$QUAERO_CC_SETUP; mkdir $QUAERO_EXP_DIR/$experiment/simulate/ >& /dev/null; cp $QUAERO_CC_RELEASE_BIN_DIR/turboSim $QUAERO_EXP_DIR/$experiment/simulate/turboSim; cp -p $QUAERO_CC_RELEASE_BIN_DIR/assignSystematicUncertainties_${experiment:t} $QUAERO_EXP_DIR/$experiment/simulate/assignSystematicUncertainties; cp $QUAERO_CC_RELEASE_BIN_DIR/normalizeProperly_${experiment:t} $QUAERO_EXP_DIR/$experiment/simulate/normalizeProperly; mkdir $QUAERO_EXP_DIR/$experiment/data >& /dev/null; "
# in the above, mkdir $QUAERO_EXP_DIR/$experiment/refine/ >& /dev/null; cp $QUAERO_CC_RELEASE_BIN_DIR/refine_${experiment:t} $QUAERO_EXP_DIR/$experiment/refine/refine; 
	cd $RELEASE_DIR/Quaero/experiments/$experiment/simulate/; scp -p simulate* ${QUAERO_CC_USER}@${QUAERO_CC_COMPILE_HOST}:$QUAERO_EXP_DIR/$experiment/simulate/; cd -;
	cd $RELEASE_DIR/Quaero/experiments/$experiment/bkg/; scp -p bkgListing.dat ${QUAERO_CC_USER}@${QUAERO_CC_COMPILE_HOST}:$QUAERO_EXP_DIR/$experiment/bkg/bkgListing.dat; cd -;

#  Copy the actual data and background events only if "full" is specified, since this takes a few minutes.
#  Do the copy in parallel to each of the QUAERO_CC_PLATFORMS

    if ( $4 == "full" ) then
        ssh -q ${QUAERO_CC_USER}@${QUAERO_CC_COMPILE_HOST} "$QUAERO_CC_SETUP; cd $QUAERO_EXP_DIR/$experiment/; rm data/data_*.txt bkg/bkg*.txt;"
	cd $RELEASE_DIR/Quaero/experiments/$experiment/; (tar -zcf - bkg/bkg_* data/data_* | ssh -q ${QUAERO_CC_USER}@${QUAERO_CC_COMPILE_HOST} "cd $QUAERO_EXP_DIR/$experiment/; tar -zxf -")& ;  cd -;
	cd $RELEASE_DIR/Quaero/experiments/$experiment/simulate/; (tar -zcf - simulate* turboSimMorphisms.txt* | ssh -q ${QUAERO_CC_USER}@${QUAERO_CC_COMPILE_HOST} "cd $QUAERO_EXP_DIR/$experiment/simulate/;  tar -zxf -")& ; cd -;

#  The special file hint_specs.txt must be copied for "hint"

	if ( $experiment:t == "hint" ) then
	    cd $RELEASE_DIR/Quaero/experiments/$experiment/; scp -p hint_specs.txt ${QUAERO_CC_USER}@${QUAERO_CC_COMPILE_HOST}:$QUAERO_EXP_DIR/$experiment/; cd -
	endif
    endif
    endif
  end
  setenv QUAERO_CC_PLATFORM mitcdf
  source $RELEASE_DIR/Quaero/utilities/environmentVariables.tcsh  
  date
  exit
endif

##################################################
#  Install Web
##################################################


if ($2 == "web") then

ssh $QUAERO_HTML_HOST "mkdir $QUAERO_HTML_DIR/ >& /dev/null; \
                       mkdir $QUAERO_HTML_DIR/$QUAERO_EDITION/ >& /dev/null; \
                       mkdir $QUAERO_HTML_DIR/$QUAERO_EDITION/requests/ >& /dev/null; \
		       chmod og+w $QUAERO_HTML_DIR/$QUAERO_EDITION/requests/; "
scp -p Quaero/web/style.css ${QUAERO_HTML_HOST}:$QUAERO_HTML_DIR/$QUAERO_EDITION/


if ( 0 == 1 ) then
# Update manual and create quaero.html file
cd Quaero/doc/manual/
ln -sf manual0b.tex manual.tex
setenv TEXINPUTS ::.:~knuteson/knuteson/tools/tex/packages/revtex4/
latex manual
latex manual
bibtex manual
latex manual
bibtex manual
latex manual
mkdir manual >& /dev/null
rm html.log
unsetup perl
latex2html -local_icons -address '' manual > html.log
setup perl
rm manual.aux manual.dvi manual.log
cd -;
Quaero/utilities/updateHelpLinks.py
endif
cp Quaero/web/quaero_meta.html Quaero/web/quaero.html
setenv STUFF `echo $QUAERO_PY_URL/$QUAERO_EDITION.py/ | sed 's/\//\\\//g'`
sed -e 's/QUAERO_CGI_SCRIPT/'$STUFF'/' Quaero/web/quaero.html > Quaero/web/tmp1.html
mv Quaero/web/tmp1.html Quaero/web/quaero.html
scp -p Quaero/web/quaero.html ${QUAERO_HTML_HOST}:$QUAERO_HTML_DIR/$QUAERO_EDITION/
scp -p Quaero/web/details.html ${QUAERO_HTML_HOST}:$QUAERO_HTML_DIR/$QUAERO_EDITION/
ssh $QUAERO_HTML_HOST "cd $QUAERO_HTML_DIR/$QUAERO_EDITION/; ln -sf quaero.html index.html"

endif

##################################################
#  Install Documentation
##################################################

if ( $2 == "doc" ) then
#  foreach f ( examples madevent manual notes pythia suspect ) 
  foreach f ( examples suspect notes ) 
    ssh $QUAERO_HTML_HOST "rm -r $QUAERO_HTML_DIR/$QUAERO_EDITION/doc/$f/ >& /dev/null"
    scp -pr Quaero/doc/$f ${QUAERO_HTML_HOST}:$QUAERO_HTML_DIR/$QUAERO_EDITION/doc/ >& /dev/null
  end
  scp -p  Quaero/doc/devel/*.jpg \
          Quaero/doc/devel/*.gif \
          Quaero/doc/devel/index.html \
          ${QUAERO_HTML_HOST}:$QUAERO_HTML_DIR/$QUAERO_EDITION/doc/devel/ >& /dev/null

endif

##################################################
#  Install Python CGI script
##################################################

if ($2 == "web") then

#  Make a few modifications to the default quaero*.py file, and
#  put the python file in the appropriate cgi-bin directory

setenv FANCY_TYPE `echo $QUAERO_HTML_DIR/ | sed -e 's/\//\\\//g' `
sed -e 's/QUAERO_HTML_DIR/'$FANCY_TYPE'/' Quaero/web/quaero.py > Quaero/web/tmp1.py

setenv FANCY_TYPE `echo $QUAERO_PY_DIR/ | sed -e 's/\//\\\//g'`
sed -e 's/QUAERO_PY_DIR/'$FANCY_TYPE'/' Quaero/web/tmp1.py > Quaero/web/tmp2.py

setenv FANCY_TYPE `echo $QUAERO_CRON_DIR/ | sed -e 's/\//\\\//g' `
sed -e 's/QUAERO_CRON_DIR/'$FANCY_TYPE'/' Quaero/web/tmp2.py > Quaero/web/tmp3.py

sed -e 's/QUAERO_HTML_HOST/'$QUAERO_HTML_HOST'/' Quaero/web/tmp3.py > Quaero/web/tmp4.py
sed -e 's/QUAERO_CRON_HOST/'$QUAERO_CRON_HOST'/' Quaero/web/tmp4.py > Quaero/web/tmp5.py
sed -e 's/QUAERO_EDITION/'$QUAERO_EDITION'/' Quaero/web/tmp5.py > Quaero/web/tmp6.py
setenv FANCY_TYPE `echo $QUAERO_PY_PYTHON | sed -e 's/\//\\\//g'`
sed -e 's/QUAERO_PY_PYTHON/'$FANCY_TYPE'/' Quaero/web/tmp6.py > Quaero/web/tmp7.py
chmod uog+x Quaero/web/tmp7.py

scp -p Quaero/web/tmp7.py ${QUAERO_PY_HOST}:$QUAERO_CGI_DIR/$QUAERO_EDITION.py
rm Quaero/web/tmp?.py

#  Set up the requests directory
#  This is the directory that holds all submitted requests

ssh $QUAERO_PY_HOST "mkdir $QUAERO_PY_DIR/$QUAERO_EDITION/ >& /dev/null"
ssh $QUAERO_PY_HOST "mkdir $QUAERO_PY_DIR/$QUAERO_EDITION/requests/ >& /dev/null"
if(`ssh $QUAERO_PY_HOST "ls $QUAERO_PY_DIR/$QUAERO_EDITION/requests/currentID.txt" | tail -1` == "") then
  ssh $QUAERO_PY_HOST "mkdir $QUAERO_PY_DIR/$QUAERO_EDITION/requests/0/; \
                       mkdir $QUAERO_PY_DIR/$QUAERO_EDITION/requests/0/generatorfiles/; \
                       mkdir $QUAERO_PY_DIR/$QUAERO_EDITION/requests/0/signalfiles/; \
                       mkdir $QUAERO_PY_DIR/$QUAERO_EDITION/requests/0/signalfiles/hepg/; \
                       mkdir $QUAERO_PY_DIR/$QUAERO_EDITION/requests/0/signalfiles/lep2/; \
                       mkdir $QUAERO_PY_DIR/$QUAERO_EDITION/requests/0/signalfiles/hera/; \
                       mkdir $QUAERO_PY_DIR/$QUAERO_EDITION/requests/0/signalfiles/tev1/; \
                       mkdir $QUAERO_PY_DIR/$QUAERO_EDITION/requests/0/signalfiles/tev2/; \
                       mkdir $QUAERO_PY_DIR/$QUAERO_EDITION/requests/0/signalfiles/lhc/;"
  ssh $QUAERO_PY_HOST "echo 1 > $QUAERO_PY_DIR/$QUAERO_EDITION/requests/currentID.txt; \
                       chmod og+w $QUAERO_PY_DIR/$QUAERO_EDITION/requests/; \
                       chmod og+w $QUAERO_PY_DIR/$QUAERO_EDITION/requests/currentID.txt; \
                       chmod og+w $QUAERO_PY_DIR/$QUAERO_EDITION/requests/0/; \
                       chmod og+w $QUAERO_PY_DIR/$QUAERO_EDITION/requests/0/*; \
                       chmod og+w $QUAERO_PY_DIR/$QUAERO_EDITION/requests/0/*/*;"
endif

endif

##################################################
#  Set up cron job
##################################################

#  The cron job picks up any requests that have been dropped off by the python cgi script,
#  and submits them to an appropriate batch queue

if ($2 == "web") then

ssh $QUAERO_CRON_HOST "crontab -r"
ssh $QUAERO_CRON_HOST "ps -u $USER | grep -i 'cron_quaero' | awk '{print $1}' | xargs kill" |& grep -v 'kill '

ssh $QUAERO_CRON_HOST "mkdir $QUAERO_CRON_DIR/ >& /dev/null; mkdir $QUAERO_CRON_DIR/$QUAERO_EDITION/ >& /dev/null; mkdir $QUAERO_CRON_DIR/$QUAERO_EDITION/requests/ >& /dev/null"

setenv FANCY_TYPE `echo $QUAERO_CRON_DIR/$QUAERO_EDITION/ | sed -e 's/\//\\\//g' `
sed -e 's/QUAERO_CRON_DIR/'$FANCY_TYPE'/' Quaero/utilities/cron.tcsh > Quaero/utilities/tmp1.tcsh
chmod uog+x Quaero/utilities/tmp1.tcsh
scp -p Quaero/utilities/tmp1.tcsh ${QUAERO_CRON_HOST}:$QUAERO_CRON_DIR/$QUAERO_EDITION/cron_${QUAERO_EDITION}.tcsh
rm Quaero/utilities/tmp?.tcsh

scp -p $RELEASE_DIR/Quaero/utilities/environmentVariables.tcsh ${QUAERO_CRON_HOST}:$QUAERO_CRON_DIR/$QUAERO_EDITION/
ssh $QUAERO_CRON_HOST "echo 'setenv QUAERO_EDITION '$QUAERO_EDITION >> $QUAERO_CRON_DIR/$QUAERO_EDITION/environmentVariables.tcsh;"
if(`ssh $QUAERO_CRON_HOST "ls $QUAERO_CRON_DIR/$QUAERO_EDITION/requests/currentID.txt"` == "") then
  ssh $QUAERO_CRON_HOST "echo 1 > $QUAERO_CRON_DIR/$QUAERO_EDITION/requests/currentID.txt"
endif

#  Set the crontab file

ssh $QUAERO_CRON_HOST "mkdir $QUAERO_CRON_DIR/$QUAERO_EDITION/requests/ >& /dev/null"
# * * * * * KRB5_CONFIG=$QUAERO_CRON_DIR/$QUAERO_EDITION/krb5.conf; /usr/krb5/bin/kcron $QUAERO_CRON_DIR/$QUAERO_EDITION/cron_${QUAERO_EDITION}.tcsh 
ssh $QUAERO_CRON_HOST "echo '* * * * * $QUAERO_CRON_COMMAND $QUAERO_CRON_DIR/$QUAERO_EDITION/cron_${QUAERO_EDITION}.tcsh' > $QUAERO_CRON_DIR/$QUAERO_EDITION/crontab.txt; cat $QUAERO_CRON_DIR/*/crontab.txt | sort -u > $QUAERO_CRON_DIR/crontab.txt; crontab $QUAERO_CRON_DIR/crontab.txt;"

endif

##################################################
#  Remove temporary files that tend to accumulate on the CC platforms
##################################################

if ($2 == "cc_clean") then
  foreach platform ( $QUAERO_CC_PLATFORMS )
    echo '\n' $platform
    setenv QUAERO_CC_PLATFORM $platform
    source Quaero/utilities/environmentVariables.tcsh
    (ssh -q ${QUAERO_CC_USER}@$QUAERO_CC_COMPILE_HOST "$QUAERO_CC_SETUP; cd $QUAERO_CC_RELEASE_DIR/Quaero/runDir/$QUAERO_EDITION/; rm -r tmp/[0-9]*/; ") >& /dev/null
    (ssh -q ${QUAERO_CC_USER}@$QUAERO_CC_COMPILE_HOST "$QUAERO_CC_SETUP; cd $QUAERO_CC_RELEASE_DIR/Quaero/runDir/$QUAERO_EDITION/; rm -r requests/[0-9]*/; ") >& /dev/null
    (ssh -q ${QUAERO_CC_USER}@$QUAERO_CC_COMPILE_HOST "$QUAERO_CC_SETUP; cd $QUAERO_CC_RELEASE_DIR/Quaero/runDir/$QUAERO_EDITION/; rm core*;  ") >& /dev/null
    (ssh -q ${QUAERO_CC_USER}@$QUAERO_CC_COMPILE_HOST "$QUAERO_CC_SETUP; cd $QUAERO_CC_RELEASE_DIR/Quaero/runDir/$QUAERO_EDITION/; rm ucntuple.[eo]*;  ") >& /dev/null
    (ssh -q ${QUAERO_CC_USER}@$QUAERO_CC_COMPILE_HOST "$QUAERO_CC_SETUP; cd $QUAERO_CC_RELEASE_DIR/Quaero/runDir/$QUAERO_EDITION/; rm tmp1_*.e;  ") >& /dev/null
    (ssh -q ${QUAERO_CC_USER}@$QUAERO_CC_COMPILE_HOST "$QUAERO_CC_SETUP; cd $QUAERO_CC_RELEASE_DIR/Quaero/runDir/$QUAERO_EDITION/; rm FBS*;  ") >& /dev/null
    (ssh -q ${QUAERO_CC_USER}@$QUAERO_CC_COMPILE_HOST "$QUAERO_CC_SETUP; cd ~; rm FBS_* tmp1_*;  ") >& /dev/null
    (ssh -q ${QUAERO_CC_USER}@$QUAERO_CC_COMPILE_HOST "$QUAERO_CC_SETUP; cd $QUAERO_CC_RELEASE_DIR/Quaero/runDir/$QUAERO_EDITION/; rm -r signalFileStorage/[0-9]*;  ") >& /dev/null
  end
endif

##################################################
#  Set up the C++ code on the batch queues
##################################################

if (($2 == "code") || ($3 == "code")) then

 if ($QUAERO_EDITION == "quaero_mit") then
#   quaero_mit is special on my local machine, with executables linked directly to my bin directory
    cd Quaero/; gmake all; cd -;
    ln -sf $RELEASE_DIR/bin/quaero $RELEASE_DIR/Quaero/runDir/$QUAERO_EDITION/quaero
    ln -sf $RELEASE_DIR/bin/cleanup $RELEASE_DIR/Quaero/runDir/$QUAERO_EDITION/cleanup
#    rm -r $RELEASE_DIR/Quaero/runDir/$QUAERO_EDITION/MadEvent/
#    ln -sf $RELEASE_DIR/MadEvent $RELEASE_DIR/Quaero/runDir/$QUAERO_EDITION/MadEvent
    ln -sf $RELEASE_DIR/MadEvent/Pyxtra/mlm $RELEASE_DIR/Quaero/runDir/$QUAERO_EDITION/mlm
  else
#   otherwise copy the executables to the appropriate directory
    cp $RELEASE_DIR/bin/quaero $RELEASE_DIR/Quaero/runDir/$QUAERO_EDITION/quaero
    cp $RELEASE_DIR/bin/cleanup $RELEASE_DIR/Quaero/runDir/$QUAERO_EDITION/cleanup
    rm -rf $RELEASE_DIR/Quaero/runDir/$QUAERO_EDITION/MadEvent/*
    cp -r $RELEASE_DIR/MadEvent/MadGraphII/ $RELEASE_DIR/MadEvent/Template/  $RELEASE_DIR/Quaero/runDir/$QUAERO_EDITION/MadEvent/
    cp $RELEASE_DIR/MadEvent/Pyxtra/mlm $RELEASE_DIR/Quaero/runDir/$QUAERO_EDITION/
  endif

  setenv GMAKE "gmake all;" 
  if( ($3 == "clean") || ($4 == "clean") ) then
    setenv GMAKE "gmake clean; gmake all;"
  endif
#  Loop over each platform in the list $QUAERO_CC_PLATFORMS
  foreach platform ( $QUAERO_CC_PLATFORMS )
    echo '\n\n\n\n\n\n\n\n\n\n' $platform
    setenv QUAERO_CC_PLATFORM $platform
    source Quaero/utilities/environmentVariables.tcsh
    (ssh -q ${QUAERO_CC_USER}@$QUAERO_CC_COMPILE_HOST "$QUAERO_CC_SETUP; mkdir $QUAERO_CC_RELEASE_DIR/Quaero/runDir/; mkdir $QUAERO_CC_RELEASE_DIR/Quaero/runDir/$QUAERO_EDITION/; cd $QUAERO_CC_RELEASE_DIR/Quaero/runDir/$QUAERO_EDITION/; mkdir requests/; mkdir tmp/; mkdir requests/justfinished/;") >& /dev/null

#   Perform a CVS update
    ssh -q ${QUAERO_CC_USER}@${QUAERO_CC_COMPILE_HOST} "$QUAERO_CC_SETUP; $QUAERO_CC_BEGINBUILD; cd $QUAERO_CC_RELEASE_DIR; setenv CVSROOT :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2; cvs update MathUtils; cvs update FewKDE; cvs update OptimalBinning; cvs update Quaero_utils; cvs update MadEvent; cvs update TurboSim; mv Quaero/experiments/ .; cvs update Quaero; rm -r Quaero/experiments/; mv experiments/ Quaero/;"
#   Execute a gmake.  Do this in the background, in parallel on each of the platforms, to save time.
    ssh -q -n ${QUAERO_CC_USER}@${QUAERO_CC_COMPILE_HOST} "$QUAERO_CC_SETUP; $QUAERO_CC_BEGINBUILD; cd $QUAERO_CC_RELEASE_DIR; source QuaeroUtils/utilities/setup.tcsh; \
			    cd MathUtils/; $GMAKE; cd -; \
			    cd FewKDE/; $GMAKE; cd -; \
			    cd OptimalBinning/; $GMAKE; cd -; \
			    cd QuaeroUtils/; $GMAKE; cd -; \
			    cd MadEvent/; $GMAKE; cd -; \
			    cd Quaero/; $GMAKE; cd -; \
			    cd TurboSim; $GMAKE; cd -; \
			    cp bin/quaero bin/cleanup QuaeroUtils/utilities/*.kumac $QUAERO_CC_LAUNCH_DIR/$QUAERO_EDITION/; \
			    rm -r $QUAERO_CC_LAUNCH_DIR/$QUAERO_EDITION/MadEvent/ >& /dev/null; \
			    mkdir $QUAERO_CC_LAUNCH_DIR/$QUAERO_EDITION/MadEvent/; \
			    cp -r MadEvent/MadGraphII/ MadEvent/Template/ $QUAERO_CC_LAUNCH_DIR/$QUAERO_EDITION/MadEvent/; \
			    cp -r MadEvent/Pyxtra/mlm $QUAERO_CC_LAUNCH_DIR/$QUAERO_EDITION/; \
			    cp QuaeroUtils/utilities/qsub_$QUAERO_CC_PLATFORM $QUAERO_CC_LAUNCH_DIR/$QUAERO_EDITION/qsub; \
			    echo '#\\!/bin/tcsh\nsetenv QUAERO_EDITION '$QUAERO_EDITION'\nsetenv QUAERO_CC_DIR '$QUAERO_CC_DIR'\nsetenv QUAERO_HTML_URL '$QUAERO_HTML_URL'\nsetenv RELEASE_DIR '$QUAERO_CC_RELEASE_DIR'/\nsetenv PATH '"'\${PATH}:\${RELEASE_DIR}'"'/bin/\n'$QUAERO_ENV_VARS ; \
			    if ($QUAERO_isCafQ != 1) echo '#\\!/bin/tcsh\nsetenv QUAERO_EDITION '$QUAERO_EDITION'\nsetenv QUAERO_CC_DIR '$QUAERO_CC_DIR'\nsetenv QUAERO_HTML_URL '$QUAERO_HTML_URL'\nsetenv RELEASE_DIR '$QUAERO_CC_RELEASE_DIR'/\nsetenv PATH '"'\$\{PATH\}:\$\{RELEASE_DIR\}'"'/bin/\n'$QUAERO_ENV_VARS > $QUAERO_CC_LAUNCH_DIR/$QUAERO_EDITION/environmentVariables.tcsh; \
			    $QUAERO_CC_ENDBUILD;" &
  end
endif
#			    if ($QUAERO_isCafQ == 1) echo '#\\!/bin/tcsh\nsetenv QUAERO_EDITION '$QUAERO_EDITION'\nsetenv QUAERO_CC_DIR \$RELEASE_DIR/Quaero/runDir/\nsetenv QUAERO_HTML_URL '$QUAERO_HTML_URL'\nsetenv PATH '"'\$\{PATH\}:\$\{RELEASE_DIR\}'"'/bin/\n'$QUAERO_ENV_VARS > $QUAERO_CC_LAUNCH_DIR/$QUAERO_EDITION/environmentVariables.tcsh; \

#  Restore things to the way they were, and exit

setenv QUAERO_CC_PLATFORM mitcdf
source $RELEASE_DIR/Quaero/utilities/environmentVariables.tcsh
date


