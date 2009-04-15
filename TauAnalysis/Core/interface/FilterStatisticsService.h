#ifndef TauAnalysis_Core_FilterStatisticsService_h  
#define TauAnalysis_Core_FilterStatisticsService_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/Core/interface/FilterStatisticsTable.h"

class FilterStatisticsService
{
 public: 
  explicit FilterStatisticsService(const edm::ParameterSet&);
  ~FilterStatisticsService();

  void createFilterStatisticsTable();
  void loadFilterStatisticsTable();
  void saveFilterStatisticsTable() const;

  FilterStatisticsTable* filterStatisticsTable() { return filterStatisticsTable_; }
  const FilterStatisticsTable* filterStatisticsTable() const { return filterStatisticsTable_; }

 private:
  void saveFilterStatisticsElement(DQMStore&, const FilterStatisticsElement*) const;

  std::string name_;

  std::string dqmDirectory_store_;

  edm::ParameterSet* cfgFilterStatisticsTable_;

  FilterStatisticsTable* filterStatisticsTable_;
};

#endif  


