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

   //2D histograms
   TH2F *h_towersEtaPhi_EM  = new TH2F ("h_towersEtaPhi_EM","h_towersEtaPhi_EM",82,-41,41, 72,0,72);
   TH2F *h_towersEtaPhi_HAD = new TH2F ("h_towersEtaPhi_HAD","h_towersEtaPhi_HAD",82,-41,41, 72,0,72);
   TH2F *h_HCalNoiseLoose_Vs_MET = new TH2F ("h_HCalNoiseLoose_Vs_MET","h_HCalNoiseLoose_Vs_MET",2, 0,2, 200,0,200.);
   TH2F *h_HCalNoiseTight_Vs_MET = new TH2F ("h_HCalNoiseTight_Vs_MET","h_HCalNoiseTight_Vs_MET",2, 0,2, 200,0,200.);
   TH2F *h_HCalNoiseHighLevel_Vs_MET = new TH2F ("h_HCalNoiseHighLevel_Vs_MET","h_HCalNoiseHighLevel_Vs_MET",2, 0,2, 200,0,200);
 
   TH2F *h_ECHF_Vs_MET   = new TH2F ("h_ECHF_Vs_MET","h_ECHF_Vs_MET",100, 0,2, 200,0,200.);
   TH2F *h_EEMF_Vs_MET   = new TH2F ("h_EEMF_Vs_MET","h_EEMF_Vs_MET",100, 0,2, 200,0,200.);
   TH2F *h_BeamHalo_Vs_MET   = new TH2F ("h_BeamHalo_Vs_MET","h_BeamHalo_Vs_MET",12, 0,12, 200,0,200.);

   TH2F *h_MET_Vs_METPhi = new TH2F ("h_MET_Vs_METPhi","h_MET_Vs_METPhi",64, 0,3.2, 200,0,200.);
   TH2F *h_MET_Vs_SumET  = new TH2F ("h_MET_Vs_SumET","h_MET_Vs_SumET",200, 0,200, 200,0,200.);
   TH2F *h_MET_Vs_METX   = new TH2F ("h_MET_Vs_METX","h_MET_Vs_METX",200, 0,200, 200,-100,100.);
   TH2F *h_MET_Vs_METY   = new TH2F ("h_MET_Vs_METY","h_MET_Vs_METY",200, 0,200, 200,-100,100.);

   TH1F *h_AllVertexZ    = new TH1F ("h_AllVertexZ","h_AllVertexZ",100,-100,100);
   TH1F *h_AllVertexChi2 = new TH1F ("h_AllVertexChi2","h_AllVertexChi",100,0,100);
   TH1F *h_AllVertexNDOF = new TH1F ("h_AllVertexNDOF","h_AllVertexNDOF",50,0,50);
   TH1F *h_AllVertexChi2_0_NDOF = new TH1F ("h_AllVertexChi2_0_NDOF","h_AllVertexChi2_0_NDOF",200,0,40);
   TH1F *h_AllVertexNtrk = new TH1F ("h_AllVertexNtrk","h_AllVertexNtrk",50,0,50);

   h_AllVertexZ->Sumw2();
   h_AllVertexChi2->Sumw2();
   h_AllVertexNDOF->Sumw2();
   h_AllVertexChi2_0_NDOF->Sumw2();
   h_AllVertexNtrk->Sumw2();
   /////////initialize variables

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
       int BSC_techbit = 0;
       int BeamHalo    = 0;
       
       //Event Selections....
//        for (int ii = 40; ii<=41; ii++)
// 	 if(l1techbits->at(ii)==1)  { BSC_techbit = 1; break; }
//        if (BSC_techbit==0) continue;
      
//        for (int ii = 0; ii < int(BeamHaloSummaryId->size()); ii++)
// 	 if(BeamHaloSummaryId->at(ii) < 11) {BeamHalo = 1; break; }
//        if(BeamHalo==0) continue;
       
