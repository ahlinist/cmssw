#ifndef INCLUSIVE_JET_TREE_PRODUCER_H
#define INCLUSIVE_JET_TREE_PRODUCER_H

#include "TTree.h"
#include "TFile.h"
#include "TH1.h" 
#include "TNamed.h"
#include <vector>
#include <map>
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include <DataFormats/VertexReco/interface/VertexFwd.h>
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"



class InclusiveJetTreeProducer : public edm::EDAnalyzer 
{
  public:
    explicit InclusiveJetTreeProducer(edm::ParameterSet const& cfg);
    virtual void beginJob(edm::EventSetup const& iSetup);
    virtual void analyze(edm::Event const& evt, edm::EventSetup const& iSetup);
    virtual void endJob();
    ~InclusiveJetTreeProducer();

  private:
    std::string mFileName;
    std::string mJetsName;
    std::string mJetExtender;
    std::string mMetName;
    std::string mMetNoHFName;
    std::vector<std::string> mTriggerNames;
    std::vector<std::string> mL1TriggerNames;
    std::string mTriggerProcessName;
    std::vector<unsigned int> mTriggerIndex;
    edm::InputTag mTriggerResultsTag;
    HLTConfigProvider mHltConfig;
    double mEtaMax;
    double mPtMin; 
    bool   mIsMCarlo;
    TFile* mFile;
    TTree* mTree;
    ///// Jet Variables //////
    float mE[20],mEmf[20], mPt[20],mEta[20],mPhi[20],mN90[20],mCTPt[20],mCTEta[20],mCTPhi[20],mVTPt[20],mVTEta[20],mVTPhi[20];
    ///// Event Variables ////
    float mMET[2],mSumET[2];
    float mPVx, mPVy, mPVz, mPtHat;
    int mHLTBits[6];
    int mL1Bits[5];
    int mJetSize, mNPV;
    int mRun, mEvent;
    int mNtrkVx[5],mNtrkCalo[5];
};
#endif
