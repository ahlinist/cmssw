#ifndef ESTBRAWDATA_ESDCCHEADERBLOCK_H
#define ESTBRAWDATA_ESDCCHEADERBLOCK_H

#include <vector>

using namespace std;

class ESDCCHeaderBlock {

  public :

  typedef int key_type;

  enum ESDCCRuntype{
    COSMIC_TEST = 0,
    TESTBEAM = 1,
    PEDESTAL = 3,
    COMSIC_GLOBAL = 10,
    PHYSICS_GLOBAL = 11,
    COSMIC_LOCAL = 20,
    PHYSICS_LOCAL = 21
  };
  
  enum ESTriggerType{
    PHYSICS_TRIGGER = 1,
    CALIBRATION_TRIGGER = 2,
    TEST_TRIGGER = 3,
    TECHNICAL_EXTERNAL_TRIGGER = 4
  };
  
  ESDCCHeaderBlock();
  ESDCCHeaderBlock(const int& dccId);

  const int& id() const { return dccId_; }
  void setId(const int& dccId) { dccId_ = dccId; };

  const int fedId() const { return fedId_; }
  void setFedId(const int& fedId) { fedId_ = fedId; };

  void setLV1(const int& LV1) { LV1_ = LV1; };
  void setBX(const int& BX) { BX_ = BX; }
  void setEventLength(const int &evtLen) { evtLen_ = evtLen; };
  void setDCCErrors(const int& dccErrs) { dccErrs_ = dccErrs; };
  void setRunNumber(const int& runNum) { runNum_ = runNum; };
  void setRunType(const int& runType) { runType_ = runType; };
  void setTriggerType(const int& trgType) { trgType_ = trgType; };
  void setCompressionFlag(const int& compFlag) { compFlag_ = compFlag; };
  void setOrbitNumber(const int& orbit) {orbit_ = orbit; };
  void setMajorVersion(const int& vmajor) {vmajor_ = vmajor; };
  void setMinorVersion(const int& vminor) {vminor_ = vminor; };
  void setOptoRX0(const int& optoRX0) { optoRX0_ = optoRX0; };
  void setOptoRX1(const int& optoRX1) { optoRX1_ = optoRX1; };
  void setOptoRX2(const int& optoRX2) { optoRX2_ = optoRX2; };
  void setFEChannelStatus(const vector<int>& FEch) { FEch_ = FEch; };

  int getLV1() const { return LV1_; }
  int getBX() const { return BX_; }
  int getEventLength() const { return evtLen_; }
  int getDCCErrors() const { return dccErrs_; }
  int getRunNumber() const { return runNum_; }
  int getRunType() const { return runType_; }
  int getTriggerType() const { return trgType_; }
  int getCompressionFlag() const { return compFlag_; }
  int getOrbitNumber() const { return orbit_; }
  int getMajorVersion() const { return vmajor_; }
  int getMinorVersion() const { return vminor_; }
  int getOptoRX0() const { return optoRX0_; }
  int getOptoRX1() const { return optoRX1_; }
  int getOptoRX2() const { return optoRX2_; }
  vector<int> getFEChannelStatus() const { return FEch_; }

  private :

  int dccId_;
  int fedId_;
  int LV1_;
  int BX_;
  int evtLen_;
  int dccErrs_;
  int runNum_;
  int runType_;
  int trgType_;
  int compFlag_;
  int orbit_;
  int vmajor_;
  int vminor_;
  int optoRX0_;
  int optoRX1_;
  int optoRX2_;
  vector<int> FEch_;

};

#endif


