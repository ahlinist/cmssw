/**
  *  New Model DDL generated by Schemester
  *  12/02/2008 10:05:14
  */


/*  LMF_RUN_TAG  */
CREATE TABLE LMF_RUN_TAG
(
  GEN_TAG VARCHAR2(100),
  TAG_ID  NUMBER NOT NULL
)
/

ALTER TABLE LMF_RUN_TAG
  ADD CONSTRAINT LMF_RUN_TAG_PK PRIMARY KEY (TAG_ID)
/


/*  LMF_RUN_IOV  */
CREATE TABLE LMF_RUN_IOV
(
  LMF_IOV_ID    NUMBER NOT NULL,
  TAG_ID        NUMBER NOT NULL,
  RUN_IOV_ID    NUMBER(10) NOT NULL,
  SUB_RUN_NUM   NUMBER NOT NULL,
  SUB_RUN_START DATE NOT NULL,
  SUB_RUN_END   DATE NOT NULL,
  SUB_RUN_TYPE  VARCHAR2(20) NOT NULL,
  DB_TIMESTAMP  DATE NOT NULL
)
/

ALTER TABLE LMF_RUN_IOV
  ADD CONSTRAINT LMF_RUN_IOV_PK PRIMARY KEY (LMF_IOV_ID)
/

ALTER TABLE LMF_RUN_IOV
  ADD CONSTRAINT LMF_RUN_IOV_UK UNIQUE (RUN_IOV_ID,SUB_RUN_NUM)
/

ALTER TABLE LMF_RUN_IOV
  ADD CONSTRAINT LMF_RUN_IOV_FK1 FOREIGN KEY (RUN_IOV_ID)
  REFERENCES RUN_IOV(IOV_ID)
/

ALTER TABLE LMF_RUN_IOV
  ADD CONSTRAINT LMF_RUN_IOV_FK2 FOREIGN KEY (TAG_ID)
  REFERENCES LMF_RUN_TAG(TAG_ID)
/


/*  LMF_LASER_CONFIG_DAT  */
CREATE TABLE LMF_LASER_CONFIG_DAT
(
  LMF_IOV_ID     NUMBER NOT NULL,
  LOGIC_ID       NUMBER NOT NULL,
  WAVELENGTH     NUMBER,
  VFE_GAIN       NUMBER,
  PN_GAIN        NUMBER,
  LSR_POWER      NUMBER,
  LSR_ATTENUATOR NUMBER,
  LSR_CURRENT    NUMBER,
  LSR_DELAY_1    NUMBER,
  LSR_DELAY_2    NUMBER
)
/

ALTER TABLE LMF_LASER_CONFIG_DAT
  ADD CONSTRAINT LMF_LASER_CONFIG_DAT_PK PRIMARY KEY (LMF_IOV_ID,LOGIC_ID)
/

ALTER TABLE LMF_LASER_CONFIG_DAT
  ADD CONSTRAINT LMF_LASER_CONFIG_DAT_FK1 FOREIGN KEY (LMF_IOV_ID)
  REFERENCES LMF_RUN_IOV(LMF_IOV_ID)
/


/*  LMF_RUN_DAT  */
CREATE TABLE LMF_RUN_DAT
(
  LMF_IOV_ID   NUMBER NOT NULL,
  LOGIC_ID     NUMBER NOT NULL,
  NEVENTS      NUMBER,
  QUALITY_FLAG NUMBER
)
/

ALTER TABLE LMF_RUN_DAT
  ADD CONSTRAINT LMF_RUN_DAT_PK PRIMARY KEY (LMF_IOV_ID,LOGIC_ID)
/

ALTER TABLE LMF_RUN_DAT
  ADD CONSTRAINT LMF_RUN_DAT_FK1 FOREIGN KEY (LMF_IOV_ID)
  REFERENCES LMF_RUN_IOV(LMF_IOV_ID)
/


/*  LMF_TEST_PULSE_CONFIG_DAT  */
CREATE TABLE LMF_TEST_PULSE_CONFIG_DAT
(
  LMF_IOV_ID NUMBER NOT NULL,
  LOGIC_ID   NUMBER NOT NULL,
  VFE_GAIN   NUMBER,
  DAC_MGPA   NUMBER,
  PN_GAIN    NUMBER,
  PN_VINJ    NUMBER
)
/

ALTER TABLE LMF_TEST_PULSE_CONFIG_DAT
  ADD CONSTRAINT LMF_TEST_PULSE_CONFIG_DAT_PK PRIMARY KEY (LMF_IOV_ID,LOGIC_ID)
/

ALTER TABLE LMF_TEST_PULSE_CONFIG_DAT
  ADD CONSTRAINT LMF_TEST_PULSE_CONFIG_DAT_FK1 FOREIGN KEY (LMF_IOV_ID)
  REFERENCES LMF_RUN_IOV(LMF_IOV_ID)
/


/*  LMF_LASER_PULSE_DAT  */
CREATE TABLE LMF_LASER_BLUE_PULSE_DAT
(
  LMF_IOV_ID  NUMBER NOT NULL,
  LOGIC_ID    NUMBER NOT NULL,
  FIT_METHOD  NUMBER,
  MTQ_AMPL    NUMBER,
  MTQ_TIME    NUMBER,
  MTQ_RISE    NUMBER,
  MTQ_FWHM    NUMBER,
  MTQ_FW20    NUMBER,
  MTQ_FW80    NUMBER,
  MTQ_SLIDING NUMBER
)
/

