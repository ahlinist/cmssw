
create table es_partition_description(
	tag  varchar2(32)
	, version  number
	, supermodule number
	, dcc  varchar2(32)
	, ccs  varchar2(32)
	, location  varchar2(32)
	, component_num  number
	, tcc  varchar2(32)
	, slb  varchar2(32)
);

-- insert into es_partition_description(tag, version, supermodule, dcc, ccs, location)
-- values ('VMELAB1', 0, 55, 'CMS-ES-DCC0001', '3020140B9CC528', 'B14R021VME1');

insert into es_partition_description(tag, version, supermodule, dcc, ccs, location)
values ('CF22CF23CF24_partition1170', 0, 5, 'CMS-ES-DCC0001', '3020140B9CCB0F', 'ESTIF');


create table ES_RUN_CONFIGURATION (
        CONFIGURATION_FILE_URL  varchar2(32)
        , DEFAULTS  varchar2(32)
        , run_MODE  varchar2(32)
        , NUM_OF_EVENTS  number
        , RUN_CONFIG_DESCRIPTION  varchar2(32)
        , RUN_TYPE  varchar2(32)
        , TAG  varchar2(32)
        , VERSION  number
        , TRG_MODE  varchar2(32)
);


insert into es_run_configuration(tag, version, defaults, run_type, run_mode, trg_mode)
values ('PEDESTAL', 0, 'PEDESTAL.DEFAULTS', 'PEDESTAL', 'testbeam', 'lts');

insert into es_run_configuration(tag, version, defaults, run_type, run_mode, trg_mode)
values ('PEDESTAL_rslu', 0, 'PEDESTAL.DEFAULTS', 'PEDESTAL', 'testbeam', 'lts');

CREATE TABLE ES_SEQUENCE_TYPE_DEF (
	SEQUENCE_NUM  NUMBER NOT NULL
	, SEQUENCE_ID  VARCHAR2(20)
        , SEQUENCE_TYPE VARCHAR2(20)
	, NUM_OF_CYCLES NUMBER NOT NULL
	, TAG  VARCHAR2(20)
	, VERSION NUMBER
);

insert into ES_SEQUENCE_TYPE_DEF(SEQUENCE_NUM, SEQUENCE_ID, SEQUENCE_TYPE, NUM_OF_CYCLES, TAG, VERSION)
values (0, 'PEDESTAL.STD', 'STD', 2, 'PEDESTAL',0);

CREATE TABLE ES_SEQUENCE (
	SEQUENCE_NUM  NUMBER NOT NULL
	, SEQUENCE_ID  VARCHAR2(20)
        , SEQUENCE_TYPE VARCHAR2(20)
	, NUM_OF_CYCLES NUMBER NOT NULL
	, TAG  VARCHAR2(20)
	, VERSION NUMBER
);

insert into ES_SEQUENCE(SEQUENCE_NUM, SEQUENCE_ID, SEQUENCE_TYPE, NUM_OF_CYCLES, TAG, VERSION)
values (0, 'PEDESTAL.STD', 'STD', 2, 'PEDESTAL',0);



create table es_cycle(
 CYCLE_ID                                   VARCHAR2(32)
 ,TAG                                        VARCHAR2(64)
 ,VERSION                                    NUMBER(22)
 ,SEQUENCE_NUM                               NUMBER(22)
 ,SEQUENCE_ID                                NUMBER
 ,CYCLE_NUM                                  NUMBER(22)
 ,CYCLE_TAG                                  VARCHAR2(64)
 ,DESCRIPTION                                VARCHAR2(200)
 ,CCS_CONFIGURATION_ID                       VARCHAR2(32)
 ,DCC_CONFIGURATION_ID                       VARCHAR2(32)
 ,LASER_CONFIGURATION_ID                     VARCHAR2(32)
 ,LTC_CONFIGURATION_ID                       VARCHAR2(32)
 ,LTS_CONFIGURATION_ID                       VARCHAR2(32)
 ,DCU_CONFIGURATION_ID                       VARCHAR2(32)
 ,TCC_CONFIGURATION_ID                       VARCHAR2(32)
 ,TTCF_CONFIGURATION_ID                      VARCHAR2(32)
 ,SRP_CONFIGURATION_ID                       VARCHAR2(32)
 ,TTCci_CONFIGURATION_ID                     VARCHAR2(32)
 ,JBH4_CONFIGURATION_ID                      VARCHAR2(32)
 ,SCAN_ID                                    NUMBER
);

