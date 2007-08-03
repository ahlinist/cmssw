create or replace procedure update_pwd is
   cursor cms_cond_cur(cond_str in varchar2) is select username from sys.dba_users where username like cond_str || '%' order by username;
   cms_cond_str cms_cond_cur%ROWTYPE;
   db_name v$database.name%TYPE;
   my_user sys.dba_users.username%TYPE;
   type my_arr is varray(3) of pietro.cmd_gen.password%TYPE;
   new_pwd my_arr;
   upd_statement pietro.cmd_gen.statement%TYPE;
begin
   new_pwd := my_arr(dbms_random.string('X', 12), dbms_random.string('U', 12), '');
   select lower(name) into db_name from v$database;
   execute immediate 'truncate table pietro.cmd_gen';
   open cms_cond_cur ('CMS_COND_');
     loop
       fetch cms_cond_cur into cms_cond_str.username;
       exit when cms_cond_cur%NOTFOUND;
       my_user := lower(cms_cond_str.username);
       new_pwd(3) := new_pwd(2 - sign(length(my_user) - instr(my_user, '_r', -1) - 1));
       upd_statement := 'alter user ' || my_user || ' identified by ' ||  new_pwd(3);
       insert into pietro.cmd_gen values(
         cms_cond_str.username,
         new_pwd(3),
         xmlelement("connection", xmlattributes('oracle://' || db_name || '/' || my_user as "name"),
         xmlelement("parameter", xmlattributes('user' as "name", my_user as "value")),
         xmlelement("parameter", xmlattributes('password' as "name", new_pwd(3) as "value"))),
         upd_statement
       );
       execute immediate(upd_statement);
       commit;
     end loop;
   close cms_cond_cur;
end;
/
