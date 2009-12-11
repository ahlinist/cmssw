#include <stdexcept>
#include <cstdlib>
#include <string>

#include <string.h>

#include "OnlineDB/Oracle/interface/Oracle.h"

#include "OnlineDB/EcalCondDB/interface/DateHandler.h"
#include "OnlineDB/ESCondDB/interface/ODESFEPedestalOffsetInfo.h"

using namespace std;
using namespace oracle::occi;


ODESFEPedestalOffsetInfo::ODESFEPedestalOffsetInfo()
{
  m_env = NULL;
  m_conn = NULL;
  m_writeStmt = NULL;
  m_readStmt = NULL;
  m_ID=0;
  m_config_tag="";
  m_version=0;
  m_iov_id_pl=0;
  m_iov_id_mi=0;
  m_user_comment="";
  clear();   
}

void ODESFEPedestalOffsetInfo::clear(){

}

ODESFEPedestalOffsetInfo::~ODESFEPedestalOffsetInfo()
{
}

int ODESFEPedestalOffsetInfo::fetchNextId()  throw(std::runtime_error) {

  int result=0;
  try {
    this->checkConnection();

    m_readStmt = m_conn->createStatement(); 
    m_readStmt->setSQL("select es_fe_conf_sq.NextVal from DUAL ");
    ResultSet* rset = m_readStmt->executeQuery();
    while (rset->next ()){
      result= rset->getInt(1);
    }
    result++;
    m_conn->terminateStatement(m_readStmt);
    return result; 

  } catch (SQLException &e) {
    throw(runtime_error("ODESFEPedestalOffsetInfo::fetchNextId():  "+e.getMessage()));
  }

}

void ODESFEPedestalOffsetInfo::prepareWrite()
  throw(runtime_error)
{
  this->checkConnection();

  int next_id=0;
  if(getId()==0){
    next_id=fetchNextId();
  }

  try {
    m_writeStmt = m_conn->createStatement();
    m_writeStmt->setSQL("INSERT INTO "+getTable()+
      " ( rec_id, tag, version,iov_id_es_pl,iov_id_es_mi,user_comment) " 
	  " VALUES ( :1, :2, :3 ,:4, :5, :6) " );
    m_writeStmt->setInt(1, next_id);
    m_ID=next_id;

  } catch (SQLException &e) {
    throw(runtime_error("ODESFEPedestalOffsetInfo::prepareWrite():  "+e.getMessage()));
  }

}

void ODESFEPedestalOffsetInfo::setParameters(std::map<string,string> my_keys_map){
  
  // parses the result of the XML parser that is a map of 
  // string string with variable name variable value 
  
  for( std::map<std::string, std::string >::iterator ci=
	 my_keys_map.begin(); ci!=my_keys_map.end(); ci++ ) {
    
    if(ci->first==  "VERSION") setVersion(atoi(ci->second.c_str()) );
    if(ci->first==  "TAG") setConfigTag(ci->second); 
  }
}

void ODESFEPedestalOffsetInfo::writeDB()
  throw(runtime_error)
{
  this->checkConnection();
  this->checkPrepare();

  try {

    // number 1 is the id 
    m_writeStmt->setString(2, this->getConfigTag());
    m_writeStmt->setInt(3, this->getVersion());
    m_writeStmt->setInt(4, this->getIov_pl());
    m_writeStmt->setInt(5, this->getIov_mi());
    m_writeStmt->setString(6, this->getUser_comment());

    m_writeStmt->executeUpdate();
  } catch (SQLException &e) {
    throw(runtime_error("ODESFEPedestalOffsetInfo::writeDB():  "+e.getMessage()));
  }


  // Now get the ID
  if (!this->fetchID()) {
    throw(runtime_error("ODESFEPedestalOffsetInfo::writeDB:  Failed to write"));
  } else {
    int old_version=this->getVersion();
    m_readStmt = m_conn->createStatement(); 
    this->fetchData (this);
    m_conn->terminateStatement(m_readStmt);
    if(this->getVersion()!=old_version) std::cout << "ODESFEPedestalOffsetInfo>>WARNING version is "<< getVersion()<< endl; 
  }
}


