REM For CSCGAINS:
SELECT
 iov_sequence_gain.NextVal iov_value_id,
 run_num time
FROM GAIN_RECORD;

REM For CSCGAINS_MAP:
SELECT
 mapid_sequence_gain.NextVal map_id,
 iov_sequence_gain.CurrVal iov_value_id,
 layer_id csc_int_id
FROM GAIN_RECORD
 order by csc_int_id;

REM For CSCGAINS_DATA:
SELECT
 vector_index.GAIN_VALUES vec_index,
 map_id.CSCGAINS_MAP,
 iov_value_id.CSCGAINS_MAP,
 gain_slope.GAIN_VALUES,
 gain_intercept.GAIN_VALUES,
 gain_chi2.GAIN_VALUES
FROM
 GAIN_VALUES,GAIN_RECORD,CSCGAINS_MAP
WHERE
 record_id.GAIN_VALUES=record_id.GAIN_RECORD and
 layer_id.CSCGAINS_MAP=layer_id.GAIN_RECORD
ORDER BY
 iov_value_id.CSCGAINS_MAP,
 map_id.CSCGAINS_MAP,
 vector_index.GAIN_VALUES;
