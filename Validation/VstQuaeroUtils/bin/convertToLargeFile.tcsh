#!/bin/tcsh
#
# Usage:  convertToLargeFile.tcsh 
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
  echo ${largeFile}
  mv $fileq $LARGE_FILE_DIR/$largeFile
  if ( -e $LARGE_FILE_DIR/$largeFile ) then
    ln -sf $LARGE_FILE_DIR/$largeFile $fileq
  endif
end

  
