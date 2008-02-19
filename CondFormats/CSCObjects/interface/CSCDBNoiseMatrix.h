#ifndef CSCDBNoiseMatrix_h
#define CSCDBNoiseMatrix_h

#include "DataFormats/MuonDetId/interface/CSCDetId.h"

class CSCDBNoiseMatrix{
 public:
  CSCDBNoiseMatrix();
  ~CSCDBNoiseMatrix();
  
  struct Item{
    short int elem33,elem34,elem35,elem44,elem45,elem46,
              elem55,elem56,elem57,elem66,elem67,elem77;
  };
  enum size{ArraySize=217728};

  const Item & item(const CSCDetId & cscId, int strip) const;
  
  typedef Item NoiseMatrixContainer;
  NoiseMatrixContainer matrix[ArraySize];
};

#endif
