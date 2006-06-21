#include "CondCore/DBCommon/interface/DBWriter.h"
#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/Exception.h"
#include "CondCore/DBCommon/interface/ServiceLoader.h"
#include "CondCore/DBCommon/interface/ConnectMode.h"
#include "CondCore/DBCommon/interface/MessageLevel.h"
#include "CondFormats/Calibration/interface/Pedestals.h"
#include "CondCore/IOVService/interface/IOV.h"
#include "CondCore/MetaDataService/interface/MetaData.h"

#include "FWCore/Framework/interface/IOVSyncValue.h"

#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "CondFormats/EcalObjects/interface/EcalPedestals.h"
#include "CondFormats/EcalObjects/interface/EcalWeightXtalGroups.h"
#include "CondFormats/EcalObjects/interface/EcalTBWeights.h"
#include "CondFormats/EcalObjects/interface/EcalWeightSet.h"
#include "CondFormats/EcalObjects/interface/EcalWeight.h"
#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"
#include "CondFormats/EcalObjects/interface/EcalGainRatios.h"
#include "CondFormats/EcalObjects/interface/EcalMGPAGainRatio.h"
#include "CondFormats/EcalObjects/interface/EcalADCToGeVConstant.h"

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4DBManager.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4Geom.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4Run.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4Crystal.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4Intercalibration.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4Gains.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4TdcRange.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4TowerPedestals.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4PedestalUtil.h"

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <unistd.h>
#include <vector>




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


using namespace std;


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
  
  //Taking informations from H4 RootDb

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
    //      cout << i << " " << ((H4Run*)myRunEntry)->getRunType() << " " << ((H4Run*)myRunEntry)->getStartTime() << " " << ((H4Run*)myRunEntry)->getStopTime() << std::endl;
//     cout << myRunEntry << endl;
//    myRunEntry->print();
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

  EcalIntercalibConstants* ical = new EcalIntercalibConstants;
  
  H4Intercalibration* entry=(H4Intercalibration *)
    myManager->getEntry("H4Intercalibration", 32, 704);
  for(int i=1; i<=1700; i++){ // Loop over crystals
    int nLines = myManager->update(32, i, entry);
    EBDetId ebid(1,i,EBDetId::SMCRYSTALMODE);
    if(nLines==1)
      {
	std::cout << i << "->" << entry->getConstant() << std::endl;
	ical->setValue( ebid.rawId(), entry->getConstant() );
    } else {
      ical->setValue( ebid.rawId(), 1. );
    }
  }


  EcalADCToGeVConstant* agc = new EcalADCToGeVConstant(0.0375, 0.);

//   H4DBEntry *myAlignmentEntry
//     =myManager->getEntry("H4Alignment", 1,925);

//   for (int i=1; i<2; i++){  // Loop over versions
//     cout << "Starting with version " << i << " ... ";
//     cout.flush();
//     for (int j=926; j <= 1700; j++){
//       myManager->update(i, j, myAlignmentEntry);
//       //      myAlignmentEntry->print();
//     }
//   }

//   H4DBEntry *myShapeParamEntry
//     =myManager->getEntry("H4ShapeParam", 0, 1);
//   for (int i = 0; i < 8; i++) {
//     cout << "Starting with version " << i << endl;
//     for (int j = 1; j < 6; j++) {
//       int r = i;
//       cout << "Event type:" << j << endl;
//       if (i == 1) r = 67740;
//       if (i == 2) r = 67940;
//       if (i == 3) r = 70300;
//       if (i == 4) r = 70381;
//       if (i == 5) r = 72744;
//       if (i == 6) r = 72800;
//       if (i == 7) r = 73095;
//       myManager->update(r, j, myShapeParamEntry);
// //       if (myShapeParamEntry)
// // 	myShapeParamEntry->print();
//     }
//   }

//   H4DBEntry *myTdcEntry
//     =myManager->getEntry("H4TdcRange", 0,1);
//   for (int j = 2; j < 6; j++) {
//     cout << "Event type:" << j << endl;
//     myManager->update(0, j, myTdcEntry);
    
//   }

