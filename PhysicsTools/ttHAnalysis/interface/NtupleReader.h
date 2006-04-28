#ifndef InputService_NtupleReader_h
#define InputService_NtupleReader_h
#include "PhysicsTools/ttHAnalysis/interface/NtupleData.h"
#include <TChain.h>
#include <string>

namespace tth {
  class NtupleReader {
  public:
    NtupleReader( const std::string & );
    void readNext();

  private:
    TChain chain_;
    Long64_t current_;

  public:
    NtupleData data_;
  };
}

#endif
