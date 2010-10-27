#!/bin/csh -f

if( $#argv < 2 || $#argv > 4 ) then
	echo ""
	echo "Usage reSubmitBatchJobs.csh job_type (runZtoElecTau, etc.) sample_name min_job_num max_job_num"
	echo ""
	exit 1
endif

if( $#argv == 2) then
	bsub -q 1nd -J "$1_$2" -L /bin/csh -eo lxbatch/$2/log/$1_$2@Batch.out -oo lxbatch/$2/log/$1_$2@Batch.out < lxbatch/$2/csh/$1_$2@Batch.csh
	exit(1)
endif

set first = $3
set last = $first
if( $#argv == 4) then
	@ last = $4
endif

foreach num_tmp (`seq $first $last`)
	set num = $num_tmp
	if( ${%num_tmp} == 1 ) then 
		set num = `printf '%.2i' $num_tmp`
	endif
	bsub -q 1nd -J "$1_$2_part${num}" -L /bin/csh -eo lxbatch/$2/log/$1_${num}@Batch.out -oo lxbatch/$2/log/$1_${num}@Batch.out < lxbatch/$2/csh/$1_$num@Batch.csh
end
