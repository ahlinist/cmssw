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

   //binning
   float Min_E=10;
   int bin_1oE=100;
   float Max_SXoS1=0.5;
   float Min_SXoS1=-0.5;
   int bin_SXoS1=100;
   float Max_Nspikes=10;
   int bin_Nspikes=10;

   //1D histograms
   TH1F *h_N_HFspikesPerEv_L    = new TH1F ("h_N_HFspikesPerEv_L","h_N_HFspikesPerEv_L",bin_Nspikes,0,Max_Nspikes);
   TH1F *h_N_HFspikesPerEv_S    = new TH1F ("h_N_HFspikesPerEv_S","h_N_HFspikesPerEv_S",bin_Nspikes,0,Max_Nspikes);
   TH1F *h_N_HFspikesPerEv_TOT    = new TH1F ("h_N_HFspikesPerEv_TOT","h_N_HFspikesPerEv_TOT",bin_Nspikes,0,Max_Nspikes);
   
   h_N_HFspikesPerEv_L->Sumw2();
   h_N_HFspikesPerEv_S->Sumw2();
   h_N_HFspikesPerEv_TOT->Sumw2();

   TH1F *h_HFRecHitE_L = new TH1F("h_HFRecHitE_L","h_HFRecHitE_L",100,-5.,200.);
   TH1F *h_HFRecHitE_L_Flagged = new TH1F("h_HFRecHitE_L_Flagged","h_HFRecHitE_L_Flagged",100,-5.,200.);
   TH1F *h_HFRecHitE_S = new TH1F("h_HFRecHitE_S","h_HFRecHitE_S",100,-5.,200.);
   TH1F *h_HFRecHitE_S_Flagged = new TH1F("h_HFRecHitE_S_Flagged","h_HFRecHitE_S_Flagged",100,-5.,200.);
   
   h_HFRecHitE_L->Sumw2();
   h_HFRecHitE_L_Flagged->Sumw2();
   h_HFRecHitE_S->Sumw2();
   h_HFRecHitE_S_Flagged->Sumw2();
   
   map<UInt_t,TH1F*> h_HFRecHitE_L_ieta;
   map<UInt_t,TH1F*> h_HFRecHitE_L_Flagged_ieta;
   map<UInt_t,TH1F*> h_HFRecHitE_S_ieta;
   map<UInt_t,TH1F*> h_HFRecHitE_S_Flagged_ieta;
      
   //2D histograms
   TH2F *h2_N_HFspikes_L_ieta_iphi  = new TH2F ("h2_N_HFspikes_L_ieta_iphi","h2_N_HFspikes_L_ieta_iphi",13,28.5,41.5,72,0.5,72.5);
   TH2F *h2_N_HFspikes_S_ieta_iphi  = new TH2F ("h2_N_HFspikes_S_ieta_iphi","h2_N_HFspikes_S_ieta_iphi",13,28.5,41.5,72,0.5,72.5);
   
   TH2F *h2_HFRecHitE_L_vs_S = new TH2F ("h2_HFRecHitE_L_vs_S","h2_HFRecHitE_L_vs_S",100,-5.,200.,100,-5.,200.);
   TH2F *h2_HFRecHitE_vs_R_L = new TH2F ("h2_HFRecHitE_vs_R_L","h2_HFRecHitE_vs_R_L",120,-1.1,1.1,100,-5.,200.);
   TH2F *h2_HFRecHitE_vs_R_S = new TH2F ("h2_HFRecHitE_vs_R_S","h2_HFRecHitE_vs_R_S",120,-1.1,1.1,100,-5.,200.);
   
//    TH2F *h2_R_vs_iphi_L  = new TH2F ("h2_R_vs_iphi_L","h2_R_vs_iphi_L",72,0.5,72.5,120,-1.1,1.1);
//    TH2F *h2_R_vs_ieta_L  = new TH2F ("h2_R_vs_ieta_L","h2_R_vs_ieta_L",13,28.5,41.5,120,-1.1,1.1);
//    TH2F *h2_R_vs_iphi_S  = new TH2F ("h2_R_vs_iphi_S","h2_R_vs_iphi_S",72,0.5,72.5,120,-1.1,1.1);
//    TH2F *h2_R_vs_ieta_S  = new TH2F ("h2_R_vs_ieta_S","h2_R_vs_ieta_S",13,28.5,41.5,120,-1.1,1.1);

