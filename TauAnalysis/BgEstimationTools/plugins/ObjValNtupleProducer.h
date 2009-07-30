#ifndef TauAnalysis_BgEstimationTools_ObjValNtupleProducer_h  
#define TauAnalysis_BgEstimationTools_ObjValNtupleProducer_h

/** \class ObjValNtupleProducer
 *
 * Produce "plain" ROOT TTree with branches containing scalar values
 * extracted using classes derived from ObjValExtractorBase
 * 
 * NOTE: this class is declared as EDAnalyzer, not as EDProducer,
 *       because if produces a "plain" ROOT TTree, 
 *       not some product that is added to the edm::Event
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: ObjValNtupleProducer.h,v 1.2 2009/06/17 12:45:43 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/BgEstimationTools/interface/ObjValExtractorBase.h"

#include <TTree.h>
#include <TH1.h>

#include <string>
#include <vector>

class ObjValNtupleProducer : public edm::EDAnalyzer
{
  struct branchEntryType
  {
    branchEntryType(const std::string& branchName, ObjValExtractorBase* objValExtractor)
      : branchName_(branchName), objValExtractor_(objValExtractor) {}
    ~branchEntryType() { delete objValExtractor_; }
    std::string branchName_;
    ObjValExtractorBase* objValExtractor_;
    Float_t objValue_;
  };

 public:
  
  explicit ObjValNtupleProducer(const edm::ParameterSet&);
  ~ObjValNtupleProducer();
  
 private:

  void beginJob(const edm::EventSetup&);
  void analyze(const edm::Event&, const edm::EventSetup&);
  void endJob() {}

//--- configuration parameters
  std::string treeName_;
  std::string treeTitle_;

//--- internal data-members for handling branches
  std::vector<branchEntryType*> branchEntries_;

  TTree* ntuple_;
  TH1* fillStatusHistogram_;
  long numEvents_filled_;

  int fileServiceError_;
};

#endif  


