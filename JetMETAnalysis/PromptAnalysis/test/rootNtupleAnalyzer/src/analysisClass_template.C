#define analysisClass_cxx
#include "analysisClass.h"
#include <TH2.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TVector2.h>
#include <TVector3.h>

analysisClass::analysisClass(string * inputList, string * cutFile, string * treeName, string * outputFileName, string * cutEfficFile)
  :baseClass(inputList, cutFile, treeName, outputFileName, cutEfficFile)
{
  std::cout << "analysisClass::analysisClass(): begins " << std::endl;

  std::cout << "analysisClass::analysisClass(): ends " << std::endl;
}

analysisClass::~analysisClass()
{
  std::cout << "analysisClass::~analysisClass(): begins " << std::endl;

  std::cout << "analysisClass::~analysisClass(): ends " << std::endl;
}

void analysisClass::Loop()
{
   std::cout << "analysisClass::Loop() begins" <<std::endl;   
    
   if (fChain == 0) return;
   
   //////////book histos here

   // number of electrons
   TH1F *h_towersEta = new TH1F ("h_towersEta","",200,-5.,5.);
   //    TH1F *h_calometPt = new TH1F ("h_calometPt","",11,-0.5,10.5);
   //    h_nEleFinal->Sumw2();

   /////////initialize variables

   Long64_t nentries = fChain->GetEntriesFast();
   std::cout << "analysisClass::Loop(): nentries = " << nentries << std::endl;   

   ////// The following ~7 lines have been taken from rootNtupleClass->Loop() /////
   ////// If the root version is updated and rootNtupleClass regenerated,     /////
   ////// these lines may need to be updated.                                 /////    
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     if(jentry < 10 || jentry%1000 == 0) std::cout << "analysisClass::Loop(): jentry = " << jentry << std::endl;   
     // if (Cut(ientry) < 0) continue;

     ////////////////////// User's code starts here ///////////////////////

     ///Stuff to be done every event
     //cout << getPreCutValue1("eleFidRegion") << endl;

     //### Calculate MET on the fly from caloTowers ###
     double sum_et = 0.0;
     double sum_ex = 0.0;
     double sum_ey = 0.0;
     for (int i = 0; i<int(CaloTowersEmEt->size()); i++)
       {
	 double Tower_ET = CaloTowersEmEt->at(i) + CaloTowersHadEt->at(i);
	 if (Tower_ET>getPreCutValue1("towerETcut"))
	   {
	     double phi   = CaloTowersPhi->at(i);
	     double et    = CaloTowersEmEt->at(i) + CaloTowersHadEt->at(i);
	     sum_et += et;
	     sum_ex += et*cos(phi);
	     sum_ey += et*sin(phi);
	   }
       }

     double my_Met    = sqrt( sum_ex*sum_ex + sum_ey*sum_ey );
     double my_Met_x  = -sum_ex;
     double my_Met_y  = -sum_ey;
     double my_Sumet  = sum_et;
     double my_MetPhi = atan2( -sum_ey, -sum_ex );
     //#################################################

     // Set the evaluation of the cuts to false and clear the variable values and filled status
     resetCuts();
     
     // Set the value of the variableNames listed in the cutFile to their current value

     //      cout << "MET from towers: " << my_Met << ", MET stored in RECO " << calometPt->at(0) 
     //       	  << "    METPhi from towers: " << my_MetPhi << ", METPhi stored in RECO " << calometPhi->at(0) << endl;
     
     fillVariableWithValue("calometPt", calometPt->at(0));
     fillVariableWithValue("calometPhi", calometPhi->at(0));

     fillVariableWithValue("my_calometPt", my_Met);
     fillVariableWithValue("my_calometPhi", my_MetPhi);
    
     for (int ii=0; ii<CaloTowersEta->size(); ii++)
       h_towersEta->Fill( CaloTowersEta->at(ii));
     // Evaluate cuts (but do not apply them)
     evaluateCuts();
     
     // Fill histograms and do analysis based on cut evaluation

     // retrieve value of previously filled variables (after making sure that they were filled)
     //double diff;
     //if ( variableIsFilled("my_calometPt") && variableIsFilled("calometPt") ) 
     //  diff = getVariableValue("my_calometPt") - getVariableValue("calometPt");

     // reject events that did not pass level 0 cuts
     //if( !passedCut("0") ) continue;
     // ......     
     // reject events that did not pass level 1 cuts
     //if( !passedCut("1") ) continue;
     // ......
     // reject events that did not pass the full cut list
     //if( !passedCut("all") ) continue;
     // ......
     // reject events that did not pass the cut in parenthesis
     //if( !passedCut("my_calometPt") )  continue;      


     ////////////////////// User's code ends here ///////////////////////

   } // End loop over events

   //////////write histos 

   //TH1F * h_example = new TH1F ("h_example","", getHistoNBins("my_calometPt"), getHistoMin("my_calometPt"), getHistoMax("my_calometPt"));
   //h_example->Add( & getHisto_noCuts_or_skim("my_calometPt") ); // all histos can be retrieved, see other getHisto_xxxx methods in baseClass.h
   h_towersEta->Write();

   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}
