
CONNECT CMS_ES_COMIS@cms_omds_lb/es164644

CREATE TABLE ES_RUN_MODE_DEF (
	DEF_ID  NUMBER NOT NULL
     , RUN_MODE_STRING VARCHAR2(20)
);
ALTER TABLE ES_RUN_MODE_DEF ADD CONSTRAINT es_run_mode_def_pk PRIMARY KEY (def_id);
ALTER TABLE ES_RUN_MODE_DEF ADD CONSTRAINT es_run_mode_def_uk1 UNIQUE (run_mode_string);

CREATE SEQUENCE es_run_mode_def_sq INCREMENT BY 1 START WITH 1;
CREATE trigger es_run_mode_def_trg
before insert on ES_RUN_MODE_DEF
for each row
begin
select es_run_mode_def_sq.NextVal into :new.def_id from dual;
end;
/

prompt FUNCTION get_run_mode_def_id;
create or replace function get_run_mode_def_id( run_mode IN VARCHAR ) return NUMBER
IS
 	ret NUMBER;
BEGIN
	SELECT DEF_ID 
		INTO 	ret 
		FROM 	ES_RUN_MODE_DEF 
		WHERE 	RUN_MODE_STRING=run_mode
	;
	return (ret);
END;
/

CREATE TABLE ES_RUN_CONFIGURATION_DAT (
       CONFIG_ID NUMBER NOT NULL
     , TAG VARCHAR2(64) NOT NULL
     , VERSION NUMBER(22) NOT NULL
     , RUN_TYPE_DEF_ID NUMBER NOT NULL
     , RUN_MODE_DEF_ID NUMBER NOT NULL
     , NUM_OF_SEQUENCES NUMBER(22) NULL
     , DESCRIPTION VARCHAR2(200) NULL
     , DEFAULTS NUMBER NULL
     , TRG_MODE VARCHAR2(64) NULL
     , NUM_OF_EVENTS NUMber NULL
     , db_timestamp  TIMESTAMP DEFAULT SYSTIMESTAMP NOT NULL     
);

ALTER TABLE ES_RUN_CONFIGURATION_DAT ADD CONSTRAINT es_config_pk PRIMARY KEY (config_id);
ALTER TABLE ES_RUN_CONFIGURATION_DAT ADD CONSTRAINT es_config_uk1 UNIQUE (tag, version);
ALTER TABLE ES_RUN_CONFIGURATION_DAT ADD CONSTRAINT es_config_fk2 FOREIGN KEY (run_mode_def_id) REFERENCES ES_RUN_MODE_DEF (DEF_ID) ;

CREATE SEQUENCE es_run_sq INCREMENT BY 1 START WITH 1;


prompt FUNCTION get_run_conf_id;
create or replace function get_run_conf_id( the_tag IN VARCHAR, the_version in NUMBER ) return NUMBER
IS
 	ret NUMBER;
BEGIN
	SELECT CONFIG_ID 
		INTO 	ret 
		FROM 	ES_RUN_CONFIGURATION_DAT r 
		WHERE 	r.TAG=the_tag
		AND	r.VERSION=the_version
	;
	return (ret);
END;
/

-- ********** ES_SEQUENCE

CREATE TABLE ES_SEQUENCE_TYPE_DEF (
	DEF_ID  NUMBER NOT NULL
	, RUN_TYPE_DEF_ID NUMBER NOT NULL
        , SEQUENCE_TYPE_STRING VARCHAR2(20)
);
ALTER TABLE ES_SEQUENCE_TYPE_DEF ADD CONSTRAINT es_sequence_type_def_pk  PRIMARY KEY (def_id);
ALTER TABLE ES_SEQUENCE_TYPE_DEF ADD CONSTRAINT es_sequence_type_def_uk1 UNIQUE (run_type_def_id,sequence_type_string);

CREATE SEQUENCE es_sequence_type_def_sq INCREMENT BY 1 START WITH 1;
CREATE trigger es_sequence_type_def_trg
before insert on ES_SEQUENCE_TYPE_DEF
for each row
begin
select es_sequence_type_def_sq.NextVal into :new.def_id from dual;
end;
/

