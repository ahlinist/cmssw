#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/PidTable.hh"


void MuID_Pos(){
  gStyle->SetOptStat(00000000000);
  int Npt(5); int Neta(5);
  double effData(-99); double effMC(-99); double effDataErr(-99); double effMCErr(-99); 
  fPidTableData = new PidTable("../tnp/PidTables/DATA/Jpsi/MuID/CowboyVeto/TrackerMuonArbitrated/PtMmbPos-jpsi.tma.nb.dat");
  fPidTableMC = new PidTable("../tnp/PidTables/MC/Jpsi/MuID/CowboyVeto/TrackerMuonArbitrated/PtMmbPos-jpsi.tma.nb.dat");
  
  
  double pt[6] = {3., 4., 5., 6., 8., 50.};
  double eta[6] = {-2.4, -1.2, -0.4, 0.4, 1.2, 2.4};

  TH1D *EffData1; TH1D *EffMC1; TH1D *EffData2; TH1D *EffMC2; TH1D *EffData3; TH1D *EffMC3; 
  TH1D *EffData4; TH1D *EffMC4; TH1D *EffData5; TH1D *EffMC5;
  EffData1 = new TH1D("MuID Efficiency", "MuID Efficiency", 5, pt); EffMC1 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 5, pt); 
  EffData2 = new TH1D("MuID Efficiency", "MuID Efficiency", 5, pt); EffMC2 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 5, pt); 
  EffData3 = new TH1D("MuID Efficiency", "MuID Efficiency", 5, pt); EffMC3 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 5, pt);
  EffData4 = new TH1D("MuID Efficiency", "MuID Efficiency", 5, pt); EffMC4 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 5, pt); 
  EffData5 = new TH1D("MuID Efficiency", "MuID Efficiency", 5, pt); EffMC5 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 5, pt);  
  
  for ( int i = 0; i < 5 ; i++){
    for ( int j = 0; j < 5 ; j++){
      
      effData = fPidTableData->effD(pt[i]+0.1, eta[j]+0.1, 0.);
      effDataErr = fPidTableData->errD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMC = fPidTableMC->effD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMCErr = fPidTableMC->errD(pt[i]+0.1, eta[j]+0.1, 0.);
      cout<< pt[i] << "-" << pt[i+1] << " " << eta[j] << "-" << eta[j+1] <<  "effData = " << effData << "effMC = " << effMC << endl;
      
      if ( j == 0 ){
	EffData1->SetBinContent(i+1,effData);
	EffData1->SetBinError(i+1,effDataErr);
	EffMC1->SetBinContent(i+1,effMC);
	EffMC1->SetBinError(i+1,effMCErr);	
      }
      
      if ( j == 1 ){
	EffData2->SetBinContent(i+1,effData);
	EffData2->SetBinError(i+1,effDataErr);
	EffMC2->SetBinContent(i+1,effMC);
	EffMC2->SetBinError(i+1,effMCErr);	
      }
      
      if ( j == 2 ){
	EffData3->SetBinContent(i+1,effData);
	EffData3->SetBinError(i+1,effDataErr);
	EffMC3->SetBinContent(i+1,effMC);
	EffMC3->SetBinError(i+1,effMCErr);	
      }      
      
      if ( j == 3 ){
	EffData4->SetBinContent(i+1,effData);
	EffData4->SetBinError(i+1,effDataErr);
	EffMC4->SetBinContent(i+1,effMC);
	EffMC4->SetBinError(i+1,effMCErr);	
      }
      
      if ( j == 4 ){
	EffData5->SetBinContent(i+1,effData);
	EffData5->SetBinError(i+1,effDataErr);
	EffMC5->SetBinContent(i+1,effMC);
	EffMC5->SetBinError(i+1,effMCErr);	
      }        
      
    }
  }
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  c1->Divide(3,2);
  c1->cd(1);
  EffData1->SetMinimum(0.2);
  EffData1->SetMaximum(1.05);
  EffData1->SetMarkerStyle(22);
  EffData1->SetMarkerColor(2);
  EffData1->SetLineColor(2);
  EffData1->Draw("pe");
  EffMC1->SetMarkerStyle(23);
  EffMC1->SetMarkerColor(3);
  EffMC1->SetLineColor(3);
  EffMC1->Draw("pesame");  
  legg = new TLegend(0.6,0.4,0.8,0.7);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi T&P Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi T&P MC","p"); legge->SetTextColor(kGreen);
  legg->Draw();
  c1->cd(2);
  EffData2->SetMinimum(0.2);
  EffData2->SetMaximum(1.05);
  EffData2->SetMarkerStyle(22);
  EffData2->SetMarkerColor(2);
  EffData2->SetLineColor(2);
  EffData2->Draw("pe");
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(3);
  EffMC2->SetLineColor(3);
  EffMC2->Draw("pesame");  
  legg = new TLegend(0.6,0.4,0.8,0.7);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffData2,  "J/#psi T&P Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC2,  "J/#psi T&P MC","p"); legge->SetTextColor(kGreen);
  legg->Draw();  
  c1->cd(3);
  EffData3->SetMinimum(0.2);
  EffData3->SetMaximum(1.05);
  EffData3->SetMarkerStyle(22);
  EffData3->SetMarkerColor(2);
  EffData3->SetLineColor(2);
  EffData3->Draw("pe");
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(3);
  EffMC3->SetLineColor(3);
  EffMC3->Draw("pesame");  
  legg = new TLegend(0.6,0.4,0.8,0.7);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffData3,  "J/#psi T&P Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC3,  "J/#psi T&P MC","p"); legge->SetTextColor(kGreen);
  legg->Draw();   
  c1->cd(4);
  EffData4->SetMinimum(0.2);
  EffData4->SetMaximum(1.05);
  EffData4->SetMarkerStyle(22);
  EffData4->SetMarkerColor(2);
  EffData4->SetLineColor(2);
  EffData4->Draw("pe");
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(3);
  EffMC4->SetLineColor(3);
  EffMC4->Draw("pesame");  
  legg = new TLegend(0.6,0.4,0.8,0.7);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffData4,  "J/#psi T&P Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC4,  "J/#psi T&P MC","p"); legge->SetTextColor(kGreen);
  legg->Draw();   
  c1->cd(5);
  EffData5->SetMinimum(0.2);
  EffData5->SetMaximum(1.05);
  EffData5->SetMarkerStyle(22);
  EffData5->SetMarkerColor(2);
  EffData5->SetLineColor(2);
  EffData5->Draw("pe");
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(3);
  EffMC5->SetLineColor(3);
  EffMC5->Draw("pesame");  
  legg = new TLegend(0.6,0.4,0.8,0.7);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffData5,  "J/#psi T&P Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC5,  "J/#psi T&P MC","p"); legge->SetTextColor(kGreen);
  legg->Draw();     
  
}

