#!/bin/tcsh -f


foreach run ( `/bin/ls plots/Laser`)
echo "Does $run exist on castor?"
if ( `rfdir /castor/cern.ch/user/c/ccecal/Laser | grep -c $run` > 0 ||`rfdir /castor/cern.ch/user/c/ccecal/LASER | grep -c $run` > 0 ) then
echo "yes it is" 
#rm -rf $run
rm -rf plots/Laser/${run}/*root 
echo "now removed"
endif

end

#end of file
