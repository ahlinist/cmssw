package cms.dqm.workflow;

import java.io.*;
import java.sql.*;
import oracle.sql.*;
import javax.servlet.jsp.JspException;
import javax.servlet.jsp.tagext.SimpleTagSupport;

public class ListGlobalNames extends SimpleTagSupport {

  private static final long serialVersionUID = 1005L;

  public void doTag() throws JspException, IOException {
    try {

      DBWorker db = new DBWorker();

      ResultSet res = db.executeSQL("SELECT distinct run_globalname FROM rr_runs ORDER BY 1");

      while (res.next()) {
        getJspContext().setAttribute("global_name", res.getString(1));
        getJspBody().invoke(null);
      }

      res.close();
      db.close();

    } catch(Exception e) {
      throw new JspException(e);
    }

  }

}

