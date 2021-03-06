#include <vector>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include "TF1.h"
#include "TH2F.h"
#include "TH2.h"
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
#include "RecoParticleFlow/PFAnalyses/interface/JGraph.h"

template<typename T> std::string obj2str(T n) {
	std::ostringstream oss;
	oss << n;
	std::string s(oss.str());
	return s;
}
/**
 * rfcp /castor/cern.ch/user/b/ballin/tb310pre8/outputtree_allGeV_notracks.root /tmp
 * Magic incantations to make this compile...
 *  gSystem->AddLinkedLibs("-L/afs/cern.ch/user/b/ballin/scratch0/CMSSW_3_1_0_pre10/lib/slc4_ia32_gcc345 -lRecoParticleFlowPFAnalyses");
 *  .include /afs/cern.ch/user/b/ballin/scratch0/CMSSW_3_1_0_pre10/src/
 *  .x isoPionAnalysis.C+
 *
 */

using namespace std;

template<typename T, typename U> std::map<U, T> converse_map(const std::map<T, U>& o) {
	map<U, T> result;
	for (typename map<T, U>::const_iterator begin(o.begin()); begin != o.end(); ++begin)
		result.insert(make_pair(begin->second, begin->first));

	return result;

}

template<typename K, typename R> std::ostream& operator<<(std::ostream& s, const std::pair<K, R>& p) {
	s << "(" << p.first << ", " << p.second << ")";
	return s;
}

class CompareTBAnalysis {
public:
	CompareTBAnalysis() {
	}

	CompareTBAnalysis(std::map<std::string, TTree*> trees, vector<int> energies, std::string graphicsFile,
			std::string macroFile, std::string directory_);

	virtual ~CompareTBAnalysis() {
	}

	void reset(std::map<std::string, TTree*> t, std::vector<int> energies, std::string graphicsFile,
			std::string macroFile);

	void evaluatePlots();

	void closeFiles();

	TMultiGraph* getMultiGraph(const map<string, TGraph*>& inputs, std::string xAxisTitle = "", std::string yAxisTitle =
			"");

private:

	void doClusterMultiplicities();

	void doResponsePlots(std::string type, bool verbose, std::string qry, std::string cut = "");

	void plotMipInEcal();

	void plotTypeSpectrum();

	std::map<std::string, TTree*> trees_;
	std::vector<int> energies_;
	TStyle* defaultStyle_;
	PlotUtil util_;

	std::string graphicsFile_;
	std::string macroFile_;

	std::string directory_;
};

class Comparator {
public:
	Comparator(std::string name, TTree* tree) :
		name_(name), tree_(tree) {
	}

	virtual ~Comparator() {
	}

	std::vector<JGraph> getRatioResponsePlots(std::vector<int> energies, std::string name, std::string queryToTree,
			PlotUtil* util_, std::string additionalCut);

	std::vector<JGraph> getRecoEfficiencyPlots(std::vector<int> energies, std::string name, std::string queryToTree,
			PlotUtil* util_, std::string additionalCut);

	TH2* getClusterMultiplicities(std::vector<int> energies, std::string name, std::string queryToTree, PlotUtil* util_,
			std::string additionalCut);

private:
	std::string name_;
	TTree* tree_;

};

CompareTBAnalysis::CompareTBAnalysis(map<string, TTree*> trees, std::vector<int> energies, std::string graphicsFile,
		std::string macroFile, std::string directory) :
	directory_(directory) {

	util_.init();
	util_.enableAutoFlush(true, 9);
	util_.setSquareCanvasDimension(3);
	reset(trees, energies, graphicsFile, macroFile);

}

