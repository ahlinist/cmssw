package cms.dqm.workflow.model;

import java.io.Serializable;
import javax.persistence.*;

@Entity
@Table (name = "RR_RUN_SUBSYSTEMS")
@IdClass(RunSubsystemPk.class)
public class RunSubsystem implements Serializable {

  static final long serialVersionUID = 02L;

  @ManyToOne
  @JoinColumn (name="RSU_RUN_ID", updatable = false, insertable = false)
  private Run run;
  public Run getRun() { return run; }
  public void setRun(Run run_) { run = run_; }

  @Id
  @Column(name = "RSU_RUN_ID")
  private long runId;
  public long getRunId() { return runId; }
  public void setRunId(long runId_) { runId = runId_; }

  @Id
  @Column(name = "RSU_SUB_ABBR")
  private String sub;
  public String getSub() { return sub; }
  public void setSub(String sub_) { sub = sub_; }

  @Id
  @Column(name = "RSU_SHIFT_TYPE")
  private String shiftType;
  public String getShiftType() { return shiftType; }
  public void setShiftType(String shiftType_) { shiftType = shiftType_; }

  @Column(name = "RSU_COMMENT")
  private String comment;
  public String getComment() { return comment; }
  public void setComment(String comment_) { comment = comment_; }

  @Column(name = "RSU_VALUE")
  private String value;
  public String getValue() { return value; }
  public void setValue(String value_) { value = value_; }

}
