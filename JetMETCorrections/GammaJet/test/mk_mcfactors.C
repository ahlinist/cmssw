{

#include <vector>
#include <string>
//#include <exception> // For Error: Symbol G__exception is not defined in current scope

using namespace std;

vector<string> files1;
// *** Summer08 ***
// *** Summer09 OctX only up to 170 ***
files1.push_back("output_PhotonJets_15.root");
files1.push_back("output_PhotonJets_20.root"); //new
files1.push_back("output_PhotonJets_30.root");
files1.push_back("output_PhotonJets_50.root");
files1.push_back("output_PhotonJets_80.root");
files1.push_back("output_PhotonJets_120.root");
files1.push_back("output_PhotonJets_170.root");
//files1.push_back("output_PhotonJets_300.root");
//files1.push_back("output_PhotonJets_470.root");
//files1.push_back("output_PhotonJets_800.root");
//files1.push_back("output_PhotonJets_1400.root");
vector<string> files2;
// *** Summer08 ***
// *** Summer09 OctX only up to 170 ***
files2.push_back("output_QCD_15.root");
files2.push_back("output_QCD_20.root"); //new
files2.push_back("output_QCD_30.root");
files2.push_back("output_QCD_50.root"); //
files2.push_back("output_QCD_80.root");
files2.push_back("output_QCD_120.root"); //
//files2.push_back("output_QCD_170.root");
//files2.push_back("output_QCD_230.root"); //
//files2.push_back("output_QCD_300.root");
//files2.push_back("output_QCD_380.root"); //
//files2.push_back("output_QCD_470.root");
//files2.push_back("output_QCD_600.root"); //
//files2.push_back("output_QCD_800.root");
//files2.push_back("output_QCD_1000.root"); //
//files2.push_back("output_QCD_1400.root");

//files2.push_back("output_EM_30.root");
//files2.push_back("output_EM_50.root");
//files2.push_back("output_EM_80.root");
//files2.push_back("output_EM_120.root");


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

string dir1 = Form("output_QCD_%s",algo.c_str());
string dir2 = Form("output_EM_%s",algo.c_str());
for (unsigned int i = 0; i != files2.size(); ++i) {
  if (TString(files2[i].c_str()).Contains("_EM_"))
    files2[i] = dir2+"/"+files2[i];
  else
    files2[i] = dir1+"/"+files2[i];
 }


/*
mcfactors(files1, files2, "loose");
gROOT->ProcessLine(Form(".! mkdir eps/mcfactors_loose_%s",algo.c_str()));
gROOT->ProcessLine(Form(".! mv eps/mcfactors*.eps eps/mcfactors_loose_%s",
			algo.c_str()));
*/

//fractiontest();

mcfactors(files1, files2, "medium", false);
mcfactors(files1, files2, "medium", true);
gROOT->ProcessLine(Form(".! mkdir eps/mcfactors_medium_%s",algo.c_str()));
gROOT->ProcessLine(Form(".! mv eps/mcfactors*.eps eps/mcfactors_medium_%s",
			algo.c_str()));


/*
mcfactors(files1, files2, "tight");
gROOT->ProcessLine(Form(".! mkdir eps/mcfactors_tight_%s",algo.c_str()));
gROOT->ProcessLine(Form(".! mv eps/mcfactors*.eps eps/mcfactors_tight_%s",
			algo.c_str()));
*/
//mcfactors(files1, files2, "NN");
//gROOT->ProcessLine(Form(".! mkdir eps/mcfactors_nn_%s",algo.c_str()));
//gROOT->ProcessLine(Form(".! mv eps/mcfactors*.eps eps/mcfactors_nn_%s",
//			algo.c_str()));

/*
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
*/
/*
mcfactors(files1, files2, "loose");
gROOT->ProcessLine(Form(".! mkdir eps/mcfactors_loose_%s",algo.c_str()));
gROOT->ProcessLine(Form(".! mv eps/mcfactors*.eps eps/mcfactors_loose_%s",
			algo.c_str()));
*/
}
