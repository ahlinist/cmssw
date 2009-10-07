//#include <map>
//#include <string>

//using namespace std;

//void mk_reducedTree() {
{

// List of signal bins and their cross sections
#include <map>
#include <string>

  using namespace std;

  // TO CREATE REDUCED TREE TO FINALIZE THE ANALYSIS
  // -----------------------------------------------
  gROOT->ProcessLine(".L Config.cpp+");
  gROOT->ProcessLine(".L gjettree.C+");

  gSystem->Setenv("CAFE_CONFIG", "gjettree.config");

  cafe::Config *cfg = new cafe::Config("tree");
  string algo = cfg->get("Algorithm", "ite");
  delete cfg;
  cout << "Running mk_reducedTree.C with algorithm " << algo << endl;
  cout << "To run in the background without terminal, use 'nohup'" << endl;

  
  // PROCESS GAMMA+JET
  // Set pT hat bin cross sections, needed for event weights
  //map<string, string> signal; // CINT seems to have issues with this
  map<string, int> signal; // => work-around
  map<string, double> signal_xsec;
  map<string, double> signal_start;
  map<string, double> signal_limit;
  // *** Summer08 ***
  // Break wide pThat bins into smaller ones for mc factorization
  // This is a manual mapping from narrow bins to original files
  //signal["15"] = 15;
  //signal["30"] = 30;
  //signal["50"] = 30;
  //signal["80"] = 80;
  //signal["120"] = 80;
  //signal["170"] = 170;
  //signal["300"] = 300;
  //signal["470"] = 470;
  //signal["800"] = 800;
  //signal["1400"] = 1400;
  // Summer09 match
  //signal["500"] = 470;
  // *** Summer09 pre-V3 ***
  //signal["0"] = 0;
  //signal["500"] = 500;
  // *** Summer09 OctX ***
  signal["15"] = 20;
  signal["20"] = 20; // new
  signal["30"] = 20;
  signal["50"] = 20;
  signal["80"] = 20;
  signal["120"] = 20;
  signal["170"] = 20;

  // Cross sections per file listed in
  // https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2008#Pythia6
  // original units in mb, change to pb
  // NB: not yet corrected for imposing an upper limit
  // *** Summer08 ***
  //signal_xsec["15"] = 2.887E+05;//E-04;
  //signal_xsec["30"] = 3.222E+04;//E-05;
  //signal_xsec["80"] = 1.010E+03;//E-06;
  //signal_xsec["170"] = 5.143E+01;//E-08;
  //signal_xsec["300"] = 4.193E+00;//E-09;
  //signal_xsec["470"] = 4.515E-01;//E-10;
  //signal_xsec["800"] = 2.003E-02;//E-11;
  //signal_xsec["1400"] = 2.686E-04;//E-13;
  // *** Summer09 OctX ***
  signal_xsec["20"] = 2.887E+05;//E-04; // same as Summer08 "15"
  // Correct for imposing an upper limit on pthat
  // *** Summer08 ***
  //signal_xsec["15"] -= signal_xsec["30"];
  //signal_xsec["30"] -= signal_xsec["80"];
  //signal_xsec["80"] -= signal_xsec["170"];
  //signal_xsec["170"] -= signal_xsec["300"];
  //signal_xsec["300"] -= signal_xsec["470"];
  //signal_xsec["470"] -= signal_xsec["800"];
  //signal_xsec["800"] -= signal_xsec["1400"];
  // Summer09-v3 separately
  //signal_xsec["0"] = 2.887E+06; // pre09-v3, fake xsec
  //signal_xsec["500"] = 3.0E-01;// pre09-v3, fake xsec
  // *** Summer09 OctX***

  // Manual mapping of the starting value
  // *** Summer08 ***
  // *** Summer09 OctX ***
  signal_start["15"] = 15.;
  signal_start["20"] = 20.; // new
  signal_start["30"] = 30.;
  signal_start["50"] = 50.;
  signal_start["80"] = 80.;
  signal_start["120"] = 120.;
  signal_start["170"] = 170.;
  signal_start["300"] = 300.;
  signal_start["470"] = 470.;
  signal_start["800"] = 800.;
  signal_start["1400"] = 1400.;
  // Summer09 v-3
  //signal_start["0"] = 0.;
  //signal_start["500"] = 500.;

  // Manual mapping of the upper limit
  // *** Summer08 ***
  //signal_limit["15"] = 30.;
  signal_limit["30"] = 50.;
  signal_limit["50"] = 80.;
  signal_limit["80"] = 120.;
  signal_limit["120"] = 170.;
  //signal_limit["170"] = 300.;
  //signal_limit["300"] = 470.;
  //signal_limit["470"] = 800.;
  //signal_limit["800"] = 1400.;
  //signal_limit["1400"] = 14000.;
  // Summer09-v3
  //signal_limit["0"] = 15.;
  //signal_limit["500"] = 800.;
  // *** Summer09 OctX ***
  signal_limit["15"] = 20.;
  signal_limit["20"] = 30.;
  signal_limit["170"] = 10000.;


  // Remove old output file list
  gROOT->ProcessLine(Form(".! rm output_PhotonJets_%s.txt", algo.c_str()));
  gROOT->ProcessLine(Form(".! mkdir output_PhotonJets_%s", algo.c_str()));

  for (map<string, int>::const_iterator it = signal.begin();
       it != signal.end(); ++it) {
    
    // Create temporary file list for given pT hat bin only
    // *** Summer08 ***
    //gROOT->ProcessLine(Form(".! grep EtGamma_%d_summer08 inputfiles_summer08_PhotonJets_new.txt > inputfiles_tmp.txt", it->second));
    // Summer09-v3
    //gROOT->ProcessLine(Form(".! grep EtGamma_%d_31x inputfiles_pre09_PhotonJets.txt > inputfiles_tmp.txt", it->second));
    // *** Summer09 OctX ***
    gROOT->ProcessLine(Form(".! grep output_gamma_oct_ex_pt%d inputfiles_OctX.txt > inputfiles_tmp.txt", it->second));

    // Produce a root tuple for the pT hat bin
    gjettree *sig = new gjettree(getchain("inputfiles_tmp.txt"), Form("output_PhotonJets_%s/output_PhotonJets_%s.root", algo.c_str(), it->first));
    sig->Loop(signal_xsec[Form("%d",it->second)], // int->string workaround
	      signal_start[it->first],
	      signal_limit[it->first],true);

    // Add new root tuple to output file list
    gROOT->ProcessLine(Form(".! echo \"output_PhotonJets_%s/output_PhotonJets_%s.root\" >> output_PhotonJets_%s.txt", algo.c_str(), it->first, algo.c_str()));
    delete sig;
  } // for signal
      

  
  // PROCESS QCD
  // Set pT hat bin cross sections, needed for event weights
  //map<string, string> qcd; // CINT can't handle this
  map<string, int> qcd; // => work-around
  map<string, double> qcd_xsec;
  map<string, double> qcd_start;
  map<string, double> qcd_limit;
  // *** Summer08 ***
  // Break wide pThat bins into smaller ones for mc factorization
  // This is a manual mapping from narrow bins to original files
  // *** Summer08 ***
  //qcd["15"] = 15;
  //qcd["20"] = 15; // new
  //qcd["30"] = 30;
  //qcd["50"] = 30;
  //qcd["80"] = 80;
  //qcd["120"] = 80;
  //qcd["170"] = 170;
  //qcd["230"] = 170;
  //qcd["300"] = 300;
  //qcd["380"] = 300;
  //qcd["470"] = 470;
  //qcd["600"] = 470;
  //qcd["800"] = 800;
  //qcd["1000"] = 800;
  //qcd["1400"] = 1400;
  // *** Summer09 OctX ***
  qcd["15"] = 15;
  qcd["20"] = 15;
  qcd["30"] = 15;//30;
  qcd["50"] = 15;//30;
  qcd["80"] = 15;//80;
  qcd["120"] = 15;//80;
  qcd["170"] = 15;//80;

  // Cross sections per file listed in
  // https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2008#Pythia6
  // Original units in pb
  // NB: not yet corrected for imposing an upper limit
  // *** Summer08 ***
  //qcd_xsec["15"] = 1457159248;
  //qcd_xsec["30"] =  109057220.4;
  //qcd_xsec["80"] =    1934639.567;
  //qcd_xsec["170"] =     62562.87713;
  //qcd_xsec["300"] =      3664.608301;
  //qcd_xsec["470"] =       315.5131272;
  //qcd_xsec["800"] =        11.9419745;
  //qcd_xsec["1400"] =        0.1720187189;
  // Correct for imposing an upper limit on pthat
  //qcd_xsec["15"] -= qcd_xsec["30"];
  //qcd_xsec["30"] -= qcd_xsec["80"];
  //qcd_xsec["80"] -= qcd_xsec["170"];
  //qcd_xsec["170"] -= qcd_xsec["300"];
  //qcd_xsec["300"] -= qcd_xsec["470"];
  //qcd_xsec["470"] -= qcd_xsec["800"];
  //qcd_xsec["800"] -= qcd_xsec["1400"];
  // *** Summer09 OctX ***
  qcd_xsec["15"] = 1457159248;
  qcd_xsec["30"] =  109057220.4;
  qcd_xsec["80"] =    1934639.567;
  // Correct for imposing an upper limit on pthat
  qcd_xsec["15"] -= qcd_xsec["30"];
  qcd_xsec["30"] -= qcd_xsec["80"];

  // Manual mapping of the starting value
  // *** Summer08 ***
  // *** Summer09 OctX ***
  qcd_start["15"] = 15.;
  qcd_start["20"] = 20.; // new
  qcd_start["30"] = 30.;
  qcd_start["50"] = 50.;
  qcd_start["80"] = 80.;
  qcd_start["120"] = 120.;
  qcd_start["170"] = 170.;
  qcd_start["230"] = 230.;
  qcd_start["300"] = 300.;
  qcd_start["380"] = 380.;
  qcd_start["470"] = 470.;
  qcd_start["600"] = 600.;
  qcd_start["800"] = 800.;
  qcd_start["1000"] = 1000.;
  qcd_start["1400"] = 1400.;

  // Manual mapping of the upper limit
  qcd_limit["15"] = 20.;//30.;
  qcd_limit["20"] = 30.; // new
  qcd_limit["30"] = 50.;
  qcd_limit["50"] = 80.;
  qcd_limit["80"] = 120.;
  qcd_limit["120"] = 170.;
  //qcd_limit["170"] = 230.;
  //qcd_limit["230"] = 300.;
  //qcd_limit["300"] = 380.;
  //qcd_limit["380"] = 470.;
  //qcd_limit["470"] = 600.;
  //qcd_limit["600"] = 800.;
  //qcd_limit["800"] = 1000.;
  //qcd_limit["1000"] = 1400.;
  //qcd_limit["1400"] = 14000.;
  // *** Summer09 OctX ***
  qcd_limit["170"] = 10000.;
  
  // Remove old output file list
  gROOT->ProcessLine(Form(".! rm output_QCD_%s.txt", algo.c_str()));
  gROOT->ProcessLine(Form(".! mkdir output_QCD_%s", algo.c_str()));

  for (map<string, int>::const_iterator it = qcd.begin();
       it != qcd.end(); ++it) {

    // Create temporary file list for given pT hat bin only
    // *** Summer08 ***
    //gROOT->ProcessLine(Form(".! grep QCD_%d_summer08 inputfiles_summer08_QCD_new.txt > inputfiles_tmp.txt", it->second));
    // *** Summer09 ***
    gROOT->ProcessLine(Form(".! grep output_qcd_%d inputfiles_OctX.txt > inputfiles_tmp.txt", it->second));

    // Produce a root tuple for the pT hat bin
    gjettree *bkg =new gjettree(getchain("inputfiles_tmp.txt"), Form("output_QCD_%s/output_QCD_%s.root", algo.c_str(), it->first));
    bkg->Loop(qcd_xsec[Form("%d",it->second)], // int->string workaround
	      qcd_start[it->first],
	      qcd_limit[it->first], false);

    // Add new root tuple to output file list
    gROOT->ProcessLine(Form(".! echo \"output_QCD_%s/output_QCD_%s.root\" >> output_QCD_%s.txt", algo.c_str(), it->first, algo.c_str()));
    delete bkg;
  } // for qcd

  
  /*
  // PROCESS QCD EM enriched sample
  // Set pT hat bin cross sections, needed for event weights
  //map<string, string> em; // CINT can't handle this
  map<string, int> em; // => work-around
  map<string, double> em_xsec;
  map<string, double> em_start;
  map<string, double> em_limit;
  // *** Summer08 ***
  // Break wide pThat bins into smaller ones for mc factorization
  // This is a manual mapping from narrow bins to original files
  //em["20"] = 20;
  //em["30"] = 30;
  //em["50"] = 30;
  //em["80"] = 80;
  //em["120"] = 80;

  // Cross sections per file listed in
  // https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2008#Pythia6
  // Original units in mb (converted to pb, pb for EM-JEC/QCD)
  // NB: EM-JEC (20-30,30-50,80-120) and QCD (15-30,30-80,80-170) bins are
  // a little different these (20-30,30-80,80-170)
  em_xsec["20"] = 0.40E9*0.0080; //EM-JEC: 3.992E8; //QCD: 1.457159248E9;
  em_xsec["30"] = 0.10E9*0.047; //EM-JEC: 9.500E7; //QCD: 1.09057220E8;
  em_xsec["80"] = 1.9E6*0.15; //EM-JEC: 1.631E6; //QCD: 1.934639E6;

  // Manual mapping of the starting value
  em_start["20"] = 20.;
  em_start["30"] = 30.;
  em_start["50"] = 50.;
  em_start["80"] = 80.;
  em_start["120"] = 120.;

  // Manual mapping of the upper limit
  em_limit["20"] = 30.;
  em_limit["30"] = 50.;
  em_limit["50"] = 80.;
  em_limit["80"] = 120.;
  em_limit["120"] = 170.;

  
  // Remove old output file list
  gROOT->ProcessLine(Form(".! rm output_EM_%s.txt", algo.c_str()));
  gROOT->ProcessLine(Form(".! mkdir output_EM_%s", algo.c_str()));

  for (map<string, int>::const_iterator it = em.begin(); it != em.end(); ++it) {

    // Create temporary file list for given pT hat bin only
    //gROOT->ProcessLine(Form(".! grep 'EM_enrich_%d' inputfiles_summer08_EM_new.txt > inputfiles_tmp.txt", it->second));
    gROOT->ProcessLine(Form(".! grep 'EMenrich_%d' inputfiles_pre09_EM.txt > inputfiles_tmp.txt", it->second)); // Summer09

    // Produce a root tuple for the pT hat bin
    gjettree *embkg =new gjettree(getchain("inputfiles_tmp.txt"), Form("output_EM_%s/output_EM_%s.root", algo.c_str(), it->first));
    embkg->Loop(em_xsec[Form("%d",it->second)], // int->string workaround
		em_start[it->first],
		em_limit[it->first], false);

    // Add new root tuple to output file list
    gROOT->ProcessLine(Form(".! echo \"output_EM_%s/output_EM_%s.root\" >> output_EM_%s.txt", algo.c_str(), it->first, algo.c_str()));
    delete embkg;
  } // for qcd
  */

  // Merge PhotonJets and QCD lists
  gROOT->ProcessLine(Form(".! cat output_PhotonJets_%s.txt > output_mixed_%s.txt", algo.c_str(), algo.c_str()));
  gROOT->ProcessLine(Form(".! cat output_QCD_%s.txt >> output_mixed_%s.txt", algo.c_str(), algo.c_str()));
  //gROOT->ProcessLine(Form(".! cat output_EM_%s.txt >> output_mixed_%s.txt", algo.c_str(), algo.c_str()));

}