prompt FUNCTION get_sequence_type_def_id;
CREATE OR REPLACE FUNCTION get_sequence_type_def_id( a_run_type IN VARCHAR, seq_type VARCHAR ) return NUMBER
IS
	ret NUMBER;
BEGIN
	SELECT s.DEF_ID 
		INTO 	ret 
		FROM 	ES_SEQUENCE_TYPE_DEF s
			, RUN_TYPE_DEF r
		WHERE 	s.SEQUENCE_TYPE_STRING=seq_type
			AND r.RUN_TYPE=a_run_type
			AND s.RUN_TYPE_DEF_ID=r.DEF_ID
	;
	return (ret);
END;
/

CREATE TABLE ES_SEQUENCE_DAT (
       SEQUENCE_ID NUMBER NOT NULL
     , ES_CONFIG_ID NUMBER NOT NULL
     , SEQUENCE_NUM NUMBER(22) NOT NULL
     , NUM_OF_CYCLES NUMBER(22) NULL
     , SEQUENCE_TYPE_DEF_ID NUMBER NOT NULL
     , DESCRIPTION VARCHAR2(200) NULL     
);

ALTER TABLE ES_SEQUENCE_DAT ADD CONSTRAINT es_sequence_dat_pk PRIMARY KEY (sequence_id);
ALTER TABLE ES_SEQUENCE_DAT ADD CONSTRAINT es_sequence_dat_fk1 FOREIGN KEY (es_config_id)       REFERENCES ES_RUN_CONFIGURATION_DAT (CONFIG_ID);
ALTER TABLE ES_SEQUENCE_DAT ADD CONSTRAINT es_sequence_dat_fk2 FOREIGN KEY (sequence_type_def_id) REFERENCES ES_SEQUENCE_TYPE_DEF (DEF_ID);
ALTER TABLE ES_SEQUENCE_DAT ADD CONSTRAINT es_sequence_dat_uk1 UNIQUE (es_config_id, SEQUENCE_NUM);

CREATE SEQUENCE es_sequence_dat_sq INCREMENT BY 1 START WITH 1;
CREATE trigger es_sequence_dat_trg
before insert on ES_SEQUENCE_DAT
for each row
begin
select es_sequence_dat_sq.NextVal into :new.sequence_id from dual;
end;
/

prompt FUNCTION get_sequence_id;
create or replace function get_sequence_id( the_run_tag IN VARCHAR, the_run_version in NUMBER, the_seq_num in NUMBER ) return NUMBER
IS
 	ret NUMBER;
BEGIN
	SELECT s.SEQUENCE_ID 
		INTO 	ret 
		FROM 	ES_RUN_CONFIGURATION_DAT r
			, ES_SEQUENCE_DAT s 
		WHERE 	r.TAG=the_run_tag
		AND	r.VERSION=the_run_version
		AND	r.CONFIG_ID=s.ES_CONFIG_ID
		AND	s.sequence_num=the_seq_num
	;
	return (ret);
END;
/
show errors;

-- TODO Add a trigger to check that sequence type and run_type are coherent


-- ********** ES_CYCLE_DAT

CREATE TABLE ES_CYCLE_DAT (
	  CYCLE_ID NUMBER NOT NULL
	, SEQUENCE_ID NUMBER NOT NULL
	, CYCLE_NUM NUMBER(22)
        , TAG VARCHAR2(64) NULL
        , DESCRIPTION VARCHAR2(200) NULL
 );    

ALTER TABLE ES_CYCLE_DAT ADD CONSTRAINT es_cycle_dat_pk PRIMARY KEY (cycle_id);
ALTER TABLE ES_CYCLE_DAT ADD CONSTRAINT es_cycle_uk1 UNIQUE (sequence_id, cycle_num);
ALTER TABLE ES_CYCLE_DAT ADD CONSTRAINT es_cycle_dat_fk1 FOREIGN KEY (sequence_id) REFERENCES ES_SEQUENCE_DAT (SEQUENCE_ID) ;