void CompareTBAnalysis::reset(map<string, TTree*> trees, std::vector<int> energies, std::string graphicsFile,
		std::string macroFile) {

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

void CompareTBAnalysis::evaluatePlots() {

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

		TH1* plot = util_.formatHisto(name, item.first, "Beam momentum (GeV/c)", util_.nextColor(), util_.nextColor(), 2);
		util_.accumulateObjects(plot, "");
		++count;
	}

	util_.addTitle("TB Analysis - statistics available");

	util_.flushPage();

	//RAW RESPONSE
	doResponsePlots("TB Rechits", false, "tb_energyEvent_/sim_energyEvent_", "tb_energyEcal_ > 0.5");

	//RAW RESPONSE
	doResponsePlots("TB Rechits ECAL MIP", false, "tb_energyEvent_/sim_energyEvent_", "tb_energyEcal_ < 0.5");
	/**
	//PF CLUSTER RESPONSE
	doResponsePlots("PF Clusters", false, "cluster_energyEvent_/sim_energyEvent_", "cluster_numEcal_ != 0");

	//PF CLUSTER RESPONSE - MIP in ECAL
	doResponsePlots("PF Clusters ECAL MIP", false, "cluster_energyEvent_/sim_energyEvent_", "cluster_numEcal_ == 0");
	*/
	//PF CANDIDATE RESPONSE
	doResponsePlots("PF Candidates", false, "(cand_energyEcal_ + cand_energyHcal_)/sim_energyEvent_",
			"cluster_numEcal_ != 0 && cand_energyNeutralEM_ < 1");

	//PF CANDIDATE RESPONSE - MIP in ECAL
	doResponsePlots("PF Candidates ECAL MIP", false, "(cand_energyEcal_ + cand_energyHcal_)/sim_energyEvent_",
			"cluster_numEcal_ == 0 && cand_energyNeutralEM_ < 1");

	//plotMipInEcal();

	//plotTypeSpectrum();

	doClusterMultiplicities();

	cout << "Leaving " << __PRETTY_FUNCTION__ << endl;
}

void CompareTBAnalysis::plotMipInEcal() {
	TStyle* rStyle = util_.makeStyle("ecalStyle");
	rStyle->SetOptLogy(false);
	rStyle->SetOptStat(11111);
	rStyle->SetOptFit(1);
	rStyle->SetOptLogz(false);
	rStyle->SetOptLogx(false);
	//Supresses TGraph errors in X
	rStyle->SetErrorX(0);
	rStyle->cd();

	util_.newPage();

	map<string, TTree*>::iterator it = trees_.begin();
	unsigned count(0);
	for (; it != trees_.end(); ++it) {
		pair<const string, TTree*> item = *it;

		std::string name("ecalE");
		name.append("_");
		name.append(obj2str(count));
		std::string qry("tb_energyEcal_>>");
		qry.append(name);
		qry.append("(110, -1,10)");
		item.second->Draw(qry.c_str());

		TH1* plot = util_.formatHisto(name, item.first, "ECAL Activity (GeV)", util_.nextColor(), util_.nextColor(), 2);
		util_.accumulateObjects(plot, "");
		++count;
	}

	util_.addTitle("TB Analysis - ECAL activity");

	util_.flushPage();

}

void CompareTBAnalysis::doClusterMultiplicities() {

	TStyle* rStyle = util_.makeStyle("typeStyle");
	rStyle->SetOptLogy(false);
	rStyle->SetOptStat(11111);
	rStyle->SetOptFit(1);
	rStyle->SetOptLogz(false);
	rStyle->SetOptLogx(true);
	//Supresses TGraph errors in X
	rStyle->SetErrorX(0);
	rStyle->cd();

	util_.newPage();
	util_.addTitle("Cluster multiplicities");

	map<string, TTree*>::iterator it = trees_.begin();
	unsigned count(0);
	for (; it != trees_.end(); ++it) {
		pair<const string, TTree*> item = *it;
		string uniqueName("mult_");
		uniqueName.append(obj2str(count));
		string query("cluster_numHcal_:sim_energyEvent_>>");
		query.append(uniqueName);
		query.append("(8, 2, 10, 8, 0, 8)");
		item.second->Draw(query.c_str(), "cluster_numEcal_==0 && sim_energyEvent_ < 10", "colz");
		TH2* mult = util_.getType<TH2> (uniqueName);
		mult->SetTitle(item.first.c_str());
		util_.accumulateObjects(mult, "colz");
		++count;

		//20 upwards
		uniqueName = "mult_";
		uniqueName.append(obj2str(count));
		query = "cluster_numHcal_:sim_energyEvent_>>";
		query.append(uniqueName);
		query.append("(29, 20, 310, 8, 0, 8)");
		item.second->Draw(query.c_str(), "cluster_numEcal_==0 && sim_energyEvent_ > 10", "colz");
		TH2* mult2 = util_.getType<TH2> (uniqueName);
		mult2->SetTitle(item.first.c_str());
		util_.accumulateObjects(mult2, "colz");
		++count;

	}
	util_.flushPage();
}

