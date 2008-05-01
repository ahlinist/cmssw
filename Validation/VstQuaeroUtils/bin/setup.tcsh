#!/bin/tcsh

# This script builds Quaero_utils and sets appropriate environment variables
# Usage:  Quaero_utils/utilities/setup.tcsh
#
# Bruce Knuteson 2004
#

# To be able to use >> rediraction to 
# create some file that doesn't already exist.
unset noclobber

setenv RELEASE_DIR `pwd`
setenv CC 'g++ '
setenv F77 'f77'
setenv AR 'ar'
setenv FFLAGS ''
setenv FLIBS ''
unsetenv MAKEFLAGS
unsetenv MAKEFILES
setenv haveWeBeenHereBefore `echo $PATH | grep -i $RELEASE_DIR/bin | wc | awk '{print $1}'`
if ( $haveWeBeenHereBefore == "0" ) then
  setenv PATH ${PATH}:${RELEASE_DIR}/bin
endif
setenv QUAERO_CC_PLATFORM `source Quaero_utils/utilities/getQuaeroClusterName.tcsh`
setenv OBJS2 ""

if ($QUAERO_CC_PLATFORM == "DEAS") then
  setenv LDFLAGS '-static -L/home/'$USER'/lib'
  setenv CVSROOT :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2
  setenv CRNLIB /Users/olympics/blackbox/libraries/cernlib/lib/
  setenv CLHEP_BASE_DIR /home/'$USER'
  setenv CLHEP_DIR $CLHEP_BASE_DIR
  setenv GSL_DIR $RELEASE_DIR/include/
  setenv FLAGS '-g -O3 -D_FILE_OFFSET_BITS=64 -I/home/'$USER'/include -I'$GSL_DIR'/include/ -I'$RELEASE_DIR'/include/  -Wno-deprecated -Wno-sign-compare -D__STANDARD_CPLUSPLUS -DDEFECT_NO_EXCEPTIONS -D ENABLE_BACKWARDS_COMPATIBILITY -D QUAERO_CC_PLATFORM_'$QUAERO_CC_PLATFORM

  setenv LIBS '-L'$RELEASE_DIR'/lib/ -L'$CRNLIB' -L/home/'$USER'/lib -L/deas/software/lib -L/deas/vendor/lib -lCLHEP -lstdhep -lFmcfio -lpdflib -lmathlib -lkernlib -lpacklib -lg2c -lgsl -lgslcblas -lm'
#-lZMtools -lZMutility -lZMtimer -lExceptions
  setenv CFLAGS '-I/home/'$USER' -I/usr/include/ -I/deas/vendor/include -I/deas/software/include '
  setenv CPPFLAGS "$FLAGS"
endif

if ($QUAERO_CC_PLATFORM == "harv-heplcl") then
  source ~cdfsoft/cdf2.cshrc
  setup cdfsoft2 6.1.4
  setenv CAF_CURRENT harcaf
  unsetenv MAKEFILES
  unsetenv MAKEFLAGS
  setup gsl v1_2
  setup cern 2000
  setup stdhep v4_09
  setup CLHEP V01-09-10
  setenv CLHEP_BASE_DIR "/w/cdf/cdfsoft/dist/packages/CLHEP/V01-09-10/CLHEP"
  setenv OBJS2 "missingCLHEProutines.o"
#-I'$CLHEP_BASE_DIR'/Vector/
  setenv FLAGS '-g -O2 -D_FILE_OFFSET_BITS=64 -I'$GSL_DIR'/include/  -I/home/'$USER'/include/ -I'$RELEASE_DIR'/include/ -I'$CDFSOFT2_DIR'/include/ -D QUAERO_CC_PLATFORM_'$QUAERO_CC_PLATFORM' -D ENABLE_BACKWARDS_COMPATIBILITY' 
  setenv LIBS '-L'$CRNLIB' -L'$CLHEP_BASE_DIR'/lib -L'$GSL_DIR'/.libs/ -L'$GSL_DIR'/lib/ -L'$RELEASE_DIR'/lib/ -L'$CDFSOFT2_DIR'/lib/Linux2-GCC_3_4/ -L'$STDHEP_DIR'/lib/ -lCLHEP -lstdhep -lFmcfio -lZMtools -lZMutility -lZMtimer -lExceptions -lpdflib -lmathlib -lkernlib -lpacklib -lnsl -lg2c -lgsl -lm'
#  setenv OBJS2 "idate_.o itime_.o"
  setenv LDFLAGS '-static -L/home/'$USER'/lib'
  setenv CPPFLAGS "$FLAGS"
  setenv CVSROOT :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2
endif

