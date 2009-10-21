#include "RecoParticleFlow/PFAnalyses/interface/PlotUtil.h"
#include "DataFormats/ParticleFlowReco/interface/Calibratable.h"
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <TTree.h>
#include <TProfile.h>
#include <string>
#include <vector>
#include <iostream>
#include <TChain.h>
#include <algorithm>

using namespace std;
using namespace pftools;
	
bool endcap(false);
int data(1);
int full(0);
int fast(0);

bool operator<(const CalibratableElement& ce1, const CalibratableElement& ce2) {
	if(ce1.energy_ < ce2.energy_)
		return true;
	return false;
}

void studyTypes(TTree* tree_, PlotUtil& util_, std::string dest, const std::vector<int>& energies) {

	string todo("candTypes");
	
	Calibratable* calib = new Calibratable();
	tree_->SetBranchAddress("Calibratable", &calib);
	
	unsigned entries = tree_->GetEntries();
	cout << "Tree has " << entries << " entries" << endl;
	
	
	for(unsigned entry(0); entry < entries; ++entry) {
		
		tree_->GetEntry(entry);
		
		const vector<CandidateWrapper>& cws = calib->cands_;
		

	}


	cout << "Done" << endl;
	

}

	
void candType() {

	cout << "candType.C -- Script to evaluate PFlow candidate types." << endl;


	vector<int> energies;
	energies.push_back(2);
	energies.push_back(3);
	energies.push_back(4);
	energies.push_back(5);
	energies.push_back(6);
	energies.push_back(7);
	energies.push_back(8);
	energies.push_back(9);
	energies.push_back(20);
	energies.push_back(30);
	energies.push_back(50);
	energies.push_back(100);
	energies.push_back(150);
	energies.push_back(200);
//	energies.push_back(225);
	energies.push_back(300);

	std::vector<int> endcap_energies;
	endcap_energies.push_back(4);
	endcap_energies.push_back(5);
	endcap_energies.push_back(6);
	endcap_energies.push_back(7);
	endcap_energies.push_back(8);
	endcap_energies.push_back(9);
	endcap_energies.push_back(20);
	endcap_energies.push_back(30);
	endcap_energies.push_back(50);
	endcap_energies.push_back(100);
	endcap_energies.push_back(150);
	endcap_energies.push_back(225);
	endcap_energies.push_back(300);


	assert(data+full+fast == 1);

	//-- selections
	TChain* chain = new TChain("extraction/Extraction");
	string directory;
	if(endcap) {
		energies = endcap_energies;
		if(data) {
			chain->Add("/tmp/ballin/PFlowTB_Tree_All_endcap_tbCalib.root");
			directory = "plots/endcap_tbCalib";
		}
		else if(full) {
			chain = new TChain("extractionToTree/Extraction");
			chain->Add("/tmp/ballin/Dipion_Tree_All_10k_endcap_noExcesses_full_4T.root");
			directory = "plots/endcap_full";
		}
		else if(fast) {
			chain = new TChain("extractionToTree/Extraction");
			chain->Add("/tmp/ballin/Dipion_Tree_All_10k_endcap_slack_fast_4T.root");
			directory = "plots/endcap_fast";
		}
	} else {
		if(data) {
			chain->Add("/tmp/ballin/PFlowTB_Tree_All_barrel_tbCalib.root");
			directory = "plots/barrel_tbCalib";
		}
		else if(full) {
			chain = new TChain("extractionToTree/Extraction");
			chain->Add("/tmp/ballin/Dipion_Tree_All_10k_barrel_noExcesses_full_4T.root");
			directory = "plots/barrel_full";
		}
		else if(fast) {
			chain = new TChain("extractionToTree/Extraction");
			chain->Add("/tmp/ballin/Dipion_Tree_All_10k_barrel_slack_fast_4T.root");
			directory = "plots/barrel_fast";
		}
	}
	
	PlotUtil util_;
	util_.init();

	// --- standard stuff
	studyTypes(chain, util_, directory, energies);

}