void MuID_Neg(){
  gStyle->SetOptStat(00000000000);
  int Npt(5); int Neta(5);
  double effData(-99); double effMC(-99); double effDataErr(-99); double effMCErr(-99); 
  fPidTableData = new PidTable("../tnp/PidTables/DATA/Jpsi/MuID/CowboyVeto/TrackerMuonArbitrated/PtMmbNeg-jpsi.tma.nb.dat");
  fPidTableMC = new PidTable("../tnp/PidTables/MC/Jpsi/MuID/CowboyVeto/TrackerMuonArbitrated/PtMmbNeg-jpsi.tma.nb.dat");
  
  
  double pt[6] = {3., 4., 5., 6., 8., 50.};
  double eta[6] = {-2.4, -1.2, -0.4, 0.4, 1.2, 2.4};

  TH1D *EffData1; TH1D *EffMC1; TH1D *EffData2; TH1D *EffMC2; TH1D *EffData3; TH1D *EffMC3; 
  TH1D *EffData4; TH1D *EffMC4; TH1D *EffData5; TH1D *EffMC5;
  EffData1 = new TH1D("MuID Efficiency", "MuID Efficiency", 5, pt); EffMC1 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 5, pt); 
  EffData2 = new TH1D("MuID Efficiency", "MuID Efficiency", 5, pt); EffMC2 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 5, pt); 
  EffData3 = new TH1D("MuID Efficiency", "MuID Efficiency", 5, pt); EffMC3 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 5, pt);
  EffData4 = new TH1D("MuID Efficiency", "MuID Efficiency", 5, pt); EffMC4 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 5, pt); 
  EffData5 = new TH1D("MuID Efficiency", "MuID Efficiency", 5, pt); EffMC5 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 5, pt);  
  
  for ( int i = 0; i < 5 ; i++){
    for ( int j = 0; j < 5 ; j++){
      
      effData = fPidTableData->effD(pt[i]+0.1, eta[j]+0.1, 0.);
      effDataErr = fPidTableData->errD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMC = fPidTableMC->effD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMCErr = fPidTableMC->errD(pt[i]+0.1, eta[j]+0.1, 0.);
      cout<< pt[i] << "-" << pt[i+1] << " " << eta[j] << "-" << eta[j+1] <<  "effData = " << effData << "effMC = " << effMC << endl;
      
      if ( j == 0 ){
	EffData1->SetBinContent(i+1,effData);
	EffData1->SetBinError(i+1,effDataErr);
	EffMC1->SetBinContent(i+1,effMC);
	EffMC1->SetBinError(i+1,effMCErr);	
      }
      
      if ( j == 1 ){
	EffData2->SetBinContent(i+1,effData);
	EffData2->SetBinError(i+1,effDataErr);
	EffMC2->SetBinContent(i+1,effMC);
	EffMC2->SetBinError(i+1,effMCErr);	
      }
      
      if ( j == 2 ){
	EffData3->SetBinContent(i+1,effData);
	EffData3->SetBinError(i+1,effDataErr);
	EffMC3->SetBinContent(i+1,effMC);
	EffMC3->SetBinError(i+1,effMCErr);	
      }      
      
      if ( j == 3 ){
	EffData4->SetBinContent(i+1,effData);
	EffData4->SetBinError(i+1,effDataErr);
	EffMC4->SetBinContent(i+1,effMC);
	EffMC4->SetBinError(i+1,effMCErr);	
      }
      
      if ( j == 4 ){
	EffData5->SetBinContent(i+1,effData);
	EffData5->SetBinError(i+1,effDataErr);
	EffMC5->SetBinContent(i+1,effMC);
	EffMC5->SetBinError(i+1,effMCErr);	
      }        
      
    }
  }
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  c2->Divide(3,2);
  c2->cd(1);
  EffData1->SetMinimum(0.2);
  EffData1->SetMaximum(1.05);
  EffData1->SetMarkerStyle(22);
  EffData1->SetMarkerColor(2);
  EffData1->SetLineColor(2);
  EffData1->Draw("pe");
  EffMC1->SetMarkerStyle(23);
  EffMC1->SetMarkerColor(3);
  EffMC1->SetLineColor(3);
  EffMC1->Draw("pesame");  
  legg = new TLegend(0.6,0.4,0.8,0.7);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi T&P Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi T&P MC","p"); legge->SetTextColor(kGreen);
  legg->Draw();
  c2->cd(2);
  EffData2->SetMinimum(0.2);
  EffData2->SetMaximum(1.05);
  EffData2->SetMarkerStyle(22);
  EffData2->SetMarkerColor(2);
  EffData2->SetLineColor(2);
  EffData2->Draw("pe");
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(3);
  EffMC2->SetLineColor(3);
  EffMC2->Draw("pesame");  
  legg = new TLegend(0.6,0.4,0.8,0.7);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffData2,  "J/#psi T&P Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC2,  "J/#psi T&P MC","p"); legge->SetTextColor(kGreen);
  legg->Draw();  
  c2->cd(3);
  EffData3->SetMinimum(0.2);
  EffData3->SetMaximum(1.05);
  EffData3->SetMarkerStyle(22);
  EffData3->SetMarkerColor(2);
  EffData3->SetLineColor(2);
  EffData3->Draw("pe");
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(3);
  EffMC3->SetLineColor(3);
  EffMC3->Draw("pesame");  
  legg = new TLegend(0.6,0.4,0.8,0.7);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffData3,  "J/#psi T&P Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC3,  "J/#psi T&P MC","p"); legge->SetTextColor(kGreen);
  legg->Draw();   
  c2->cd(4);
  EffData4->SetMinimum(0.2);
  EffData4->SetMaximum(1.05);
  EffData4->SetMarkerStyle(22);
  EffData4->SetMarkerColor(2);
  EffData4->SetLineColor(2);
  EffData4->Draw("pe");
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(3);
  EffMC4->SetLineColor(3);
  EffMC4->Draw("pesame");  
  legg = new TLegend(0.6,0.4,0.8,0.7);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffData4,  "J/#psi T&P Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC4,  "J/#psi T&P MC","p"); legge->SetTextColor(kGreen);
  legg->Draw();   
  c2->cd(5);
  EffData5->SetMinimum(0.2);
  EffData5->SetMaximum(1.05);
  EffData5->SetMarkerStyle(22);
  EffData5->SetMarkerColor(2);
  EffData5->SetLineColor(2);
  EffData5->Draw("pe");
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(3);
  EffMC5->SetLineColor(3);
  EffMC5->Draw("pesame");  
  legg = new TLegend(0.6,0.4,0.8,0.7);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffData5,  "J/#psi T&P Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC5,  "J/#psi T&P MC","p"); legge->SetTextColor(kGreen);
  legg->Draw();     
  
}

