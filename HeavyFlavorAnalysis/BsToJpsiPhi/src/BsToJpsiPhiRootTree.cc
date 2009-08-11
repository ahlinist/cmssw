#include "HeavyFlavorAnalysis/BsToJpsiPhi/interface/BsToJpsiPhiRootTree.h"

BsToJpsiPhiRootTree::BsToJpsiPhiRootTree(const std::string filename)
{
	//reset all values

	triggerbit_HLTmu3_ = -10;
	triggerbit_HLTmu5_ = -10;
	triggerbit_HLTmu9_ = -10;
	triggerbit_HLTdoubleIsoMu3_ = -10;
	triggerbit_HLTdoubleMu3_ = -10;
	triggerbit_HLTdoubleMu3_JPsi_ = -10;
	
	flagKstar_ =-10;
	flagKs_ =-10;
	flagPhi_ =-10;

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
	MuCounter1_ = -10;
	MuCounter2_ = -10;
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
	JpsiMass_before_ = -10;
	PhiMass_before_ = -10;
	 
	chi2_Bs_ = -10;
	ndof_Bs_ = -10;
	BsVtxProb_ = -10;
	JpsiM_ = -10;
	JpsiPhi_ = -10;
	JpsiEta_ = -10;
	JpsiPt_ = -10;
	PhiM_ = -10;
	PhiPhi_ = -10;
	PhiEta_ = -10;
	PhiPt_ = -10;
	BsM_ = -10;
	BsPhi_ = -10;
	BsEta_ = -10;
	BsPt_ = -10;

	BmassC_ = -10;
	BLxy_ = -10;
	BLxy2_ = -10;
	BerrX_ = -10;
	BerrY_ = -10;
	BerrXY_ = -10;
	Bsct1_ = -10;
	Bsct2_ = -10;

	K1Pt_ = -10;
	K2Pt_ = -10;
	K1Eta_ = -10;
	K2Eta_ = -10;
	K1Phi_ = -10;
	K2Phi_ = -10;
	K1Chi2_ = -10;
	K1nHits_ = -10;
	K2Chi2_ = -10;
	K2nHits_ = -10;
	K1pixH_ = -10;
	K1trkH_ = -10;
	K2pixH_ = -10;
	K2trkH_ = -10;
	
	Mu1Pt_ = -10;
	Mu2Pt_ = -10;
	Mu1Eta_ = -10;
	Mu2Eta_ = -10;
	Mu1Phi_ = -10;
	Mu2Phi_ = -10;
	Mu1Chi2_ = -10;
	Mu1nHits_ = -10;
	Mu2Chi2_ = -10;
	Mu2nHits_ = -10;
	Mu1pixH_ = -10;
	Mu1trkH_ = -10;
	Mu2pixH_ = -10;
	Mu2trkH_ = -10;
	
	costheta_ = -10;
	phi_ = -10;
	cospsi_ = -10;
	AngleBsDecayLength_ = -10;
	
	isMatched_ = -10;

	BdVtxProb_ = -10;
	KstarM_ = -10;
	BdM_ = -10;
	BdPhi_ = -10;
	BdEta_ = -10;
	BdPt_ = -10;
	BdmassC_ = -10;
	JpsiM_Bd_ = -10;
	AngleBsDecayLength_ = -10;

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

	dedx_ = -10;
	errdedx_ = -10;
	numdedx_ = -10;
	
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
	bsTree_->Branch("flagKstar", &flagKstar_, "flagKstar/I");
	bsTree_->Branch("flagKs", &flagKs_, "flagKs/I");
	bsTree_->Branch("flagPhi", &flagPhi_, "flagPhi/I");
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
        bsTree_->Branch("MuCounter1", &MuCounter1_, "MuCounter1/I");
        bsTree_->Branch("MuCounter2", &MuCounter2_, "MuCounter2/I");
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
        bsTree_->Branch("JpsiMass_before", &JpsiMass_before_, "JpsiMass_before/D");
        bsTree_->Branch("PhiMass_before", &PhiMass_before_, "PhiMass_before/D");
        bsTree_->Branch("chi2_Bs", &chi2_Bs_, "chi2_Bs/D");
        bsTree_->Branch("ndof_Bs", &ndof_Bs_, "ndof_Bs/I");
        bsTree_->Branch("BsVtxProb", &BsVtxProb_, "BsVtxProb/D");
        bsTree_->Branch("JpsiM", &JpsiM_, "JpsiM/D");
        bsTree_->Branch("JpsiPhi", &JpsiPhi_, "JpsiPhi/D");
        bsTree_->Branch("JpsiEta", &JpsiEta_, "JpsiEta/D");
        bsTree_->Branch("JpsiPt", &JpsiPt_, "JpsiPt/D");
        bsTree_->Branch("PhiM", &PhiM_, "PhiM/D");
        bsTree_->Branch("PhiPhi", &PhiPhi_, "PhiPhi/D");
        bsTree_->Branch("PhiEta", &PhiEta_, "PhiEta/D");
        bsTree_->Branch("PhiPt", &PhiPt_, "PhiPt/D");
        bsTree_->Branch("BsM", &BsM_, "BsM/D");
        bsTree_->Branch("BsPhi", &BsPhi_, "BsPhi/D");
        bsTree_->Branch("BsEta", &BsEta_, "BsEta/D");
        bsTree_->Branch("BsPt", &BsPt_, "BsPt/D");
        bsTree_->Branch("BmassC", &BmassC_, "BmassC/D");
        bsTree_->Branch("BLxy", &BLxy_, "BLxy/D");
        bsTree_->Branch("BLxy2", &BLxy2_, "BLxy2/D");
        bsTree_->Branch("BerrX", &BerrX_, "BerrX/D");
        bsTree_->Branch("BerrY", &BerrY_, "BerrY/D");
        bsTree_->Branch("BerrXY", &BerrXY_, "BerrXY/D");
        bsTree_->Branch("Bsct1", &Bsct1_, "Bsct1/D");
        bsTree_->Branch("Bsct2", &Bsct2_, "Bsct2/D");

        bsTree_->Branch("K1Phi", &K1Phi_, "K1Phi/D");
        bsTree_->Branch("K1Eta", &K1Eta_, "K1Eta/D");
        bsTree_->Branch("K1Pt", &K1Pt_, "K1Pt/D");
        bsTree_->Branch("K1Chi2", &K1Chi2_, "K1Chi2/D");
        bsTree_->Branch("K1nHits", &K1nHits_, "K1nHits/D");
        bsTree_->Branch("K1pixH", &K1pixH_, "K1pixH/D");
        bsTree_->Branch("K1trkH", &K1trkH_, "K1trkH/D");
        bsTree_->Branch("K2Phi", &K2Phi_, "K2Phi/D");
        bsTree_->Branch("K2Eta", &K2Eta_, "K2Eta/D");
        bsTree_->Branch("K2Pt", &K2Pt_, "K2Pt/D");
        bsTree_->Branch("K2Chi2", &K2Chi2_, "K2Chi2/D");
        bsTree_->Branch("K2nHits", &K2nHits_, "K2nHits/D");
        bsTree_->Branch("K2pixH", &K2pixH_, "K2pixH/D");
        bsTree_->Branch("K2trkH", &K2trkH_, "K2trkH/D");

        bsTree_->Branch("Mu1Phi", &Mu1Phi_, "Mu1Phi/D");
        bsTree_->Branch("Mu1Eta", &Mu1Eta_, "Mu1Eta/D");
        bsTree_->Branch("Mu1Pt", &Mu1Pt_, "Mu1Pt/D");
        bsTree_->Branch("Mu1Chi2", &Mu1Chi2_, "Mu1Chi2/D");
        bsTree_->Branch("Mu1nHits", &Mu1nHits_, "Mu1nHits/D");
        bsTree_->Branch("Mu1pixH", &Mu1pixH_, "Mu1pixH/D");
        bsTree_->Branch("Mu1trkH", &Mu1trkH_, "Mu1trkH/D");
        bsTree_->Branch("Mu2Phi", &Mu2Phi_, "Mu2Phi/D");
        bsTree_->Branch("Mu2Eta", &Mu2Eta_, "Mu2Eta/D");
        bsTree_->Branch("Mu2Pt", &Mu2Pt_, "Mu2Pt/D");
        bsTree_->Branch("Mu2Chi2", &Mu2Chi2_, "Mu2Chi2/D");
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

        bsTree_->Branch("BdVtxProb", &BdVtxProb_, "BdVtxProb/D");
        bsTree_->Branch("BdM", &BdM_, "BdM/D");
        bsTree_->Branch("KstarM", &KstarM_, "KstarM/D");
        bsTree_->Branch("BdPhi", &BdPhi_, "BdPhi/D");
        bsTree_->Branch("BdEta", &BdEta_, "BdEta/D");
        bsTree_->Branch("BdPt", &BdPt_, "BdPt/D");
        bsTree_->Branch("BdmassC", &BdmassC_, "BdmassC/D");
        bsTree_->Branch("JpsiM_Bd", &JpsiM_Bd_, "JpsiM_Bd/D");
        bsTree_->Branch("AngleBdDecayLength", &AngleBdDecayLength_, "AngleBdDecayLength/D");

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

        bsTree_->Branch("dedx", &dedx_, "dedx/D");
        bsTree_->Branch("errdedx", &errdedx_, "errdedx/D");
        bsTree_->Branch("numdedx", &numdedx_, "numdedx/I");


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

  flagKstar_ = -10;
  flagKs_ = -10;
  flagPhi_ = -10;

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
  MuCounter1_ = -10;
  MuCounter2_ = -10;
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
  JpsiMass_before_ = -10;
  PhiMass_before_ = -10;
  chi2_Bs_ = -10;
  ndof_Bs_ = -10;
  BsVtxProb_ = -10;
  JpsiM_ = -10;
  JpsiPhi_ = -10;
  JpsiEta_ = -10;
  JpsiPt_ = -10;
  PhiM_ = -10;
  PhiPhi_ = -10;
  PhiEta_ = -10;
  PhiPt_ = -10;
  BsM_ = -10;
  BsPhi_ = -10;
  BsEta_ = -10;
  BsPt_ = -10;
  
  K1Pt_ = -10;
  K2Pt_ = -10;
  K1Eta_ = -10;
  K2Eta_ = -10;
  K1Phi_ = -10;
  K2Phi_ = -10;
  K1Chi2_ = -10;
  K1nHits_ = -10;
  K2Chi2_ = -10;
  K2nHits_ = -10;
  K1pixH_ = -10;
  K1trkH_ = -10;
  K2pixH_ = -10;
  K2trkH_ = -10;
  
  Mu1Pt_ = -10;
  Mu2Pt_ = -10;
  Mu1Eta_ = -10;
  Mu2Eta_ = -10;
  Mu1Phi_ = -10;
  Mu2Phi_ = -10;
  Mu1Chi2_ = -10;
  Mu1nHits_ = -10;
  Mu2Chi2_ = -10;
  Mu2nHits_ = -10;
  Mu1pixH_ = -10;
  Mu1trkH_ = -10;
  Mu2pixH_ = -10;
  Mu2trkH_ = -10;

  BmassC_ = -10;
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
  
  BdVtxProb_ = -10;
  KstarM_ = -10;
  BdM_ = -10;
  BdPhi_ = -10;
  BdEta_ = -10;
  BdPt_ = -10;
  BdmassC_ = -10;
  JpsiM_Bd_ = -10;
  AngleBsDecayLength_ = -10;

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

  dedx_ = -10;
  errdedx_ = -10;
  numdedx_ = -10;
  
} 

