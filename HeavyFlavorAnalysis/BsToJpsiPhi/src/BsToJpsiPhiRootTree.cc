#include "HeavyFlavorAnalysis/BsToJpsiPhi/interface/BsToJpsiPhiRootTree.h"

BsToJpsiPhiRootTree::BsToJpsiPhiRootTree(const std::string filename)
{
  resetEntries();
  
  // open root file
  bsFile_ = new TFile (filename.c_str(), "RECREATE" );
  int bufsize = 64000;
  // create tree structure
  bsTree_ = new TTree("BsTree","BsTree",bufsize);
  
  bsTree_->Branch("triggerbit_HLTmu3", &triggerbit_HLTmu3_, "triggerbit_HLTmu3/I");
  bsTree_->Branch("triggerbit_HLTmu5", &triggerbit_HLTmu5_, "triggerbit_HLTmu5/I");
  bsTree_->Branch("triggerbit_HLTmu9", &triggerbit_HLTmu9_, "triggerbit_HLTmu9/I");
  bsTree_->Branch("triggerbit_HLTdoubleIsoMu3", &triggerbit_HLTdoubleIsoMu3_, "triggerbit_HLTdoubleIsoMu3/I");
  bsTree_->Branch("triggerbit_HLTdoubleMu3", &triggerbit_HLTdoubleMu3_, "triggerbit_HLTdoubleMu3/I");
  bsTree_->Branch("triggerbit_HLTdoubleMu3_JPsi", &triggerbit_HLTdoubleMu3_JPsi_, "triggerbit_HLTdoubleMu3_JPsi/I");
  bsTree_->Branch("nBsDau", &nBsDau_, "nBsDau/I");
  bsTree_->Branch("momID", &momID_, "momID/I");
  bsTree_->Branch("dau_1_ID", &dau_1_ID_, "dau_1_ID/I");
  bsTree_->Branch("dau_2_ID", &dau_2_ID_, "dau_2_ID/I");
  
  bsTree_->Branch("BeamspotX", &BSx_, "BeamspotX/D");
  bsTree_->Branch("BeamspotY", &BSy_, "BeamspotY/D");
  bsTree_->Branch("BeamspotZ", &BSz_, "BeamspotZ/D");
  bsTree_->Branch("PVx", &PVx_, "PVx/D");
  bsTree_->Branch("PVy", &PVy_, "PVy/D");
  bsTree_->Branch("PVz", &PVz_, "PVz/D");
  bsTree_->Branch("PVerrx", &PVerrx_, "PVerrx/D");
  bsTree_->Branch("PVerry", &PVerry_, "PVerry/D");
  bsTree_->Branch("PVerrz", &PVerrz_, "PVerrz/D");
  
  bsTree_->Branch("JpsiVtxProb", &JpsiVtxProb_, "JpsiVtxProb/D");
  bsTree_->Branch("JpsiM_alone", &JpsiM_alone_, "JpsiM_alone/D");
  bsTree_->Branch("JpsiPhi_alone", &JpsiPhi_alone_, "JpsiPhi_alone/D");
  bsTree_->Branch("JpsiEta_alone", &JpsiEta_alone_, "JpsiEta_alone/D");
  bsTree_->Branch("JpsiPt_alone", &JpsiPt_alone_, "JpsiPt_alone/D");
  bsTree_->Branch("JpsiMu1Pt", &JpsiMu1Pt_, "JpsiMu1Pt/D");
  bsTree_->Branch("JpsiMu2Pt", &JpsiMu2Pt_, "JpsiMu2Pt/D");
  bsTree_->Branch("JpsiMu1Phi", &JpsiMu1Phi_, "JpsiMu1Phi/D");
  bsTree_->Branch("JpsiMu2Phi", &JpsiMu2Phi_, "JpsiMu2Phi/D");
  bsTree_->Branch("JpsiMu1Eta", &JpsiMu1Eta_, "JpsiMu1Eta/D");
  bsTree_->Branch("JpsiMu2Eta", &JpsiMu2Eta_, "JpsiMu2Eta/D");
  bsTree_->Branch("JpsiMuon1Cat", &JpsiMuon1Cat_, "JpsiMuon1Cat/I");
  bsTree_->Branch("JpsiMuon2Cat", &JpsiMuon2Cat_, "JpsiMuon2Cat/I");

  bsTree_->Branch("BsMass_before", &BsMass_before_, "BsMass_before/D");
  bsTree_->Branch("BsPhi_before", &BsPhi_before_, "BsPhi_before/D");
  bsTree_->Branch("BsEta_before", &BsEta_before_, "BsEta_before/D");
  bsTree_->Branch("BsPt_before", &BsPt_before_, "BsPt_before/D");
  bsTree_->Branch("BsPz_before", &BsPz_before_, "BsPz_before/D");

  bsTree_->Branch("JpsiMass_before", &JpsiMass_before_, "JpsiMass_before/D");
  bsTree_->Branch("JpsiPhi_before", &JpsiPhi_before_, "JpsiPhi_before/D");
  bsTree_->Branch("JpsiEta_before", &JpsiEta_before_, "JpsiEta_before/D");
  bsTree_->Branch("JpsiPt_before", &JpsiPt_before_, "JpsiPt_before/D");
  bsTree_->Branch("JpsiPz_before", &JpsiPz_before_, "JpsiPz_before/D");

  bsTree_->Branch("PhiMass_before", &PhiMass_before_, "PhiMass_before/D");
  bsTree_->Branch("PhiPhi_before", &PhiPhi_before_, "PhiPhi_before/D");
  bsTree_->Branch("PhiEta_before", &PhiEta_before_, "PhiEta_before/D");
  bsTree_->Branch("PhiPt_before", &PhiPt_before_, "PhiPt_before/D");
  bsTree_->Branch("PhiPz_before", &PhiPz_before_, "PhiPz_before/D");

  bsTree_->Branch("K1Phi_before", &K1Phi_before_, "K1Phi_before/D");
  bsTree_->Branch("K1Eta_before", &K1Eta_before_, "K1Eta_before/D");
  bsTree_->Branch("K1Pt_before", &K1Pt_before_, "K1Pt_before/D");
  bsTree_->Branch("K1Pz_before", &K1Pz_before_, "K1Pz_before/D");
  bsTree_->Branch("K2Phi_before", &K2Phi_before_, "K2Phi_before/D");
  bsTree_->Branch("K2Eta_before", &K2Eta_before_, "K2Eta_before/D");
  bsTree_->Branch("K2Pt_before", &K2Pt_before_, "K2Pt_before/D");
  bsTree_->Branch("K2Pz_before", &K2Pz_before_, "K2Pz_before/D");

  bsTree_->Branch("BsVtxProb", &BsVtxProb_, "BsVtxProb/D");
  bsTree_->Branch("BsVtxProbKpi", &BsVtxProbKpi_, "BsVtxProbKpi/D");
  bsTree_->Branch("BsVtxProbpipi", &BsVtxProbpipi_, "BsVtxProbpipi/D");

  bsTree_->Branch("BsVtx_x", &BsVtx_x_, "BsVtx_x/D");
  bsTree_->Branch("BsVtx_y", &BsVtx_y_, "BsVtx_y/D");
  bsTree_->Branch("BsVtx_z", &BsVtx_z_, "BsVtx_z/D");

  bsTree_->Branch("chi2_Bs", &chi2_Bs_, "chi2_Bs/D");
  bsTree_->Branch("ndof_Bs", &ndof_Bs_, "ndof_Bs/I");

  bsTree_->Branch("BFitM_KK", &BfitM_KK_, "BFitM_KK/D");
  bsTree_->Branch("BFitM_Kpi", &BfitM_Kpi_, "BFitM_Kpi/D");
  bsTree_->Branch("BFitM_pipi", &BfitM_pipi_, "BFitM_pipi/D");

  bsTree_->Branch("BsMass_after", &BsMass_after_, "BsMass_after/D");
  bsTree_->Branch("BsPhi_after", &BsPhi_after_, "BsPhi_after/D");
  bsTree_->Branch("BsEta_after", &BsEta_after_, "BsEta_after/D");
  bsTree_->Branch("BsPt_after", &BsPt_after_, "BsPt_after/D");
  bsTree_->Branch("BsPz_after", &BsPz_after_, "BsPz_after/D");

  bsTree_->Branch("JpsiMass_after", &JpsiMass_after_, "JpsiMass_after/D");
  bsTree_->Branch("JpsiPhi_after", &JpsiPhi_after_, "JpsiPhi_after/D");
  bsTree_->Branch("JpsiEta_after", &JpsiEta_after_, "JpsiEta_after/D");
  bsTree_->Branch("JpsiPt_after", &JpsiPt_after_, "JpsiPt_after/D");
  bsTree_->Branch("JpsiPz_after", &JpsiPz_after_, "JpsiPz_after/D");

  bsTree_->Branch("PhiMass_after", &PhiMass_after_, "PhiMass_after/D");
  bsTree_->Branch("PhiPhi_after", &PhiPhi_after_, "PhiPhi_after/D");
  bsTree_->Branch("PhiEta_after", &PhiEta_after_, "PhiEta_after/D");
  bsTree_->Branch("PhiPt_after", &PhiPt_after_, "PhiPt_after/D");
  bsTree_->Branch("PhiPz_after", &PhiPz_after_, "PhiPz_after/D");

  bsTree_->Branch("K1Phi_after", &K1Phi_after_, "K1Phi_after/D");
  bsTree_->Branch("K1Eta_after", &K1Eta_after_, "K1Eta_after/D");
  bsTree_->Branch("K1Pt_after", &K1Pt_after_, "K1Pt_after/D");
  bsTree_->Branch("K1Pz_after", &K1Pz_after_, "K1Pz_after/D");
  bsTree_->Branch("K2Phi_after", &K2Phi_after_, "K2Phi_after/D");
  bsTree_->Branch("K2Eta_after", &K2Eta_after_, "K2Eta_after/D");
  bsTree_->Branch("K2Pt_after", &K2Pt_after_, "K2Pt_after/D");
  bsTree_->Branch("K2Pz_after", &K2Pz_after_, "K2Pz_after/D");

  bsTree_->Branch("BLxy", &BLxy_, "BLxy/D");
  bsTree_->Branch("BLxy2", &BLxy2_, "BLxy2/D");
  bsTree_->Branch("BerrX", &BerrX_, "BerrX/D");
  bsTree_->Branch("BerrY", &BerrY_, "BerrY/D");
  bsTree_->Branch("BerrXY", &BerrXY_, "BerrXY/D");
  bsTree_->Branch("Bsct1", &Bsct1_, "Bsct1/D");
  bsTree_->Branch("Bsct2", &Bsct2_, "Bsct2/D");

  bsTree_->Branch("K1Chi2", &K1Chi2_, "K1Chi2/D");
  bsTree_->Branch("K1nHits", &K1nHits_, "K1nHits/D");
  bsTree_->Branch("K1pixH", &K1pixH_, "K1pixH/D");
  bsTree_->Branch("K1trkH", &K1trkH_, "K1trkH/D");

  bsTree_->Branch("K2Chi2", &K2Chi2_, "K2Chi2/D");
  bsTree_->Branch("K2nHits", &K2nHits_, "K2nHits/D");
  bsTree_->Branch("K2pixH", &K2pixH_, "K2pixH/D");
  bsTree_->Branch("K2trkH", &K2trkH_, "K2trkH/D");
  
  bsTree_->Branch("Mu1Chi2", &Mu1Chi2_, "Mu1Chi2/D");
  bsTree_->Branch("Mu1nHits", &Mu1nHits_, "Mu1nHits/D");
  bsTree_->Branch("Mu1pixH", &Mu1pixH_, "Mu1pixH/D");
  bsTree_->Branch("Mu1trkH", &Mu1trkH_, "Mu1trkH/D");
  bsTree_->Branch("Mu2nHits", &Mu2nHits_, "Mu2nHits/D");
  bsTree_->Branch("Mu2pixH", &Mu2pixH_, "Mu2pixH/D");
  bsTree_->Branch("Mu2trkH", &Mu2trkH_, "Mu2trkH/D");
  
  bsTree_->Branch("K1trkLay", &K1trkLay_, "K1trkLay/I");
  bsTree_->Branch("K1muDTh", &K1muDTh_, "K1muDTh/I");
  bsTree_->Branch("K1muCSCh", &K1muCSCh_, "K1muCSCh/I");
  bsTree_->Branch("K1muRPCh", &K1muRPCh_, "K1muRPCh/I");
  bsTree_->Branch("K2trkLay", &K2trkLay_, "K2trkLay/I");
  bsTree_->Branch("K2muDTh", &K2muDTh_, "K2muDTh/I");
  bsTree_->Branch("K2muCSCh", &K2muCSCh_, "K2muCSCh/I");
  bsTree_->Branch("K2muRPCh", &K2muRPCh_, "K2muRPCh/I");
  bsTree_->Branch("Mu1trkLay", &Mu1trkLay_, "Mu1trkLay/I");
  bsTree_->Branch("Mu1muDTh", &Mu1muDTh_, "Mu1muDTh/I");
  bsTree_->Branch("Mu1muCSCh", &Mu1muCSCh_, "Mu1muCSCh/I");
  bsTree_->Branch("Mu1muRPCh", &Mu1muRPCh_, "Mu1muRPCh/I");
  bsTree_->Branch("Mu2trkLay", &Mu2trkLay_, "Mu2trkLay/I");
  bsTree_->Branch("Mu2muDTh", &Mu2muDTh_, "Mu2muDTh/I");
  bsTree_->Branch("Mu2muCSCh", &Mu2muCSCh_, "Mu2muCSCh/I");
  bsTree_->Branch("Mu2muRPCh", &Mu2muRPCh_, "Mu2muRPCh/I");
  
  bsTree_->Branch("costheta", &costheta_, "costheta/D");
  bsTree_->Branch("phi", &phi_, "phi/D");
  bsTree_->Branch("cospsi", &cospsi_, "cospsi/D");
  bsTree_->Branch("AngleBsDecayLength", &AngleBsDecayLength_, "AngleBsDecayLength/D");
  
  bsTree_->Branch("isMatched", &isMatched_, "isMatched/I");
  
  bsTree_->Branch("K1mcId", &K1mcId_, "K1mcId/I");
  bsTree_->Branch("K1momId", &K1momId_, "K1momId/I");
  bsTree_->Branch("K1gmomId", &K1gmomId_, "K1gmomId/I");
  bsTree_->Branch("K2mcId", &K2mcId_, "K2mcId/I");
  bsTree_->Branch("K2momId", &K2momId_, "K2momId/I");
  bsTree_->Branch("K2gmomId", &K2gmomId_, "K2gmomId/I");
  bsTree_->Branch("Mu1mcId", &Mu1mcId_, "Mu1mcId/I");
  bsTree_->Branch("Mu1momId", &Mu1momId_, "Mu1momId/I");
  bsTree_->Branch("Mu1gmomId", &Mu1gmomId_, "Mu1gmomId/I");
  bsTree_->Branch("Mu2mcId", &Mu2mcId_, "Mu2mcId/I");
  bsTree_->Branch("Mu2momId", &Mu2momId_, "Mu2momId/I");
  bsTree_->Branch("Mu2gmomId", &Mu2gmomId_, "Mu2gmomId/I");
  bsTree_->Branch("K1Truth", &K1Truth_, "K1Truth/I");
  bsTree_->Branch("K2Truth", &K2Truth_, "K2Truth/I");
  bsTree_->Branch("Mu1Truth", &Mu1Truth_, "Mu1Truth/I");
  bsTree_->Branch("Mu2Truth", &Mu2Truth_, "Mu2Truth/I");
  
  bsTree_->Branch("Dist3d", &Dist3d_, "Dist3d/D");
  bsTree_->Branch("dDist3d", &dDist3d_, "dDist3d/D");
  bsTree_->Branch("Time3d", &Time3d_, "Time3d/D");
  bsTree_->Branch("dTime3d", &dTime3d_, "dTime3d/D");
  bsTree_->Branch("Dist", &Dist_, "Dist/D");
  bsTree_->Branch("dDist", &dDist_, "dDist/D");
  bsTree_->Branch("Time", &Time_, "Time/D");
  bsTree_->Branch("dTime", &dTime_, "dTime/D");

  bsTree_->Branch("dedxTrk", &dedxTrk_, "dedxTrk/D");
  bsTree_->Branch("errdedxTrk", &errdedxTrk_, "errdedxTrk/D");
  bsTree_->Branch("numdedxTrk", &numdedxTrk_, "numdedxTrk/I");
 
  bsTree_->Branch("iPassedCutIdent", &iPassedCutIdent_, "iPassedCutIdent/I");  

  bsTree_->Branch("isGenBsEvent", &isGenBsEvent_, "isGenBsEvent/I");
  bsTree_->Branch("GenNumberOfBsDaughters", &GenNumberOfBsDaughters_, "GenNumberOfBsDaughters/I");
  bsTree_->Branch("isGenBsJpsiPhiEvent", &isGenBsJpsiPhiEvent_, "isGenBsJpsiPhiEvent/I");
  bsTree_->Branch("isGenBsJpsiPhiMuMuKKEvent", & isGenBsJpsiPhiMuMuKKEvent_, " isGenBsJpsiPhiMuMuKKEvent/I");
 
  bsTree_->Branch("genBsM", &genBsM_, "genBsM/D"); 
  bsTree_->Branch("genBsPt", &genBsPt_, "genBsPt/D");
  bsTree_->Branch("genBsPz", &genBsPz_, "genBsPz/D");
  bsTree_->Branch("genBsEta", &genBsEta_, "genBsEta/D");
  bsTree_->Branch("genBsPhi", &genBsPhi_, "genBsPhi/D");
  bsTree_->Branch("genJpsiM", &genJpsiM_, "genJpsiM/D"); 
  bsTree_->Branch("genJpsiPt", &genJpsiPt_, "genJpsiPt/D");
  bsTree_->Branch("genJpsiPz", &genJpsiPz_, "genJpsiPz/D");
  bsTree_->Branch("genJpsiEta", &genJpsiEta_, "genJpsiEta/D");
  bsTree_->Branch("genJpsiPhi", &genJpsiPhi_, "genJpsiPhi/D");
  bsTree_->Branch("genPhiM", &genPhiM_, "genPhiM/D"); 
  bsTree_->Branch("genPhiPt", &genPhiPt_, "genPhiPt/D");
  bsTree_->Branch("genPhiPz", &genPhiPz_, "genPhiPz/D");
  bsTree_->Branch("genPhiEta", &genPhiEta_, "genPhiEta/D");
  bsTree_->Branch("genPhiPhi", &genPhiPhi_, "genPhiPhi/D");
  bsTree_->Branch("genMu1M", &genMu1M_, "genMu1M/D"); 
  bsTree_->Branch("genMu1Pt", &genMu1Pt_, "genMu1Pt/D");
  bsTree_->Branch("genMu1Pz", &genMu1Pz_, "genMu1Pz/D");
  bsTree_->Branch("genMu1Eta", &genMu1Eta_, "genMu1Eta/D");
  bsTree_->Branch("genMu1Phi", &genMu1Phi_, "genMu1Phi/D");
  bsTree_->Branch("genMu2M", &genMu2M_, "genMu2M/D"); 
  bsTree_->Branch("genMu2Pt", &genMu2Pt_, "genMu2Pt/D");
  bsTree_->Branch("genMu2Pz", &genMu2Pz_, "genMu2Pz/D");
  bsTree_->Branch("genMu2Eta", &genMu2Eta_, "genMu2Eta/D");
  bsTree_->Branch("genMu2Phi", &genMu2Phi_, "genMu2Phi/D");
  bsTree_->Branch("genK1M", &genK1M_, "genK1M/D"); 
  bsTree_->Branch("genK1Pt", &genK1Pt_, "genK1Pt/D");
  bsTree_->Branch("genK1Pz", &genK1Pz_, "genK1Pz/D");
  bsTree_->Branch("genK1Eta", &genK1Eta_, "genK1Eta/D");
  bsTree_->Branch("genK1Phi", &genK1Phi_, "genK1Phi/D");
  bsTree_->Branch("genK2M", &genK2M_, "genK2M/D"); 
  bsTree_->Branch("genK2Pt", &genK2Pt_, "genK2Pt/D");
  bsTree_->Branch("genK2Pz", &genK2Pz_, "genK2Pz/D");
  bsTree_->Branch("genK2Eta", &genK2Eta_, "genK2Eta/D");
  bsTree_->Branch("genK2Phi", &genK2Phi_, "genK2Phi/D");

  bsTree_->Branch("K1_par0KK", &K1_kk_par0_, "K1_par0KK/D");
  bsTree_->Branch("K1_par1KK", &K1_kk_par1_, "K1_par1KK/D");
  bsTree_->Branch("K1_par2KK", &K1_kk_par2_, "K1_par2KK/D");
  bsTree_->Branch("K1_par3KK", &K1_kk_par3_, "K1_par3KK/D");
  bsTree_->Branch("K1_par4KK", &K1_kk_par4_, "K1_par4KK/D");
  bsTree_->Branch("K1_par5KK", &K1_kk_par5_, "K1_par5KK/D");
  bsTree_->Branch("K1_par6KK", &K1_kk_par6_, "K1_par6KK/D");

  bsTree_->Branch("K2_par0KK", &K2_kk_par0_, "K2_par0KK/D");
  bsTree_->Branch("K2_par1KK", &K2_kk_par1_, "K2_par1KK/D");
  bsTree_->Branch("K2_par2KK", &K2_kk_par2_, "K2_par2KK/D");
  bsTree_->Branch("K2_par3KK", &K2_kk_par3_, "K2_par3KK/D");
  bsTree_->Branch("K2_par4KK", &K2_kk_par4_, "K2_par4KK/D");
  bsTree_->Branch("K2_par5KK", &K2_kk_par5_, "K2_par5KK/D");
  bsTree_->Branch("K2_par6KK", &K2_kk_par6_, "K2_par6KK/D");

  bsTree_->Branch("K1_par0Kpi", &K1_kpi_par0_, "K1_par0Kpi/D");
  bsTree_->Branch("K1_par1Kpi", &K1_kpi_par1_, "K1_par1Kpi/D");
  bsTree_->Branch("K1_par2Kpi", &K1_kpi_par2_, "K1_par2Kpi/D");
  bsTree_->Branch("K1_par3Kpi", &K1_kpi_par3_, "K1_par3Kpi/D");
  bsTree_->Branch("K1_par4Kpi", &K1_kpi_par4_, "K1_par4Kpi/D");
  bsTree_->Branch("K1_par5Kpi", &K1_kpi_par5_, "K1_par5Kpi/D");
  bsTree_->Branch("K1_par6Kpi", &K1_kpi_par6_, "K1_par6Kpi/D");

  bsTree_->Branch("K2_par0Kpi", &K2_kpi_par0_, "K2_par0Kpi/D");
  bsTree_->Branch("K2_par1Kpi", &K2_kpi_par1_, "K2_par1Kpi/D");
  bsTree_->Branch("K2_par2Kpi", &K2_kpi_par2_, "K2_par2Kpi/D");
  bsTree_->Branch("K2_par3Kpi", &K2_kpi_par3_, "K2_par3Kpi/D");
  bsTree_->Branch("K2_par4Kpi", &K2_kpi_par4_, "K2_par4Kpi/D");
  bsTree_->Branch("K2_par5Kpi", &K2_kpi_par5_, "K2_par5Kpi/D");
  bsTree_->Branch("K2_par6Kpi", &K2_kpi_par6_, "K2_par6Kpi/D");

  bsTree_->Branch("K1_par0pipi", &K1_pipi_par0_, "K1_par0pipi/D");
  bsTree_->Branch("K1_par1pipi", &K1_pipi_par1_, "K1_par1pipi/D");
  bsTree_->Branch("K1_par2pipi", &K1_pipi_par2_, "K1_par2pipi/D");
  bsTree_->Branch("K1_par3pipi", &K1_pipi_par3_, "K1_par3pipi/D");
  bsTree_->Branch("K1_par4pipi", &K1_pipi_par4_, "K1_par4pipi/D");
  bsTree_->Branch("K1_par5pipi", &K1_pipi_par5_, "K1_par5pipi/D");
  bsTree_->Branch("K1_par6pipi", &K1_pipi_par6_, "K1_par6pipi/D");

  bsTree_->Branch("K2_par0pipi", &K2_pipi_par0_, "K2_par0pipi/D");
  bsTree_->Branch("K2_par1pipi", &K2_pipi_par1_, "K2_par1pipi/D");
  bsTree_->Branch("K2_par2pipi", &K2_pipi_par2_, "K2_par2pipi/D");
  bsTree_->Branch("K2_par3pipi", &K2_pipi_par3_, "K2_par3pipi/D");
  bsTree_->Branch("K2_par4pipi", &K2_pipi_par4_, "K2_par4pipi/D");
  bsTree_->Branch("K2_par5pipi", &K2_pipi_par5_, "K2_par5pipi/D");
  bsTree_->Branch("K2_par6pipi", &K2_pipi_par6_, "K2_par6pipi/D");

  bsTree_->Branch("K1_kk_sigX_", &K1_kk_sigX_, "K1_kk_sigX/D");
  bsTree_->Branch("K1_kk_sigY_", &K1_kk_sigY_, "K1_kk_sigY/D");
  bsTree_->Branch("K1_kk_sigZ_", &K1_kk_sigZ_, "K1_kk_sigZ/D");

  bsTree_->Branch("K1_kpi_sigX_", &K1_kpi_sigX_, "K1_kpi_sigX/D");
  bsTree_->Branch("K1_kpi_sigY_", &K1_kpi_sigY_, "K1_kpi_sigY/D");
  bsTree_->Branch("K1_kpi_sigZ_", &K1_kpi_sigZ_, "K1_kpi_sigZ/D");

  bsTree_->Branch("K1_pipi_sigX_", &K1_pipi_sigX_, "K1_pipi_sigX/D");
  bsTree_->Branch("K1_pipi_sigY_", &K1_pipi_sigY_, "K1_pipi_sigY/D");
  bsTree_->Branch("K1_pipi_sigZ_", &K1_pipi_sigZ_, "K1_pipi_sigZ/D");

  bsTree_->Branch("K2_kk_sigX_", &K2_kk_sigX_, "K2_kk_sigX/D");
  bsTree_->Branch("K2_kk_sigY_", &K2_kk_sigY_, "K2_kk_sigY/D");
  bsTree_->Branch("K2_kk_sigZ_", &K2_kk_sigZ_, "K2_kk_sigZ/D");

  bsTree_->Branch("K2_kpi_sigX_", &K2_kpi_sigX_, "K2_kpi_sigX/D");
  bsTree_->Branch("K2_kpi_sigY_", &K2_kpi_sigY_, "K2_kpi_sigY/D");
  bsTree_->Branch("K2_kpi_sigZ_", &K2_kpi_sigZ_, "K2_kpi_sigZ/D");

  bsTree_->Branch("K2_pipi_sigX_", &K2_pipi_sigX_, "K2_pipi_sigX/D");
  bsTree_->Branch("K2_pipi_sigY_", &K2_pipi_sigY_, "K2_pipi_sigY/D");
  bsTree_->Branch("K2_pipi_sigZ_", &K2_pipi_sigZ_, "K2_pipi_sigZ/D");

  bsTree_->Branch("K1_kk_sigPX_", &K1_kk_sigPX_, "K1_kk_sigPX/D");
  bsTree_->Branch("K1_kk_sigPY_", &K1_kk_sigPY_, "K1_kk_sigPY/D");
  bsTree_->Branch("K1_kk_sigPZ_", &K1_kk_sigPZ_, "K1_kk_sigPZ/D");

  bsTree_->Branch("K1_kpi_sigPX_", &K1_kpi_sigPX_, "K1_kpi_sigPX/D");
  bsTree_->Branch("K1_kpi_sigPY_", &K1_kpi_sigPY_, "K1_kpi_sigPY/D");
  bsTree_->Branch("K1_kpi_sigPZ_", &K1_kpi_sigPZ_, "K1_kpi_sigPZ/D");

  bsTree_->Branch("K1_pipi_sigPX_", &K1_pipi_sigPX_, "K1_pipi_sigPX/D");
  bsTree_->Branch("K1_pipi_sigPY_", &K1_pipi_sigPY_, "K1_pipi_sigPY/D");
  bsTree_->Branch("K1_pipi_sigPZ_", &K1_pipi_sigPZ_, "K1_pipi_sigPZ/D");

  bsTree_->Branch("K2_kk_sigPX_", &K2_kk_sigPX_, "K2_kk_sigPX/D");
  bsTree_->Branch("K2_kk_sigPY_", &K2_kk_sigPY_, "K2_kk_sigPY/D");
  bsTree_->Branch("K2_kk_sigPZ_", &K2_kk_sigPZ_, "K2_kk_sigPZ/D");

  bsTree_->Branch("K2_kpi_sigPX_", &K2_kpi_sigPX_, "K2_kpi_sigPX/D");
  bsTree_->Branch("K2_kpi_sigPY_", &K2_kpi_sigPY_, "K2_kpi_sigPY/D");
  bsTree_->Branch("K2_kpi_sigPZ_", &K2_kpi_sigPZ_, "K2_kpi_sigPZ/D");

  bsTree_->Branch("K2_pipi_sigPX_", &K2_pipi_sigPX_, "K2_pipi_sigPX/D");
  bsTree_->Branch("K2_pipi_sigPY_", &K2_pipi_sigPY_, "K2_pipi_sigPY/D");
  bsTree_->Branch("K2_pipi_sigPZ_", &K2_pipi_sigPZ_, "K2_pipi_sigPZ/D");

  bsTree_->Branch("sigmaK1Pt", &K1Pt_error_, "sigmaK1Pt/D");
  bsTree_->Branch("sigmaK2Pt", &K2Pt_error_, "sigmaK2Pt/D");

  bsTree_->Branch("isGenBdEvent", &isGenBdEvent_, "isGenBdEvent/I");
  bsTree_->Branch("GenNumberOfBdDaughters", &GenNumberOfBdDaughters_, "GenNumberOfBdDaughters/I");
  bsTree_->Branch("isGenBdJpsiKstarEvent", &isGenBdJpsiKstarEvent_, "isGenBdJpsiKstarEvent/I");
  bsTree_->Branch("isGenBdJpsiKstarMuMuKpiEvent", & isGenBdJpsiKstarMuMuKpiEvent_, " isGenBdJpsiKstarMuMuKpiEvent/I");

  bsTree_->Branch("isGenBsJpsiKKEvent", &isGenBsJpsiKKEvent_, "isGenBsJpsiKKEvent/I");
  bsTree_->Branch("BdDausId", &BdDausId_, "BdDausId/I");
}

