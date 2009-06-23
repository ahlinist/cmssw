package cms.dqm.workflow;

import java.io.*;
import java.sql.*;
import java.util.*;
import oracle.sql.*;
import org.json.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class EditProvider extends HttpServlet {

  private static final long serialVersionUID = 1003L;

  public enum RunType {
    ONLINE,
    OFFLINE,
    OFFLINE_NEXT,
    SIGNOFF
  };

  public final static String KEY_NUMBER = "RUN_NUMBER";
  public final static String KEY_GLOBALNAME = "RUN_GLOBALNAME";
  public final static String KEY_STOP_REASON = "RUN_STOP_REASON";
  public final static String KEY_COMMENT = "RUN_ONLINE_COMMENT";
  public final static String KEY_OFFLINE_COMMENT = "RUN_OFFLINE_COMMENT";
  public final static String KEY_STATUS = "RUN_STATUS";
  public final static String KEY_CURRENT_STATUS = "CURRENT_STATUS";
  public final static String KEY_BFIELD_COMMENT = "RUN_BFIELD_COMMENT";
  public final static String KEY_LAST_USER = "RUN_LAST_USER";
  public final static String KEY_ID = "RUN_ID";
  public final static String KEY_TAG = "TAG";

  DBWorker db = null;

  public void doPost(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {

    response.setContentType("text/xml");

    try {

      db = new DBWorker();
      db.setAutoCommit(false);  

      StringBuilder sb = new StringBuilder();
      { 
        BufferedReader reader = request.getReader();
        String line = null;
        while ((line = reader.readLine()) != null) sb.append(line);
      }

      Hashtable<String, String> data = new Hashtable<String, String>();
      {
        JSONObject obj = new JSONObject(sb.toString());	
        @SuppressWarnings("unchecked")
        Iterator<String> it = obj.keys();
        while (it.hasNext()) {
          String k = it.next();
          data.put(k, obj.getString(k));
        }
      }

      int run_number = Integer.valueOf(data.get(KEY_NUMBER));
      boolean exists = (db.getFirstRowValueInt("select count(*) from rr_last_runs where run_number = ?", run_number, 1) == 1 ? true : false);
      
      String current_status = "";
      int current_id = 0;
      int prev_tags = 0;
      if (exists) {
        current_id = db.getFirstRowValueInt("select run_last_id from rr_last_runs where run_number = ?", run_number, 1); 
        current_status = db.getFirstRowValueString("select run_status from rr_runs where run_id = ?", current_id, 1); 
        prev_tags      = db.getFirstRowValueInt("select run_count_tags from rr_last_runs where run_last_id = ?", current_id, 1); 
        prev_tags--;
      }

      String next_status = data.get(KEY_STATUS);
      if (next_status == null) next_status = current_status;
      User user = User.get(request);
      boolean delete = (data.get("DELETE") != null && data.get("DELETE").equals("true") ? true : false);

      if (!user.isLogged()) throw new Exception("Not logged in!");
      if (current_status.equals("COMPLETED")) throw new Exception("Can not edit COMLETED run!");
      if (data.get(KEY_STATUS) != null && 
         !data.get(KEY_STATUS).equals("ONLINE") && 
         !data.get(KEY_STATUS).equals("OFFLINE") && 
         !data.get(KEY_STATUS).equals("SIGNOFF") && 
         !data.get(KEY_STATUS).equals("COMPLETED")) throw new Exception("Wrong status provided [" + data.get(KEY_STATUS) + "]!");

      data.put(KEY_LAST_USER, user.getFullname());
      data.put(KEY_CURRENT_STATUS, current_status);

      if (!delete && !exists && (user.hasRole(User.ONLINE) || user.hasRole(User.EXPERT))) {

        doInsert(run_number, data);
        MessageBoardSyn.getInstance().sendMessage(user, 2, "Run #" + String.valueOf(run_number) + " has been created in Run Registry");

      } else if (delete && ((current_status.equals("ONLINE") && user.hasRole(User.ONLINE)) || user.hasRole(User.EXPERT))) {
        
        doDelete(current_id);
        MessageBoardSyn.getInstance().sendMessage(user, 2, "Run #" + String.valueOf(run_number) + " has been deleted from Run Registry");

      } else if (

        user.hasRole(User.EXPERT) ||
        (current_status.equals("ONLINE") && (next_status.equals("ONLINE") || next_status.equals("OFFLINE")) && user.hasRole(User.ONLINE)) ||
        (current_status.equals("OFFLINE") && (next_status.equals("OFFLINE") || next_status.equals("SIGNOFF")) && user.hasRole(User.OFFLINE))) {

        if (user.hasRole(User.EXPERT)) {

          if (next_status.equals("COMPLETED")) {

            if (data.get(KEY_TAG) == null || data.get(KEY_TAG) == "") throw new Exception("Tag does not provided for COMPLETED run");
            if(writeToDBS(run_number, db, null) == false) {
              throw new Exception("Writing to DBS failed. Run was not moved to COMPLETED.");
            }

          }

          doUpdate(current_id, data, RunType.SIGNOFF);

        } else {
          if (current_status.equals("ONLINE")) {
            doUpdate(current_id, data, RunType.ONLINE);    
          } else if (current_status.equals("OFFLINE")) {
            doUpdate(current_id, data, (prev_tags == 0 ? RunType.OFFLINE : RunType.OFFLINE_NEXT));
          }
        } 

        MessageBoardSyn.getInstance().sendMessage(user, 1, "Run #" + String.valueOf(run_number) + " (" + current_status + ") has been updated (status " + next_status + ")");

      } else {

        db.close();
        db = null;
      
        String message = "current_status=" + current_status +
          ", next_status=" + next_status +
          ", " + User.ONLINE + "=" + user.hasRole(User.ONLINE) +
          ", " + User.OFFLINE + "=" + user.hasRole(User.OFFLINE) +
          ", " + User.EXPERT + "=" + user.hasRole(User.EXPERT);

        throw new Exception("Bad role/status combination: " + message);
      }

      db.close();
      db = null;

    } catch (Exception e) {
      if (db != null) db.close();
      throw new ServletException(e);
    }

    response.getWriter().write("<results>OK</results>\n");

  }

  public void doDelete(int id) throws Exception {

    try {
      db.executeSQLSilent("DELETE FROM rr_run_l1sources WHERE rl1_run_id = ?", id); 
      db.executeSQLSilent("DELETE FROM rr_run_subsystems WHERE rsu_run_id = ?", id);
      db.executeSQLSilent("DELETE FROM rr_runs WHERE run_id = ?", id);
      db.commit();
    } catch (SQLException e) {
      db.rollback();
      throw e;
    }

  }

  private void doInsert(int run_number, Hashtable<String, String>  data) throws Exception {

    try {
      PreparedStatement pstmt = db.prepareSQL("select rr_runinfo.create_run( ?, ?, ?) from dual");
      pstmt.setInt(1, run_number);
      pstmt.setString(2, data.get(KEY_LAST_USER));
      pstmt.setString(3, data.get(KEY_GLOBALNAME));
      int id = db.getFirstRowValueInt(pstmt);
      data.put(KEY_CURRENT_STATUS, "ONLINE");
      doUpdate(id, data, RunType.ONLINE);
      db.commit();
    } catch (SQLException e) {
      db.rollback();
      throw e;
    }

  }

  
  private void doUpdate(int id, Hashtable<String, String>  data, RunType type) throws Exception {

    PreparedStatement pstmt, pstmt2;
    ResultSet res;
    int pindex = 1;

    try {

      switch (type) {
        case ONLINE:

          if (data.get(KEY_STATUS) != null) {
            pstmt = db.prepareSQL("UPDATE RR_RUNS SET run_status = ?, run_stop_reason = ?, run_online_comment = ?, run_globalname = ?, run_bfield_comment = ? WHERE run_id = ?");
            pstmt.setString(pindex++, data.get(KEY_STATUS));
          } else {
            pstmt = db.prepareSQL("UPDATE RR_RUNS SET run_stop_reason = ?, run_online_comment = ?, run_globalname = ?, run_bfield_comment = ? WHERE run_id = ?");
          }
          db.setStringNull(pstmt, data.get(KEY_STOP_REASON), pindex++);
          db.setStringNull(pstmt, data.get(KEY_COMMENT), pindex++);
          pstmt.setString(pindex++, data.get(KEY_GLOBALNAME));
          db.setStringNull(pstmt, data.get(KEY_BFIELD_COMMENT), pindex++);
          pstmt.setInt(pindex++, id);
          pstmt.execute();

          updateL1Sources(id, data);

          break;

        case OFFLINE:
        case OFFLINE_NEXT:

          if (data.get(KEY_STATUS) != null) {
            pstmt = db.prepareSQL("UPDATE RR_RUNS SET run_status = ?, run_offline_comment = ?, run_offline_shifter = ? WHERE run_id = ?");
            pstmt.setString(pindex++, data.get(KEY_STATUS));
          } else {
            pstmt = db.prepareSQL("UPDATE RR_RUNS SET run_offline_comment = ?, run_offline_shifter = ? WHERE run_id = ?");
          }
          db.setStringNull(pstmt, data.get(KEY_OFFLINE_COMMENT), pindex++);
          db.setStringNull(pstmt, data.get(KEY_LAST_USER), pindex++);
          pstmt.setInt(pindex++, id);
          pstmt.execute();

          break;

        case SIGNOFF:

          if (data.get(KEY_STATUS) != null) {
            if (data.get(KEY_STATUS).equals("COMPLETED")) {
              pstmt = db.prepareSQL("UPDATE RR_RUNS SET run_tag = ?, run_status = ?, run_offline_comment = ?, run_stop_reason = ?, run_online_comment = ?, run_globalname = ?, run_bfield_comment = ? WHERE run_id = ?");
              pstmt.setString(pindex++, data.get(KEY_TAG));
            } else {
              pstmt = db.prepareSQL("UPDATE RR_RUNS SET run_status = ?, run_offline_comment = ?, run_stop_reason = ?, run_online_comment = ?, run_globalname = ?, run_bfield_comment = ? WHERE run_id = ?");
            }
            pstmt.setString(pindex++, data.get(KEY_STATUS));
          } else {
            pstmt = db.prepareSQL("UPDATE RR_RUNS SET run_offline_comment = ?, run_stop_reason = ?, run_online_comment = ?, run_globalname = ?, run_bfield_comment = ? WHERE run_id = ?");
          }
          db.setStringNull(pstmt, data.get(KEY_OFFLINE_COMMENT), pindex++);
          db.setStringNull(pstmt, data.get(KEY_STOP_REASON), pindex++);
          db.setStringNull(pstmt, data.get(KEY_COMMENT), pindex++);
          pstmt.setString(pindex++, data.get(KEY_GLOBALNAME));
          db.setStringNull(pstmt, data.get(KEY_BFIELD_COMMENT), pindex++);
          pstmt.setInt(pindex++, id);
          pstmt.execute();

          updateL1Sources(id, data);

          break;

      }

      updateSubsystems(id, data);

      db.commit();

    } catch (SQLException e) {
      db.rollback();
      throw e;
    }

  }
  
  private void updateSubsystems(int id, Hashtable<String, String> data) throws Exception {

    PreparedStatement pstmt, pstmt2;
    ResultSet res;
    boolean online = data.get(KEY_CURRENT_STATUS).equals("ONLINE");

    try {

      if (online) {
        pstmt = db.prepareSQL("SELECT sub_abbr, DECODE(rsu_run_id, null, 0, 1), rsu_comment, rsu_value FROM rr_subsystems left join " +
                "(select * from rr_run_subsystems where rsu_run_id = ? and rsu_shift_type = 'ONLINE') on (rsu_sub_abbr = sub_abbr) WHERE sub_type = 'ONLINE'");
      } else {
        pstmt = db.prepareSQL("SELECT sub_abbr, DECODE(rsu_run_id, null, 0, 1), rsu_comment, rsu_value FROM rr_subsystems left join " +
                "(select * from rr_run_subsystems where rsu_run_id = ? and rsu_shift_type = 'OFFLINE') on (rsu_sub_abbr = sub_abbr)");
      }

      pstmt.setInt(1, id);
      res = pstmt.executeQuery();

      while (res.next()) {

        String abbr = res.getString(1);
        boolean exist = (res.getInt(2) == 1 ? true : false);
        String comment = res.getString(3);
        if (comment == null) comment = "";
        String value = res.getString(4);

        boolean rexists = (data.get(abbr) != null ? true : false);
        String rcomment = data.get(abbr + "_comment");
        String rvalue = null;
        if (rexists) rvalue = data.get(abbr);

        pstmt2 = null;

        if (rexists) {
          if (exist && (!comment.equals(rcomment) || !value.equals(rvalue))) {
            pstmt2 = db.prepareSQL("UPDATE RR_RUN_SUBSYSTEMS SET rsu_comment = ?, rsu_value = ? WHERE rsu_sub_abbr = ? AND rsu_run_id = ? AND rsu_shift_type = ?");
          } else 
          if (!exist) {
            pstmt2 = db.prepareSQL("INSERT INTO RR_RUN_SUBSYSTEMS (rsu_comment, rsu_value, rsu_sub_abbr, rsu_run_id, rsu_shift_type) VALUES (?, ?, ?, ?, ?)");
          }
          if (pstmt2 != null) {
            db.setStringNull(pstmt2, rcomment, 1);
            pstmt2.setString(2, rvalue);
            pstmt2.setString(3, abbr);
            pstmt2.setInt(4, id);
            if (online) pstmt2.setString(5, "ONLINE");
            else pstmt2.setString(5, "OFFLINE");
          }
        } else {
          pstmt2 = db.prepareSQL("DELETE FROM RR_RUN_SUBSYSTEMS WHERE rsu_sub_abbr = ? AND rsu_run_id = ? AND rsu_shift_type = ?");
          pstmt2.setString(1, abbr);
          pstmt2.setInt(2, id);
          if (online) pstmt2.setString(3, "ONLINE");
          else pstmt2.setString(3, "OFFLINE");
        }
        if (pstmt2 != null) pstmt2.execute();

      }

      db.commit();

    } catch (SQLException e) {
      db.rollback();
      throw e;
    }

  }
  
  private void updateL1Sources(int id, Hashtable<String, String>  data) throws Exception {

    PreparedStatement pstmt, pstmt2;
    ResultSet res;

    try {

      pstmt = db.prepareSQL("SELECT l1s_abbr, DECODE(rl1_run_id, null, 0, 1), rl1_comment " +
        "FROM rr_l1sources left join (select * from rr_run_l1sources where rl1_run_id = ? ) on (rl1_l1s_abbr = l1s_abbr)");
      pstmt.setInt(1, id);
      res = pstmt.executeQuery();

      System.err.println(id);

      while (res.next()) {
        String abbr = res.getString(1);
        System.err.println("Writing: " + abbr);

        boolean exist = (res.getInt(2) == 1 ? true : false);
        String comment = res.getString(3);
        if (comment == null) comment = "";

        System.err.println(exist);
        System.err.print("comment = ");
        System.err.println(comment);

        boolean rexists = (data.get("l1s_" + abbr) != null && data.get("l1s_" + abbr).equals("true") ? true : false);
        String rcomment = data.get("l1s_" + abbr + "_comment");

        pstmt2 = null;

        if (rexists) {
          if (exist && !comment.equals(rcomment)) {
            pstmt2 = db.prepareSQL("UPDATE RR_RUN_L1SOURCES SET rl1_comment = ? WHERE rl1_l1s_abbr = ? AND rl1_run_id = ?");
          } else 
          if (!exist) {
            pstmt2 = db.prepareSQL("INSERT INTO RR_RUN_L1SOURCES (rl1_comment, rl1_l1s_abbr, rl1_run_id) VALUES (?,?,?)");
          }
          if (pstmt2 != null) {
            db.setStringNull(pstmt2, rcomment, 1);
            pstmt2.setString(2, abbr);
            pstmt2.setInt(3, id);
          }
        } else {
          pstmt2 = db.prepareSQL("DELETE FROM RR_RUN_L1SOURCES WHERE rl1_l1s_abbr = ? AND rl1_run_id = ?");
          pstmt2.setString(1, abbr);
          pstmt2.setInt(2, id);
        }            
        if (pstmt2 != null) pstmt2.execute();
      }

    } catch (SQLException e) {
      throw e;
    }

  }

  public boolean writeToDBS(int run_number, DBWorker db, String url) throws Exception {

    boolean success = false;
    boolean dbclose = false;

    if (db == null) {
      db = new DBWorker();
      dbclose = true;
    }

    if (url == null) {
      url = WebUtils.GetEnv("dbs_url");
    }

    DBSWriter dbswriter = new DBSWriter(url);

    PreparedStatement pstmt = db.prepareSQL("SELECT \"tag\", \"value\" FROM RR_DBSFLAGS WHERE \"run\" = ? ");
    pstmt.setInt(1, run_number);
    Hashtable<String, String> map = db.getStringsMap(pstmt);
    String result = dbswriter.write(run_number, map);
    if (result.compareTo("OK") == 0) {
      success = true;
    }
    if (dbclose) { db.close(); }
    return success;
  }

}

