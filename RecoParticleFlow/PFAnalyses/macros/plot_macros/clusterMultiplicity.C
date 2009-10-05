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

template<typename T, typename U> std::map<U, T> converse_map(const std::map<T,
		U>& o) {
	map<U, T> result;
	for (typename map<T, U>::const_iterator begin(o.begin()); begin != o.end(); ++begin)
		result.insert(make_pair(begin->second, begin->first));

	return result;

}

template<typename K, typename R> std::ostream& operator<<(std::ostream& s,
		const std::pair<K, R>& p) {
	s << "(" << p.first << ", " << p.second << ")";
	return s;
}

class ClusterMultiplicity {
public:
	ClusterMultiplicity() {
	}

	ClusterMultiplicity(std::map<std::string, TTree*> trees,
			vector<int> energies, std::string graphicsFile,
			std::string macroFile, std::string directory_);

	virtual ~ClusterMultiplicity() {
	}

	void reset(std::map<std::string, TTree*> t, std::vector<int> energies,
			std::string graphicsFile, std::string macroFile);

	void evaluatePlots();

	void closeFiles();

private:

	std::map<std::string, TTree*> trees_;
	std::vector<int> energies_;
	TStyle* defaultStyle_;
	PlotUtil util_;

	std::string graphicsFile_;
	std::string macroFile_;

	std::string directory_;
};

void clusterMulitplicity() {
	gROOT->Reset();

	/*
	 * rfcp /castor/cern.ch/user/b/ballin/tbv8/Dikaon_allGeV_2k_full.root /tmp
	 * rfcp /castor/cern.ch/user/b/ballin/tb310pre8/outputtree_allGeV_notracks.root /tmp
	 * rfcp /castor/cern.ch/user/b/ballin/tb310pre10/Dikaon_allGeV_10k_fast.root /tmp
	 */
	TFile* testbeam = TFile::Open("/tmp/outputtree_allGeV_notracks.root");
	TTree* testbeamTree = (TTree*) testbeam->FindObjectAny("Extraction");

	std::map<std::string, TTree*> source;
	std::cout << "Testbeam tree is " << testbeamTree << endl;

	source["Testbeam 2006 (Barrel)"] = testbeamTree;


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
	energies.push_back(200);
	//	energies.push_back(300);

	ClusterMultiplicity cm(source, energies, "cm.ps", "cm.C", "cm");

	cm.evaluatePlots();

	cm.closeFiles();

}

ClusterMultiplicity::ClusterMultiplicity(map<string, TTree*> trees,
		std::vector<int> energies, std::string graphicsFile,
		std::string macroFile, std::string directory) :
	directory_(directory) {

	util_.init();
	util_.enableAutoFlush(true, 9);
	util_.setSquareCanvasDimension(3);
	reset(trees, energies, graphicsFile, macroFile);

}

void ClusterMultiplicity::reset(map<string, TTree*> trees,
		std::vector<int> energies, std::string graphicsFile,
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

void ClusterMultiplicity::evaluatePlots() {


}


void ClusterMultiplicity::closeFiles() {
	std::string namecpy2(graphicsFile_);
	namecpy2.append("]");
	gPad->Print(namecpy2.c_str());

	std::string macrocpy2(macroFile_);
	macrocpy2.append("]");
	gPad->Print(macrocpy2.c_str());

	util_.flushSpecials(directory_);
}

