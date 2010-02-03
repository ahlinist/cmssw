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
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"

//Hcal Noise Objects
#include "RecoMET/METAlgorithms/interface/HcalNoiseRBXArray.h"
#include "DataFormats/METReco/interface/HcalNoiseHPD.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"

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
    virtual ~InclusiveJetTreeProducer();

  private:
    
    void buildTree();
    void clearTreeVectors();
    
    //---- configurable parameters --------  
    bool mIsMCarlo;
    double mJetPtMin;
    std::string mJetsName;
    std::string mGenJetsName;
    std::string mJetsIDName;
    std::string mJetExtender;
    std::string mMetName;
    std::string mMetNoHFName;
    std::string mTriggerProcessName;
    std::vector<std::string> mTriggerNames;
    std::vector<std::string> mL1TriggerNames;
    std::vector<unsigned int> mTriggerIndex;
    edm::InputTag mHcalNoiseTag;
    edm::InputTag mTriggerResultsTag, mL1GTReadoutRcdSource, mL1GTObjectMapRcdSource;    
    HLTConfigProvider mHltConfig;     

    edm::Service<TFileService> fs;
    TTree *mTree;
    bool mFillHLT, mFillL1;
    //---- TREE variables --------
    int mRunNo, mEvtNo, mLumi, mBunch, mLooseHcalNoise, mTightHcalNoise;
    double mMET, mMETnoHF, mSumET, mSumETnoHF, mPtHat, mWeight;
    std::vector<int>    *mNtrkVtx,*mNtrkCalo,*mN90,*mN90Hits,*mPVntracks;
    std::vector<double> *mGenMatchR;
    std::vector<double> *mE,*mPt,*mEta,*mEtaD,*mPhi,*mY,*mEmf;
    std::vector<double> *mTrkCaloPt,*mTrkCaloEta,*mTrkCaloPhi;
    std::vector<double> *mTrkVtxPt,*mTrkVtxEta,*mTrkVtxPhi;
    std::vector<double> *mfHPD,*mfRBX,*mEtaMoment,*mPhiMoment;
    std::vector<double> *mPVx,*mPVy,*mPVz,*mPVchi2,*mPVndof;
    std::vector<double> *mfHcalNoise;
    std::vector<std::string> *mHLTNames;
    std::vector<std::string> *mL1Names;
};
#endif