//    TH2F *h2_S9oS1_vs_1oE_L_AllIeta  = new TH2F ("h2_S9oS1_vs_1oE_L_AllIeta","h2_S9oS1_vs_1oE_L_AllIeta"
// 						,bin_1oE,0,1/Min_E, bin_SXoS1,Min_SXoS1,Max_SXoS1);
//    TH2F *h2_S9oS1_vs_1oE_S_AllIeta  = new TH2F ("h2_S9oS1_vs_1oE_S_AllIeta","h2_S9oS1_vs_1oE_S_AllIeta"
// 						,bin_1oE,0,1/Min_E, bin_SXoS1,Min_SXoS1,Max_SXoS1);
   
   /////////initialize variables
   
   //////////////////////////////
   ///// Goood Run List  ////////
   //////////////////////////////
   int goodruns[] = {123596, 123615, 123732, 123815, 123818,
		     123908, 124008, 124009, 124020, 124022,
		     124023, 124024, 124025, 124027, 124030,
		     124120};
                     //124120 at 2360 GeV
   
   int goodLSmin[] = {2, 70, 62, 8, 2,
                      2, 1, 1, 12, 66,
                      38, 2, 5, 24, 2,
                      1};
   
   int goodLSmax[] = {9999, 9999, 109, 9999, 42,
                      12, 1, 68, 94, 179,
                      9999, 83, 13, 9999, 9999,
                      9999};
   
   Long64_t nentries = fChain->GetEntriesFast();
   std::cout << "analysisClass::Loop(): nentries = " << nentries << std::endl;   


   
   ////// The following ~7 lines have been taken from rootNtupleClass->Loop() /////
   ////// If the root version is updated and rootNtupleClass regenerated,     /////
   ////// these lines may need to be updated.                                 /////    
   Long64_t nb = 0;

   for (Long64_t jentry=0; jentry<nentries;jentry++) 
     {
       Long64_t ientry = LoadTree(jentry);
       if (ientry < 0) break;
       nb = fChain->GetEntry(jentry);   
       
       if(jentry < 10 || jentry%1000 == 0) std::cout << "analysisClass::Loop(): jentry = " << jentry << std::endl;   
       ////////////////////// User's code starts here ///////////////////////

       //## Event selection --> see below under fillVariables 

       //## Check if the run is in the list of good runs
       int pass_GoodRunList = 0;
       if(isData==1)
	 {
	   for (int i = 0; i < sizeof(goodruns)/sizeof(int) ; i++) {
	     if (goodruns[i] == run && ls >= goodLSmin[i] && ls <= goodLSmax[i]) {
	       pass_GoodRunList = 1;
	       break;
	     }
	   }
	 }
       else if(isData == 0)
	 {
	   pass_GoodRunList = 1;
	 }

       //#################################################
       
       // Set the evaluation of the cuts to false and clear the variable values and filled status
       resetCuts();
       
       // Set the value of the variableNames listed in the cutFile to their current value
       

       //## Fill variables for Event selection
       if(isData==1)
	 {
	   fillVariableWithValue("passPhysDecl", PMTnoiseisPhysDeclared);
	   fillVariableWithValue("passBSCMinBias", PMTnoiseisBSCminBias);
	   fillVariableWithValue("passVetoBSChalo", !PMTnoiseisBSChalo);
	   fillVariableWithValue("passBPTX0", PMTnoiseisBPTX0);
	   fillVariableWithValue("passBeamScrap", PMTnoisePassesBeamScraping);
	   fillVariableWithValue("passGoodPV", PMTnoiseisPrimaryVertex);
	   fillVariableWithValue("passGoodRunList", pass_GoodRunList);
	 }

       if(isData==0)
	 {
	   fillVariableWithValue("passPhysDecl", 1);
	   fillVariableWithValue("passBSCMinBias", PMTnoiseisBSCminBias);
	   fillVariableWithValue("passVetoBSChalo", 1);
	   fillVariableWithValue("passBPTX0", 1);
	   fillVariableWithValue("passBeamScrap", PMTnoisePassesBeamScraping);
	   fillVariableWithValue("passGoodPV", PMTnoiseisPrimaryVertex);
	   fillVariableWithValue("passGoodRunList", pass_GoodRunList);
	 }

       //## Fill other variables
       //fillVariableWithValue("passPhysDecl", PMTnoiseisPhysDeclared);

       // Evaluate cuts (but do not apply them)
       evaluateCuts();
       
       // Fill histograms and do analysis based on cut evaluation
       
       if( passedCut("0") )
	 {
          
           //## Loop over HF rechits             

           int N_HFspikes_L=0;
           int N_HFspikes_S=0;
           int N_HFspikes_TOT=0;
         
           for (int i = 0; i<int(PMTnoiseRecHitET->size()); i++)
             {       

               double energy = PMTnoiseRecHitEnergy->at(i);
               double partenergy = PMTnoiseRecHitPartEnergy->at(i);
               int ieta = PMTnoiseRecHitIeta->at(i);
               int iphi = PMTnoiseRecHitIphi->at(i);
               int depth = PMTnoiseRecHitDepth->at(i);
               //R = |L|-|S|/|L|+|S|
               double R = ( fabs(energy) - fabs(partenergy) ) / ( fabs(energy) + fabs(partenergy) );
               if(depth==2)
                 R = R * -1;

               //## identify spikes a la Igor
               bool isPMThit = false;

               if( depth==1 && energy>(162.4-10.19*abs(ieta)+0.21*ieta*ieta) && R>0.98 ) {
                 isPMThit = true;
                 N_HFspikes_L++;
               } else if( depth==2 && energy>(130-6.61*abs(ieta)+0.1153*ieta*ieta) && R<-0.98 ) {
                 isPMThit = true;
                 N_HFspikes_S++;
               }

               if( depth==1 ) {
                 h_HFRecHitE_L->Fill( energy );
                 h2_HFRecHitE_vs_R_L->Fill( R, energy );
                 h2_HFRecHitE_L_vs_S->Fill( partenergy, energy );

                 if(h_HFRecHitE_L_ieta[abs(ieta)]) {
                   h_HFRecHitE_L_ieta[abs(ieta)]->Fill( energy );
                 } else {
                   h_HFRecHitE_L_ieta[abs(ieta)] = new TH1F(Form("h_HFRecHitE_L_ieta_%u",abs(ieta)),Form("h_HFRecHitE_L_ieta_%u",abs(ieta)),100,-5.,200.);
                   h_HFRecHitE_L_ieta[abs(ieta)]->Sumw2();
                   h_HFRecHitE_L_ieta[abs(ieta)]->Fill( energy );
                 }
               }
               if ( depth==1 && isPMThit ) {
                 h2_N_HFspikes_L_ieta_iphi->Fill(abs(ieta),iphi);
                 h_HFRecHitE_L_Flagged->Fill( energy );
                 
                 if(h_HFRecHitE_L_Flagged_ieta[abs(ieta)]) {
                   h_HFRecHitE_L_Flagged_ieta[abs(ieta)]->Fill( energy );
                 } else {
                   h_HFRecHitE_L_Flagged_ieta[abs(ieta)] = new TH1F(Form("h_HFRecHitE_L_Flagged_ieta_%u",abs(ieta)),Form("h_HFRecHitE_L_Flagged_ieta_%u",abs(ieta)),100,-5.,200.);
                   h_HFRecHitE_L_Flagged_ieta[abs(ieta)]->Sumw2();
                   h_HFRecHitE_L_Flagged_ieta[abs(ieta)]->Fill( energy );
                 }
               }
               if( depth==2 ) {
                 h_HFRecHitE_S->Fill( energy );
                 h2_HFRecHitE_vs_R_S->Fill( R, energy );
                 h2_HFRecHitE_L_vs_S->Fill( energy, partenergy );

                 if(h_HFRecHitE_S_ieta[abs(ieta)]) {
                   h_HFRecHitE_S_ieta[abs(ieta)]->Fill( energy );
                 } else {
                   h_HFRecHitE_S_ieta[abs(ieta)] = new TH1F(Form("h_HFRecHitE_S_ieta_%u",abs(ieta)),Form("h_HFRecHitE_S_ieta_%u",abs(ieta)),100,-5.,200.);
                   h_HFRecHitE_S_ieta[abs(ieta)]->Sumw2();
                   h_HFRecHitE_S_ieta[abs(ieta)]->Fill( energy );
                 }
               }
               if ( depth==2 && isPMThit ) {
                 h2_N_HFspikes_S_ieta_iphi->Fill(abs(ieta),iphi);
                 h_HFRecHitE_S_Flagged->Fill( energy );
                 
                 if(h_HFRecHitE_S_Flagged_ieta[abs(ieta)]) {
                   h_HFRecHitE_S_Flagged_ieta[abs(ieta)]->Fill( energy );
                 } else {
                   h_HFRecHitE_S_Flagged_ieta[abs(ieta)] = new TH1F(Form("h_HFRecHitE_S_Flagged_ieta_%u",abs(ieta)),Form("h_HFRecHitE_S_Flagged_ieta_%u",abs(ieta)),100,-5.,200.);
                   h_HFRecHitE_S_Flagged_ieta[abs(ieta)]->Sumw2();
                   h_HFRecHitE_S_Flagged_ieta[abs(ieta)]->Fill( energy );
                 }
               }
               
               /*
               //S9/S1
               float S9oS1=-99;
               if( PMTnoiseRecHitIsSeed->at(i) == 1 && PMTnoiseRecHitEnergy->at(i) > 0)
                 {
                   S9oS1 = ( PMTnoiseRecHitPartEnergy->at(i) 
                             + PMTnoiseRecHitSum4Long->at(i)
                             + PMTnoiseRecHitSum4Short->at(i) ) / PMTnoiseRecHitEnergy->at(i);
                 }

               //S4/S1
               float S4oS1=-99;
               if( PMTnoiseRecHitIsSeed->at(i) == 1 && PMTnoiseRecHitDepth->at(i)==1 && PMTnoiseRecHitEnergy->at(i) > 0) //L
                 {
                   S4oS1 = ( PMTnoiseRecHitPartEnergy->at(i) 
                             + PMTnoiseRecHitSum4Long->at(i) ) / PMTnoiseRecHitEnergy->at(i);
                 }
               if( PMTnoiseRecHitIsSeed->at(i) == 1 && PMTnoiseRecHitDepth->at(i)==2 && PMTnoiseRecHitEnergy->at(i) > 0) //S
                 {
                   S4oS1 = ( PMTnoiseRecHitPartEnergy->at(i) 
                             + PMTnoiseRecHitSum4Short->at(i) ) / PMTnoiseRecHitEnergy->at(i);
                 }
               */
               
               //S9/S1
//                if( PMTnoiseRecHitIsSeed->at(i) == 1 && PMTnoiseRecHitEnergy->at(i) > Min_E )
//                  {
//                    float S9oS1 = ( PMTnoiseRecHitPartEnergy->at(i) 
//                                    + PMTnoiseRecHitSum4Long->at(i)
//                                    + PMTnoiseRecHitSum4Short->at(i) ) / PMTnoiseRecHitEnergy->at(i);
//                                   
//                    if( PMTnoiseRecHitDepth->at(i)==1 )
//                      h2_S9oS1_vs_1oE_L_AllIeta->Fill( 1/PMTnoiseRecHitEnergy->at(i) , S9oS1 );
// 
//                    if( PMTnoiseRecHitDepth->at(i)==2 )
//                      h2_S9oS1_vs_1oE_S_AllIeta->Fill( 1/PMTnoiseRecHitEnergy->at(i) , S9oS1 );
// 
//                  }
               
               //## identify spikes a la caloMET
               
//                if( PMTnoiseRecHitDepth->at(i)==1 
//                    && (PMTnoiseRecHitET->at(i) + ETpart) > 5 && R > 0.99)
//                  N_HFspikes_L++;
//                
//                if( PMTnoiseRecHitDepth->at(i)==2  
//                    && (PMTnoiseRecHitET->at(i) + ETpart) > 5 && R < -0.8)
//                  N_HFspikes_S++;

               
             }//end loop over hf rechits

           N_HFspikes_TOT = N_HFspikes_L + N_HFspikes_S;
          
	   h_N_HFspikesPerEv_L->Fill(N_HFspikes_L);
	   h_N_HFspikesPerEv_S->Fill(N_HFspikes_S);
	   h_N_HFspikesPerEv_TOT->Fill(N_HFspikes_TOT);

	 }//end pass all cut level 0

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

   h_N_HFspikesPerEv_L->Write();
   h_N_HFspikesPerEv_S->Write();
   h_N_HFspikesPerEv_TOT->Write();

   h_HFRecHitE_L->Write();
   h_HFRecHitE_L_Flagged->Write();
   h_HFRecHitE_S->Write();
   h_HFRecHitE_S_Flagged->Write();
   
   for(map<UInt_t,TH1F*>::const_iterator it = h_HFRecHitE_L_ieta.begin(); it != h_HFRecHitE_L_ieta.end(); it++) it->second->Write();
   for(map<UInt_t,TH1F*>::const_iterator it = h_HFRecHitE_L_Flagged_ieta.begin(); it != h_HFRecHitE_L_Flagged_ieta.end(); it++) it->second->Write();
   for(map<UInt_t,TH1F*>::const_iterator it = h_HFRecHitE_S_ieta.begin(); it != h_HFRecHitE_S_ieta.end(); it++) it->second->Write();
   for(map<UInt_t,TH1F*>::const_iterator it = h_HFRecHitE_S_Flagged_ieta.begin(); it != h_HFRecHitE_S_Flagged_ieta.end(); it++) it->second->Write();
   
   h2_N_HFspikes_L_ieta_iphi->Write();
   h2_N_HFspikes_S_ieta_iphi->Write();
   
   h2_HFRecHitE_L_vs_S->Write();
   h2_HFRecHitE_vs_R_L->Write();
   h2_HFRecHitE_vs_R_S->Write();
   
//    h2_R_vs_iphi_L->Write();
//    h2_R_vs_ieta_L->Write();
//    h2_R_vs_iphi_S->Write();
//    h2_R_vs_ieta_S->Write();

//    h2_S9oS1_vs_1oE_L_AllIeta->Write();
//    h2_S9oS1_vs_1oE_S_AllIeta->Write();

   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}