if ($QUAERO_CC_PLATFORM == "harvard") then
  setenv CVSROOT :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2
  Setenv CRNLIB /Users/olympics/blackbox/libraries/cernlib/lib/
  setenv CLHEP_DIR /cdfsoft/dist/releases/5.2.0/include/CLHEP/
  setenv FLAGS '-g -O3 -D_FILE_OFFSET_BITS=64 -I'$RELEASE_DIR'/include/ -I/sw/include/ -Wno-deprecated -Wno-sign-compare -D__STANDARD_CPLUSPLUS -DDEFECT_NO_EXCEPTIONS -D QUAERO_CC_PLATFORM_'$QUAERO_CC_PLATFORM' -bind_at_load ' 
  setenv LIBS '-L'$RELEASE_DIR'/lib/ -L'$CRNLIB' -L/sw/lib/ -lCLHEP -lstdhep -lFmcfio -lcernlib -lpdflib -lmathlib -lkernlib -lpacklib -lg2c -lgsl -lm'
#-lZMtools -lZMutility -lZMtimer -lExceptions
  setenv CFLAGS '-I/sw/include/ '
endif

if ( ($QUAERO_CC_PLATFORM == "laptop") && ( `whoami` == knuteson ) ) then
  setenv CVSROOT :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2
  setenv FLAGS '-g -O3 -D_FILE_OFFSET_BITS=64 -I'$RELEASE_DIR'/include/ -Wno-deprecated -Wno-sign-compare -D__STANDARD_CPLUSPLUS -DDEFECT_NO_EXCEPTIONS -D QUAERO_CC_PLATFORM_'$QUAERO_CC_PLATFORM 
#-I'$GSL_DIR'/include/ -I'$CDFSOFT2_DIR'/include/ 
#-L'$CRNLIB' -L'$GSL_DIR'/.libs/ -L'$GSL_DIR'/lib/ -L'$CDFSOFT2_DIR'/lib//Linux2-GCC_3_4/ -L'$STDHEP_DIR'/lib/ -lCLHEP -lstdhep -lFmcfio -lZMtools -lZMutility -lZMtimer -lExceptions -lpdflib -lmathlib -lkernlib -lpacklib -lnsl 
  alias f77 g77
  setenv FF $F77
  setenv LIBS '-L'$RELEASE_DIR'/lib/ -lg2c -lgsl -lm'
endif

if ($QUAERO_CC_PLATFORM == "MrennaDesktop") then
  source /fnal/ups/etc/setups.csh
  setup stdhep v4_09
  setup gcc v3_4_3 -f Linux+2.4-2.3.2
  setup clhep v1_9_2_1 -q GCC3_4_3
  rm $RELEASE_DIR/include/CLHEP
  ln -sf $CLHEP_BASE_DIR/include/CLHEP $RELEASE_DIR/include/CLHEP
  setup gsl v1_2
  setup cern 2002
  echo "cernlib is " $CRNLIB
  setenv FLAGS '-g -O3 -D_FILE_OFFSET_BITS=64 -I'$CLHEP_DIR'/include/CLHEP -I'$RELEASE_DIR'/include/ -I'$GSL_DIR'/include/ -Wno-deprecated -Wno-sign-compare -D__STANDARD_CPLUSPLUS -DDEFECT_NO_EXCEPTIONS -D QUAERO_CC_PLATFORM_'$QUAERO_CC_PLATFORM
  setenv LIBS '-L'$CRNLIB' -lpdflib804 -lmathlib -lpacklib -lkernlib -lpdflib804 -L'$STDHEP_DIR'/lib/ -lstdhep -lFmcfio -L'$CLHEP_DIR'/lib/ -lCLHEP -L'$RELEASE_DIR'/lib/ -lctype -L'$GSL_DIR'/lib -lgsl -lg2c  -lm -lnsl'
  setenv CVSROOT :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2
endif

if ( ($QUAERO_CC_PLATFORM == "laptop") && ( `whoami` == mrenna ) ) then
  source /local/ups/etc/setups.csh
  setup gcc 
  setup clhep
  rm $RELEASE_DIR/include/CLHEP
  ln -sf $CLHEP_BASE_DIR/include/CLHEP $RELEASE_DIR/include/CLHEP
  setup gsl 
  setup stdhep 
  setup cern 
  echo "cernlib is " $CRNLIB

  cd $RELEASE_DIR/; cd Quaero_utils/src/; g++ -c ctype_b.c;  ar rcv $RELEASE_DIR/lib/libctype.a ctype_b.o; cd $RELEASE_DIR


  setenv FLAGS '-g -O3 -D_FILE_OFFSET_BITS=64 -I'$CLHEP_DIR'/include/CLHEP -I'$RELEASE_DIR'/include/ -I'$GSL_DIR'/include/ -Wno-deprecated -Wno-sign-compare -D__STANDARD_CPLUSPLUS -DDEFECT_NO_EXCEPTIONS -D QUAERO_CC_PLATFORM_'$QUAERO_CC_PLATFORM
  setenv LIBS '-L'$CRNLIB' -lpdflib804 -lmathlib -lpacklib -lkernlib -lpdflib804 -L'$STDHEP_DIR'/lib/ -lstdhep -lFmcfio -L'$CLHEP_DIR'/lib/ -lCLHEP -L'$RELEASE_DIR'/lib/ -lctype -L'$GSL_DIR'/lib -lgsl -lg2c  -lm -lnsl'
  setenv CVSROOT :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2
