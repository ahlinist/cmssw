#ifndef ttHAnalysis_NtupleData_h
#define ttHAnalysis_NtupleData_h
#include <Ttypes.h>

class NtupleData {
 public:
  NtupleData();
  Int_t Event;
  Int_t Run;
  Float_t         TotalMuonX;
  Float_t         TotalMuonY;
  Float_t         TowerEtX;
  Float_t         TowerEtY;

  Int_t           NumbLep;
  Float_t         LeptonPx[50];
  Float_t         LeptonPy[50];
  Float_t         LeptonPz[50];
  Float_t         LeptonPtIso[50];
 
  Int_t           NumbJet;
  Float_t         JetE[50];
  Float_t         JetEt[50];
  Float_t         JetEta[50];
  Float_t         JetPhi[50];
  Float_t         JetTheta[50];
  Float_t         JetMass[50];
  Float_t         JetEcalFraction[50];
  Int_t           BJetFlag[50];
  Float_t         BDiscriminator[50];
  
};

#endif
