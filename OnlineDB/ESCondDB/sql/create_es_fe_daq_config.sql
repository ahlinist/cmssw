

CREATE SEQUENCE es_fe_conf_sq INCREMENT BY 1 START WITH 1 nocache;


CREATE TABLE ES_FE_CONF_info (
 rec_id NUMBER(10) NOT NULL,
 TAG VARCHAR2(100) NOT NULL,
 version NUMBER(10) NOT NULL,
 iov_id_es_pl NUMBER(10) NOT NULL,
 iov_id_es_mi NUMBER(10) NOT NULL,
 USER_COMMENT VARCHAR2(1000),
 db_timestamp  TIMESTAMP DEFAULT SYSTIMESTAMP NOT NULL
);

ALTER TABLE ES_FE_CONF_INFO ADD CONSTRAINT  ES_FE_CONF_INFO_PK PRIMARY KEY (rec_id);
ALTER TABLE ES_FE_CONF_INFO ADD CONSTRAINT ES_FE_CONF_INFO_uk UNIQUE(tag,version);


create table ES_FE_CONF_DAT (
 REC_ID NUMBER(10) NOT NULL,
 FED_ID   NUMBER(4) NOT NULL,
 FIBER_ID NUMBER(2) NOT NULL,
 KCHIP_ID NUMBER(4) NOT NULL,
 PACE_ID  NUMBER(1) NOT NULL,
 STRIP_ID NUMBER(2) NOT NULL,
 RX_ID    NUMBER(1) not null,
 GAIN     NUMBER(3) NOT NULL,
 PEDESTAL number not null,
 MASKED    NUMBER(1) NOT NULL,
 ZS        NUMBER NOT NULL,
 CM_MASKED NUMBER(1) NOT NULL,
 CM_RANGE  NUMBER NOT NULL
);

ALTER TABLE ES_FE_CONF_DAT ADD CONSTRAINT
ES_FE_CONF_DAT_FK FOREIGN KEY (REC_ID)
REFERENCES ES_FE_CONF_INFO (REC_ID);
ALTER TABLE ES_FE_CONF_DAT ADD CONSTRAINT ES_FE_CONF_DAT_pk PRIMARY KEY (rec_id, FED_ID, FIBER_ID, KCHIP_ID, PACE_ID, STRIP_ID, RX_ID );

CREATE OR REPLACE function test_update_tag_and_version
( cndc_table IN VARCHAR2,
  tag IN varchar2,
  version IN integer)  return INTEGER IS
 /*
 *
 * Procedure to attribute a new version number if a tag exists with that name
 */
  sql_str VARCHAR(1000);
  num_tags INTEGER;
  last_version INTEGER;
  new_version INTEGER;
  cur_version INTEGER;

  BEGIN

  cur_version := version;
        IF version IS NULL THEN
        new_version :=0;
        cur_version :=0;
        END IF;

    -- Make sure that this tag does not exist
    sql_str := 'SELECT count(*) FROM ' || cndc_table || ' WHERE tag = :s ';
    EXECUTE IMMEDIATE sql_str INTO num_tags USING tag ;

    IF num_tags = 0 THEN
        new_version := cur_version;
    END IF;

    -- Make sure that if it exists the proposed tag is higher

    sql_str := 'SELECT max(version) FROM ' || cndc_table || ' WHERE tag = :s ';
    EXECUTE IMMEDIATE sql_str INTO last_version USING tag ;

    IF last_version IS NOT NULL THEN
        IF last_version>=cur_version THEN
          new_version:= last_version+1;
        ELSIF 1=1 THEN
          new_version:= cur_version;
        END IF;
    END IF;

    return new_version;
end;
/
show errors;



CREATE OR REPLACE TRIGGER es_fe_conf_info_ver_tg
  BEFORE INSERT ON ES_FE_CONF_INFO
  FOR EACH ROW
    begin
  select test_update_tag_and_version('ES_FE_CONF_INFO', :new.tag, :new.version) into :new.version from dual;
end;
/
SHOW ERRORS;


