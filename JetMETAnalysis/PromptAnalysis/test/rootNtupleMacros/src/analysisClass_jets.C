
#define analysisClass_cxx
#include "analysisClass.h"
#include <TH2.h>
#include <TH1F.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TVector2.h>
#include <TVector3.h>
#include <fstream>
#include <iostream>


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
  
  char dataset[200]="900 GeV collision data";
  //   char dataset[200]="2.36 TeV collision data";
  
  if (fChain == 0) return;

  // ---------------Settings ------------------------------------------
  // decide wether you want to apply jet corrections or not
  bool makeJetCorr = true;
  // cut values
  double barreleta =1.4;
  double endcapeta =2.6;
  double fhpdmax = 0.98;
  double n90hitsmin =1;
  double emffrac = 0.01;
  // minimum pt cuts (depending on jet corrections)
  double ptMin;
  double ptMinDijet;
  if (makeJetCorr==true) {
    ptMin=15.;
    ptMinDijet=10.;
  }
  if (makeJetCorr==false) {
    ptMin=7.;
    ptMinDijet=5.;
  }
  // Binning 
  double ptMax=100.;
  int ptBin=50;
  
  int  phiBin=100;
  double phiMax=3.2;   // -
  double phiMin=-3.2;
  
  int  etaBin=100;
  double etaMax=3.4;   //-
  double etaMin=-3.4;
  // -------------------Basic distributions - all ak5 ----------------------------
   
  TH1I *njets = new TH1I("njets","",20,0,20);
  njets->SetXTitle("Number of jets per event");
  njets->SetTitle(dataset);
  TH1I *nalljets = new TH1I("nalljets","",20,0,20);
  nalljets->SetXTitle("Number of jets per event");
  nalljets->SetTitle(dataset);
  TH1I *nconst = new TH1I("nconst","",20,0,20);
  nconst->SetXTitle("Number of constituents");
  nconst->SetTitle(dataset);
  TH1I *ncleanedjets = new TH1I("ncleanedjets","",20,0,20);
  ncleanedjets->SetXTitle("Number of jets per event");
  ncleanedjets->SetTitle(dataset);
  TH1I *nconstcleaned = new TH1I("nconstcleaned","",20,0,20);
  nconstcleaned->SetXTitle("Number of constituents");
  nconstcleaned->SetTitle(dataset);
  TH1D *ptall = new TH1D("ptall","",ptBin+8,0.,ptMax);
  ptall->SetXTitle("p_{T}[GeV]");
  ptall->SetTitle(dataset);
  TH1D *pt = new TH1D("pt","",ptBin,ptMin,ptMax);
  pt->SetXTitle("p_{T}[GeV]");
  pt->SetTitle(dataset);
  TH1D *ptcleaned = new TH1D("ptcleaned","",ptBin,ptMin,ptMax);
  ptcleaned->SetXTitle("p_{T}[GeV]");
  ptcleaned->SetTitle(dataset);
  TH1D *eta = new TH1D("eta","",etaBin,etaMin,etaMax);
  eta->SetXTitle("#eta");
  eta->SetTitle(dataset);
  TH1D *etacleaned = new TH1D("etacleaned","",etaBin,etaMin,etaMax);
  etacleaned->SetXTitle("#eta");
  etacleaned->SetTitle(dataset);
  TH1D *phi = new TH1D("phi","",phiBin,phiMin,phiMax);
  phi->SetXTitle("#phi");
  phi->SetTitle(dataset);
  TH1D *phicleaned = new TH1D("phicleaned","",phiBin,phiMin,phiMax);
  phicleaned->SetXTitle("#phi");
  phicleaned->SetTitle(dataset);
  TH1D *Ebarrel = new TH1D("Ebarrel","",ptBin,ptMin,ptMax);
  Ebarrel->SetXTitle("E_{barrel} [GeV]");
  Ebarrel->SetTitle(dataset);
  TH1D *Ebarrelcleaned = new TH1D("Ebarrelcleaned","",ptBin,ptMin,ptMax);
  Ebarrelcleaned->SetXTitle("E_{barrel} [GeV]");
  Ebarrelcleaned->SetTitle(dataset);
  TH1D *Eendcap = new TH1D("Eendcap","",292,ptMin,300);
  Eendcap->SetXTitle("E_{endcap} [GeV]");
  Eendcap->SetTitle(dataset);
  TH1D *Eendcapcleaned = new TH1D("Eendcapcleaned","",ptBin,ptMin,ptMax);
  Eendcapcleaned->SetXTitle("E_{endcap} [GeV]");
  Eendcapcleaned->SetTitle(dataset);
  
  // Jet Cleaning related variables
  TH1I *NlooseTracks = new TH1I("NlooseTracks","",50,0,50);
  NlooseTracks->SetXTitle("Number of loose tracks in the cone");
  NlooseTracks->SetTitle(dataset);
  TH1I *NtightTracks = new TH1I("NtightTracks","",50,0,50);
  NtightTracks->SetXTitle("Number of tight tracks in the cone");
  NtightTracks->SetTitle(dataset);
  TH1D *ChFracLoose = new TH1D("ChFracLoose","",101,-0.005,1.005);
  ChFracLoose->SetXTitle("charged fraction (loose tracks)");
  ChFracLoose->SetTitle(dataset);
  TH1D *ChFracTight = new TH1D("ChFracTight","",101,-0.005,1.005);
  ChFracTight->SetXTitle("charged fraction (tight tracks)");
  ChFracTight->SetTitle(dataset); 
  TH1D *resemf = new TH1D("resemf","",101,-0.005,1.005);
  resemf->SetXTitle("restricted emf");
  resemf->SetTitle(dataset);
  TH1D *fhpd = new TH1D("fhpd","",101,-0.005,1.005);
  fhpd->SetXTitle("f_{HPD}");
  fhpd->SetTitle(dataset);
  TH1D *frbx = new TH1D("frbx","",101,-0.005,1.005);
  frbx->SetXTitle("f_{RBX}");
  frbx->SetTitle(dataset);
  TH1I *n90hits = new TH1I("n90hits","",50,0,50);
  n90hits->SetXTitle("N_{90}hits");
  n90hits->SetTitle(dataset);


  TH2D *mapall = new TH2D("mapall","",50,-5.,5.,24,-3.2,3.2);
  mapall->SetXTitle("#eta_{jet}");
  mapall->SetYTitle("#phi_{jet}");
  mapall->SetTitle(dataset);
  TH2D *map = new TH2D("map","",50,-5.,5.,24,-3.2,3.2);
  map->SetXTitle("#eta_{jet}");
  map->SetYTitle("#phi_{jet}");
  map->SetTitle(dataset);
  TH2D *mapcleaned = new TH2D("mapcleaned","",50,-5.,5.,24,-3.2,3.2);
  mapcleaned->SetXTitle("#eta_{jet}");
  mapcleaned->SetYTitle("#phi_{jet}");
  mapcleaned->SetTitle(dataset);




  // ------------------------Di Jets - all dijets are   ----------------------
  TH1D *dijetptall1 = new TH1D("dijetptall1","",ptBin, ptMinDijet,ptMax);
  dijetptall1->SetXTitle("p_{T}[GeV]");
  dijetptall1->SetTitle(dataset);
  TH1D *dijetptall2 = new TH1D("dijetptall2","",ptBin, ptMinDijet ,ptMax);
  dijetptall2->SetXTitle("p_{T}[GeV]");
  dijetptall2->SetTitle(dataset);
  TH1D *dijetdphi = new TH1D("dijetdphi","",phiBin, 0., 3.5);