CREATE SEQUENCE es_cycle_dat_sq INCREMENT BY 1 START WITH 1;
CREATE trigger es_cycle_dat_trg
before insert on ES_CYCLE_DAT
for each row
begin
select es_cycle_dat_sq.NextVal into :new.cycle_id from dual;
end;
/

-- ********** ES_CCS

CREATE TABLE ES_CCS_CONFIGURATION (
	ccs_configuration_id NUMBER NOT NULL
        , ccs_tag VARCHAR2(32) NOT NULL
	, DACCAL NUMBER
        , DELAY NUMBER
        , GAIN VARCHAR2(64)
        , MEMGAIN VARCHAR2(64)
        , OFFSET_HIGH NUMBER
        , OFFSET_LOW  NUMBER
        , OFFSET_MID  NUMBER
	, TRG_MODE VARCHAR2(64)
        , TRG_FILTER VARCHAR2(64)
        , CLOCK NUMBER
        , BGO_SOURCE VARCHAR2(64)
        , TTS_MASK NUMBER
        , DAQ_BCID_PRESET NUMBER
        , TRIG_BCID_PRESET NUMBER
        , BC0_COUNTER NUMBER
        , BC0_DELAY NUMBER
        , TE_DELAY NUMBER
);
ALTER TABLE ES_CCS_CONFIGURATION ADD CONSTRAINT es_ccs_config_pk PRIMARY KEY (ccs_configuration_id);

CREATE SEQUENCE es_CCS_CONFIG_sq INCREMENT BY 1 START WITH 1;


CREATE TABLE ES_CCS_CYCLE (
	  CYCLE_ID NUMBER NOT NULL,
	  CCS_CONFIGURATION_ID NUMBER NOT NULL
         );    

ALTER TABLE ES_CCS_CYCLE ADD CONSTRAINT es_ccs_cycle_pk PRIMARY KEY (CYCLE_ID);
ALTER TABLE ES_CCS_CYCLE ADD CONSTRAINT es_ccs_cycle_fk1 FOREIGN KEY (cycle_id) REFERENCES ES_CYCLE_DAT (cycle_id);
ALTER TABLE ES_CCS_CYCLE ADD CONSTRAINT es_ccs_cycle_fk2 FOREIGN KEY (ccs_configuration_id) REFERENCES ES_CCS_CONFIGURATION (ccs_configuration_id);


-- ********** ES_DCC

CREATE TABLE ES_DCC_CONFIGURATION (
	DCC_CONFIGURATION_ID NUMBER NOT NULL
        , dcc_tag VARCHAR2(32) NOT NULL
	, DCC_CONFIGURATION_URL VARCHAR2(100)
	, TESTPATTERN_FILE_URL VARCHAR2(100)
	, N_TESTPATTERNS_TO_LOAD NUMBER
        , SM_HALF NUMBER
    	, dcc_CONFIGURATION CLOB
);
ALTER TABLE ES_DCC_CONFIGURATION ADD CONSTRAINT es_dcc_config_pk PRIMARY KEY (dcc_configuration_id);

CREATE SEQUENCE es_DCC_CONFIG_sq INCREMENT BY 1 START WITH 1;


CREATE TABLE ES_DCC_CYCLE (
	  CYCLE_ID NUMBER NOT NULL
	 , DCC_CONFIGURATION_ID NUMBER NOT NULL
         );    

ALTER TABLE ES_DCC_CYCLE ADD CONSTRAINT es_dcc_cycle_pk PRIMARY KEY (cycle_id);
ALTER TABLE ES_DCC_CYCLE ADD CONSTRAINT es_dcc_cycle_fk1 FOREIGN KEY (cycle_id) REFERENCES ES_CYCLE_DAT (cycle_id);
ALTER TABLE ES_DCC_CYCLE ADD CONSTRAINT es_dcc_cycle_fk2 FOREIGN KEY (dcc_configuration_id) REFERENCES ES_DCC_CONFIGURATION (dcc_configuration_id);

