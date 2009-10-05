//#include "RecoParticleFlow/PFAnalyses/interface/operations.h"
#include "RecoParticleFlow/PFAnalyses/interface/JGraph.h"

#include <iostream>
#include <TROOT.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TPaveStats.h>
#include <TLegend.h>
#include <TColor.h>
#include <TList.h>
#include <TPaveText.h>
#include <cmath>
#include <ostream>
#include <sstream>
#include <utility>
#include <map>
#include <TF1.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TText.h>
#include <iostream>

#include <vector>
#include <TTree.h>
#include <TObject.h>
#include <string>
#include <TColor.h>
#include <TH1.h>
#include <TStyle.h>
#include <utility>
#include <THStack.h>
#include <TLegend.h>
#include <TChain.h>

#include "RecoParticleFlow/PFAnalyses/interface/PlotUtil.h"

class CommonProcessing {
public:

	CommonProcessing() {

	}

	CommonProcessing(TTree* t, std::string graphicsFile, std::string macroFile,
			std::string directory_);

	virtual ~CommonProcessing();

	void reset(TTree* t, std::string graphicsFile, std::string macroFile);

	void doResponsePlots(const std::vector<int>& energies);

	void evaluatePlots(const std::vector<int>& energies, bool eff, bool resp);

	void doEfficiencyPlots(const std::vector<int>& energies);

	void doMipInEcalPlots(const std::vector<int>& energies);

	void closeFiles();

private:

	TTree* tree_;
	TStyle* defaultStyle_;
	PlotUtil util_;

	std::string graphicsFile_;
	std::string macroFile_;

	std::string directory_;

};

template<typename T> std::string obj2str(T n) {
	std::ostringstream oss;
	oss << n;
	std::string s(oss.str());
	return s;
}

CommonProcessing::CommonProcessing(TTree* t, std::string graphicsFile,
		std::string macroFile, std::string directory) :
	directory_(directory) {
	util_.init();
	util_.enableAutoFlush(true, 9);
	util_.setSquareCanvasDimension(3);
	reset(t, graphicsFile, macroFile);

}

CommonProcessing::~CommonProcessing() {

}

