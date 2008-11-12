/* TABLES */

create table cmd_gen (
  username varchar2(30) constraint cs_cmd_gen_1 not null,
  password varchar2(16) constraint cs_cmd_gen_2 not null,
  xmlcode sys.xmltype,
  constraint pk_cmd_gen primary key (username)
);

grant select on cmd_gen to cms_cond_general with grant option;



/* VIEWS */

create or replace force view cms_cond_logins as 
select username, password
from cmd_gen
order by username;

grant select on cms_cond_logins to cms_cond_general with grant option;

create or replace force view cmd_xml_complete as 
select xmlroot(xmlelement("connectiontype", xmlagg(xmlcode)), version '1.0', standalone yes) as xmlstring 
from cmd_gen;

grant select on cmd_xml_complete to cms_cond_general with grant option;



/* MATERIALIZED VIEWS */

create materialized view cms_cond_accounts
build immediate
refresh force on demand
start with to_date('22-giu-2008 09.00.00', 'dd-mon-yyyy hh24:mi:ss')
with primary key as
select username
from sys.dba_users
where username like 'CMS!_COND!_%' escape '!';

grant select on  cms_cond_accounts to cms_cond_frontier_r with grant option;

grant select on  cms_cond_accounts to cms_cond_general_r with grant option;



/* FUNCTIONS AND PROCEDURES AND PACKAGES */

create or replace function ptr_pwd(length in number) return varchar2 is
  pwd cmd_gen.password%TYPE;
  mylen number;
begin
  if length > 16 then mylen := 16;
  elsif length < 6 then mylen := 6;
  else mylen := floor(length);
  end if;
  loop
    pwd := dbms_random.string('U', 1) || dbms_random.string('X', mylen - 1);
    exit when regexp_instr(pwd, '[[:digit:]]') > 0;
  end loop;
  return pwd;
end;
/

create or replace procedure gen_pwd is
  type my_arr is varray(2) of cmd_gen.password%TYPE;
  new_pwd my_arr;
  j number;
begin
  new_pwd := my_arr(ptr_pwd(16), ptr_pwd(12));
  select count(*) into j from sys.user_views where view_name = 'CMS_COND_LOGINS';
  if j = 0 then
    execute immediate('create view cms_cond_logins as select username, password from cmd_gen order by username');
    execute immediate('grant select on cms_cond_logins to cms_cond_general with grant option');
  end if;
  execute immediate('truncate table cmd_gen');
  insert into cmd_gen select
    username,
    decode(regexp_instr(username, '_R$'), 0, new_pwd(1), new_pwd(2)),
    xmlelement("connection", xmlattributes('oracle://cms_orcoff_int2r/' || regexp_replace(username, '_R$', '') as "name"),
    xmlelement("parameter", xmlattributes('user' as "name", lower(username) as "value")),
    xmlelement("parameter", xmlattributes('password' as "name", decode(regexp_instr(username, '_R$'), 0, new_pwd(1), new_pwd(2)) as "value")))
  from sys.dba_users
  where username like 'CMS!_COND!_%' escape '!';
  dbms_job.submit(j, 'begin sys.update_cms_cond_pwds; end;', trunc(sysdate + 2) + 6/24);
  commit;
end;
/