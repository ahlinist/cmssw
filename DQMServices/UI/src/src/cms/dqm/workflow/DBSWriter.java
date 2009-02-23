package cms.dqm.workflow;

import org.apache.xmlrpc.client.XmlRpcClient;
import org.apache.xmlrpc.client.XmlRpcClientConfigImpl;
import java.net.URL;
import java.util.*;

public class DBSWriter {

  XmlRpcClientConfigImpl config;
  XmlRpcClient client;  

  DBSWriter(String url) throws Exception {

    config = new XmlRpcClientConfigImpl();
    config.setServerURL(new URL(url));
    client = new XmlRpcClient();
    client.setConfig(config);
  }

  public String write(int run, Hashtable<String, String> tags) throws Exception {
    Object[] params = new Object[]{new Integer(run), tags};
    String result = (String) client.execute("insertdq", params);
    return result;	
  }

  public String write(int run, String tag, String value) throws Exception {
    Object[] params = new Object[]{new Integer(run), tag, value};
    String result = (String) client.execute("insertdq", params);
    return result;	
  }

  public String query(int run, String tag) throws Exception {
    Object[] params = new Object[]{new Integer(run), tag};
    String result = (String) client.execute("querydq", params);
    return result;
  }

}

