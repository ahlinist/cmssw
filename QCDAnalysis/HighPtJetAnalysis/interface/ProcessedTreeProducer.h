#ifndef PROCESSED_TREE_PRODUCER_H
#define PROCESSED_TREE_PRODUCER_H

#include "TTree.h"
#include "TFile.h"
#include "TH1.h" 
#include "TNamed.h"
#include <vector>
#include <string>
#include <map>
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
//TFile Service 
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

class ProcessedTreeProducer : public edm::EDAnalyzer 
{
  public:
    explicit ProcessedTreeProducer(edm::ParameterSet const& cfg);
    virtual void beginJob();
    virtual void analyze(edm::Event const& evt, edm::EventSetup const& iSetup);
    virtual void endJob();
    virtual ~ProcessedTreeProducer();

  private:
    
    void buildTree();
    void clearTreeArrays();
    
    //---- configurable parameters --------  
    bool   mIsMCarlo;
    int    mNCaloJETS_MAX;
    int    mNPFJETS_MAX;
    int    mGoodVtxNdof;
    double mGoodVtxZ; 
    std::string mCaloJetsName;
    std::string mCaloJetID;
    std::string mCaloJetExtender;
    std::string mPFJetsName;
    std::string mGenJetsName;
    std::string mCaloJECservice;
    std::string mPFJECservice;
    std::string mPFPayloadName;
    std::string mCaloPayloadName;
    
    typedef struct {
      int   runNo;
      int   evtNo;
      int   lumi;
      int   bunch;
      int   nVtx;
      int   nVtxGood;
      int   isPVgood;
      float PVndof;
      float PVx;
      float PVy;
      float PVz;
      float pfCorMass;
      float pfRawMass;
      float caloCorMass;
      float caloRawMass;
      float pfDeta;
      float caloDeta;
      float pfmet;
      float pfsumet;
      float pfmet_over_sumet;
      float calomet;
      float calosumet;
      float calomet_over_sumet;  
      float pthat;
      float weight;
    } EVENT_VAR;
    typedef struct {
      int   passLooseID;
      int   npr;
      int   chm;
      int   nhm;
      int   phm;
      int   elm;
      float jec;
      float jecUnc;
      float rawPt;
      float corPt;
      float eta;
      float y;
      float phi;
      float rawE;
      float corE;
      float m;  
      float chf;
      float nhf;
      float phf;
      float elf;
      float genR;
      float genPt;
      float genEta;
      float genPhi;
      float genE;
      float genM;
      float corRsp;
      float rawRsp;
    } PFJET_VAR;
    typedef struct {
      int   passLooseID;
      int   n90hits;
      int   nTrkVtx;
      int   nTrkCalo;
      float jec;
      float jecUnc;
      float rawPt;
      float corPt;
      float eta;
      float y;
      float phi;
      float rawE;
      float corE;
      float m;
      float emf;
      float fHPD;
      float fRBX;
      float genR;
      float genPt;
      float genEta;
      float genPhi;
      float genE;
      float genM;
      float corRsp;
      float rawRsp;
    } CALOJET_VAR;
    
    edm::Service<TFileService> fs;
    TTree *mTree;
    //---- TREE variables --------
    EVENT_VAR mEvent;
    PFJET_VAR mPFJets[20];
    CALOJET_VAR mCaloJets[20];
    static bool sort_pfjets(PFJET_VAR j1, PFJET_VAR j2) {
      return j1.corPt > j2.corPt;
    }
    static bool sort_calojets(CALOJET_VAR j1, CALOJET_VAR j2) {
      return j1.corPt > j2.corPt;
    }
};
#endif
