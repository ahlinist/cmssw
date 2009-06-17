#ifndef TauAnalysis_Core_GenericAnalyzer_h  
#define TauAnalysis_Core_GenericAnalyzer_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "PhysicsTools/UtilAlgos/interface/EventSelectorBase.h"

#include "TauAnalysis/Core/interface/AnalyzerPluginBase.h"
#include "TauAnalysis/Core/interface/EventDumpBase.h"
#include "TauAnalysis/Core/interface/FilterStatisticsService.h"
#include "TauAnalysis/Core/interface/RunEventNumberService.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include "DataFormats/Provenance/interface/RunID.h"
#include "DataFormats/Provenance/interface/EventID.h"

#include <TBenchmark.h>

#include <string>
#include <list>
#include <map>

class GenericAnalyzer : public edm::EDAnalyzer 
{
  struct analysisSequenceEntry
  {
    analysisSequenceEntry(const std::string&);
    virtual ~analysisSequenceEntry();
    virtual bool filter_cumulative(const edm::Event&, const edm::EventSetup&) { return true; }
    virtual bool filter_individual(const edm::Event&, const edm::EventSetup&) { return true; }
    virtual void beginJob() {}
    virtual void analyze(const edm::Event&, const edm::EventSetup&) {}
    virtual void endJob() {}
    virtual int type() const = 0;
    enum { kUndefined, kFilter, kAnalyzer };
    std::string name_;
  };

  struct analysisSequenceEntry_filter : analysisSequenceEntry
  {
    explicit analysisSequenceEntry_filter(const std::string&, const std::string&, const edm::ParameterSet&, int&);
    virtual ~analysisSequenceEntry_filter();
    void print() const;
    bool filter_cumulative(const edm::Event&, const edm::EventSetup&);
    bool filter_individual(const edm::Event&, const edm::EventSetup&);
    int type() const { return analysisSequenceEntry::kFilter; }
    EventSelectorBase* filterPlugin_cumulative_;
    EventSelectorBase* filterPlugin_individual_;
    static unsigned filterId_;
  };

  struct analysisSequenceEntry_analyzer : analysisSequenceEntry
  {
    explicit analysisSequenceEntry_analyzer(const std::string&, const std::list<edm::ParameterSet>&);
    virtual ~analysisSequenceEntry_analyzer();
    void print() const;
    void beginJob();
    void analyze(const edm::Event&, const edm::EventSetup&);
    void endJob();
    int type() const { return analysisSequenceEntry::kAnalyzer; }
    std::list<AnalyzerPluginBase*> analyzerPlugins_;
  };

 public: 
  explicit GenericAnalyzer(const edm::ParameterSet&);
  ~GenericAnalyzer();
  
  void beginJob(const edm::EventSetup&);
  void analyze(const edm::Event&, const edm::EventSetup&);
  void endJob();

 private:
  typedef std::vector<std::string> vstring;
  void addFilter(const std::string&, const vstring&);
  void addAnalyzers(const vstring&, const std::string&, const std::string&, const vstring&);

  std::string name_;

  std::map<std::string, edm::ParameterSet> cfgFilters_;
  std::map<std::string, edm::ParameterSet> cfgAnalyzers_;
  
  std::list<analysisSequenceEntry*> analysisSequence_;

  FilterStatisticsService* filterStatisticsService_;
  std::string filterStatisticsService_dqmDirectory_;
  FilterStatisticsTable* filterStatisticsTable_;

  RunEventNumberService* runEventNumberService_;
  
  std::list<EventDumpBase*> eventDumps_;

  TBenchmark benchmark_;

  int cfgError_;
};

#endif  


