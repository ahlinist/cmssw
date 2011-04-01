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
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

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
    void initEvent();
    
    //---- configurable parameters --------  
    bool   mIsMCarlo;
    bool   mPreselection;
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
    //---- CORRECTORS ----------------------
    const JetCorrector *mPFJEC;
    const JetCorrector *mCALOJEC;
    JetCorrectionUncertainty *mCALOUnc;
    JetCorrectionUncertainty *mPFUnc;

    typedef struct {
      int   runNo;// run number
      int   evtNo;// event number
      int   lumi;// luminosity section
      int   bunch;// bunch crossing
      int   nVtx;// number of vertices
      int   nVtxGood;// number of good vertices
      int   isPVgood;// status flag for the primary vertex
      float PVndof;// number of degrees-of-freedom for the primary vertex
      float PVx;// primary vertex position in X
      float PVy;// primary vertex position in Y
      float PVz;// primary vertex position in Z
      float pfCorMass;// mass calculated from corrected PF jets
      float pfCorMassUp;// mass claculated from corrected PF jets shifted up by the JEC uncertainty
      float pfCorMassDo;// mass claculated from corrected PF jets shifted down by the JEC uncertainty
      float pfRawMass;// mass claculated from raw PF jets
      float caloCorMass;// mass claculated from corrected CALO jets
      float caloCorMassUp;// mass claculated from corrected CALO jets shifted up by the JEC uncertainty
      float caloCorMassDo;// mass claculated from corrected CALO jets shifted down by the JEC uncertainty
      float caloRawMass;// mass calculated from raw CALO jets
      float pfDeta;// delta eta between leading PFJets
      float caloDeta;// delta eta between leading CALO jets
      float pfYmax;// maximum absolute rapidity of the leading PFJets
      float caloYmax;// maximum absolute rapidity of the leading CALO jets
      float pfmet;// PF met
      float pfsumet;// PF sumEt
      float pfmet_over_sumet;// PF met/sumet
      float calomet;// CALO met
      float calosumet;// CALO sumEt
      float calomet_over_sumet;// CALO met/sumEt  
      float pthat;
      float weight;
    } EVENT_VAR;
    typedef struct {
      int   passLooseID;// ID status flag
      int   npr;// number of particles
      int   chm;// charged hadrons multiplicity
      int   nhm;// neutral hardons multiplicity
      int   phm;// photons multiplicity
      int   elm;// electrons multiplicity
      float jec;// jet energy corrections
      float jecUnc;// uncertainty of the corrected pt
      float rawPt;// raw pt
      float corPt;// corrected pt
      float eta;
      float y;
      float phi;
      float rawE;// raw energy
      float corE;// corrected energy
      float m;// jet mass  
      float chf;// charged hadron energy fraction
      float nhf;// neutral hadron energy fraction
      float phf;// photon energy fraction
      float elf;// electron energy fraction
      float genR;// matching radius between genjet and PF jet
      float genPt;// pt of matched genjet
      float genEta;// eta of matched genjet
      float genPhi;// phi of matched genjet
      float genE;// energy of matched genjet
      float genM;// mass of matched genket
      float corRsp;// corrected response
      float rawRsp;// raw response
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
