{

#include <vector>
#include <string>
#include <pair>

using namespace std;

gROOT->ProcessLine(".L plots.C+");

string dir = ".";
vector<string> files1;
vector<string> files2;
files1.push_back("output_PhotonJets/output_PhotonJets_30_50.root");
files2.push_back("output_QCD/output_QCD_30_50.root");
files1.push_back("output_PhotonJets/output_PhotonJets_80_120.root");
files2.push_back("output_QCD/output_QCD_80_120.root");
files1.push_back("output_PhotonJets/output_PhotonJets_300_500.root");
files2.push_back("output_QCD/output_QCD_300_380.root");

gROOT->ProcessLine(".! mkdir eps");

assert(files2.size()>=files1.size());
string bin = "";
for (unsigned int i = 0; i != files1.size(); ++i) {

  if (i==0) bin = "pt30/";
  if (i==1) bin = "pt80/";
  if (i==2) bin = "pt300/";

  gROOT->ProcessLine(Form(".! mkdir eps/%s",bin.c_str()));
  gROOT->ProcessLine(Form(".! mkdir eps/%s/basic",bin.c_str()));
  gROOT->ProcessLine(Form(".! mkdir eps/%s/basicid",bin.c_str()));  

  plots((dir+"/"+files1[i]), (dir+"/"+files2[i]), "EkinEM", "EkinSB",
	"eps/"+bin+"basic", true);//false);
  plots((dir+"/"+files1[i]), (dir+"/"+files2[i]), "EkinID", "EkinID",
	"eps/"+bin+"basicid", true);//false);
}

/*
gROOT->ProcessLine(".! cd tex");
gROOT->ProcessLine(".! latex plots.tex");
gROOT->ProcessLine(".! dvipdf plots.dvi");
gROOT->ProcessLine(".! cd ..");
*/
}
