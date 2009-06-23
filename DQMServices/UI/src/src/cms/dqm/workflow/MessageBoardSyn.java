package cms.dqm.workflow;

import java.util.*;
import java.sql.*;
import javax.servlet.http.HttpSession;
import javax.servlet.http.HttpServletRequest;

public class MessageBoardSyn {

  static private MessageBoardSyn instance = null;

  private long last;
  private long lastUser;
  private Hashtable<String, UserSyn> users = new Hashtable<String, UserSyn>();
  private Vector<User> user_list = new Vector<User>();

  private MessageBoardSyn() {
    resetLast();
    lastUser = 1;
  }

  static public MessageBoardSyn getInstance() {
    if (null == instance) {
      instance = new MessageBoardSyn();
    }
    return instance;
  }

  static public void destroy() {
    instance = null;
  }

  public long getLast() { return last; }
  public void setLast(long last_) { last = last_; }
  public void resetLast() { last = -1; }
  public long getLastUser() { return lastUser; }

  public void loginUser(User user) {
    if (!user.isLogged()) return;
    String id = user.getMessageBoardId();
    if (!users.containsKey(id)) {
      users.put(id, new UserSyn(user));
      user_list.add(user);
      sendMessage("User " + user.getMessageLine() + " logged in", 0);
      lastUser++;
    } else {
      users.get(id).loginNext();
    }
  }

  public void logoutUser(User user) {
    logoutUser(user.getMessageBoardId());
  }

  private void logoutUser(String id) {
    if (!users.containsKey(id)) return;
    if (users.get(id).logoutLast()) {
      User user = users.remove(id).getUser();
      user_list.remove(user);
      if (user != null) {
        if (user.isLogged()) {
          sendMessage("User " + user.getMessageLine() + " logged out", 0);
        }
        lastUser++;
      }
    }
  }

  public void pingUser(User user) {
    String id = user.getMessageBoardId();
    if (id == null) return;
    if (users.containsKey(id)) {
      if (!user.isLogged()) logoutUser(id);
      else users.get(id).ping();
    }
  }

  public void refreshUsers() {
    long now = Calendar.getInstance().getTimeInMillis();

    Enumeration<String> it = users.keys();
    while (it.hasMoreElements()) {
      String id = it.nextElement();
      if (users.get(id).getAlive() < now || !users.get(id).getUser().isLogged()) {
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

  public Iterator<User> getUsers() { 
    return user_list.iterator();
  }

  public void sendMessage(User user, int priority, String text) {
    try {
      DBWorker db = new DBWorker();
      PreparedStatement pstmt = db.prepareSQL("select RR_MSG_BOARD.ADD_MESSAGE(?, ?, ?, ?, ?) from dual");
      db.setStringNull(pstmt, user.getFullname(), 1);
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

  private class UserSyn {

    private User user;
    private int  count = 0;
    private long alive = 0;

    public UserSyn(User user_) {
      user = user_;
      loginNext();
    }

    public void loginNext() {
      count++; 
      ping();
    }

    public void ping() {
      alive = Calendar.getInstance().getTimeInMillis() + (30 * 1000);
    }

    public boolean logoutLast() {
      count--;
      return (count == 0);
    }

    public long getAlive() { return alive; }
    public User getUser()  { return user; }

  };

}

