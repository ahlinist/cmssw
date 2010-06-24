#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include"TFile.h"
#include"TMath.h"
#include"TH1.h"
#include"TH2.h"
#include"TTree.h"
#include"TCanvas.h"
#include"TString.h"


void mergeTrees(){

  int nrunsmax=100;

  TFile* inputfile;
  TTree* TMPtree;

  int nrunsTMP=0;
  int runnumberTMP[nrunsmax];
  int NumOfJetsTMP[nrunsmax];
  double SumJetPtTMP[nrunsmax];
  int SelectedEvtsTMP[nrunsmax];
  int NumOfJets_dijetsTMP[nrunsmax];
  double SumJetPt_dijetsTMP[nrunsmax];
  int SelectedEvts_dijetsTMP[nrunsmax];
  //
  int runnumber[nrunsmax];
  int NumOfJets[nrunsmax];
  double SumJetPt[nrunsmax];
  int SelectedEvts[nrunsmax];
  int NumOfJets_dijets[nrunsmax];
  double SumJetPt_dijets[nrunsmax];
  int SelectedEvts_dijets[nrunsmax];

  for(int i=0; i<nrunsmax ; i++) {
    runnumberTMP[i]=0;
    NumOfJetsTMP[i]=0;
    SumJetPtTMP[i]=0.;
    SelectedEvtsTMP[i]=0.;
    NumOfJets_dijetsTMP[i]=0;
    SumJetPt_dijetsTMP[i]=0.;
    SelectedEvts_dijetsTMP[i]=0.;
    runnumber[i]=0;
    NumOfJets[i]=0;
    SumJetPt[i]=0;
    SelectedEvts[i]=0.;
    NumOfJets_dijets[i]=0;
    SumJetPt_dijets[i]=0;
    SelectedEvts_dijets[i]=0.;
  }


      int thatrun=0;
      bool found=false;
      int nruns=0;

  //look for the one with maximum numbers of runs
  TString rootFile;
  ifstream is("listoffiles");
  while(rootFile.ReadLine(is) && (!rootFile.IsNull())){
    inputfile= new TFile(rootFile);
    TMPtree=(TTree*) inputfile->Get("tree");
      TMPtree->SetBranchAddress("nruns",&nrunsTMP);
      TMPtree->SetBranchAddress("runnumber",&runnumberTMP);
      TMPtree->SetBranchAddress("SumJetPt",&SumJetPtTMP);
      TMPtree->SetBranchAddress("NumOfJets",&NumOfJetsTMP);
      TMPtree->SetBranchAddress("SelectedEvts",&SelectedEvtsTMP);
      TMPtree->SetBranchAddress("SumJetPt_dijets",&SumJetPt_dijetsTMP);
      TMPtree->SetBranchAddress("NumOfJets_dijets",&NumOfJets_dijetsTMP);
      TMPtree->SetBranchAddress("SelectedEvts_dijets",&SelectedEvts_dijetsTMP);

      TMPtree->GetEntry(0);

      for(int i=0; i<nrunsTMP; i++){
	//	if(runnumberTMP[i]<=134630) continue; //use this if you are only interested in runs with runnumber>something
	found=false;
	for(int j=0; j<nrunsmax; j++){
	  if(runnumberTMP[i]==runnumber[j]) {
	    thatrun=j;
	    found=true;
	  }
	}
	if(found==false){
	  thatrun=nruns;
	  runnumber[thatrun]=runnumberTMP[i];
	  nruns++;
	}
	NumOfJets[thatrun]+=NumOfJetsTMP[i];
	SumJetPt[thatrun]+=SumJetPtTMP[i];
	SelectedEvts[thatrun]+=SelectedEvtsTMP[i];
	NumOfJets_dijets[thatrun]+=NumOfJets_dijetsTMP[i];
	SumJetPt_dijets[thatrun]+=SumJetPt_dijetsTMP[i];
	SelectedEvts_dijets[thatrun]+=SelectedEvts_dijetsTMP[i];
      }
  }
  

  int runnumforlumi[nrunsmax];
  double luminosity[nrunsmax];
  int count=0;
  ifstream lumis;
  lumis.open("runslumis");
  while(1){
    lumis>>runnumforlumi[count]>>luminosity[count];
    count++;
    if(!lumis.good()) break;
  }

  //look inside the ntuple variables you have filled and for each run
  //fill the variable lumi with corresponding luminosity
  double lumi[nruns]; //will be added to the tree
  //initialize
  for(int in=0; in<nruns; in++){
    lumi[in]=-1.;
  }
  for(int n=0; n<nruns ; n++) {
    for(int l=0; l<count ; l++) {
      if(runnumber[n]==runnumforlumi[l]) {
	lumi[n]=luminosity[l];
      }
    }
  }

  TTree *outtree = new TTree("tree","tree");
  outtree->Branch("nruns",&nruns,"nruns/I");
  outtree->Branch("runnumber",&runnumber,"run[nruns]/I");
  outtree->Branch("lumi",&lumi,"lumi[nruns]/D");
  outtree->Branch("SumJetPt",&SumJetPt,"SumJetPt[nruns]/D");  
  outtree->Branch("NumOfJets",&NumOfJets,"NumOfJets[nruns]/I");
  outtree->Branch("SelectedEvts",&SelectedEvts,"SelectedEvts[nruns]/I");
  outtree->Branch("SumJetPt_dijets",&SumJetPt_dijets,"SumJetPt_dijets[nruns]/D");  
  outtree->Branch("NumOfJets_dijets",&NumOfJets_dijets,"NumOfJets_dijets[nruns]/I");
  outtree->Branch("SelectedEvts_dijets",&SelectedEvts_dijets,"SelectedEvts_dijets[nruns]/I");

  outtree->Fill();
  TFile outfile("MergedOutput.root", "RECREATE");
  outfile.cd();
  outtree->Write();
  outfile.Close();
  
}
