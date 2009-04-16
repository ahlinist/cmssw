#ifndef TauAnalysis_Core_FilterStatisticsService_h  
#define TauAnalysis_Core_FilterStatisticsService_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/Core/interface/FilterStatisticsTable.h"

class FilterStatisticsService
{
 public: 
  explicit FilterStatisticsService();
  ~FilterStatisticsService();

  FilterStatisticsTable* createFilterStatisticsTable(const edm::ParameterSet&) const;
  FilterStatisticsTable* loadFilterStatisticsTable(const std::string&) const;
  void saveFilterStatisticsTable(const std::string&, const FilterStatisticsTable*) const;

 private:
  void saveFilterStatisticsElement(DQMStore&, const FilterStatisticsElement*) const;

  std::string name_;
};

#endif  


