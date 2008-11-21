//#include <map>
//#include <string>

//using namespace std;

//void mk_efficiency() {
{

// List of signal bins and their cross sections
#include <map>
#include <string>

  using namespace std;

  // TO CREATE REDUCED TREE TO FINALIZE THE ANALYSIS
  // -----------------------------------------------
  gROOT->ProcessLine(".L Config.cpp++");
  gROOT->ProcessLine(".L gjetefficiency.C++");

  gSystem->Setenv("CAFE_CONFIG", "gjetefficiency.config");


  // PROCESS GAMMA+JET
  // Set pT hat bin cross sections, needed for event weights
  map<string, double> signal;
  //signal["0_15"] = 1.6e6; // what's the xsect?
  //signal["15_20"] = 2.5e5;// what's the xsect?
  //signal["20_30"] = 
  signal["30_50"] = 41140.;
  signal["50_80"] = 7210.;
  signal["80_120"] = 1307.;
  signal["120_170"] = 276.;
  signal["170_300"] = 87.1;
  signal["300_500"] = 8.29;
  signal["500_7000"] = 0.878;

  // Remove old output file list
  gROOT->ProcessLine(".! rm output_PhotonJets_all.txt");
  gROOT->ProcessLine(".! mkdir output_PhotonJets_eff");
  for (map<string, double>::iterator it = signal.begin();
       it != signal.end(); ++it) {

    // Create temporary file list for given pT hat bin only
    gROOT->ProcessLine(Form(".! grep EtGamma_%s inputfiles_PhotonJets_delre.txt > inputfiles_tmp.txt", it->first));
    // Produce a root tuple for the pT hat bin
    gjetefficiency *sig = new gjetefficiency(getchain("inputfiles_tmp.txt"), Form("output_PhotonJets_eff/output_PhotonJets_%s.root", it->first));
    sig->Loop(it->second, true);
    // Add new root tuple to output file list
    gROOT->ProcessLine(Form(".! echo \"output_PhotonJets_eff/output_PhotonJets_%s.root\" >> output_PhotonJets_all.txt", it->first));
    delete sig;
  } // for signal


  // PROCESS QCD
  // Set pT hat bin cross sections, needed for event weights
  map<string, double> qcd;
  qcd["30_50"] = 163000000.;
  qcd["50_80"]  =  21600000.;
  qcd["80_120"] =   3080000.;
  qcd["120_170"]  =    494000.;
  qcd["170_230"]  =    101000.;
  qcd["230_300"]  =     24500.;
  qcd["300_380"]  =      6240.;
  qcd["380_470"]  =      1780.;
  qcd["470_600"]  =       683.;
  qcd["600_800"]  =       204.;
  qcd["800_1000"] =       35.1;
  qcd["1000_1400"]=       10.9;
  qcd["1400_1800"]=       1.06;
  qcd["1800_2200"]=       0.145;
  qcd["2200_2600"]=       0.0238;
  qcd["2600_3000"]=       0.00429;
  qcd["3000_3500"]=       0.000844;
  qcd["3500_inf"] =       0.000108;

  // Remove old output file list
  gROOT->ProcessLine(".! rm output_QCD_all.txt");
  gROOT->ProcessLine(".! mkdir output_QCD_eff");
  for (map<string, double>::iterator it = qcd.begin();
       it != qcd.end(); ++it) {

    // Create temporary file list for given pT hat bin only
    gROOT->ProcessLine(Form(".! grep QCD_Pt_%s inputfiles_QCD_delre.txt > inputfiles_tmp.txt", it->first));
    // Produce a root tuple for the pT hat bin
    gjetefficiency *bkg =new gjetefficiency(getchain("inputfiles_tmp.txt"), Form("output_QCD_eff/output_QCD_%s.root", it->first));
    bkg->Loop(it->second, false,200000);
    // Add new root tuple to output file list
    gROOT->ProcessLine(Form(".! echo \"output_QCD_eff/output_QCD_%s.root\" >> output_QCD_all.txt", it->first));
    delete bkg;
  } // for qcd


  // Merge PhotonJets and QCD lists
  gROOT->ProcessLine(".! cat output_PhotonJets_all.txt > output_mixed_all.txt");
  gROOT->ProcessLine(".! cat output_QCD_all.txt >> output_mixed_all.txt");
}
