#define analysisClass_cxx
#include "analysisClass.h"
#include <TH2.h>
#include <TH1F.h>
#include <TF1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TVector2.h>
#include <TVector3.h>
#include <TProfile.h>
#include <TGraphErrors.h>
#include <TLine.h>

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

double DeltaPhi(double jetPhi1,double jetPhi2){
  double deltaphi=fabs(jetPhi1-jetPhi2);
  if(deltaphi>M_PI){
    deltaphi=2*M_PI-deltaphi;
  }
  return deltaphi;
}

//first proposal - give only index
bool JetIdloose(double ak5JetJIDresEMF,double ak5JetJIDfHPD,int ak5JetJIDn90Hits, double ak5JetEta){
  bool jetidresEMF=true;
  bool jetidloose=false;
  double fhpdmax = 0.98;
  double n90hitsmin =1;
  double emf_min = 0.01;

  if(fabs(ak5JetEta)<2.6 && ak5JetJIDresEMF<emf_min) jetidresEMF=false;
  if(jetidresEMF && ak5JetJIDfHPD<fhpdmax && ak5JetJIDn90Hits>n90hitsmin) {
    jetidloose=true;
  }
  return jetidloose;
}

//first proposal - give only index
bool JetIdtight(double ak5JetJIDresEMF,double ak5JetJIDfHPD,double ak5JetJIDfRBX, int ak5JetJIDn90Hits, double ak5JetEta){
  bool jetidresEMF=true;
  bool jetidtight=false;
  double fhpdmax = 0.98;
  double frbxmax = 0.98;
  double n90hitsmin =4;
  double emf_min = 0.01;
  if(fabs(ak5JetEta)<2.6 && ak5JetJIDresEMF<emf_min) jetidresEMF=false;
  if(jetidresEMF && ak5JetJIDfHPD<fhpdmax && ak5JetJIDfRBX<fhpdmax&& ak5JetJIDn90Hits>n90hitsmin) {
    jetidtight=true;
  }
  return jetidtight;
}

