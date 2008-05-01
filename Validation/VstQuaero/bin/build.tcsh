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
# cvs checkout Quaero
#
# and are now ready to execute
#
# source ./Quaero/utilities/build.tcsh |& tee log_build.txt
##################################################



##################################################
# CVS checkout all relevant packages
##################################################
echo "Checking out all relevant packages"
setenv packages "MathUtils FewKDE OptimalBinning MadEvent Quaero_utils TurboSim"
foreach f ( $packages )
  cvs checkout $f
end

source QuaeroUtils/utilities/setup.tcsh

##################################################
# Set up links in preparation for compilation
##################################################
echo "Setting up links in preparation for compilation"
mkdir bin include lib
setenv packages "$packages Quaero"
cd include
foreach f ( $packages )
  ln -sf ../$f/$f/ .
end
cd -
cat ~knuteson/cvs/current/GNUmakefile | grep -v Vista | grep -v Bard | grep -v Sleuth > GNUmakefile

##################################################
# Grab KtJet
##################################################
echo "Grabbing KtJet"
wget http://hepforge.cedar.ac.uk/hf/archive/ktjet/KtJet-1.06.tar.gz
gunzip KtJet-1.06.tar.gz
tar xvf KtJet-1.06.tar
rm KtJet-1.06.tar
cd include/
ln -sf ../KtJet-1.06/KtJet/ .
cd -
cd KtJet-1.06/
if ( $QUAERO_CC_PLATFORM == "fcdflnx" ) setenv CLHEP_BASE_DIR /home/cdfsoft/products/clhep/v1_9_2_1GCC3_4_3/Linux+2.4-2.3.2/
if ( $QUAERO_CC_PLATFORM == "cmsuaf" ) setenv CLHEP_BASE_DIR /uscmst1/prod/sw/cms/slc3_ia32_gcc323/external/clhep/1.9.2.2/

./configure --prefix=$RELEASE_DIR --with-clhep=$CLHEP_BASE_DIR
make
cd $RELEASE_DIR

##################################################
# Compile
##################################################
echo "Compiling"
source QuaeroUtils/utilities/setup.tcsh
gmake all

##################################################
# Set up Quaero/runDir/quaero/ directory
##################################################
echo "Setting up Quaero/runDir/quaero/ directory"
cd Quaero/runDir/quaero/
if ( $QUAERO_CC_PLATFORM == "fixedtarget" ) cp /local/ups/prd/gcc/v3_3_1/Linux-2-4-2-3-2/bin/g77 .
cp -pr ../quaero_mit/requests/0 requests/0
rm -r requests/0/CVS
echo '5' > currentId.txt
ln -sf $RELEASE_DIR/MadEvent MadEvent
echo '#\!/bin/tcsh' > environmentVariables.tcsh
echo "setenv QUAERO_EDITION quaero" >> environmentVariables.tcsh
echo "setenv QUAERO_CC_DIR $RELEASE_DIR/Quaero/runDir/" >> environmentVariables.tcsh
echo "setenv QUAERO_HTML_URL http://www-cdf.fnal.gov/~$USER/Quaero/" >> environmentVariables.tcsh
ln -sf $RELEASE_DIR/bin/quaero .
mkdir signalFileStorage
ln -sf $RELEASE_DIR/MadEvent/Pyxtra/mlm .

ln -sf $RELEASE_DIR/Quaero/experiments/ .
if ( $QUAERO_CC_PLATFORM == "fixedtarget" ) setenv filesDirectory /farm/run2mc_stage01/run2/Quaero/experiments/tev2/cdf/
if ( $QUAERO_CC_PLATFORM == "fcdflnx" ) setenv filesDirectory ~knuteson/cvs/current/Quaero/experiments/tev2/cdf/
cp -p $filesDirectory/data/data_* experiments/tev2/cdf/data/
mkdir experiments/tev2/cdf/bkg/
cp -p $filesDirectory/bkg/bkg_* experiments/tev2/cdf/bkg/
rm experiments/tev2/cdf/bkg/bkgListing.dat >& /dev/null
foreach file ( experiments/tev2/cdf/bkg/bkg_*.txt )
  setenv fs $file:t:r:s/bkg_//
  echo -n $fs ' '
  echo $fs `sumWeight -bkgListing $file` ';' >> experiments/tev2/cdf/bkg/bkgListing.dat
end

cd experiments/tev2/cdf/simulate/; 
ln -sf $RELEASE_DIR/bin/turboSim .
ln -sf $RELEASE_DIR/bin/assignSystematicUncertainties_cdf assignSystematicUncertainties
ln -sf $RELEASE_DIR/bin/normalizeProperly_cdf normalizeProperly
cp $filesDirectory/simulate/turboSimMorphisms.txt .

if ( $QUAERO_CC_PLATFORM == "fixedtarget" ) cp /export/lsi_stage/run2mc_stage01/run2/lib/libctype.a lib/

cd -

rm -r ../quaero_mit/ ../quaero_development/

##################################################
# Now perform a test run
##################################################
echo "Now performing a test run"
cp -pr requests/0 requests/1
cat requests/0/request.txt | sed -e 's/#0/#1/' > requests/1/request.txt
source environmentVariables.tcsh
./quaero 1 -debugLevel5 |& tee requests/1/logfile.txt

endif

##################################################
# Finis
##################################################
echo 'done!'