BsToJpsiPhiRootTree::~BsToJpsiPhiRootTree()
{
  bsFile_->Write();
  bsFile_->Close();
  
}

void BsToJpsiPhiRootTree::resetEntries()
{

  triggerbit_HLTmu3_ = -10;
  triggerbit_HLTmu5_ = -10;
  triggerbit_HLTmu9_ = -10;
  triggerbit_HLTdoubleIsoMu3_ = -10;
  triggerbit_HLTdoubleMu3_ = -10;
  triggerbit_HLTdoubleMu3_JPsi_ = -10;

  nBsDau_ = -10; 
  momID_ = -10;
  dau_1_ID_ = -10;
  dau_2_ID_ = -10;

  BSx_ = -10;
  BSy_ = -10;
  BSz_ = -10;
  PVx_ = -10;
  PVy_ = -10;
  PVz_ = -10;
  PVerrx_ = -10;
  PVerry_ = -10;
  PVerrz_ = -10;
  
  JpsiVtxProb_ = -10;
  JpsiM_alone_ = -10;
  JpsiPhi_alone_ = -10;
  JpsiEta_alone_ = -10;
  JpsiPt_alone_ = -10;
  JpsiMu1Pt_ = -10;
  JpsiMu2Pt_ = -10;
  JpsiMu1Phi_ = -10;
  JpsiMu2Phi_ = -10;
  JpsiMu1Eta_ = -10;
  JpsiMu2Eta_ = -10;
  JpsiMuon1Cat_ = -10;
  JpsiMuon2Cat_ = -10;

  BsMass_before_ = -10;  
  BsPhi_before_ = -10;
  BsEta_before_ = -10;
  BsPt_before_ = -10;

  JpsiMass_before_ = -10;
  JpsiPhi_before_ = -10;
  JpsiEta_before_ = -10;
  JpsiPt_before_ = -10;

  PhiMass_before_ = -10;
  PhiPhi_before_ = -10;
  PhiEta_before_ = -10;
  PhiPt_before_ = -10;

  K1Pt_before_ = -10;
  K1Phi_before_ = -10;
  K1Eta_before_ = -10;
  K2Pt_before_ = -10;
  K2Phi_before_ = -10;
  K2Eta_before_ = -10;

  chi2_Bs_ = -10;
  ndof_Bs_ = -10;

  BsVtxProb_ = -10;
  BsVtxProbKpi_ = -10;
  BsVtxProbpipi_ = -10;
  BsVtx_x_ = -10;
  BsVtx_y_ = -10;
  BsVtx_z_ = -10;

  BfitM_KK_ = -10;  
  BfitM_Kpi_= -10;
  BfitM_pipi_= -10;


  BsMass_after_ = -10;
  BsPhi_after_ = -10;
  BsEta_after_ = -10;
  BsPt_after_ = -10;

  JpsiMass_after_ = -10;
  JpsiPhi_after_ = -10;
  JpsiEta_after_ = -10;
  JpsiPt_after_ = -10;

  PhiMass_after_ = -10;
  PhiPhi_after_ = -10;
  PhiEta_after_ = -10;
  PhiPt_after_ = -10;

  K1Pt_after_ = -10;
  K1Phi_after_ = -10;
  K1Eta_after_ = -10;
  K2Pt_after_ = -10;
  K2Phi_after_ = -10;
  K2Eta_after_ = -10;

  K1Chi2_ = -10;
  K1nHits_ = -10;
  K2Chi2_ = -10;
  K2nHits_ = -10;
  K1pixH_ = -10;
  K1trkH_ = -10;
  K2pixH_ = -10;
  K2trkH_ = -10;
  
  Mu1Chi2_ = -10;
  Mu1nHits_ = -10;
  Mu2Chi2_ = -10;
  Mu2nHits_ = -10;
  Mu1pixH_ = -10;
  Mu1trkH_ = -10;
  Mu2pixH_ = -10;
  Mu2trkH_ = -10;

  BLxy_ = -10;
  BLxy2_ = -10;
  BerrX_ = -10;
  BerrY_ = -10;
  BerrXY_ = -10;
  Bsct1_ = -10;
  Bsct2_ = -10;
  
  costheta_ = -10;
  phi_ = -10;
  cospsi_ = -10; 
  AngleBsDecayLength_ = -10;

  isMatched_ = -10;
  
  K1trkLay_ = -10;
  K1muDTh_ = -10;
  K1muCSCh_ = -10;
  K1muRPCh_ = -10;
  K2trkLay_ = -10;
  K2muDTh_ = -10;
  K2muCSCh_ = -10;
  K2muRPCh_ = -10;
  Mu1trkLay_ = -10;
  Mu1muDTh_ = -10;
  Mu1muCSCh_ = -10;
  Mu1muRPCh_ = -10;
  Mu2trkLay_ = -10;
  Mu2muDTh_ = -10;
  Mu2muCSCh_ = -10;
  Mu2muRPCh_ = -10;

  K1mcId_ = -10;
  K1momId_ = -10;
  K1gmomId_ = -10;
  K1Truth_ = -10;
  K2mcId_ = -10;
  K2momId_ = -10;
  K2gmomId_ = -10;
  K2Truth_ = -10;
  Mu1mcId_ = -10;
  Mu1momId_ = -10;
  Mu1gmomId_ = -10;
  Mu1Truth_ = -10;
  Mu2mcId_ = -10;
  Mu2momId_ = -10;
  Mu2gmomId_ = -10;
  Mu2Truth_ = -10;  
  
  Dist3d_ = -10;
  dDist3d_ = -10;
  Time3d_ = -10;
  dTime3d_ = -10;
  Dist_ = -10;
  dDist_ = -10;
  Time_ = -10;
  dTime_ = -10;

  dedxTrk_ = -10;
  errdedxTrk_ = -10;
  numdedxTrk_ = -10;
  
  iPassedCutIdent_ = -10;

  isGenBsEvent_ = GenNumberOfBsDaughters_ = isGenBsJpsiPhiEvent_ = isGenBsJpsiPhiMuMuKKEvent_ = -10;
  isGenBdEvent_ = GenNumberOfBdDaughters_ = isGenBdJpsiKstarEvent_ = isGenBdJpsiKstarMuMuKpiEvent_ = -10;  

  genBsM_ = genBsPt_ = genBsEta_ = genBsPhi_ = -10;
  genJpsiM_ = genJpsiPt_= genJpsiEta_= genJpsiPhi_ = -10;
  genPhiM_= genPhiPt_= genPhiEta_= genPhiPhi_ = -10;
  genMu1M_= genMu1Pt_= genMu1Eta_= genMu1Phi_ = -10;
  genMu2M_= genMu2Pt_= genMu2Eta_= genMu2Phi_ = -10;
  genK1M_= genK1Pt_= genK1Eta_= genK1Phi_ = -10;
  genK2M_= genK2Pt_= genK2Eta_= genK2Phi_ = -10;

  K1_kk_par0_ = -100;
  K1_kk_par1_ = -100;
  K1_kk_par2_ = -100;
  K1_kk_par3_ = -100;
  K1_kk_par4_ = -100;
  K1_kk_par5_ = -100;
  K1_kk_par6_ = -100;

  K2_kk_par0_ = -100;
  K2_kk_par1_ = -100;
  K2_kk_par2_ = -100;
  K2_kk_par3_ = -100;
  K2_kk_par4_ = -100;
  K2_kk_par5_ = -100;
  K2_kk_par6_ = -100;

  K1_kpi_par0_ = -100;
  K1_kpi_par1_ = -100;
  K1_kpi_par2_ = -100;
  K1_kpi_par3_ = -100;
  K1_kpi_par4_ = -100;
  K1_kpi_par5_ = -100;
  K1_kpi_par6_ = -100;

  K2_kpi_par0_ = -100;
  K2_kpi_par1_ = -100;
  K2_kpi_par2_ = -100;
  K2_kpi_par3_ = -100;
  K2_kpi_par4_ = -100;
  K2_kpi_par5_ = -100;
  K2_kpi_par6_ = -100;

  K1_pipi_par0_ = -100;
  K1_pipi_par1_ = -100;
  K1_pipi_par2_ = -100;
  K1_pipi_par3_ = -100;
  K1_pipi_par4_ = -100;
  K1_pipi_par5_ = -100;
  K1_pipi_par6_ = -100;

  K2_pipi_par0_ = -100;
  K2_pipi_par1_ = -100;
  K2_pipi_par2_ = -100;
  K2_pipi_par3_ = -100;
  K2_pipi_par4_ = -100;
  K2_pipi_par5_ = -100;
  K2_pipi_par6_ = -100;

  K1_kk_sigX_ = -10;
  K1_kk_sigY_ = -10;
  K1_kk_sigZ_ = -10;

  K1_kpi_sigX_ = -10;
  K1_kpi_sigY_ = -10;
  K1_kpi_sigZ_ = -10;

  K1_pipi_sigX_ = -10;
  K1_pipi_sigY_ = -10;
  K1_pipi_sigZ_ = -10;

  K2_kk_sigX_ = -10;
  K2_kk_sigY_ = -10;
  K2_kk_sigZ_ = -10;

  K2_kpi_sigX_ = -10;
  K2_kpi_sigY_ = -10;
  K2_kpi_sigZ_ = -10;

  K2_pipi_sigX_ = -10;
  K2_pipi_sigY_ = -10;
  K2_pipi_sigZ_ = -10;

  K1_kk_sigPX_ = -10;
  K1_kk_sigPY_ = -10;
  K1_kk_sigPZ_ = -10;

  K1_kpi_sigPX_ = -10;
  K1_kpi_sigPY_ = -10;
  K1_kpi_sigPZ_ = -10;

  K1_pipi_sigPX_ = -10;
  K1_pipi_sigPY_ = -10;
  K1_pipi_sigPZ_ = -10;

  K2_kk_sigPX_ = -10;
  K2_kk_sigPY_ = -10;
  K2_kk_sigPZ_ = -10;

  K2_kpi_sigPX_ = -10;
  K2_kpi_sigPY_ = -10;
  K2_kpi_sigPZ_ = -10;

  K2_pipi_sigPX_ = -10;
  K2_pipi_sigPY_ = -10;
  K2_pipi_sigPZ_ = -10;

  K1Pt_error_ = -10;
  K2Pt_error_ = -10;

  isGenBsJpsiKKEvent_ = -10;
  BdDausId_ = -10;

} 

