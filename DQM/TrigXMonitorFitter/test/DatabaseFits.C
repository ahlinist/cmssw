#include <iostream>


void DatabaseFits(int allTriggers = 1, int defaultRunNumbers = 1)
{
    
  gROOT->SetStyle("Plain");
  
  //
  // compile needed functions
  if ( !gSystem->CompileMacro("ResidualStat.cc", "k")  ) return;
  if ( !gSystem->CompileMacro("TriggerLineInfo.cc", "k")  ) return;


  //
  // set file for Run numbers MUST be done before setting mode
  if (! defaultRunNumbers) // default is RunNumbers.txt
    {
      TriggerLineInfo::setRunNumberFilename("runsHighLumi.txt");   
    } 
  
  //
  // set mode 
  TriggerLineInfo::setMode(TriggerLineInfo::TLI_kDataBase);
  

  //
  // process all triggers
  if (allTriggers)
    {     
      cout << "Processing all triggers" << endl;
      TriggerLineInfo::ProcessAllTriggers();
    }
  else 
    {
      cout << "** Processing only some triggers **" << endl;
    
      TriggerLineInfo::ProcessSpecificTriggers("PEM8");                               // Process only triggers with 'PEM8'
      TriggerLineInfo::ProcessSpecificTriggers("DIELECTRON_CENTRAL_L1_TWO_CEM4_v2");  // Process only triggers with 'DIELECTRON_CENTRAL_L1_TWO_CEM4_v2'
    }
  

  //
  // make html files with residual information for each run
  TriggerLineInfo::MakeStatisticsWebsite();


}
