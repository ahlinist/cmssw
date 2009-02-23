package cms.dqm.workflow;

import java.io.*;
import java.sql.*;
import oracle.sql.*;
import javax.servlet.jsp.JspException;
import javax.servlet.jsp.tagext.SimpleTagSupport;

public class ListSubsystems extends SimpleTagSupport {

  private String type;
  private int run;

  public ListSubsystems() throws Exception {
    type = null;
    run = 0;
  }

  public void doTag() throws JspException, IOException {
    try {

      DBWorker db = new DBWorker();
      String sql;

      if (run == 0) {
        if (type == null) {
          sql = "SELECT sub_abbr, null FROM rr_subsystems ORDER BY 1";
        } else {
          sql = "SELECT sub_abbr, null FROM rr_subsystems WHERE sub_type = ? ORDER BY 1";
        }
      } else {
        if (type == null) {
          sql = "SELECT rsu_abbr, rsu_value FROM rr_run_subsystems WHERE rsu_run_number = ? ORDER BY 1";
        } else {
          sql = "SELECT rsu_abbr, rsu_value FROM rr_run_subsystems, rr_subsystems WHERE rsu_sub_abbr = sub_abbr and rsu_run_number = ? and sub_type = ? ORDER BY 1";
        }
      }

      PreparedStatement stat = db.prepareSQL(sql);

      int num = 1;
      if (type != null) stat.setString(num++, type);
      if (run != 0) stat.setInt(num++, run);

      ResultSet res = stat.executeQuery();

      while (res.next()) {
        getJspContext().setAttribute("sub_abbr", res.getString(1));
        getJspContext().setAttribute("sub_value", res.getString(2));
        getJspBody().invoke(null);
      }

      res.close();
      db.close();

    } catch(Exception e) {
      throw new JspException(e);
    }
  }

  public void setType(String type) {
    this.type = type;
  }

  public void setRun(int run) {
    this.run = run;
  }

}

