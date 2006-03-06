REM For CSCPEDESTALS:
SELECT
 iov_sequence.NextVal iov_value_id,
 run_num time,
 data_taking_time
FROM PED_RECORD
WHERE
 run_num=given_value;

REM For CSCPEDESTALS_MAP:
SELECT
 mapid_sequence.NextVal map_id,
 iov_sequence.CurrVal iov_value_id,
 layer_id csc_int_id
FROM PED_RECORD
WHERE
 run_num=given_value order by csc_int_id;

REM For CSCPEDESTALS_DATA:
SELECT
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
