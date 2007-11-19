/*
 *  Creates all the data tables required to configure the trigger and FE for the trigger
 */


CREATE TABLE FE_CONFIG_MAIN (
conf_id NUMBER NOT NULL, -- (the most important trigger key)
ped_conf_id NUMBER NOT NULL, -- (the link to the pedestals)
lin_conf_id NUMBER NOT NULL, -- (the link to the lin table)
lut_conf_id NUMBER NOT NULL, -- (the link to the LUT table)
fgr_conf_id NUMBER NOT NULL, -- (the link to the fine grain table)
sli_conf_id NUMBER NOT NULL, -- (the link to the sliding window table)
wei_conf_id NUMBER NOT NULL, -- (the link to the weight configuration table)
tag          VARCHAR2(100), -- (a comment if you want to add it)
db_timestamp		TIMESTAMP DEFAULT SYSTIMESTAMP NOT NULL
);

ALTER TABLE FE_CONFIG_MAIN ADD CONSTRAINT FE_CONFIG_MAIN_PK PRIMARY KEY (CONF_ID);
ALTER TABLE FE_CONFIG_MAIN ADD CONSTRAINT FE_CONFIG_LINE_UNIQUE_uk UNIQUE (ped_conf_id,lin_conf_id,lut_conf_id,fgr_conf_id,sli_conf_id,wei_conf_id);

CREATE SEQUENCE FE_CONFIG_MAIN_SQ INCREMENT BY 1 START WITH 1;

/*
 *  the conf_id is auto-incremented at each time you insert in the table 
 *  no need to bother about inserting conf_id
 */

CREATE trigger FE_CONFIG_MAIN_TRG
before insert on FE_CONFIG_MAIN
for each row
begin
select FE_CONFIG_MAIN_SQ.NextVal into :new.conf_id from dual;
end;
/



CREATE TABLE FE_CONFIG_PED_INFO (
 ped_conf_id NUMBER(10) NOT NULL,
 iov_id NUMBER(10) , -- references the condition DB table used 
 db_timestamp  TIMESTAMP DEFAULT SYSTIMESTAMP NOT NULL,	
 TAG VARCHAR2(100)	
);
ALTER TABLE FE_CONFIG_PED_INFO ADD CONSTRAINT  FE_CONFIG_PED_INFO_PK PRIMARY KEY (ped_conf_id);


CREATE TABLE FE_CONFIG_LIN_INFO (
 lin_conf_id NUMBER(10) NOT NULL,
 iov_id NUMBER(10) , -- references the condition DB table used 
 db_timestamp  TIMESTAMP DEFAULT SYSTIMESTAMP NOT NULL,	
 TAG VARCHAR2(100)	
);
ALTER TABLE FE_CONFIG_lin_INFO ADD CONSTRAINT  FE_CONFIG_lin_INFO_PK PRIMARY KEY (lin_conf_id);



CREATE TABLE FE_CONFIG_LUT_INFO (
 lut_conf_id NUMBER(10) NOT NULL,
 iov_id NUMBER(10) , -- references the condition DB table used 
 db_timestamp  TIMESTAMP DEFAULT SYSTIMESTAMP NOT NULL,	
 TAG VARCHAR2(100)	
);
ALTER TABLE FE_CONFIG_lut_INFO ADD CONSTRAINT  FE_CONFIG_lut_INFO_PK PRIMARY KEY (lut_conf_id);

CREATE TABLE FE_CONFIG_fgr_INFO (
 fgr_conf_id NUMBER(10) NOT NULL,
 iov_id NUMBER(10) , -- references the condition DB table used 
 db_timestamp  TIMESTAMP DEFAULT SYSTIMESTAMP NOT NULL,	
 TAG VARCHAR2(100)	
);
ALTER TABLE FE_CONFIG_fgr_INFO ADD CONSTRAINT  FE_CONFIG_fgr_INFO_PK PRIMARY KEY (fgr_conf_id);

CREATE TABLE FE_CONFIG_sliding_INFO (
 sli_conf_id NUMBER(10) NOT NULL,
 iov_id NUMBER(10) , -- references the condition DB table used 
 db_timestamp  TIMESTAMP DEFAULT SYSTIMESTAMP NOT NULL,	
 TAG VARCHAR2(100)	
);
ALTER TABLE FE_CONFIG_sliding_INFO ADD CONSTRAINT  FE_CONFIG_SLIDING_INFO_PK PRIMARY KEY (sli_conf_id);

CREATE TABLE FE_CONFIG_WEIGHT_INFO (
 wei_conf_id NUMBER(10) NOT NULL,
 number_of_groups NUMBER(10) , -- (the number of groups of weights)
 db_timestamp  TIMESTAMP DEFAULT SYSTIMESTAMP NOT NULL,	
 TAG VARCHAR2(100)	
);
ALTER TABLE FE_CONFIG_WEIGHT_INFO ADD CONSTRAINT  FE_CONFIG_WEIGHT_INFO_PK PRIMARY KEY (wei_conf_id);




/*
 *  here we have the pedestals - 3 values per crystal (1 per gain)
 */