//   dijetdphi->SetXTitle("p_{T}[GeV]");
  dijetdphi->SetXTitle("#Delta #phi_{di-jet}");
  dijetdphi->SetTitle(dataset);
  TH1D *dijeteta = new TH1D("dijeteta","",25,etaMin,etaMax);
  dijeteta->SetXTitle("#eta");
  dijeteta->SetTitle(dataset);
  TH1D *dijetphi = new TH1D("dijetphi","",25,phiMin,phiMax);
  dijetphi->SetXTitle("#phi");
  dijetphi->SetTitle(dataset);
  TH1D *dijetetacleaned = new TH1D("dijetetacleaned","",25,etaMin,etaMax);
  dijetetacleaned->SetXTitle("#eta");
  dijetetacleaned->SetTitle(dataset);
  TH1D *dijetphicleaned = new TH1D("dijetphicleaned","",25,phiMin,phiMax);
  dijetphicleaned->SetXTitle("#phi");
  dijetphicleaned->SetTitle(dataset);
  TH2D *mapalldijets = new TH2D("mapalldijets","",25,etaMin,etaMax,24,-3.2,3.2);
  mapalldijets->SetXTitle("#eta_{jet}");
  mapalldijets->SetYTitle("#phi_{jet}");
  mapalldijets->SetTitle(dataset);
  TH1I *NlooseTracksdijets = new TH1I("NlooseTracksdijets","",50,0,50);
  NlooseTracksdijets->SetXTitle("Number of loose tracks in the cone");
  NlooseTracksdijets->SetTitle(dataset);
  TH1I *NtightTracksdijets = new TH1I("NtightTracksdijets","",50,0,50);
  NtightTracksdijets->SetXTitle("Number of tight tracks in the cone");
  NtightTracksdijets->SetTitle(dataset);
  TH1D *ChFracLoosedijets = new TH1D("ChFracLoosedijets","",101,-0.005,1.005);
  ChFracLoosedijets->SetXTitle("charged fraction (loose tracks)");
  ChFracLoosedijets->SetTitle(dataset);
  TH1D *ChFracTightdijets = new TH1D("ChFracTightdijets","",101,-0.005,1.005);
  ChFracTightdijets->SetXTitle("charged fraction (tight tracks)");
  ChFracTightdijets->SetTitle(dataset);
  TH1D *resemfdijets = new TH1D("resemfdijets","",101,-0.005,1.005);
  resemfdijets->SetXTitle("restricted emf");
  resemfdijets->SetTitle(dataset);
  TH1D *fhpddijets = new TH1D("fhpddijets","",101,-0.005,1.005);
  fhpddijets->SetXTitle("f_{HPD}");
  fhpddijets->SetTitle(dataset);
  TH1D *frbxdijets = new TH1D("frbxdijets","",101,-0.005,1.005);
  frbxdijets->SetXTitle("f_{RBX}");
  frbxdijets->SetTitle(dataset);
  TH1I *n90hitsdijets = new TH1I("n90hitsdijets","",50,0,50);
  n90hitsdijets->SetXTitle("N_{90}hits");
  n90hitsdijets->SetTitle(dataset);
  TH1I *njetsindijets = new TH1I("njetsindijets","",20,0,20);
  njetsindijets->SetXTitle("Number of jets per event");
  njetsindijets->SetTitle(dataset); 
  TH1D *dijetptall1cleaned = new TH1D("dijetptall1cleaned","",ptBin, ptMinDijet,ptMax);
  dijetptall1cleaned->SetXTitle("p_{T}[GeV]");
  dijetptall1cleaned->SetTitle(dataset);
  TH1D *dijetptall2cleaned = new TH1D("dijetptall2cleaned","",ptBin, ptMinDijet ,ptMax);
  dijetptall2cleaned->SetXTitle("p_{T}[GeV]");
  dijetptall2cleaned->SetTitle(dataset);
  TH1D *dijetdphicleaned = new TH1D("dijetdphicleaned","",phiBin, 0., 3.5);
  dijetdphicleaned->SetXTitle("#Delta #phi_{di-jet}");
  dijetdphicleaned->SetTitle(dataset);
  TH2D *mapalldijetscleaned = new TH2D("mapalldijetscleaned","",25,etaMin,etaMax,24,-3.2,3.2);
  mapalldijetscleaned->SetXTitle("#eta_{jet}");
  mapalldijetscleaned->SetYTitle("#phi_{jet}");
  mapalldijetscleaned->SetTitle(dataset);
  TH1I *njetsindijetscleaned = new TH1I("njetsindijetscleaned","",20,0,20);
  njetsindijetscleaned->SetXTitle("Number of jets per event");
  njetsindijetscleaned->SetTitle(dataset);

 
  // -----------------------Efficiency ---------------------------------------------------

  TH1D *variousEffindijets = new TH1D("variousEffindijets","",4,0,4);
  variousEffindijets->SetTitle(dataset);
  variousEffindijets->GetXaxis()->SetBinLabel(1,"Loose JetID");
  variousEffindijets->GetXaxis()->SetBinLabel(2,"Tight JetID");
  variousEffindijets->GetXaxis()->SetBinLabel(3,">1 Associated Loose Tracks");
  variousEffindijets->GetXaxis()->SetBinLabel(4,">1 Associated Tight Tracks");
 //  TH1D *jetcleaningeffeta = new TH1D("jetcleaningeffeta","",100,-5,5);
  TH1D *jetcleaningeffeta = new TH1D("jetcleaningeffeta","",etaBin, etaMin, etaMax);
  TH1D *jetcleaningeffphi = new TH1D("jetcleaningeffphi","",phiBin,phiMin,phiMax);
  
  TH1D *variousEff = new TH1D("variousEff","",4,0,4);
  variousEff->SetTitle(dataset);
  variousEff->GetXaxis()->SetBinLabel(1,"Loose JetID");
  variousEff->GetXaxis()->SetBinLabel(2,"Tight JetID");
  variousEff->GetXaxis()->SetBinLabel(3,">1 Ass. Loose Tracks");
  variousEff->GetXaxis()->SetBinLabel(4,">1 Ass. Tight Tracks");
  
  TH1D *evtNumber = new TH1D("evtNumber","",7,0,7);
  evtNumber->SetTitle(dataset);
  evtNumber->GetXaxis()->SetBinLabel(1,"Evt");
  evtNumber->GetXaxis()->SetBinLabel(2,"BPTX");
  evtNumber->GetXaxis()->SetBinLabel(3,"BSC");
  evtNumber->GetXaxis()->SetBinLabel(4,"HALO");
  evtNumber->GetXaxis()->SetBinLabel(5,"PHYSBIT");
  evtNumber->GetXaxis()->SetBinLabel(6,"Monster");
  evtNumber->GetXaxis()->SetBinLabel(7,"PV");

  
  TH1D *cutEff = new TH1D("cutEff","",7,0,7);
  cutEff->SetTitle(dataset);
  cutEff->GetXaxis()->SetBinLabel(1,"Good run evts");
  cutEff->GetXaxis()->SetBinLabel(2,"BPTX");
  cutEff->GetXaxis()->SetBinLabel(3,"BSC");
  cutEff->GetXaxis()->SetBinLabel(4,"HALO");
  cutEff->GetXaxis()->SetBinLabel(5,"PHYSBIT");
  cutEff->GetXaxis()->SetBinLabel(6,"Monster");
  cutEff->GetXaxis()->SetBinLabel(7,"PV"); 
  
  TH1D *jetNumber = new TH1D("jetNumber","",9,0,9);
  jetNumber->SetTitle(dataset);
  jetNumber->GetXaxis()->SetBinLabel(1,"Jet");
  jetNumber->GetXaxis()->SetBinLabel(2,"BPTX");
  jetNumber->GetXaxis()->SetBinLabel(3,"BSC");
  jetNumber->GetXaxis()->SetBinLabel(4,"HALO");
  jetNumber->GetXaxis()->SetBinLabel(5,"PHYSBIT");
  jetNumber->GetXaxis()->SetBinLabel(6,"Monster");
  jetNumber->GetXaxis()->SetBinLabel(7,"PV");
  // after all trigger/PV/Monster .
  jetNumber->GetXaxis()->SetBinLabel(8,"All + #eta, p_{T} cuts");
  jetNumber->GetXaxis()->SetBinLabel(9,"All + Lose Cleaning");
 
 
  // fake jetes -----------------------
  TH1D *fhpdfakejets = new TH1D("fhpdfakejets","",101,-0.005,1.005);
  fhpdfakejets->SetXTitle("f_{HPD}");
  fhpdfakejets->SetTitle(dataset);
  TH1I *n90hitsfakejets = new TH1I("n90hitsfakejets","",50,0,50);
  n90hitsfakejets->SetXTitle("N_{90}hits");
  n90hitsfakejets->SetTitle(dataset);
  TH1D *resemffakejets = new TH1D("resemffakejets","",101,-0.005,1.005);
  resemffakejets->SetXTitle("restricted emf");
  resemffakejets->SetTitle(dataset);
  TH1D *fakejetptall1 = new TH1D("fakejetptall1","",ptBin, 0,50);
  fakejetptall1->SetXTitle("p_{T}[GeV]");
  fakejetptall1->SetTitle(dataset);


  // ----------------------- IC5  ---------------------------------------------------

  TH1I *ic5njets = new TH1I("ic5njets","",20,0,20);
  ic5njets->SetXTitle("Number of jets per event");
  ic5njets->SetTitle(dataset);
  TH1I *ic5nalljets = new TH1I("ic5nalljets","",20,0,20);
  ic5nalljets->SetXTitle("Number of jets per event");
  ic5nalljets->SetTitle(dataset);
  TH1I *ic5nconst = new TH1I("ic5nconst","",20,0,20);
  ic5nconst->SetXTitle("Number of constituents");
  ic5nconst->SetTitle(dataset);
  TH1I *ic5ncleanedjets = new TH1I("ic5ncleanedjets","",20,0,20);
  ic5ncleanedjets->SetXTitle("Number of jets per event");
  ic5ncleanedjets->SetTitle(dataset);
  TH1I *ic5nconstcleaned = new TH1I("ic5nconstcleaned","",20,0,20);
  ic5nconstcleaned->SetXTitle("Number of constituents"); 
  TH1D *ic5pt = new TH1D("ic5pt","",ptBin,ptMin,ptMax);
  ic5pt->SetXTitle("p_{T}[GeV]");
  ic5pt->SetTitle(dataset);
  TH1D *ic5ptcleaned = new TH1D("ic5ptcleaned","",ptBin,ptMin,ptMax);
  ic5ptcleaned->SetXTitle("p_{T}[GeV]");
  ic5ptcleaned->SetTitle(dataset);
  ic5nconstcleaned->SetTitle(dataset);
  // ----------------------- Histo End  ---------------------------------------------------


  
  ofstream  outfile;
  outfile.open("interestingevents.txt");  
  Long64_t nentries = fChain->GetEntriesFast();
  std::cout << "analysisClass::Loop(): nentries = " << nentries << std::endl;   

   Long64_t nb = 0;

   //counters   
   int NJetsTOT=0;
   int NJetIDLooseTOT=0;
   int NJetIDTightTOT=0;
   int NAssTrksLooseTOT=0;
   int NAssTrksTightTOT=0;
   int NJetsic5TOT=0;
   int Ncleanedic5TOT=0;
   int NindijetsTOT=0;
   int NindijetsJetIDLooseTOT=0;
   int NindijetsJetIDTightTOT=0;
   int NindijetsAssTrksLooseTOT=0;
   int NindijetsAssTrksTightTOT=0;

   int goodevts=0;
   int bptxevt=0;
   int bscevt=0;
   int beamhaloevt=0;
   int phybitevt=0;
   int trckevt=0;
   int pvevt=0;

   int alljets=0;
   int bptxjets=0;
   int bscjets=0;
   int beamhalojets=0;
   int phybitjets=0;
   int trckjets=0;
   int pvjets=0;

   
   for (Long64_t jentry=0; jentry<nentries;jentry++) 
     {
       Long64_t ientry = LoadTree(jentry);
       if (ientry < 0) break;
       nb = fChain->GetEntry(jentry);   
      
      if(jentry < 10 || jentry%1000 == 0) std::cout << "analysisClass::Loop(): jentry = " << jentry << std::endl;   
   
      // --------------------------------------------------------------------
      int isdata = isData;
      int eventid = event;
      int LS = ls;
      int runid = run;

          
      // skip some lumi sections...
      if(isdata == 1 && (
			 (runid==123596 && LS<2) ||
			 (runid==123615 && LS<70) ||
			 (runid==123732 && LS<62) ||
			 (runid==123815 && LS<62) ||
			 (runid==123815 && LS>109) ||
			 (runid==123818 && LS<2) ||
			 (runid==123818 && LS>42) ||
			 (runid==123908 && LS<2) ||
			 (runid==123908 && LS>12) ||
			 (runid==124008 && LS!=1) ||
			 (runid==124009 && LS>68) ||
			 (runid==124020 && LS<12) ||
			 (runid==124020 && LS>94) ||
			 (runid==124022 && LS<66) ||
			 (runid==124022 && LS>179) ||
			 (runid==124023 && LS<38) ||
			 (runid==124024 && LS<2) ||
			 (runid==124024 && LS>83) ||
			 (runid==124025 && LS<5) ||
			 (runid==124025 && LS>13) ||
			 (runid==124027 && LS<24) ||
			 (runid==124030 && LS<2)  )
	 ) {
	continue;
      }
      goodevts++;
      alljets+=ak5JetpT->size();
      // ---------------------------------------------------------------
      //########## Trigger Selection - to be 100 % sure: 
      int pass_BPTX              = 0;
      int pass_BSC_MB            = 0;
      int pass_BSC_BeamHaloVeto  = 0;
      int pass_PhysicsBit        = 0;
  
      //## pass_BPTX - Two beams crossing at CMS (only Data)
      if(isData==1)
	{
	  if(l1techbits->at(0)==1)
	    pass_BPTX = 1; 
  	    bptxevt++;
	    bptxjets+=ak5JetpT->size();
	}
      else if(isData==0)
	pass_BPTX = 1;
      
      //## pass_BSC_MB - BSC MinBias triggers firing (both Data and MC)
      if( l1techbits->at(40)==1 || l1techbits->at(41)==1 ) {	
	pass_BSC_MB = 1;
	bscevt++;
        bscjets+=ak5JetpT->size();
	}

      //## pass_BSC_BeamHaloVeto - Veto on BSC Beam Halo Triggers firing
      if(isData==1)
	{
	  pass_BSC_BeamHaloVeto = 1;
	  if( l1techbits->at(36) == 1 || l1techbits->at(37) == 1 || l1techbits->at(38) == 1 || l1techbits->at(39) == 1 )
	  pass_BSC_BeamHaloVeto = 0;	
	  
	  if (pass_BSC_BeamHaloVeto==1){
	    beamhaloevt++;
	    beamhalojets+=ak5JetpT->size();
	    }
	  
	}
      else if(isData == 0)
		pass_BSC_BeamHaloVeto = 1; 
      

      //## pass_PhysicsBit - HLT Physics Declared bit set 
      if(isData==1)
	{
	  if(hltbits->at(116)==1)
	    {pass_PhysicsBit = 1;
	    phybitevt++;
	    phybitjets+=ak5JetpT->size(); 
	    }
	}
      else if(isData == 0)
	pass_PhysicsBit = 1;
    
      if (pass_BPTX && 	pass_BSC_MB && pass_PhysicsBit && pass_BSC_BeamHaloVeto  ) {
     // ---------------------------------------------------------------
     //# Reco-based Selection
      //## pass_MonsterTRKEventVeto - "Monster Events" Tracker Filter
      //see https://twiki.cern.ch/twiki/bin/viewauth/CMS/TRKPromptFeedBack#Event_and_track_selection_recipe
      int pass_MonsterTRKEventVeto = 0;
      int num_good_tracks = 0;
      float fraction      = 0.;  
      float thresh        = 0.25;

      if(tracksPt->size()<=10){
	  pass_MonsterTRKEventVeto = 1;
      }//<=10 tracks	   
      else if(tracksPt->size()>10){
	  for (int ii=0; ii<tracksPt->size(); ii++)  {
	      int trackFlags = tracksQuality->at(ii);
	      int highPurityFlag = 3;
	      if( ( trackFlags & 1 << highPurityFlag) > 0)
		{
		  num_good_tracks++;		      
		  fraction = (float)num_good_tracks / (float)tracksPt->size();

		  if( fraction > thresh ) {
		    pass_MonsterTRKEventVeto = 1;
		    }
		}//if trackFlags & 1 << highPurityFla
	  }//int ii=0
	}//>10 tracks	    
      if ( pass_MonsterTRKEventVeto == 1){
      trckevt++; 
      trckjets+=ak5JetpT->size(); 
      }
      //PV event selection - cut on vertex for now. l1 tech bits already asked at skimming step
      //https://twiki.cern.ch/twiki/bin/viewauth/CMS/TRKPromptFeedBack#Event_and_track_selection_recipe   
      if(pass_MonsterTRKEventVeto && fabs(vertexZ->at(0))<15 && vertexNDF->at(0)>=5 && vertexisValid->at(0)==true){    // "newest" event selection
	pvevt++;
	pvjets+=ak5JetpT->size(); 	
	// --------------------------------------------------------------------
	
	//counters   
	int NJets=0;
	int NJetIDLoose=0;
	int NJetIDTight=0;
	int NAssTrksLoose=0;
	int NAssTrksTight=0;
	int NJetsic5=0;
	int Ncleanedic5=0;
	int Nindijets=0;
	int NindijetsJetIDLoose=0;
	int NALLindijetsJetIDLoose=0;
	int NindijetsJetIDTight=0;
	int NindijetsAssTrksLoose=0;
	int NindijetsAssTrksTight=0;
	nalljets->Fill(ak5JetpT->size());
	ic5nalljets->Fill(ic5JetpT->size());
	 
	// Inclusive  Jets -----------------------------------------------------------------------
	for (int j = 0; j<int(ak5JetpT->size()); j++){	  
	  // ----------------------
	  // JET CORRECTION
	  // ----------------------
	  double jcScale;    
	  if(makeJetCorr==true) jcScale = ak5JetscaleL2L3->at(j);
	  else jcScale = 1;	    
	  ptall->Fill(ak5JetpT->at(j) * jcScale);   
	  mapall->Fill(ak5JetEta->at(j),ak5JetPhi->at(j));

	  // Loop over Inclusive jets ----- 
	  if(ak5JetpT->at(j) * jcScale >ptMin && fabs(ak5JetEta->at(j))<endcapeta){    //jc
	    NJets++;	    
	    nconst->Fill(ak5JetNConstituents->at(j));
	    pt->Fill(ak5JetpT->at(j) * jcScale);    //jc 
	    if(fabs(ak5JetEta->at(j))<barreleta){
	      Ebarrel->Fill(ak5JetEnergy->at(j) * jcScale);  //jc
	    } else {
	      Eendcap->Fill(ak5JetEnergy->at(j) * jcScale);   //jc
	    }
	    map->Fill(ak5JetEta->at(j),ak5JetPhi->at(j)); 
	    eta->Fill(ak5JetEta->at(j));
	    phi->Fill(ak5JetPhi->at(j));
	    NlooseTracks->Fill(ak5JetNAssoTrksLoose->at(j));
	    NtightTracks->Fill(ak5JetNAssoTrksTight->at(j));
	    ChFracLoose->Fill(sqrt(pow(ak5JetLooseAssoTrkspx->at(j),2)+pow(ak5JetLooseAssoTrkspy->at(j),2))/(ak5JetpT->at(j) * jcScale));  //jc
	    ChFracTight->Fill(sqrt(pow(ak5JetTightAssoTrkspx->at(j),2)+pow(ak5JetTightAssoTrkspy->at(j),2))/(ak5JetpT->at(j) * jcScale));  //jc
	    resemf->Fill(ak5JetJIDresEMF->at(j));
	    fhpd->Fill(ak5JetJIDfHPD->at(j));
	    frbx->Fill(ak5JetJIDfRBX->at(j));
	    n90hits->Fill(ak5JetJIDn90Hits->at(j));	   
	    //calculate jetid loose
	    bool emf=false;	      
	    if(ak5JetJIDresEMF->at(j)>emffrac) emf=true;	        
	    else  emf=false;
	   
	    //fill the histos for cleaned jets
	    if(emf && ak5JetJIDfHPD->at(j)<fhpdmax && ak5JetJIDn90Hits->at(j)>n90hitsmin  ){//loose cleaning
	      NJetIDLoose++;
	      nconstcleaned->Fill(ak5JetNConstituents->at(j));
	      ptcleaned->Fill(ak5JetpT->at(j) * jcScale);  //jc
	      etacleaned->Fill(ak5JetEta->at(j));
	      phicleaned->Fill(ak5JetPhi->at(j));
	      
	      if(fabs(ak5JetEta->at(j))<barreleta){
		Ebarrelcleaned->Fill(ak5JetEnergy->at(j));
	      } else {
		Eendcapcleaned->Fill(ak5JetEnergy->at(j));
	      }
	      mapcleaned->Fill(ak5JetEta->at(j),ak5JetPhi->at(j));
	      cout<<runid<< " " << LS<< " "<< eventid << endl;
	      outfile<<runid<< "\t" << LS<< "\t"<< eventid << endl;
	    }
	    if(emf && ak5JetJIDfHPD->at(j)<fhpdmax && ak5JetJIDn90Hits->at(j)>n90hitsmin  ){//tight cleaning (for the moment equal to loose!!)
	      NJetIDTight++;
	    }	    
	    // @@@ Seems to be an error ////
	    if(ak5JetNAssoTrksLoose->at(j)>n90hitsmin){
	      NAssTrksLoose++;
	    }
	    if(ak5JetNAssoTrksTight->at(j)>n90hitsmin){
	      NAssTrksTight++;
	    }
	  } //pt min/ eta 
	} //loop on inclusive ak5 jets 
	
	   
	// --------------------DiJets---------------------------------------------------------------------   
	// JET CORRECTION
	// --------------------
	double jcScale0;
	double jcScale1;	
	//dijet
	if(int(ak5JetpT->size()>=2)){
	  // both passed pT and eta cuts
	  
	  if(makeJetCorr == true) {
	    jcScale0 = ak5JetscaleL2L3->at(0);
	    jcScale1 = ak5JetscaleL2L3->at(1);
	  }
	  else {
	    jcScale0=1;
	    jcScale1=1;
	  }
	  
	  if(fabs(ak5JetEta->at(0))<endcapeta && ak5JetpT->at(0) * jcScale0 >ptMinDijet && fabs(ak5JetEta->at(1))<endcapeta && ak5JetpT->at(1) * jcScale1 >ptMinDijet){   //jc
	    // dphi
	    double dphi = fabs(ak5JetPhi->at(0) - ak5JetPhi->at(1) );
	    if (dphi > 3.14) dphi=fabs(dphi -6.28 );
	    if (dphi >2.1) {
	      // fake jet study
	      double dijcScale;
	      for (int dj = 0; dj<int(ak5JetpT->size()); dj++){
		if(makeJetCorr==true) {
		  dijcScale = ak5JetscaleL2L3->at(dj);
		}
		else {
		  dijcScale = 1;
		}
		if(ak5JetpT->at(dj) * dijcScale >ptMinDijet) Nindijets++;
		// cleaning variables for fake jets in di-jet events @@@ cut to be adjuested
		if(dj>2 && ak5JetpT->at(dj)>2.) {
		  resemffakejets->Fill(ak5JetJIDresEMF->at(dj));
		  fhpdfakejets->Fill(ak5JetJIDfHPD->at(dj));
		  n90hitsfakejets->Fill(ak5JetJIDn90Hits->at(dj));
		  fakejetptall1 ->Fill(ak5JetpT->at(dj));
		}
	      }
	      
	      // basic di-jet variables 
	      dijetptall1->Fill(ak5JetpT->at(0) * jcScale0);  //jc
	      dijetptall2->Fill(ak5JetpT->at(1) * jcScale1);   //jc
	      dijetdphi->Fill(dphi);
	      mapalldijets->Fill(ak5JetEta->at(0),ak5JetPhi->at(0));
	      mapalldijets->Fill(ak5JetEta->at(1),ak5JetPhi->at(1));
      	      dijeteta->Fill(ak5JetEta->at(0));
	      dijeteta->Fill(ak5JetEta->at(1));
	      dijetphi->Fill(ak5JetPhi->at(0));
	      dijetphi->Fill(ak5JetPhi->at(1));
      	      NlooseTracksdijets->Fill(ak5JetNAssoTrksLoose->at(0));
	      NtightTracksdijets->Fill(ak5JetNAssoTrksTight->at(0));
	      NlooseTracksdijets->Fill(ak5JetNAssoTrksLoose->at(1));
	      NtightTracksdijets->Fill(ak5JetNAssoTrksTight->at(1));
	      ChFracLoosedijets->Fill(sqrt(pow(ak5JetLooseAssoTrkspx->at(0),2)+pow(ak5JetLooseAssoTrkspy->at(0),2))/ak5JetpT->at(0) * jcScale0);  //jc
	      ChFracLoosedijets->Fill(sqrt(pow(ak5JetLooseAssoTrkspx->at(1),2)+pow(ak5JetLooseAssoTrkspy->at(1),2))/ak5JetpT->at(1) * jcScale1);  //jc
	      ChFracTightdijets->Fill(sqrt(pow(ak5JetTightAssoTrkspx->at(0),2)+pow(ak5JetTightAssoTrkspy->at(0),2))/ak5JetpT->at(0) * jcScale0);  //jc
	      ChFracTightdijets->Fill(sqrt(pow(ak5JetTightAssoTrkspx->at(1),2)+pow(ak5JetTightAssoTrkspy->at(1),2))/ak5JetpT->at(1) * jcScale1);  //jc
	      //jetID variables for jets in the dijet sample
	      resemfdijets->Fill(ak5JetJIDresEMF->at(0));
	      fhpddijets->Fill(ak5JetJIDfHPD->at(0));
	      frbxdijets->Fill(ak5JetJIDfRBX->at(0));
	      n90hitsdijets->Fill(ak5JetJIDn90Hits->at(0));
	      resemfdijets->Fill(ak5JetJIDresEMF->at(1));
	      fhpddijets->Fill(ak5JetJIDfHPD->at(1));
	      frbxdijets->Fill(ak5JetJIDfRBX->at(1));
	      n90hitsdijets->Fill(ak5JetJIDn90Hits->at(1));

	      // both passed jet cleaning
	      if(ak5JetJIDresEMF->at(0)>emffrac && ak5JetJIDfHPD->at(0)<fhpdmax && ak5JetJIDn90Hits->at(0)>n90hitsmin && ak5JetJIDresEMF->at(1) >emffrac && ak5JetJIDfHPD->at(1)<fhpdmax && ak5JetJIDn90Hits->at(1)>n90hitsmin){  
		dijetptall1cleaned->Fill(ak5JetpT->at(0) * jcScale0);   //jc
		dijetptall2cleaned->Fill(ak5JetpT->at(1) * jcScale1);   //jc
		dijetdphicleaned->Fill(dphi);
		mapalldijetscleaned->Fill(ak5JetEta->at(0),ak5JetPhi->at(0));
		mapalldijetscleaned->Fill(ak5JetEta->at(1),ak5JetPhi->at(1));
		dijetetacleaned->Fill(ak5JetEta->at(0));
		dijetetacleaned->Fill(ak5JetEta->at(1));
		dijetphicleaned->Fill(ak5JetPhi->at(0));
		dijetphicleaned->Fill(ak5JetPhi->at(1));
		//now loop on jets and count how many CLEANED jets with pT>8 are in each event
		for (int dj = 0; dj<int(ak5JetpT->size()); dj++){
		  if(makeJetCorr==true) {
		    dijcScale = ak5JetscaleL2L3->at(dj);
		  }
		  else {
		    dijcScale = 1;
		  }
		  if(ak5JetpT->at(dj) * dijcScale >ptMinDijet && ak5JetJIDresEMF->at(dj)>emffrac && ak5JetJIDfHPD->at(dj)<fhpdmax && ak5JetJIDn90Hits->at(dj)>n90hitsmin && ak5JetEta->at(dj)<endcapeta){   ///
		    NALLindijetsJetIDLoose++;
		  }
		}
	      } //end  both passed jet cleaning

	      //how many of the jets in dijets events pass the loose jetID (look only at the two leading jets)
	      if(ak5JetJIDresEMF->at(0)>emffrac && ak5JetJIDfHPD->at(0)<fhpdmax && ak5JetJIDn90Hits->at(0)>n90hitsmin){
		NindijetsJetIDLoose++;
	      }
	      if(ak5JetJIDresEMF->at(1)>emffrac && ak5JetJIDfHPD->at(1)<fhpdmax && ak5JetJIDn90Hits->at(1)>n90hitsmin){
		NindijetsJetIDLoose++;
	      }	    	      
	      //how many of the jets in dijets events pass the tight jetID (look only at the two leading jets)
	      //FOR THE MOMENT THE TIGHT JETID IS EQUAL TO THE LOOSE ONE!
	      if(ak5JetJIDresEMF->at(0)>emffrac && ak5JetJIDfHPD->at(0)<fhpdmax && ak5JetJIDn90Hits->at(0)>n90hitsmin){
		NindijetsJetIDTight++;
	      }
	      if(ak5JetJIDresEMF->at(1)>emffrac && ak5JetJIDfHPD->at(1)<fhpdmax && ak5JetJIDn90Hits->at(1)>n90hitsmin){
		NindijetsJetIDTight++;
	      }
	      //how many of the jets in dijets events have two or more associated loose tracks
	      if(ak5JetNAssoTrksLoose->at(0)>1){
		NindijetsAssTrksLoose++;
	      }
	      if(ak5JetNAssoTrksLoose->at(1)>1){
		NindijetsAssTrksLoose++;
	      }
	      //how many of the jets in dijets events have two or more associated tight tracks
	      if(ak5JetNAssoTrksTight->at(0)>1){
		NindijetsAssTrksTight++;
	      }
	      if(ak5JetNAssoTrksTight->at(1)>1){
		NindijetsAssTrksTight++;
	      }
	    }//dphi cut
	  }//eta/pt cuts on dijets
	}//di jets >= 2 jets
	



	 // ------------------------IC5 ---------------------------------------------------------------------------    //jc	
	for(int j = 0; j<int(ic5JetpT->size()); j++){	     
	  // ------------------------
	  // JET CORRECTION
	  // -----------------------
	  double jcScale;    //jc	  
	  if(makeJetCorr==true)jcScale = ic5JetscaleL2L3->at(j);
	  else jcScale = 1;
	    	  
	  
	  if(ic5JetpT->at(j) * jcScale >ptMin && fabs(ic5JetEta->at(j))<endcapeta){   //jc
	    NJetsic5++;
	    ic5nconst->Fill(ic5JetNConstituents->at(j));
	    ic5pt->Fill(ic5JetpT->at(j) * jcScale);
	    bool emfic5=false;
	    if(fabs(ic5JetEta->at(j))<2.6){
	      if(ic5JetJIDresEMF->at(j)>emffrac) emfic5=true;
	    } else {
	      emfic5=true;
	    }
	    //fill the histos for cleaned jets
	    if(emfic5 && ic5JetJIDn90Hits->at(j)>n90hitsmin && ic5JetJIDfHPD->at(j)<fhpdmax ){//loose cleaning
	      Ncleanedic5++;
	      ic5nconstcleaned->Fill(ic5JetNConstituents->at(j));
	    }
	  }
	}



	//after looping on jets
	ic5njets->Fill(NJetsic5);     
	ic5ncleanedjets->Fill(Ncleanedic5);	
	njets->Fill(NJets);     
	ncleanedjets->Fill(NJetIDLoose);
	njetsindijetscleaned->Fill(NALLindijetsJetIDLoose);
	njetsindijets->Fill(Nindijets);	
	//Counting Jets
	NJetsTOT+=NJets;
	NJetIDLooseTOT+=NJetIDLoose;
	NJetIDTightTOT+=NJetIDTight;
	NAssTrksLooseTOT+=NAssTrksLoose;
	NAssTrksTightTOT+=NAssTrksTight;
	NJetsic5TOT+=NJetsic5;
	Ncleanedic5TOT+=Ncleanedic5;
	NindijetsTOT+=Nindijets;
	NindijetsJetIDLooseTOT+=NindijetsJetIDLoose;
	NindijetsJetIDTightTOT+=NindijetsJetIDTight;
	NindijetsAssTrksLooseTOT+=NindijetsAssTrksLoose;
	NindijetsAssTrksTightTOT+=NindijetsAssTrksTight;
      } //vertex monster event
      }// techbits
     } // End loop over events
   


   //--------------------------------------------------------------------------------------------------

   //efficiency histos
   if(NJetsTOT>0){
     cout<< 1.*NJetIDLooseTOT <<"  /  " <<1.*NJetsTOT << "  =  " << endl;
     cout<< (1.*NJetIDLooseTOT/(1.*NJetsTOT)) << endl;
     variousEff->SetBinContent(1,(1.*NJetIDLooseTOT/(1.*NJetsTOT)));
     variousEff->SetBinContent(2,(1.*NJetIDTightTOT/(1.*NJetsTOT)));
     variousEff->SetBinContent(3,(1.*NAssTrksLooseTOT/(1.*NJetsTOT)));
     variousEff->SetBinContent(4,(1.*NAssTrksTightTOT/(1.*NJetsTOT)));
   }
   //efficiency histos
   if(NindijetsTOT>0){
     variousEffindijets->SetBinContent(1,(1.*NindijetsJetIDLooseTOT/(1.*NindijetsTOT)));
     variousEffindijets->SetBinContent(2,(1.*NindijetsJetIDTightTOT/(1.*NindijetsTOT)));
     variousEffindijets->SetBinContent(3,(1.*NindijetsAssTrksLooseTOT/(1.*NindijetsTOT)));
     variousEffindijets->SetBinContent(4,(1.*NindijetsAssTrksTightTOT/(1.*NindijetsTOT)));
   }
   if(goodevts>0){ 
     cutEff->SetMaximum(1.3);
     cutEff->SetBinContent(1,1 );
     cutEff->SetBinContent(2,(1.*bptxevt)/(1*goodevts));
     cutEff->SetBinContent(3,(1.*bscevt)/(1*goodevts));
     cutEff->SetBinContent(4,(1.*beamhaloevt)/(1*goodevts));
     cutEff->SetBinContent(5,(1.*phybitevt)/(1*goodevts));
     cutEff->SetBinContent(6,(1.*trckevt)/(1 *goodevts));
     cutEff->SetBinContent(7,(1.*pvevt)/(1*goodevts)); 
   }
   
  
   jetNumber->SetBinContent(1,  alljets );
   jetNumber->SetBinContent(2,   bptxjets);
   jetNumber->SetBinContent(3,   bscjets );
   jetNumber->SetBinContent(4,   beamhalojets );
   jetNumber->SetBinContent(5,   phybitjets );
   jetNumber->SetBinContent(6,   trckjets );
   jetNumber->SetBinContent(7,   pvjets ); 
   jetNumber->SetBinContent(8,NJetsTOT );
   jetNumber->SetBinContent(9,NJetIDLooseTOT );
   
   evtNumber->SetBinContent(1,  goodevts );
   evtNumber->SetBinContent(2,   bptxevt);
   evtNumber->SetBinContent(3,   bscevt );
   evtNumber->SetBinContent(4,   beamhaloevt );
   evtNumber->SetBinContent(5,   phybitevt );
   evtNumber->SetBinContent(6,   trckevt );
   evtNumber->SetBinContent(7,   pvevt ); 

   //  jetNumber->SetBinContent(3,NindijetsTOT ) ;
   // jetNumber->SetBinContent(4,NindijetsJetIDLooseTOT );
      cout <<"###################################"       << endl;
      cout <<"Good Events " << goodevts      <<" Selected events="<< pvevt<<  endl;

      cout <<"###################################"       << endl;

   // cleaning efficiencies
   jetcleaningeffeta->Add(etacleaned);
   jetcleaningeffeta->Divide(eta);
   jetcleaningeffphi->Add(phicleaned);
   jetcleaningeffphi->Divide(phi);
   
   
   //////////write histos 
   outfile.close();
   ptall->Write();
   pt->Write();
   ptcleaned->Write();
   mapall->Write();
   map->Write();
   mapcleaned->Write();
   Ebarrel->Write();
   Ebarrelcleaned->Write();
   Eendcap->Write();
   Eendcapcleaned->Write();
   resemf->Write();
   fhpd->Write();
   frbx->Write();
   n90hits->Write();
   nalljets->Write();
   njets->Write();
   nconst->Write();
   ncleanedjets->Write();
   nconstcleaned->Write();
   ic5nalljets->Write();
   ic5njets->Write();
   ic5nconst->Write();
   ic5ncleanedjets->Write();
   ic5nconstcleaned->Write();	 
   phi->Write();
   phicleaned->Write();
   eta->Write(); 
   etacleaned->Write(); 
   NlooseTracks->Write();
   NtightTracks->Write();
   ChFracLoose->Write();
   ChFracTight->Write();
   dijetptall1->Write();
   dijetptall2->Write();
   dijetdphi->Write();
   mapalldijets->Write();
   NlooseTracksdijets->Write();
   NtightTracksdijets->Write();
   ChFracLoosedijets->Write();
   ChFracTightdijets->Write();
   dijetptall1cleaned->Write();
   dijetptall2cleaned->Write();
   dijetdphicleaned->Write();
   mapalldijetscleaned->Write();
   njetsindijets->Write();
   njetsindijetscleaned->Write();
   NlooseTracksdijets->Write();
   NtightTracksdijets->Write();
   resemfdijets->Write();
   fhpddijets->Write();
   frbxdijets->Write();
   n90hitsdijets->Write();
   variousEff->Write();
   cutEff->Write();
   jetNumber->Write(); 
   evtNumber->Write();
   variousEffindijets->Write();
   resemffakejets->Write();
   fhpdfakejets->Write();
   n90hitsfakejets->Write();
   fakejetptall1 ->Write();
   
   dijeteta->Write();
   dijetphi->Write();
   dijetetacleaned->Write();
   dijetphicleaned->Write();

   ic5pt->Write();
   jetcleaningeffeta->Write();
   jetcleaningeffphi->Write();
   jetNumber->Write();
   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}
