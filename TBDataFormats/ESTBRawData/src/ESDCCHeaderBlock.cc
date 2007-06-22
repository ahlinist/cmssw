#include "TBDataFormats/ESTBRawData/interface/ESDCCHeaderBlock.h"

ESDCCHeaderBlock::ESDCCHeaderBlock()
{

  dccId_ = -1;
  fedId_ = -1;
  LV1_ = -1;
  BX_ = -1;
  evtLen_ = -1;
  dccErrs_ = -1;
  runNum_ = -1;
  runType_ = -1;
  trgType_ = -1;
  compFlag_ = -1;
  orbit_ = -1;
  vmajor_ = -1;
  vminor_ = -1;
  optoRX0_ = -1;
  optoRX1_ = -1;
  optoRX2_ = -1;
  FEch_.reserve(36);

}

ESDCCHeaderBlock::ESDCCHeaderBlock(const int& dccId)
{

  dccId_ = dccId;
  fedId_ = -1;
  LV1_ = -1;
  BX_ = -1;
  evtLen_ = -1;
  dccErrs_ = -1;
  runNum_ = -1;
  runType_ = -1;
  trgType_ = -1;
  compFlag_ = -1;
  orbit_ = -1;
  vmajor_ = -1;
  vminor_ = -1;
  optoRX0_ = -1;
  optoRX1_ = -1;
  optoRX2_ = -1;
  FEch_.reserve(36);

}
