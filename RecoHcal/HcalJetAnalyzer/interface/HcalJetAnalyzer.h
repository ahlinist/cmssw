#ifndef HcalJetAnalyzer_H
#define HcalJetAnalyzer_H 1
#define TRIG_DT     2
#define TRIG_RPC    3
#define TRIG_RPCDT  4
#define TRIG_HB     5



// system include files
#include <memory>
#include <string>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
//#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
 
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Candidate/interface/Candidate.h"
//#include "DataFormats/HLTReco/interface/HLTFilterObject.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/L1Trigger/interface/L1ParticleMap.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/JetTracksAssociation.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
//#include "DataFormats/JetReco/interface/GenJetfwd.h"
//#include "DataFormats/JetReco/interface/GenJetCollection.h "
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
//#include "DataFormats/BTauReco/interface/JetTracksAssociation.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h" 

#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include "DataFormats/LTCDigi/interface/LTCDigi.h"

#include "CalibMuon/DTDigiSync/interface/DTTTrigBaseSync.h"
#include "CalibMuon/DTDigiSync/interface/DTTTrigSyncFactory.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "DataFormats/DTRecHit/interface/DTRecHitCollection.h"
#include "DataFormats/DTRecHit/interface/DTRangeMapAccessor.h"
#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoMuon/TrackingTools/interface/MuonPatternRecoDumper.h"

// this is to handle extrapolations
#include "DataFormats/GeometrySurface/interface/Cylinder.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"

// this is to retrieve DT digi's
#include "DataFormats/DTDigi/interface/DTDigiCollection.h"
#include "DataFormats/MuonDetId/interface/DTWireId.h"
#include "DataFormats/DTDigi/interface/DTLocalTriggerCollection.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment2DCollection.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
// this is to retrieve HCAL digi's
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h" 
#include "DataFormats/EcalDetId/interface/EBDetId.h" 
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalUncalibratedRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EBDataFrame.h"
#include "DataFormats/HcalDigi/interface/HcalQIESample.h"
//#include "RecoJets/JetAnalyzers/interface/MyCluster.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"

#include "TH2.h"
#include  "TProfile2D.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>
const int MAXGEN =  10;
const int MAXRPC =  20;
const int MAXDTBX = 20;
const int MAXCSC =  20;    
const int MAXGMT =  20;

	
class TFile;
class TTree;
class TH1F;
class TH2F;

class HcalJetAnalyzer : public edm::EDAnalyzer
{
public:
  //
  // construction/destruction
  //
  explicit HcalJetAnalyzer(const edm::ParameterSet&);
  ~HcalJetAnalyzer();


private:
  //
  // member functions
  //
//void  addBranches(TTree* tree_);
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
 
  
  // member data
  
     string m_calotowers;                                   
     string m_electronSrc;                                  
     string m_muonSrc;                                      
     string m_tracksSrc;                    
     string m_vertexSrc;                                                                                 
     string m_jetsSrc;                                                                                                   
     string m_jetsgenSrc;                                                                                                
     string m_tautaginfo;                                                                                                
     string m_photonSrc;                                                                                                 
     string m_calometSrc;
     string m_bjettag;  // for b-tagging
     string m_tautag;  // for tau-tagging
    /* string m_HBHEdigi; 
     string m_HFdigi ;  
     string m_HOdigi ;  
         
     */ 
     string m_HBHEreco;
     string m_HFreco ; 
     string m_HOreco ;  
     string m_GTdigi ;  
     string m_EcalRecHits;
    // string m_EcalRecHitsEE;
     
     
      DTTTrigBaseSync *theSync;
      int _local_event;
      std::string theSTAMuonLabel;
      std::string thePropagatorName;
      mutable Propagator* thePropagator;
      //////////////////////////////////////////////////////////////////////////////////////
      edm::ESHandle<HcalDbService> COND;
      HcalCalibrations calibs;
     
 
  std::string outputFileName_;
  TFile*      file_;
  TTree*      treedt_;
  TTree*      tree_;
  TTree*      treejcon_;
  
   int nmu;
   int nmu_;
   int dimuon;
   int numMuons;
   int nsay;
   int  nrh;
   float nCalotower; 
   float DRJetTwr ; 
   
   bool useRealOnly;
   bool Hcodedqie;
  