void CompareTBAnalysis::plotTypeSpectrum() {
	TStyle* rStyle = util_.makeStyle("typeStyle");
	rStyle->SetOptLogy(false);
	rStyle->SetOptStat(11111);
	rStyle->SetOptFit(1);
	rStyle->SetOptLogz(false);
	rStyle->SetOptLogx(false);
	//Supresses TGraph errors in X
	rStyle->SetErrorX(0);
	rStyle->cd();

	util_.newPage();

	vector<Color_t> colorMap = util_.colorMap();
	const int categories(3);
	Int_t colors[categories] = { colorMap[2], colorMap[5], colorMap[6] };
	const char* labels[categories] = { "Just n^{0}", "Just #gamma", "n^{0} and #gamma" };
	util_.addTitle("TB Analysis - Type Spectrum");
	util_.addTitle(" ");
	util_.addTitle(" ");
	util_.addTitle("< 10 GeV");
	util_.addTitle("10 GeV <= E < 100 GeV");
	util_.addTitle("E >= 100 GeV");

	//First key is type
	//Second is energy range and number of particles
	map<string, map<string, int> > types;

	map<string, TTree*>::iterator it = trees_.begin();
	unsigned count(0);
	for (; it != trees_.end(); ++it) {
		pair<const string, TTree*> item = *it;

		std::string name("type");
		name.append("_");
		name.append(obj2str(count));
		std::string qry("cand_type_>>");
		qry.append(name);
		qry.append("(20, 0, 20)");

		const Long64_t had0s_lowE = item.second->Draw(qry.c_str(), "cand_type_ % 5 == 0 && sim_energyEvent_< 10");
		const Long64_t no_had0s_lowE = item.second->Draw(qry.c_str(), "cand_type_ % 4 == 0 && sim_energyEvent_< 10");
		const Long64_t mix_lowE = item.second->Draw(qry.c_str(),
				"cand_type_ % 4 != 0 && cand_type_ % 5 != 0 && sim_energyEvent_< 10");
		Double_t vals_lowE[categories] = { had0s_lowE, no_had0s_lowE, mix_lowE };
		string piName(name);
		piName.append("_low");

		cout << "\n*** LOW Type breakdown for:" << item.first << endl;
		cout << "\tJust hads\tJust phot\tBoth\n";
		cout << "\t" << had0s_lowE << "\t" << no_had0s_lowE << "\t" << mix_lowE << "\n";

		TPie* p_low = new TPie(name.c_str(), item.first.c_str(), categories, vals_lowE, colors, labels);
		p_low->SetAngularOffset(30);
		p_low->SetRadius(.35);
		p_low->SetLabelFormat("%txt (%frac)");
		util_.accumulateObjects(p_low, "3d");

		const Long64_t had0s_midE = item.second->Draw(qry.c_str(),
				"cand_type_ % 5 == 0 && sim_energyEvent_> 10 && sim_energyEvent_ < 100");
		const Long64_t no_had0s_midE = item.second->Draw(qry.c_str(),
				"cand_type_ % 4 == 0 && sim_energyEvent_> 10 && sim_energyEvent_ < 100");
		const Long64_t mix_midE = item.second->Draw(qry.c_str(),
				"cand_type_ % 4 != 0 && cand_type_ % 5 != 0 && sim_energyEvent_> 10 && sim_energyEvent_ < 100");
		Double_t vals_midE[categories] = { had0s_midE, no_had0s_midE, mix_midE };
		piName = name;
		piName.append("_mid");

		cout << "\n*** MID Type breakdown for:" << item.first << endl;
		cout << "\tJust hads\tJust phot\tBoth\n";
		cout << "\t" << had0s_midE << "\t" << no_had0s_midE << "\t" << mix_midE << "\n";

		TPie* p_mid = new TPie(piName.c_str(), item.first.c_str(), categories, vals_midE, colors, labels);
		p_mid->SetAngularOffset(30.);
		p_mid->SetRadius(.35);
		p_mid->SetLabelFormat("%txt (%frac)");
		util_.accumulateObjects(p_mid, "3d");

		const Long64_t had0s_highE = item.second->Draw(qry.c_str(), "cand_type_ % 5 == 0 && sim_energyEvent_ > 99");
		const Long64_t no_had0s_highE = item.second->Draw(qry.c_str(), "cand_type_ % 4 == 0 && sim_energyEvent_ > 99");
		const Long64_t mix_highE = item.second->Draw(qry.c_str(),
				"cand_type_ % 4 != 0 && cand_type_ % 5 != 0 && sim_energyEvent_ > 99");
		Double_t vals_highE[categories] = { had0s_highE, no_had0s_highE, mix_highE };
		piName = name;
		piName.append("_high");

		cout << "\n*** HIGH Type breakdown for:" << item.first << endl;
		cout << "\tJust hads\tJust phot\tBoth\n";
		cout << "\t" << had0s_highE << "\t" << no_had0s_highE << "\t" << mix_highE << "\n";

		TPie* p_high = new TPie(piName.c_str(), item.first.c_str(), categories, vals_highE, colors, labels);
		p_high->SetAngularOffset(30.);
		p_high->SetRadius(.35);

		p_high->SetLabelFormat("%txt (%frac)");

		util_.accumulateObjects(p_high, "3d");
		p_high->Draw("3d");
		gPad->Print("pie.C");

		++count;
	}

	util_.flushPage();

	util_.newPage();
	it = trees_.begin();

	map<string, TGraph*> effs;
	map<string, TGraph*> energyEffs;

	for (; it != trees_.end(); ++it) {

		pair<const string, TTree*> item = *it;
		string title("RecoEff");
		title.append(item.first);

		Comparator tbComp(item.first, item.second);
		vector<JGraph> eff_plots = tbComp.getRecoEfficiencyPlots(energies_, title.c_str(), "cand_type_", &util_,
				"cand_type_ % 4 != 0");

		TGraph* gEff = new TGraph(eff_plots[0].finalise());
		util_.formatGraph(gEff, item.first.c_str(), "E_{beam} (GeV)", "Events with n^{0}s/N", kCyan, 1);
		gEff->GetYaxis()->SetRangeUser(0, 1);
		effs[item.first] = gEff;

		TGraph* gEnergyEff = new TGraph(eff_plots[1].finalise());
		util_.formatGraph(gEnergyEff, item.first.c_str(), "E_{beam} (GeV)", "Energy n^{0}s/N", kCyan, 1);
		gEnergyEff->GetYaxis()->SetRangeUser(0, 1);
		energyEffs[item.first] = gEnergyEff;

	}

	rStyle->SetOptTitle(0);
	rStyle->SetOptLogx(true);

	std::string sampleTitle("Hadronic reconstruction efficiencies ");
	util_.addTitle(sampleTitle);

	TMultiGraph* tmg = getMultiGraph(effs, "Beam momentum (GeV/c)", "Fraction of events with n^{0}/N");

	util_.accumulateObjects(tmg, "CP");

	TMultiGraph* tmg2 = getMultiGraph(energyEffs, "Beam momentum (GeV/c)", "Fraction of energy associated with n^{0}/N");

	util_.accumulateObjects(tmg2, "CP");

	util_.flushPage();

}