//   for (int i = 64036; i < maxRunNumber; i++) {
//     if(myManager->update(i, H4DBEntry::NoSuchSubindex, myTdcEntry)<1)
//       continue;
//     for (int j=1; j<6 ; j++){
//       cout<<"Event type:"<<j<<endl ;
//       myManager->update(i, j, myTdcEntry);
// //       H4TdcRange* t=dynamic_cast<H4TdcRange*>(myTdcEntry);
// //       t->print();
//     }
//   }

  EcalGainRatios* gratio = new EcalGainRatios;  
  H4DBEntry *myGainsEntry
    =myManager->getEntry("H4Gains", 1,-1);
  //  for (int i=0; i<4; i++){
  //int entries=myManager->update(i, H4DBEntry::NoSuchSubindex, myGainsEntry);
  for (int j=1; j<=1700 ; j++)
    {
      myManager->update(3, j, myGainsEntry);

      if (!myGainsEntry)
	continue;

      EBDetId ebid(1,j,EBDetId::SMCRYSTALMODE);
      EcalMGPAGainRatio gr;
      gr.setGain12Over6( ((H4Gains*)myGainsEntry)->getRatio5() );
      gr.setGain6Over1( ((H4Gains*)myGainsEntry)->getRatio1() / ((H4Gains*)myGainsEntry)->getRatio5() );
      gratio->setValue( ebid.rawId(), gr );
      //	myGainsEntry->print();
    }
  //  }
  
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
  
  EcalPedestals::Item item;
  
  EcalPedestals* ped = new EcalPedestals();
  
  H4Geom g;
  g.init();
  
  for (int i=0;i<68;i++)
    for (int j=0;j<25;j++)
      {
	std::cout << "++++++++++++++++++++++++++++++++++" << std::endl;
	std::cout << "Tower " << i << " Channel " << j << std::endl;
	item.mean_x1  = tped->getMean(i+1,j,2);
	item.rms_x1   = tped->getRMS(i+1,j,2);  
	item.mean_x6  = tped->getMean(i+1,j,1);	
	item.rms_x6   = tped->getRMS(i+1,j,1);  
	item.mean_x12 = tped->getMean(i+1,j,0);	
	item.rms_x12  = tped->getRMS(i+1,j,0);  
	EBDetId myId(1,g.getSMCrystalNumber(i+1,j),EBDetId::SMCRYSTALMODE);
	ped->m_pedestals.insert(std::make_pair(myId.rawId(),item));
	std::cout << "++++++++++++++++++++++++++++++++++" << std::endl;
      }
  
  // create weights for the test-beam
  EcalTBWeights* tbwgt = new EcalTBWeights();
  // create groups of crystals - make one group for each crystal for ENTIRE barrel
  EcalWeightXtalGroups* xtalGroups = new EcalWeightXtalGroups();

  //Standard weights for pedestal
  std::vector<EcalWeight> vped;
  vped.push_back( EcalWeight(0.333) );
  vped.push_back( EcalWeight(0.333) );
  vped.push_back( EcalWeight(0.333) );
  vped.push_back( EcalWeight(0.000) );
  vped.push_back( EcalWeight(0.000) );
  vped.push_back( EcalWeight(0.000) );
  vped.push_back( EcalWeight(0.000) );
  vped.push_back( EcalWeight(0.000) );
  vped.push_back( EcalWeight(0.000) );
  vped.push_back( EcalWeight(0.000) );  

  // default weights to reco jitter
  std::vector<EcalWeight> vjitt;
  vjitt.push_back( EcalWeight(0.000) );
  vjitt.push_back( EcalWeight(0.000) );
  vjitt.push_back( EcalWeight(0.000) );
  vjitt.push_back( EcalWeight(0.800) );
  vjitt.push_back( EcalWeight(0.800) );
  vjitt.push_back( EcalWeight(0.800) );
  vjitt.push_back( EcalWeight(0.200) );
  vjitt.push_back( EcalWeight(0.300) );
  vjitt.push_back( EcalWeight(0.300) );
  vjitt.push_back( EcalWeight(0.300) );

  // open ascii files with values of weights
  std::ifstream fAmpWeights("ampWeights.txt");
  if (!fAmpWeights)
    exit(-1);
  std::cout << "Weight file open" << std::endl;

  while (!fAmpWeights.eof()) {

    int iSM, iCry, nSamples, nTdcBins;
    fAmpWeights >> iSM >> iCry >> nSamples >> nTdcBins;
    //    cout << iSM << " " << iCry << " " << nSamples << " " << nTdcBins << " " << std::endl;
    EBDetId ebid(iSM,iCry,EBDetId::SMCRYSTALMODE);
    int igrp = ebid.hashedIndex(); // one group for each xtal
    
    //    for(int itdc = 1; itdc <= nTdcBins; ++itdc) 
    for(int itdc = 1; itdc <= nTdcBins; ++itdc) 
      {
	EcalWeightSet wgt; // one set of weights
	EcalWeightSet::EcalWeightMatrix& wgt1 = wgt.getWeightsBeforeGainSwitch();
	EcalWeightSet::EcalWeightMatrix& wgt2 = wgt.getWeightsAfterGainSwitch();
	
	EcalWeightSet::EcalWeightMatrix& chisq1 = wgt.getChi2WeightsBeforeGainSwitch();
	EcalWeightSet::EcalWeightMatrix& chisq2 = wgt.getChi2WeightsAfterGainSwitch();
	
	std::vector<EcalWeight> wamp, wped, wtime; //weights
	
	double r = (double)std::rand()/( double(RAND_MAX)+double(1) );
	for(int j = 0; j < nSamples; ++j) {
	  float ww;
	  
	  // weights for amplitude reco
	  fAmpWeights >> ww;
	  //	  cout << ww << endl;
	  wamp.push_back(EcalWeight(ww));
	  
	  //         // weights for  ampli+ped reco
	  //         fPedWeights >> ww;
	  //         wped.push_back(EcalWeight(ww));
	  
	  //         // no weights yet fir jitter
	  //         wtime.push_back(0.);
	  
	  // fill chi2 matrix
// 	  std::vector<EcalWeight> vChi2; // row of chi2 matrix
// 	  for(int iRow = 0; iRow < nSamples; ++iRow) 
// 	    {
// 	      vChi2.push_back(EcalWeight(igrp*itdc*r + j*10. + iRow));
// 	    }
// 	  chisq1.push_back(vChi2);
// 	  chisq2.push_back(vChi2);    
	}
	
	// use same weights before and after gain switch (?)
	wgt1.push_back(wamp);
	wgt1.push_back(vped);
	wgt1.push_back(vjitt);
	
	wgt2.push_back(wamp);
	wgt2.push_back(vped);
	wgt2.push_back(vjitt);
	
	//Fill Weight Matrix
	tbwgt->setValue(std::make_pair(igrp,itdc), wgt);
      }
    //Fill Group Map
    xtalGroups->setValue(ebid.rawId(), EcalXtalGroupId( ebid.hashedIndex()) );
  }  
  
  cout << "Setting up Offline DB Connection..." << flush;
  cond::ServiceLoader* loader = new cond::ServiceLoader;
  loader->loadMessageService(cond::Error);
  //  loader->loadAuthenticationService(cond::XML);  // unneccessary?
  
  string contact("sqlite_file:ecal2004condDB.db");
  cond::DBSession* session = new cond::DBSession(contact);
  cond::MetaData* metadata_svc = new cond::MetaData(contact, *loader);
  cout << "Done." << endl;
  
  try {
    cout << "Making Connections..." << flush;
    session->connect(cond::ReadWriteCreate);
    metadata_svc->connect();
    cout << "Done." << endl;
    
    cout << "Creating Writers..." << flush;
    cond::DBWriter pedWriter(*session, "EcalPedestals");
    cond::DBWriter grpWriter(*session, "EcalWeightXtalGroups");
    cond::DBWriter tbwgtWriter(*session, "EcalTBWeights");
    cond::DBWriter agcWriter(*session, "EcalADCToGeVConstant");
    cond::DBWriter grWriter(*session, "EcalGainRatios");
    cond::DBWriter icalWriter(*session, "EcalIntercalibConstants");
    cond::DBWriter iovWriter(*session, "IOV");
    cout << "Done." << endl;
    
    cout << "Starting Transaction..." << endl;
    session->startUpdateTransaction();
    cout << "Done." << endl;
    
    std::string gr_tok = grWriter.markWrite<EcalGainRatios>(gratio);//pool::Ref takes the ownership of ped1
    std::string ical_tok = icalWriter.markWrite<EcalIntercalibConstants>(ical);//pool::Ref takes the ownership of ped1
    
    // create IOV for EcalGainRatios
    cond::IOV* gr_iov=new cond::IOV;
    gr_iov->iov.insert( std::make_pair(edm::IOVSyncValue::endOfTime().eventID().run(),gr_tok) );
    std::string gr_iov_Token = iovWriter.markWrite<cond::IOV>(gr_iov);
    std::cout << "Gain ratios stored in db with IoV" << std::endl;

    // create IOV for ADC->Gev scale
    cond::IOV* agc_iov =new cond::IOV;
    std::string agc_tok = agcWriter.markWrite<EcalADCToGeVConstant>(agc);
    agc_iov->iov.insert(std::make_pair(edm::IOVSyncValue::endOfTime().eventID().run(),agc_tok)); // run number
    std::string agc_iov_Token = iovWriter.markWrite<cond::IOV>(agc_iov);
    std::cout << "ADC->GeV scale stored in DB with IoV" << std::endl;    

    // create IOV for EcalIntercalibConstants
    cond::IOV* ical_iov=new cond::IOV;
    ical_iov->iov.insert( std::make_pair(edm::IOVSyncValue::endOfTime().eventID().run(),ical_tok) );
    std::string ical_iov_Token = iovWriter.markWrite<cond::IOV>(ical_iov);
    
    std::cout << "Intercalib constants stored in DB with IoV" << std::endl;
    
    std::string grp_tok = grpWriter.markWrite<EcalWeightXtalGroups>(xtalGroups);
    
    //    iov for groups of xtals
    cond::IOV* grp_iov = new cond::IOV;
    grp_iov->iov.insert(std::make_pair(edm::IOVSyncValue::endOfTime().eventID().run(),grp_tok));
    std::string grp_iov_Token = iovWriter.markWrite<cond::IOV>(grp_iov);
    std::cout << "Map of xtals put in the DB with IoV" << std::endl; 

    //    create and store IOV for EcalTBWeights
    std::string tbwgt_tok = tbwgtWriter.markWrite<EcalTBWeights>(tbwgt);
    cond::IOV* tbwgt_iov =new cond::IOV;
    tbwgt_iov->iov.insert(std::make_pair(edm::IOVSyncValue::endOfTime().eventID().run(),tbwgt_tok));
    std::string tbwgt_iov_Token = iovWriter.markWrite<cond::IOV>(tbwgt_iov);
    std::cout << "Weights for the TB put in DB with IoV" << std::endl;
 
    std::string pedTok = pedWriter.markWrite<EcalPedestals>(ped);
    cond::IOV* pedIOV = new cond::IOV;
    //    pedIOV->iov.insert(std::make_pair(tpedutil->getPedestalRun(minRunNumber) , pedTok));
    pedIOV->iov.insert(std::make_pair(edm::IOVSyncValue::endOfTime().eventID().run() , pedTok));
    std::string pedIOVTok = iovWriter.markWrite<cond::IOV>(pedIOV);
    std::cout << "Pedestals put in DB with IoV" << std::endl;


    cout << "Commit..." << flush;
    session->commit();
    cout << "Disconnection Session..." << endl;
    session->disconnect();
    delete session;
    cout << "Done." << endl;
    cout << "Registering to MetaData Service..." << flush;
    metadata_svc->addMapping("EcalWeightXtalGroups", grp_iov_Token);
    metadata_svc->addMapping("EcalTBWeights", tbwgt_iov_Token);
    metadata_svc->addMapping("EcalPedestals", pedIOVTok);
    metadata_svc->addMapping("EcalGainRatios", gr_iov_Token);
    metadata_svc->addMapping("EcalIntercalibConstants", ical_iov_Token);
    metadata_svc->addMapping("EcalADCToGeVConstant", agc_iov_Token);
    cout << "Done." << endl;
    cout << endl;
    cout << "Disconnecting MetaData Service..." << flush;
    metadata_svc->disconnect();
    delete metadata_svc;
    cout << "Done." << endl;
    
    delete loader;
    
  } catch(cond::Exception &e) {
    cout << e.what() << endl;
  } catch(...) {
    cout << "Unknown exception" << endl;
  } // try


  
  delete myManager;
  delete tpedutil;
  delete tped;
  
  return 0;
}

