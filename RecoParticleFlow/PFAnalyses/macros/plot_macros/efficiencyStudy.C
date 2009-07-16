#include <vector>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include "TF1.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "Math/SMatrix.h"
#include "Math/SVector.h"
#include "TCanvas.h"
#include <TMultiGraph.h>
#include <TPie.h>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <string>
#include <utility>
#include <map>

#include <ostream>
#include <sstream>

//#include <cmath>

#include "RecoParticleFlow/PFAnalyses/interface/PlotUtil.h"
#include "RecoParticleFlow/PFAnalyses/interface/PlotUtil.h"
#include "RecoParticleFlow/PFAnalyses/interface/JGraph.h"
#include "DataFormats/ParticleFlowReco/interface/CaloWindow.h"
#include "DataFormats/ParticleFlowReco/interface/Calibratable.h"
#include "DataFormats/Math/interface/deltaR.h"

using namespace std;
using namespace pftools;

template<typename T> std::string obj2str(T n) {
	std::ostringstream oss;
	oss << n;
	std::string s(oss.str());
	return s;
}

template<typename K, typename R> std::ostream& operator<<(std::ostream& s,
		const std::pair<K, R>& p) {
	s << "(" << p.first << ", " << p.second << ")";
	return s;
}

/**
 * In my "macros", I go for the 'Java' style: write implementation for a class with
 * with its definition.
 *
 */
class EfficiencyStudy {
public:

	EfficiencyStudy() {

	}

	virtual ~EfficiencyStudy() {

	}

	EfficiencyStudy(map<string, TTree*> trees, std::vector<int> energies,
			std::string graphicsFile, std::string macroFile,
			std::string directory) :
		directory_(directory) {

		util_.init();
		util_.enableAutoFlush(true, 9);
		util_.setSquareCanvasDimension(3);
		reset(trees, energies, graphicsFile, macroFile);

	}

	void reset(map<string, TTree*> trees, std::vector<int> energies,
			std::string graphicsFile, std::string macroFile) {

		trees_ = trees;
		energies_ = energies;
		graphicsFile_ = graphicsFile;
		macroFile_ = macroFile;
		defaultStyle_ = (TStyle*) gStyle->Clone();

		TCanvas* c = new TCanvas("Global", "Global canvas");
		std::string namecpy(directory_);
		namecpy.append("/");
		namecpy.append(graphicsFile);
		graphicsFile_ = namecpy;
		util_.setGraphicsFile(namecpy);
		namecpy.append("[");
		c->Print(namecpy.c_str());

		std::string macrocpy(directory_);
		macrocpy.append("/");
		macrocpy.append(macroFile);
		macroFile_ = macrocpy;
		util_.setMacroFile(macrocpy);
		macrocpy.append("[");
		c->Print(macrocpy.c_str());
	}

	void drawEfficiencyPlots() {

		TStyle* rStyle = util_.makeStyle("normalStyle");
		rStyle->SetOptLogy(false);
		rStyle->SetOptStat(1);
		rStyle->SetOptFit(1);
		rStyle->SetOptLogz(false);
		rStyle->SetOptLogx(true);
		//Supresses TGraph errors in X
		rStyle->SetErrorX(0);
		rStyle->cd();

		util_.newPage();

		map<string, TTree*>::iterator it = trees_.begin();
		unsigned count(0);
		for (; it != trees_.end(); ++it) {
			pair<const string, TTree*> item = *it;

			std::string name("simE");
			name.append("_");
			name.append(obj2str(count));
			std::string qry("sim_energyEvent_>>");
			qry.append(name);
			qry.append("(300, 1,300)");
			item.second->Draw(qry.c_str());

			TH1* plot = util_.formatHisto(name, item.first,
					"Beam momentum (GeV/c)", util_.nextColor(),
					util_.nextColor(), 2);
			util_.accumulateObjects(plot, "");
			++count;
		}

		util_.addTitle("TB Analysis - statistics available");

		util_.flushPage();

	}

	void drawClusterEnergyPlots();

	void drawEnergyDepositionPlots();

	void closeFiles() {
		std::string namecpy2(graphicsFile_);
		namecpy2.append("]");
		gPad->Print(namecpy2.c_str());

		std::string macrocpy2(macroFile_);
		macrocpy2.append("]");
		gPad->Print(macrocpy2.c_str());

		util_.flushSpecials(directory_);
	}

private:
	std::map<std::string, TTree*> trees_;
	std::vector<int> energies_;
	TStyle* defaultStyle_;
	PlotUtil util_;

	std::string graphicsFile_;
	std::string macroFile_;

	std::string directory_;

};

