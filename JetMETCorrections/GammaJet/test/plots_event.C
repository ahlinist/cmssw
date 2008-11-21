#define plots_event_cxx
#include "plots_event.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <iostream>
#include <fstream>


float  plots_event::delta_phi(float phi1, float phi2) {
  
  float dphi = fabs(phi1 - phi2);
  return (dphi <= TMath::Pi())? dphi : TMath::TwoPi() - dphi;
}

float  plots_event::delta_eta(float eta1, float eta2) {
  
  return (eta2 >= 0 ? eta1 - eta2 : eta2 - eta1);
}

void plots_event::Loop()
{
  //   In a ROOT session, you can do:
//      Root > .L plots_event.C
//      Root > plots_event t
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
   hOutputFile = new TFile("plots_event.root" , "RECREATE" ) ;
   TH1D* etajetsig;
   TH1D* deltaphisig;
   TH1D* deltaetasig;
   TH1D* secondjetsig;
   TH1D* etajetbkg;
   TH1D* deltaphibkg;
   TH1D* deltaetabkg;
   TH1D* secondjetbkg;
   TH1D* etajetcutsig;
   TH1D* deltaphicutsig;
   TH1D* deltaetacutsig;
   TH1D* secondjetcutsig;
   TH1D* etajetcutbkg;
   TH1D* deltaphicutbkg;
   TH1D* deltaetacutbkg;
   TH1D* secondjetcutbkg;

   etajetsig =  new TH1D("etajetsig","#eta(jet) after isolation for signal", 30, -5, 5 );
   deltaphisig = new TH1D("deltaphisig","#Delta(#phi)(#gamma,jet) after isolation for signal", 100, 2*TMath::Pi()/3., TMath::Pi() );
   secondjetsig =  new TH1D("secondjetsig","pt of the 2^{nd} jet after isolation for signal", 300, 0., 5. );
   deltaetasig = new TH1D("deltaetasig","#Delta(#eta)(#gamma,jet) after isolation for signal", 100,-6, 6  );
   etajetbkg =  new TH1D("etajetbkg","#eta(jet) after isolation for QCD", 30, -5, 5 );
   deltaphibkg = new TH1D("deltaphibkg","#Delta(#phi)(#gamma,jet) after isolation for QCD", 100, 2*TMath::Pi()/3., TMath::Pi() );
   secondjetbkg =  new TH1D("secondjetbkg","pt of the 2^{nd} jet after isolation for QCD", 300, 0., 5. );
   deltaetabkg = new TH1D("deltaetabkg","#Delta(#eta)(#gamma,jet) after isolation for QCD", 100,-6, 6  );
   etajetcutsig =  new TH1D("etajetcutsig","#eta(jet) after isolation for signal", 30, -5, 5 );
   deltaphicutsig = new TH1D("deltaphicutsig","#Delta(#phi)(#gamma,jet) after isolation for signal", 100, 2*TMath::Pi()/3., TMath::Pi() );
   secondjetcutsig =  new TH1D("secondjetcutsig","pt of the 2^{nd} jet after isolation for signal", 300, 0., 5. );
   deltaetacutsig = new TH1D("deltaetacutsig","#Delta(#eta)(#gamma,jet) after isolation for signal", 100,-6, 6  );
   etajetcutbkg =  new TH1D("etajetcutbkg","#eta(jet) after isolation for QCD", 30, -5, 5 );
   deltaphicutbkg = new TH1D("deltaphicutbkg","#Delta(#phi)(#gamma,jet) after isolation for QCD", 100, 2*TMath::Pi()/3., TMath::Pi() );
   secondjetcutbkg =  new TH1D("secondjetcutbkg","pt of the 2^{nd} jet after isolation for QCD", 300, 0., 5. );
   deltaetacutbkg = new TH1D("deltaetacutbkg","#Delta(#eta)(#gamma,jet) after isolation for QCD", 100,-6, 6  );

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      // Topological cuts
      bool ptcut = (ptphot > 25.);
      bool pt2jetcut = (pt2jet < 0.10*ptphot || pt2jet < 10.);
      bool etacut = (fabs(etaphot) < 1.3);
      bool etajetcut = (fabs(etajet) < 1.3);
      bool deltaphicut = (delta_phi(phijet,phiphot) >  TMath::Pi()-0.2);
      bool deltaetacut = (fabs(delta_eta(etajet,etaphot)) < 1.0);
      bool presele = ptcut && etacut && photonid>=10;

      if(!presele) continue;

      if(isgamma){
	etajetsig->Fill(etajet);
	deltaphisig->Fill(delta_phi(phijet,phiphot));
	if(pt2jet>10) secondjetsig->Fill(pt2jet/ptphot);
        deltaetasig->Fill(delta_eta(etajet,etaphot));	  
	if(deltaphicut && deltaetacut && pt2jetcut) etajetcutsig->Fill(etajet);
	if(deltaetacut && pt2jetcut && etajetcut) deltaphicutsig->Fill(delta_phi(phijet,phiphot));
	if(deltaphicut && deltaetacut && etajetcut) if(pt2jet>10) secondjetcutsig->Fill(pt2jet/ptphot);
        if(deltaphicut && pt2jetcut && etajetcut) deltaetacutsig->Fill(delta_eta(etajet,etaphot));
      } else {
	etajetbkg->Fill(etajet);
	deltaphibkg->Fill(delta_phi(phijet,phiphot));
	if(pt2jet>10) secondjetbkg->Fill(pt2jet/ptphot);
        deltaetabkg->Fill(delta_eta(etajet,etaphot));	
	if(deltaphicut && deltaetacut && pt2jetcut) etajetcutbkg->Fill(etajet);
	if(deltaetacut && pt2jetcut && etajetcut) deltaphicutbkg->Fill(delta_phi(phijet,phiphot));
	if(deltaphicut && deltaetacut && etajetcut) if(pt2jet>10) secondjetcutbkg->Fill(pt2jet/ptphot);
        if(deltaphicut && pt2jetcut && etajetcut) deltaetacutbkg->Fill(delta_eta(etajet,etaphot));
      }
   }
   hOutputFile->Write() ;
   hOutputFile->Close() ;
   hOutputFile->Delete();
}

TChain * getchain(char *thechain) {

  TChain *chain = new TChain("AnaTree");
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

