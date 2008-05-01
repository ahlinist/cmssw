#!/bin/tcsh
#############################################################################################
#  Quaero environment variables
#  This script sets up environment variables appropriate for Quaero.
#  These variables are specific to several individual (known) clusters.
#  Legal clusters include b0pcmit, lxplus, h1-wgs.desy.de, rtc.rice.edu, cdf.uchicago.edu, 
#                     and fcdflnx4.fnal.gov
#############################################################################################


# PLATFORMS
setenv QUAERO_HTML_PLATFORM mitcdf
setenv QUAERO_PY_PLATFORM fcdfwww
setenv QUAERO_CRON_PLATFORM mitcdf

setenv QUAERO_CC_PLATFORMS "atom"
if( `printenv allQuaeroCcPlatforms` == "1" ) then
#  setenv QUAERO_CC_PLATFORMS "rtc fnpc1 desy lxplus clued0 atom cmsuaf fcdflnx"
#  setenv QUAERO_CC_PLATFORMS "rtc fnpc1 desy lxplus clued0 atom"
endif
#uchicago -- banned by Frish
#clued0 -- I get a /usr/bin/ld: final link failed:  No such file or directory in ./bin/survey
#cmsuaf -- need to make work
#fcdflnx -- need to make work
#rtc -- seems like a problem with the queues


setenv QUAERO_ENV_VARS ""
setenv QUAERO_CC_SETUP ""
setenv QUAERO_CC_ENDBUILD ""
setenv QUAERO_CC_BEGINBUILD ""
setenv QUAERO_CC_LAUNCH_DIR ""
setenv QUAERO_CC_CLEANUP ""
setenv QUAERO_CC_COMPILE_HOST ""
setenv QUAERO_isCafQ 0

# HTML
switch ($QUAERO_HTML_PLATFORM)
    case mitcdf:
	setenv QUAERO_HTML_URL "http://mit.fnal.gov/~`whoami`/Quaero/"
	setenv QUAERO_HTML_HOST b0pcmit35.fnal.gov
	setenv QUAERO_HTML_DIR /home/`whoami`/Public/Quaero/
    breaksw
    case uchicago:
	setenv QUAERO_HTML_URL "http://hep.uchicago.edu/cdf/~`whoami`/Quaero/"
	setenv QUAERO_HTML_HOST cdf13.uchicago.edu
	setenv QUAERO_HTML_DIR /web/cdf/`whoami`/Quaero/
    breaksw
    case laptop:
	setenv QUAERO_HTML_URL "http://localhost/Quaero/"
	setenv QUAERO_HTML_HOST localhost
	setenv QUAERO_HTML_DIR /usr/local/apache2/htdocs/Quaero/
    breaksw
    default:
	echo 'WARNING!  Unknown QUAERO_HTML_PLATFORM'
    breaksw
endsw

# PY
switch ($QUAERO_PY_PLATFORM)
# cdfsga is old; these environment variables are appropriate before the switch to the new www-cdf.fnal.gov server
    case cdfsga:
	setenv QUAERO_PY_URL "http://www-cdf.fnal.gov/cgi-bin/"
	setenv QUAERO_PY_HOST cdfsga.fnal.gov
	setenv QUAERO_CGI_DIR /usr/people/cdfwww/bin/
	setenv QUAERO_PY_DIR /home/xlv6/`whoami`/Quaero/
	setenv QUAERO_PY_PYTHON /usr/local/bin/python
    breaksw
# cdflnx3 is also old; these environment variables are appropriate before security issues forced Glenn Cooper to change
    case fcdflnx3:
	setenv QUAERO_PY_URL "http://fcdfwww.fnal.gov/cgi-bin/"
	setenv QUAERO_PY_HOST fcdflnx3.fnal.gov
	setenv QUAERO_CGI_DIR /cdf/cgi-bin/
	setenv QUAERO_PY_DIR /cdf/spool/`whoami`/Quaero/
	setenv QUAERO_PY_PYTHON /usr/bin/python
    breaksw
