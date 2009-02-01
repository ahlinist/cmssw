--------------------------------------------------------
--  File created - Sunday-February-01-2009   
--------------------------------------------------------
--------------------------------------------------------
--  DDL for Sequence RR_RUN_ID_SQ
--------------------------------------------------------

   CREATE SEQUENCE  "RR_RUN_ID_SQ"  MINVALUE 1 MAXVALUE 999999999999999999999999999 INCREMENT BY 1 START WITH 921 CACHE 20 NOORDER  NOCYCLE ;
--------------------------------------------------------
--  DDL for Table RR_L1SOURCES
--------------------------------------------------------

  CREATE TABLE "RR_L1SOURCES" 
   (	"L1S_ABBR" VARCHAR2(10), 
	"L1S_NAME" VARCHAR2(100)
   ) ;
  GRANT SELECT ON "RR_L1SOURCES" TO PUBLIC;
 
  GRANT SELECT ON "RR_L1SOURCES" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for Table RR_RUNINFO_CACHE
--------------------------------------------------------

  CREATE TABLE "RR_RUNINFO_CACHE" 
   (	"RUN_NUMBER" NUMBER, 
	"RUN_EVENTS" NUMBER, 
	"RUN_START_TIME" TIMESTAMP (6), 
	"RUN_END_TIME" TIMESTAMP (6), 
	"RUN_RATE" NUMBER, 
	"RUN_L1KEY" VARCHAR2(4000), 
	"RUN_HLTKEY" VARCHAR2(4000), 
	"CSC" VARCHAR2(4), 
	"DT" VARCHAR2(4), 
	"ECAL" VARCHAR2(4), 
	"HCAL" VARCHAR2(4), 
	"TRG" VARCHAR2(4), 
	"PIX" VARCHAR2(4), 
	"RPC" VARCHAR2(4), 
	"SIST" VARCHAR2(4), 
	"SCAL" VARCHAR2(4), 
	"LAST_UPDATE" DATE
   ) ;
  GRANT SELECT ON "RR_RUNINFO_CACHE" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for Table RR_RUNS
--------------------------------------------------------

  CREATE TABLE "RR_RUNS" 
   (	"RUN_ID" NUMBER, 
	"RUN_NUMBER" NUMBER(10,0), 
	"RUN_STATUS" VARCHAR2(10) DEFAULT 'ONLINE', 
	"RUN_CREATE_TIME" DATE, 
	"RUN_GLOBALNAME" VARCHAR2(20), 
	"RUN_ONLINE_SHIFTER" VARCHAR2(100), 
	"RUN_OFFLINE_SHIFTER" VARCHAR2(100), 
	"RUN_EVENTS" NUMBER(10,0), 
	"RUN_TAG" VARCHAR2(100), 
	"RUN_RATE" NUMBER(10,2), 
	"RUN_START_TIME" DATE, 
	"RUN_END_TIME" DATE, 
	"RUN_BFIELD" NUMBER(6,5), 
	"RUN_STOP_REASON" VARCHAR2(1000), 
	"RUN_L1KEY" VARCHAR2(200), 
	"RUN_HLTKEY" VARCHAR2(200), 
	"RUN_ONLINE_COMMENT" VARCHAR2(1000), 
	"RUN_OFFLINE_COMMENT" VARCHAR2(1000), 
	"RUN_BFIELD_COMMENT" VARCHAR2(1000)
   ) ;
  GRANT DELETE ON "RR_RUNS" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT INSERT ON "RR_RUNS" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT SELECT ON "RR_RUNS" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT UPDATE ON "RR_RUNS" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for Table RR_RUNS$A
--------------------------------------------------------

  CREATE TABLE "RR_RUNS$A" 
   (	"RUN_ID" NUMBER, 
	"RUN_NUMBER" NUMBER(10,0), 
	"RUN_STATUS" VARCHAR2(10) DEFAULT 'ONLINE', 
	"RUN_CREATE_TIME" DATE, 
	"RUN_GLOBALNAME" VARCHAR2(20), 
	"RUN_ONLINE_SHIFTER" VARCHAR2(100), 
	"RUN_OFFLINE_SHIFTER" VARCHAR2(100), 
	"RUN_EVENTS" NUMBER(10,0), 
	"RUN_TAG" VARCHAR2(100), 
	"RUN_RATE" NUMBER(10,2), 
	"RUN_START_TIME" DATE, 
	"RUN_END_TIME" DATE, 
	"RUN_BFIELD" NUMBER(6,5), 
	"RUN_STOP_REASON" VARCHAR2(1000), 
	"RUN_L1KEY" VARCHAR2(200), 
	"RUN_HLTKEY" VARCHAR2(200), 
	"RUN_ONLINE_COMMENT" VARCHAR2(1000), 
	"RUN_OFFLINE_COMMENT" VARCHAR2(1000), 
	"RUN_BFIELD_COMMENT" VARCHAR2(1000), 
	"RUN_ACTION_DATE" DATE, 
	"RUN_ACTION_TYPE" VARCHAR2(1)
   ) ;
  GRANT INSERT ON "RR_RUNS$A" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT SELECT ON "RR_RUNS$A" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for Table RR_RUN_L1SOURCES
--------------------------------------------------------

  CREATE TABLE "RR_RUN_L1SOURCES" 
   (	"RL1_L1S_ABBR" VARCHAR2(10), 
	"RL1_COMMENT" VARCHAR2(1000), 
	"RL1_RUN_ID" NUMBER
   ) ;
  GRANT DELETE ON "RR_RUN_L1SOURCES" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT INSERT ON "RR_RUN_L1SOURCES" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT SELECT ON "RR_RUN_L1SOURCES" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT UPDATE ON "RR_RUN_L1SOURCES" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for Table RR_RUN_L1SOURCES$A
--------------------------------------------------------

  CREATE TABLE "RR_RUN_L1SOURCES$A" 
   (	"RL1_L1S_ABBR" VARCHAR2(10), 
	"RL1_COMMENT" VARCHAR2(1000), 
	"RL1_RUN_ID" NUMBER, 
	"RL1_ACTION_DATE" DATE, 
	"RL1_ACTION_TYPE" VARCHAR2(1)
   ) ;
  GRANT INSERT ON "RR_RUN_L1SOURCES$A" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT SELECT ON "RR_RUN_L1SOURCES$A" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for Table RR_RUN_SUBSYSTEMS
--------------------------------------------------------

  CREATE TABLE "RR_RUN_SUBSYSTEMS" 
   (	"RSU_SUB_ABBR" VARCHAR2(10), 
	"RSU_COMMENT" VARCHAR2(1000), 
	"RSU_VALUE" VARCHAR2(10) DEFAULT 'GOOD', 
	"RSU_RUN_ID" NUMBER
   ) ;
  GRANT DELETE ON "RR_RUN_SUBSYSTEMS" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT INSERT ON "RR_RUN_SUBSYSTEMS" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT SELECT ON "RR_RUN_SUBSYSTEMS" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT UPDATE ON "RR_RUN_SUBSYSTEMS" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for Table RR_RUN_SUBSYSTEMS$A
--------------------------------------------------------

  CREATE TABLE "RR_RUN_SUBSYSTEMS$A" 
   (	"RSU_SUB_ABBR" VARCHAR2(10), 
	"RSU_COMMENT" VARCHAR2(1000), 
	"RSU_VALUE" VARCHAR2(10) DEFAULT 'GOOD', 
	"RSU_RUN_ID" NUMBER, 
	"RSU_ACTION_DATE" DATE, 
	"RSU_ACTION_TYPE" VARCHAR2(1)
   ) ;
  GRANT INSERT ON "RR_RUN_SUBSYSTEMS$A" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT SELECT ON "RR_RUN_SUBSYSTEMS$A" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for Table RR_SUBSYSTEMS
--------------------------------------------------------

  CREATE TABLE "RR_SUBSYSTEMS" 
   (	"SUB_ABBR" VARCHAR2(10), 
	"SUB_NAME" VARCHAR2(100), 
	"SUB_DBS_FLAG" VARCHAR2(50), 
	"SUB_TYPE" VARCHAR2(10) DEFAULT 'ONLINE', 
	"SUB_RUNSESSION_PARAMETER_NAME" VARCHAR2(50)
   ) ;
  GRANT SELECT ON "RR_SUBSYSTEMS" TO PUBLIC;
 
  GRANT SELECT ON "RR_SUBSYSTEMS" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  Constraints for Table RR_L1SOURCES
--------------------------------------------------------

  ALTER TABLE "RR_L1SOURCES" ADD CONSTRAINT "RR_L1SOURCES_PK" PRIMARY KEY ("L1S_ABBR") ENABLE;
 
  ALTER TABLE "RR_L1SOURCES" MODIFY ("L1S_ABBR" NOT NULL ENABLE);
  GRANT SELECT ON "RR_L1SOURCES" TO PUBLIC;
 
  GRANT SELECT ON "RR_L1SOURCES" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  Constraints for Table RR_RUNINFO_CACHE
--------------------------------------------------------

  ALTER TABLE "RR_RUNINFO_CACHE" ADD CONSTRAINT "CMS_RUNINFO_CACHE_CHK1" CHECK (
(CSC='GOOD' or CSC IS NULL) and (DT='GOOD' or DT IS NULL) and (ECAL='GOOD' or ECAL IS NULL) and (HCAL='GOOD' or HCAL IS NULL) and (TRG='GOOD' or TRG IS NULL) and (PIX='GOOD' or PIX IS NULL) and (RPC='GOOD' or RPC IS NULL) and (SIST='GOOD' or SIST IS NULL) and (SCAL='GOOD' or SCAL IS NULL)
) ENABLE;
 
  ALTER TABLE "RR_RUNINFO_CACHE" ADD CONSTRAINT "CMS_RUNINFO_CACHE_PK" PRIMARY KEY ("RUN_NUMBER") ENABLE;
 
  ALTER TABLE "RR_RUNINFO_CACHE" MODIFY ("RUN_NUMBER" NOT NULL ENABLE);
  GRANT SELECT ON "RR_RUNINFO_CACHE" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  Constraints for Table RR_RUNS
