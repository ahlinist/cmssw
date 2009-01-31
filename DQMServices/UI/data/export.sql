--------------------------------------------------------
--  File created - Saturday-January-31-2009   
--------------------------------------------------------
--------------------------------------------------------
--  DDL for Table RR_L1SOURCES
--------------------------------------------------------

  CREATE TABLE "RR_L1SOURCES" 
   (	"L1S_ABBR" VARCHAR2(10), 
	"L1S_NAME" VARCHAR2(100)
   ) ;
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
--------------------------------------------------------
--  DDL for Table RR_RUNS
--------------------------------------------------------

  CREATE TABLE "RR_RUNS" 
   (	"RUN_NUMBER" NUMBER(10,0), 
	"RUN_SHIFTER" VARCHAR2(100), 
	"RUN_EVENTS" NUMBER(10,0), 
	"RUN_RATE" NUMBER(10,2), 
	"RUN_START_TIME" DATE, 
	"RUN_END_TIME" DATE, 
	"RUN_STOP_REASON" VARCHAR2(1000), 
	"RUN_L1KEY" VARCHAR2(200), 
	"RUN_HLTKEY" VARCHAR2(200), 
	"RUN_INDBS" VARCHAR2(1) DEFAULT 'N', 
	"RUN_CREATE_TIME" DATE, 
	"RUN_CREATE_USER" VARCHAR2(30), 
	"RUN_COMMENT" VARCHAR2(1000), 
	"RUN_GLOBALNAME" VARCHAR2(20), 
	"RUN_OFFLINE_COMMENT" VARCHAR2(1000), 
	"RUN_STATUS" VARCHAR2(10) DEFAULT 'ONLINE', 
	"RUN_LAST_USER" VARCHAR2(100), 
	"RUN_LAST_CERT" CLOB, 
	"RUN_BFIELD" NUMBER(6,5), 
	"RUN_BFIELD_COMMENT" VARCHAR2(100)
   ) ;
--------------------------------------------------------
--  DDL for Table RR_RUNS$A
--------------------------------------------------------

  CREATE TABLE "RR_RUNS$A" 
   (	"RUN_NUMBER" NUMBER(10,0), 
	"RUN_SHIFTER" VARCHAR2(100), 
	"RUN_EVENTS" NUMBER(10,0), 
	"RUN_RATE" NUMBER(10,2), 
	"RUN_START_TIME" DATE, 
	"RUN_END_TIME" DATE, 
	"RUN_STOP_REASON" VARCHAR2(200), 
	"RUN_L1KEY" VARCHAR2(200), 
	"RUN_HLTKEY" VARCHAR2(200), 
	"RUN_INDBS" VARCHAR2(1) DEFAULT 'N', 
	"RUN_CREATE_TIME" DATE, 
	"RUN_CREATE_USER" VARCHAR2(30), 
	"RUN_COMMENT" VARCHAR2(1000), 
	"RUN_GLOBALNAME" VARCHAR2(20), 
	"RUN_OFFLINE_COMMENT" VARCHAR2(1000), 
	"RUN_STATUS" VARCHAR2(10), 
	"RUN_ACTION_DATE" DATE, 
	"RUN_ACTION_TYPE" VARCHAR2(1), 
	"RUN_LAST_USER" VARCHAR2(100), 
	"RUN_LAST_CERT" CLOB, 
	"RUN_BFIELD" NUMBER(6,5), 
	"RUN_BFIELD_COMMENT" VARCHAR2(100)
   ) ;
--------------------------------------------------------
--  DDL for Table RR_RUNS_VERSIONS
--------------------------------------------------------

  CREATE TABLE "RR_RUNS_VERSIONS" 
   (	"RUV_NUMBER" NUMBER(10,0), 
	"RUV_SHIFTER" VARCHAR2(100), 
	"RUV_EVENTS" NUMBER(10,0), 
	"RUV_RATE" NUMBER(10,2), 
	"RUV_START_TIME" DATE, 
	"RUV_END_TIME" DATE, 
	"RUV_STOP_REASON" VARCHAR2(1000), 
	"RUV_L1KEY" VARCHAR2(200), 
	"RUV_HLTKEY" VARCHAR2(200), 
	"RUV_INDBS" VARCHAR2(1), 
	"RUV_CREATE_TIME" DATE, 
	"RUV_CREATE_USER" VARCHAR2(30), 
	"RUV_COMMENT" VARCHAR2(1000), 
	"RUV_GLOBALNAME" VARCHAR2(20), 
	"RUV_OFFLINE_COMMENT" VARCHAR2(1000), 
	"RUV_STATUS" VARCHAR2(10), 
	"RUV_LAST_USER" VARCHAR2(100), 
	"RUV_LAST_CERT" CLOB, 
	"RUV_BFIELD" NUMBER(6,5), 
	"RUV_BFIELD_COMMENT" VARCHAR2(100), 
	"RUV_ID" NUMBER(10,0)
   ) ;
--------------------------------------------------------
--  DDL for Table RR_RUN_L1SOURCES
--------------------------------------------------------

  CREATE TABLE "RR_RUN_L1SOURCES" 
   (	"RL1_RUN_NUMBER" NUMBER(10,0), 
	"RL1_L1S_ABBR" VARCHAR2(10), 
	"RL1_COMMENT" VARCHAR2(100)
   ) ;
--------------------------------------------------------
--  DDL for Table RR_RUN_L1SOURCES$A
--------------------------------------------------------

  CREATE TABLE "RR_RUN_L1SOURCES$A" 
   (	"RL1_RUN_NUMBER" NUMBER(10,0), 
	"RL1_L1S_ABBR" VARCHAR2(10), 
	"RL1_COMMENT" VARCHAR2(100), 
	"RL1_ACTION_DATE" DATE, 
	"RL1_ACTION_TYPE" VARCHAR2(1)
   ) ;
--------------------------------------------------------
--  DDL for Table RR_RUN_L1SOURCES_VERSIONS
--------------------------------------------------------

  CREATE TABLE "RR_RUN_L1SOURCES_VERSIONS" 
   (	"RL1_RUN_NUMBER" NUMBER(10,0), 
	"RL1_L1S_ABBR" VARCHAR2(10), 
	"RL1_COMMENT" VARCHAR2(100)
   ) ;
