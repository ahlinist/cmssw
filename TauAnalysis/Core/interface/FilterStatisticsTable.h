#ifndef TauAnalysis_Core_FilterStatisticsTable_h  
#define TauAnalysis_Core_FilterStatisticsTable_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <string>
#include <vector>
#include <map>

class FilterStatisticsService;

namespace fsElement
{
  const std::string processed = "processed";
  const std::string passed = "passed";
  const std::string exclRejected = "exclRejected";
  const std::string processed_cumulative = "processed_cumulative";
  const std::string passed_cumulative = "passed_cumulative";
}

class FilterStatisticsElement
{
  friend class FilterStatisticsService;
  
 public:
  explicit FilterStatisticsElement(const std::string& name)
    : name_(name), num_(0), numWeighted_(0.) {}
  explicit FilterStatisticsElement(const std::string& name, int64_t num, double numWeighted)
    : name_(name), num_(num), numWeighted_(numWeighted) {}  
  ~FilterStatisticsElement() {}

  void update(bool filterPassed, double eventWeight) {
    if ( filterPassed ) {
      ++num_;
      numWeighted_ += eventWeight;
    }
  }
  
  int64_t num() const { return num_; }
  double numWeighted() const { return numWeighted_; }

 private:
  std::string name_;
  
  int64_t num_;
  double numWeighted_;
};

class FilterStatisticsRow
{
  friend class FilterStatisticsService;

 public:
  explicit FilterStatisticsRow(int, const std::string&, const std::string&);
  ~FilterStatisticsRow();

  int filterId() const { return filterId_; }
  const std::string& filterName() const { return filterName_; }
  const std::string& filterTitle() const { return filterTitle_; }

  void update(bool, bool, bool, unsigned, unsigned, double, double);
  
  void print(std::ostream&, unsigned, unsigned) const;

  static std::vector<std::string> columnLabels() { return columnLabels_; }
  enum { kFilterTitle, kPassed_cumulative, kEff_cumulative, kEff_marginal, kEff_individual, kExclRejected, kProcessed, kNumColumns };

  double extractNumber(const std::string&, bool) const;
  
 private:
  int filterId_;
  std::string filterName_;
  std::string filterTitle_;
  
  FilterStatisticsElement* numEvents_processed_;
  FilterStatisticsElement* numEvents_passed_;
  FilterStatisticsElement* numEvents_exclRejected_;
  FilterStatisticsElement* numEvents_processed_cumulative_;
  FilterStatisticsElement* numEvents_passed_cumulative_;

  static std::vector<std::string> columnLabels_;
};

class FilterStatisticsTable
{
  friend class FilterStatisticsService;

 public: 
  explicit FilterStatisticsTable();
  explicit FilterStatisticsTable(const edm::ParameterSet&);
  ~FilterStatisticsTable();

  const std::string& name() const { return name_; }
  
  typedef std::map<std::string, bool> filterResults_type;
  typedef std::map<std::string, double> eventWeights_type;
  void update(const filterResults_type&, const filterResults_type&, const eventWeights_type&, const eventWeights_type&);

  void print(std::ostream&, unsigned = 42, unsigned = 19) const;

  std::vector<std::string> extractFilterTitleColumn() const;
  std::vector<double> extractColumn(const std::string&, bool) const;

  /// Extract a number from the table
  double extractNumber(const std::string& row, const std::string& col, bool weighted=false) const;

 private:
  std::string name_;

  int64_t numEvents_processed_;
  int64_t numEvents_passedAllFilters_;

  typedef std::pair<std::string, FilterStatisticsRow*> rowEntry_type; 
  std::vector<rowEntry_type> rows_; 
};

#endif  


