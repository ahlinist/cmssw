#ifndef TauAnalysis_Core_BinningService_h  
#define TauAnalysis_Core_BinningService_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include "TauAnalysis/Core/interface/BinningServiceBase.h"

#include <string>
#include <vector>

template<typename T>
class BinningService : public BinningServiceBase
{
 public: 
  explicit BinningService(const edm::ParameterSet&);
  ~BinningService();

 private:
  virtual BinningBase* createBinning() const;
};

#endif  


