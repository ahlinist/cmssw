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
#include <map>

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

void studyTypes(TTree* tree_, PlotUtil& util_, std::string dest, const std::vector<int>& energies) {

	string todo("candTypes");
	
	Calibratable* calib = new Calibratable();
	tree_->SetBranchAddress("Calibratable", &calib);
	
	unsigned entries = tree_->GetEntries();
	//unsigned entries = 1000;
	cout << "Tree has " << entries << " entries" << endl;
	
	map<int, int> oneChargedHadron;
	map<int, int> extraPhoton;
	map<int, int> extraNeutron;
	map<int, int> twoExtras;
	
	map<int, double> extraEnergyContribution;
	map<int, int> extraNormalization;
	
	map<int, double> chargedHadCaloEnergyContribution;
	
	for(unsigned entry(0); entry < entries; ++entry) {
		if (entry % 10000 == 0)
			cout << "Processing entry " << entry << "\n";
		
		tree_->GetEntry(entry);
		
		const vector<CandidateWrapper>& cws = calib->cands_;
		unsigned nPhotons(0);
		unsigned nNeutrons(0);
		double ePhotons(0);
		double eNeutrons(0);
		
		int energy = static_cast<int>(calib->sim_energyEvent_);
		
		vector<CandidateWrapper>::const_iterator candit = cws.begin();
		for(; candit != cws.end(); ++candit) {
			const CandidateWrapper& wrap = *candit;
			switch(wrap.type_) {
				case 4:
					++nPhotons;
					ePhotons += wrap.energy_;
					break;
				case 5:
					++nNeutrons;
					eNeutrons += wrap.energy_;
					break;
				case 1:
					chargedHadCaloEnergyContribution[energy] += wrap.energyEcal_ + wrap.energyHcal_;
					break;
			}
		}
		if(nNeutrons == 0 && nPhotons == 0) {
			oneChargedHadron[energy] += 1; 
		} else if(nNeutrons == 0 && nPhotons >0) {
			extraPhoton[energy] += 1;
		} else if(nNeutrons !=0 && nPhotons == 0) {
			extraNeutron[energy] += 1;
		} else {
			twoExtras[energy] += 1;
		}
		
		if(nNeutrons != 0 || nPhotons != 0) {
			extraEnergyContribution[energy] += ePhotons + eNeutrons;
			extraNormalization[energy] += 1;
		}
		
	}
	
	map<int, int> nEvents;
	map<int, int>::iterator i = oneChargedHadron.begin();
	cout << "** Event candidate type fractions:\n";
	cout << "Energy\tN\tT1\tT4\tT5\tTN\n";
	for(; i != oneChargedHadron.end(); ++i) {
		int e = i->first;
		nEvents[e] = oneChargedHadron[e] + extraNormalization[e];
		cout << setw(3) << e << "\t"
			<< nEvents[e] << "\t" << setprecision(4)
			<< static_cast<double>(oneChargedHadron[e])/ nEvents[e] << "\t"
			<< static_cast<double>(extraPhoton[e])/nEvents[e] << "\t"
			<< static_cast<double>(extraNeutron[e])/nEvents[e] << "\t"
			<< static_cast<double>(twoExtras[e])/nEvents[e] << "\n";
	}
	
	cout << "\n** Extra candidate energy contamination:\n";
	cout << "Energy\tEContrib/E\n";
	i = extraNormalization.begin();
	for(; i != extraNormalization.end(); ++i) {
		int e = i->first;
		cout << e << "\t" << (extraEnergyContribution[e] / extraNormalization[e])/e << "\n";
	}
	
	cout << "\n** Extra candidate energy transportation, as fraction of charged hadron energy:\n";
	cout << "Energy\tEContrib/E\n";
	i = extraNormalization.begin();
	for(; i != extraNormalization.end(); ++i) {
		int e = i->first;
		cout << e << "\t" << (extraEnergyContribution[e] / chargedHadCaloEnergyContribution[e]) << "\n";
	}


	cout << "Done" << endl;
	

}

	
void candType() {

	cout << "candType.C -- Script to evaluate PFlow candidate types." << endl;

#include "src/selector.cc"

	// --- standard stuff
	studyTypes(chain, util_, directory, energies);

}

