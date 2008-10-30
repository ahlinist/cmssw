BEGIN
  FOR idx IN (SELECT regid FROM user_change_notification_regs)
  LOOP
    DBMS_CHANGE_NOTIFICATION.DEREGISTER (idx.regid);
  END LOOP;
END;
/
