package cms.dqm.workflow;

import java.io.*;
import java.sql.*;
import oracle.sql.*;
import javax.servlet.jsp.*;
import javax.servlet.jsp.tagext.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class ListSubsystemsJS extends TagSupport {

  private static final long serialVersionUID = 1005L;

  public int doStartTag() throws JspException {
    try {

      JspWriter out = pageContext.getOut();
      HttpServletResponse response = (HttpServletResponse)pageContext.getResponse();
      DBWorker db = new DBWorker();

      ResultSet res = db.executeSQL("SELECT sub_abbr FROM rr_subsystems ORDER BY 1");

      boolean first = true;
      while (res.next()) {
        if (!first) out.write(",");
        out.write("\"" + res.getString(1) + "\"");
        first = false;
      }

      res.close();
      db.close();

    } catch(Exception e) {
      throw new JspException(e);
    }

    return EVAL_PAGE;

  }

}

