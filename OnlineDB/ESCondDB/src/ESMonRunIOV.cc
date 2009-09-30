#include <stdexcept>

#include "OnlineDB/Oracle/interface/Oracle.h"

#include "OnlineDB/ESCondDB/interface/ESMonRunIOV.h"

#include "OnlineDB/EcalCondDB/interface/RunIOV.h"
#include "OnlineDB/EcalCondDB/interface/Tm.h"
#include "OnlineDB/EcalCondDB/interface/DateHandler.h"

using namespace std;
using namespace oracle::occi;

ESMonRunIOV::ESMonRunIOV()
{
  m_conn = NULL;
  m_ID = 0;
  m_monRunTag = "";
  m_runIOV = RunIOV();
  m_subRunNum = 0;
  m_subRunStart = Tm();
  m_subRunEnd = Tm();
}

ESMonRunIOV::~ESMonRunIOV()
{

}

void ESMonRunIOV::setID(int id)
{
    m_ID = id;
}

void ESMonRunIOV::setMonRunTag(string tag)
{
  if (tag != m_monRunTag) {
    m_ID = 0;
    m_monRunTag = tag;
  }
}

string ESMonRunIOV::getMonRunTag() const
{
  return m_monRunTag;
}

void ESMonRunIOV::setRunIOV(RunIOV iov)
{
  if (iov != m_runIOV) {
    m_ID = 0;
    m_runIOV = iov;
  }
}

RunIOV ESMonRunIOV::getRunIOV()
{
  return m_runIOV;
}

void ESMonRunIOV::setSubRunNumber(subrun_t subrun)
{
  if (subrun != m_subRunNum) {
    m_ID = 0;
    m_subRunNum = subrun;
  }
}

run_t ESMonRunIOV::getSubRunNumber() const
{
  return m_subRunNum;
}

void ESMonRunIOV::setSubRunStart(Tm start)
{
  if (start != m_subRunStart) {
    m_ID = 0;
    m_subRunStart = start;
  }
}

Tm ESMonRunIOV::getSubRunStart() const
{
  return m_subRunStart;
}

void ESMonRunIOV::setSubRunEnd(Tm end)
{
  if (end != m_subRunEnd) {
    m_ID = 0;
    m_subRunEnd = end;
  }
}

Tm ESMonRunIOV::getSubRunEnd() const
{
  return m_subRunEnd;
}

int ESMonRunIOV::fetchID()
  throw(runtime_error)
{
  // Return from memory if available
  if (m_ID) {
    return m_ID;
  }
  this->checkConnection();
  // fetch the parent IDs
  int  runIOVID=0;
  
  cout << " IOV= " << m_ID << endl;
  this->fetchParentIDs(&runIOVID);
   cout << " runIOV= " << runIOVID << endl;
  if (!runIOVID) {
    return 0;
  }
  DateHandler dh(m_env, m_conn);
  if (m_subRunEnd.isNull()) {
    m_subRunEnd = dh.getPlusInfTm();
  }
  try {
  	cout << "write select query" << endl;
    Statement* stmt = m_conn->createStatement();
    stmt->setSQL("SELECT iov_id FROM es_mon_run_iov "
		 "WHERE tag = :1 AND "
		 "run_iov_id   = :2 AND "
		 "subrun_num   = :3 AND "
		 "subrun_start = :4 AND "
		 "subrun_end   = :5");
    stmt->setString(1, m_monRunTag);
    stmt->setInt(2, runIOVID);
    stmt->setInt(3, m_subRunNum);
    stmt->setDate(4, dh.tmToDate(m_subRunStart));
    stmt->setDate(5, dh.tmToDate(m_subRunEnd));
    ResultSet* rset = stmt->executeQuery();
    if (rset->next()) {
      m_ID = rset->getInt(1);
    } else {
      m_ID = 0;
    }
    m_conn->terminateStatement(stmt);
  } catch (SQLException &e) {
    throw(runtime_error("ESMonRunIOV::fetchID:  "+e.getMessage()));
  }
  return m_ID;
}

