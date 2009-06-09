package cms.dqm.workflow;

import javax.xml.transform.Templates;
import java.util.*;
import java.io.*;
import java.security.NoSuchAlgorithmException;
import java.security.MessageDigest;
import java.math.BigInteger;

public class CacheSyn {

  private static CacheSyn instance;
  private HashMap<String, Templates> templates;
  private HashMap<String, CacheItem> data;
  private int dataSyn;
  private static File cacheDir;
  private MessageDigest md;
  private static String cache_url_base;

  private CacheSyn() throws java.security.NoSuchAlgorithmException {
    templates  = new HashMap<String, Templates>();
    data = new HashMap<String, CacheItem>();
    md = MessageDigest.getInstance("MD5");
    cache_url_base = WebUtils.GetEnv("cache_url");
    dataSyn = 0;
  }

  static public CacheSyn getInstance() {
    if (null == instance) {
      try {
        instance = new CacheSyn();
      } catch (NoSuchAlgorithmException e) {
        e.printStackTrace(System.err);
      }
    }
    return instance;
  }

  static public void destroy() {
    instance = null;
  }

  /**
   * Template (translet) management
   */

  public boolean hasTemplate(String name) {
    return templates.containsKey(name);
  }

  public Templates getTemplate(String name) {
    return templates.get(name);
  }

  public void setTemplate(String name, Templates template) {
    templates.put(name, template);
  }

  /**
   * Data query cache management
   */

  private String getCacheId(String servlet_name, String query_string) {
    String s = servlet_name.concat("{" + (query_string == null ? "" : query_string) + "}"); 
    md.update(s.getBytes(), 0, s.length());
    return String.valueOf(dataSyn).concat("-").concat(new BigInteger(1, md.digest()).toString(16));
  }

  public String getCachedContent(String servlet_name, String query_string) {
    String id = getCacheId(servlet_name, query_string);
    try {

      DBWorker db = new DBWorker();
      int currSyn = db.getFirstRowValueInt(db.prepareSQL("select datasyn from rr_cache_syn"));
      db.close();

      if (dataSyn == currSyn) {
        if (data.containsKey(id)) {
          CacheItem item = data.get(id);
          item.incReads();
          return item.getUrl();
        }
      } else {
        data.clear();
        dataSyn = currSyn;
      }
    } catch (Exception e) { }
    return null;
  }

  public String setCachedContent(String servlet_name, String query_string, String content, String mime) throws Exception {
    String id = getCacheId(servlet_name, query_string);
    CacheItem item = new CacheItem(id, mime, servlet_name, query_string);

    File f = new File(cacheDir, item.getFileName());
    FileWriter fw = new FileWriter(f);
    fw.write(content);
    fw.close();

    data.put(id, item);
    return item.getUrl();
  }

  public int getDataSyn() { return dataSyn; }

  public static String getUrl(String filename) {
    return cache_url_base + filename;
  }  

  public CacheItem getCachedItem(int id) {
    return data.get(id);
  }

  public HashMap<String, CacheItem> getCachedItems() {
    return data;
  }

  public void setCacheDir(File dir) {
    cacheDir = dir;
  }

  public class CacheItem {
    
    private String id;
    private String file_name;
    private String mime;
    private String servlet_name;
    private String query_string;
    private int reads;

    CacheItem(String id_, String mime_, String servlet_name_, String query_string_) {
      id = id_;
      mime = mime_;
      servlet_name = servlet_name_;
      query_string = query_string_;
      if (mime.equalsIgnoreCase("text/html")) {
        file_name = id.concat(".html");
      } else 
      if (mime.equalsIgnoreCase("text/xml")) {
        file_name = id.concat(".xml");
      } else {
        file_name = id.concat(".txt");
      }
      reads = 1;
    }

    public String getServletName() { return servlet_name; }
    public String getQueryString() { return query_string; }
    public String getMime() { return mime; }
    public String getFileName() { return file_name; }    
    public String getUrl() { return CacheSyn.getUrl(file_name); }    
    public int getReads() { return reads; }    
    public void incReads() { reads++; }    

  }

}

