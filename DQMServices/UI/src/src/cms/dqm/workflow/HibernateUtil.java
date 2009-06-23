package cms.dqm.workflow;

import org.hibernate.*;
import org.hibernate.cfg.*;

public class HibernateUtil {

  private static SessionFactory sessionFactory = null;

  static {

    try {
      
      String datasource = WebUtils.GetEnv("db_name");
      String username = WebUtils.GetEnv("db_username");
      String password = WebUtils.getPassword(username);
      
      AnnotationConfiguration config = new AnnotationConfiguration();
      config.setProperty("hibernate.connection.datasource", "java:comp/env/" + datasource);
      config.setProperty("hibernate.connection.username", username);
      config.setProperty("hibernate.connection.password", password);

      sessionFactory = config.configure().buildSessionFactory();

    } catch (Exception e) {
      System.err.println(e);
      e.printStackTrace(System.err);
    }

  }

  public static SessionFactory getSessionFactory() {
    return sessionFactory;
  }

}
