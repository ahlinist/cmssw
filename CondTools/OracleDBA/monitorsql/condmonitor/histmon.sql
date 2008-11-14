/* TABLES */

create table hs_cond_occupancy_hourly (
  id_snapshot varchar2(2),
  username varchar2(30),
  tablespace varchar2(30),
  occupancy number,
  quota number,
  constraint cs_cond_occupancy_hourly check (id_snapshot is not null and username is not null and tablespace is not null and occupancy is not null and quota is not null),
  constraint pk_cond_occupancy_hourly primary key (id_snapshot, username, tablespace)
);

grant select on hs_cond_occupancy_hourly to cms_cond_history_r with grant option;

grant select on hs_cond_occupancy_hourly to cms_cond_general with grant option;

create table hs_cond_occupancy_daily (
  id_snapshot varchar2(6),
  username varchar2(30),
  tablespace varchar2(30),
  occupancy number,
  quota number,
  constraint cs_cond_occupancy_daily check (id_snapshot is not null and username is not null and tablespace is not null and occupancy is not null and quota is not null),
  constraint pk_cond_occupancy_daily primary key (id_snapshot, username, tablespace)
);

grant select on hs_cond_occupancy_daily to cms_cond_history_r with grant option;

grant select on hs_cond_occupancy_daily to cms_cond_general with grant option;

create table hs_det_details_hourly (
  id_snapshot varchar2(2),
  owner varchar2(30),
  object varchar2(128),
  type varchar2(19),
  occupancy number,
  status varchar2(31),
  tablespace varchar2(30),
  last_modified varchar2(57),
  constraint cs_det_details_hourly check (id_snapshot is not null and owner is not null and object is not null and type is not null and occupancy is not null and status is not null and tablespace is not null and last_modified is not null),
  constraint pk_det_details_hourly primary key (id_snapshot, owner, object, type)
);

grant select on hs_det_details_hourly to cms_cond_history_r with grant option;

grant select on hs_det_details_hourly to cms_cond_general with grant option;

create table hs_det_details_daily (
  id_snapshot varchar2(6),
  owner varchar2(30),
  object varchar2(128),
  type varchar2(19),
  occupancy number,
  status varchar2(31),
  tablespace varchar2(30),
  last_modified varchar2(57),
  constraint cs_det_details_daily check (id_snapshot is not null and owner is not null and object is not null and type is not null and occupancy is not null and status is not null and tablespace is not null and last_modified is not null),
  constraint pk_det_details_daily primary key (id_snapshot, owner, object, type)
);

grant select on hs_det_details_daily to cms_cond_history_r with grant option;

grant select on hs_det_details_daily to cms_cond_general with grant option;

create table hs_server_workload_hourly (
  id_snapshot varchar2(2),
  who varchar2(30),
  what varchar2(1000),
  when date,
  constraint cs_server_workload_hourly check (id_snapshot is not null and who is not null and what is not null and when is not null)
);

grant select on hs_server_workload_hourly to cms_cond_history_r with grant option;

grant select on hs_server_workload_hourly to cms_cond_general with grant option;

create table hs_server_workload_daily (
  id_snapshot varchar2(6),
  who varchar2(30),
  what varchar2(1000),
  when date,
  constraint cs_server_workload_daily check (id_snapshot is not null and who is not null and what is not null and when is not null)
);

grant select on hs_server_workload_daily to cms_cond_history_r with grant option;

grant select on hs_server_workload_daily to cms_cond_general with grant option;



/* VIEWS */

create or replace force view cond_occupancy as 
select
  a.username,
  nvl(b.tablespace_name, '-') as tablespace,
  nvl(b.bytes, 0) as occupancy,
  nvl(c.maxbytes, 0) as quota
from sys.dba_users a, sys.dba_ts_quotas b, sys.dba_data_files c
where a.username = b.username(+)
  and b.tablespace_name = c.tablespace_name(+)
  and a.username like 'CMS!_COND!_%' escape '!'
order by a.username;

create or replace force view det_details as 
select
  a.owner,
  a.object_name as object,
  initcap(a.object_type) as type,
  nvl(b.bytes, 0) as occupancy,
  decode(a.status, 'INVALID', '<span class="ug">Invalid</span>', initcap(a.status)) as status,
  nvl(b.tablespace_name, 'SYSTEM') as tablespace,
  nvl(case
        when sysdate - a.last_ddl_time > 365
        then '<span class="ug">' || to_char(a.last_ddl_time, 'FMDay DD Month YYYY') || to_char(a.last_ddl_time, ' HH24:MI') || '</span>'
        else to_char(a.last_ddl_time, 'FMDay DD Month YYYY') || to_char(a.last_ddl_time, ' HH24:MI')
      end, 'Non determinable') as last_modified
from sys.dba_objects a, sys.dba_segments b
where a.owner = b.owner(+)
  and a.object_name = b.segment_name(+)
  and a.owner like 'CMS!_COND!_%' escape '!'
order by a.owner, a.object_type, a.object_name;

create or replace force view cms_cond_history.server_workload as 
select b.username as who, a.sql_text as what, to_date(a.first_load_time, 'yyyy-mm-dd/hh24:mi:ss') as when 
from gv$sql a, dba_users b 
where a.parsing_user_id = b.user_id and a.command_type != 3
  and b.username like 'CMS!_COND!_%' escape '!'
