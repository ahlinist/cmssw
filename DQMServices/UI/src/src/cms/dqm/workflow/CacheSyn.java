package cms.dqm.workflow;

import javax.xml.transform.Templates;
import java.util.HashMap;
import java.io.*;
import java.util.SortedMap;
import java.util.TreeMap;

public class CacheSyn {

  private static CacheSyn instance;
  private HashMap<String, Templates> templates;
  private HashMap<Integer, CacheItem> data;
  private int dataSyn;

  private CacheSyn() {
    templates  = new HashMap<String, Templates>();
    data = new HashMap<Integer, CacheItem>();
    dataSyn = 0;
  }

  static public CacheSyn getInstance() {
    if (null == instance) {
      instance = new CacheSyn();
    }
    return instance;
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

  private int getCacheId(String servlet_name, String query_string) {
    return servlet_name.concat("{" + (query_string == null ? "" : query_string) + "}").hashCode();
  }

  public String getCachedContent(String servlet_name, String query_string) {
    int id = getCacheId(servlet_name, query_string);
    String ret = null;
    try {
      DBWorker db = new DBWorker();
      int currSyn = db.getFirstRowValueInt(db.prepareSQL("select datasyn from rr_cache_syn"));
      db.close();
      if (dataSyn == currSyn) {
        if (data.containsKey(id)) ret = String.valueOf(id);
      } else {
        data.clear();
        dataSyn = currSyn;
      }
    } catch (Exception e) { }
    return ret;
  }

  public String setCachedContent(String servlet_name, String query_string, String content, String mime) throws Exception {
    int id = getCacheId(servlet_name, query_string);
    File f = File.createTempFile("runregistry_cache", servlet_name, DataProvider.tempDir);
    FileWriter fw = new FileWriter(f);
    fw.write(content);
    fw.close();
    data.put(id, new CacheItem(f, mime, servlet_name, query_string));
    return String.valueOf(id);
  }

  public int getDataSyn() { return dataSyn; }

  public CacheItem getCachedItem(int id) {
    return data.get(id);
  }

  public HashMap<Integer, CacheItem> getCachedItems() {
    return data;
  }

  public class CacheItem {
    
    private File file;
    private int reads;
    private String mime;
    private String servlet_name;
    private String query_string;

    CacheItem(File file_, String mime_, String servlet_name_, String query_string_) {
      file = file_;
      mime = mime_;
      servlet_name = servlet_name_;
      query_string = query_string_;
      reads = 0;
    }

    public void incReads() { this.reads += 1; }

    public String getServletName() { return servlet_name; }
    public String getQueryString() { return query_string; }
    public int getReads() { return reads; }
    public String getMime() { return mime; }
    public File getFile() { return file; }    

  }

}