void Trig_Pos(){
  gStyle->SetOptStat(00000000000);
  int Npt(5); int Neta(5);
  double effData(-99); double effMC(-99); double effDataErr(-99); double effMCErr(-99); 
  fPidTableData = new PidTable("../tnp/PidTables/DATA/Jpsi/Trig/MuOnia/CowboyVeto/PtMmbPos-jpsi.tma.nb.dat");
  fPidTableMC = new PidTable("../tnp/PidTables/MC/Jpsi/Trig/CowboyVeto/PtMmbPos-jpsi.tma.nb.dat");
    
  double pt[6] = {3., 4., 5., 6., 8., 50.};
  double eta[6] = {-2.4, -1.2, -0.4, 0.4, 1.2, 2.4};

  TH1D *EffData1; TH1D *EffMC1; TH1D *EffData2; TH1D *EffMC2; TH1D *EffData3; TH1D *EffMC3; 
  TH1D *EffData4; TH1D *EffMC4; TH1D *EffData5; TH1D *EffMC5;
  EffData1 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); EffMC1 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); 
  EffData2 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); EffMC2 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); 
  EffData3 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); EffMC3 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt);
  EffData4 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); EffMC4 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); 
  EffData5 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); EffMC5 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt);  
  
  for ( int i = 0; i < 5 ; i++){
    for ( int j = 0; j < 5 ; j++){
      
      effData = fPidTableData->effD(pt[i]+0.1, eta[j]+0.1, 0.);
      effDataErr = fPidTableData->errD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMC = fPidTableMC->effD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMCErr = fPidTableMC->errD(pt[i]+0.1, eta[j]+0.1, 0.);
      cout<< pt[i] << "-" << pt[i+1] << " " << eta[j] << "-" << eta[j+1] <<  "effData = " << effData << "effMC = " << effMC << endl;
      
      if ( j == 0 ){
	EffData1->SetBinContent(i+1,effData);
	EffData1->SetBinError(i+1,effDataErr);
	EffMC1->SetBinContent(i+1,effMC);
	EffMC1->SetBinError(i+1,effMCErr);	
      }
      
      if ( j == 1 ){
	EffData2->SetBinContent(i+1,effData);
	EffData2->SetBinError(i+1,effDataErr);
	EffMC2->SetBinContent(i+1,effMC);
	EffMC2->SetBinError(i+1,effMCErr);	
      }
      
      if ( j == 2 ){
	EffData3->SetBinContent(i+1,effData);
	EffData3->SetBinError(i+1,effDataErr);
	EffMC3->SetBinContent(i+1,effMC);
	EffMC3->SetBinError(i+1,effMCErr);	
      }      
      
      if ( j == 3 ){
	EffData4->SetBinContent(i+1,effData);
	EffData4->SetBinError(i+1,effDataErr);
	EffMC4->SetBinContent(i+1,effMC);
	EffMC4->SetBinError(i+1,effMCErr);	
      }
      
      if ( j == 4 ){
	EffData5->SetBinContent(i+1,effData);
	EffData5->SetBinError(i+1,effDataErr);
	EffMC5->SetBinContent(i+1,effMC);
	EffMC5->SetBinError(i+1,effMCErr);	
      }        
      
    }
  }
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  c3->Divide(3,2);
  c3->cd(1);
  EffData1->SetMinimum(0.2);
  EffData1->SetMaximum(1.05);
  EffData1->SetMarkerStyle(22);
  EffData1->SetMarkerColor(2);
  EffData1->SetLineColor(2);
  EffData1->Draw("pe");
  EffMC1->SetMarkerStyle(23);
  EffMC1->SetMarkerColor(3);
  EffMC1->SetLineColor(3);
  EffMC1->Draw("pesame");  
  legg = new TLegend(0.6,0.4,0.8,0.7);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi T&P Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi T&P MC","p"); legge->SetTextColor(kGreen);
  legg->Draw();
  c3->cd(2);
  EffData2->SetMinimum(0.2);
  EffData2->SetMaximum(1.05);
  EffData2->SetMarkerStyle(22);
  EffData2->SetMarkerColor(2);
  EffData2->SetLineColor(2);
  EffData2->Draw("pe");
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(3);
  EffMC2->SetLineColor(3);
  EffMC2->Draw("pesame");  
  legg = new TLegend(0.6,0.4,0.8,0.7);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffData2,  "J/#psi T&P Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC2,  "J/#psi T&P MC","p"); legge->SetTextColor(kGreen);
  legg->Draw();  
  c3->cd(3);
  EffData3->SetMinimum(0.2);
  EffData3->SetMaximum(1.05);
  EffData3->SetMarkerStyle(22);
  EffData3->SetMarkerColor(2);
  EffData3->SetLineColor(2);
  EffData3->Draw("pe");
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(3);
  EffMC3->SetLineColor(3);
  EffMC3->Draw("pesame");  
  legg = new TLegend(0.6,0.4,0.8,0.7);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffData3,  "J/#psi T&P Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC3,  "J/#psi T&P MC","p"); legge->SetTextColor(kGreen);
  legg->Draw();   
  c3->cd(4);
  EffData4->SetMinimum(0.2);
  EffData4->SetMaximum(1.05);
  EffData4->SetMarkerStyle(22);
  EffData4->SetMarkerColor(2);
  EffData4->SetLineColor(2);
  EffData4->Draw("pe");
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(3);
  EffMC4->SetLineColor(3);
  EffMC4->Draw("pesame");  
  legg = new TLegend(0.6,0.4,0.8,0.7);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffData4,  "J/#psi T&P Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC4,  "J/#psi T&P MC","p"); legge->SetTextColor(kGreen);
  legg->Draw();   
  c3->cd(5);
  EffData5->SetMinimum(0.2);
  EffData5->SetMaximum(1.05);
  EffData5->SetMarkerStyle(22);
  EffData5->SetMarkerColor(2);
  EffData5->SetLineColor(2);
  EffData5->Draw("pe");
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(3);
  EffMC5->SetLineColor(3);
  EffMC5->Draw("pesame");  
  legg = new TLegend(0.6,0.4,0.8,0.7);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffData5,  "J/#psi T&P Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC5,  "J/#psi T&P MC","p"); legge->SetTextColor(kGreen);
  legg->Draw();     
  
}

