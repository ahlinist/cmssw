#include "TCanvas.h"
#include <TStyle.h>

void histo(){
  cout<<"usage: "<<endl;
  cout<<".x histo.C(runnumber)"<<endl;
}

void histo(int runnumber){
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetOptFit(1111); 
  gStyle->SetOptStat(111111);
  gStyle->SetPalette(1);
  char output[1024];
  sprintf(output,"/afs/cern.ch/cms/CAF/CMSCOMM/COMM_HCAL/data/HFMon/Run%i",runnumber);  char hist[2048];

  TFile *hfile1 = new TFile("/tmp/efe/combined_prompt_hf.root", "READ");
  demo->cd();
  HF->cd();
  TCanvas *c1 = new TCanvas("c1","");
  c1->cd();
  c1->SetLogy();
  Energy_Occ->SetAxisRange(0,400);
  Energy_Occ->SetFillColor(4);
  Energy_Occ->Draw();
  sprintf(hist,"/afs/cern.ch/cms/CAF/CMSCOMM/COMM_HCAL/data/HFMon/Run%i/Energy_Occ.gif",runnumber);
  c1->Print(hist);  


  TCanvas *c2 = new TCanvas("c2","");
  c2->cd();
  c2->Divide(2,2);
  c2->cd(1);
  Occ_eta_L->SetMarkerStyle(22);
  Occ_eta_L->Draw();  
  c2->cd(2);
  Occ_phi_L->SetMarkerStyle(22);
  Occ_phi_L->Draw();
  c2->cd(3);
  Occ_rphi_L->Draw("COLZ");
  c2->cd(4);
  Occ_map_L->Draw("COLZ");
  sprintf(hist,"/afs/cern.ch/cms/CAF/CMSCOMM/COMM_HCAL/data/HFMon/Run%i/Occ_L.gif",runnumber);
  c2->Print(hist);

  TCanvas *c3 = new TCanvas("c3","");
  c3->cd();
  c3->Divide(2,2);
  c3->cd(1);
  Occ_eta_S->SetMarkerStyle(22);
  Occ_eta_S->Draw();  
  c3->cd(2);
  Occ_phi_S->SetMarkerStyle(22);
  Occ_phi_S->Draw();
  c3->cd(3);
  Occ_rphi_S->Draw();
  c3->cd(4);
  Occ_map_S->Draw();
  sprintf(hist,"/afs/cern.ch/cms/CAF/CMSCOMM/COMM_HCAL/data/HFMon/Run%i/Occ_S.gif",runnumber);
  c3->Print(hist);

  TCanvas *c4 = new TCanvas("c4","");
  c4->cd();
  gPad->SetGrid();
  HFCenterOfEnergy->Draw("COLZ");
  sprintf(hist,"/afs/cern.ch/cms/CAF/CMSCOMM/COMM_HCAL/data/HFMon/Run%i/HFCenterOfEnergy.gif",runnumber);
  c4->Print(hist);

  TCanvas *c5 = new TCanvas("c5","");
  c5->cd();
  c5->Divide(2,2);
  c5->cd(1);
  Etsum_eta_L->SetMarkerStyle(22);
  Etsum_eta_L->Draw();
  c5->cd(2);
  Etsum_phi_L->SetMarkerStyle(22);
  Etsum_phi_L->Draw();
  c5->cd(3);
  Etsum_rphi_L->Draw("COLZ");
  c5->cd(4);
  Etsum_map_L->Draw("COLZ");
  sprintf(hist,"/afs/cern.ch/cms/CAF/CMSCOMM/COMM_HCAL/data/HFMon/Run%i/Etsum_L.gif",runnumber);
  c5->Print(hist);

  TCanvas *c6 = new TCanvas("c6","");
  c6->cd();
  c6->Divide(2,2);
  c6->cd(1);
  Etsum_eta_S->SetMarkerStyle(22);
  Etsum_eta_S->Draw();
  c6->cd(2);
  Etsum_phi_S->SetMarkerStyle(22);
  Etsum_phi_S->Draw();
  c6->cd(3);
  Etsum_rphi_S->Draw("COLZ");
  c6->cd(4);
  Etsum_map_S->Draw("COLZ");
  sprintf(hist,"/afs/cern.ch/cms/CAF/CMSCOMM/COMM_HCAL/data/HFMon/Run%i/Etsum_S.gif",runnumber);
  c6->Print(hist);

  TCanvas *c7 = new TCanvas("c7","");
  c7->cd();
  c7->Divide(2,2);
  c7->cd(1);
  gPad->SetLogy();
  Etsum_ratio_m->SetFillColor(4);
  Etsum_ratio_m->Draw();
  c7->cd(2);
  gPad->SetLogy();
  Etsum_ratio_p->SetFillColor(4);
  Etsum_ratio_p->Draw();
  c7->cd(3);
  Etsum_ratio_map->Draw("COLZ");
  c7->cd(4);
  sprintf(hist,"/afs/cern.ch/cms/CAF/CMSCOMM/COMM_HCAL/data/HFMon/Run%i/Etsum_ratio.gif",runnumber);
  c7->Print(hist);

  /*
  TCanvas *c8 = new TCanvas("c8","");
  TCanvas *c9 = new TCanvas("c9","");
  TCanvas *c10 = new TCanvas("c10","");
  TCanvas *c11 = new TCanvas("c11","");
  TCanvas *c12 = new TCanvas("c12","");
  TCanvas *c13 = new TCanvas("c13","");
  TCanvas *c14 = new TCanvas("c14","");
  TCanvas *c15 = new TCanvas("c15","");
  */
}