insert into es_cycle(cycle_id, tag, version, lts_configuration_id, dcc_configuration_id, ccs_configuration_id)
values ('PEDESTAL,DEFAULTS', 'PEDESTAL',0, 'PEDESTAL', 'DCC-LOCAL', 'PEDESTAL.STD.HIGH');


create table es_cycle_view (
        CCS_CONFIGURATION_ID  VARCHAR2(20)
        ,CYCLE_ID  VARCHAR2(20)
        ,CYCLE_NUM  NUMBER
        ,DCC_CONFIGURATION_ID  VARCHAR2(20)
        ,LASER_CONFIGURATION_ID  VARCHAR2(20)
        ,LTC_CONFIGURATION_ID  VARCHAR2(20)
        ,LTS_CONFIGURATION_ID  VARCHAR2(20)
        ,SCAN_COMMAND  VARCHAR2(20)
        ,SEQUENCE_ID  VARCHAR2(20)
        ,TCC_CONFIGURATION_ID  VARCHAR2(20)
        ,TTCci_CONFIGURATION_ID  VARCHAR2(20)
        ,MATACQ_CONFIGURATION_ID  VARCHAR2(20)
        ,JBH4_CONFIGURATION_ID  VARCHAR2(20)
        ,FRL_CONFIGURATION_ID  VARCHAR2(20)
        ,SCAN_ID  NUMBER
);

insert into es_cycle_view(cycle_id, lts_configuration_id, dcc_configuration_id, ccs_configuration_id)
values ('PEDESTAL.DEFAULTS', 'PEDESTAL', 'DCC-LOCAL', 'PEDESTAL.STD.HIGH');

create table es_ttcci_configuration (
	TTCci_configuration_id  VARCHAR2(20)
	, Configuration  VARCHAR2(20)
);


create table es_ccs_configuration(
        CCS_CONFIGURATION_ID VARCHAR2(32)
        ,DELAY NUMBER
        ,GAIN VARCHAR2(32)
        ,CLOCK NUMBER
        ,BGO_SOURCE VARCHAR2(32)
);

insert into es_ccs_configuration(ccs_configuration_id, gain, delay, clock, bgo_source)
values ('PEDESTAL.STD.HIGH', 'HIGH', 3888, -1, 'Internal');


create table es_dcc_configuration(
	DCC_CONFIGURATION_ID  VARCHAR2(32)
        ,DEVICE_CONFIG_PARAM_ID  VARCHAR2(32)
        ,DCC_CONFIGURATION_URL  VARCHAR2(100)
        ,DCC_ID  VARCHAR2(32)
        ,CONFIGURATION_ITEM_NAME  VARCHAR2(32)
        ,CONFIGURATION_ITEM_VALUE  NUMBER
);

insert into es_dcc_configuration(dcc_configuration_id, dcc_configuration_url)
values ('DCC-LOCAL', '/eshome/estif/DAQ/RunTime/common/config/esdcc/esDCC_Configuration_local.xml');



create table ES_FED_ID(
	ES_FED_ID_ID  varchar2(32)
	, SM  number
	, FED_ID  varchar2(32)
);

insert into es_fed_id(es_fed_id_id, sm, fed_id)
values ('MTCC', 5, '606');
insert into es_fed_id(es_fed_id_id, sm, fed_id)
values ('TB06', 27, '27');

create table ES_JBH4_CONFIGURATION(
        JBH4_CONFIGURATION_ID  varchar2(32)
        ,useBuffer  number
        ,halModuleFile  varchar2(32)
        ,halAddressTableFile  varchar2(32)
        ,halStaticTableFile  varchar2(32)
        ,halCbd8210SerialNumber  varchar2(32)
        ,caenBridgeType  varchar2(32)
        ,caenLinkNumber  number
        ,caenBoardNumber  number
);

insert into es_jbh4_configuration(jbh4_configuration_id, usebuffer, halmodulefile, haladdresstablefile, halstatictablefile, halcbd8210serialnumber, caenbridgetype, caenlinknumber, caenboardnumber)
values ('PEDESTAL', 1, '/etc/daq/Modules.dat', '/etc/daq/AddressTable.dat','/etc/daq/StaticVME.dat','JBH4VME_1','V2718', 0, 0);

insert into ecal_lts_configuration(lts_configuration_id, num_of_events, rate, trigger_type, trig_loc_l1_delay, lts_tag)
values (11, 1000, 100, 'PEDESTAL', 104, 'PEDESTAL');

