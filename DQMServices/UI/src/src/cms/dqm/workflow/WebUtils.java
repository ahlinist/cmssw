package cms.dqm.workflow;

import java.util.*;
import javax.naming.*;
import java.util.regex.*;
import java.security.cert.*;
import javax.servlet.*;
import javax.servlet.http.*;
import java.security.*;
import java.math.*;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;

public class WebUtils {

  public static final String ONLINE = "ONLINE";
  public static final String OFFLINE = "OFFLINE";
  public static final String EXPERT = "EXPERT";
  public static Properties osenv = new Properties();

  private static Pattern p1 = Pattern.compile("OU\\=\"*([a-zA-Z,]+)\"*");
  private static Pattern p2 = Pattern.compile("dqm(\\w+)");
  private static Pattern p3 = Pattern.compile("CN\\=\"*([^\",]+)\"*");

  private static Map<String, String> auths = new HashMap<String, String>();

  static {
    try {
      osenv.load(Runtime.getRuntime().exec("env").getInputStream());
    } catch (java.io.IOException e) { }
  }

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

  public static String md5(String s) {
    try {
      MessageDigest m = MessageDigest.getInstance("MD5");
      m.update(s.getBytes(), 0, s.length());
      return (new BigInteger(1, m.digest())).toString(16);
    } catch (java.security.NoSuchAlgorithmException e) {
      return s;
    }
  }

  public static String getPassword(String username) {

    if (auths.containsKey(username)) return auths.get(username);
    return WebUtils.getPassword(username, osenv.get("HOME") + "/" + WebUtils.GetEnv("db_auth_file"));

  }

  public static String getPassword(String username, String auth_file) {

    if (auths.containsKey(username)) return auths.get(username);
    if (auth_file == null) return null;

    String password = null;

    try {
      FileInputStream in = new FileInputStream(auth_file);
      BufferedReader br = new BufferedReader(new InputStreamReader(in));
      String line;
      Pattern p = Pattern.compile("^" + username + "/(.+)$");
      while ((line = br.readLine()) != null) {
        Matcher m = p.matcher(line);
        if (m.find()) {
          password = m.group(1);
          break;
        }
      }
      in.close();
      if (password != null && password != "") {
        auths.put(username, password);
      }
    } catch (Exception e) { }

    return password;
  }


}
