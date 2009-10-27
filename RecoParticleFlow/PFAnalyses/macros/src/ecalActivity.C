#include "RecoParticleFlow/PFAnalyses/interface/PlotUtil.h"
#include <TH1.h>
#include <TFile.h>
#include <TTree.h>
#include <string>

void ecalActivity() {

	using namespace std;
//
	TFile::Open("/tmp/ballin/Dipion_Tree_50GeV_10k_barrel_slack_full_noSrZsp_0T.root");
	string dest("plots/barrel_full_0T_noSrZsp/");


//		TFile::Open("/tmp/ballin/PFlowTB_Tree_All_endcap_tbCalib.root");
//		string dest("plots/endcap_tbCalib/");

//	TFile::Open("/tmp/ballin/Dipion_Tree_All_10k_barrel_noExcesses_full_4T.root");
//	string dest("plots/barrel_full/");

//		TFile::Open("/tmp/ballin/Dipion_Tree_All_10k_barrel_slack_fast_4T.root");
//		string dest("plots/barrel_fast/");

//		TFile::Open("/tmp/ballin/Dipion_Tree_All_10k_endcap_slack_fast_4T.root");
//		string dest("plots/endcap_fast/");
//
//		TFile::Open("/tmp/ballin/Dipion_Tree_All_10k_endcap_noExcesses_full_4T.root");
//		string dest("plots/endcap_full/");


	string todo("ecalActivity");

	TTree* tree = (TTree*) gDirectory->FindObjectAny("Extraction");
	assert(tree);
	PlotUtil util_;

//	gPad->SetLogy(true);
	tree->Draw("tb_energyEcal_>>em(530,-3,50)", "int(sim_energyEvent_)==50");
	TH1* em = util_.getType<TH1> ("em");
	assert(em);
	double maxNorm = 1.0 / em->GetBinContent(em->GetMaximumBin());
	em->Scale(maxNorm);

	util_.streamTH1ToGraphFile(string(dest).append(todo).append(".dat"), em, true);
	gPad->SaveAs(string(todo).append(".png").c_str());


}