--------------------------------------------------------
--  DDL for Table RR_RUN_SUBSYSTEMS
--------------------------------------------------------

  CREATE TABLE "RR_RUN_SUBSYSTEMS" 
   (	"RSU_RUN_NUMBER" NUMBER(10,0), 
	"RSU_SUB_ABBR" VARCHAR2(10), 
	"RSU_COMMENT" VARCHAR2(100), 
	"RSU_VALUE" VARCHAR2(10) DEFAULT 'GOOD'
   ) ;
--------------------------------------------------------
--  DDL for Table RR_RUN_SUBSYSTEMS$A
--------------------------------------------------------

  CREATE TABLE "RR_RUN_SUBSYSTEMS$A" 
   (	"RSU_RUN_NUMBER" NUMBER(10,0), 
	"RSU_SUB_ABBR" VARCHAR2(10), 
	"RSU_COMMENT" VARCHAR2(100), 
	"RSU_VALUE" VARCHAR2(10), 
	"RSU_ACTION_DATE" DATE, 
	"RSU_ACTION_TYPE" VARCHAR2(1)
   ) ;
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
--------------------------------------------------------
--  Constraints for Table RR_RUNINFO_CACHE
--------------------------------------------------------

  ALTER TABLE "RR_RUNINFO_CACHE" ADD CONSTRAINT "CMS_RUNINFO_CACHE_CHK1" CHECK (
(CSC='GOOD' or CSC IS NULL) and (DT='GOOD' or DT IS NULL) and (ECAL='GOOD' or ECAL IS NULL) and (HCAL='GOOD' or HCAL IS NULL) and (TRG='GOOD' or TRG IS NULL) and (PIX='GOOD' or PIX IS NULL) and (RPC='GOOD' or RPC IS NULL) and (SIST='GOOD' or SIST IS NULL) and (SCAL='GOOD' or SCAL IS NULL)
) ENABLE;
 
  ALTER TABLE "RR_RUNINFO_CACHE" ADD CONSTRAINT "CMS_RUNINFO_CACHE_PK" PRIMARY KEY ("RUN_NUMBER") ENABLE;
 
  ALTER TABLE "RR_RUNINFO_CACHE" MODIFY ("RUN_NUMBER" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table RR_RUN_L1SOURCES_VERSIONS
--------------------------------------------------------

  ALTER TABLE "RR_RUN_L1SOURCES_VERSIONS" MODIFY ("RL1_RUN_NUMBER" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUN_L1SOURCES_VERSIONS" MODIFY ("RL1_L1S_ABBR" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table RR_RUNS_VERSIONS
--------------------------------------------------------

  ALTER TABLE "RR_RUNS_VERSIONS" ADD CONSTRAINT "RR_RUNS_VERSIONS_PK" PRIMARY KEY ("RUV_ID") ENABLE;
 
  ALTER TABLE "RR_RUNS_VERSIONS" MODIFY ("RUV_NUMBER" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS_VERSIONS" MODIFY ("RUV_SHIFTER" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS_VERSIONS" MODIFY ("RUV_EVENTS" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS_VERSIONS" MODIFY ("RUV_RATE" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS_VERSIONS" MODIFY ("RUV_START_TIME" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS_VERSIONS" MODIFY ("RUV_L1KEY" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS_VERSIONS" MODIFY ("RUV_HLTKEY" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS_VERSIONS" MODIFY ("RUV_INDBS" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS_VERSIONS" MODIFY ("RUV_CREATE_TIME" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS_VERSIONS" MODIFY ("RUV_CREATE_USER" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS_VERSIONS" MODIFY ("RUV_GLOBALNAME" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS_VERSIONS" MODIFY ("RUV_STATUS" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS_VERSIONS" MODIFY ("RUV_ID" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table RR_RUN_SUBSYSTEMS
--------------------------------------------------------

  ALTER TABLE "RR_RUN_SUBSYSTEMS" ADD CONSTRAINT "RR_RUN_SUBSYSTEMS_CHK1" CHECK (
RSU_VALUE IN ('GOOD', 'BAD', 'NOTSET', 'EXCL')
) ENABLE;
 
  ALTER TABLE "RR_RUN_SUBSYSTEMS" ADD CONSTRAINT "RR_RUN_SUBSYSTEMS_PK" PRIMARY KEY ("RSU_RUN_NUMBER", "RSU_SUB_ABBR") ENABLE;
 
  ALTER TABLE "RR_RUN_SUBSYSTEMS" MODIFY ("RSU_RUN_NUMBER" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUN_SUBSYSTEMS" MODIFY ("RSU_SUB_ABBR" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUN_SUBSYSTEMS" MODIFY ("RSU_VALUE" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table RR_RUN_L1SOURCES
--------------------------------------------------------

  ALTER TABLE "RR_RUN_L1SOURCES" ADD CONSTRAINT "RR_RUN_L1SOURCES_PK" PRIMARY KEY ("RL1_RUN_NUMBER", "RL1_L1S_ABBR") ENABLE;
 
  ALTER TABLE "RR_RUN_L1SOURCES" MODIFY ("RL1_RUN_NUMBER" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUN_L1SOURCES" MODIFY ("RL1_L1S_ABBR" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table RR_L1SOURCES
--------------------------------------------------------

  ALTER TABLE "RR_L1SOURCES" ADD CONSTRAINT "RR_L1SOURCES_PK" PRIMARY KEY ("L1S_ABBR") ENABLE;
 
  ALTER TABLE "RR_L1SOURCES" MODIFY ("L1S_ABBR" NOT NULL ENABLE);
--------------------------------------------------------
--  Constraints for Table RR_RUNS
--------------------------------------------------------

  ALTER TABLE "RR_RUNS" ADD CONSTRAINT "RR_RUNS_CHK1" CHECK (
RUN_STATUS IN ('ONLINE', 'OFFLINE', 'SIGNOFF', 'COMPLETED')
) ENABLE;
 
  ALTER TABLE "RR_RUNS" ADD CONSTRAINT "RR_RUNS_INDBS_CHK" CHECK (
RUN_INDBS IN ('Y','N')
) ENABLE;
 
  ALTER TABLE "RR_RUNS" ADD CONSTRAINT "RR_RUNS_PK" PRIMARY KEY ("RUN_NUMBER") ENABLE;
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_NUMBER" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_SHIFTER" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_EVENTS" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_RATE" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_START_TIME" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_L1KEY" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_HLTKEY" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_INDBS" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_CREATE_TIME" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_CREATE_USER" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_GLOBALNAME" NOT NULL ENABLE);
 
  ALTER TABLE "RR_RUNS" MODIFY ("RUN_STATUS" NOT NULL ENABLE);
--------------------------------------------------------
--  DDL for Index RR_SUBSYSTEMS_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "RR_SUBSYSTEMS_PK" ON "RR_SUBSYSTEMS" ("SUB_ABBR") 
  ;
--------------------------------------------------------
--  DDL for Index CMS_RUNINFO_CACHE_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "CMS_RUNINFO_CACHE_PK" ON "RR_RUNINFO_CACHE" ("RUN_NUMBER") 
  ;
--------------------------------------------------------
--  DDL for Index RR_RUNS_INDEX1
--------------------------------------------------------

  CREATE INDEX "RR_RUNS_INDEX1" ON "RR_RUNS" ("RUN_SHIFTER") 
  ;
--------------------------------------------------------
--  DDL for Index RR_RUN_L1SOURCES_INDEX1
--------------------------------------------------------

  CREATE INDEX "RR_RUN_L1SOURCES_INDEX1" ON "RR_RUN_L1SOURCES" ("RL1_L1S_ABBR") 
  ;
--------------------------------------------------------
--  DDL for Index RR_L1SOURCES_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "RR_L1SOURCES_PK" ON "RR_L1SOURCES" ("L1S_ABBR") 
  ;
--------------------------------------------------------
--  DDL for Index RR_RUN_SUBSYSTEMS_INDEX1
--------------------------------------------------------

  CREATE INDEX "RR_RUN_SUBSYSTEMS_INDEX1" ON "RR_RUN_SUBSYSTEMS" ("RSU_SUB_ABBR") 
  ;
--------------------------------------------------------
--  DDL for Index RR_RUN_L1SOURCES_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "RR_RUN_L1SOURCES_PK" ON "RR_RUN_L1SOURCES" ("RL1_RUN_NUMBER", "RL1_L1S_ABBR") 
  ;
--------------------------------------------------------
--  DDL for Index RR_RUNS_VERSIONS_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "RR_RUNS_VERSIONS_PK" ON "RR_RUNS_VERSIONS" ("RUV_ID") 
  ;
--------------------------------------------------------
--  DDL for Index RR_RUNS_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "RR_RUNS_PK" ON "RR_RUNS" ("RUN_NUMBER") 
  ;
--------------------------------------------------------
--  DDL for Index RR_RUN_SUBSYSTEMS_PK
--------------------------------------------------------

  CREATE UNIQUE INDEX "RR_RUN_SUBSYSTEMS_PK" ON "RR_RUN_SUBSYSTEMS" ("RSU_RUN_NUMBER", "RSU_SUB_ABBR") 
  ;
--------------------------------------------------------
--  Ref Constraints for Table RR_RUN_L1SOURCES
--------------------------------------------------------

  ALTER TABLE "RR_RUN_L1SOURCES" ADD CONSTRAINT "RR_RUN_L1SOURCES_RR_L1SOU_FK1" FOREIGN KEY ("RL1_L1S_ABBR")
	  REFERENCES "RR_L1SOURCES" ("L1S_ABBR") ENABLE;
 
  ALTER TABLE "RR_RUN_L1SOURCES" ADD CONSTRAINT "RR_RUN_L1SOURCES_RR_RUNS_FK1" FOREIGN KEY ("RL1_RUN_NUMBER")
	  REFERENCES "RR_RUNS" ("RUN_NUMBER") ENABLE;
--------------------------------------------------------
--  Ref Constraints for Table RR_RUN_SUBSYSTEMS
--------------------------------------------------------

  ALTER TABLE "RR_RUN_SUBSYSTEMS" ADD CONSTRAINT "RR_RUN_SUBSYSTEMS_RR_RUNS_FK1" FOREIGN KEY ("RSU_RUN_NUMBER")
	  REFERENCES "RR_RUNS" ("RUN_NUMBER") ENABLE;
 
  ALTER TABLE "RR_RUN_SUBSYSTEMS" ADD CONSTRAINT "RR_RUN_SUBSYSTEMS_RR_SUBS_FK1" FOREIGN KEY ("RSU_SUB_ABBR")
	  REFERENCES "RR_SUBSYSTEMS" ("SUB_ABBR") ENABLE;
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
ALTER TRIGGER "RR_TRG_RUNINFO_CACHE_DATE" ENABLE;
--------------------------------------------------------
--  DDL for Trigger RR_TRG_RUNS$A
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "RR_TRG_RUNS$A" 
AFTER INSERT OR DELETE OR UPDATE ON RR_RUNS
REFERENCING OLD AS old NEW AS new 
FOR EACH ROW 
DECLARE
  l_sql varchar2(2000) := 'INSERT INTO RR_RUNS$A VALUES (:1, :2, :3, :4, :5, :6, :7, :8, :9, :10, :11, :12, :13, :14, :15, :16, sysdate, :17, :18, :19, :20, :21)';
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
    execute immediate l_sql using 
      :old.RUN_NUMBER, 
      :old.RUN_SHIFTER, 
      :old.RUN_EVENTS, 
      :old.RUN_RATE, 
      :old.RUN_START_TIME, 
      :old.RUN_END_TIME, 
      :old.RUN_STOP_REASON, 
      :old.RUN_L1KEY, 
      :old.RUN_HLTKEY, 
      :old.RUN_INDBS, 
      :old.RUN_CREATE_TIME, 
      :old.RUN_CREATE_USER, 
      :old.RUN_COMMENT, 
      :old.RUN_GLOBALNAME, 
      :old.RUN_OFFLINE_COMMENT, 
      :old.RUN_STATUS, 
      l_action,
      :old.RUN_LAST_USER,
      :old.RUN_LAST_CERT,
      :old.RUN_BFIELD,
      :old.RUN_BFIELD_COMMENT;
  else
    execute immediate l_sql using 
      :new.RUN_NUMBER, 
      :new.RUN_SHIFTER, 
      :new.RUN_EVENTS, 
      :new.RUN_RATE, 
      :new.RUN_START_TIME, 
      :new.RUN_END_TIME, 
      :new.RUN_STOP_REASON, 
      :new.RUN_L1KEY, 
      :new.RUN_HLTKEY, 
      :new.RUN_INDBS, 
      :new.RUN_CREATE_TIME, 
      :new.RUN_CREATE_USER, 
      :new.RUN_COMMENT, 
      :new.RUN_GLOBALNAME, 
      :new.RUN_OFFLINE_COMMENT, 
      :new.RUN_STATUS, 
      l_action,
      :new.RUN_LAST_USER,
      :new.RUN_LAST_CERT,
      :new.RUN_BFIELD,
      :new.RUN_BFIELD_COMMENT;
  end if;
  
END;


/
ALTER TRIGGER "RR_TRG_RUNS$A" ENABLE;
--------------------------------------------------------
--  DDL for Trigger RR_TRG_RUNS_IN_DBS
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "RR_TRG_RUNS_IN_DBS" 
BEFORE INSERT OR UPDATE OF RUN_STATUS ON RR_RUNS
REFERENCING OLD AS old NEW AS new 
FOR EACH ROW 
BEGIN
  if :new.run_status = 'COMPLETED' then
    :new.run_indbs := 'Y';
  else
    :new.run_indbs := 'N';
  end if;
END;


/
ALTER TRIGGER "RR_TRG_RUNS_IN_DBS" ENABLE;
--------------------------------------------------------
--  DDL for Trigger RR_TRG_RUN_L1SOURCES$A
--------------------------------------------------------

  CREATE OR REPLACE TRIGGER "RR_TRG_RUN_L1SOURCES$A" 
AFTER INSERT OR DELETE OR UPDATE ON RR_RUN_L1SOURCES
REFERENCING OLD AS old NEW AS new 
FOR EACH ROW 
DECLARE
  l_sql varchar2(2000) := 'INSERT INTO RR_RUN_L1SOURCES$A VALUES (:1, :2, :3, sysdate, :4)';
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
    execute immediate l_sql using 
      :old.RL1_RUN_NUMBER, 
      :old.RL1_L1S_ABBR, 
      :old.RL1_COMMENT, 
      l_action;
  else
    execute immediate l_sql using 
      :new.RL1_RUN_NUMBER, 
      :new.RL1_L1S_ABBR, 
      :new.RL1_COMMENT, 
      l_action;
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
  l_sql varchar2(2000) := 'INSERT INTO RR_RUN_SUBSYSTEMS$A VALUES (:1, :2, :3, :4, sysdate, :5)';
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
    execute immediate l_sql using 
      :old.RSU_RUN_NUMBER, 
      :old.RSU_SUB_ABBR, 
      :old.RSU_COMMENT, 
      :old.RSU_VALUE, 
      l_action;
  else
    execute immediate l_sql using 
      :new.RSU_RUN_NUMBER, 
      :new.RSU_SUB_ABBR, 
      :new.RSU_COMMENT, 
      :new.RSU_VALUE, 
      l_action;
  end if;
  
END;

/
ALTER TRIGGER "RR_TRG_RUN_SUBSYSTEMS$A" ENABLE;
--------------------------------------------------------
--  DDL for View RI_RUN_DATA
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "RI_RUN_DATA" ("RUN_NUMBER", "RUN_GLOBALNAME", "RUN_INDBS", "RUN_STATUS", "RUN_BFIELD", "RUN_BFIELD_COMMENT", "RUN_STOP_REASON", "RUN_COMMENT", "RUN_OFFLINE_COMMENT", "RUN_CREATE_TIME", "RUN_CREATE_USER", "RUN_SHIFTER", "RUN_L1SOURCES", "RSU_RUN_NUMBER", "RUN_EVENTS", "RUN_START_TIME", "RUN_END_TIME", "RUN_RATE", "RUN_L1KEY", "RUN_HLTKEY", "CSC", "DT", "ECAL", "HCAL", "TRG", "PIX", "RPC", "SIST", "SCAL", "JMET", "EGAM", "MUON", "CSC_comment", "DT_comment", "ECAL_comment", "HCAL_comment", "TRG_comment", "PIX_comment", "RPC_comment", "SIST_comment", "JMET_comment", "EGAM_comment", "MUON_comment", "SCAL_comment", "l1s_CSC", "l1s_DT", "l1s_ECAL", "l1s_HCAL", "l1s_Random", "l1s_RPC", "l1s_Other", "l1s_CSC_comment", "l1s_DT_comment", "l1s_ECAL_comment", "l1s_HCAL_comment", "l1s_Random_comment", "l1s_RPC_comment", "l1s_Other_comment") AS 
  select 
  a.RUN_NUMBER,
  b.RUN_GLOBALNAME,
  b.RUN_INDBS,
  b.RUN_STATUS,
  b.RUN_BFIELD,
  b.run_bfield_comment,
  b.RUN_STOP_REASON,
  b.RUN_COMMENT,
  b.RUN_OFFLINE_COMMENT,
  b.RUN_CREATE_TIME, 
  b.RUN_CREATE_USER,
  b.RUN_SHIFTER,
  b.RUN_L1SOURCES,
  b.RSU_RUN_NUMBER,
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

  CREATE OR REPLACE FORCE VIEW "RR_DBSFLAGS" ("run", "tag", "value") AS 
  select 
  run_number "run", 
  sub_dbs_flag "tag", 
  rsu_value "value" 
from 
  rr_runs 
  join rr_run_subsystems on (run_number = rsu_run_number) 
  join rr_subsystems on (sub_abbr = rsu_sub_abbr) 
where 
  sub_dbs_flag is not null and
  rsu_value in ('GOOD', 'BAD');
--------------------------------------------------------
--  DDL for View RR_L1S_DATA
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "RR_L1S_DATA" ("RL1_RUN_NUMBER", "l1s_CSC", "l1s_DT", "l1s_ECAL", "l1s_HCAL", "l1s_Random", "l1s_RPC", "l1s_Other", "l1s_CSC_comment", "l1s_DT_comment", "l1s_ECAL_comment", "l1s_HCAL_comment", "l1s_Random_comment", "l1s_RPC_comment", "l1s_Other_comment") AS 
  select 
  rl1_run_number, 

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
  rl1_run_number;
--------------------------------------------------------
--  DDL for View RR_L1S_XML
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "RR_L1S_XML" ("RL1_RUN_NUMBER", "RL1_XML") AS 
  select 
  rl1_run_number,
  XMLElement("l1sources",
    XMLAgg(
      XMLElement("l1source",
        XMLForest(
          rl1_l1s_abbr as "abbr", 
          l1s_name as "name",
          rl1_comment as "comment"
        )
      )
    )
  ) as rl1_xml
from 
  rr_run_l1sources join rr_l1sources on (rl1_l1s_abbr = l1s_abbr)
group by 
  rl1_run_number
 ;
--------------------------------------------------------
--  DDL for View RR_RUNINFO_CACHE_TO_INSERT
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "RR_RUNINFO_CACHE_TO_INSERT" ("RUN_NUMBER", "RUN_EVENTS", "RUN_START_TIME", "RUN_END_TIME", "RUN_RATE", "RUN_L1KEY", "RUN_HLTKEY", "CSC", "DT", "ECAL", "HCAL", "TRG", "PIX", "RPC", "SIST", "SCAL", "currdate") AS 
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

  CREATE OR REPLACE FORCE VIEW "RR_RUNINFO_CACHE_TO_UPDATE" ("RUN_NUMBER", "RUN_EVENTS", "RUN_START_TIME", "RUN_END_TIME", "RUN_RATE", "RUN_L1KEY", "RUN_HLTKEY", "CSC", "DT", "ECAL", "HCAL", "TRG", "PIX", "RPC", "SIST", "SCAL") AS 
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

  CREATE OR REPLACE FORCE VIEW "RR_RUNINFO_SUBS_TO_DELETE" ("RSU_RUN_NUMBER", "RSU_SUB_ABBR", "RSU_COMMENT", "RSU_VALUE") AS 
  select 
  a."RSU_RUN_NUMBER",a."RSU_SUB_ABBR",a."RSU_COMMENT",a."RSU_VALUE" 
from 
  rr_run_subsystems a,
  rr_subsystems b
where
  a.rsu_sub_abbr = b.sub_abbr and
  b.sub_type = 'ONLINE' and
  (rsu_run_number, rsu_sub_abbr) not in (select distinct a.runnumber "number", b.sub_abbr "sub" from runsession_parameter a, rr_subsystems b where a.string_value = 'In' and a.name = b.sub_runsession_parameter_name)
  ;
--------------------------------------------------------
--  DDL for View RR_RUNINFO_SUBS_TO_INSERT
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "RR_RUNINFO_SUBS_TO_INSERT" ("number", "sub", "value") AS 
  select distinct 
  a.runnumber "number",
  b.sub_abbr "sub",
  'GOOD' "value"
from 
  runsession_parameter a,
  rr_subsystems b,
  rr_runs c,
  rr_run_subsystems d
where 
  a.string_value = 'In' and
  a.name = b.sub_runsession_parameter_name and
  a.runnumber = c.run_number and
  (a.runnumber, b.sub_abbr) not in (select rsu_run_number, rsu_sub_abbr from rr_run_subsystems)
  ;
--------------------------------------------------------
--  DDL for View RR_RUNINFO_SUBS_TO_UPDATE
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "RR_RUNINFO_SUBS_TO_UPDATE" ("number", "sub", "value") AS 
  select distinct 
  a.runnumber "number",
  b.sub_abbr "sub",
  rsu_value "value"
from 
  runsession_parameter a,
  rr_subsystems b,
  rr_run_subsystems c
where 
  a.string_value = 'In' and
  a.name = b.sub_runsession_parameter_name and
  a.runnumber = c.rsu_run_number and
  b.sub_abbr = c.rsu_sub_abbr and
  (c.rsu_value <> 'GOOD' or 
  (c.rsu_value = 'BAD' and c.rsu_comment is null))
  
  ;
--------------------------------------------------------
--  DDL for View RR_RUNS_TO_OFFLINE
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "RR_RUNS_TO_OFFLINE" ("run_number") AS 
  select 
  r.run_number
from 
  rr_runs r 
  join rr_run_mtime m on (m."run_number" = r.run_number) 
where 
  r.run_end_time is not null and 
  r.run_status = 'ONLINE' and
  ((sysdate - m."mtime") > (1 / 24));
--------------------------------------------------------
--  DDL for View RR_RUN_DATA
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "RR_RUN_DATA" ("RUN_GLOBALNAME", "RUN_NUMBER", "RUN_EVENTS", "RUN_RATE", "RUN_INDBS", "RUN_STATUS", "RUN_BFIELD", "RUN_BFIELD_COMMENT", "RUN_START_TIME", "RUN_END_TIME", "RUN_STOP_REASON", "RUN_L1KEY", "RUN_HLTKEY", "RUN_COMMENT", "RUN_OFFLINE_COMMENT", "RUN_CREATE_TIME", "RUN_CREATE_USER", "RUN_SHIFTER", "RUN_L1SOURCES", "RSU_RUN_NUMBER", "CSC", "DT", "ECAL", "HCAL", "TRG", "PIX", "RPC", "SIST", "JMET", "EGAM", "MUON", "SCAL", "CSC_comment", "DT_comment", "ECAL_comment", "HCAL_comment", "TRG_comment", "PIX_comment", "RPC_comment", "SIST_comment", "JMET_comment", "EGAM_comment", "MUON_comment", "SCAL_comment", "RL1_RUN_NUMBER", "l1s_CSC", "l1s_DT", "l1s_ECAL", "l1s_HCAL", "l1s_Random", "l1s_RPC", "l1s_Other", "l1s_CSC_comment", "l1s_DT_comment", "l1s_ECAL_comment", "l1s_HCAL_comment", "l1s_Random_comment", "l1s_RPC_comment", "l1s_Other_comment") AS 
  SELECT
   RUN_GLOBALNAME,
   RUN_NUMBER, 
	 RUN_EVENTS, 
	 RUN_RATE, 
   RUN_INDBS,
   RUN_STATUS,
   RUN_BFIELD,
   run_bfield_comment,
   TO_CHAR(RUN_START_TIME, 'YYYY.MM.DD HH24:MI:SS') as RUN_START_TIME, 
   TO_CHAR(RUN_END_TIME, 'YYYY.MM.DD HH24:MI:SS') as RUN_END_TIME,
	 RUN_STOP_REASON, 
	 RUN_L1KEY, 
   RUN_HLTKEY, 
   RUN_COMMENT,
   RUN_OFFLINE_COMMENT,
   TO_CHAR(RUN_CREATE_TIME, 'YYYY.MM.DD HH24:MI:SS') as RUN_CREATE_TIME,  
   RUN_CREATE_USER,
   RUN_SHIFTER,
   RR_UTILS.get_l1sources(RUN_NUMBER) as RUN_L1SOURCES,
   s.*,
   l.*
from 
  rr_runs r 
    left join rr_sub_data s on (RUN_NUMBER = rsu_run_number)
    left join rr_l1s_data l on (RUN_NUMBER = rl1_run_number);
--------------------------------------------------------
--  DDL for View RR_RUN_DATA_XML
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "RR_RUN_DATA_XML" ("XML", "RUN_NUMBER") AS 
  select 
  XMLElement("RUN",
    XMLElement("RUN_NUMBER","RUN_NUMBER"),
    XMLElement("RUN_GLOBALNAME","RUN_GLOBALNAME"),
    XMLElement("RUN_INDBS","RUN_INDBS"),
    XMLElement("RUN_STATUS","RUN_STATUS"),
    XMLElement("RUN_STOP_REASON","RUN_STOP_REASON"),
    XMLElement("RUN_COMMENT","RUN_COMMENT"),
    XMLElement("RUN_OFFLINE_COMMENT","RUN_OFFLINE_COMMENT"),
    XMLElement("RUN_CREATE_TIME","RUN_CREATE_TIME"),
    XMLElement("RUN_CREATE_USER","RUN_CREATE_USER"),
    XMLElement("RUN_SHIFTER","RUN_SHIFTER"),
    XMLElement("RUN_L1SOURCES","RUN_L1SOURCES"),
    XMLElement("RSU_RUN_NUMBER","RSU_RUN_NUMBER"),
    XMLElement("RUN_EVENTS","RUN_EVENTS"),
    XMLElement("RUN_START_TIME","RUN_START_TIME"),
    XMLElement("RUN_END_TIME","RUN_END_TIME"),
    XMLElement("RUN_RATE","RUN_RATE"),
    XMLElement("RUN_L1KEY","RUN_L1KEY"),
    XMLElement("RUN_HLTKEY","RUN_HLTKEY"),
    XMLElement("RUN_BFIELD","RUN_BFIELD"),
    XMLElement("RUN_BFIELD_COMMENT","RUN_BFIELD_COMMENT"),
    XMLElement("CSC","CSC"),
    XMLElement("DT","DT"),
    XMLElement("ECAL","ECAL"),
    XMLElement("HCAL","HCAL"),
    XMLElement("TRG","TRG"),
    XMLElement("PIX","PIX"),
    XMLElement("RPC","RPC"),
    XMLElement("SIST","SIST"),
    XMLElement("SCAL","SCAL"),
    XMLElement("JMET","JMET"),
    XMLElement("EGAM","EGAM"),
    XMLElement("MUON","MUON"),
    XMLElement("CSC_comment","CSC_comment"),
    XMLElement("DT_comment","DT_comment"),
    XMLElement("ECAL_comment","ECAL_comment"),
    XMLElement("HCAL_comment","HCAL_comment"),
    XMLElement("TRG_comment","TRG_comment"),
    XMLElement("PIX_comment","PIX_comment"),
    XMLElement("RPC_comment","RPC_comment"),
    XMLElement("SIST_comment","SIST_comment"),
    XMLElement("JMET_comment","JMET_comment"),
    XMLElement("EGAM_comment","EGAM_comment"),
    XMLElement("MUON_comment","MUON_comment"),
    XMLElement("SCAL_comment","SCAL_comment"),
    XMLElement("l1s_CSC","l1s_CSC"),
    XMLElement("l1s_DT","l1s_DT"),
    XMLElement("l1s_ECAL","l1s_ECAL"),
    XMLElement("l1s_HCAL","l1s_HCAL"),
    XMLElement("l1s_Random","l1s_Random"),
    XMLElement("l1s_RPC","l1s_RPC"),
    XMLElement("l1s_Other","l1s_Other"),
    XMLElement("l1s_CSC_comment","l1s_CSC_comment"),
    XMLElement("l1s_DT_comment","l1s_DT_comment"),
    XMLElement("l1s_ECAL_comment","l1s_ECAL_comment"),
    XMLElement("l1s_HCAL_comment","l1s_HCAL_comment"),
    XMLElement("l1s_Random_comment","l1s_Random_comment"),
    XMLElement("l1s_RPC_comment","l1s_RPC_comment"),
    XMLElement("l1s_Other_comment","l1s_Other_comment")
  ).getClobVal() as xml,
  RUN_NUMBER
from 
  (
select
    RUN_NUMBER,
    RUN_GLOBALNAME,
    RUN_INDBS,
    RUN_STATUS,
    RUN_STOP_REASON,
    RUN_COMMENT,
    RUN_OFFLINE_COMMENT,
    RUN_CREATE_TIME,
    RUN_CREATE_USER,
    RUN_SHIFTER,
    RUN_L1SOURCES,
    RSU_RUN_NUMBER,
    RUN_EVENTS,
    RUN_START_TIME,
    RUN_END_TIME,
    RUN_RATE,
    RUN_L1KEY,
    RUN_HLTKEY,
    RUN_BFIELD,
    RUN_BFIELD_COMMENT,
    CSC,
    DT,
    ECAL,
    HCAL,
    TRG,
    PIX,
    RPC,
    SIST,
    SCAL,
    JMET,
    EGAM,
    MUON,
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
FROM rr_run_data
UNION ALL
select 
  a.RUN_NUMBER,
  null as RUN_GLOBALNAME,
  null as RUN_INDBS,
  null as RUN_STATUS,
  null as RUN_STOP_REASON,
  null as RUN_COMMENT,
  null as RUN_OFFLINE_COMMENT,
  null as RUN_CREATE_TIME,
  null as RUN_CREATE_USER,
  null as RUN_SHIFTER,
  null as RUN_L1SOURCES,
  null as RSU_RUN_NUMBER,
  a.RUN_EVENTS,
	to_char(a.RUN_START_TIME, 'YYYY-MM-DD HH24:MI:SS') as RUN_START_TIME, 
  to_char(a.RUN_END_TIME, 'YYYY-MM-DD HH24:MI:SS') as RUN_END_TIME, 
  a.RUN_RATE,
  a.RUN_L1KEY,
  a.RUN_HLTKEY,
  null as RUN_BFIELD,
  null as RUN_BFIELD_COMMENT,
  a.CSC,
  a.DT,
  a.ECAL,
  a.HCAL,
  a.TRG,
  a.PIX,
  a.RPC,
  a.SIST,
  a.SCAL,
  null as JMET,
  null as EGAM,
  null as MUON,
  null as "CSC_comment",
  null as "DT_comment",
  null as "ECAL_comment",
  null as "HCAL_comment",
  null as "TRG_comment",
  null as "PIX_comment",
  null as "RPC_comment",
  null as "SIST_comment",
  null as "JMET_comment",
  null as "EGAM_comment",
  null as "MUON_comment",
  null as "SCAL_comment",
  null as "l1s_CSC",
  null as "l1s_DT",
  null as "l1s_ECAL",
  null as "l1s_HCAL",
  null as "l1s_Random",
  null as "l1s_RPC",
  null as "l1s_Other",
  null as "l1s_CSC_comment",
  null as "l1s_DT_comment",
  null as "l1s_ECAL_comment",
  null as "l1s_HCAL_comment",
  null as "l1s_Random_comment",
  null as "l1s_RPC_comment",
  null as "l1s_Other_comment"
from
  RR_RUNINFO_CACHE a
where 
  a.RUN_NUMBER not in (select run_number FROM rr_run_data)
  );
--------------------------------------------------------
--  DDL for View RR_RUN_MTIME
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "RR_RUN_MTIME" ("run_number", "mtime") AS 
  select
 a.n "run_number",
 greatest(a.t, b.t, c.t) "mtime"
from
  (select rl1_run_number n, max(rl1_action_date) t FROM rr_run_l1sources$a group by rl1_run_number) a,
  (select rsu_run_number n, max(rsu_action_date) t FROM rr_run_subsystems$a group by rsu_run_number) b,
  (select run_number n, max(run_action_date) t FROM rr_runs$a group by run_number) c
where
  a.n = b.n and a.n = c.n;
--------------------------------------------------------
--  DDL for View RR_SUB_DATA
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "RR_SUB_DATA" ("RSU_RUN_NUMBER", "CSC", "DT", "ECAL", "HCAL", "TRG", "PIX", "RPC", "SIST", "JMET", "EGAM", "MUON", "SCAL", "CSC_comment", "DT_comment", "ECAL_comment", "HCAL_comment", "TRG_comment", "PIX_comment", "RPC_comment", "SIST_comment", "JMET_comment", "EGAM_comment", "MUON_comment", "SCAL_comment") AS 
  select 
  rsu_run_number, 

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
  rsu_run_number;
--------------------------------------------------------
--  DDL for View RR_SUB_XML
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "RR_SUB_XML" ("RSU_RUN_NUMBER", "RSU_XML") AS 
  select 
  rsu_run_number,
  XMLElement("subsystems",
    XMLAgg(
      XMLElement("subsystem",
        XMLForest(
          rsu_sub_abbr as "abbr", 
          sub_name as "name",
          rsu_comment as "comment"
        )
      )
    )
  ) as rsu_xml
from 
  rr_run_subsystems join rr_subsystems on (rsu_sub_abbr = sub_abbr)
group by 
  rsu_run_number
 ;
--------------------------------------------------------
--  DDL for View RR_XMLDATA
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "RR_XMLDATA" ("XML", "RUN_NUMBER", "RUN_SHIFTER", "RUN_GLOBALNAME", "RUN_STATUS", "RUN_SUBSYSTEMS") AS 
  select XMLElement("run", 
	 XMLAttributes(
     RUN_NUMBER as "id"
  ),
  XMLForest(
    RUN_GLOBALNAME,
    RUN_NUMBER, 
	  RUN_EVENTS, 
	  RUN_RATE, 
    RUN_INDBS,
    RUN_STATUS,
	  TO_CHAR(RUN_START_TIME, 'YYYY.MM.DD HH24:MI:SS') as run_start_time, 
	  TO_CHAR(RUN_END_TIME, 'YYYY.MM.DD HH24:MI:SS') as run_end_time, 
    rr_utils.get_subsystems(RUN_NUMBER) as "subsystems",
    rr_utils.get_formatted_subsystems(RUN_NUMBER) as "formatted_subsystems",
    rr_utils.get_l1sources(RUN_NUMBER) as "l1sources",
	  RUN_STOP_REASON as "stop_reason", 
	  '<b>L1&nbsp;Key:</b>' || RUN_L1KEY || '<br>' ||
	  '<b>HLT&nbsp;Key:</b>' || RUN_HLTKEY || 
    rr_utils.get_comments(RUN_NUMBER) ||
	  DECODE(RUN_COMMENT, null, '', '<br>' || '<b>Online&nbsp;Comment:</b>' || RUN_COMMENT) ||
    DECODE(RUN_OFFLINE_COMMENT, null, '', '<br>' || '<b>Offline&nbsp;Comment:</b>' || RUN_OFFLINE_COMMENT) as "comment",
    TO_CHAR(RUN_CREATE_TIME, 'YYYY.MM.DD HH24:MI:SS') as "run_create_time", 
    RUN_CREATE_USER as "run_create_user",
    run_shifter,
    RUN_COMMENT,
    RUN_OFFLINE_COMMENT,
	  round(rr_utils.date2unix(RUN_START_TIME), 0) as RUN_START_TIME, 
	  round(rr_utils.date2unix(RUN_END_TIME), 0) as RUN_END_TIME
  )
).getClobVal() xml, 
r.RUN_NUMBER, 
r.RUN_SHIFTER, 
r.RUN_GLOBALNAME, 
r.RUN_STATUS,
rr_utils.get_subsystems(RUN_NUMBER) as "RUN_SUBSYSTEMS"
from 
  rr_runs r;
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

  function create_run(p_runnumber in number, p_username in VARCHAR2, p_globalname in VARCHAR2, p_cert in VARCHAR2) RETURN number;
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

--------------------------------------------------------
--  DDL for Package RR_UTILS
--------------------------------------------------------

  CREATE OR REPLACE PACKAGE "RR_UTILS" AS

  FUNCTION get_subsystems(run in number) return varchar2;
  FUNCTION get_formatted_subsystems(run in number) return varchar2;
  FUNCTION get_l1sources(run in number) return varchar2;
  FUNCTION get_comments(run in number) return varchar2;
  FUNCTION date2unix(d in date) return number;
  FUNCTION unix2date(n in number) return date;
  
  PROCEDURE audit_preload;

END RR_UTILS;

/

--------------------------------------------------------
--  DDL for Package Body RR_RUNINFO
--------------------------------------------------------

  CREATE OR REPLACE PACKAGE BODY "RR_RUNINFO" AS

  function create_run(p_runnumber in number, p_username in VARCHAR2, p_globalname in VARCHAR2, p_cert in VARCHAR2) RETURN number AS
    PRAGMA AUTONOMOUS_TRANSACTION;
    l_number NUMBER := 0; 
  BEGIN

    set transaction name 'new run';

    select count(*) into l_number from rr_runs where run_number = p_runnumber;
    if l_number > 0 then
      rollback;
      return 2;
    end if;
    
    INSERT INTO
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
        RUN_LAST_CERT,
        RUN_BFIELD
      ) select
        a.run_number,
        p_username,
        a.run_events,
        a.run_rate,
        a.run_start_time,
        a.run_end_time,
        a.run_l1key,
        a.run_hltkey,
        sysdate,
        p_username,
        p_globalname,
        p_username,
        p_cert,
        get_magnetic_field(p_runnumber)
      from 
        rr_runinfo_cache a
      where
        run_number = p_runnumber;
    
    commit;
    RETURN 0;
    
  exception when NO_DATA_FOUND then
    rollback;
    RETURN 1;
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
      update rr_runs set run_status = 'OFFLINE' where run_number in (select "run_number" from rr_runs_to_offline a);

      -- Update run data from run info
      update RR_RUNS a set (RUN_EVENTS, RUN_RATE, RUN_END_TIME) = 
        (select RUN_EVENTS, RUN_RATE, RUN_END_TIME from RR_RUNINFO_CACHE b 
          where a.run_number = b.run_number) where a.run_end_time is null;
        
      commit;

    exception when NO_DATA_FOUND then
      rollback;
    end;

  end update_runs;

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
    delete from rr_run_subsystems where (rsu_run_number, rsu_sub_abbr) in (select rsu_run_number, rsu_sub_abbr from rr_runinfo_subs_to_delete);
    insert into rr_run_subsystems select "number", "sub", null, "value" from rr_runinfo_subs_to_insert;
    update rr_run_subsystems a set rsu_value = 'GOOD' where (a.rsu_run_number, a.rsu_sub_abbr) in (select "number", "sub" from rr_runinfo_subs_to_update);
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
      run_SHIFTER, run_globalname into p_RUN_SHIFTER, p_RUN_GLOBALNAME
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

--------------------------------------------------------
--  DDL for Package Body RR_UTILS
--------------------------------------------------------

  CREATE OR REPLACE PACKAGE BODY "RR_UTILS" AS

  FUNCTION get_subsystems(run in number) return varchar2 IS
    ret VARCHAR2(10000) := '';
    cursor c is select rsu_sub_abbr from rr_run_subsystems where rsu_run_number = run order by rsu_sub_abbr; -- and rsu_value not in ('NOTSET');
  BEGIN
    for p in c loop
      if length(ret) > 0 then
        ret := ret || ', ';
      end if;
      ret := ret || p.rsu_sub_abbr;
    end loop;
    return ret;
  END;
  
  FUNCTION get_formatted_subsystems(run in number) return varchar2 IS
    ret VARCHAR2(10000) := '';
    cursor c is select rsu_sub_abbr, rsu_value from rr_run_subsystems where rsu_run_number = run order by rsu_sub_abbr; -- and rsu_value not in ('NOTSET');
  BEGIN
    for p in c loop
      if length(ret) > 0 then
        ret := ret || ', ';
      end if;
      ret := ret || '<span class="SS_' || p.rsu_value || '">' || p.rsu_sub_abbr || '</span>';
    end loop;
    return ret;
  END;
  
  FUNCTION get_l1sources(run in number) return varchar2 IS
    ret VARCHAR2(1000) := '';
    cursor c is select rl1_l1s_abbr from rr_run_l1sources where rl1_run_number = run;
  BEGIN
    for p in c loop
      if length(ret) > 0 then
        ret := ret || ', ';
      end if;
      ret := ret || p.rl1_l1s_abbr;
    end loop;
    return ret;
  END;

  FUNCTION get_comments(run in number) return varchar2 IS
    ret VARCHAR2(10000) := '';
    cursor c1 is select rl1_l1s_abbr, rl1_comment from rr_run_l1sources where rl1_run_number = run and rl1_comment is not null;
    cursor c2 is select rsu_sub_abbr, rsu_comment from rr_run_subsystems where rsu_run_number = run and rsu_comment is not null;
  BEGIN
  
    for p in c1 loop
      if length(ret) > 0 then
        ret := ret || '<br />';
      end if;
      ret := ret || '<b>L1&nbsp;' || p.rl1_l1s_abbr ||  ':</b>' || p.rl1_comment;
    end loop;

    for p in c2 loop
      if length(ret) > 0 then
        ret := ret || '<br />';
      end if;
      ret := ret || '<b>Sub&nbsp;' || p.rsu_sub_abbr ||  ':</b>' || p.rsu_comment;
    end loop;
    
    if length(ret) > 0 then
      ret := '<br />' || ret;
    end if;
    
    return ret;
  END;

  PROCEDURE audit_preload AS
  begin
/*
    DELETE FROM RR_RUN_L1SOURCES$A;
    DELETE FROM rr_run_subsystems$a;
    DELETE FROM rr_runs$a;
    INSERT INTO RR_RUN_L1SOURCES$A select rr_run_l1sources.*, sysdate, 'A' from RR_RUN_L1SOURCES;
    INSERT INTO rr_run_subsystems$a select rr_run_subsystems.*, sysdate, 'A' from rr_run_subsystems;
    INSERT INTO rr_runs$a a select  b.*, sysdate, 'A' from rr_runs b;
    commit;
*/
    null;
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

--------------------------------------------------------
--  DDL for Synonymn RUNSESSION_PARAMETER
--------------------------------------------------------

  CREATE OR REPLACE SYNONYM "RUNSESSION_PARAMETER" FOR "CMS_RUNINFO"."RUNSESSION_PARAMETER";
