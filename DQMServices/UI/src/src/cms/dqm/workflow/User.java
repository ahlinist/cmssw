package cms.dqm.workflow;

import java.util.*;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import javax.servlet.*;
import javax.servlet.http.*;
import java.security.cert.*;

public class User {
    
  public static final String ONLINE = "ONLINE";
  public static final String OFFLINE = "OFFLINE";
  public static final String EXPERT = "EXPERT";
  public static final String COOKIE = "RR_USER";

  private Integer id = null;
  private Vector<String> roles = new Vector<String>();
  private String location = null;

  private String username = null;
  private String fullname = null;
  private String certinfo = null;
  private String certid = null;
  private String mbid = null;
    
  public User(HttpServletRequest request) throws Exception {
    if (request.isSecure() && request.getHeader("ADFS_LOGIN") != null) {
      
      username = request.getHeader("ADFS_LOGIN");
      fullname = request.getHeader("ADFS_FULLNAME");
      certinfo = getCertInfo(request);
      certid = WebUtils.md5(certinfo);
      mbid = WebUtils.md5(username + "/" + certid + "/" + location);

      DBWorker db = new DBWorker();
      try {
        PreparedStatement pstmt = db.prepareSQL("select ID, LOCATION from RR_ACTIVE_USERS where (CERT_ID = ? or CERT_ID is null) and (USERNAME = ? or USERNAME is null)");
        pstmt.setString(1, certid);
        pstmt.setString(2, username);
        ResultSet res = pstmt.executeQuery();
        res.next();
        id = res.getInt(1);
        location = res.getString(2);
        pstmt = db.prepareSQL("select ROL_NAME from RR_ACTIVE_ROLES where USR_ID = ?");
        pstmt.setInt(1, id);
        roles = db.getStringList(pstmt, 1);
        pstmt = db.prepareSQL("insert into RR_LOGINS (LOG_USR_ID, LOG_IP) values (?, ?)"); 
        pstmt.setInt(1, id);
        pstmt.setString(2, request.getRemoteAddr());
        pstmt.executeUpdate();
      } catch (Exception e) {
        id = null;
        roles = new Vector<String>();
        location = null;
      }
      db.close();
    }
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

  public static User get(HttpServletRequest request) throws Exception {
    
    HttpSession session = request.getSession();
    User user = (User) session.getAttribute(COOKIE);

    if (user != null) {
      if (!user.isLogged() && (request.isSecure() && request.getHeader("ADFS_LOGIN") != null)) {
        user = null;
      } else
      if (user.isLogged() &&  (!request.isSecure() || request.getHeader("ADFS_LOGIN") == null)) {
        user = null;
      }
    }

    if (user == null) {
      user = new User(request);
      session.setAttribute(COOKIE, user);
      if (user.isLogged()) {
        MessageBoardSyn.getInstance().loginUser(user);
      }
    } else {
      MessageBoardSyn.getInstance().pingUser(user);
    }
    return user;
  }

  public static String getCertInfo(HttpServletRequest request) {
    String cert = null;
    X509Certificate[] certs = (X509Certificate[])request.getAttribute("javax.servlet.request.X509Certificate");
    if (certs != null) {
      for (int i = 0; i < certs.length; i++) {
        String n = certs[i].getSubjectDN().getName();
        if (n != null) {
          cert += n;
        }
      }
    }
    return cert;
  }

}

