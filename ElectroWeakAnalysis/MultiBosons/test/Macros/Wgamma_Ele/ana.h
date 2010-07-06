//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Jun  6 14:55:04 2010 by ROOT version 5.22/00d
// from TTree EventTree/Event data
// found on file: 35x_Wgamma_Pythia.root
//////////////////////////////////////////////////////////

#ifndef ana_h
#define ana_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class ana {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           run;
   Int_t           event;
   Int_t           orbit;
   Int_t           bx;
   Int_t           lumis;
   Bool_t          isData;
   Int_t           ttbit0;
   Int_t           ttbit34;
   Int_t           ttbit40;
   Int_t           ttbit41;
   Int_t           ttbitBH;
   Int_t           nHLT;
   Int_t           HLT[144];   //[nHLT]
   Int_t           nHFTowersP;
   Int_t           nHFTowersN;
   Int_t           nVtx;
   Float_t         vtx[2][3];   //[nVtx]
   Int_t           vtxNTrk[2];   //[nVtx]
   Int_t           vtxNDF[2];   //[nVtx]
   Float_t         vtxD0[2];   //[nVtx]
   Int_t           IsVtxGood;
   Int_t           nTrk;
   Int_t           nGoodTrk;
   Int_t           IsTracksGood;
   Float_t         pdf[7];
   Float_t         processID;
   Int_t           nMC;
   Int_t           mcPID[19];   //[nMC]
   Float_t         mcPt[19];   //[nMC]
   Float_t         mcMass[19];   //[nMC]
   Float_t         mcEta[19];   //[nMC]
   Float_t         mcPhi[19];   //[nMC]
   Int_t           mcGMomPID[19];   //[nMC]
   Int_t           mcMomPID[19];   //[nMC]
   Float_t         mcMomPt[19];   //[nMC]
   Float_t         mcMomMass[19];   //[nMC]
   Float_t         mcMomEta[19];   //[nMC]
   Float_t         mcMomPhi[19];   //[nMC]
   Int_t           mcIndex[19];   //[nMC]
   Int_t           mcDecayType[19];   //[nMC]
   Float_t         genMET;
   Float_t         genMETx;
   Float_t         genMETy;
   Float_t         genMETPhi;
   Float_t         MET;
   Float_t         METx;
   Float_t         METy;
   Float_t         METPhi;
   Float_t         METsumEt;
   Float_t         uncorrMET[3];
   Float_t         uncorrMETPhi[3];
   Float_t         uncorrMETSumEt[3];
   Float_t         tcMET;
   Float_t         tcMETx;
   Float_t         tcMETy;
   Float_t         tcMETPhi;
   Float_t         tcMETsumEt;
   Float_t         tcMETmEtSig;
   Float_t         tcMETSig;
   Float_t         pfMET;
   Float_t         pfMETx;
   Float_t         pfMETy;
   Float_t         pfMETPhi;
   Float_t         pfMETsumEt;
   Float_t         pfMETmEtSig;
   Float_t         pfMETSig;
   Int_t           nEle;
   Int_t           eleID[50][12];   //[nEle]
   Int_t           eleClass[50];   //[nEle]
   Int_t           eleCharge[50];   //[nEle]
   Float_t         eleEn[50];   //[nEle]
   Float_t         eleSCRawEn[50];   //[nEle]
   Float_t         eleESEn[50];   //[nEle]
   Float_t         eleSCEn[50];   //[nEle]
   Float_t         elePt[50];   //[nEle]
   Float_t         elePz[50];   //[nEle]
   Float_t         eleEta[50];   //[nEle]
   Float_t         elePhi[50];   //[nEle]
   Float_t         eleSCEta[50];   //[nEle]
   Float_t         eleSCPhi[50];   //[nEle]
   Float_t         eleSCEtaWidth[50];   //[nEle]
   Float_t         eleSCPhiWidth[50];   //[nEle]
   Float_t         eleVtx[50][3];   //[nEle]
   Float_t         eleCaloPos[50][3];   //[nEle]
   Float_t         eleSCPos[50][3];   //[nEle]
   Float_t         eleHoverE[50];   //[nEle]
   Float_t         eleEoverP[50];   //[nEle]
   Float_t         elePin[50];   //[nEle]
   Float_t         elePout[50];   //[nEle]
   Float_t         eleBrem[50];   //[nEle]
   Float_t         eledEtaAtVtx[50];   //[nEle]
   Float_t         eledPhiAtVtx[50];   //[nEle]
   Float_t         eleSigmaEtaEta[50];   //[nEle]
   Float_t         eleSigmaIEtaIEta[50];   //[nEle]
   Float_t         eleEMax[50];   //[nEle]
   Float_t         eleE3x3[50];   //[nEle]
   Float_t         eleERight[50];   //[nEle]
   Float_t         eleELeft[50];   //[nEle]
   Float_t         eleETop[50];   //[nEle]
   Float_t         eleEBottom[50];   //[nEle]
   Int_t           eleGenIndex[50];   //[nEle]
   Int_t           eleGenGMomPID[50];   //[nEle]
   Int_t           eleGenMomPID[50];   //[nEle]
   Float_t         eleGenMomPt[50];   //[nEle]
   Float_t         eleIsoTrkDR03[50];   //[nEle]
   Float_t         eleIsoEcalDR03[50];   //[nEle]
   Float_t         eleIsoHcalDR03[50];   //[nEle]
   Float_t         eleIsoTrkDR04[50];   //[nEle]
   Float_t         eleIsoEcalDR04[50];   //[nEle]
   Float_t         eleIsoHcalDR04[50];   //[nEle]
   Float_t         eleChi2NDF[50];   //[nEle]
   Float_t         eleD0[50];   //[nEle]
   Int_t           eleNumberOfValidHits[50];   //[nEle]
   Int_t           eleValidHitInFirstPXB[50];   //[nEle]
   Int_t           eleTrkExpectHitsInner[50];   //[nEle]
   Int_t           nPho;
   Bool_t          phoIsPhoton[100];   //[nPho]
   Float_t         phoE[100];   //[nPho]
   Float_t         phoEt[100];   //[nPho]
   Float_t         phoPz[100];   //[nPho]
   Float_t         phoEta[100];   //[nPho]
   Float_t         phoPhi[100];   //[nPho]
   Float_t         phoR9[100];   //[nPho]
   Float_t         phoTrkIsoSolidDR03[100];   //[nPho]
   Float_t         phoTrkIsoHollowDR03[100];   //[nPho]
   Int_t           phoNTrkSolidDR03[100];   //[nPho]
   Int_t           phoNTrkHollowDR03[100];   //[nPho]
   Float_t         phoEcalIsoDR03[100];   //[nPho]
   Float_t         phoHcalIsoDR03[100];   //[nPho]
   Float_t         phoTrkIsoSolidDR04[100];   //[nPho]
   Float_t         phoTrkIsoHollowDR04[100];   //[nPho]
   Int_t           phoNTrkSolidDR04[100];   //[nPho]
   Int_t           phoNTrkHollowDR04[100];   //[nPho]
   Float_t         phoEcalIsoDR04[100];   //[nPho]
   Float_t         phoHcalIsoDR04[100];   //[nPho]
   Float_t         phoHoverE[100];   //[nPho]
   Float_t         phoSigmaEtaEta[100];   //[nPho]
   Float_t         phoSigmaIEtaIEta[100];   //[nPho]
   Float_t         phoSeedTime[100];   //[nPho]
   Int_t           phoPos[100];   //[nPho]
   Float_t         phoEMax[100];   //[nPho]
   Float_t         phoE3x3[100];   //[nPho]
   Float_t         phoERight[100];   //[nPho]
   Float_t         phoELeft[100];   //[nPho]
   Float_t         phoETop[100];   //[nPho]
   Float_t         phoEBottom[100];   //[nPho]
   Float_t         phoRoundness[100];   //[nPho]
   Float_t         phoAngle[100];   //[nPho]
   Int_t           phoGenIndex[100];   //[nPho]
   Int_t           phoGenGMomPID[100];   //[nPho]
   Int_t           phoGenMomPID[100];   //[nPho]
   Float_t         phoGenMomPt[100];   //[nPho]
   Float_t         phoSCE[100];   //[nPho]
   Float_t         phoSCEt[100];   //[nPho]
   Float_t         phoSCEta[100];   //[nPho]
   Float_t         phoSCPhi[100];   //[nPho]
   Float_t         phoSCEtaWidth[100];   //[nPho]
   Float_t         phoSCPhiWidth[100];   //[nPho]
   Int_t           phoOverlap[100];   //[nPho]
   Int_t           phohasPixelSeed[100];   //[nPho]
   Int_t           nMu;
   Float_t         muEta[20];   //[nMu]
   Float_t         muPhi[20];   //[nMu]
   Int_t           muCharge[20];   //[nMu]
   Float_t         muPt[20];   //[nMu]
   Float_t         muPz[20];   //[nMu]
   Int_t           muGenIndex[20];   //[nMu]
   Float_t         muIsoTrk[20];   //[nMu]
   Float_t         muIsoCalo[20];   //[nMu]
   Float_t         muIsoEcal[20];   //[nMu]
   Float_t         muIsoHcal[20];   //[nMu]
   Float_t         muChi2NDF[20];   //[nMu]
   Float_t         muEmVeto[20];   //[nMu]
   Float_t         muHadVeto[20];   //[nMu]
   Int_t           muType[20];   //[nMu]
   Bool_t          muID[20][6];   //[nMu]
   Float_t         muD0[20];   //[nMu]
   Int_t           muNumberOfValidTrkHits[20];   //[nMu]
   Int_t           muNumberOfValidPixelHits[20];   //[nMu]
   Int_t           muNumberOfValidMuonHits[20];   //[nMu]
   Int_t           nZee;
   Float_t         ZeeMass[50];   //[nZee]
   Float_t         ZeePt[50];   //[nZee]
   Float_t         ZeeEta[50];   //[nZee]
   Float_t         ZeePhi[50];   //[nZee]
   Int_t           ZeeLeg1Index[50];   //[nZee]
   Int_t           ZeeLeg2Index[50];   //[nZee]
   Int_t           nZmumu;
   Float_t         ZmumuMass[50];   //[nZmumu]
   Float_t         ZmumuPt[50];   //[nZmumu]
   Float_t         ZmumuEta[50];   //[nZmumu]
   Float_t         ZmumuPhi[50];   //[nZmumu]
   Int_t           ZmumuLeg1Index[50];   //[nZmumu]
   Int_t           ZmumuLeg2Index[50];   //[nZmumu]
   Int_t           nWenu;
   Float_t         WenuMassTCaloMET[150];   //[nWenu]
   Float_t         WenuEtCaloMET[150];   //[nWenu]
   Float_t         WenuACopCaloMET[150];   //[nWenu]
   Float_t         WenuMassTTcMET[150];   //[nWenu]
   Float_t         WenuEtTcMET[150];   //[nWenu]
   Float_t         WenuACopTcMET[150];   //[nWenu]
   Float_t         WenuMassTPfMET[150];   //[nWenu]
   Float_t         WenuEtPfMET[150];   //[nWenu]
   Float_t         WenuACopPfMET[150];   //[nWenu]
   Int_t           WenuEleIndex[150];   //[nWenu]
   Int_t           nWmunu;
   Float_t         WmunuMassTCaloMET[150];   //[nWmunu]
   Float_t         WmunuEtCaloMET[150];   //[nWmunu]
   Float_t         WmunuACopCaloMET[150];   //[nWmunu]
   Float_t         WmunuMassTTcMET[150];   //[nWmunu]
   Float_t         WmunuEtTcMET[150];   //[nWmunu]
   Float_t         WmunuACopTcMET[150];   //[nWmunu]
   Float_t         WmunuMassTPfMET[150];   //[nWmunu]
   Float_t         WmunuEtPfMET[150];   //[nWmunu]
   Float_t         WmunuACopPfMET[150];   //[nWmunu]
   Int_t           WmunuMuIndex[150];   //[nWmunu]

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_event;   //!
   TBranch        *b_orbit;   //!
   TBranch        *b_bx;   //!
   TBranch        *b_lumis;   //!
   TBranch        *b_isData;   //!
   TBranch        *b_ttbit0;   //!
   TBranch        *b_ttbit34;   //!
   TBranch        *b_ttbit40;   //!
   TBranch        *b_ttbit41;   //!
   TBranch        *b_ttbitBH;   //!
   TBranch        *b_nHLT;   //!
   TBranch        *b_HLT;   //!
   TBranch        *b_nHFTowersP;   //!
   TBranch        *b_nHFTowersN;   //!
   TBranch        *b_nVtx;   //!
   TBranch        *b_vtx;   //!
   TBranch        *b_vtxNTrk;   //!
   TBranch        *b_vtxNDF;   //!
   TBranch        *b_vtxD0;   //!
   TBranch        *b_IsVtxGood;   //!
   TBranch        *b_nTrk;   //!
   TBranch        *b_nGoodTrk;   //!
   TBranch        *b_IsTracksGood;   //!
   TBranch        *b_pdf;   //!
   TBranch        *b_processID;   //!
   TBranch        *b_nMC;   //!
   TBranch        *b_mcPID;   //!
   TBranch        *b_mcPt;   //!
   TBranch        *b_mcMass;   //!
   TBranch        *b_mcEta;   //!
   TBranch        *b_mcPhi;   //!
   TBranch        *b_mcGMomPID;   //!
   TBranch        *b_mcMomPID;   //!
   TBranch        *b_mcMomPt;   //!
   TBranch        *b_mcMomMass;   //!
   TBranch        *b_mcMomEta;   //!
   TBranch        *b_mcMomPhi;   //!
   TBranch        *b_mcIndex;   //!
   TBranch        *b_mcDecayType;   //!
   TBranch        *b_genMET;   //!
   TBranch        *b_genMETx;   //!
   TBranch        *b_genMETy;   //!
   TBranch        *b_genMETPhi;   //!
   TBranch        *b_MET;   //!
   TBranch        *b_METx;   //!
   TBranch        *b_METy;   //!
   TBranch        *b_METPhi;   //!
   TBranch        *b_METsumEt;   //!
   TBranch        *b_uncorrMET;   //!
   TBranch        *b_uncorrMETPhi;   //!
   TBranch        *b_uncorrMETSumEt;   //!
   TBranch        *b_tcMET;   //!
   TBranch        *b_tcMETx;   //!
   TBranch        *b_tcMETy;   //!
   TBranch        *b_tcMETPhi;   //!
   TBranch        *b_tcMETsumEt;   //!
   TBranch        *b_tcMETmEtSig;   //!
   TBranch        *b_tcMETSig;   //!
   TBranch        *b_pfMET;   //!
   TBranch        *b_pfMETx;   //!
   TBranch        *b_pfMETy;   //!
   TBranch        *b_pfMETPhi;   //!
   TBranch        *b_pfMETsumEt;   //!
   TBranch        *b_pfMETmEtSig;   //!
   TBranch        *b_pfMETSig;   //!
   TBranch        *b_nEle;   //!
   TBranch        *b_eleID;   //!
   TBranch        *b_eleClass;   //!
   TBranch        *b_eleCharge;   //!
   TBranch        *b_eleEn;   //!
   TBranch        *b_eleSCRawEn;   //!
   TBranch        *b_eleESEn;   //!
   TBranch        *b_eleSCEn;   //!
   TBranch        *b_elePt;   //!
   TBranch        *b_elePz;   //!
   TBranch        *b_eleEta;   //!
   TBranch        *b_elePhi;   //!
   TBranch        *b_eleSCEta;   //!
   TBranch        *b_eleSCPhi;   //!
   TBranch        *b_eleSCEtaWidth;   //!
   TBranch        *b_eleSCPhiWidth;   //!
   TBranch        *b_eleVtx;   //!
   TBranch        *b_eleCaloPos;   //!
   TBranch        *b_eleSCPos;   //!
   TBranch        *b_eleHoverE;   //!
   TBranch        *b_eleEoverP;   //!
   TBranch        *b_elePin;   //!
   TBranch        *b_elePout;   //!
   TBranch        *b_eleBrem;   //!
   TBranch        *b_eledEtaAtVtx;   //!
   TBranch        *b_eledPhiAtVtx;   //!
   TBranch        *b_eleSigmaEtaEta;   //!
   TBranch        *b_eleSigmaIEtaIEta;   //!
   TBranch        *b_eleEMax;   //!
   TBranch        *b_eleE3x3;   //!
   TBranch        *b_eleERight;   //!
   TBranch        *b_eleELeft;   //!
   TBranch        *b_eleETop;   //!
   TBranch        *b_eleEBottom;   //!
   TBranch        *b_eleGenIndex;   //!
   TBranch        *b_eleGenGMomPID;   //!
   TBranch        *b_eleGenMomPID;   //!
   TBranch        *b_eleGenMomPt;   //!
   TBranch        *b_eleIsoTrkDR03;   //!
   TBranch        *b_eleIsoEcalDR03;   //!
   TBranch        *b_eleIsoHcalDR03;   //!
   TBranch        *b_eleIsoTrkDR04;   //!
   TBranch        *b_eleIsoEcalDR04;   //!
   TBranch        *b_eleIsoHcalDR04;   //!
   TBranch        *b_eleChi2NDF;   //!
   TBranch        *b_eleD0;   //!
   TBranch        *b_eleNumberOfValidHits;   //!
   TBranch        *b_eleValidHitInFirstPXB;   //!
   TBranch        *b_eleTrkExpectHitsInner;   //!
   TBranch        *b_nPho;   //!
   TBranch        *b_phoIsPhoton;   //!
   TBranch        *b_phoE;   //!
   TBranch        *b_phoEt;   //!
   TBranch        *b_phoPz;   //!
   TBranch        *b_phoEta;   //!
   TBranch        *b_phoPhi;   //!
   TBranch        *b_phoR9;   //!
   TBranch        *b_phoTrkIsoSolidDR03;   //!
   TBranch        *b_phoTrkIsoHollowDR03;   //!
   TBranch        *b_phoNTrkSolidDR03;   //!
   TBranch        *b_phoNTrkHollowDR03;   //!
   TBranch        *b_phoEcalIsoDR03;   //!
   TBranch        *b_phoHcalIsoDR03;   //!
   TBranch        *b_phoTrkIsoSolidDR04;   //!
   TBranch        *b_phoTrkIsoHollowDR04;   //!
   TBranch        *b_phoNTrkSolidDR04;   //!
   TBranch        *b_phoNTrkHollowDR04;   //!
   TBranch        *b_phoEcalIsoDR04;   //!
   TBranch        *b_phoHcalIsoDR04;   //!
   TBranch        *b_phoHoverE;   //!
   TBranch        *b_phoSigmaEtaEta;   //!
   TBranch        *b_phoSigmaIEtaIEta;   //!
   TBranch        *b_phoSeedTime;   //!
   TBranch        *b_phoPos;   //!
   TBranch        *b_phoEMax;   //!
   TBranch        *b_phoE3x3;   //!
   TBranch        *b_phoERight;   //!
   TBranch        *b_phoELeft;   //!
   TBranch        *b_phoETop;   //!
   TBranch        *b_phoEBottom;   //!
   TBranch        *b_phoRoundness;   //!
   TBranch        *b_phoAngle;   //!
   TBranch        *b_phoGenIndex;   //!
   TBranch        *b_phoGenGMomPID;   //!
   TBranch        *b_phoGenMomPID;   //!
   TBranch        *b_phoGenMomPt;   //!
   TBranch        *b_phoSCE;   //!
   TBranch        *b_phoSCEt;   //!
   TBranch        *b_phoSCEta;   //!
   TBranch        *b_phoSCPhi;   //!
   TBranch        *b_phoSCEtaWidth;   //!
   TBranch        *b_phoSCPhiWidth;   //!
   TBranch        *b_phoOverlap;   //!
   TBranch        *b_phohasPixelSeed;   //!
   TBranch        *b_nMu;   //!
   TBranch        *b_muEta;   //!
   TBranch        *b_muPhi;   //!
   TBranch        *b_muCharge;   //!
   TBranch        *b_muPt;   //!
   TBranch        *b_muPz;   //!
   TBranch        *b_muGenIndex;   //!
   TBranch        *b_muIsoTrk;   //!
   TBranch        *b_muIsoCalo;   //!
   TBranch        *b_muIsoEcal;   //!
   TBranch        *b_muIsoHcal;   //!
   TBranch        *b_muChi2NDF;   //!
   TBranch        *b_muEmVeto;   //!
   TBranch        *b_muHadVeto;   //!
   TBranch        *b_muType;   //!
   TBranch        *b_muID;   //!
   TBranch        *b_muD0;   //!
   TBranch        *b_muNumberOfValidTrkHits;   //!
   TBranch        *b_muNumberOfValidPixelHits;   //!
   TBranch        *b_muNumberOfValidMuonHits;   //!
   TBranch        *b_nZee;   //!
   TBranch        *b_ZeeMass;   //!
   TBranch        *b_ZeePt;   //!
   TBranch        *b_ZeeEta;   //!
   TBranch        *b_ZeePhi;   //!
   TBranch        *b_ZeeLeg1Index;   //!
   TBranch        *b_ZeeLeg2Index;   //!
   TBranch        *b_nZmumu;   //!
   TBranch        *b_ZmumuMass;   //!
   TBranch        *b_ZmumuPt;   //!
   TBranch        *b_ZmumuEta;   //!
   TBranch        *b_ZmumuPhi;   //!
   TBranch        *b_ZmumuLeg1Index;   //!
   TBranch        *b_ZmumuLeg2Index;   //!
   TBranch        *b_nWenu;   //!
   TBranch        *b_WenuMassTCaloMET;   //!
   TBranch        *b_WenuEtCaloMET;   //!
   TBranch        *b_WenuACopCaloMET;   //!
   TBranch        *b_WenuMassTTcMET;   //!
   TBranch        *b_WenuEtTcMET;   //!
   TBranch        *b_WenuACopTcMET;   //!
   TBranch        *b_WenuMassTPfMET;   //!
   TBranch        *b_WenuEtPfMET;   //!
   TBranch        *b_WenuACopPfMET;   //!
   TBranch        *b_WenuEleIndex;   //!
   TBranch        *b_nWmunu;   //!
   TBranch        *b_WmunuMassTCaloMET;   //!
   TBranch        *b_WmunuEtCaloMET;   //!
   TBranch        *b_WmunuACopCaloMET;   //!
   TBranch        *b_WmunuMassTTcMET;   //!
   TBranch        *b_WmunuEtTcMET;   //!
   TBranch        *b_WmunuACopTcMET;   //!
   TBranch        *b_WmunuMassTPfMET;   //!
   TBranch        *b_WmunuEtPfMET;   //!
   TBranch        *b_WmunuACopPfMET;   //!
   TBranch        *b_WmunuMuIndex;   //!

   ana(TTree *tree=0);
   virtual ~ana();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef ana_cxx