void Trig_Neg(){
  gStyle->SetOptStat(00000000000);
  int Npt(5); int Neta(5);
  double effData(-99); double effMC(-99); double effDataErr(-99); double effMCErr(-99); 
  fPidTableData = new PidTable("../tnp/PidTables/DATA/Jpsi/Trig/MuOnia/CowboyVeto/PtMmbNeg-jpsi.tma.nb.dat");
  fPidTableMC = new PidTable("../tnp/PidTables/MC/Jpsi/Trig/CowboyVeto/PtMmbNeg-jpsi.tma.nb.dat");
    
  double pt[6] = {3., 4., 5., 6., 8., 50.};
  double eta[6] = {-2.4, -1.2, -0.4, 0.4, 1.2, 2.4};

  TH1D *EffData1; TH1D *EffMC1; TH1D *EffData2; TH1D *EffMC2; TH1D *EffData3; TH1D *EffMC3; 
  TH1D *EffData4; TH1D *EffMC4; TH1D *EffData5; TH1D *EffMC5;
  EffData1 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); EffMC1 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); 
  EffData2 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); EffMC2 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); 
  EffData3 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); EffMC3 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt);
  EffData4 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); EffMC4 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); 
  EffData5 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); EffMC5 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt);  
  
  for ( int i = 0; i < 5 ; i++){
    for ( int j = 0; j < 5 ; j++){
      
      effData = fPidTableData->effD(pt[i]+0.1, eta[j]+0.1, 0.);
      effDataErr = fPidTableData->errD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMC = fPidTableMC->effD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMCErr = fPidTableMC->errD(pt[i]+0.1, eta[j]+0.1, 0.);
      cout<< pt[i] << "-" << pt[i+1] << " " << eta[j] << "-" << eta[j+1] <<  "effData = " << effData << "effMC = " << effMC << endl;
      
      if ( j == 0 ){
	EffData1->SetBinContent(i+1,effData);
	EffData1->SetBinError(i+1,effDataErr);
	EffMC1->SetBinContent(i+1,effMC);
	EffMC1->SetBinError(i+1,effMCErr);	
      }
      
      if ( j == 1 ){
	EffData2->SetBinContent(i+1,effData);
	EffData2->SetBinError(i+1,effDataErr);
	EffMC2->SetBinContent(i+1,effMC);
	EffMC2->SetBinError(i+1,effMCErr);	
      }
      
      if ( j == 2 ){
	EffData3->SetBinContent(i+1,effData);
	EffData3->SetBinError(i+1,effDataErr);
	EffMC3->SetBinContent(i+1,effMC);
	EffMC3->SetBinError(i+1,effMCErr);	
      }      
      
      if ( j == 3 ){
	EffData4->SetBinContent(i+1,effData);
	EffData4->SetBinError(i+1,effDataErr);
	EffMC4->SetBinContent(i+1,effMC);
	EffMC4->SetBinError(i+1,effMCErr);	
      }
      
      if ( j == 4 ){
	EffData5->SetBinContent(i+1,effData);
	EffData5->SetBinError(i+1,effDataErr);
	EffMC5->SetBinContent(i+1,effMC);
	EffMC5->SetBinError(i+1,effMCErr);	
      }        
      
    }
  }
  TCanvas *c4 = new TCanvas("c4","c4",1200,600); 
  c4->Divide(3,2);
  c4->cd(1);
  EffData1->SetMinimum(0.2);
  EffData1->SetMaximum(1.05);
  EffData1->SetMarkerStyle(22);
  EffData1->SetMarkerColor(2);
  EffData1->SetLineColor(2);
  EffData1->Draw("pe");
  EffMC1->SetMarkerStyle(23);
  EffMC1->SetMarkerColor(3);
  EffMC1->SetLineColor(3);
  EffMC1->Draw("pesame");  
  legg = new TLegend(0.6,0.4,0.8,0.7);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi T&P Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi T&P MC","p"); legge->SetTextColor(kGreen);
  legg->Draw();
  c4->cd(2);
  EffData2->SetMinimum(0.2);
  EffData2->SetMaximum(1.05);
  EffData2->SetMarkerStyle(22);
  EffData2->SetMarkerColor(2);
  EffData2->SetLineColor(2);
  EffData2->Draw("pe");
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(3);
  EffMC2->SetLineColor(3);
  EffMC2->Draw("pesame");  
  legg = new TLegend(0.6,0.4,0.8,0.7);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffData2,  "J/#psi T&P Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC2,  "J/#psi T&P MC","p"); legge->SetTextColor(kGreen);
  legg->Draw();  
  c4->cd(3);
  EffData3->SetMinimum(0.2);
  EffData3->SetMaximum(1.05);
  EffData3->SetMarkerStyle(22);
  EffData3->SetMarkerColor(2);
  EffData3->SetLineColor(2);
  EffData3->Draw("pe");
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(3);
  EffMC3->SetLineColor(3);
  EffMC3->Draw("pesame");  
  legg = new TLegend(0.6,0.4,0.8,0.7);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffData3,  "J/#psi T&P Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC3,  "J/#psi T&P MC","p"); legge->SetTextColor(kGreen);
  legg->Draw();   
  c4->cd(4);
  EffData4->SetMinimum(0.2);
  EffData4->SetMaximum(1.05);
  EffData4->SetMarkerStyle(22);
  EffData4->SetMarkerColor(2);
  EffData4->SetLineColor(2);
  EffData4->Draw("pe");
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(3);
  EffMC4->SetLineColor(3);
  EffMC4->Draw("pesame");  
  legg = new TLegend(0.6,0.4,0.8,0.7);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffData4,  "J/#psi T&P Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC4,  "J/#psi T&P MC","p"); legge->SetTextColor(kGreen);
  legg->Draw();   
  c4->cd(5);
  EffData5->SetMinimum(0.2);
  EffData5->SetMaximum(1.05);
  EffData5->SetMarkerStyle(22);
  EffData5->SetMarkerColor(2);
  EffData5->SetLineColor(2);
  EffData5->Draw("pe");
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(3);
  EffMC5->SetLineColor(3);
  EffMC5->Draw("pesame");  
  legg = new TLegend(0.6,0.4,0.8,0.7);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffData5,  "J/#psi T&P Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC5,  "J/#psi T&P MC","p"); legge->SetTextColor(kGreen);
  legg->Draw();     
  
}