endif


if ($QUAERO_CC_PLATFORM == "clued0") then
  setup D0RunII p18.09.00
  setup cern 2002
#  setup stdhep v5_01
  setup stdhep v4_09
  setup clhep v1_8
  setup gsl v1_2
  setup gcc v3_2_1
#  setup gcc v3_1
#  setup gcc v3_4_3_dzero
# cd $RELEASE_DIR/; cd Quaero_utils/src/; g++ -c ctype.c;  rm lib/libfixes.a; ar rcv lib/libfixes.a Quaero_utils/src/ctype.o Quaero_utils/src/init_array.o
#  cd $RELEASE_DIR/; cd Quaero_utils/src/; gcc -g -Wall -c ctype.c;  gcc -g -Wall -c init_array.c; cd $RELEASE_DIR/; rm lib/libfixes.a; ar rcv lib/libfixes.a Quaero_utils/src/ctype.o Quaero_utils/src/init_array.o
#  cd $RELEASE_DIR/lib; g++ -g -Wall -c $CLHEP_DIR/CLHEP/Vector/*.cc -I$CLHEP_DIR/; cd $RELEASE_DIR; ar rcv lib/libclhephack.a lib/*.o; rm lib/*.o
#  setenv LIBS '-L/D0/ups/cern/Linux-2-4/2002/lib/ -L'$CRNLIB' -L/D0/ups/stdhep/v4_10/lib/ -L'$STDHEP_DIR'/lib/ -L'$CLHEP_DIR'/lib/ -lCLHEP-g++.1.8.0.0  -lstdhep -lFmcfio -lpdflib -lmathlib -lkernlib -lpacklib -lnsl -lstdc++ -lg2c -lfixes -lg2c -L'$GSL_DIR'/lib/ -lgsl -lm'
#  setenv FLIBS '-L/D0/ups/cern/Linux-2-4/2002/lib/ -L'$STDHEP_DIR'/lib/ -L/D0/ups/clhep/Linux-2-4/v1_8/lib/ -lCLHEP-g++.1.8.0.0 -L'$RELEASE_DIR'/lib/ -lstdhep -lFmcfio -lpdflib804 -lmathlib -lkernlib -lpacklib -lnsl -lstdc++ -lg2c -lfixes '$RELEASE_DIR'/lib/init_array.o -lg2c -L'$GSL_DIR'/lib/ -lgsl -lm'
#  setenv LIBS '-L/D0/ups/cern/Linux-2-4/2002/lib/ -L'$STDHEP_DIR'/lib/ -L/D0/ups/clhep/Linux-2-4/v1_8/lib/ -lCLHEP-g++.1.8.0.0 -L'$RELEASE_DIR'/lib/ -lstdhep -lFmcfio -lpdflib804 -lmathlib -lkernlib -lpacklib -lnsl -lstdc++ -lg2c -lfixes '$RELEASE_DIR'/lib/init_array.o -lg2c -L'$GSL_DIR'/lib/ -lgsl -lm'
 setenv FLIBS '-L/D0/ups/cern/Linux-2-4/2002/lib/ -L'$STDHEP_DIR'/lib/ -L/D0/ups/clhep/Linux-2-4/v1_8/lib/ -lCLHEP-g++.1.8.0.0 -L'$RELEASE_DIR'/lib/ -lstdhep -lFmcfio -lpdflib804 -lmathlib -lkernlib -lpacklib -lnsl -lstdc++ -lg2c -lfixes -lclhephack -L'$GSL_DIR'/lib/ -lgsl -lm'
  setenv LIBS '-L/D0/ups/cern/Linux-2-4/2002/lib/ -L'$STDHEP_DIR'/lib/ -L/D0/ups/clhep/Linux-2-4/v1_8/lib/ -lCLHEP-g++.1.8.0.0 -L'$RELEASE_DIR'/lib/ -lstdhep -lFmcfio -lpdflib804 -lmathlib -lkernlib -lpacklib -lnsl -lstdc++ -lg2c -lfixes -lclhephack -L'$GSL_DIR'/lib/ -lgsl -lm'
  setenv FLAGS '-g -O2 -D_FILE_OFFSET_BITS=64 -I'$GSL_DIR'/include/ -I'$RELEASE_DIR'/include/ -I'$CLHEP_DIR'/include/ -Wno-deprecated -D QUAERO_CC_PLATFORM_clued0 '
# -static
  setenv CVS_RSH ssh
  setenv CVSROOT :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2
endif


if ($QUAERO_CC_PLATFORM == "mitcdf") then
  source ~cdfsoft/cdf2.cshrc
  setup cdfsoft2 6.1.4
  unsetenv MAKEFILES
  unsetenv MAKEFLAGS
  setup gsl v1_2
  setup cern 2000
  setup stdhep v4_09
