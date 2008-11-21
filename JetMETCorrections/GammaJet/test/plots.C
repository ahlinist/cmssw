#define plots_cxx
#include "plots.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>

void plots::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L plots.C
//      Root > plots t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   TFile *hOutputFile;
   hOutputFile = new TFile("plots.root" , "RECREATE" ) ;

   TH1D* sMajMajphot;
   TH1D* sMinMinphot;
   TH1D* ecalisophot;
   TH1D* ptisophot;
   TH1D* ntrkisophot;
   TH1D* hcalovecalphot;
   TH1D* sMajMajbkg;
   TH1D* sMinMinbkg;
   TH1D* ecalisobkg;
   TH1D* ptisobkg;
   TH1D* ntrkisobkg;
   TH1D* hcalovecalbkg;
   TH1D* sMajMajcutphot;
   TH1D* sMinMincutphot;
   TH1D* ecalisocutphot;
   TH1D* ptisocutphot;
   TH1D* ntrkisocutphot;
   TH1D* hcalovecalcutphot;
   TH1D* sMajMajcutbkg;
   TH1D* sMinMincutbkg;
   TH1D* ecalisocutbkg;
   TH1D* ptisocutbkg;
   TH1D* ntrkisocutbkg;
   TH1D* hcalovecalcutbkg;

   sMajMajphot = new TH1D("sMajMajphot","2^{nd} Maj Moment for #gamma",1000,0.,2.);
   sMinMinphot = new TH1D("sMinMinphot","2^{nd} Min Moment for #gamma",1000,0.,1.4);
   ecalisophot = new TH1D("ecalisophot","ECAL isol R=0.4 for #gamma", 200, -0.15, 1.5 );
   ptisophot = new TH1D("ptisophot","trk isolation R=0.3 for #gamma", 200, 0., 5. );
   ntrkisophot = new TH1D("ntrkisophot","# trk isolation R=0.3 for #gamma", 25, 0., 25. );
   hcalovecalphot = new TH1D("hcalovecalphot","HCAL/ECAL isolation R=0.4 for #gamma", 1000, -0.15, 1.5 );

   sMajMajbkg = new TH1D("sMajMajbkg","2^{nd} Maj Moment for QCD bkg",1000,0.,2.);
   sMinMinbkg = new TH1D("sMinMinbkg","2^{nd} Min Moment for QCD bkg",1000,0.,1.4);
   ecalisobkg = new TH1D("ecalisobkg","ECAL isol R=0.4 for QCD bkg", 200, -0.15, 1.5 );
   ptisobkg = new TH1D("ptisobkg","trk isolation R=0.3 for QCD bkg", 200, 0., 5. );
   ntrkisobkg = new TH1D("ntrkisobkg","# trk isolation R=0.3 for QCD bkg", 25, 0., 25. );
   hcalovecalbkg = new TH1D("hcalovecalbkg","HCAL/ECAL isolation R=0.4 for QCD bkg", 1000, -0.15, 1.5 );

   sMajMajcutphot = new TH1D("sMajMajcutphot","2^{nd} Maj Moment for #gamma",1000,0.,2.);
   sMinMincutphot = new TH1D("sMinMincutphot","2^{nd} Min Moment for #gamma",1000,0.,1.4);
   ecalisocutphot = new TH1D("ecalisocutphot","ECAL isol R=0.4 for #gamma", 200, -0.15, 1.5 );
   ptisocutphot = new TH1D("ptisocutphot","trk isolation R=0.3 for #gamma", 200, 0., 5. );
   ntrkisocutphot = new TH1D("ntrkisocutphot","# trk isolation R=0.3 for #gamma", 25, 0., 25. );
   hcalovecalcutphot = new TH1D("hcalovecalcutphot","HCAL/ECAL isolation R=0.4 for #gamma", 1000, -0.15, 1.5 );

   sMajMajcutbkg = new TH1D("sMajMajcutbkg","2^{nd} Maj Moment for QCD bkg",1000,0.,2.);
   sMinMincutbkg = new TH1D("sMinMincutbkg","2^{nd} Min Moment for QCD bkg",1000,0.,1.4);
   ecalisocutbkg = new TH1D("ecalisocutbkg","ECAL isol R=0.4 for QCD bkg", 200, -0.15, 1.5 );
   ptisocutbkg = new TH1D("ptisocutbkg","trk isolation R=0.3 for QCD bkg", 200, 0., 5. );
   ntrkisocutbkg = new TH1D("ntrkisocutbkg","# trk isolation R=0.3 for QCD bkg", 25, 0., 25. );
   hcalovecalcutbkg = new TH1D("hcalovecalcutbkg","HCAL/ECAL isolation R=0.4 for QCD bkg", 1000, -0.15, 1.5 );

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      if (jentry%10000 == 0) std::cout << "Event " << jentry << std::endl;      
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      bool ntrk = ntrkiso < 5;
      bool pti = ptiso / pt < 0.20;
      bool emf = hcalovecal < 0.1111;
      bool smaj = sMajMaj < 0.60;
      bool smin = sMinMin < 0.50;
      bool ecali = ecaliso < 0.10;
      if(isgamma){
	sMajMajphot->Fill(sMajMaj);
	sMinMinphot->Fill(sMinMin);
	ecalisophot->Fill(ecaliso);
	ptisophot->Fill(ptiso);
	ntrkisophot->Fill(ntrkiso);
	hcalovecalphot->Fill(hcalovecal);       
	if(ntrk && pti && emf && smin && ecali)  sMajMajcutphot->Fill(sMajMaj);	  
	if(ntrk && pti && emf && smaj && ecali)  sMinMincutphot->Fill(sMinMin);	  
	if(ntrk && pti && emf && smaj && smin) 	  ecalisocutphot->Fill(ecaliso);	  
	if(emf && smaj && smin && ecali) 	  ptisocutphot->Fill(ptiso);		  
	if(emf && smaj && smin && ecali) 	  ntrkisocutphot->Fill(ntrkiso);	  
	if(ntrk && pti && smaj && smin && ecali) hcalovecalcutphot->Fill(hcalovecal); 
      } else {
	sMajMajbkg->Fill(sMajMaj);
	sMinMinbkg->Fill(sMinMin);
	ecalisobkg->Fill(ecaliso);
	ptisobkg->Fill(ptiso);
	ntrkisobkg->Fill(ntrkiso);
	hcalovecalbkg->Fill(hcalovecal);	
	if(ntrk && pti && emf && smin && ecali)  sMajMajcutbkg->Fill(sMajMaj);	  
	if(ntrk && pti && emf && smaj && ecali)  sMinMincutbkg->Fill(sMinMin);	  
	if(ntrk && pti && emf && smaj && smin) 	  ecalisocutbkg->Fill(ecaliso);	  
	if(emf && smaj && smin && ecali) 	  ptisocutbkg->Fill(ptiso);		  
	if(emf && smaj && smin && ecali) 	  ntrkisocutbkg->Fill(ntrkiso);	  
	if(ntrk && pti && smaj && smin && ecali) hcalovecalcutbkg->Fill(hcalovecal); 
       }
      // if (Cut(ientry) < 0) continue;
   }
   hOutputFile->Write() ;
   hOutputFile->Close() ;
   hOutputFile->Delete();
}


TChain * getchain(char *thechain) {

  TChain *chain = new TChain("TreeS");
  std::cout << "Chaining ... " << thechain << std::endl;
  char pName[2000];
  char buffer[200];
  sprintf(buffer, "%s", thechain);
  ifstream is(buffer);
  cout << "files " << buffer <<  endl;
  while(is.getline(buffer, 200, '\n')){
    //    if (buffer[0] == '#') continue;
    sscanf(buffer, "%s", pName);
    std::cout << "   Add: " << buffer << std::endl;
    chain->Add(pName);
  }
  is.close();
  return chain;

}

