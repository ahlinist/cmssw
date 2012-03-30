/*****************************************************************************

TurboSim

Usage:  turboSim -create turboSimMorphisms.txt partonEvents.txt recoEvents.txt
   or   turboSim turboSimMorphisms.txt partonEvents.txt recoEvents.txt
   or   turboSim -convert partonEvents.stdhep partonEvents.txt
   turboSim -convertHepevt partonEvents.hepevt partonEvents.txt
   turboSim -prepareconvertHepevt partonEvents.hepevt partonEvents.txt

Purpose:  perform a fast simulation
Primary author:   Bruce Knuteson, knuteson@mit.edu
Secondary author:  Khaldoun Makhoul, khaled@mit.edu
Editor: David Friend, friend@mit.edu
First implementation: Summer 2003, at Fermilab and the University of Chicago 

*****************************************************************************/


#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Validation/VstQuaeroUtils/interface/QuaeroItemFile.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroEvent.hh"
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstTurboSim/interface/QuaeroPartonEvent.hh"
#include "Validation/VstTurboSim/interface/TurboSimMorphisms1.hh"
#include "Validation/VstTurboSim/interface/TurboSimMorphisms2.hh"
#include "Validation/VstTurboSim/interface/stdhep2partonWrap.hh"
#include "Validation/VstTurboSim/interface/hepevt2parton.hh"
#include <unistd.h>
#include <cassert>
using namespace std;

void turboSim(string, ofstream &, TurboSimMorphisms1 &);
void replace(string &, string, string);

