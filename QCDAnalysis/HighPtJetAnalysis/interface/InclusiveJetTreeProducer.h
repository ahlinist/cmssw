#ifndef INCLUSIVE_JET_TREE_PRODUCER_H
#define INCLUSIVE_JET_TREE_PRODUCER_H

#include "TTree.h"
#include "TFile.h"
#include "TH1.h" 
#include "TNamed.h"
#include <vector>
#include <string>
#include <map>
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMap.h"


//TFile Service 
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

class InclusiveJetTreeProducer : public edm::EDAnalyzer 
{
  public:
    explicit InclusiveJetTreeProducer(edm::ParameterSet const& cfg);
    virtual void beginJob(edm::EventSetup const& iSetup);
    virtual void analyze(edm::Event const& evt, edm::EventSetup const& iSetup);
    virtual void endJob();
    InclusiveJetTreeProducer();

  private:
    
    void buildTree();
    void clearTreeVectors();

    std::string mJetsName;
    std::string mJetExtender;
    std::string mMetName;
    std::string mMetNoHFName;
    std::vector<std::string> mTriggerNames;
    std::vector<std::string> mL1TriggerNames;
    std::string mTriggerProcessName;
    std::vector<unsigned int> mTriggerIndex;
    edm::InputTag mTriggerResultsTag;
    double mEtaMax;
    double mPtMin; 
    edm::InputTag L1GTReadoutRcdSource_, L1GTObjectMapRcdSource_;    
    HLTConfigProvider mHltConfig;

    edm::Service<TFileService> fs;                                                                                                                       
    TTree* mTree;

    std::vector<double> *mE,   *mPt,     *mEta,      *mPhi;
    std::vector<double> *mEmf, *mNtrkVx, *mNtrkCalo;
    std::vector<double> *mCTPt,*mCTEta,  *mCTPhi;
    std::vector<double> *mVTPt,*mVTEta,  *mVTPhi;
    std::vector<double> *mN90;
    std::vector<double> *mPVx, *mPVy,    *mPVz;
    std::vector<std::string> *mHLTNames;
    std::vector<std::string> *mL1Names;
    
    double mMET, mMETnoHF, mSumET, mSumETnoHF;
    int mRun, mEvent;
};
#endif
