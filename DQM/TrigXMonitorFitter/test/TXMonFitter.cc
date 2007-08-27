#include <iostream>
#include <stdlib.h>
#include "TriggerLineInfo.h"

using namespace std;

int main(Int_t argc, char **argv)
{
  gROOT->SetStyle("Plain");
  if (argc < 3)
    {
      cout << "Usage: " << argv[0] << " Triggername txmonfile123456.root ..." << endl;
      exit(0);
    }

  string Triggername = argv[1];
  vector< string > RunVector;



  for (int loop = 2; loop < argc; ++loop)
    {
      RunVector.push_back( argv[loop] );
    } // for loop

  TriggerLineInfo::setMatchname (Triggername);
  TriggerLineInfo::setRunList (RunVector);
  // set mode, this must be done after setRunList
  TriggerLineInfo::setMode(TriggerLineInfo::TLIc_TXMonRoot); 


  string firstTXMRfile = RunVector[0];
  int Version;

  TriggerLineInfo::LevelType Level = TriggerLineInfo::decodeLevelType(Triggername);
  string Novers = TriggerLineInfo::StripVersionInfo (Triggername, Version);
  int Position = TriggerLineInfo::getPositionOfname (Level, Novers, firstTXMRfile, Version);

  cout <<"Name: " <<Novers <<" Position: " <<Position << " Version " <<Version << endl;




  return 0;
}
