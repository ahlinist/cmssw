package cms.dqm.workflow;

import java.util.*;
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
      lastUser++;
    }
    alive.put(user.getId(), Calendar.getInstance().getTimeInMillis() + (30 * 1000));
  }

  public void logoutUser(MessageUser user) {
    if (users.remove(user.getId()) != null) lastUser++;
    alive.remove(user.getId());
  }

  public void refreshUsers() {
    
    long now = Calendar.getInstance().getTimeInMillis();
    boolean changed = false;

    Enumeration<String> it = alive.keys();
    while (it.hasMoreElements()) {
      String id = it.nextElement();
      if (alive.get(id).longValue() < now) {
        users.remove(id);
        alive.remove(id);
        changed = true;
      }
    }

    if (changed) {
      lastUser++;
    }

  }

  public Iterator<MessageUser> getUsers() { 
    return users.values().iterator();
  }

}

