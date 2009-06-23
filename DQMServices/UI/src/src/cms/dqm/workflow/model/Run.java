package cms.dqm.workflow.model;

import org.hibernate.*;
import cms.dqm.workflow.HibernateUtil;
import java.io.Serializable;
import javax.persistence.*;
import java.util.*;

@Entity
@Table (name = "RR_RUNS")
public class Run implements Serializable {

  static final long serialVersionUID = 01L;

  @Transient
  private long rowNumber;
  public long getRowNumber() { return rowNumber; }
  public void setRowNumber(long rowNumber_) { rowNumber = rowNumber_; }

  @Transient
  private boolean selected = false;
  public boolean isSelected() { return selected; }
  public void setSelected(boolean selected_) { selected = selected_; }

  @Transient
  public String getGroupNumber() { return String.valueOf(number); }

  @Id
  @Column(name = "RUN_ID", nullable = false, unique = true)
  private long id;
  public long getId() { return id; }
  public void setId(long id_) { id = id_; }

  @Column(name = "RUN_NUMBER", nullable = false, unique = false)
  private long number;
  public long getNumber() { return number; }
  public void setNumber(long number_) { number = number_; }

  @Column(name = "RUN_GLOBALNAME")
  private String globalName;
  public String getGlobalName() { return globalName; }
  public void setGlobalName(String globalName_) { globalName = globalName_; }

  @Column(name = "RUN_ONLINE_SHIFTER")
  private String onlineShifter;
  public String getOnlineShifter() { return onlineShifter; }
  public void setOnlineShifter(String onlineShifter_) { onlineShifter = onlineShifter_; }

  @Column(name = "RUN_OFFLINE_SHIFTER")
  private String offlineShifter;
  public String getOfflineShifter() { return offlineShifter; }
  public void setOfflineShifter(String offlineShifter_) { offlineShifter = offlineShifter_; }

  @Column(name = "RUN_STATUS")
  private String status;
  public String getStatus() { return status; }
  public void setStatus(String status_) { status = status_; }

  @Column(name = "RUN_TAG")
  private String tag;
  public String getTag() { return tag; }
  public void setTag(String tag_) { tag = tag_; }

  @Column(name = "RUN_START_TIME")
  private Date started;
  public Date getStarted() { return started; }
  public void setStarted(Date started_) { started = started_; }

  @Column(name = "RUN_END_TIME")
  private Date stopped;
  public Date getStopped() { return stopped; }
  public void setStopped(Date stopped_) { stopped = stopped_; }

  @Column(name = "RUN_EVENTS")
  private long events;
  public long getEvents() { return events; }
  public void setEvents(long events_) { events = events_; }

  @Column(name = "RUN_RATE")
  private double rate;
  public double getRate() { return rate; }
  public void setRate(double rate_) { rate = rate_; }

  @Column(name = "RUN_BFIELD")
  private float bfield;
  public float getBfield() { return bfield; }
  public void setBfield(float bfield_) { bfield = bfield_; }

  @Column(name = "RUN_BFIELD_COMMENT")
  private String bfieldComment;
  public String getBfieldComment() { return bfieldComment; }
  public void setBfieldComment(String bfieldComment_) { bfieldComment = bfieldComment_; }

  @Column(name = "RUN_STOP_REASON")
  private String stopReason;
  public String getStopReason() { return stopReason; }
  public void setStopReason(String stopReason_) { stopReason = stopReason_; }

  @Column(name = "RUN_L1KEY")
  private String l1Key;
  public String getL1Key() { return l1Key; }
  public void setL1Key(String l1Key_) { l1Key = l1Key_; }

  @Column(name = "RUN_HLTKEY")
  private String hltKey;
  public String getHltKey() { return hltKey; }
  public void setHltKey(String hltKey_) { hltKey = hltKey_; }

  @Column(name = "RUN_ONLINE_COMMENT")
  private String onlineComment;
  public String getOnlineComment() { return onlineComment; }
  public void setOnlineComment(String onlineComment_) { onlineComment = onlineComment_; }

  @Column(name = "RUN_OFFLINE_COMMENT")
  private String offlineComment;
  public String getOfflineComment() { return offlineComment; }
  public void setOfflineComment(String offlineComment_) { offlineComment = offlineComment_; }

