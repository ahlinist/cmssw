package cms.dqm.workflow;

import org.quartz.Job;
import org.quartz.JobExecutionException;
import org.quartz.JobExecutionContext;

public class MessageBoardJob implements Job {

  public void execute(JobExecutionContext context) throws JobExecutionException {
    MessageBoardSyn m = MessageBoardSyn.getInstance();
    m.refreshUsers();
  }

}
