package cms.dqm.workflow;

import java.io.*;
import java.sql.*;
import javax.servlet.*;
import javax.servlet.http.*;
import oracle.sql.*;
import org.json.*;
import java.util.*;
import java.util.regex.*;

public class AccessProvider extends HttpServlet {

  private static final long serialVersionUID = 1008L;

  public void init(ServletConfig config) throws ServletException {
    super.init(config);
  }

  public void doPost(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {
    doGet(request, response);
  }

  public void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {

  }

}