-- ********** ECAL_ttcf

CREATE TABLE ECAL_TTCF_CONFIGURATION (
	TTCF_CONFIGURATION_ID NUMBER NOT NULL
        , TTCF_tag VARCHAR2(32) NOT NULL
        , TTCF_CONFIGURATION_FILE VARCHAR2(100) 
      	, TTCF_CONFIGURATION CLOB,
RXBC0_DELAY NUMBER, REG_30 NUMBER
);
ALTER TABLE ECAL_TTCF_CONFIGURATION ADD CONSTRAINT ecal_ttcf_config_pk PRIMARY KEY (ttcf_configuration_id);

CREATE SEQUENCE ecal_TTCF_CONFIG_sq INCREMENT BY 1 START WITH 1;

CREATE TABLE ECAL_TTCF_CYCLE (
	  CYCLE_ID NUMBER NOT NULL
	 , TTCF_CONFIGURATION_ID NUMBER NOT NULL
         );    

ALTER TABLE ECAL_TTCF_CYCLE ADD CONSTRAINT ecal_ttcf_cycle_pk PRIMARY KEY (cycle_id);
ALTER TABLE ECAL_TTCF_CYCLE ADD CONSTRAINT ecal_ttcf_cycle_fk1 FOREIGN KEY (cycle_id) REFERENCES ES_CYCLE_DAT (cycle_id);
ALTER TABLE ECAL_TTCF_CYCLE ADD CONSTRAINT ecal_ttcf_cycle_fk2 FOREIGN KEY (ttcf_configuration_id) REFERENCES ECAL_TTCF_CONFIGURATION (ttcf_configuration_id);

-- ********** ECAL_srp

CREATE TABLE ECAL_SRP_CONFIGURATION (
	SRP_CONFIGURATION_ID NUMBER NOT NULL
        , SRP_tag VARCHAR2(32) NOT NULL
	, DEBUGMODE NUMBER
	, DUMMYMODE NUMBER
	, PATTERN_DIRECTORY VARCHAR2(100)
        , AUTOMATIC_MASKS NUMBER
        , SRP0BUNCHADJUSTPOSITION NUMBER 
	, SRP_CONFIG_FILE VARCHAR2(100)
      	, SRP_CONFIGURATION CLOB
);
ALTER TABLE ECAL_SRP_CONFIGURATION ADD CONSTRAINT ecal_SRP_config_pk PRIMARY KEY (SRP_configuration_id);

CREATE SEQUENCE ecal_SRP_CONFIG_sq INCREMENT BY 1 START WITH 1;


CREATE TABLE ECAL_SRP_CYCLE (
	  CYCLE_ID NUMBER NOT NULL
	 , srp_CONFIGURATION_ID NUMBER NOT NULL
         );    

ALTER TABLE ECAL_srp_CYCLE ADD CONSTRAINT ecal_srp_cycle_pk PRIMARY KEY (cycle_id);
ALTER TABLE ECAL_srp_CYCLE ADD CONSTRAINT ecal_srp_cycle_fk1 FOREIGN KEY (cycle_id) REFERENCES ES_CYCLE_DAT (cycle_id);
ALTER TABLE ECAL_srp_CYCLE ADD CONSTRAINT ecal_srp_cycle_fk2 FOREIGN KEY (srp_configuration_id) REFERENCES ECAL_SRP_CONFIGURATION (srp_configuration_id);



-- ********** ECAL_Ttcci

CREATE TABLE ECAL_TTCCI_CONFIGURATION (
	TTCCI_configuration_id NUMBER NOT NULL
        , TTCCI_tag VARCHAR2(32) NOT NULL
	, TTCCI_configuration_file varchar2(130)
	,TRG_MODE varchar2(32)
	,TRG_SLEEP NUMBER
    	, Configuration CLOB
       , CONFIGURATION_SCRIPT varchar2(100)
       , CONFIGURATION_SCRIPT_PARAMS varchar2(100)
);
ALTER TABLE ECAL_TTCCI_CONFIGURATION ADD CONSTRAINT ecal_ttcci_config_pk PRIMARY KEY (TTCCI_configuration_id);

