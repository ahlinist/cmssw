#include "RecoParticleFlow/PFAnalyses/interface/PlotUtil.h"
#include <TH1.h>
#include <TFile.h>
#include <TTree.h>
#include <string>

void candSpectrum() {

	using namespace std;

	TFile::Open("../test/PFlowTB_Tree_100GeV.root");
//	TFile::Open("../test/Dipion_Tree_10GeV_5k_1SigmaExcesses_Eqn5Fix.root");

	string todo("_noExcesses_");

	TTree* tree = (TTree*) gDirectory->FindObjectAny("Extraction");

	PlotUtil util_;

	gPad->SetLogy(true);
	tree->Draw("cand_energyNeutralEM_>>em(100,0,10)", "cand_energyNeutralEM_>0");
	TH1* em = util_.getType<TH1> ("em");

	util_.streamTH1ToGraphFile(string("em").append(todo).append(".dat"), em, true);
	gPad->SaveAs(string("em").append(todo).append(".png").c_str());

	tree->Draw("cand_energyNeutralHad_>>had(100,0,10)" "cand_energyNeutralHad_>0");
	TH1* had = util_.getType<TH1> ("had");

	util_.streamTH1ToGraphFile(string("had").append(todo).append(".dat"), had, true);
	gPad->SaveAs(string("had").append(todo).append(".png").c_str());

	tree->Draw("cand_energyNeutralHad_+cand_energyNeutralEM_>>all(100,0,10)", "cand_energyNeutralEM_+cand_energyNeutralHad_>0");
	TH1* all = util_.getType<TH1> ("all");

	util_.streamTH1ToGraphFile(string("all").append(todo).append(".dat"), all, true);
	gPad->SaveAs(string("all").append(todo).append(".png").c_str());


}