void analysisClass::Loop()
{
  std::cout << "analysisClass::Loop() begins" <<std::endl; 

  if (fChain == 0) return;

  //////////book histos here


  int   Nbins_METSumET = 100;
  float Max_METSumET = 100;
  int   Nbins_Phi = 50;
  int   Nbins_METSig = 50;
  int   Max_METSig = 20;
  float Max_Phi = 3.15;
  int Nbins_Energy = 300;
  float Max_Energy = 300;

  //## 1D histograms

  //calomet
  TH1F *h_calometPt   = new TH1F ("h_calometPt","h_calometPt",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_calometPhi  = new TH1F ("h_calometPhi","h_calometPhi",Nbins_Phi,-Max_Phi,Max_Phi);
  TH1F *h_calometPx   = new TH1F ("h_calometPx","h_calometPx",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_calometPy   = new TH1F ("h_calometPy","h_calometPy",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_caloSumet   = new TH1F ("h_caloSumet","h_caloSumet",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_calometSig   = new TH1F ("h_calometSig","h_calometSig",Nbins_METSig,0,Max_METSig);

  h_calometPt->Sumw2();
  h_calometPhi->Sumw2(); 
  h_calometPx->Sumw2();
  h_calometPy->Sumw2();
  h_caloSumet->Sumw2();
  h_calometSig->Sumw2(); 

  //calomet in dijets (loose)
  TH1F *h_dijetLoose_calometPt   = new TH1F ("h_dijetLoose_calometPt","h_dijetLoose_calometPt",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetLoose_calometPhi  = new TH1F ("h_dijetLoose_calometPhi","h_dijetLoose_calometPhi",Nbins_Phi,-Max_Phi,Max_Phi);
  TH1F *h_dijetLoose_calometPx   = new TH1F ("h_dijetLoose_calometPx","h_dijetLoose_calometPx",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_dijetLoose_calometPy   = new TH1F ("h_dijetLoose_calometPy","h_dijetLoose_calometPy",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_dijetLoose_caloSumet   = new TH1F ("h_dijetLoose_caloSumet","h_dijetLoose_caloSumet",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetLoose_calometSig   = new TH1F ("h_dijetLoose_calometSig","h_dijetLoose_calometSig",Nbins_METSig,0,Max_METSig);

  h_dijetLoose_calometPt->Sumw2();
  h_dijetLoose_calometPhi->Sumw2(); 
  h_dijetLoose_calometPx->Sumw2();
  h_dijetLoose_calometPy->Sumw2();
  h_dijetLoose_caloSumet->Sumw2();
  h_dijetLoose_calometSig->Sumw2(); 

  //calomet in dijets (tight)
  TH1F *h_dijetTight_calometPt   = new TH1F ("h_dijetTight_calometPt","h_dijetTight_calometPt",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetTight_calometPhi  = new TH1F ("h_dijetTight_calometPhi","h_dijetTight_calometPhi",Nbins_Phi,-Max_Phi,Max_Phi);
  TH1F *h_dijetTight_calometPx   = new TH1F ("h_dijetTight_calometPx","h_dijetTight_calometPx",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_dijetTight_calometPy   = new TH1F ("h_dijetTight_calometPy","h_dijetTight_calometPy",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_dijetTight_caloSumet   = new TH1F ("h_dijetTight_caloSumet","h_dijetTight_caloSumet",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetTight_calometSig   = new TH1F ("h_dijetTight_calometSig","h_dijetTight_calometSig",Nbins_METSig,0,Max_METSig);

  h_dijetTight_calometPt->Sumw2();
  h_dijetTight_calometPhi->Sumw2(); 
  h_dijetTight_calometPx->Sumw2();
  h_dijetTight_calometPy->Sumw2();
  h_dijetTight_caloSumet->Sumw2();
  h_dijetTight_calometSig->Sumw2(); 

  //tcmet
  TH1F *h_tcmetPt   = new TH1F ("h_tcmetPt","h_tcmetPt",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_tcmetPhi  = new TH1F ("h_tcmetPhi","h_tcmetPhi",Nbins_Phi,-Max_Phi,Max_Phi);
  TH1F *h_tcmetPx   = new TH1F ("h_tcmetPx","h_tcmetPx",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_tcmetPy   = new TH1F ("h_tcmetPy","h_tcmetPy",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_tcSumet   = new TH1F ("h_tcSumet","h_tcSumet",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_tcmetSig   = new TH1F ("h_tcmetSig","h_tcmetSig",Nbins_METSig,0,Max_METSig);

  h_tcmetPt->Sumw2();
  h_tcmetPhi->Sumw2(); 
  h_tcmetPx->Sumw2();
  h_tcmetPy->Sumw2();
  h_tcSumet->Sumw2();
  h_tcmetSig->Sumw2(); 

  //tcmet in dijet (loose)
  TH1F *h_dijetLoose_tcmetPt   = new TH1F ("h_dijetLoose_tcmetPt","h_dijetLoose_tcmetPt",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetLoose_tcmetPhi  = new TH1F ("h_dijetLoose_tcmetPhi","h_dijetLoose_tcmetPhi",Nbins_Phi,-Max_Phi,Max_Phi);
  TH1F *h_dijetLoose_tcmetPx   = new TH1F ("h_dijetLoose_tcmetPx","h_dijetLoose_tcmetPx",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_dijetLoose_tcmetPy   = new TH1F ("h_dijetLoose_tcmetPy","h_dijetLoose_tcmetPy",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_dijetLoose_tcSumet   = new TH1F ("h_dijetLoose_tcSumet","h_dijetLoose_tcSumet",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetLoose_tcmetSig   = new TH1F ("h_dijetLoose_tcmetSig","h_dijetLoose_tcmetSig",Nbins_METSig,0,Max_METSig);

  h_dijetLoose_tcmetPt->Sumw2();
  h_dijetLoose_tcmetPhi->Sumw2(); 
  h_dijetLoose_tcmetPx->Sumw2();
  h_dijetLoose_tcmetPy->Sumw2();
  h_dijetLoose_tcSumet->Sumw2();
  h_dijetLoose_tcmetSig->Sumw2(); 

  //tcmet in dijet (tight)
  TH1F *h_dijetTight_tcmetPt   = new TH1F ("h_dijetTight_tcmetPt","h_dijetTight_tcmetPt",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetTight_tcmetPhi  = new TH1F ("h_dijetTight_tcmetPhi","h_dijetTight_tcmetPhi",Nbins_Phi,-Max_Phi,Max_Phi);
  TH1F *h_dijetTight_tcmetPx   = new TH1F ("h_dijetTight_tcmetPx","h_dijetTight_tcmetPx",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_dijetTight_tcmetPy   = new TH1F ("h_dijetTight_tcmetPy","h_dijetTight_tcmetPy",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_dijetTight_tcSumet   = new TH1F ("h_dijetTight_tcSumet","h_dijetTight_tcSumet",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetTight_tcmetSig   = new TH1F ("h_dijetTight_tcmetSig","h_dijetTight_tcmetSig",Nbins_METSig,0,Max_METSig);

  h_dijetTight_tcmetPt->Sumw2();
  h_dijetTight_tcmetPhi->Sumw2(); 
  h_dijetTight_tcmetPx->Sumw2();
  h_dijetTight_tcmetPy->Sumw2();
  h_dijetTight_tcSumet->Sumw2();
  h_dijetTight_tcmetSig->Sumw2(); 

  //pfmet
  TH1F *h_pfmetPt   = new TH1F ("h_pfmetPt","h_pfmetPt",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_pfmetPhi  = new TH1F ("h_pfmetPhi","h_pfmetPhi",Nbins_Phi,-Max_Phi,Max_Phi);
  TH1F *h_pfmetPx   = new TH1F ("h_pfmetPx","h_pfmetPx",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_pfmetPy   = new TH1F ("h_pfmetPy","h_pfmetPy",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_pfSumet   = new TH1F ("h_pfSumet","h_pfSumet",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_pfmetSig   = new TH1F ("h_pfmetSig","h_pfmetSig",Nbins_METSig,0,Max_METSig);

  h_pfmetPt->Sumw2();
  h_pfmetPhi->Sumw2(); 
  h_pfmetPx->Sumw2();
  h_pfmetPy->Sumw2();
  h_pfSumet->Sumw2();
  h_pfmetSig->Sumw2(); 

  //pfmet in dijet (loose)
  TH1F *h_dijetLoose_pfmetPt   = new TH1F ("h_dijetLoose_pfmetPt","h_dijetLoose_pfmetPt",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetLoose_pfmetPhi  = new TH1F ("h_dijetLoose_pfmetPhi","h_dijetLoose_pfmetPhi",Nbins_Phi,-Max_Phi,Max_Phi);
  TH1F *h_dijetLoose_pfmetPx   = new TH1F ("h_dijetLoose_pfmetPx","h_dijetLoose_pfmetPx",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_dijetLoose_pfmetPy   = new TH1F ("h_dijetLoose_pfmetPy","h_dijetLoose_pfmetPy",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_dijetLoose_pfSumet   = new TH1F ("h_dijetLoose_pfSumet","h_dijetLoose_pfSumet",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetLoose_pfmetSig   = new TH1F ("h_dijetLoose_pfmetSig","h_dijetLoose_pfmetSig",Nbins_METSig,0,Max_METSig);

  h_dijetLoose_pfmetPt->Sumw2();
  h_dijetLoose_pfmetPhi->Sumw2(); 
  h_dijetLoose_pfmetPx->Sumw2();
  h_dijetLoose_pfmetPy->Sumw2();
  h_dijetLoose_pfSumet->Sumw2();
  h_dijetLoose_pfmetSig->Sumw2(); 

  //pfmet in dijet (tight)
  TH1F *h_dijetTight_pfmetPt   = new TH1F ("h_dijetTight_pfmetPt","h_dijetTight_pfmetPt",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetTight_pfmetPhi  = new TH1F ("h_dijetTight_pfmetPhi","h_dijetTight_pfmetPhi",Nbins_Phi,-Max_Phi,Max_Phi);
  TH1F *h_dijetTight_pfmetPx   = new TH1F ("h_dijetTight_pfmetPx","h_dijetTight_pfmetPx",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_dijetTight_pfmetPy   = new TH1F ("h_dijetTight_pfmetPy","h_dijetTight_pfmetPy",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_dijetTight_pfSumet   = new TH1F ("h_dijetTight_pfSumet","h_dijetTight_pfSumet",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetTight_pfmetSig   = new TH1F ("h_dijetTight_pfmetSig","h_dijetTight_pfmetSig",Nbins_METSig,0,Max_METSig);

  h_dijetTight_pfmetPt->Sumw2();
  h_dijetTight_pfmetPhi->Sumw2(); 
  h_dijetTight_pfmetPx->Sumw2();
  h_dijetTight_pfmetPy->Sumw2();
  h_dijetTight_pfSumet->Sumw2();
  h_dijetTight_pfmetSig->Sumw2(); 

  //Vertex
  TH1F *h_AllVertexZ    = new TH1F ("h_AllVertexZ","h_AllVertexZ",100,-100,100);
  TH1F *h_AllVertexChi2 = new TH1F ("h_AllVertexChi2","h_AllVertexChi",100,0,100);
  TH1F *h_AllVertexNDOF = new TH1F ("h_AllVertexNDOF","h_AllVertexNDOF",50,0,50);
  TH1F *h_AllVertexChi2_0_NDOF = new TH1F ("h_AllVertexChi2_0_NDOF","h_AllVertexChi2_0_NDOF",200,0,40);
  TH1F *h_AllVertexNtrk = new TH1F ("h_AllVertexNtrk","h_AllVertexNtrk",50,0,50);
  TH1F *h_AllNVertex    = new TH1F ("h_AllNVertex","h_AllNVertex",50,0,50);
  TH1F *h_VertexSumpt   = new TH1F ("h_VertexSumpt","h_VertexSumpt",200,0,200);
  TH1F *h_VertexSumptW5 = new TH1F ("h_VertexSumptW5","h_VertexSumptW5",200,0,200);

  h_AllVertexZ->Sumw2();
  h_AllVertexChi2->Sumw2(); 
  h_AllVertexNDOF->Sumw2(); 
  h_AllVertexChi2_0_NDOF->Sumw2();
  h_AllVertexNtrk->Sumw2(); 
  h_AllNVertex->Sumw2();
  h_VertexSumpt->Sumw2(); 
  h_VertexSumptW5->Sumw2(); 

  /////////initialize variables
  float HFEnergyCut = getPreCutValue1("HFEnergyCut");

  //////////////////////////////
  ///// Goood Run List  ////////
  //////////////////////////////
  int goodruns[] = {123596, 123615, 123732, 123815, 123818,
                    123908, 124008, 124009, 124020, 124022,
                    124023, 124024, 124025, 124027, 124030/*,
							    124120*/};
                   //124120 at 2360 GeV

   int goodLSmin[] = {2, 70, 62, 8, 2,
                      2, 1, 1, 12, 66,
                      38, 2, 5, 24, 2/*,
				       1*/};

   int goodLSmax[] = {9999, 9999, 109, 9999, 42,
                      12, 1, 68, 94, 179,
                      9999, 83, 13, 9999, 9999/*,
						9999*/};

  Long64_t nentries = fChain->GetEntriesFast();
  std::cout << "analysisClass::Loop(): nentries = " << nentries << std::endl; 

  Long64_t nb = 0;

  for (Long64_t jentry=0; jentry<nentries;jentry++) 
    //for (Long64_t jentry=0; jentry<2000;jentry++) 
    {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      //       if(jentry>300000) break;
      nb = fChain->GetEntry(jentry); 
      
      if(jentry < 10 || jentry%1000 == 0) std::cout << "analysisClass::Loop(): jentry = " << jentry << std::endl; 
      ////////////////////// User's code starts here ///////////////////////
      

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

      //#####################
      //## Trigger selection
      //#####################

      int pass_BPTX              = 0;
      int pass_BSC_MB            = 0;
      int pass_BSC_BeamHaloVeto  = 0;
      int pass_PhysicsBit        = 0;

      //## pass_BPTX - Two beams crossing at CMS (only Data)
      if(isData==1)
	{
	  if(l1techbits->at(0)==1)
	    pass_BPTX = 1;
	}
      else if(isData==0)
	pass_BPTX = 1;
      
      //## pass_BSC_MB - BSC MinBias triggers firing (both Data and MC)
      if( l1techbits->at(40)==1 || l1techbits->at(41)==1 ) 
	pass_BSC_MB = 1;
      
      //## pass_BSC_BeamHaloVeto - Veto on BSC Beam Halo Triggers firing
      if(isData==1)
	{
	  pass_BSC_BeamHaloVeto = 1;
	  if( l1techbits->at(36) == 1 || l1techbits->at(37) == 1 || l1techbits->at(38) == 1 || l1techbits->at(39) == 1 )
	    pass_BSC_BeamHaloVeto = 0;
	}
      else if(isData == 0)
	pass_BSC_BeamHaloVeto = 1;

      //## pass_PhysicsBit - HLT Physics Declared bit set 
      if(isData==1)
	{
	  if(hltbits->at(116)==1)
	    pass_PhysicsBit = 1;
	}
      else if(isData == 0)
	pass_PhysicsBit = 1;

      //#####################
      //## Reco-based filters
      //#####################

      //pass_HFEnergyCut
      int pass_HFEnergyCut = 0;
      int pass_HFEnergyCut_Plus = 0;
      int pass_HFEnergyCut_Minus = 0;
      
      for (int i = 0; i<int(CaloTowersEmEt->size()); i++)
	{

	  if( fabs(CaloTowersIeta->at(i)) > 29 ) //HF only
	    {
	 
	      TVector3 * towerL = new TVector3;
	      TVector3 * towerS = new TVector3;
	      towerL->SetPtEtaPhi(CaloTowersEmEt->at(i)+0.5*CaloTowersHadEt->at(i), CaloTowersEta->at(i), CaloTowersPhi->at(i));
	      towerS->SetPtEtaPhi(0.5*CaloTowersHadEt->at(i), CaloTowersEta->at(i), CaloTowersPhi->at(i));

	      // energy on plus side	    
	      if( CaloTowersIeta->at(i) > 0 && ( towerL->Mag() + towerS->Mag() ) > HFEnergyCut )
		{
		  pass_HFEnergyCut_Plus=1;		  
		  if( pass_HFEnergyCut_Plus == 1 && pass_HFEnergyCut_Minus == 1 )
		    {
		      pass_HFEnergyCut = 1;
		      break;
		    }
		}

	      // energy on minus side	    
	      if( CaloTowersIeta->at(i) < 0 && ( towerL->Mag() + towerS->Mag() ) > HFEnergyCut )
		{
		  pass_HFEnergyCut_Minus=1;
		  if( pass_HFEnergyCut_Plus == 1 && pass_HFEnergyCut_Minus == 1 )
		    {
		      pass_HFEnergyCut = 1;
		      break;
		    }
		}
			
	      delete towerL;
	      delete towerS;
	
	    }//end loop over calotowers in HF

	}//end loop over calotowers
      

      //pass_GoodVertex 
      //https://twiki.cern.ch/twiki/bin/viewauth/CMS/TRKPromptFeedBack#Event_and_track_selection_recipe
      int pass_GoodVertex = 0;

      if(vertexZ->size() == 0) pass_GoodVertex = 0;
      for (int ii=0; ii<vertexZ->size(); ii++)
	if( vertexChi2->at(ii) != 0. && vertexNDF->at(ii) != 0 && vertexNDF->at(ii) >= 5 && fabs(vertexZ->at(ii)) <= 15. )
	  {
	    pass_GoodVertex = 1;
	    break;
	  }
      
      //## pass_MonsterTRKEventVeto - "Monster Events" Tracker Filter
      //see https://twiki.cern.ch/twiki/bin/viewauth/CMS/TRKPromptFeedBack#Event_and_track_selection_recipe
      int pass_MonsterTRKEventVeto = 0;

      int num_good_tracks = 0;
      float fraction      = 0.;
      float thresh        = 0.25;
	 
      if(tracksPt->size()<=10)
	{
	  pass_MonsterTRKEventVeto = 1;
	}//<=10 tracks	    
      else if(tracksPt->size()>10)
	{
	  for (int ii=0; ii<tracksPt->size(); ii++)
	    {
	      int trackFlags = tracksQuality->at(ii);
	      int highPurityFlag = 3;
	      if( ( trackFlags & 1 << highPurityFlag) > 0)
		{
		  num_good_tracks++;		      
		  fraction = (float)num_good_tracks / (float)tracksPt->size();
		  
		  if( fraction > thresh ) 
		    pass_MonsterTRKEventVeto = 1;
		}
	    }
	}//>10 tracks	    

      //## pass_HFPMTHitVeto - Reject anomalous events in HF due to PMT hits - 
      int pass_HFPMTHitVeto_tcMET   = 1;

      //masked towers
      // HF(37,67,1): STATUS = 0x8040
      // HF(29,67,1): STATUS = 0x40
      // HF(35,67,1): STATUS = 0x8040
      // HF(29,67,2): STATUS = 0x40
      // HF(30,67,2): STATUS = 0x8040
      // HF(32,67,2): STATUS = 0x8040
      // HF(36,67,2): STATUS = 0x8040
      // HF(38,67,2): STATUS = 0x8040

      for (int i = 0; i<int(CaloTowersEmEt->size()); i++)
	{
	  if( fabs(CaloTowersIeta->at(i)) > 29 ) //HF only
	    {
	      TVector3 * towerL = new TVector3;
	      TVector3 * towerS = new TVector3;
	      towerL->SetPtEtaPhi(CaloTowersEmEt->at(i)+0.5*CaloTowersHadEt->at(i), CaloTowersEta->at(i), CaloTowersPhi->at(i));
	      towerS->SetPtEtaPhi(0.5*CaloTowersHadEt->at(i), CaloTowersEta->at(i), CaloTowersPhi->at(i));

	      //tower masked
	      int isLongMasked=0;
	      int isShortMasked=0;
	      if( CaloTowersIeta->at(i) == 37 && CaloTowersIphi->at(i) == 67)
		isLongMasked = 1;
	      if( CaloTowersIeta->at(i) == 29 && CaloTowersIphi->at(i) == 67)
		isLongMasked = 1;
	      if( CaloTowersIeta->at(i) == 35 && CaloTowersIphi->at(i) == 67)
		isLongMasked = 1;

	      if( CaloTowersIeta->at(i) == 29 && CaloTowersIphi->at(i) == 67)
		isShortMasked = 1;
	      if( CaloTowersIeta->at(i) == 30 && CaloTowersIphi->at(i) == 67)
		isShortMasked = 1;
	      if( CaloTowersIeta->at(i) == 32 && CaloTowersIphi->at(i) == 67)
		isShortMasked = 1;
	      if( CaloTowersIeta->at(i) == 36 && CaloTowersIphi->at(i) == 67)
		isShortMasked = 1;
	      if( CaloTowersIeta->at(i) == 38 && CaloTowersIphi->at(i) == 67)
		isShortMasked = 1;

	      //-- a la tcMET		
	      float ET_cut_tcMET      = 5; 
	      float Rplus_cut_tcMET   = 0.99; 
	      float Rminus_cut_tcMET  = 0.8; 
	      Float_t ratio_tcMET     = -1.5;	      
	      if(  ( CaloTowersEmEt->at(i) + CaloTowersHadEt->at(i) ) > ET_cut_tcMET 
		   && isShortMasked==0 && isLongMasked==0 )
		{		
		  ratio_tcMET = ( fabs(towerL->Mag()) - fabs(towerS->Mag()) ) 
		    / ( fabs(towerL->Mag()) + fabs(towerS->Mag()) );

		  if( ratio_tcMET < -Rminus_cut_tcMET || ratio_tcMET > Rplus_cut_tcMET )
		    pass_HFPMTHitVeto_tcMET = 0; 
		}
	      delete towerL;
	      delete towerS;
	    }
	}

      //ECAL spikes EB
      int pass_ECALSpikesVeto_tcMET = 1;

      for (int ii=0; ii<ECALnoiseECalEBSeedEnergy->size(); ii++)
	{

	  //-- seed crystal info --
	  float seedEnergy = ECALnoiseECalEBSeedEnergy->at(ii);
	  float seedet = ECALnoiseECalEBSeedEnergy->at(ii) / cosh(ECALnoiseECalEBSeedEta->at(ii));
	  float seedex = seedet * cos( ECALnoiseECalEBSeedPhi->at(ii) );
	  float seedey = seedet * sin( ECALnoiseECalEBSeedPhi->at(ii) );
	  float seedeta = ECALnoiseECalEBSeedEta->at(ii);
	  float seedphi = ECALnoiseECalEBSeedPhi->at(ii);

	  //S4/S1 vs ET (a la tcMET)
	  float S4_tcMET = 0.;
	  S4_tcMET = ECALnoiseECalEBSeedERight->at(ii) 
	    + ECALnoiseECalEBSeedELeft->at(ii)
	    + ECALnoiseECalEBSeedETop->at(ii)
	    + ECALnoiseECalEBSeedEBottom->at(ii);

	  float S4_tcMEToverS1 = S4_tcMET / seedEnergy;

	  if(seedet > 5. && S4_tcMEToverS1 < 0.05)
	    pass_ECALSpikesVeto_tcMET = 0;
	}

      //############################
      //## Calculate Reco Quantities 
      //############################

      //=================================================================

      // Set the evaluation of the cuts to false and clear the variable values and filled status
      resetCuts();

      // Set the value of the variableNames listed in the cutFile to their current value
      fillVariableWithValue("pass_GoodRunList", pass_GoodRunList);
      fillVariableWithValue("pass_BPTX", pass_BPTX);
      fillVariableWithValue("pass_BSC_MB", pass_BSC_MB);
      fillVariableWithValue("pass_BSC_BeamHaloVeto", pass_BSC_BeamHaloVeto);
      fillVariableWithValue("pass_PhysicsBit", pass_PhysicsBit);
      fillVariableWithValue("pass_GoodVertex", pass_GoodVertex);
      fillVariableWithValue("pass_MonsterTRKEventVeto", pass_MonsterTRKEventVeto);
      fillVariableWithValue("pass_HFEnergyCut", pass_HFEnergyCut);

      fillVariableWithValue("pass_ECALSpikesVeto_tcMET", pass_ECALSpikesVeto_tcMET);
      fillVariableWithValue("pass_HFPMTHitVeto_tcMET", pass_HFPMTHitVeto_tcMET);

      // Evaluate cuts (but do not apply them)
      evaluateCuts();

      //###########################
      //## Start filling histograms
      //###########################


      if( passedAllPreviousCuts("pass_GoodVertex") )
	{
	  //Vertex
	  h_AllNVertex->Fill(vertexZ->size());
	  for (int ii=0; ii<vertexZ->size(); ii++)
	    {
	      if(vertexNTracksW5->at(ii)==0)
		continue;

	      h_AllVertexZ->Fill(vertexZ->at(ii));
	      h_AllVertexChi2->Fill(vertexChi2->at(ii));
	      h_AllVertexNDOF->Fill(vertexNDF->at(ii));
	      h_AllVertexNtrk->Fill(vertexNTracks->at(ii));
	      if(vertexNDF->at(ii)!=0)
		h_AllVertexChi2_0_NDOF->Fill( vertexChi2->at(ii) / vertexNDF->at(ii) );
	      
	      h_VertexSumpt->Fill(vertexSumPt->at(ii));
	      h_VertexSumptW5->Fill(vertexSumPtW5->at(ii));
	    }
	}

      if( passedCut("all") )
	{
	  //#########################
	  //## inclusive MET
	  //#########################
	  h_calometPt->Fill( calometPt->at(0) );
	  h_calometPhi->Fill( calometPhi->at(0) );
	  h_calometPx->Fill( calometPx->at(0) ); 
	  h_calometPy->Fill( calometPy->at(0) ); 
	  h_caloSumet->Fill( calometSumEt->at(0) ); 

	  h_tcmetPt->Fill( tcmetPt->at(0) );
	  h_tcmetPhi->Fill( tcmetPhi->at(0) );
	  h_tcmetPx->Fill( tcmetPx->at(0) ); 
	  h_tcmetPy->Fill( tcmetPy->at(0) ); 
	  h_tcSumet->Fill( tcmetSumEt->at(0) ); 

	  h_pfmetPt->Fill( pfmetPt->at(0) );
	  h_pfmetPhi->Fill( pfmetPhi->at(0) );
	  h_pfmetPx->Fill( pfmetPx->at(0) ); 
	  h_pfmetPy->Fill( pfmetPy->at(0) ); 
	  h_pfSumet->Fill( pfmetSumEt->at(0) ); 

	  ///////////////////////////////////////
	  ///////// Print High MET events////////
	  ///////////////////////////////////////
	  if( calometPt->at(0) > 20 || tcmetPt->at(0) > 20 || pfmetPt->at(0) > 20. )
	    {
	      cout << "event: " << event << " " 
		   << "ls: " << ls << " "
		   << "run: " << run << "  "
		   << "--  calometPt->at(0) : " <<  calometPt->at(0) << " "
		   << "--  tcmetPt->at(0) : "   <<  tcmetPt->at(0)   <<" "
		   << "--  pfmetPt->at(0) : "   <<  pfmetPt->at(0)   <<" "
		   << endl;
	    }

	  if( calometSumEt->at(0) > 50 || tcmetSumEt->at(0) > 100 || pfmetSumEt->at(0) > 100 )
	    {
	      cout << "event: " << event << " " 
		   << "ls: " << ls << " "
		   << "run: " << run << "  "
		   << "--  calometSumEt->at(0) : " <<  calometSumEt->at(0) << " "
		   << "--  tcmetSumEt->at(0) : "   <<  tcmetSumEt->at(0) << " "
		   << "--  pfmetSumEt->at(0) : "   <<  pfmetSumEt->at(0) << " "
		   << endl;
	    }


	  //##########################
	  //## MET in dijets (ak5)
	  //##########################
	  
	  bool makeJetCorr = true;

	  // cut values
	  double endcapeta =2.6;
	  double endcapeta_dijet =3.0;
	  double cut_CaloDiJetDeltaPhi_min = 2.10;

	  // minimum pt cuts (depending on jet corrections)
	  double ptMin;
	  double ptMinDijet;
	  if (makeJetCorr==true) 
	    {
	      ptMin=15.;
	      ptMinDijet=10.;
	    }
	  if (makeJetCorr==false) 
	    {
	      ptMin=7.;
	      ptMinDijet=5.;
	    }

	  int index_jet1 = -10;
	  int index_jet2 = -10;
	  double mypt1=-10;
	  double mypt2=-10;

	  std::vector<TLorentzVector> vPtEtaPhiE;
	  if(!vPtEtaPhiE.empty()){ vPtEtaPhiE.clear(); }
	  
	  // --------------------DiJets---------------------------------------------------------------------   
	  // JET CORRECTION
	  // --------------------
	  double jcScale0;
	  double jcScale1;

	  //dijet
	  if(int(ak5JetpT->size())>=2)
	    {
	      for (int j = 0; j<int(ak5JetpT->size()); j++)
		{
		  //check if jet is among hardest two
		  //as jets are ordered in uncorrected pT: needs to be done only for corrected jets
		  if(makeJetCorr == true) {
		    if((ak5JetscaleL2L3->at(j)*ak5JetpT->at(j))>mypt1){
		      mypt2=mypt1;
		      index_jet2=index_jet1;
		      mypt1=ak5JetscaleL2L3->at(j)*ak5JetpT->at(j);
		      index_jet1=j;
		    }else if((ak5JetscaleL2L3->at(j)*ak5JetpT->at(j))>mypt2){
		      mypt2=ak5JetscaleL2L3->at(j)*ak5JetpT->at(j);
		      index_jet2=j;
		    }
		  }
		}

	      if((index_jet2==-10)||(index_jet1==-10))
		{
		  cout<<"index should be set ERROR: "<<index_jet2<<"/"<<index_jet1<<endl;
		}
	      // both passed pT and eta cuts
	      if(makeJetCorr == true) 
		{
		  jcScale0 = ak5JetscaleL2L3->at(index_jet1);
		  jcScale1 = ak5JetscaleL2L3->at(index_jet2);
		}
	      else 
		{
		  index_jet1 =  0;
		  index_jet2 =  1;
		  jcScale0    = 1;
		  jcScale1    = 1;
		}
	      
	      if( fabs(ak5JetEta->at(index_jet1)) < endcapeta_dijet && 
		  ( ak5JetpT->at(index_jet1) * jcScale0 ) > ptMinDijet && 
		  fabs( ak5JetEta->at(index_jet2) ) < endcapeta_dijet && 
		  ( ak5JetpT->at(index_jet2) * jcScale1 ) > ptMinDijet )
		{ 
		  // dphi
		double dphi = DeltaPhi(ak5JetPhi->at(index_jet1), ak5JetPhi->at(index_jet2) );
		
		if ( dphi > cut_CaloDiJetDeltaPhi_min ) 
		  {
		    // both passed jet ID loose
		    if(
		       JetIdloose(ak5JetJIDresEMF->at(index_jet1),ak5JetJIDfHPD->at(index_jet1),ak5JetJIDn90Hits->at(index_jet1),ak5JetEta->at(index_jet1)) &&
		       JetIdloose(ak5JetJIDresEMF->at(index_jet2),ak5JetJIDfHPD->at(index_jet2),ak5JetJIDn90Hits->at(index_jet2),ak5JetEta->at(index_jet2)))
		      {
			h_dijetLoose_calometPt->Fill( calometPt->at(0) );
			h_dijetLoose_calometPhi->Fill( calometPhi->at(0) );
			h_dijetLoose_calometPx->Fill( calometPx->at(0) ); 
			h_dijetLoose_calometPy->Fill( calometPy->at(0) ); 
			h_dijetLoose_caloSumet->Fill( calometSumEt->at(0) ); 
			
			h_dijetLoose_tcmetPt->Fill( tcmetPt->at(0) );
			h_dijetLoose_tcmetPhi->Fill( tcmetPhi->at(0) );
			h_dijetLoose_tcmetPx->Fill( tcmetPx->at(0) ); 
			h_dijetLoose_tcmetPy->Fill( tcmetPy->at(0) ); 
			h_dijetLoose_tcSumet->Fill( tcmetSumEt->at(0) ); 
			
			h_dijetLoose_pfmetPt->Fill( pfmetPt->at(0) );
			h_dijetLoose_pfmetPhi->Fill( pfmetPhi->at(0) );
			h_dijetLoose_pfmetPx->Fill( pfmetPx->at(0) ); 
			h_dijetLoose_pfmetPy->Fill( pfmetPy->at(0) ); 
			h_dijetLoose_pfSumet->Fill( pfmetSumEt->at(0) ); 
			
			// both passed jet ID tight
			if(
			   JetIdtight(ak5JetJIDresEMF->at(index_jet1),ak5JetJIDfHPD->at(index_jet1),ak5JetJIDfRBX->at(index_jet1),ak5JetJIDn90Hits->at(index_jet1),ak5JetEta->at(index_jet1)) &&
			   JetIdtight(ak5JetJIDresEMF->at(index_jet2),ak5JetJIDfHPD->at(index_jet2),ak5JetJIDfRBX->at(index_jet2),ak5JetJIDn90Hits->at(index_jet2),ak5JetEta->at(index_jet2)))
			  {
			    h_dijetTight_calometPt->Fill( calometPt->at(0) );
			    h_dijetTight_calometPhi->Fill( calometPhi->at(0) );
			    h_dijetTight_calometPx->Fill( calometPx->at(0) ); 
			    h_dijetTight_calometPy->Fill( calometPy->at(0) ); 
			    h_dijetTight_caloSumet->Fill( calometSumEt->at(0) ); 
			    
			    h_dijetTight_tcmetPt->Fill( tcmetPt->at(0) );
			    h_dijetTight_tcmetPhi->Fill( tcmetPhi->at(0) );
			    h_dijetTight_tcmetPx->Fill( tcmetPx->at(0) ); 
			    h_dijetTight_tcmetPy->Fill( tcmetPy->at(0) ); 
			    h_dijetTight_tcSumet->Fill( tcmetSumEt->at(0) ); 
			    
			    h_dijetTight_pfmetPt->Fill( pfmetPt->at(0) );
			    h_dijetTight_pfmetPhi->Fill( pfmetPhi->at(0) );
			    h_dijetTight_pfmetPx->Fill( pfmetPx->at(0) ); 
			    h_dijetTight_pfmetPy->Fill( pfmetPy->at(0) ); 
			    h_dijetTight_pfSumet->Fill( pfmetSumEt->at(0) ); 
			  }
		      }
		  }//dphi cut
		}//eta/pt cuts on dijets
	    }//di jets >= 2 jets
	  
	  //##########################
	}//-------------- passed cuts "0"
      
      ////////////////////// User's code ends here ///////////////////////
    } // End loop over events

  //////////write histos 

  //## 1D histograms

  //calomet
  h_calometPt->Write(); 
  h_calometPhi->Write();
  h_calometPx->Write(); 
  h_calometPy->Write(); 
  h_caloSumet->Write(); 
  h_tcmetSig->Write(); 
  
  //tcmet
  h_tcmetPt->Write(); 
  h_tcmetPhi->Write();
  h_tcmetPx->Write(); 
  h_tcmetPy->Write(); 
  h_tcSumet->Write(); 
  h_tcmetSig->Write(); 

  //pfmet
  h_pfmetPt->Write(); 
  h_pfmetPhi->Write();
  h_pfmetPx->Write(); 
  h_pfmetPy->Write(); 
  h_pfSumet->Write(); 
  h_pfmetSig->Write(); 

  //Dijets (loose)
  h_dijetLoose_calometPt->Write(); 
  h_dijetLoose_calometPhi->Write(); 
  h_dijetLoose_calometPx->Write(); 
  h_dijetLoose_calometPy->Write(); 
  h_dijetLoose_caloSumet->Write(); 
  
  h_dijetLoose_tcmetPt->Write(); 
  h_dijetLoose_tcmetPhi->Write(); 
  h_dijetLoose_tcmetPx->Write(); 
  h_dijetLoose_tcmetPy->Write(); 
  h_dijetLoose_tcSumet->Write(); 
		    
  h_dijetLoose_pfmetPt->Write(); 
  h_dijetLoose_pfmetPhi->Write(); 
  h_dijetLoose_pfmetPx->Write(); 
  h_dijetLoose_pfmetPy->Write(); 
  h_dijetLoose_pfSumet->Write(); 

  //Dijets (tight)
  h_dijetTight_calometPt->Write(); 
  h_dijetTight_calometPhi->Write(); 
  h_dijetTight_calometPx->Write(); 
  h_dijetTight_calometPy->Write(); 
  h_dijetTight_caloSumet->Write(); 
  
  h_dijetTight_tcmetPt->Write(); 
  h_dijetTight_tcmetPhi->Write(); 
  h_dijetTight_tcmetPx->Write(); 
  h_dijetTight_tcmetPy->Write(); 
  h_dijetTight_tcSumet->Write(); 
		    
  h_dijetTight_pfmetPt->Write(); 
  h_dijetTight_pfmetPhi->Write(); 
  h_dijetTight_pfmetPx->Write(); 
  h_dijetTight_pfmetPy->Write(); 
  h_dijetTight_pfSumet->Write(); 

  //Vertex
  h_AllVertexZ->Write();
  h_AllVertexChi2->Write(); 
  h_AllVertexNDOF->Write(); 
  h_AllVertexChi2_0_NDOF->Write();
  h_AllVertexNtrk->Write(); 
  h_AllNVertex->Write();
  h_VertexSumpt->Write(); 
  h_VertexSumptW5->Write(); 

  //## 2D histograms

  std::cout << "analysisClass::Loop() ends" <<std::endl; 
}