CREATE SEQUENCE ecal_ttcci_CONFIG_sq INCREMENT BY 1 START WITH 1;


CREATE TABLE ECAL_TTCCI_CYCLE (
           CYCLE_ID NUMBER NOT NULL
	 , TTCCI_CONFIGURATION_ID NUMBER NOT NULL
         );    

ALTER TABLE ECAL_TTCCI_CYCLE ADD CONSTRAINT ecal_ttcci_cycle_pk PRIMARY KEY (cycle_id);
ALTER TABLE ECAL_TTCCI_CYCLE ADD CONSTRAINT ecal_ttcci_cycle_fk1 FOREIGN KEY (cycle_id) REFERENCES ES_CYCLE_DAT (cycle_id);
ALTER TABLE ECAL_TTCCI_CYCLE ADD CONSTRAINT ecal_ttcci_cycle_fk2 FOREIGN KEY (ttcci_configuration_id) REFERENCES ECAL_TTCCI_CONFIGURATION (TTCCI_configuration_id);

-- ********** ECAL_LTC

CREATE TABLE ECAL_LTC_CONFIGURATION (
	LTC_configuration_id NUMBER NOT NULL
        , LTC_tag VARCHAR2(32) NOT NULL
	, ltc_Configuration_file varchar2(100)
      	, Configuration CLOB
);
ALTER TABLE ECAL_LTC_CONFIGURATION ADD CONSTRAINT ecal_LTC_config_pk PRIMARY KEY (LTC_configuration_id);

CREATE SEQUENCE ecal_LTC_CONFIG_sq INCREMENT BY 1 START WITH 1;


CREATE TABLE ECAL_LTC_CYCLE (
	  CYCLE_ID NUMBER NOT NULL
	 , LTC_CONFIGURATION_ID NUMBER NOT NULL
         );    

ALTER TABLE ECAL_LTC_CYCLE ADD CONSTRAINT ecal_LTC_cycle_pk PRIMARY KEY (cycle_id);
ALTER TABLE ECAL_LTC_CYCLE ADD CONSTRAINT ecal_LTC_cycle_fk1 FOREIGN KEY (cycle_id) REFERENCES ES_CYCLE_DAT (cycle_id);
ALTER TABLE ECAL_LTC_CYCLE ADD CONSTRAINT ecal_LTC_cycle_fk2 FOREIGN KEY (ltc_configuration_id) REFERENCES ECAL_LTC_CONFIGURATION (LTC_configuration_id);


-- ********** ECAL_LTS

CREATE TABLE ECAL_LTS_CONFIGURATION (
	LTS_configuration_id NUMBER NOT NULL
        , lts_tag VARCHAR2(32) NOT NULL
        , TRIGGER_TYPE VARCHAR2(32)
        , NUM_OF_EVENTS NUMBER
        , RATE NUMBER
	, TRIG_LOC_L1_DELAY NUMBER
);
ALTER TABLE ECAL_LTS_CONFIGURATION ADD CONSTRAINT ecal_LTS_config_pk PRIMARY KEY (LTS_configuration_id);

CREATE SEQUENCE ecal_LTS_CONFIG_sq INCREMENT BY 1 START WITH 1;


CREATE TABLE ECAL_LTS_CYCLE (
	  CYCLE_ID NUMBER NOT NULL
	 , LTS_CONFIGURATION_ID NUMBER NOT NULL
         );    

ALTER TABLE ECAL_LTS_CYCLE ADD CONSTRAINT ecal_LTS_cycle_pk PRIMARY KEY (cycle_id);
ALTER TABLE ECAL_LTS_CYCLE ADD CONSTRAINT ecal_LTS_cycle_fk1 FOREIGN KEY (cycle_id) REFERENCES ES_CYCLE_DAT (cycle_id);
ALTER TABLE ECAL_LTS_CYCLE ADD CONSTRAINT ecal_LTS_cycle_fk2 FOREIGN KEY (lts_configuration_id) REFERENCES ECAL_LTS_CONFIGURATION (LTS_configuration_id);