void BsToJpsiPhiRootTree::getDeDx(const double f1, const double f2, const int f3)
{
  dedxTrk_ = f1;
  errdedxTrk_ = f2;
  numdedxTrk_ = f3;
}

void BsToJpsiPhiRootTree::getTrigBit(const int flag_1, const int flag_2, const int flag_3, const int flag_4, const int flag_5, const int flag_6)
{
  triggerbit_HLTmu3_ = flag_1;
  triggerbit_HLTmu5_ = flag_2;
  triggerbit_HLTmu9_ = flag_3;
  triggerbit_HLTdoubleIsoMu3_ = flag_4;
  triggerbit_HLTdoubleMu3_ = flag_5;
  triggerbit_HLTdoubleMu3_JPsi_ = flag_6;

}




void BsToJpsiPhiRootTree::getVtx(const double aa, const double bb, const double cc, const double dd, const double ee, const double ff,
				 const double gg, const double hh, const double ii)
{
  BSx_ = aa;
  BSy_ = bb;
  BSz_ = cc;
  PVx_ = dd;
  PVy_ = ee;
  PVz_ = ff;
  PVerrx_ = gg;
  PVerry_ = hh;
  PVerrz_ = ii;
}



void BsToJpsiPhiRootTree::getMCmatch(const int aa)
{
  isMatched_ = aa; 
}