# This is the current set of environment variables (as of 2004 Oct 1)
# Change /cdf/spool/knuteson/ to /cdf/scratch/knuteson (on 2006 Oct 6)
    case fcdfwww:
	setenv QUAERO_PY_URL "http://fcdfwww.fnal.gov/cgi-bin/"
	setenv QUAERO_PY_HOST fcdfwww.fnal.gov
	setenv QUAERO_CGI_DIR /usr/local/apache2/cgi-bin/
	setenv QUAERO_PY_DIR /cdf/scratch/`whoami`/Quaero/
	setenv QUAERO_PY_PYTHON /usr/bin/python
    breaksw
    case uchicago:
	setenv QUAERO_PY_URL "http://hep.uchicago.edu/cgi-bin/"
	setenv QUAERO_PY_HOST hep.uchicago.edu
	setenv QUAERO_CGI_DIR /usr/local/apache/cgi-bin/
	setenv QUAERO_PY_DIR /web/cdf/`whoami`/Quaero/
	setenv QUAERO_PY_PYTHON /usr/bin/python
    breaksw
    case laptop:
	setenv QUAERO_PY_URL "http://localhost/cgi-bin/"
	setenv QUAERO_PY_HOST localhost
	setenv QUAERO_CGI_DIR /usr/local/apache2/cgi-bin/
	setenv QUAERO_PY_DIR /usr/local/apache2/htdocs/Quaero/tmp/
	setenv QUAERO_PY_PYTHON /usr/bin/python
    breaksw
    default:
	echo 'WARNING!  Unknown QUAERO_PY_PLATFORM'
    breaksw
endsw

# CRON
switch ($QUAERO_CRON_PLATFORM)
    case fcdflnx2:
	setenv QUAERO_CRON_HOST fcdflnx2.fnal.gov
	setenv QUAERO_CRON_DIR /cdf/home/`whoami`/Quaero/cron/
	setenv QUAERO_CRON_COMMAND /usr/krb5/bin/kcron
    breaksw
    case clued0:
	setenv QUAERO_CRON_HOST thedude-clued0.fnal.gov
	setenv QUAERO_CRON_DIR /work/thedude-clued0/Quaero/Quaero/cron/
	setenv QUAERO_CRON_COMMAND /usr/krb5/bin/kcron
    breaksw
    case mitcdf:
	setenv QUAERO_CRON_HOST b0pcmit18.fnal.gov
	setenv QUAERO_CRON_DIR /home/`whoami`/cvs/current/Quaero/cron/
	setenv QUAERO_CRON_COMMAND /usr/krb5/bin/kcron
    breaksw
    case uchicago:
	setenv QUAERO_CRON_HOST cdf13.uchicago.edu
	setenv QUAERO_CRON_DIR /home/cdf/`whoami`/cvs/current/Quaero/cron/
	setenv QUAERO_CRON_COMMAND /usr/krb5/bin/kcron
    breaksw
    case laptop:
	setenv QUAERO_CRON_HOST localhost
	setenv QUAERO_CRON_DIR /home/`whoami`/`whoami`/cdf/cvs/4.10.4/Quaero/cron/
	setenv QUAERO_CRON_COMMAND ""
    breaksw
    default:
	echo 'WARNING!  Unknown QUAERO_CRON_PLATFORM'
    breaksw
endsw


## CC

