package cms.dqm.workflow;

import java.util.Vector;
import javax.naming.*;
import java.util.regex.*;
import java.security.cert.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class WebUtils {

  public static final String ONLINE = "ONLINE";
  public static final String OFFLINE = "OFFLINE";
  public static final String EXPERT = "EXPERT";

  private static Pattern p1 = Pattern.compile("OU\\=\"*([a-zA-Z,]+)\"*");
  private static Pattern p2 = Pattern.compile("dqm(\\w+)");
  private static Pattern p3 = Pattern.compile("CN\\=\"*([a-zA-Z,]+)\"*");

  public static String getLoggedUser(HttpServletRequest request) {
    if (request.isSecure() && request.getHeader("ADFS_LOGIN") != null) {
      return request.getHeader("ADFS_FULLNAME");
    }
    return null;
  }

  public static Vector<String> getLoggedRoles(HttpServletRequest request) {
    Vector<String> v = new Vector<String>();
    X509Certificate[] certs = (X509Certificate[])request.getAttribute("javax.servlet.request.X509Certificate");
    if (certs != null) {
      for (int i = 0; i < certs.length; i++) {
        Matcher m = p1.matcher(certs[i].getSubjectDN().getName());
        if (m.find()) {
          String role_string = m.group(1);
          String[] roles = role_string.split(",");
          for (int j = 0; j < roles.length; j++) {
            m = p2.matcher(roles[j]);
            if (m.find()) {
              v.add(m.group(1).toUpperCase());
            }
          }
        }
      }
    }
    return v;
  }

  public static String getCertCN(HttpServletRequest request) {
    X509Certificate[] certs = (X509Certificate[])request.getAttribute("javax.servlet.request.X509Certificate");
    if (certs != null) {
      for (int i = 0; i < certs.length; i++) {
        Matcher m = p3.matcher(certs[i].getSubjectDN().getName());
        if (m.find()) return m.group(1);
      }
    }
    return null;
  }

  public static String getCertInfo(HttpServletRequest request) {
    String cert = "";
    X509Certificate[] certs = (X509Certificate[])request.getAttribute("javax.servlet.request.X509Certificate");
    if (certs != null) {
      for (int i = 0; i < certs.length; i++) {
        cert += certs[i].getSubjectDN().getName();
      }
    }
    return cert;
  }

  public static String GetEnv(String name) {
    try {
      Context initCtx = new InitialContext();
      return (String)initCtx.lookup("java:comp/env/" + name);
    } catch(Exception e) { }
    return null;
  }

  public static String IfNull(String s) {
    return IfNull(s, s);
  }

  public static String IfNull(String s, String result) {
    if (s == null) return "";
    return (result == null ? s : result);
  }

  public static String IfEqual(String s1, String s2, String result) {
    if (s1 == null && s2 == null) return result;
    if ((s1 == null && s2 != null) || (s1 != null && s2 == null)) return "";
    if (s1.compareTo(s2) != 0) return "";
    return result;
  }

  public static boolean EqualStrings(String s1, String s2) {
    if (s1 == null && s2 == null) return true;
    if (s1 != null && s2 != null && s1.equals(s2)) return true;
    return false;
  }

}
