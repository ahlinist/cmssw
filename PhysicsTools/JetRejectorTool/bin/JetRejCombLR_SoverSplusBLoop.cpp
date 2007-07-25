#include <iostream>
#include <cassert>
#include <TROOT.h>
#include <TSystem.h>
#include <Cintex/Cintex.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TF1.h>
#include <TFormula.h>
#include <TStyle.h>
#include <TKey.h>
#include <vector>
#include "FWCore/FWLite/src/AutoLibraryLoader.h"
#include "TopQuarkAnalysis/TopTools/interface/LRHelpFunctions.h"
#include "AnalysisDataFormats/TopObjects/interface/JetRejObs.h"
#include "DataFormats/JetReco/interface/CaloJet.h"

using namespace std;
using namespace reco;


///////////////////////
// Constants         //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//input files
const  int       nrFiles  	  		= 1;
const  TString   path     	  		= "../test/JetRejObsProducer.root";

//matching variables
const  bool  	 useSpaceAngle    		= true;
const  double 	 SumAlphaCut  	  		= 0.7;
//@@//const  double 	 DRCut  	  		= 0.3;


//observable histogram variables (include all defined observables!!!)
const  int      nrJetCombObs  			= 13;
const  int      JetCombObs[nrJetCombObs] 	= {1,2,3,4,5,6,7,8,9,10,11,12,13};
const  int   	nrJetCombHistBins    		= 50;
const  double   JetCombObsMin[nrJetCombObs]	= {  0,    0, -0.5,  0, -3, -3,  0,   0,   0, 0,   0, 0,  0};
const  double   JetCombObsMax[nrJetCombObs]	= {5.2, 1.05,  6.5, 50, 50, 50, 50,   1, 0.4, 4, 1.1, 5, 50};


//observable fit functions
const char*     JetCombObsFits[nrJetCombObs] 	= {  "[0]/(1 + 1/exp([1]*([2] - x)))",  //obs1	
						     "[0]/(1 + 1/exp([1]*([2] - x)))",  //obs2	
						     "[0]/(1 + 1/exp([1]*([2] - x)))",  //obs3
						     "[0]/(1 + 1/exp([1]*([2] - x)))", //obs4
						     "[0]/(1 + 1/exp([1]*([2] - x)))", //obs5
						     "([0]+[3]*abs(x)/x)*(1-exp([1]*(abs(x)-[2])))",  //obs6	
						     "[0]/(1 + 1/exp([1]*([2] - x)))",  //obs7
						     "[0]/(1 + 1/exp([1]*([2] - x)))", //obs8
						     "landaun", //obs9
						     "[0]/(1 + 1/exp([1]*([2] - x)))", //obs10
						     "[0]/(1 + 1/exp([1]*([2] - x)))", //obs11
						     "landaun", //obs12
						     "[0]/(1 + 1/exp([1]*([2] - x)))" //obs13
						  };

//output files ps/root
const  TString  JetCombOutfileName   		= "../data/JetRejCombLRAllObs.root";
const  TString  JetCombPSfile     		= "../data/JetRejCombLRAllObs.ps";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//
// Global variables
//
LRHelpFunctions *myLRhelper;
void doEventloop();
vector<int> obsNrs;
vector<double> obsMin,obsMax;
vector<const char*> obsFits;




//
// Main analysis
//

