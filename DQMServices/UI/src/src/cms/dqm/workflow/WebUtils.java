package cms.dqm.workflow;

import java.util.*;
import java.util.regex.*;
import javax.naming.*;
import java.security.*;
import java.math.*;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;

public class WebUtils {

  private static Properties osenv = new Properties();
  private static Map<String, String> auths = new HashMap<String, String>();
  private static String mediaurl;

  static {
    try {
      osenv.load(Runtime.getRuntime().exec("env").getInputStream());
    } catch (java.io.IOException e) { }
    mediaurl = WebUtils.GetEnv("media_url");
    if (mediaurl == null) mediaurl = "media/";
  }

  public static String GetEnv(String name) {
    try {
      Context initCtx = new InitialContext();
      return (String)initCtx.lookup("java:comp/env/" + name);
    } catch(Exception e) { }
    return null;
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

  public static String getMediaURL() { return mediaurl; }

  public static String getPassword(String username) throws Exception {
    if (auths.containsKey(username)) return auths.get(username);
    String home = (String) osenv.get("HOME");
    if (home == null) {
      home = "/nfshome0/" + (String) osenv.get("USER");
    }
    return WebUtils.getPassword(username, home + "/" + WebUtils.GetEnv("db_auth_file"));
  }

  public static String getPassword(String username, String auth_file) throws Exception {

    if (auths.containsKey(username)) return auths.get(username);
    if (auth_file == null) return null;

    String password = null;

    FileInputStream in;
    in = new FileInputStream(auth_file);
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

    return password;
  }

}
