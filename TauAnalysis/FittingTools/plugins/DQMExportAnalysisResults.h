#ifndef TauAnalysis_FittingTools_DQMExportAnalysisResults_h  
#define TauAnalysis_FittingTools_DQMExportAnalysisResults_h

/** \class DQMExportAnalysisResults
 *
 * Auxiliary class to export analysis results into ASCII files,
 * in the format used by the CDF collaboration
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: DQMExportAnalysisResults.h,v 1.2 2010/10/22 11:55:02 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>
#include <string>

class DQMExportAnalysisResults : public edm::EDAnalyzer
{
 public:
  
  explicit DQMExportAnalysisResults(const edm::ParameterSet&);
  ~DQMExportAnalysisResults();
  
 private:

  void beginJob() {}
  void analyze(const edm::Event&, const edm::EventSetup&) {}	
  void endJob();

//--- configuration parameters
  std::string dqmDirectory_;
  std::string outputFilePath_;

  struct processEntryType
  { 
    processEntryType(const std::string& name, const edm::ParameterSet& cfg)
      : name_(name),
	dqmDirectory_(cfg.getParameter<std::string>("dqmDirectory")),
        outputFilePath_(cfg.getParameter<std::string>("outputFilePath")),
	outputFileName_(cfg.getParameter<std::string>("outputFileName")),
	hasSysUncertainties_(cfg.getParameter<bool>("hasSysUncertainties"))
    {}
    ~processEntryType() {}
    std::string name_;	
    std::string dqmDirectory_;	
    std::string outputFilePath_;
    std::string outputFileName_;
    bool hasSysUncertainties_;
  };

  std::vector<processEntryType*> processes_;

  struct channelEntryType
  { 
    channelEntryType(const std::string& name, unsigned index, const edm::ParameterSet& cfg)
      : name_(name),
	index_(index),
	meNameTemplate_(cfg.getParameter<std::string>("template")),
	outputFileName_(cfg.getParameter<std::string>("outputFileName"))
    {
      edm::ParameterSet cfgNormalization = cfg.getParameter<edm::ParameterSet>("normalization");
      meNameNumEventsProcessed_ = cfgNormalization.getParameter<std::string>("numEventsProcessed");
      meNameNumEventsPassed_ = cfgNormalization.getParameter<std::string>("numEventsPassed");
    }
    ~channelEntryType() {}
    std::string name_;	
    unsigned index_;
    std::string meNameTemplate_;	
    std::string meNameNumEventsProcessed_;
    std::string meNameNumEventsPassed_;
    std::string outputFileName_;
  };

  std::vector<channelEntryType*> channels_;

  struct systematicEntryType
  { 
    systematicEntryType(const std::string& name, const edm::ParameterSet& cfg)
      : name_(name),
	dqmDirectory_(cfg.getParameter<std::string>("dqmDirectory")),
	outputFilePath_(cfg.getParameter<std::string>("outputFilePath"))
    {}
    ~systematicEntryType() {}
    std::string name_;	
    std::string dqmDirectory_;	
    std::string outputFilePath_;
  };

  std::vector<systematicEntryType*> systematics_;
};

#endif  