#  setup CLHEP V01-07-15
#  setenv CLHEP_DIR /cdfsoft/dist/releases/4.10.4/include/CLHEP/
#  setenv GSL_DIR /disk/user28a/knuteson/Knuteson/tools/gsl/gsl-1.4/
#  setenv CDFSOFT2_DIR /cdfsoft/dist/releases/4.11.2/
#  setenv CDFSOFT2_DIR /cdfsoft/dist/releases/6.1.2/
#  setenv FLAGS '-g -O2 -D_FILE_OFFSET_BITS=64 -I'$GSL_DIR'/include/ -I'$RELEASE_DIR'/include/ -I'$CLHEP_DIR'/include/ -I'$CDFSOFT2_DIR'/include/ -D QUAERO_CC_PLATFORM_'$QUAERO_CC_PLATFORM 
#  setenv LIBS '-L'$CRNLIB' -L'$GSL_DIR'/.libs/ -L'$GSL_DIR'/lib/ -L'$RELEASE_DIR'/lib/ -L'$CDFSOFT2_DIR'/lib/Linux2-KCC_4_0/ -L'$STDHEP_DIR'/lib/ -L'$CLHEP_DIR'/lib/ -lCLHEP -lstdhep -lFmcfio -lpdflib -lmathlib -lkernlib -lpacklib -lnsl -lg2c -lgsl -lm'
  setenv FLAGS '-g -O3 -D_FILE_OFFSET_BITS=64 -I'$GSL_DIR'/include/ -I'$RELEASE_DIR'/include/ -I'$CDFSOFT2_DIR'/include/ -Wno-deprecated -Wno-sign-compare -D__STANDARD_CPLUSPLUS -DDEFECT_NO_EXCEPTIONS -D QUAERO_CC_PLATFORM_'$QUAERO_CC_PLATFORM 
#  setenv LIBS '-L'$CRNLIB' -L'$GSL_DIR'/.libs/ -L'$GSL_DIR'/lib/ -L'$RELEASE_DIR'/lib/ -L'$CDFSOFT2_DIR'/lib/Linux2-KCC_4_0/ -L'$STDHEP_DIR'/lib/ -lCLHEP -lstdhep -lFmcfio -lpdflib -lmathlib -lkernlib -lpacklib -lnsl -lg2c -lgsl -lm'
  setenv LIBS '-L'$CRNLIB' -L'$GSL_DIR'/lib/ -L'$RELEASE_DIR'/lib/ -L'$CDFSOFT2_DIR'/lib/Linux2-GCC_3_4-maxopt/ -L'$STDHEP_DIR'/lib/ -lCLHEP -lZMtools -lZMutility -lZMtimer -lExceptions -lstdhep -lFmcfio -lpdflib -lmathlib -lkernlib -lpacklib -lnsl -lg2c -lgsl -lm'

endif

if ($QUAERO_CC_PLATFORM == "atom") then
#  setup gcc v3_3_1
#  setup gcc v3_4_3
#  source ~cdfsoft/cdf2.cshrc
  source /cdf/cdf.cshrc
  setup cdfsoft2 6.1.2
  unsetenv MAKEFILES
  unsetenv MAKEFLAGS
  setup gsl v1_2
  setup cern 2000
  setup stdhep v4_09
#  setup CLHEP V01-07-15
#  setenv CLHEP_DIR /cdf/atom/code/cdfsoft/dist/releases/4.10.4/include/CLHEP/
#  setenv CDFSOFT2_DIR /cdf/atom/code/cdfsoft/dist/releases/4.11.2/
#  setenv CDFSOFT2_DIR /cdfsoft/dist/releases/6.1.2/
#  setenv FLAGS '-g -O2 -D_FILE_OFFSET_BITS=64 -I'$GSL_DIR'/include/ -I'$RELEASE_DIR'/include/ -I'$CDFSOFT2_DIR'/include/ -D QUAERO_CC_PLATFORM_'$QUAERO_CC_PLATFORM 
#  setenv LIBS '-L'$CRNLIB' -L'$GSL_DIR'/.libs/ -L'$GSL_DIR'/lib/ -L'$RELEASE_DIR'/lib/ -L'$CDFSOFT2_DIR'/lib/Linux2-KCC_4_0/ -L'$STDHEP_DIR'/lib/ -lCLHEP -lstdhep -lFmcfio -lpdflib -lmathlib -lkernlib -lpacklib -lnsl -lg2c -lgsl -lm'
#  setenv OBJS2 "idate_.o itime_.o"
  setenv FLAGS '-g -O3 -D_FILE_OFFSET_BITS=64 -I'$GSL_DIR'/include/ -I'$RELEASE_DIR'/include/ -I'$CDFSOFT2_DIR'/include/ -Wno-deprecated -Wno-sign-compare -D__STANDARD_CPLUSPLUS -DDEFECT_NO_EXCEPTIONS -D QUAERO_CC_PLATFORM_'$QUAERO_CC_PLATFORM 
  setenv LIBS '-L'$CRNLIB' -L'$GSL_DIR'/.libs/ -L'$GSL_DIR'/lib/ -L'$RELEASE_DIR'/lib/ -L'$CDFSOFT2_DIR'/lib//Linux2-GCC_3_4/ -L'$STDHEP_DIR'/lib/ -lCLHEP -lstdhep -lFmcfio -lZMtools -lZMutility -lZMtimer -lExceptions -lpdflib -lmathlib -lkernlib -lpacklib -lnsl -lg2c -lfixes2 -lgsl -lm'
  setenv CVSROOT :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2