//        if(run==122314)
// 	 if(ls<24 || ls >37) continue;
       //Event selections ends
       
       //Calculate MET on the fly from caloTowers ###
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
       
       //Calculate MET from EM towers only
       sum_et = 0.0;
       sum_ex = 0.0;
       sum_ey = 0.0;
       for (int i = 0; i<int(CaloTowersEmEt->size()); i++)
	 {
	   double Tower_ET = CaloTowersEmEt->at(i);
	   if (Tower_ET>getPreCutValue1("towerETcut"))
	     {
	       double phi   = CaloTowersPhi->at(i);
	       double et    = CaloTowersEmEt->at(i);
	       sum_et += et;
	       sum_ex += et*cos(phi);
	       sum_ey += et*sin(phi);
	     }
	 }
       
       double my_MetEM    = sqrt( sum_ex*sum_ex + sum_ey*sum_ey );
       double my_MetEM_x  = -sum_ex;
       double my_MetEM_y  = -sum_ey;
       double my_SumetEM  = sum_et;
       double my_MetEMPhi = atan2( -sum_ey, -sum_ex );
  
       //Calculate MET from HAD towers only
       sum_et = 0.0;
       sum_ex = 0.0;
       sum_ey = 0.0;
       for (int i = 0; i<int(CaloTowersHadEt->size()); i++)
	 {
	   double Tower_ET = CaloTowersHadEt->at(i);
	   if (Tower_ET>getPreCutValue1("towerETcut"))
	     {
	       double phi   = CaloTowersPhi->at(i);
	       double et    = CaloTowersHadEt->at(i);
	       sum_et += et;
	       sum_ex += et*cos(phi);
	       sum_ey += et*sin(phi);
	     }
	 }
       
       double my_MetHAD    = sqrt( sum_ex*sum_ex + sum_ey*sum_ey );
       double my_MetHAD_x  = -sum_ex;
       double my_MetHAD_y  = -sum_ey;
       double my_SumetHAD  = sum_et;
       double my_MetHADPhi = atan2( -sum_ey, -sum_ex );
       
        //Calculate EEMF
       Double_t Et, Eta, sum_Et=0, eemf=0;
       for (int ii=0; ii<ic5JetpT->size(); ii++)
	 //	 if(ic5JetpT->at(ii)>20. && fabs(ic5JetEta->at(ii))<3.)
	   {
	     sum_Et+=ic5JetpT->at(ii);
	     eemf+=(ic5JetpT->at(ii))*(ic5JetEmEnergyFraction->at(ii));
	   }
       if (sum_Et!=0) eemf/=sum_Et;
            
       //Calculate ECHF
       Double_t echf=0;
       Int_t ngoodjets=0;
       for (int ii=0; ii<ic5JetpT->size(); ii++)
	 //	 if(ic5JetpT->at(ii)>20. && fabs(ic5JetEta->at(ii))<1.7)
	 {
	   Double_t sumpt=0;
	   ngoodjets++;

	   for (int jj=0; jj<tracksPt->size(); jj++)
	     {
	       Float_t dr=0;
	       
	       dr = sqrt(pow((acos(cos(ic5JetPhi->at(ii)-tracksPhi->at(jj)))),2.)+pow((ic5JetEta->at(ii)-tracksEta->at(jj)),2.));
	       if(dr<0.75)
		 sumpt += tracksPt->at(jj);
	     }
	   if (sumpt!=0) echf+=ic5JetpT->at(ii)/sumpt;
	 }

       if (ngoodjets!=0) echf/=ngoodjets;
       //#################################################
       
       // Set the evaluation of the cuts to false and clear the variable values and filled status
       resetCuts();
       
       // Set the value of the variableNames listed in the cutFile to their current value
       
       fillVariableWithValue("calometPt", calometPt->at(0));
       fillVariableWithValue("calometPhi", calometPhi->at(0));
       fillVariableWithValue("calometPx", calometPx->at(0));
       fillVariableWithValue("calometPy", calometPx->at(0));
       fillVariableWithValue("caloSumet", calometSumEt->at(0));
       fillVariableWithValue("calometSig", calometPt->at(0)/sqrt(calometSumEt->at(0)));

       fillVariableWithValue("tcmetPt", tcmetPt->at(0));
       fillVariableWithValue("tcmetPhi", tcmetPhi->at(0));
       fillVariableWithValue("tcmetPx", tcmetPx->at(0));
       fillVariableWithValue("tcmetPy", tcmetPx->at(0));
       fillVariableWithValue("tcSumet", tcmetSumEt->at(0));
       fillVariableWithValue("tcmetSig", tcmetPt->at(0)/sqrt(tcmetSumEt->at(0)));

       fillVariableWithValue("my_calometPt", my_Met);
       fillVariableWithValue("my_calometPhi", my_MetPhi);

       if(ic5JetPhi->size()>0)
	 fillVariableWithValue("DPhiMET_J1", acos(cos( ic5JetPhi->at(0) - calometPhi->at(0) )));
       if(ic5JetPhi->size()>1)
	 fillVariableWithValue("DPhiMET_J1", acos(cos( ic5JetPhi->at(1) - calometPhi->at(0) )));

       fillVariableWithValue("my_calometEMPt", my_MetEM);
       fillVariableWithValue("my_calometEMPhi", my_MetEMPhi);
       fillVariableWithValue("my_SumetEM", my_SumetEM);
       fillVariableWithValue("my_calometXEM", my_MetEM_x);
       fillVariableWithValue("my_calometYEM", my_MetHAD_x);

       fillVariableWithValue("my_calometHADPt", my_MetHAD);
       fillVariableWithValue("my_calometHADPhi", my_MetHADPhi);
       fillVariableWithValue("my_SumetHAD", my_SumetHAD);
       fillVariableWithValue("my_calometXHAD", my_MetHAD_x);
       fillVariableWithValue("my_calometYHAD", my_MetHAD_x);

       fillVariableWithValue("caloSumetEB", calometEmEtInEB->at(0));
       fillVariableWithValue("caloSumetEE", calometEmEtInEE->at(0));
       fillVariableWithValue("caloSumetHB", calometHadEtInHB->at(0));
       fillVariableWithValue("caloSumetHE", calometHadEtInHE->at(0));
       fillVariableWithValue("caloSumetHF", calometHadEtInHF->at(0));
     
       fillVariableWithValue("EEMF", eemf);
       fillVariableWithValue("ECHF", echf);

       for (int ii=0; ii<vertexZ->size(); ii++)
	 {
	   h_AllVertexZ->Fill(vertexZ->at(ii));
	   h_AllVertexChi2->Fill(vertexChi2->at(ii));
	   h_AllVertexNDOF->Fill(vertexNDF->at(ii));
	   h_AllVertexNtrk->Fill(vertexNTracks->at(ii));
	   if(vertexNDF->at(ii)!=0)
       	     h_AllVertexChi2_0_NDOF->Fill(vertexChi2->at(ii)/vertexNDF->at(ii));
	 }

       //Fill 2D histograms
       h_HCalNoiseLoose_Vs_MET->Fill(HcalNoiseFilterLoose,calometPt->at(0));
       h_HCalNoiseTight_Vs_MET->Fill(HcalNoiseFilterTight,calometPt->at(0));
       h_HCalNoiseHighLevel_Vs_MET->Fill(HcalNoiseFilterHighLevel,calometPt->at(0));
       
       h_EEMF_Vs_MET->Fill(eemf,calometPt->at(0));
       h_ECHF_Vs_MET->Fill(echf,calometPt->at(0));