if ( .`printenv QUAERO_CC_PLATFORM` != "." ) then
  switch ($QUAERO_CC_PLATFORM)
    case uchicago:
	setenv QUAERO_CC_HOST cdf13.uchicago.edu
	setenv QUAERO_CC_USER knuteson
	setenv QUAERO_CC_RELEASE_DIR /home/cdf/`whoami`/`whoami`/cdf/cvs/current/
	setenv QUAERO_CC_RELEASE_BIN_DIR $QUAERO_CC_RELEASE_DIR/bin/
        setenv QUAERO_ENV_VARS "setup gcc v3_0_2 \; setenv F77 g77 \; setenv FF g77 \;"
    breaksw
    case lxplus:
	setenv QUAERO_CC_HOST lxplus.cern.ch
	setenv QUAERO_CC_USER brucek
	setenv QUAERO_CC_RELEASE_DIR /afs/cern.ch/phys/lep/quaero/www/cvs/
	setenv QUAERO_CC_RELEASE_BIN_DIR $QUAERO_CC_RELEASE_DIR/bin/
        setenv QUAERO_CC_SETUP "source /afs/cern.ch/phys/lep/quaero/www/private/.yo/setUpStuff.tcsh "
	setenv QUAERO_ENV_VARS "setenv LD_LIBRARY_PATH /afs/cern.ch/sw/lhcxx/specific/redhat61/CLHEP/1.8.0.0/lib/:/afs/cern.ch/sw/lcg/external/GSL/1.4/rh73_gcc32/lib/\;"
    breaksw
    case desy:
	setenv QUAERO_CC_HOST h1wgs.desy.de
	setenv QUAERO_CC_USER knuteson
	setenv QUAERO_CC_RELEASE_DIR /user/knuteson/cvs/
	setenv QUAERO_CC_RELEASE_BIN_DIR $QUAERO_CC_RELEASE_DIR/bin/
	setenv QUAERO_ENV_VARS "setenv LD_LIBRARY_PATH /afs/cern.ch/sw/lhcxx/specific/redhat61/CLHEP/1.8.0.0/lib/:/afs/cern.ch/sw/lcg/external/GSL/1.4/rh73_gcc32/lib/:/afs/desy.de/products/gcc-3.2/spec/i386_linux24/lib/\;"
	setenv QUAERO_CC_SETUP "source /user/knuteson/private/.yo/setUpStuff.tcsh "
    breaksw
    case fnpc1:
	setenv QUAERO_CC_COMPILE_HOST fnpc1.fnal.gov
	setenv QUAERO_CC_HOST fnpc123.fnal.gov
	setenv QUAERO_CC_USER knuteson
	setenv QUAERO_CC_RELEASE_DIR /home/knuteson/cvs/
	setenv QUAERO_CC_RELEASE_BIN_DIR /local/stage1/knuteson/cvs/bin/
        setenv QUAERO_ENV_VARS "setup cern 2003 \; setup gcc v3_3_1 \; setup clhep v1_9_1_1 -q GCC3_3_1 \; setenv F77 g77 \; setenv FF g77 \; setenv FLIBS \'-L../../lib/ -L../lib/ -lfixes2 -lg2c -lfixes2 \' \;"
	setenv QUAERO_CC_BEGINBUILD "setup cvs; ln -nsf /local/stage1/knuteson/cvs /home/knuteson/cvs; "
	setenv QUAERO_CC_ENDBUILD "ln -nsf /home/knuteson/cvs2 /home/knuteson/cvs; "
	setenv QUAERO_CC_LAUNCH_DIR /farm/cepa_stage01/knuteson/cvs2/Quaero/runDir/
    breaksw
    case cmsuaf:
	setenv QUAERO_CC_COMPILE_HOST cmsuaf.fnal.gov
	setenv QUAERO_CC_HOST cmsuaf.fnal.gov
	setenv QUAERO_CC_USER knuteson
	setenv QUAERO_CC_RELEASE_DIR /afs/fnal.gov/files/home/room1/knuteson/cvs/
	setenv QUAERO_CC_RELEASE_BIN_DIR $QUAERO_CC_RELEASE_DIR/bin/