-- ********** ES_JBH4

CREATE TABLE ES_JBH4_CONFIGURATION (
	JBH4_configuration_id NUMBER NOT NULL
        , JBH4_tag VARCHAR2(32) NOT NULL
        , useBuffer NUMBER
        , halModuleFile VARCHAR2(64)
        , halAddressTableFile VARCHAR2(64)
        , halStaticTableFile VARCHAR2(64)
        , halCbd8210SerialNumber VARCHAR2(64)
        , caenBridgeType VARCHAR2(64)
        , caenLinkNumber NUMBER
        , caenBoardNumber NUMBER
);
ALTER TABLE ES_JBH4_CONFIGURATION ADD CONSTRAINT es_JBH4_config_pk PRIMARY KEY (JBH4_configuration_id);

CREATE SEQUENCE es_JBH4_CONFIG_sq INCREMENT BY 1 START WITH 1;


CREATE TABLE ES_JBH4_CYCLE (
	  CYCLE_ID NUMBER NOT NULL
	 , JBH4_CONFIGURATION_ID NUMBER NOT NULL
         );    

ALTER TABLE ES_JBH4_CYCLE ADD CONSTRAINT es_JBH4_cycle_pk PRIMARY KEY (cycle_id);
ALTER TABLE ES_JBH4_CYCLE ADD CONSTRAINT es_JBH4_cycle_fk1 FOREIGN KEY (cycle_id) REFERENCES ES_CYCLE_DAT (cycle_id);
ALTER TABLE ES_JBH4_CYCLE ADD CONSTRAINT es_JBH4_cycle_fk2 FOREIGN KEY (jbh4_configuration_id) REFERENCES ES_JBH4_CONFIGURATION (JBH4_configuration_id);


-- ********** ES_SCAN

CREATE TABLE ES_SCAN_DAT (
          SCAN_ID NUMBER	
        , SCAN_tag VARCHAR2(32) NOT NULL
        , type_id number
        , scan_type varchar2(32) 
	, FROM_VAL NUMBER
	, TO_VAL NUMBER
	, STEP NUMBER
);

ALTER TABLE ES_SCAN_DAT ADD CONSTRAINT es_scan_dat_pk  PRIMARY KEY (scan_id);
CREATE SEQUENCE es_SCAN_CONFIG_sq INCREMENT BY 1 START WITH 1;





CREATE TABLE ES_SCAN_CYCLE (
	  CYCLE_ID NUMBER NOT NULL
	 , SCAN_ID NUMBER NOT NULL
         );    

ALTER TABLE ES_SCAN_CYCLE ADD CONSTRAINT es_SCAN_cycle_pk  PRIMARY KEY (cycle_id);
ALTER TABLE ES_SCAN_CYCLE ADD CONSTRAINT es_SCAN_cycle_fk1 FOREIGN KEY (cycle_id) REFERENCES ES_CYCLE_DAT (cycle_id);
ALTER TABLE ES_SCAN_CYCLE ADD CONSTRAINT es_SCAN_cycle_fk2 FOREIGN KEY (scan_id)  REFERENCES ES_SCAN_DAT  (SCAN_id);

-- ********** VIEWS

CREATE OR REPLACE VIEW ES_RUN_CONFIGURATION AS
select r.CONFIG_ID
     , r.TAG
     , r.VERSION
     , rtd.RUN_TYPE RUN_TYPE
     , rmd.RUN_MODE_STRING RUN_MODE
     , r.NUM_OF_SEQUENCES
     , r.DESCRIPTION RUN_CONFIG_DESCRIPTION
     , r.DEFAULTS
     , r.TRG_MODE
from
	ES_RUN_CONFIGURATION_DAT r
	, RUN_TYPE_DEF rtd
	, ES_RUN_MODE_DEF rmd