void EfficiencyStudy::drawClusterEnergyPlots() {
	TStyle* rStyle = util_.makeStyle("cStyle");
	rStyle->SetOptLogy(false);
	rStyle->SetOptStat(1);
	rStyle->SetOptFit(1);
	rStyle->SetOptLogz(false);
	rStyle->SetOptLogx(false);
	//Supresses TGraph errors in X
	rStyle->SetErrorX(0);
	rStyle->cd();

	util_.newPage();
	util_.addTitle("Cluster energy profiles");

	map<string, TTree*>::iterator it = trees_.begin();
	unsigned count(0);
	for (; it != trees_.end(); ++it) {
		pair<const string, TTree*> item = *it;
		TTree* tree_ = item.second;

		Calibratable* calib = new Calibratable();
		tree_->SetBranchAddress("Calibratable", &calib);

		TH1F* eClosestCluster = new TH1F("eClosestCluster",
				"E of closest cluster", 100, 0, 100);
		TH1F* eOtherClusters = new TH1F("eOtherClusters",
				"Sum of energy on other clusters", 100, 0, 100);

		TH1F* eMostEnergeticCluster = new TH1F("eMostEnergeticCluster",
				"E of most energetic cluster", 100, 0, 100);

		TH1F
				* fAgreement =
						new TH1F(
								"fAgreement",
								"Frequency closest is most energetic too;Closest == Most energetic?",
								2, 0, 2);

		TH1F* dRHitsAroundType1 = new TH1F("drHitsAroundType1",
				"Delta R rechits around type 1 impact", 30, 0, 0.3);

		TH1F* dRHitsAroundType1EWeighted =
				new TH1F("drHitsAroundType1EWeighted",
						"Delta R rechits around type 1 impact (E weighted)",
						30, 0, 0.3);

		TH1F* nClusters = new TH1F("nClusters", "Number of HCAL clusters", 10,
				0, 10);

		TH2F* pOtherClusters = new TH2F("pOtherClusters",
				"Position of other clusters;#eta;#phi", 60, -0.3, 0.3, 60,
				-0.3, 0.3);

		for (unsigned j(0); j < tree_->GetEntries(); ++j) {

			tree_->GetEntry(j);

			nClusters->Fill(calib->cluster_numHcal_);

			//Plot energy of cluster closest to track and next clusters etc, etc.
			const vector<CalibratableElement>& hClusters = calib->cluster_hcal_;
			const vector<CandidateWrapper>& cands = calib->cands_;
			double eta, phi;
			bool ok(false);
			vector<CandidateWrapper>::const_iterator l = cands.begin();
			for (; l != cands.end(); ++l) {
				if (l->type_ == 1) {
					eta = l->eta_;
					phi = l->phi_;
					ok = true;
					break;
				}
			}
			if (!ok)
				continue;

			const vector<CalibratableElement>& hRechits = calib->rechits_hcal_;
			vector<CalibratableElement>::const_iterator hit = hRechits.begin();
			for (; hit != hRechits.end(); ++hit) {
				double dR = reco::deltaR(eta, phi, hit->eta_, hit->phi_);
				dRHitsAroundType1EWeighted->Fill(dR, hit->energy_);
				dRHitsAroundType1->Fill(dR);
			}

			double minDeltaR(9999);
			double maxE(0);
			vector<CalibratableElement>::const_iterator c = hClusters.begin();
			vector<CalibratableElement>::const_iterator winnerClosest =
					hClusters.end();
			vector<CalibratableElement>::const_iterator winnerEnergy =
					hClusters.end();
			cout << "Track at " << eta << ", " << phi << "\n";
			for (; c != hClusters.end(); ++c) {
				double testDR = reco::deltaR(eta, phi, c->eta_, c->phi_);
				if (testDR < minDeltaR) {
					minDeltaR = testDR;
					winnerClosest = c;
				}
				if (c->energy_ > maxE) {
					maxE = c->energy_;
					winnerEnergy = c;
				}
				cout << "\t" << *c << ", dR = " << testDR << "\n";
			}

			//loop again, sum energy in other non-winning clusters
			c = hClusters.begin();
			double energyOtherClusters(0.0);
			for (; c != hClusters.end(); ++c) {
				if (c == winnerClosest)
					continue;

				energyOtherClusters += c->energy_;
				pOtherClusters->Fill(c->eta_ - eta, c->phi_ - phi);
			}
			if (winnerClosest == hClusters.end() || winnerEnergy
					== hClusters.end())
				continue;

			eClosestCluster->Fill(winnerClosest->energy_);
			eMostEnergeticCluster->Fill(winnerEnergy->energy_);
			eOtherClusters->Fill(energyOtherClusters);

			if (winnerClosest == winnerEnergy)
				fAgreement->Fill(1);
			else
				fAgreement->Fill(0);

		}
		util_.accumulateObjects(eClosestCluster);
		util_.accumulateObjects(eOtherClusters);
		util_.accumulateObjects(eMostEnergeticCluster);
		util_.accumulateObjects(fAgreement);
		util_.accumulateObjects(dRHitsAroundType1);
		util_.accumulateObjects(dRHitsAroundType1EWeighted);
		util_.accumulateObjects(nClusters);
		util_.accumulateObjects(pOtherClusters, "colz");
	}

	util_.flushPage();
}

