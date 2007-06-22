#ifndef ESTBRAWDATA_ESKCHIPBLOCK_H
#define ESTBRAWDATA_ESKCHIPBLOCK_H

#include <vector>

using namespace std;

class ESKCHIPBlock {
  
  public :
    
  typedef int key_type; // For the sorted collection 

  ESKCHIPBlock();
  ESKCHIPBlock(const int& kId);

  const int& id() const { return kId_; }
  void setId(const int& kId) { kId_ = kId; };

  const int dccdId() const { return dccId_; }
  void setDccId(const int& dccId) { dccId_ = dccId; };

  const int fedId() const { return fedId_; }
  void setFedId(const int& fedId) { fedId_ = fedId; };

  void setBC(const int& BC) { BC_ = BC; }
  void setEC(const int& EC) { EC_ = EC; }
  void setFlag1(const int& flag1) { flag1_ = flag1; };
  void setFlag2(const int& flag2) { flag2_ = flag2; };
  void setCRC(const int& CRC) { CRC_ = CRC; }

  int getBC() const { return BC_; }
  int getEC() const { return EC_; }
  int getFlag1() const { return flag1_; }
  int getFlag2() const { return flag2_; }
  int getCRC() const { return CRC_; }

  private :

  int kId_;
  int dccId_;
  int fedId_;
  int BC_;
  int EC_;
  int flag1_;
  int flag2_;
  int CRC_;

};

#endif


