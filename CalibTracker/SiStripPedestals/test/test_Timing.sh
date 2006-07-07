#!/bin/sh

function test_db() {

    mode=""
    [ `echo $@ | grep -c "\-write[ ]*"` = 1 ] && mode=write 
    [ `echo $@ | grep -c "\-read[ ]*"`  = 1 ] && mode=read 

    [ "$mode" != "write" ] && [ "$mode" != "read" ] && return
    
    blobflag=noblob
    [ `echo $@ | grep -c "\-blob[ ]*"` = 1 ] && blobflag=blob 

    [ `echo $@ | grep -c "\-ped[ ]*"`      = 1 ] && module=ped      && mapping=blobmappingfile_pedestal.xml
    [ `echo $@ | grep -c "\-noise[ ]*"`    = 1 ] && module=noise    && mapping=blobmappingfile_noise.xml
    [ `echo $@ | grep -c "\-pednoise[ ]*"` = 1 ] && module=pednoise && mapping=blobmappingfile_pednoise.xml

    [ `echo $@ | grep -c "\-geom_mtcc[ ]*"`  = 1 ] && geom=geom_mtcc
    [ `echo $@ | grep -c "\-geom_ideal[ ]*"` = 1 ] && geom=geom_ideal

    debugflag="false"
    [ `echo $@ | grep -c "\-debug[ ]*"` = 1 ] && debugflag=debug 

    logfile=${mode}_${module}_${geom}_${blobflag}.log
    dbfile=${workdir}/${module}_${geom}_${blobflag}.db
    dbcatalog=${workdir}/${module}_${geom}_${blobflag}.xml

    cfgfile=${workdir}/${mode}_${module}_${geom}_${blobflag}.cfg


    eval `scramv1 runtime -sh`


    if [ ${blobflag} == "blob" ] && [ "${mode}" == "write" ]; then

	[ -e ${dbfile} ] && rm -f ${dbfile}
	echo	pool_build_object_relational_mapping -f ${mapping} -d CondFormatsSiStripObjects -c sqlite_file:${dbfile} -u me -p mypass  
	pool_build_object_relational_mapping -f ${mapping} -d CondFormatsSiStripObjects -c sqlite_file:${dbfile} -u me -p mypass  
    fi

    cat template_Timing_${mode}.cfg | sed \
	-e "s@insert_workdir@${workdir}@"  \
	-e "s@insert_logfile@${logfile}@"  \
	-e "s@insert_dbfile@${dbfile}@"  \
	-e "s@insert_dbcatalog@${dbcatalog}@"  \
	-e "s@\#${geom}@@"  \
	-e "s@\#${blobflag}@@"  \
	-e "s@\#${debugflag}[ ]* @@g" \
	-e "s@\#${module}[ ]* @@g" \
	> ${cfgfile}

    export CORAL_AUTH_USER="me"
    export CORAL_AUTH_PASSWORD="me"

    echo -e "\ncmsRun ${cfgfile} | tee ${workdir}/out \n"
    cmsRun ${cfgfile} | tee ${workdir}/out

    export timereport=`grep "Time report complete in" ${workdir}/out | awk '{print $6}'`
}

echo -e "Creating sqlite db schema from custom mapping"

export workdir=/tmp/$USER/$$

echo -e "workdir $workdir\n"

[ -e ${workdir} ] && rm -rf ${workdir} 
mkdir ${workdir}

i=0
for stream in blob noblob; 
  do
  for mode in write read;
    do
    for what in ped noise pednoise;
      do
      echo -e "\n\n$mode $what with $stream on geometry mtcc\n\n"      
      test_db -$mode -$what -$stream -geom_mtcc -debug
      timeis[$i]=$timereport
      let i++
      echo -e "\n\n$mode $what with $stream on geometry ideal\n\n"      
      test_db -$mode -$what -$stream -geom_ideal
      timeis[$i]=$timereport
      let i++
      done
  done
done

echo -e "\n\nTime Report\n\n"


i=0
for stream in blob noblob; 
  do
  for mode in write read;
    do
    for what in ped noise pednoise;
      do
      echo -e "$mode \t$what \twith $stream on geometry mtcc debug \t\t" ${timeis[$i]}     
      let i++
      echo -e "$mode \t$what \twith $stream on geometry ideal      \t\t" ${timeis[$i]}     
      let i++
      done
  done
done
