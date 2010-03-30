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

//L1 trigger beasties
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

//TFile Service 
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

class InclusiveJetTreeProducer : public edm::EDAnalyzer 
{
  public:
    explicit InclusiveJetTreeProducer(edm::ParameterSet const& cfg);
    virtual void beginJob();
    virtual void analyze(edm::Event const& evt, edm::EventSetup const& iSetup);
    virtual void endJob();
    virtual ~InclusiveJetTreeProducer();

  private:
    
    void buildTree();
    // called ahead of filling the tree to allow cutting on whatever 
    // combination of tree vars deemed appropriate
    bool preTreeFillCut();
    void clearTreeVectors();
    
    //---- configurable parameters --------  
    bool mIsMCarlo;
    //bool mFirstEventFlag;
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
    
    typedef struct 
    {
      int prescale;
      int fired;
    } TrigStruct;
    const L1GtTriggerMenu* m_l1GtMenu;  
    
    edm::Service<TFileService> fs;
    TTree *mTree;
    bool mFillHLT, mFillL1;
    //---- TREE variables --------
    int mRunNo, mEvtNo, mLumi, mBunch, mLooseHcalNoise, mTightHcalNoise;
    double mMET, mMETnoHF, mSumET, mSumETnoHF, mPtHat, mWeight;
    std::vector<int>    *mNtrkVtx,*mNtrkCalo,*mN90,*mN90Hits,*mPVntracks;
    std::vector<double> *mGenMatchR,*mGenMatchPt,*mGenMatchEta,*mGenMatchPhi;
    std::vector<double> *mE,*mPt,*mEta,*mEtaD,*mPhi,*mY,*mEmf;
    std::vector<double> *mTrkCaloPt,*mTrkCaloEta,*mTrkCaloPhi;
    std::vector<double> *mTrkVtxPt,*mTrkVtxEta,*mTrkVtxPhi;
    std::vector<double> *mfHPD,*mfRBX,*mEtaMoment,*mPhiMoment;
    std::vector<double> *mPVx,*mPVy,*mPVz,*mPVchi2,*mPVndof;
    std::vector<double> *mfHcalNoise;
    TrigStruct mHLTTrigResults[100],mL1TrigResults[100];
};
#endif