void BsToJpsiPhiRootTree::getAngles(const double aa, const double bb, const double cc, const double dd)
{
  costheta_ = aa;
  phi_ = bb;
  cospsi_ = cc;
  AngleBsDecayLength_ = dd;
}

void BsToJpsiPhiRootTree::getLXY(const double aa, const double bb, const double cc, const double dd, const double ee, const double ff, const double gg)
{
  BLxy_ = aa;
  BLxy2_ = bb;
  BerrX_ = cc;
  BerrY_ = dd;
  BerrXY_ = ee;
  Bsct1_ = ff;
  Bsct2_ = ff;
}

void BsToJpsiPhiRootTree::getInfoK1(const int aa, const int bb, const int cc, const int dd)
{
  K1mcId_ = aa;
  K1momId_ = bb;
  K1gmomId_ = cc;
  K1Truth_ = dd;

}

void BsToJpsiPhiRootTree::getInfoK2(const int aa, const int bb, const int cc, const int dd)
{
  K2mcId_ = aa;
  K2momId_ = bb;
  K2gmomId_ = cc;
  K2Truth_ = dd;

}

void BsToJpsiPhiRootTree::getInfoMu1(const int aa, const int bb, const int cc, const int dd)
{
  Mu1mcId_ = aa;
  Mu1momId_ = bb;
  Mu1gmomId_ = cc;
  Mu1Truth_ = dd;

}

