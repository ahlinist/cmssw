#ifndef TauAnalysis_Core_RunLumiSectionEventNumberService_h  
#define TauAnalysis_Core_RunLumiSectionEventNumberService_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Provenance/interface/RunID.h"
#include "DataFormats/Provenance/interface/EventID.h"

#include <string>
#include <map>

class RunLumiSectionEventNumberService
{
  struct filterConfigEntry
  {
    bool doSaveRunLumiSectionEventNumbers_passed_;
    std::string dqmDirectory_passed_;
    bool doSaveRunLumiSectionEventNumbers_rejected_;
    std::string dqmDirectory_rejected_;
    bool doSaveRunLumiSectionEventNumbers_exclRejected_;
    std::string dqmDirectory_exclRejected_;
    bool doSaveRunLumiSectionEventNumbers_passed_cumulative_;
    std::string dqmDirectory_passed_cumulative_;
    bool doSaveRunLumiSectionEventNumbers_rejected_cumulative_;
    std::string dqmDirectory_rejected_cumulative_;
  };

 public: 
  explicit RunLumiSectionEventNumberService(const edm::ParameterSet&);
  ~RunLumiSectionEventNumberService();

  typedef std::map<std::string, bool> filterResults_type;
  void update(edm::RunNumber_t, edm::LuminosityBlockNumber_t, edm::EventNumber_t,
	      const filterResults_type&, const filterResults_type&, double);

 private:
  std::string name_;

  std::string dqmDirectory_store_;

  std::map<std::string, filterConfigEntry> filterConfigs_;

  int cfgError_;
};

#endif  


