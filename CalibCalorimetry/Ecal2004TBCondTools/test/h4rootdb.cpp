//! Main program to copy MySQL database into root-program
//!
//! $Date: 2004/12/17 10:04:12 $
//! $Author: brunelie $
//!
//! If you are interested in only parts of the database, you can
//! do this by changing the loops over the entries or by commenting
//! out the entire access to the database


#include <unistd.h>
#include <string>
#include <vector>
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4DBManager.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4Run.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4Crystal.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4TdcRange.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4TowerPedestals.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4PedestalUtil.h"

void usage()
{
  cerr << "usage: h4rootdb [OPTIONS]                                  " << endl << endl;
  cerr << "   dumps the contents of the mySQL run database into a small ROOT file" << endl;
  cerr << "   may also add info stored in simple ASCII files to the ROOT database" << endl << endl;
  cerr << " OPTIONS:                                                  " << endl;
  cerr << "    [-h]                     print this message            " << endl;
  cerr << "    [-v]                     turn up verbosity             " << endl;
  cerr << "    [-o <rootdbfile>]        output file name              " << endl;
  cerr << "    [-r <minRunNumber>]      skip runs below this limit    " << endl;
  cerr << "    [-R <maxRunNumber>]      skip runs above this          " << endl;
  cerr << "    [-f <input ascii file>]  may be specified several times" << endl;
  exit(-1);
}




