#!/bin/tcsh
#
# Usage:  restoreFromLargeFile.tcsh <files>
#
# Change LARGE_FILE_DIR below to your large file directory
#

setenv LARGE_FILE_DIR /home/$USER/LargeFiles/

foreach fileq ( $* )
   switch ( $fileq )
     case /.*:
	setenv largeFile `echo $fileq | sed -e 's/\//#/g'`
     breaksw
     default:
        setenv largeFile `pwd`/${fileq}
        setenv largeFile `echo $largeFile | sed -e 's/\//#/g'`
     breaksw
   endsw
  if ( -e $LARGE_FILE_DIR/$largeFile ) then
    echo ${largeFile}
    rm $fileq
    mv $LARGE_FILE_DIR/$largeFile $fileq 
  else
    echo 'Large file' $fileq 'does not exist'
  endif
end
