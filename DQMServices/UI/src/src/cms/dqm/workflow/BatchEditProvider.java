package cms.dqm.workflow;

import java.io.*;
import java.sql.*;
import java.util.*;
import oracle.sql.*;
import org.w3c.dom.*;
import javax.servlet.*;
import javax.servlet.http.*;
import javax.xml.parsers.*;

public class BatchEditProvider extends HttpServlet {

  public void batchStatusUpdate(int run_number, String status, DBWorker db, String url, String username, String tag) throws Exception {

      if (status.equals("COMPLETED")) {

        if (tag == null || tag == "") {
          throw new Exception("Tag value is not provided for COMPLETED run state!");
        }

        EditProvider eprovider = new EditProvider();
        if (!eprovider.writeToDBS(run_number, db, url)) {
          throw new Exception("Write to DBS did not succeeded!");
        }

      }

      PreparedStatement pstmt = db.prepareSQL("select rr_runinfo.update_run_status(?, ?, ?, ?) from dual");
      pstmt.setInt(1, run_number);
      pstmt.setString(2, status);
      pstmt.setString(3, username);
      pstmt.setString(4, tag);
      pstmt.execute();

  }

  public void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {

    response.setContentType("text/xml");

    String action = request.getParameter("action");
    int run_number = Integer.parseInt(request.getParameter("run_number"));

    DBWorker db = null;

    try {

      if (!MessageUser.get(request).hasLoggedRole(WebUtils.EXPERT)) throw new Exception("No rights!");

      db = new DBWorker();

      if (action.equals("changestatus")) {
        String status = request.getParameter("status");
        String tag = request.getParameter("tag");
        batchStatusUpdate(run_number, status, db, null, MessageUser.get(request).getName(), tag);
      }

      db.close();

    } catch(Exception e) {
      if (db != null) db.close();
      throw new ServletException(e);
    }

    response.getWriter().print("<result>OK</result>");

  }

  public static void main(String [] args) {

    String url  = args[0];
    String auth = args[1];
    String user = args[2];
    String tag  = args[3];

    System.out.println("url  = " + url);
    System.out.println("auth = " + auth);

    try {
      DBWorker db = new DBWorker(url, auth);
      BatchEditProvider beprovider = new BatchEditProvider();
      Vector<String> numbers = db.getStringList("select run_number from rr_runs where run_globalname like 'CRAFT%' and run_number > 67219 order by run_number", 1);
      db.close();

      for (int i = 0; i < numbers.size(); i++) {
        db = new DBWorker(url, auth);
        System.out.print("Writing run #" + numbers.elementAt(i) + "...");
        beprovider.batchStatusUpdate(Integer.valueOf(numbers.elementAt(i)), "COMPLETED", db, "http://localhost:9999", user, tag);
        System.out.println("OK");
        db.close();
      }

      db.close();

    } catch (Exception e) {
      System.out.println("ERROR : " + e);
    }
  }

}

