select table_name, to_char(change_time, 'mm/dd/yyyy hh24:mi:ss') from last_modified_times order by change_time;