void CommonProcessing::reset(TTree* t, std::string graphicsFile,
		std::string macroFile) {
	tree_ = t;
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

void CommonProcessing::doMipInEcalPlots(const std::vector<int>& energies) {
	TStyle* rStyle = util_.makeStyle("mipStyle");
	rStyle->SetOptLogy(false);
	rStyle->SetOptStat(0);
	rStyle->SetOptFit(0);
	rStyle->SetOptLogz(false);
	rStyle->SetOptLogx(false);
	rStyle->SetOptTitle(1);
	rStyle->SetFrameFillColor(kWhite);
	//Supresses TGraph errors in X
	rStyle->SetErrorX(0);

	rStyle->SetLabelSize(0.08, "xyz");
	rStyle->SetHistLineWidth(2);
	rStyle->SetTitleFontSize(0.1);
	rStyle->SetTitleX(0.2);
	//rStyle->SetStatFontSize(0.1);
	//rStyle->SetStatH(0.15);
	//rStyle->SetStatW(0.3);
	rStyle->SetPadGridX(true);
	rStyle->SetPadGridY(false);
	rStyle->SetNdivisions(5, "xyz");

	rStyle->cd();

	util_.newPage();
	util_.addTitle("MIP in ECAL");
	util_.flushPage();

	util_.newPage();
	util_.enableAutoFlush(true, 16);
	util_.setSquareCanvasDimension(4);

	JGraph tbGraph("tbGraph");
	JGraph clusterGraph("clusterGraph");
	JGraph candGraph("candGraph");

	JGraph tbGraphSample("tbGraphSample");
	JGraph clusterGraphSample("clusterGraphSample");
	JGraph candGraphSample("candGraphSample");

	util_.addTitle("Raw Rechits");

	unsigned seen(1);
	for (std::vector<int>::const_iterator i = energies.begin(); i
			!= energies.end(); ++i) {

		int energy = *i;
		//build query
		//		std::string
		//				cut(
		//						"tb_energyEcal_ < 0.5 && cand_type_==1 && int(sim_energyEvent_) == ");
		std::string cut("tb_energyEcal_<0.5 && int(sim_energyEvent_) == ");
		cut.append(obj2str(energy));

		std::string histoName("tbNeutralENoEcal");
		histoName.append(obj2str(energy));

		std::string qry("tb_energyEvent_/sim_energyEvent_>>");
		qry.append(histoName);
		qry.append("(170, -1.2, 2.2)");

		std::string poshHistoName(obj2str(energy));
		poshHistoName.append(" GeV");

		tree_->Draw(qry.c_str(), cut.c_str());
		TH1* tbNeutralENoEcal = util_.formatHisto(histoName, poshHistoName,
				"E_{rechits}/E_{beam}", util_.nextColor(), kWhite, 2);
		std::pair<double, double> gaus = util_.fitStabilisedGaussian(
				tbNeutralENoEcal);
		tbGraphSample.addPoint(energy, tbNeutralENoEcal->GetMean());
		double gaussianess = fabs(gaus.first / tbNeutralENoEcal->GetMean());
		if (gaussianess < 2 && gaussianess > 0.5) {
			tbGraph.addPoint(energy, gaus.first);
		}
		util_.accumulateObjects(tbNeutralENoEcal);

	}

	util_.flushPage();

	util_.newPage();

	util_.addTitle("PFClusters");

	seen = 1;
	for (std::vector<int>::const_iterator i = energies.begin(); i
			!= energies.end(); ++i) {

		int energy = *i;
		//build query
		//		std::string
		//				cut(
		//						"tb_energyEcal_ < 0.5 && cand_type_==1 && int(sim_energyEvent_) == ");
		std::string cut("cluster_numEcal_==0 && int(sim_energyEvent_) == ");
		cut.append(obj2str(energy));

		std::string histoName("clustersNeutralENoEcal");
		histoName.append(obj2str(energy));

		std::string qry("cluster_energyEvent_/sim_energyEvent_>>");
		qry.append(histoName);
		qry.append("(170, -1.2, 2.2)");

		std::string poshHistoName(obj2str(energy));
		poshHistoName.append(" GeV");

		tree_->Draw(qry.c_str(), cut.c_str());
		TH1* tbNeutralENoEcal = util_.formatHisto(histoName, poshHistoName,
				"E_{clusters}/E_{true}", util_.nextColor(), kWhite, 2);
		std::pair<double, double> gaus = util_.fitStabilisedGaussian(
				tbNeutralENoEcal);
		clusterGraphSample.addPoint(energy, tbNeutralENoEcal->GetMean());
		double gaussianess = fabs(gaus.first / tbNeutralENoEcal->GetMean());
		if (gaussianess < 2 && gaussianess > 0.5) {
			clusterGraph.addPoint(energy, gaus.first);
		}
		util_.accumulateObjects(tbNeutralENoEcal);

	}

	util_.flushPage();
	util_.newPage();

	util_.addTitle("PFCandidates");

	seen = 1;
	for (std::vector<int>::const_iterator i = energies.begin(); i
			!= energies.end(); ++i) {

		int energy = *i;
		//build query
		//		std::string
		//				cut(
		//						"tb_energyEcal_ < 0.5 && cand_type_==5 && int(sim_energyEvent_) == ");
		std::string cut("cluster_numEcal_==0 && int(sim_energyEvent_) == ");
		cut.append(obj2str(energy));

		std::string histoName("candNeutralENoEcal");
		histoName.append(obj2str(energy));

		std::string qry(
				"(cand_energyHcal_ + cand_energyEcal_)/sim_energyEvent_>>");
		qry.append(histoName);
		qry.append("(170, -1.2, 2.2)");
		std::string poshHistoName(obj2str(energy));
		poshHistoName.append(" GeV");

		tree_->Draw(qry.c_str(), cut.c_str());
		TH1* tbNeutralENoEcal = util_.formatHisto(histoName, poshHistoName,
				"E_{reco}/E_{true}", util_.nextColor(), kWhite, 2);
		std::pair<double, double> gaus = util_.fitStabilisedGaussian(
				tbNeutralENoEcal);
		candGraphSample.addPoint(energy, tbNeutralENoEcal->GetMean());
		double gaussianess = fabs(gaus.first / tbNeutralENoEcal->GetMean());
		if (gaussianess < 2 && gaussianess > 0.5) {
			candGraph.addPoint(energy, gaus.first);
		}
		util_.accumulateObjects(tbNeutralENoEcal);

	}

	util_.flushPage();

	util_.newPage();

	rStyle->SetOptLogx(true);

	util_.addTitle("Gaussian means");
	TGraph* tbG = new TGraph(tbGraph.finalise());
	util_.formatGraph(tbG, "TB Rechit energy (MIP in ECAL)", "E_{beam} (GeV)",
			"Response", util_.nextColor());
	tbG->GetYaxis()->SetRangeUser(0, 2);
	tbG->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(tbG, "ALP");

	TGraph* clusterG = new TGraph(clusterGraph.finalise());
	util_.formatGraph(clusterG, "Cluster energy (MIP in ECAL)",
			"E_{beam} (GeV)", "Response", util_.nextColor());
	clusterG->GetYaxis()->SetRangeUser(0, 2);
	clusterG->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(clusterG, "ALP");

	TGraph* candG = new TGraph(candGraph.finalise());
	util_.formatGraph(candG, "PFCandidate energy", "E_{beam} (GeV)",
			"Response", util_.nextColor());
	candG->GetYaxis()->SetRangeUser(0, 2);
	candG->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(candG, "ALP");
	util_.addTitle("Arithmetic means");
	//Sample means
	TGraph* tbGS = new TGraph(tbGraphSample.finalise());
	util_.formatGraph(tbGS, "TB Rechit energy (MIP in ECAL)", "E_{beam} (GeV)",
			"Response", util_.nextColor());
	tbGS->GetYaxis()->SetRangeUser(0, 2);
	tbGS->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(tbGS, "ALP");

	TGraph* clusterGS = new TGraph(clusterGraphSample.finalise());
	util_.formatGraph(clusterGS, "Cluster energy (MIP in ECAL)",
			"E_{beam} (GeV)", "Response", util_.nextColor());
	clusterGS->GetYaxis()->SetRangeUser(0, 2);
	clusterGS->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(clusterGS, "ALP");

	TGraph* candGS = new TGraph(candGraphSample.finalise());
	util_.formatGraph(candGS, "PFCandidate energy", "E_{beam} (GeV)",
			"Response", util_.nextColor());
	candGS->GetYaxis()->SetRangeUser(0, 2);
	candGS->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(candGS, "ALP");

	util_.accumulateTable(tbGraphSample, "Rechits MIP in ECAL, Sample means");
	util_.accumulateTable(candGraphSample, "PFCands MIP in ECAL, Sample means");

	util_.flushPage();

}

void CommonProcessing::doEfficiencyPlots(const std::vector<int>& energies) {
	TStyle* rStyle = util_.makeStyle("effStyle");
	rStyle->SetOptLogy(false);
	rStyle->SetOptStat(0);
	rStyle->SetOptFit(1);
	rStyle->SetOptLogz(false);
	rStyle->SetOptLogx(false);
	rStyle->SetOptTitle(0);
	rStyle->SetFrameFillColor(kWhite);
	//Supresses TGraph errors in X
	rStyle->SetErrorX(0);
	rStyle->cd();
	util_.newPage();

	util_.addTitle("Efficiency Plots; PFCandidates");

	JGraph typeGraph("typeGraph");
	JGraph hadEMGraph("hadEMGraph");
	unsigned seen(1);
	for (std::vector<int>::const_iterator i = energies.begin(); i
			!= energies.end(); ++i) {

		int energy = *i;
		//build query
		std::string cut("int(sim_energyEvent_) == ");
		cut.append(obj2str(energy));

		std::string histoName("type");
		histoName.append(obj2str(energy));

		std::string qry("cand_type_>>");
		qry.append(histoName);
		qry.append("(14,1,15)");

		tree_->Draw(qry.c_str(), cut.c_str());
		TH1* histo = util_.getHisto(histoName);
		std::string beamE("E_{beam} = ");
		beamE.append(obj2str(energy));
		beamE.append(" GeV");

		util_.formatHisto(histoName, beamE, "Candidate type",
				util_.nextColor(), util_.nextColor(), 1);

		util_.accumulateObjects(histo);

		double nonHadrons = histo->GetBinContent(4) + histo->GetBinContent(8);
		double total = histo->GetEntries();
		double hadronEff = 1.0 - static_cast<double> (nonHadrons) / total;
		std::cout << "Hadron efficiency for energy " << energy << " = "
				<< hadronEff * 100.0 << "%" << std::endl;

		typeGraph.addPoint(energy, hadronEff);

		std::string qryb("cand_energyNeutralHad_/cand_energyEvent_>>");

		std::string fracName("frac");
		fracName.append(obj2str(energy));
		qryb.append(fracName);

		tree_->Draw(qryb.c_str(), cut.c_str());
		TH1* frac = (TH1*) util_.getHisto(fracName);

		double ePhot = frac->GetBinContent(1);
		double photFrac = ePhot / frac->GetEntries();
		std::cout << "Hadronic energy fraction = " << 1.0 - photFrac << "\n";
		hadEMGraph.addPoint(energy, 1.0 - photFrac);

	}

	util_.flushPage();
	util_.newPage();

	util_.addTitle("Efficiency Plots; Performance");

	rStyle->SetOptLogx(true);
	TGraph* typeG = new TGraph(typeGraph.finalise());
	TGraph* hadEMG = new TGraph(hadEMGraph.finalise());

	util_.formatGraph(typeG, "PFCandidate efficiency", "E_{beam} (GeV)",
			"Hadron finding efficiency", util_.nextColor());
	typeG->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(typeG, "ALP");

	util_.formatGraph(hadEMG, "Hadronic energy fraction", "E_{beam} (GeV)",
			"Hadron energy efficiency", util_.nextColor());
	hadEMG->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(hadEMG, "ALP");

	util_.addTitle("Frequency spectrum");

	tree_->Draw("sim_energyEvent_>>simELow(10,0,10)", "sim_energyEvent_ < 10");
	util_.accumulateObjects(util_.formatHisto("simELow", "", "p_{beam} (GeV)",
			util_.nextColor(), util_.nextColor(), 1));

	tree_->Draw("sim_energyEvent_>>simE(301,0,301)", "");
	util_.accumulateObjects(util_.formatHisto("simE", "", "p_{beam} (GeV)",
			util_.nextColor(), util_.nextColor(), 1));

	util_.flushPage();

	util_.accumulateSpecial(typeG, rStyle, "ALP", "hadronFindingEff");
	util_.accumulateSpecial(hadEMG, rStyle, "ALP", "hadronEnergyEff");

}

void CommonProcessing::doResponsePlots(const std::vector<int>& energies) {
	TStyle* rStyle = util_.makeStyle("respStyle");
	rStyle->SetOptLogy(false);
	rStyle->SetOptStat(0);
	rStyle->SetOptFit(0);
	rStyle->SetOptLogz(false);
	rStyle->SetOptLogx(false);
	rStyle->SetOptTitle(1);
	rStyle->SetFrameFillColor(kWhite);
	//Supresses TGraph errors in X
	rStyle->SetErrorX(0);
	rStyle->cd();

	util_.newPage();
	util_.addTitle("Interacting in ECAL");
	util_.flushPage();

	util_.newPage();
	util_.enableAutoFlush(true, 16);
	util_.setSquareCanvasDimension(4);

	JGraph tbGraph("tbGraph");
	JGraph clusterGraph("clusterGraph");
	JGraph candGraph("candGraph");

	JGraph tbGraphSample("tbGraphSample");
	JGraph clusterGraphSample("clusterGraphSample");
	JGraph candGraphSample("candGraphSample");

	util_.addTitle("Raw Rechits");

	unsigned seen(1);
	for (std::vector<int>::const_iterator i = energies.begin(); i
			!= energies.end(); ++i) {

		int energy = *i;
		//build query
		//		std::string
		//				cut(
		//						"tb_energyEcal_ < 0.5 && cand_type_==1 && int(sim_energyEvent_) == ");
		std::string cut("tb_energyEcal_> 0.5 && int(sim_energyEvent_) == ");
		cut.append(obj2str(energy));

		std::string histoName("tbNeutralENoEcal");
		histoName.append(obj2str(energy));

		std::string qry("tb_energyEvent_/sim_energyEvent_>>");
		qry.append(histoName);
		qry.append("(170, -1.2, 2.2)");

		std::string poshHistoName(obj2str(energy));
		poshHistoName.append(" GeV");

		tree_->Draw(qry.c_str(), cut.c_str());
		TH1* tbNeutralENoEcal = util_.formatHisto(histoName, poshHistoName,
				"E_{rechits}/E_{beam}", util_.nextColor(), kWhite, 2);
		std::pair<double, double> gaus = util_.fitStabilisedGaussian(
				tbNeutralENoEcal);
		tbGraphSample.addPoint(energy, tbNeutralENoEcal->GetMean());
		double gaussianess = fabs(gaus.first / tbNeutralENoEcal->GetMean());
		if (gaussianess < 2 && gaussianess > 0.5) {
			tbGraph.addPoint(energy, gaus.first);
		}
		util_.accumulateObjects(tbNeutralENoEcal);

	}

	util_.flushPage();

	util_.newPage();

	util_.addTitle("PFClusters");

	seen = 1;
	for (std::vector<int>::const_iterator i = energies.begin(); i
			!= energies.end(); ++i) {

		int energy = *i;
		//build query
		//		std::string
		//				cut(
		//						"tb_energyEcal_ < 0.5 && cand_type_==1 && int(sim_energyEvent_) == ");
		std::string cut("cluster_numEcal_!=0 && int(sim_energyEvent_) == ");
		cut.append(obj2str(energy));

		std::string histoName("clustersNeutralENoEcal");
		histoName.append(obj2str(energy));

		std::string qry("cluster_energyEvent_/sim_energyEvent_>>");
		qry.append(histoName);
		qry.append("(170, -1.2, 2.2)");

		std::string poshHistoName(obj2str(energy));
		poshHistoName.append(" GeV");

		tree_->Draw(qry.c_str(), cut.c_str());
		TH1* tbNeutralENoEcal = util_.formatHisto(histoName, poshHistoName,
				"E_{clusters}/E_{true}", util_.nextColor(), kWhite, 2);
		std::pair<double, double> gaus = util_.fitStabilisedGaussian(
				tbNeutralENoEcal);
		clusterGraphSample.addPoint(energy, tbNeutralENoEcal->GetMean());
		double gaussianess = fabs(gaus.first / tbNeutralENoEcal->GetMean());
		if (gaussianess < 2 && gaussianess > 0.5) {
			clusterGraph.addPoint(energy, gaus.first);
		}
		util_.accumulateObjects(tbNeutralENoEcal);

	}

	util_.flushPage();
	util_.newPage();

	util_.addTitle("PFCandidates");

	seen = 1;
	for (std::vector<int>::const_iterator i = energies.begin(); i
			!= energies.end(); ++i) {

		int energy = *i;
		//build query
		//		std::string
		//				cut(
		//						"tb_energyEcal_ < 0.5 && cand_type_==5 && int(sim_energyEvent_) == ");
		std::string cut("cluster_numEcal_!=0 && int(sim_energyEvent_) == ");
		cut.append(obj2str(energy));

		std::string histoName("candNeutralENoEcal");
		histoName.append(obj2str(energy));

		std::string qry(
				"(cand_energyHcal_ + cand_energyEcal_)/sim_energyEvent_>>");
		qry.append(histoName);
		qry.append("(170, -1.2, 2.2)");
		std::string poshHistoName(obj2str(energy));
		poshHistoName.append(" GeV");

		tree_->Draw(qry.c_str(), cut.c_str());
		TH1* tbNeutralENoEcal = util_.formatHisto(histoName, poshHistoName,
				"E_{reco}/E_{true}", util_.nextColor(), kWhite, 2);
		std::pair<double, double> gaus = util_.fitStabilisedGaussian(
				tbNeutralENoEcal);
		candGraphSample.addPoint(energy, tbNeutralENoEcal->GetMean());
		double gaussianess = fabs(gaus.first / tbNeutralENoEcal->GetMean());
		if (gaussianess < 2 && gaussianess > 0.5) {
			candGraph.addPoint(energy, gaus.first);
		}
		util_.accumulateObjects(tbNeutralENoEcal);

	}

	util_.flushPage();

	util_.newPage();

	rStyle->SetOptLogx(true);

	util_.addTitle("Gaussian means");
	TGraph* tbG = new TGraph(tbGraph.finalise());
	util_.formatGraph(tbG, "TB Rechit energy (Int in ECAL)", "E_{beam} (GeV)",
			"Response", util_.nextColor());
	tbG->GetYaxis()->SetRangeUser(0, 2);
	tbG->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(tbG, "ALP");

	TGraph* clusterG = new TGraph(clusterGraph.finalise());
	util_.formatGraph(clusterG, "Cluster energy (Int in ECAL)",
			"E_{beam} (GeV)", "Response", util_.nextColor());
	clusterG->GetYaxis()->SetRangeUser(0, 2);
	clusterG->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(clusterG, "ALP");

	TGraph* candG = new TGraph(candGraph.finalise());
	util_.formatGraph(candG, "PFCandidate energy (Int in ECAL)", "E_{beam} (GeV)",
			"Response", util_.nextColor());
	candG->GetYaxis()->SetRangeUser(0, 2);
	candG->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(candG, "ALP");
	util_.addTitle("Arithmetic means");
	//Sample means
	TGraph* tbGS = new TGraph(tbGraphSample.finalise());
	util_.formatGraph(tbGS, "TB Rechit energy (Int in ECAL)", "E_{beam} (GeV)",
			"Response", util_.nextColor());
	tbGS->GetYaxis()->SetRangeUser(0, 2);
	tbGS->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(tbGS, "ALP");

	TGraph* clusterGS = new TGraph(clusterGraphSample.finalise());
	util_.formatGraph(clusterGS, "Cluster energy (Int in ECAL)",
			"E_{beam} (GeV)", "Response", util_.nextColor());
	clusterGS->GetYaxis()->SetRangeUser(0, 2);
	clusterGS->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(clusterGS, "ALP");

	TGraph* candGS = new TGraph(candGraphSample.finalise());
	util_.formatGraph(candGS, "PFCandidate energy (Int in ECAL)", "E_{beam} (GeV)",
			"Response", util_.nextColor());
	candGS->GetYaxis()->SetRangeUser(0, 2);
	candGS->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(candGS, "ALP");

	util_.accumulateTable(tbGraphSample, "Rechits interacting in ECAL, sample mean");
	util_.accumulateTable(candGraphSample, "PFCands interacting in ECAL, sample mean");

	util_.flushPage();
}

void CommonProcessing::evaluatePlots(const std::vector<int>& energies,
		bool eff, bool resp) {

	doMipInEcalPlots(energies);

	if (resp)
		doResponsePlots(energies);

	if (eff)
		doEfficiencyPlots(energies);

	util_.flushTables();

}

void CommonProcessing::closeFiles() {

	std::string namecpy2(graphicsFile_);
	namecpy2.append("]");
	gPad->Print(namecpy2.c_str());

	std::string macrocpy2(macroFile_);
	macrocpy2.append("]");
	gPad->Print(macrocpy2.c_str());

	util_.flushSpecials(directory_);

}

void commonProcessing() {

	using namespace std;

	cout
			<< "commonProcessing.C -- Script to evaluate PFlow response at discrete energies "
			<< "in the absence of tracking information." << endl;

	string graphicsFile("plots.ps");
	string macroFile("macro.C");

	TChain* chain = new TChain("extraction/Extraction");

	std::vector<int> energies;
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
//	//energies.push_back(225);
	energies.push_back(300);

	//chain->Add("/tmp/PFlowTB_Tree_All_notracks_barrel.root");
	//chain->Add("/tmp/Dipion_Tree_All_10k_fast_0T.root");
	//chain->Add("/tmp/ballin/Dipion_Tree_All_2k_full_0T.root");
	chain->Add("/tmp/ballin/PFlowTB_Tree_All_barrel_tbCalib.root");
	//chain->Add("../test/PFlow_tmp.root");

	CommonProcessing cp(chain, graphicsFile.c_str(), macroFile.c_str(),
			"plots/barrel_tbCalib");
	cp.evaluatePlots(energies, false, true);
	cp.closeFiles();

	gROOT->ProcessLine(".! ps2pdf plots/barrel_tbCalib/plots.ps");
	gROOT->ProcessLine(".! mv plots.pdf plots/barrel_tbCalib/");
}

