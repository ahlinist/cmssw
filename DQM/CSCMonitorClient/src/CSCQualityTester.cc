/** \file
 *
 *  Implementation of CSCQualityTester
 *
 *  $Date: 2006/01/19 15:17:47 $
 *  $Revision: 1.3 $
 *  \author Ilaria Segoni
 */
#include "DQM/CSCMonitorClient/interface/CSCQualityTester.h"
#include "DQMServices/QualityTests/interface/QCriterionRoot.h"

#include<iostream>
#include <sstream>


CSCQualityTester::CSCQualityTester()
{
  
  printout=true;
  logFile.open("CSCDQMClient.log");
  
  if(printout) logFile <<"A Quality Tester Is being Created"<<std::endl;

}


CSCQualityTester::~CSCQualityTester(){ 
  logFile.close();
}



void CSCQualityTester::GetTestsFromDB(MonitorUserInterface * mui){ 

  FILE* TestsFile;
  char  fileName[128];
  sprintf(fileName,"QualityTests.db");


  int TestType;
  char TestName[20];
  float WarningLevel=0;
  float params[5];
  
  for(int ii=0; ii<5; ii++) params[ii]=0.0;
  
  TestsFile= fopen(fileName,"r");
  
  int counter =1;
  int file_status=1;
  
  do{
    if(printout) logFile<< "File Status before= "<<file_status<<" Reading line: "<<counter<<std::endl;
    
    file_status=fscanf(TestsFile,"%d%s%f%f%f%f%f%f\n",
                         &TestType,&TestName,&WarningLevel,
			 &params[0],&params[1],&params[2],&params[3],&params[4]);
			 
    if(file_status != EOF){			 
       if(printout)  logFile<<" Reading Quality Tests Configuration: "
            <<TestType<<" "<<TestName<<" "<<WarningLevel<<" "
	    <<params[0]<<" "<<params[1]<<" "<<params[2]
	    <<" "<<params[3]<<" "<<params[4]<< std::endl;
	
    if(TestType==1) SetContentsXRangeROOTTest(mui,TestName,WarningLevel,params);    
       	    
    }	

     counter++;
    if(printout) logFile<< "File Status after= "<<file_status<<std::endl;


  }while( file_status!=EOF ); 



}



void CSCQualityTester::SetContentsXRangeROOTTest(MonitorUserInterface * mui, char  TestName[20], float  WarningLevel, float  params[5]){

	
 /// X-axis content withing a given range [Xmin, Xmax]
 qTests.push_back(TestName);
 
 
 QCriterion * qc1 = mui->createQTest(ContentsXRangeROOT::getAlgoName(),TestName);
 MEContentsXRangeROOT * me_qc1 = (MEContentsXRangeROOT *) qc1;
 me_qc1->setAllowedXRange(params[0],params[1]);
 me_qc1->setWarningProb(WarningLevel);
 
}



void CSCQualityTester::SetupTests(MonitorUserInterface * mui){    
    
 /// X-axis content withing a given range [Xmin, Xmax]
 qtest1 = "xRange";
 
 
 QCriterion * qc1 = mui->createQTest(ContentsXRangeROOT::getAlgoName(),qtest1);
 MEContentsXRangeROOT * me_qc1 = (MEContentsXRangeROOT *) qc1;
 /// set allowed range in X-axis (default values: histogram's X-range)
 me_qc1->setAllowedXRange(1.0, 200.0);
 /// set probability limit for test warning (default: 90%)
 me_qc1->setWarningProb(0.90);
 
 
 
 return;

}

void CSCQualityTester::AttachTests(MonitorUserInterface * mui){

 /// use test <qtest1> on all MEs matching string ""
 if(printout) logFile<<"... for DDU_L1A_Increment_0 ..."<<std::endl;
 mui->useQTest("DDU_L1A_Increment_*", qtest1);
 if(printout) logFile<<"...done."<<std::endl;


}



void CSCQualityTester::CheckTests(MonitorUserInterface * mui) 
{
  if(printout) logFile << "Checking Status of Quality Tests" << std::endl;

  
	int status = 1980;
	
	status= mui->getSystemStatus();
        if(printout) logFile << "The STATUS IS " << status<<std::endl;
        if(printout) logFile << "Possible states: \nerror:  " 
	<< dqm::qstatus::ERROR<<
	"\nwarning:  "<< dqm::qstatus::WARNING<<
	"\n Other: "<< dqm::qstatus::OTHER<<std::endl;
	
	switch(status)
	  {
	  case dqm::qstatus::ERROR:
	    std::cout << " Error(s)";
	    break;
	  case dqm::qstatus::WARNING:
	    std::cout << " Warning(s)";
	    break;
	  case dqm::qstatus::OTHER:
	    std::cout << " Some tests did not run;";
	    break; 
	  default:
	    std::cout << " No problems";
	  }

	//MonitorElement * me4 = mui->get("Collector/FU0/C1/C2/histo4");
	//if(me4)
	  //checkTests(me4);
	
  return;
}

  