--------------------------------------------------------

  ALTER TABLE "RR_RUNS" ADD CONSTRAINT "RR_RUNS_CHK01" CHECK (
RUN_STATUS IN ('ONLINE', 'OFFLINE', 'SIGNOFF', 'COMPLETED')
) ENABLE;
 
  ALTER TABLE "RR_RUNS" ADD CONSTRAINT "RR_RUNS_CHK02" CHECK (
(RUN_TAG IS NOT NULL and RUN_STATUS = 'COMPLETED') OR (RUN_STATUS <> 'COMPLETED' AND RUN_TAG IS NULL)
) ENABLE;
 
  ALTER TABLE "RR_RUNS" ADD CONSTRAINT "RR_RUNS_PK" PRIMARY KEY ("RUN_ID") ENABLE;
 
  ALTER TABLE "RR_RUNS" ADD CONSTRAINT "RR_RUNS_UK1" UNIQUE ("RUN_NUMBER", "RUN_TAG") ENABLE;
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_ID" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_NUMBER" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_STATUS" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_CREATE_TIME" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_GLOBALNAME" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_ONLINE_SHIFTER" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_EVENTS" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_RATE" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_START_TIME" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_L1KEY" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_HLTKEY" NOT NULL ENABLE);
  GRANT DELETE ON "RR_RUNS" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT INSERT ON "RR_RUNS" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT SELECT ON "RR_RUNS" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT UPDATE ON "RR_RUNS" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  Constraints for Table RR_RUN_L1SOURCES
