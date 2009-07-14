CREATE OR REPLACE FUNCTION INSTANCE_CHECK(run in number, numInstance in number, maxInstance in number, currentSETUP in VARCHAR2, maxClosed in number)
    RETURN NUMBER AS
    status NUMBER(5);
    lastNumInstance NUMBER(5);
BEGIN
    status := 0;

    
    IF (maxClosed < 10) THEN
         RETURN status;
    END IF;

    SELECT COUNT(INSTANCE) INTO lastNumInstance FROM (SELECT RUNNUMBER, INSTANCE, SETUPLABEL, DENSE_RANK() OVER (ORDER BY RUNNUMBER DESC NULLS LAST) RUN_RANK
         FROM SM_INSTANCES WHERE RUNNUMBER < run AND SETUPLABEL = currentSETUP) WHERE RUN_RANK = 1;

    IF (numInstance != lastNumInstance) THEN
         status := 1;
    END IF;
    
    IF (numInstance != maxInstance + 1) THEN
         status := 1;
    END IF;

    RETURN status;
END INSTANCE_CHECK; 
/ 

CREATE OR REPLACE FUNCTION GENERATE_FLAG_CLOSED(run in number, maxNum in number, maxLastWrite in DATE)
    RETURN VARCHAR2 AS
    flag VARCHAR2(1000);
    threshold NUMBER(10);
    numFlagged NUMBER(5); 
BEGIN
    --Determine minimum below which should be flagged.

    flag := ' ';
    numFlagged := 0;
    FOR entry IN (SELECT * FROM SM_INSTANCES WHERE RUNNUMBER = run)
    LOOP
	IF ( (time_diff(maxLastWrite, entry.Last_Write_Time) > 240) ) THEN --AND (maxNum - NVL(entry.N_INJECTED, 0) > .30 * maxNum) )  THEN
		numFlagged := numFlagged + 1;
		IF (numFlagged = 1) THEN
			flag := flag || 'CLOSED:';
		END IF;
		flag := flag || ' ' || TO_CHAR(entry.INSTANCE) || '(' || TO_CHAR(NVL(entry.N_INJECTED,0)) || ')'; 
	END IF;
    END LOOP;

    RETURN flag;
END GENERATE_FLAG_CLOSED;
/

CREATE OR REPLACE FUNCTION GENERATE_FLAG_SAFE0(run in number, maxRatio in number, sumNum in number)
    RETURN VARCHAR2 AS
    flag VARCHAR2(1000);
    threshold NUMBER(10);
    numFlagged NUMBER(5); 
BEGIN
    --Determine minimum below which should be flagged.

    flag := ' ';
    numFlagged := 0;
    FOR entry IN (SELECT * FROM SM_INSTANCES WHERE RUNNUMBER = run)
    LOOP
	IF ( (NVL(entry.N_INJECTED,0) * maxRatio - NVL(entry.N_NEW,0)) > 50) OR (sumNum > 50 AND NVL(entry.N_NEW,0) = 0) THEN
		numFlagged := numFlagged + 1;
		IF (numFlagged = 1) THEN
			flag := flag || 'SAFE0:';
		END IF;
		flag := flag || ' ' || TO_CHAR(entry.INSTANCE) || '(' || TO_CHAR(NVL(entry.N_NEW,0)) || ')'; 
	END IF;
    END LOOP;

    RETURN flag;
END GENERATE_FLAG_SAFE0;
/

CREATE OR REPLACE FUNCTION GENERATE_FLAG_SAFE99(run in number, maxRatio in number, sumNum in number)
    RETURN VARCHAR2 AS
    flag VARCHAR2(1000);
    threshold NUMBER(10);
    numFlagged NUMBER(5); 
BEGIN
    --Determine minimum below which should be flagged.

    flag := ' ';
    numFlagged := 0;
    FOR entry IN (SELECT * FROM SM_INSTANCES WHERE RUNNUMBER = run)
    LOOP
	IF ( (NVL(entry.N_NEW,0) * maxRatio - NVL(entry.N_CHECKED,0)) > 50) OR (sumNum > 50 AND NVL(entry.N_CHECKED,0) = 0) THEN
		numFlagged := numFlagged + 1;
		IF (numFlagged = 1) THEN
			flag := flag || 'SAFE99:';
		END IF;
		flag := flag || ' ' || TO_CHAR(entry.INSTANCE) || '(' || TO_CHAR(NVL(entry.N_CHECKED,0)) || ')'; 
	END IF;
    END LOOP;

    RETURN flag;
