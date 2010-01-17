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


  int   Nbins_METSumET = 100;
  float Max_METSumET = 100;
  int   Nbins_Phi = 50; // -4 + 4
  int   Nbins_METSig = 50;
  int   Max_METSig = 20;

  //## 1D histograms

  //calomet
  TH1F *h_calometPt   = new TH1F ("h_calometPt","h_calometPt",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_calometPhi  = new TH1F ("h_calometPhi","h_calometPhi",Nbins_Phi,-4,4);
  TH1F *h_calometPx   = new TH1F ("h_calometPx","h_calometPx",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_calometPy   = new TH1F ("h_calometPy","h_calometPy",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_caloSumet   = new TH1F ("h_caloSumet","h_caloSumet",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_calometSig   = new TH1F ("h_calometSig","h_calometSig",Nbins_METSig,0,Max_METSig);
  TH1F *h_calometSigCornell   = new TH1F ("h_calometSigCornell","h_calometSigCornell",Nbins_METSig,0,Max_METSig);

  h_calometPt->Sumw2();
  h_calometPhi->Sumw2(); 
  h_calometPx->Sumw2();  
  h_calometPy->Sumw2();  
  h_caloSumet->Sumw2();  
  h_calometSig->Sumw2(); 
  h_calometSigCornell->Sumw2();

  //calometNoHF
  TH1F *h_calometNoHFPt   = new TH1F ("h_calometNoHFPt","h_calometNoHFPt",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_calometNoHFPhi  = new TH1F ("h_calometNoHFPhi","h_calometNoHFPhi",Nbins_Phi,-4,4);
  TH1F *h_calometNoHFPx   = new TH1F ("h_calometNoHFPx","h_calometNoHFPx",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_calometNoHFPy   = new TH1F ("h_calometNoHFPy","h_calometNoHFPy",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_caloSumetNoHF   = new TH1F ("h_caloSumetNoHF","h_caloSumetNoHF",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_calometSigNoHF   = new TH1F ("h_calometSigNoHF","h_calometSigNoHF",Nbins_METSig,0,Max_METSig);
  TH1F *h_calometSigCornellNoHF   = new TH1F ("h_calometSigCornellNoHF","h_calometSigCornellNoHF",Nbins_METSig,0,Max_METSig);

  h_calometNoHFPt->Sumw2();
  h_calometNoHFPhi->Sumw2(); 
  h_calometNoHFPx->Sumw2();  
  h_calometNoHFPy->Sumw2();  
  h_caloSumetNoHF->Sumw2();  
  h_calometSigNoHF->Sumw2(); 
  h_calometSigCornellNoHF->Sumw2();

  //calosumet for different subdetectors
  TH1F *h_caloSumetEB     = new TH1F ("h_caloSumetEB","h_caloSumetEB",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_caloSumetEE     = new TH1F ("h_caloSumetEE","h_caloSumetEE",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_caloSumetHB     = new TH1F ("h_caloSumetHB","h_caloSumetHB",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_caloSumetHE     = new TH1F ("h_caloSumetHE","h_caloSumetHE",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_caloSumetHadHF  = new TH1F ("h_caloSumetHadHF","h_caloSumetHadHF",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_caloSumetEmHF   = new TH1F ("h_caloSumetEmHF","h_caloSumetEmHF",Nbins_METSumET,0,Max_METSumET);

  h_caloSumetEB->Sumw2();
  h_caloSumetEE->Sumw2();
  h_caloSumetHB->Sumw2();
  h_caloSumetHE->Sumw2();
  h_caloSumetHadHF->Sumw2();
  h_caloSumetEmHF->Sumw2(); 

  //HF PMT hits
  TH1F *h_HFRatio       = new TH1F ("h_HFRatio","h_HFRatio",400,-2.,2.);
  TH1F *h_calometPt_noHFPMTfilter   = new TH1F ("h_calometPt_noHFPMTfilter","h_calometPt_noHFPMTfilter",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_caloSumet_noHFPMTfilter = new TH1F ("h_caloSumet_noHFPMTfilter","h_caloSumet_noHFPMTfilter",Nbins_METSumET,0,Max_METSumET);

  h_HFRatio->Sumw2();
  h_calometPt_noHFPMTfilter->Sumw2();
  h_caloSumet_noHFPMTfilter->Sumw2();

  //MPT
  TH1F *h_nGoodTracks = new TH1F ("h_nGoodTracks","h_nGoodTracks",100,0,100);
  TH1F *h_MPT   = new TH1F ("h_MPT","h_MPT",Nbins_METSumET,0,Max_METSumET);
  TH1F *h_MPTPhi  = new TH1F ("h_MPTPhi","h_MPTPhi",Nbins_Phi,-4,4);
  TH1F *h_MPx   = new TH1F ("h_MPx","h_MPx",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_MPy   = new TH1F ("h_MPy","h_MPy",Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH1F *h_Phi_MPT_MET   = new TH1F ("h_Phi_MPT_MET","h_Phi_MPT_MET",50,0,4);

  h_nGoodTracks->Sumw2();
  h_MPT->Sumw2();  
  h_MPTPhi->Sumw2();
  h_MPx->Sumw2();   
  h_MPy->Sumw2();   
  h_Phi_MPT_MET->Sumw2(); 

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

  //## 2D histograms

  //MET vs ...
  TH2F *h2_MET_Vs_METPhi = new TH2F ("h2_MET_Vs_METPhi","h2_MET_Vs_METPhi",Nbins_Phi, -4,4, Nbins_METSumET,0,Max_METSumET);
  TH2F *h2_MET_Vs_SumET  = new TH2F ("h2_MET_Vs_SumET","h2_MET_Vs_SumET",Nbins_METSumET, 0,Max_METSumET, Nbins_METSumET,0,Max_METSumET);
  TH2F *h2_MET_Vs_METX   = new TH2F ("h2_MET_Vs_METX","h2_MET_Vs_METX",Nbins_METSumET, -Max_METSumET/2,Max_METSumET/2, Nbins_METSumET,0,Max_METSumET);
  TH2F *h2_MET_Vs_METY   = new TH2F ("h2_MET_Vs_METY","h2_MET_Vs_METY",Nbins_METSumET, -Max_METSumET/2,Max_METSumET/2, Nbins_METSumET,0,Max_METSumET);
  TH2F *h2_METX_Vs_METY  = new TH2F ("h2_METX_Vs_METY","h2_METX_Vs_METY",
				     Nbins_METSumET, -Max_METSumET/2,Max_METSumET/2, Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
  TH2F *h2_MET_Vs_MPT    = new TH2F ("h2_MET_Vs_MPT","h2_MET_Vs_MPT",Nbins_METSumET,0,Max_METSumET,Nbins_METSumET,0,Max_METSumET);

  //hcal noise
  TH2F *h2_HCalNoiseLoose_Vs_MET = new TH2F ("h2_HCalNoiseLoose_Vs_MET","h2_HCalNoiseLoose_Vs_MET",2, 0,2, Nbins_METSumET,0,Max_METSumET);
  TH2F *h2_HCalNoiseTight_Vs_MET = new TH2F ("h2_HCalNoiseTight_Vs_MET","h2_HCalNoiseTight_Vs_MET",2, 0,2, Nbins_METSumET,0,Max_METSumET);
  TH2F *h2_HCalNoiseHighLevel_Vs_MET = new TH2F ("h2_HCalNoiseHighLevel_Vs_MET","h2_HCalNoiseHighLevel_Vs_MET",2, 0,2, Nbins_METSumET,0,Max_METSumET);

  //calotowers
  TH2F *h2_towersEtaPhi_EM  = new TH2F ("h2_towersEtaPhi_EM","h2_towersEtaPhi_EM",82,-41,41, 72,0,72);
  TH2F *h2_towersEtaPhi_HAD = new TH2F ("h2_towersEtaPhi_HAD","h2_towersEtaPhi_HAD",82,-41,41, 72,0,72);



  // met_quantities vs SumET (1D + 2D histograms)
  int   nhistos  = 20;
  float MaxSumEt = Max_METSumET;
  //float MaxSumEt = 100;

  TH2F *h2_metx_sumet = new TH2F("h2_metx_sumet","h2_metx_sumet", nhistos, 0, MaxSumEt, Nbins_METSumET, -Max_METSumET/2, Max_METSumET/2); 
  TH2F *h2_mety_sumet = new TH2F("h2_mety_sumet","h2_mety_sumet", nhistos, 0, MaxSumEt, Nbins_METSumET, -Max_METSumET/2, Max_METSumET/2); 
  TH2F *h2_met_sumet  = new TH2F("h2_met_sumet","h2_met_sumet", nhistos, 0, MaxSumEt, Nbins_METSumET, 0, Max_METSumET);
  TH2F *h2_metsig_sumet  = new TH2F("h2_metsig_sumet","h2_metsig_sumet", nhistos, 0, MaxSumEt, Nbins_METSig, 0, Max_METSig); 

  TH1D *h_metx[nhistos];
  TH1D *h_mety[nhistos];
  TH1D *h_met[nhistos];
  TH1D *h_metsig[nhistos];

  TH1F *h_metxrms_sumet = new TH1F ("h_metxrms_sumet","h_metxrms_sumet", nhistos,0,MaxSumEt);
  TH1F *h_metxsigma_sumet = new TH1F ("h_metxsigma_sumet","h_metxsigma_sumet", nhistos,0,MaxSumEt);
  TH1F *h_metyrms_sumet = new TH1F ("h_metyrms_sumet","h_metyrms_sumet", nhistos,0,MaxSumEt);
  TH1F *h_metysigma_sumet = new TH1F ("h_metysigma_sumet","h_metysigma_sumet", nhistos,0,MaxSumEt);
  TH1F *h_metrms_sumet = new TH1F ("h_metrms_sumet","h_metrms_sumet", nhistos,0,MaxSumEt);
  TH1F *h_metsig_sumet = new TH1F ("h_metsig_sumet","h_metsig_sumet", nhistos,0,MaxSumEt);


//   TProfile * p_metstability         = new TProfile ("p_metstability","p_metstability", 360,0,360);
//   TProfile * p_metxstability        = new TProfile ("p_metxstability","p_metxstability", 360,0,360);
//   TProfile * p_metystability        = new TProfile ("p_metystability","p_metystability", 360,0,360);
//   TProfile * p_metxsigmastability   = new TProfile ("p_metxsigmastability","p_metxsigmastability", 360,0,360);
//   TProfile * p_metysigmastability   = new TProfile ("p_metysigmastability","p_metysigmastability", 360,0,360);
//   TProfile * p_sumetstability       = new TProfile ("p_sumetstability","p_sumetstability", 360,0,360);
//   TProfile * p_metsigmastability    = new TProfile ("p_metsigmastability","p_metsigmastability", 360,0,360);
//   TProfile * p_sumetsigmastability  = new TProfile ("p_sumetsigmastability","p_sumetsigmastability", 360,0,360);
//   p_metsigmastability  ->BuildOptions(0,360,"s");
//   p_metxsigmastability ->BuildOptions(0,360,"s");
//   p_metysigmastability ->BuildOptions(0,360,"s");
//   p_sumetsigmastability->BuildOptions(0,360,"s");

  //#################################
  //## Trend plotting section
  //#################################
  map<UInt_t,TH1F*> calometPtMap;
  map<UInt_t,TH1F*> caloSumetMap;
  map<UInt_t,TH1F*> calometPxMap;
  map<UInt_t,TH1F*> calometPyMap;
  //#################################

  /////////initialize variables


  Long64_t nentries = fChain->GetEntriesFast();
  std::cout << "analysisClass::Loop(): nentries = " << nentries << std::endl;   

  Long64_t nb = 0;

  for (Long64_t jentry=0; jentry<nentries;jentry++) 
  //for (Long64_t jentry=0; jentry<1000;jentry++) 
    {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      //       if(jentry>300000) break;
      nb = fChain->GetEntry(jentry);   
      
      if(jentry < 10 || jentry%1000 == 0) std::cout << "analysisClass::Loop(): jentry = " << jentry << std::endl;   
      ////////////////////// User's code starts here ///////////////////////

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
	{
	  pass_BPTX = 1;
	}      

      //## pass_BSC_MB - BSC MinBias triggers firing (both Data and MC)
      if( l1techbits->at(40)==1 || l1techbits->at(41)==1 ) 
	{
	  pass_BSC_MB = 1;
	}	  

      //## pass_BSC_BeamHaloVeto - Veto on BSC Beam Halo Triggers firing
      if(isData==1)
	{
	  pass_BSC_BeamHaloVeto = 1;
	  if( l1techbits->at(36) == 1 || l1techbits->at(37) == 1 || l1techbits->at(38) == 1 || l1techbits->at(39) == 1 )
	    pass_BSC_BeamHaloVeto = 0;
	}
      else if(isData == 0)
	{ 
	  pass_BSC_BeamHaloVeto = 1;
	}

      //## pass_PhysicsBit - HLT Physics Declared bit set 
      if(isData==1)
	{
	  if(hltbits->at(116)==1)
	    pass_PhysicsBit = 1;
	}
      else if(isData == 0)
	{
	  pass_PhysicsBit = 1;
	}


      //#####################
      //## Reco-based filters
      //#####################

      //## pass_MonsterTRKEventVeto - "Monster Events" Tracker Filter
      //see https://twiki.cern.ch/twiki/bin/viewauth/CMS/TRKPromptFeedBack#Event_and_track_selection_recipe
      int pass_MonsterTRKEventVeto = 0;

      int num_good_tracks = 0;
      float fraction      = 0.;  
      float thresh        = 0.2;
	 
      if(isData == 1)
	{
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
	}
      else if(isData == 0)
	{ 
	  pass_MonsterTRKEventVeto = 1;
	}

      //## pass_HFPMTHitVeto - Reject anomalous events in HF due to PMT hits
      //--> Recipe from Efe Yazgan  -  14 Jan 2010 
      //if (EL < 1.2 && ES < 1.8) do not flag anything.
      //if ((ETL > 5 && ES < 1.8) or (EL < 1.2 && ETS > 5)) calculate 
      //R=||EL|-|ES||/||EL|+|ES||
      //Then if R>0.995 then flag.
      //EL=energy in the long fiber,
      //ES=energy in the short fiber,
      //ETL=et of the long fiber,
      //ETS=et of the short fiber
      int pass_HFPMTHitVeto = 1;

      float ETL_cut     = 5;       
      float ETS_cut     = 5;       
      float EL_cut      = 1.2;       
      float ES_cut      = 1.8;       
      float Rplus_cut   = 0.995;       
      float Rminus_cut  = 0.995;       

      for (int i = 0; i<int(CaloTowersEmEtVtx0->size()); i++)
	{
	  if( fabs(CaloTowersIeta->at(i)) >= 29 ) //HF only
	  {
	    TVector3 * towerL = new TVector3;
	    TVector3 * towerS = new TVector3;
	    towerL->SetPtEtaPhi(CaloTowersEmEt->at(i)+0.5*CaloTowersHadEt->at(i), CaloTowersEta->at(i), CaloTowersPhi->at(i));
	    towerS->SetPtEtaPhi(0.5*CaloTowersHadEt->at(i), CaloTowersEta->at(i), CaloTowersPhi->at(i));
		
	    Float_t ratio = -1.5;
	    if( (towerL->Pt() > ETL_cut) || (towerS->Pt() > ETS_cut) ) 
	    {
	      ratio = ( fabs(towerL->Mag()) - fabs(towerS->Mag()) ) 
		/ ( fabs(towerL->Mag()) + fabs(towerS->Mag()) );
	    }	    

	    if( (towerL->Pt() > ETL_cut && towerS->Mag() < ES_cut) 
		|| (towerS->Pt() > ETS_cut && towerL->Mag() < EL_cut) )
	    {		
	      if( ratio < -Rminus_cut || ratio > Rplus_cut)
		pass_HFPMTHitVeto = 0; //this is a PMT hit candidate		    
	    }

	    //plot "ratio" for all HF towers - after event selection
	    if(pass_BPTX == 1 && pass_BSC_MB == 1 
	       && pass_BSC_BeamHaloVeto == 1 && pass_PhysicsBit == 1 
	       && pass_MonsterTRKEventVeto == 1 )
	      {
		h_HFRatio->Fill(ratio);
	      }
 
	  }
	}


      //############################
      //## Calculate Reco Quantities 
      //############################

      //## Reconstructed MPT from tracks (Artur)

      int    nGoodTracks=0;
      double MPx=0.;
      double MPy=0.;
      double MPT=0.;
      double MPTPhi=0.;

      for (int ii=0; ii<tracksPt->size(); ii++)
	{
	  int trackFlags = tracksQuality->at(ii);
	  int highPurityFlag = 3;
	  if( ( trackFlags & 1 << highPurityFlag) > 0)
	    {
	      MPx += -1.*(tracksPt->at(ii)*cos(tracksPhi->at(ii)));
	      MPy += -1.*(tracksPt->at(ii)*sin(tracksPhi->at(ii)));
	      nGoodTracks++;
	    }	  
	}

      TVector2 *mpt2 = new TVector2(MPx,MPy);
      MPT    = mpt2->Mod();
      MPTPhi = mpt2->Phi();


      //=================================================================

      //#################################
      //## Additional quantities 
      //## (commented out for the moment)
      //#################################
      /*   

      //## Calculate MET on the fly from caloTowers
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

      //## Calculate MET on the fly from caloTowers with VTX0
      sum_et = 0.0;
      sum_ex = 0.0;
      sum_ey = 0.0;
      for (int i = 0; i<int(CaloTowersEmEtVtx0->size()); i++)
	{
	   double Tower_ET = CaloTowersEmEtVtx0->at(i) + CaloTowersHadEtVtx0->at(i);
	   if (Tower_ET>getPreCutValue1("towerETcut"))
	     {
	       double phi   = CaloTowersPhi->at(i);
	       double et    = CaloTowersEmEtVtx0->at(i) + CaloTowersHadEtVtx0->at(i);
	       sum_et += et;
	       sum_ex += et*cos(phi);
	       sum_ey += et*sin(phi);
	     }
	 }

      double my_MetVtx0    = sqrt( sum_ex*sum_ex + sum_ey*sum_ey );
      double my_MetVtx0_x  = -sum_ex;
      double my_MetVtx0_y  = -sum_ey;
      double my_SumetVtx0  = sum_et;
      double my_MetPhiVtx0 = atan2( -sum_ey, -sum_ex );       

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

      */
      //=================================================================

      // Set the evaluation of the cuts to false and clear the variable values and filled status
      resetCuts();

      // Set the value of the variableNames listed in the cutFile to their current value       
      fillVariableWithValue("pass_BPTX", pass_BPTX);
      fillVariableWithValue("pass_BSC_MB", pass_BSC_MB);
      fillVariableWithValue("pass_BSC_BeamHaloVeto", pass_BSC_BeamHaloVeto);
      fillVariableWithValue("pass_PhysicsBit", pass_PhysicsBit);
      fillVariableWithValue("pass_MonsterTRKEventVeto", pass_MonsterTRKEventVeto);
      fillVariableWithValue("pass_HFPMTHitVeto", pass_HFPMTHitVeto);



      // Evaluate cuts (but do not apply them)
      evaluateCuts();



      if( passedCut("all") )
	 {

	   //#########################
	   //## 1D histograms
	   //#########################

	   //calomet
	   h_calometPt->Fill( calometPt->at(0) );
	   h_calometPhi->Fill( calometPhi->at(0) );
	   h_calometPx->Fill( calometPx->at(0) ); 
	   h_calometPy->Fill( calometPy->at(0) ); 
	   h_caloSumet->Fill( calometSumEt->at(0) ); 
	   h_calometSigCornell->Fill( calometMETSigCornell->at(0) ); 
	   if( calometSumEt->at(0) > 0)
	     h_calometSig->Fill( calometPt->at(0) / sqrt(calometSumEt->at(0)) );

	   //calometNoHF
	   h_calometNoHFPt->Fill(metnohfPtCalo->at(0));
	   h_calometNoHFPhi->Fill(metnohfPhiCalo->at(0));
	   h_calometNoHFPx->Fill(metnohfPxCalo->at(0)); 
	   h_calometNoHFPy->Fill(metnohfPyCalo->at(0)); 
	   h_caloSumetNoHF->Fill(metnohfSumEtCalo->at(0)); 
	   h_calometSigCornellNoHF->Fill( metnohfMETSigCornellCalo->at(0) ); 
	   if( metnohfSumEtCalo->at(0) > 0)
	     h_calometSigNoHF->Fill( metnohfPtCalo->at(0) / sqrt(metnohfSumEtCalo->at(0)) );

	   //calosumet for different subdetectors
	   h_caloSumetEB->Fill( calometEmEtInEB->at(0) ) ;
	   h_caloSumetEE->Fill( calometEmEtInEE->at(0) ) ;
	   h_caloSumetHB->Fill( calometHadEtInHB->at(0) ) ;
	   h_caloSumetHE->Fill( calometHadEtInHE->at(0) ) ;
	   h_caloSumetHadHF->Fill( calometHadEtInHF->at(0) ) ;
	   h_caloSumetEmHF->Fill( calometEmEtInHF->at(0) ) ;

	   //MPT
	   h_nGoodTracks->Fill(nGoodTracks);	       
	   if(nGoodTracks>0)
	     {
	       h_MPT->Fill( MPT );
	       h_MPTPhi->Fill( MPTPhi );
	       h_MPx->Fill( MPx );
	       h_MPy->Fill( MPy );
	       h_Phi_MPT_MET->Fill( acos( cos( MPTPhi - calometPhi->at(0) ) ) );	       
	     }

	   //Vertex
	   h_AllNVertex->Fill(vertexZ->size());
	   for (int ii=0; ii<vertexZ->size(); ii++)
	     {
	       if(vertexNTracks->at(ii)<2) continue;
	       h_AllVertexZ->Fill(vertexZ->at(ii));
	       h_AllVertexChi2->Fill(vertexChi2->at(ii));
	       h_AllVertexNDOF->Fill(vertexNDF->at(ii));
	       h_AllVertexNtrk->Fill(vertexNTracks->at(ii));
	       if(vertexNDF->at(ii)!=0)
		 h_AllVertexChi2_0_NDOF->Fill( vertexChi2->at(ii) / vertexNDF->at(ii) );

	       h_VertexSumpt->Fill(vertexSumPt->at(ii));
	       h_VertexSumptW5->Fill(vertexSumPtW5->at(ii));
	     }
	
	   //#########################
	   //## 2D histograms
	   //#########################	  

	   //calotowers
	   for (int ii=0; ii<CaloTowersEta->size(); ii++)
	     h2_towersEtaPhi_EM->Fill( CaloTowersIeta->at(ii), CaloTowersIphi->at(ii), CaloTowersEmEt->at(ii));
	   for (int ii=0; ii<CaloTowersEta->size(); ii++)
	     h2_towersEtaPhi_HAD->Fill( CaloTowersIeta->at(ii), CaloTowersIphi->at(ii), CaloTowersHadEt->at(ii));

	   //hcal noise
	   h2_HCalNoiseLoose_Vs_MET->Fill(HcalNoiseFilterLoose,calometPt->at(0));
	   h2_HCalNoiseTight_Vs_MET->Fill(HcalNoiseFilterTight,calometPt->at(0));
	   h2_HCalNoiseHighLevel_Vs_MET->Fill(HcalNoiseFilterHighLevel,calometPt->at(0));

	   //MET vs ...
	   h2_MET_Vs_METPhi->Fill(calometPhi->at(0), calometPt->at(0));
	   h2_MET_Vs_SumET ->Fill(calometSumEt->at(0), calometPt->at(0));
	   h2_MET_Vs_METX  ->Fill(calometPx->at(0), calometPt->at(0));
	   h2_MET_Vs_METY  ->Fill(calometPy->at(0), calometPt->at(0));
	   h2_METX_Vs_METY ->Fill(calometPx->at(0), calometPy->at(0));
	   if(nGoodTracks>0)
	     h2_MET_Vs_MPT   ->Fill(calometPt->at(0), MPT);

	   // met_quantities vs SumET
	   h2_metx_sumet->Fill( calometSumEt->at(0), calometPx->at(0) );
	   h2_mety_sumet->Fill( calometSumEt->at(0), calometPy->at(0) );
	   h2_met_sumet ->Fill( calometSumEt->at(0), calometPt->at(0) );
	   if( calometSumEt->at(0) > 0)
	     h2_metsig_sumet->Fill( calometSumEt->at(0), calometPt->at(0) / sqrt(calometSumEt->at(0)) );
	   
           //#################################
           //## Trend plotting section
           //#################################
           if(calometPtMap[run]) {
             calometPtMap[run]->Fill( calometPt->at(0) );
           } else {
             calometPtMap[run] = new TH1F(Form("h_calometPt_Run_%u",run), Form("h_calometPt_Run_%u",run), Nbins_METSumET, 0, Max_METSumET);
             calometPtMap[run]->Fill( calometPt->at(0) );
           }
           if(caloSumetMap[run]) {
             caloSumetMap[run]->Fill( calometSumEt->at(0) );
           } else {
             caloSumetMap[run] = new TH1F(Form("h_caloSumet_Run_%u",run), Form("h_caloSumet_Run_%u",run), Nbins_METSumET, 0, Max_METSumET);
             caloSumetMap[run]->Fill( calometSumEt->at(0) );
           }
           if(calometPxMap[run]) {
             calometPxMap[run]->Fill( calometPx->at(0) );
           } else {
             calometPxMap[run] = new TH1F(Form("h_calometPx_Run_%u",run), Form("h_calometPx_Run_%u",run), Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
             calometPxMap[run]->Fill( calometPx->at(0) );
           }
           if(calometPyMap[run]) {
             calometPyMap[run]->Fill( calometPy->at(0) );
           } else {
             calometPyMap[run] = new TH1F(Form("h_calometPy_Run_%u",run), Form("h_calometPy_Run_%u",run), Nbins_METSumET,-Max_METSumET/2,Max_METSumET/2);
             calometPyMap[run]->Fill( calometPy->at(0) );
           }
           //#################################
           
	 }        
           

      if( passedAllOtherCuts("pass_HFPMTHitVeto") )
	 {
	   h_calometPt_noHFPMTfilter->Fill( calometPt->at(0) );
	   h_caloSumet_noHFPMTfilter->Fill( calometSumEt->at(0) );
	 }


      //######## print event number and ls ############

      /*
      if( passedCut("0") )
	 {

	   if ( pass_MonsterTRKEventVeto == 0 ) 
	     {
	       cout << "event: " << event << " " 
		    << "ls: " << ls << " "
		    << "run: " << run << "  "
		    << "--  Is Monster" << " "
		    << endl;
	     }

	   if( calometPt->at(0) > 15 )
	     {
	       cout << "event: " << event << " " 
		    << "ls: " << ls << " "
		    << "run: " << run << "  "
		    << "--  calometPt->at(0) : " <<  calometPt->at(0) << " "
		    << endl;
	     }

	   if( calometSumEt->at(0) > 50 )
	     {
	       cout << "event: " << event << " " 
		    << "ls: " << ls << " "
		    << "run: " << run << "  "
		    << "--  calometSumEt->at(0) : " <<  calometSumEt->at(0) << " "
		    << endl;
	     }

	   if( calometHadEtInHF->at(0) > 20 )
	     {
	       cout << "event: " << event << " " 
		    << "ls: " << ls << " "
		    << "run: " << run << "  "
		    << "--  calometHadEtInHF->at(0) : " <<  calometHadEtInHF->at(0) << " "
		    << endl;
	     }

	   if( calometEmEtInHF->at(0) > 20 )
	     {
	       cout << "event: " << event << " " 
		    << "ls: " << ls << " "
		    << "run: " << run << "  "
		    << "--  calometEmEtInHF->at(0) : " <<  calometEmEtInHF->at(0) << " "
		    << endl;
	     }

	 }
      */


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

  //

  for(Int_t i=0; i<h2_metx_sumet->GetNbinsX(); i++)
    {
      float N_rms_fit = 1.5;

      h_metx[i] =   h2_metx_sumet->ProjectionY(Form("h_metx_%d",i), i, i+1, "e");
      h_mety[i] =   h2_mety_sumet->ProjectionY(Form("h_mety_%d",i), i, i+1, "e");
      h_met[i] =    h2_met_sumet->ProjectionY(Form("h_met_%d",i), i, i+1, "e");
      h_metsig[i] = h2_metsig_sumet->ProjectionY(Form("h_metsig_%d",i), i, i+1, "e");

      TF1 *fit0 = new TF1("fit0", "gaus", h_metx[i]->GetMean()-N_rms_fit*h_metx[i]->GetRMS(), h_metx[i]->GetMean()+N_rms_fit*h_metx[i]->GetRMS());
      TF1 *fit1 = new TF1("fit1", "gaus", h_mety[i]->GetMean()-N_rms_fit*h_mety[i]->GetRMS(), h_mety[i]->GetMean()+N_rms_fit*h_mety[i]->GetRMS());
      // TF1 *fit2 = new TF1("fit2", "gaus", h_met[i]->GetMean()-N_rms_fit*h_met[i]->GetRMS(), h_met[i]->GetMean()+N_rms_fit*h_met[i]->GetRMS());
      // TF1 *fit3 = new TF1("fit3", "gaus", h_metsig[i]->GetMean()-N_rms_fit*h_metsig[i]->GetRMS(), h_metsig[i]->GetMean()+N_rms_fit*h_metsig[i]->GetRMS());

      if(h_metx[i]->Integral()>10)
	 {
	   h_metx[i]  ->Fit("fit0", "R");
	   h_mety[i]  ->Fit("fit1", "R");
	   //h_met[i]   ->Fit("fit2", "R");
	   //h_metsig[i]->Fit("fit3", "R");
	   
	   h_metxsigma_sumet->SetBinContent(i+1, fit0->GetParameter(2) );
	   h_metxsigma_sumet->SetBinError(i+1, fit0->GetParError(2) );
	   
	   h_metxrms_sumet->SetBinContent(i+1, h_metx[i]->GetRMS() );
 	   h_metxrms_sumet->SetBinError(i+1, h_metx[i]->GetRMSError() );
	   
	   h_metysigma_sumet->SetBinContent(i+1, fit1->GetParameter(2) );
	   h_metysigma_sumet->SetBinError(i+1, fit1->GetParError(2) );
	   
	   h_metyrms_sumet->SetBinContent(i+1, h_mety[i]->GetRMS() );
 	   h_metyrms_sumet->SetBinError(i+1, h_mety[i]->GetRMSError() );
	   
	   h_metrms_sumet->SetBinContent(i+1, h_met[i]->GetRMS() );
	   h_metrms_sumet->SetBinError(i+1,  h_met[i]->GetRMSError() );
	   
	   h_metsig_sumet->SetBinContent(i+1, h_metsig[i]->GetMean());
	   h_metsig_sumet->SetBinError(i+1, h_metsig[i]->GetMeanError() );
	 }
	 
      delete fit0;
      delete fit1;
    }
    
    
  //#################################
  //## Trend plotting section
  //#################################
  Int_t n = calometPtMap.size();
  Double_t x[n], ex[n], yM[n], eyM[n], yR[n], eyR[n];
  // ## calometPt
//   cout<<"-----------------------------------"<<endl;
//   cout<<">> Total number of runs processed: "<<n<<endl;
  Int_t i=0;
  for (map<UInt_t,TH1F*>::const_iterator it = calometPtMap.begin(); it != calometPtMap.end(); it++) {
//     cout<<">> Run: "<<it->first<<", MET Mean: "<<it->second->GetMean()<<" GeV"<<", MET Mean Error:"<<it->second->GetMeanError()<<" GeV"<<endl;

    x[i]=it->first;
    yM[i]=it->second->GetMean();
    yR[i]=it->second->GetRMS();
    ex[i]=0;
    eyM[i]=it->second->GetMeanError();
    eyR[i]=it->second->GetRMSError();
    
    it->second->Write();
    i++;
  }
//   cout<<"-----------------------------------"<<endl;

  TGraphErrors *calometPtMean_vs_run = new TGraphErrors(n,x,yM,ex,eyM);
  calometPtMean_vs_run->SetName("g_calometPtMean_vs_run");
  calometPtMean_vs_run->SetTitle("CaloMET Mean vs. Run");
  calometPtMean_vs_run->GetXaxis()->SetTitle("Run");
  calometPtMean_vs_run->GetYaxis()->SetTitle("CaloMET Mean [GeV]");
  calometPtMean_vs_run->Write();
  
  TGraphErrors *calometPtRMS_vs_run = new TGraphErrors(n,x,yR,ex,eyR);
  calometPtRMS_vs_run->SetName("g_calometPtRMS_vs_run");
  calometPtRMS_vs_run->SetTitle("CaloMET RMS vs. Run");
  calometPtRMS_vs_run->GetXaxis()->SetTitle("Run");
  calometPtRMS_vs_run->GetYaxis()->SetTitle("CaloMET RMS [GeV]");
  calometPtRMS_vs_run->Write();
  
  // ## caloSumet
  i=0;
  for (map<UInt_t,TH1F*>::const_iterator it = caloSumetMap.begin(); it != caloSumetMap.end(); it++) {

    x[i]=it->first;
    yM[i]=it->second->GetMean();
    yR[i]=it->second->GetRMS();
    ex[i]=0;
    eyM[i]=it->second->GetMeanError();
    eyR[i]=it->second->GetRMSError();
    
    it->second->Write();
    i++;
  }

  TGraphErrors *caloSumetMean_vs_run = new TGraphErrors(n,x,yM,ex,eyM);
  caloSumetMean_vs_run->SetName("g_caloSumetMean_vs_run");
  caloSumetMean_vs_run->SetTitle("CaloSumET Mean vs. Run");
  caloSumetMean_vs_run->GetXaxis()->SetTitle("Run");
  caloSumetMean_vs_run->GetYaxis()->SetTitle("CaloSumET Mean [GeV]");
  caloSumetMean_vs_run->Write();
  
  TGraphErrors *caloSumetRMS_vs_run = new TGraphErrors(n,x,yR,ex,eyR);
  caloSumetRMS_vs_run->SetName("g_caloSumetRMS_vs_run");
  caloSumetRMS_vs_run->SetTitle("CaloSumET RMS vs. Run");
  caloSumetRMS_vs_run->GetXaxis()->SetTitle("Run");
  caloSumetRMS_vs_run->GetYaxis()->SetTitle("CaloSumET RMS [GeV]");
  caloSumetRMS_vs_run->Write();
  
  // ## calometPx
  i=0;
  for (map<UInt_t,TH1F*>::iterator it = calometPxMap.begin(); it != calometPxMap.end(); it++) {
    
    Double_t N_rms_fit = 1.5;
    Double_t mean = it->second->GetMean();
    Double_t rms = it->second->GetRMS();
    
    TF1 *fit = new TF1("fit", "gaus", mean-N_rms_fit*rms, mean+N_rms_fit*rms);
    it->second->Fit("fit", "R");

    x[i]=it->first;
    yM[i]=fit->GetParameter(1);
    yR[i]=fit->GetParameter(2);
    ex[i]=0;
    eyM[i]=fit->GetParError(1);
    eyR[i]=fit->GetParError(2);

    it->second->Write();
    i++;
        
    delete fit;
  }
  
  TGraphErrors *calometPxMean_vs_run = new TGraphErrors(n,x,yM,ex,eyM);
  calometPxMean_vs_run->SetName("g_calometPxMean_vs_run");
  calometPxMean_vs_run->SetTitle("CaloMETPx #mu vs. Run");
  calometPxMean_vs_run->GetXaxis()->SetTitle("Run");
  calometPxMean_vs_run->GetYaxis()->SetTitle("CaloMETPx #mu [GeV]");
  calometPxMean_vs_run->Write();
  
  TGraphErrors *calometPxSigma_vs_run = new TGraphErrors(n,x,yR,ex,eyR);
  calometPxSigma_vs_run->SetName("g_calometPxSigma_vs_run");
  calometPxSigma_vs_run->SetTitle("CaloMETPx #sigma vs. Run");
  calometPxSigma_vs_run->GetXaxis()->SetTitle("Run");
  calometPxSigma_vs_run->GetYaxis()->SetTitle("CaloMETPx #sigma [GeV]");
  calometPxSigma_vs_run->Write();
  
  // ## calometPy
  i=0;
  for (map<UInt_t,TH1F*>::iterator it = calometPyMap.begin(); it != calometPyMap.end(); it++) {

    Double_t N_rms_fit = 1.5;
    Double_t mean = it->second->GetMean();
    Double_t rms = it->second->GetRMS();
    
    TF1 *fit = new TF1("fit", "gaus", mean-N_rms_fit*rms, mean+N_rms_fit*rms);
    
    it->second->Fit("fit", "R");
      
    x[i]=it->first;
    yM[i]=fit->GetParameter(1);
    yR[i]=fit->GetParameter(2);
    ex[i]=0;
    eyM[i]=fit->GetParError(1);
    eyR[i]=fit->GetParError(2);
    
    it->second->Write();
    i++;
    
    delete fit;
  }
  
  TGraphErrors *calometPyMean_vs_run = new TGraphErrors(n,x,yM,ex,eyM);
  calometPyMean_vs_run->SetName("g_calometPyMean_vs_run");
  calometPyMean_vs_run->SetTitle("CaloMETPx #mu vs. Run");
  calometPyMean_vs_run->GetXaxis()->SetTitle("Run");
  calometPyMean_vs_run->GetYaxis()->SetTitle("CaloMETPx #mu [GeV]");
  calometPyMean_vs_run->Write();
  
  TGraphErrors *calometPySigma_vs_run = new TGraphErrors(n,x,yR,ex,eyR);
  calometPySigma_vs_run->SetName("g_calometPySigma_vs_run");
  calometPySigma_vs_run->SetTitle("CaloMETPx #sigma vs. Run");
  calometPySigma_vs_run->GetXaxis()->SetTitle("Run");
  calometPySigma_vs_run->GetYaxis()->SetTitle("CaloMETPx #sigma [GeV]");
  calometPySigma_vs_run->Write();
  //#################################
  
  //////////write histos 

  //TH1F * h_example = new TH1F ("h_example","", getHistoNBins("my_calometPt"), getHistoMin("my_calometPt"), getHistoMax("my_calometPt"));
  //h_example->Add( & getHisto_noCuts_or_skim("my_calometPt") ); // all histos can be retrieved, see other getHisto_xxxx methods in baseClass.h

  //## 1D histograms

  //calomet
  h_calometPt->Write();   
  h_calometPhi->Write();    
  h_calometPx->Write();   
  h_calometPy->Write();   
  h_caloSumet->Write();   
  h_calometSig->Write();   
  h_calometSigCornell->Write();   

  //calometNoHF  
  h_calometNoHFPt->Write();
  h_calometNoHFPhi->Write(); 
  h_calometNoHFPx->Write();  
  h_calometNoHFPy->Write();  
  h_caloSumetNoHF->Write();  
  h_calometSigNoHF->Write(); 
  h_calometSigCornellNoHF->Write();

  //calosumet for different subdetectors
  h_caloSumetEB->Write();
  h_caloSumetEE->Write();
  h_caloSumetHB->Write();
  h_caloSumetHE->Write();
  h_caloSumetHadHF->Write();
  h_caloSumetEmHF->Write(); 

  //HF PMT hits
  h_HFRatio->Write();
  h_calometPt_noHFPMTfilter->Write();
  h_caloSumet_noHFPMTfilter->Write();

  //MPT
  h_nGoodTracks->Write();
  h_MPT->Write();
  h_MPTPhi->Write();
  h_MPx->Write();  
  h_MPy->Write();   
  h_Phi_MPT_MET->Write(); 

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

  //MET vs ...
  h2_MET_Vs_METPhi->Write();   
  h2_MET_Vs_SumET->Write();    
  h2_MET_Vs_METX->Write();     
  h2_MET_Vs_METY->Write();     
  h2_METX_Vs_METY->Write();    
  h2_MET_Vs_MPT->Write();      

  //hcal noise
  h2_HCalNoiseLoose_Vs_MET->Write();   
  h2_HCalNoiseTight_Vs_MET->Write();   
  h2_HCalNoiseHighLevel_Vs_MET->Write();   

  //calotowers
  h2_towersEtaPhi_EM->Write();    
  h2_towersEtaPhi_HAD->Write();   


  // met_quantities vs SumET (1D + 2D histograms)
  for(int i=0;i<nhistos;i++)
    {
      h_metx[i]->Write();
      h_mety[i]->Write();
      h_met[i]->Write();
      h_metsig[i]->Write();
    }

  h_metxsigma_sumet->Write();
  h_metxrms_sumet->Write();
  h_metysigma_sumet->Write();
  h_metyrms_sumet->Write();
  h_metrms_sumet->Write();
  h_metsig_sumet->Write();

  h2_metx_sumet->Write(); 
  h2_mety_sumet->Write(); 
  h2_met_sumet->Write();  
  h2_metsig_sumet->Write();  

  //   p_metstability->Write();
  //   p_metxstability->Write();
  //   p_metystability->Write();
  //   p_sumetstability->Write();
  //   p_metsigmastability->Write();
  //   p_metxsigmastability->Write();
  //   p_metysigmastability->Write();
  //   p_sumetsigmastability->Write();

  std::cout << "analysisClass::Loop() ends" <<std::endl;   
}
