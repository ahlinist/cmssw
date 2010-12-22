#ifndef TauAnalysis_FittingTools_DQMExportAnalysisResults_h  
#define TauAnalysis_FittingTools_DQMExportAnalysisResults_h

/** \class DQMExportAnalysisResults
 *
 * Auxiliary class to export analysis results into ASCII files,
 * in the format used by the CDF collaboration
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.6 $
 *
 * $Id: DQMExportAnalysisResults.h,v 1.6 2010/12/08 10:22:18 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <TH1.h>

#include <vector>
#include <set>
#include <string>

class DQMExportAnalysisResults : public edm::EDAnalyzer
{
  typedef std::vector<std::string> vstring;

 public:
  
  explicit DQMExportAnalysisResults(const edm::ParameterSet&);
  ~DQMExportAnalysisResults();
  
 private:

  void beginJob() {}
  void analyze(const edm::Event&, const edm::EventSetup&) {}	
  void endJob();

//--- configuration parameters
  std::string outputFilePath_;

  struct channelEntryType
  { 
    channelEntryType(unsigned index, const edm::ParameterSet& cfg)
      : name_(cfg.getParameter<std::string>("name")),
        index_(index),
	shortName_(cfg.getParameter<std::string>("shortName")),
	meNameBinning_(cfg.getParameter<std::string>("binning")),
	histogramBinning_(0),
	dataIntLumi_(cfg.getParameter<double>("dataIntLumi"))
    {}
    ~channelEntryType() {}
    std::string name_;
    unsigned index_;
    std::string shortName_;
    std::string meNameBinning_;
    TH1* histogramBinning_;
    double dataIntLumi_;
  };

  std::vector<channelEntryType*> channels_;
  
  unsigned numChannels_;
  unsigned numBinsTotal_;
  std::map<unsigned, unsigned> binOffsets_; // key = channelIndex

  struct distributionEntryType
  { 
    distributionEntryType(const std::string& name, const edm::ParameterSet& cfg)
      : name_(name),
	meNameTemplate_(cfg.getParameter<std::string>("template"))
    {
      if ( cfg.exists("systematics") ) {
        vstring sysNames = cfg.getParameter<vstring>("systematics");
        for ( vstring::const_iterator sysName = sysNames.begin();
  	      sysName != sysNames.end(); ++sysName ) {
	  systematics_.insert(*sysName);
        }
      }

      edm::ParameterSet cfgNormalization = cfg.getParameter<edm::ParameterSet>("normalization");
      meNameNumEventsProcessed_ = cfgNormalization.getParameter<std::string>("numEventsProcessed");
      meNameNumEventsPassed_ = cfgNormalization.getParameter<std::string>("numEventsPassed");
    }
    ~distributionEntryType() {}
    std::string name_;	
    std::string meNameTemplate_;
    std::set<std::string> systematics_;
    std::string meNameNumEventsProcessed_;
    std::string meNameNumEventsPassed_;
  };

  struct processEntryType
  { 
    processEntryType(const std::string& name, const edm::ParameterSet& cfg)
      : name_(name),
	outputFilePath_(cfg.getParameter<std::string>("outputFilePath")),
	outputFileName_(cfg.getParameter<std::string>("outputFileName"))
    {
      edm::ParameterSet cfgDistributions = cfg.getParameter<edm::ParameterSet>("distributions");
      vstring channelNames = cfgDistributions.getParameterNamesForType<edm::ParameterSet>();
      for ( vstring::const_iterator channelName = channelNames.begin();
	    channelName != channelNames.end(); ++channelName ) {
	edm::ParameterSet cfgDistribution = cfgDistributions.getParameter<edm::ParameterSet>(*channelName);
	distributionEntryType* distributionEntry = new distributionEntryType(*channelName, cfgDistribution);
	distributions_.insert(std::pair<std::string, distributionEntryType*>(*channelName, distributionEntry));
      }
      
      xSection_ = ( cfg.exists("xSection") ) ?
	cfg.getParameter<double>("xSection") : -1.;
    }
    ~processEntryType() 
    {
      for ( std::map<std::string, distributionEntryType*>::iterator it = distributions_.begin();
	    it != distributions_.end(); ++it ) {
	delete it->second;
      }
    }
    std::string name_;	
    std::map<std::string, distributionEntryType*> distributions_; // key = channelName
    double xSection_; // cross-section [pb] (@ highest available order)
    std::string outputFilePath_;
    std::string outputFileName_;
  };

  std::vector<processEntryType*> processes_;

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


