/* TABLES */

create table cms_cond_21x_popconlog.cond_log_table (
  logid number(20),
  exectime varchar2(4000) constraint cond_log_table_exectime not null,
  iovtag varchar2(4000) constraint cond_log_table_iovtag not null,
  iovtimetype varchar2(4000) constraint cond_log_table_iovtimetype not null,
  payloadcontainer varchar2(4000) constraint cond_log_table_payloadcont not null,
  payloadname varchar2(4000) constraint cond_log_table_payloadname not null,
  payloadtoken varchar2(4000) constraint cond_log_table_payloadtoken not null,
  payloadindex number(10) constraint cond_log_table_payloadindex not null,
  destinationdb varchar2(4000) constraint cond_log_table_destinationdb not null,
  provenance varchar2(4000),
  usertext varchar2(4000),
  execmessage varchar2(4000),
  constraint cond_log_table_pk primary key (logid)
);

grant select on cond_log_table to public;

create table cond_sequence_table (
  reftable_name varchar2(4000),
  idvalue number(20) constraint cond_sequence_table_cs not null,
  constraint cond_sequence_table_pk primary key (reftable_name)
);

grant select on cond_sequence_table to public;

create table logtails (
  filename varchar2(1000),
  crontime timestamp(6) with time zone,
  short_tail varchar2(4000),
  long_tail clob,
  constraint tail_integr check (filename is not null and crontime is not null and short_tail is not null),
  constraint pk_logtails primary key (filename)
);

create table p_con_execution (
  exec_id number(10),
  obj_id number(10) constraint p_con_execution_cs not null,
  exec_start timestamp(6) default null,
  exec_end timestamp(6) default null,
  status varchar2(100) default 'FAILED',
  constraint p_con_execution_pk primary key (exec_id),
  constraint p_con_execution_fk foreign key (obj_id) references p_con_payload_state (obj_id) on delete cascade
);

create table p_con_execution_payload (
  pl_id number(10),
  exec_id number(10),
  written timestamp(6) default null,
  except_description varchar2(200) default 'Not Written',
  constraint p_con_execution_payload_pk primary key (pl_id),
  constraint p_con_execution_payload_fk foreign key (exec_id) references p_con_execution (exec_id) on delete cascade
);

create table p_con_lock (
  lock_id number(10),
  object_name varchar2(50)
);

create table p_con_payload_state (
  obj_id number(10) constraint p_con_payload_state_objid_cs not null,
  name varchar2(100 byte) constraint p_con_payload_state_name_cs not null,
  payload_size number(10) default 0,
  except_description varchar2(200) default null,
  manual_override varchar2(100)default null,
  connect_string varchar2(200) default null,
  constraint p_con_payload_state_pk primary key (obj_id),
  constraint p_con_payload_state_uniq unique (name, connect_string)
);



/* VIEWS */

create or replace force view cond_log_view as
select
  logid,
  to_date(exectime, 'YYYY-MM-DD-HH24-MI-SS') as exectime,
  iovtag,
  iovtimetype,
  payloadcontainer,
  htf.escape_sc(payloadname) as payloadname,
  replace(payloadtoken, '][', ']<br>[') as payloadtoken,
  payloadindex,
  lower(destinationdb) as destinationdb,
  provenance,
  case when lower(iovtag || payloadname) like '%ecal%' then regexp_replace(usertext, '[[:space:]]*([^[:alnum:]=.]+)[[:space:]]*', ' ') else usertext end as usertext,
  execmessage,
  case
    when lower(destinationdb) like 'oracle%' then rpad(chr(13), 7) || '<nobr>' || rpad(chr(13), 9) || '<a href="javascript:see(''popcongraph.php?tc=hourly' || chr(38) || 'svr=cms_orcoff_prod' || chr(38) || 'app=new' || chr(38) || 'det=' || regexp_substr(destinationdb, '([^/]+)$') || ''', ''hourly'');" class="p2k">Hourly</a>' || rpad(chr(13), 9) || '<a href="javascript:see(''popcongraph.php?tc=daily' || chr(38) || 'svr=cms_orcoff_prod' || chr(38) || 'app=new' || chr(38) || 'det=' || regexp_substr(destinationdb, '([^/]+)$') || ''', ''daily'');" class="p2k">Daily</a>' || rpad(chr(13), 7) || '<nobr>' || rpad(chr(13), 5)
    else 'N/A for not Oracle destination DB'
  end as activitygraph
from cond_log_table;

create or replace force view p_con_all_in_one as
select
  a.pl_id as payload_id,
  a.written as payload_written,
  b.exec_id,
  b.exec_start,
  b.exec_end,
  b.status,
  c.obj_id,
  c.name,
  c.payload_size,
  c.connect_string
from
  p_con_execution_payload a,
  p_con_execution b,
  p_con_payload_state c