endif

if ($QUAERO_CC_PLATFORM == "fcdflnx") then
  source ~cdfsoft/cdf2.cshrc
  setup cdfsoft2 6.1.4
  unsetenv MAKEFILES
  unsetenv MAKEFLAGS
  setup gsl v1_2
  setup cern 2000
  setup stdhep v4_09
  setenv OBJS2 "missingCLHEProutines.o"
  setenv FLAGS '-g -O2 -D_FILE_OFFSET_BITS=64 -I'$GSL_DIR'/include/ -I'$RELEASE_DIR'/include/ -I'$CDFSOFT2_DIR'/include/ -D CLHEP1 -D QUAERO_CC_PLATFORM_'$QUAERO_CC_PLATFORM 
  setenv LIBS '-L'$CRNLIB' -L'$GSL_DIR'/.libs/ -L'$GSL_DIR'/lib/ -L'$RELEASE_DIR'/lib/ -L'$CDFSOFT2_DIR'/lib/Linux2_SL-GCC_3_4/ -L'$STDHEP_DIR'/lib/ -lCLHEP -lstdhep -lFmcfio -lZMtools -lZMutility -lZMtimer -lExceptions -lpdflib -lmathlib -lkernlib -lpacklib -lnsl -lg2c -lgsl -lm'
#  setenv OBJS2 "idate_.o itime_.o"
  setenv CVSROOT :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2
  setenv CC 'g++ '
  setenv F77 'f77'
endif

if ($QUAERO_CC_PLATFORM == "cmsuaf") then
   setenv CLHEP_DIR /uscmst1/prod/sw/cms/slc3_ia32_gcc323/external/clhep/1.9.2.2
#  source /afs/fnal.gov/files/code/cms/setup/cshrc fnal
#  eval `scram setroot -csh ORCA ORCA_8_5_0`
#  eval `scram runtime -csh` 
  setenv LOCALRT /afs/fnal.gov/exp/cms/l/Releases/ORCA/ORCA_8_5_0
##  setenv LD_LIBRARY_PATH /afs/fnal.gov/exp/cms/l/lcg/external/gsl/1.4-1/rh73_gcc32/lib/
  #setup cern 2002 
#  setenv CRNLIB /uscmst1/prod/sw/cms/external/cern/2003/slc3_ia32_gcc323/lib  
  setenv CRNLIB /cern/pro/lib
  setenv GSL_DIR /usr/lib64

  #setup stdhep v5_01
  setenv F77 'g77'
##  setenv GSL_DIR /afs/fnal.gov/exp/cms/l/lcg/external/gsl/1.4/rh73_gcc32/
  setenv FLAGS '-g -O2 -D_FILE_OFFSET_BITS=64 -I'$GSL_DIR'/include/ -I'$RELEASE_DIR'/include/ -I'$CLHEP_DIR'/include/ -Wno-deprecated -D QUAERO_CC_PLATFORM_'$QUAERO_CC_PLATFORM
# -static
# cd $RELEASE_DIR/; cd Quaero_utils/src/; gcc -c ctype_b.c; gcc -c init_array.c; cd -;
#                   rm lib/libfixes1.a; ar rcv lib/libfixes1.a Quaero_utils/src/ctype_b.o Quaero_utils/src/init_array.o
# cd $RELEASE_DIR/; cd Quaero_utils/src/; gcc -c ctype.c; gcc -c init_array.c; cd -;
#                   rm lib/libfixes2.a; ar rcv lib/libfixes2.a Quaero_utils/src/ctype.o Quaero_utils/src/init_array.o
  setenv FLIBS '-L'$CRNLIB' -L'$GSL_DIR'/lib/ -L'$RELEASE_DIR'/lib/ -lCLHEP -lstdhep -lFmcfio -lpdflib804 -lmathlib -lkernlib -lpacklib -lnsl -lg2c -lgsl -lm'
# -lfixes2
  setenv LIBS '-L'$CRNLIB' -L'$GSL_DIR'/lib/ -L'$RELEASE_DIR'/lib/ -L'$CLHEP_DIR'/lib/ -lCLHEP -lstdhep -lFmcfio -lpdflib804 -lmathlib -lkernlib -lpacklib -lnsl -lg2c -lgsl -lgslcblas -lm'     