int main() { 
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();
  
  
  // define all histograms & fit functions
  //to replace with something more elegant
  for(int j = 0; j < nrJetCombObs; j++){
    obsNrs.push_back(JetCombObs[j]);
    obsMin.push_back(JetCombObsMin[j]);
    obsMax.push_back(JetCombObsMax[j]);
    obsFits.push_back(JetCombObsFits[j]);
  }
  myLRhelper = new LRHelpFunctions(obsNrs, nrJetCombHistBins, obsMin, obsMax, obsFits); 
  
  vector<double> parsFobs6; 
  parsFobs6.push_back(0.8);
  parsFobs6.push_back(-0.1);
  parsFobs6.push_back(-0.8);
  parsFobs6.push_back(0.2);
  myLRhelper -> setObsFitParameters(6,parsFobs6);


  // fill signal and background contributions to S and B histograms
  doEventloop(); 
  
  // normalize the S and B histograms to construct the pdf's
  myLRhelper -> normalizeSandBhists();
  
  // produce and fit the S/S+N histograms
  myLRhelper -> makeAndFitSoverSplusBHists();
  
  // store histograms and fits in root-file
  myLRhelper -> storeToROOTfile(JetCombOutfileName);
     
  // make some control plots and put them in a .ps file
  myLRhelper -> storeControlPlots(JetCombPSfile);
}





//
// Loop over the events (with the definition of what is considered signal and background)
//

void doEventloop(){ 
  cout<<endl<<endl<<"**** STARTING EVENT LOOP ****"<<endl;
  int okEvents = 0, totNrEv = 0;
  for (int nr = 1; nr <= nrFiles; nr++) {
    TString ft = path; 
    //    ft += nr-1; 
    // ft += ".root";
    if (!gSystem->AccessPathName(ft)) {
      TFile *file = TFile::Open(ft);
      TTree * events = dynamic_cast<TTree*>( file->Get( "Events" ) );
      assert( events != 0 );
      TBranch * obsbranch = events->GetBranch( "JetRejObss_JRobs__MYTEST.obj" );
      assert( obsbranch != 0 );
      vector< JetRejObs > obs;
      obsbranch->SetAddress( & obs );
    
      //loop over all events in a file 
      for( int ev = 0; ev < events->GetEntries(); ++ ev ) {
        ++totNrEv;
        if((double)((totNrEv*1.)/1000.) == (double) (totNrEv/1000)) cout<< "  Processing event "<< totNrEv<<endl; 
	obsbranch->GetEntry( ev );
       
  		
	JetRejObs obsValue;
	double myJet= -1;
	vector<double> obsVals;
	for(unsigned int ll =0; ll<obs.size(); ll++){ // loop on the jets in the event!
	  obsValue = obs[ll]; // vector of observables for each jet!

	  obsVals.clear();
	  for(unsigned int j=0; j<obsValue.getSize(); j++){//loop su tutte le variabili
	    if(obsValue.getPair(j).first == 0 ) myJet = obsValue.getPair(j).second;
	    
	    for(int os = 0; os < nrJetCombObs; os++){ // loop sulle variabili scelte!
	      if(obsValue.getPair(j).first == JetCombObs[os]){
		obsVals.push_back(obsValue.getPair(j).second);
	      }
	    }//chiude loop sulle variabili scelte
	  }//chiude loop su tutte le variabili

	  // Fill the observables for each jet combination if a good matching exists
	  // signal: best matching solution
	  // background: all other solutions 
	  if(myJet == 1) {
	    myLRhelper -> fillToSignalHists(obsVals);
	    ++okEvents;
	  }
	  else
	    {
	      myLRhelper -> fillToBackgroundHists(obsVals);
	    }
	  
	}// close loop on the jets
        
      }//close loop over the events (ev)
      file->Close();
    }
    else
    {
      cout<<ft<<" doesn't exist"<<endl;
    }
  }
  cout<<endl<<"***********************  STATISTICS  *************************"<<endl;
  cout<<" Probability that a correct jet combination exists:"<<endl;
  cout<<" (fraction events with ";
  if(useSpaceAngle) cout<<"min SumAngle_jp < ";
  if(!useSpaceAngle) cout<<"min DR_jp < ";
  cout<<SumAlphaCut<<" )"<<endl;
  cout<<endl<<"                 "<<(100.*okEvents)/(1.*totNrEv)<<" %"<<endl;
  cout<<endl<<"******************************************************************"<<endl;
}
