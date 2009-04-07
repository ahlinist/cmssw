package cms.dqm.workflow;

import org.quartz.*;

public class MessageBoardJob implements Job {

  public void execute(JobExecutionContext context) throws JobExecutionException {
    //MessageBoardSyn.getInstance().refreshUsers();
  }

}