CREATE TABLE FE_CONFIG_PED_DAT (
 ped_conf_id NUMBER(10) NOT NULL,
 logic_id NUMBER(10) not null, 
 mean_12 NUMBER(10),
 mean_6 NUMBER(10),
 mean_1 NUMBER(10));

ALTER TABLE FE_CONFIG_PED_DAT ADD CONSTRAINT FE_CONFIG_PED_pk PRIMARY KEY (ped_conf_id, logic_id);
ALTER TABLE FE_CONFIG_PED_DAT ADD CONSTRAINT FE_CONFIG_PED_fk FOREIGN KEY (ped_conf_id) REFERENCES FE_CONFIG_PED_INFO (ped_conf_id);



/*
 * adc_to_gev is the LSB equivalent energy typical is 0.035 GeV/ADC 
 *  here we have the absolute calibration - 1 value per barrel + 1 value per EC maybe
 * logic_id=2000000001 for EE and 1000000000 for EB
 * Energy_crystal=FE_CONFIG_calib_lsb_DAT.adc_to_gev * fe_config_calib_dat.calibration * Peak(ADCCounts)
 * and at higher gains 
 * Energy_crystal= FE_CONFIG_calib_lsb_DAT.adc_to_gev * fe_config_calib_dat.calibration * gain_ratio * Peak(ADCCounts) 
 */


/*
 *  here we have the linearization - 1 value per crystal
 */

CREATE TABLE FE_CONFIG_lin_DAT (
  lin_conf_id        NUMBER(10),
  logic_id              NUMBER(10), -- (crystal)
  multx12                Number,
  multx6                 number,
  multx1                 number,
  shift12                number,
  shift6                 number,
  shift1                 number
);

ALTER TABLE FE_CONFIG_lin_DAT ADD CONSTRAINT FE_CONFIG_lin_pk PRIMARY KEY (lin_conf_id, logic_id);
ALTER TABLE FE_CONFIG_lin_DAT ADD CONSTRAINT FE_CONFIG_lin_fk FOREIGN KEY (lin_conf_id) REFERENCES FE_CONFIG_lin_INFO (lin_conf_id);


/*
 *  here we have the linearization parameters used to compute the lin coeff.
 */


CREATE TABLE FE_CONFIG_param_DAT (
  lin_conf_id        NUMBER(10),
  logic_id           NUMBER(10), -- (crystal)
  etsat                Number,
  ttthreshlow         Number,
  ttthreshhigh         Number,
  fg_lowthresh         Number,
  fg_highthresh         Number,
  fg_lowratio         Number,
  fg_highratio         Number
);

ALTER TABLE FE_CONFIG_param_DAT ADD CONSTRAINT FE_CONFIG_param_pk PRIMARY KEY (lin_conf_id, logic_id);
ALTER TABLE FE_CONFIG_param_DAT ADD CONSTRAINT FE_CONFIG_param_fk FOREIGN KEY (lin_conf_id) REFERENCES FE_CONFIG_lin_INFO (lin_conf_id);



/*
 *  here we have the weights and the sliding window parameters - values per strip
 */


CREATE TABLE FE_CONFIG_sliding_DAT (
  sli_conf_id        NUMBER(10),
  logic_id              NUMBER(10), -- (strip)
  sliding                 NUMBER(10)
);

ALTER TABLE FE_CONFIG_sliding_DAT ADD CONSTRAINT FE_CONFIG_sliding_pk PRIMARY KEY (sli_conf_id, logic_id);
ALTER TABLE FE_CONFIG_sliding_DAT ADD CONSTRAINT FE_CONFIG_sliding_fk FOREIGN KEY (sli_conf_id) REFERENCES FE_CONFIG_sliding_INFO (sli_conf_id);



create table FE_WEIGHT_PER_GROUP_DAT(
 wei_conf_id number not null,
 group_id number(10) not null,
 W0 NUMBER,
 W1 NUMBER,
 W2 NUMBER,
 W3 NUMBER,
 W4 NUMBER	
 );

ALTER TABLE FE_WEIGHT_PER_GROUP_DAT ADD CONSTRAINT FE_WEIGHT_PER_GROUP_pk PRIMARY KEY (wei_conf_id , group_id);
ALTER TABLE FE_WEIGHT_PER_GROUP_DAT ADD CONSTRAINT FE_WEIGHT_PER_GROUP_fk foreign KEY (wei_conf_id) REFERENCES FE_CONFIG_WEIGHT_INFO (wei_conf_id);


CREATE TABLE FE_CONFIG_WEIGHT_DAT (
 wei_conf_id NUMBER NOT NULL,
 logic_id NUMBER(10) not null, -- ( of the strip)
 group_id number(10) not null);

ALTER TABLE FE_CONFIG_WEIGHT_DAT ADD CONSTRAINT FE_CONFIG_WEIGHT_fk  FOREIGN KEY (wei_conf_id) REFERENCES FE_CONFIG_WEIGHT_INFO (wei_conf_id);
ALTER TABLE FE_CONFIG_WEIGHT_DAT ADD CONSTRAINT FE_CONFIG_WEIGHT_fk  FOREIGN KEY (wei_conf_id, group_id) REFERENCES FE_CONFIG_WEIGHT_PER_GROUP_DAT (wei_conf_id, group_id);


