#!/bin/tcsh

cd /work/thedude-clued0/Quaero/;
source QuaeroUtils/utilities/setup.tcsh;
cd Quaero/runDir/;

if ( `qstat -u $USER | grep -v ' E ' | grep -i $USER` == "" ) then
  foreach edition ( quaero* )
    cd $edition
    foreach stuckJob ( `find ./requests/ -mmin +20 -name runcommand.tcsh -print` )
      if ( -e ${stuckJob:h}/done ) then
        ;
      else
        if ( -e ${stuckJob:h}/.request.txt ) then
          mv ${stuckJob:h}/.request.txt ${stuckJob:h}/request.txt
        endif
        ./qsub 'cd '`pwd`'; source environmentVariables.tcsh; source '$stuckJob
      endif
    end
    cd -
  end
endif