END GENERATE_FLAG_SAFE99;
/

CREATE OR REPLACE FUNCTION GENERATE_FLAG_DELETED(run in number, maxLastClosedTime in DATE)
    RETURN VARCHAR2 AS
    flag VARCHAR2(1000);
    numFlagged NUMBER(5); 
BEGIN
    --Determine minimum below which should be flagged.

    flag := ' ';
    numFlagged := 0;
    
    IF (time_diff(sysdate, maxLastClosedTime) < 9000) THEN
        RETURN flag;
    END IF;

    FOR entry IN (SELECT * FROM SM_INSTANCES WHERE RUNNUMBER = run)
    LOOP
	IF ( (NVL(entry.N_NEW,0) = NVL(entry.N_CHECKED,0) ) AND ( NVL(entry.N_CHECKED,0) > NVL(entry.N_DELETED,0) ) ) THEN
		numFlagged := numFlagged + 1;
		IF (numFlagged = 1) THEN
			flag := flag || 'DELETED:';
		END IF;
		flag := flag || ' ' || TO_CHAR(entry.INSTANCE) || '(' || TO_CHAR(NVL(entry.N_DELETED,0)) || ')'; 
	END IF;
    END LOOP;

    RETURN flag;
END GENERATE_FLAG_DELETED;
/

create or replace view view_sm_instance_summary
AS SELECT "RUN_NUMBER",
          "N_INSTANCES",
          "MIN_CLOSED",
          "MAX_CLOSED",
	  "MIN_SAFE0",
          "MAX_SAFE0",
          "MIN_SAFE99",
          "MAX_SAFE99",
          "MIN_DELETED",
          "MAX_DELETED",
          "INSTANCE_STATUS",
          "FLAGS"
FROM (SELECT TO_CHAR( RUNNUMBER ) AS RUN_NUMBER,
             TO_CHAR( TO_CHAR(COUNT(INSTANCE)) || '/' || TO_CHAR(MAX(INSTANCE) + 1)  ) AS N_INSTANCES,
             TO_CHAR( MIN(NVL(N_INJECTED, 0))) AS MIN_CLOSED,
             TO_CHAR( MAX(NVL(N_INJECTED, 0))) AS MAX_CLOSED,
             TO_CHAR( MIN(NVL(N_NEW, 0))) AS MIN_SAFE0,
             TO_CHAR( MAX(NVL(N_NEW, 0))) AS MAX_SAFE0,
             TO_CHAR( MIN(NVL(N_CHECKED, 0))) AS MIN_SAFE99,
             TO_CHAR( MAX(NVL(N_CHECKED, 0))) AS MAX_SAFE99,
             TO_CHAR( MIN(NVL(N_DELETED, 0))) AS MIN_DELETED,
             TO_CHAR( MAX(NVL(N_DELETED, 0))) AS MAX_DELETED,
             TO_CHAR( INSTANCE_CHECK(RUNNUMBER, COUNT(INSTANCE), MAX(INSTANCE), MAX(SETUPLABEL), MAX(NVL(N_INJECTED, 0)) ) ) AS INSTANCE_STATUS,
             TO_CHAR( GENERATE_FLAG_CLOSED(RUNNUMBER, MAX(N_INJECTED), MAX(LAST_WRITE_TIME)) ||
                      GENERATE_FLAG_SAFE0(RUNNUMBER, MAX(N_NEW / NVL(N_INJECTED, 1)), MAX(N_NEW) ) ||
                      GENERATE_FLAG_SAFE99(RUNNUMBER, MAX(N_CHECKED / NVL(N_NEW, 1)), MAX(N_CHECKED) ) ||
                      GENERATE_FLAG_DELETED(RUNNUMBER, MAX(LAST_WRITE_TIME) ) ) AS FLAGS
FROM (SELECT RUNNUMBER, INSTANCE, SETUPLABEL, N_CREATED, N_INJECTED, N_NEW, N_COPIED, N_CHECKED, N_INSERTED, N_REPACKED, N_DELETED, LAST_WRITE_TIME, DENSE_RANK() OVER (ORDER BY RUNNUMBER DESC NULLS LAST) run
FROM SM_INSTANCES)
WHERE run <= 10
GROUP BY RUNNUMBER
ORDER BY 1 DESC);

grant select on view_sm_instance_summary to public;
