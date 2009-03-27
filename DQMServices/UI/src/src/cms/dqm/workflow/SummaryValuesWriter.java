package cms.dqm.workflow;

import java.util.*;
import org.json.*;
import java.sql.*;

public class SummaryValuesWriter {

  public int write(String values) throws Exception {

    int rows = 0;
    DBWorker db = new DBWorker();

    JSONObject oRoot = new JSONObject(values);
    JSONObject oSubs = oRoot.getJSONObject("subs");

    int runNumber = oRoot.getInt("run");
    int runId = db.getFirstRowValueInt("select RUN_LAST_ID from RR_LAST_RUNS where RUN_NUMBER = ?", runNumber, 1);

    PreparedStatement insert = db.prepareSQL("insert into RR_SUMMARY_VALUES (SVA_RUN_ID, SVA_SUB_ABBR, SVA_FOLDER, SVA_NAME, SVA_VALUE) values (?, ?, ?, ?, ?)");
    PreparedStatement delete = db.prepareSQL("delete from RR_SUMMARY_VALUES where SVA_RUN_ID = ?");
    delete.setInt(1, runId);
    rows += delete.executeUpdate();
    
    Iterator iSub = oSubs.keys();
    while (iSub.hasNext()) {

      String subName = (String) iSub.next();
      JSONObject oSub = oSubs.getJSONObject(subName);

      Iterator iFolder = oSub.keys();
      while (iFolder.hasNext()) {

        String folderName = (String) iFolder.next();
        JSONObject oFolder = oSub.getJSONObject(folderName);

        Iterator iValue = oFolder.keys();
        while (iValue.hasNext()) {

          String valueName = (String) iValue.next();
          Double value = oFolder.getDouble(valueName);

          insert.setInt(1, runId);
          insert.setString(2, subName);
          insert.setString(3, folderName);
          insert.setString(4, valueName);
          insert.setDouble(5, value);
          rows += insert.executeUpdate();

        }

      }
    }

    
    return rows;

  }

}

