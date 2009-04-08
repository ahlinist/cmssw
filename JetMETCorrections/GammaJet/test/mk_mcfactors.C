{

#include <vector>
#include <string>
//#include <exception> // For Error: Symbol G__exception is not defined in current scope

using namespace std;

vector<string> files1;
// *** Summer08 ***
files1.push_back("output_PhotonJets_15.root");
files1.push_back("output_PhotonJets_30.root");
files1.push_back("output_PhotonJets_50.root"); //
files1.push_back("output_PhotonJets_80.root");
files1.push_back("output_PhotonJets_120.root"); //
files1.push_back("output_PhotonJets_170.root");
//files1.push_back("output_PhotonJets_230.root"); //
files1.push_back("output_PhotonJets_300.root");
//files1.push_back("output_PhotonJets_380.root"); //
files1.push_back("output_PhotonJets_470.root");
files1.push_back("output_PhotonJets_800.root");
files1.push_back("output_PhotonJets_1400.root");
vector<string> files2;
// *** Summer08 ***
files2.push_back("output_QCD_30.root");
files2.push_back("output_QCD_50.root"); //
files2.push_back("output_QCD_80.root");
files2.push_back("output_QCD_120.root"); //
files2.push_back("output_QCD_170.root");
files2.push_back("output_QCD_230.root"); //
files2.push_back("output_QCD_300.root");
files2.push_back("output_QCD_380.root"); //
files2.push_back("output_QCD_470.root");
files2.push_back("output_QCD_600.root"); //
files2.push_back("output_QCD_800.root");
files2.push_back("output_QCD_1000.root"); //
files2.push_back("output_QCD_1400.root");

gROOT->ProcessLine(".L Config.cpp+");
gROOT->ProcessLine(".L mcfactors.C+");
gROOT->ProcessLine(".x cms_jes_style.C");

gSystem->Setenv("CAFE_CONFIG", "gjettree.config");
gROOT->ProcessLine(".! mkdir eps");

cafe::Config *cfg = new cafe::Config("tree");
string algo = cfg->get("Algorithm", "ite");
delete cfg;
cout << "Running mk_mcfactors.C with algorithm " << algo << endl;

string dir = Form("output_PhotonJets_%s",algo.c_str());
for (unsigned int i = 0; i != files1.size(); ++i)
  files1[i] = dir+"/"+files1[i];

string dir = Form("output_QCD_%s",algo.c_str());
for (unsigned int i = 0; i != files2.size(); ++i)
  files2[i] = dir+"/"+files2[i];

/*
mcfactors(files1, files2, "loose");
gROOT->ProcessLine(Form(".! mkdir eps/mcfactors_loose_%s",algo.c_str()));
gROOT->ProcessLine(Form(".! mv eps/mcfactors*.eps eps/mcfactors_loose_%s",
			algo.c_str()));

mcfactors(files1, files2, "medium");
gROOT->ProcessLine(Form(".! mkdir eps/mcfactors_medium_%s",algo.c_str()));
gROOT->ProcessLine(Form(".! mv eps/mcfactors*.eps eps/mcfactors_medium_%s",
			algo.c_str()));

mcfactors(files1, files2, "tight");
gROOT->ProcessLine(Form(".! mkdir eps/mcfactors_tight_%s",algo.c_str()));
gROOT->ProcessLine(Form(".! mv eps/mcfactors*.eps eps/mcfactors_tight_%s",
			algo.c_str()));

mcfactors(files1, files2, "NN");
gROOT->ProcessLine(Form(".! mkdir eps/mcfactors_nn_%s",algo.c_str()));
gROOT->ProcessLine(Form(".! mv eps/mcfactors*.eps eps/mcfactors_nn_%s",
			algo.c_str()));
*/
mcfactors(files1, files2, "medium05");
gROOT->ProcessLine(Form(".! mkdir eps/mcfactors_medium05_%s",algo.c_str()));
gROOT->ProcessLine(Form(".! mv eps/mcfactors*.eps eps/mcfactors_medium05_%s",
			algo.c_str()));

mcfactors(files1, files2, "medium10");
gROOT->ProcessLine(Form(".! mkdir eps/mcfactors_medium10_%s",algo.c_str()));
gROOT->ProcessLine(Form(".! mv eps/mcfactors*.eps eps/mcfactors_medium10_%s",
			algo.c_str()));

mcfactors(files1, files2, "medium20");
gROOT->ProcessLine(Form(".! mkdir eps/mcfactors_medium20_%s",algo.c_str()));
gROOT->ProcessLine(Form(".! mv eps/mcfactors*.eps eps/mcfactors_medium20_%s",
			algo.c_str()));

/*
mcfactors(files1, files2, "loose");
gROOT->ProcessLine(Form(".! mkdir eps/mcfactors_loose_%s",algo.c_str()));
gROOT->ProcessLine(Form(".! mv eps/mcfactors*.eps eps/mcfactors_loose_%s",
			algo.c_str()));
*/
}
