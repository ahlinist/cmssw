#ifndef ODESFEPEDESTALINFO_H
#define ODESFEPEDESTALINFO_H

#include <map>
#include <string>

#include "OnlineDB/EcalCondDB/interface/Tm.h"
#include "OnlineDB/EcalCondDB/interface/DateHandler.h"
#include "OnlineDB/EcalCondDB/interface/IDataItem.h"
#include "OnlineDB/EcalCondDB/interface/EcalLogicID.h"
#include "OnlineDB/EcalCondDB/interface/IODConfig.h"


class ODESFEPedestalOffsetInfo : public IODConfig {
 public:
  friend class EcalCondDBInterface;

  ODESFEPedestalOffsetInfo();
  ~ODESFEPedestalOffsetInfo();


  // User data methods
  inline std::string getTable() { return "ES_FE_CONF_INFO"; }

  inline void setId(int id) { m_ID = id; }
  inline int getId() const { return m_ID; }

  // the tag is already in IODConfig 

  inline void setVersion(int id) { m_version = id; }
  inline int getVersion() const { return m_version; }
  
  inline void setIov_pl(int iov) { m_iov_id_pl = iov; }
  inline int getIov_pl() const { return m_iov_id_pl; }
  
  inline void setIov_mi(int iov) { m_iov_id_mi = iov; }
  inline int getIov_mi() const { return m_iov_id_mi; }
  
  inline void setUser_comment(string comment) { m_user_comment = comment; }
  inline string getUser_comment() const { return m_user_comment; }
  
  int fetchNextId() throw(std::runtime_error);
  void setParameters(std::map<string,string> my_keys_map);
  int fetchID()  throw(std::runtime_error);

  vector<int>  fetchRunPlus(const int recid)  throw(std::runtime_error);
  std::map< int,int > fetchGain(const string gain)  throw(std::runtime_error);
 
private:
  void prepareWrite()  throw(std::runtime_error);
  void fetchLastData(ODESFEPedestalOffsetInfo * result)     throw(std::runtime_error);
  void writeDB()       throw(std::runtime_error);

  void clear();

  void fetchData(ODESFEPedestalOffsetInfo * result)     throw(std::runtime_error);
 

  // User data
  int m_ID;
  string m_Tag;
  int m_version;
  int m_iov_id_pl;
  int m_iov_id_mi;
  string m_user_comment;
  
};

#endif