int main (int argc, char **argv)
{
  int i, c;
  int verb=0;
  int write=0;
  // int minRunNumber=62941; // 2003 runs so far
  int minRunNumber=72800; // SM10 2004 runs so far
  int maxRunNumber=75000; // we may have to increase this default soon
  string outputFileName="myrootdb.root";
  string mysqlServer= "mysql://suncms100.cern.ch:3306";
  string user= "WWW";
  string pwd= "myWeb01";
  vector<string>  asciiFiles;
  
  while( (c=getopt(argc, argv, "hvwr:R:f:s:o:u:p:")) != -1 )
    switch(c) {
    case 'h':
      usage();
      break;
    case 'v':
      verb++;
      break;
    case 'w':
      write++;
      break;
     case 'o':
      outputFileName=optarg;
      break;
     case 's':
      mysqlServer=optarg;
      break;
    case 'u':
      user=optarg;
      break;
    case 'p':
      pwd=optarg;
      break;
    case 'r':
      minRunNumber=atoi(optarg);
      break;
    case 'R':
      maxRunNumber=atoi(optarg);
      break;
    case 'f':
      asciiFiles.push_back(optarg);
      break;
    default:
      usage();
    }

  cout << "Ciao" << endl;
  if(verb) {
    cout << "*** h4rootdb: creating H4 ROOT run database: --> " 
	 << outputFileName << endl;
    cout << "***   minRun: " << minRunNumber << endl;
    cout << "***   maxRun: " << maxRunNumber << endl;
    cout << "***   number of external ASCII files: " << asciiFiles.size() 
	 << ", list follows:" << endl;
    for(i=0;i<(int)asciiFiles.size();i++)
      cout << "***   " << asciiFiles[i] << endl;
  }
  
  if(verb)
    cout << "Creating myrootdb.root copy of MySQL database" << endl; 
  H4DBManager* myManager;
  // Interactive mysql -h suncms100 -u WWW -pmyWeb01
  if ( mysqlServer != "")
    {
      cout << "Using a MySQL server" << endl;
      myManager= new H4DBManager(write, outputFileName.c_str(),
			    mysqlServer.c_str(),
			    "WWW", "myWeb01");
    }
  else
    {
      cout << "Using a RootDB server" << endl;
      myManager= new H4DBManager(false, outputFileName.c_str());
    }

  H4DBEntry *myRunEntry=myManager->getEntry("H4Run", 10065);
  if(!myRunEntry){
    cout << "You will need a different start value" << endl;
    abort();
  }

  for (i= minRunNumber; i<maxRunNumber; i++){
    if(i>11325 && i< 30000) continue; // Skip some empty runs out
    cout << "looking at " << i << endl;
    int n = myManager->update(i, H4DBEntry::NoSuchSubindex, myRunEntry);
    if(n<1) continue; // No record found for this run-number
    
    if(n>1) { // A run with several subruns
      int max=myRunEntry->getMaxSubIndex();
      for(int j=myRunEntry->getMinSubIndex(); j <= max; j++)
	myManager->update( i, j, myRunEntry);
      //      myRunEntry->print();
    }
    //  cout << i << " " << ((H4Run*)myRunEntry)->getRunType() << " " << ((H4Run*)myRunEntry)->getStartTime() << " " << ((H4Run*)myRunEntry)->getStopTime() << std::endl;
//     cout << myRunEntry << endl;
//     myRunEntry->print();
//     cout << myRunEntry << endl;
  }
  
//    H4DBEntry *myCrystalEntry=myManager->getEntry("H4Crystal", 1,0);

//   for (int i=1; i<9; i++){  // Loop over tables
//     cout << "Starting with table " << i << " ... ";
//     cout.flush();
//     for (int j=0; j < 1700; j++){
//       myManager->update(i, j, myCrystalEntry);
//       cout << ((H4Crystal *)myCrystalEntry)-> << std::endl;
//       //      myCrystalEntry->print();
//     }
//     //    myManager->update(8, 1660, myCrystalEntry);
//     //    
//     //	 << endl;
//   }

  H4DBEntry *myIntercalibrationEntry
    =myManager->getEntry("H4Intercalibration", 0, 925);

  // for (int i=0; i<32; i++){  // Loop over versions
  for (int i=0; i<32; i++){  // Loop over versions
    cout << "Starting with version " << i << " ... ";
    cout.flush();
    for (int j=0; j < 1700; j++){
      myManager->update(i, j, myIntercalibrationEntry);
      //      myIntercalibrationEntry->print();
    }
  }

  H4DBEntry *myAlignmentEntry
    =myManager->getEntry("H4Alignment", 1,925);

  for (int i=1; i<2; i++){  // Loop over versions
    cout << "Starting with version " << i << " ... ";
    cout.flush();
    for (int j=926; j < 1700; j++){
      myManager->update(i, j, myAlignmentEntry);
      //      myAlignmentEntry->print();
    }
  }

  H4DBEntry *myShapeParamEntry
    =myManager->getEntry("H4ShapeParam", 0, 1);
  for (int i = 0; i < 8; i++) {
    cout << "Starting with version " << i << endl;
    for (int j = 1; j < 6; j++) {
      int r = i;
      cout << "Event type:" << j << endl;
      if (i == 1) r = 67740;
      if (i == 2) r = 67940;
      if (i == 3) r = 70300;
      if (i == 4) r = 70381;
      if (i == 5) r = 72744;
      if (i == 6) r = 72800;
      if (i == 7) r = 73095;
      myManager->update(r, j, myShapeParamEntry);
//       if (myShapeParamEntry)
// 	myShapeParamEntry->print();
    }
  }

  H4DBEntry *myTdcEntry
    =myManager->getEntry("H4TdcRange", 0,1);
  for (int j = 1; j < 6; j++) {
    cout << "Event type:" << j << endl;
    H4TdcRange* t=dynamic_cast<H4TdcRange*>(myTdcEntry);
    myManager->update(0, j, myTdcEntry);
    cout << "Tdc Min " << t->getTdcMin()
	 << "Tdc Max " << t->getTdcMax()
	 << endl;
  }
  for (int i = 63000; i < maxRunNumber; i++) {
    if(myManager->update(i, H4DBEntry::NoSuchSubindex, myTdcEntry)<1)
      continue;
    for (int j=1; j<6 ; j++){
      cout<<"Event type:"<<j<<endl ;
      myManager->update(i, j, myTdcEntry);
      H4TdcRange* t=dynamic_cast<H4TdcRange*>(myTdcEntry);
      cout << "Tdc Min " << t->getTdcMin()
	   << "Tdc Max " << t->getTdcMax()
	   << endl;
//       t->print();
    }
  }
  
  H4DBEntry *myGainsEntry
    =myManager->getEntry("H4Gains", 1,-1);
  for (int i=0; i<4; i++){
    int entries=myManager->update(i, H4DBEntry::NoSuchSubindex, myGainsEntry);
    if(entries <=1){
      continue;
    } else { 
      for (int j=-1; j<1701 ; j++){
	myManager->update(i, j, myGainsEntry);
	//	myGainsEntry->print();
      }
    }
  }
  
//   H4DBEntry *myPnLinearityEntry
//     = myManager->getEntry("H4PnLinearity", 72744, 1600);
//   for (int iRun = 0; iRun < 1; iRun++) {
//     int run = 0;
//     if (iRun == 0) run = 72744;
//     cout << "Starting with runNumber " << run << endl;
//     for (int option = 0; option < 2; option++)
//       for (int gain = 1; gain < 17; gain += 15)
// 	for (int pn = 0; pn < 10; pn++) {
// 	  int subIndex = option*10000 + gain*100 + pn;
// 	  myManager->update(run, subIndex, myPnLinearityEntry);
// 	  //	  myPnLinearityEntry->print();
// 	}
//   }

//   // Write out database from ascii file
//   // 
//   for(int j=0;j<(int)asciiFiles.size();j++)
//     {
//       myManager->readAsciiFile(asciiFiles[j].c_str());
//       if (verb) cout << "***  Writing rootDB from ascii file: " 
// 		     << asciiFiles[j] << endl;
//     }

  string pedFileName="Ped2004.root";
  string pedTreeName= "TPedestals" ;
  string pedTowerNamePrefix="Tower_Peds_";
  string pedTowerNameSuffix=".";
  vector<int>    pedTowerNumbers;

  for (int i=0;i<68;i++)
    pedTowerNumbers.push_back(i+1);

  int n = myManager->update(minRunNumber, H4DBEntry::NoSuchSubindex, myRunEntry);
  H4TowerPedestals* tped = new H4TowerPedestals(pedFileName, pedTreeName, pedTowerNumbers,
			      pedTowerNamePrefix, pedTowerNameSuffix);
  H4PedestalUtil* tpedutil = new H4PedestalUtil(myManager, (H4Run*) myRunEntry);
  std::cout << "Setting Pedestal Run for " <<  minRunNumber << " to "
	    << (tpedutil->getPedestalRun(minRunNumber)) << std::endl;
  tped->selectPedestalRun(tpedutil->getPedestalRun(minRunNumber));



  for (int i=0;i<68;i++)
    for (int j=0;j<25;j++)
      {
	std::cout << "++++++++++++++++++++++++++++++++++" << std::endl;
	std::cout << "Tower " << i << " Channel " << j << std::endl;
	for (int g=0;g<3;g++)
	  {
	    std::cout << "Gain " << g << " ped mean " << tped->getMean(i+1, j, g) << " ped RMS " << tped->getRMS(i+1, j, g) << std::endl;
	  }
	std::cout << "++++++++++++++++++++++++++++++++++" << std::endl;
      }

  delete myManager;

  
}
