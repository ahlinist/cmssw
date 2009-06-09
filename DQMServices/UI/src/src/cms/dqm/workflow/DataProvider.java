package cms.dqm.workflow;

import java.io.*;
import java.sql.*;
import javax.servlet.*;
import javax.servlet.http.*;
import oracle.sql.*;
import org.json.*;
import java.util.*;
import java.util.regex.*;

import org.w3c.dom.*;
/*
import org.xml.sax.InputSource;
import javax.xml.transform.*;
import javax.xml.transform.stream.*;
*/
import javax.xml.parsers.*;
import javax.xml.transform.dom.*;
import javax.xml.transform.Templates;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;


public class DataProvider extends HttpServlet {

  private static final long serialVersionUID = 1002L;

  enum FormatType { JSON, XML };

  private String query;
  private int default_page_size;
  private String default_sort_name;
  private String default_sort_dir;
  private FormatType default_format;
  private Pattern fieldPattern;
  private Pattern sortPattern;
  private String servlet_name;

  DocumentBuilderFactory factory = null;
  DocumentBuilder builder = null;
  TransformerFactory tf = null;

  public void init(ServletConfig config) throws ServletException {
    super.init(config);
    servlet_name = config.getServletName();
    query = config.getInitParameter("query");
    default_page_size = (config.getInitParameter("default_page_size") == null ? 0     : Integer.parseInt(getServletConfig().getInitParameter("default_page_size")));
    default_sort_name = (config.getInitParameter("default_sort_name")  == null ? "1"   : getServletConfig().getInitParameter("default_sort_name"));
    default_sort_dir  = (config.getInitParameter("default_sort_dir")  == null ? "asc" : getServletConfig().getInitParameter("default_sort_dir"));
    default_format  = FormatType.JSON;
    if (config.getInitParameter("format")  != null) {
      if (config.getInitParameter("format").equals("xml")) {
        default_format  = FormatType.XML;
      }
    }
    fieldPattern = Pattern.compile("^[a-zA-Z_0-9]+$");
    sortPattern = Pattern.compile("^(asc|desc|ASC|DESC)$");

    try {
      factory = DocumentBuilderFactory.newInstance();
      builder = factory.newDocumentBuilder();
      tf = TransformerFactory.newInstance();
    } catch (Exception e) {
      throw new ServletException(e);
    }
  }