where
	r.RUN_TYPE_DEF_ID=rtd.DEF_ID
	and r.RUN_MODE_DEF_ID=rmd.DEF_ID
;


CREATE OR REPLACE VIEW ES_SEQUENCE AS
select
       s.SEQUENCE_ID
     , r.TAG
     , r.VERSION
     , s.SEQUENCE_NUM
     , s.NUM_OF_CYCLES
     , std.SEQUENCE_TYPE_STRING sequence_type
     , s.DESCRIPTION
from
	ES_SEQUENCE_DAT s
	, ES_SEQUENCE_TYPE_DEF std
	, ES_RUN_CONFIGURATION_DAT r
where
	s.ES_CONFIG_ID=r.CONFIG_ID
	and s.SEQUENCE_TYPE_DEF_ID=std.DEF_ID
;

CREATE OR REPLACE VIEW ES_CYCLE_VIEW AS
SELECT 
	e.cycle_id
	, r.tag tag
	, r.version version
	, s.sequence_num
        , s.sequence_id
	, e.cycle_num
	, e.tag cycle_tag
        , e.description
	, ccs.CCS_CONFIGURATION_ID
	, dcc.dcc_CONFIGURATION_ID
--	, laser.laser_CONFIGURATION_ID
	, ltc.ltc_CONFIGURATION_ID
	, lts.lts_CONFIGURATION_ID
--	, dcu.dcu_CONFIGURATION_ID
--	, tcc.tcc_CONFIGURATION_ID
        , ttcf.ttcf_CONFIGURATION_ID
        , srp.srp_configuration_id
	, ttcci.ttcci_CONFIGURATION_ID "TTCci_CONFIGURATION_ID"
	, jbh4.jbh4_CONFIGURATION_ID
	, scan.scan_id
FROM
	ES_RUN_CONFIGURATION_DAT r,
	ES_SEQUENCE_DAT s,
	ES_CYCLE_Dat e
	LEFT OUTER join
	ES_CCS_CYCLE ccs on  e.cycle_id=ccs.cycle_ID
	left outer join
	ES_DCC_CYCLE dcc on  e.cycle_id=dcc.cycle_ID
	left outer join
--	ECAL_LASER_CYCLE laser on e.cycle_id=laser.cycle_ID
--	left outer join
	ECAL_ltc_CYCLE ltc on  e.cycle_id=ltc.cycle_ID
	left outer join
	ECAL_lts_CYCLE lts on e.cycle_id=lts.cycle_ID
	left outer join
--	ECAL_dcu_CYCLE dcu on e.cycle_id=dcu.cycle_ID
--	left outer join
--	ECAL_tcc_CYCLE tcc on e.cycle_id=tcc.cycle_ID
--	left outer join
	ecal_ttcci_CYCLE ttcci on  e.cycle_id=ttcci.cycle_ID
	left outer join
	ES_jbh4_CYCLE jbh4 on  e.cycle_id=jbh4.cycle_ID
	left outer join
	ES_SCAN_cycle scan on e.cycle_id=scan.cycle_id
	left outer join
	ECAL_srp_cycle srp on e.cycle_id=srp.cycle_id
	left outer join
	ECAL_ttcf_CYCLE ttcf on e.cycle_id=ttcf.cycle_ID
where 
	r.config_id=s.es_config_id 
	and e.sequence_id=s.sequence_id
;


CREATE OR REPLACE VIEW ES_SCAN_CONFIGURATION AS
select r.SCAN_ID SCAN_ID
        , r.SCAN_tag tag
     , r.type_id type_id
     , r.scan_type scan_type
     , r.FROM_VAL from_val
     , r.to_val to_val
     , r.STEP  step
 from
	ES_scan_dat r
;


CREATE TABLE ES_DEAD_CHANNELS (
	FED_ID  NUMBER NOT NULL
      , CH_ID   NUMBER(20) NOT NULL
);


@insert_ES_run_mod_defs

