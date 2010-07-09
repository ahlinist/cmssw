#ifndef RELJEC_TREE_PRODUCER_H
#define RELJEC_TREE_PRODUCER_H

#include "TTree.h"
#include "TFile.h"
#include "TNamed.h"
#include <vector>
#include <string>
#include "FWCore/Framework/interface/EDAnalyzer.h"

//TFile Service 
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

class RelJECTreeProducer : public edm::EDAnalyzer 
{
  public:
    explicit RelJECTreeProducer(edm::ParameterSet const& cfg);
    virtual void beginJob();
    virtual void analyze(edm::Event const& evt, edm::EventSetup const& iSetup);
    virtual void endJob();
    virtual ~RelJECTreeProducer();

  private:
    
    void buildTree();
    bool preTreeFillCut();
    void clearTreeVectors();
    
    bool   mIsMCarlo;
    double mJetEtaMax;
    double mJetPtMin;
    std::string mCaloJetsName;
    std::string mPFJetsName;
    std::string mJPTJetsName;   
   
    edm::Service<TFileService> fs;
    TTree *mTree;
    //---- TREE variables --------
    int mRunNo, mEvtNo, mLumi, mBunch;
    float mPtHat, mWeight;
    std::vector<float> *mCaloMatchR,*mCaloMatchE,*mCaloMatchPt,*mCaloMatchEta,*mCaloMatchPhi;
    std::vector<float> *mJPTMatchR,*mJPTMatchE,*mJPTMatchPt,*mJPTMatchEta,*mJPTMatchPhi;
    std::vector<float> *mCaloResponse,*mJPTResponse;
    std::vector<float> *mE,*mPt,*mEta,*mPhi;
};
#endif

