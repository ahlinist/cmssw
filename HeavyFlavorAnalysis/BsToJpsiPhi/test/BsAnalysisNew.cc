#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include <string>
#include <vector>
#include <iostream>

// ***************main function*****************************
void BsAnalysisNew() {
  

  TString filename = "JpsiX/JpsiX.root";
  //  TString filename = "JpsiX_pre/bJpsiX.root";
  //  TString filename = "promptJpsi/promptJpsi.root";

  TFile *f = new TFile(filename);
  TTree *tree = (TTree*)f->Get("BsTree");

  TString outputhistofilename = "bJpsiX_histo.root";
  //  TString outputhistofilename = "bJpsiX_pre_histo.root";
  //  TString outputhistofilename = "promptJpsi_histo.root";


  int triggerbit_HLTmu3_;
  int triggerbit_HLTmu5_;
  int triggerbit_HLTmu9_;
  int triggerbit_HLTdoubleIsoMu3_;
  int triggerbit_HLTdoubleMu3_;
  int triggerbit_HLTdoubleMu3_JPsi_;
  
  int momID_;
  int nBsDau_;
  int dau_1_ID_;
  int dau_2_ID_;

  double BSx_ ;
  double BSy_ ;
  double BSz_ ;
  double PVx_ ;
  double PVy_ ;
  double PVz_ ;
  double PVerrx_ ;
  double PVerry_ ;
  double PVerrz_ ;

  double JpsiVtxProb_;
  int MuCounter1_;
  int MuCounter2_;
  double JpsiM_alone_;
  double JpsiPhi_alone_;
  double JpsiEta_alone_;
  double JpsiPt_alone_;
  double JpsiMu1Pt_;
  double JpsiMu2Pt_;
  double JpsiMu1Phi_;
  double JpsiMu2Phi_;
  double JpsiMu1Eta_;
  double JpsiMu2Eta_;
  int JpsiMuon1Cat_;
  int JpsiMuon2Cat_;

  double BsMass_before_;
  double BsPhi_before_;
  double BsEta_before_;
  double BsPt_before_;
  double BsPz_before_;

  double JpsiMass_before_;
  double JpsiPhi_before_;
  double JpsiEta_before_;
  double JpsiPt_before_;
  double JpsiPz_before_;

  double PhiMass_before_;
  double PhiPhi_before_;
  double PhiEta_before_;
  double PhiPt_before_;
  double PhiPz_before_;

  double  K1Pt_before_;
  double  K1Pz_before_;
  double  K1Eta_before_;
  double  K1Phi_before_;
  double  K2Eta_before_;
  double  K2Pt_before_;
  double  K2Pz_before_;
  double  K2Phi_before_;

  double chi2_Bs_;
  int ndof_Bs_;

  double BsVtxProb_;
  double BsVtxProbKpi_;
  double BsVtxProbpipi_;

  double BfitM_KK_;
  double BfitM_Kpi_;
  double BfitM_pipi_;

  double BsMass_after_;
  double BsPhi_after_;
  double BsEta_after_;
  double BsPt_after_;
  double BsPz_after_;

  double JpsiMass_after_;
  double JpsiPhi_after_;
  double JpsiEta_after_;
  double JpsiPt_after_;
  double JpsiPz_after_;

  double PhiMass_after_;
  double PhiPhi_after_;
  double PhiEta_after_;
  double PhiPt_after_;
  double PhiPz_after_;

  double  K1Pt_after_;
  double  K1Pz_after_;
  double  K1Eta_after_;
  double  K1Phi_after_;
  double  K2Eta_after_;
  double  K2Pt_after_;
  double  K2Pz_after_;
  double  K2Phi_after_;

  double  K1Chi2_;
  double     K1nHits_;
  double  K2Chi2_;
  double     K2nHits_;
  double  K1pixH_;
  double     K1trkH_;
  double     K2pixH_;
  double     K2trkH_;

  double  Mu1Chi2_;
  double     Mu1nHits_;
  double  Mu2Chi2_;
  double     Mu2nHits_;
  double  Mu1pixH_;
  double     Mu1trkH_;
  double     Mu2pixH_;
  double     Mu2trkH_;
  
  double costheta_;
  double phi_;
  double cospsi_;
  double AngleBsDecayLength_;

  int isMatched_;

  double BmassC_;
  double BLxy_;
  double BLxy2_;
  double BerrX_;
  double BerrY_;
  double BerrXY_;
  double Bsct1_;
  double Bsct2_;

  int     K1trkLay_;
  int     K1muDTh_;
  int     K1muCSCh_;
  int     K1muRPCh_;
  int     K2trkLay_;
  int     K2muDTh_;
  int     K2muCSCh_;
  int     K2muRPCh_;
  int     Mu1trkLay_;
  int     Mu1muDTh_;
  int     Mu1muCSCh_;
  int     Mu1muRPCh_;
  int     Mu2trkLay_;
  int     Mu2muDTh_;
  int     Mu2muCSCh_;
  int     Mu2muRPCh_;

  int K1mcId_;
  int K1momId_;
  int K1gmomId_;
  int K2mcId_;
  int K2momId_;
  int K2gmomId_;
  int Mu1mcId_;
  int Mu1momId_;
  int Mu1gmomId_;
  int Mu2mcId_;
  int Mu2momId_;
  int Mu2gmomId_;
  int K1Truth_;
  int K2Truth_;
  int Mu1Truth_;
  int Mu2Truth_;

  double Dist3d_;
  double dDist3d_;
  double Time3d_;
  double dTime3d_;
  double Dist_;
  double dDist_;
  double Time_;
  double dTime_;

  double dedxTrk_;
  double errdedxTrk_;
  int numdedxTrk_;

  int iPassedCutIdent_;

  //MC info
  int isGenBsEvent_;
  int GenNumberOfBsDaughters_;
  int isGenBsJpsiPhiEvent_;
  int isGenBsJpsiPhiMuMuKKEvent_;
  int isGenBdEvent_;
  int GenNumberOfBdDaughters_;
  int isGenBdJpsiKstarEvent_;
  int isGenBdJpsiKstarMuMuKpiEvent_;

  double genBsM_, genBsPt_, genBsEta_, genBsPhi_;
  double genJpsiM_, genJpsiPt_, genJpsiEta_, genJpsiPhi_;
  double genPhiM_, genPhiPt_, genPhiEta_, genPhiPhi_;
  double genMu1M_, genMu1Pt_, genMu1Eta_, genMu1Phi_;
  double genMu2M_, genMu2Pt_, genMu2Eta_, genMu2Phi_;
  double genK1M_, genK1Pt_, genK1Eta_, genK1Phi_;
  double genK2M_, genK2Pt_, genK2Eta_, genK2Phi_;
  double genBsPz_, genJpsiPz_, genPhiPz_, genMu1Pz_, genMu2Pz_, genK1Pz_, genK2Pz_;

  double K1_kk_par0_;
  double K1_kk_par1_;
  double K1_kk_par2_;
  double K1_kk_par3_;
  double K1_kk_par4_;
  double K1_kk_par5_;
  double K1_kk_par6_;

  double K2_kk_par0_;
  double K2_kk_par1_;
  double K2_kk_par2_;
  double K2_kk_par3_;
  double K2_kk_par4_;
  double K2_kk_par5_;
  double K2_kk_par6_;

  double K1_kpi_par0_;
  double K1_kpi_par1_;
  double K1_kpi_par2_;
  double K1_kpi_par3_;
  double K1_kpi_par4_;
  double K1_kpi_par5_;
  double K1_kpi_par6_;

  double K2_kpi_par0_;
  double K2_kpi_par1_;
  double K2_kpi_par2_;
  double K2_kpi_par3_;
  double K2_kpi_par4_;
  double K2_kpi_par5_;
  double K2_kpi_par6_;

  double K1_pipi_par0_;
  double K1_pipi_par1_;
  double K1_pipi_par2_;
  double K1_pipi_par3_;
  double K1_pipi_par4_;
  double K1_pipi_par5_;
  double K1_pipi_par6_;

  double K2_pipi_par0_;
  double K2_pipi_par1_;
  double K2_pipi_par2_;
  double K2_pipi_par3_;
  double K2_pipi_par4_;
  double K2_pipi_par5_;
  double K2_pipi_par6_;

  double K1_kk_sigX_;
  double K1_kk_sigY_;
  double K1_kk_sigZ_;

  double K1_kpi_sigX_;
  double K1_kpi_sigY_;
  double K1_kpi_sigZ_;

  double K1_pipi_sigX_;
  double K1_pipi_sigY_;
  double K1_pipi_sigZ_;

  double K2_kk_sigX_;
  double K2_kk_sigY_;
  double K2_kk_sigZ_;

  double K2_kpi_sigX_;
  double K2_kpi_sigY_;
  double K2_kpi_sigZ_;

  double K2_pipi_sigX_;
  double K2_pipi_sigY_;
  double K2_pipi_sigZ_;

  double K1_kk_sigPX_;
  double K1_kk_sigPY_;
  double K1_kk_sigPZ_;

  double K1_kpi_sigPX_;
  double K1_kpi_sigPY_;
  double K1_kpi_sigPZ_;

  double K1_pipi_sigPX_;
  double K1_pipi_sigPY_;
  double K1_pipi_sigPZ_;

  double K2_kk_sigPX_;
  double K2_kk_sigPY_;
  double K2_kk_sigPZ_;

  double K2_kpi_sigPX_;
  double K2_kpi_sigPY_;
  double K2_kpi_sigPZ_;

  double K2_pipi_sigPX_;
  double K2_pipi_sigPY_;
  double K2_pipi_sigPZ_;

  double K1Pt_error_;
  double K2Pt_error_;

  int isGenBsJpsiKKEvent_;
  int BdDausId_;

  double BsVtxProb_;

  // **************

  tree->SetBranchAddress("triggerbit_HLTmu3", &triggerbit_HLTmu3_);
  tree->SetBranchAddress("triggerbit_HLTmu5", &triggerbit_HLTmu5_);
  tree->SetBranchAddress("triggerbit_HLTmu9", &triggerbit_HLTmu9_);
  tree->SetBranchAddress("triggerbit_HLTdoubleIsoMu3", &triggerbit_HLTdoubleIsoMu3_);
  tree->SetBranchAddress("triggerbit_HLTdoubleMu3", &triggerbit_HLTdoubleMu3_);
  tree->SetBranchAddress("triggerbit_HLTdoubleMu3_JPsi", &triggerbit_HLTdoubleMu3_JPsi_);
  tree->SetBranchAddress("nBsDau", &nBsDau_);
  tree->SetBranchAddress("momID", &momID_);
  tree->SetBranchAddress("dau_1_ID", &dau_1_ID_);
  tree->SetBranchAddress("dau_2_ID", &dau_2_ID_);
  
  tree->SetBranchAddress("BeamspotX", &BSx_);
  tree->SetBranchAddress("BeamspotY", &BSy_);
  tree->SetBranchAddress("BeamspotZ", &BSz_);
  tree->SetBranchAddress("PVx", &PVx_);
  tree->SetBranchAddress("PVy", &PVy_);
  tree->SetBranchAddress("PVz", &PVz_);
  tree->SetBranchAddress("PVerrx", &PVerrx_);
  tree->SetBranchAddress("PVerry", &PVerry_);
  tree->SetBranchAddress("PVerrz", &PVerrz_);
  
  tree->SetBranchAddress("JpsiVtxProb", &JpsiVtxProb_);
  tree->SetBranchAddress("JpsiM_alone", &JpsiM_alone_);
  tree->SetBranchAddress("JpsiPhi_alone", &JpsiPhi_alone_);
  tree->SetBranchAddress("JpsiEta_alone", &JpsiEta_alone_);
  tree->SetBranchAddress("JpsiPt_alone", &JpsiPt_alone_);
  tree->SetBranchAddress("JpsiMu1Pt", &JpsiMu1Pt_);
  tree->SetBranchAddress("JpsiMu2Pt", &JpsiMu2Pt_);
  tree->SetBranchAddress("JpsiMu1Phi", &JpsiMu1Phi_);
  tree->SetBranchAddress("JpsiMu2Phi", &JpsiMu2Phi_);
  tree->SetBranchAddress("JpsiMu1Eta", &JpsiMu1Eta_);
  tree->SetBranchAddress("JpsiMu2Eta", &JpsiMu2Eta_);
  tree->SetBranchAddress("JpsiMuon1Cat", &JpsiMuon1Cat_);
  tree->SetBranchAddress("JpsiMuon2Cat", &JpsiMuon2Cat_);

  tree->SetBranchAddress("BsMass_before", &BsMass_before_);
  tree->SetBranchAddress("BsPhi_before", &BsPhi_before_);
  tree->SetBranchAddress("BsEta_before", &BsEta_before_);
  tree->SetBranchAddress("BsPt_before", &BsPt_before_);
  tree->SetBranchAddress("BsPz_before", &BsPz_before_);

  tree->SetBranchAddress("JpsiMass_before", &JpsiMass_before_);
  tree->SetBranchAddress("JpsiPhi_before", &JpsiPhi_before_);
  tree->SetBranchAddress("JpsiEta_before", &JpsiEta_before_);
  tree->SetBranchAddress("JpsiPt_before", &JpsiPt_before_);
  tree->SetBranchAddress("JpsiPz_before", &JpsiPz_before_);

  tree->SetBranchAddress("PhiMass_before", &PhiMass_before_);
  tree->SetBranchAddress("PhiPhi_before", &PhiPhi_before_);
  tree->SetBranchAddress("PhiEta_before", &PhiEta_before_);
  tree->SetBranchAddress("PhiPt_before", &PhiPt_before_);
  tree->SetBranchAddress("PhiPz_before", &PhiPz_before_);

  tree->SetBranchAddress("K1Phi_before", &K1Phi_before_);
  tree->SetBranchAddress("K1Eta_before", &K1Eta_before_);
  tree->SetBranchAddress("K1Pt_before", &K1Pt_before_);
  tree->SetBranchAddress("K1Pz_before", &K1Pz_before_);
  tree->SetBranchAddress("K2Phi_before", &K2Phi_before_);
  tree->SetBranchAddress("K2Eta_before", &K2Eta_before_);
  tree->SetBranchAddress("K2Pt_before", &K2Pt_before_);
  tree->SetBranchAddress("K2Pz_before", &K2Pz_before_);

  tree->SetBranchAddress("BsVtxProb", &BsVtxProb_);
  tree->SetBranchAddress("BsVtxProbKpi", &BsVtxProbKpi_);
  tree->SetBranchAddress("BsVtxProbpipi", &BsVtxProbpipi_);

  tree->SetBranchAddress("chi2_Bs", &chi2_Bs_);
  tree->SetBranchAddress("ndof_Bs", &ndof_Bs_);

  tree->SetBranchAddress("BFitM_KK", &BfitM_KK_);
  tree->SetBranchAddress("BFitM_Kpi", &BfitM_Kpi_);
  tree->SetBranchAddress("BFitM_pipi", &BfitM_pipi_);

  tree->SetBranchAddress("BsMass_after", &BsMass_after_);
  tree->SetBranchAddress("BsPhi_after", &BsPhi_after_);
  tree->SetBranchAddress("BsEta_after", &BsEta_after_);
  tree->SetBranchAddress("BsPt_after", &BsPt_after_);
  tree->SetBranchAddress("BsPz_after", &BsPz_after_);

  tree->SetBranchAddress("JpsiMass_after", &JpsiMass_after_);
  tree->SetBranchAddress("JpsiPhi_after", &JpsiPhi_after_);
  tree->SetBranchAddress("JpsiEta_after", &JpsiEta_after_);
  tree->SetBranchAddress("JpsiPt_after", &JpsiPt_after_);
  tree->SetBranchAddress("JpsiPz_after", &JpsiPz_after_);

  tree->SetBranchAddress("PhiMass_after", &PhiMass_after_);
  tree->SetBranchAddress("PhiPhi_after", &PhiPhi_after_);
  tree->SetBranchAddress("PhiEta_after", &PhiEta_after_);
  tree->SetBranchAddress("PhiPt_after", &PhiPt_after_);
  tree->SetBranchAddress("PhiPz_after", &PhiPz_after_);

  tree->SetBranchAddress("K1Phi_after", &K1Phi_after_);
  tree->SetBranchAddress("K1Eta_after", &K1Eta_after_);
  tree->SetBranchAddress("K1Pt_after", &K1Pt_after_);
  tree->SetBranchAddress("K1Pz_after", &K1Pz_after_);
  tree->SetBranchAddress("K2Phi_after", &K2Phi_after_);
  tree->SetBranchAddress("K2Eta_after", &K2Eta_after_);
  tree->SetBranchAddress("K2Pt_after", &K2Pt_after_);
  tree->SetBranchAddress("K2Pz_after", &K2Pz_after_);

  tree->SetBranchAddress("BLxy", &BLxy_);
  tree->SetBranchAddress("BLxy2", &BLxy2_);
  tree->SetBranchAddress("BerrX", &BerrX_);
  tree->SetBranchAddress("BerrY", &BerrY_);
  tree->SetBranchAddress("BerrXY", &BerrXY_);
  tree->SetBranchAddress("Bsct1", &Bsct1_);
  tree->SetBranchAddress("Bsct2", &Bsct2_);

  tree->SetBranchAddress("K1Chi2", &K1Chi2_);
  tree->SetBranchAddress("K1nHits", &K1nHits_);
  tree->SetBranchAddress("K1pixH", &K1pixH_);
  tree->SetBranchAddress("K1trkH", &K1trkH_);

  tree->SetBranchAddress("K2Chi2", &K2Chi2_);
  tree->SetBranchAddress("K2nHits", &K2nHits_);
  tree->SetBranchAddress("K2pixH", &K2pixH_);
  tree->SetBranchAddress("K2trkH", &K2trkH_);
  
  tree->SetBranchAddress("Mu1Chi2", &Mu1Chi2_);
  tree->SetBranchAddress("Mu1nHits", &Mu1nHits_);
  tree->SetBranchAddress("Mu1pixH", &Mu1pixH_);
  tree->SetBranchAddress("Mu1trkH", &Mu1trkH_);
  tree->SetBranchAddress("Mu2nHits", &Mu2nHits_);
  tree->SetBranchAddress("Mu2pixH", &Mu2pixH_);
  tree->SetBranchAddress("Mu2trkH", &Mu2trkH_);
  
  tree->SetBranchAddress("K1trkLay", &K1trkLay_);
  tree->SetBranchAddress("K1muDTh", &K1muDTh_);
  tree->SetBranchAddress("K1muCSCh", &K1muCSCh_);
  tree->SetBranchAddress("K1muRPCh", &K1muRPCh_);
  tree->SetBranchAddress("K2trkLay", &K2trkLay_);
  tree->SetBranchAddress("K2muDTh", &K2muDTh_);
  tree->SetBranchAddress("K2muCSCh", &K2muCSCh_);
  tree->SetBranchAddress("K2muRPCh", &K2muRPCh_);
  tree->SetBranchAddress("Mu1trkLay", &Mu1trkLay_);
  tree->SetBranchAddress("Mu1muDTh", &Mu1muDTh_);
  tree->SetBranchAddress("Mu1muCSCh", &Mu1muCSCh_);
  tree->SetBranchAddress("Mu1muRPCh", &Mu1muRPCh_);
  tree->SetBranchAddress("Mu2trkLay", &Mu2trkLay_);
  tree->SetBranchAddress("Mu2muDTh", &Mu2muDTh_);
  tree->SetBranchAddress("Mu2muCSCh", &Mu2muCSCh_);
  tree->SetBranchAddress("Mu2muRPCh", &Mu2muRPCh_);

  tree->SetBranchAddress("costheta", &costheta_);
  tree->SetBranchAddress("phi", &phi_);
  tree->SetBranchAddress("cospsi", &cospsi_);
  tree->SetBranchAddress("AngleBsDecayLength", &AngleBsDecayLength_);
  
  tree->SetBranchAddress("isMatched", &isMatched_);
  
  tree->SetBranchAddress("K1mcId", &K1mcId_);
  tree->SetBranchAddress("K1momId", &K1momId_);
  tree->SetBranchAddress("K1gmomId", &K1gmomId_);
  tree->SetBranchAddress("K2mcId", &K2mcId_);
  tree->SetBranchAddress("K2momId", &K2momId_);
  tree->SetBranchAddress("K2gmomId", &K2gmomId_);
  tree->SetBranchAddress("Mu1mcId", &Mu1mcId_);
  tree->SetBranchAddress("Mu1momId", &Mu1momId_);
  tree->SetBranchAddress("Mu1gmomId", &Mu1gmomId_);
  tree->SetBranchAddress("Mu2mcId", &Mu2mcId_);
  tree->SetBranchAddress("Mu2momId", &Mu2momId_);
  tree->SetBranchAddress("Mu2gmomId", &Mu2gmomId_);
  tree->SetBranchAddress("K1Truth", &K1Truth_);
  tree->SetBranchAddress("K2Truth", &K2Truth_);
  tree->SetBranchAddress("Mu1Truth", &Mu1Truth_);
  tree->SetBranchAddress("Mu2Truth", &Mu2Truth_);
  
  tree->SetBranchAddress("Dist3d", &Dist3d_);
  tree->SetBranchAddress("dDist3d", &dDist3d_);
  tree->SetBranchAddress("Time3d", &Time3d_);
  tree->SetBranchAddress("dTime3d", &dTime3d_);
  tree->SetBranchAddress("Dist", &Dist_);
  tree->SetBranchAddress("dDist", &dDist_);
  tree->SetBranchAddress("Time", &Time_);
  tree->SetBranchAddress("dTime", &dTime_);

  tree->SetBranchAddress("dedxTrk", &dedxTrk_);
  tree->SetBranchAddress("errdedxTrk", &errdedxTrk_);
  tree->SetBranchAddress("numdedxTrk", &numdedxTrk_);
 
  tree->SetBranchAddress("iPassedCutIdent", &iPassedCutIdent_);

  tree->SetBranchAddress("isGenBsEvent", &isGenBsEvent_);
  tree->SetBranchAddress("GenNumberOfBsDaughters", &GenNumberOfBsDaughters_);
  tree->SetBranchAddress("isGenBsJpsiPhiEvent", &isGenBsJpsiPhiEvent_);
  tree->SetBranchAddress("isGenBsJpsiPhiMuMuKKEvent", & isGenBsJpsiPhiMuMuKKEvent_);
 
  tree->SetBranchAddress("genBsM", &genBsM_);
  tree->SetBranchAddress("genBsPt", &genBsPt_);
  tree->SetBranchAddress("genBsPz", &genBsPz_);
  tree->SetBranchAddress("genBsEta", &genBsEta_);
  tree->SetBranchAddress("genBsPhi", &genBsPhi_);
  tree->SetBranchAddress("genJpsiM", &genJpsiM_);
  tree->SetBranchAddress("genJpsiPt", &genJpsiPt_);
  tree->SetBranchAddress("genJpsiPz", &genJpsiPz_);
  tree->SetBranchAddress("genJpsiEta", &genJpsiEta_);
  tree->SetBranchAddress("genJpsiPhi", &genJpsiPhi_);
  tree->SetBranchAddress("genPhiM", &genPhiM_);
  tree->SetBranchAddress("genPhiPt", &genPhiPt_);
  tree->SetBranchAddress("genPhiPz", &genPhiPz_);
  tree->SetBranchAddress("genPhiEta", &genPhiEta_);
  tree->SetBranchAddress("genPhiPhi", &genPhiPhi_);
  tree->SetBranchAddress("genMu1M", &genMu1M_);
  tree->SetBranchAddress("genMu1Pt", &genMu1Pt_);
  tree->SetBranchAddress("genMu1Pz", &genMu1Pz_);
  tree->SetBranchAddress("genMu1Eta", &genMu1Eta_);
  tree->SetBranchAddress("genMu1Phi", &genMu1Phi_);
  tree->SetBranchAddress("genMu2M", &genMu2M_);
  tree->SetBranchAddress("genMu2Pt", &genMu2Pt_);
  tree->SetBranchAddress("genMu2Pz", &genMu2Pz_);
  tree->SetBranchAddress("genMu2Eta", &genMu2Eta_);
  tree->SetBranchAddress("genMu2Phi", &genMu2Phi_);
  tree->SetBranchAddress("genK1M", &genK1M_);
  tree->SetBranchAddress("genK1Pt", &genK1Pt_);
  tree->SetBranchAddress("genK1Pz", &genK1Pz_);
  tree->SetBranchAddress("genK1Eta", &genK1Eta_);
  tree->SetBranchAddress("genK1Phi", &genK1Phi_);
  tree->SetBranchAddress("genK2M", &genK2M_);
  tree->SetBranchAddress("genK2Pt", &genK2Pt_);
  tree->SetBranchAddress("genK2Pz", &genK2Pz_);
  tree->SetBranchAddress("genK2Eta", &genK2Eta_);
  tree->SetBranchAddress("genK2Phi", &genK2Phi_);

  tree->SetBranchAddress("K1_par0KK", &K1_kk_par0_);
  tree->SetBranchAddress("K1_par1KK", &K1_kk_par1_);
  tree->SetBranchAddress("K1_par2KK", &K1_kk_par2_);
  tree->SetBranchAddress("K1_par3KK", &K1_kk_par3_);
  tree->SetBranchAddress("K1_par4KK", &K1_kk_par4_);
  tree->SetBranchAddress("K1_par5KK", &K1_kk_par5_);
  tree->SetBranchAddress("K1_par6KK", &K1_kk_par6_);

  tree->SetBranchAddress("K2_par0KK", &K2_kk_par0_);
  tree->SetBranchAddress("K2_par1KK", &K2_kk_par1_);
  tree->SetBranchAddress("K2_par2KK", &K2_kk_par2_);
  tree->SetBranchAddress("K2_par3KK", &K2_kk_par3_);
  tree->SetBranchAddress("K2_par4KK", &K2_kk_par4_);
  tree->SetBranchAddress("K2_par5KK", &K2_kk_par5_);
  tree->SetBranchAddress("K2_par6KK", &K2_kk_par6_);

  tree->SetBranchAddress("K1_par0Kpi", &K1_kpi_par0_);
  tree->SetBranchAddress("K1_par1Kpi", &K1_kpi_par1_);
  tree->SetBranchAddress("K1_par2Kpi", &K1_kpi_par2_);
  tree->SetBranchAddress("K1_par3Kpi", &K1_kpi_par3_);
  tree->SetBranchAddress("K1_par4Kpi", &K1_kpi_par4_);
  tree->SetBranchAddress("K1_par5Kpi", &K1_kpi_par5_);
  tree->SetBranchAddress("K1_par6Kpi", &K1_kpi_par6_);

  tree->SetBranchAddress("K2_par0Kpi", &K2_kpi_par0_);
  tree->SetBranchAddress("K2_par1Kpi", &K2_kpi_par1_);
  tree->SetBranchAddress("K2_par2Kpi", &K2_kpi_par2_);
  tree->SetBranchAddress("K2_par3Kpi", &K2_kpi_par3_);
  tree->SetBranchAddress("K2_par4Kpi", &K2_kpi_par4_);
  tree->SetBranchAddress("K2_par5Kpi", &K2_kpi_par5_);
  tree->SetBranchAddress("K2_par6Kpi", &K2_kpi_par6_);

  tree->SetBranchAddress("K1_par0pipi", &K1_pipi_par0_);
  tree->SetBranchAddress("K1_par1pipi", &K1_pipi_par1_);
  tree->SetBranchAddress("K1_par2pipi", &K1_pipi_par2_);
  tree->SetBranchAddress("K1_par3pipi", &K1_pipi_par3_);
  tree->SetBranchAddress("K1_par4pipi", &K1_pipi_par4_);
  tree->SetBranchAddress("K1_par5pipi", &K1_pipi_par5_);
  tree->SetBranchAddress("K1_par6pipi", &K1_pipi_par6_);

  tree->SetBranchAddress("K2_par0pipi", &K2_pipi_par0_);
  tree->SetBranchAddress("K2_par1pipi", &K2_pipi_par1_);
  tree->SetBranchAddress("K2_par2pipi", &K2_pipi_par2_);
  tree->SetBranchAddress("K2_par3pipi", &K2_pipi_par3_);
  tree->SetBranchAddress("K2_par4pipi", &K2_pipi_par4_);
  tree->SetBranchAddress("K2_par5pipi", &K2_pipi_par5_);
  tree->SetBranchAddress("K2_par6pipi", &K2_pipi_par6_);

  tree->SetBranchAddress("K1_kk_sigX_", &K1_kk_sigX_);
  tree->SetBranchAddress("K1_kk_sigY_", &K1_kk_sigY_);
  tree->SetBranchAddress("K1_kk_sigZ_", &K1_kk_sigZ_);

  tree->SetBranchAddress("K1_kpi_sigX_", &K1_kpi_sigX_);
  tree->SetBranchAddress("K1_kpi_sigY_", &K1_kpi_sigY_);
  tree->SetBranchAddress("K1_kpi_sigZ_", &K1_kpi_sigZ_);

  tree->SetBranchAddress("K1_pipi_sigX_", &K1_pipi_sigX_);
  tree->SetBranchAddress("K1_pipi_sigY_", &K1_pipi_sigY_);
  tree->SetBranchAddress("K1_pipi_sigZ_", &K1_pipi_sigZ_);

  tree->SetBranchAddress("K2_kk_sigX_", &K2_kk_sigX_);
  tree->SetBranchAddress("K2_kk_sigY_", &K2_kk_sigY_);
  tree->SetBranchAddress("K2_kk_sigZ_", &K2_kk_sigZ_);

  tree->SetBranchAddress("K2_kpi_sigX_", &K2_kpi_sigX_);
  tree->SetBranchAddress("K2_kpi_sigY_", &K2_kpi_sigY_);
  tree->SetBranchAddress("K2_kpi_sigZ_", &K2_kpi_sigZ_);

  tree->SetBranchAddress("K2_pipi_sigX_", &K2_pipi_sigX_);
  tree->SetBranchAddress("K2_pipi_sigY_", &K2_pipi_sigY_);
  tree->SetBranchAddress("K2_pipi_sigZ_", &K2_pipi_sigZ_);

  tree->SetBranchAddress("K1_kk_sigPX_", &K1_kk_sigPX_);
  tree->SetBranchAddress("K1_kk_sigPY_", &K1_kk_sigPY_);
  tree->SetBranchAddress("K1_kk_sigPZ_", &K1_kk_sigPZ_);

  tree->SetBranchAddress("K1_kpi_sigPX_", &K1_kpi_sigPX_);
  tree->SetBranchAddress("K1_kpi_sigPY_", &K1_kpi_sigPY_);
  tree->SetBranchAddress("K1_kpi_sigPZ_", &K1_kpi_sigPZ_);

  tree->SetBranchAddress("K1_pipi_sigPX_", &K1_pipi_sigPX_);
  tree->SetBranchAddress("K1_pipi_sigPY_", &K1_pipi_sigPY_);
  tree->SetBranchAddress("K1_pipi_sigPZ_", &K1_pipi_sigPZ_);

  tree->SetBranchAddress("K2_kk_sigPX_", &K2_kk_sigPX_);
  tree->SetBranchAddress("K2_kk_sigPY_", &K2_kk_sigPY_);
  tree->SetBranchAddress("K2_kk_sigPZ_", &K2_kk_sigPZ_);

  tree->SetBranchAddress("K2_kpi_sigPX_", &K2_kpi_sigPX_);
  tree->SetBranchAddress("K2_kpi_sigPY_", &K2_kpi_sigPY_);
  tree->SetBranchAddress("K2_kpi_sigPZ_", &K2_kpi_sigPZ_);

  tree->SetBranchAddress("K2_pipi_sigPX_", &K2_pipi_sigPX_);
  tree->SetBranchAddress("K2_pipi_sigPY_", &K2_pipi_sigPY_);
  tree->SetBranchAddress("K2_pipi_sigPZ_", &K2_pipi_sigPZ_);

  tree->SetBranchAddress("sigmaK1Pt", &K1Pt_error_);
  tree->SetBranchAddress("sigmaK2Pt", &K2Pt_error_);

  tree->SetBranchAddress("isGenBdEvent", &isGenBdEvent_);
  tree->SetBranchAddress("GenNumberOfBdDaughters", &GenNumberOfBdDaughters_);
  tree->SetBranchAddress("isGenBdJpsiKstarEvent", &isGenBdJpsiKstarEvent_);
  tree->SetBranchAddress("isGenBdJpsiKstarMuMuKpiEvent", & isGenBdJpsiKstarMuMuKpiEvent_);

  tree->SetBranchAddress("isGenBsJpsiKKEvent", &isGenBsJpsiKKEvent_);
  tree->SetBranchAddress("BdDausId", &BdDausId_);

  tree->SetBranchAddress("BsVtxProb", &BsVtxProb_);
  int nentries = tree->GetEntries();
  std::cout<<"Number of entries in tree = " << nentries << std::endl;

  // #################  DEFINE HISTOGRAMS ######################

  TH1F * hSelectionEff_BsJpsiSignal = new TH1F("hSelectionEff_BsJpsiSignal", "hSelectionEff_BsJpsiSignal", 200, -20, 20);
  TH1F * hSelectionEff_BsJpsiKKSignal = new TH1F("hSelectionEff_BsJpsiKKSignal", "hSelectionEff_BsJpsiKKSignal", 200, -20, 20);
  TH1F * hSelectionEff_BdJpsiSignal = new TH1F("hSelectionEff_BdJpsiSignal", "hSelectionEff_BdJpsiSignal", 200, -20, 20);

  //#############################################################
  

 int entrycounter = 0;

 int iBsJPsiPhiSignalEvents = 0;
 int iBsJPsiKKSignalEvents = 0;
 int iBdJPsiKstarSignalEvents = 0;
 int iBsOtherEvents = 0;
 int iBdOtherEvents = 0;

 int iTriggered_DoubleMu3_BsJPsiPhiSignalEvents = 0;
 int iTriggered_DoubleMu3_BdJpsiKstar = 0;
 int iTriggered_DoubleMu3_BsJpsiKK = 0;
 int iTriggered_DoubleMu3_BsOtherEvents = 0;
 int iTriggered_DoubleMu3_BdOtherEvents = 0;
 int iTriggered_DoubleMu3_promptEvents = 0;

 int iBsJPsiPhiSignalEventsPreKinFit = 0;
 int iBsJPsiKKSignalEventsPreKinFit = 0;
 int iBdJPsiKstarSignalEventsPreKinFit = 0;
 int iBsOtherEventsPreKinFit = 0;
 int iBdOtherEventsPreKinFit = 0;
 int ipromptEventsPreKinFit = 0;

 int iBsJPsiPhiSignalEventsOfflineSel1 = 0;
 int iBsJPsiKKSignalEventsOfflineSel1 = 0;
 int iBdJPsiKstarSignalEventsOfflineSel1 = 0;
 int iBsOtherEventsOfflineSel1 = 0;
 int iBdOtherEventsOfflineSel1 = 0;
 int ipromptEventsOfflineSel1 = 0;

 int iBsJPsiPhiSignalEventsProbVertex = 0;
 int iBsJPsiKKSignalEventsProbVertex = 0;
 int iBdJPsiKstarSignalEventsProbVertex = 0;
 int iBsOtherEventsProbVertex = 0;
 int iBdOtherEventsProbVertex = 0;
 int ipromptEventsProbVertex = 0;

 int HLT = 0;
 double minProbability = 0.05;

 // int iTriggered_Double


   // STARTING event loop
 for(int i=0; i < nentries; i++){
   entrycounter++;
   if( (entrycounter % 10000) == 0) std::cout<<"processing entry number " << entrycounter << std::endl;
   tree->GetEntry(i);

   if(triggerbit_HLTdoubleMu3_ == 1){      // IS TRIGGERED_HLTdoubleMu3**************
     HLT++;
     if(isGenBsJpsiPhiMuMuKKEvent_ == 1){  // IS SIGNAL EVENT
       iTriggered_DoubleMu3_BsJPsiPhiSignalEvents++;    //count triggered signal events
       
       // check offline selection:
       hSelectionEff_BsJpsiSignal ->Fill( 0 );                        // is triggered
       if(iPassedCutIdent_ >= 1) hSelectionEff_BsJpsiSignal->Fill(1); // two opposite sign muons
       if(iPassedCutIdent_ >= 2) hSelectionEff_BsJpsiSignal->Fill(2); // passed Jpsi mass and pt cut
       if(iPassedCutIdent_ >= 3) hSelectionEff_BsJpsiSignal->Fill(3); // ....
       if(iPassedCutIdent_ >= 4) hSelectionEff_BsJpsiSignal->Fill(4);
       if(iPassedCutIdent_ >= 5) hSelectionEff_BsJpsiSignal->Fill(5);
       if(iPassedCutIdent_ >= 6) hSelectionEff_BsJpsiSignal->Fill(6);
       if(iPassedCutIdent_ >= 7) hSelectionEff_BsJpsiSignal->Fill(7);
       if(iPassedCutIdent_ >= 8) hSelectionEff_BsJpsiSignal->Fill(8);
       if(iPassedCutIdent_ >= 9) hSelectionEff_BsJpsiSignal->Fill(9);
       if(iPassedCutIdent_ >= 10) hSelectionEff_BsJpsiSignal->Fill(10);
       if(iPassedCutIdent_ >= 11) hSelectionEff_BsJpsiSignal->Fill(11);
       if(iPassedCutIdent_ >= 12) hSelectionEff_BsJpsiSignal->Fill(12);
       if(iPassedCutIdent_ >= 13) hSelectionEff_BsJpsiSignal->Fill(13);
       
       if(iPassedCutIdent_ >= 7){
	 iBsJPsiPhiSignalEventsPreKinFit++;
       }
        if(iPassedCutIdent_ >= 13){
	 iBsJPsiPhiSignalEventsOfflineSel1++;
       }

        if(iPassedCutIdent_ >= 13 && BsVtxProb_> minProbability ){
	 iBsJPsiPhiSignalEventsProbVertex++;
       }



     }
     else if(isGenBsJpsiKKEvent_ == 1 ){             // Bs to Jpsi KK
       iTriggered_DoubleMu3_BsJpsiKK++;  // count triggered events

       // check offline selection:
       hSelectionEff_BsJpsiKKSignal ->Fill( 0 );                        // is triggered
       if(iPassedCutIdent_ >= 1) hSelectionEff_BsJpsiKKSignal->Fill(1); // two opposite sign muons
       if(iPassedCutIdent_ >= 2) hSelectionEff_BsJpsiKKSignal->Fill(2); // passed JpsiKK mass and pt cut
       if(iPassedCutIdent_ >= 3) hSelectionEff_BsJpsiKKSignal->Fill(3); // ....
       if(iPassedCutIdent_ >= 4) hSelectionEff_BsJpsiKKSignal->Fill(4);
       if(iPassedCutIdent_ >= 5) hSelectionEff_BsJpsiKKSignal->Fill(5);
       if(iPassedCutIdent_ >= 6) hSelectionEff_BsJpsiKKSignal->Fill(6);
       if(iPassedCutIdent_ >= 7) hSelectionEff_BsJpsiKKSignal->Fill(7);
       if(iPassedCutIdent_ >= 8) hSelectionEff_BsJpsiKKSignal->Fill(8);
       if(iPassedCutIdent_ >= 9) hSelectionEff_BsJpsiKKSignal->Fill(9);
       if(iPassedCutIdent_ >= 10) hSelectionEff_BsJpsiKKSignal->Fill(10);
       if(iPassedCutIdent_ >= 11) hSelectionEff_BsJpsiKKSignal->Fill(11);
       if(iPassedCutIdent_ >= 12) hSelectionEff_BsJpsiKKSignal->Fill(12);
       if(iPassedCutIdent_ >= 13) hSelectionEff_BsJpsiKKSignal->Fill(13);
       


       if(iPassedCutIdent_ >= 7){
	 iBsJPsiKKSignalEventsPreKinFit++;
       }
       if(iPassedCutIdent_ >= 13){
	 iBsJPsiKKSignalEventsOfflineSel1++;
       }
       if(iPassedCutIdent_ >= 13 && BsVtxProb_> minProbability ){
	 iBsJPsiKKSignalEventsProbVertex++;
       }

     }
     else if (isGenBdJpsiKstarMuMuKpiEvent_ == 1 ){   // Bd to Jpsi Kstar
       iTriggered_DoubleMu3_BdJpsiKstar++;  // count triggered events

       // check offline selection:
       hSelectionEff_BdJpsiSignal ->Fill( 0 );                        // is triggered
       if(iPassedCutIdent_ >= 1) hSelectionEff_BdJpsiSignal->Fill(1); // two opposite sign muons
       if(iPassedCutIdent_ >= 2) hSelectionEff_BdJpsiSignal->Fill(2); // passed Jpsi mass and pt cut
       if(iPassedCutIdent_ >= 3) hSelectionEff_BdJpsiSignal->Fill(3); // ....
       if(iPassedCutIdent_ >= 4) hSelectionEff_BdJpsiSignal->Fill(4);
       if(iPassedCutIdent_ >= 5) hSelectionEff_BdJpsiSignal->Fill(5);
       if(iPassedCutIdent_ >= 6) hSelectionEff_BdJpsiSignal->Fill(6);
       if(iPassedCutIdent_ >= 7) hSelectionEff_BdJpsiSignal->Fill(7);
       if(iPassedCutIdent_ >= 8) hSelectionEff_BdJpsiSignal->Fill(8);
       if(iPassedCutIdent_ >= 9) hSelectionEff_BdJpsiSignal->Fill(9);
       if(iPassedCutIdent_ >= 10) hSelectionEff_BdJpsiSignal->Fill(10);
       if(iPassedCutIdent_ >= 11) hSelectionEff_BdJpsiSignal->Fill(11);
       if(iPassedCutIdent_ >= 12) hSelectionEff_BdJpsiSignal->Fill(12);
       if(iPassedCutIdent_ >= 13) hSelectionEff_BdJpsiSignal->Fill(13);
       
       if(iPassedCutIdent_ >= 7){
	 iBdJPsiKstarSignalEventsPreKinFit++;
       }
       if(iPassedCutIdent_ >= 13){
	 iBdJPsiKstarSignalEventsOfflineSel1++;
       }
       if(iPassedCutIdent_ >= 13 && BsVtxProb_> minProbability ){
	 iBdJPsiKstarSignalEventsProbVertex++;
       }
       
     }
     else if (isGenBsEvent_ == 1 && isGenBsJpsiKKEvent_ != 1 && isGenBsJpsiPhiMuMuKKEvent_ != 1){
       iTriggered_DoubleMu3_BsOtherEvents++;     
       
       if(iPassedCutIdent_ >= 7){
	 iBsOtherEventsPreKinFit++;
       }
       if(iPassedCutIdent_ >= 13){
	 iBsOtherEventsOfflineSel1++;
       }
       if(iPassedCutIdent_ >= 13 && BsVtxProb_> minProbability ){
	 iBsOtherEventsProbVertex++;
       }
     }
     else if (isGenBdEvent_ == 1 && isGenBdJpsiKstarMuMuKpiEvent_ != 1){
       iTriggered_DoubleMu3_BdOtherEvents++;     
       
       if(iPassedCutIdent_ >= 7){
	 iBdOtherEventsPreKinFit++;
       }
       if(iPassedCutIdent_ >= 13){
	 iBdOtherEventsOfflineSel1++;
       }
       if(iPassedCutIdent_ >= 13 && BsVtxProb_> minProbability ){
	 iBdOtherEventsProbVertex++;
       }
     }
     
     if (filename == "promptJpsi/promptJpsi.root"){
       iTriggered_DoubleMu3_promptEvents++;
       
       if(iPassedCutIdent_ >= 7){
         ipromptEventsPreKinFit++;
       }
       if(iPassedCutIdent_ >= 13){
         ipromptEventsOfflineSel1++;
       }
       if(iPassedCutIdent_ >= 13 && BsVtxProb_> minProbability ){
         ipromptEventsProbVertex++;
       }
     }
     
   } // END  double mu3 trigger *************
   

   // ALL EVENTS (triggered and not triggered)
   if(isGenBsJpsiPhiMuMuKKEvent_ == 1){
     iBsJPsiPhiSignalEvents++;
     hSelectionEff_BsJpsiSignal->Fill ( -1 ); 
   }
   else if (isGenBsJpsiKKEvent_ == 1){
     iBsJPsiKKSignalEvents++;
     hSelectionEff_BsJpsiKKSignal->Fill ( -1 ); 
   }
   else if (isGenBdJpsiKstarMuMuKpiEvent_ == 1){
     iBdJPsiKstarSignalEvents++;
     hSelectionEff_BdJpsiSignal->Fill ( -1 ); 
   }
   else if (isGenBsEvent_ == 1 && isGenBsJpsiPhiMuMuKKEvent_ != 1 && isGenBsJpsiKKEvent_ != 1){
     iBsOtherEvents++;
   }
   else if (isGenBdEvent_ == 1 && isGenBdJpsiKstarMuMuKpiEvent_ != 1){
     iBdOtherEvents++;
   }

   //   if( entrycounter > 2000000 ) break;


 } /// FINISHED EVENT LOOP
 

 // print out numbers 

 std::cout<< "iBsJPsiPhiSignalEvents = " << iBsJPsiPhiSignalEvents <<  std::endl;
 std::cout<< "iTriggered_DoubleMu3_BsJPsiPhiSignalEvents = " << iTriggered_DoubleMu3_BsJPsiPhiSignalEvents << std::endl;
 std::cout<< "iBsJPsiPhiSignalEventsPreKinFit = "  << iBsJPsiPhiSignalEventsPreKinFit << std::endl;
 std::cout<< "iBsJPsiPhiSignalEventsOfflineSel1 = "  << iBsJPsiPhiSignalEventsOfflineSel1 << std::endl;
 std::cout<< "iBsJPsiPhiSignalEventsProbVertex = "  << iBsJPsiPhiSignalEventsProbVertex << std::endl;
 std::cout<<std::endl;
 
 std::cout<< "iBdJPsiKstarSignalEvents = " << iBdJPsiKstarSignalEvents <<  std::endl;
 std::cout<< "iTriggered_DoubleMu3_BdJPsiKstar = " << iTriggered_DoubleMu3_BdJpsiKstar << std::endl;
 std::cout<< "iBdJPsiKstarSignalEventsPreKinFit = "  << iBdJPsiKstarSignalEventsPreKinFit << std::endl;
 std::cout<< "iBdJPsiKstarSignalEventsOfflineSel1 = "  << iBdJPsiKstarSignalEventsOfflineSel1 << std::endl;
 std::cout<< "iBdJPsiKstarSignalEventsProbVertex = "  << iBdJPsiKstarSignalEventsProbVertex << std::endl;
 std::cout<<std::endl;
 
 std::cout<< "iBsJPsiKKSignalEvents = " << iBsJPsiKKSignalEvents <<  std::endl;
 std::cout<< "iTriggered_DoubleMu3_BsJpsiKK = " << iTriggered_DoubleMu3_BsJpsiKK << std::endl;
 std::cout<< "iBsJPsiKKSignalEventsPreKinFit = "  << iBsJPsiKKSignalEventsPreKinFit << std::endl;
 std::cout<< "iBsJPsiKKSignalEventsOfflineSel1 = "  << iBsJPsiKKSignalEventsOfflineSel1 << std::endl;
 std::cout<< "iBsJPsiKKSignalEventsProbVertex = "  << iBsJPsiKKSignalEventsProbVertex << std::endl;
 std::cout<<std::endl;
 
 std::cout<< "iBsOtherEvents = " << iBsOtherEvents <<  std::endl;
 std::cout<< "iTriggered_DoubleMu3_BsOtherEvents = " << iTriggered_DoubleMu3_BsOtherEvents << std::endl;
 std::cout<< "iBsOtherEventsPreKinFit = "  << iBsOtherEventsPreKinFit << std::endl;
 std::cout<< "iBsOtherEventsOfflineSel1 = "  << iBsOtherEventsOfflineSel1 << std::endl;
 std::cout<< "iBsOtherEventsProbVertex = "  << iBsOtherEventsProbVertex << std::endl;
 std::cout<<std::endl;
 
 std::cout<< "iBdOtherEvents = " << iBdOtherEvents <<  std::endl;
 std::cout<< "iTriggered_DoubleMu3_BdOtherEvents = " << iTriggered_DoubleMu3_BdOtherEvents << std::endl;
 std::cout<< "iBdOtherEventsPreKinFit = "  << iBdOtherEventsPreKinFit << std::endl;
 std::cout<< "iBdOtherEventsOfflineSel1 = "  << iBdOtherEventsOfflineSel1 << std::endl;
 std::cout<< "iBdOtherEventsProbVertex = "  << iBdOtherEventsProbVertex << std::endl;
 std::cout<<std::endl;

 if (filename == "promptJpsi/promptJpsi.root"){
   
   std::cout<< "ipromptEvents = " << entrycounter <<  std::endl;
   std::cout<< "iTriggered_DoubleMu3_promptEvents = " << iTriggered_DoubleMu3_promptEvents << std::endl;
   std::cout<< "ipromptEventsPreKinFit = "  << ipromptEventsPreKinFit << std::endl;
   std::cout<< "ipromptEventsOfflineSel1 = "  << ipromptEventsOfflineSel1 << std::endl;
   std::cout<< "ipromptEventsProbVertex = "  << ipromptEventsProbVertex << std::endl;
   std::cout<<std::endl;
   
 }

 std::cout<< "There are " << entrycounter << " events in the tree." << std::endl;
 std::cout<< "Events with only HLT: " << HLT << std::endl << std::endl;
 
 std::cout<< "                          | Bs->JpsiPhi | Bd->Jpsi K* | Bs->Jpsi KK | Other Bs | Other Bd "<<  std::endl;
 std::cout<< "events num                |    " << iBsJPsiPhiSignalEvents << "     |    " << iBdJPsiKstarSignalEvents  << "    |    " << iBsJPsiKKSignalEvents <<"     |   " << iBsOtherEvents << " |   " << iBdOtherEvents <<  std::endl;
 std::cout<< "Triggered  events         |     " << iTriggered_DoubleMu3_BsJPsiPhiSignalEvents << "     |     " << iTriggered_DoubleMu3_BdJpsiKstar  << "    |     " << iTriggered_DoubleMu3_BsJpsiKK << "     |     " << iTriggered_DoubleMu3_BsOtherEvents << " |    " << iTriggered_DoubleMu3_BdOtherEvents <<  std::endl;
 std::cout<< "Pre   Kinem  fit*         |      " << iBsJPsiPhiSignalEventsPreKinFit << "     |      " << iBdJPsiKstarSignalEventsPreKinFit << "    |     " << iBsJPsiKKSignalEventsPreKinFit << "     |     " << iBsOtherEventsPreKinFit  << " |    " << iBdOtherEventsPreKinFit <<  std::endl;
 std::cout<< "After Kinem. fit.         |      " << iBsJPsiPhiSignalEventsOfflineSel1 << "     |      " << iBdJPsiKstarSignalEventsOfflineSel1 << "    |      " << iBsJPsiKKSignalEventsOfflineSel1 << "     |      " << iBsOtherEventsOfflineSel1 << " |     " << iBdOtherEventsOfflineSel1 <<  std::endl; 
 std::cout<< "Prob vertex(KK)>5 percent |      " << iBsJPsiPhiSignalEventsProbVertex <<  "     |       " <<  iBdJPsiKstarSignalEventsProbVertex << "    |       " << iBsJPsiKKSignalEventsProbVertex << "     |      " << iBsOtherEventsProbVertex << " |      " << iBdOtherEventsProbVertex     << std::endl;



 std::cout<< "  "<<  std::endl;
 std::cout<< "  "<<  std::endl;


 std::cout<< "READY for the tex file:  "                    <<  std::endl;
 //std::cout<< "\begin { table }[tp] "                      <<  std::endl;
 //std::cout<< "\caption {Datasets and events filtering.}"  <<  std::endl;
 //std::cout<< "\begin { tabular }{ clccc }"                <<  std::endl;
 //std::cout<< "\hline"                                      <<  std::endl;
 //std::cout<< "\multicolumn{6}{|c|}{Datasets and events filtering} \\"  <<  std::endl;
 //std::cout<< "\hline"                                      <<  std::endl;
 std::cout<< "                          && Bs->JpsiPhi && Bd->Jpsi K* && Bs->Jpsi KK && Other Bs && Other Bd "<<  std::endl;
 std::cout<< "events num                &&    " << iBsJPsiPhiSignalEvents << "     &&    " << iBdJPsiKstarSignalEvents  << "    &&    " << iBsJPsiKKSignalEvents <<"     &&   " << iBsOtherEvents << " &&   " << iBdOtherEvents <<  std::endl;
 std::cout<< "Triggered  events         &&     " << iTriggered_DoubleMu3_BsJPsiPhiSignalEvents << "     &&     " << iTriggered_DoubleMu3_BdJpsiKstar  << "    &&     " << iTriggered_DoubleMu3_BsJpsiKK << "     &&     " << iTriggered_DoubleMu3_BsOtherEvents << " &&    " << iTriggered_DoubleMu3_BdOtherEvents <<  std::endl;
 std::cout<< "Pre   Kinem  fit*         &&     " << iBsJPsiPhiSignalEventsPreKinFit << "     &&      " << iBdJPsiKstarSignalEventsPreKinFit << "    &&     " << iBsJPsiKKSignalEventsPreKinFit << "     &&     " << iBsOtherEventsPreKinFit  << " &&    " << iBdOtherEventsPreKinFit <<  std::endl;
 std::cout<< "After Kinem. fit.         &&      " << iBsJPsiPhiSignalEventsOfflineSel1 << "     &&      " << iBdJPsiKstarSignalEventsOfflineSel1 << "    &&      " << iBsJPsiKKSignalEventsOfflineSel1 << "     &&      " << iBsOtherEventsOfflineSel1 << " &&     " << iBdOtherEventsOfflineSel1 <<  std::endl; 
 std::cout<< "Prob vertex(KK)>5 percent &&      " << iBsJPsiPhiSignalEventsProbVertex <<  "     &&       " <<  iBdJPsiKstarSignalEventsProbVertex << "    &&       " << iBsJPsiKKSignalEventsProbVertex << "     &&      " << iBsOtherEventsProbVertex << " &&      " << iBdOtherEventsProbVertex     << std::endl;
  //std::cout<< "\hline"                                      <<  std::endl;
 //std::cout<< "\ end { tabular }"                           <<  std::endl;
 //std::cout<< "\ end { table }"                             <<  std::endl;
 
 
 
 TFile *histofile = new TFile(outputhistofilename , "RECREATE");

 hSelectionEff_BsJpsiSignal->Write();
 hSelectionEff_BsJpsiKKSignal->Write();
 hSelectionEff_BdJpsiSignal->Write();
 
 histofile->Write();
 histofile->Close();


}
