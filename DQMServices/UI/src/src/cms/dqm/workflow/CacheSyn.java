package cms.dqm.workflow;

import javax.xml.transform.Templates;
import java.util.HashMap;

public class CacheSyn {

  private static CacheSyn instance;
  private HashMap<String, Templates> templates;

  private CacheSyn() {
    templates  = new HashMap<String, Templates>();
  }

  static public CacheSyn getInstance() {
    if (null == instance) {
      instance = new CacheSyn();
    }
    return instance;
  }

  public boolean hasTemplate(String name) {
    return templates.containsKey(name);
  }

  public Templates getTemplate(String name) {
    return templates.get(name);
  }

  public void setTemplate(String name, Templates template) {
    templates.put(name, template);
  }

}

