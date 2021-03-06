CREATE TABLE OD_MON_DAT (
	RUN_START    	DATE DEFAULT SYSTIMESTAMP NOT NULL,
	LOGIC_ID 	NUMBER(10), -- THE FED/CCU CHANNEL
	DAQ_STATE       NUMBER(2),  -- DAQ STATE AT THE TIME OF READING
	FE_MON_WORD_1	NUMBER(10), -- FE MONITORING STATUS WORD AT CONFIG TIME
	FE_MON_WORD_2	NUMBER(10), -- FE MONITORING STATUS WORD AT TIMESTAMP
	CCS_TR_WORD_1   NUMBER(10), -- CCS BOARD + TOKEN RINGS STATUS WORD 
				    -- AT CONFIG TIME
	CCS_TR_WORD_2   NUMBER(10), -- CCS BOARD + TOKEN RINGS STATUS WORD 
				    -- AT TIMESTAMP
        DB_TIMESTAMP    TIMESTAMP DEFAULT SYSTIMESTAMP NOT NULL
);

ALTER TABLE OD_MON_DAT ADD CONSTRAINT od_mon_dat_pk PRIMARY KEY (RUN_START, 
	LOGIC_ID);
CREATE INDEX od_mon_dat_ix ON OD_MON_DAT(LOGIC_ID);
