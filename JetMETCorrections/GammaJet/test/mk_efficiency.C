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

  cafe::Config *cfg = new cafe::Config("tree");
  string algo = cfg->get("Algorithm", "ite");
  delete cfg;
  cout << "Running mk_efficiency.C with algorithm " << algo << endl;
  cout << "To run in the background without terminal, use 'nohup'" << endl;

  // PROCESS GAMMA+JET
  // Set pT hat bin cross sections, needed for event weights
  //map<string, string> signal; // CINT seems to have issues with this
  map<string, int> signal; // => work-around
  map<string, double> signal_xsec;
  map<string, double> signal_start;
  map<string, double> signal_limit;
//   signal["15"] = 15;
//   signal["30"] = 30;
//   signal["80"] = 80;
//   signal["170"] = 170;
//   signal["300"] = 300;
//   signal["470"] = 470;
//   signal["800"] = 800;
//   signal["1400"] = 1400;
  // Cross sections per file listed in
  // https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2008#Pythia6
  // original units in mb, change to pb
  // NB: not yet corrected for imposing an upper limit
  signal_xsec["15"] = 2.887E+05;//E-04;
  signal_xsec["30"] = 3.222E+04;//E-05;
  signal_xsec["80"] = 1.010E+03;//E-06;
  signal_xsec["170"] = 5.143E+01;//E-08;
  signal_xsec["300"] = 4.193E+00;//E-09;
  signal_xsec["470"] = 4.515E-01;//E-10;
  signal_xsec["800"] = 2.003E-02;//E-11;
  signal_xsec["1400"] = 2.686E-04;//E-13;
 
  // Manual mapping of the starting value
  signal_start["15"] = 15.;
  signal_start["30"] = 30.;
  signal_start["80"] = 80.;
  signal_start["170"] = 170.;
  signal_start["300"] = 300.;
  signal_start["470"] = 470.;
  signal_start["800"] = 800.;
  signal_start["1400"] = 1400.;

  // Manual mapping of the upper limit
  signal_limit["15"] = 30.;
  signal_limit["30"] = 80.;
  signal_limit["80"] = 170.;
  signal_limit["170"] = 300.;//230.;
  signal_limit["300"] = 470.;//380.;
  signal_limit["470"] = 800.;
  signal_limit["800"] = 1400.;
  signal_limit["1400"] = 14000.;

  // Remove old output file list
  gROOT->ProcessLine(Form(".! rm output_PhotonJets_%s.txt", algo.c_str()));
  gROOT->ProcessLine(Form(".! mkdir output_PhotonJets_%s", algo.c_str()));
  for (map<string, int>::const_iterator it = signal.begin();
       it != signal.end(); ++it) {

    // Create temporary file list for given pT hat bin only
    gROOT->ProcessLine(Form(".! grep EtGamma_%d_summer08 inputfiles_summer08_PhotonJets_new.txt > inputfiles_tmp.txt", it->second));
    // Produce a root tuple for the pT hat bin
    gjetefficiency *sig = new gjetefficiency(getchain("inputfiles_tmp.txt"), Form("output_fix_PhotonJets_%s/output_PhotonJets_%s.root", algo.c_str(),it->first));
    sig->Loop(signal_xsec[Form("%d",it->second)],signal_start[it->first],
	      signal_limit[it->first],true);
    // Add new root tuple to output file list
    gROOT->ProcessLine(Form(".! echo \"output_fix_PhotonJets_%s/output_PhotonJets_%s.root\" >> output_PhotonJets_%s.txt", algo.c_str(), it->first, algo.c_str()));
    delete sig;
  } // for signal


  // PROCESS QCD
  // Set pT hat bin cross sections, needed for event weights
  map<string, int> qcd; // => work-around
  map<string, double> qcd_xsec;
  map<string, double> qcd_start;
  map<string, double> qcd_limit;
  // *** Summer08 ***
  // https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2008#Pythia6
  // units in pb
  qcd["30"] = 30;
  qcd["80"] = 80;
//   qcd["170"] = 170;
  qcd["300"] = 300;
  qcd["470"] = 470;
  qcd["800"] = 800;
//   qcd["1400"] = 1400;

  qcd_xsec["30"] = 109057220.4;
  qcd_xsec["80"] = 1934639.567;
  qcd_xsec["170"] = 62562.87713;
  qcd_xsec["300"] = 3664.608301;
  qcd_xsec["470"] = 315.5131272;
  qcd_xsec["800"] = 11.9419745;
  qcd_xsec["1400"] = 0.1720187189;

  qcd_start["30"] = 30.;
  qcd_start["80"] = 80.;
  qcd_start["170"] = 170.;
  qcd_start["300"] = 300.;
  qcd_start["470"] = 470.;
  qcd_start["800"] = 800.;
  qcd_start["1400"] = 1400.;

  qcd_limit["30"] = 80.;
  qcd_limit["80"] = 170.;
  qcd_limit["170"] = 300.;
  qcd_limit["300"] = 4700.;
  qcd_limit["470"] = 800.;
  qcd_limit["800"] = 1400.;
  qcd_limit["1400"] = 14000.;

  // Remove old output file list
  gROOT->ProcessLine(Form(".! rm output_QCD_%s.txt", algo.c_str()));
  gROOT->ProcessLine(Form(".! mkdir output_QCD_%s", algo.c_str()));
  for (map<string, int>::const_iterator it = qcd.begin();
       it != qcd.end(); ++it) {

    // Create temporary file list for given pT hat bin only
    gROOT->ProcessLine(Form(".! grep QCD_%d_summer08 inputfiles_summer08_QCD_new.txt > inputfiles_tmp.txt", it->second));
    // Produce a root tuple for the pT hat bin
    gjetefficiency *bkg =new gjetefficiency(getchain("inputfiles_tmp.txt"), Form("output_fix_QCD_%s/output_QCD_%s.root",  algo.c_str(), it->first));
    bkg->Loop(qcd_xsec[Form("%d",it->second)], qcd_start[it->first],
	      qcd_limit[it->first], false);
    // Add new root tuple to output file list
    gROOT->ProcessLine(Form(".! echo \"output_fix_QCD_%s/output_QCD_%s.root\" >> output_QCD_%s.txt", algo.c_str(), it->first, algo.c_str()));
    delete bkg;
  } // for qcd


  // Merge PhotonJets and QCD lists
  gROOT->ProcessLine(Form(".! cat output_PhotonJets_%s.txt > output_mixed_%s.txt", algo.c_str(), algo.c_str()));
  gROOT->ProcessLine(Form(".! cat output_QCD_%s.txt >> output_mixed_%s.txt", algo.c_str(), algo.c_str()));
}
