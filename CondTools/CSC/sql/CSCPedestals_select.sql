create sequence iov_sequence_ped increment by 1 start with 1 order;

create view RUNS as select distinct run_num from ped_record
 order by run;

REM For CSCPEDESTALS:
SELECT
 iov_sequence_ped.NextVal iov_value_id,
 run_num time
FROM RUNS;

REM For CSCPEDESTALS_MAP:
SELECT
 record_id map_id,
 iov_value_id,
 layer_id csc_int_id
FROM PED_RECORD, CSCPEDESTALS
WHERE CSCPEDESTALS.time=PED_RECORD.run_num
 order by csc_int_id;

REM For CSCPEDESTALS_DATA:
SELECT
 vector_index vec_index,
 PED_RECORD.record_id map_id,
 CSCPEDESTALS_MAP.iov_value_id,
 PED_VALUES.ped pedestals_ped,
 PED_VALUES.rms pedestals_rms
FROM
 PED_VALUES,PED_RECORD,CSCPEDESTALS_MAP
WHERE
 PED_VALUES.record_id=PED_RECORD.record_id and
 CSCPEDESTALS_MAP.map_id=PED_RECORD.record_id
ORDER BY
 iov_value_id,
 map_id,
 vector_index;
