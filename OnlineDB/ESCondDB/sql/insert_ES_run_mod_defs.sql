insert into es_run_mode_def(run_mode_string) values ('LOCAL');
insert into es_run_mode_def(run_mode_string) values ('GLOBAL');
insert into es_run_mode_def(run_mode_string) values ('TESTBEAM');
insert into es_sequence_type_def(RUN_TYPE_DEF_ID, SEQUENCE_TYPE_STRING) values (116, 'DUMMY_SEQUENCE');
insert into es_sequence_type_def(RUN_TYPE_DEF_ID, SEQUENCE_TYPE_STRING) values (117, 'TEST_SEQUENCE');
insert into es_sequence_type_def(RUN_TYPE_DEF_ID, SEQUENCE_TYPE_STRING) values (118, 'DEFAULT_SEQUENCE');
insert into es_sequence_type_def(RUN_TYPE_DEF_ID, SEQUENCE_TYPE_STRING) values (100, 'STD');
insert into es_sequence_type_def(RUN_TYPE_DEF_ID, SEQUENCE_TYPE_STRING) values (101, 'DELTA_SCAN');

-- insert into ES_RUN_CONFIGURATION_DAT (TAG, version, RUN_TYPE_DEF_ID, RUN_MODE_DEF_ID, NUM_OF_SEQUENCES, description, CONFIG_ID, DB_TIMESTAMP) values ('DEFAULT',1,116,1,1,'dummy run', 1, '28-Jul-08');
-- insert into es_sequence_dat (es_config_id, SEQUENCE_NUM,NUM_OF_CYCLES,SEQUENCE_TYPE_DEF_ID,description, sequence_id) values (1,1,1,1,'dummy sequence', 1);

-- insert into ES_RUN_CONFIGURATION_DAT (TAG, version, RUN_TYPE_DEF_ID, RUN_MODE_DEF_ID, NUM_OF_SEQUENCES, description, CONFIG_ID, DB_TIMESTAMP) values ('Pedestal',0,116,1,1,'dummy run', 0, '28-Jul-08');