void CompareTBAnalysis::doResponsePlots(std::string type, bool verbose, std::string qry, std::string cut) {

	TStyle* rStyle = util_.makeStyle("anotherStyle");
	rStyle->SetOptLogy(false);
	rStyle->SetOptStat(1);
	rStyle->SetOptFit(1);
	rStyle->SetOptLogz(false);
	rStyle->SetOptLogx(true);
	//Supresses TGraph errors in X
	rStyle->SetErrorX(0);
	rStyle->SetOptTitle(1);
	rStyle->cd();

	map<string, TGraph*> sampleMeans;
	map<string, TGraph*> gausMeans;
	map<string, TGraph*> gausRes;

	map<string, TTree*>::iterator it = trees_.begin();

	for (; it != trees_.end(); ++it) {

		pair<const string, TTree*> item = *it;
		string pageTitle(type);

		if (verbose) {
			util_.newPage();
			pageTitle.append(" response with: ");
			pageTitle.append(item.first);
			util_.addTitle(pageTitle);
		}
		Comparator tbComp(item.first, item.second);
		vector<JGraph> cands = tbComp.getRatioResponsePlots(energies_, type.c_str(), qry.c_str(), &util_, cut.c_str());

		TGraph* gGraphRes = new TGraph(cands[2].finalise());
		TGraph* gGraph = new TGraph(cands[0].finalise());
		TGraph* sGraph = new TGraph(cands[1].finalise());

		util_.formatGraph(gGraphRes, item.first.c_str(), "E_{beam} (GeV)", "#sigma/#mu", kCyan, 1);
		util_.formatGraph(gGraph, item.first.c_str(), "E_{beam}", "#mu/E_{beam}", kRed, 1);
		util_.formatGraph(sGraph, item.first.c_str(), "E_{beam} (GeV)", "<E>/E_{beam}", kBlue, 1);

		gGraphRes->GetXaxis()->SetRangeUser(2, 400);
		gGraph->GetXaxis()->SetRangeUser(2, 400);
		sGraph->GetXaxis()->SetRangeUser(2, 400);

		gGraphRes->GetYaxis()->SetRangeUser(0, 2);
		gGraph->GetYaxis()->SetRangeUser(0, 2);
		sGraph->GetYaxis()->SetRangeUser(0, 2);

		if (verbose) {
			util_.accumulateObjects(gGraphRes, "ALP");
			util_.accumulateObjects(gGraph, "ALP");
			util_.accumulateObjects(sGraph, "ALP");
			util_.flushPage();
		}

		sampleMeans[item.first] = sGraph;
		gausRes[item.first] = gGraphRes;
		gausMeans[item.first] = gGraph;

	}

	util_.newPage();

	rStyle->SetOptTitle(0);

	std::string sampleTitle("Sample means for ");
	sampleTitle.append(type);
	util_.addTitle(sampleTitle);
	TMultiGraph* tmg4 = getMultiGraph(sampleMeans, "Beam momentum (GeV/c)", "E_{reco}/E_{true} (GeV)");
	util_.accumulateObjects(tmg4, "LP");

	std::string fileName("name");
	if (type == "PF Candidates")
		fileName = "pfCandsIntEcalSampleMean";

	if (type == "TB Rechits")
		fileName = "tbRechitsIntEcalSampleMean";

	if (type == "PF Candidates ECAL MIP")
		fileName = "pfCandsMipEcalSampleMean";

	if (type == "TB Rechits ECAL MIP")
		fileName = "tbRechitsMipEcalSampleMean";

	util_.addTitle("(blank)");
	std::string gausTitle("Gaussian means for ");
	gausTitle.append(type);
	util_.addTitle(gausTitle);
	TMultiGraph* tmg5 = getMultiGraph(gausMeans, "Beam momentum (GeV/c)", "#mu_{reco}/E_{true} (GeV)");
	util_.accumulateObjects(tmg5, "LP");

	util_.addTitle("(blank)");
	std::string resTitle("Gaussian resolutions for ");
	resTitle.append(type);
	util_.addTitle(resTitle);
	TMultiGraph* tmg6 = getMultiGraph(gausRes, "Beam momentum (GeV/c)", "#sigma_{reco}/#mu_{reco} (GeV)");
	util_.accumulateObjects(tmg6, "LP");

	util_.flushPage();

	util_.accumulateSpecial(tmg4, rStyle, "LP", fileName);
	util_.flushSpecials("specials");
	util_.clearSpecials();

	cout << "Leaving " << __PRETTY_FUNCTION__ << endl;
}

