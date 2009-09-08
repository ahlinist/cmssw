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
 * \version $Revision: 1.3 $
 *
 * $Id: ObjValNtupleProducer.h,v 1.3 2009/07/30 16:41:46 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/BgEstimationTools/interface/ObjValExtractorBase.h"
#include "TauAnalysis/BgEstimationTools/interface/ObjValVectorExtractorBase.h"

#include <TTree.h>
#include <TH1.h>

#include <string>
#include <vector>
#include <memory>

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
  
  struct branchVectorEntryType
  {
    branchVectorEntryType(const std::string& branchName, ObjValVectorExtractorBase* objValExtractor)
      : branchName_(branchName), objValExtractor_(objValExtractor) {}
    ~branchVectorEntryType() { delete objValExtractor_; }
    std::string branchName_;
    ObjValVectorExtractorBase* objValExtractor_;
    std::auto_ptr<std::vector<double> > objValue_;
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
  std::vector<branchVectorEntryType*> branchVectorEntries_;

  TTree* ntuple_;
  TH1* fillStatusHistogram_;
  long numEvents_filled_;

  int fileServiceError_;
};

#endif  