void ODESFEPedestalOffsetInfo::fetchData(ODESFEPedestalOffsetInfo * result)
  throw(runtime_error)
{
  this->checkConnection();
    
  DateHandler dh(m_env, m_conn);

  result->clear();
  if(result->getId()==0 && (result->getConfigTag()=="") ){
    throw(runtime_error("ODESFEPedestalOffsetInfo::fetchData(): no Id defined for this ODESFEPedestalOffsetInfo "));
  }

  try {
    if(result->getId()!=0) { 
      m_readStmt->setSQL("SELECT * FROM " + getTable() +   
			 " where  rec_id = :1 ");
      m_readStmt->setInt(1, result->getId());
    } else if (result->getConfigTag()!="") {
      m_readStmt->setSQL("SELECT * FROM " + getTable() +   
			 " where  tag=:1 AND version=:2 " );
      m_readStmt->setString(1, result->getConfigTag());
      m_readStmt->setInt(2, result->getVersion());
    } else {
      // we should never pass here 
      throw(runtime_error("ODESFEPedestalOffsetInfo::fetchData(): no Id defined for this record "));
    }

    ResultSet* rset = m_readStmt->executeQuery();

    rset->next();

    // 1 is the id and 2 is the config tag and 3 is the version

    result->setId(rset->getInt(1));
    result->setConfigTag(rset->getString(2));
    result->setVersion(rset->getInt(3));
    result->setIov_pl(rset->getInt(4));
    result->setIov_mi(rset->getInt(5));
    result->setUser_comment(rset->getString(6));

    
  } catch (SQLException &e) {
    throw(runtime_error("ODESFEPedestalOffsetInfo::fetchData():  "+e.getMessage()));
  }
}

void ODESFEPedestalOffsetInfo::fetchLastData(ODESFEPedestalOffsetInfo * result)
  throw(runtime_error)
{
  this->checkConnection();
 
  DateHandler dh(m_env, m_conn);

  result->clear();
  try {

    m_readStmt->setSQL("SELECT * FROM " + getTable() +   
                       " where  rec_id = ( select max(rec_id) from "+ getTable() +" ) " );
    ResultSet* rset = m_readStmt->executeQuery();

    rset->next();

    result->setId(rset->getInt(1));
    result->setConfigTag(rset->getString(2));
    result->setVersion(rset->getInt(3));
    result->setIov_pl(rset->getInt(4));
    result->setIov_mi(rset->getInt(5));
    result->setUser_comment(rset->getString(6));
    
  } catch (SQLException &e) {
    throw(runtime_error("ODESFEPedestalOffsetInfo::fetchData():  "+e.getMessage()));
  }
}


int ODESFEPedestalOffsetInfo::fetchID()    throw(std::runtime_error)
{
  // Return from memory if available
  if (m_ID!=0) {
    return m_ID;
  }

  this->checkConnection();

  try {
    Statement* stmt = m_conn->createStatement();
    stmt->setSQL("SELECT rec_id FROM "+ getTable()+
                 " WHERE  tag=:1 and version=:2 " );

    stmt->setString(1, getConfigTag() );
    stmt->setInt(2, getVersion() );

    ResultSet* rset = stmt->executeQuery();

    if (rset->next()) {
      m_ID = rset->getInt(1);
    } else {
      m_ID = 0;
    }
    m_conn->terminateStatement(stmt);
  } catch (SQLException &e) {
    throw(runtime_error("ODESFEPedestalOffsetInfo::fetchID:  "+e.getMessage()));
  }

  return m_ID;
}

  
 vector<int>  ODESFEPedestalOffsetInfo::fetchRunPlus(const int recid)    throw(std::runtime_error)
{
   vector<int> refrun ;
 
  this->checkConnection();
  cout << "REC_ID= " << recid << endl;
  cout << "Query=" << "SELECT iov_id_es_pl,iov_id_es_pl,iov_id_es_mi FROM "+ getTable()+" WHERE  rec_id=:1 " << endl;
  try {
    Statement* stmt = m_conn->createStatement();
    stmt->setSQL("SELECT iov_id_es_pl,iov_id_es_pl,iov_id_es_mi FROM "+ getTable()+
                 " WHERE  rec_id=:1 " );

    stmt->setInt(1,recid);
    ResultSet* rset = stmt->executeQuery();
    while (rset->next()) {
      refrun.push_back(rset->getInt(1));
      refrun.push_back(rset->getInt(2));
      refrun.push_back(rset->getInt(3));
    }

    m_conn->terminateStatement(stmt);
  } catch (SQLException &e) {
    throw(runtime_error("ODESFEPedestalOffsetInfo::fetchRunPlus:  "+e.getMessage()));
  }
 return  refrun ;
}