TMultiGraph* CompareTBAnalysis::getMultiGraph(const map<string, TGraph*>& inputs, std::string xAxisTitle,
		std::string yAxisTitle) {
	TMultiGraph* tmg = new TMultiGraph();

	cout << __PRETTY_FUNCTION__ << endl;
	map<string, TGraph*>::const_iterator u = inputs.begin();

	vector<Color_t> colors = util_.colorMap();
	bool first(true);
	unsigned index(0);
	for (; u != inputs.end(); ++u) {
		cout << "\tDoing " << u->first << endl;
		pair<string, TGraph*> p = *u;
		TGraph* q = p.second;

		q->SetMarkerColor(colors[index % colors.size()]);
		q->GetXaxis()->SetTitle(xAxisTitle.c_str());
		q->GetYaxis()->SetTitle(yAxisTitle.c_str());
		if (first) {
			q->Draw("ALP");
			tmg->Add(q, "ALP");
			first = false;
		} else {
			q->Draw("LP");
			tmg->Add(q, "LP");
		}
		++index;
	}
	return tmg;
}

void CompareTBAnalysis::closeFiles() {
	std::string namecpy2(graphicsFile_);
	namecpy2.append("]");
	gPad->Print(namecpy2.c_str());

	std::string macrocpy2(macroFile_);
	macrocpy2.append("]");
	gPad->Print(macrocpy2.c_str());

	util_.flushSpecials(directory_);
}

