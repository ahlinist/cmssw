#ifndef TauAnalysis_BgEstimationTools_ObjValCorrelationAnalyzer_h  
#define TauAnalysis_BgEstimationTools_ObjValCorrelationAnalyzer_h

/** \class ObjValCorrelationAnalyzer
 *
 * Analyze correlation between observables stored as scalar values
 * in different branches of "plain" ROOT TTree
 * (produced by ObjValNtupleProducer)
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: ObjValCorrelationAnalyzer.h,v 1.1 2009/02/04 15:53:56 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <TChain.h>

#include <string>

class ObjValCorrelationAnalyzer : public edm::EDAnalyzer
{
  struct branchEntryType
  {
    branchEntryType(const std::string& branchName)
      : branchName_(branchName) {}
    ~branchEntryType() {}
    std::string branchName_;
    Float_t objValue_;
  };

 public:
  
  explicit ObjValCorrelationAnalyzer(const edm::ParameterSet&);
  ~ObjValCorrelationAnalyzer();
  
 private:

  void beginJob(const edm::EventSetup&);
  void analyze(const edm::Event&, const edm::EventSetup&) {}
  void endJob() {}

//--- configuration parameters
  typedef std::vector<std::string> vstring;
  std::string processName_;
  vstring fileNames_;
  std::string treeName_;

  vstring branchNames_;

//--- internal data-members for handling branches
  std::vector<branchEntryType*> branchEntries_;
};

#endif  