ALTER TABLE LMF_LASER_BLUE_PULSE_DAT
  ADD CONSTRAINT LMF_LASER_BLUE_PULSE_DAT_PK PRIMARY KEY (LMF_IOV_ID,LOGIC_ID)
/

ALTER TABLE LMF_LASER_BLUE_PULSE_DAT
  ADD CONSTRAINT LMF_LASER_BLUE_PULSE_DAT_FK1 FOREIGN KEY (LMF_IOV_ID)
  REFERENCES LMF_RUN_IOV(LMF_IOV_ID)
/


/*  LMF_LASER_PRIM_DAT  */
CREATE TABLE LMF_LASER_BLUE_PRIM_DAT
(
  LMF_IOV_ID        NUMBER NOT NULL,
  LOGIC_ID          NUMBER NOT NULL,
  FLAG              NUMBER,
  MEAN              NUMBER,
  RMS               NUMBER,
  PEAK              NUMBER,
  APD_OVER_PNA_MEAN NUMBER,
  APD_OVER_PNA_RMS  NUMBER,
  APD_OVER_PNA_PEAK NUMBER,
  APD_OVER_PNB_MEAN NUMBER,
  APD_OVER_PNB_RMS  NUMBER,
  APD_OVER_PNB_PEAK NUMBER,
  APD_OVER_PN_MEAN  NUMBER,
  APD_OVER_PN_RMS   NUMBER,
  APD_OVER_PN_PEAK  NUMBER,
  ALPHA             NUMBER,
  BETA              NUMBER,
  SHAPE_COR         NUMBER
)
/

ALTER TABLE LMF_LASER_BLUE_PRIM_DAT
  ADD CONSTRAINT LMF_LASER_BLUE_PRIM_DAT_PK PRIMARY KEY (LMF_IOV_ID,LOGIC_ID)
/

ALTER TABLE LMF_LASER_BLUE_PRIM_DAT
  ADD CONSTRAINT LMF_LASER_BLUE_PRIM_DAT_FK1 FOREIGN KEY (LMF_IOV_ID)
  REFERENCES LMF_RUN_IOV(LMF_IOV_ID)
/


/*  LMF_CALIB_PRIM_DAT  */
CREATE TABLE LMF_CALIB_PRIM_DAT
(
  LMF_IOV_ID NUMBER NOT NULL,
  LOGIC_ID   NUMBER NOT NULL,
  FLAG       NUMBER,
  MEAN       NUMBER,
  RMS        NUMBER,
  PEAK       NUMBER
)
/

ALTER TABLE LMF_CALIB_PRIM_DAT
  ADD CONSTRAINT LMF_CALIB_PRIM_DAT_PK PRIMARY KEY (LMF_IOV_ID,LOGIC_ID)
/

ALTER TABLE LMF_CALIB_PRIM_DAT
  ADD CONSTRAINT LMF_CALIB_PRIM_DAT_FK FOREIGN KEY (LMF_IOV_ID)
  REFERENCES LMF_RUN_IOV(LMF_IOV_ID)
/


/*  RUN_LASERRUN_CONFIG_DAT  */
CREATE TABLE RUN_LASERRUN_CONFIG_DAT
(
  IOV_ID              NUMBER NOT NULL,
  LOGIC_ID            NUMBER NOT NULL,
  LASER_SEQUENCE_TYPE VARCHAR2(20),
  LASER_SEQUENCE_COND VARCHAR2(20)
)
/

ALTER TABLE RUN_LASERRUN_CONFIG_DAT
  ADD CONSTRAINT RUN_LASERRUN_CONFIG_DAT_PK UNIQUE (IOV_ID,LOGIC_ID)
/

ALTER TABLE RUN_LASERRUN_CONFIG_DAT
  ADD CONSTRAINT RUN_LASERRUN_CONFIG_DAT_FK FOREIGN KEY (IOV_ID)
  REFERENCES RUN_IOV(IOV_ID)
/



/*  LMF_LASER_PN_PRIM_DAT  */
CREATE TABLE LMF_LASER_BLUE_PN_PRIM_DAT
(
  LMF_IOV_ID        NUMBER NOT NULL,
  LOGIC_ID          NUMBER NOT NULL,
  FLAG              NUMBER,
  MEAN              NUMBER,
  RMS               NUMBER,
  PEAK              NUMBER,
  PNA_OVER_PNB_MEAN NUMBER,
  PNA_OVER_PNB_RMS  NUMBER,
  PNA_OVER_PNB_PEAK NUMBER
)
/

ALTER TABLE LMF_LASER_BLUE_PN_PRIM_DAT
  ADD CONSTRAINT LMF_LASER_BLUE_PN_DAT_PK PRIMARY KEY (LMF_IOV_ID,LOGIC_ID)
/

ALTER TABLE LMF_LASER_BLUE_PN_PRIM_DAT
  ADD CONSTRAINT LMF_LASER_BLUE_PN_PRIM_DAT_FK FOREIGN KEY (LMF_IOV_ID)
  REFERENCES LMF_RUN_IOV(LMF_IOV_ID)
/