--------------------------------------------------------

  ALTER TABLE "RR_RUN_L1SOURCES" ADD CONSTRAINT "RR_RUN_L1SOURCES_PK" PRIMARY KEY ("RL1_RUN_ID", "RL1_L1S_ABBR") ENABLE;
 
  ALTER TABLE "RR_RUN_L1SOURCES" MODIFY ("RL1_L1S_ABBR" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUN_L1SOURCES" MODIFY ("RL1_RUN_ID" NOT NULL ENABLE);
  GRANT DELETE ON "RR_RUN_L1SOURCES" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT INSERT ON "RR_RUN_L1SOURCES" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT SELECT ON "RR_RUN_L1SOURCES" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT UPDATE ON "RR_RUN_L1SOURCES" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  Constraints for Table RR_RUN_SUBSYSTEMS
--------------------------------------------------------

  ALTER TABLE "RR_RUN_SUBSYSTEMS" ADD CONSTRAINT "RR_RUN_SUBSYSTEMS_CHK1" CHECK (
RSU_VALUE IN ('GOOD', 'BAD', 'NOTSET', 'EXCL')
) ENABLE;
 
  ALTER TABLE "RR_RUN_SUBSYSTEMS" ADD CONSTRAINT "RR_RUN_SUBSYSTEMS_PK" PRIMARY KEY ("RSU_RUN_ID", "RSU_SUB_ABBR") ENABLE;
 
  ALTER TABLE "RR_RUN_SUBSYSTEMS" MODIFY ("RSU_SUB_ABBR" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUN_SUBSYSTEMS" MODIFY ("RSU_VALUE" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUN_SUBSYSTEMS" MODIFY ("RSU_RUN_ID" NOT NULL ENABLE);
  GRANT DELETE ON "RR_RUN_SUBSYSTEMS" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT INSERT ON "RR_RUN_SUBSYSTEMS" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT SELECT ON "RR_RUN_SUBSYSTEMS" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT UPDATE ON "RR_RUN_SUBSYSTEMS" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  Constraints for Table RR_SUBSYSTEMS
--------------------------------------------------------

  ALTER TABLE "RR_SUBSYSTEMS" ADD CONSTRAINT "RR_SUBSYSTEMS_CHK1" CHECK (
SUB_TYPE IN ('ONLINE', 'OFFLINE')
) ENABLE;
 
  ALTER TABLE "RR_SUBSYSTEMS" ADD CONSTRAINT "RR_SUBSYSTEMS_PK" PRIMARY KEY ("SUB_ABBR") ENABLE;
 
  ALTER TABLE "RR_SUBSYSTEMS" MODIFY ("SUB_ABBR" NOT NULL ENABLE);
 
  ALTER TABLE "RR_SUBSYSTEMS" MODIFY ("SUB_NAME" NOT NULL ENABLE);
 
  ALTER TABLE "RR_SUBSYSTEMS" MODIFY ("SUB_TYPE" NOT NULL ENABLE);
  GRANT SELECT ON "RR_SUBSYSTEMS" TO PUBLIC;
 
  GRANT SELECT ON "RR_SUBSYSTEMS" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for Index CMS_RUNINFO_CACHE_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "CMS_RUNINFO_CACHE_PK" ON "RR_RUNINFO_CACHE" ("RUN_NUMBER") 
  ;
--------------------------------------------------------
--  DDL for Index RR_L1SOURCES_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "RR_L1SOURCES_PK" ON "RR_L1SOURCES" ("L1S_ABBR") 
  ;
--------------------------------------------------------
--  DDL for Index RR_RUNS_INDEX1
--------------------------------------------------------

  CREATE INDEX "RR_RUNS_INDEX1" ON "RR_RUNS" ("RUN_NUMBER") 
  ;
--------------------------------------------------------
--  DDL for Index RR_RUNS_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "RR_RUNS_PK" ON "RR_RUNS" ("RUN_ID") 
  ;
--------------------------------------------------------
--  DDL for Index RR_RUNS_UK1
--------------------------------------------------------

  CREATE UNIQUE INDEX "RR_RUNS_UK1" ON "RR_RUNS" ("RUN_NUMBER", "RUN_TAG") 
  ;
--------------------------------------------------------
--  DDL for Index RR_RUN_L1SOURCES_INDEX1
--------------------------------------------------------

  CREATE INDEX "RR_RUN_L1SOURCES_INDEX1" ON "RR_RUN_L1SOURCES" ("RL1_L1S_ABBR") 
  ;
--------------------------------------------------------
--  DDL for Index RR_RUN_L1SOURCES_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "RR_RUN_L1SOURCES_PK" ON "RR_RUN_L1SOURCES" ("RL1_RUN_ID", "RL1_L1S_ABBR") 
  ;
--------------------------------------------------------
--  DDL for Index RR_RUN_SUBSYSTEMS_INDEX1
--------------------------------------------------------

  CREATE INDEX "RR_RUN_SUBSYSTEMS_INDEX1" ON "RR_RUN_SUBSYSTEMS" ("RSU_SUB_ABBR") 
  ;
--------------------------------------------------------
--  DDL for Index RR_RUN_SUBSYSTEMS_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "RR_RUN_SUBSYSTEMS_PK" ON "RR_RUN_SUBSYSTEMS" ("RSU_RUN_ID", "RSU_SUB_ABBR") 
  ;
--------------------------------------------------------
--  DDL for Index RR_SUBSYSTEMS_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "RR_SUBSYSTEMS_PK" ON "RR_SUBSYSTEMS" ("SUB_ABBR") 
  ;
--------------------------------------------------------
--  Ref Constraints for Table RR_RUN_L1SOURCES
--------------------------------------------------------

  ALTER TABLE "RR_RUN_L1SOURCES" ADD CONSTRAINT "RR_RUN_L1SOURCES_RR_L1SOU_FK1" FOREIGN KEY ("RL1_L1S_ABBR")
	  REFERENCES "RR_L1SOURCES" ("L1S_ABBR") ENABLE;
 
  ALTER TABLE "RR_RUN_L1SOURCES" ADD CONSTRAINT "RR_RUN_L1SOURCES_RR_RUNS_FK1" FOREIGN KEY ("RL1_RUN_ID")
	  REFERENCES "RR_RUNS" ("RUN_ID") ENABLE;
  GRANT DELETE ON "RR_RUN_L1SOURCES" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT INSERT ON "RR_RUN_L1SOURCES" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT SELECT ON "RR_RUN_L1SOURCES" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT UPDATE ON "RR_RUN_L1SOURCES" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  Ref Constraints for Table RR_RUN_SUBSYSTEMS
--------------------------------------------------------

  ALTER TABLE "RR_RUN_SUBSYSTEMS" ADD CONSTRAINT "RR_RUN_SUBSYSTEMS_RR_RUNS_FK1" FOREIGN KEY ("RSU_RUN_ID")
	  REFERENCES "RR_RUNS" ("RUN_ID") ENABLE;
 
  ALTER TABLE "RR_RUN_SUBSYSTEMS" ADD CONSTRAINT "RR_RUN_SUBSYSTEMS_RR_SUBS_FK1" FOREIGN KEY ("RSU_SUB_ABBR")
	  REFERENCES "RR_SUBSYSTEMS" ("SUB_ABBR") ENABLE;
  GRANT DELETE ON "RR_RUN_SUBSYSTEMS" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT INSERT ON "RR_RUN_SUBSYSTEMS" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT SELECT ON "RR_RUN_SUBSYSTEMS" TO "CMS_DQM_RUN_REGISTRY_W";
 
  GRANT UPDATE ON "RR_RUN_SUBSYSTEMS" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for Trigger RR_TRG_RUNINFO_CACHE_DATE
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "RR_TRG_RUNINFO_CACHE_DATE" 
BEFORE INSERT OR UPDATE ON RR_RUNINFO_CACHE
FOR EACH ROW
BEGIN
  select sysdate into :new.last_update from dual;
END;
/
ALTER TRIGGER "RR_TRG_RUNINFO_CACHE_DATE" DISABLE;
--------------------------------------------------------
--  DDL for Trigger RR_TRG_RUNS$A
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "RR_TRG_RUNS$A" 
AFTER INSERT OR DELETE OR UPDATE ON RR_RUNS
REFERENCING OLD AS old NEW AS new 
FOR EACH ROW 
DECLARE
  l_action varchar2(1);
BEGIN

  if INSERTING then
    l_action := 'I';
  end if;
  
  if UPDATING then
    l_action := 'U';
  end if;

  if DELETING then
    l_action := 'D';
    INSERT INTO RR_RUNS$A VALUES (
      :old.RUN_ID ,
      :old.RUN_NUMBER ,
      :old.RUN_STATUS ,
      :old.RUN_CREATE_TIME ,
      :old.RUN_GLOBALNAME ,
      :old.RUN_ONLINE_SHIFTER ,
      :old.RUN_OFFLINE_SHIFTER ,
      :old.RUN_EVENTS ,
      :old.RUN_TAG ,
      :old.RUN_RATE ,
      :old.RUN_START_TIME ,
      :old.RUN_END_TIME ,
      :old.RUN_BFIELD ,
      :old.RUN_STOP_REASON ,
      :old.RUN_L1KEY ,
      :old.RUN_HLTKEY ,
      :old.RUN_ONLINE_COMMENT ,
      :old.RUN_OFFLINE_COMMENT ,
      :old.RUN_BFIELD_COMMENT ,
      sysdate,
      l_action);
  else
    INSERT INTO RR_RUNS$A VALUES (
      :new.RUN_ID ,
      :new.RUN_NUMBER ,
      :new.RUN_STATUS ,
      :new.RUN_CREATE_TIME ,
      :new.RUN_GLOBALNAME ,
      :new.RUN_ONLINE_SHIFTER ,
      :new.RUN_OFFLINE_SHIFTER ,
      :new.RUN_EVENTS ,
      :new.RUN_TAG ,
      :new.RUN_RATE ,
      :new.RUN_START_TIME ,
      :new.RUN_END_TIME ,
      :new.RUN_BFIELD ,
      :new.RUN_STOP_REASON ,
      :new.RUN_L1KEY ,
      :new.RUN_HLTKEY ,
      :new.RUN_ONLINE_COMMENT ,
      :new.RUN_OFFLINE_COMMENT ,
      :new.RUN_BFIELD_COMMENT ,
      sysdate,
      l_action);
  end if;
  
END;


/
ALTER TRIGGER "RR_TRG_RUNS$A" ENABLE;
--------------------------------------------------------
--  DDL for Trigger RR_TRG_RUN_L1SOURCES$A
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "RR_TRG_RUN_L1SOURCES$A" 
AFTER INSERT OR DELETE OR UPDATE ON RR_RUN_L1SOURCES
REFERENCING OLD AS old NEW AS new 
FOR EACH ROW 
DECLARE
  l_action varchar2(1);
BEGIN

  if INSERTING then
    l_action := 'I';
  end if;
  
  if UPDATING then
    l_action := 'U';
  end if;

  if DELETING then
    l_action := 'D';
    INSERT INTO RR_RUN_L1SOURCES$A VALUES (
      :old.RL1_L1S_ABBR,
      :old.RL1_COMMENT,
      :old.RL1_RUN_ID,
      sysdate, 
      l_action);
  else
    INSERT INTO RR_RUN_L1SOURCES$A VALUES (
      :new.RL1_L1S_ABBR,
      :new.RL1_COMMENT,
      :new.RL1_RUN_ID,
      sysdate, 
      l_action);
  end if;
  
END;
/
ALTER TRIGGER "RR_TRG_RUN_L1SOURCES$A" ENABLE;
--------------------------------------------------------
--  DDL for Trigger RR_TRG_RUN_SUBSYSTEMS$A
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "RR_TRG_RUN_SUBSYSTEMS$A" 
AFTER INSERT OR DELETE OR UPDATE ON RR_RUN_SUBSYSTEMS
REFERENCING OLD AS old NEW AS new 
FOR EACH ROW 
DECLARE
  l_action varchar2(1);
BEGIN

  if INSERTING then
    l_action := 'I';
  end if;
  
  if UPDATING then
    l_action := 'U';
  end if;

  if DELETING then
    l_action := 'D';
    INSERT INTO RR_RUN_SUBSYSTEMS$A VALUES (
      :old.RSU_SUB_ABBR,
      :old.RSU_COMMENT,
      :old.RSU_VALUE,
      :old.RSU_RUN_ID,
      sysdate,
      l_action
    );
  else
    INSERT INTO RR_RUN_SUBSYSTEMS$A VALUES (
      :new.RSU_SUB_ABBR,
      :new.RSU_COMMENT,
      :new.RSU_VALUE,
      :new.RSU_RUN_ID,
      sysdate,
      l_action
    );
  end if;
  
END;
/
ALTER TRIGGER "RR_TRG_RUN_SUBSYSTEMS$A" ENABLE;
--------------------------------------------------------
--  DDL for View RI_RUN_DATA
--------------------------------------------------------

  CREATE OR REPLACE VIEW "RI_RUN_DATA" ("RUN_ID", "RUN_NUMBER", "RUN_GLOBALNAME", "RUN_STATUS", "RUN_BFIELD", "RUN_BFIELD_COMMENT", "RUN_STOP_REASON", "RUN_ONLINE_COMMENT", "RUN_OFFLINE_COMMENT", "RUN_CREATE_TIME", "RUN_ONLINE_SHIFTER", "RUN_OFFLINE_SHIFTER", "RUN_L1SOURCES", "RUN_TAG", "RUN_COUNT_TAGS", "RUN_EVENTS", "RUN_START_TIME", "RUN_END_TIME", "RUN_RATE", "RUN_L1KEY", "RUN_HLTKEY", "CSC", "DT", "ECAL", "HCAL", "TRG", "PIX", "RPC", "SIST", "SCAL", "JMET", "EGAM", "MUON", "CSC_comment", "DT_comment", "ECAL_comment", "HCAL_comment", "TRG_comment", "PIX_comment", "RPC_comment", "SIST_comment", "JMET_comment", "EGAM_comment", "MUON_comment", "SCAL_comment", "l1s_CSC", "l1s_DT", "l1s_ECAL", "l1s_HCAL", "l1s_Random", "l1s_RPC", "l1s_Other", "l1s_CSC_comment", "l1s_DT_comment", "l1s_ECAL_comment", "l1s_HCAL_comment", "l1s_Random_comment", "l1s_RPC_comment", "l1s_Other_comment") AS 
  select 
    b.RUN_ID,
    a.RUN_NUMBER,
    b.RUN_GLOBALNAME,
    b.RUN_STATUS,
    b.RUN_BFIELD,
    b.run_bfield_comment,
    b.RUN_STOP_REASON,
    b.RUN_ONLINE_COMMENT,
    b.RUN_OFFLINE_COMMENT,
    b.RUN_CREATE_TIME, 
    b.RUN_ONLINE_SHIFTER,
    b.RUN_OFFLINE_SHIFTER,
    b.RUN_L1SOURCES,
    b.RUN_TAG,
    b.run_count_tags,
    a.RUN_EVENTS as RUN_EVENTS,
    TO_CHAR(a.RUN_START_TIME, 'YYYY.MM.DD HH24:MI:SS') as RUN_START_TIME, 
    TO_CHAR(a.RUN_END_TIME, 'YYYY.MM.DD HH24:MI:SS') as RUN_END_TIME, 
    DECODE(b.RUN_NUMBER, null, a.RUN_RATE, b.RUN_RATE) as RUN_RATE,
    DECODE(b.RUN_NUMBER, null, a.RUN_L1KEY, b.RUN_L1KEY) as RUN_L1KEY,
    DECODE(b.RUN_NUMBER, null, a.RUN_HLTKEY, b.RUN_HLTKEY) as RUN_HLTKEY,
    DECODE(b.RUN_NUMBER, null, a.CSC, b.CSC) as CSC,
    DECODE(b.RUN_NUMBER, null, a.DT, b.DT) as DT,
    DECODE(b.RUN_NUMBER, null, a.ECAL, b.ECAL) as ECAL,
  DECODE(b.RUN_NUMBER, null, a.HCAL, b.HCAL) as HCAL,
  DECODE(b.RUN_NUMBER, null, a.TRG, b.TRG) as TRG,
  DECODE(b.RUN_NUMBER, null, a.PIX, b.PIX) as PIX,
  DECODE(b.RUN_NUMBER, null, a.RPC, b.RPC) as RPC,
  DECODE(b.RUN_NUMBER, null, a.SIST, b.SIST) as SIST,
  DECODE(b.RUN_NUMBER, null, a.SCAL, b.SCAL) as SCAL,
  b.JMET,
  b.EGAM,
  b.MUON,
  b."CSC_comment",
  b."DT_comment",
  b."ECAL_comment",
  b."HCAL_comment",
  b."TRG_comment",
  b."PIX_comment",
  b."RPC_comment",
  b."SIST_comment",
  b."JMET_comment",
  b."EGAM_comment",
  b."MUON_comment",
  b."SCAL_comment",
  b."l1s_CSC",
  b."l1s_DT",
  b."l1s_ECAL",
  b."l1s_HCAL",
  b."l1s_Random",
  b."l1s_RPC",
  b."l1s_Other",
  b."l1s_CSC_comment",
  b."l1s_DT_comment",
  b."l1s_ECAL_comment",
  b."l1s_HCAL_comment",
  b."l1s_Random_comment",
  b."l1s_RPC_comment",
  b."l1s_Other_comment"
from
  RR_RUNINFO_CACHE a 
    left join RR_RUN_DATA b on (a.RUN_NUMBER = b.RUN_NUMBER);
  GRANT SELECT ON "RI_RUN_DATA" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for View RI_RUN_DATA_OLD
--------------------------------------------------------

  CREATE OR REPLACE VIEW "RI_RUN_DATA_OLD" ("RUN_NUMBER", "RUN_GLOBALNAME", "RUN_STATUS", "RUN_BFIELD", "RUN_BFIELD_COMMENT", "RUN_STOP_REASON", "RUN_ONLINE_COMMENT", "RUN_OFFLINE_COMMENT", "RUN_CREATE_TIME", "RUN_ONLINE_SHIFTER", "RUN_OFFLINE_SHIFTER", "RUN_L1SOURCES", "RUN_EVENTS", "RUN_START_TIME", "RUN_END_TIME", "RUN_RATE", "RUN_L1KEY", "RUN_HLTKEY", "CSC", "DT", "ECAL", "HCAL", "TRG", "PIX", "RPC", "SIST", "SCAL", "JMET", "EGAM", "MUON", "CSC_comment", "DT_comment", "ECAL_comment", "HCAL_comment", "TRG_comment", "PIX_comment", "RPC_comment", "SIST_comment", "JMET_comment", "EGAM_comment", "MUON_comment", "SCAL_comment", "l1s_CSC", "l1s_DT", "l1s_ECAL", "l1s_HCAL", "l1s_Random", "l1s_RPC", "l1s_Other", "l1s_CSC_comment", "l1s_DT_comment", "l1s_ECAL_comment", "l1s_HCAL_comment", "l1s_Random_comment", "l1s_RPC_comment", "l1s_Other_comment") AS 
  select 
  a.RUN_NUMBER,
  b.RUN_GLOBALNAME,
  b.RUN_STATUS,
  b.RUN_BFIELD,
  b.run_bfield_comment,
  b.RUN_STOP_REASON,
  b.RUN_ONLINE_COMMENT,
  b.RUN_OFFLINE_COMMENT,
  b.RUN_CREATE_TIME, 
  b.RUN_ONLINE_SHIFTER,
  b.RUN_OFFLINE_SHIFTER,
  b.RUN_L1SOURCES,
  DECODE(b.RUN_NUMBER, null, a.RUN_EVENTS, a.RUN_EVENTS) as RUN_EVENTS,
	TO_CHAR(a.RUN_START_TIME, 'YYYY.MM.DD HH24:MI:SS') as RUN_START_TIME, 
  TO_CHAR(a.RUN_END_TIME, 'YYYY.MM.DD HH24:MI:SS') as RUN_END_TIME, 
  DECODE(b.RUN_NUMBER, null, a.RUN_RATE, b.RUN_RATE) as RUN_RATE,
  DECODE(b.RUN_NUMBER, null, a.RUN_L1KEY, b.RUN_L1KEY) as RUN_L1KEY,
  DECODE(b.RUN_NUMBER, null, a.RUN_HLTKEY, b.RUN_HLTKEY) as RUN_HLTKEY,
  DECODE(b.RUN_NUMBER, null, a.CSC, b.CSC) as CSC,
  DECODE(b.RUN_NUMBER, null, a.DT, b.DT) as DT,
  DECODE(b.RUN_NUMBER, null, a.ECAL, b.ECAL) as ECAL,
  DECODE(b.RUN_NUMBER, null, a.HCAL, b.HCAL) as HCAL,
  DECODE(b.RUN_NUMBER, null, a.TRG, b.TRG) as TRG,
  DECODE(b.RUN_NUMBER, null, a.PIX, b.PIX) as PIX,
  DECODE(b.RUN_NUMBER, null, a.RPC, b.RPC) as RPC,
  DECODE(b.RUN_NUMBER, null, a.SIST, b.SIST) as SIST,
  DECODE(b.RUN_NUMBER, null, a.SCAL, b.SCAL) as SCAL,
  b.JMET,
  b.EGAM,
  b.MUON,
  b."CSC_comment",
  b."DT_comment",
  b."ECAL_comment",
  b."HCAL_comment",
  b."TRG_comment",
  b."PIX_comment",
  b."RPC_comment",
  b."SIST_comment",
  b."JMET_comment",
  b."EGAM_comment",
  b."MUON_comment",
  b."SCAL_comment",
  b."l1s_CSC",
  b."l1s_DT",
  b."l1s_ECAL",
  b."l1s_HCAL",
  b."l1s_Random",
  b."l1s_RPC",
  b."l1s_Other",
  b."l1s_CSC_comment",
  b."l1s_DT_comment",
  b."l1s_ECAL_comment",
  b."l1s_HCAL_comment",
  b."l1s_Random_comment",
  b."l1s_RPC_comment",
  b."l1s_Other_comment"
from
  RR_RUNINFO_CACHE a 
    left join RR_RUN_DATA b on (a.RUN_NUMBER = b.RUN_NUMBER);
--------------------------------------------------------
--  DDL for View RR_DBSFLAGS
--------------------------------------------------------

  CREATE OR REPLACE VIEW "RR_DBSFLAGS" ("run", "tag", "value") AS 
  select 
  run_number "run", 
  sub_dbs_flag "tag", 
  rsu_value "value" 
from 
  rr_last_runs 
  join rr_run_subsystems on (run_last_id = rsu_run_id) 
  join rr_subsystems on (sub_abbr = rsu_sub_abbr) 
where 
  sub_dbs_flag is not null and
  rsu_value in ('GOOD', 'BAD');
  GRANT SELECT ON "RR_DBSFLAGS" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for View RR_L1S_DATA
--------------------------------------------------------

  CREATE OR REPLACE VIEW "RR_L1S_DATA" ("RL1_RUN_ID", "l1s_CSC", "l1s_DT", "l1s_ECAL", "l1s_HCAL", "l1s_Random", "l1s_RPC", "l1s_Other", "l1s_CSC_comment", "l1s_DT_comment", "l1s_ECAL_comment", "l1s_HCAL_comment", "l1s_Random_comment", "l1s_RPC_comment", "l1s_Other_comment") AS 
  select 
  rl1_run_id, 

  MIN(DECODE(rl1_l1s_abbr, 'CSC', 'IN', null)) as "l1s_CSC",
  MIN(DECODE(rl1_l1s_abbr, 'DT', 'IN', null)) as "l1s_DT",
  MIN(DECODE(rl1_l1s_abbr, 'ECAL', 'IN', null)) as "l1s_ECAL",
  MIN(DECODE(rl1_l1s_abbr, 'HCAL', 'IN', null)) as "l1s_HCAL",
  MIN(DECODE(rl1_l1s_abbr, 'Random', 'IN', null)) as "l1s_Random",
  MIN(DECODE(rl1_l1s_abbr, 'RPC', 'IN', null)) as "l1s_RPC",
  MIN(DECODE(rl1_l1s_abbr, 'Other', 'IN', null)) as "l1s_Other",

  MAX(DECODE(rl1_l1s_abbr, 'CSC', rl1_comment, null)) as "l1s_CSC_comment",
  MAX(DECODE(rl1_l1s_abbr, 'DT', rl1_comment, null)) as "l1s_DT_comment",
  MAX(DECODE(rl1_l1s_abbr, 'ECAL', rl1_comment, null)) as "l1s_ECAL_comment",
  MAX(DECODE(rl1_l1s_abbr, 'HCAL', rl1_comment, null)) as "l1s_HCAL_comment",
  MAX(DECODE(rl1_l1s_abbr, 'Random', rl1_comment, null)) as "l1s_Random_comment",
  MAX(DECODE(rl1_l1s_abbr, 'RPC', rl1_comment, null)) as "l1s_RPC_comment",
  MAX(DECODE(rl1_l1s_abbr, 'Other', rl1_comment, null)) as "l1s_Other_comment"

from 
  rr_run_l1sources
group by 
  rl1_run_id;
  GRANT SELECT ON "RR_L1S_DATA" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for View RR_LAST_RUNS
--------------------------------------------------------

  CREATE OR REPLACE VIEW "RR_LAST_RUNS" ("RUN_LAST_ID", "RUN_NUMBER", "RUN_COUNT_TAGS") AS 
  select 
  max(run_id) as run_last_id, 
  run_number,
  count(*) as run_count_tags 
from 
  rr_runs 
group by 
  run_number;
  GRANT SELECT ON "RR_LAST_RUNS" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for View RR_RUNINFO_CACHE_TO_INSERT
--------------------------------------------------------

  CREATE OR REPLACE VIEW "RR_RUNINFO_CACHE_TO_INSERT" ("RUN_NUMBER", "RUN_EVENTS", "RUN_START_TIME", "RUN_END_TIME", "RUN_RATE", "RUN_L1KEY", "RUN_HLTKEY", "CSC", "DT", "ECAL", "HCAL", "TRG", "PIX", "RPC", "SIST", "SCAL", "currdate") AS 
  select 
  RUNNUMBER as RUN_NUMBER,
  MAX(DECODE(name, 'CMS.TRG:EVNR', to_number(string_value), null)) as RUN_EVENTS,
  MAX(DECODE(name, 'CMS.LVL0:START_TIME_T', time, null)) as RUN_START_TIME,
  MAX(DECODE(name, 'CMS.LVL0:STOP_TIME_T', time, null)) as RUN_END_TIME,
  nvl(ROUND(AVG(DECODE(name, 'CMS.TRG:Rate', to_number(string_value), null)), 0), 0) as RUN_RATE,
  nvl(MAX(DECODE(name, 'CMS.TRG:TSC_KEY', string_value, null)), 'n/a') as RUN_L1KEY,
  nvl(MAX(DECODE(name, 'CMS.LVL0:HLT_KEY_DESCRIPTION', string_value, null)), 'n/a') as RUN_HLTKEY,
  DECODE(MAX(DECODE(name, 'CMS.LVL0:CSC', string_value, null)), 'In', 'GOOD', DECODE(MAX(DECODE(name, 'CMS.LVL0:CSC_STATE', string_value, null)), null, null, 'GOOD')) as CSC,
  DECODE(MAX(DECODE(name, 'CMS.LVL0:DT', string_value, null)), 'In', 'GOOD', DECODE(MAX(DECODE(name, 'CMS.LVL0:DT_STATE', string_value, null)), null, null, 'GOOD')) as DT,
  DECODE(MAX(DECODE(name, 'CMS.LVL0:ECAL', string_value, null)), 'In', 'GOOD', DECODE(MAX(DECODE(name, 'CMS.LVL0:ECAL_STATE', string_value, null)), null, null, 'GOOD')) as ECAL,
  DECODE(MAX(DECODE(name, 'CMS.LVL0:HCAL', string_value, null)), 'In', 'GOOD', DECODE(MAX(DECODE(name, 'CMS.LVL0:HCAL_STATE', string_value, null)), null, null, 'GOOD')) as HCAL,
  DECODE(MAX(DECODE(name, 'CMS.LVL0:TRG', string_value, null)), 'In', 'GOOD', DECODE(MAX(DECODE(name, 'CMS.LVL0:TRG_STATE', string_value, null)), null, null, 'GOOD')) as TRG,
  DECODE(MAX(DECODE(name, 'CMS.LVL0:PIXEL', string_value, null)), 'In', 'GOOD', DECODE(MAX(DECODE(name, 'CMS.LVL0:PIXEL_STATE', string_value, null)), null, null, 'GOOD')) as PIX,
  DECODE(MAX(DECODE(name, 'CMS.LVL0:RPC', string_value, null)), 'In', 'GOOD', DECODE(MAX(DECODE(name, 'CMS.LVL0:RPC_STATE', string_value, null)), null, null, 'GOOD')) as RPC,
  DECODE(MAX(DECODE(name, 'CMS.LVL0:TRACKER', string_value, null)), 'In', 'GOOD', DECODE(MAX(DECODE(name, 'CMS.LVL0:TRACKER_STATE', string_value, null)), null, null, 'GOOD')) as SIST,
  DECODE(MAX(DECODE(name, 'CMS.LVL0:SCAL', string_value, null)), 'In', 'GOOD', DECODE(MAX(DECODE(name, 'CMS.LVL0:SCAL_STATE', string_value, null)), null, null, 'GOOD')) as SCAL,
  sysdate
from
  RUNSESSION_PARAMETER
where
  RUNNUMBER > nvl((select max(RUN_NUMBER) from rr_runinfo_cache), 0)
group by
  RUNNUMBER;
--------------------------------------------------------
--  DDL for View RR_RUNINFO_CACHE_TO_UPDATE
--------------------------------------------------------

  CREATE OR REPLACE VIEW "RR_RUNINFO_CACHE_TO_UPDATE" ("RUN_NUMBER", "RUN_EVENTS", "RUN_START_TIME", "RUN_END_TIME", "RUN_RATE", "RUN_L1KEY", "RUN_HLTKEY", "CSC", "DT", "ECAL", "HCAL", "TRG", "PIX", "RPC", "SIST", "SCAL") AS 
  select /*+ USE_NL(b, a)*/
  a.RUNNUMBER as RUN_NUMBER,
  MAX(DECODE(name, 'CMS.TRG:EVNR', to_number(string_value), null)) as RUN_EVENTS,
  MAX(DECODE(name, 'CMS.LVL0:START_TIME_T', time, null)) as RUN_START_TIME,
  MAX(DECODE(name, 'CMS.LVL0:STOP_TIME_T', time, null)) as RUN_END_TIME,
  nvl(ROUND(AVG(DECODE(name, 'CMS.TRG:Rate', to_number(string_value), null)), 0), 0) as RUN_RATE,
  nvl(MAX(DECODE(name, 'CMS.TRG:TSC_KEY', string_value, null)), 'n/a') as RUN_L1KEY,
  nvl(MAX(DECODE(name, 'CMS.LVL0:HLT_KEY_DESCRIPTION', string_value, null)), 'n/a') as RUN_HLTKEY,
  DECODE(MAX(DECODE(name, 'CMS.LVL0:CSC', string_value, null)), 'In', 'GOOD', DECODE(MAX(DECODE(name, 'CMS.LVL0:CSC_STATE', string_value, null)), null, null, 'GOOD')) as CSC,
  DECODE(MAX(DECODE(name, 'CMS.LVL0:DT', string_value, null)), 'In', 'GOOD', DECODE(MAX(DECODE(name, 'CMS.LVL0:DT_STATE', string_value, null)), null, null, 'GOOD')) as DT,
  DECODE(MAX(DECODE(name, 'CMS.LVL0:ECAL', string_value, null)), 'In', 'GOOD', DECODE(MAX(DECODE(name, 'CMS.LVL0:ECAL_STATE', string_value, null)), null, null, 'GOOD')) as ECAL,
  DECODE(MAX(DECODE(name, 'CMS.LVL0:HCAL', string_value, null)), 'In', 'GOOD', DECODE(MAX(DECODE(name, 'CMS.LVL0:HCAL_STATE', string_value, null)), null, null, 'GOOD')) as HCAL,
  DECODE(MAX(DECODE(name, 'CMS.LVL0:TRG', string_value, null)), 'In', 'GOOD', DECODE(MAX(DECODE(name, 'CMS.LVL0:TRG_STATE', string_value, null)), null, null, 'GOOD')) as TRG,
  DECODE(MAX(DECODE(name, 'CMS.LVL0:PIXEL', string_value, null)), 'In', 'GOOD', DECODE(MAX(DECODE(name, 'CMS.LVL0:PIXEL_STATE', string_value, null)), null, null, 'GOOD')) as PIX,
  DECODE(MAX(DECODE(name, 'CMS.LVL0:RPC', string_value, null)), 'In', 'GOOD', DECODE(MAX(DECODE(name, 'CMS.LVL0:RPC_STATE', string_value, null)), null, null, 'GOOD')) as RPC,
  DECODE(MAX(DECODE(name, 'CMS.LVL0:TRACKER', string_value, null)), 'In', 'GOOD', DECODE(MAX(DECODE(name, 'CMS.LVL0:TRACKER_STATE', string_value, null)), null, null, 'GOOD')) as SIST,
  DECODE(MAX(DECODE(name, 'CMS.LVL0:SCAL', string_value, null)), 'In', 'GOOD', DECODE(MAX(DECODE(name, 'CMS.LVL0:SCAL_STATE', string_value, null)), null, null, 'GOOD')) as SCAL

from
  RUNSESSION_PARAMETER a,
  (select RUN_NUMBER from (select RUN_NUMBER, ROW_NUMBER() OVER (ORDER BY RUN_NUMBER desc) as n from rr_runinfo_cache) where N < 20 union select RUN_NUMBER from rr_runs where run_end_time is null) b
where
  a.RUNNUMBER = b.run_number 
group by
  RUNNUMBER;
--------------------------------------------------------
--  DDL for View RR_RUNINFO_SUBS_TO_DELETE
--------------------------------------------------------

  CREATE OR REPLACE VIEW "RR_RUNINFO_SUBS_TO_DELETE" ("RUN_ID", "RSU_SUB_ABBR", "RSU_COMMENT", "RSU_VALUE") AS 
  select 
  c."RUN_LAST_ID",a."RSU_SUB_ABBR",a."RSU_COMMENT",a."RSU_VALUE" 
from 
  rr_run_subsystems a,
  rr_subsystems b, 
  rr_last_runs c
where
  a.rsu_sub_abbr = b.sub_abbr and
  b.sub_type = 'ONLINE' and
  c.run_last_id = a.rsu_run_id and
  (c.run_number, rsu_sub_abbr) not in (select distinct a.runnumber "number", b.sub_abbr "sub" from runsession_parameter a, rr_subsystems b where a.string_value = 'In' and a.name = b.sub_runsession_parameter_name);
--------------------------------------------------------
--  DDL for View RR_RUNINFO_SUBS_TO_INSERT
--------------------------------------------------------

  CREATE OR REPLACE VIEW "RR_RUNINFO_SUBS_TO_INSERT" ("id", "sub", "value") AS 
  select distinct 
  c.run_last_id "id",
  b.sub_abbr "sub",
  'GOOD' "value"
from 
  runsession_parameter a,
  rr_subsystems b,
  rr_last_runs c,
  rr_run_subsystems d
where 
  a.string_value = 'In' and
  a.name = b.sub_runsession_parameter_name and
  a.runnumber = c.run_number and
  (c.run_last_id, b.sub_abbr) not in (select rsu_run_id, rsu_sub_abbr from rr_run_subsystems);
--------------------------------------------------------
--  DDL for View RR_RUNINFO_SUBS_TO_UPDATE
--------------------------------------------------------

  CREATE OR REPLACE VIEW "RR_RUNINFO_SUBS_TO_UPDATE" ("id", "sub", "value") AS 
  select distinct 
  d.run_last_id "id",
  b.sub_abbr "sub",
  rsu_value "value"
from 
  runsession_parameter a,
  rr_subsystems b,
  rr_run_subsystems c,
  rr_last_runs d
where 
  a.string_value = 'In' and
  a.name = b.sub_runsession_parameter_name and
  c.rsu_run_id = d.run_last_id and
  a.runnumber = d.run_number and
  b.sub_abbr = c.rsu_sub_abbr and
  (c.rsu_value <> 'GOOD' or 
  (c.rsu_value = 'BAD' and c.rsu_comment is null));
--------------------------------------------------------
--  DDL for View RR_RUNS_TO_OFFLINE
--------------------------------------------------------

  CREATE OR REPLACE VIEW "RR_RUNS_TO_OFFLINE" ("run_id") AS 
  select 
  r.run_id
from 
  rr_runs r 
  join rr_run_mtime m on (m."run_id" = r.run_id) 
where 
  r.run_end_time is not null and 
  r.run_status = 'ONLINE' and
  ((sysdate - m."mtime") > (1 / 24));
--------------------------------------------------------
--  DDL for View RR_RUN_DATA
--------------------------------------------------------

  CREATE OR REPLACE VIEW "RR_RUN_DATA" ("RUN_ID", "RUN_GLOBALNAME", "RUN_NUMBER", "RUN_EVENTS", "RUN_RATE", "RUN_STATUS", "RUN_BFIELD", "RUN_BFIELD_COMMENT", "RUN_START_TIME", "RUN_END_TIME", "RUN_STOP_REASON", "RUN_L1KEY", "RUN_HLTKEY", "RUN_ONLINE_COMMENT", "RUN_OFFLINE_COMMENT", "RUN_CREATE_TIME", "RUN_ONLINE_SHIFTER", "RUN_OFFLINE_SHIFTER", "RUN_TAG", "RUN_L1SOURCES", "RUN_COUNT_TAGS", "RSU_RUN_ID", "CSC", "DT", "ECAL", "HCAL", "TRG", "PIX", "RPC", "SIST", "JMET", "EGAM", "MUON", "SCAL", "CSC_comment", "DT_comment", "ECAL_comment", "HCAL_comment", "TRG_comment", "PIX_comment", "RPC_comment", "SIST_comment", "JMET_comment", "EGAM_comment", "MUON_comment", "SCAL_comment", "RL1_RUN_ID", "l1s_CSC", "l1s_DT", "l1s_ECAL", "l1s_HCAL", "l1s_Random", "l1s_RPC", "l1s_Other", "l1s_CSC_comment", "l1s_DT_comment", "l1s_ECAL_comment", "l1s_HCAL_comment", "l1s_Random_comment", "l1s_RPC_comment", "l1s_Other_comment") AS 
  SELECT
   r.RUN_ID,
   r.RUN_GLOBALNAME,
   r.RUN_NUMBER, 
	 r.RUN_EVENTS, 
	 r.RUN_RATE, 
   r.RUN_STATUS,
   r.RUN_BFIELD,
   r.run_bfield_comment,
   TO_CHAR(r.RUN_START_TIME, 'YYYY.MM.DD HH24:MI:SS') as RUN_START_TIME, 
   TO_CHAR(r.RUN_END_TIME, 'YYYY.MM.DD HH24:MI:SS') as RUN_END_TIME,
	 r.RUN_STOP_REASON, 
	 r.RUN_L1KEY, 
   r.RUN_HLTKEY, 
   r.RUN_ONLINE_COMMENT,
   r.RUN_OFFLINE_COMMENT,
   TO_CHAR(r.RUN_CREATE_TIME, 'YYYY.MM.DD HH24:MI:SS') as RUN_CREATE_TIME,
   r.RUN_ONLINE_SHIFTER,
   r.RUN_OFFLINE_SHIFTER,
   r.RUN_TAG,
   RR_UTILS.get_l1sources(r.RUN_NUMBER) as RUN_L1SOURCES,
   lr.run_count_tags,
   s.*,
   l.*
from 
  rr_runs r
    join rr_last_runs lr on (r.run_id = lr.run_last_id)
    left join rr_sub_data s on (r.run_id = s.rsu_run_id)
    left join rr_l1s_data l on (r.run_id = l.rl1_run_id);
  GRANT SELECT ON "RR_RUN_DATA" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for View RR_RUN_DATA_EDIT
--------------------------------------------------------

  CREATE OR REPLACE VIEW "RR_RUN_DATA_EDIT" ("RUN_ID", "RUN_GLOBALNAME", "RUN_NUMBER", "RUN_EVENTS", "RUN_RATE", "RUN_STATUS", "RUN_BFIELD", "RUN_BFIELD_COMMENT", "RUN_START_TIME", "RUN_END_TIME", "RUN_STOP_REASON", "RUN_L1KEY", "RUN_HLTKEY", "RUN_ONLINE_COMMENT", "RUN_OFFLINE_COMMENT", "RUN_CREATE_TIME", "RUN_ONLINE_SHIFTER", "RUN_OFFLINE_SHIFTER", "RUN_TAG", "RUN_L1SOURCES", "RUN_COUNT_TAGS", "CSC", "DT", "ECAL", "HCAL", "TRG", "PIX", "RPC", "SIST", "JMET", "EGAM", "MUON", "SCAL", "CSC_comment", "DT_comment", "ECAL_comment", "HCAL_comment", "TRG_comment", "PIX_comment", "RPC_comment", "SIST_comment", "JMET_comment", "EGAM_comment", "MUON_comment", "SCAL_comment", "l1s_CSC", "l1s_DT", "l1s_ECAL", "l1s_HCAL", "l1s_Random", "l1s_RPC", "l1s_Other", "l1s_CSC_comment", "l1s_DT_comment", "l1s_ECAL_comment", "l1s_HCAL_comment", "l1s_Random_comment", "l1s_RPC_comment", "l1s_Other_comment") AS 
  select
    "RUN_ID",
    "RUN_GLOBALNAME",
    "RUN_NUMBER",
    "RUN_EVENTS",
    "RUN_RATE",
    "RUN_STATUS",
    "RUN_BFIELD",
    "RUN_BFIELD_COMMENT",
    "RUN_START_TIME",
    "RUN_END_TIME",
    "RUN_STOP_REASON",
    "RUN_L1KEY",
    "RUN_HLTKEY",
    "RUN_ONLINE_COMMENT",
    "RUN_OFFLINE_COMMENT",
    "RUN_CREATE_TIME",
    "RUN_ONLINE_SHIFTER",
    "RUN_OFFLINE_SHIFTER",
    "RUN_TAG",
    "RUN_L1SOURCES",
    "RUN_COUNT_TAGS",
    "CSC",
    "DT",
    "ECAL",
    "HCAL",
    "TRG",
    "PIX",
    "RPC",
    "SIST",
    "JMET",
    "EGAM",
    "MUON",
    "SCAL",
    "CSC_comment",
    "DT_comment",
    "ECAL_comment",
    "HCAL_comment",
    "TRG_comment",
    "PIX_comment",
    "RPC_comment",
    "SIST_comment",
    "JMET_comment",
    "EGAM_comment",
    "MUON_comment",
    "SCAL_comment",
    "l1s_CSC",
    "l1s_DT",
    "l1s_ECAL",
    "l1s_HCAL",
    "l1s_Random",
    "l1s_RPC",
    "l1s_Other",
    "l1s_CSC_comment",
    "l1s_DT_comment",
    "l1s_ECAL_comment",
    "l1s_HCAL_comment",
    "l1s_Random_comment",
    "l1s_RPC_comment",
    "l1s_Other_comment"
FROM 
  rr_run_data a
UNION ALL
select 
  NULL,
  NULL,
  RUN_NUMBER,
  RUN_EVENTS,
  RUN_RATE,
  NULL,
  NULL,
  NULL,
	to_char(RUN_START_TIME, 'YYYY-MM-DD HH24:MI:SS') as RUN_START_TIME, 
  to_char(RUN_END_TIME, 'YYYY-MM-DD HH24:MI:SS') as RUN_END_TIME, 
  NULL,
  RUN_L1KEY,
  RUN_HLTKEY,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  CSC,
  DT,
  ECAL,
  HCAL,
  TRG,
  PIX,
  RPC,
  SIST,
  NULL,
  NULL,
  NULL,
  SCAL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
from
  RR_RUNINFO_CACHE a
where 
  a.RUN_NUMBER not in (select run_number FROM rr_run_data);
  GRANT SELECT ON "RR_RUN_DATA_EDIT" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for View RR_RUN_DATA_PREV
--------------------------------------------------------

  CREATE OR REPLACE VIEW "RR_RUN_DATA_PREV" ("RUN_ID", "RUN_GLOBALNAME", "RUN_NUMBER", "RUN_EVENTS", "RUN_RATE", "RUN_STATUS", "RUN_BFIELD", "RUN_BFIELD_COMMENT", "RUN_START_TIME", "RUN_END_TIME", "RUN_STOP_REASON", "RUN_L1KEY", "RUN_HLTKEY", "RUN_ONLINE_COMMENT", "RUN_OFFLINE_COMMENT", "RUN_CREATE_TIME", "RUN_ONLINE_SHIFTER", "RUN_OFFLINE_SHIFTER", "RUN_TAG", "RUN_L1SOURCES", "RUN_COUNT_TAGS", "RSU_RUN_ID", "CSC", "DT", "ECAL", "HCAL", "TRG", "PIX", "RPC", "SIST", "JMET", "EGAM", "MUON", "SCAL", "CSC_comment", "DT_comment", "ECAL_comment", "HCAL_comment", "TRG_comment", "PIX_comment", "RPC_comment", "SIST_comment", "JMET_comment", "EGAM_comment", "MUON_comment", "SCAL_comment", "RL1_RUN_ID", "l1s_CSC", "l1s_DT", "l1s_ECAL", "l1s_HCAL", "l1s_Random", "l1s_RPC", "l1s_Other", "l1s_CSC_comment", "l1s_DT_comment", "l1s_ECAL_comment", "l1s_HCAL_comment", "l1s_Random_comment", "l1s_RPC_comment", "l1s_Other_comment") AS 
  SELECT
   r.RUN_ID,
   r.RUN_GLOBALNAME,
   r.RUN_NUMBER, 
	 r.RUN_EVENTS, 
	 r.RUN_RATE, 
   r.RUN_STATUS,
   r.RUN_BFIELD,
   r.run_bfield_comment,
   TO_CHAR(r.RUN_START_TIME, 'YYYY.MM.DD HH24:MI:SS') as RUN_START_TIME, 
   TO_CHAR(r.RUN_END_TIME, 'YYYY.MM.DD HH24:MI:SS') as RUN_END_TIME,
	 r.RUN_STOP_REASON, 
	 r.RUN_L1KEY, 
   r.RUN_HLTKEY, 
   r.RUN_ONLINE_COMMENT,
   r.RUN_OFFLINE_COMMENT,
   TO_CHAR(r.RUN_CREATE_TIME, 'YYYY.MM.DD HH24:MI:SS') as RUN_CREATE_TIME,
   r.RUN_ONLINE_SHIFTER,
   r.RUN_OFFLINE_SHIFTER,
   r.RUN_TAG,
   RR_UTILS.get_l1sources(r.RUN_NUMBER) as RUN_L1SOURCES,
   null as run_count_tags,
   s.*,
   l.*
from 
  rr_runs r
    left join rr_sub_data s on (r.run_id = s.rsu_run_id)
    left join rr_l1s_data l on (r.run_id = l.rl1_run_id)
where
  r.run_id not in (select run_last_id from rr_last_runs);
  GRANT SELECT ON "RR_RUN_DATA_PREV" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for View RR_RUN_MTIME
--------------------------------------------------------

  CREATE OR REPLACE VIEW "RR_RUN_MTIME" ("run_id", "mtime") AS 
  select
  a.i "run_id",
 greatest(a.t, b.t, c.t) "mtime"
from
  (select rl1_run_id i, max(rl1_action_date) as t FROM rr_run_l1sources$a group by rl1_run_id)  a,
  (select rsu_run_id i, max(rsu_action_date) as t FROM rr_run_subsystems$a group by rsu_run_id) b,
  (select run_id i    , max(run_action_date) as t FROM rr_runs$a group by run_id) c
where
  a.i = b.i and a.i = c.i;
  GRANT SELECT ON "RR_RUN_MTIME" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for View RR_SUB_DATA
--------------------------------------------------------

  CREATE OR REPLACE VIEW "RR_SUB_DATA" ("RSU_RUN_ID", "CSC", "DT", "ECAL", "HCAL", "TRG", "PIX", "RPC", "SIST", "JMET", "EGAM", "MUON", "SCAL", "CSC_comment", "DT_comment", "ECAL_comment", "HCAL_comment", "TRG_comment", "PIX_comment", "RPC_comment", "SIST_comment", "JMET_comment", "EGAM_comment", "MUON_comment", "SCAL_comment") AS 
  select 
  rsu_run_id, 

  MAX(DECODE(rsu_sub_abbr, 'CSC', rsu_value, null)) as "CSC",
  MAX(DECODE(rsu_sub_abbr, 'DT', rsu_value, null)) as "DT",
  MAX(DECODE(rsu_sub_abbr, 'ECAL', rsu_value, null)) as "ECAL",
  MAX(DECODE(rsu_sub_abbr, 'HCAL', rsu_value, null)) as "HCAL",
  MAX(DECODE(rsu_sub_abbr, 'TRG', rsu_value, null)) as "TRG",
  MAX(DECODE(rsu_sub_abbr, 'PIX', rsu_value, null)) as "PIX",
  MAX(DECODE(rsu_sub_abbr, 'RPC', rsu_value, null)) as "RPC",
  MAX(DECODE(rsu_sub_abbr, 'SIST', rsu_value, null)) as "SIST", 
  MAX(DECODE(rsu_sub_abbr, 'JMET', rsu_value, null)) as "JMET",
  MAX(DECODE(rsu_sub_abbr, 'EGAM', rsu_value, null)) as "EGAM",
  MAX(DECODE(rsu_sub_abbr, 'MUON', rsu_value, null)) as "MUON",
  MAX(DECODE(rsu_sub_abbr, 'SCAL', rsu_value, null)) as "SCAL",

  MAX(DECODE(rsu_sub_abbr, 'CSC', rsu_comment, null)) as "CSC_comment",
  MAX(DECODE(rsu_sub_abbr, 'DT', rsu_comment, null)) as "DT_comment",
  MAX(DECODE(rsu_sub_abbr, 'ECAL', rsu_comment, null)) as "ECAL_comment",
  MAX(DECODE(rsu_sub_abbr, 'HCAL', rsu_comment, null)) as "HCAL_comment",
  MAX(DECODE(rsu_sub_abbr, 'TRG', rsu_comment, null)) as "TRG_comment",
  MAX(DECODE(rsu_sub_abbr, 'PIX', rsu_comment, null)) as "PIX_comment",
  MAX(DECODE(rsu_sub_abbr, 'RPC', rsu_comment, null)) as "RPC_comment",
  MAX(DECODE(rsu_sub_abbr, 'SIST', rsu_comment, null)) as "SIST_comment", 
  MAX(DECODE(rsu_sub_abbr, 'JMET', rsu_comment, null)) as "JMET_comment",
  MAX(DECODE(rsu_sub_abbr, 'EGAM', rsu_comment, null)) as "EGAM_comment",
  MAX(DECODE(rsu_sub_abbr, 'MUON', rsu_comment, null)) as "MUON_comment",
  MAX(DECODE(rsu_sub_abbr, 'SCAL', rsu_comment, null)) as "SCAL_comment"

from 
  rr_run_subsystems
group by 
  rsu_run_id;
  GRANT SELECT ON "RR_SUB_DATA" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for Function GET_MAGNETIC_FIELD
--------------------------------------------------------

  CREATE OR REPLACE FUNCTION "GET_MAGNETIC_FIELD" 
( run_number IN NUMBER
) RETURN NUMBER AS
BEGIN
  return cms_wbm.rundb.get_magnetic_field(run_number);
END GET_MAGNETIC_FIELD;

/

--------------------------------------------------------
--  DDL for Package RR_RUNINFO
--------------------------------------------------------

  CREATE OR REPLACE PACKAGE "RR_RUNINFO" AS

  function create_run(p_runnumber in number, p_username in VARCHAR2, p_globalname in VARCHAR2) RETURN number;
  function update_run_status(p_runnumber in number, p_status in VARCHAR2, p_username in varchar2, p_tag in VARCHAR2 default '') RETURN number;
  procedure update_runs;
  procedure start_jobs;
  procedure stop_jobs;
  procedure normalize_runs;
  
  function ri_info(p_run_number in number) return clob;
  
  function ri_events(p_run_number in number) return NUMBER;
  function ri_start_time(p_run_number in number) return date;
  function ri_end_time(p_run_number in number) return date;
  function ri_rate(p_run_number in number) return NUMBER;
  function ri_l1key(p_run_number in number) return varchar2;
  function ri_hltkey(p_run_number in number) return varchar2;
  procedure ri_lastentries(p_RUN_SHIFTER out nocopy VARCHAR2, p_RUN_GLOBALNAME out nocopy VARCHAR2);

  
END RR_RUNINFO;

/

  GRANT EXECUTE ON "RR_RUNINFO" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for Package RR_UTILS
--------------------------------------------------------

  CREATE OR REPLACE PACKAGE "RR_UTILS" AS

  FUNCTION get_subsystems(run in number) return varchar2;
  FUNCTION get_l1sources(run in number) return varchar2;
  FUNCTION date2unix(d in date) return number;
  FUNCTION unix2date(n in number) return date;
  
  PROCEDURE audit_preload;

END RR_UTILS;

/

  GRANT EXECUTE ON "RR_UTILS" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for Package Body RR_RUNINFO
--------------------------------------------------------

  CREATE OR REPLACE PACKAGE BODY "RR_RUNINFO" AS

  function create_run(p_runnumber in number, p_username in VARCHAR2, p_globalname in VARCHAR2) RETURN number AS
    PRAGMA AUTONOMOUS_TRANSACTION;
    l_number NUMBER := 0; 
    l_run_id number := 0;
  BEGIN

    set transaction name 'new run';

    select count(*) into l_number from rr_runs where run_number = p_runnumber;
    if l_number > 0 then
      rollback;
      return 0;
    end if;
    
    select RR_RUN_ID_SQ.nextval into l_run_id from dual;
    
    INSERT INTO
      RR_RUNS (
        RUN_ID,
        RUN_NUMBER,
        RUN_ONLINE_SHIFTER,
        RUN_EVENTS,
        RUN_RATE,
        RUN_START_TIME,
        RUN_END_TIME,
        RUN_L1KEY,
        RUN_HLTKEY,
        RUN_CREATE_TIME,
        RUN_GLOBALNAME,
        RUN_BFIELD
      ) select
        l_run_id,
        a.run_number,
        p_username,
        a.run_events,
        a.run_rate,
        a.run_start_time,
        a.run_end_time,
        a.run_l1key,
        a.run_hltkey,
        sysdate,
        p_globalname,
        get_magnetic_field(p_runnumber)
      from 
        rr_runinfo_cache a
      where
        run_number = p_runnumber;
    
    commit;
    RETURN l_run_id;
    
  exception when NO_DATA_FOUND then
    rollback;
    RETURN 0;
  END create_run;

  procedure update_runs as

    PRAGMA AUTONOMOUS_TRANSACTION;

  begin

    set transaction name 'new run';

    begin

      -- Update current run from RUN INFO
      update RR_RUNINFO_CACHE a set 
        (RUN_EVENTS, RUN_START_TIME, RUN_END_TIME, RUN_RATE, RUN_L1KEY, RUN_HLTKEY, CSC, DT, ECAL, HCAL, TRG, PIX, RPC, SIST, SCAL) = 
        (select RUN_EVENTS, RUN_START_TIME, RUN_END_TIME, RUN_RATE, RUN_L1KEY, RUN_HLTKEY, CSC, DT, ECAL, HCAL, TRG, PIX, RPC, SIST, SCAL 
        from RR_RUNINFO_CACHE_TO_UPDATE b where a.RUN_NUMBER = b.RUN_NUMBER) where a.RUN_NUMBER IN (select RUN_NUMBER from RR_RUNINFO_CACHE_TO_UPDATE);
                                    
      -- Insert new runs from RUN_INFO
      insert into RR_RUNINFO_CACHE select * from RR_RUNINFO_CACHE_TO_INSERT;

      -- Update pending ONLINE runs to offline
      update rr_runs set run_status = 'OFFLINE' where run_id in (select "run_id" from rr_runs_to_offline a);

      -- Update run data from run info
      update RR_RUNS a set (RUN_EVENTS, RUN_RATE, RUN_END_TIME) = 
        (select RUN_EVENTS, RUN_RATE, RUN_END_TIME from RR_RUNINFO_CACHE b 
          where a.run_number = b.run_number) where a.run_end_time is null;
        
      commit;

    exception when NO_DATA_FOUND then
      rollback;
    end;

  end update_runs;

  function update_run_status(p_runnumber in number, p_status in VARCHAR2, p_username in VARCHAR2, p_tag in VARCHAR2) RETURN number as
    PRAGMA AUTONOMOUS_TRANSACTION;
    l_curr_status rr_runs.run_status%type;
    l_curr_id rr_runs.run_id%type;
    l_next_id rr_runs.run_id%type;
    l_online_shifter rr_runs.run_online_shifter%type;
    l_offline_shifter rr_runs.run_offline_shifter%type;
  begin
    
    set transaction name 'run status update';
    
    select 
      run_status, 
      rr_last_runs.run_last_id,
      run_online_shifter,
      run_offline_shifter
    into 
      l_curr_status, 
      l_curr_id,
      l_online_shifter,
      l_offline_shifter
    from 
      rr_runs 
      join rr_last_runs on (rr_last_runs.run_last_id = rr_runs.run_id and rr_last_runs.run_number = p_runnumber); 
    
    if p_status = 'COMPLETED' then
    
      update rr_runs set run_status = p_status, run_tag = p_tag where run_id = l_curr_id;
    
    else
    
      if l_curr_status = 'COMPLETED' then
        
        ----------------------------------
        -- making copy of completed run --
        ----------------------------------
        
        select RR_RUN_ID_SQ.nextval into l_next_id from dual;
        
        if p_status = 'ONLINE' then
          l_online_shifter := p_username;
        elsif p_status = 'OFFLINE' then
          l_offline_shifter := p_username;
        end if;
        
        insert into
          rr_runs
        select
          l_next_id,
          RUN_NUMBER,
          p_status,
          sysdate,
          RUN_GLOBALNAME,
          l_online_shifter,
          l_offline_shifter,
          RUN_EVENTS,
          null,
          RUN_RATE,
          RUN_START_TIME,
          RUN_END_TIME,
          RUN_BFIELD,
          RUN_STOP_REASON,
          RUN_L1KEY,
          RUN_HLTKEY,
          RUN_ONLINE_COMMENT,
          RUN_OFFLINE_COMMENT,
          RUN_BFIELD_COMMENT
        from
          rr_runs
        where
          run_id = l_curr_id;
          
        insert into
          rr_run_l1sources
        select 
          RL1_L1S_ABBR,
          RL1_COMMENT,
          l_next_id
        from
          rr_run_l1sources
        where
          RL1_RUN_ID = l_curr_id;

        insert into
          rr_run_subsystems
        select 
          RSU_SUB_ABBR,
          RSU_COMMENT,
          RSU_VALUE,
          l_next_id
        from
          rr_run_subsystems
        where
          RSU_RUN_ID = l_curr_id;
      
        l_curr_id := l_next_id;
      
      else
      
        update rr_runs set run_status = p_status where run_id = l_curr_id;
      
      end if;
      
    end if;
  
    commit;
    return l_curr_id;
    
    exception when others then
      rollback;
      return 0;
  end;

  procedure start_jobs as
  begin
  
    dbms_scheduler.create_job(
      job_name        => 'RUN_UPDATER',
      job_type        => 'STORED_PROCEDURE', 
      job_action      => 'CMS_DQM_RUN_REGISTRY.RR_RUNINFO.UPDATE_RUNS',
      start_date      => sysdate,
      repeat_interval => 'FREQ=MINUTELY;BYSECOND=0',
      enabled         => TRUE);
      
    commit;  
      
  end;

  procedure stop_jobs as
  begin
  
    dbms_scheduler.drop_job(
      job_name        => 'RUN_UPDATER',
      force           => TRUE);
      
    commit;
    
  end;

  procedure normalize_runs as
  begin
    delete from rr_run_subsystems where (rsu_run_id, rsu_sub_abbr) in (select run_id, rsu_sub_abbr from rr_runinfo_subs_to_delete);
    insert into rr_run_subsystems select "id", "sub", null, "value" from rr_runinfo_subs_to_insert;
    update rr_run_subsystems a set rsu_value = 'GOOD' where (a.rsu_run_id, a.rsu_sub_abbr) in (select "id", "sub" from rr_runinfo_subs_to_update);
  end;

  function ri_info(p_run_number in number) return clob as
    l_lob clob;
  begin
    select
      XMLElement(
        "row", 
        XMLForest(
          p_run_number as "number",
          rr_runinfo.ri_events(p_run_number) as "events",
          rr_runinfo.ri_start_time(p_run_number) as "start_time",
          rr_runinfo.ri_end_time(p_run_number) as "end_time",
          round(rr_runinfo.ri_rate(p_run_number),0) as "rate",
          rr_runinfo.ri_l1key(p_run_number) as "l1key",
          rr_runinfo.ri_hltkey(p_run_number) as "hltkey"
        )
      ).getClobVal() into l_lob
    from 
      dual;
    return l_lob;
  end;

  function ri_events(p_run_number in number) return NUMBER as
    l_RUN_EVENTS number;
  begin
    select 
      to_number(string_value) into l_RUN_EVENTS
    from
      RUNSESSION_PARAMETER
    where
      id = (select max(id) from RUNSESSION_PARAMETER where RUNNUMBER = p_run_number and name = 'CMS.TRG:EVNR');
    return l_RUN_EVENTS;
  end;
  
  function ri_start_time(p_run_number in number) return date as
    l_RUN_START_TIME date;
  begin
    select
      time into l_RUN_START_TIME
    from 
      RUNSESSION_PARAMETER 
    where 
      RUNNUMBER = p_run_number and name = 'CMS.LVL0:START_TIME_T';
    return l_RUN_START_TIME;
  end;
  
  function ri_end_time(p_run_number in number) return date as
    l_RUN_END_TIME date;
  begin
    select 
      time into l_RUN_END_TIME
    from 
      RUNSESSION_PARAMETER 
    where 
      RUNNUMBER = p_run_number and name = 'CMS.LVL0:STOP_TIME_T';
    return l_RUN_END_TIME;
  exception when NO_DATA_FOUND then
    return null;
  end;

  function ri_rate(p_run_number in number) return NUMBER as
    l_RUN_RATE number;
  begin
    select 
      avg(to_number(string_value)) into l_RUN_RATE 
    from 
      RUNSESSION_PARAMETER 
    where 
      RUNNUMBER = p_run_number and name = 'CMS.TRG:Rate';
    return l_RUN_RATE;
  end;

  function ri_l1key(p_run_number in number) return varchar2 as
    l_RUN_L1KEY VARCHAR2(100);
  begin
      select 
        substr(string_value, 1, 100) into l_RUN_L1KEY 
      from 
        RUNSESSION_PARAMETER
      where 
        RUNNUMBER = p_run_number and name = 'CMS.TRG:TSC_KEY';
    return l_RUN_L1KEY;
  end;
      
  function ri_hltkey(p_run_number in number) return varchar2 as
    l_RUN_HLTKEY VARCHAR2(100);
  begin
    select 
      substr(string_value, 1, 100) into l_RUN_HLTKEY 
    from 
      RUNSESSION_PARAMETER 
    where 
      RUNNUMBER = p_run_number and name = 'CMS.LVL0:HLT_KEY_DESCRIPTION';
    return l_RUN_HLTKEY;
  end;
      
  procedure ri_lastentries(p_RUN_SHIFTER out nocopy VARCHAR2, p_RUN_GLOBALNAME out nocopy VARCHAR2) as
  begin
    select 
      run_online_SHIFTER, run_globalname into p_RUN_SHIFTER, p_RUN_GLOBALNAME
    from 
      rr_runs
    where 
      run_number = (select max(run_number) from rr_runs);
  end;
  
/*
  function create_run(p_runnumber in number, p_username in VARCHAR2, p_globalname in VARCHAR2, p_cert in VARCHAR2) RETURN number AS

    PRAGMA AUTONOMOUS_TRANSACTION;

    l_RUN_NUMBER RR_RUNS.RUN_NUMBER%type := p_runnumber;
    l_RUN_SHIFTER RR_RUNS.RUN_SHIFTER%type := p_username;
    l_RUN_EVENTS RR_RUNS.RUN_EVENTS%type;
    l_RUN_RATE RR_RUNS.RUN_RATE%type;
    l_RUN_START_TIME RR_RUNS.RUN_START_TIME%type;
    l_RUN_END_TIME RR_RUNS.RUN_END_TIME%type;
    l_RUN_L1KEY RR_RUNS.RUN_L1KEY%type;
    l_RUN_HLTKEY RR_RUNS.RUN_HLTKEY%type;
    l_RUN_CREATE_TIME RR_RUNS.RUN_CREATE_TIME%type := sysdate;
    l_RUN_CREATE_USER RR_RUNS.RUN_CREATE_USER%type := p_username;
    l_RUN_GLOBALNAME RR_RUNS.RUN_GLOBALNAME%type := p_globalname;
    l_RUN_LAST_USER RR_RUNS.RUN_LAST_USER%type := p_username;
    l_RUN_LAST_CERT RR_RUNS.RUN_LAST_CERT%type := p_cert;
    
    l_number NUMBER := 0; 
    
    l_sql varchar2(2000) := 'INSERT INTO
      RR_RUNS (
        RUN_NUMBER,
        RUN_SHIFTER,
        RUN_EVENTS,
        RUN_RATE,
        RUN_START_TIME,
        RUN_END_TIME,
        RUN_L1KEY,
        RUN_HLTKEY,
        RUN_CREATE_TIME,
        RUN_CREATE_USER,
        RUN_GLOBALNAME,
        RUN_LAST_USER,
        RUN_LAST_CERT
      ) values (
        :1,
        :2,
        :3,
        :4,
        :5,
        :6,
        :7,
        :8,
        :9,
        :10,
        :11,
        :12,
        :13
      )';

  BEGIN

    set transaction name 'new run';

    select count(*) into l_number from rr_runs where run_number = l_RUN_NUMBER;
    if l_number > 0 then
      rollback;
      return 2;
    end if;

    begin
    
      l_RUN_EVENTS := ri_events(l_RUN_NUMBER);
      l_RUN_START_TIME := ri_start_time(l_RUN_NUMBER);
      l_RUN_RATE := round(ri_rate(l_RUN_NUMBER), 0);
      l_RUN_L1KEY := ri_l1key(l_RUN_NUMBER);
      l_RUN_HLTKEY := ri_hltkey(l_RUN_NUMBER);
      -- ri_lastentries(l_RUN_SHIFTER, l_RUN_GLOBALNAME);
      l_RUN_END_TIME := ri_end_time(l_RUN_NUMBER);
      
    exception when NO_DATA_FOUND then
      rollback;
      RETURN 1;
    end;
    
    execute immediate l_sql using
        l_RUN_NUMBER,
        l_RUN_SHIFTER,
        l_RUN_EVENTS,
        l_RUN_RATE,
        l_RUN_START_TIME,
        l_RUN_END_TIME,
        l_RUN_L1KEY,
        l_RUN_HLTKEY,
        l_RUN_CREATE_TIME,
        l_RUN_CREATE_USER,
        l_RUN_GLOBALNAME,
        l_RUN_LAST_USER,
        l_RUN_LAST_CERT;
    
        -- Insert existing subsystems
/ *
    insert into 
      RR_RUN_SUBSYSTEMS 
      (RSU_RUN_NUMBER, RSU_SUB_ABBR, RSU_VALUE)
    select 
      runsession_parameter.runnumber,
      rr_subsystems.sub_abbr,
      'GOOD'
    from 
      runsession_parameter join rr_subsystems on (runsession_parameter.name = rr_subsystems.sub_runsession_parameter_name) 
    where 
      runsession_parameter.string_value = 'In' and
      runsession_parameter.runnumber = l_RUN_NUMBER;
* /

    commit;
    RETURN 0;
    
  exception when NO_DATA_FOUND then
    rollback;
    RETURN 1;
  END create_run;
*/

END RR_RUNINFO;

/

  GRANT EXECUTE ON "RR_RUNINFO" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for Package Body RR_UTILS
--------------------------------------------------------

  CREATE OR REPLACE PACKAGE BODY "RR_UTILS" AS

  FUNCTION get_subsystems(run in number) return varchar2 IS
    ret VARCHAR2(10000) := '';
    cursor c is 
      select 
        rsu_sub_abbr 
      from 
        rr_run_subsystems join rr_runs on (rr_runs.run_id = rr_run_subsystems.rsu_run_id) 
      where 
        run_number = run 
      order by rsu_sub_abbr; -- and rsu_value not in ('NOTSET');
  BEGIN
    for p in c loop
      if length(ret) > 0 then
        ret := ret || ', ';
      end if;
      ret := ret || p.rsu_sub_abbr;
    end loop;
    return ret;
  END;
  
  FUNCTION get_l1sources(run in number) return varchar2 IS
    ret VARCHAR2(1000) := '';
    cursor c is select rl1_l1s_abbr from rr_run_l1sources join rr_runs on (rr_run_l1sources.rl1_run_id = rr_runs.run_id) where run_number = run;
  BEGIN
    for p in c loop
      if length(ret) > 0 then
        ret := ret || ', ';
      end if;
      ret := ret || p.rl1_l1s_abbr;
    end loop;
    return ret;
  END;

  PROCEDURE audit_preload AS
  begin
    null;
/*
    DELETE FROM RR_RUN_L1SOURCES$A;
    DELETE FROM rr_run_subsystems$a;
    DELETE FROM rr_runs$a;
    INSERT INTO RR_RUN_L1SOURCES$A select rr_run_l1sources.*, sysdate, 'A' from RR_RUN_L1SOURCES;
    INSERT INTO rr_run_subsystems$a select rr_run_subsystems.*, sysdate, 'A' from rr_run_subsystems;
    INSERT INTO rr_runs$a a select  b.*, sysdate, 'A' from rr_runs b;
    commit;
*/
  end;


  FUNCTION date2unix(d in date) return number as
    n NUMBER := (d - to_date('01-JAN-1970','DD-MON-YYYY')) * (86400);
  begin
    return n;
  end;

  FUNCTION unix2date(n in number) return date as
    d date := TO_DATE('19700101000000','YYYYMMDDHH24MISS') + NUMTODSINTERVAL(n, 'SECOND');
  begin
    return d;
  end;

END RR_UTILS;

/

  GRANT EXECUTE ON "RR_UTILS" TO "CMS_DQM_RUN_REGISTRY_W";
 

--------------------------------------------------------
--  DDL for Synonymn RUNSESSION_PARAMETER
--------------------------------------------------------

  CREATE OR REPLACE SYNONYM "RUNSESSION_PARAMETER" FOR "CMS_RUNINFO"."RUNSESSION_PARAMETER";
