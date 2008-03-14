#ifndef ODLTSCYCLE_H
#define ODLTSCYCLE_H

#include <stdexcept>
#include <iostream>

#include "OnlineDB/EcalCondDB/interface/IODConfig.h"



class ODLTSCycle :  public IODConfig  {
 public:
  friend class EcalCondDBInterface;
  friend class ODEcalCycle;

  ODLTSCycle();
  ~ODLTSCycle();

  inline std::string getTable() { return "ECAL_LTS_CYCLE"; }

  inline void setId(int id){m_ID=id;}
  inline int getId()const{ return m_ID;} ;

  // Methods for user data
  inline void setLTSConfigurationID(int x){m_lts_config_id=x;}
  inline int getLTSConfigurationID() const{return m_lts_config_id;}

  // Operators
  inline bool operator==(const ODLTSCycle &m) const { return ( m_ID   == m.m_ID); }
  inline bool operator!=(const ODLTSCycle &m) const { return !(*this == m); }

 private:
  // User data 
  int m_ID;
  int  m_lts_config_id;
  void writeDB() throw(std::runtime_error);
  void prepareWrite()  throw(std::runtime_error);
  void clear();
  void fetchData(ODLTSCycle * result)     throw(std::runtime_error);
   void insertConfig() throw(std::runtime_error);


  // Methods from IUniqueDBObject
  int fetchID() throw(std::runtime_error);
  void setByID(int id) throw(std::runtime_error);


};

#endif
