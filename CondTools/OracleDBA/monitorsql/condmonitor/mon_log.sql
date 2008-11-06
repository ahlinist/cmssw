/* TABLES */

create table mon_log (
  id number constraint cs_mon_log_1 not null,
  domain varchar2(500) constraint cs_mon_log_2 not null,
  ip varchar2(15) constraint cs_mon_log_3 not null,
  browser varchar2(200) constraint cs_mon_log_4 not null,
  when date default sysdate constraint cs_mon_log_5 not null,
  kind varchar2(1) default 'L' constraint cs_mon_log_6 not null,
  country varchar2(50) constraint cs_mon_log_7 not null,
  constraint pk_mon_log primary key (id),
  constraint fk_mon_log foreign key (kind) references mon_log_desc (kind) on delete cascade
);

create table mon_log_desc (
  kind varchar2(1) constraint cs_mon_log_desc_1 not null,
  kind_desc varchar2(40) constraint cs_mon_log_desc_2 not null,
  constraint pk_mon_log_desc primary key (kind)
);

create table mon_log_colours (
  id number constraint cs_mon_log_colours_id not null,
  attempts number constraint cs_mon_log_colours_attempts not null,
  query_string varchar2(80) default 'Not available' constraint cs_mon_log_colours_qs not null,
  constraint pk_mon_log_colours primary key (id),
  constraint fk_mon_log_colours foreign key (id) references mon_log (id) on delete cascade
);



/* INDEXES */

create bitmap index bi_mon_log on mon_log (kind);



/* VIEWS */

create or replace force view mon_log_complete as 
select a.id, a.domain, a.ip, a.country, a.browser, a.when, b.kind_desc 
from mon_log a, mon_log_desc b 
where a.kind = b.kind
  and a.browser like 'Mozilla%' and a.browser not like '%http://%'
order by a.id desc;



/* FUNCTIONS AND PROCEDURES AND PACKAGES */

create or replace package glob_pkg as
  flag boolean := FALSE;
end;
/



/* TRIGGERS */

create or replace trigger trg_autonumber
before insert on mon_log for each row
begin
  select std_seq.nextval into :new.id from dual;
end;
/

create or replace trigger mon_log_upd_cascade
after update of id on mon_log for each row
begin
  glob_pkg.flag := TRUE;
  update mon_log_colours set id = :new.id where id = :old.id;
  glob_pkg.flag := FALSE;
end;
/

create or replace trigger mon_log_colours_chk
before insert or update
on mon_log_colours 
referencing new as new old as old
for each row
declare
  k pietro.mon_log.kind%TYPE;
  s pietro.mon_log_desc.kind_desc%TYPE;
  q pietro.mon_log_colours.query_string%TYPE;
begin
  if glob_pkg.flag = FALSE then
    select m.kind into k from pietro.mon_log m where :new.id = m.id;
    select d.kind_desc into s from pietro.mon_log_desc d where d.kind = k;
    if k != 'G' then raise_application_error(-20555, 'The key ' || to_char(:new.id) || ' pertains to ''' || s || ''' and not to a graph!');
    end if;
  end if;
  select data_default into q from sys.user_tab_columns where table_name = 'MON_LOG_COLOURS' and column_name = 'QUERY_STRING';
  if trim(:new.query_string) is null then :new.query_string := regexp_substr(q, '[^[:punct:]]+');
  end if;
end;
/



/* SEQUENCES */

create sequence std_seq
increment by 1
start with 1
minvalue 1
nocycle order;