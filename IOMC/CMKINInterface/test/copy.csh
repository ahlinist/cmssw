#!/bin/csh
#
#  Hector Naves Sordo
# (Hector.Naves.Sordo@cern.ch)
#
echo ""
echo "*********************************"
echo "* Copying CMKIN libs locally... *"
echo "********************************"
echo ""

# Creating libs directory if it dosn't exists.
#
#mkdir -p ../../../../lib/Linux__2.4/
mkdir -p ../../../../lib/slc3_ia32_gcc323/

# SCRAM needs found CMKIN libraries!
#
#ln -sv /afs/cern.ch/cms/Releases/CMKIN/CMKIN/lib/i386_linux24/*.a ../../../../lib/Linux__2.4/
ln -sv  /afs/cern.ch/cms/Releases/CMKIN/CMKIN/lib/cel3-i386_gcc323/*.a ../../../../lib/slc3_ia32_gcc323/

ln -s /usr/local/lib/libshif*  ../../../../lib/slc3_ia32_gcc323/
ln -s  /cern/pro/lib/*.a  ../../../../lib/slc3_ia32_gcc323/

#ln -s /afs/fanae/cms/lcg/external/clhep/1.9.1.2/slc3_ia32_gcc323/lib/libCLHEP-1.9.1.2.so.0  ../../../../lib/slc3_ia32_gcc323/
echo ""
echo "Done!"
echo ""
