{

#include "TROOT.h"

#include <vector>
#include <string>
#include <pair>

using namespace std;

gROOT->ProcessLine(".L plots.C+");

string algo = "ite";

string dirs = Form("output_PhotonJets_%s", algo.c_str());
string dirb = Form("output_QCD_%s", algo.c_str());
vector<string> files1;
vector<string> files2;
files1.push_back("output_PhotonJets_30.root");//_50.root");
files2.push_back("output_QCD_30.root");//_50.root");
files1.push_back("output_PhotonJets_80.root");//_120.root");
files2.push_back("output_QCD_80.root");//_120.root");
files1.push_back("output_PhotonJets_300.root");//_500.root");
files2.push_back("output_QCD_300.root");//_380.root");

gROOT->ProcessLine(".! mkdir eps");

assert(files2.size()>=files1.size());
string bin = "";
for (unsigned int i = 0; i != files1.size(); ++i) {

  if (i==0) bin = Form("pt30_%s/", algo.c_str());
  if (i==1) bin = Form("pt80_%s/", algo.c_str());
  if (i==2) bin = Form("pt300_%s/", algo.c_str());

  gROOT->ProcessLine(Form(".! mkdir eps/%s", bin.c_str()));
  gROOT->ProcessLine(Form(".! mkdir eps/%s/basic", bin.c_str()));
  gROOT->ProcessLine(Form(".! mkdir eps/%s/basicid", bin.c_str()));  

  plots((dirs+"/"+files1[i]), (dirb+"/"+files2[i]), "EkinEM", "EkinSB",
	"eps/"+bin+"basic", true);//false);
  plots((dirs+"/"+files1[i]), (dirb+"/"+files2[i]), "EkinID", "EkinID",
	"eps/"+bin+"basicid", true);//false);
}

/*
gROOT->ProcessLine(".! cd tex");
gROOT->ProcessLine(".! latex plots.tex");
gROOT->ProcessLine(".! dvipdf plots.dvi");
gROOT->ProcessLine(".! cd ..");
*/
}
