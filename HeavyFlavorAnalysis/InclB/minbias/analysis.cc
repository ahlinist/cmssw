#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>

#include "TROOT.h"
#include "TRint.h"
#include "TChain.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TString.h"
#include "TString.h"
#include "TRandom.h"
#include "TUnixSystem.h"

#include "treeAnalysis.hh"


using namespace std;


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int main(int argc, char *argv[]) {

  int processID = gSystem->GetPid();
  cout << "Running under process ID  " << processID << endl;

  TString progName  = argv[0]; 
  TString writeName, fileName;
  Int_t isVerbose(0);
  Int_t option(0);
  Double_t weightlow(1.);
  Double_t weight020(1.);
  Double_t weight2030(1.);
  Double_t weight3050(1.);
  Double_t weight5080(1.);
  Double_t weight80120(1.);
  Double_t weight120170(1.);
  Double_t weight170(1.);
  Double_t pthat(0.);
  Int_t file(0);
  Int_t dirspec(0);
  Int_t nevents(-1), start(-1);
  Int_t randomSeed(processID);


  // -- Some defaults
  TString dirBase("./");               // this could point to "/home/ursl/data/root/."
  TString dirName("."); dirspec = 0;   // and this to, e.g. "analysis", "bee", "bem", ...
  TString cutFile("analysis.default.cuts");
 
  TString treeName("T1");
  TString evtClassName("TAna00Event");


  // -- command line arguments
  for (int i = 0; i < argc; i++){
    if (!strcmp(argv[i],"-C"))  {cutFile    = TString(argv[++i]); }               // file with cuts
    if (!strcmp(argv[i],"-c"))  {fileName   = TString(argv[++i]); file = 0; }     // file with chain definition
    if (!strcmp(argv[i],"-D"))  {dirName    = TString(argv[++i]); dirspec = 1; }  // where to put the output
    if (!strcmp(argv[i],"-f"))  {fileName   = TString(argv[++i]); file = 1; }     // single file instead of chain
    if (!strcmp(argv[i],"-n"))  {nevents    = atoi(argv[++i]); }                  // number of events to run 
    if (!strcmp(argv[i],"-ni")) {start      = atoi(argv[++i]); }                  // start at event
    if (!strcmp(argv[i],"-r"))  {randomSeed = atoi(argv[++i]); }                  // set seed for random gen.
    if (!strcmp(argv[i],"-v"))  {isVerbose  = atoi(argv[++i]);  }                 // debug level
    if (!strcmp(argv[i],"-o"))  {option     = atoi(argv[++i]);  }                 // option in eventprocessing 
    if (!strcmp(argv[i],"-wlow"))    {weightlow   = atof(argv[++i]);  }           // events weight
    if (!strcmp(argv[i],"-w020"))    {weight020   = atof(argv[++i]);  }           // events weight
    if (!strcmp(argv[i],"-w2030"))   {weight2030  = atof(argv[++i]);  }           // events weight
    if (!strcmp(argv[i],"-w3050"))   {weight3050  = atof(argv[++i]);  }           // events weight 
    if (!strcmp(argv[i],"-w5080"))   {weight5080  = atof(argv[++i]);  }           // events weight
    if (!strcmp(argv[i],"-w80120"))  {weight80120  = atof(argv[++i]); }           // events weight 
    if (!strcmp(argv[i],"-w120170")) {weight120170  = atof(argv[++i]);}           // events weight 
    if (!strcmp(argv[i],"-w170"))    {weight170     = atof(argv[++i]);}           // events weight
    if (!strcmp(argv[i],"-pthat"))   {pthat         = atof(argv[++i]);}           // pthat
   
  }


  // -- Prepare histfilename variation with (part of) cut file name
  TString fn(cutFile);
  fn.ReplaceAll("cuts/", ""); 
  fn.ReplaceAll("analysis", ""); 
  fn.ReplaceAll("bjk", ""); 
  fn.ReplaceAll("cuts", ""); 
  fn.ReplaceAll(".", ""); 
  fn.ReplaceAll("/shome/caminada/cmssw/CMSSW_3_5_6/src/HeavyFlavorAnalysis/InclB/", ""); 
  fn.ReplaceAll("/shome/caminada/cmssw/CMSSW_3_5_6/src/HeavyFlavorAnalysis/InclB/", "");  
    
  // -- Determine filename for output histograms and 'final' small/reduced tree
  TString  barefile(fileName), chainFile, meta, histfile;
  if (file == 0) {
    // -- input from chain
    if (barefile.Contains("chains/")) { 
      meta = barefile;
      barefile.ReplaceAll("chains/", ""); 
      barefile.ReplaceAll("/shome/caminada/cmssw/CMSSW_3_5_6/src/HeavyFlavorAnalysis/InclB/", "");
      histfile = barefile + "." + fn + ".root"; 
      if (dirspec) {
	histfile = dirBase + "/" + dirName + "/" + histfile;
      } 
      
    } else {
      meta = barefile;
      barefile.ReplaceAll("/shome/caminada/cmssw/CMSSW_3_5_6/src/HeavyFlavorAnalysis/InclB/", ""); 
      histfile =  barefile + "." + fn + ".root"; 
      if (dirspec) {
	histfile = dirBase + "/" + dirName + "/" + histfile;
      } 
    }
    // -- The following lines strip everything from the string up to and including the last '/'
    int fl = barefile.Last('/');
    TString bla(barefile);
    bla.Replace(0, fl+1, ' '); bla.Strip(TString::kLeading, ' ');  bla.Remove(0,1);
    histfile =  bla + "." + fn + ".root";
    if (dirspec) {
      histfile = dirBase + "/" + dirName + "/" + histfile;
    } 
  }  else if (file == 1) {
    // -- single file input
    // -- The following lines strip everything from the string up to and including the last '/'
    int fl = barefile.Last('/');
    TString bla(barefile);
    bla.Replace(0, fl+1, ' '); bla.Strip(TString::kLeading, ' ');  bla.Remove(0,1);
    histfile =  bla;
    histfile.ReplaceAll(".root", "");
    histfile +=  "." + fn + ".root";
    if (dirspec) {
      histfile = dirBase + "/" + dirName + "/" + histfile;
    }
  }
  
  cout << "Opening " << histfile.Data() << " for output histograms" << endl;
  cout << "Opening " << fileName.Data() << " for input" << endl;


  // -- Set up chain
  TChain *chain = new TChain(treeName);
  cout << "Chaining ... " << treeName << endl;
  char pName[2000]; 
  int nentries; 
  if (file == 0) {
    // -- non-trivial chain input
    ifstream is(meta);  
    while(meta.ReadLine(is) && (!meta.IsNull())){ 
      nentries = -1;
      if (meta.Data()[0] == '#') continue; 
      sscanf(meta.Data(), "%s %d", pName, &nentries); 
      if (nentries > -1) {
        cout << pName << " -> " << nentries << " entries" << endl; 
        chain->Add(pName, nentries); 
      } else {
        cout << meta << endl; 
        chain->Add(meta); 
      }
    }
    is.close();
  }
  else if (file == 1) {
    // -- single file input
    cout << fileName << endl;
    chain->Add(fileName);
  }

  // -- Now instantiate the tree-analysis class object, initialize, and run it ...
  treeAnalysis a(chain, evtClassName);
  a.chainFileName(fileName);
  a.openHistFile(histfile); 
   
  a.debugLevel(isVerbose);
  a.readCuts(cutFile, 1);

  
  a.startAnalysis();
  a.bookHist(option);
  a.loop(nevents, start, option, weightlow, weight020, weight2030, weight3050, weight5080, weight80120, weight120170, weight170, pthat);
  a.endAnalysis();
  a.closeHistFile();
  
  return 0; 
  
}
