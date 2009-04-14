package cms.dqm.workflow;

import java.util.*;
import java.sql.*;
import javax.servlet.http.HttpSession;
import javax.servlet.http.HttpServletRequest;

public class MessageBoardSyn {

  static private MessageBoardSyn instance = null;
  private long last;
  private long lastUser;

  private Hashtable<String, MessageUser> users;
  private Hashtable<String, Long>        alive;

  private MessageBoardSyn() {
    users = new Hashtable<String, MessageUser>();
    alive = new Hashtable<String, Long>();
    resetLast();
    lastUser = 1;
  }

  public long getLast() { return last; }
  public void setLast(long last_) { last = last_; }
  public void resetLast() { last = -1; }
  public long getLastUser() { return lastUser; }

  static public MessageBoardSyn getInstance() {
    if (null == instance) {
      instance = new MessageBoardSyn();
    }
    return instance;
  }

  public void loginUser(MessageUser user) {
    if (!user.isLogged()) return;
    if (!users.containsKey(user.getId())) {
      users.put(user.getId(), user);
      sendMessage("User " + user.getMessageLine() + " logged in", 0);
      lastUser++;
    }
    alive.put(user.getId(), Calendar.getInstance().getTimeInMillis() + (30 * 1000));
  }

  public void logoutUser(MessageUser user) {
    logoutUser(user.getId());
  }

  private void logoutUser(String userId) {
    MessageUser user = users.remove(userId);
    if (user != null) {
      if (user.isLogged()) {
        sendMessage("User " + user.getMessageLine() + " logged out", 0);
      }
      lastUser++;
    }
    alive.remove(userId);
  }

  public void refreshUsers() {
    
    long now = Calendar.getInstance().getTimeInMillis();

    Enumeration<String> it = alive.keys();
    while (it.hasMoreElements()) {
      String id = it.nextElement();
      if (alive.get(id).longValue() < now || !users.get(id).isLogged()) {
        logoutUser(id);
      }
    }

    try {
      DBWorker db = new DBWorker();
      PreparedStatement pstmt = db.prepareSQL("select max(msg_id) from rr_messages");
      last = db.getFirstRowValueInt(pstmt);
      db.close();
    } catch (Exception e) { }

  }

  public Iterator<MessageUser> getUsers() { 
    return users.values().iterator();
  }

  public void sendMessage(MessageUser user, int priority, String text) {
    try {
      DBWorker db = new DBWorker();
      PreparedStatement pstmt = db.prepareSQL("select RR_MSG_BOARD.ADD_MESSAGE(?, ?, ?, ?, ?) from dual");
      db.setStringNull(pstmt, user.getName(), 1);
      db.setStringNull(pstmt, text, 2);
      pstmt.setInt(3, priority);
      db.setStringNull(pstmt, user.getRoles(), 4);
      db.setStringNull(pstmt, user.getLocation(), 5);
      last = db.getFirstRowValueInt(pstmt);
      db.close();
    } catch (Exception e) { }
  }

  public void sendMessage(String text, int priority) {
    try {
      DBWorker db = new DBWorker();
      PreparedStatement pstmt = db.prepareSQL("select RR_MSG_BOARD.ADD_SYS_MESSAGE(?, ?) from dual");
      db.setStringNull(pstmt, text, 1);
      pstmt.setInt(2, priority);
      last = db.getFirstRowValueInt(pstmt);
      db.close();
    } catch (Exception e) { }
  }

}