  public void doPost(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {
    doGet(request, response);
  }

  public boolean checkParam(String pr)
  {
    if (pr == null) return false;
    if (pr.equals("")) return false;
    if (pr.toLowerCase().equals("undefined")) return false;

    return true;
  }

  public String checkSQL(String f) throws ServletException {
    if (
        (f.indexOf('\'') > -1) ||
        (f.indexOf('\"') > -1) ||
        (f.indexOf(';')  > -1)
    ) throw new ServletException("Invalid character in filter expr.");

    return f;
  }

  public void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {

    PrintWriter out = response.getWriter();
    String query_string = request.getQueryString();
    String cache_url = CacheSyn.getInstance().getCachedContent(servlet_name, query_string);
    if (cache_url != null) {
      response.sendRedirect(cache_url);
      return;
    }

    int page = (!checkParam(request.getParameter("page")) ? 1 : Integer.parseInt(request.getParameter("page")));
    int page_size = (!checkParam(request.getParameter("rp")) ? default_page_size : Integer.parseInt(request.getParameter("rp")));
    String sort_name = (!checkParam(request.getParameter("sortname")) ? default_sort_name : checkSQL(request.getParameter("sortname")));
    String sort_dir = (!checkParam(request.getParameter("sortorder")) ? default_sort_dir : checkSQL(request.getParameter("sortorder")));
    String filter_field = (!checkParam(request.getParameter("qtype")) ? null : checkSQL(request.getParameter("qtype")));
    String filter_value = (!checkParam(request.getParameter("query")) ? null : checkSQL(request.getParameter("query")));
    String filter_values = (!checkParam(request.getParameter("querya")) ? null : request.getParameter("querya"));
    String filter_operator = (!checkParam(request.getParameter("qoperator")) ? "OR" : checkSQL(request.getParameter("qoperator")));
    boolean debug = (!checkParam(request.getParameter("debug")) ? false : true);
    String template  = request.getParameter("tpl");
    String intemplate = request.getParameter("intpl");

    FormatType format = default_format;
    String mimeType = "text/xml";
    if (checkParam(request.getParameter("format"))) {
      if (request.getParameter("format").equals("xml"))  {
        format  = FormatType.XML;
        mimeType = "text/xml";
      } else {
        if (request.getParameter("format").equals("json")) {
          format  = FormatType.JSON;
          mimeType = "text/plain";
        }
      }
    }

    Matcher m = null;

    if (sort_dir != default_sort_dir) {
      m = sortPattern.matcher(sort_dir);
      if (!m.find()) sort_dir = default_sort_dir;
    }

    if (sort_name != default_sort_name) {
      m = fieldPattern.matcher(sort_name);
      if (!m.find()) sort_name = default_sort_name;
    }

    if (checkParam(request.getParameter("mime"))) {
      mimeType = request.getParameter("mime");
    }

    boolean where_exists = false;
    boolean where_exists2 = false;
    String where = "";
 
    if (filter_values != null) {
      try {
        where = "WHERE ";
        JSONArray fvalues = new JSONArray(filter_values);	
        for (int i = 0; i < fvalues.length(); i++) {
          JSONObject o = fvalues.getJSONObject(i);
          String field = o.getString("field");
          m = fieldPattern.matcher(field);
          if (m.find()) {
            if ((o.has("range_from")) && (o.has("range_to"))) {
              where = where + (i > 0 ? filter_operator : "") + " ( ? < " + field + " and ? > " + field + ") ";
            } else {
              where = where + (i > 0 ? filter_operator : "") + " REGEXP_LIKE(" +  field + ", ?, 'i') ";
            }
          }
	}
			
        where_exists2 = true;
      } catch (JSONException e) {
        throw new ServletException(e.toString() + "\nJSON Object: " + filter_values);
      }
		
    } else { 
      if (filter_field != null && filter_value != null) {
        m = fieldPattern.matcher(filter_field);
        if (m.find()) {
          filter_field = filter_field.replaceAll("'", "\'");
          filter_value = filter_value.replaceAll("'", "\'");
          where = "WHERE REGEXP_LIKE(" + filter_field + ", ?, 'i')";
          where_exists = true;
        }
      }
    }

    JSONArray arr = null;
    Document doc = null;
    Element results = null;

    try {

      response.setContentType(mimeType);
      switch (format) {
        case JSON:
          arr = new JSONArray();
          break;
        case XML:
          doc = builder.newDocument();
          results = doc.createElement("RESULTS");
          doc.appendChild(results);
          break;
      }

      DBWorker db = new DBWorker();
      PreparedStatement pstmt = null;
      ResultSet r = null;

      long totalResultsAvailable = 0;
      if (page_size > 0) {

        String count_sql = "select count(*) from (select * from (" + query + ") "+ where + ")";
        pstmt = db.prepareSQL(count_sql);


        if (where_exists) {
          pstmt.setString(1, filter_value);
        } else {
          if (where_exists2) {
            try {
              JSONArray fvalues = new JSONArray(filter_values);
              int p = 1;
              for (int i = 0; i < fvalues.length(); i++) {
                JSONObject o = fvalues.getJSONObject(i);
                if ((o.has("range_from")) && (o.has("range_to"))) {
                  pstmt.setDouble(p++, o.getDouble("range_from")); 
                  pstmt.setDouble(p++, o.getDouble("range_to"));
                } else {
                  pstmt.setString(p++, o.getString("value"));
                }
              }
            } catch (JSONException e) {
              throw new ServletException(e.toString() + "\nJSON Object: " + filter_values);
            }
          }
        }
        r = pstmt.executeQuery();
        r.next();
        totalResultsAvailable = r.getLong(1);
        r.close();
      }

      long totalResultsReturned = 0;

      int page_start = (page - 1) * page_size;
      String sql = "SELECT a.* FROM (" +
        "SELECT rownum num, a.* FROM (" +
          "SELECT a.* FROM (" + 
            query + 
          ") a " + where + " ORDER BY " + sort_name + " " + sort_dir +
        ") a";
      if (page_size > 0) {
        sql += " WHERE rownum <= ? ";
      }
      sql += ") a WHERE num >= ? ";

      pstmt = db.prepareSQL(sql);
      int pnum = 1;
      if (where_exists) {
        pstmt.setString(pnum++, filter_value);
      } else {
        if (where_exists2) {
          try {
            JSONArray fvalues = new JSONArray(filter_values);
	  
            for (int i = 0; i < fvalues.length(); i++) {
              JSONObject o = fvalues.getJSONObject(i);
              if ((o.has("range_from")) && (o.has("range_to"))) { 
                pstmt.setDouble(pnum++, o.getDouble("range_from")); 
                pstmt.setDouble(pnum++, o.getDouble("range_to"));
              }	else {
                pstmt.setString(pnum++, o.getString("value"));
              }
            }
          } catch (JSONException e) {
            throw new ServletException(e.toString() + "\nJSON Object: " + filter_values);
          }
        }
      }

      if (page_size > 0) pstmt.setInt(pnum++, page_start + page_size);
      pstmt.setInt(pnum++, page_start + 1);
      r = pstmt.executeQuery();

      ResultSetMetaData rm = r.getMetaData();
      int cols = rm.getColumnCount();

      String [] colnames  = new String[cols];
      for (int i = 1; i < cols + 1; i++) colnames[i - 1] = rm.getColumnName(i);

      while (r.next()) {

        switch (format) {
          case JSON:

            HashMap<String, Object> map  = new HashMap<String, Object>();
            for (int i = 1; i < cols + 1; i++) map.put(colnames[i - 1], r.getObject(i));
            arr.put(map);

            break;
          case XML:

            Element row = doc.createElement("ROW");
            results.appendChild(row);
            for (int i = 1; i <= cols; i++) {
              Object value      = r.getObject(i);
              Element node      = doc.createElement(colnames[i - 1]);
              if (value != null) node.appendChild(doc.createTextNode(value.toString()));
              row.appendChild(node);
            }

            break;
        }

        totalResultsReturned++;
      }

      r.close();
      db.close();

      if (totalResultsAvailable == 0) totalResultsAvailable = totalResultsReturned;
      String content = "";

      switch (format) {

        case JSON:

          HashMap<String, Object> list  = new HashMap<String, Object>();
          if (debug) {
            list.put("query", query);
            list.put("sql", sql);
            list.put("page_start", page_start);
            list.put("page_size", page_size);
            list.put("sort_name", sort_name);
            list.put("sort_dir", sort_dir);
            list.put("default_page_size", default_page_size);
            list.put("default_sort_name", default_sort_name);
            list.put("default_sort_dir", default_sort_dir);
            list.put("where", where);
          }
          list.put("total", totalResultsAvailable);
          list.put("page", page);
          list.put("rp", page_size);
          list.put("rows", arr);
          JSONObject o = new JSONObject(list);
          content = o.toString();

          break;

        case XML:

          String [] intemplates = { };
          if (intemplate != null) {
            intemplates = intemplate.split(",");
          }

          if (debug) {
            results.setAttribute("query", query);
            results.setAttribute("sql", sql);
            results.setAttribute("sort_name", sort_name);
            results.setAttribute("sort_dir", sort_dir);
            results.setAttribute("default_page_size", String.valueOf(default_page_size));
            results.setAttribute("default_sort_name", default_sort_name);
            results.setAttribute("default_sort_dir", default_sort_dir);
            results.setAttribute("where", where);
            results.setAttribute("avalues", filter_values);
            results.setAttribute("aoperator", filter_operator);
            for (int i = 0; i < intemplates.length; i++) {
              results.setAttribute("internal_template_" + String.valueOf(i), intemplates[i]);
              results.setAttribute("internal_template_" + String.valueOf(i) + "_file", this.getClass().getResource("/templates/" + intemplates[i] + ".xsl").getPath());
            }
          }
          results.setAttribute("total", String.valueOf(totalResultsAvailable));
          results.setAttribute("page", String.valueOf(page));
          results.setAttribute("rp", String.valueOf(page_size));

          CacheSyn cache = CacheSyn.getInstance();
          for (int i = 0; i < intemplates.length; i++) {
            String t = intemplates[i];
            Templates translet = null;
            if (cache.hasTemplate(t)) {
              translet = cache.getTemplate(t);
            } else {
              m = fieldPattern.matcher(t);
              if (m.find()) {
                String filename = this.getClass().getResource("/templates/" + t + ".xsl").getPath();
                translet = tf.newTemplates(new StreamSource(filename));
                cache.setTemplate(t, translet);
              }
            }
            DOMSource domSource = new DOMSource(doc);
            DOMResult result = new DOMResult();
            Transformer transformer = translet.newTransformer();
            transformer.transform(domSource, result);
            doc = (Document) result.getNode();
          }

          if (template != null) {
            ProcessingInstruction process = doc.createProcessingInstruction("xml-stylesheet", "type=\"text/xsl\" href=\"templates/" + template + ".xsl\"");
            doc.insertBefore(process, doc.getDocumentElement());
          }

          DOMSource domSource = new DOMSource(doc);
          StringWriter stringWriter = new StringWriter();
          StreamResult result = new StreamResult(stringWriter);

          Transformer transformer = tf.newTransformer();
          transformer.setOutputProperty("media-type", mimeType);
          transformer.setOutputProperty("omit-xml-declaration", "yes");
          transformer.transform(domSource, result);
          content = stringWriter.getBuffer().toString();

          break;
      }

      response.sendRedirect(CacheSyn.getInstance().setCachedContent(servlet_name, query_string, content, mimeType));

    } catch(SQLException e) {
      throw new ServletException(e.toString() + "\nWHERE clause: " + where);
    } catch(Exception e) {
      throw new ServletException(e);
    }
  }

}

