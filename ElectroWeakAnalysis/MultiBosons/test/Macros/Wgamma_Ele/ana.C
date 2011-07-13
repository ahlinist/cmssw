#define ana_cxx
#include "ana.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <iostream>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TFrame.h>

void ana::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L ana.C
//      Root > ana t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   Int_t ele_Index(-1);
   Int_t W_Index(-1);

   TFile *file = new TFile("Data_2011A.root", "UPDATE");
   TTree *tree = new TTree("DataTree", "Data");
   tree->Branch("run", &run, "run/I");
   tree->Branch("event", &event, "event/I");
   tree->Branch("nHLT", &nHLT, "nHLT/I");
   tree->Branch("HLT", HLT, "HLT[nHLT]/I");
   tree->Branch("HLTIndex", HLTIndex, "HLTIndex[205]/I");
   tree->Branch("HLTprescale", HLTprescale, "HLTprescale[nHLT]/I");
   tree->Branch("nHFTowersP", &nHFTowersP, "nHFTowersP/I");
   tree->Branch("nHFTowersN", &nHFTowersN, "nHFTowersN/I");
   tree->Branch("nVtx", &nVtx, "nVtx/I");
   tree->Branch("vtx", vtx, "vtx[nVtx][3]/F");
   tree->Branch("vtxNTrk", vtxNTrk, "vtxNTrk[nVtx]/I");
   tree->Branch("vtxNDF", vtxNDF, "vtxNDF[nVtx]/I");
   tree->Branch("vtxD0", vtxD0, "vtxD0[nVtx]/F");
   tree->Branch("nGoodVtx", &nGoodVtx, "nGoodVtx/I");
   tree->Branch("rho", &rho, "rho/F");
   tree->Branch("pfMET", &pfMET, "pfMET/F");
   tree->Branch("pfMETx", &pfMETx, "pfMETx/F");
   tree->Branch("pfMETy", &pfMETy, "pfMETy/F");
   tree->Branch("pfMETPhi", &pfMETPhi, "pfMETPhi/F");
   tree->Branch("pfMETsumEt", &pfMETsumEt, "pfMETsumEt/F");
   tree->Branch("pfMETmEtSig", &pfMETmEtSig, "pfMETmEtSig/F");
   tree->Branch("pfMETSig", &pfMETSig, "pfMETSig/F");
   tree->Branch("nEle", &nEle, "nEle/I");
   tree->Branch("eleTrg", eleTrg, "eleTrg[nEle][31]/I");
   tree->Branch("eleID", eleID, "eleID[nEle][30]/I");
   tree->Branch("eleClass", eleClass, "eleClass[nEle]/I");
   tree->Branch("eleCharge", eleCharge, "eleCharge[nEle]/I");
   tree->Branch("eleEn", eleEn, "eleEn[nEle]/F");
   tree->Branch("eleSCRawEn", eleSCRawEn, "eleSCRawEn[nEle]/F");
   tree->Branch("elePt", elePt, "elePt[nEle]/F");
   tree->Branch("elePz", elePz, "elePz[nEle]/F");
   tree->Branch("eleEta", eleEta, "eleEta[nEle]/F");
   tree->Branch("elePhi", elePhi, "elePhi[nEle]/F");
   tree->Branch("eleSCEta", eleSCEta, "eleSCEta[nEle]/F");
   tree->Branch("eleSCPhi", eleSCPhi, "eleSCPhi[nEle]/F");
   tree->Branch("eleVtx", eleVtx, "eleVtx[nEle][3]/F");
   tree->Branch("eleHoverE", eleHoverE, "eleHoverE[nEle]/F");
   tree->Branch("elePin", elePin, "elePin[nEle]/F");
   tree->Branch("elePout", elePout, "elePout[nEle]/F");
   tree->Branch("eleBrem", eleBrem, "eleBrem[nEle]/F");
   tree->Branch("elenBrem", elenBrem, "elenBrem[nEle]/I");
   tree->Branch("eledEtaAtVtx", eledEtaAtVtx, "eledEtaAtVtx[nEle]/F");
   tree->Branch("eledPhiAtVtx", eledPhiAtVtx, "eledPhiAtVtx[nEle]/F");
   tree->Branch("eleSigmaIEtaIEta", eleSigmaIEtaIEta, "eleSigmaIEtaIEta[nEle]/F");
   tree->Branch("eleSigmaIEtaIPhi", eleSigmaIEtaIPhi, "eleSigmaIEtaIPhi[nEle]/F");
   tree->Branch("eleSigmaIPhiIPhi", eleSigmaIPhiIPhi, "eleSigmaIPhiIPhi[nEle]/F");
   tree->Branch("eleE3x3", eleE3x3, "eleE3x3[nEle]/F");
   tree->Branch("eleSeedTime", eleSeedTime, "eleSeedTime[nEle]/F");
   tree->Branch("eleIsoTrkDR03", eleIsoTrkDR03, "eleIsoTrkDR03[nEle]/F");
   tree->Branch("eleIsoEcalDR03", eleIsoEcalDR03, "eleIsoEcalDR03[nEle]/F");
   tree->Branch("eleIsoHcalDR03", eleIsoHcalDR03, "eleIsoHcalDR03[nEle]/F");
   tree->Branch("eleIsoHcalSolidDR03", eleIsoHcalSolidDR03, "eleIsoHcalSolidDR03[nEle]/F");
   tree->Branch("eleIsoTrkDR04", eleIsoTrkDR04, "eleIsoTrkDR04[nEle]/F");
   tree->Branch("eleIsoEcalDR04", eleIsoEcalDR04, "eleIsoEcalDR04[nEle]/F");
   tree->Branch("eleIsoHcalDR04", eleIsoHcalDR04, "eleIsoHcalDR04[nEle]/F");
   tree->Branch("eleIsoHcalSolidDR04", eleIsoHcalSolidDR04, "eleIsoHcalSolidDR04[nEle]/F");
   tree->Branch("eleConvDist", eleConvDist, "eleConvDist[nEle]/F");
   tree->Branch("eleConvDcot", eleConvDcot, "eleConvDcot[nEle]/F");
   tree->Branch("eleConvRadius", eleConvRadius, "eleConvRadius[nEle]/F");
   tree->Branch("eleConvFlag", eleConvFlag, "eleConvFlag[nEle]/I");
   tree->Branch("eleConvMissinghit", eleConvMissinghit, "eleConvMissinghit[nEle]/I");
   tree->Branch("elePVD0", elePVD0, "elePVD0[nEle]/F");
   tree->Branch("elePVDz", elePVDz, "elePVDz[nEle]/F");
   tree->Branch("nPho", &nPho, "nPho/I");
   tree->Branch("phoE", phoE, "phoE[nPho]/F");
   tree->Branch("phoEt", phoEt, "phoEt[nPho]/F");
   tree->Branch("phoPz", phoPz, "phoPz[nPho]/F");
   tree->Branch("phoEta", phoEta, "phoEta[nPho]/F");
   tree->Branch("phoPhi", phoPhi, "phoPhi[nPho]/F");
   tree->Branch("phoR9", phoR9, "phoR9[nPho]/F");
   tree->Branch("phoTrkIsoSolidDR03", phoTrkIsoSolidDR03, "phoTrkIsoSolidDR03[nPho]/F");
   tree->Branch("phoTrkIsoHollowDR03", phoTrkIsoHollowDR03, "phoTrkIsoHollowDR03[nPho]/F");
   tree->Branch("phoNTrkSolidDR03", phoNTrkSolidDR03, "phoNTrkSolidDR03[nPho]/I");
   tree->Branch("phoNTrkHollowDR03", phoNTrkHollowDR03, "phoNTrkHollowDR03[nPho]/I");
   tree->Branch("phoEcalIsoDR03", phoEcalIsoDR03, "phoEcalIsoDR03[nPho]/F");
   tree->Branch("phoHcalIsoDR03", phoHcalIsoDR03, "phoHcalIsoDR03[nPho]/F");
   tree->Branch("phoHcalIsoSolidDR03", phoHcalIsoSolidDR03, "phoHcalIsoSolidDR03[nPho]/F");
   tree->Branch("phoTrkIsoSolidDR04", phoTrkIsoSolidDR04, "phoTrkIsoSolidDR04[nPho]/F");
   tree->Branch("phoTrkIsoHollowDR04", phoTrkIsoHollowDR04, "phoTrkIsoHollowDR04[nPho]/F");
   tree->Branch("phoNTrkSolidDR04", phoNTrkSolidDR04, "phoNTrkSolidDR04[nPho]/I");
   tree->Branch("phoNTrkHollowDR04", phoNTrkHollowDR04, "phoNTrkHollowDR04[nPho]/I");
   tree->Branch("phoEcalIsoDR04", phoEcalIsoDR04, "phoEcalIsoDR04[nPho]/F");
   tree->Branch("phoHcalIsoDR04", phoHcalIsoDR04, "phoHcalIsoDR04[nPho]/F");
   tree->Branch("phoHcalIsoSolidDR04", phoHcalIsoSolidDR04, "phoHcalIsoSolidDR04[nPho]/F");
   tree->Branch("phoHoverE", phoHoverE, "phoHoverE[nPho]/F");
   tree->Branch("phoSigmaIEtaIEta", phoSigmaIEtaIEta, "phoSigmaIEtaIEta[nPho]/F");
   tree->Branch("phoSigmaIEtaIPhi", phoSigmaIEtaIPhi, "phoSigmaIEtaIPhi[nPho]/F");
   tree->Branch("phoSigmaIPhiIPhi", phoSigmaIPhiIPhi, "phoSigmaIPhiIPhi[nPho]/F");
   tree->Branch("phoE3x3", phoE3x3, "phoE3x3[nPho]/F");
   tree->Branch("phoSeedTime", phoSeedTime, "phoSeedTime[nPho]/F");
   tree->Branch("phoSCE", phoSCE, "phoSCE[nPho]/F");
   tree->Branch("phoSCEt", phoSCEt, "phoSCEt[nPho]/F");
   tree->Branch("phoSCEta", phoSCEta, "phoSCEta[nPho]/F");
   tree->Branch("phoSCPhi", phoSCPhi, "phoSCPhi[nPho]/F");
   tree->Branch("phoOverlap", phoOverlap, "phoOverlap[nPho]/I");
   tree->Branch("phohasPixelSeed", phohasPixelSeed, "phohasPixelSeed[nPho]/I");  
   tree->Branch("nJet", &nJet, "nJet/I");
   tree->Branch("jetTrg", jetTrg, "jetTrg[nJet][58]/I");
   tree->Branch("jetEn", jetEn, "jetEn[nJet]/F");
   tree->Branch("jetPt", jetPt, "jetPt[nJet]/F");
   tree->Branch("jetEta", jetEta, "jetEta[nJet]/F");
   tree->Branch("jetPhi", jetPhi, "jetPhi[nJet]/F");
   tree->Branch("jetMass", jetMass, "jetMass[nJet]/F");
   tree->Branch("jetEt", jetEt, "jetEt[nJet]/F");
   tree->Branch("jetpartonFlavour", jetpartonFlavour, "jetpartonFlavour[nJet]/I");
   tree->Branch("jetRawPt", jetRawPt, "jetRawPt[nJet]/F");
   tree->Branch("jetRawEn", jetRawEn, "jetRawEn[nJet]/F");
   tree->Branch("jetCharge", jetCharge, "jetCharge[nJet]/F");
   tree->Branch("jetNeutralEmEnergy", jetNeutralEmEnergy, "jetNeutralEmEnergy[nJet]/F");
   tree->Branch("jetNeutralEmEnergyFraction", jetNeutralEmEnergyFraction, "jetNeutralEmEnergyFraction[nJet]/F");
   tree->Branch("jetNeutralHadronEnergy", jetNeutralHadronEnergy, "jetNeutralHadronEnergy[nJet]/F");
   tree->Branch("jetNeutralHadronEnergyFraction", jetNeutralHadronEnergyFraction, "jetNeutralHadronEnergyFraction[nJet]/F");
   tree->Branch("jetNConstituents", jetNConstituents, "jetNConstituents[nJet]/I");
   tree->Branch("jetChargedEmEnergy", jetChargedEmEnergy, "jetChargedEmEnergy[nJet]/F");
   tree->Branch("jetChargedEmEnergyFraction", jetChargedEmEnergyFraction, "jetChargedEmEnergyFraction[nJet]/F");
   tree->Branch("jetChargedHadronEnergy", jetChargedHadronEnergy, "jetChargedHadronEnergy[nJet]/F");
   tree->Branch("jetChargedHadronEnergyFraction", jetChargedHadronEnergyFraction, "jetChargedHadronEnergyFraction[nJet]/F");
   tree->Branch("jetChargedHadronMultiplicity", jetChargedHadronMultiplicity, "jetChargedHadronMultiplicity[nJet]/I");
   tree->Branch("jetChargedMuEnergy", jetChargedMuEnergy, "jetChargedMuEnergy[nJet]/F");
   tree->Branch("jetChargedMuEnergyFraction", jetChargedMuEnergyFraction, "jetChargedMuEnergyFraction[nJet]/F");
   tree->Branch("nWenu", &nWenu, "nWenu/I");
   tree->Branch("WenuMassTCaloMET", WenuMassTCaloMET, "WenuMassTCaloMET[nWenu]/F");
   tree->Branch("WenuEtCaloMET", WenuEtCaloMET, "WenuEtCaloMET[nWenu]/F");
   tree->Branch("WenuACopCaloMET", WenuACopCaloMET, "WenuACopCaloMET[nWenu]/F");
   tree->Branch("WenuMassTTcMET", WenuMassTTcMET, "WenuMassTTcMET[nWenu]/F");
   tree->Branch("WenuEtTcMET", WenuEtTcMET, "WenuEtTcMET[nWenu]/F");
   tree->Branch("WenuACopTcMET", WenuACopTcMET, "WenuACopTcMET[nWenu]/F");
   tree->Branch("WenuMassTPfMET", WenuMassTPfMET, "WenuMassTPfMET[nWenu]/F");
   tree->Branch("WenuEtPfMET", WenuEtPfMET, "WenuEtPfMET[nWenu]/F");
   tree->Branch("WenuACopPfMET", WenuACopPfMET, "WenuACopPfMET[nWenu]/F");
   tree->Branch("WenuEleIndex", WenuEleIndex, "WenuEleIndex[nWenu]/I");
   tree->Branch("ele_Index", &ele_Index, "ele_Index/I");
   tree->Branch("W_Index", &W_Index, "W_Index/I");

   float Pi = 3.14159265;

   bool  HLT_pass(false);
   int   Wgamma_candidates(0);
   int   Wgamma_MT3_candidates(0);
   float ele_Pt(30);
   int   ele_num(0);
   float CTM[3];
   int   pho_Index(-1);
   float pho_Pt(10);
   float dR(999);
   for (int a=0; a<3; a++) 
     CTM[a] = 0;

   float dEta(0), dPhi(0);

   TH1F *h1 = new TH1F("ElePt_EB_EleCuts", "", 300, 0, 300);
   TH1F *h2 = new TH1F("ElePt_EE_EleCuts", "", 300, 0, 300);
   TH1F *h3 = new TH1F("EleEta_EleCuts", "", 60, -3, 3);
   TH1F *h4 = new TH1F("ElePhi_EB_EleCuts", "", 80, -4, 4);
   TH1F *h5 = new TH1F("ElePhi_EE_EleCuts", "", 80, -4, 4);
   TH1F *h6 = new TH1F("tcMET_EleCuts", "", 300, 0, 300);
   TH1F *h7 = new TH1F("pfMET_EleCuts", "", 300, 0, 300);
   TH1F *h8 = new TH1F("tcMT_EleCuts", "", 300, 0, 300);
   TH1F *h9 = new TH1F("pfMT_EleCuts", "", 300, 0, 300);
   TH1F *h10 = new TH1F("nVtx_EleCuts", "", 50, 0, 50);

   TH1F *h11 = new TH1F("ElePt_EB_AllCuts", "", 2000, 0, 2000);
   TH1F *h12 = new TH1F("ElePt_EE_AllCuts", "", 2000, 0, 2000);
   TH1F *h13 = new TH1F("EleEta_AllCuts", "", 60, -3, 3);
   TH1F *h14 = new TH1F("ElePhi_EB_AllCuts", "", 80, -4, 4);
   TH1F *h15 = new TH1F("ElePhi_EE_AllCuts", "", 80, -4, 4);
   TH1F *h16 = new TH1F("tcMET_AllCuts", "", 2000, 0, 2000);
   TH1F *h17 = new TH1F("pfMET_AllCuts", "", 2000, 0, 2000);
   TH1F *h18 = new TH1F("tcMT_AllCuts", "", 2000, 0, 2000);
   TH1F *h19 = new TH1F("pfMT_AllCuts", "", 2000, 0, 2000);
   TH1F *h20 = new TH1F("PhoPt_EB_AllCuts", "", 2000, 0, 2000);
   TH1F *h21 = new TH1F("PhoPt_EE_AllCuts", "", 2000, 0, 2000);
   TH1F *h22 = new TH1F("PhoEta_AllCuts", "", 60, -3, 3);
   TH1F *h23 = new TH1F("PhoPhi_EB_AllCuts", "", 80, -4, 4);
   TH1F *h24 = new TH1F("PhoPhi_EE_AllCuts", "", 80, -4, 4);
   TH2F *h25 = new TH2F("tcMT_CTM_AllCuts", "", 2000, 0, 2000, 2000, 0, 2000);
   TH2F *h26 = new TH2F("pfMT_CTM_AllCuts", "", 2000, 0, 2000, 2000, 0, 2000);
   TH1F *h27 = new TH1F("ElePhoMass_EB_AllCuts", "", 200, 0, 200);
   TH1F *h28 = new TH1F("ElePhoMass_EE_AllCuts", "", 200, 0, 200);
   TH1F *h29 = new TH1F("nVtx_AllCuts", "", 50, 0, 50);
   TH1F *h30 = new TH1F("dR_AllCuts", "", 100, 0, 10);
   TH1F *h65 = new TH1F("MWG1_AllCuts", "", 1000, 0, 1000);
   TH1F *h66 = new TH1F("MWG2_AllCuts", "", 1000, 0, 1000);
   TH1F *h67 = new TH1F("MWG3_AllCuts", "", 1000, 0, 1000);
   TH1F *h68 = new TH1F("MWG4_AllCuts", "", 1000, 0, 1000);

   TH1F *h31 = new TH1F("ElePt_EB_MT3Cuts", "", 2000, 0, 2000);
   TH1F *h32 = new TH1F("ElePt_EE_MT3Cuts", "", 2000, 0, 2000);
   TH1F *h33 = new TH1F("EleEta_MT3Cuts", "", 60, -3, 3);
   TH1F *h34 = new TH1F("ElePhi_EB_MT3Cuts", "", 80, -4, 4);
   TH1F *h35 = new TH1F("ElePhi_EE_MT3Cuts", "", 80, -4, 4);
   TH1F *h36 = new TH1F("tcMET_MT3Cuts", "", 2000, 0, 2000);
   TH1F *h37 = new TH1F("pfMET_MT3Cuts", "", 2000, 0, 2000);
   TH1F *h38 = new TH1F("tcMT_MT3Cuts", "", 2000, 0, 2000);
   TH1F *h39 = new TH1F("pfMT_MT3Cuts", "", 2000, 0, 2000);
   TH1F *h40 = new TH1F("PhoPt_EB_MT3Cuts", "", 2000, 0, 2000);
   TH1F *h41 = new TH1F("PhoPt_EE_MT3Cuts", "", 2000, 0, 2000);
   TH1F *h42 = new TH1F("PhoEta_MT3Cuts", "", 60, -3, 3);
   TH1F *h43 = new TH1F("PhoPhi_EB_MT3Cuts", "", 80, -4, 4);
   TH1F *h44 = new TH1F("PhoPhi_EE_MT3Cuts", "", 80, -4, 4);
   TH1F *h45 = new TH1F("ElePhoMass_EB_MT3Cuts", "", 200, 0, 200);
   TH1F *h46 = new TH1F("ElePhoMass_EE_MT3Cuts", "", 200, 0, 200);
   TH1F *h47 = new TH1F("RAZ_EB_MT3Cuts", "", 100, -5, 5);
   TH1F *h48 = new TH1F("RAZ_EE_MT3Cuts", "", 100, -5, 5);
   TH1F *h49 = new TH1F("RAZ_MT3Cuts", "", 100, -5, 5);
   TH1F *h52 = new TH1F("nVtx_MT3Cuts", "", 50, 0, 50);
   TH1F *h53 = new TH1F("dR_MT3Cuts", "", 100, 0, 10);
   TH1F *h75 = new TH1F("MWG1_MT3Cuts", "", 2000, 0, 2000);
   TH1F *h76 = new TH1F("MWG2_MT3Cuts", "", 2000, 0, 2000);
   TH1F *h77 = new TH1F("MWG3_MT3Cuts", "", 2000, 0, 2000);
   TH1F *h78 = new TH1F("MWG4_MT3Cuts", "", 2000, 0, 2000);

   TH1F *h50 = new TH1F("PhoSeedTiming_Barrel", "", 500, -50, 50);
   TH1F *h51 = new TH1F("PhoSeedTiming_Endcap", "", 500, -50, 50);

   TH2F *g1 = new TH2F("PhoPtSigmaIEtaIEta_Barrel", "", 100, 0, 500, 9000, 0.0005, 0.0905);
   TH2F *g2 = new TH2F("PhoPtSigmaIEtaIEta_Endcap", "", 100, 0, 500, 9000, 0.0005, 0.0905);

   TH2F *g3 = new TH2F("Run_selection", "", 70000, 130000, 200000, 300, 0, 300);
   TH1F *g4 = new TH1F("Num_Pho", "", 10, 0, 10);

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      HLT_pass = false;
      // HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1
      if (run <= 161176 && HLTIndex[111] != -1 && HLT[HLTIndex[111]] == 1) HLT_pass = true; 
      // HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2
      else if (run >= 161217 && run <= 163261 && HLTIndex[112] != -1 && HLT[HLTIndex[112]] == 1) HLT_pass = true; 
      // HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3
      else if (run >= 163270 && run <= 163869 && HLTIndex[113] != -1 && HLT[HLTIndex[113]] == 1) HLT_pass = true;
      //HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3
      else if (run >= 165088 && run <= 165633 && HLTIndex[116] != -1 && HLT[HLTIndex[116]] == 1) HLT_pass = true; 
      //HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v4
      else if (run >= 165970 && run <= 166861 && HLTIndex[117] != -1 && HLT[HLTIndex[117]] == 1) HLT_pass = true; 
      if (HLT_pass == false) continue;

      if (nGoodVtx == 0) continue;
      if (IsTracksGood == 0) continue;

      // Energy scale
      for (int a=0; a<nEle; a++) {
        if (run >= 160431 && run <= 163869 && fabs(eleSCEta[a]) < 1.4442 && eleE3x3[a]/eleSCRawEn[a] > 0.94)
          elePt[a] = elePt[a]*(1-0.0047);
        if (run >= 165071 && run <= 165970 && fabs(eleSCEta[a]) < 1.4442 && eleE3x3[a]/eleSCRawEn[a] > 0.94)
          elePt[a] = elePt[a]*(1-0.0007);
        if (run >= 165971 && run <= 166502 && fabs(eleSCEta[a]) < 1.4442 && eleE3x3[a]/eleSCRawEn[a] > 0.94)
          elePt[a] = elePt[a]*(1+0.0003);
        if (run >= 166503 && run <= 166861 && fabs(eleSCEta[a]) < 1.4442 && eleE3x3[a]/eleSCRawEn[a] > 0.94)
          elePt[a] = elePt[a]*(1+0.0011);

        if (run >= 160431 && run <= 163869 && fabs(eleSCEta[a]) < 1.4442 && eleE3x3[a]/eleSCRawEn[a] < 0.94)
          elePt[a] = elePt[a]*(1+0.0025);
        if (run >= 165071 && run <= 165970 && fabs(eleSCEta[a]) < 1.4442 && eleE3x3[a]/eleSCRawEn[a] < 0.94)
          elePt[a] = elePt[a]*(1+0.0049);
        if (run >= 165971 && run <= 166502 && fabs(eleSCEta[a]) < 1.4442 && eleE3x3[a]/eleSCRawEn[a] < 0.94)
          elePt[a] = elePt[a]*(1+0.0067);
        if (run >= 166503 && run <= 166861 && fabs(eleSCEta[a]) < 1.4442 && eleE3x3[a]/eleSCRawEn[a] < 0.94)
          elePt[a] = elePt[a]*(1+0.0063);

        if (run >= 160431 && run <= 163869 && fabs(eleSCEta[a]) > 1.566 && eleE3x3[a]/eleSCRawEn[a] > 0.94)
          elePt[a] = elePt[a]*(1+0.0058);
        if (run >= 165071 && run <= 165970 && fabs(eleSCEta[a]) > 1.566 && eleE3x3[a]/eleSCRawEn[a] > 0.94)
          elePt[a] = elePt[a]*(1+0.0249);
        if (run >= 165971 && run <= 166502 && fabs(eleSCEta[a]) > 1.566 && eleE3x3[a]/eleSCRawEn[a] > 0.94)
          elePt[a] = elePt[a]*(1+0.0376);
        if (run >= 166503 && run <= 166861 && fabs(eleSCEta[a]) > 1.566 && eleE3x3[a]/eleSCRawEn[a] > 0.94)
          elePt[a] = elePt[a]*(1+0.0450);

        if (run >= 160431 && run <= 163869 && fabs(eleSCEta[a]) > 1.566 && eleE3x3[a]/eleSCRawEn[a] < 0.94)
          elePt[a] = elePt[a]*(1-0.0010);
        if (run >= 165071 && run <= 165970 && fabs(eleSCEta[a]) > 1.566 && eleE3x3[a]/eleSCRawEn[a] < 0.94)
          elePt[a] = elePt[a]*(1+0.0062);
        if (run >= 165971 && run <= 166502 && fabs(eleSCEta[a]) > 1.566 && eleE3x3[a]/eleSCRawEn[a] < 0.94)
          elePt[a] = elePt[a]*(1+0.0133);
        if (run >= 166503 && run <= 166861 && fabs(eleSCEta[a]) > 1.566 && eleE3x3[a]/eleSCRawEn[a] < 0.94)
          elePt[a] = elePt[a]*(1+0.0178);
      }

      ele_Index = -1;
      W_Index = -1;
      ele_Pt = 35;
      for (int a=0; a<nWenu; a++) {
        if (elePt[WenuEleIndex[a]] < ele_Pt) continue;
        if (fabs(eleSCEta[WenuEleIndex[a]]) > 2.5) continue;
	if (fabs(eleSCEta[WenuEleIndex[a]]) > 1.4442 && fabs(eleSCEta[WenuEleIndex[a]]) < 1.566) continue;
	if (fabs(eleSCEta[WenuEleIndex[a]]) < 1.4442 && eleSigmaIEtaIEta[WenuEleIndex[a]] < 0.001) continue;

	// Spike clean: kTime || kWeird || kBad
	if (eleSeverity[WenuEleIndex[a]] == 3 || eleSeverity[WenuEleIndex[a]] == 4 || eleSeverity[WenuEleIndex[a]] == 5) continue;

	// dZ and d0 cut
	if (fabs(elePVD0[WenuEleIndex[a]]) > 0.02) continue;
	if (fabs(elePVDz[WenuEleIndex[a]]) > 0.1) continue;

	// conversion
	if (eleConvMissinghit[WenuEleIndex[a]] != 0) continue;
	if (fabs(eleConvDist[WenuEleIndex[a]]) < 0.02 && fabs(eleConvDcot[WenuEleIndex[a]]) < 0.02) continue;

	// H2WW WP80 for barrel
	if (fabs(eleSCEta[WenuEleIndex[a]]) < 1.4442 &&
	    (max((float)0., eleIsoEcalDR03[WenuEleIndex[a]] - 1) + eleIsoHcalSolidDR03[WenuEleIndex[a]] + eleIsoTrkDR03[WenuEleIndex[a]] - rho*Pi*0.3*0.3)/elePt[WenuEleIndex[a]] > 0.04) continue;
	if (fabs(eleSCEta[WenuEleIndex[a]]) < 1.4442 && eleSigmaIEtaIEta[WenuEleIndex[a]] > 0.01) continue;
	if (fabs(eleSCEta[WenuEleIndex[a]]) < 1.4442 && fabs(eledPhiAtVtx[WenuEleIndex[a]]) > 0.027) continue;
	if (fabs(eleSCEta[WenuEleIndex[a]]) < 1.4442 && fabs(eledEtaAtVtx[WenuEleIndex[a]]) > 0.005) continue;
	// H2WW WP80 for endcap
	if (fabs(eleSCEta[WenuEleIndex[a]]) > 1.566 &&
	    (eleIsoEcalDR03[WenuEleIndex[a]] + eleIsoHcalSolidDR03[WenuEleIndex[a]] + eleIsoTrkDR03[WenuEleIndex[a]] - rho*Pi*0.3*0.3)/elePt[WenuEleIndex[a]] > 0.033) continue;
	if (fabs(eleSCEta[WenuEleIndex[a]]) > 1.566 && eleSigmaIEtaIEta[WenuEleIndex[a]] > 0.031) continue;
	if (fabs(eleSCEta[WenuEleIndex[a]]) > 1.566 && fabs(eledPhiAtVtx[WenuEleIndex[a]]) > 0.021) continue;
	if (fabs(eleSCEta[WenuEleIndex[a]]) > 1.566 && fabs(eledEtaAtVtx[WenuEleIndex[a]]) > 0.006) continue;

	if (run <= 161176 && eleTrg[WenuEleIndex[a]][10] != 1) continue;
	if (run >= 161217 && run <= 163261 && eleTrg[WenuEleIndex[a]][11] != 1) continue;
	if (run >= 163270 && run <= 163869 && eleTrg[WenuEleIndex[a]][12] != 1) continue;
	if (run >= 165088 && run <= 165633 && eleTrg[WenuEleIndex[a]][15] != 1) continue;
	if (run >= 165970 && run <= 166861 && eleTrg[WenuEleIndex[a]][16] != 1) continue;

	W_Index = a;
	ele_Pt = elePt[WenuEleIndex[a]];
      }
      if (W_Index == -1) continue;
      ele_Index = WenuEleIndex[W_Index];

      ele_num = 0;
      for (int a=0; a<nEle; a++) {
        if (a == WenuEleIndex[W_Index]) continue;
        if (elePt[a] < 20) continue;
	if (fabs(eleSCEta[a]) > 2.5) continue;
	if (fabs(eleSCEta[a]) > 1.4442 && fabs(eleSCEta[a]) < 1.566) continue;
	if (fabs(eleSCEta[a]) < 1.4442 && eleSigmaIEtaIEta[a] < 0.001) continue;

	// Spike clean: kTime || kWeird || kBad
	if (eleSeverity[a] == 3 || eleSeverity[a] == 4 || eleSeverity[a] == 5) continue;

	// conversion
	if (eleConvMissinghit[a] != 0) continue;

	// H2WW WP95 for barrel
	if (fabs(eleSCEta[a]) < 1.4442 &&
	    (max((float)0., eleIsoEcalDR03[a] - 1) + eleIsoHcalSolidDR03[a] + eleIsoTrkDR03[a] - rho*Pi*0.3*0.3)/elePt[a] > 0.15) continue;
	if (fabs(eleSCEta[a]) < 1.4442 && eleSigmaIEtaIEta[a] > 0.012) continue;
	if (fabs(eleSCEta[a]) < 1.4442 && fabs(eledPhiAtVtx[a]) > 0.8) continue;
	if (fabs(eleSCEta[a]) < 1.4442 && fabs(eledEtaAtVtx[a]) > 0.007) continue;
	// H2WW WP95 for endcap
	if (fabs(eleSCEta[a]) > 1.566 &&
	    (eleIsoEcalDR03[a] + eleIsoHcalSolidDR03[a] + eleIsoTrkDR03[a] - rho*Pi*0.3*0.3)/elePt[a] > 0.1) continue;
	if (fabs(eleSCEta[a]) > 1.566 && eleSigmaIEtaIEta[a] > 0.031) continue;
        if (fabs(eleSCEta[a]) > 1.566 && fabs(eledPhiAtVtx[a]) > 0.7) continue;
        if (fabs(eleSCEta[a]) > 1.566 && fabs(eledEtaAtVtx[a]) > 0.011) continue;

 	ele_num += 1;
      }
      if (ele_num != 0) continue;

      tree->Fill();

      if (fabs(eleSCEta[WenuEleIndex[W_Index]]) < 1.4442) 
	h1->Fill(elePt[WenuEleIndex[W_Index]]);
      else 
	h2->Fill(elePt[WenuEleIndex[W_Index]]);
      h3->Fill(eleEta[WenuEleIndex[W_Index]]);
      if (fabs(eleSCEta[WenuEleIndex[W_Index]]) < 1.4442) 
	h4->Fill(elePhi[WenuEleIndex[W_Index]]);
      else
	h5->Fill(elePhi[WenuEleIndex[W_Index]]);
      h6->Fill(tcMET);
      h7->Fill(pfMET);
      h8->Fill(WenuMassTTcMET[W_Index]);
      h9->Fill(WenuMassTPfMET[W_Index]);
      h10->Fill(nGoodVtx);

      if (pfMET < 25) continue;

      pho_Index = -1;
      pho_Pt = 15.;
      int phonum(0);
      for (int a=0; a<nPho; a++) {
        // Energy scale
        if (run >= 160431 && run <= 163869 && fabs(phoSCEta[a]) < 1.4442 && phoR9[a] > 0.94)
          phoEt[a] = phoEt[a]*(1-0.0047);
        if (run >= 165071 && run <= 165970 && fabs(phoSCEta[a]) < 1.4442 && phoR9[a] > 0.94)
          phoEt[a] = phoEt[a]*(1-0.0007);
        if (run >= 165971 && run <= 166502 && fabs(phoSCEta[a]) < 1.4442 && phoR9[a] > 0.94)
          phoEt[a] = phoEt[a]*(1+0.0003);
        if (run >= 166503 && run <= 166861 && fabs(phoSCEta[a]) < 1.4442 && phoR9[a] > 0.94)
          phoEt[a] = phoEt[a]*(1+0.0011);

        if (run >= 160431 && run <= 163869 && fabs(phoSCEta[a]) < 1.4442 && phoR9[a] < 0.94)
          phoEt[a] = phoEt[a]*(1+0.0025);
        if (run >= 165071 && run <= 165970 && fabs(phoSCEta[a]) < 1.4442 && phoR9[a] < 0.94)
          phoEt[a] = phoEt[a]*(1+0.0049);
        if (run >= 165971 && run <= 166502 && fabs(phoSCEta[a]) < 1.4442 && phoR9[a] < 0.94)
          phoEt[a] = phoEt[a]*(1+0.0067);
        if (run >= 166503 && run <= 166861 && fabs(phoSCEta[a]) < 1.4442 && phoR9[a] < 0.94)
          phoEt[a] = phoEt[a]*(1+0.0063);

	if (run >= 160431 && run <= 163869 && fabs(phoSCEta[a]) > 1.4442 && phoR9[a] > 0.94)
          phoEt[a] = phoEt[a]*(1+0.0058);
        if (run >= 165071 && run <= 165970 && fabs(phoSCEta[a]) > 1.4442 && phoR9[a] > 0.94)
          phoEt[a] = phoEt[a]*(1+0.0249);
        if (run >= 165971 && run <= 166502 && fabs(phoSCEta[a]) > 1.4442 && phoR9[a] > 0.94)
          phoEt[a] = phoEt[a]*(1+0.0376);
        if (run >= 166503 && run <= 166861 && fabs(phoSCEta[a]) > 1.4442 && phoR9[a] > 0.94)
          phoEt[a] = phoEt[a]*(1+0.0450);

        if (run >= 160431 && run <= 163869 && fabs(phoSCEta[a]) > 1.4442 && phoR9[a] < 0.94)
          phoEt[a] = phoEt[a]*(1-0.0010);
        if (run >= 165071 && run <= 165970 && fabs(phoSCEta[a]) > 1.4442 && phoR9[a] < 0.94)
          phoEt[a] = phoEt[a]*(1+0.0062);
        if (run >= 165971 && run <= 166502 && fabs(phoSCEta[a]) > 1.4442 && phoR9[a] < 0.94)
          phoEt[a] = phoEt[a]*(1+0.0133);
        if (run >= 166503 && run <= 166861 && fabs(phoSCEta[a]) > 1.4442 && phoR9[a] < 0.94)
          phoEt[a] = phoEt[a]*(1+0.0178);

	if (phoEt[a] < pho_Pt) continue;
        if (fabs(phoSCEta[a]) > 2.5) continue;
        if (fabs(phoSCEta[a]) > 1.4442 && fabs(phoSCEta[a]) < 1.566) continue;
	if (fabs(phoSCEta[a]) < 1.4442 && phoSigmaIEtaIEta[a] < 0.001) continue;

        if (phoHoverE[a] > 0.5) continue;
        if (phohasPixelSeed[a] == 1) continue;

	// Spike clean: kTime || kWeird || kBad
	if (phoSeverity[a] == 3 || phoSeverity[a] == 4 || phoSeverity[a] == 5) continue;

	dEta = phoEta[a] - eleEta[WenuEleIndex[W_Index]];
	dPhi = phoPhi[a] - elePhi[WenuEleIndex[W_Index]];
	if (dPhi >  3.1415927) dPhi -= 2*3.1415927;
	if (dPhi < -3.1415927) dPhi += 2*3.1415927;
	if (sqrt(pow(dEta,2)+pow(dPhi,2)) < 0.7) continue;

        // Require photon selection, satisfying isTight
	if (fabs(phoSCEta[a]) < 1.4442 && (phoEcalIsoDR04[a]-0.006*phoEt[a]-0.183*rho) > 4.2) continue;
        if (fabs(phoSCEta[a]) < 1.4442 && (phoHcalIsoDR04[a]-0.0025*phoEt[a]-0.062*rho) > 2.2) continue;
        if (fabs(phoSCEta[a]) < 1.4442 && (phoTrkIsoHollowDR04[a]-0.001*phoEt[a]-0.167*rho) > 2) continue;
        if (fabs(phoSCEta[a]) > 1.566 && (phoEcalIsoDR04[a]-0.006*phoEt[a]-0.090*rho) > 4.2) continue;
        if (fabs(phoSCEta[a]) > 1.566 && (phoHcalIsoDR04[a]-0.0025*phoEt[a]-0.180*rho) > 2.2) continue;
        if (fabs(phoSCEta[a]) > 1.566 && (phoTrkIsoHollowDR04[a]-0.001*phoEt[a]-0.032*rho) > 2) continue;
        if (phoHoverE[a] > 0.05) continue;
        if (fabs(phoSCEta[a]) < 1.4442 && phoSigmaIEtaIEta[a] > 0.011) continue;
        if (fabs(phoSCEta[a]) > 1.566 && phoSigmaIEtaIEta[a] > 0.03) continue;

	phonum += 1;

        pho_Index = a;
        pho_Pt = phoEt[a];
      }
      if (pho_Index == -1) continue;

      dEta = phoEta[pho_Index] - eleEta[WenuEleIndex[W_Index]];
      dPhi = phoPhi[pho_Index] - elePhi[WenuEleIndex[W_Index]];
      if (dPhi >  3.1415927) dPhi -= 2*3.1415927;
      if (dPhi < -3.1415927) dPhi += 2*3.1415927;
      dR = sqrt(pow(dEta,2)+pow(dPhi,2));
      Wgamma_candidates += 1;

      if (run <= 161176)
        g3->Fill(run, HLTprescale[HLTIndex[111]]);
      else if (run >= 161217 && run <= 163261)
        g3->Fill(run, HLTprescale[HLTIndex[112]]);
      else if (run >= 163270 && run <= 163869)
        g3->Fill(run, HLTprescale[HLTIndex[113]]);
      else if (run >= 165088 && run <= 165633)
        g3->Fill(run, HLTprescale[HLTIndex[116]]);
      else if (run >= 165970 && run <= 166967)
        g3->Fill(run, HLTprescale[HLTIndex[117]]);
      else if (run >= 167039 && run <= 167151)
        g3->Fill(run, HLTprescale[HLTIndex[190]]);

      g4->Fill(phonum);

      TLorentzVector LEle,LPho;
      TLorentzVector LZep;
      LPho.SetPxPyPzE(phoEt[pho_Index]*cos(phoPhi[pho_Index]),phoEt[pho_Index]*sin(phoPhi[pho_Index]),phoPz[pho_Index],phoE[pho_Index]);
      LEle.SetPxPyPzE(elePt[WenuEleIndex[W_Index]]*cos(elePhi[WenuEleIndex[W_Index]]),elePt[WenuEleIndex[W_Index]]*sin(elePhi[WenuEleIndex[W_Index]]),elePz[WenuEleIndex[W_Index]],eleEn[WenuEleIndex[W_Index]]);
      LZep = LPho + LEle;

      // Calculate neutrino Pz and then Wgamma mass
      double MW = 80.4;
      double Y = pow(MW,2) + 2.*elePt[WenuEleIndex[W_Index]]*pfMET*cos(pfMETPhi - elePhi[WenuEleIndex[W_Index]]);
      double A = 4.*pow(elePt[WenuEleIndex[W_Index]], 2);
      double B = 4.*fabs(elePz[WenuEleIndex[W_Index]])*Y;
      double C = 4.*(pow(elePt[WenuEleIndex[W_Index]], 2) + pow(elePz[WenuEleIndex[W_Index]], 2))*pow(pfMET, 2) - pow(Y, 2);
      double BAC = sqrt(B*B - 4.*A*C);

      // Ez_nu is in the oposite direction to Pz_mu
      double Ez_nu_1 = (-B + BAC)/(2.*A);
      double Ez_nu_2 = (-B - BAC)/(2.*A);
      // Ez_nu is in the same direction to Pz_mu
      double Ez_nu_3 = (B + BAC)/(2.*A);
      double Ez_nu_4 = (B - BAC)/(2.*A);

      double E_nu_1 = sqrt(pfMETx*pfMETx + pfMETy*pfMETy + Ez_nu_1*Ez_nu_1);
      double E_nu_2 = sqrt(pfMETx*pfMETx + pfMETy*pfMETy + Ez_nu_2*Ez_nu_2);
      double E_nu_3 = sqrt(pfMETx*pfMETx + pfMETy*pfMETy + Ez_nu_3*Ez_nu_3);
      double E_nu_4 = sqrt(pfMETx*pfMETx + pfMETy*pfMETy + Ez_nu_4*Ez_nu_4);

      double sign = 0;
      if (elePz[WenuEleIndex[W_Index]] <=0)
        sign = -1;
      else
        sign = 1;

      TLorentzVector LNu_1, LNu_2, LNu_3, LNu_4;
      LNu_1.SetPxPyPzE(pfMETx, pfMETy, -Ez_nu_1*sign, E_nu_1);
      LNu_2.SetPxPyPzE(pfMETx, pfMETy, -Ez_nu_2*sign, E_nu_2);
      LNu_3.SetPxPyPzE(pfMETx, pfMETy, Ez_nu_3*sign, E_nu_3);
      LNu_4.SetPxPyPzE(pfMETx, pfMETy, Ez_nu_4*sign, E_nu_4);

      TLorentzVector LW_1, LW_2, LW_3, LW_4;
      LW_1 = LEle + LNu_1;
      LW_2 = LEle + LNu_2;
      LW_3 = LEle + LNu_3;
      LW_4 = LEle + LNu_4;

      TLorentzVector LWG_1, LWG_2, LWG_3, LWG_4;
      LWG_1 = LW_1 + LPho;
      LWG_2 = LW_2 + LPho;
      LWG_3 = LW_3 + LPho;
      LWG_4 = LW_4 + LPho;
      h65->Fill(LWG_1.M());
      h66->Fill(LWG_2.M());
      h67->Fill(LWG_3.M());
      h68->Fill(LWG_4.M());

      float first(0);
      float second(0);

      first = pow((sqrt(pow(LZep.M(), 2) + pow(phoEt[pho_Index]*cos(phoPhi[pho_Index]) + elePt[WenuEleIndex[W_Index]]*cos(elePhi[WenuEleIndex[W_Index]]) ,2) + pow(phoEt[pho_Index]*sin(phoPhi[pho_Index]) + elePt[WenuEleIndex[W_Index]]*sin(elePhi[WenuEleIndex[W_Index]]), 2)) + tcMET), 2);
      second = pow(phoEt[pho_Index]*cos(phoPhi[pho_Index]) + elePt[WenuEleIndex[W_Index]]*cos(elePhi[WenuEleIndex[W_Index]]) + tcMETx, 2) + pow(phoEt[pho_Index]*sin(phoPhi[pho_Index]) + elePt[WenuEleIndex[W_Index]]*sin(elePhi[WenuEleIndex[W_Index]]) + tcMETy, 2);
      CTM[0] = sqrt(first - second);

      first = 0;
      second = 0;
      first = pow((sqrt(pow(LZep.M(), 2) + pow(phoEt[pho_Index]*cos(phoPhi[pho_Index]) + elePt[WenuEleIndex[W_Index]]*cos(elePhi[WenuEleIndex[W_Index]]) ,2) + pow(phoEt[pho_Index]*sin(phoPhi[pho_Index]) + elePt[WenuEleIndex[W_Index]]*sin(elePhi[WenuEleIndex[W_Index]]), 2)) + pfMET), 2);
      second = pow(phoEt[pho_Index]*cos(phoPhi[pho_Index]) + elePt[WenuEleIndex[W_Index]]*cos(elePhi[WenuEleIndex[W_Index]]) + pfMETx, 2) + pow(phoEt[pho_Index]*sin(phoPhi[pho_Index]) + elePt[WenuEleIndex[W_Index]]*sin(elePhi[WenuEleIndex[W_Index]]) + pfMETy, 2);
      CTM[1] = sqrt(first - second);

      if (fabs(eleSCEta[WenuEleIndex[W_Index]]) < 1.4442)
	h11->Fill(elePt[WenuEleIndex[W_Index]]);
      else 
	h12->Fill(elePt[WenuEleIndex[W_Index]]);
      h13->Fill(eleEta[WenuEleIndex[W_Index]]);
      if (fabs(eleSCEta[WenuEleIndex[W_Index]]) < 1.4442)
	h14->Fill(elePhi[WenuEleIndex[W_Index]]);
      else 
	h15->Fill(elePhi[WenuEleIndex[W_Index]]);
      h16->Fill(tcMET);
      h17->Fill(pfMET);
      h18->Fill(WenuMassTTcMET[W_Index]);
      h19->Fill(WenuMassTPfMET[W_Index]);

      if (fabs(phoSCEta[pho_Index]) < 1.4442)
	h20->Fill(phoEt[pho_Index]);
      else 
	h21->Fill(phoEt[pho_Index]);
      h22->Fill(phoEta[pho_Index]);
      if (fabs(phoSCEta[pho_Index]) < 1.4442)
	h23->Fill(phoPhi[pho_Index]);
      else
	h24->Fill(phoPhi[pho_Index]);
      h25->Fill(WenuMassTTcMET[W_Index], CTM[0]);
      h26->Fill(WenuMassTPfMET[W_Index], CTM[1]);
      if (fabs(phoSCEta[pho_Index]) < 1.4442)
	h27->Fill(LZep.M());
      else
	h28->Fill(LZep.M());
      h29->Fill(nGoodVtx);
      h30->Fill(dR);

      if (fabs(phoSCEta[pho_Index]) < 1.4442)
        h50->Fill(phoSeedTime[pho_Index]);
      else
        h51->Fill(phoSeedTime[pho_Index]);

      if (CTM[1] > 110) {
	Wgamma_MT3_candidates += 1;

	if (fabs(eleSCEta[WenuEleIndex[W_Index]]) < 1.4442)
          h31->Fill(elePt[WenuEleIndex[W_Index]]);
        else
          h32->Fill(elePt[WenuEleIndex[W_Index]]);
        h33->Fill(eleEta[WenuEleIndex[W_Index]]);
        if (fabs(eleSCEta[WenuEleIndex[W_Index]]) < 1.4442)
          h34->Fill(elePhi[WenuEleIndex[W_Index]]);
        else
          h35->Fill(elePhi[WenuEleIndex[W_Index]]);
        h36->Fill(tcMET);
        h37->Fill(pfMET);
        h38->Fill(WenuMassTTcMET[W_Index]);
        h39->Fill(WenuMassTPfMET[W_Index]);
  
	if (fabs(phoSCEta[pho_Index]) < 1.4442)
          h40->Fill(phoEt[pho_Index]);
        else
          h41->Fill(phoEt[pho_Index]);
        h42->Fill(phoEta[pho_Index]);
        if (fabs(phoSCEta[pho_Index]) < 1.4442)
          h43->Fill(phoPhi[pho_Index]);
        else
          h44->Fill(phoPhi[pho_Index]);
        if (fabs(phoSCEta[pho_Index]) < 1.4442)
          h45->Fill(LZep.M());
        else
          h46->Fill(LZep.M());

        if (fabs(phoSCEta[pho_Index]) < 1.4442)
	  h47->Fill(eleCharge[WenuEleIndex[W_Index]]*(phoEta[pho_Index] - eleEta[WenuEleIndex[W_Index]]));
	else
	  h48->Fill(eleCharge[WenuEleIndex[W_Index]]*(phoEta[pho_Index] - eleEta[WenuEleIndex[W_Index]]));
	h49->Fill(eleCharge[WenuEleIndex[W_Index]]*(phoEta[pho_Index] - eleEta[WenuEleIndex[W_Index]]));
        h52->Fill(nGoodVtx);
	h53->Fill(dR);
	h75->Fill(LWG_1.M());
	h76->Fill(LWG_2.M());
	h77->Fill(LWG_3.M());
	h78->Fill(LWG_4.M());
      }

      if (fabs(phoSCEta[pho_Index]) < 1.4442)
	g1->Fill(phoEt[pho_Index], phoSigmaIEtaIEta[pho_Index]);
      else
	g2->Fill(phoEt[pho_Index], phoSigmaIEtaIEta[pho_Index]);
      
      char text[200];
      sprintf(text, "Run = %d; Event = %d; Lumis = %d; WGamma Candidate = %d; MT3 > 90 GeV = %d", run, event, lumis, Wgamma_candidates, Wgamma_MT3_candidates);
      cout<<text<<endl;
 
      cout<<"   Triger info: "<<endl;
      if (HLTIndex[111] != -1 && HLT[HLTIndex[111]] == 1) cout<<"      HLT info: HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1; Prescale = "<<HLTprescale[HLTIndex[111]]<<endl;
      if (HLTIndex[112] != -1 && HLT[HLTIndex[112]] == 1) cout<<"      HLT info: HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2; Prescale = "<<HLTprescale[HLTIndex[112]]<<endl;
      if (HLTIndex[113] != -1 && HLT[HLTIndex[113]] == 1) cout<<"      HLT info: HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3; Prescale = "<<HLTprescale[HLTIndex[113]]<<endl;
      if (HLTIndex[114] != -1 && HLT[HLTIndex[114]] == 1) cout<<"      HLT info: HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1; Prescale = "<<HLTprescale[HLTIndex[114]]<<endl;
      if (HLTIndex[115] != -1 && HLT[HLTIndex[115]] == 1) cout<<"      HLT info: HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2; Prescale = "<<HLTprescale[HLTIndex[115]]<<endl;
      if (HLTIndex[116] != -1 && HLT[HLTIndex[116]] == 1) cout<<"      HLT info: HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3; Prescale = "<<HLTprescale[HLTIndex[116]]<<endl;
      if (HLTIndex[117] != -1 && HLT[HLTIndex[117]] == 1) cout<<"      HLT info: HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v4; Prescale = "<<HLTprescale[HLTIndex[117]]<<endl;
      if (HLTIndex[189] != -1 && HLT[HLTIndex[189]] == 1) cout<<"      HLT info: HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v5; Prescale = "<<HLTprescale[HLTIndex[189]]<<endl;
      if (HLTIndex[190] != -1 && HLT[HLTIndex[190]] == 1) cout<<"      HLT info: HLT_Ele27_WP80_PFMT50_v1; Prescale = "<<HLTprescale[HLTIndex[190]]<<endl;

      cout<<"   MET info: "<<endl;
      sprintf(text, "      tcMET = %5.1f; pfMET = %5.1f", tcMET, pfMET);
      cout<<text<<endl;
      cout<<"   W info: "<<endl;
      sprintf(text, "      tcMT = %5.2f; pfMT = %5.2f", WenuMassTTcMET[W_Index], WenuMassTPfMET[W_Index]);
      cout<<text<<endl;
      sprintf(text, "      tcCTM = %5.2f; pfCTM = %5.2f", CTM[0], CTM[1]);
      cout<<text<<endl;
      cout<<"   Ele info: "<<endl;
      sprintf(text, "      Pt = %5.2f; Eta = %5.2f; Phi = %5.2f; SCEta = %5.2f", elePt[WenuEleIndex[W_Index]], eleEta[WenuEleIndex[W_Index]], elePhi[WenuEleIndex[W_Index]], eleSCEta[WenuEleIndex[W_Index]]);
      cout<<text<<endl;
      sprintf(text, "      EIso = %5.2f; HIso = %5.2f; TIso = %5.2f", eleIsoEcalDR03[WenuEleIndex[W_Index]], eleIsoHcalSolidDR03[WenuEleIndex[W_Index]], eleIsoTrkDR03[WenuEleIndex[W_Index]]);
      cout<<text<<endl;
      sprintf(text, "      dEta = %5.3f; dPhi = %5.3f; SigmaIEtaIEta = %5.3f", eledEtaAtVtx[WenuEleIndex[W_Index]], eledPhiAtVtx[WenuEleIndex[W_Index]], eleSigmaIEtaIEta[WenuEleIndex[W_Index]]);
      cout<<text<<endl;

      cout<<"   Pho info: "<<endl;
      sprintf(text, "      Pt = %5.2f; Eta = %5.2f; Phi = %5.2f; SCEta = %5.2f", phoEt[pho_Index], phoEta[pho_Index], phoPhi[pho_Index], phoSCEta[pho_Index]);
      cout<<text<<endl;
      sprintf(text, "      EIso = %5.2f; HIso = %5.2f; TIso(Hollow) = %5.2f; TIso(Solid) = %5.2f", phoEcalIsoDR04[pho_Index], phoHcalIsoDR04[pho_Index], phoTrkIsoHollowDR04[pho_Index], phoTrkIsoSolidDR04[pho_Index]);
      cout<<text<<endl;
      sprintf(text, "      SigmaIEtaIEta = %5.3f; HoverE = %5.3f; deltaR = %5.3f; R9 = %5.2f", phoSigmaIEtaIEta[pho_Index], phoHoverE[pho_Index], dR, phoR9[pho_Index]);
      cout<<text<<endl;
      cout<<"      Seed Timing = "<<phoSeedTime[pho_Index]<<endl;
      cout<<" ------------------------------------------------ "<<endl;
   }

   file->Write();
   file->Close();
}
