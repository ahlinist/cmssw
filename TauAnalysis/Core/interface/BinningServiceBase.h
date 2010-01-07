#ifndef TauAnalysis_Core_BinningServiceBase_h  
#define TauAnalysis_Core_BinningServiceBase_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include "TauAnalysis/Core/interface/BinningBase.h"

#include <string>
#include <vector>

class BinningServiceBase
{
  struct meEntryType
  {
    meEntryType(int id, const std::string& name, const std::string& type, const std::string& value)
      : id_(id), name_(name), type_(type), value_(value) {}
    int id_;
    std::string name_;
    std::string type_;
    std::string value_;
  };

  friend bool operator<(const meEntryType&, const meEntryType&);

 public: 
  BinningServiceBase(const edm::ParameterSet&);
  virtual ~BinningServiceBase();
  
  virtual BinningBase* loadBinningResults(const std::string&) const;
  virtual void saveBinningResults(const std::string&, const BinningBase*) const;

 protected:
  virtual BinningBase* createBinning() const = 0;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<BinningServiceBase* (const edm::ParameterSet&)> BinningServicePluginFactory;

#endif  


