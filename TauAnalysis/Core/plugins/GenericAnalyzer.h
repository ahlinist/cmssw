#ifndef TauAnalysis_Core_GenericAnalyzer_h  
#define TauAnalysis_Core_GenericAnalyzer_h

 /** \class GenericAnalyzer
  *
  * Top-level EDAnalyzer module for event selection and histogram filling in tau analyses
  * (all decay channels supported by GenericAnalyzer module; 
  *  just needs suitable python configuration parametersets...)
  * 
  * \author Christian Veelken, UC Davis
  *
  * \version $Revision: 1.15 $
  *
  * $Id: GenericAnalyzer.h,v 1.15 2010/12/24 17:20:23 veelken Exp $
  *
  */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CommonTools/UtilAlgos/interface/EventSelectorBase.h"

#include "TauAnalysis/Core/interface/AnalyzerPluginBase.h"
#include "TauAnalysis/Core/interface/EventDumpBase.h"
#include "TauAnalysis/Core/interface/FilterStatisticsService.h"
#include "TauAnalysis/Core/interface/RunLumiSectionEventNumberService.h"
#include "TauAnalysis/Core/interface/genericAnalyzerAuxFunctions.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include "DataFormats/Provenance/interface/RunID.h"
#include "DataFormats/Provenance/interface/EventID.h"

#include <TBenchmark.h>

#include <string>
#include <list>
#include <map>

class SysUncertaintyService;

class GenericAnalyzer : public edm::EDAnalyzer 
{
  struct analysisSequenceEntry
  {
    analysisSequenceEntry(const std::string&);
    virtual ~analysisSequenceEntry();
    virtual bool filter_cumulative(const edm::Event&, const edm::EventSetup&, const SysUncertaintyService*) { return true; }
    virtual bool filter_individual(const edm::Event&, const edm::EventSetup&, const SysUncertaintyService*) { return true; }
    virtual void beginJob() {}
    virtual void analyze(const edm::Event&, const edm::EventSetup&, double, bool) {}
    virtual void endJob() {}
    virtual int type() const = 0;
    virtual int filterId() const { return -1; }
    enum { kUndefined, kFilter, kAnalyzer };
    std::string name_;
  };

  struct analysisSequenceEntry_filter : analysisSequenceEntry
  {
    explicit analysisSequenceEntry_filter(const std::string&, const std::string&, const edm::ParameterSet&, bool, int&);
    virtual ~analysisSequenceEntry_filter();
    void print() const;
    bool filter_cumulative(const edm::Event&, const edm::EventSetup&, const SysUncertaintyService*);
    bool filter_individual(const edm::Event&, const edm::EventSetup&, const SysUncertaintyService*);
    int type() const { return analysisSequenceEntry::kFilter; }
    int filterId() const { return filterId_; }
    bool filter(const edm::Event&, const edm::EventSetup&, const SysUncertaintyService*, 
		const std::map<std::string, EventSelectorBase*>&);
    std::map<std::string, EventSelectorBase*> filterPlugins_cumulative_;
    std::map<std::string, EventSelectorBase*> filterPlugins_individual_;
    bool estimateSysUncertainties_;
    int filterId_;
    static int filterIdCounter_;
  };

  struct analysisSequenceEntry_analyzer : analysisSequenceEntry
  {
    explicit analysisSequenceEntry_analyzer(const std::string&, const std::list<edm::ParameterSet>&);
    virtual ~analysisSequenceEntry_analyzer();
    void print() const;
    void beginJob();
    void analyze(const edm::Event&, const edm::EventSetup&, double, bool);
    void endJob();
    int type() const { return analysisSequenceEntry::kAnalyzer; }
    struct analyzerPluginEntry
    {
      AnalyzerPluginBase* plugin_;
      bool supportsSystematics_;
    };
    std::list<analyzerPluginEntry> analyzerPlugins_;
    double eventWeight_;
  };

 public: 
  explicit GenericAnalyzer(const edm::ParameterSet&);
  ~GenericAnalyzer();
  
  void beginJob();
  void analyze(const edm::Event&, const edm::EventSetup&);
  void endJob();
  
 private:
  typedef std::vector<std::string> vstring;
  void addFilter(const std::string&, const vstring&);
  void addAnalyzers(const vstring&, const std::string&, const std::string&, const vstring&);

  int checkEventWeightConfig(const std::string&, const std::string&, const std::list<analysisSequenceEntry*>&, int&);
  friend class analysisSequenceMatch;

  std::string name_;

  struct eventWeightType
  {
    eventWeightType(const edm::ParameterSet& cfg)
      : src_(cfg.getParameter<edm::InputTag>("src"))
    {
      applyAfterFilter_  = ( cfg.exists("applyAfterFilter")  ) ? 
	cfg.getParameter<std::string>("applyAfterFilter")  : "*";
      applyBeforeFilter_ = ( cfg.exists("applyBeforeFilter") ) ? 
	cfg.getParameter<std::string>("applyBeforeFilter") : "*";
    }
    void update(const edm::Event& evt) {
      edm::Handle<double> weight;
      evt.getByLabel(src_, weight);
      value_ = (*weight);
    }
    edm::InputTag src_;
    double value_;
    std::string applyAfterFilter_;
    int applyAfterFilterId_;
    std::string applyBeforeFilter_;
    int applyBeforeFilterId_;
  };
  std::vector<eventWeightType> eventWeights_;

  std::map<std::string, edm::ParameterSet> cfgFilters_;
  std::map<std::string, edm::ParameterSet> cfgAnalyzers_;

  bool estimateSysUncertainties_;
  vstring systematics_;
  
  std::list<analysisSequenceEntry*> analysisSequence_;

  FilterStatisticsService* filterStatisticsService_;
  std::string filterStatisticsService_dqmDirectory_;
  FilterStatisticsTable* filterStatisticsTable_;

  RunLumiSectionEventNumberService* runLumiSectionEventNumberService_;
  
  std::list<EventDumpBase*> eventDumps_;

  TBenchmark benchmark_;

  int cfgError_;
};

#endif  


