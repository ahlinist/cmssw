package cms.dqm.workflow;

import org.hibernate.*;
import cms.dqm.workflow.model.*;

public class Model {

  public static Run getRun(int number) {
    Session session = HibernateUtil.getSessionFactory().getCurrentSession();
    session.beginTransaction();
    Query q = session.createQuery("from Run run where run.number = ?").setInteger(0, number);
    Run run = (Run) q.uniqueResult();
    session.getTransaction().commit();
    return run;
  }

}
