#include <iostream>
#include <stdlib.h>
#include "TriggerLineInfo.h"

using namespace std;

int main(int argc, char **argv)
{
  gROOT->SetStyle("Plain");
  if (argc < 4)
    {
      cout <<"Usage: " <<argv[0] <<" Triggername base_file_name [-f p0 p1 p2 p3 Error] " <<"txmonfile123456.root ..." <<endl;
      exit(0);
    }

  string Triggername = argv[1];
  string basename = argv[2];
  string third = argv[3];
  
  // Is somebody trying to pass in the fit parameters?
  int offset = 0;
  double p0 = 0;
  double p1 = 0;
  double p2 = 0;
  double p3 = 0;
  double pn = 0;
  double Error = 0;
  
  if ( third == "-f" )
    { 
      cout << "trying -f" << endl;
      // do we have enough arguments?
      if (argc < 10)
	{
	  cout <<"Usage: " << argv[0] <<" Triggername base_file_name [-f p0 p1 p2 p3 Error] " <<"txmonfile123456.root ..." <<endl;
	  exit(0);
	}
      p0  = atof( argv[4] );
      p1  = atof( argv[5] );
      p2  = atof( argv[6] );
      p3  = atof( argv[7] );
      Error = atof( argv[8] );
      offset = 6; // we used 1 for '-f' and 5 for parameters
      TriggerLineInfo::useOfficialFit (p0, p1, p2, p3, Error);
    }
  
  vector< string > RunVector;
  for (int loop = 3 + offset; loop < argc; ++loop)
    {
      RunVector.push_back( argv[loop] );
    } // for loop
  
  if ( RunVector.size() == 0 )
    {
      // Sorry, no root files.
      cout <<"Usage: " << argv[0] <<" Triggername base_file_name [-f p0 p1 p2 p3 Error] " <<"txmonfile123456.root ..." <<endl;
      exit(0);
    }
  
  TriggerLineInfo::setRunList (RunVector);
  
  // set mode MUST be done after setRunList
  TriggerLineInfo::setMode(TriggerLineInfo::TLIc_TXMonRoot);
  
  //TriggerLineInfo::ProcessOneTXMonRootTrigger(Triggername);
  TriggerLineInfo::ProcessOneTXMonRootTrigger(Triggername, basename, true);
  
  return 0;
}
