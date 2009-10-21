#include "RecoParticleFlow/PFAnalyses/interface/PlotUtil.h"
#include <TH1.h>
#include <TFile.h>
#include <TTree.h>
#include <string>

void plotTemplate() {

	using namespace std;

	TFile::Open("../test/PFlowTB_Tree_300GeV_endcap_noise.root");

	TTree* tree = (TTree*) gDirectory->FindObjectAny("Extraction");

	PlotUtil util_;


}