  @Column(name = "RUN_CREATE_TIME")
  private Date created;
  public Date getCreated() { return created; }
  public void setCreated(Date created_) { created = created_; }

/*
  @OneToMany
  @JoinColumn (name = "RSU_RUN_ID")
  Set<RunSubsystem> subsystems = new HashSet<RunSubsystem>();
  public Set<RunSubsystem> getSubsystems() { return subsystems; }
  public void setSubsystems(Set<RunSubsystem> subsystems_) { subsystems = subsystems_; }
*/	

/*
  @Column(name = "ON_CSC")
  private String subOnCSC;
  public String getSubOnCSC() { return subOnCSC; }
  public void   setSubOnCSC(String subOnCSC_) { subOnCSC = subOnCSC_; }

  @Column(name = "OFF_CSC")
  private String subOffCSC;
  public String getSubOffCSC() { return subOffCSC; }
  public void   setSubOffCSC(String subOffCSC_) { subOffCSC = subOffCSC_; }

  @Column(name = "ON_DT")
  private String subOnDT;
  public String getSubOnDT() { return subOnDT; }
  public void   setSubOnDT(String subOnDT_) { subOnDT = subOnDT_; }

  @Column(name = "OFF_DT")
  private String subOffDT;
  public String getSubOffDT() { return subOffDT; }
  public void   setSubOffDT(String subOffDT_) { subOffDT = subOffDT_; }

  @Column(name = "ON_ECAL")
  private String subOnECAL;
  public String getSubOnECAL() { return subOnECAL; }
  public void   setSubOnECAL(String subOnECAL_) { subOnECAL = subOnECAL_; }

  @Column(name = "OFF_ECAL")
  private String subOffECAL;
  public String getSubOffECAL() { return subOffECAL; }
  public void   setSubOffECAL(String subOffECAL_) { subOffECAL = subOffECAL_; }

  @Column(name = "ON_HCAL")
  private String subOnHCAL;
  public String getSubOnHCAL() { return subOnHCAL; }
  public void   setSubOnHCAL(String subOnHCAL_) { subOnHCAL = subOnHCAL_; }

  @Column(name = "OFF_HCAL")
  private String subOffHCAL;
  public String getSubOffHCAL() { return subOffHCAL; }
  public void   setSubOffHCAL(String subOffHCAL_) { subOffHCAL = subOffHCAL_; }

  @Column(name = "ON_PIX")
  private String subOnPIX;
  public String getSubOnPIX() { return subOnPIX; }
  public void   setSubOnPIX(String subOnPIX_) { subOnPIX = subOnPIX_; }

  @Column(name = "OFF_PIX")
  private String subOffPIX;
  public String getSubOffPIX() { return subOffPIX; }
  public void   setSubOffPIX(String subOffPIX_) { subOffPIX = subOffPIX_; }

  @Column(name = "ON_RPC")
  private String subOnRPC;
  public String getSubOnRPC() { return subOnRPC; }
  public void   setSubOnRPC(String subOnRPC_) { subOnRPC = subOnRPC_; }

  @Column(name = "OFF_RPC")
  private String subOffRPC;
  public String getSubOffRPC() { return subOffRPC; }
  public void   setSubOffRPC(String subOffRPC_) { subOffRPC = subOffRPC_; }

  @Column(name = "ON_SCAL")
  private String subOnSCAL;
  public String getSubOnSCAL() { return subOnSCAL; }
  public void   setSubOnSCAL(String subOnSCAL_) { subOnSCAL = subOnSCAL_; }

  @Column(name = "OFF_SCAL")
  private String subOffSCAL;
  public String getSubOffSCAL() { return subOffSCAL; }
  public void   setSubOffSCAL(String subOffSCAL_) { subOffSCAL = subOffSCAL_; }

  @Column(name = "ON_STRIP")
  private String subOnSTRIP;
  public String getSubOnSTRIP() { return subOnSTRIP; }
  public void   setSubOnSTRIP(String subOnSTRIP_) { subOnSTRIP = subOnSTRIP_; }

  @Column(name = "OFF_STRIP")
  private String subOffSTRIP;
  public String getSubOffSTRIP() { return subOffSTRIP; }
  public void   setSubOffSTRIP(String subOffSTRIP_) { subOffSTRIP = subOffSTRIP_; }

  @Column(name = "ON_TRG")
  private String subOnTRG;
  public String getSubOnTRG() { return subOnTRG; }
  public void   setSubOnTRG(String subOnTRG_) { subOnTRG = subOnTRG_; }

  @Column(name = "OFF_TRG")
  private String subOffTRG;
  public String getSubOffTRG() { return subOffTRG; }
  public void   setSubOffTRG(String subOffTRG_) { subOffTRG = subOffTRG_; }

  @Column(name = "OFF_EGAM")
  private String subOffEGAM;
  public String getSubOffEGAM() { return subOffEGAM; }
  public void   setSubOffEGAM(String subOffEGAM_) { subOffEGAM = subOffEGAM_; }

  @Column(name = "OFF_JMET")
  private String subOffJMET;
  public String getSubOffJMET() { return subOffJMET; }
  public void   setSubOffJMET(String subOffJMET_) { subOffJMET = subOffJMET_; }

  @Column(name = "OFF_MUON")
  private String subOffMUON;
  public String getSubOffMUON() { return subOffMUON; }
  public void   setSubOffMUON(String subOffMUON_) { subOffMUON = subOffMUON_; }
*/

}
