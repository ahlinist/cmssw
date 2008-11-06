/* VIEWS */

create or replace force view expiring_accounts as 
select "USERNAME", "EXPIRY_DATE", "SERVER"
from (
  select username, expiry_date , 'INT2R' as server
  from dba_users
  union select username, expiry_date , 'INT9R' as server
  from dba_users@int9r.cern.ch
  union select username, expiry_date , 'CMSR' as server
  from dba_users@cmsr.cern.ch
)
where expiry_date between sysdate and sysdate + 30
order by expiry_date;



/* FUNCTIONS AND PROCEDURES AND PACKAGES */

create or replace procedure weekly_report is
  c sys.utl_smtp.connection;
  s varchar2(20);
  n varchar2(16);
  d varchar2(32000);
  cursor ea is select * from pietro.expiring_accounts;
  cursor mlcg is select when_utc as time, domain, country, attempts from pietro.mon_log_colours_grp where when_utc between sysdate - 7 and sysdate order by when_utc desc;
begin
  c := sys.utl_smtp.open_connection('mail.cern.ch', 25);
  s := 'Pietro.Picca@cern.ch';
  select instance_name into n from v$instance;
  d := 'From: Oracle Database' || ' ' || n || sys.utl_tcp.crlf ||
       'To: ' || s || sys.utl_tcp.crlf ||
       'Subject: ' || 'Weekly report' || sys.utl_tcp.crlf || sys.utl_tcp.crlf;
  d := d || '*** EXPIRING ACCOUNTS ***' || chr(13) || chr(13) || rpad('USERNAME', 35) || rpad('EXPIRY DATE', 15) || 'SERVER' || chr(13) || lpad(chr(13), 100, '_') || chr(13);
  for t in ea
  loop
    d := d || rpad(t.username, 35) || rpad(to_char(t.expiry_date, 'dd/mm/yyyy'), 15) || t.server || chr(13);
  end loop;
  d := d || chr(13) || chr(13) || '*** MON LOG COLOURS ATTEMPTS ***' || chr(13) || chr(13) || rpad('WHEN', 15) || rpad('DOMAIN', 50) || rpad('COUNTRY', 15) || 'ATTEMPTS' || chr(13) || lpad(chr(13), 100, '_') || chr(13);
  for t in mlcg
  loop
    d := d || rpad(to_char(t.time, 'dd/mm/yyyy'), 15) ||  rpad(t.domain, 50) || rpad(t.country, 15) || to_char(t.attempts) || chr(13);
  end loop;
  sys.utl_smtp.helo(c,'mail.cern.ch');
  sys.utl_smtp.mail(c, s);
  sys.utl_smtp.rcpt(c, s);
  sys.utl_smtp.data(c, d);
  sys.utl_smtp.quit(c);
end;
/



/* JOBS */

declare
  jobno number;
begin
  dbms_job.submit(:jobno, 'begin weekly_report; end;', trunc(sysdate) + 7 / 24, 'next_day(trunc(sysdate), ''TUESDAY'') + 7 / 24');
end;
/