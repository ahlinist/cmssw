#!/bin/tcsh
##################################################
# We assume you have already executed the following:
#
# setup cdfsoft2 6.1.4
#  or
# setenv CVSROOT cdfcvs@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2
#  or
# setenv CVSROOT :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2
#
# cd ~; mkdir -p cvs/current/; cd cvs/current/
# cvs checkout Quaero_utils
#
# and are now ready to execute
#
# source ./Quaero_utils/utilities/build.tcsh
##################################################



##################################################
# CVS checkout all relevant packages
##################################################
setenv packages "Math_utils"
foreach f ( $packages )
  cvs checkout $f
end

source Quaero_utils/utilities/setup.tcsh

##################################################
# Set up links in preparation for compilation
##################################################
mkdir bin include lib
setenv packages "$packages Quaero_utils"
cd include
foreach f ( $packages )
  ln -sf ../$f/$f/ .
end
cd -
echo '# Makefile for release directory\n\n' > GNUmakefile
echo 'all:' >> GNUmakefile
echo '\t cd Math_utils; gmake all; cd -;' >> GNUmakefile
echo '\t cd Quaero_utils; gmake all; cd -;' >> GNUmakefile
echo '' >> GNUmakefile
echo 'update:' >> GNUmakefile
echo '\t cd Math_utils; cvs update; cd -;' >> GNUmakefile
echo '\t cd Quaero_utils; cvs update; cd -;' >> GNUmakefile
echo '' >> GNUmakefile


##################################################
# Grab KtJet
##################################################
wget http://hepforge.cedar.ac.uk/hf/archive/ktjet/KtJet-1.06.tar.gz
gunzip KtJet-1.06.tar.gz
tar xvf KtJet-1.06.tar
rm KtJet-1.06.tar
cd include/
ln -sf ../KtJet-1.06/KtJet/ .
cd -
cd KtJet-1.06/
./configure --with-clhep=/home/cdfsoft/products/clhep/v1_9_2_1GCC3_4_3/Linux+2.4-2.3.2/
make
mv src/libKtEvent.a $RELEASE_DIR/lib/
cd $RELEASE_DIR

##################################################
# Compile
##################################################

gmake all


 
