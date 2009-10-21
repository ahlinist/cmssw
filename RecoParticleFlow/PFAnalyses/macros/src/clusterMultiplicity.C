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
	
bool endcap(true);
int data(1);
int full(0);
int fast(0);

bool operator<(const CalibratableElement& ce1, const CalibratableElement& ce2) {
	if(ce1.energy_ < ce2.energy_)
		return true;
	return false;
}

void studyClusters(TTree* tree_, PlotUtil& util_, std::string dest, const std::vector<int>& energies) {

	string todo("clusterActivity");
	
	Calibratable* calib = new Calibratable();
	tree_->SetBranchAddress("Calibratable", &calib);
	
	unsigned entries = tree_->GetEntries();
	cout << "Tree has " << entries << " entries" << endl;
	
	TH2F* otherFrac = new TH2F("otherFrac", "otherFrac", 300, 1, 301, 101, 0, 101);
	TH2F* numClust = new TH2F("numClust", "numClust", 300, 1, 301, 5, 0, 5);
	TH2F* deltaR = new TH2F("deltaR", "deltaR", 300, 1, 301, 16, 0, 0.16);
	
	for(unsigned entry(0); entry < entries; ++entry) {
		
		tree_->GetEntry(entry);
		
		numClust->Fill(calib->sim_energyEvent_, calib->cluster_hcal_.size());
		
		if(calib->cluster_hcal_.size() < 2)
			continue;
		
		vector<CalibratableElement> hcalClusters = calib->cluster_hcal_;
		
		sort(hcalClusters.begin(), hcalClusters.end());
		//reverse(hcalClusters.begin(), hcalClusters.end());
		
		double leadingEnergy = hcalClusters.begin()->energy_;
		double smallestEnergy = hcalClusters.rbegin()->energy_;
		vector<CalibratableElement>::const_iterator cit = hcalClusters.begin();
		cit++;
		double dr;
		if(endcap)
			dr = sqrt(pow(cit->eta_ - calib->tb_eta_, 2) + pow(cit->phi_ -calib->tb_phi_, 2));
		else {
			//fix this
			dr = sqrt(pow(cit->eta_ - calib->tb_eta_, 2) + pow(cit->phi_ -calib->tb_phi_, 2));
		}

		deltaR->Fill(calib->sim_energyEvent_, dr);
		
		double remainder = (calib->cluster_energyHcal_ - leadingEnergy)/calib->cluster_energyHcal_;
		otherFrac->Fill(calib->sim_energyEvent_, remainder * 100.0);
		
//		cout << "Total = " << calib->cluster_energyHcal_ << ",\tleading one has " << leadingEnergy << ",\t fraction carried by others = " << remainder / calib->cluster_energyHcal_ * 100.0 << "%\n";
		
	}
	numClust->ProfileX();
	otherFrac->ProfileX();
	deltaR->ProfileX();
	TProfile* pfx = util_.getType<TProfile>("otherFrac_pfx");
	TProfile* numClust_pfx = util_.getType<TProfile>("numClust_pfx");
	TProfile* deltaR_pfx = util_.getType<TProfile>("deltaR_pfx");
	
	std::vector<int>::const_iterator eit = energies.begin();
	JGraph temp("temp", true);
	JGraph num("num", true);
	JGraph drtemp("dr", true);
	for(; eit != energies.end(); ++eit) {
		temp.addPoint(*eit, 0,  pfx->GetBinContent(*eit), pfx->GetBinError(*eit));
		num.addPoint(*eit, 0,  numClust_pfx->GetBinContent(*eit), numClust_pfx->GetBinError(*eit));
		drtemp.addPoint(*eit, 0,  deltaR_pfx->GetBinContent(*eit), deltaR_pfx->GetBinError(*eit));
	}
	
	cout << "** Cluster energy disassociation:\n";
	cout << temp;
	cout << "** Writing to " << string(dest).append("/clusterEnergyDissasociation.dat") << "\n";
	temp.streamToFile(string(dest).append("/clusterEnergyDissasociation.dat"), true);
	
	cout << "** Number of clusters:\n";
	cout << num;
	
	cout << "** Delta R to second cluster:\n";
	cout << drtemp;
	//cout << "** Writing to " << string(dest).append("/clusterEnergyDissasociation.dat" << "\n";
	//temp.streamToFile(string(dest).append("/clusterEnergyDissasociation.dat"), true);
	
	pfx->Draw();
	gPad->SetLogx(true);
	gPad->Update();
	cout << "Done" << endl;
	
	gPad->SaveAs("clusterMultiplicity.png");

}

	
void clusterMultiplicity() {

	cout << "clusterMultiplicity.C -- Script to evaluate PFlow cluster response." << endl;


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
	studyClusters(chain, util_, directory, energies);

}