void BsToJpsiPhiRootTree::getDeDx(const double f1, const double f2, const int f3)
{
  dedx_ = f1;
  errdedx_ = f2;
  numdedx_ = f3;
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

void BsToJpsiPhiRootTree::getBdFlags(const int bdjpsikstar, const int bdjpsiks, const int bdjpsiphi)
{
  cout << "bdjpsiphi flag while filling: " << bdjpsiphi << endl;
  cout << "bdjpsikstar flag while filling: " << bdjpsikstar << endl;
  cout << "bdjpsiks flag while filling: " << bdjpsiks << endl;
  flagPhi_ = bdjpsiphi;
  flagKstar_ = bdjpsikstar;
  flagKs_ = bdjpsiks;
}

void BsToJpsiPhiRootTree::getParams(const int nBsDau, const int momID, const int dau_1_ID, const int dau_2_ID)
{
  nBsDau_ = nBsDau; 
  momID_ = momID;
  dau_1_ID_ = dau_1_ID;
  dau_2_ID_ = dau_2_ID;

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

void BsToJpsiPhiRootTree::getPar(const double aa, const double bb, const double cc)
{
  BsMass_before_ = aa; 
  JpsiMass_before_ = bb; 
  PhiMass_before_ = cc; 
}

void BsToJpsiPhiRootTree::getMCmatch(const int aa)
{
  isMatched_ = aa; 
}

void BsToJpsiPhiRootTree::getRef(const reco::TrackRef& aa)
{
  cout <<"ref:    "<< &aa << endl; 
}

void BsToJpsiPhiRootTree::getJpsiAlone(const double aa, const int bb, const int cc, const double dd, const double ee,
				       const double ff, const double gg, const double hh, const double ii, const double ll,
				       const double mm, const double nn, const double oo, const int pp, const int qq)
{

  JpsiVtxProb_ = aa;
  MuCounter1_ = bb;
  MuCounter2_ = cc;
  JpsiM_alone_ = dd;
  JpsiPhi_alone_ = ee;
  JpsiEta_alone_ = ff;
  JpsiPt_alone_ = gg;
  JpsiMu1Pt_ = hh;
  JpsiMu2Pt_ = ii;
  JpsiMu1Phi_ = ll;
  JpsiMu2Phi_ = mm;
  JpsiMu1Eta_ = nn;
  JpsiMu2Eta_ = oo;
  JpsiMuon1Cat_ = pp;
  JpsiMuon2Cat_ = qq;
  
}

void BsToJpsiPhiRootTree::getBs(const double aa, const int bb, const double cc, const double dd, 
				const double ee, const double ff, const double gg, const double hh)
{
  chi2_Bs_ = aa;
  ndof_Bs_ = bb;
  BsVtxProb_ = cc;
  BsM_ = dd;
  BsPhi_ = ee;
  BsEta_ = ff;
  BsPt_ = gg;
  BmassC_ = hh;

}

void BsToJpsiPhiRootTree::getJpsi(const double aa, const double bb, const double cc, const double dd)
{
  JpsiM_ = aa;
  JpsiPhi_ = bb;
  JpsiEta_ = cc;
  JpsiPt_ = dd;
}

void BsToJpsiPhiRootTree::getPhi(const double aa, const double bb, const double cc, const double dd)
{
  PhiM_ = aa;
  PhiPhi_ = bb;
  PhiEta_ = cc;
  PhiPt_ = dd;
}

void BsToJpsiPhiRootTree::getK1(const double aa, const double bb, const double cc, const double dd, const int ee, const int ff, const int gg,
				const int hh, const int ii, const int ll, const int mm)
{
  K1Pt_ = aa;
  K1Eta_ = bb;
  K1Phi_ = cc;
  K1Chi2_ = dd;
  K1nHits_ = ee;
  K1pixH_ = ff;
  K1trkH_ = gg;
  K1trkLay_ = hh;
  K1muDTh_ = ii;
  K1muCSCh_ = ll;
  K1muRPCh_ = mm;

}

void BsToJpsiPhiRootTree::getK2(const double aa, const double bb, const double cc, const double dd, const int ee, const int ff, const int gg,
				const int hh, const int ii, const int ll, const int mm)
{
  K2Pt_ = aa;
  K2Eta_ = bb;
  K2Phi_ = cc;
  K2Chi2_ = dd;
  K2nHits_ = ee;
  K2pixH_ = ff;
  K2trkH_ = gg;
  K2trkLay_ = hh;
  K2muDTh_ = ii;
  K2muCSCh_ = ll;
  K2muRPCh_ = mm;
}

void BsToJpsiPhiRootTree::getMu1(const double aa, const double bb, const double cc, const double dd, const int ee, const int ff, const int gg,
				const int hh, const int ii, const int ll, const int mm)
{
  Mu1Pt_ = aa;
  Mu1Eta_ = bb;
  Mu1Phi_ = cc;
  Mu1Chi2_ = dd;
  Mu1nHits_ = ee;
  Mu1pixH_ = ff;
  Mu1trkH_ = gg;
  Mu1trkLay_ = hh;
  Mu1muDTh_ = ii;
  Mu1muCSCh_ = ll;
  Mu1muRPCh_ = mm;
}

void BsToJpsiPhiRootTree::getMu2(const double aa, const double bb, const double cc, const double dd, const int ee, const int ff, const int gg,
				const int hh, const int ii, const int ll, const int mm)
{
  Mu2Pt_ = aa;
  Mu2Eta_ = bb;
  Mu2Phi_ = cc;
  Mu2Chi2_ = dd;
  Mu2nHits_ = ee;
  Mu2pixH_ = ff;
  Mu2trkH_ = gg;
  Mu2trkLay_ = hh;
  Mu2muDTh_ = ii;
  Mu2muCSCh_ = ll;
  Mu2muRPCh_ = mm;
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

void BsToJpsiPhiRootTree::getBdPar(const double aa, const double bb, const double cc, const double dd, const double ee, const double ff, const double gg, 
				   const double hh, const double ii)
{
  BdM_ = aa;
  BdmassC_ = bb;
  KstarM_ = cc;
  BdVtxProb_ = dd;
  BdPhi_ = ee;
  BdEta_ = ff;
  BdPt_ = gg;
  JpsiM_Bd_ = hh;
  AngleBdDecayLength_ = ii;
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