void BsToJpsiPhiRootTree::getInfoMu2(const int aa, const int bb, const int cc, const int dd)
{
  Mu2mcId_ = aa;
  Mu2momId_ = bb;
  Mu2gmomId_ = cc;
  Mu2Truth_ = dd;

}

void BsToJpsiPhiRootTree::get3d(const double aa, const double bb, const double cc, const double dd)
{
  Dist3d_ = aa;
  dDist3d_ = bb;
  Time3d_ = cc;
  dTime3d_ = dd;
}

void BsToJpsiPhiRootTree::get1d(const double aa, const double bb, const double cc, const double dd)
{
  Dist_ = aa;
  dDist_ = bb;
  Time_ = cc;
  dTime_ = dd;
}

void BsToJpsiPhiRootTree::fill()
{
  bsTree_->Fill();
}

void BsToJpsiPhiRootTree::setFitParKK(RefCountedKinematicTree& myTree)
{
  
  vector< RefCountedKinematicParticle > bs_children = myTree->finalStateParticles();
  
  // first particle: kaon 1   
  
  AlgebraicVector7 bs_par1 = bs_children[0]->currentState().kinematicParameters().vector();
  
  K1_kk_par0_ = bs_par1[0];
  K1_kk_par1_ = bs_par1[1];
  K1_kk_par2_ = bs_par1[2];
  K1_kk_par3_ = bs_par1[3];
  K1_kk_par4_ = bs_par1[4];
  K1_kk_par5_ = bs_par1[5];
  K1_kk_par6_ = bs_par1[6];
  
  AlgebraicSymMatrix77 bs_err1 = bs_children[0]->currentState().kinematicParametersError().matrix();
  K1_kk_sigX_ = sqrt(bs_err1(0,0));
  K1_kk_sigY_ = sqrt(bs_err1(1,1));
  K1_kk_sigZ_ = sqrt(bs_err1(2,2));
  K1_kk_sigX_ = sqrt(bs_err1(3,3));
  K1_kk_sigY_ = sqrt(bs_err1(4,4));
  K1_kk_sigZ_ = sqrt(bs_err1(5,5));
  
  // first particle: kaon 2  
  
    
  AlgebraicVector7 bs_par2 = bs_children[1]->currentState().kinematicParameters().vector();
  
  K2_kk_par0_ = bs_par2[0];
  K2_kk_par1_ = bs_par2[1];
  K2_kk_par2_ = bs_par2[2];
  K2_kk_par3_ = bs_par2[3];
  K2_kk_par4_ = bs_par2[4];
  K2_kk_par5_ = bs_par2[5];
  K2_kk_par6_ = bs_par2[6];

  AlgebraicSymMatrix77 bs_err2 = bs_children[1]->currentState().kinematicParametersError().matrix();
  K2_kk_sigX_ = sqrt(bs_err2(0,0));
  K2_kk_sigY_ = sqrt(bs_err2(1,1));
  K2_kk_sigZ_ = sqrt(bs_err2(2,2));
  K2_kk_sigPX_ = sqrt(bs_err2(3,3));
  K2_kk_sigPY_ = sqrt(bs_err2(4,4));
  K2_kk_sigPZ_ = sqrt(bs_err2(5,5));
  
}

