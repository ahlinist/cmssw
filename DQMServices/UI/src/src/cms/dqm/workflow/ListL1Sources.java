package cms.dqm.workflow;

import java.io.*;
import java.sql.*;
import oracle.sql.*;
import javax.servlet.jsp.JspException;
import javax.servlet.jsp.tagext.SimpleTagSupport;

public class ListL1Sources extends SimpleTagSupport {

  public void doTag() throws JspException, IOException {
    try {

      DBWorker db = new DBWorker();
      ResultSet res = db.executeSQL("SELECT l1s_abbr FROM rr_l1sources ORDER BY 1");

      while (res.next()) {
        getJspContext().setAttribute("l1s_abbr", res.getString(1));
        getJspBody().invoke(null);
      }

      res.close();
      db.close();

    } catch(Exception e) {
      throw new JspException(e);
    }
  }

}