   float ana_muonEtaMax;
   float ana_muonPtMin1;
   float ana_muonPtMin2; 
   float invmass;
  

   float JetRecHFenergy[100];
    float JetRecHFieta[100];
    float JetRecHFiphi[100]; 

  float JetRecHBenergy[100]; 
  float JetRecHBieta[100];
  float JetRecHBiphi[100];
  float JetRecHEenergy[100];
  float JetRecHEieta[100];
  float JetRecHEiphi[100];
  
  float JetRecHOenergy[100];
  float JetRecHOieta[100];
  float JetRecHOiphi[100];
  
  float JetRecHBdep[100];
  float JetRecHEdep[100];
  float JetRecHOdep[100];
  float JetRecHFdep[100];
  

float RecoJetArea[100];  
float RecoJetmaxEemT[100];
float RecoJetmaxEHadT[100];
float RecoJetEnhadfrac[100];
float RecoJetEnemfrac[100]; 
float RecoJetEnHB[100];
float RecoJetEnHO[100];
float RecoJetEnHE[100]; 
float RecoJetEnEB[100];
float RecoJetEnEE[100];
float RecoJetEnHF[100];
float RecoJetTowerA[100];
float RecoJetn90[100];
 
  
float JetRecEBenergy[100];
float JetRecEBieta[100]; 
float JetRecEBiphi[100]; 
float JetRecEEenergy[100];
float JetRecEEieta[100];
float JetRecEEiphi[100];
  
  
  TProfile*   HcalDigiminuHBphi_;
  TProfile*   HcalDigiplusHBphi_;
  TProfile*   HcalDigiminuHBeta_;
  TProfile*   HcalDigiplusHBeta_;
  TProfile*   HcalRecminuHBphi_;
  TProfile*   HcalRecminuHBeta_;
  TProfile*   HcalRecplusHBphi_;
  TProfile*   HcalRecplusHBeta_ ;
  TProfile2D* HcalDigietaphi_;  
  TProfile2D* HcalDigiHEetaphi_;
  TProfile2D* HcalDigiHFetaphi_;
  TProfile2D* HcalDigiHOetaphi_;
  //TH2F* HcalDigisignal[50];
  TProfile2D* HBRecietaphi_; 
  TProfile*   ECALDigiminuEBphi_;
  TProfile*   ECALDigiminuEBeta_;
  TProfile*   ECALDigiplusEBphi_;
  TProfile*   ECALDigiplusEBeta_;
  TProfile2D* ECALEBDigietaphi_ ;
  TProfile2D* ECALEEDigietaphi_ ;
  
//TH2F* HcalRecminuHBphi_;
//TH2F* HcalRecplusHBphi_;
  TH1F* HBREChithis_; 
  TH1F* HEREChithis_ ;
  TH1F* HOREChithis_ ;
  TH1F* HFREChithis_ ;
   TH1F *_dimuon;
   TH1F* hjetcalotower;
   //cosmicmuon stuff
    
      //////////////////////////////////////////////////////////////////////////////////////
    
      TH1F  *dtAnglePhi;
      TH1F  *dtAngleEta;
      TH2F  *dthoEta;
      TH2F  *dthoPhi;
      TH1F  *dthoDEta;
      TH1F  *dthoDPhi;
      TH2F  *dthbEta;
      TH2F  *dthbPhi;
      TH1F  *dthbDEta;
      TH1F  *dthbDPhi;
      TH1F  *dthoEn;
      TH1F  *dthbEn;
      TH1F  *dthoRaw;
      TH1F  *dthbRaw;
      TH1F  *HBmuons; 

      //TProfile*   HcalDigiminuHBphi_;
      //TProfile*  dthbTime_;
      TH1F  *dthbTime;
      TH1F  *dthoTime;
      TH1F  *dthbTimeth;
      TH1F  *dthoTimeth;

      TH1F  *dtimpr;
      TH1F  *dtimpx;
      TH1F  *dtimpy;
      TH1F  *dtimpz;
    

