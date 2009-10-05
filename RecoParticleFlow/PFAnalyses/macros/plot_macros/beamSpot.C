void beamSpot() {

	gSystem->Load("libCintex.so");
	//gSystem->Load("libFWCoreFWLite.so");
	gSystem->Load("libRecoParticleFlowPFAnalyses");
	gSystem->Load("libDataFormatsParticleFlowReco");
	Cintex::Enable();


	TFile* f = TFile::Open("PFlowTB_Tree_9GeV_2k.root");
	TTree* tree = (TTree*) gDirectory->FindObjectAny("Extraction");

	//gPad->Divide(2);
	TH2F beamSpotHcal("beamSpotHcal", "beamSpotHcal;eta;phi", 100, 0, 3, 100, -1, 1);
	TH2F beamSpotEcal("beamSpotEcal", "beamSpotEcal;eta;phi", 100, 0, 3, 100, -1, 1);

	using namespace pftools;
	using namespace std;
	Calibratable* calib = new Calibratable();
	tree->SetBranchAddress("Calibratable", &calib);

	for (unsigned j(0); j < tree->GetEntriesFast(); ++j) {
			tree->GetEntry(j);
			for(unsigned k(0); k < calib->tb_numEcal_; ++k) {
				beamSpotEcal.Fill(calib->tb_ecal_[k].eta_,calib->tb_ecal_[k].phi_, calib->tb_ecal_[k].energy_);
			}
			for(unsigned k(0); k < calib->tb_numHcal_; ++k) {
				beamSpotHcal.Fill(calib->tb_hcal_[k].eta_,calib->tb_hcal_[k].phi_, calib->tb_hcal_[k].energy_);
			}
	}
	//gPad->cd(1);
	beamSpotEcal.Draw("colz");
	//gPad->cd(2);
	//beamSpotHcal.Draw("colz");

}

