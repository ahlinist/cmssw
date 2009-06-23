package cms.dqm.workflow;

import javax.servlet.*;

public class HibernateListener implements ServletContextListener {

  public void contextInitialized(ServletContextEvent event) {
    HibernateUtil.getSessionFactory();
  }

  public void contextDestroyed(ServletContextEvent event) {
    HibernateUtil.getSessionFactory().close();
  }

}

