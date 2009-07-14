
CONNECT CMS_ES_COMIS@cms_omds_lb/es164644

create table ES_FED_DEF(
        DEF_ID NUMBER NOT NULL
        , FED_KEY VARCHAR2(16) NOT NULL
        , FED_ID  NUMBER NOT NULL
);

ALTER TABLE ES_FED_DEF ADD CONSTRAINT es_fed_def_pk PRIMARY KEY (def_id);
ALTER TABLE ES_FED_DEF ADD CONSTRAINT es_fed_def_uk1 UNIQUE (fed_key);
ALTER TABLE ES_FED_DEF ADD CONSTRAINT es_fed_def_uk2 UNIQUE (fed_id);

CREATE SEQUENCE es_fed_def_sq INCREMENT BY 1 START WITH 1;
CREATE trigger es_fed_def_trg
before insert on ES_FED_DEF
for each row
begin
select es_fed_def_sq.NextVal into :new.def_id from dual;
end;
/

insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED534',534);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED535',535);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED537',537);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED539',539);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED540',540);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED541',541);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED542',542);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED545',545);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED546',546);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED547',547);

insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED520',520);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED522',522);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED523',523);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED524',524);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED525',525);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED528',528);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED529',529);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED530',530);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED531',531);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED532',532);

insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED548',548);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED549',549);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED551',551);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED553',553);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED554',554);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED555',555);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED556',556);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED557',557);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED560',560);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED561',561);

insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED563',563);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED564',564);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED565',565);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED566',566);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED568',568);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED570',570);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED571',571);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED572',572);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED573',573);
insert into ES_FED_DEF(FED_KEY,FED_ID) values('FED574',574);


