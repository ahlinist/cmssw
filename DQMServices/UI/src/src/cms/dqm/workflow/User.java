package cms.dqm.workflow;

import java.util.*;
import javax.servlet.*;
import javax.servlet.http.*;
import java.security.cert.*;

public class User {
    
  public static final String ONLINE = "ONLINE";
  public static final String OFFLINE = "OFFLINE";
  public static final String EXPERT = "EXPERT";

  private Integer id = null;
  private Vector<String> roles = new Vector<String>();
  private String location = null;

  private String username = null;
  private String fullname = null;
  private String certinfo = null;
  private String certid = null;
  private String mbid = null;
    
  public User(HttpServletRequest request) {
    if (request.isSecure() && request.getHeader("ADFS_LOGIN") != null) {
      username = request.getHeader("ADFS_LOGIN");
      fullname = request.getHeader("ADFS_FULLNAME");
      certinfo = getCertInfo(request);
      certid = WebUtils.md5(certinfo);
      mbid = WebUtils.md5(username + "/" + certid + "/" + location);
    }
    if (isLogged()) MessageBoardSyn.getInstance().loginUser(this);
  }

  public String getUsername() { return username; }

  public String getFullname() { return fullname; }

  public String getCertinfo() { return certinfo; }

  public String getCertid()   { return certid; }

  public Vector<String> getRolesV() { return roles; }

  public String getRoles() {
    String r = null;
    for (Iterator i = roles.iterator(); i.hasNext(); ) {
      r = (r == null ? "" : r + ",") + i.next();
    }
    return r;
  }

  public boolean hasRole(String role) {
    role = role.toUpperCase();
    return (roles.indexOf(role) > -1);
  }

  public boolean isLogged() { 
    return (mbid != null);
  }

  public boolean isAuthorized() { 
    return (isLogged() && id != null);
  }

  public String getLocation() {
    return location;
  }

  public String getMessageBoardId() { return mbid; }

  public String getMessageLine() {
    String s = getFullname();
    if (isAuthorized()) {
      s = s + " (" + getRoles() + ")@" + getLocation();
    }
    return s;
  }

  public static User get(HttpServletRequest request) {
    User user = (User) request.getSession().getAttribute("rr_user");
    if (user == null) {
      user = new User(request);
      request.setAttribute("rr_user", user);
    }
    return user;
  }

  public static String getCertInfo(HttpServletRequest request) {
    String cert = null;
    X509Certificate[] certs = (X509Certificate[])request.getAttribute("javax.servlet.request.X509Certificate");
    if (certs != null) {
      for (int i = 0; i < certs.length; i++) {
        cert += certs[i].getSubjectDN().getName();
      }
    }
    return cert;
  }

}

