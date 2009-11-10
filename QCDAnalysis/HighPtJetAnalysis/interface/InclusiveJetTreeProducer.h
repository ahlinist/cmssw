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

class InclusiveJetTreeProducer : public edm::EDAnalyzer 
{
  public:
    explicit InclusiveJetTreeProducer(edm::ParameterSet const& cfg);
    virtual void beginJob(edm::EventSetup const& iSetup);
    virtual void analyze(edm::Event const& evt, edm::EventSetup const& iSetup);
    virtual void endJob();
    InclusiveJetTreeProducer();

  private:
    std::string mFileName;
    std::string mJetsName;
    std::string mJetExtender;
    std::string mMetName;
    std::vector<std::string> mTriggerNames;
    std::string mTriggerProcessName;
    std::vector<unsigned int> mTriggerIndex;
    edm::InputTag mTriggerResultsTag;
    HLTConfigProvider mHltConfig;
    double mEtaMax;
    double mPtMin; 
    TFile* mFile;
    TTree* mTree;
    ///// Jet Variables //////
    float mPx[20],mPy[20],mPz[20],mE[20],mEmf[20],mNtrkVx[20],mNtrkCalo[20],mPt[20],mEta[20],mPhi[20],mN90[20],mCTPt[20],mCTEta[20],mCTPhi[20],mVTPt[20],mVTEta[20],mVTPhi[20];
    ///// Event Variables ////
    float mMET,mSumET;
    double mNPV, mPVx, mPVy, mPVz;
    int mHLTBits[6];
    int mJetSize;
};
#endif
