BEGIN
  FOR result IN (SELECT table_name FROM all_catalog WHERE OWNER='CMS_COND_DT' AND table_type='TABLE')
  LOOP
    EXECUTE IMMEDIATE 'DELETE FROM CMS_COND_DT.' || result.table_name ;
  END LOOP;
END;
/