# setenv CVSROOT lpccvs@cdcvs.fnal.gov:/cvs/lpc
# read-only access: setenv CVSROOT :pserver:anonymous@cdcvs.fnal.gov:/cvs/lpc  cvs login (the password is: lpc)
# setenv CVSROOT :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2
 setenv CVSROOT uscmscvs@cdcvs.fnal.gov:/cvs/uscms
endif

if ($QUAERO_CC_PLATFORM == "cmslpc") then
   setenv CLHEP_DIR /uscmst1/prod/sw/cms/slc4_ia32_gcc345/external/clhep/1.9.2.2
#  source /afs/fnal.gov/files/code/cms/setup/cshrc fnal
#  eval `scram setroot -csh ORCA ORCA_8_5_0`
#  eval `scram runtime -csh` 
  setenv LOCALRT /afs/fnal.gov/exp/cms/l/Releases/ORCA/ORCA_8_5_0
##  setenv LD_LIBRARY_PATH /afs/fnal.gov/exp/cms/l/lcg/external/gsl/1.4-1/rh73_gcc32/lib/
  #setup cern 2002 
  setenv CRNLIB /uscmst1/prod/sw/cms/external/cern/2003/slc4_ia32_gcc345/lib  
  setenv GSL_DIR /usr/lib64

  #setup stdhep v5_01
  setenv F77 'g77'
##  setenv GSL_DIR /afs/fnal.gov/exp/cms/l/lcg/external/gsl/1.4/rh73_gcc32/
  setenv FLAGS '-g -O2 -D_FILE_OFFSET_BITS=64 -I'$GSL_DIR'/include/ -I'$RELEASE_DIR'/include/ -I'$CLHEP_DIR'/include/ -Wno-deprecated -D QUAERO_CC_PLATFORM_'$QUAERO_CC_PLATFORM
# -static
# cd $RELEASE_DIR/; cd Quaero_utils/src/; gcc -c ctype_b.c; gcc -c init_array.c; cd -;
#                   rm lib/libfixes1.a; ar rcv lib/libfixes1.a Quaero_utils/src/ctype_b.o Quaero_utils/src/init_array.o
# cd $RELEASE_DIR/; cd Quaero_utils/src/; gcc -c ctype.c; gcc -c init_array.c; cd -;
#                   rm lib/libfixes2.a; ar rcv lib/libfixes2.a Quaero_utils/src/ctype.o Quaero_utils/src/init_array.o
  setenv FLIBS '-L'$CRNLIB' -L'$GSL_DIR'/lib/ -L'$RELEASE_DIR'/lib/ -lCLHEP -lstdhep -lFmcfio -lpdflib804 -lmathlib -lkernlib -lpacklib -lnsl -lg2c -lgsl -lm'
# -lfixes2
  setenv LIBS '-L'$CRNLIB' -L'$GSL_DIR'/lib/ -L'$RELEASE_DIR'/lib/ -L'$CLHEP_DIR'/lib/ -lCLHEP -lstdhep -lFmcfio -lpdflib804 -lmathlib -lkernlib -lpacklib -lnsl -lg2c -lgsl -lgslcblas -lm'     
# setenv CVSROOT lpccvs@cdcvs.fnal.gov:/cvs/lpc
# read-only access: setenv CVSROOT :pserver:anonymous@cdcvs.fnal.gov:/cvs/lpc  cvs login (the password is: lpc)
# setenv CVSROOT :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2
 setenv CVSROOT uscmscvs@cdcvs.fnal.gov:/cvs/uscms
endif
                                                                            
if ($QUAERO_CC_PLATFORM == "fixedtarget") then
  source /fnal/ups/etc/setups.csh
  setup stdhep v4_09
  setup cern 2003
  setup gcc v3_3_1 -f Linux+2.4-2.3.2
  setup clhep v1_9_1_1 -q GCC3_3_1
  echo "clhep is " $CLHEP_DIR
  rm -f $RELEASE_DIR/include/CLHEP
  ln -sf $CLHEP_BASE_DIR/include/CLHEP $RELEASE_DIR/include/CLHEP
  setup gsl v1_4
  setenv F77 g77
  echo "clhep is " $CLHEP_DIR
  echo "clhep is " $CLHEP_DIR
  echo "clhep is " $CLHEP_DIR
  echo "cernlib is " $CRNLIB
  echo "clhep is " $CLHEP_DIR
  setenv FLAGS '-g -O3 -D_FILE_OFFSET_BITS=64 -I'$CLHEP_DIR'/include/CLHEP -I'$RELEASE_DIR'/include/ -I'$GSL_DIR'/include/ -Wno-deprecated -Wno-sign-compare -D__STANDARD_CPLUSPLUS -DDEFECT_NO_EXCEPTIONS -D QUAERO_CC_PLATFORM_'$QUAERO_CC_PLATFORM
  echo $FLAGS
  setenv LIBS '-L'$CRNLIB' -lpdflib804 -lmathlib -lpacklib -lkernlib -lpdflib804 -L'$STDHEP_DIR'/lib/ -lstdhep -lFmcfio -L'$CLHEP_DIR'/lib/ -lCLHEP -L'$RELEASE_DIR'/lib/ -lctype -L'$GSL_DIR'/lib -lgsl -lg2c  -lm -lnsl'
