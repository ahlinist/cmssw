#!/bin/tcsh

#setup atom_tools
setenv farm_nodes_file ~/cron/farm_nodes.txt
setenv ATOM_TOOLS_DIR /cdf/atom/code/products/atom_tools/v1_00/
setenv nNodes `wc -l $farm_nodes_file | awk '{print $1}'`
setenv runningJobs `ls ~/cron/running/cron_* |& grep -v 'No match' | wc -l` 
setenv maxJobs `echo "print $nNodes*2" | python`
if ( $runningJobs < $maxJobs ) then
  setenv f `ls ~/cron/toRun/ | head -1`
  if ( $f != "" ) then
    setenv tmpFile /tmp/tmp_$$
    cat $farm_nodes_file | awk 'NR % '`echo $nNodes`' == '`echo $runningJobs`' % '`echo $nNodes` > $tmpFile
    setenv myTargetNode `cat $tmpFile`
    if ( `ping $myTargetNode -w 5 | grep -i 'unreachable' | wc -l` > 0 ) then
      cat $farm_nodes_file | awk '(NR-1) % '`echo $nNodes`' == '`echo $runningJobs`' % '`echo $nNodes` > $tmpFile.1
      setenv myTargetNode `cat $tmpFile.1`
    endif
    rm $tmpFile
    setenv ff ~/cron/running/$f
    mv ~/cron/toRun/$f $ff
    echo `cat $ff | egrep -i '[0-9]+' | sed -e 's/.*requests\///' | sed -e 's/\/runcommand.*//'` '     ' $myTargetNode '    ' `date` >> ~/cron/submissions.txt
    ssh $myTargetNode "$ff" >& /dev/null
    rm $ff
  endif
endif
