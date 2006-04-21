// -*- C++ -*-
// 
// testAsciiDB.cpp
// 
// This is a small example programs, to test the ASCII DB 
// the H4Run class. We dump a few runs into an ASCII file, 
// re-read them using H4DNManager::readAsciiFile() and 
// make sure that we have not lost any information during 
// this operation.
// 
// last modified : $Date: 2003/08/26 16:28:25 $
// by :            $Author: nrl $
// 
#include <unistd.h>
#include <string>
#include <fstream>
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4DBManager.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4Run.h"



void usage()
{
  std::cerr << "usage: testAsciiDB [-v] [-s <start_value>] [-n <nentries>]" << std::endl;
  std::cerr << "                                      <rootDBFileName>    " << std::endl;
  exit(-1);
}


int main(int argc, char** argv)
{
  int i;
  char c;
  int verb=0;
  int startValue=63130;
  int nentries=100;
  while( (c=getopt(argc, argv, "vs:n:")) != -1 )
    switch(c) {
    case 's':
      startValue=atoi(optarg);
      break;
    case 'n':
      nentries=atoi(optarg);
      break;
    case 'v':
      verb=1;
      break;
    default:
      usage();
    }
  if(optind==argc)
    usage();
  string rootDBFileName = argv[optind];
  
  H4DBManager mgr(false, rootDBFileName.c_str());
  
  H4Run* runEntry=(H4Run*)mgr.getEntry("H4Run", startValue);
  if(!runEntry) {
    std::cout << "testAsciiDB: wrong start value val="
	 << startValue << std::endl;
    usage();
  }
  
  // loop over a few runs and dumps them into an ASCII file
  ofstream ofs("rootdbDump.txt");
  ofs << "class H4Run" << std::endl;
  for(i=0;i<nentries;i++) {
    int n = mgr.update(startValue+i, H4DBEntry::NoSuchSubindex, runEntry);
    if(n<1) continue;
    if(n>1) continue; // we don't care (but we certainly should)
    runEntry->dumpToAsciiFile(ofs);
  }
  ofs.close();
  if(verb) 
    std::cout << "testAsciiDB: dumped " << nentries << " events," << std::endl
	 << "             starting from evt# "
	 << startValue << " into file rootdbDump.txt" << std::endl;
  
  if(verb)
    std::cout << "testAsciiDB: now attempting to reread the ASCII file...";
  
  // Now, we want to re-read the ASCII file with a new H4DBManager
  // in write mode ... 
  H4DBManager* mgr_tmp = new H4DBManager;
  mgr_tmp->openDB(true, "rootdbDump.root");
  mgr_tmp->readAsciiFile("rootdbDump.txt");
  delete mgr_tmp;
  if(verb)
    std::cout << "testAsciiDB: done." << std::endl;
  
  if(verb)
    std::cout << "testAsciiDB: checking that no information was lost during the operation..."
	 << std::endl;
  
  H4DBManager mgr2(false, "rootdbDump.root");
  H4Run* runEntry2=(H4Run*)mgr2.getEntry("H4Run", startValue);
  if(!runEntry2) {
    std::cout << "testAsciiDB: unable to reread root database created from Ascii file" << std::endl;
    exit(-2);
  }
  
  for(i=0;i<nentries;i++) {
    int index = i+startValue;
    int n  = mgr.update(index, H4DBEntry::NoSuchSubindex, runEntry);
    int n2 = mgr2.update(index, H4DBEntry::NoSuchSubindex, runEntry2);
    
    if(n != n2) {
      std::cout << "testAsciiDB: ERROR rereading db, index=" << index
	   << " n,n2=" << n << "," << n2 << std::endl;
      continue;
    }
    
    if(runEntry->getRunNumber() != runEntry2->getRunNumber() ) {
      std::cout << "testAsciiDB: ERROR rereading db, index=" << index
	   << " runNumbers 1,2=" << runEntry->getRunNumber() 
	   << ", " << runEntry2->getRunNumber() << std::endl;
      continue;
    }
    
    if(runEntry->getSubRun() != runEntry2->getSubRun() ) {
      std::cout << "testAsciiDB: ERROR rereading db, index=" << index
	   << " subRuns 1,2=" << runEntry->getSubRun() 
	   << ", " << runEntry2->getSubRun() << std::endl;
      continue;
    }
    
    if(runEntry->getStartTime() != runEntry2->getStartTime() ) {
      std::cout << "testAsciiDB: ERROR rereading db, index=" << index
	   << " startTime 1,2=" << runEntry->getStartTime() 
	   << ", " << runEntry2->getStartTime() << std::endl;
      continue;
    }

    if(runEntry->getStopTime() != runEntry2->getStopTime() ) {
      std::cout << "testAsciiDB: ERROR rereading db, index=" << index
	   << " startTime 1,2=" << runEntry->getStopTime() 
	   << ", " << runEntry2->getStopTime() << std::endl;
      continue;
    }
    
    if(verb) {
      runEntry->print();
      runEntry2->print();
    }
  }
  
  if(verb)
    std::cout << "OK!" << std::endl;
}