std::vector<JGraph> Comparator::getRecoEfficiencyPlots(std::vector<int> energies, std::string name,
		std::string queryToTree, PlotUtil* util_, std::string additionalCut) {

	JGraph effGraph("effGraph");
	JGraph energyEffGraph("energyEffGraph");
	for (std::vector<int>::const_iterator i = energies.begin(); i != energies.end(); ++i) {

		int energy = *i;
		//build query
		std::string cut("int(sim_energyEvent_) == ");
		cut.append(obj2str(energy));
		string eCut(cut);
		if (additionalCut != "") {
			cut.append(" && ");
			cut.append(additionalCut);
		}
		std::string histoName(name);
		histoName.append(obj2str(energy));
		std::string qry(queryToTree);
		qry.append(">>");
		qry.append(histoName);

		Long64_t n_had = tree_->Draw(qry.c_str(), cut.c_str());
		Long64_t n_poss = tree_->Draw(qry.c_str(), eCut.c_str());
		Double_t efficiency = static_cast<double> (n_had) / n_poss;
		effGraph.addPoint(energy, efficiency);

		tree_->Draw("cand_energyNeutralHad_/cand_energyEvent_>>ht(101, 0, 1.01)", eCut.c_str());
		TH1* htemp = util_->getHisto("ht");
		if (htemp) {
			double mean_e_had = htemp->GetMean();
			energyEffGraph.addPoint(energy, mean_e_had);
		} else
			cout << __PRETTY_FUNCTION__ << ": l" << __LINE__ << ": Couldn't find ht plot?!" << endl;

	}
	vector<JGraph> answers;
	answers.push_back(effGraph);
	answers.push_back(energyEffGraph);
	return answers;

}

TH2* Comparator::getClusterMultiplicities(std::vector<int> energies, std::string name, std::string queryToTree,
		PlotUtil* util_, std::string additionalCut) {

	std::string histoName(name);
	std::string qry(queryToTree);
	qry.append(">>");
	qry.append(histoName);
	qry.append("(300, 1, 301, 8, 0, 1)");

	std::string cut(additionalCut);

	tree_->Draw(qry.c_str(), cut.c_str());
	TH2* histo = util_->getType<TH2> (histoName);

	return histo;

}

vector<JGraph> Comparator::getRatioResponsePlots(vector<int> energies, std::string name, std::string queryToTree,
		PlotUtil* util_, std::string additionalCut) {

	//	util_.newPage();

	JGraph gausGraph("gausGraph");
	JGraph sampleGraph("sampleGraph");
	JGraph gausGraphRes("gausGraphRes");

	//	TText* title = new TText(0, 0, "Response Plots; TB Rechits");
	//	title->SetNDC();
	//	title->SetX(0.1);
	//	title->SetY(0.9);
	//	util_.accumulateObjects(title, "");

	for (std::vector<int>::const_iterator i = energies.begin(); i != energies.end(); ++i) {

		int energy = *i;
		//build query
		std::string cut("int(sim_energyEvent_) == ");
		cut.append(obj2str(energy));
		if (additionalCut != "") {
			cut.append(" && ");
			cut.append(additionalCut);
		}
		//		std::string cut("cand_type_ == 5 && int(sim_energyEvent_) == ");
		//		cut.append(obj2str(energy));

		std::string histoName(name);
		histoName.append(obj2str(energy));
		bool fitOk(true);
		bool sampleOk(true);
		//		std::string qry("(tb_energyEvent_)/sim_energyEvent_>>");
		std::string qry(queryToTree);
		qry.append(">>");
		qry.append(histoName);
		qry.append("(170, -1.2, 2.2)");

		tree_->Draw(qry.c_str(), cut.c_str());
		TH1* histo = util_->getHisto(histoName);
		std::string beamE("E_{beam} = ");
		beamE.append(obj2str(energy));
		beamE.append(" GeV");

		util_->formatHisto(histoName, beamE, "#mu/E_{beam}", util_->nextColor(), kWhite, 2);
		double peakVal = histo->GetBinCenter(histo->GetMaximumBin());

		std::cout << "Fitting for energy " << energy << " with max at " << peakVal << " and RMS of " << histo->GetRMS()
				<< "\n";

		std::pair<double, double> gaus;
		if (name == "TB Rechits" || name == "TB Rechits ECAL MIP")
			gaus = util_->fitStabilisedGaussian(histo);
		else
			gaus = util_->fitStabilisedGaussian(histo, 0.1, 1.5);

		double gaussianess = fabs(gaus.first / histo->GetMean());
		if (gaussianess > 2 || gaussianess < 0.5) {
			fitOk = false;
			cout << "*** WARNING - gauss fit went kaputt for " << gaus << endl;
		}

		if (fitOk) {
			gausGraph.addPoint(energy, 0, gaus.first, gaus.second);
			gausGraphRes.addPoint(energy, gaus.second / gaus.first);
		}

		if (histo->GetMean() <= 0 || histo->GetEntries() == 0) {
			sampleOk = false;
			cout << "*** WARNING - sample mean is " << histo->GetMean() << " and entries = " << histo->GetEntries() << endl;
		}
		if (sampleOk) {
			sampleGraph.addPoint(energy, histo->GetMean());
		}

	}

	vector<JGraph> answers;
	answers.push_back(gausGraph);
	answers.push_back(sampleGraph);
	answers.push_back(gausGraphRes);
	return answers;
}

