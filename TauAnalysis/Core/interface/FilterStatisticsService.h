#ifndef TauAnalysis_Core_FilterStatisticsService_h  
#define TauAnalysis_Core_FilterStatisticsService_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include <string>
#include <map>
#include <vector>

class FilterStatisticsService
{
  struct monElement
  {
    explicit monElement(const std::string&);
    ~monElement();
    void update(bool, double);
    std::string name_;
    MonitorElement* num_;
    MonitorElement* numWeighted_;
  };

  struct filterEntry
  {
    explicit filterEntry(const std::string&, const std::string&, unsigned, const std::string&);
    ~filterEntry();
    void update(bool, bool, bool, unsigned, unsigned, double);
    void print(std::ostream&, unsigned = 20, unsigned = 20) const;
    void printNumber(std::ostream&, unsigned, const monElement*) const;
    void printPercentage(std::ostream&, unsigned, const monElement*, const monElement*) const;
    std::string dqmDirectory_store_;
    MonitorElement* name_;
    monElement* processed_;
    monElement* passed_;
    monElement* exclRejected_;
    monElement* processed_cumulative_;
    monElement* passed_cumulative_;
  };

 public: 
  explicit FilterStatisticsService(const edm::ParameterSet&);
  ~FilterStatisticsService();
  
  typedef std::vector<std::pair<std::string, bool> > filterResults_type;
  void update(const filterResults_type&, const filterResults_type&, double);

  void print(std::ostream&, unsigned = 30, unsigned = 20) const;

 private:
  std::string name_;

  std::string dqmDirectory_store_;

  typedef std::pair<std::string, filterEntry*> filterEntry_type; 
  std::vector<filterEntry_type> filterEntries_; 

  long numEventsProcessed_;
  long numEventsPassedAllFilters_;

  int cfgError_;
};

#endif  


