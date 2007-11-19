#ifndef FECONFIGLUT_H
#define FECONFIGLUT_H

#include <stdexcept>
#include <iostream>

#include "OnlineDB/EcalCondDB/interface/IIOV.h"
#include "OnlineDB/EcalCondDB/interface/Tm.h"





typedef int run_t;

class FEConfigLUTInfo : public IIOV {
 public:
  friend class EcalCondDBInterface;

  FEConfigLUTInfo();
  ~FEConfigLUTInfo();

  // Methods for user data
  Tm getDBTime() const;
  void setIOVId(int iov_id);
  int getIOVId() const;
  void setID(int id) ; 
  int getID() ;
  void setTag(std::string x);
  std::string getTag() const;

  // Methods from IUniqueDBObject
  int fetchID() throw(std::runtime_error);
  int fetchIDFromTag() throw(std::runtime_error);
  int fetchIDLast() throw(std::runtime_error);
  void setByID(int id) throw(std::runtime_error);

  // operators
  inline bool operator==(const FEConfigLUTInfo &r) const {  return (m_ID   == r.m_ID ); }
  inline bool operator!=(const FEConfigLUTInfo &r) const { return !(*this == r); }

 private:
  // User data for this IOV
  int m_iov_id ;
  Tm m_db_time;
  std::string m_tag;

  int writeDB() throw(std::runtime_error);

};



#endif
