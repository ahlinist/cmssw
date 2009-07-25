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
 * \version $Revision: 1.2 $
 *
 * $Id: TemplateHistProducer.h,v 1.2 2009/07/15 09:04:59 veelken Exp $
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
 public:
  
  explicit TemplateHistProducer(const edm::ParameterSet&);
  ~TemplateHistProducer();
  
 private:

  void beginJob(const edm::EventSetup&) {}
  void analyze(const edm::Event&, const edm::EventSetup&) {}
  void endJob();

//--- configuration parameters
  typedef std::vector<std::string> vstring;
  vstring fileNames_;
  std::string treeName_;

  std::string treeSelection_;

  std::string branchName_eventWeight_;
  Float_t eventWeight_;

  std::string branchName_objValue_;
  Float_t objValue_;

  std::string dqmDirectory_store_;
  std::string meName_;

  double norm_;

  unsigned numBinsX_;
  double xMin_;
  double xMax_;
  typedef std::vector<double> vdouble;
  vdouble xBins_;

//--- internal data-members for handling branches
//    and filling histogram
  TChain* allEventsTree_;
  TTree* selEventsTree_;

  MonitorElement* hTemplate_;

  int cfgError_;
};

#endif  


