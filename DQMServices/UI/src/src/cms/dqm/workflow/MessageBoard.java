package cms.dqm.workflow;

import java.io.*;
import java.sql.*;
import javax.servlet.*;
import javax.servlet.http.*;
import oracle.sql.*;
import org.json.*;
import java.util.*;
import java.util.regex.*;

public class MessageBoard extends HttpServlet {

  public void init(ServletConfig config) throws ServletException {
    super.init(config);
  }

  public boolean checkParam(String pr) {
    if (pr == null) return false;
    if (pr.equals("")) return false;
    if (pr.toLowerCase().equals("undefined")) return false;
    return true;
  }

  public synchronized void doPost(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {
    doGet(request, response);
  }

  public synchronized void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {

    String msg = (!checkParam(request.getParameter("msg"))  ? null : request.getParameter("msg"));
    long  last = (!checkParam(request.getParameter("last")) ? 0    : Long.valueOf(request.getParameter("last")));
    long  lastUser = (!checkParam(request.getParameter("last_user")) ? 0    : Long.valueOf(request.getParameter("last_user")));

    response.setContentType("text/plain");
    PrintWriter out = response.getWriter();

    try {

    DBWorker db = null;

    MessageUser user = MessageUser.get(request);

    if (msg != null) {

      JSONObject message = new JSONObject(msg);	
      String text = message.getString("text").trim();
      int priority = message.getInt("priority");

      if (text.length() != 0) {

        if (!user.isLogged()) throw new Exception("Not logged in!");
        if (db == null) db = new DBWorker();

        PreparedStatement pstmt = db.prepareSQL("select RR_MSG_BOARD.ADD_MESSAGE(?, ?, ?, ?, ?) from dual");
        db.setStringNull(pstmt, user.getName(), 1);
        db.setStringNull(pstmt, text, 2);
        pstmt.setInt(3, priority);
        db.setStringNull(pstmt, user.getRoles(), 4);
        db.setStringNull(pstmt, user.getLocation(), 5);
        pstmt.execute();

        MessageBoardSyn.getInstance().resetLast();

      }

    }

    JSONObject resp = new JSONObject();
    resp.put("logged", user.isLogged());

    JSONArray msgs = new JSONArray();

    if (MessageBoardSyn.getInstance().getLast() < 0 || MessageBoardSyn.getInstance().getLast() > last) {

      if (db == null) db = new DBWorker();
      String sql = "select msg_id id, msg_time time, msg_user_name name, msg_user_roles roles, msg_user_location loc, msg_priority prio, msg_text text " +
                   "from rr_messages_24h where msg_id > ? order by msg_id asc";

      PreparedStatement pstmt = db.prepareSQL(sql);
      pstmt.setLong(1, last);
    
      ResultSet r = pstmt.executeQuery();
      ResultSetMetaData rm = r.getMetaData();
      int cols = rm.getColumnCount();
      long newLast = MessageBoardSyn.getInstance().getLast();

      while (r.next()) {
        HashMap<String, Object> map  = new HashMap<String, Object>();
        for (int i = 1; i < cols + 1; i++) map.put(rm.getColumnName(i), r.getObject(i));
        msgs.put(map);
        newLast = r.getLong(1);
      }
      MessageBoardSyn.getInstance().setLast(newLast);
    }
    resp.put("msgs", msgs);

    JSONArray users = new JSONArray();
    if (MessageBoardSyn.getInstance().getLastUser() > lastUser) {
      Iterator<MessageUser> it = MessageBoardSyn.getInstance().getUsers();
      if (it != null) {
        while (it.hasNext()) {
          HashMap<String, Object> map  = new HashMap<String, Object>();
          MessageUser u = it.next();
          map.put("name", u.getName());
          map.put("roles", u.getRoles());
          map.put("loc", u.getLocation());
          users.put(map);
        }
      }
    }
    resp.put("users", users);

    resp.put("last_msg", MessageBoardSyn.getInstance().getLast());
    resp.put("last_user", MessageBoardSyn.getInstance().getLastUser());
    //resp.put("alive", MessageBoardSyn.getInstance().alive);
    resp.write(out);

    } catch (Exception e) {
      throw new ServletException(e);
    }

  }

}

