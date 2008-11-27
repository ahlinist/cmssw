{

#include <vector>
#include <string>

using namespace std;

string dir = ".";
vector<string> files1;
files1.push_back("output_PhotonJets/output_PhotonJets_30_50.root");
files1.push_back("output_PhotonJets/output_PhotonJets_50_80.root");
files1.push_back("output_PhotonJets/output_PhotonJets_80_120.root");
files1.push_back("output_PhotonJets/output_PhotonJets_120_170.root");
files1.push_back("output_PhotonJets/output_PhotonJets_170_300.root");
files1.push_back("output_PhotonJets/output_PhotonJets_300_500.root");
//
files1.push_back("output_PhotonJets/output_PhotonJets_500_7000.root");
vector<string> files2;
//files2.push_back("output_QCD/output_QCD_30_50.root");
files2.push_back("output_QCD/output_QCD_50_80.root");
files2.push_back("output_QCD/output_QCD_80_120.root");
files2.push_back("output_QCD/output_QCD_120_170.root");
files2.push_back("output_QCD/output_QCD_170_230.root");
files2.push_back("output_QCD/output_QCD_230_300.root");
files2.push_back("output_QCD/output_QCD_300_380.root");
files2.push_back("output_QCD/output_QCD_380_470.root");
files2.push_back("output_QCD/output_QCD_470_600.root");
//
files2.push_back("output_QCD/output_QCD_600_800.root");
files2.push_back("output_QCD/output_QCD_800_1000.root");
files2.push_back("output_QCD/output_QCD_1000_1400.root");
files2.push_back("output_QCD/output_QCD_1400_1800.root");
files2.push_back("output_QCD/output_QCD_1800_2200.root");
files2.push_back("output_QCD/output_QCD_1800_2200.root");
//files2.push_back("output_QCD/output_QCD_2200_2600.root");
//files2.push_back("output_QCD/output_QCD_2600_3000.root");
//files2.push_back("output_QCD/output_QCD_3000_3500.root");
//files2.push_back("output_QCD/output_QCD_3500_inf.root");

for (unsigned int i = 0; i != files1.size(); ++i)
  files1[i] = dir+"/"+files1[i];
for (unsigned int i = 0; i != files2.size(); ++i)
  files2[i] = dir+"/"+files2[i];

gROOT->ProcessLine(".L Config.cpp+");
gROOT->ProcessLine(".L mcfactors.C+");
gROOT->ProcessLine(".x cms_jes_style.C");

gSystem->Setenv("CAFE_CONFIG", "gjettree.config");
gROOT->ProcessLine(".! mkdir eps");

/*
mcfactors(files1, files2, "loose");
gROOT->ProcessLine(".! mkdir eps/mcfactors_loose");
gROOT->ProcessLine(".! mv eps/mcfactors*.eps eps/mcfactors_loose");

mcfactors(files1, files2, "medium");
gROOT->ProcessLine(".! mkdir eps/mcfactors_medium");
gROOT->ProcessLine(".! mv eps/mcfactors*.eps eps/mcfactors_medium");

mcfactors(files1, files2, "tight");
gROOT->ProcessLine(".! mkdir eps/mcfactors_tight");
gROOT->ProcessLine(".! mv eps/mcfactors*.eps eps/mcfactors_tight");

mcfactors(files1, files2, "NN");
gROOT->ProcessLine(".! mkdir eps/mcfactors_nn");
gROOT->ProcessLine(".! mv eps/mcfactors*.eps eps/mcfactors_nn");
*/

mcfactors(files1, files2, "medium");
gROOT->ProcessLine(".! mkdir eps/mcfactors_medium");
gROOT->ProcessLine(".! mv eps/mcfactors*.eps eps/mcfactors_medium");

}