# eval `scram setroot -csh ORCA ORCA_8_5_0` ; eval `scram runtime -csh` ;
        setenv QUAERO_ENV_VARS "source /afs/fnal.gov/files/code/cms/setup/cshrc fnal \; setenv LOCALRT /afs/fnal.gov/exp/cms/l/Releases/ORCA/ORCA_8_5_0 \; setenv LD_LIBRARY_PATH /afs/fnal.gov/exp/cms/l/lcg/external/gsl/1.4-1/rh73_gcc32/lib/ \; setup cern 2002 \; setenv F77 g77 \; setenv FF g77 \; "
    breaksw
    case rtc:
	setenv QUAERO_CC_HOST rtc.rice.edu
	setenv QUAERO_CC_USER knuteson
	setenv QUAERO_CC_RELEASE_DIR /users/`whoami`/test/
	setenv QUAERO_CC_RELEASE_BIN_DIR $QUAERO_CC_RELEASE_DIR/bin/
        setenv QUAERO_ENV_VARS "setenv PATH "'\$\{PATH\}'":/usr/local/CERN/2003/bin/ \; setenv INTEL_PATH "'\$PATH'" \; setenv MPICH_PATH "'\$PATH'" \; setenv F77 f77 \; setenv FF f77 \; "
    breaksw
    case clued0:
	setenv QUAERO_CC_HOST brunette-clued0.fnal.gov
#  my ability to ssh into thedude-clued0 has stopped on 8 May 2005
#  http://quaero.fnal.gov is still up and running, however, so
#  my workaround is just to ssh into a different machine.
#	setenv QUAERO_CC_HOST thedude-clued0.fnal.gov
	setenv QUAERO_CC_USER knuteson
	setenv QUAERO_CC_RELEASE_DIR /work/thedude-clued0/Quaero/
	setenv QUAERO_CC_RELEASE_BIN_DIR $QUAERO_CC_RELEASE_DIR/bin/
        setenv QUAERO_CC_SETUP "source /work/thedude-clued0/Quaero/.private/.setUpStuff.tcsh "
        setenv QUAERO_ENV_VARS "setup gcc v3_2_1 \; setenv F77 g77 \; setenv FF g77 \; setenv FLIBS \'-lg2c -L../../lib/ -L../lib/ -lc_nonshared -lfixes \' \;"
    breaksw
    case mitcdf:
	setenv QUAERO_CC_HOST b0pcmit35.fnal.gov
	setenv QUAERO_CC_USER knuteson
	setenv QUAERO_CC_RELEASE_DIR /home/`whoami`/cvs/current/
	setenv QUAERO_CC_RELEASE_BIN_DIR $QUAERO_CC_RELEASE_DIR/bin/
    breaksw
    case fcdflnx:
	setenv QUAERO_CC_HOST fcdflnx4.fnal.gov
	setenv QUAERO_CC_USER knuteson
	setenv QUAERO_CC_RELEASE_DIR /cdf/home/`whoami`/cvs/current/
	setenv QUAERO_CC_RELEASE_BIN_DIR $QUAERO_CC_RELEASE_DIR/bin/
	setenv QUAERO_CC_LAUNCH_DIR ./Quaero/runDir/
	setenv QUAERO_isCafQ 1
    breaksw
    case atom:
	setenv QUAERO_CC_HOST iodine.fnal.gov
	setenv QUAERO_CC_USER knuteson
	setenv QUAERO_CC_RELEASE_DIR /cdf/atom/home/`whoami`/cvs/current/
	setenv QUAERO_CC_RELEASE_BIN_DIR $QUAERO_CC_RELEASE_DIR/bin/
	setenv QUAERO_ENV_VARS "setup cern 2000 \; setenv F77 g77 \; setenv FF g77 \;"
    breaksw
    case laptop:
	setenv QUAERO_CC_HOST localhost
	setenv QUAERO_CC_USER knuteson
	setenv QUAERO_CC_RELEASE_DIR /home/`whoami`/`whoami`/cdf/cvs/4.10.4/
	setenv QUAERO_CC_RELEASE_BIN_DIR $QUAERO_CC_RELEASE_DIR/bin/
    breaksw
    default:
	echo 'Unknown QUAERO_CC_PLATFORM'
    breaksw
  endsw
  setenv QUAERO_CC_DIR $QUAERO_CC_RELEASE_DIR/Quaero/runDir/
  if ( $QUAERO_CC_LAUNCH_DIR == "" ) setenv QUAERO_CC_LAUNCH_DIR $QUAERO_CC_DIR
  if ( $QUAERO_CC_COMPILE_HOST == "" ) setenv QUAERO_CC_COMPILE_HOST $QUAERO_CC_HOST
endif
