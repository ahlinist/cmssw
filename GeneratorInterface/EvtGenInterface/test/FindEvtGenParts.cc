#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include <fstream.h>
#include <iostream.h>
#include <iomanip.h>
#include <math.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>

using namespace edm;

class FindEvtGenParts : public edm::EDAnalyzer {

 public:
  explicit FindEvtGenParts(const edm::ParameterSet& pset);
  ~FindEvtGenParts(){}

  virtual void analyze(const edm::Event& event, const edm::EventSetup& setup);
   
  protected:    
    std::string decFile;
    std::string pdtFile;
    std::string outFile;
};

FindEvtGenParts::FindEvtGenParts(const edm::ParameterSet& pset):
  decFile( pset.getParameter<std::string>( "decPath" ) ),
  pdtFile( pset.getParameter<std::string>( "pdtPath" ) ),
  outFile( pset.getUntrackedParameter<std::string>( "outFile","partlist.txt" ))
{}

void FindEvtGenParts::analyze(const edm::Event& event, const edm::EventSetup& setup) {

  FILE *decfile = fopen(decFile.c_str(), "r");
  FILE *pdtfile;
  
  ofstream *outfile = new ofstream(outFile.c_str());

  char firstWord[100];
  char partName[100];
  char add[100];
  char p[100];
  char partType[100];
  char partName2[100];
  char lundIdstr[100];
  char mass[100];
  char width[100];
  char dm[100];
  char spin[10];
  char charge[10];
  char lifetime[100];
  char lundKc[10];

  while (!feof(decfile)) {
     fscanf(decfile, "%s", firstWord) ;
     if ( !strcmp(firstWord,"Decay") || !strcmp(firstWord,"CDecay")) {
       fscanf(decfile, "%s", partName) ;
       pdtfile = fopen(pdtFile.c_str(), "r");
       while (!feof(pdtfile)) {
         fscanf(pdtfile, "%s", add);
	 if (!strcmp(add,"add")) {
           fscanf(pdtfile, "%s%s%s%s%s",  p, partType, partName2, lundIdstr, mass) ;
           fscanf(pdtfile, "%s%s%s%s%s%s", width, dm, spin, charge, lifetime, lundKc ) ;
	   // if ( !strcmp(partName,partName2) ) *outfile << lundKc << endl;
           if ( !strcmp(partName,partName2) ) *outfile << lundIdstr << endl;
	 }
       }
       fclose(pdtfile);
     }
  }
  
  fclose(decfile);
  
}

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(FindEvtGenParts);

