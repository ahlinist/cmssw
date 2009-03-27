package cms.dqm.workflow;

import java.util.*;
import org.json.*;

public class SummaryValuesWriter {

  public String write(String values) throws Exception {

    String res = null;

    DBWorker db = new DBWorker();

    JSONObject root = new JSONObject(values);
    JSONObject subs = root.getJSONObject("subs");
    int runNumber = root.getInt("run");
    int runId = db.getFirstRowValueInt("select RUN_LAST_ID from RR_LAST_RUNS where RUN_NUMBER = ?", runNumber, 1);
    
    
    return values;

  }

}

