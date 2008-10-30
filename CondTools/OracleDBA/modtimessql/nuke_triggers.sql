BEGIN
  FOR idx IN (SELECT trigger_name FROM user_triggers)
  LOOP
    EXECUTE IMMEDIATE 'DROP TRIGGER '||idx.trigger_name;
  END LOOP;
END;
/