void BsToJpsiPhiRootTree::setFitParKpi(RefCountedKinematicTree& myTree)
{
  
  vector< RefCountedKinematicParticle > bs_children = myTree->finalStateParticles();
  
  // first particle: kaon 1   
  
  AlgebraicVector7 bs_par1 = bs_children[0]->currentState().kinematicParameters().vector();
  
  K1_kpi_par0_ = bs_par1[0];
  K1_kpi_par1_ = bs_par1[1];
  K1_kpi_par2_ = bs_par1[2];
  K1_kpi_par3_ = bs_par1[3];
  K1_kpi_par4_ = bs_par1[4];
  K1_kpi_par5_ = bs_par1[5];
  K1_kpi_par6_ = bs_par1[6];
  
  AlgebraicSymMatrix77 bs_err1 = bs_children[0]->currentState().kinematicParametersError().matrix();
  K1_kpi_sigX_ = sqrt(bs_err1(0,0));
  K1_kpi_sigY_ = sqrt(bs_err1(1,1));
  K1_kpi_sigZ_ = sqrt(bs_err1(2,2));
  K1_kpi_sigPX_ = sqrt(bs_err1(3,3));
  K1_kpi_sigPY_ = sqrt(bs_err1(4,4));
  K1_kpi_sigPZ_ = sqrt(bs_err1(5,5));
  
  // first particle: kaon 2  
  
    
  AlgebraicVector7 bs_par2 = bs_children[1]->currentState().kinematicParameters().vector();
  
  K2_kpi_par0_ = bs_par2[0];
  K2_kpi_par1_ = bs_par2[1];
  K2_kpi_par2_ = bs_par2[2];
  K2_kpi_par3_ = bs_par2[3];
  K2_kpi_par4_ = bs_par2[4];
  K2_kpi_par5_ = bs_par2[5];
  K2_kpi_par6_ = bs_par2[6];

  AlgebraicSymMatrix77 bs_err2 = bs_children[1]->currentState().kinematicParametersError().matrix();
  K2_kpi_sigX_ = sqrt(bs_err2(0,0));
  K2_kpi_sigY_ = sqrt(bs_err2(1,1));
  K2_kpi_sigZ_ = sqrt(bs_err2(2,2));
  K2_kpi_sigPX_ = sqrt(bs_err2(3,3));
  K2_kpi_sigPY_ = sqrt(bs_err2(4,4));
  K2_kpi_sigPZ_ = sqrt(bs_err2(5,5));
  
}

