// -*- C++ -*-
// 
// fillShapeParamDB.cpp
// 
// 
// last modified : $Date: 2003/08/28 09:19:09 $
// by :            $Author: paganini $
// 
#include <unistd.h>
#include <string>
#include <fstream>
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4DBManager.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4ShapeParam.h"



void usage()
{
  std::cerr << "usage: fillShapeParamDB [-v] [-d] [-s <start_value>] [-n <nentries>]" << std::endl;
  std::cerr << "                                      <fileName>    " << std::endl;
  std::cerr << " -v : verbose, -d : dump root db to ascii " << std::endl;
  exit(-1);
}


int main(int argc, char** argv)
{
  int i;
  char c;
  int verb=0;
  bool dump = false ;
  int startValue=63130;
  int nentries=1;
  while( (c=getopt(argc, argv, "vds:n:")) != -1 )
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
    case 'd':
      dump = true ;
      break;
    default:
      usage();
    }
  if(optind==argc)
    usage();
  string fileName = argv[optind];
  



  if (!dump)
    {
  
      // We read the ASCII file with a H4DBManager in write mode ... 
      H4DBManager* mgr = new H4DBManager;
      mgr->openDB(true, "rootdbDump.root");
      mgr->readAsciiFile(fileName.c_str());
      delete mgr;
      if(verb)
	std::cout << "fillShapeParamDB: done." << std::endl;

    }
  else 
    {
      
      H4DBManager mgr(false, fileName.c_str());      
      H4ShapeParam* shapeEntry=(H4ShapeParam*)mgr.getEntry("H4ShapeParam", startValue);
      if(!shapeEntry) {
	std::cout << "fillShapeParamDB: unable to read root database" << std::endl;
	exit(-2);
      }
      ofstream ofs("rootdbDump.txt");
      ofs << "class H4ShapeParam" << std::endl;
      for(i=0;i<nentries;i++) {
	int index = i+startValue;
	int n  = mgr.update(index, H4DBEntry::NoSuchSubindex, shapeEntry);
	cout<<n<<endl ;
	if(n<1) continue;
	if(n>1) continue; // we don't care (but we certainly should)
	if(verb) {
	  cout<<"==== entry: "<<i<<std::endl;
	  shapeEntry->print();
	}
	shapeEntry->dumpToAsciiFile(ofs);
	Float_t alpha, tPeak, tMax ;
	Int_t smCrystal = -1 ;
	if (shapeEntry->getShapeParam(smCrystal, alpha, tPeak, tMax))
	  cout<<"Crystal "<<smCrystal<<": "<<alpha<<", "<<tPeak<<", "<<tMax<<endl ;
	else
	  cout<<"nothing found"<<endl ;
      }
      ofs.close();    
    }
}