void compareTB() {
	gROOT->Reset();

	TFile* testbeamBarrel = TFile::Open("/tmp/ballin/PFlowTB_Tree_All_barrel_tbCalib.root");
	TTree* testbeamBarrelTree = (TTree*) testbeamBarrel->FindObjectAny("Extraction");

	TFile* testbeamEndcap = TFile::Open("/tmp/ballin/PFlowTB_Tree_All_endcap_tbCalib.root");
	TTree* testbeamEndcapTree = (TTree*) testbeamEndcap->FindObjectAny("Extraction");

	TFile* fastBarrel = TFile::Open("/tmp/ballin/Dipion_Tree_All_10k_barrel_noExcesses_fast_4T.root");
	TTree* fastBarrelTree = (TTree*) fastBarrel->FindObjectAny("Extraction");

	TFile* fastEndcap = TFile::Open("/tmp/ballin/Dipion_Tree_All_10k_endcap_noExcesses_fast_4T.root");
	TTree* fastEndcapTree = (TTree*) fastEndcap->FindObjectAny("Extraction");

	TFile* fullBarrel = TFile::Open("/tmp/ballin/Dipion_Tree_All_2k_barrel_noExcesses_full_4T.root");
	TTree* fullBarrelTree = (TTree*) fullBarrel->FindObjectAny("Extraction");

	TFile* fullEndcap = TFile::Open("/tmp/ballin/Dipion_Tree_All_2k_endcap_noExcesses_full_4T.root");
	TTree* fullEndcapTree = (TTree*) fullEndcap->FindObjectAny("Extraction");

	std::map<std::string, TTree*> source;

	std::cout << "Testbeam barrel tree is " << testbeamBarrelTree << endl;
	std::cout << "Testbeam endcap tree is " << testbeamEndcapTree << endl;
	std::cout << "Fast barrel tree is " << fastBarrelTree << endl;
	std::cout << "Fast endcap tree is " << fastEndcapTree << endl;
	std::cout << "Full barrel tree is " << fullBarrelTree << endl;
	std::cout << "Full endcap tree is " << fullEndcapTree << endl;


	source["TB Barrel"] = testbeamBarrelTree;
//	source["TB Endcap"] = testbeamEndcapTree;
	source["Fast Barrel"] = fastBarrelTree;
//	source["Fast Endcap"] = fastEndcapTree;
	source["Full Barrel"] = fullBarrelTree;
//	source["Full Endcap"] = fullEndcapTree;


	std::vector<int> energies;
	//	energies.push_back(2);
	//	energies.push_back(3);
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
//	energies.push_back(150);
	//	energies.push_back(200);
//	energies.push_back(300);

	CompareTBAnalysis ctba(source, energies, "ctba.ps", "ctba.C", "plots/ctba");

	ctba.evaluatePlots();

	ctba.closeFiles();

}

