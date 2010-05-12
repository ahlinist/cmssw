// ROOT includes
#include <TROOT.h>
#include <TFile.h>

#include "RooFit.h"
#include "RooGlobalFunc.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooCategory.h"

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <math.h>

int main(int argc, char* argv[]) {

  char inputFileName[150];
  if ( argc < 2 ){
    std::cout << "missing argument: insert inputFile with list of root files" << std::endl; 
    return 1;
  }
  strcpy(inputFileName,argv[1]);
  char Buffer[500];
  char MyRootFile[2000];
  std::cout << "input: " << inputFileName << std::endl;
  ifstream *inputFile = new ifstream(inputFileName);

  /// WEIGHTS AND SCALE HERE
  float weights[] = {0.23,0.08,8.19,0.542};
  int scaleStats = 10;
  ///
  /// CATEGORIES HERE
  RooCategory MCType("MCType","Category of MC");
  MCType.defineType("PR",0);
  MCType.defineType("NP",1);
  MCType.defineType("BK",2);
  ///
  
  RooRealVar* MCweight = new RooRealVar("MCweight","Monte Carlo Weight",0.,1000.);

  TFile *theFile;
  TFile theOutput("totalDataSet.root","RECREATE");

  RooDataSet* thisData;
  RooDataSet* newData;
  RooDataSet* finalData;

  int nfiles=0;
  int nevents=0;

  while( !(inputFile->eof()) ){

    inputFile->getline(Buffer,500);
    if (!strstr(Buffer,"#") && !(strspn(Buffer," ") == strlen(Buffer))) { 
      sscanf(Buffer,"%s",MyRootFile);
      std::cout << "Merging " << MyRootFile << std::endl;

      theFile = TFile::Open(MyRootFile);
      thisData = (RooDataSet*)theFile->Get("data");

      RooDataSet* addVars = new RooDataSet("addVars","Weight and type",
					   RooArgList(*MCweight,MCType));

      const RooArgSet* thisRow = thisData->get(0);   
      newData = new RooDataSet("data","new data",*thisRow);

      for (Int_t iSamp = 0; iSamp < thisData->numEntries(); iSamp++)
	{
          nevents++;
          if (nevents%scaleStats == 0) {
	    thisRow = thisData->get(iSamp);
	    
	    RooCategory* myMatched = (RooCategory*)thisRow->find("matchType");
	    int isMatched = (int)(myMatched->getIndex());
	    int theMCType = 2;
	    if (isMatched && strstr(MyRootFile,"prompt")) theMCType = 0; 
	    if (isMatched && strstr(MyRootFile,"nonpr")) theMCType = 1;
	    
	    MCType.setIndex(theMCType);
	    MCweight->setVal(weights[nfiles]);          
	    
	    newData->add(*thisRow);
	    addVars->add(RooArgSet(*MCweight,MCType));
	  }
	}

      newData->merge(addVars);

      if (nfiles == 0) {
	finalData = new RooDataSet(*newData);
      } else {
	finalData->append(*newData);
      }

      nfiles++;
    }
  }

  theOutput.cd();
  finalData->setWeightVar(*MCweight);
  finalData->Write();
  theOutput.Close();
  inputFile->close();
  delete inputFile;
  
  return 0;

}
