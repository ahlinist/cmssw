/* TABLES */

create table audit_test (
  id number constraint cs_audit_test_0 not null,
  first number constraint cs_audit_test_1 not null,
  second number constraint cs_audit_test_2 not null,
  third number constraint cs_audit_test_3 not null,
  fourth number constraint cs_audit_test_4 not null,
  fifth number constraint cs_audit_test_5 not null,
  sixth number constraint cs_audit_test_6 not null,
  seventh number constraint cs_audit_test_7 not null,
  eighth number constraint cs_audit_test_8 not null,
  ninth nvarchar2(12) constraint cs_audit_test_9 not null,
  tenth nvarchar2(12) constraint cs_audit_test_10 not null,
  constraint pk_audit_test primary key (id)
)



/* FUNCTIONS AND PROCEDURES AND PACKAGES */

create or replace procedure audit_pop is
begin
  dbms_workload_repository.create_snapshot;
  execute immediate('truncate table audit_test');
  for j in 1..10000 loop
    for i in 1..100 loop
      insert into audit_test values(
        audit_seq.nextval,
        dbms_random.random,
        dbms_random.random,
        dbms_random.random,
        dbms_random.random,
        dbms_random.random,
        dbms_random.random,
        dbms_random.random,
        dbms_random.random,
        dbms_random.string('l', 12),
        dbms_random.string('l', 12)
      );
      update audit_test set
        first = dbms_random.random,
        second = dbms_random.random,
        third = dbms_random.random,
        fourth = dbms_random.random,
        fifth = dbms_random.random,
        sixth = dbms_random.random,
        seventh = dbms_random.random,
        eighth = dbms_random.random,
        ninth = dbms_random.string('l', 12),
        tenth = dbms_random.string('l', 12)
      where id = (select max(id) from audit_test);
    end loop;
    commit;
  end loop;
  dbms_workload_repository.create_snapshot;
end;
/



/* SEQUENCES */

create sequence audit_seq
increment by 1
start with 1
minvalue 1
nocycle order;