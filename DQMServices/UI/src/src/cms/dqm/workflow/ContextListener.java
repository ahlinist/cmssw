package cms.dqm.workflow;

import java.io.File;
import javax.servlet.ServletContext;
import javax.servlet.ServletContextAttributeEvent;
import javax.servlet.ServletContextAttributeListener;
import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;

public class ContextListener implements ServletContextAttributeListener,ServletContextListener  {

  private ServletContext context = null;

  public void contextInitialized(ServletContextEvent event) {
    this.context = event.getServletContext();
    //CacheSyn.getInstance().setCacheDir((File) context.getAttribute("javax.servlet.context.tempdir"));
    CacheSyn.getInstance().setCacheDir(new File(context.getRealPath(WebUtils.GetEnv("cache_dir"))));
    MessageBoardSyn.getInstance();
  }

  public void contextDestroyed(ServletContextEvent event) {
    CacheSyn.destroy();
    MessageBoardSyn.destroy();
    this.context = null;
  }

  //This method is invoked when an attribute
  //is added to the ServletContext object
  public void attributeAdded (ServletContextAttributeEvent scab) {
  }

  //This method is invoked when an attribute
  //is removed from the ServletContext object
  public void attributeRemoved (ServletContextAttributeEvent scab) {
  }

  //This method is invoked when an attribute
  //is replaced in the ServletContext object
  public void attributeReplaced (ServletContextAttributeEvent scab) {
  }

}
