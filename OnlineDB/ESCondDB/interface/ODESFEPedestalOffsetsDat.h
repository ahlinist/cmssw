
#ifndef ODESFEPEDESTALOFFSETSDAT_H
#define ODESFEPEDESTALOFFSETSDAT_H

#include <vector>
#include <stdexcept>

#include <sstream>


#include "OnlineDB/EcalCondDB/interface/IDataItem.h"
#include "OnlineDB/EcalCondDB/interface/EcalLogicID.h"
#include "OnlineDB/ESCondDB/interface/ODESFEPedestalOffsetInfo.h"

class ODESFEPedestalOffsetsDat : public IODConfig {
 public:
  friend class EcalCondDBInterface;

  ODESFEPedestalOffsetsDat();

  ~ODESFEPedestalOffsetsDat();

  // User data methods
  inline std::string getTable() { return "ES_FE_CONF_DAT"; }

  inline void setId(int dac) { m_ID = dac; }
  inline int getId() const { return m_ID; }

  inline void setFedId(int fed) { m_fed_id = fed; }
  inline int getFedId() const { return m_fed_id; }

  inline void setFiberId(int fiber) { m_fiber_id = fiber; }
  inline int getFiberId() const { return m_fiber_id; }

  inline void setKchipId(int kch) { m_kchip_id = kch; }
  inline int getKchipId() const { return m_kchip_id; }

  inline void setPaceId(int pace) { m_pace_id = pace; }
  inline int getPaceId() const { return m_pace_id; }

  inline void setStripId(int strip) { m_strip_id = strip; }
  inline int getStripId() const { return m_strip_id; }

  inline void setRxId(int rx) { m_rx_id = rx; }
  inline int getRxId() const { return m_rx_id; }

  inline void setGain(int gain) { m_gain = gain; }
  inline int getGain() const { return m_gain; }

  inline void setPedestal(int pedestal) { m_pedestal = pedestal; }
  inline int getPedestal() const { return m_pedestal; }

  inline void setMasked(int mask) { m_masked = mask; }
  inline int getMasked() const { return m_masked; }
  
  inline void setZs(int zs) { m_zs = zs; }
  inline int getZs() const { return m_zs; }   

  inline void setCmMasked(int cmask) { m_cm_masked = cmask; }
  inline int getCmMasked() const { return m_cm_masked; }
  
  inline void setCmRange(int range) { m_cm_range = range; }
  inline int getCmRange() const { return m_cm_range; }

  inline void setRms(int rms) { m_rms = rms; }
  inline int getRms() const { return m_rms; }

 private:
  void prepareWrite() 
    throw(std::runtime_error);

  void writeDB(const ODESFEPedestalOffsetsDat* item, ODESFEPedestalOffsetInfo* iov )
    throw(std::runtime_error);

  void writeArrayDB(const std::vector< ODESFEPedestalOffsetsDat > data, ODESFEPedestalOffsetInfo* iov)
    throw(std::runtime_error);


  void fetchData(std::vector< ODESFEPedestalOffsetsDat >* fillMap, ODESFEPedestalOffsetInfo* iov)
     throw(std::runtime_error);

  // User data
  int m_ID;
  int m_fed_id;
  int m_fiber_id;
  int m_kchip_id;
  int m_pace_id;
  int m_strip_id;
  int m_rx_id;
  int m_gain;
  int m_pedestal;
  int m_masked;
  int m_zs;
  int m_cm_masked;
  int m_cm_range;
  int m_rms;
};

#endif
