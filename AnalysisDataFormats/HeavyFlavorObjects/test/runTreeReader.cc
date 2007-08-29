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

#include "treeReader.hh"


using namespace std;


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int main(int argc, char *argv[]) {

  int processID = gSystem->GetPid();
  cout << "Running under process ID  " << processID << endl;

  TString progName  = argv[0]; 
  TString writeName, fileName;
  Int_t file(0);
  Int_t dirspec(0);
  Int_t nevents(-1), start(-1);
  Int_t randomSeed(processID);

  // -- Some defaults
  TString dirBase("./");               // this could point to "/home/ursl/data/root/."
  TString dirName("."); dirspec = 0;   // and this to, e.g. "bmm", "bee", "bem", ...
  TString cutFile("bmm.default.cuts");
  TString decaySign("2mu");

  TString treeName("T1");
  TString evtClassName("TAna00Event");


  // -- command line arguments
  for (int i = 0; i < argc; i++){
    if (!strcmp(argv[i],"-c"))  {fileName   = TString(argv[++i]); file = 0; }     // file with chain definition
    if (!strcmp(argv[i],"-D"))  {dirName    = TString(argv[++i]);  dirspec = 1; } // where to put the output
    if (!strcmp(argv[i],"-f"))  {fileName   = TString(argv[++i]); file = 1; }     // single file instead of chain
    if (!strcmp(argv[i],"-n"))  {nevents    = atoi(argv[++i]); }                  // number of events to run 
    if (!strcmp(argv[i],"-r"))  {randomSeed = atoi(argv[++i]); }                  // set seed for random gen.
  }


  
  // -- Determine filename for output histograms and 'final' small/reduced tree
  TString  barefile(fileName), chainFile, meta, histfile;
  if (file == 0) {
    // -- input from chain
    if (barefile.Contains("chains/")) {
      meta = barefile;
      barefile.ReplaceAll("chains/", "");
      histfile = barefile + "." + ".root";
      if (dirspec) {
        histfile = dirBase + "/" + dirName + "/" + histfile;
      }
    } else {
      meta = barefile;
      histfile =  barefile + "." + ".root";
      if (dirspec) {
        histfile = dirBase + "/" + dirName + "/" + histfile;
      }
    }
    // -- The following lines strip everything from the string up to and including the last '/'
    int fl = barefile.Last('/');
    TString bla(barefile);
    bla.Replace(0, fl+1, ' '); bla.Strip(TString::kLeading, ' ');  bla.Remove(0,1);
    histfile =  bla + "." + ".root";
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
    histfile +=  ".root";
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
  treeReader a(chain, evtClassName);
  a.openHistFile(histfile); 
  a.bookHist(); 

  a.startAnalysis(); 
  a.loop(nevents, start);
  a.closeHistFile(); 

  return 0; 
  
}
