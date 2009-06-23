package cms.dqm.workflow;

import java.io.*;
import java.sql.*;
import oracle.sql.*;
import javax.servlet.jsp.JspException;
import javax.servlet.jsp.tagext.SimpleTagSupport;

public class ListRoles extends SimpleTagSupport {

  public void doTag() throws JspException, IOException {
    try {

      DBWorker db = new DBWorker();
      ResultSet res = db.executeSQL("SELECT ROL_NAME FROM rr_roles ORDER BY 1");

      while (res.next()) {
        getJspContext().setAttribute("role", res.getString(1));
        getJspBody().invoke(null);
      }

      res.close();
      db.close();

    } catch(Exception e) {
      throw new JspException(e);
    }
  }

}

