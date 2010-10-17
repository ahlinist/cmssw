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
 * $Id: DQMExportAnalysisResults.h,v 1.2 2010/09/28 11:23:34 jkolb Exp $
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
        numEvents_(cfg.getParameter<unsigned>("numEvents")),
	hasSysUncertainties_(cfg.getParameter<bool>("hasSysUncertainties"))
    {}
    ~processEntryType() {}
    std::string name_;	
    std::string dqmDirectory_;	
    std::string outputFilePath_;
    std::string outputFileName_;
    double numEvents_;
    bool hasSysUncertainties_;
  };

  std::vector<processEntryType*> processes_;

  struct channelEntryType
  { 
    channelEntryType(const std::string& name, unsigned index, const edm::ParameterSet& cfg)
      : name_(name),
	index_(index),
	meName_(cfg.getParameter<std::string>("meName")),
	outputFileName_(cfg.getParameter<std::string>("outputFileName"))
    {}
    ~channelEntryType() {}
    std::string name_;	
    unsigned index_;
    std::string meName_;	
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


