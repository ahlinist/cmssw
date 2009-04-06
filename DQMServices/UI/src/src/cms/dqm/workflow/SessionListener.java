package cms.dqm.workflow;

import javax.servlet.http.HttpSessionListener;
import javax.servlet.http.HttpSessionEvent;

public class SessionListener implements HttpSessionListener {

  public void sessionCreated(HttpSessionEvent se) {
    MessageBoardSyn.getInstance().addSession(se.getSession());
  }

  public void sessionDestroyed(HttpSessionEvent se) {
    MessageBoardSyn.getInstance().remSession(se.getSession());
  }

}