order by a.first_load_time desc;



/* INDEXES */

create bitmap index bi_cond_occupancy_hourly on hs_cond_occupancy_hourly (id_snapshot);

create bitmap index bi_cond_occupancy_daily on hs_cond_occupancy_daily (id_snapshot);

create bitmap index bi_det_details_hourly on hs_det_details_hourly (id_snapshot);

create bitmap index bi_det_details_daily on hs_det_details_daily (id_snapshot);

create bitmap index bi_server_workload_hourly on hs_server_workload_hourly (id_snapshot);

create bitmap index bi_server_workload_daily on hs_server_workload_daily (id_snapshot);



/* FUNCTIONS AND PROCEDURES AND PACKAGES */

create or replace procedure hist_recorder is
  myn sys.all_users.username%TYPE;
  type my_arr is varray(3) of sys.user_views.view_name%TYPE;
  inv_obj my_arr := my_arr('cond_occupancy', 'det_details', 'server_workload');
  type my_freq is varray(2) of varchar2(6);
  inv_freq my_freq := my_freq('hourly', 'daily');
  i number;
  j number;
  k number;
  strhh varchar2(4000);
  fseq varchar2(4000);
  fsep varchar2(20) := ' is not null and ';
  cursor curr_vw(tbl in varchar2) is select lower(column_name) as cname, lower(column_name || ' ' || data_type || case when data_type like '%VARCHAR%' then '(' || data_length || ')' else '' end) as field from sys.user_tab_columns where table_name = upper(tbl) order by column_id;
  strtt curr_vw%ROWTYPE;
  tbl varchar2(100);
begin
  select lower(user) into myn from dual;
  for i in inv_obj.first..inv_obj.last loop
    for j in inv_freq.first..inv_freq.last loop
      select count(*) into k from sys.user_tables where table_name = upper('hs_' || inv_obj(i) || '_' || inv_freq(j));
      if k = 0 then
        strhh := 'create table ' || myn || '.hs_' || inv_obj(i) || '_' || inv_freq(j) || ' (id_snapshot varchar2(';
        if inv_freq(j) like 'hour%' then
          strhh := strhh || '2), ';
        else
          strhh := strhh || '6), ';
        end if;
        fseq := 'id_snapshot' || fsep;
        for strtt in curr_vw(inv_obj(i))
        loop
          strhh := strhh || strtt.field || ', ';
          fseq := fseq || strtt.cname || fsep;
        end loop;
        strhh := substr(strhh, 1, length(strhh) - 2) || ', constraint cs_' || inv_obj(i) || '_' || inv_freq(j) || ' check (' || substr(fseq, 1, length(fseq) - 5) || '))';
        execute immediate(strhh);
        execute immediate('grant select on ' || myn || '.hs_' || inv_obj(i) || '_' || inv_freq(j) || ' to cms_cond_general, ' || myn || '_r with grant option');
        select count(*) into k from sys.user_indexes where index_name = upper('bi_' || inv_obj(i) || '_' || inv_freq(j));
        if k = 0 then
          execute immediate('create bitmap index ' || myn || '.bi_' || inv_obj(i) || '_' || inv_freq(j) || ' on hs_' || inv_obj(i) || '_' || inv_freq(j) || ' (id_snapshot)');
        end if;
      end if;
      tbl := myn || '.hs_' || inv_obj(i) || '_' || inv_freq(j);
      if inv_freq(j) like 'hour%' then
        execute immediate('delete from ' || tbl || ' where id_snapshot = to_char(sysdate, ''HH24'')');
        execute immediate('insert into ' || tbl || ' select to_char(sysdate, ''HH24'') as id_snapshot, ' || myn || '.' || inv_obj(i) || '.* from ' || myn || '.' || inv_obj(i));
      else
        execute immediate('insert into ' || tbl || ' select to_char(sysdate, ''YYMMDD'') as id_snapshot, ' || myn || '.' || inv_obj(i) || '.* from ' || myn || '.' || inv_obj(i) || ' where not exists (select 1 from ' || tbl || ' where id_snapshot = to_char(sysdate, ''YYMMDD''))');
      end if;
    end loop;
    commit;
  end loop;
end;
/

create or replace procedure pk_add is
begin
  execute immediate('alter table hs_cond_occupancy_hourly add constraint pk_cond_occupancy_hourly primary key (id_snapshot, username, tablespace)');
  execute immediate('alter table hs_cond_occupancy_daily add constraint pk_cond_occupancy_daily primary key (id_snapshot, username, tablespace)');
  execute immediate('alter table hs_det_details_hourly add constraint pk_det_details_hourly primary key (id_snapshot, owner, object, type)');
  execute immediate('alter table hs_det_details_daily add constraint pk_det_details_daily primary key (id_snapshot, owner, object, type)');
end;
/



/* JOBS */

declare
  jobno number;
begin
  dbms_job.submit(:jobno, 'begin hist_recorder; end;', trunc(sysdate) + 20 / 24, 'trunc(sysdate + 1 / 24, ''HH'')');
end;
/