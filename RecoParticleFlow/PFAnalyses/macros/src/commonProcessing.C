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

bool endcap(false);
int data(0);
int full(0);
int fast(1);

class CommonProcessing {
public:

	CommonProcessing() {

	}

	CommonProcessing(TTree* t, std::string graphicsFile, std::string macroFile, std::string directory_);

	virtual ~CommonProcessing();

	void reset(TTree* t, std::string graphicsFile, std::string macroFile);

	void doResponsePlots(const std::vector<int>& energies);

	void evaluatePlots(const std::vector<int>& energies, bool eff, bool resp, bool endcapHack = false);

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

	TFile* output_;

	//Real data for the endcaps has high cluster multiplicity
	//Exclude events with lots of neutral activity in addition to charged hadron
	bool endcapHack_;

};

template<typename T> std::string obj2str(T n) {
	std::ostringstream oss;
	oss << n;
	std::string s(oss.str());
	return s;
}

CommonProcessing::CommonProcessing(TTree* t, std::string graphicsFile, std::string macroFile, std::string directory) :
	directory_(directory) {
	util_.init();
	util_.enableAutoFlush(true, 9);
	util_.setSquareCanvasDimension(3);
	reset(t, graphicsFile, macroFile);

}

CommonProcessing::~CommonProcessing() {

}

void CommonProcessing::reset(TTree* t, std::string graphicsFile, std::string macroFile) {
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
	string file(directory_);
	file.append("/commonProcessing.root");
	output_ = new TFile(file.c_str(), "recreate");
}