ana::ana(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   TChain *Chain = new TChain("VgAnalyzerKit/EventTree","");
   // Input files
   FILE *Table = fopen("Input_table", "r");
   char filename[200];
   char tmp[100];
   int flag = 1;
   while (flag != -1){
      flag=fscanf(Table, "%s", filename);
      if (flag == -1) continue;
      flag=fscanf(Table, "%s", tmp);
      double cross=atof(tmp);
      flag=fscanf(Table, "%s", tmp);
      double evt=atof(tmp);
      flag=fscanf(Table, "%s", tmp);
      double skim_evt=atof(tmp);

      Chain->Add(filename);
   }
   Init(Chain);
}

ana::~ana()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t ana::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t ana::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void ana::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("orbit", &orbit, &b_orbit);
   fChain->SetBranchAddress("bx", &bx, &b_bx);
   fChain->SetBranchAddress("lumis", &lumis, &b_lumis);
   fChain->SetBranchAddress("isData", &isData, &b_isData);
   fChain->SetBranchAddress("ttbit0", &ttbit0, &b_ttbit0);
   fChain->SetBranchAddress("ttbit34", &ttbit34, &b_ttbit34);
   fChain->SetBranchAddress("ttbit40", &ttbit40, &b_ttbit40);
   fChain->SetBranchAddress("ttbit41", &ttbit41, &b_ttbit41);
   fChain->SetBranchAddress("ttbitBH", &ttbitBH, &b_ttbitBH);
   fChain->SetBranchAddress("nHLT", &nHLT, &b_nHLT);
   fChain->SetBranchAddress("HLT", HLT, &b_HLT);
   fChain->SetBranchAddress("nHFTowersP", &nHFTowersP, &b_nHFTowersP);
   fChain->SetBranchAddress("nHFTowersN", &nHFTowersN, &b_nHFTowersN);
   fChain->SetBranchAddress("nVtx", &nVtx, &b_nVtx);
   fChain->SetBranchAddress("vtx", vtx, &b_vtx);
   fChain->SetBranchAddress("vtxNTrk", vtxNTrk, &b_vtxNTrk);
   fChain->SetBranchAddress("vtxNDF", vtxNDF, &b_vtxNDF);
   fChain->SetBranchAddress("vtxD0", vtxD0, &b_vtxD0);
   fChain->SetBranchAddress("IsVtxGood", &IsVtxGood, &b_IsVtxGood);
   fChain->SetBranchAddress("nTrk", &nTrk, &b_nTrk);
   fChain->SetBranchAddress("nGoodTrk", &nGoodTrk, &b_nGoodTrk);
   fChain->SetBranchAddress("IsTracksGood", &IsTracksGood, &b_IsTracksGood);
   fChain->SetBranchAddress("pdf", pdf, &b_pdf);
   fChain->SetBranchAddress("processID", &processID, &b_processID);
   fChain->SetBranchAddress("nMC", &nMC, &b_nMC);
   fChain->SetBranchAddress("mcPID", mcPID, &b_mcPID);
   fChain->SetBranchAddress("mcPt", mcPt, &b_mcPt);
   fChain->SetBranchAddress("mcMass", mcMass, &b_mcMass);
   fChain->SetBranchAddress("mcEta", mcEta, &b_mcEta);
   fChain->SetBranchAddress("mcPhi", mcPhi, &b_mcPhi);
   fChain->SetBranchAddress("mcGMomPID", mcGMomPID, &b_mcGMomPID);
   fChain->SetBranchAddress("mcMomPID", mcMomPID, &b_mcMomPID);
   fChain->SetBranchAddress("mcMomPt", mcMomPt, &b_mcMomPt);
   fChain->SetBranchAddress("mcMomMass", mcMomMass, &b_mcMomMass);
   fChain->SetBranchAddress("mcMomEta", mcMomEta, &b_mcMomEta);
   fChain->SetBranchAddress("mcMomPhi", mcMomPhi, &b_mcMomPhi);
   fChain->SetBranchAddress("mcIndex", mcIndex, &b_mcIndex);
   fChain->SetBranchAddress("mcDecayType", mcDecayType, &b_mcDecayType);
   fChain->SetBranchAddress("genMET", &genMET, &b_genMET);
   fChain->SetBranchAddress("genMETx", &genMETx, &b_genMETx);
   fChain->SetBranchAddress("genMETy", &genMETy, &b_genMETy);
   fChain->SetBranchAddress("genMETPhi", &genMETPhi, &b_genMETPhi);
   fChain->SetBranchAddress("MET", &MET, &b_MET);
   fChain->SetBranchAddress("METx", &METx, &b_METx);
   fChain->SetBranchAddress("METy", &METy, &b_METy);
   fChain->SetBranchAddress("METPhi", &METPhi, &b_METPhi);
   fChain->SetBranchAddress("METsumEt", &METsumEt, &b_METsumEt);
   fChain->SetBranchAddress("uncorrMET", uncorrMET, &b_uncorrMET);
   fChain->SetBranchAddress("uncorrMETPhi", uncorrMETPhi, &b_uncorrMETPhi);
   fChain->SetBranchAddress("uncorrMETSumEt", uncorrMETSumEt, &b_uncorrMETSumEt);
   fChain->SetBranchAddress("tcMET", &tcMET, &b_tcMET);
   fChain->SetBranchAddress("tcMETx", &tcMETx, &b_tcMETx);
   fChain->SetBranchAddress("tcMETy", &tcMETy, &b_tcMETy);
   fChain->SetBranchAddress("tcMETPhi", &tcMETPhi, &b_tcMETPhi);
   fChain->SetBranchAddress("tcMETsumEt", &tcMETsumEt, &b_tcMETsumEt);
   fChain->SetBranchAddress("tcMETmEtSig", &tcMETmEtSig, &b_tcMETmEtSig);
   fChain->SetBranchAddress("tcMETSig", &tcMETSig, &b_tcMETSig);
   fChain->SetBranchAddress("pfMET", &pfMET, &b_pfMET);
   fChain->SetBranchAddress("pfMETx", &pfMETx, &b_pfMETx);
   fChain->SetBranchAddress("pfMETy", &pfMETy, &b_pfMETy);
   fChain->SetBranchAddress("pfMETPhi", &pfMETPhi, &b_pfMETPhi);
   fChain->SetBranchAddress("pfMETsumEt", &pfMETsumEt, &b_pfMETsumEt);
   fChain->SetBranchAddress("pfMETmEtSig", &pfMETmEtSig, &b_pfMETmEtSig);
   fChain->SetBranchAddress("pfMETSig", &pfMETSig, &b_pfMETSig);
   fChain->SetBranchAddress("nEle", &nEle, &b_nEle);
   fChain->SetBranchAddress("eleID", eleID, &b_eleID);
   fChain->SetBranchAddress("eleClass", eleClass, &b_eleClass);
   fChain->SetBranchAddress("eleCharge", eleCharge, &b_eleCharge);
   fChain->SetBranchAddress("eleEn", eleEn, &b_eleEn);
   fChain->SetBranchAddress("eleSCRawEn", eleSCRawEn, &b_eleSCRawEn);
   fChain->SetBranchAddress("eleESEn", eleESEn, &b_eleESEn);
   fChain->SetBranchAddress("eleSCEn", eleSCEn, &b_eleSCEn);
   fChain->SetBranchAddress("elePt", elePt, &b_elePt);
   fChain->SetBranchAddress("elePz", elePz, &b_elePz);
   fChain->SetBranchAddress("eleEta", eleEta, &b_eleEta);
   fChain->SetBranchAddress("elePhi", elePhi, &b_elePhi);
   fChain->SetBranchAddress("eleSCEta", eleSCEta, &b_eleSCEta);
   fChain->SetBranchAddress("eleSCPhi", eleSCPhi, &b_eleSCPhi);
   fChain->SetBranchAddress("eleSCEtaWidth", eleSCEtaWidth, &b_eleSCEtaWidth);
   fChain->SetBranchAddress("eleSCPhiWidth", eleSCPhiWidth, &b_eleSCPhiWidth);
   fChain->SetBranchAddress("eleVtx", eleVtx, &b_eleVtx);
   fChain->SetBranchAddress("eleCaloPos", eleCaloPos, &b_eleCaloPos);
   fChain->SetBranchAddress("eleSCPos", eleSCPos, &b_eleSCPos);
   fChain->SetBranchAddress("eleHoverE", eleHoverE, &b_eleHoverE);
   fChain->SetBranchAddress("eleEoverP", eleEoverP, &b_eleEoverP);
   fChain->SetBranchAddress("elePin", elePin, &b_elePin);
   fChain->SetBranchAddress("elePout", elePout, &b_elePout);
   fChain->SetBranchAddress("eleBrem", eleBrem, &b_eleBrem);
   fChain->SetBranchAddress("eledEtaAtVtx", eledEtaAtVtx, &b_eledEtaAtVtx);
   fChain->SetBranchAddress("eledPhiAtVtx", eledPhiAtVtx, &b_eledPhiAtVtx);
   fChain->SetBranchAddress("eleSigmaEtaEta", eleSigmaEtaEta, &b_eleSigmaEtaEta);
   fChain->SetBranchAddress("eleSigmaIEtaIEta", eleSigmaIEtaIEta, &b_eleSigmaIEtaIEta);
   fChain->SetBranchAddress("eleEMax", eleEMax, &b_eleEMax);
   fChain->SetBranchAddress("eleE3x3", eleE3x3, &b_eleE3x3);
   fChain->SetBranchAddress("eleERight", eleERight, &b_eleERight);
   fChain->SetBranchAddress("eleELeft", eleELeft, &b_eleELeft);
   fChain->SetBranchAddress("eleETop", eleETop, &b_eleETop);
   fChain->SetBranchAddress("eleEBottom", eleEBottom, &b_eleEBottom);
   fChain->SetBranchAddress("eleGenIndex", eleGenIndex, &b_eleGenIndex);
   fChain->SetBranchAddress("eleGenGMomPID", eleGenGMomPID, &b_eleGenGMomPID);
   fChain->SetBranchAddress("eleGenMomPID", eleGenMomPID, &b_eleGenMomPID);
   fChain->SetBranchAddress("eleGenMomPt", eleGenMomPt, &b_eleGenMomPt);
   fChain->SetBranchAddress("eleIsoTrkDR03", eleIsoTrkDR03, &b_eleIsoTrkDR03);
   fChain->SetBranchAddress("eleIsoEcalDR03", eleIsoEcalDR03, &b_eleIsoEcalDR03);
   fChain->SetBranchAddress("eleIsoHcalDR03", eleIsoHcalDR03, &b_eleIsoHcalDR03);
   fChain->SetBranchAddress("eleIsoTrkDR04", eleIsoTrkDR04, &b_eleIsoTrkDR04);
   fChain->SetBranchAddress("eleIsoEcalDR04", eleIsoEcalDR04, &b_eleIsoEcalDR04);
   fChain->SetBranchAddress("eleIsoHcalDR04", eleIsoHcalDR04, &b_eleIsoHcalDR04);
   fChain->SetBranchAddress("eleChi2NDF", eleChi2NDF, &b_eleChi2NDF);
   fChain->SetBranchAddress("eleD0", eleD0, &b_eleD0);
   fChain->SetBranchAddress("eleNumberOfValidHits", eleNumberOfValidHits, &b_eleNumberOfValidHits);
   fChain->SetBranchAddress("eleValidHitInFirstPXB", eleValidHitInFirstPXB, &b_eleValidHitInFirstPXB);
   fChain->SetBranchAddress("eleTrkExpectHitsInner", eleTrkExpectHitsInner, &b_eleTrkExpectHitsInner);
   fChain->SetBranchAddress("nPho", &nPho, &b_nPho);
   fChain->SetBranchAddress("phoIsPhoton", phoIsPhoton, &b_phoIsPhoton);
   fChain->SetBranchAddress("phoE", phoE, &b_phoE);
   fChain->SetBranchAddress("phoEt", phoEt, &b_phoEt);
   fChain->SetBranchAddress("phoPz", phoPz, &b_phoPz);
   fChain->SetBranchAddress("phoEta", phoEta, &b_phoEta);
   fChain->SetBranchAddress("phoPhi", phoPhi, &b_phoPhi);
   fChain->SetBranchAddress("phoR9", phoR9, &b_phoR9);
   fChain->SetBranchAddress("phoTrkIsoSolidDR03", phoTrkIsoSolidDR03, &b_phoTrkIsoSolidDR03);
   fChain->SetBranchAddress("phoTrkIsoHollowDR03", phoTrkIsoHollowDR03, &b_phoTrkIsoHollowDR03);
   fChain->SetBranchAddress("phoNTrkSolidDR03", phoNTrkSolidDR03, &b_phoNTrkSolidDR03);
   fChain->SetBranchAddress("phoNTrkHollowDR03", phoNTrkHollowDR03, &b_phoNTrkHollowDR03);
   fChain->SetBranchAddress("phoEcalIsoDR03", phoEcalIsoDR03, &b_phoEcalIsoDR03);
   fChain->SetBranchAddress("phoHcalIsoDR03", phoHcalIsoDR03, &b_phoHcalIsoDR03);
   fChain->SetBranchAddress("phoTrkIsoSolidDR04", phoTrkIsoSolidDR04, &b_phoTrkIsoSolidDR04);
   fChain->SetBranchAddress("phoTrkIsoHollowDR04", phoTrkIsoHollowDR04, &b_phoTrkIsoHollowDR04);
   fChain->SetBranchAddress("phoNTrkSolidDR04", phoNTrkSolidDR04, &b_phoNTrkSolidDR04);
   fChain->SetBranchAddress("phoNTrkHollowDR04", phoNTrkHollowDR04, &b_phoNTrkHollowDR04);
   fChain->SetBranchAddress("phoEcalIsoDR04", phoEcalIsoDR04, &b_phoEcalIsoDR04);
   fChain->SetBranchAddress("phoHcalIsoDR04", phoHcalIsoDR04, &b_phoHcalIsoDR04);
   fChain->SetBranchAddress("phoHoverE", phoHoverE, &b_phoHoverE);
   fChain->SetBranchAddress("phoSigmaEtaEta", phoSigmaEtaEta, &b_phoSigmaEtaEta);
   fChain->SetBranchAddress("phoSigmaIEtaIEta", phoSigmaIEtaIEta, &b_phoSigmaIEtaIEta);
   fChain->SetBranchAddress("phoSeedTime", phoSeedTime, &b_phoSeedTime);
   fChain->SetBranchAddress("phoPos", phoPos, &b_phoPos);
   fChain->SetBranchAddress("phoEMax", phoEMax, &b_phoEMax);
   fChain->SetBranchAddress("phoE3x3", phoE3x3, &b_phoE3x3);
   fChain->SetBranchAddress("phoERight", phoERight, &b_phoERight);
   fChain->SetBranchAddress("phoELeft", phoELeft, &b_phoELeft);
   fChain->SetBranchAddress("phoETop", phoETop, &b_phoETop);
   fChain->SetBranchAddress("phoEBottom", phoEBottom, &b_phoEBottom);
   fChain->SetBranchAddress("phoRoundness", phoRoundness, &b_phoRoundness);
   fChain->SetBranchAddress("phoAngle", phoAngle, &b_phoAngle);
   fChain->SetBranchAddress("phoGenIndex", phoGenIndex, &b_phoGenIndex);
   fChain->SetBranchAddress("phoGenGMomPID", phoGenGMomPID, &b_phoGenGMomPID);
   fChain->SetBranchAddress("phoGenMomPID", phoGenMomPID, &b_phoGenMomPID);
   fChain->SetBranchAddress("phoGenMomPt", phoGenMomPt, &b_phoGenMomPt);
   fChain->SetBranchAddress("phoSCE", phoSCE, &b_phoSCE);
   fChain->SetBranchAddress("phoSCEt", phoSCEt, &b_phoSCEt);
   fChain->SetBranchAddress("phoSCEta", phoSCEta, &b_phoSCEta);
   fChain->SetBranchAddress("phoSCPhi", phoSCPhi, &b_phoSCPhi);
   fChain->SetBranchAddress("phoSCEtaWidth", phoSCEtaWidth, &b_phoSCEtaWidth);
   fChain->SetBranchAddress("phoSCPhiWidth", phoSCPhiWidth, &b_phoSCPhiWidth);
   fChain->SetBranchAddress("phoOverlap", phoOverlap, &b_phoOverlap);
   fChain->SetBranchAddress("phohasPixelSeed", phohasPixelSeed, &b_phohasPixelSeed);
   fChain->SetBranchAddress("nMu", &nMu, &b_nMu);
   fChain->SetBranchAddress("muEta", muEta, &b_muEta);
   fChain->SetBranchAddress("muPhi", muPhi, &b_muPhi);
   fChain->SetBranchAddress("muCharge", muCharge, &b_muCharge);
   fChain->SetBranchAddress("muPt", muPt, &b_muPt);
   fChain->SetBranchAddress("muPz", muPz, &b_muPz);
   fChain->SetBranchAddress("muGenIndex", muGenIndex, &b_muGenIndex);
   fChain->SetBranchAddress("muIsoTrk", muIsoTrk, &b_muIsoTrk);
   fChain->SetBranchAddress("muIsoCalo", muIsoCalo, &b_muIsoCalo);
   fChain->SetBranchAddress("muIsoEcal", muIsoEcal, &b_muIsoEcal);
   fChain->SetBranchAddress("muIsoHcal", muIsoHcal, &b_muIsoHcal);
   fChain->SetBranchAddress("muChi2NDF", muChi2NDF, &b_muChi2NDF);
   fChain->SetBranchAddress("muEmVeto", muEmVeto, &b_muEmVeto);
   fChain->SetBranchAddress("muHadVeto", muHadVeto, &b_muHadVeto);
   fChain->SetBranchAddress("muType", muType, &b_muType);
   fChain->SetBranchAddress("muID", muID, &b_muID);
   fChain->SetBranchAddress("muD0", muD0, &b_muD0);
   fChain->SetBranchAddress("muNumberOfValidTrkHits", muNumberOfValidTrkHits, &b_muNumberOfValidTrkHits);
   fChain->SetBranchAddress("muNumberOfValidPixelHits", muNumberOfValidPixelHits, &b_muNumberOfValidPixelHits);
   fChain->SetBranchAddress("muNumberOfValidMuonHits", muNumberOfValidMuonHits, &b_muNumberOfValidMuonHits);
   fChain->SetBranchAddress("nZee", &nZee, &b_nZee);
   fChain->SetBranchAddress("ZeeMass", ZeeMass, &b_ZeeMass);
   fChain->SetBranchAddress("ZeePt", ZeePt, &b_ZeePt);
   fChain->SetBranchAddress("ZeeEta", ZeeEta, &b_ZeeEta);
   fChain->SetBranchAddress("ZeePhi", ZeePhi, &b_ZeePhi);
   fChain->SetBranchAddress("ZeeLeg1Index", ZeeLeg1Index, &b_ZeeLeg1Index);
   fChain->SetBranchAddress("ZeeLeg2Index", ZeeLeg2Index, &b_ZeeLeg2Index);
   fChain->SetBranchAddress("nZmumu", &nZmumu, &b_nZmumu);
   fChain->SetBranchAddress("ZmumuMass", ZmumuMass, &b_ZmumuMass);
   fChain->SetBranchAddress("ZmumuPt", ZmumuPt, &b_ZmumuPt);
   fChain->SetBranchAddress("ZmumuEta", ZmumuEta, &b_ZmumuEta);
   fChain->SetBranchAddress("ZmumuPhi", ZmumuPhi, &b_ZmumuPhi);
   fChain->SetBranchAddress("ZmumuLeg1Index", ZmumuLeg1Index, &b_ZmumuLeg1Index);
   fChain->SetBranchAddress("ZmumuLeg2Index", ZmumuLeg2Index, &b_ZmumuLeg2Index);
   fChain->SetBranchAddress("nWenu", &nWenu, &b_nWenu);
   fChain->SetBranchAddress("WenuMassTCaloMET", WenuMassTCaloMET, &b_WenuMassTCaloMET);
   fChain->SetBranchAddress("WenuEtCaloMET", WenuEtCaloMET, &b_WenuEtCaloMET);
   fChain->SetBranchAddress("WenuACopCaloMET", WenuACopCaloMET, &b_WenuACopCaloMET);
   fChain->SetBranchAddress("WenuMassTTcMET", WenuMassTTcMET, &b_WenuMassTTcMET);
   fChain->SetBranchAddress("WenuEtTcMET", WenuEtTcMET, &b_WenuEtTcMET);
   fChain->SetBranchAddress("WenuACopTcMET", WenuACopTcMET, &b_WenuACopTcMET);
   fChain->SetBranchAddress("WenuMassTPfMET", WenuMassTPfMET, &b_WenuMassTPfMET);
   fChain->SetBranchAddress("WenuEtPfMET", WenuEtPfMET, &b_WenuEtPfMET);
   fChain->SetBranchAddress("WenuACopPfMET", WenuACopPfMET, &b_WenuACopPfMET);
   fChain->SetBranchAddress("WenuEleIndex", WenuEleIndex, &b_WenuEleIndex);
   fChain->SetBranchAddress("nWmunu", &nWmunu, &b_nWmunu);
   fChain->SetBranchAddress("WmunuMassTCaloMET", WmunuMassTCaloMET, &b_WmunuMassTCaloMET);
   fChain->SetBranchAddress("WmunuEtCaloMET", WmunuEtCaloMET, &b_WmunuEtCaloMET);
   fChain->SetBranchAddress("WmunuACopCaloMET", WmunuACopCaloMET, &b_WmunuACopCaloMET);
   fChain->SetBranchAddress("WmunuMassTTcMET", WmunuMassTTcMET, &b_WmunuMassTTcMET);
   fChain->SetBranchAddress("WmunuEtTcMET", WmunuEtTcMET, &b_WmunuEtTcMET);
   fChain->SetBranchAddress("WmunuACopTcMET", WmunuACopTcMET, &b_WmunuACopTcMET);
   fChain->SetBranchAddress("WmunuMassTPfMET", WmunuMassTPfMET, &b_WmunuMassTPfMET);
   fChain->SetBranchAddress("WmunuEtPfMET", WmunuEtPfMET, &b_WmunuEtPfMET);
   fChain->SetBranchAddress("WmunuACopPfMET", WmunuACopPfMET, &b_WmunuACopPfMET);
   fChain->SetBranchAddress("WmunuMuIndex", WmunuMuIndex, &b_WmunuMuIndex);
   Notify();
}

Bool_t ana::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void ana::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t ana::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef ana_cxx