      TH1F  *hNSA;    
      TH1F  *hNhitsSA;  
      TH1F  *hChi2SA;  
      TH1F  *hPIPSA;     
      TH1F  *hPtIPSA;
      TH1F  *hPhiIPSA;
      TH1F  *hEtaIPSA;
      TH1F  *EBREChithis_;
      TH1F  *EEREChithis_;
      TH2F  *HBclust;
      TH1F *obs1; 
      TH1F *obs2;
      TH1F *obs3; 
      TH1F *obs4;
      TH1F *obs5;
      TH1F *obs6;
      TH1F *obs7;
      TH1F *obs8;
      TH1F *obs9;
      TH1F *obs10; 
      TH1F *obs11;
      TH1F *obs12;
      TH1F *obs13;
      TH1F *obs14;
      TH1F *HFP_Rechits_long_fiber; 
      TH1F *HFM_Rechits_long_fiber; 
      TH1F *HFP_Rechits_short_fiber;
      TH1F *HFM_Rechits_short_fiber;
        int ico_;
        int icb_;
        int nevt_;
	int hcqie;
   int nJt;
   int nGJt;
   int ihbhit;
   int ihehit;
   int ihohit;
   int ihfhit;
   int iebhit;
   int ieehit;
   int calohit;
   short njt_;
   float  Track_Size;
      /* short ico_;
       short icb_;
       short nevt_;*/
      float enhophi_[4000];
      float enhoeta_[4000];
      float enhbphi_[4000];
      float enhbeta_[4000];

      float enhb_[4000];
      float enho_[4000];
      float menhb_[4000];
      float menho_[4000];
      float timehb_[4000];
      float timeho_[4000];
      float mtimehb_[4000];
      float mtimeho_[4000];
      
float  caloET[1000] ;
float  caloPT[1000] ;
float  caloE[1000] ;
float  caloPhi[1000]; 
float  caloEta[1000];
float  caloiphi[1000];
float  caloieta[1000]; 

float  RecoJetET[100] ;
float  RecoJetE[100] ;
float  RecoJetPhi[100]; 
float  RecoJetEta[100];
float  RecoJetiphi[100];
float  RecoJetieta[100]; 
float  RecoJetPT[100];
float  recoJetCaltower[100];
float  GENJetET[100];
float  GENJetPT[100];
float  GENJetE[100];
float  GENJetPhi[100];
float  GENJetEta[100];

float  RecHBPhi[4000];
float  RecHBEta[4000];
float  RecHBen[4000];
float  RecHEPhi[4000]; 
float  RecHEEta[4000];
float  RecHEen[4000];
float  RecHOPhi[4000];
float  RecHOEta[4000];
float  RecHOen[4000];
float  RecHFPhi[4000];
float  RecHFEta[4000];
float  RecHFen[4000];
float  RecHBdep[4000];
float  RecHEdep[4000];
float  RecHOdep[4000];
float  RecHFdep[4000];
float  RecEEenergy[20000];
float  RecEEieta[20000];
float  RecEEiphi[20000]; 
float  RecEBenergy[20000];
float  RecEBieta[20000];
float  RecEBiphi[20000];      
//float gtimehb;
float dt0_[4];
float dtho_[4];
float dthoO_[4];
float dthb_[4];
float dthbO_[4];
float gtimehb;
ofstream opt;
      
float EtMaxTower[100];
float sumEXdr[100]; 
float sumEtower[100];
float imp,hoz0,hoz1,hbz0,hbz1,hophi0,hophi1,hbphi0,hbphi1,enhbmuon; 
   
   
   
  /// histograms for reco efficiency
  TH1F* hjetent;
  TH1F* hjetentH;
  TH1F* hjetentD;
  TH1F* hjetpt ;
  TH1F* hjeteta;
  TH1F* hjetphi;
  TH1F* hjetmult;
  TH1F* hjetmultE;
  TH1F* hjetenergy;//
  TH1F* hGenJetEt; 
  TH1F* hGenJetPt; 
  TH1F* hGenJetEta;
  TH1F* hGenJetPhi;
  TH1F* hcalosize; // calotower
  TH1F* hcaloenergy;
  TH1F* hcaloenergyH;
  TH1F* hcaloenergyD;
  TH1F* hcaloent;	
  TH1F* hcalopt;	
  TH1F* hcaloeta;	
  TH1F* hcalophi;
  TH1F* EBRECtime_;
  TH1F* EERECtime_;
  TH1F* HBRECtime_;
  TH1F* HERECtime_;
  TH1F* HFRECtime_;
  TH1F* HORECtime_;
  /////////


};


#endif
