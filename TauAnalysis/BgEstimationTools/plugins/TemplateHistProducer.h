#ifndef TauAnalysis_BgEstimationTools_TemplateHistProducer_h  
#define TauAnalysis_BgEstimationTools_TemplateHistProducer_h

/** \class TemplateHistProducer
 *
 * Produce histogram of observable stored as scalar value in branch 
 * of "plain" ROOT TTree (produced by ObjValNtupleProducer);
 * histogram is used as template for determining from data
 * contributions of signal and background processes to final event sample
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.6 $
 *
 * $Id: TemplateHistProducer.h,v 1.6 2009/10/27 16:44:21 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include <TChain.h>
#include <TTree.h>

#include <string>

class TemplateHistProducer : public edm::EDAnalyzer
{
  typedef std::vector<double> vdouble;

  struct eventWeightEntryType
  {
    eventWeightEntryType(const std::string& branchName)
      : branchName_(branchName), value_(1.) {}
    std::string branchName_;
    Float_t value_;
  };

  struct histEntryType
  { 
    histEntryType() 
      : me_(0) {}
    ~histEntryType();

    struct axisEntryType
    {
      std::string branchName_;
      Float_t value_;

      unsigned numBins_;
      float* binEdges_;
    };

    std::string meName_;
    std::string dqmDirectory_store_;
    
    MonitorElement* me_;

    std::vector<axisEntryType> axisEntries_;

    bool sumWeights_;
  };

 public:
  
  explicit TemplateHistProducer(const edm::ParameterSet&);
  ~TemplateHistProducer();
  
 private:

  void beginJob(const edm::EventSetup&) {}
  void analyze(const edm::Event&, const edm::EventSetup&) {}
  void endJob();

  void readHistEntry(const edm::ParameterSet& cfg);

//--- configuration parameters
  typedef std::vector<std::string> vstring;
  vstring fileNames_;
  std::string treeName_;

  std::string treeSelection_;

  std::vector<eventWeightEntryType> eventWeights_;

  double norm_;

//--- internal data-members for handling branches
//    and filling histogram
  TChain* allEventsTree_;
  TTree* selEventsTree_;

  std::vector<histEntryType> histograms_; 

  int cfgError_;
};

#endif  