int main(int argc, char* argv[]) {

  /*
    USAGES:
      turboSim -create morphisms.txt partonEvents.txt recoEvents.txt
        Creates a TurboSim Morphisms table from parton and reco events and
        puts the data into morphisms.txt file.
      turboSim morphisms.txt partonEvents.txt [pe2.txt . . .] recoEvents.txt
        Morphs parton events into reconstructed events using the TurboSim
        Morphisms table (morphisms.txt). Reconstructed events are saved
        in recoEvents.txt.
      turboSim -convert partonEvents.stdhep partonEvents.txt
        Converts .stdhep events file into .txt file understandable by
        TurboSim.
      turboSim -convertHepevt partonEvents.hepevt partonEvents.txt
        Converts .hepevt events file (our trimmed version of the hepevt format)
	into .txt file understandable by TurboSim.
      turboSim -prepareconvertHepevt partonEvents.hepevt partonEvents.txt
        Converts .hepevt events file (this is the format coming directly from
	HEPEVT Common Block - it contains brackets and commas and so on) into 
	.txt file understandable by TurboSim.
  */

  if (argc < 4) {
    cout << "Usage:  turboSim -create turboSimMorphisms.txt partonEvents.txt recoEvents.txt " << endl
	 << "   or   turboSim turboSimMorphisms.txt partonEvents.txt recoEvents.txt" << endl
	 << "   or   turboSim -convert partonEvents.stdhep partonEvents.txt" << endl
	 << "   or   turboSim -convertHepevt partonEvents.hepevt partonEvents.txt" << endl
	 << "   or   turboSim -prepareconvertHepevt partonEvents.hepevt partonEvents.txt" << endl ;
    return 0;
  }

//  assert(argc >= 4);  //Throws error if invalid number of arguments is passed to TurboSim.

  cout << "TurboSim" << endl;

  //Establish turboSimMorphismsType

  /*  There are two versions of the TurboSim algorithm -- 1 and 2. Default is 
      v.1, but can run v.2 by setting environmental variable,
      TurboSimMorphismsType. V.2 differs from v.1 in its treatement of 
      Doublets. */

  int turboSimMorphismsType = 2;

  if (getenv("TurboSimMorphismsType") != NULL) {
    turboSimMorphismsType = atoi(getenv("TurboSimMorphismsType"));
    assert( turboSimMorphismsType == 1 || turboSimMorphismsType == 2 );
  }

  cout << "Using TurboSimMorphismsType " << turboSimMorphismsType << endl;

  // -convert 

  if (((string) argv[1]) == "-convert") {
    assert(argc == 4);
    string stdhepFileName = argv[2];
    string txtFileName = argv[3];
    //char* stdhepFileNameC = (char*)stdhepFileName.c_str();
    //char* txtFileNameC = (char*)txtFileName.c_str();
//    stdhep2partonWrap(stdhepFileName, txtFileName);
  }
  
  
  else if(((string)argv[1])=="-convertHepevt") {
    assert(argc == 4);
    string hepevtFileName = argv[2];
    string partonFileName = argv[3];
    //    hepevt2parton(hepevtFileName, partonFileName);
  }      
  
  else if(((string)argv[1])=="-prepareconvertHepevt") {
    assert(argc == 4);
    string hepevtFileName = argv[2];
    string partonFileName = argv[3];
    //    hepevt_prepare(hepevtFileName, hepevtFileName+"_temp");
    //    hepevt2parton(hepevtFileName+"_temp", partonFileName);
    //    hepevt2parton(hepevtFileName, partonFileName);

    //    system(("rm " + hepevtFileName+"_temp").c_str());
  }  

  // -create

  else if (((string) argv[1]) == "-create") {
    assert(argc == 5);
    string partonFileName = argv[3];
    string recoFileName = argv[4];
    string morphismsFileName = argv[2];
    cout << "Creating TurboSim table from " << partonFileName << " and " << recoFileName << " . . . " << endl;

    if (turboSimMorphismsType == 1) {
      TurboSimMorphisms1 turboSimMorphisms;
      turboSimMorphisms.create(partonFileName, recoFileName, morphismsFileName);
    }
    if (turboSimMorphismsType == 2) {
      TurboSimMorphisms2 turboSimMorphisms;
      turboSimMorphisms.create(partonFileName, recoFileName, morphismsFileName);
    }
  }

  // morph partonEvents

  else {
    
    assert(argc >= 4);
    
    string morphismsFileName = argv[1];
    string outputFileName = argv[argc - 1];
    vector<string> inputFileNames;
    
    for (int i = 2; i < argc - 1; i++)
	inputFileNames.push_back(argv[i]);
  
    // Version 1

    if (turboSimMorphismsType == 1) {
      
      TurboSimMorphisms1 turboSimMorphisms;
      
      // Read in Morphisms table from file
      turboSimMorphisms.readFromFile(morphismsFileName);
      
      // Initialize output file stream
      ofstream fout(outputFileName.c_str());
      
      // Convert each partonEvents file using turboSim(...)
      for( vector<string>::const_iterator it = inputFileNames.begin();
	   it != inputFileNames.end(); ++it) {
	cout << "Morphing " << *it << " . . . "<< endl;
	turboSim(*it, fout, turboSimMorphisms);
      }
      
      fout.close();
    }

    // Version 2

    if (turboSimMorphismsType == 2) {
      TurboSimMorphisms2 turboSimMorphisms;
      turboSimMorphisms.morph(morphismsFileName, inputFileNames, outputFileName);      
    }
  }

  return 0;

}


void turboSim(string partonFileName, 
	      ofstream& fout, 
	      TurboSimMorphisms1& turboSimMorphisms) {
  /*
    ABSTRACT:
      Morphs parton events stored in external file into reconstructed events. 
      Reconstructed events are streamed to output file stream.
    INPUT:
      partonFileName - name of file containing parton level events
      fout - valid output file stream
      turboSimMorphisms - parton to reco mapping data
    RETURNS: 
      void
  */
 
  QuaeroItemFile<QuaeroPartonEvent> partonFile(partonFileName);
  QuaeroPartonEvent partonEvent;
  QuaeroEvent quaeroEvent;
  
  while (partonFile.nextEvent(partonEvent)) {
    quaeroEvent = turboSimMorphisms.morph(partonEvent);
    fout << quaeroEvent << endl;
  }
  
  return;

}

void replace(string& s, string a, string b) {

  /* 
     ABSTRACT:
       String utility. Replaces occurances of _a_ in _s_ with _b_.
     INPUT:
       s - pointer to string of interest
       a - substring to search for
       b - replacement for substring
     RETURNS:
       void
       Modifies _s_.
  */
  
  size_t i = s.find(a);
  
  while (i < s.size()) {
    s.replace(i, a.length(), b);
    i = s.find(a);
  }

  return;

}