void EfficiencyStudy::drawEnergyDepositionPlots() {
	TStyle* rStyle = util_.makeStyle("eStyle");
	rStyle->SetOptLogy(false);
	rStyle->SetOptStat(1);
	rStyle->SetOptFit(1);
	rStyle->SetOptLogz(false);
	rStyle->SetOptLogx(false);
	//Supresses TGraph errors in X
	rStyle->SetErrorX(0);
	rStyle->cd();

	util_.newPage();
	util_.addTitle("Transverse shower profiles");

	map<string, TTree*>::iterator it = trees_.begin();
	unsigned count(0);
	for (; it != trees_.end(); ++it) {
		pair<const string, TTree*> item = *it;
		TTree* tree_ = item.second;

		Calibratable* calib = new Calibratable();
		tree_->SetBranchAddress("Calibratable", &calib);

		//loop over calibratables
		cout << "Tree has " << tree_->GetEntries() << " entries" << endl;
		TH1F* deltaREcalCluster = new TH1F("deltaREcalCluster",
				"deltaR Ecal - Cluster hits", 10, 0, 10);
		TH1F* deltaRHcalCluster = new TH1F("deltaRHcalCluster",
				"deltaR Hcal - Cluster hits", 10, 0, 10);
		TH1F* deltaREcalCalo = new TH1F("deltaREcalCalo",
				"deltaREcal - Calo rechits", 10, 0, 10);
		TH1F* deltaRHcalCalo = new TH1F("deltaRHcalCalo",
				"deltaRHcal - Calo rechits", 20, 0, 20);
		TH1F* deltaRHcalManual = new TH1F("deltaRHcalManual",
				"deltaRHcal - Manual rechits", 20, 0, 0.5);

		TH2F* hcalDepositions = new TH2F("hcalDepositions", "hcal depositions",
				40, -0.5, 0.5, 40, -0.5, 0.5);
		for (unsigned j(0); j < tree_->GetEntries(); ++j) {

			tree_->GetEntry(j);

			//			cout << *calib << "\n";

			const CaloWindow& cwh = calib->calowindow_hcal_;
			CaloWindow tbh;
			tbh.init(calib->tb_meanHcal_.eta_, calib->tb_meanHcal_.phi_, 20,
					0.05, 1);
			calib->fillCaloWindow(calib->tb_hcal_, tbh);

			map<unsigned, double> energies = cwh.getRingEnergySummations();
			map<unsigned, double>::const_iterator u = energies.begin();
			for (; u != energies.end(); ++u)
				deltaRHcalCluster->Fill(u->first, u->second);

			const CaloWindow& cwe = calib->calowindow_ecal_;
			energies = cwe.getRingEnergySummations();
			u = energies.begin();
			for (; u != energies.end(); ++u) {
				deltaREcalCluster->Fill(u->first, u->second);
			}

			energies = tbh.getRingEnergySummations();
			u = energies.begin();
			for (; u != energies.end(); ++u)
				deltaRHcalCalo->Fill(u->first, u->second);

			const vector<CalibratableElement>& hits = calib->tb_hcal_;
			vector<CalibratableElement>::const_iterator hit = hits.begin();
			for (; hit != hits.end(); ++hit) {
				const CalibratableElement& ce = *hit;
				double deltaR = reco::deltaR(ce.eta_, ce.phi_,
						calib->tb_meanHcal_.eta_, calib->tb_meanHcal_.phi_);
				deltaRHcalManual->Fill(deltaR, ce.energy_);
				hcalDepositions->Fill(ce.eta_ - calib->tb_eta_, ce.phi_
						- calib->tb_phi_, ce.energy_ > 0 ? 1 : 0);
			}

		}
		util_.accumulateObjects(deltaREcalCluster);
		util_.accumulateObjects(deltaRHcalCluster);
		util_.accumulateObjects(deltaRHcalCalo);
		util_.accumulateObjects(deltaRHcalManual);

		util_.accumulateObjects(hcalDepositions, "colz");

		++count;
	}

	util_.flushPage();
}

void efficiencyStudy() {
	gROOT->Reset();

	TFile
			* study =
					TFile::Open(
							"/afs/cern.ch/user/b/ballin/scratch0/cmssw/src/RecoParticleFlow/PFAnalyses/test/outputtree_50GeV_2k.root");
	TTree* studyTree = (TTree*) study->FindObjectAny("Extraction");

	std::map<std::string, TTree*> source;

	std::cout << "Study tree is " << studyTree << endl;

	source["Study"] = studyTree;

	std::vector<int> energies;
	//	energies.push_back(2);
	//	energies.push_back(3);
	//	energies.push_back(4);
	//	energies.push_back(5);
	//	energies.push_back(6);
	//	energies.push_back(7);
	//	energies.push_back(8);
	//	energies.push_back(9);
	//	energies.push_back(20);
	//	energies.push_back(30);
	energies.push_back(50);
	//	energies.push_back(100);
	//	energies.push_back(200);
	//	energies.push_back(300);

	EfficiencyStudy es(source, energies, "es.ps", "es.C", "es");
	es.drawEfficiencyPlots();
	es.drawEnergyDepositionPlots();
	es.drawClusterEnergyPlots();

	es.closeFiles();

}
