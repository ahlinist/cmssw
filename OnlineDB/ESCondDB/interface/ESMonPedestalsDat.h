// /CMSSW/OnlineDB/ESCondDB/interface/ESMonPedestalsDat.h

#ifndef ESMONPEDESTALSDAT_H
#define ESMONPEDESTALSDAT_H
#include <vector>
#include <stdexcept>

#include "OnlineDB/EcalCondDB/interface/IDataItem.h"
#include "OnlineDB/EcalCondDB/interface/RunIOV.h"
#include "OnlineDB/EcalCondDB/interface/EcalLogicID.h"
#include "OnlineDB/ESCondDB/interface/ESMonRunIOV.h"

class ESMonPedestalsDat : public IDataItem {
friend class ESCondDBInterface;
friend class EcalCondDBInterface;
 public:
  
  ESMonPedestalsDat();
  ~ESMonPedestalsDat();

  // User data methods
  inline std::string getTable() { return "ES_MON_PEDESTALS_DAT"; }

  inline void setPedMean(float mean) { m_pedMean = mean; }
  inline float getPedMean() const { return m_pedMean; }

  inline void setPedRMS(float rms) { m_pedRMS = rms; }
  inline float getPedRMS() const { return m_pedRMS; }

  inline void setTaskStatus(bool status) { m_taskStatus = status; }
  inline bool getTaskStatus() const { return m_taskStatus; }

 private:
  void prepareWrite() 
    throw(std::runtime_error);

  void writeDB(const EcalLogicID* ecid, const ESMonPedestalsDat* item, ESMonRunIOV* iov)
    throw(std::runtime_error);

  void writeArrayDB(const std::map< EcalLogicID, ESMonPedestalsDat>* data, ESMonRunIOV* iov)
  throw(std::runtime_error);

  void fetchData(std::map< EcalLogicID, ESMonPedestalsDat >* fillMap, ESMonRunIOV* iov)
     throw(std::runtime_error);

  // User data
  float m_pedMean;
  float m_pedRMS;
  bool  m_taskStatus;
};

#endif
