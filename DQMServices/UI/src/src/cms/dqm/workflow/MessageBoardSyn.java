package cms.dqm.workflow;

import java.util.*;
import javax.servlet.http.HttpSession;
import javax.servlet.http.HttpServletRequest;

public class MessageBoardSyn {

  static private MessageBoardSyn instance = null;
  private long last;

  Hashtable<String, HttpSession> sessions;

  private MessageBoardSyn() {
    resetLast();
  }

  public long getLast() { return last; }
  public void setLast(long last_) { last = last_; }
  public void resetLast() { last = -1; }

  static public MessageBoardSyn getInstance() {
    if (null == instance) {
      instance = new MessageBoardSyn();
    }
    return instance;
  }

  public void addSession(HttpSession session) {
    String id = session.getId();
    if (sessions.get(id) != null) return;
    sessions.put(id, session);
  }

  public void remSession(HttpSession session) {
    sessions.remove(session.getId());
  }


}

