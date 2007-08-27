#include <iostream>
#include <vector>
#include <string>

using namespace std;

void TXMonRootFits(char *Triggername,
                  string Run1  = "", string Run2  = "", string Run3  = "",
                  string Run4  = "", string Run5  = "", string Run6  = "",
                  string Run7  = "", string Run8  = "", string Run9  = "",
                  string Run10 = "", string Run11 = "", string Run12 = "",
		  string RunN  = "")
{
  // I hate gray
  gROOT->SetStyle("Plain");
  
  // compile needed functions
  if ( !gSystem->CompileMacro("ResidualStat.cc", "k")  ) return;
  if ( !gSystem->CompileMacro("TriggerLineInfo.cc", "k")  ) return;
  
  vector< string > RunVector;
  if (Run1.size() ) RunVector.push_back( Run1  );
  if (Run2.size() ) RunVector.push_back( Run2  );
  if (Run3.size() ) RunVector.push_back( Run3  );
  if (Run4.size() ) RunVector.push_back( Run4  );
  if (Run5.size() ) RunVector.push_back( Run5  );
  if (Run6.size() ) RunVector.push_back( Run6  );
  if (Run7.size() ) RunVector.push_back( Run7  );
  if (Run8.size() ) RunVector.push_back( Run8  );
  if (Run9.size() ) RunVector.push_back( Run9  );
  if (Run10.size()) RunVector.push_back( Run10 );
  if (Run11.size()) RunVector.push_back( Run11 );
  if (Run12.size()) RunVector.push_back( Run12 );
  if (RunN.size() ) RunVector.push_back( RunN  );

  if (! RunVector.size())
    {
      cerr << "You must provide at least one Run number." << endl;
      return;
    }
  
  TriggerLineInfo::setMatchname (Triggername);
  TriggerLineInfo::setRunList (RunVector);
  
  // set mode 
  TriggerLineInfo::setMode(TriggerLineInfo::TLIc_TXMonRoot);
  
  string firstTXMRfile = RunVector[0];
  int Version;
  TriggerLineInfo::LevelType level = TriggerLineInfo::decodeLevelType(Triggername);
  string Novers = TriggerLineInfo::StripVersionInfo (Triggername, Version);
  int Position  = TriggerLineInfo::getPositionOfname (Level, Novers, firstTXMRfile, Version);

  cout <<"Name: " <<Novers <<" Position: " <<Position <<" Version: " <<Version << endl;
}
