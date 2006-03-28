REM
REM Table GAINS
REM
create table GAINS (
run_num          number NOT NULL,
data_taking_time date DEFAULT sysdate NOT NULL,
insert_time      date DEFAULT sysdate NOT NULL);
REM
REM Adding constraints for table GAINS
REM
alter table GAINS
  add constraint gain_run_pk primary key (run_num);

REM
REM Table GAINS_MAP
REM
create table GAINS_MAP (
map_id      number NOT NULL,
run_num     number NOT NULL,
layer_id    varchar2(32) NOT NULL);
REM
REM Adding constraints for table GAINS_MAP
REM
alter table GAINS_MAP add (
   constraint gain_map_pk primary key (map_id),
   unique (run_num,layer_id),
   constraint gain_map_fk foreign key (run_num)
                         references gains(run_num));

REM
REM Table GAINS_DATA
REM
create table GAINS_DATA (
map_id         number NOT NULL,
vec_index      number(5) NOT NULL,
gain_slope     binary_float NOT NULL,
gain_intercept binary_float NOT NULL,
gain_chi2      binary_float NOT NULL);
REM
REM Adding constraints for table GAINS_DATA
REM
alter table GAINS_DATA add (
   constraint gain_data_pk primary key (map_id,vec_index),
   constraint gain_data_fk foreign key (map_id)
                          references gains_map(map_id));
