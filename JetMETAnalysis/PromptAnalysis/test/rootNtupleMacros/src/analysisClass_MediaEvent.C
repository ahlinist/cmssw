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


  int   Nbins_METSumET = 500;
  float Max_METSumET = 500;

  //calomet
  TH1F *h_calometPt   = new TH1F ("h_calometPt","h_calometPt",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_calometPxy   = new TH1F ("h_calometPxy","h_calometPxy",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_caloSumet   = new TH1F ("h_caloSumet","h_caloSumet",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_caloMetOSumet   = new TH1F ("h_caloMetOSumet","h_caloMetOSumet",50,0,1.);

  h_calometPt->Sumw2();
  h_calometPxy->Sumw2();
  h_caloSumet->Sumw2();
  h_caloMetOSumet->Sumw2();

  //calomet in dijets (loose)
  TH1F *h_dijetLoose_calometPt   = new TH1F ("h_dijetLoose_calometPt","h_dijetLoose_calometPt",0.5*Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetLoose_calometPxy   = new TH1F ("h_dijetLoose_calometPxy","h_dijetLoose_calometPxy",0.5*Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_dijetLoose_caloSumet   = new TH1F ("h_dijetLoose_caloSumet","h_dijetLoose_caloSumet",0.5*Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetLoose_caloMetOSumet   = new TH1F ("h_dijetLoose_caloMetOSumet","h_dijetLoose_caloMetOSumet",50,0,1.);

  h_dijetLoose_calometPt->Sumw2();
  h_dijetLoose_calometPxy->Sumw2();
  h_dijetLoose_caloSumet->Sumw2();
  h_dijetLoose_caloMetOSumet->Sumw2();

  //calomet in dijets (tight)
  TH1F *h_dijetTight_calometPt   = new TH1F ("h_dijetTight_calometPt","h_dijetTight_calometPt",0.5*Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetTight_calometPxy   = new TH1F ("h_dijetTight_calometPxy","h_dijetTight_calometPxy",0.5*Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_dijetTight_caloSumet   = new TH1F ("h_dijetTight_caloSumet","h_dijetTight_caloSumet",0.5*Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetTight_caloMetOSumet   = new TH1F ("h_dijetTight_caloMetOSumet","h_dijetTight_caloMetOSumet",50,0,1.);

  h_dijetTight_calometPt->Sumw2();
  h_dijetTight_calometPxy->Sumw2();
  h_dijetTight_caloSumet->Sumw2();
  h_dijetTight_caloMetOSumet->Sumw2();

  //tcmet
  TH1F *h_tcmetPt   = new TH1F ("h_tcmetPt","h_tcmetPt",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_tcmetPxy   = new TH1F ("h_tcmetPxy","h_tcmetPxy",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_tcSumet   = new TH1F ("h_tcSumet","h_tcSumet",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_tcMetOSumet   = new TH1F ("h_tcMetOSumet","h_tcMetOSumet",50,0,1.);

  h_tcmetPt->Sumw2();
  h_tcmetPxy->Sumw2();
  h_tcSumet->Sumw2();
  h_tcMetOSumet->Sumw2();

  //tcmet in dijet (loose)
  TH1F *h_dijetLoose_tcmetPt   = new TH1F ("h_dijetLoose_tcmetPt","h_dijetLoose_tcmetPt",0.5*Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetLoose_tcmetPxy   = new TH1F ("h_dijetLoose_tcmetPxy","h_dijetLoose_tcmetPxy",0.5*Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_dijetLoose_tcSumet   = new TH1F ("h_dijetLoose_tcSumet","h_dijetLoose_tcSumet",0.5*Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetLoose_tcMetOSumet   = new TH1F ("h_dijetLoose_tcMetOSumet","h_dijetLoose_tcMetOSumet",50,0,1.);

  h_dijetLoose_tcmetPt->Sumw2();
  h_dijetLoose_tcmetPxy->Sumw2();
  h_dijetLoose_tcSumet->Sumw2();
  h_dijetLoose_tcMetOSumet->Sumw2();

  //tcmet in dijet (tight)
  TH1F *h_dijetTight_tcmetPt   = new TH1F ("h_dijetTight_tcmetPt","h_dijetTight_tcmetPt",0.5*Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetTight_tcmetPxy   = new TH1F ("h_dijetTight_tcmetPxy","h_dijetTight_tcmetPxy",0.5*Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_dijetTight_tcSumet   = new TH1F ("h_dijetTight_tcSumet","h_dijetTight_tcSumet",0.5*Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetTight_tcMetOSumet   = new TH1F ("h_dijetTight_tcMetOSumet","h_dijetTight_tcMetOSumet",50,0,1.);

  h_dijetTight_tcmetPt->Sumw2();
  h_dijetTight_tcmetPxy->Sumw2();
  h_dijetTight_tcSumet->Sumw2();
  h_dijetTight_tcMetOSumet->Sumw2();

  //pfmet
  TH1F *h_pfmetPt   = new TH1F ("h_pfmetPt","h_pfmetPt",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_pfmetPxy   = new TH1F ("h_pfmetPxy","h_pfmetPxy",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_pfSumet   = new TH1F ("h_pfSumet","h_pfSumet",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_pfMetOSumet   = new TH1F ("h_pfMetOSumet","h_pfMetOSumet",50,0,1.);

  h_pfmetPt->Sumw2();
  h_pfmetPxy->Sumw2();
  h_pfSumet->Sumw2();
  h_pfMetOSumet->Sumw2();

  //pfmet in dijet (loose)
  TH1F *h_dijetLoose_pfmetPt   = new TH1F ("h_dijetLoose_pfmetPt","h_dijetLoose_pfmetPt",0.5*Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetLoose_pfmetPxy   = new TH1F ("h_dijetLoose_pfmetPxy","h_dijetLoose_pfmetPxy",0.5*Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_dijetLoose_pfSumet   = new TH1F ("h_dijetLoose_pfSumet","h_dijetLoose_pfSumet",0.5*Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetLoose_pfMetOSumet   = new TH1F ("h_dijetLoose_pfMetOSumet","h_dijetLoose_pfMetOSumet",50,0,1.);

  h_dijetLoose_pfmetPt->Sumw2();
  h_dijetLoose_pfmetPxy->Sumw2();
  h_dijetLoose_pfSumet->Sumw2();
  h_dijetLoose_pfMetOSumet->Sumw2();

  //pfmet in dijet (tight)
  TH1F *h_dijetTight_pfmetPt   = new TH1F ("h_dijetTight_pfmetPt","h_dijetTight_pfmetPt",0.5*Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetTight_pfmetPxy   = new TH1F ("h_dijetTight_pfmetPxy","h_dijetTight_pfmetPxy",0.5*Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_dijetTight_pfSumet   = new TH1F ("h_dijetTight_pfSumet","h_dijetTight_pfSumet",0.5*Nbins_METSumET,0,Max_METSumET);
  TH1F *h_dijetTight_pfMetOSumet   = new TH1F ("h_dijetTight_pfMetOSumet","h_dijetTight_pfMetOSumet",50,0,1.);

  h_dijetTight_pfmetPt->Sumw2();
  h_dijetTight_pfmetPxy->Sumw2();
  h_dijetTight_pfSumet->Sumw2();
  h_dijetTight_pfMetOSumet->Sumw2();

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


  // For S9/S1 flagging
  double slopes[] = {0.0171519,0.0245339,0.0311146,0.0384983,0.0530911,0.0608012,0.0789118,0.084833,0.0998253,0.118896,0.0913756,0.0589927};

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


 //## pass_HFPMTHitVeto from 2010 HCAL DPG studies - Reject anomalous events in HF due to PMT hits - 
      int pass_HFPMTHitVeto_S9S1   = 1;
      int pass_HFPMTHitVeto_PET   = 1;

      for (int i = 0; i<int(PMTnoiseRecHitET->size()); i++)
	{
	  
	  bool isPMThit = false;
	  double energy = PMTnoiseRecHitEnergy->at(i);
	  double ET = PMTnoiseRecHitET->at(i);
	  double partenergy = PMTnoiseRecHitPartEnergy->at(i);
	  double sum4Long = PMTnoiseRecHitSum4Long->at(i);
	  double sum4Short = PMTnoiseRecHitSum4Short->at(i);
	  int ieta = PMTnoiseRecHitIeta->at(i);
	  int iphi = PMTnoiseRecHitIphi->at(i);
	  double phi = ((2*3.14159)/72) * iphi;
	  if(abs(ieta)>39) phi = ((2*3.14159)/72) * (iphi+1);
	  int depth = PMTnoiseRecHitDepth->at(i);

	  //skip the RecHit if it's just a pedestal noise
	  if( (depth==1 && energy<1.2) || (depth==2 && energy<1.8) ) continue;
               
	  //--> NOTE : all crystals has been removed in 2010 --> check if there is some channel with black tape on the window
	  //masked towers
	  // HF(37,67,1): STATUS = 0x8040
	  // HF(29,67,1): STATUS = 0x40
	  // HF(35,67,1): STATUS = 0x8040
	  // HF(29,67,2): STATUS = 0x40
	  // HF(30,67,2): STATUS = 0x8040
	  // HF(32,67,2): STATUS = 0x8040
	  // HF(36,67,2): STATUS = 0x8040
	  // HF(38,67,2): STATUS = 0x8040

	  //tower masked
	  int isLongMasked=0;
	  int isShortMasked=0;
	  if( (ieta==37 || ieta==29 || ieta==35) && iphi==67)
	    isLongMasked = 1;

	  if( (ieta==29 || ieta==30 || ieta==32 || ieta==36 || ieta==38) && iphi==67)
	    isShortMasked = 1;
               
	  //skip the RecHit if it's in the tower with crystals mounted
	  if( isLongMasked==1 || isShortMasked==1 ) continue;
               
	  //R = L-S/L+S
	  double R = PMTnoiseRecHitRValue->at(i);
               
	  //S9/S1
	  double S9oS1 = ( partenergy + sum4Long + sum4Short ) / energy;
               
	  // For S9/S1 flagging
	  double slope = (0.3084-0.02577*abs(ieta)+0.0005351*ieta*ieta);
	  if( abs(ieta)>39 ) slope = slopes[abs(ieta)-30];
	  double intercept = -slope*log((162.4-10.19*abs(ieta)+0.21*ieta*ieta));
                
	  //## identify HF spikes

	  //long fibers
	  if( depth==1 ) 
	    {

	      //PET
	      if( energy>(162.4-10.19*abs(ieta)+0.21*ieta*ieta) && R>0.98 ) 
		{ 
		  isPMThit = true;
		  pass_HFPMTHitVeto_PET = 0;
		}
	      
	      //S9/S1
	      if( abs(ieta)==29 && ( energy>(162.4-10.19*abs(ieta)+0.21*ieta*ieta) && R>0.98 ) ) //special case (as PET)
		{ 
		  isPMThit = true;
		  pass_HFPMTHitVeto_S9S1 = 0;		  
		}
	      else if( abs(ieta)>29 && ( energy>(162.4-10.19*abs(ieta)+0.21*ieta*ieta) && S9oS1<(intercept+slope*log(energy)) ) )
		{ 
		  isPMThit = true;
		  pass_HFPMTHitVeto_S9S1 = 0;		  
		}
	      
	    }
	  //short fibers (same cut, PET-based, for both PET and S9/S1 flags)
	  else if( depth==2 && energy>(129.9-6.61*abs(ieta)+0.1153*ieta*ieta) && R<-0.98 ) 
	    {
	      isPMThit = true;
	      pass_HFPMTHitVeto_PET = 0;
	      pass_HFPMTHitVeto_S9S1 = 0;
	    }
	  
	}//end loop over HF rechits




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

      //HF cleaning - S9/S1 and PET - HCAL DPG 2010
      fillVariableWithValue("pass_HFPMTHitVeto_S9S1", pass_HFPMTHitVeto_S9S1);
      fillVariableWithValue("pass_HFPMTHitVeto_PET", pass_HFPMTHitVeto_PET);

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
      
      if( passedCut("0") && pass_HFPMTHitVeto_tcMET == 1 )
	{
	  //#########################
	  //## inclusive MET
	  //#########################
	  h_calometPt->Fill( calometPt->at(0) );
	  h_calometPxy->Fill( calometPx->at(0) ); 
	  h_calometPxy->Fill( calometPy->at(0) ); 
	  h_caloSumet->Fill( calometSumEt->at(0) ); 
	  h_caloMetOSumet->Fill( calometPt->at(0) / calometSumEt->at(0) ); 

	  h_tcmetPt->Fill( tcmetPt->at(0) );
	  h_tcmetPxy->Fill( tcmetPx->at(0) ); 
	  h_tcmetPxy->Fill( tcmetPy->at(0) ); 
	  h_tcSumet->Fill( tcmetSumEt->at(0) ); 
	  h_tcMetOSumet->Fill( tcmetPt->at(0) / tcmetSumEt->at(0) ); 

	  h_pfmetPt->Fill( pfmetPt->at(0) );
	  h_pfmetPxy->Fill( pfmetPx->at(0) ); 
	  h_pfmetPxy->Fill( pfmetPy->at(0) ); 
	  h_pfSumet->Fill( pfmetSumEt->at(0) ); 
	  h_pfMetOSumet->Fill( pfmetPt->at(0) / pfmetSumEt->at(0) ); 

	  ///////////////////////////////////////
	  ///////// Print High MET events////////
	  ///////////////////////////////////////
	  if(isData==1)
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

	  if(isData==1)
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
			h_dijetLoose_calometPxy->Fill( calometPx->at(0) ); 
			h_dijetLoose_calometPxy->Fill( calometPy->at(0) ); 
			h_dijetLoose_caloSumet->Fill( calometSumEt->at(0) ); 
			h_dijetLoose_caloMetOSumet->Fill( calometPt->at(0) / calometSumEt->at(0) ); 

			h_dijetLoose_tcmetPt->Fill( tcmetPt->at(0) );
			h_dijetLoose_tcmetPxy->Fill( tcmetPx->at(0) ); 
			h_dijetLoose_tcmetPxy->Fill( tcmetPy->at(0) ); 
			h_dijetLoose_tcSumet->Fill( tcmetSumEt->at(0) ); 
			h_dijetLoose_tcMetOSumet->Fill( tcmetPt->at(0) / tcmetSumEt->at(0) ); 

			h_dijetLoose_pfmetPt->Fill( pfmetPt->at(0) );
			h_dijetLoose_pfmetPxy->Fill( pfmetPx->at(0) ); 
			h_dijetLoose_pfmetPxy->Fill( pfmetPy->at(0) ); 
			h_dijetLoose_pfSumet->Fill( pfmetSumEt->at(0) ); 
			h_dijetLoose_pfMetOSumet->Fill( pfmetPt->at(0) / pfmetSumEt->at(0) ); 
			
			// both passed jet ID tight
			if(
			   JetIdtight(ak5JetJIDresEMF->at(index_jet1),ak5JetJIDfHPD->at(index_jet1),ak5JetJIDfRBX->at(index_jet1),ak5JetJIDn90Hits->at(index_jet1),ak5JetEta->at(index_jet1)) &&
			   JetIdtight(ak5JetJIDresEMF->at(index_jet2),ak5JetJIDfHPD->at(index_jet2),ak5JetJIDfRBX->at(index_jet2),ak5JetJIDn90Hits->at(index_jet2),ak5JetEta->at(index_jet2)))
			  {
			    h_dijetTight_calometPt->Fill( calometPt->at(0) );
			    h_dijetTight_calometPxy->Fill( calometPx->at(0) ); 
			    h_dijetTight_calometPxy->Fill( calometPy->at(0) ); 
			    h_dijetTight_caloSumet->Fill( calometSumEt->at(0) ); 
			    h_dijetTight_caloMetOSumet->Fill( calometPt->at(0) / calometSumEt->at(0) ); 

			    h_dijetTight_tcmetPt->Fill( tcmetPt->at(0) );
			    h_dijetTight_tcmetPxy->Fill( tcmetPx->at(0) ); 
			    h_dijetTight_tcmetPxy->Fill( tcmetPy->at(0) ); 
			    h_dijetTight_tcSumet->Fill( tcmetSumEt->at(0) ); 
			    h_dijetTight_tcMetOSumet->Fill( tcmetPt->at(0) / tcmetSumEt->at(0) ); 
			    
			    h_dijetTight_pfmetPt->Fill( pfmetPt->at(0) );
			    h_dijetTight_pfmetPxy->Fill( pfmetPx->at(0) ); 
			    h_dijetTight_pfmetPxy->Fill( pfmetPy->at(0) ); 
			    h_dijetTight_pfSumet->Fill( pfmetSumEt->at(0) ); 
			    h_dijetTight_pfMetOSumet->Fill( pfmetPt->at(0) / pfmetSumEt->at(0) ); 
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
  h_calometPxy->Write(); 
  h_caloSumet->Write(); 
  h_caloMetOSumet->Write();
  
  //tcmet
  h_tcmetPt->Write(); 
  h_tcmetPxy->Write(); 
  h_tcSumet->Write(); 
  h_tcMetOSumet->Write();

  //pfmet
  h_pfmetPt->Write(); 
  h_pfmetPxy->Write(); 
  h_pfSumet->Write(); 
  h_pfMetOSumet->Write();

  //Dijets (loose)
  h_dijetLoose_calometPt->Write(); 
  h_dijetLoose_calometPxy->Write(); 
  h_dijetLoose_caloSumet->Write(); 
  h_dijetLoose_caloMetOSumet->Write(); 
 
  h_dijetLoose_tcmetPt->Write(); 
  h_dijetLoose_tcmetPxy->Write(); 
  h_dijetLoose_tcSumet->Write();
  h_dijetLoose_tcMetOSumet->Write(); 
		    
  h_dijetLoose_pfmetPt->Write(); 
  h_dijetLoose_pfmetPxy->Write(); 
  h_dijetLoose_pfSumet->Write(); 
  h_dijetLoose_pfMetOSumet->Write(); 

  //Dijets (tight)
  h_dijetTight_calometPt->Write(); 
  h_dijetTight_calometPxy->Write(); 
  h_dijetTight_caloSumet->Write(); 
  h_dijetTight_caloMetOSumet->Write(); 
  
  h_dijetTight_tcmetPt->Write(); 
  h_dijetTight_tcmetPxy->Write(); 
  h_dijetTight_tcSumet->Write(); 
  h_dijetTight_tcMetOSumet->Write(); 
		    
  h_dijetTight_pfmetPt->Write(); 
  h_dijetTight_pfmetPxy->Write(); 
  h_dijetTight_pfSumet->Write(); 
  h_dijetTight_pfMetOSumet->Write(); 

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