void BsToJpsiPhiRootTree::setFitParpipi(RefCountedKinematicTree& myTree)
{
 
  vector< RefCountedKinematicParticle > bs_children = myTree->finalStateParticles();
  
  // first particle: kaon 1   
  
  AlgebraicVector7 bs_par1 = bs_children[0]->currentState().kinematicParameters().vector();
  
  K1_pipi_par0_ = bs_par1[0];
  K1_pipi_par1_ = bs_par1[1];
  K1_pipi_par2_ = bs_par1[2];
  K1_pipi_par3_ = bs_par1[3];
  K1_pipi_par4_ = bs_par1[4];
  K1_pipi_par5_ = bs_par1[5];
  K1_pipi_par6_ = bs_par1[6];
  
  AlgebraicSymMatrix77 bs_err1 = bs_children[0]->currentState().kinematicParametersError().matrix();
  K1_pipi_sigX_ = sqrt(bs_err1(0,0));
  K1_pipi_sigY_ = sqrt(bs_err1(1,1));
  K1_pipi_sigZ_ = sqrt(bs_err1(2,2));
  K1_pipi_sigPX_ = sqrt(bs_err1(3,3));
  K1_pipi_sigPY_ = sqrt(bs_err1(4,4));
  K1_pipi_sigPZ_ = sqrt(bs_err1(5,5));
  
  // first particle: kaon 2  
  
    
  AlgebraicVector7 bs_par2 = bs_children[1]->currentState().kinematicParameters().vector();
  
  K2_pipi_par0_ = bs_par2[0];
  K2_pipi_par1_ = bs_par2[1];
  K2_pipi_par2_ = bs_par2[2];
  K2_pipi_par3_ = bs_par2[3];
  K2_pipi_par4_ = bs_par2[4];
  K2_pipi_par5_ = bs_par2[5];
  K2_pipi_par6_ = bs_par2[6];

  AlgebraicSymMatrix77 bs_err2 = bs_children[1]->currentState().kinematicParametersError().matrix();
  K2_pipi_sigX_ = sqrt(bs_err2(0,0));
  K2_pipi_sigY_ = sqrt(bs_err2(1,1));
  K2_pipi_sigZ_ = sqrt(bs_err2(2,2));
  K2_pipi_sigPX_ = sqrt(bs_err2(3,3));
  K2_pipi_sigPY_ = sqrt(bs_err2(4,4));
  K2_pipi_sigPZ_ = sqrt(bs_err2(5,5));
  
}