//        h_BeamHalo_Vs_MET->Fill(BeamHaloSummaryId->at(ii), calometPt->at(0));

       h_MET_Vs_METPhi->Fill(calometPhi->at(0), calometPt->at(0));
       h_MET_Vs_SumET ->Fill(calometSumEt->at(0), calometPt->at(0));
       h_MET_Vs_METX  ->Fill(calometPx->at(0), calometPt->at(0));
       h_MET_Vs_METY  ->Fill(calometPx->at(0), calometPt->at(0));

     for (int ii=0; ii<CaloTowersEta->size(); ii++)
	 h_towersEtaPhi_EM->Fill( CaloTowersIeta->at(ii), CaloTowersIphi->at(ii), CaloTowersEmEt->at(ii));
       
       for (int ii=0; ii<CaloTowersEta->size(); ii++)
	 h_towersEtaPhi_HAD->Fill( CaloTowersIeta->at(ii), CaloTowersIphi->at(ii), CaloTowersHadEt->at(ii));

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
   h_towersEtaPhi_EM->Write();   
   h_towersEtaPhi_HAD->Write();
   h_HCalNoiseLoose_Vs_MET->Write();
   h_HCalNoiseTight_Vs_MET->Write();
   h_HCalNoiseHighLevel_Vs_MET->Write();
   h_AllVertexZ->Write();
   h_AllVertexChi2->Write();
   h_AllVertexNDOF->Write();
   h_AllVertexChi2_0_NDOF->Write();
   h_EEMF_Vs_MET->Write();
   h_ECHF_Vs_MET->Write();
   h_AllVertexNtrk->Write();
   
   h_MET_Vs_METPhi->Write();
   h_MET_Vs_SumET ->Write();
   h_MET_Vs_METX  ->Write();
   h_MET_Vs_METY  ->Write();

   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}
