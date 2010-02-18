#include "RecoParticleFlow/PFAnalyses/interface/PlotUtil.h"
#include <TH1.h>
#include <TFile.h>
#include <TTree.h>
#include <string>

void noisePlot() {

	using namespace std;
	string barrel("hcalNoiseBarrel");
	string endcap("hcalNoiseEndcap");

//	string todo = barrel;
//	TFile::Open("../test/PFlowTB_Tree_300GeV_barrel_noise.root");
	string todo = endcap;
	TFile::Open("../test/PFlowTB_Tree_300GeV_endcap_noise.root");

	TTree* tree = (TTree*) gDirectory->FindObjectAny("Extraction");

	PlotUtil util_;
	cout << "USING ENDCAP PI/E = 0.85!!" << endl;
	tree->Draw("tb_hcal_.energy_/0.85>>noise(20,-1,1)", "");
	TH1* noise = util_.getType<TH1> ("noise");
	double maxNorm = 1.0 / noise->GetBinContent(noise->GetMaximumBin());
	noise->Scale(maxNorm);
	util_.streamTH1ToGraphFile(string(todo).append(".dat"), noise, true, true);
	gPad->SaveAs(string(todo).append(".png").c_str());

}