void CommonProcessing::doMipInEcalPlots(const std::vector<int>& energies) {
	TStyle* rStyle = util_.makeSquashedStyle("mipStyle");
	rStyle->cd();

	util_.newPage();
	util_.addTitle("MIP in ECAL");
	util_.flushPage();

	util_.newPage();
	util_.enableAutoFlush(true, 16);
	util_.setSquareCanvasDimension(4);

	JGraph tbGraph("tbMipGraph", true);
	JGraph clusterGraph("clusterMipGraph", true);
	JGraph candGraph("candMipGraph", true);

	JGraph tbGraphSample("tbGraphMipSample", true);
	JGraph clusterGraphSample("clusterMipGraphSample", true);
	JGraph candGraphSample("candMipGraphSample", true);

	JGraph tbReso("tbResMip", true);
	JGraph candReso("candResMip", true);

	util_.addTitle("Raw Rechits");

	unsigned seen(1);
	for (std::vector<int>::const_iterator i = energies.begin(); i != energies.end(); ++i) {

		int energy = *i;
		//build query
		//		std::string
		//				cut(
		//						"tb_energyEcal_ < 0.5 && cand_type_==1 && int(sim_energyEvent_) == ");
		std::string cut("tb_energyEcal_<1.0 && int(sim_energyEvent_) == ");
		cut.append(obj2str(energy));

		std::string histoName("tbNeutralENoEcal");
		histoName.append(obj2str(energy));

		std::string qry("tb_energyEvent_/sim_energyEvent_>>");
		qry.append(histoName);
		qry.append("(170, -1.2, 2.2)");

		std::string poshHistoName(obj2str(energy));
		poshHistoName.append(" GeV");

		tree_->Draw(qry.c_str(), cut.c_str());
		TH1* tbNeutralENoEcal = util_.formatHisto(histoName, poshHistoName, "R", util_.nextColor(), kWhite, 2);
		double maxNorm = 1.0 / tbNeutralENoEcal->GetBinContent(tbNeutralENoEcal->GetMaximumBin());
		tbNeutralENoEcal->Scale(maxNorm);

		util_.streamTH1ToGraphFile(string(directory_).append("/hits_").append(obj2str(energy)).append("GeV_mip.dat"),
				tbNeutralENoEcal, true);

		std::pair<double, double> gaus = util_.fitStabilisedGaussian(tbNeutralENoEcal);

		tbGraphSample.addPoint(energy, tbNeutralENoEcal->GetMean());
		double gaussianess = fabs(gaus.first / tbNeutralENoEcal->GetMean());
		if (gaussianess < 2 && gaussianess > 0.5) {
			tbGraph.addPoint(energy, 0, gaus.first, tbNeutralENoEcal->GetFunction("stableGaus")->GetParError(1));
			tbReso.addPoint(energy, 0, gaus.second, tbNeutralENoEcal->GetFunction("stableGaus")->GetParError(2));
		}
		util_.accumulateObjects(tbNeutralENoEcal);

	}

	util_.flushPage(true);

	util_.newPage();

	util_.addTitle("PFClusters");

	seen = 1;
	for (std::vector<int>::const_iterator i = energies.begin(); i != energies.end(); ++i) {

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
		TH1* tbNeutralENoEcal = util_.formatHisto(histoName, poshHistoName, "R", util_.nextColor(), kWhite, 2);
		double maxNorm = 1.0 / tbNeutralENoEcal->GetBinContent(tbNeutralENoEcal->GetMaximumBin());
		tbNeutralENoEcal->Scale(maxNorm);
		std::pair<double, double> gaus = util_.fitStabilisedGaussian(tbNeutralENoEcal, 0.1, 1.5);
		clusterGraphSample.addPoint(energy, tbNeutralENoEcal->GetMean());
		double gaussianess = fabs(gaus.first / tbNeutralENoEcal->GetMean());
		if (gaussianess < 2 && gaussianess > 0.5) {
			clusterGraph.addPoint(energy, 0, gaus.first, tbNeutralENoEcal->GetFunction("stableGaus")->GetParError(1));
		}
		util_.accumulateObjects(tbNeutralENoEcal);

	}

	util_.flushPage(true);
	util_.newPage();

	util_.addTitle("PFCandidates");

	seen = 1;
	for (std::vector<int>::const_iterator i = energies.begin(); i != energies.end(); ++i) {

		int energy = *i;
		//build query
		//		std::string
		//				cut(
		//						"tb_energyEcal_ < 0.5 && cand_type_==5 && int(sim_energyEvent_) == ");
		std::string cut("cluster_numEcal_==0 && int(sim_energyEvent_) == ");
		cut.append(obj2str(energy));

		std::string histoName("candNeutralENoEcal");
		histoName.append(obj2str(energy));

		std::string qry("(cand_energyHcal_ + cand_energyEcal_)/sim_energyEvent_>>");
		qry.append(histoName);
		qry.append("(170, -1.2, 2.2)");
		std::string poshHistoName(obj2str(energy));
		poshHistoName.append(" GeV");

		tree_->Draw(qry.c_str(), cut.c_str());
		TH1* tbNeutralENoEcal = util_.formatHisto(histoName, poshHistoName, "R", util_.nextColor(), kWhite, 2);
		double maxNorm = 1.0 / tbNeutralENoEcal->GetBinContent(tbNeutralENoEcal->GetMaximumBin());
		tbNeutralENoEcal->Scale(maxNorm);
		std::pair<double, double> gaus = util_.fitStabilisedGaussian(tbNeutralENoEcal, 0.1, 1.5);
		candGraphSample.addPoint(energy, tbNeutralENoEcal->GetMean());
		double gaussianess = fabs(gaus.first / tbNeutralENoEcal->GetMean());
		if (gaussianess < 2 && gaussianess > 0.5) {
			candGraph.addPoint(energy, 0, gaus.first, tbNeutralENoEcal->GetFunction("stableGaus")->GetParError(1));
			candReso.addPoint(energy, 0, gaus.second, tbNeutralENoEcal->GetFunction("stableGaus")->GetParError(2));
		}
		util_.accumulateObjects(tbNeutralENoEcal);

		util_.streamTH1ToGraphFile(string(directory_).append("/cands_").append(obj2str(energy)).append("GeV_mip.dat"),
				tbNeutralENoEcal, true);


	}

	util_.flushPage(true);

	TStyle* zStyle = util_.makeStyle("zStyle");
	zStyle->SetOptLogx(true);
	zStyle->cd();

	util_.newPage();

	util_.addTitle("Gaussian means");
	TGraphErrors* tbG = new TGraphErrors(tbGraph.finaliseWithErrors());
	util_.formatGraph(tbG, "TB Rechit energy (MIP in ECAL)", "E_{beam} (GeV)", "Response", util_.nextColor());
	tbG->GetYaxis()->SetRangeUser(0, 2);
	tbG->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(tbG, "ALP");

	TGraphErrors* clusterG = new TGraphErrors(clusterGraph.finaliseWithErrors());
	util_.formatGraph(clusterG, "Cluster energy (MIP in ECAL)", "E_{beam} (GeV)", "Response", util_.nextColor());
	clusterG->GetYaxis()->SetRangeUser(0, 2);
	clusterG->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(clusterG, "ALP");

	TGraphErrors* candG = new TGraphErrors(candGraph.finaliseWithErrors());
	util_.formatGraph(candG, "PFCandidate energy", "E_{beam} (GeV)", "Response", util_.nextColor());
	candG->GetYaxis()->SetRangeUser(0, 2);
	candG->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(candG, "ALP");
	util_.addTitle("Arithmetic means");
	//Sample means
	TGraph* tbGS = new TGraph(tbGraphSample.finalise());
	util_.formatGraph(tbGS, "TB Rechit energy (MIP in ECAL)", "E_{beam} (GeV)", "Response", util_.nextColor());
	tbGS->GetYaxis()->SetRangeUser(0, 2);
	tbGS->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(tbGS, "ALP");

	TGraph* clusterGS = new TGraph(clusterGraphSample.finalise());
	util_.formatGraph(clusterGS, "Cluster energy (MIP in ECAL)", "E_{beam} (GeV)", "Response", util_.nextColor());
	clusterGS->GetYaxis()->SetRangeUser(0, 2);
	clusterGS->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(clusterGS, "ALP");

	TGraph* candGS = new TGraph(candGraphSample.finalise());
	util_.formatGraph(candGS, "PFCandidate energy", "E_{beam} (GeV)", "Response", util_.nextColor());
	candGS->GetYaxis()->SetRangeUser(0, 2);
	candGS->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(candGS, "ALP");

	util_.accumulateTable(tbGraph, "Rechits MIP in ECAL, Fitted means");
	util_.accumulateTable(candGraph, "PFCands MIP in ECAL, Fitted means");

	cout << "Streaming MIP JGraphs to file\n";
	tbGraph.streamToFile(string(directory_).append("/rechits_mip.dat"), true);
	candGraph.streamToFile(string(directory_).append("/pfcands_mip.dat"), true);
	tbReso.streamToFile(string(directory_).append("/rechits_mip_reso.dat"), true);
	candReso.streamToFile(string(directory_).append("/pfcands_mip_reso.dat"), true);

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

	JGraph chargedHadEFraction("chargedHadEFraction", true);
	JGraph hcalClusterMultiplicity("hcalClusterMultiplicity", true);

	unsigned seen(1);
	for (std::vector<int>::const_iterator i = energies.begin(); i != energies.end(); ++i) {

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

		util_.formatHisto(histoName, beamE, "Candidate type", util_.nextColor(), util_.nextColor(), 1);

		util_.accumulateObjects(histo);

		double nonHadrons = histo->GetBinContent(4) + histo->GetBinContent(8);
		double total = histo->GetEntries();
		double hadronEff = 1.0 - static_cast<double> (nonHadrons) / total;
		std::cout << "Hadron efficiency for energy " << energy << " = " << hadronEff * 100.0 << "%" << std::endl;

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

		string qryc("1.0-(cand_energyNeutralHad_ + cand_energyNeutralEM_)/cand_energyEvent_>>");
		string hadFracName("hadFrac");
		hadFracName.append(obj2str(energy));
		qryc.append(hadFracName);
		tree_->Draw(qryc.c_str(), cut.c_str());
		TH1* hadEFrac = util_.getType<TH1>(hadFracName);
		double mean = hadEFrac->GetMean();
		chargedHadEFraction.addPoint(energy, 0, hadEFrac->GetMean(), hadEFrac->GetRMS());

		string qryd("cluster_numHcal_>>");
		string clusterName("clusterMult");
		clusterName.append(obj2str(energy));
		qryd.append(clusterName);
		qryd.append("(10, 0, 10)"); //changing this will affect the mode below
		tree_->Draw(qryd.c_str(), cut.c_str());
		TH1* clusters = util_.getType<TH1>(clusterName);
		double mode = clusters->GetMean(); //note that for the binning specified, this is the number of clusters
		hcalClusterMultiplicity.addPoint(energy, 0, mode, clusters->GetRMS());

	}

	chargedHadEFraction.streamToFile(string(directory_).append("/chargedHadEFrac.dat"), true);
	hcalClusterMultiplicity.streamToFile(string(directory_).append("/hcalClusterMultiplicity.dat"), true);
	cout << "HCAl cluster mulitplicities: \n" << hcalClusterMultiplicity << "\n";
	util_.flushPage();
	util_.newPage();

	util_.addTitle("Efficiency Plots; Performance");

	rStyle->SetOptLogx(true);
	TGraph* typeG = new TGraph(typeGraph.finalise());
	TGraph* hadEMG = new TGraph(hadEMGraph.finalise());

	util_.formatGraph(typeG, "PFCandidate efficiency", "E_{beam} (GeV)", "Hadron finding efficiency", util_.nextColor());
	typeG->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(typeG, "ALP");

	util_.formatGraph(hadEMG, "Hadronic energy fraction", "E_{beam} (GeV)", "Hadron energy efficiency", util_.nextColor());
	hadEMG->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(hadEMG, "ALP");

	util_.addTitle("Frequency spectrum");

	tree_->Draw("sim_energyEvent_>>simELow(10,0,10)", "sim_energyEvent_ < 10");
	util_.accumulateObjects(util_.formatHisto("simELow", "", "p_{beam} (GeV)", util_.nextColor(), util_.nextColor(), 1));

	tree_->Draw("sim_energyEvent_>>simE(301,0,301)", "");
	util_.accumulateObjects(util_.formatHisto("simE", "", "p_{beam} (GeV)", util_.nextColor(), util_.nextColor(), 1));

	util_.flushPage();

	util_.accumulateSpecial(typeG, rStyle, "ALP", "hadronFindingEff");
	util_.accumulateSpecial(hadEMG, rStyle, "ALP", "hadronEnergyEff");

}

