create sequence iov_sequence_gain increment by 1 start with 1 order;

create view GAIN_RUNS as select distinct run_num from gain_record
 order by run;

REM For CSCGAINS:
SELECT
 iov_sequence_gain.NextVal iov_value_id,
 run_num time
FROM GAIN_RUNS;

REM For CSCGAINS_MAP:
SELECT
 record_id map_id,
 iov_value_id,
 layer_id csc_int_id
FROM GAIN_RECORD, CSCGAINS
WHERE CSCGAINS.time=GAIN_RECORD.run_num
 order by csc_int_id;

REM For CSCGAINS_DATA:
SELECT
 vector_index vec_index,
 GAIN_RECORD.record_id map_id,
 CSCGAINS_MAP.iov_value_id,
 GAIN_VALUES.gain_chi2 gains_chi2, 
 GAIN_VALUES.gain_intercept gains_intercept,
 GAIN_VALUES.gain_slope, gains_slope
FROM
 GAIN_VALUES,GAIN_RECORD,CSCGAINS_MAP
WHERE
 GAIN_VALUES.record_id=GAIN_RECORD.record_id and
 CSCGAINS_MAP.map_id=GAIN_RECORD.record_id
ORDER BY
 iov_value_id,
 map_id,
 vector_index;