# -L/export/lsi_stage/run2mc_stage01/run2/lib/ 
  setenv CVSROOT :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2
endif

if ($QUAERO_CC_PLATFORM == "fnpc1") then
  setup cvs
  setup xemacs
#  setup gcc v3_2_1
  setup gcc v3_3_1
#  setup cern 2000  
  setup cern 2003
  setup stdhep v4_09  
#  setup gsl v1_3
  setup gsl v1_4
#  setup clhep v1_8_0_2 -q GCC3_2_1
  setup clhep v1_9_1_1 -q GCC3_3_1
  setenv F77 'g77'
#  setenv F77 $RELEASE_DIR/Quaero/runDir/quaero_development/g77
#  setenv FF $RELEASE_DIR/Quaero/runDir/quaero_development/g77
  setenv FLAGS '-g -O2 -D_FILE_OFFSET_BITS=64 -I'$GSL_DIR'/include/ -I'$RELEASE_DIR'/include/ -I'$CLHEP_DIR'/include/ -D QUAERO_CC_PLATFORM_'$QUAERO_CC_PLATFORM
# -static
# cd $RELEASE_DIR/; cd Quaero_utils/src/; gcc -c ctype_b.c; gcc -c init_array.c; cd -;
#                   rm lib/libfixes1.a; ar rcv lib/libfixes1.a Quaero_utils/src/ctype_b.o Quaero_utils/src/init_array.o
# cd $RELEASE_DIR/; cd Quaero_utils/src/; gcc -c ctype.c; gcc -c init_array.c; cd -;
#                   rm lib/libfixes2.a; ar rcv lib/libfixes2.a Quaero_utils/src/ctype.o Quaero_utils/src/init_array.o
#  setenv FLIBS '-L'$CRNLIB' -L'$GSL_DIR'/lib/ -L'$RELEASE_DIR'/lib/ -L'$STDHEP_DIR'/lib/ -L'$CLHEP_DIR'/lib/ -L'$RELEASE_DIR'/lib/ -lCLHEP-g++.1.8.0.2 -lstdhep -lFmcfio -lpdflib -lmathlib -lkernlib -lpacklib -lnsl -lg2c -lfixes2 -lg2c -lgsl -lm'
  setenv FLIBS '-L'$CRNLIB' -L'$GSL_DIR'/lib/ -L'$RELEASE_DIR'/lib/ -L'$STDHEP_DIR'/lib/ -L'$CLHEP_DIR'/lib/ -L'$RELEASE_DIR'/lib/ -lCLHEP-1.9.1.1 -lstdhep -lFmcfio -lpdflib -lmathlib -lkernlib -lpacklib -lnsl -lg2c -lg2c -lgsl -lm'
# -lfixes1
# -lfixes2
#  setenv LIBS '-L'$CRNLIB' -L'$GSL_DIR'/lib/ -L'$RELEASE_DIR'/lib/ -L'$STDHEP_DIR'/lib/ -L'$CLHEP_DIR'/lib/ -L'$RELEASE_DIR'/lib/ -lCLHEP-g++.1.8.0.2 -lstdhep -lFmcfio -lpdflib -lmathlib -lkernlib -lpacklib -lnsl -lg2c -lfixes2 -lg2c -lgsl -lm'     
  setenv LIBS '-L'$CRNLIB' -L'$GSL_DIR'/lib/ -L'$RELEASE_DIR'/lib/ -L'$STDHEP_DIR'/lib/ -L'$CLHEP_DIR'/lib/ -L'$RELEASE_DIR'/lib/ -lCLHEP-1.9.1.1 -lstdhep -lFmcfio -lpdflib -lmathlib -lkernlib -lpacklib -lnsl -lg2c -lg2c -lgsl -lm'     
#-lfixes2
  setenv CVSROOT :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2
endif                                                                                                                        
                                                                                                                             
if (($QUAERO_CC_PLATFORM == "uchicago")) then
  setup gcc v3_0_2
  setup gsl v1_2
  setup cern 2000
  setup stdhep v4_09
  setup CLHEP V01-07-15
  setenv CLHEP_DIR /cdf/code/cdfsoft/dist/releases/4.10.4/include/CLHEP/