/*
 *  here we have the LUT and fine grain para - values per TT
 */



create table FE_LUT_PER_GROUP_DAT(
 lut_conf_id number(10) not null,
 group_id number(10) not null,
 lut_id NUMBER(10),
 lut_value NUMBER
 );

ALTER TABLE FE_LUT_PER_GROUP_DAT ADD CONSTRAINT FE_LUT_PER_GROUP_pk PRIMARY KEY (lut_conf_id, group_id , lut_id);
ALTER TABLE FE_LUT_PER_GROUP_DAT ADD CONSTRAINT FE_LUT_PER_GROUP_fk foreign KEY (lut_conf_id) REFERENCES FE_CONFIG_LUT_INFO (lut_conf_id);


CREATE TABLE FE_CONFIG_LUT_DAT (
 lut_conf_id NUMBER NOT NULL,
 logic_id NUMBER(10) not null, -- ( of the TT)
 group_id number(10) not null);

ALTER TABLE FE_CONFIG_LUT_DAT ADD CONSTRAINT FE_CONFIG_LUT_fk  FOREIGN KEY (lut_conf_id) REFERENCES FE_CONFIG_LUT_INFO (lut_conf_id);



create table FE_fgr_PER_GROUP_DAT(
 fgr_conf_id NUMBER NOT NULL,
 group_id number(10) not null,
 threshold_low NUMBER,
 threshold_high NUMBER,
 ratio_low NUMBER,
 ratio_high NUMBER,
 lut_conf_id NUMBER	
 );

ALTER TABLE FE_fgr_PER_GROUP_DAT ADD CONSTRAINT FE_fgr_PER_GROUP_pk PRIMARY KEY (fgr_conf_id, group_id);


CREATE TABLE FE_CONFIG_FGR_DAT (
 fgr_conf_id NUMBER NOT NULL,
 logic_id NUMBER(10) not null, -- ( of the TT)
 group_id number(10) not null);

ALTER TABLE FE_CONFIG_FGR_DAT ADD CONSTRAINT FE_CONFIG_FGR_pk PRIMARY KEY (fgr_conf_id, logic_id);
ALTER TABLE FE_CONFIG_FGR_DAT ADD CONSTRAINT FE_CONFIG_FGR_fk  FOREIGN KEY (fgr_conf_id) REFERENCES FE_CONFIG_FGR_INFO (fgr_conf_id);


/* now the main table constraints */


ALTER TABLE FE_CONFIG_MAIN ADD CONSTRAINT FE_CONFIG_MAIN_to_PED_fk FOREIGN KEY (ped_conf_id) REFERENCES FE_CONFIG_PED_INFO (ped_conf_id);
ALTER TABLE FE_CONFIG_MAIN ADD CONSTRAINT FE_CONFIG_MAIN_to_lin_fk FOREIGN KEY (lin_conf_id) REFERENCES FE_CONFIG_LIN_INFO (lin_conf_id);
ALTER TABLE FE_CONFIG_MAIN ADD CONSTRAINT FE_CONFIG_MAIN_to_lut_fk FOREIGN KEY (lut_conf_id) REFERENCES FE_CONFIG_LUT_INFO (lut_conf_id);
ALTER TABLE FE_CONFIG_MAIN ADD CONSTRAINT FE_CONFIG_MAIN_to_fgr_fk FOREIGN KEY (fgr_conf_id) REFERENCES FE_CONFIG_fgr_INFO (fgr_conf_id);
ALTER TABLE FE_CONFIG_MAIN ADD CONSTRAINT FE_CONFIG_MAIN_to_sli_fk FOREIGN KEY (sli_conf_id) REFERENCES FE_CONFIG_sliding_INFO (sli_conf_id);
ALTER TABLE FE_CONFIG_MAIN ADD CONSTRAINT FE_CONFIG_MAIN_to_WEIGHT_fk FOREIGN KEY (wei_conf_id) REFERENCES FE_CONFIG_WEIGHT_INFO (wei_conf_id);




CREATE SEQUENCE FE_CONFIG_PED_SQ INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE FE_CONFIG_LIN_SQ INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE FE_CONFIG_LUT_SQ INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE FE_CONFIG_FGR_SQ INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE FE_CONFIG_SLI_SQ INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE FE_CONFIG_WEIGHT_SQ INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE FE_CONFIG_LUTGROUP_SQ INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE FE_CONFIG_FGRGROUP_SQ INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE FE_CONFIG_WEIGHTGROUP_SQ INCREMENT BY 1 START WITH 1;


/*
 *  the id is auto-incremented at each time you insert in the table 
 *  no need to bother about inserting conf_id
 */

CREATE trigger FE_CONFIG_PED_TRG
before insert on FE_CONFIG_PED_INFO
for each row
begin
select FE_CONFIG_PED_SQ.NextVal into :new.ped_conf_id from dual;
end;
/



create synonym CHANNELVIEW              for cond01.CHANNELVIEW              ;
create synonym VIEWDESCRIPTION          for cond01.VIEWDESCRIPTION          ;




