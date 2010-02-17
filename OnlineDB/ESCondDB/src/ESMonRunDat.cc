#include <stdexcept>
#include <string>
#include "OnlineDB/Oracle/interface/Oracle.h"

#include "OnlineDB/ESCondDB/interface/ESMonRunDat.h"
#include "OnlineDB/EcalCondDB/interface/RunIOV.h"

using namespace std;
using namespace oracle::occi;

ESMonRunDat::ESMonRunDat()
{
  m_env = NULL;
  m_conn = NULL;
  m_writeStmt = NULL;
  m_numEvents = 0;
}

ESMonRunDat::~ESMonRunDat()
{
}

void ESMonRunDat::prepareWrite()
  throw(runtime_error)
{
  this->checkConnection();

  try {
    m_writeStmt = m_conn->createStatement();
    m_writeStmt->setSQL("INSERT INTO es_mon_run_dat (iov_id, logic_id, "
			"num_events, mon_status) "
			"VALUES (:iov_id, :logic_id, "
			":num_events, :mon_status)");
  } catch (SQLException &e) {
    throw(runtime_error("ESMonRunDat::prepareWrite():  "+e.getMessage()));
  }
}


void ESMonRunDat::writeDB(const EcalLogicID* ecid, const ESMonRunDat* item, ESMonRunIOV* iov)
  throw(runtime_error)
{
  this->checkConnection();
  this->checkPrepare();

  int iovID = iov->fetchID();
  if (!iovID) { throw(runtime_error("ESMonRunDat::writeDB:  IOV not in DB")); }

  int logicID = ecid->getLogicID();
  if (!logicID) { throw(runtime_error("ESMonRunDat::writeDB:  Bad EcalLogicID")); }
  
  try {
    m_writeStmt->setInt(1, iovID);
    m_writeStmt->setInt(2, logicID);
    m_writeStmt->setInt(3, item->getNumEvents());
    m_writeStmt->setInt(4, item->getMonStatus());

    m_writeStmt->executeUpdate();
  } catch (SQLException &e) {
    throw(runtime_error("RunDat::writeDB():  "+e.getMessage()));
  }
}


void ESMonRunDat::fetchData(map< EcalLogicID, ESMonRunDat >* fillMap, ESMonRunIOV* iov)
  throw(runtime_error)
{
  this->checkConnection();
  fillMap->clear();

  iov->setConnection(m_env, m_conn);
  int iovID = iov->fetchID();
  if (!iovID) { 
    //  throw(runtime_error("ESMonRunDat::writeDB:  IOV not in DB")); 
    return;
  }

  try {
    Statement* stmt = m_conn->createStatement();
    stmt->setSQL("SELECT cv.name, cv.logic_id, cv.id1, cv.id2, cv.id3, cv.maps_to, "
		 "d.num_events "
		 "FROM channelview cv JOIN es_mon_run_dat d "
		 "ON cv.logic_id = d.logic_id AND cv.name = cv.maps_to "
		 "WHERE d.iov_id = :iov_id");
    stmt->setInt(1, iovID);
    ResultSet* rset = stmt->executeQuery();
    
    std::pair< EcalLogicID, ESMonRunDat > p;
    ESMonRunDat dat;
    while(rset->next()) {
      p.first = EcalLogicID( rset->getString(1),     // name
			     rset->getInt(2),        // logic_id
			     rset->getInt(3),        // id1
			     rset->getInt(4),        // id2
			     rset->getInt(5),        // id3
			     rset->getString(6));    // maps_to

      dat.setNumEvents( rset->getInt(7) );

      p.second = dat;
      fillMap->insert(p);
    }
    m_conn->terminateStatement(stmt);
  } catch (SQLException &e) {
    throw(runtime_error("ESMonRunDat::fetchData():  "+e.getMessage()));
  }
}