#  setenv GSL_DIR /disk/user28a/knuteson/Knuteson/tools/gsl/gsl-1.4/
  setenv CDFSOFT2_DIR /cdf/code/cdfsoft/dist/releases/5.2.0/
  setenv FLAGS '-g -O2 -D_FILE_OFFSET_BITS=64 -I'$GSL_DIR'/include/ -I'$RELEASE_DIR'/include/ -I'$CLHEP_DIR'/include/ -I'$CDFSOFT2_DIR'/include/ -D QUAERO_CC_PLATFORM_'$QUAERO_CC_PLATFORM 
  setenv LIBS '-L'$CRNLIB' -L'$GSL_DIR'/.libs/ -L'$GSL_DIR'/lib/ -L'$RELEASE_DIR'/lib/ -L'$CDFSOFT2_DIR'/lib/Linux2-KCC_4_0/ -L'$STDHEP_DIR'/lib/ -L'$CLHEP_DIR'/lib/ -lCLHEP-g++.1.7.0.0 -lstdhep -lFmcfio -lpdflib -lmathlib -lkernlib -lpacklib -lnsl -lf2c -lgsl -lm'
  setenv OBJS2 "idate_.o itime_.o"
  setenv CVSROOT :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2
endif

if (($QUAERO_CC_PLATFORM == "rtc")) then
  setenv INTEL_PATH $PATH
  setenv MPICH_PATH $PATH
#  setenv F77 'g77-3'
  setenv FFLAGS '-i4 -g -static'
  setenv FLAGS '-g -O2 -D_FILE_OFFSET_BITS=64 -I'$RELEASE_DIR'/include/ -I/usr/local/CLHEP/clhep-1.8.2.0/include/ -D QUAERO_CC_PLATFORM_rtc '
# -static
# ln -sf /usr/local/CERN/2003/lib/libpdflib804.a $RELEASE_DIR/lib/libpdflib.a
  setenv OBJS2 "missingCLHEProutines.o"
  setenv LIBS '-L/usr/local/CERN/2003/lib/ -L/usr/local/CLHEP/clhep-1.8.2.0/lib/ -L'$RELEASE_DIR'/lib/ -lCLHEP-g++.1.8.2.0  -lstdhep -lFmcfio -lpdflib -lmathlib -lkernlib -lpacklib -lnsl -lg2c -lgsl -lgslcblas -lm'
  setenv CVSROOT :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2
endif

if (($QUAERO_CC_PLATFORM == "lxplus")) then
  setenv FLAGS '-g -O2 -D_FILE_OFFSET_BITS=64 -I'$RELEASE_DIR'/include/ -I/cern/pro/include/ -I/afs/cern.ch/sw/lhcxx/specific/redhat61/CLHEP/1.8.0.0/include/ -I/afs/cern.ch/sw/lcg/external/GSL/1.4/rh73_gcc32/include/ -Wno-deprecated -DQUAERO_CC_PLATFORM_lxplus -static'
  setenv LIBS '-L/cern/pro/lib/ -L/afs/cern.ch/sw/lhcxx/specific/redhat61/CLHEP/1.8.0.0/lib/ -L/afs/cern.ch/sw/lcg/external/GSL/1.4/rh73_gcc32/lib/ -lCLHEP-g++.1.8.0.0  -lstdhep -lFmcfio -lpdflib804 -lmathlib -lkernlib -lpacklib -lnsl -lg2c -lgsl -lgslcblas -lm'
#  setenv OBJS2 "idate_.o itime_.o"
  setenv CVSROOT :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2
endif

if (($QUAERO_CC_PLATFORM == "desy")) then
  alias g++ '/products/gcc-3.2/bin/g++'
#  setenv LD_LIBRARY_PATH "/afs/cern.ch/sw/lhcxx/specific/redhat61/CLHEP/1.8.0.0/lib/:/afs/cern.ch/sw/lcg/external/GSL/1.4/rh73_gcc32/lib/:/afs/desy.de/products/gcc-3.2/spec/i386_linux24/lib/"
  setenv FLAGS '-g -O2 -D_FILE_OFFSET_BITS=64 -I'$RELEASE_DIR'/include/ -I/cern/pro/include/ -I/afs/cern.ch/sw/lhcxx/specific/redhat61/CLHEP/1.8.0.0/include/ -I/afs/cern.ch/sw/lcg/external/GSL/1.4/rh73_gcc32/include/ -DQUAERO_CC_PLATFORM_desy'
  setenv LIBS '-L'$RELEASE_DIR'/lib/ -L/cern/pro/lib/ -L/afs/cern.ch/sw/lhcxx/specific/redhat61/CLHEP/1.8.0.0/lib/ -L/afs/cern.ch/sw/lcg/external/GSL/1.4/rh73_gcc32/lib/ -lCLHEP-g++.1.8.0.0  -lstdhep -lFmcfio -lpdflib -lmathlib -lkernlib -lpacklib -lnsl -lg2c -lgsl -lgslcblas -lm'
  setenv OBJS2 "idate_.o itime_.o"
  setenv CVSROOT :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2
endif

if( "$FLIBS" == "" ) then
  setenv FLIBS "$LIBS"
endif

setenv FF $F77

#echo 'done!'