void ESMonRunIOV::setByID(int id)
  throw(std::runtime_error)
{
   this->checkConnection();
   DateHandler dh(m_env, m_conn);
   try {
     Statement* stmt = m_conn->createStatement();
     stmt->setSQL("SELECT tag, run_iov_id, subrun_num, subrun_start, subrun_end FROM es_mon_run_iov WHERE iov_id = :1");
     stmt->setInt(1, id);
     ResultSet* rset = stmt->executeQuery();
     if (rset->next()) {
       string monRunTag = rset->getString(1);
       int runIOVID = rset->getInt(2);
       m_subRunNum = rset->getInt(3);
       Date startDate = rset->getDate(4);
       Date endDate = rset->getDate(5);
       m_subRunStart = dh.dateToTm( startDate );
       m_subRunEnd = dh.dateToTm( endDate );
  //     m_monRunTag.setConnection(m_env, m_conn);
 //    m_monRunTag.setByID(monRunTagID);
       m_runIOV.setConnection(m_env, m_conn);
       m_runIOV.setByID(runIOVID);
       m_ID = id;
     } else {
       throw(runtime_error("ESMonRunIOV::setByID:  Given tag_id is not in the database"));
     }
     m_conn->terminateStatement(stmt);
   } catch (SQLException &e) {
     throw(runtime_error("ESMonRunIOV::setByID:  "+e.getMessage()));
   }
}

int ESMonRunIOV::writeDB()
  throw(runtime_error)
{
  
  this->checkConnection();
  // Check if this IOV has already been written
  if (this->fetchID()) {
    return m_ID;
  }
  // fetch Parent IDs
  int  runIOVID;
  this->fetchParentIDs(&runIOVID);
  
  //if (!monRunTagID) {
  //  monRunTagID = m_monRunTag.writeDB();
  //}
  // Validate the data, use infinity-till convention
  DateHandler dh(m_env, m_conn);
  if (m_subRunStart.isNull()) {
    throw(runtime_error("ESMonRunIOV::writeDB:  Must setSubRunStart before writing"));
  }
  if (m_subRunEnd.isNull()) {
    m_subRunEnd = dh.getPlusInfTm();
  }
  try {
    Statement* stmt = m_conn->createStatement();
    stmt->setSQL("INSERT INTO es_mon_run_iov (iov_id, tag, run_iov_id, subrun_num, subrun_start, subrun_end) "
		 "VALUES (es_mon_run_iov_sq.NextVal, :1, :2, :3, :4, :5)");
    stmt->setString(1, m_monRunTag );
    stmt->setInt(2, runIOVID);
    stmt->setInt(3, m_subRunNum);
    stmt->setDate(4, dh.tmToDate(m_subRunStart));
    stmt->setDate(5, dh.tmToDate(m_subRunEnd));
    stmt->executeUpdate();
    m_conn->terminateStatement(stmt);
  } catch (SQLException &e) {
    throw(runtime_error("ESMonRunIOV::writeDB:  "+e.getMessage()));
  }

  // Now get the ID

  if (!this->fetchID()) {
    throw(runtime_error("ESMonRunIOV::writeDB:  Failed to write"));
  }
  return m_ID;
}


void ESMonRunIOV::fetchParentIDs( int* runIOVID)
  throw(runtime_error)
{

  // get the RunIOV
  m_runIOV.setConnection(m_env, m_conn);
  *runIOVID = m_runIOV.fetchID();
  cout << "run_iov=" << *runIOVID << endl;
  if (! *runIOVID) {
    throw(runtime_error("ESMonRunIOV:  Given RunIOV does not exist in DB"));
  }
}

void ESMonRunIOV::setByRun(string montag, RunIOV* runiov, subrun_t subrun)
  throw(std::runtime_error)
{
  this->checkConnection();
  runiov->setConnection(m_env, m_conn);
  int runIOVID = runiov->fetchID();
  if (!runIOVID) {
    throw(runtime_error("ESMonRunIOV::setByRun:  Given RunIOV does not exist in DB"));
  }
 
  DateHandler dh(m_env, m_conn);
  try {
    Statement* stmt = m_conn->createStatement();
    stmt->setSQL("SELECT iov_id, subrun_start, subrun_end FROM es_mon_run_iov "
		 "WHERE tag = :1 AND run_iov_id = :2 AND subrun_num = :3");
    stmt->setString(1, montag);
    stmt->setInt(2, runIOVID);
    stmt->setInt(3, subrun);
    ResultSet* rset = stmt->executeQuery();
    if (rset->next()) {
      m_monRunTag = montag;
      m_runIOV = *runiov;
      m_subRunNum = subrun;
      m_ID = rset->getInt(1);
      Date startDate = rset->getDate(2);
      Date endDate = rset->getDate(3);
      m_subRunStart = dh.dateToTm( startDate );
      m_subRunEnd = dh.dateToTm( endDate );
    } else {
      throw(runtime_error("ESMonRunIOV::setByRun:  Given subrun is not in the database"));
    }
    m_conn->terminateStatement(stmt);
  } catch (SQLException &e) {
    throw(runtime_error("ESMonRunIOV::setByRun:  "+e.getMessage()));
  }
}
