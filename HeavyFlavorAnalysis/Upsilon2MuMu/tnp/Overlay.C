#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/PidTable.hh"

void OverlayALL(){
  DataOverlayBarrelTrigNeg();
  DataOverlayBarrelTrigPos();
  DataOverlayBarrelNoTrigPos();
  DataOverlayBarrelNoTrigNeg();
  DataOverlayEndCapTrigNeg();
  DataOverlayEndCapTrigPos();
  DataOverlayEndCapNoTrigPos();
  DataOverlayEndCapNoTrigNeg();
}

void Plot(){
  
  fPidTablePtTnpPosD = new PidTable("PidTables/DATA/Jpsi/MuID/PtTnpPos-jpsi.dat");
  fPidTablePtTnpPosM = new PidTable("PidTables/MC/Jpsi/MuID/PtTnpPos-jpsi.dat");
  TH2D *hptD = fPidTablePtTnpPosD->get2dHist("hptD", "hptD", 1);
  TH2D *hptM = fPidTablePtTnpPosM->get2dHist("hptM", "hptM", 1);
  TH1D *hPtnpD = new TH1D("hPtnpD", "", hptD->GetNbinsY(), hptD->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPtnpM = new TH1D("hPtnpM", "", hptM->GetNbinsY(), hptM->GetYaxis()->GetXbins()->GetArray());

  double Deff1 = fPidTablePtTnpPosD->effD(3., 0, 0.);  double Derr1 = fPidTablePtTnpPosD->errD(3., 0, 0.); 
  double Deff2 = fPidTablePtTnpPosD->effD(4.5, 0, 0.); double Derr2 = fPidTablePtTnpPosD->errD(4.5, 0, 0.); 
  double Deff3 = fPidTablePtTnpPosD->effD(6., 0, 0.);  double Derr3 = fPidTablePtTnpPosD->errD(6., 0, 0.);
  double Meff1 = fPidTablePtTnpPosM->effD(3., 0, 0.);  double Merr1 = fPidTablePtTnpPosM->errD(3., 0, 0.); 
  double Meff2 = fPidTablePtTnpPosM->effD(4.5, 0, 0.); double Merr2 = fPidTablePtTnpPosM->errD(4.5, 0, 0.); 
  double Meff3 = fPidTablePtTnpPosM->effD(6., 0, 0.);  double Merr3 = fPidTablePtTnpPosM->errD(6., 0, 0.);

  hPtnpD->SetBinContent(1,Deff1);  hPtnpD->SetBinError(1,Derr1);
  hPtnpD->SetBinContent(2,Deff2);  hPtnpD->SetBinError(2,Derr2);
  hPtnpD->SetBinContent(3,Deff3);  hPtnpD->SetBinError(3,Derr3);
  hPtnpM->SetBinContent(1,Meff1);  hPtnpM->SetBinError(1,Merr1);
  hPtnpM->SetBinContent(2,Meff2);  hPtnpM->SetBinError(2,Merr2);
  hPtnpM->SetBinContent(3,Meff3);  hPtnpM->SetBinError(3,Merr3);  
  
  cout <<"DATA"<<endl; 
  cout <<" eff1  =  "<<Deff1<<", eff2 =  "<<Deff2<<", eff3 =  "<<Deff3<< endl;
  cout <<" err1  =  "<<Derr1<<", err2 =  "<<Derr2<<", err3 =  "<<Derr3<< endl;
  cout <<"MC"<<endl; 
  cout <<" eff1  =  "<<Meff1<<", eff2 =  "<<Meff2<<", eff3 =  "<<Meff3<< endl;
  cout <<" err1  =  "<<Merr1<<", err2 =  "<<Merr2<<", err3 =  "<<Merr3<< endl;  
  
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(00000000000);
  hPtnpD->SetMarkerStyle(22);
  hPtnpD->SetMarkerColor(2);
  hPtnpD->SetLineColor(2);
  hPtnpM->SetMarkerStyle(23);
  hPtnpM->SetMarkerColor(3);
  hPtnpM->SetLineColor(3);
  hPtnpD->SetMinimum(0.3);
  hPtnpD->Draw("p");
  hPtnpM->Draw("samep");
}

void DataOverlayBarrelTrigNeg() {

  fPidTablePtTnpPos = new PidTable("PidTables/DATA/Jpsi/MuID/PtTnpNeg-jpsi.dat");
  fPidTablePtMmbPos = new PidTable("PidTables/DATA/Jpsi/MuID/PtMmbNeg-jpsi.dat");
  fPidTablePtICHEP  = new PidTable("PidTables/DATA/Jpsi/MuID/PtICHEP-jpsi.dat");
  fPidTablePtTnpPosTrS = new PidTable("PidTables/DATA/Jpsi/MuID/TrSel/PtTnpNeg-jpsi.dat");
  fPidTablePtMmbPosTrS = new PidTable("PidTables/DATA/Jpsi/MuID/TrSel/PtMmbNeg-jpsi.dat");  
  
  TH2D *hpt = fPidTablePtTnpPos->get2dHist("hpt", "hpt", 1);  
  TH1D *hPtnp = new TH1D("hPtnp", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPMmb = new TH1D("hPMmb", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPIch = new TH1D("hPIch", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPtnpts = new TH1D("hPtnpTS", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPMmbts = new TH1D("hPMmbTS", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  
  double teff1 = fPidTablePtTnpPos->effD(3.5, 0, 0.); double terr1 = fPidTablePtTnpPos->errD(3.5, 0, 0.); 
  double teff2 = fPidTablePtTnpPos->effD(5., 0, 0.);  double terr2 = fPidTablePtTnpPos->errD(5., 0, 0.); 
  double teff3 = fPidTablePtTnpPos->effD(7., 0, 0.);  double terr3 = fPidTablePtTnpPos->errD(7., 0, 0.);

  double meff1 = fPidTablePtMmbPos->effD(3.5, 0, 0.); double merr1 = fPidTablePtMmbPos->errD(3.5, 0, 0.); 
  double meff2 = fPidTablePtMmbPos->effD(5., 0, 0.);  double merr2 = fPidTablePtMmbPos->errD(5., 0, 0.); 
  double meff3 = fPidTablePtMmbPos->effD(7., 0, 0.);  double merr3 = fPidTablePtMmbPos->errD(7., 0, 0.);

  double ieff1 = fPidTablePtICHEP->effD(3.5, 0, 0.);  double ierr1 = fPidTablePtICHEP->errD(3.5, 0, 0.); 
  double ieff2 = fPidTablePtICHEP->effD(5., 0, 0.);   double ierr2 = fPidTablePtICHEP->errD(5., 0, 0.); 
  double ieff3 = fPidTablePtICHEP->effD(7., 0, 0.);   double ierr3 = fPidTablePtICHEP->errD(7., 0, 0.);
  
  double tseff1 = fPidTablePtTnpPosTrS->effD(3.5, 0, 0.); double tserr1 = fPidTablePtTnpPosTrS->errD(3.5, 0, 0.); 
  double tseff2 = fPidTablePtTnpPosTrS->effD(5., 0, 0.);  double tserr2 = fPidTablePtTnpPosTrS->errD(5., 0, 0.); 
  double tseff3 = fPidTablePtTnpPosTrS->effD(7., 0, 0.);  double tserr3 = fPidTablePtTnpPosTrS->errD(7., 0, 0.);

  double mtseff1 = fPidTablePtMmbPosTrS->effD(3.5, 0, 0.); double mtserr1 = fPidTablePtMmbPosTrS->errD(3.5, 0, 0.); 
  double mtseff2 = fPidTablePtMmbPosTrS->effD(5., 0, 0.);  double mtserr2 = fPidTablePtMmbPosTrS->errD(5., 0, 0.); 
  double mtseff3 = fPidTablePtMmbPosTrS->effD(7., 0, 0.);  double mtserr3 = fPidTablePtMmbPosTrS->errD(7., 0, 0.);
  

  hPtnp->SetBinContent(1,teff1);  hPtnp->SetBinError(1,terr1);
  hPtnp->SetBinContent(2,teff2);  hPtnp->SetBinError(2,terr2);
  hPtnp->SetBinContent(3,teff3);  hPtnp->SetBinError(3,terr3);
  
  hPMmb->SetBinContent(1,meff1);  hPMmb->SetBinError(1,merr1);
  hPMmb->SetBinContent(2,meff2);  hPMmb->SetBinError(2,merr2);
  hPMmb->SetBinContent(3,meff3);  hPMmb->SetBinError(3,merr3);
  
  hPIch->SetBinContent(1,ieff1);  hPIch->SetBinError(1,ierr1);
  hPIch->SetBinContent(2,ieff2);  hPIch->SetBinError(2,ierr2);
  hPIch->SetBinContent(3,ieff3);  hPIch->SetBinError(3,ierr3); 

  hPtnpts->SetBinContent(1,tseff1);  hPtnpts->SetBinError(1,tserr1);
  hPtnpts->SetBinContent(2,tseff2);  hPtnpts->SetBinError(2,tserr2);
  hPtnpts->SetBinContent(3,tseff3);  hPtnpts->SetBinError(3,tserr3);
  
  hPMmbts->SetBinContent(1,mtseff1);  hPMmbts->SetBinError(1,mtserr1);
  hPMmbts->SetBinContent(2,mtseff2);  hPMmbts->SetBinError(2,mtserr2);
  hPMmbts->SetBinContent(3,mtseff3);  hPMmbts->SetBinError(3,mtserr3);
  
  cout <<"TNP"<<endl; 
  cout <<" eff1  =  "<<teff1<<", eff2 =  "<<teff2<<", eff3 =  "<<teff3<< endl;
  cout <<" err1  =  "<<terr1<<", err2 =  "<<terr2<<", err3 =  "<<terr3<< endl;
  cout <<"Mmb"<<endl; 
  cout <<" eff1  =  "<<meff1<<", eff2 =  "<<meff2<<", eff3 =  "<<meff3<< endl;
  cout <<" err1  =  "<<merr1<<", err2 =  "<<merr2<<", err3 =  "<<merr3<< endl;  
  cout <<"ICHEP"<<endl; 
  cout <<" eff1  =  "<<ieff1<<", eff2 =  "<<ieff2<<", eff3 =  "<<ieff3<< endl;
  cout <<" err1  =  "<<ierr1<<", err2 =  "<<ierr2<<", err3 =  "<<ierr3<< endl;  
  cout <<"TNP TS"<<endl; 
  cout <<" eff1  =  "<<tseff1<<", eff2 =  "<<tseff2<<", eff3 =  "<<tseff3<< endl;
  cout <<" err1  =  "<<tserr1<<", err2 =  "<<tserr2<<", err3 =  "<<tserr3<< endl;
  cout <<"Mmb TS"<<endl; 
  cout <<" eff1  =  "<<mtseff1<<", eff2 =  "<<mtseff2<<", eff3 =  "<<mtseff3<< endl;
  cout <<" err1  =  "<<mtserr1<<", err2 =  "<<mtserr2<<", err3 =  "<<mtserr3<< endl;
  
  
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(00000000000);
  hPtnp->SetMarkerStyle(22); hPtnpts->SetMarkerStyle(21);
  hPtnp->SetMarkerColor(2);  hPtnpts->SetMarkerColor(3);
  hPtnp->SetLineColor(2);    hPtnpts->SetLineColor(3);
  
  hPIch->SetMarkerStyle(24);
  hPIch->SetMarkerColor(4);
  hPIch->SetLineColor(4);  
  
  hPMmb->SetMarkerStyle(26); hPMmbts->SetMarkerStyle(25);
  hPMmb->SetMarkerColor(2);  hPMmbts->SetMarkerColor(3);
  hPMmb->SetLineColor(2);    hPMmbts->SetLineColor(3); 
  
  TCanvas *c0 = new TCanvas("c0", "c0");
  hPtnp->SetMinimum(0.3);
  hPtnp->SetMaximum(1.0);
  hPtnp->SetTitle("Barrel");
  hPtnp->GetXaxis()->SetTitle("P_{T}");
  hPtnp->GetYaxis()->SetTitle("Efficiency");
  hPtnp->Draw("p");
  hPMmb->Draw("samep");
  hPIch->Draw("samep");
  hPtnpts->Draw("samep");
  hPMmbts->Draw("samep");
  
  legg = new TLegend(0.4,0.2,0.8,0.6);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.08); legg->SetTextFont(132); 
  legg->SetHeader("negative muons");
  legge = legg->AddEntry(hPtnp,  " tag and probe 1","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPMmb,  " tag and probe 2","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPtnpts,  "TrSel tag and probe 1","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPMmbts,  "TrSel tag and probe 2","p"); legge->SetTextColor(kBlack);  
  legge = legg->AddEntry(hPIch,  " ICHEP","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
}

void DataOverlayBarrelTrigPos() {

  fPidTablePtTnpPos = new PidTable("PidTables/DATA/Jpsi/MuID/PtTnpPos-jpsi.dat");
  fPidTablePtMmbPos = new PidTable("PidTables/DATA/Jpsi/MuID/PtMmbPos-jpsi.dat");
  fPidTablePtICHEP  = new PidTable("PidTables/DATA/Jpsi/MuID/PtICHEP-jpsi.dat");
  fPidTablePtTnpPosTrS = new PidTable("PidTables/DATA/Jpsi/MuID/TrSel/PtTnpPos-jpsi.dat");
  fPidTablePtMmbPosTrS = new PidTable("PidTables/DATA/Jpsi/MuID/TrSel/PtMmbPos-jpsi.dat");  
  
  TH2D *hpt = fPidTablePtTnpPos->get2dHist("hpt", "hpt", 1);  
  TH1D *hPtnp = new TH1D("hPtnp", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPMmb = new TH1D("hPMmb", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPIch = new TH1D("hPIch", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPtnpts = new TH1D("hPtnpTS", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPMmbts = new TH1D("hPMmbTS", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  
  double teff1 = fPidTablePtTnpPos->effD(3.5, 0, 0.); double terr1 = fPidTablePtTnpPos->errD(3.5, 0, 0.); 
  double teff2 = fPidTablePtTnpPos->effD(5., 0, 0.);  double terr2 = fPidTablePtTnpPos->errD(5., 0, 0.); 
  double teff3 = fPidTablePtTnpPos->effD(7., 0, 0.);  double terr3 = fPidTablePtTnpPos->errD(7., 0, 0.);

  double meff1 = fPidTablePtMmbPos->effD(3.5, 0, 0.); double merr1 = fPidTablePtMmbPos->errD(3.5, 0, 0.); 
  double meff2 = fPidTablePtMmbPos->effD(5., 0, 0.);  double merr2 = fPidTablePtMmbPos->errD(5., 0, 0.); 
  double meff3 = fPidTablePtMmbPos->effD(7., 0, 0.);  double merr3 = fPidTablePtMmbPos->errD(7., 0, 0.);

  double ieff1 = fPidTablePtICHEP->effD(3.5, 0, 0.);  double ierr1 = fPidTablePtICHEP->errD(3.5, 0, 0.); 
  double ieff2 = fPidTablePtICHEP->effD(5., 0, 0.);   double ierr2 = fPidTablePtICHEP->errD(5., 0, 0.); 
  double ieff3 = fPidTablePtICHEP->effD(7., 0, 0.);   double ierr3 = fPidTablePtICHEP->errD(7., 0, 0.);
  
  double tseff1 = fPidTablePtTnpPosTrS->effD(3.5, 0, 0.); double tserr1 = fPidTablePtTnpPosTrS->errD(3.5, 0, 0.); 
  double tseff2 = fPidTablePtTnpPosTrS->effD(5., 0, 0.);  double tserr2 = fPidTablePtTnpPosTrS->errD(5., 0, 0.); 
  double tseff3 = fPidTablePtTnpPosTrS->effD(7., 0, 0.);  double tserr3 = fPidTablePtTnpPosTrS->errD(7., 0, 0.);

  double mtseff1 = fPidTablePtMmbPosTrS->effD(3.5, 0, 0.); double mtserr1 = fPidTablePtMmbPosTrS->errD(3.5, 0, 0.); 
  double mtseff2 = fPidTablePtMmbPosTrS->effD(5., 0, 0.);  double mtserr2 = fPidTablePtMmbPosTrS->errD(5., 0, 0.); 
  double mtseff3 = fPidTablePtMmbPosTrS->effD(7., 0, 0.);  double mtserr3 = fPidTablePtMmbPosTrS->errD(7., 0, 0.);
  

  hPtnp->SetBinContent(1,teff1);  hPtnp->SetBinError(1,terr1);
  hPtnp->SetBinContent(2,teff2);  hPtnp->SetBinError(2,terr2);
  hPtnp->SetBinContent(3,teff3);  hPtnp->SetBinError(3,terr3);
  
  hPMmb->SetBinContent(1,meff1);  hPMmb->SetBinError(1,merr1);
  hPMmb->SetBinContent(2,meff2);  hPMmb->SetBinError(2,merr2);
  hPMmb->SetBinContent(3,meff3);  hPMmb->SetBinError(3,merr3);
  
  hPIch->SetBinContent(1,ieff1);  hPIch->SetBinError(1,ierr1);
  hPIch->SetBinContent(2,ieff2);  hPIch->SetBinError(2,ierr2);
  hPIch->SetBinContent(3,ieff3);  hPIch->SetBinError(3,ierr3); 

  hPtnpts->SetBinContent(1,tseff1);  hPtnpts->SetBinError(1,tserr1);
  hPtnpts->SetBinContent(2,tseff2);  hPtnpts->SetBinError(2,tserr2);
  hPtnpts->SetBinContent(3,tseff3);  hPtnpts->SetBinError(3,tserr3);
  
  hPMmbts->SetBinContent(1,mtseff1);  hPMmbts->SetBinError(1,mtserr1);
  hPMmbts->SetBinContent(2,mtseff2);  hPMmbts->SetBinError(2,mtserr2);
  hPMmbts->SetBinContent(3,mtseff3);  hPMmbts->SetBinError(3,mtserr3);
  
  cout <<"TNP"<<endl; 
  cout <<" eff1  =  "<<teff1<<", eff2 =  "<<teff2<<", eff3 =  "<<teff3<< endl;
  cout <<" err1  =  "<<terr1<<", err2 =  "<<terr2<<", err3 =  "<<terr3<< endl;
  cout <<"Mmb"<<endl; 
  cout <<" eff1  =  "<<meff1<<", eff2 =  "<<meff2<<", eff3 =  "<<meff3<< endl;
  cout <<" err1  =  "<<merr1<<", err2 =  "<<merr2<<", err3 =  "<<merr3<< endl;  
  cout <<"ICHEP"<<endl; 
  cout <<" eff1  =  "<<ieff1<<", eff2 =  "<<ieff2<<", eff3 =  "<<ieff3<< endl;
  cout <<" err1  =  "<<ierr1<<", err2 =  "<<ierr2<<", err3 =  "<<ierr3<< endl;  
  cout <<"TNP TS"<<endl; 
  cout <<" eff1  =  "<<tseff1<<", eff2 =  "<<tseff2<<", eff3 =  "<<tseff3<< endl;
  cout <<" err1  =  "<<tserr1<<", err2 =  "<<tserr2<<", err3 =  "<<tserr3<< endl;
  cout <<"Mmb TS"<<endl; 
  cout <<" eff1  =  "<<mtseff1<<", eff2 =  "<<mtseff2<<", eff3 =  "<<mtseff3<< endl;
  cout <<" err1  =  "<<mtserr1<<", err2 =  "<<mtserr2<<", err3 =  "<<mtserr3<< endl;
  
  
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(00000000000);
  hPtnp->SetMarkerStyle(22); hPtnpts->SetMarkerStyle(21);
  hPtnp->SetMarkerColor(2);  hPtnpts->SetMarkerColor(3);
  hPtnp->SetLineColor(2);    hPtnpts->SetLineColor(3);
  
  hPIch->SetMarkerStyle(24);
  hPIch->SetMarkerColor(4);
  hPIch->SetLineColor(4);  
  
  hPMmb->SetMarkerStyle(26); hPMmbts->SetMarkerStyle(25);
  hPMmb->SetMarkerColor(2);  hPMmbts->SetMarkerColor(3);
  hPMmb->SetLineColor(2);    hPMmbts->SetLineColor(3); 
  
  TCanvas *c1 = new TCanvas("c1", "c1");
  hPtnp->SetMinimum(0.3);
  hPtnp->SetMaximum(1.0);
  hPtnp->SetTitle("Barrel");
  hPtnp->GetXaxis()->SetTitle("P_{T}");
  hPtnp->GetYaxis()->SetTitle("Efficiency");
  hPtnp->Draw("p");
  hPMmb->Draw("samep");
  hPIch->Draw("samep");
  hPtnpts->Draw("samep");
  hPMmbts->Draw("samep");
  
  legg = new TLegend(0.4,0.2,0.8,0.6);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.08); legg->SetTextFont(132); 
  legg->SetHeader("positive muons");
  legge = legg->AddEntry(hPtnp,  " tag and probe 1","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPMmb,  " tag and probe 2","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPtnpts,  "TrSel tag and probe 1","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPMmbts,  "TrSel tag and probe 2","p"); legge->SetTextColor(kBlack);  
  legge = legg->AddEntry(hPIch,  " ICHEP","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
}



void DataOverlayBarrelNoTrigNeg() {

  fPidTablePtTnpPos = new PidTable("PidTables/DATA/Jpsi/MuID/NoTrig/PtTnpNeg-jpsi.dat");
  fPidTablePtMmbPos = new PidTable("PidTables/DATA/Jpsi/MuID/NoTrig/PtMmbNeg-jpsi.dat");
  fPidTablePtICHEP  = new PidTable("PidTables/DATA/Jpsi/MuID/PtICHEP-jpsi.dat");
  fPidTablePtTnpPosTrS = new PidTable("PidTables/DATA/Jpsi/MuID/TrSel/PtTnpNeg-jpsi.dat");
  fPidTablePtMmbPosTrS = new PidTable("PidTables/DATA/Jpsi/MuID/TrSel/PtMmbNeg-jpsi.dat");  
  
  TH2D *hpt = fPidTablePtTnpPos->get2dHist("hpt", "hpt", 1);  
  TH1D *hPtnp = new TH1D("hPtnp", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPMmb = new TH1D("hPMmb", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPIch = new TH1D("hPIch", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPtnpts = new TH1D("hPtnpTS", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPMmbts = new TH1D("hPMmbTS", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  
  double teff1 = fPidTablePtTnpPos->effD(3.5, 0, 0.); double terr1 = fPidTablePtTnpPos->errD(3.5, 0, 0.); 
  double teff2 = fPidTablePtTnpPos->effD(5., 0, 0.);  double terr2 = fPidTablePtTnpPos->errD(5., 0, 0.); 
  double teff3 = fPidTablePtTnpPos->effD(7., 0, 0.);  double terr3 = fPidTablePtTnpPos->errD(7., 0, 0.);

  double meff1 = fPidTablePtMmbPos->effD(3.5, 0, 0.); double merr1 = fPidTablePtMmbPos->errD(3.5, 0, 0.); 
  double meff2 = fPidTablePtMmbPos->effD(5., 0, 0.);  double merr2 = fPidTablePtMmbPos->errD(5., 0, 0.); 
  double meff3 = fPidTablePtMmbPos->effD(7., 0, 0.);  double merr3 = fPidTablePtMmbPos->errD(7., 0, 0.);

  double ieff1 = fPidTablePtICHEP->effD(3.5, 0, 0.);  double ierr1 = fPidTablePtICHEP->errD(3.5, 0, 0.); 
  double ieff2 = fPidTablePtICHEP->effD(5., 0, 0.);   double ierr2 = fPidTablePtICHEP->errD(5., 0, 0.); 
  double ieff3 = fPidTablePtICHEP->effD(7., 0, 0.);   double ierr3 = fPidTablePtICHEP->errD(7., 0, 0.);
  
  double tseff1 = fPidTablePtTnpPosTrS->effD(3.5, 0, 0.); double tserr1 = fPidTablePtTnpPosTrS->errD(3.5, 0, 0.); 
  double tseff2 = fPidTablePtTnpPosTrS->effD(5., 0, 0.);  double tserr2 = fPidTablePtTnpPosTrS->errD(5., 0, 0.); 
  double tseff3 = fPidTablePtTnpPosTrS->effD(7., 0, 0.);  double tserr3 = fPidTablePtTnpPosTrS->errD(7., 0, 0.);

  double mtseff1 = fPidTablePtMmbPosTrS->effD(3.5, 0, 0.); double mtserr1 = fPidTablePtMmbPosTrS->errD(3.5, 0, 0.); 
  double mtseff2 = fPidTablePtMmbPosTrS->effD(5., 0, 0.);  double mtserr2 = fPidTablePtMmbPosTrS->errD(5., 0, 0.); 
  double mtseff3 = fPidTablePtMmbPosTrS->effD(7., 0, 0.);  double mtserr3 = fPidTablePtMmbPosTrS->errD(7., 0, 0.);
  

  hPtnp->SetBinContent(1,teff1);  hPtnp->SetBinError(1,terr1);
  hPtnp->SetBinContent(2,teff2);  hPtnp->SetBinError(2,terr2);
  hPtnp->SetBinContent(3,teff3);  hPtnp->SetBinError(3,terr3);
  
  hPMmb->SetBinContent(1,meff1);  hPMmb->SetBinError(1,merr1);
  hPMmb->SetBinContent(2,meff2);  hPMmb->SetBinError(2,merr2);
  hPMmb->SetBinContent(3,meff3);  hPMmb->SetBinError(3,merr3);
  
  hPIch->SetBinContent(1,ieff1);  hPIch->SetBinError(1,ierr1);
  hPIch->SetBinContent(2,ieff2);  hPIch->SetBinError(2,ierr2);
  hPIch->SetBinContent(3,ieff3);  hPIch->SetBinError(3,ierr3); 

  hPtnpts->SetBinContent(1,tseff1);  hPtnpts->SetBinError(1,tserr1);
  hPtnpts->SetBinContent(2,tseff2);  hPtnpts->SetBinError(2,tserr2);
  hPtnpts->SetBinContent(3,tseff3);  hPtnpts->SetBinError(3,tserr3);
  
  hPMmbts->SetBinContent(1,mtseff1);  hPMmbts->SetBinError(1,mtserr1);
  hPMmbts->SetBinContent(2,mtseff2);  hPMmbts->SetBinError(2,mtserr2);
  hPMmbts->SetBinContent(3,mtseff3);  hPMmbts->SetBinError(3,mtserr3);
  
  cout <<"TNP"<<endl; 
  cout <<" eff1  =  "<<teff1<<", eff2 =  "<<teff2<<", eff3 =  "<<teff3<< endl;
  cout <<" err1  =  "<<terr1<<", err2 =  "<<terr2<<", err3 =  "<<terr3<< endl;
  cout <<"Mmb"<<endl; 
  cout <<" eff1  =  "<<meff1<<", eff2 =  "<<meff2<<", eff3 =  "<<meff3<< endl;
  cout <<" err1  =  "<<merr1<<", err2 =  "<<merr2<<", err3 =  "<<merr3<< endl;  
  cout <<"ICHEP"<<endl; 
  cout <<" eff1  =  "<<ieff1<<", eff2 =  "<<ieff2<<", eff3 =  "<<ieff3<< endl;
  cout <<" err1  =  "<<ierr1<<", err2 =  "<<ierr2<<", err3 =  "<<ierr3<< endl;  
  cout <<"TNP TS"<<endl; 
  cout <<" eff1  =  "<<tseff1<<", eff2 =  "<<tseff2<<", eff3 =  "<<tseff3<< endl;
  cout <<" err1  =  "<<tserr1<<", err2 =  "<<tserr2<<", err3 =  "<<tserr3<< endl;
  cout <<"Mmb TS"<<endl; 
  cout <<" eff1  =  "<<mtseff1<<", eff2 =  "<<mtseff2<<", eff3 =  "<<mtseff3<< endl;
  cout <<" err1  =  "<<mtserr1<<", err2 =  "<<mtserr2<<", err3 =  "<<mtserr3<< endl;
  
  
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(00000000000);
  hPtnp->SetMarkerStyle(22); hPtnpts->SetMarkerStyle(21);
  hPtnp->SetMarkerColor(2);  hPtnpts->SetMarkerColor(3);
  hPtnp->SetLineColor(2);    hPtnpts->SetLineColor(3);
  
  hPIch->SetMarkerStyle(24);
  hPIch->SetMarkerColor(4);
  hPIch->SetLineColor(4);  
  
  hPMmb->SetMarkerStyle(26); hPMmbts->SetMarkerStyle(25);
  hPMmb->SetMarkerColor(2);  hPMmbts->SetMarkerColor(3);
  hPMmb->SetLineColor(2);    hPMmbts->SetLineColor(3); 
  
  TCanvas *c2 = new TCanvas("c2", "c2");
  hPtnp->SetMinimum(0.3);
  hPtnp->SetMaximum(1.0);
  hPtnp->SetTitle("Barrel");
  hPtnp->GetXaxis()->SetTitle("P_{T}");
  hPtnp->GetYaxis()->SetTitle("Efficiency");
  hPtnp->Draw("p");
  hPMmb->Draw("samep");
  hPIch->Draw("samep");
  hPtnpts->Draw("samep");
  hPMmbts->Draw("samep");
  
  legg = new TLegend(0.4,0.2,0.8,0.6);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.08); legg->SetTextFont(132); 
  legg->SetHeader("negative muons");
  legge = legg->AddEntry(hPtnp,  "No Trig TrSel tag and probe 1","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPMmb,  "No Trig TrSel tag and probe 2","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPtnpts,  "TrSel tag and probe 1","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPMmbts,  "TrSel tag and probe 2","p"); legge->SetTextColor(kBlack);  
  legge = legg->AddEntry(hPIch,  " ICHEP","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
}

void DataOverlayBarrelNoTrigPos() {

  fPidTablePtTnpPos = new PidTable("PidTables/DATA/Jpsi/MuID/NoTrig/PtTnpPos-jpsi.dat");
  fPidTablePtMmbPos = new PidTable("PidTables/DATA/Jpsi/MuID/NoTrig/PtMmbPos-jpsi.dat");
  fPidTablePtICHEP  = new PidTable("PidTables/DATA/Jpsi/MuID/PtICHEP-jpsi.dat");
  fPidTablePtTnpPosTrS = new PidTable("PidTables/DATA/Jpsi/MuID/TrSel/PtTnpPos-jpsi.dat");
  fPidTablePtMmbPosTrS = new PidTable("PidTables/DATA/Jpsi/MuID/TrSel/PtMmbPos-jpsi.dat");  
  
  TH2D *hpt = fPidTablePtTnpPos->get2dHist("hpt", "hpt", 1);  
  TH1D *hPtnp = new TH1D("hPtnp", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPMmb = new TH1D("hPMmb", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPIch = new TH1D("hPIch", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPtnpts = new TH1D("hPtnpTS", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPMmbts = new TH1D("hPMmbTS", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  
  double teff1 = fPidTablePtTnpPos->effD(3.5, 0, 0.); double terr1 = fPidTablePtTnpPos->errD(3.5, 0, 0.); 
  double teff2 = fPidTablePtTnpPos->effD(5., 0, 0.);  double terr2 = fPidTablePtTnpPos->errD(5., 0, 0.); 
  double teff3 = fPidTablePtTnpPos->effD(7., 0, 0.);  double terr3 = fPidTablePtTnpPos->errD(7., 0, 0.);

  double meff1 = fPidTablePtMmbPos->effD(3.5, 0, 0.); double merr1 = fPidTablePtMmbPos->errD(3.5, 0, 0.); 
  double meff2 = fPidTablePtMmbPos->effD(5., 0, 0.);  double merr2 = fPidTablePtMmbPos->errD(5., 0, 0.); 
  double meff3 = fPidTablePtMmbPos->effD(7., 0, 0.);  double merr3 = fPidTablePtMmbPos->errD(7., 0, 0.);

  double ieff1 = fPidTablePtICHEP->effD(3.5, 0, 0.);  double ierr1 = fPidTablePtICHEP->errD(3.5, 0, 0.); 
  double ieff2 = fPidTablePtICHEP->effD(5., 0, 0.);   double ierr2 = fPidTablePtICHEP->errD(5., 0, 0.); 
  double ieff3 = fPidTablePtICHEP->effD(7., 0, 0.);   double ierr3 = fPidTablePtICHEP->errD(7., 0, 0.);
  
  double tseff1 = fPidTablePtTnpPosTrS->effD(3.5, 0, 0.); double tserr1 = fPidTablePtTnpPosTrS->errD(3.5, 0, 0.); 
  double tseff2 = fPidTablePtTnpPosTrS->effD(5., 0, 0.);  double tserr2 = fPidTablePtTnpPosTrS->errD(5., 0, 0.); 
  double tseff3 = fPidTablePtTnpPosTrS->effD(7., 0, 0.);  double tserr3 = fPidTablePtTnpPosTrS->errD(7., 0, 0.);

  double mtseff1 = fPidTablePtMmbPosTrS->effD(3.5, 0, 0.); double mtserr1 = fPidTablePtMmbPosTrS->errD(3.5, 0, 0.); 
  double mtseff2 = fPidTablePtMmbPosTrS->effD(5., 0, 0.);  double mtserr2 = fPidTablePtMmbPosTrS->errD(5., 0, 0.); 
  double mtseff3 = fPidTablePtMmbPosTrS->effD(7., 0, 0.);  double mtserr3 = fPidTablePtMmbPosTrS->errD(7., 0, 0.);
  

  hPtnp->SetBinContent(1,teff1);  hPtnp->SetBinError(1,terr1);
  hPtnp->SetBinContent(2,teff2);  hPtnp->SetBinError(2,terr2);
  hPtnp->SetBinContent(3,teff3);  hPtnp->SetBinError(3,terr3);
  
  hPMmb->SetBinContent(1,meff1);  hPMmb->SetBinError(1,merr1);
  hPMmb->SetBinContent(2,meff2);  hPMmb->SetBinError(2,merr2);
  hPMmb->SetBinContent(3,meff3);  hPMmb->SetBinError(3,merr3);
  
  hPIch->SetBinContent(1,ieff1);  hPIch->SetBinError(1,ierr1);
  hPIch->SetBinContent(2,ieff2);  hPIch->SetBinError(2,ierr2);
  hPIch->SetBinContent(3,ieff3);  hPIch->SetBinError(3,ierr3); 

  hPtnpts->SetBinContent(1,tseff1);  hPtnpts->SetBinError(1,tserr1);
  hPtnpts->SetBinContent(2,tseff2);  hPtnpts->SetBinError(2,tserr2);
  hPtnpts->SetBinContent(3,tseff3);  hPtnpts->SetBinError(3,tserr3);
  
  hPMmbts->SetBinContent(1,mtseff1);  hPMmbts->SetBinError(1,mtserr1);
  hPMmbts->SetBinContent(2,mtseff2);  hPMmbts->SetBinError(2,mtserr2);
  hPMmbts->SetBinContent(3,mtseff3);  hPMmbts->SetBinError(3,mtserr3);
  
  cout <<"TNP"<<endl; 
  cout <<" eff1  =  "<<teff1<<", eff2 =  "<<teff2<<", eff3 =  "<<teff3<< endl;
  cout <<" err1  =  "<<terr1<<", err2 =  "<<terr2<<", err3 =  "<<terr3<< endl;
  cout <<"Mmb"<<endl; 
  cout <<" eff1  =  "<<meff1<<", eff2 =  "<<meff2<<", eff3 =  "<<meff3<< endl;
  cout <<" err1  =  "<<merr1<<", err2 =  "<<merr2<<", err3 =  "<<merr3<< endl;  
  cout <<"ICHEP"<<endl; 
  cout <<" eff1  =  "<<ieff1<<", eff2 =  "<<ieff2<<", eff3 =  "<<ieff3<< endl;
  cout <<" err1  =  "<<ierr1<<", err2 =  "<<ierr2<<", err3 =  "<<ierr3<< endl;  
  cout <<"TNP TS"<<endl; 
  cout <<" eff1  =  "<<tseff1<<", eff2 =  "<<tseff2<<", eff3 =  "<<tseff3<< endl;
  cout <<" err1  =  "<<tserr1<<", err2 =  "<<tserr2<<", err3 =  "<<tserr3<< endl;
  cout <<"Mmb TS"<<endl; 
  cout <<" eff1  =  "<<mtseff1<<", eff2 =  "<<mtseff2<<", eff3 =  "<<mtseff3<< endl;
  cout <<" err1  =  "<<mtserr1<<", err2 =  "<<mtserr2<<", err3 =  "<<mtserr3<< endl;
  
  
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(00000000000);
  hPtnp->SetMarkerStyle(22); hPtnpts->SetMarkerStyle(21);
  hPtnp->SetMarkerColor(2);  hPtnpts->SetMarkerColor(3);
  hPtnp->SetLineColor(2);    hPtnpts->SetLineColor(3);
  
  hPIch->SetMarkerStyle(24);
  hPIch->SetMarkerColor(4);
  hPIch->SetLineColor(4);  
  
  hPMmb->SetMarkerStyle(26); hPMmbts->SetMarkerStyle(25);
  hPMmb->SetMarkerColor(2);  hPMmbts->SetMarkerColor(3);
  hPMmb->SetLineColor(2);    hPMmbts->SetLineColor(3); 
  
  TCanvas *c3 = new TCanvas("c3", "c3");
  hPtnp->SetMinimum(0.3);
  hPtnp->SetMaximum(1.0);
  hPtnp->SetTitle("Barrel");
  hPtnp->GetXaxis()->SetTitle("P_{T}");
  hPtnp->GetYaxis()->SetTitle("Efficiency");
  hPtnp->Draw("p");
  hPMmb->Draw("samep");
  hPIch->Draw("samep");
  hPtnpts->Draw("samep");
  hPMmbts->Draw("samep");
  
  legg = new TLegend(0.4,0.2,0.8,0.6);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.08); legg->SetTextFont(132); 
  legg->SetHeader("positive muons");
  legge = legg->AddEntry(hPtnp,  "No Trig TrSel tag and probe 1","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPMmb,  "No Trig TrSel tag and probe 2","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPtnpts,  "TrSel tag and probe 1","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPMmbts,  "TrSel tag and probe 2","p"); legge->SetTextColor(kBlack);  
  legge = legg->AddEntry(hPIch,  " ICHEP","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
}


void DataOverlayEndCapTrigNeg() {

  fPidTablePtTnpPos = new PidTable("PidTables/DATA/Jpsi/MuID/PtTnpNeg-jpsi.dat");
  fPidTablePtMmbPos = new PidTable("PidTables/DATA/Jpsi/MuID/PtMmbNeg-jpsi.dat");
  fPidTablePtICHEP  = new PidTable("PidTables/DATA/Jpsi/MuID/PtICHEP-jpsi.dat");
  fPidTablePtTnpPosTrS = new PidTable("PidTables/DATA/Jpsi/MuID/TrSel/PtTnpNeg-jpsi.dat");
  fPidTablePtMmbPosTrS = new PidTable("PidTables/DATA/Jpsi/MuID/TrSel/PtMmbNeg-jpsi.dat");  
  
  TH2D *hpt = fPidTablePtTnpPos->get2dHist("hpt", "hpt", 1);  
  TH1D *hPtnp = new TH1D("hPtnp", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPMmb = new TH1D("hPMmb", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPIch = new TH1D("hPIch", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPtnpts = new TH1D("hPtnpTS", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPMmbts = new TH1D("hPMmbTS", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  
  double teff1 = fPidTablePtTnpPos->effD(3.5, 2., 0.); double terr1 = fPidTablePtTnpPos->errD(3.5, 2., 0.); 
  double teff2 = fPidTablePtTnpPos->effD(5., 2., 0.);  double terr2 = fPidTablePtTnpPos->errD(5., 2., 0.); 
  double teff3 = fPidTablePtTnpPos->effD(7., 2., 0.);  double terr3 = fPidTablePtTnpPos->errD(7., 2., 0.);

  double meff1 = fPidTablePtMmbPos->effD(3.5, 2., 0.); double merr1 = fPidTablePtMmbPos->errD(3.5, 2., 0.); 
  double meff2 = fPidTablePtMmbPos->effD(5., 2., 0.);  double merr2 = fPidTablePtMmbPos->errD(5., 2., 0.); 
  double meff3 = fPidTablePtMmbPos->effD(7., 2., 0.);  double merr3 = fPidTablePtMmbPos->errD(7., 2., 0.);

  double ieff1 = fPidTablePtICHEP->effD(3.5, 2., 0.);  double ierr1 = fPidTablePtICHEP->errD(3.5, 2., 0.); 
  double ieff2 = fPidTablePtICHEP->effD(5., 2., 0.);   double ierr2 = fPidTablePtICHEP->errD(5., 2., 0.); 
  double ieff3 = fPidTablePtICHEP->effD(7., 2., 0.);   double ierr3 = fPidTablePtICHEP->errD(7., 2., 0.);
  
  double tseff1 = fPidTablePtTnpPosTrS->effD(3.5, 2., 0.); double tserr1 = fPidTablePtTnpPosTrS->errD(3.5, 2., 0.); 
  double tseff2 = fPidTablePtTnpPosTrS->effD(5., 2., 0.);  double tserr2 = fPidTablePtTnpPosTrS->errD(5., 2., 0.); 
  double tseff3 = fPidTablePtTnpPosTrS->effD(7., 2., 0.);  double tserr3 = fPidTablePtTnpPosTrS->errD(7., 2., 0.);

  double mtseff1 = fPidTablePtMmbPosTrS->effD(3.5, 2., 0.); double mtserr1 = fPidTablePtMmbPosTrS->errD(3.5, 2., 0.); 
  double mtseff2 = fPidTablePtMmbPosTrS->effD(5., 2., 0.);  double mtserr2 = fPidTablePtMmbPosTrS->errD(5., 2., 0.); 
  double mtseff3 = fPidTablePtMmbPosTrS->effD(7., 2., 0.);  double mtserr3 = fPidTablePtMmbPosTrS->errD(7., 2., 0.);
  

  hPtnp->SetBinContent(1,teff1);  hPtnp->SetBinError(1,terr1);
  hPtnp->SetBinContent(2,teff2);  hPtnp->SetBinError(2,terr2);
  hPtnp->SetBinContent(3,teff3);  hPtnp->SetBinError(3,terr3);
  
  hPMmb->SetBinContent(1,meff1);  hPMmb->SetBinError(1,merr1);
  hPMmb->SetBinContent(2,meff2);  hPMmb->SetBinError(2,merr2);
  hPMmb->SetBinContent(3,meff3);  hPMmb->SetBinError(3,merr3);
  
  hPIch->SetBinContent(1,ieff1);  hPIch->SetBinError(1,ierr1);
  hPIch->SetBinContent(2,ieff2);  hPIch->SetBinError(2,ierr2);
  hPIch->SetBinContent(3,ieff3);  hPIch->SetBinError(3,ierr3); 

  hPtnpts->SetBinContent(1,tseff1);  hPtnpts->SetBinError(1,tserr1);
  hPtnpts->SetBinContent(2,tseff2);  hPtnpts->SetBinError(2,tserr2);
  hPtnpts->SetBinContent(3,tseff3);  hPtnpts->SetBinError(3,tserr3);
  
  hPMmbts->SetBinContent(1,mtseff1);  hPMmbts->SetBinError(1,mtserr1);
  hPMmbts->SetBinContent(2,mtseff2);  hPMmbts->SetBinError(2,mtserr2);
  hPMmbts->SetBinContent(3,mtseff3);  hPMmbts->SetBinError(3,mtserr3);
  
  cout <<"TNP"<<endl; 
  cout <<" eff1  =  "<<teff1<<", eff2 =  "<<teff2<<", eff3 =  "<<teff3<< endl;
  cout <<" err1  =  "<<terr1<<", err2 =  "<<terr2<<", err3 =  "<<terr3<< endl;
  cout <<"Mmb"<<endl; 
  cout <<" eff1  =  "<<meff1<<", eff2 =  "<<meff2<<", eff3 =  "<<meff3<< endl;
  cout <<" err1  =  "<<merr1<<", err2 =  "<<merr2<<", err3 =  "<<merr3<< endl;  
  cout <<"ICHEP"<<endl; 
  cout <<" eff1  =  "<<ieff1<<", eff2 =  "<<ieff2<<", eff3 =  "<<ieff3<< endl;
  cout <<" err1  =  "<<ierr1<<", err2 =  "<<ierr2<<", err3 =  "<<ierr3<< endl;  
  cout <<"TNP TS"<<endl; 
  cout <<" eff1  =  "<<tseff1<<", eff2 =  "<<tseff2<<", eff3 =  "<<tseff3<< endl;
  cout <<" err1  =  "<<tserr1<<", err2 =  "<<tserr2<<", err3 =  "<<tserr3<< endl;
  cout <<"Mmb TS"<<endl; 
  cout <<" eff1  =  "<<mtseff1<<", eff2 =  "<<mtseff2<<", eff3 =  "<<mtseff3<< endl;
  cout <<" err1  =  "<<mtserr1<<", err2 =  "<<mtserr2<<", err3 =  "<<mtserr3<< endl;
  
  
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(00000000000);
  hPtnp->SetMarkerStyle(22); hPtnpts->SetMarkerStyle(21);
  hPtnp->SetMarkerColor(2);  hPtnpts->SetMarkerColor(3);
  hPtnp->SetLineColor(2);    hPtnpts->SetLineColor(3);
  
  hPIch->SetMarkerStyle(24);
  hPIch->SetMarkerColor(4);
  hPIch->SetLineColor(4);  
  
  hPMmb->SetMarkerStyle(26); hPMmbts->SetMarkerStyle(25);
  hPMmb->SetMarkerColor(2);  hPMmbts->SetMarkerColor(3);
  hPMmb->SetLineColor(2);    hPMmbts->SetLineColor(3); 
  
  TCanvas *c4 = new TCanvas("c0", "c4");
  hPtnp->SetMinimum(0.3);
  hPtnp->SetMaximum(1.0);
  hPtnp->SetTitle("EndCaps");
  hPtnp->GetXaxis()->SetTitle("P_{T}");
  hPtnp->GetYaxis()->SetTitle("Efficiency");
  hPtnp->Draw("p");
  hPMmb->Draw("samep");
  hPIch->Draw("samep");
  hPtnpts->Draw("samep");
  hPMmbts->Draw("samep");
  
  legg = new TLegend(0.4,0.2,0.8,0.6);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.08); legg->SetTextFont(132); 
  legg->SetHeader("negative muons");
  legge = legg->AddEntry(hPtnp,  " tag and probe 1","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPMmb,  " tag and probe 2","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPtnpts,  "TrSel tag and probe 1","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPMmbts,  "TrSel tag and probe 2","p"); legge->SetTextColor(kBlack);  
  legge = legg->AddEntry(hPIch,  " ICHEP","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
}

void DataOverlayEndCapTrigPos() {

  fPidTablePtTnpPos = new PidTable("PidTables/DATA/Jpsi/MuID/PtTnpPos-jpsi.dat");
  fPidTablePtMmbPos = new PidTable("PidTables/DATA/Jpsi/MuID/PtMmbPos-jpsi.dat");
  fPidTablePtICHEP  = new PidTable("PidTables/DATA/Jpsi/MuID/PtICHEP-jpsi.dat");
  fPidTablePtTnpPosTrS = new PidTable("PidTables/DATA/Jpsi/MuID/TrSel/PtTnpPos-jpsi.dat");
  fPidTablePtMmbPosTrS = new PidTable("PidTables/DATA/Jpsi/MuID/TrSel/PtMmbPos-jpsi.dat");  
  
  TH2D *hpt = fPidTablePtTnpPos->get2dHist("hpt", "hpt", 1);  
  TH1D *hPtnp = new TH1D("hPtnp", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPMmb = new TH1D("hPMmb", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPIch = new TH1D("hPIch", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPtnpts = new TH1D("hPtnpTS", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPMmbts = new TH1D("hPMmbTS", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  
  double teff1 = fPidTablePtTnpPos->effD(3.5, 2., 0.); double terr1 = fPidTablePtTnpPos->errD(3.5, 2., 0.); 
  double teff2 = fPidTablePtTnpPos->effD(5., 2., 0.);  double terr2 = fPidTablePtTnpPos->errD(5., 2., 0.); 
  double teff3 = fPidTablePtTnpPos->effD(7., 2., 0.);  double terr3 = fPidTablePtTnpPos->errD(7., 2., 0.);

  double meff1 = fPidTablePtMmbPos->effD(3.5, 2., 0.); double merr1 = fPidTablePtMmbPos->errD(3.5, 2., 0.); 
  double meff2 = fPidTablePtMmbPos->effD(5., 2., 0.);  double merr2 = fPidTablePtMmbPos->errD(5., 2., 0.); 
  double meff3 = fPidTablePtMmbPos->effD(7., 2., 0.);  double merr3 = fPidTablePtMmbPos->errD(7., 2., 0.);

  double ieff1 = fPidTablePtICHEP->effD(3.5, 2., 0.);  double ierr1 = fPidTablePtICHEP->errD(3.5, 2., 0.); 
  double ieff2 = fPidTablePtICHEP->effD(5., 2., 0.);   double ierr2 = fPidTablePtICHEP->errD(5., 2., 0.); 
  double ieff3 = fPidTablePtICHEP->effD(7., 2., 0.);   double ierr3 = fPidTablePtICHEP->errD(7., 2., 0.);
  
  double tseff1 = fPidTablePtTnpPosTrS->effD(3.5, 2., 0.); double tserr1 = fPidTablePtTnpPosTrS->errD(3.5, 2., 0.); 
  double tseff2 = fPidTablePtTnpPosTrS->effD(5., 2., 0.);  double tserr2 = fPidTablePtTnpPosTrS->errD(5., 2., 0.); 
  double tseff3 = fPidTablePtTnpPosTrS->effD(7., 2., 0.);  double tserr3 = fPidTablePtTnpPosTrS->errD(7., 2., 0.);

  double mtseff1 = fPidTablePtMmbPosTrS->effD(3.5, 2., 0.); double mtserr1 = fPidTablePtMmbPosTrS->errD(3.5, 2., 0.); 
  double mtseff2 = fPidTablePtMmbPosTrS->effD(5., 2., 0.);  double mtserr2 = fPidTablePtMmbPosTrS->errD(5., 2., 0.); 
  double mtseff3 = fPidTablePtMmbPosTrS->effD(7., 2., 0.);  double mtserr3 = fPidTablePtMmbPosTrS->errD(7., 2., 0.);
  

  hPtnp->SetBinContent(1,teff1);  hPtnp->SetBinError(1,terr1);
  hPtnp->SetBinContent(2,teff2);  hPtnp->SetBinError(2,terr2);
  hPtnp->SetBinContent(3,teff3);  hPtnp->SetBinError(3,terr3);
  
  hPMmb->SetBinContent(1,meff1);  hPMmb->SetBinError(1,merr1);
  hPMmb->SetBinContent(2,meff2);  hPMmb->SetBinError(2,merr2);
  hPMmb->SetBinContent(3,meff3);  hPMmb->SetBinError(3,merr3);
  
  hPIch->SetBinContent(1,ieff1);  hPIch->SetBinError(1,ierr1);
  hPIch->SetBinContent(2,ieff2);  hPIch->SetBinError(2,ierr2);
  hPIch->SetBinContent(3,ieff3);  hPIch->SetBinError(3,ierr3); 

  hPtnpts->SetBinContent(1,tseff1);  hPtnpts->SetBinError(1,tserr1);
  hPtnpts->SetBinContent(2,tseff2);  hPtnpts->SetBinError(2,tserr2);
  hPtnpts->SetBinContent(3,tseff3);  hPtnpts->SetBinError(3,tserr3);
  
  hPMmbts->SetBinContent(1,mtseff1);  hPMmbts->SetBinError(1,mtserr1);
  hPMmbts->SetBinContent(2,mtseff2);  hPMmbts->SetBinError(2,mtserr2);
  hPMmbts->SetBinContent(3,mtseff3);  hPMmbts->SetBinError(3,mtserr3);
  
  cout <<"TNP"<<endl; 
  cout <<" eff1  =  "<<teff1<<", eff2 =  "<<teff2<<", eff3 =  "<<teff3<< endl;
  cout <<" err1  =  "<<terr1<<", err2 =  "<<terr2<<", err3 =  "<<terr3<< endl;
  cout <<"Mmb"<<endl; 
  cout <<" eff1  =  "<<meff1<<", eff2 =  "<<meff2<<", eff3 =  "<<meff3<< endl;
  cout <<" err1  =  "<<merr1<<", err2 =  "<<merr2<<", err3 =  "<<merr3<< endl;  
  cout <<"ICHEP"<<endl; 
  cout <<" eff1  =  "<<ieff1<<", eff2 =  "<<ieff2<<", eff3 =  "<<ieff3<< endl;
  cout <<" err1  =  "<<ierr1<<", err2 =  "<<ierr2<<", err3 =  "<<ierr3<< endl;  
  cout <<"TNP TS"<<endl; 
  cout <<" eff1  =  "<<tseff1<<", eff2 =  "<<tseff2<<", eff3 =  "<<tseff3<< endl;
  cout <<" err1  =  "<<tserr1<<", err2 =  "<<tserr2<<", err3 =  "<<tserr3<< endl;
  cout <<"Mmb TS"<<endl; 
  cout <<" eff1  =  "<<mtseff1<<", eff2 =  "<<mtseff2<<", eff3 =  "<<mtseff3<< endl;
  cout <<" err1  =  "<<mtserr1<<", err2 =  "<<mtserr2<<", err3 =  "<<mtserr3<< endl;
  
  
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(00000000000);
  hPtnp->SetMarkerStyle(22); hPtnpts->SetMarkerStyle(21);
  hPtnp->SetMarkerColor(2);  hPtnpts->SetMarkerColor(3);
  hPtnp->SetLineColor(2);    hPtnpts->SetLineColor(3);
  
  hPIch->SetMarkerStyle(24);
  hPIch->SetMarkerColor(4);
  hPIch->SetLineColor(4);  
  
  hPMmb->SetMarkerStyle(26); hPMmbts->SetMarkerStyle(25);
  hPMmb->SetMarkerColor(2);  hPMmbts->SetMarkerColor(3);
  hPMmb->SetLineColor(2);    hPMmbts->SetLineColor(3); 
  
  TCanvas *c5 = new TCanvas("c5", "c5");
  hPtnp->SetMinimum(0.3);
  hPtnp->SetMaximum(1.0);
  hPtnp->SetTitle("EndCaps");
  hPtnp->GetXaxis()->SetTitle("P_{T}");
  hPtnp->GetYaxis()->SetTitle("Efficiency");
  hPtnp->Draw("p");
  hPMmb->Draw("samep");
  hPIch->Draw("samep");
  hPtnpts->Draw("samep");
  hPMmbts->Draw("samep");
  
  legg = new TLegend(0.4,0.2,0.8,0.6);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.08); legg->SetTextFont(132); 
  legg->SetHeader("positive muons");
  legge = legg->AddEntry(hPtnp,  " tag and probe 1","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPMmb,  " tag and probe 2","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPtnpts,  "TrSel tag and probe 1","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPMmbts,  "TrSel tag and probe 2","p"); legge->SetTextColor(kBlack);  
  legge = legg->AddEntry(hPIch,  " ICHEP","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
}



void DataOverlayEndCapNoTrigNeg() {

  fPidTablePtTnpPos = new PidTable("PidTables/DATA/Jpsi/MuID/NoTrig/PtTnpNeg-jpsi.dat");
  fPidTablePtMmbPos = new PidTable("PidTables/DATA/Jpsi/MuID/NoTrig/PtMmbNeg-jpsi.dat");
  fPidTablePtICHEP  = new PidTable("PidTables/DATA/Jpsi/MuID/PtICHEP-jpsi.dat");
  fPidTablePtTnpPosTrS = new PidTable("PidTables/DATA/Jpsi/MuID/TrSel/PtTnpNeg-jpsi.dat");
  fPidTablePtMmbPosTrS = new PidTable("PidTables/DATA/Jpsi/MuID/TrSel/PtMmbNeg-jpsi.dat");  
  
  TH2D *hpt = fPidTablePtTnpPos->get2dHist("hpt", "hpt", 1);  
  TH1D *hPtnp = new TH1D("hPtnp", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPMmb = new TH1D("hPMmb", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPIch = new TH1D("hPIch", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPtnpts = new TH1D("hPtnpTS", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPMmbts = new TH1D("hPMmbTS", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  
  double teff1 = fPidTablePtTnpPos->effD(3.5, 2., 0.); double terr1 = fPidTablePtTnpPos->errD(3.5, 2., 0.); 
  double teff2 = fPidTablePtTnpPos->effD(5., 2., 0.);  double terr2 = fPidTablePtTnpPos->errD(5., 2., 0.); 
  double teff3 = fPidTablePtTnpPos->effD(7., 2., 0.);  double terr3 = fPidTablePtTnpPos->errD(7., 2., 0.);

  double meff1 = fPidTablePtMmbPos->effD(3.5, 2., 0.); double merr1 = fPidTablePtMmbPos->errD(3.5, 2., 0.); 
  double meff2 = fPidTablePtMmbPos->effD(5., 2., 0.);  double merr2 = fPidTablePtMmbPos->errD(5., 2., 0.); 
  double meff3 = fPidTablePtMmbPos->effD(7., 2., 0.);  double merr3 = fPidTablePtMmbPos->errD(7., 2., 0.);

  double ieff1 = fPidTablePtICHEP->effD(3.5, 2., 0.);  double ierr1 = fPidTablePtICHEP->errD(3.5, 2., 0.); 
  double ieff2 = fPidTablePtICHEP->effD(5., 2., 0.);   double ierr2 = fPidTablePtICHEP->errD(5., 2., 0.); 
  double ieff3 = fPidTablePtICHEP->effD(7., 2., 0.);   double ierr3 = fPidTablePtICHEP->errD(7., 2., 0.);
  
  double tseff1 = fPidTablePtTnpPosTrS->effD(3.5, 2., 0.); double tserr1 = fPidTablePtTnpPosTrS->errD(3.5, 2., 0.); 
  double tseff2 = fPidTablePtTnpPosTrS->effD(5., 2., 0.);  double tserr2 = fPidTablePtTnpPosTrS->errD(5., 2., 0.); 
  double tseff3 = fPidTablePtTnpPosTrS->effD(7., 2., 0.);  double tserr3 = fPidTablePtTnpPosTrS->errD(7., 2., 0.);

  double mtseff1 = fPidTablePtMmbPosTrS->effD(3.5, 2., 0.); double mtserr1 = fPidTablePtMmbPosTrS->errD(3.5, 2., 0.); 
  double mtseff2 = fPidTablePtMmbPosTrS->effD(5., 2., 0.);  double mtserr2 = fPidTablePtMmbPosTrS->errD(5., 2., 0.); 
  double mtseff3 = fPidTablePtMmbPosTrS->effD(7., 2., 0.);  double mtserr3 = fPidTablePtMmbPosTrS->errD(7., 2., 0.);
  

  hPtnp->SetBinContent(1,teff1);  hPtnp->SetBinError(1,terr1);
  hPtnp->SetBinContent(2,teff2);  hPtnp->SetBinError(2,terr2);
  hPtnp->SetBinContent(3,teff3);  hPtnp->SetBinError(3,terr3);
  
  hPMmb->SetBinContent(1,meff1);  hPMmb->SetBinError(1,merr1);
  hPMmb->SetBinContent(2,meff2);  hPMmb->SetBinError(2,merr2);
  hPMmb->SetBinContent(3,meff3);  hPMmb->SetBinError(3,merr3);
  
  hPIch->SetBinContent(1,ieff1);  hPIch->SetBinError(1,ierr1);
  hPIch->SetBinContent(2,ieff2);  hPIch->SetBinError(2,ierr2);
  hPIch->SetBinContent(3,ieff3);  hPIch->SetBinError(3,ierr3); 

  hPtnpts->SetBinContent(1,tseff1);  hPtnpts->SetBinError(1,tserr1);
  hPtnpts->SetBinContent(2,tseff2);  hPtnpts->SetBinError(2,tserr2);
  hPtnpts->SetBinContent(3,tseff3);  hPtnpts->SetBinError(3,tserr3);
  
  hPMmbts->SetBinContent(1,mtseff1);  hPMmbts->SetBinError(1,mtserr1);
  hPMmbts->SetBinContent(2,mtseff2);  hPMmbts->SetBinError(2,mtserr2);
  hPMmbts->SetBinContent(3,mtseff3);  hPMmbts->SetBinError(3,mtserr3);
  
  cout <<"TNP"<<endl; 
  cout <<" eff1  =  "<<teff1<<", eff2 =  "<<teff2<<", eff3 =  "<<teff3<< endl;
  cout <<" err1  =  "<<terr1<<", err2 =  "<<terr2<<", err3 =  "<<terr3<< endl;
  cout <<"Mmb"<<endl; 
  cout <<" eff1  =  "<<meff1<<", eff2 =  "<<meff2<<", eff3 =  "<<meff3<< endl;
  cout <<" err1  =  "<<merr1<<", err2 =  "<<merr2<<", err3 =  "<<merr3<< endl;  
  cout <<"ICHEP"<<endl; 
  cout <<" eff1  =  "<<ieff1<<", eff2 =  "<<ieff2<<", eff3 =  "<<ieff3<< endl;
  cout <<" err1  =  "<<ierr1<<", err2 =  "<<ierr2<<", err3 =  "<<ierr3<< endl;  
  cout <<"TNP TS"<<endl; 
  cout <<" eff1  =  "<<tseff1<<", eff2 =  "<<tseff2<<", eff3 =  "<<tseff3<< endl;
  cout <<" err1  =  "<<tserr1<<", err2 =  "<<tserr2<<", err3 =  "<<tserr3<< endl;
  cout <<"Mmb TS"<<endl; 
  cout <<" eff1  =  "<<mtseff1<<", eff2 =  "<<mtseff2<<", eff3 =  "<<mtseff3<< endl;
  cout <<" err1  =  "<<mtserr1<<", err2 =  "<<mtserr2<<", err3 =  "<<mtserr3<< endl;
  
  
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(00000000000);
  hPtnp->SetMarkerStyle(22); hPtnpts->SetMarkerStyle(21);
  hPtnp->SetMarkerColor(2);  hPtnpts->SetMarkerColor(3);
  hPtnp->SetLineColor(2);    hPtnpts->SetLineColor(3);
  
  hPIch->SetMarkerStyle(24);
  hPIch->SetMarkerColor(4);
  hPIch->SetLineColor(4);  
  
  hPMmb->SetMarkerStyle(26); hPMmbts->SetMarkerStyle(25);
  hPMmb->SetMarkerColor(2);  hPMmbts->SetMarkerColor(3);
  hPMmb->SetLineColor(2);    hPMmbts->SetLineColor(3); 
  
  TCanvas *c6 = new TCanvas("c6", "c6");
  hPtnp->SetMinimum(0.3);
  hPtnp->SetMaximum(1.0);
  hPtnp->SetTitle("EndCaps");
  hPtnp->GetXaxis()->SetTitle("P_{T}");
  hPtnp->GetYaxis()->SetTitle("Efficiency");
  hPtnp->Draw("p");
  hPMmb->Draw("samep");
  hPIch->Draw("samep");
  hPtnpts->Draw("samep");
  hPMmbts->Draw("samep");
  
  legg = new TLegend(0.4,0.2,0.8,0.6);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.08); legg->SetTextFont(132); 
  legg->SetHeader("negative muons");
  legge = legg->AddEntry(hPtnp,  "No Trig TrSel tag and probe 1","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPMmb,  "No Trig TrSel tag and probe 2","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPtnpts,  "TrSel tag and probe 1","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPMmbts,  "TrSel tag and probe 2","p"); legge->SetTextColor(kBlack);  
  legge = legg->AddEntry(hPIch,  " ICHEP","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
}

void DataOverlayEndCapNoTrigPos() {

  fPidTablePtTnpPos = new PidTable("PidTables/DATA/Jpsi/MuID/NoTrig/PtTnpPos-jpsi.dat");
  fPidTablePtMmbPos = new PidTable("PidTables/DATA/Jpsi/MuID/NoTrig/PtMmbPos-jpsi.dat");
  fPidTablePtICHEP  = new PidTable("PidTables/DATA/Jpsi/MuID/PtICHEP-jpsi.dat");
  fPidTablePtTnpPosTrS = new PidTable("PidTables/DATA/Jpsi/MuID/TrSel/PtTnpPos-jpsi.dat");
  fPidTablePtMmbPosTrS = new PidTable("PidTables/DATA/Jpsi/MuID/TrSel/PtMmbPos-jpsi.dat");  
  
  TH2D *hpt = fPidTablePtTnpPos->get2dHist("hpt", "hpt", 1);  
  TH1D *hPtnp = new TH1D("hPtnp", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPMmb = new TH1D("hPMmb", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPIch = new TH1D("hPIch", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPtnpts = new TH1D("hPtnpTS", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  TH1D *hPMmbts = new TH1D("hPMmbTS", "", hpt->GetNbinsY(), hpt->GetYaxis()->GetXbins()->GetArray());
  
  double teff1 = fPidTablePtTnpPos->effD(3.5, 2., 0.); double terr1 = fPidTablePtTnpPos->errD(3.5, 2., 0.); 
  double teff2 = fPidTablePtTnpPos->effD(5., 2., 0.);  double terr2 = fPidTablePtTnpPos->errD(5., 2., 0.); 
  double teff3 = fPidTablePtTnpPos->effD(7., 2., 0.);  double terr3 = fPidTablePtTnpPos->errD(7., 2., 0.);

  double meff1 = fPidTablePtMmbPos->effD(3.5, 2., 0.); double merr1 = fPidTablePtMmbPos->errD(3.5, 2., 0.); 
  double meff2 = fPidTablePtMmbPos->effD(5., 2., 0.);  double merr2 = fPidTablePtMmbPos->errD(5., 2., 0.); 
  double meff3 = fPidTablePtMmbPos->effD(7., 2., 0.);  double merr3 = fPidTablePtMmbPos->errD(7., 2., 0.);

  double ieff1 = fPidTablePtICHEP->effD(3.5, 2., 0.);  double ierr1 = fPidTablePtICHEP->errD(3.5, 2., 0.); 
  double ieff2 = fPidTablePtICHEP->effD(5., 2., 0.);   double ierr2 = fPidTablePtICHEP->errD(5., 2., 0.); 
  double ieff3 = fPidTablePtICHEP->effD(7., 2., 0.);   double ierr3 = fPidTablePtICHEP->errD(7., 2., 0.);
  
  double tseff1 = fPidTablePtTnpPosTrS->effD(3.5, 2., 0.); double tserr1 = fPidTablePtTnpPosTrS->errD(3.5, 2., 0.); 
  double tseff2 = fPidTablePtTnpPosTrS->effD(5., 2., 0.);  double tserr2 = fPidTablePtTnpPosTrS->errD(5., 2., 0.); 
  double tseff3 = fPidTablePtTnpPosTrS->effD(7., 2., 0.);  double tserr3 = fPidTablePtTnpPosTrS->errD(7., 2., 0.);

  double mtseff1 = fPidTablePtMmbPosTrS->effD(3.5, 2., 0.); double mtserr1 = fPidTablePtMmbPosTrS->errD(3.5, 2., 0.); 
  double mtseff2 = fPidTablePtMmbPosTrS->effD(5., 2., 0.);  double mtserr2 = fPidTablePtMmbPosTrS->errD(5., 2., 0.); 
  double mtseff3 = fPidTablePtMmbPosTrS->effD(7., 2., 0.);  double mtserr3 = fPidTablePtMmbPosTrS->errD(7., 2., 0.);
  

  hPtnp->SetBinContent(1,teff1);  hPtnp->SetBinError(1,terr1);
  hPtnp->SetBinContent(2,teff2);  hPtnp->SetBinError(2,terr2);
  hPtnp->SetBinContent(3,teff3);  hPtnp->SetBinError(3,terr3);
  
  hPMmb->SetBinContent(1,meff1);  hPMmb->SetBinError(1,merr1);
  hPMmb->SetBinContent(2,meff2);  hPMmb->SetBinError(2,merr2);
  hPMmb->SetBinContent(3,meff3);  hPMmb->SetBinError(3,merr3);
  
  hPIch->SetBinContent(1,ieff1);  hPIch->SetBinError(1,ierr1);
  hPIch->SetBinContent(2,ieff2);  hPIch->SetBinError(2,ierr2);
  hPIch->SetBinContent(3,ieff3);  hPIch->SetBinError(3,ierr3); 

  hPtnpts->SetBinContent(1,tseff1);  hPtnpts->SetBinError(1,tserr1);
  hPtnpts->SetBinContent(2,tseff2);  hPtnpts->SetBinError(2,tserr2);
  hPtnpts->SetBinContent(3,tseff3);  hPtnpts->SetBinError(3,tserr3);
  
  hPMmbts->SetBinContent(1,mtseff1);  hPMmbts->SetBinError(1,mtserr1);
  hPMmbts->SetBinContent(2,mtseff2);  hPMmbts->SetBinError(2,mtserr2);
  hPMmbts->SetBinContent(3,mtseff3);  hPMmbts->SetBinError(3,mtserr3);
  
  cout <<"TNP"<<endl; 
  cout <<" eff1  =  "<<teff1<<", eff2 =  "<<teff2<<", eff3 =  "<<teff3<< endl;
  cout <<" err1  =  "<<terr1<<", err2 =  "<<terr2<<", err3 =  "<<terr3<< endl;
  cout <<"Mmb"<<endl; 
  cout <<" eff1  =  "<<meff1<<", eff2 =  "<<meff2<<", eff3 =  "<<meff3<< endl;
  cout <<" err1  =  "<<merr1<<", err2 =  "<<merr2<<", err3 =  "<<merr3<< endl;  
  cout <<"ICHEP"<<endl; 
  cout <<" eff1  =  "<<ieff1<<", eff2 =  "<<ieff2<<", eff3 =  "<<ieff3<< endl;
  cout <<" err1  =  "<<ierr1<<", err2 =  "<<ierr2<<", err3 =  "<<ierr3<< endl;  
  cout <<"TNP TS"<<endl; 
  cout <<" eff1  =  "<<tseff1<<", eff2 =  "<<tseff2<<", eff3 =  "<<tseff3<< endl;
  cout <<" err1  =  "<<tserr1<<", err2 =  "<<tserr2<<", err3 =  "<<tserr3<< endl;
  cout <<"Mmb TS"<<endl; 
  cout <<" eff1  =  "<<mtseff1<<", eff2 =  "<<mtseff2<<", eff3 =  "<<mtseff3<< endl;
  cout <<" err1  =  "<<mtserr1<<", err2 =  "<<mtserr2<<", err3 =  "<<mtserr3<< endl;
  
  
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(00000000000);
  hPtnp->SetMarkerStyle(22); hPtnpts->SetMarkerStyle(21);
  hPtnp->SetMarkerColor(2);  hPtnpts->SetMarkerColor(3);
  hPtnp->SetLineColor(2);    hPtnpts->SetLineColor(3);
  
  hPIch->SetMarkerStyle(24);
  hPIch->SetMarkerColor(4);
  hPIch->SetLineColor(4);  
  
  hPMmb->SetMarkerStyle(26); hPMmbts->SetMarkerStyle(25);
  hPMmb->SetMarkerColor(2);  hPMmbts->SetMarkerColor(3);
  hPMmb->SetLineColor(2);    hPMmbts->SetLineColor(3); 
  
  TCanvas *c7 = new TCanvas("c7", "c7");
  hPtnp->SetMinimum(0.3);
  hPtnp->SetMaximum(1.0);
  hPtnp->SetTitle("EndCaps");
  hPtnp->GetXaxis()->SetTitle("P_{T}");
  hPtnp->GetYaxis()->SetTitle("Efficiency");
  hPtnp->Draw("p");
  hPMmb->Draw("samep");
  hPIch->Draw("samep");
  hPtnpts->Draw("samep");
  hPMmbts->Draw("samep");
  
  legg = new TLegend(0.4,0.2,0.8,0.6);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.08); legg->SetTextFont(132); 
  legg->SetHeader("positive muons");
  legge = legg->AddEntry(hPtnp,  "No Trig TrSel tag and probe 1","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPMmb,  "No Trig TrSel tag and probe 2","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPtnpts,  "TrSel tag and probe 1","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(hPMmbts,  "TrSel tag and probe 2","p"); legge->SetTextColor(kBlack);  
  legge = legg->AddEntry(hPIch,  " ICHEP","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  
}


