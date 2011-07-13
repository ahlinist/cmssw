//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jul 11 10:18:59 2011 by ROOT version 5.27/06b
// from TTree EventTree/Event data
// found on file: /data3/ncuhep/data/425_vgamma_42x_v8/SingleElectron_Run2011A_May10ReRecov1/SingleElectron_Run2011A_May10ReRecov1.root
//////////////////////////////////////////////////////////

#ifndef ana_h
#define ana_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
   const Int_t kMaxmuValidFraction = 1;

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
   Int_t           nHLT;
   Int_t           HLT[353];   //[nHLT]
   Int_t           HLTIndex[305];
   Int_t           HLTprescale[353];   //[nHLT]
   Int_t           nHFTowersP;
   Int_t           nHFTowersN;
   Int_t           nVtx;
   Float_t         vtx[55][3];   //[nVtx]
   Int_t           vtxNTrk[55];   //[nVtx]
   Int_t           vtxNDF[55];   //[nVtx]
   Float_t         vtxD0[55];   //[nVtx]
   Int_t           nGoodVtx;
   Int_t           IsVtxGood;
   Int_t           nTrk;
   Int_t           nGoodTrk;
   Int_t           IsTracksGood;
   Float_t         rho;
   Float_t         sigma;
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
   Int_t           eleTrg[100][31];   //[nEle]
   Int_t           eleID[100][30];   //[nEle]
   Float_t         eleIDLH[100];   //[nEle]
   Int_t           eleClass[100];   //[nEle]
   Int_t           eleCharge[100];   //[nEle]
   Float_t         eleEn[100];   //[nEle]
   Float_t         eleSCRawEn[100];   //[nEle]
   Float_t         eleESEn[100];   //[nEle]
   Float_t         eleSCEn[100];   //[nEle]
   Float_t         elePt[100];   //[nEle]
   Float_t         elePz[100];   //[nEle]
   Float_t         eleEta[100];   //[nEle]
   Float_t         elePhi[100];   //[nEle]
   Float_t         eleSCEta[100];   //[nEle]
   Float_t         eleSCPhi[100];   //[nEle]
   Float_t         eleSCEtaWidth[100];   //[nEle]
   Float_t         eleSCPhiWidth[100];   //[nEle]
   Float_t         eleVtx[100][3];   //[nEle]
   Float_t         eleCaloPos[100][3];   //[nEle]
   Float_t         eleSCPos[100][3];   //[nEle]
   Float_t         eleHoverE[100];   //[nEle]
   Float_t         eleEoverP[100];   //[nEle]
   Float_t         elePin[100];   //[nEle]
   Float_t         elePout[100];   //[nEle]
   Float_t         eleBrem[100];   //[nEle]
   Int_t           elenBrem[100];   //[nEle]
   Float_t         eledEtaAtVtx[100];   //[nEle]
   Float_t         eledPhiAtVtx[100];   //[nEle]
   Float_t         eleSigmaEtaEta[100];   //[nEle]
   Float_t         eleSigmaIEtaIEta[100];   //[nEle]
   Float_t         eleSigmaIEtaIPhi[100];   //[nEle]
   Float_t         eleSigmaIPhiIPhi[100];   //[nEle]
   Float_t         eleE2overE9[100];   //[nEle]
   Float_t         eleE3x3[100];   //[nEle]
   Float_t         eleSeedTime[100];   //[nEle]
   Int_t           eleRecoFlag[100];   //[nEle]
   Int_t           eleSeverity[100];   //[nEle]
   Float_t         eleIsoTrkDR03[100];   //[nEle]
   Float_t         eleIsoEcalDR03[100];   //[nEle]
   Float_t         eleIsoHcalDR03[100];   //[nEle]
   Float_t         eleIsoHcalSolidDR03[100];   //[nEle]
   Float_t         eleIsoTrkDR04[100];   //[nEle]
   Float_t         eleIsoEcalDR04[100];   //[nEle]
   Float_t         eleIsoHcalDR04[100];   //[nEle]
   Float_t         eleIsoHcalSolidDR04[100];   //[nEle]
   Float_t         eleConvDist[100];   //[nEle]
   Float_t         eleConvDcot[100];   //[nEle]
   Float_t         eleConvRadius[100];   //[nEle]
   Int_t           eleConvFlag[100];   //[nEle]
   Int_t           eleConvMissinghit[100];   //[nEle]
   Float_t         eleESRatio[100];   //[nEle]
   Float_t         eleESProfileFront[100][123];   //[nEle]
   Float_t         eleESProfileRear[100][123];   //[nEle]
   Float_t         elePV2D[100];   //[nEle]
   Float_t         elePV3D[100];   //[nEle]
   Float_t         eleBS2D[100];   //[nEle]
   Float_t         eleBS3D[100];   //[nEle]
   Float_t         elePVD0[100];   //[nEle]
   Float_t         elePVDz[100];   //[nEle]
   Int_t           nPho;
   Int_t           phoTrg[100][8];   //[nPho]
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
   Float_t         phoHcalIsoSolidDR03[100];   //[nPho]
   Float_t         phoTrkIsoSolidDR04[100];   //[nPho]
   Float_t         phoTrkIsoHollowDR04[100];   //[nPho]
   Int_t           phoNTrkSolidDR04[100];   //[nPho]
   Int_t           phoNTrkHollowDR04[100];   //[nPho]
   Float_t         phoEcalIsoDR04[100];   //[nPho]
   Float_t         phoHcalIsoDR04[100];   //[nPho]
   Float_t         phoHcalIsoSolidDR04[100];   //[nPho]
   Float_t         phoHoverE[100];   //[nPho]
   Float_t         phoSigmaEtaEta[100];   //[nPho]
   Float_t         phoSigmaIEtaIEta[100];   //[nPho]
   Float_t         phoSigmaIEtaIPhi[100];   //[nPho]
   Float_t         phoSigmaIPhiIPhi[100];   //[nPho]
   Float_t         phoE2overE9[100];   //[nPho]
   Float_t         phoE3x3[100];   //[nPho]
   Float_t         phoSeedTime[100];   //[nPho]
   Int_t           phoRecoFlag[100];   //[nPho]
   Int_t           phoSeverity[100];   //[nPho]
   Int_t           phoPos[100];   //[nPho]
   Float_t         phoRoundness[100];   //[nPho]
   Float_t         phoAngle[100];   //[nPho]
   Float_t         phoSCE[100];   //[nPho]
   Float_t         phoSCEt[100];   //[nPho]
   Float_t         phoSCEta[100];   //[nPho]
   Float_t         phoSCPhi[100];   //[nPho]
   Float_t         phoSCEtaWidth[100];   //[nPho]
   Float_t         phoSCPhiWidth[100];   //[nPho]
   Int_t           phoOverlap[100];   //[nPho]
   Int_t           phohasPixelSeed[100];   //[nPho]
   Int_t           phoIsConv[100];   //[nPho]
   Float_t         phoPi0Disc[100];   //[nPho]
   Float_t         phoESRatio[100];   //[nPho]
   Float_t         phoESProfileFront[100][123];   //[nPho]
   Float_t         phoESProfileRear[100][123];   //[nPho]
   Int_t           nJet;
   Int_t           jetTrg[100][58];   //[nJet]
   Float_t         jetEn[100];   //[nJet]
   Float_t         jetPt[100];   //[nJet]
   Float_t         jetEta[100];   //[nJet]
   Float_t         jetPhi[100];   //[nJet]
   Float_t         jetMass[100];   //[nJet]
   Float_t         jetEt[100];   //[nJet]
   Int_t           jetpartonFlavour[100];   //[nJet]
   Float_t         jetRawPt[100];   //[nJet]
   Float_t         jetRawEn[100];   //[nJet]
   Float_t         jetCharge[100];   //[nJet]
   Float_t         jetNeutralEmEnergy[100];   //[nJet]
   Float_t         jetNeutralEmEnergyFraction[100];   //[nJet]
   Float_t         jetNeutralHadronEnergy[100];   //[nJet]
   Float_t         jetNeutralHadronEnergyFraction[100];   //[nJet]
   Int_t           jetNConstituents[100];   //[nJet]
   Float_t         jetChargedEmEnergy[100];   //[nJet]
   Float_t         jetChargedEmEnergyFraction[100];   //[nJet]
   Float_t         jetChargedHadronEnergy[100];   //[nJet]
   Float_t         jetChargedHadronEnergyFraction[100];   //[nJet]
   Int_t           jetChargedHadronMultiplicity[100];   //[nJet]
   Float_t         jetChargedMuEnergy[100];   //[nJet]
   Float_t         jetChargedMuEnergyFraction[100];   //[nJet]
   Int_t           nWenu;
   Float_t         WenuMassTCaloMET[100];   //[nWenu]
   Float_t         WenuEtCaloMET[100];   //[nWenu]
   Float_t         WenuACopCaloMET[100];   //[nWenu]
   Float_t         WenuMassTTcMET[100];   //[nWenu]
   Float_t         WenuEtTcMET[100];   //[nWenu]
   Float_t         WenuACopTcMET[100];   //[nWenu]
   Float_t         WenuMassTPfMET[100];   //[nWenu]
   Float_t         WenuEtPfMET[100];   //[nWenu]
   Float_t         WenuACopPfMET[100];   //[nWenu]
   Int_t           WenuEleIndex[100];   //[nWenu]

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_event;   //!
   TBranch        *b_orbit;   //!
   TBranch        *b_bx;   //!
   TBranch        *b_lumis;   //!
   TBranch        *b_isData;   //!
   TBranch        *b_ttbit0;   //!
   TBranch        *b_nHLT;   //!
   TBranch        *b_HLT;   //!
   TBranch        *b_HLTIndex;   //!
   TBranch        *b_HLTprescale;   //!
   TBranch        *b_nHFTowersP;   //!
   TBranch        *b_nHFTowersN;   //!
   TBranch        *b_nVtx;   //!
   TBranch        *b_vtx;   //!
   TBranch        *b_vtxNTrk;   //!
   TBranch        *b_vtxNDF;   //!
   TBranch        *b_vtxD0;   //!
   TBranch        *b_nGoodVtx;   //!
   TBranch        *b_IsVtxGood;   //!
   TBranch        *b_nTrk;   //!
   TBranch        *b_nGoodTrk;   //!
   TBranch        *b_IsTracksGood;   //!
   TBranch        *b_rho;   //!
   TBranch        *b_sigma;   //!
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
   TBranch        *b_eleTrg;   //!
   TBranch        *b_eleID;   //!
   TBranch        *b_eleIDLH;   //!
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
   TBranch        *b_elenBrem;   //!
   TBranch        *b_eledEtaAtVtx;   //!
   TBranch        *b_eledPhiAtVtx;   //!
   TBranch        *b_eleSigmaEtaEta;   //!
   TBranch        *b_eleSigmaIEtaIEta;   //!
   TBranch        *b_eleSigmaIEtaIPhi;   //!
   TBranch        *b_eleSigmaIPhiIPhi;   //!
   TBranch        *b_eleE2overE9;   //!
   TBranch        *b_eleE3x3;   //!
   TBranch        *b_eleSeedTime;   //!
   TBranch        *b_eleRecoFlag;   //!
   TBranch        *b_eleSeverity;   //!
   TBranch        *b_eleIsoTrkDR03;   //!
   TBranch        *b_eleIsoEcalDR03;   //!
   TBranch        *b_eleIsoHcalDR03;   //!
   TBranch        *b_eleIsoHcalSolidDR03;   //!
   TBranch        *b_eleIsoTrkDR04;   //!
   TBranch        *b_eleIsoEcalDR04;   //!
   TBranch        *b_eleIsoHcalDR04;   //!
   TBranch        *b_eleIsoHcalSolidDR04;   //!
   TBranch        *b_eleConvDist;   //!
   TBranch        *b_eleConvDcot;   //!
   TBranch        *b_eleConvRadius;   //!
   TBranch        *b_eleConvFlag;   //!
   TBranch        *b_eleConvMissinghit;   //!
   TBranch        *b_eleESRatio;   //!
   TBranch        *b_eleESProfileFront;   //!
   TBranch        *b_eleESProfileRear;   //!
   TBranch        *b_elePV2D;   //!
   TBranch        *b_elePV3D;   //!
   TBranch        *b_eleBS2D;   //!
   TBranch        *b_eleBS3D;   //!
   TBranch        *b_elePVD0;   //!
   TBranch        *b_elePVDz;   //!
   TBranch        *b_nPho;   //!
   TBranch        *b_phoTrg;   //!
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
   TBranch        *b_phoHcalIsoSolidDR03;   //!
   TBranch        *b_phoTrkIsoSolidDR04;   //!
   TBranch        *b_phoTrkIsoHollowDR04;   //!
   TBranch        *b_phoNTrkSolidDR04;   //!
   TBranch        *b_phoNTrkHollowDR04;   //!
   TBranch        *b_phoEcalIsoDR04;   //!
   TBranch        *b_phoHcalIsoDR04;   //!
   TBranch        *b_phoHcalIsoSolidDR04;   //!
   TBranch        *b_phoHoverE;   //!
   TBranch        *b_phoSigmaEtaEta;   //!
   TBranch        *b_phoSigmaIEtaIEta;   //!
   TBranch        *b_phoSigmaIEtaIPhi;   //!
   TBranch        *b_phoSigmaIPhiIPhi;   //!
   TBranch        *b_phoE2overE9;   //!
   TBranch        *b_phoE3x3;   //!
   TBranch        *b_phoSeedTime;   //!
   TBranch        *b_phoRecoFlag;   //!
   TBranch        *b_phoSeverity;   //!
   TBranch        *b_phoPos;   //!
   TBranch        *b_phoRoundness;   //!
   TBranch        *b_phoAngle;   //!
   TBranch        *b_phoSCE;   //!
   TBranch        *b_phoSCEt;   //!
   TBranch        *b_phoSCEta;   //!
   TBranch        *b_phoSCPhi;   //!
   TBranch        *b_phoSCEtaWidth;   //!
   TBranch        *b_phoSCPhiWidth;   //!
   TBranch        *b_phoOverlap;   //!
   TBranch        *b_phohasPixelSeed;   //!
   TBranch        *b_phoIsConv;   //!
   TBranch        *b_phoPi0Disc;   //!
   TBranch        *b_phoESRatio;   //!
   TBranch        *b_phoESProfileFront;   //!
   TBranch        *b_phoESProfileRear;   //!
   TBranch        *b_nJet;   //!
   TBranch        *b_jetTrg;   //!
   TBranch        *b_jetEn;   //!
   TBranch        *b_jetPt;   //!
   TBranch        *b_jetEta;   //!
   TBranch        *b_jetPhi;   //!
   TBranch        *b_jetMass;   //!
   TBranch        *b_jetEt;   //!
   TBranch        *b_jetpartonFlavour;   //!
   TBranch        *b_jetRawPt;   //!
   TBranch        *b_jetRawEn;   //!
   TBranch        *b_jetCharge;   //!
   TBranch        *b_jetNeutralEmEnergy;   //!
   TBranch        *b_jetNeutralEmEnergyFraction;   //!
   TBranch        *b_jetNeutralHadronEnergy;   //!
   TBranch        *b_jetNeutralHadronEnergyFraction;   //!
   TBranch        *b_jetNConstituents;   //!
   TBranch        *b_jetChargedEmEnergy;   //!
   TBranch        *b_jetChargedEmEnergyFraction;   //!
   TBranch        *b_jetChargedHadronEnergy;   //!
   TBranch        *b_jetChargedHadronEnergyFraction;   //!
   TBranch        *b_jetChargedHadronMultiplicity;   //!
   TBranch        *b_jetChargedMuEnergy;   //!
   TBranch        *b_jetChargedMuEnergyFraction;   //!
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
   Chain->Add("/data3/ncuhep/data/425_vgamma_42x_v8/SingleElectron_Run2011A_May10ReRecov1/SingleElectron_Run2011A_May10ReRecov1.root");
   Chain->Add("/data3/ncuhep/data/425_vgamma_42x_v8/SingleElectron_Run2011A_PromptRecov4_Run165088to167913/SingleElectron_Run2011A_PromptRecov4_Run165088to167913.root");
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
   fChain->SetBranchAddress("nHLT", &nHLT, &b_nHLT);
   fChain->SetBranchAddress("HLT", HLT, &b_HLT);
   fChain->SetBranchAddress("HLTIndex", HLTIndex, &b_HLTIndex);
   fChain->SetBranchAddress("HLTprescale", HLTprescale, &b_HLTprescale);
   fChain->SetBranchAddress("nHFTowersP", &nHFTowersP, &b_nHFTowersP);
   fChain->SetBranchAddress("nHFTowersN", &nHFTowersN, &b_nHFTowersN);
   fChain->SetBranchAddress("nVtx", &nVtx, &b_nVtx);
   fChain->SetBranchAddress("vtx", vtx, &b_vtx);
   fChain->SetBranchAddress("vtxNTrk", vtxNTrk, &b_vtxNTrk);
   fChain->SetBranchAddress("vtxNDF", vtxNDF, &b_vtxNDF);
   fChain->SetBranchAddress("vtxD0", vtxD0, &b_vtxD0);
   fChain->SetBranchAddress("nGoodVtx", &nGoodVtx, &b_nGoodVtx);
   fChain->SetBranchAddress("IsVtxGood", &IsVtxGood, &b_IsVtxGood);
   fChain->SetBranchAddress("nTrk", &nTrk, &b_nTrk);
   fChain->SetBranchAddress("nGoodTrk", &nGoodTrk, &b_nGoodTrk);
   fChain->SetBranchAddress("IsTracksGood", &IsTracksGood, &b_IsTracksGood);
   fChain->SetBranchAddress("rho", &rho, &b_rho);
   fChain->SetBranchAddress("sigma", &sigma, &b_sigma);
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
   fChain->SetBranchAddress("eleTrg", eleTrg, &b_eleTrg);
   fChain->SetBranchAddress("eleID", eleID, &b_eleID);
   fChain->SetBranchAddress("eleIDLH", eleIDLH, &b_eleIDLH);
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
   fChain->SetBranchAddress("elenBrem", elenBrem, &b_elenBrem);
   fChain->SetBranchAddress("eledEtaAtVtx", eledEtaAtVtx, &b_eledEtaAtVtx);
   fChain->SetBranchAddress("eledPhiAtVtx", eledPhiAtVtx, &b_eledPhiAtVtx);
   fChain->SetBranchAddress("eleSigmaEtaEta", eleSigmaEtaEta, &b_eleSigmaEtaEta);
   fChain->SetBranchAddress("eleSigmaIEtaIEta", eleSigmaIEtaIEta, &b_eleSigmaIEtaIEta);
   fChain->SetBranchAddress("eleSigmaIEtaIPhi", eleSigmaIEtaIPhi, &b_eleSigmaIEtaIPhi);
   fChain->SetBranchAddress("eleSigmaIPhiIPhi", eleSigmaIPhiIPhi, &b_eleSigmaIPhiIPhi);
   fChain->SetBranchAddress("eleE2overE9", eleE2overE9, &b_eleE2overE9);
   fChain->SetBranchAddress("eleE3x3", eleE3x3, &b_eleE3x3);
   fChain->SetBranchAddress("eleSeedTime", eleSeedTime, &b_eleSeedTime);
   fChain->SetBranchAddress("eleRecoFlag", eleRecoFlag, &b_eleRecoFlag);
   fChain->SetBranchAddress("eleSeverity", eleSeverity, &b_eleSeverity);
   fChain->SetBranchAddress("eleIsoTrkDR03", eleIsoTrkDR03, &b_eleIsoTrkDR03);
   fChain->SetBranchAddress("eleIsoEcalDR03", eleIsoEcalDR03, &b_eleIsoEcalDR03);
   fChain->SetBranchAddress("eleIsoHcalDR03", eleIsoHcalDR03, &b_eleIsoHcalDR03);
   fChain->SetBranchAddress("eleIsoHcalSolidDR03", eleIsoHcalSolidDR03, &b_eleIsoHcalSolidDR03);
   fChain->SetBranchAddress("eleIsoTrkDR04", eleIsoTrkDR04, &b_eleIsoTrkDR04);
   fChain->SetBranchAddress("eleIsoEcalDR04", eleIsoEcalDR04, &b_eleIsoEcalDR04);
   fChain->SetBranchAddress("eleIsoHcalDR04", eleIsoHcalDR04, &b_eleIsoHcalDR04);
   fChain->SetBranchAddress("eleIsoHcalSolidDR04", eleIsoHcalSolidDR04, &b_eleIsoHcalSolidDR04);
   fChain->SetBranchAddress("eleConvDist", eleConvDist, &b_eleConvDist);
   fChain->SetBranchAddress("eleConvDcot", eleConvDcot, &b_eleConvDcot);
   fChain->SetBranchAddress("eleConvRadius", eleConvRadius, &b_eleConvRadius);
   fChain->SetBranchAddress("eleConvFlag", eleConvFlag, &b_eleConvFlag);
   fChain->SetBranchAddress("eleConvMissinghit", eleConvMissinghit, &b_eleConvMissinghit);
   fChain->SetBranchAddress("eleESRatio", eleESRatio, &b_eleESRatio);
   fChain->SetBranchAddress("eleESProfileFront", eleESProfileFront, &b_eleESProfileFront);
   fChain->SetBranchAddress("eleESProfileRear", eleESProfileRear, &b_eleESProfileRear);
   fChain->SetBranchAddress("elePV2D", elePV2D, &b_elePV2D);
   fChain->SetBranchAddress("elePV3D", elePV3D, &b_elePV3D);
   fChain->SetBranchAddress("eleBS2D", eleBS2D, &b_eleBS2D);
   fChain->SetBranchAddress("eleBS3D", eleBS3D, &b_eleBS3D);
   fChain->SetBranchAddress("elePVD0", elePVD0, &b_elePVD0);
   fChain->SetBranchAddress("elePVDz", elePVDz, &b_elePVDz);
   fChain->SetBranchAddress("nPho", &nPho, &b_nPho);
   fChain->SetBranchAddress("phoTrg", phoTrg, &b_phoTrg);
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
   fChain->SetBranchAddress("phoHcalIsoSolidDR03", phoHcalIsoSolidDR03, &b_phoHcalIsoSolidDR03);
   fChain->SetBranchAddress("phoTrkIsoSolidDR04", phoTrkIsoSolidDR04, &b_phoTrkIsoSolidDR04);
   fChain->SetBranchAddress("phoTrkIsoHollowDR04", phoTrkIsoHollowDR04, &b_phoTrkIsoHollowDR04);
   fChain->SetBranchAddress("phoNTrkSolidDR04", phoNTrkSolidDR04, &b_phoNTrkSolidDR04);
   fChain->SetBranchAddress("phoNTrkHollowDR04", phoNTrkHollowDR04, &b_phoNTrkHollowDR04);
   fChain->SetBranchAddress("phoEcalIsoDR04", phoEcalIsoDR04, &b_phoEcalIsoDR04);
   fChain->SetBranchAddress("phoHcalIsoDR04", phoHcalIsoDR04, &b_phoHcalIsoDR04);
   fChain->SetBranchAddress("phoHcalIsoSolidDR04", phoHcalIsoSolidDR04, &b_phoHcalIsoSolidDR04);
   fChain->SetBranchAddress("phoHoverE", phoHoverE, &b_phoHoverE);
   fChain->SetBranchAddress("phoSigmaEtaEta", phoSigmaEtaEta, &b_phoSigmaEtaEta);
   fChain->SetBranchAddress("phoSigmaIEtaIEta", phoSigmaIEtaIEta, &b_phoSigmaIEtaIEta);
   fChain->SetBranchAddress("phoSigmaIEtaIPhi", phoSigmaIEtaIPhi, &b_phoSigmaIEtaIPhi);
   fChain->SetBranchAddress("phoSigmaIPhiIPhi", phoSigmaIPhiIPhi, &b_phoSigmaIPhiIPhi);
   fChain->SetBranchAddress("phoE2overE9", phoE2overE9, &b_phoE2overE9);
   fChain->SetBranchAddress("phoE3x3", phoE3x3, &b_phoE3x3);
   fChain->SetBranchAddress("phoSeedTime", phoSeedTime, &b_phoSeedTime);
   fChain->SetBranchAddress("phoRecoFlag", phoRecoFlag, &b_phoRecoFlag);
   fChain->SetBranchAddress("phoSeverity", phoSeverity, &b_phoSeverity);
   fChain->SetBranchAddress("phoPos", phoPos, &b_phoPos);
   fChain->SetBranchAddress("phoRoundness", phoRoundness, &b_phoRoundness);
   fChain->SetBranchAddress("phoAngle", phoAngle, &b_phoAngle);
   fChain->SetBranchAddress("phoSCE", phoSCE, &b_phoSCE);
   fChain->SetBranchAddress("phoSCEt", phoSCEt, &b_phoSCEt);
   fChain->SetBranchAddress("phoSCEta", phoSCEta, &b_phoSCEta);
   fChain->SetBranchAddress("phoSCPhi", phoSCPhi, &b_phoSCPhi);
   fChain->SetBranchAddress("phoSCEtaWidth", phoSCEtaWidth, &b_phoSCEtaWidth);
   fChain->SetBranchAddress("phoSCPhiWidth", phoSCPhiWidth, &b_phoSCPhiWidth);
   fChain->SetBranchAddress("phoOverlap", phoOverlap, &b_phoOverlap);
   fChain->SetBranchAddress("phohasPixelSeed", phohasPixelSeed, &b_phohasPixelSeed);
   fChain->SetBranchAddress("phoIsConv", phoIsConv, &b_phoIsConv);
   fChain->SetBranchAddress("phoPi0Disc", phoPi0Disc, &b_phoPi0Disc);
   fChain->SetBranchAddress("phoESRatio", phoESRatio, &b_phoESRatio);
   fChain->SetBranchAddress("phoESProfileFront", phoESProfileFront, &b_phoESProfileFront);
   fChain->SetBranchAddress("phoESProfileRear", phoESProfileRear, &b_phoESProfileRear);
   fChain->SetBranchAddress("nJet", &nJet, &b_nJet);
   fChain->SetBranchAddress("jetTrg", jetTrg, &b_jetTrg);
   fChain->SetBranchAddress("jetEn", jetEn, &b_jetEn);
   fChain->SetBranchAddress("jetPt", jetPt, &b_jetPt);
   fChain->SetBranchAddress("jetEta", jetEta, &b_jetEta);
   fChain->SetBranchAddress("jetPhi", jetPhi, &b_jetPhi);
   fChain->SetBranchAddress("jetMass", jetMass, &b_jetMass);
   fChain->SetBranchAddress("jetEt", jetEt, &b_jetEt);
   fChain->SetBranchAddress("jetpartonFlavour", jetpartonFlavour, &b_jetpartonFlavour);
   fChain->SetBranchAddress("jetRawPt", jetRawPt, &b_jetRawPt);
   fChain->SetBranchAddress("jetRawEn", jetRawEn, &b_jetRawEn);
   fChain->SetBranchAddress("jetCharge", jetCharge, &b_jetCharge);
   fChain->SetBranchAddress("jetNeutralEmEnergy", jetNeutralEmEnergy, &b_jetNeutralEmEnergy);
   fChain->SetBranchAddress("jetNeutralEmEnergyFraction", jetNeutralEmEnergyFraction, &b_jetNeutralEmEnergyFraction);
   fChain->SetBranchAddress("jetNeutralHadronEnergy", jetNeutralHadronEnergy, &b_jetNeutralHadronEnergy);
   fChain->SetBranchAddress("jetNeutralHadronEnergyFraction", jetNeutralHadronEnergyFraction, &b_jetNeutralHadronEnergyFraction);
   fChain->SetBranchAddress("jetNConstituents", jetNConstituents, &b_jetNConstituents);
   fChain->SetBranchAddress("jetChargedEmEnergy", jetChargedEmEnergy, &b_jetChargedEmEnergy);
   fChain->SetBranchAddress("jetChargedEmEnergyFraction", jetChargedEmEnergyFraction, &b_jetChargedEmEnergyFraction);
   fChain->SetBranchAddress("jetChargedHadronEnergy", jetChargedHadronEnergy, &b_jetChargedHadronEnergy);
   fChain->SetBranchAddress("jetChargedHadronEnergyFraction", jetChargedHadronEnergyFraction, &b_jetChargedHadronEnergyFraction);
   fChain->SetBranchAddress("jetChargedHadronMultiplicity", jetChargedHadronMultiplicity, &b_jetChargedHadronMultiplicity);
   fChain->SetBranchAddress("jetChargedMuEnergy", jetChargedMuEnergy, &b_jetChargedMuEnergy);
   fChain->SetBranchAddress("jetChargedMuEnergyFraction", jetChargedMuEnergyFraction, &b_jetChargedMuEnergyFraction);
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