void CommonProcessing::doResponsePlots(const std::vector<int>& energies) {
	TStyle* rStyle = util_.makeSquashedStyle("respStyle");

	rStyle->cd();
	util_.newPage();
	util_.addTitle("Interacting in ECAL");
	util_.flushPage();

	util_.newPage();
	util_.enableAutoFlush(true, 16);
	util_.setSquareCanvasDimension(4);

	JGraph tbGraph("tbIntGraph", true);
	JGraph clusterGraph("clusterIntGraph", true);
	JGraph candGraph("candIntGraph", true);

	JGraph tbGraphSample("tbIntGraphSample", true);
	JGraph clusterGraphSample("clusterIntGraphSample", true);
	JGraph candGraphSample("candIntGraphSample", true);

	JGraph tbReso("tbResInt", true);
	JGraph candReso("candResInt", true);

	util_.addTitle("Raw Rechits");

	unsigned seen(1);
	for (std::vector<int>::const_iterator i = energies.begin(); i != energies.end(); ++i) {

		int energy = *i;
		//build query
		//		std::string
		//				cut(
		//						"tb_energyEcal_ < 0.5 && cand_type_==1 && int(sim_energyEvent_) == ");
		std::string cut("tb_energyEcal_> 1.0 && int(sim_energyEvent_) == ");
		cut.append(obj2str(energy));

		std::string histoName("tbIntEcal");
		histoName.append(obj2str(energy));

		std::string qry("tb_energyEvent_/sim_energyEvent_>>");
		qry.append(histoName);
		qry.append("(170, -1.2, 2.2)");

		std::string poshHistoName(obj2str(energy));
		poshHistoName.append(" GeV");

		tree_->Draw(qry.c_str(), cut.c_str());
		TH1* tbNeutralENoEcal = util_.formatHisto(histoName, poshHistoName, "R", util_.nextColor(), kWhite, 2);
		double maxNorm = 1.0 / tbNeutralENoEcal->GetBinContent(tbNeutralENoEcal->GetMaximumBin());
		tbNeutralENoEcal->Scale(maxNorm);


		util_.streamTH1ToGraphFile(string(directory_).append("/hits_").append(obj2str(energy)).append("GeV_int.dat"),
				tbNeutralENoEcal, true);

		std::pair<double, double> gaus = util_.fitStabilisedGaussian(tbNeutralENoEcal);
		tbGraphSample.addPoint(energy, tbNeutralENoEcal->GetMean());
		double gaussianess = fabs(gaus.first / tbNeutralENoEcal->GetMean());
		if (gaussianess < 2 && gaussianess > 0.5) {
			tbGraph.addPoint(energy, 0, gaus.first, tbNeutralENoEcal->GetFunction("stableGaus")->GetParError(1));
			tbReso.addPoint(energy, 0, gaus.second, tbNeutralENoEcal->GetFunction("stableGaus")->GetParError(2));
		}
		util_.accumulateObjects(tbNeutralENoEcal);

		string ecalActivity("ecalActivity");
		ecalActivity.append(obj2str(energy));
		string qryEcal("tb_energyEcal_>>");
		qryEcal.append(ecalActivity);
		qryEcal.append("(520,-2,50)");

		tree_->Draw(qryEcal.c_str(), "int(sim_energyEvent_)==50");
		TH1* ecal = util_.getType<TH1> (ecalActivity);
       		maxNorm = 1.0 / ecal->GetBinContent(ecal->GetMaximumBin());
	        ecal->Scale(maxNorm);

		util_.streamTH1ToGraphFile(string(directory_).append("/ecalActivity.dat"), ecal, true);

//		string histoName2D("tbIntEcal2D");
//		histoName2D.append(obj2str(energy));
//		string qry2D("tb_energyEcal_/sim_energyEvent_:tb_energyHcal_/sim_energyEvent_>>");
//		qry2D.append(histoName2D);
//		qry2D.append("(15, -0.2, 1.3, 15, -0.2, 1.3)");
//		string eCut("int(sim_energyEvent_) == ");
//		eCut.append(obj2str(energy));
//
//		tree_->Draw(qry2D.c_str(), eCut.c_str());
//		TH2* histo2D = util_.getType<TH2>(histoName2D);
//		//double maxNorm = 1.0 / histo2D->GetBinContent(histo2D->GetMaximumBin());
//		//histo2D->Scale(maxNorm);
//
//		util_.streamTH2ToGraphFile(string(directory_).append("/ecalAndHcal_").append(obj2str(energy)).append("GeV_2D.dat"),
//				histo2D, true);

	}

	util_.flushPage(true);

	util_.newPage();

	util_.addTitle("PFClusters");

	seen = 1;
	for (std::vector<int>::const_iterator i = energies.begin(); i != energies.end(); ++i) {

		int energy = *i;
		//build query
		//		std::string
		//				cut(
		//						"tb_energyEcal_ < 0.5 && cand_type_==1 && int(sim_energyEvent_) == ");
		std::string cut("cluster_numEcal_!=0 && int(sim_energyEvent_) == ");
		cut.append(obj2str(energy));

		std::string histoName("clustersIntEcal");
		histoName.append(obj2str(energy));

		std::string qry("cluster_energyEvent_/sim_energyEvent_>>");
		qry.append(histoName);
		qry.append("(170, -1.2, 2.2)");

		std::string poshHistoName(obj2str(energy));
		poshHistoName.append(" GeV");

		tree_->Draw(qry.c_str(), cut.c_str());
		TH1* tbNeutralENoEcal = util_.formatHisto(histoName, poshHistoName, "R", util_.nextColor(), kWhite, 2);
		double maxNorm = 1.0 / tbNeutralENoEcal->GetBinContent(tbNeutralENoEcal->GetMaximumBin());
		tbNeutralENoEcal->Scale(maxNorm);
		std::pair<double, double> gaus = util_.fitStabilisedGaussian(tbNeutralENoEcal, 0.1, 1.5);
		clusterGraphSample.addPoint(energy, tbNeutralENoEcal->GetMean());
		double gaussianess = fabs(gaus.first / tbNeutralENoEcal->GetMean());
		if (gaussianess < 2 && gaussianess > 0.5) {
			clusterGraph.addPoint(energy, 0, gaus.first, tbNeutralENoEcal->GetFunction("stableGaus")->GetParError(1));
		}
		util_.accumulateObjects(tbNeutralENoEcal);

	}

	util_.flushPage(true);
	util_.newPage();

	util_.addTitle("PFCandidates");

	seen = 1;
	for (std::vector<int>::const_iterator i = energies.begin(); i != energies.end(); ++i) {

		int energy = *i;
		//build query
		//		std::string
		//				cut(
		//						"tb_energyEcal_ < 0.5 && cand_type_==5 && int(sim_energyEvent_) == ");

		string cut;
		cut.append("cand_energyNeutralEM_ < 0.05 && cluster_numEcal_!=0 && int(sim_energyEvent_) == ");

		cut.append(obj2str(energy));

		std::string histoName("candIntEcal");
		histoName.append(obj2str(energy));

		std::string qry("(cand_energyHcal_ + cand_energyEcal_)/sim_energyEvent_>>");
		qry.append(histoName);
		qry.append("(170, -1.2, 2.2)");
		std::string poshHistoName(obj2str(energy));
		poshHistoName.append(" GeV");

		tree_->Draw(qry.c_str(), cut.c_str());
		TH1* tbNeutralENoEcal = util_.formatHisto(histoName, poshHistoName, "R", util_.nextColor(), kWhite, 2);
		//tbNeutralENoEcal->Scale(1.0/tbNeutralENoEcal->Integral());
		double maxNorm = 1.0 / tbNeutralENoEcal->GetBinContent(tbNeutralENoEcal->GetMaximumBin());
		tbNeutralENoEcal->Scale(maxNorm);
		std::pair<double, double> gaus = util_.fitStabilisedGaussian(tbNeutralENoEcal, 0.1, 1.5);
		candGraphSample.addPoint(energy, tbNeutralENoEcal->GetMean());
		double gaussianess = fabs(gaus.first / tbNeutralENoEcal->GetMean());
		if (gaussianess < 2 && gaussianess > 0.5) {
			candGraph.addPoint(energy, 0, gaus.first, tbNeutralENoEcal->GetFunction("stableGaus")->GetParError(1));
			candReso.addPoint(energy, 0, gaus.second, tbNeutralENoEcal->GetFunction("stableGaus")->GetParError(2));
		}
		util_.accumulateObjects(tbNeutralENoEcal);

		util_.streamTH1ToGraphFile(string(directory_).append("/cands_").append(obj2str(energy)).append("GeV_int.dat"),
				tbNeutralENoEcal, true);


	}

	util_.flushPage(true);

	TStyle* u = util_.makeStyle("uStyle");
	u->SetOptLogx(true);
	u->cd();

	util_.newPage();

	util_.addTitle("Gaussian means");
	TGraphErrors* tbG = new TGraphErrors(tbGraph.finaliseWithErrors());
	util_.formatGraph(tbG, "TB Rechit energy (Int in ECAL)", "E_{beam} (GeV)", "R", util_.nextColor());
	tbG->GetYaxis()->SetRangeUser(0, 2);
	tbG->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(tbG, "ALP");

	TGraphErrors* clusterG = new TGraphErrors(clusterGraph.finaliseWithErrors());
	util_.formatGraph(clusterG, "Cluster energy (Int in ECAL)", "E_{beam} (GeV)", "R", util_.nextColor());
	clusterG->GetYaxis()->SetRangeUser(0, 2);
	clusterG->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(clusterG, "ALP");

	TGraphErrors* candG = new TGraphErrors(candGraph.finaliseWithErrors());
	util_.formatGraph(candG, "PFCandidate energy (Int in ECAL)", "E_{beam} (GeV)", "R", util_.nextColor());
	candG->GetYaxis()->SetRangeUser(0, 2);
	candG->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(candG, "ALP");
	util_.addTitle("Arithmetic means");
	//Sample means
	TGraph* tbGS = new TGraph(tbGraphSample.finalise());
	util_.formatGraph(tbGS, "TB Rechit energy (Int in ECAL)", "E_{beam} (GeV)", "R", util_.nextColor());
	tbGS->GetYaxis()->SetRangeUser(0, 2);
	tbGS->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(tbGS, "ALP");

	TGraph* clusterGS = new TGraph(clusterGraphSample.finalise());
	util_.formatGraph(clusterGS, "Cluster energy (Int in ECAL)", "E_{beam} (GeV)", "R", util_.nextColor());
	clusterGS->GetYaxis()->SetRangeUser(0, 2);
	clusterGS->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(clusterGS, "ALP");

	TGraph* candGS = new TGraph(candGraphSample.finalise());
	util_.formatGraph(candGS, "PFCandidate energy (Int in ECAL)", "E_{beam} (GeV)", "R", util_.nextColor());
	candGS->GetYaxis()->SetRangeUser(0, 2);
	candGS->GetXaxis()->SetRangeUser(2, 400);
	util_.accumulateObjects(candGS, "ALP");

	util_.accumulateTable(tbGraph, "Rechits interacting in ECAL, Fitted mean");
	util_.accumulateTable(candGraph, "PFCands interacting in ECAL, Fitted mean");

	cout << "Streaming INT JGraphs to file\n";
	tbGraph.streamToFile(string(directory_).append("/rechits_int.dat"), true);
	candGraph.streamToFile(string(directory_).append("/pfcands_int.dat"), true);
	tbReso.streamToFile(string(directory_).append("/rechits_int_reso.dat"), true);
	candReso.streamToFile(string(directory_).append("/pfcands_int_reso.dat"), true);

	util_.flushPage();
}

void CommonProcessing::evaluatePlots(const std::vector<int>& energies, bool eff, bool resp, bool endcapHack) {
	endcapHack_ = endcapHack;
	util_.setSquareCanvasDimension(4);
	
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
	TStyle* normal = util_.makeStyle("standard");
	normal->cd();
	normal->Write();
	output_->Write();
	output_->Close();

	gROOT->ProcessLine(string(".! ps2pdf plots.ps").c_str());
	gROOT->ProcessLine(string(".! mv plots.pdf ").append(directory_).c_str());

}

void commonProcessing() {

	using namespace std;

	cout << "commonProcessing.C -- Script to evaluate PFlow response at discrete energies." << endl;

	string graphicsFile("plots.ps");
	string macroFile("macro.C");


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

	// --- standard stuff
	CommonProcessing cp(chain, graphicsFile.c_str(), macroFile.c_str(), directory);
	cp.evaluatePlots(energies, true, true, false);
	cp.closeFiles();

}

