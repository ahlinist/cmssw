 create sequence iov_sequence_ped increment by 1 start with 1 order;

REM For CSCPEDESTALS:
insert into "CSCPEDESTALS" SELECT
 iov_sequence_ped.NextVal iov_value_id,
 run_num time
FROM PED_RECORD;

create sequence mapid_sequence_ped increment by 1 start with 1 order;

REM For CSCPEDESTALS_MAP:
insert into "CSCPEDESTALS_MAP" SELECT
 mapid_sequence_ped.NextVal map_id,
 CSCPEDESTALS.iov_value_id iov_value_id,
 layer_id csc_int_id
FROM PED_RECORD, CSCPEDESTALS
WHERE CSCPEDESTALS.iov_value_id=
 order by csc_int_id;

REM For CSCPEDESTALS_DATA:
insert into "CSCPEDESTALS_DATA" SELECT
 vector_index.PED_VALUES vec_index,
 map_id.CSCPEDESTALS_MAP,
 iov_value_id.CSCPEDESTALS_MAP,
 ped.PED_VALUES,
 rms.PED_VALUES
FROM
 PED_VALUES,PED_RECORD,CSCPEDESTALS_MAP
WHERE
 record_id.PED_VALUES=record_id.PED_RECORD and
 layer_id.CSCPEDESTALS_MAP=layer_id.PED_RECORD
ORDER BY
 iov_value_id.CSCPEDESTALS_MAP,
 map_id.CSCPEDESTALS_MAP,
 vector_index.PED_VALUES;
