#ifndef ESMONRUNDAT_H
#define ESMONRUNDAT_H

#include <vector>
#include <stdexcept>

#include "OnlineDB/EcalCondDB/interface/IDataItem.h"
#include "OnlineDB/ESCondDB/interface/ESMonRunIOV.h"
#include "OnlineDB/EcalCondDB/interface/EcalLogicID.h"

class ESMonRunDat : public IDataItem {
 public:
  friend class EcalCondDBInterface;
  ESMonRunDat();
  ~ESMonRunDat();

  // User data methods
  inline std::string getTable() { return "ES_MON_RUN_DAT"; }

  inline void setNumEvents(int num) { m_numEvents = num; }
  inline int getNumEvents() const { return m_numEvents; }
  inline void setMonStatus(int num) { m_status = num; }
  inline int getMonStatus() const { return m_status; }

 private:
  void prepareWrite() 
    throw(std::runtime_error);

  void writeDB(const EcalLogicID* ecid, const ESMonRunDat* item, ESMonRunIOV* iov )
    throw(std::runtime_error);

  void fetchData(std::map< EcalLogicID, ESMonRunDat >* fillMap, ESMonRunIOV* iov)
     throw(std::runtime_error);

  // User data
  int m_numEvents;
  int m_status;
};

#endif
