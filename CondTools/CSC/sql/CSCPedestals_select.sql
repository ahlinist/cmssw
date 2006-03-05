REM For CSCPEDESTALS:
select iov_sequence.NextVal,run_num,data_taking_time from PED_RECORD
  where run_num=given_value;

REM For CSCPEDESTALS_MAP:
select mapid_sequence.NextVal,iov_sequence.CurrVal,layer_id from PED_RECORD
  where run_num=given_value order by layer_id;

REM For CSCPEDESTALS_DATA:
select vector_index.PED_VALUES,map_id.CSCPEDESTALS_MAP,iov_value_id.CSCPEDESTALS_MAP,
  ped.PED_VALUES,rms.PED_VALUES from PED_VALUES,PED_RECORD,CSCPEDESTALS_MAP
  where record_id.PED_VALUES=record_id.PED_RECORD and
    layer_id.CSCPEDESTALS_MAP=layer_id.PED_RECORD
  order by iov_value_id.CSCPEDESTALS_MAP,map_id.CSCPEDESTALS_MAP,vector_index.PED_VALUES;