where a.exec_id = b.exec_id
  and b.obj_id = c.obj_id;

create or replace force view p_con_execution_payload_toad as
select pl_id, exec_id, cast(written as date) as written, except_description
from p_con_execution_payload;

create or replace force view p_con_execution_toad as
select exec_id, obj_id, cast(exec_start as date) as exec_start, cast(exec_end as date) as exec_end, status
from p_con_execution;

create or replace force view p_con_hits_daily_new as
select
  case
    when lower(destinationdb) like 'oracle%' then upper(regexp_substr(destinationdb, '([^/]+)$'))
    else upper(regexp_substr(destinationdb, '([^:]+)'))
  end as account,
  to_char(exectime, 'yyyy:mm:dd') as day,
  count(*) as frequency
from cms_cond_21x_popconlog.cond_log_view
where trunc(sysdate) - trunc(exectime) < 30
  and execmessage = 'OK'
group by case when lower(destinationdb) like 'oracle%' then upper(regexp_substr(destinationdb, '([^/]+)$')) else upper(regexp_substr(destinationdb, '([^:]+)')) end, to_char(exectime, 'yyyy:mm:dd')
order by to_char(exectime, 'yyyy:mm:dd') desc;

create or replace force view p_con_hits_daily_old as
select
  upper(regexp_substr(connection_string, '([^/]+)$')) as account,
  to_char(execution_time, 'yyyy:mm:dd') as day,
  count(*) as frequency
from cms_cond_21x_popconlog.p_con_web_show
where trunc(sysdate) - trunc(execution_time) < 30
  and status = 'OK'
group by regexp_substr(connection_string, '([^/]+)$'), to_char(execution_time, 'yyyy:mm:dd')
order by to_char(execution_time, 'yyyy:mm:dd') desc;

create or replace force view p_con_hourly_new as
select
  case
    when lower(destinationdb) like 'oracle%' then upper(regexp_substr(destinationdb, '([^/]+)$'))
    else upper(regexp_substr(destinationdb, '([^:]+)'))
  end as account,
  to_char(exectime, 'yyyy:mm:dd:hh24') as hour,
  count(*) as frequency
from cms_cond_21x_popconlog.cond_log_view
where trunc(sysdate, 'hh24') - trunc(exectime, 'hh24') < 1
  and execmessage = 'OK'
group by case when lower(destinationdb) like 'oracle%' then upper(regexp_substr(destinationdb, '([^/]+)$')) else upper(regexp_substr(destinationdb, '([^:]+)')) end, to_char(exectime, 'yyyy:mm:dd:hh24')
order by to_char(exectime, 'yyyy:mm:dd:hh24') desc;

create or replace force view p_con_hits_hourly_old as
select
  upper(regexp_substr(connection_string, '([^/]+)$')) as account,
  to_char(execution_time, 'yyyy:mm:dd:hh24') as hour,
  count(*) as frequency
from cms_cond_21x_popconlog.p_con_web_show
where trunc(sysdate, 'hh24') - trunc(execution_time, 'hh24') < 1
  and status = 'OK'
group by regexp_substr(connection_string, '([^/]+)$'), to_char(execution_time, 'yyyy:mm:dd:hh24')
order by to_char(execution_time, 'yyyy:mm:dd:hh24') desc;

create or replace force view p_con_web_show as
select
  a.name as payload_name,
  a.connect_string as connection_string,
  b.exec_end as execution_time,
  b.status
from
  p_con_payload_state a,
  p_con_execution b
where a.obj_id = b.obj_id
order by b.exec_end desc;



/* INDEXES */

create index exectime_ix on cond_log_table (exectime);



/* TRIGGERS */

create or replace trigger p_con_exec_autonumber
before insert on p_con_execution
for each row
begin
  if :new.exec_id < 0 then select p_con_exec_seq.nextval into :new.exec_id from dual;
  end if;
end;
/

create or replace trigger p_con_payload_autonumber
before insert on p_con_execution_payload
for each row
begin
  if :new.exec_id < 0 then select p_con_exec_seq.currval into :new.exec_id from dual;
  end if;
  if :new.pl_id < 0 then select p_con_payload_seq.nextval into :new.pl_id from dual;
  end if;
end;
/

create or replace trigger p_con_state_autonumber
before insert on p_con_payload_state
for each row
begin
  if :new.obj_id <= 0 or :new.obj_id is null then select p_con_state_seq.nextval into :new.obj_id from dual;
  end if;
end;
/



/* SEQUENCES */

create sequence p_con_exec_seq
increment by 1
start with 1
minvalue 1
nocycle order;

create sequence p_con_payload_seq
increment by 1
start with 1
minvalue 1
nocycle order;

create sequence p_con_state_seq
increment by 1
start with 1
minvalue 1
nocycle order;