package cms.dqm.workflow.model;

import java.io.Serializable;
import javax.persistence.*;

@Embeddable
public class RunSubsystemPk implements Serializable {

  static final long serialVersionUID = 021L;

  private long runId;
  public long getRunId() { return runId; }
  public void setRunId(long runId_) { runId = runId_; }

  private String sub;
  public String getSub() { return sub; }
  public void setSub(String sub_) { sub = sub_; }

  private String shiftType;
  public String getShiftType() { return shiftType; }
  public void setShiftType(String shiftType_) { shiftType = shiftType_; }

  public boolean equals(RunSubsystemPk pk) {
    return (getRunId() == pk.getRunId() && getSub().compareTo(pk.getSub()) == 0 && getShiftType().compareTo(pk.getShiftType()) == 0); 
  }

}
