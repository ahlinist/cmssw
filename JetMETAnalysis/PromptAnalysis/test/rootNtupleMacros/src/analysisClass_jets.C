#define analysisClass_cxx
#include "analysisClass.h"
#include <TH2.h>
#include <TH1F.h>
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

   if (fChain == 0) return;
   double ptMin=8.;
   double ptMax=100.;
   int ptBin=50;
   
  int  phiBin=100;
  double phiMax=-3.2;
  double phiMin=3.2;
  
  int  etaBin=100;
  double etaMax=-5  ;
  double etaMin=5  ;


   
  TH1I *ak5njets = new TH1I("ak5njets","",20,0,20);
  ak5njets->SetXTitle("Number of jets per event");
  ak5njets->SetTitle(dataset);
  TH1I *ak5nalljets = new TH1I("ak5nalljets","",20,0,20);
  ak5nalljets->SetXTitle("Number of jets per event");
  ak5nalljets->SetTitle(dataset);
  TH1I *ak5nconst = new TH1I("ak5nconst","",20,0,20);
  ak5nconst->SetXTitle("Number of constituents");
  ak5nconst->SetTitle(dataset);
  TH1I *ak5ncleanedjets = new TH1I("ak5ncleanedjets","",20,0,20);
  ak5ncleanedjets->SetXTitle("Number of jets per event");
  ak5ncleanedjets->SetTitle(dataset);
  TH1I *ak5nconstcleaned = new TH1I("ak5nconstcleaned","",20,0,20);
  ak5nconstcleaned->SetXTitle("Number of constituents");
  ak5nconstcleaned->SetTitle(dataset);


  TH1I *kt4njets = new TH1I("kt4njets","",20,0,20);
  kt4njets->SetXTitle("Number of jets per event");
  kt4njets->SetTitle(dataset);
  TH1I *kt4nalljets = new TH1I("kt4nalljets","",20,0,20);
  kt4nalljets->SetXTitle("Number of jets per event");
  kt4nalljets->SetTitle(dataset);
  TH1I *kt4nconst = new TH1I("kt4nconst","",20,0,20);
  kt4nconst->SetXTitle("Number of constituents");
  kt4nconst->SetTitle(dataset);
  TH1I *kt4ncleanedjets = new TH1I("kt4ncleanedjets","",20,0,20);
  kt4ncleanedjets->SetXTitle("Number of jets per event");
  kt4ncleanedjets->SetTitle(dataset);
  TH1I *kt4nconstcleaned = new TH1I("kt4nconstcleaned","",20,0,20);
  kt4nconstcleaned->SetXTitle("Number of constituents");
  kt4nconstcleaned->SetTitle(dataset);
  TH1D *ptall = new TH1D("ptall","",ptBin+8,0.,ptMax);
  ptall->SetXTitle("p_{T}[GeV]");
  ptall->SetTitle(dataset);

  TH1I *ak5NlooseTracks = new TH1I("ak5NlooseTracks","",50,0,50);
  ak5NlooseTracks->SetXTitle("Number of loose tracks in the cone");
  ak5NlooseTracks->SetTitle(dataset);
  TH1I *ak5NtightTracks = new TH1I("ak5NtightTracks","",50,0,50);
  ak5NtightTracks->SetXTitle("Number of tight tracks in the cone");
  ak5NtightTracks->SetTitle(dataset);
  TH1D *ChFracLoose = new TH1D("ChFracLoose","",101,-0.005,1.005);
  ChFracLoose->SetXTitle("charged fraction (loose tracks)");
  ChFracLoose->SetTitle(dataset);
  TH1D *ChFracTight = new TH1D("ChFracTight","",101,-0.005,1.005);
  ChFracTight->SetXTitle("charged fraction (tight tracks)");
  ChFracTight->SetTitle(dataset);

  TH1D *dijetptall1 = new TH1D("dijetptall1","",ptBin, ptMin,ptMax);
  dijetptall1->SetXTitle("p_{T}[GeV]");
  dijetptall1->SetTitle(dataset);
  TH1D *dijetptall2 = new TH1D("dijetptall2","",ptBin, ptMin ,ptMax);
  dijetptall2->SetXTitle("p_{T}[GeV]");
  dijetptall2->SetTitle(dataset);
  TH1D *dijetdphi = new TH1D("dijetdphi","",phiBin, 0., 3.5);
  dijetdphi->SetXTitle("p_{T}[GeV]");
  dijetdphi->SetTitle(dataset);
  TH2D *mapalldijets = new TH2D("mapalldijets","",25,-2.4,2.4,24,-3.2,3.2);
  mapalldijets->SetXTitle("#eta_{jet}");
  mapalldijets->SetYTitle("#phi_{jet}");
  mapalldijets->SetTitle(dataset);
  TH1I *ak5NlooseTracksdijets = new TH1I("ak5NlooseTracksdijets","",50,0,50);
  ak5NlooseTracksdijets->SetXTitle("Number of loose tracks in the cone");
  ak5NlooseTracksdijets->SetTitle(dataset);
  TH1I *ak5NtightTracksdijets = new TH1I("ak5NtightTracksdijets","",50,0,50);
  ak5NtightTracksdijets->SetXTitle("Number of tight tracks in the cone");
  ak5NtightTracksdijets->SetTitle(dataset);
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
  TH1I *ak5njetsindijets = new TH1I("ak5njetsindijets","",20,0,20);
  ak5njetsindijets->SetXTitle("Number of jets per event");
  ak5njetsindijets->SetTitle(dataset);


  TH1D *dijetptall1cleaned = new TH1D("dijetptall1cleaned","",ptBin, ptMin,ptMax);
  dijetptall1cleaned->SetXTitle("p_{T}[GeV]");
  dijetptall1cleaned->SetTitle(dataset);
  TH1D *dijetptall2cleaned = new TH1D("dijetptall2cleaned","",ptBin, ptMin ,ptMax);
  dijetptall2cleaned->SetXTitle("p_{T}[GeV]");
  dijetptall2cleaned->SetTitle(dataset);
  TH1D *dijetdphicleaned = new TH1D("dijetdphicleaned","",phiBin, 0., 3.5);
  dijetdphicleaned->SetXTitle("p_{T}[GeV]");
  dijetdphicleaned->SetTitle(dataset);
  TH2D *mapalldijetscleaned = new TH2D("mapalldijetscleaned","",25,-2.4,2.4,24,-3.2,3.2);
  mapalldijetscleaned->SetXTitle("#eta_{jet}");
  mapalldijetscleaned->SetYTitle("#phi_{jet}");
  mapalldijetscleaned->SetTitle(dataset);
  TH1I *ak5njetsindijetscleaned = new TH1I("ak5njetsindijetscleaned","",20,0,20);
  ak5njetsindijetscleaned->SetXTitle("Number of jets per event");
  ak5njetsindijetscleaned->SetTitle(dataset);


  TH1D *pt = new TH1D("pt","",ptBin,ptMin,ptMax);
  pt->SetXTitle("p_{T}[GeV]");
  pt->SetTitle(dataset);
  TH1D *ptcleaned = new TH1D("ptcleaned","",ptBin,ptMin,ptMax);
  ptcleaned->SetXTitle("p_{T}[GeV]");
  ptcleaned->SetTitle(dataset);

  TH1D *eta = new TH1D("eta","",etaBin,etaMin,etaMax);
  eta->SetXTitle("p_{T}[GeV]");
  eta->SetTitle(dataset);
  TH1D *etacleaned = new TH1D("etacleaned","",etaBin,etaMin,etaMax);
  etacleaned->SetXTitle("p_{T}[GeV]");
  etacleaned->SetTitle(dataset);

  TH1D *phi = new TH1D("phi","",phiBin,phiMin,phiMax);
  phi->SetXTitle("p_{T}[GeV]");
  phi->SetTitle(dataset);
  TH1D *phicleaned = new TH1D("phicleaned","",phiBin,phiMin,phiMax);
  phicleaned->SetXTitle("p_{T}[GeV]");
  phicleaned->SetTitle(dataset);

  

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
  TH1D *Ebarrel = new TH1D("Ebarrel","",ptBin,ptMin,ptMax);
  Ebarrel->SetXTitle("E_{barrel} [GeV]");
  Ebarrel->SetTitle(dataset);
  TH1D *Ebarrelcleaned = new TH1D("Ebarrelcleaned","",ptBin,ptMin,ptMax);
  Ebarrelcleaned->SetXTitle("E_{barrel} [GeV]");
  Ebarrelcleaned->SetTitle(dataset);
  //  TH1D *Eendcap = new TH1D("Eendcap","",ptBin,ptMin,ptMax);
  TH1D *Eendcap = new TH1D("Eendcap","",292,ptMin,300);
  Eendcap->SetXTitle("E_{endcap} [GeV]");
  Eendcap->SetTitle(dataset);
  TH1D *Eendcapcleaned = new TH1D("Eendcapcleaned","",ptBin,ptMin,ptMax);
  Eendcapcleaned->SetXTitle("E_{endcap} [GeV]");
  Eendcapcleaned->SetTitle(dataset);
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

   //////////book histos here

   //2D histograms

       ofstream  outfile;
       outfile.open("interestingevents.txt");

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


       //event selection - cut on vertex for now. l1 tech bits already asked at skimming step
       if(vertexNTracks->at(0)>1. && fabs(vertexZ->at(0))<20){
	   int eventid = event;
	   int LS = ls;
	   int runid = run;
	   
	   ak5nalljets->Fill(ak5JetpT->size());
	   kt4nalljets->Fill(kt4JetpT->size());
	   int Nak5=0;
	   int Nkt4=0;
	   int Ncleanedak5=0;
	   int Ncleanedkt4=0;
	   int Nak5indijets=0;
	   int Ncleanedak5indijets=0;
	   //
	   for (int j = 0; j<int(ak5JetpT->size()); j++){
	  
	      ptall->Fill(ak5JetpT->at(j));
	      mapall->Fill(ak5JetEta->at(j),ak5JetPhi->at(j));
	     
	      if(ak5JetpT->at(j)>ptMin){
	       Nak5++;
	       ak5nconst->Fill(ak5JetNConstituents->at(j));
	       pt->Fill(ak5JetpT->at(j));
	       if(fabs(ak5JetEta->at(j))<1.4){
		 Ebarrel->Fill(ak5JetEnergy->at(j));
	       } else {
		 Eendcap->Fill(ak5JetEnergy->at(j));
	       }
	       map->Fill(ak5JetEta->at(j),ak5JetPhi->at(j)); 
	       eta->Fill(ak5JetEta->at(j));
	       phi->Fill(ak5JetPhi->at(j));
	       ak5NlooseTracks->Fill(ak5JetNAssoTrksLoose->at(j));
	       ak5NtightTracks->Fill(ak5JetNAssoTrksTight->at(j));
	       ChFracLoose->Fill(sqrt(pow(ak5JetLooseAssoTrkspx->at(j),2)+pow(ak5JetLooseAssoTrkspy->at(j),2))/ak5JetpT->at(j));
	       ChFracTight->Fill(sqrt(pow(ak5JetTightAssoTrkspx->at(j),2)+pow(ak5JetTightAssoTrkspy->at(j),2))/ak5JetpT->at(j));
	       resemf->Fill(ak5JetJIDresEMF->at(j));
	       fhpd->Fill(ak5JetJIDfHPD->at(j));
	       frbx->Fill(ak5JetJIDfRBX->at(j));
	       n90hits->Fill(ak5JetJIDn90Hits->at(j));
	       //calculate jetid loose
	       bool emf=false;	      
		 if(ak5JetJIDresEMF->at(j)>0.01) emf=true;	        else  emf=false;
	       //fill the histos for cleaned jets
		 //  if(emf && ak5JetJIDn90Hits->at(j)>1 && ak5JetJIDfHPD->at(j)<0.98 ){//loose cleaning
		 if(emf && ak5JetJIDfHPD->at(j)<0.98 && ak5JetJIDn90Hits->at(j)>1  ){//loose cleaning
		   
		 Ncleanedak5++;
		 ak5nconstcleaned->Fill(ak5JetNConstituents->at(j));
		 ptcleaned->Fill(ak5JetpT->at(j)); 
		 etacleaned->Fill(ak5JetEta->at(j));
		 phicleaned->Fill(ak5JetPhi->at(j));

		 if(fabs(ak5JetEta->at(j))<1.4){
		   Ebarrelcleaned->Fill(ak5JetEnergy->at(j));
		 } else {
		   Eendcapcleaned->Fill(ak5JetEnergy->at(j));
		 }
		 mapcleaned->Fill(ak5JetEta->at(j),ak5JetPhi->at(j));
		 cout<<runid<< " " << LS<< " "<< eventid << endl;
		 outfile<<runid<< "\t" << LS<< "\t"<< eventid << endl;
		 }
	      } //<  8
	   } //jety loop


	   //dijet
	   if(int(ak5JetpT->size()>=2)){
	     // both passed pT and eta cuts
	     if(fabs(ak5JetEta->at(0))<2.4 && ak5JetpT->at(0)>ptMin && fabs(ak5JetEta->at(1))<2.4 && ak5JetpT->at(1)>ptMin){
	       // dphi
	       double dphi = fabs(ak5JetPhi->at(0) - ak5JetPhi->at(1) );
	       if (dphi > 3.14) dphi=fabs(dphi -6.28 );
	       if (dphi >2.1) {
		 for (int dj = 0; dj<int(ak5JetpT->size()); dj++){
		   if(ak5JetpT->at(dj)>ptMin) Nak5indijets++;
		 }
		 dijetptall1->Fill(ak5JetpT->at(0));
		 dijetptall2->Fill(ak5JetpT->at(1)); 
		 dijetdphi->Fill(dphi);
		 mapalldijets->Fill(ak5JetEta->at(0),ak5JetPhi->at(0));
		 mapalldijets->Fill(ak5JetEta->at(1),ak5JetPhi->at(1));
		 ak5NlooseTracksdijets->Fill(ak5JetNAssoTrksLoose->at(0));
		 ak5NtightTracksdijets->Fill(ak5JetNAssoTrksTight->at(0));
		 ak5NlooseTracksdijets->Fill(ak5JetNAssoTrksLoose->at(1));
		 ak5NtightTracksdijets->Fill(ak5JetNAssoTrksTight->at(1));
		 ChFracLoosedijets->Fill(sqrt(pow(ak5JetLooseAssoTrkspx->at(0),2)+pow(ak5JetLooseAssoTrkspy->at(0),2))/ak5JetpT->at(0));
		 ChFracLoosedijets->Fill(sqrt(pow(ak5JetLooseAssoTrkspx->at(1),2)+pow(ak5JetLooseAssoTrkspy->at(1),2))/ak5JetpT->at(1));
		 ChFracTightdijets->Fill(sqrt(pow(ak5JetTightAssoTrkspx->at(0),2)+pow(ak5JetTightAssoTrkspy->at(0),2))/ak5JetpT->at(0));
		 ChFracTightdijets->Fill(sqrt(pow(ak5JetTightAssoTrkspx->at(1),2)+pow(ak5JetTightAssoTrkspy->at(1),2))/ak5JetpT->at(1));
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
		 if(ak5JetJIDresEMF->at(0)>0.01 && ak5JetJIDfHPD->at(0)<0.98 && ak5JetJIDn90Hits->at(0)>1 && ak5JetJIDresEMF->at(1) >0.01 && ak5JetJIDfHPD->at(1)<0.98 && ak5JetJIDn90Hits->at(1)>1){  
		   dijetptall1cleaned->Fill(ak5JetpT->at(0));
		   dijetptall2cleaned->Fill(ak5JetpT->at(1)); 
		   dijetdphicleaned->Fill(dphi);
		   mapalldijetscleaned->Fill(ak5JetEta->at(0),ak5JetPhi->at(0));
		   mapalldijetscleaned->Fill(ak5JetEta->at(1),ak5JetPhi->at(1));
		   //now loop on jets and count how many CLEANED jets with pT>8 are in each event
		   for (int dj = 0; dj<int(ak5JetpT->size()); dj++){
		     if(ak5JetpT->at(dj)>ptMin && ak5JetJIDresEMF->at(dj)>0.01 && ak5JetJIDfHPD->at(dj)<0.98 && ak5JetJIDn90Hits->at(dj)>1){
		       Ncleanedak5indijets++;
		     }
		   }
		 }
	       }
	     }
	   }

	   for(int j = 0; j<int(kt4JetpT->size()); j++){
	     if(kt4JetpT->at(j)>ptMin){
	       Nkt4++;
	       kt4nconst->Fill(kt4JetNConstituents->at(j));
	       bool emfkt4=false;
	       if(fabs(kt4JetEta->at(j))<2.6){
		 if(kt4JetJIDresEMF->at(j)>0.01) emfkt4=true;
	       } else {
		 emfkt4=true;
	       }
	       //fill the histos for cleaned jets
	       if(emfkt4 && kt4JetJIDn90Hits->at(j)>1 && kt4JetJIDfHPD->at(j)<0.98 ){//loose cleaning
		 Ncleanedkt4++;
		 kt4nconstcleaned->Fill(kt4JetNConstituents->at(j));
	       }
	     }
	   }
	   //after looping on jets
	   ak5njets->Fill(Nak5);     
	   ak5ncleanedjets->Fill(Ncleanedak5);
	   kt4njets->Fill(Nkt4);     
	   kt4ncleanedjets->Fill(Ncleanedkt4);
	   ak5njetsindijetscleaned->Fill(Ncleanedak5indijets);
	   ak5njetsindijets->Fill(Nak5indijets);
	   //#################################################
       
       /*       // Set the evaluation of the cuts to false and clear the variable values and filled status
       resetCuts();
       // Set the value of the variableNames listed in the cutFile to their current value
       //       fillVariableWithValue("calometPt", calometPt->at(0));
       // Evaluate cuts (but do not apply them)
       evaluateCuts();
       */
 
       
       ////////////////////// User's code ends here ///////////////////////
	   //		 } // chi2 vertex
	   }// techbits/vertex/tracks
     } // End loop over events
   
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
   ak5nalljets->Write();
   ak5njets->Write();
   ak5nconst->Write();
   ak5ncleanedjets->Write();
   ak5nconstcleaned->Write();
   kt4nalljets->Write();
   kt4njets->Write();
   kt4nconst->Write();
   kt4ncleanedjets->Write();
   kt4nconstcleaned->Write();	 
   phi->Write();
   phicleaned->Write();
   eta->Write(); 
   etacleaned->Write(); 
   ak5NlooseTracks->Write();
   ak5NtightTracks->Write();
   ChFracLoose->Write();
   ChFracTight->Write();
   dijetptall1->Write();
   dijetptall2->Write();
   dijetdphi->Write();
   mapalldijets->Write();
   ak5NlooseTracksdijets->Write();
   ak5NtightTracksdijets->Write();
   ChFracLoosedijets->Write();
   ChFracTightdijets->Write();
   dijetptall1cleaned->Write();
   dijetptall2cleaned->Write();
   dijetdphicleaned->Write();
   mapalldijetscleaned->Write();
   ak5njetsindijets->Write();
   ak5njetsindijetscleaned->Write();
   ak5NlooseTracksdijets->Write();
   ak5NtightTracksdijets->Write();
   resemfdijets->Write();
   fhpddijets->Write();
   frbxdijets->Write();
   n90hitsdijets->Write();

   std::cout << "analysisClass::Loop() ends" <<std::endl;   
     }
