package cms.dqm.workflow;

import java.util.*;
import javax.servlet.http.HttpSession;
import javax.servlet.http.HttpServletRequest;

public class MessageBoardSyn {

  static private MessageBoardSyn instance = null;
  private long last;
  private long lastUser;

  Hashtable<String, MessageUser> users;

  private MessageBoardSyn() {
    users = new Hashtable<String, MessageUser>();
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
    if (users.get(user.getId()) != null) return;
    users.put(user.getId(), user);
    lastUser++;
  }

  public void logoutUser(MessageUser user) {
    if (!user.isLogged()) return;
    if (users.get(user.getId()) != null) return;
    users.remove(user.getId());
    lastUser++;
  }

  public Iterator<MessageUser> getUsers() { 
    return users.values().iterator();
  }

}

