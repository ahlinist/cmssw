#include <iostream>
#include <stdlib.h>
#include "TriggerLineInfo.h"
#include "dout.h"

using namespace std;

int main(int argc, char **argv)
{
  
  
  gROOT->SetStyle("Plain");
  
  
  //
  // if not enough root files, Fitter will exit
  if (argc < 2)
    {
      cout << "Usage: " << argv[0] << " txmonfile123456.root ..." << endl;
      exit(0);
    }
  
  
  vector<string> RunVector;
  for (int loop = 1; loop < argc; ++loop)
    {
      RunVector.push_back( argv[loop] );
    }  
  cout << "Runs " << RunVector.size() << endl;
  

  //
  // set run list
  TriggerLineInfo::setRunList (RunVector);   
  
  
  //
  // set mode, this must be done after setting the run list
  TriggerLineInfo::setMode(TriggerLineInfo::TLI_kTXMonRoot);  // set mode MUST be done after setRunList
  
  
  vector<string> names = TriggerLineInfo::allTriggerLinesInRootFile (RunVector[0]);
  unsigned int NumberLines = names.size();    // NumberLines = 6;//Nagesh
  for (unsigned int loop = 0; loop < NumberLines; ++loop)
    {
      cout << endl << endl << "Fitting " << names[loop] << endl;
      TriggerLineInfo::ProcessOneTXMonRootTrigger(names[loop]);
    }
  
    
  return 0;

}
