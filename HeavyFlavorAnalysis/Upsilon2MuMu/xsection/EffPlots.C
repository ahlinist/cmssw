#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/PidTable.hh"
//#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/functions.hh"

void PosVsNeg_MuID(){
  
  // Data -> Pos
  // MC -> Neg
  
  gStyle->SetOptStat(00000000000);
  int Npt(5); int Neta(5);
  double effData(-99); double effMC(-99); double effDataErr(-99); double effMCErr(-99); 
  fPidTableData = new PidTable("../tnp/PidTables/DATA/Jpsi/MuID/CowboyVeto/TrackerMuonArbitrated/PtMmbPos-jpsi.tma.nb.dat");
  fPidTableMC = new PidTable("../tnp/PidTables/DATA/Jpsi/MuID/CowboyVeto/TrackerMuonArbitrated/PtMmbNeg-jpsi.tma.nb.dat");
  
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
	if ( i > 0 ){
	  EffData2->SetBinContent(i+1,effData);
	  EffData2->SetBinError(i+1,effDataErr);
	  EffMC2->SetBinContent(i+1,effMC);
	  EffMC2->SetBinError(i+1,effMCErr);	
	}
      }
      
      if ( j == 2 ){
	if ( i > 0 ){
	  EffData3->SetBinContent(i+1,effData);
	  EffData3->SetBinError(i+1,effDataErr);
	  EffMC3->SetBinContent(i+1,effMC);
	  EffMC3->SetBinError(i+1,effMCErr);	
	}      
      }
      
      if ( j == 3 ){
	if ( i > 0 ){
	  EffData4->SetBinContent(i+1,effData);
	  EffData4->SetBinError(i+1,effDataErr);
	  EffMC4->SetBinContent(i+1,effMC);
	  EffMC4->SetBinError(i+1,effMCErr);	
	}
      }
      
      if ( j == 4 ){
	EffData5->SetBinContent(i+1,effData);
	EffData5->SetBinError(i+1,effDataErr);
	EffMC5->SetBinContent(i+1,effMC);
	EffMC5->SetBinError(i+1,effMCErr);	
      }        
      
    }
  }
  TCanvas *c9 = new TCanvas("c9","c9",1200,600); 
  c9->Divide(3,2);
  c9->cd(1);
  EffData1->SetMinimum(0.2);
  EffData1->SetMaximum(1.05);
  EffData1->SetMarkerStyle(22);
  EffData1->SetMarkerColor(1);
  EffData1->SetLineColor(1);
  EffData1->Draw("pe");
  EffMC1->SetMarkerStyle(23);
  EffMC1->SetMarkerColor(2);
  EffMC1->SetLineColor(2);
  EffMC1->Draw("pesame");  
  legg = new TLegend(0.5,0.2,0.7,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data Pos","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP Data Neg","p"); legge->SetTextColor(kRed);
  legg->Draw();
  c9->cd(2);
  EffData2->SetMinimum(0.2);
  EffData2->SetMaximum(1.05);
  EffData2->SetMarkerStyle(22);
  EffData2->SetMarkerColor(1);
  EffData2->SetLineColor(1);
  EffData2->Draw("pe");
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(2);
  EffMC2->SetLineColor(2);
  EffMC2->Draw("pesame");  
  legg = new TLegend(0.5,0.2,0.7,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffData2,  "J/#psi TNP Data Pos","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(EffMC2,  "J/#psi TNP Data Neg","p"); legge->SetTextColor(kRed);
  legg->Draw();  
  c9->cd(3);
  EffData3->SetMinimum(0.2);
  EffData3->SetMaximum(1.05);
  EffData3->SetMarkerStyle(22);
  EffData3->SetMarkerColor(1);
  EffData3->SetLineColor(1);
  EffData3->Draw("pe");
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(2);
  EffMC3->SetLineColor(2);
  EffMC3->Draw("pesame");  
  legg = new TLegend(0.5,0.2,0.7,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffData3,  "J/#psi TNP Data Pos","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(EffMC3,  "J/#psi TNP Data Neg","p"); legge->SetTextColor(kRed);
  legg->Draw();   
  c9->cd(4);
  EffData4->SetMinimum(0.2);
  EffData4->SetMaximum(1.05);
  EffData4->SetMarkerStyle(22);
  EffData4->SetMarkerColor(1);
  EffData4->SetLineColor(1);
  EffData4->Draw("pe");
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(2);
  EffMC4->SetLineColor(2);
  EffMC4->Draw("pesame");  
  legg = new TLegend(0.5,0.2,0.7,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffData4,  "J/#psi TNP Data Pos","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(EffMC4,  "J/#psi TNP Data Neg","p"); legge->SetTextColor(kRed);
  legg->Draw();   
  c9->cd(5);
  EffData5->SetMinimum(0.2);
  EffData5->SetMaximum(1.05);
  EffData5->SetMarkerStyle(22);
  EffData5->SetMarkerColor(1);
  EffData5->SetLineColor(1);
  EffData5->Draw("pe");
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(2);
  EffMC5->SetLineColor(2);
  EffMC5->Draw("pesame");  
  legg = new TLegend(0.5,0.2,0.7,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffData5,  "J/#psi TNP Data Pos","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(EffMC5,  "J/#psi TNP Data Neg","p"); legge->SetTextColor(kRed);
  legg->Draw();     
  
  TCanvas *c901 = new TCanvas("c901","c901",800,600); 
  EffData1->SetTitle("");
  EffData1->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData1->GetYaxis()->SetTitle("Efficieny");
  EffData1->SetMinimum(0.75);
  EffData1->SetMaximum(1.05);
  EffData1->SetMarkerStyle(22);
  EffData1->SetMarkerColor(1);
  EffData1->SetLineColor(1);
  EffData1->Draw("pe");
  EffMC1->SetMarkerStyle(23);
  EffMC1->SetMarkerColor(2);
  EffMC1->SetLineColor(2);
  EffMC1->Draw("pesame");  
  legg = new TLegend(0.5,0.2,0.7,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data Pos","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP Data Neg","p"); legge->SetTextColor(kRed);
  legg->Draw();
  c901->SaveAs("PosVsNeg_MuID1.pdf");
  
  TCanvas *c902 = new TCanvas("c902","c902",800,600); 
  EffData2->SetTitle("");
  EffData2->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData2->GetYaxis()->SetTitle("Efficieny");
  EffData2->SetMinimum(0.75);
  EffData2->SetMaximum(1.05);
  EffData2->SetMarkerStyle(22);
  EffData2->SetMarkerColor(1);
  EffData2->SetLineColor(1);
  EffData2->Draw("pe");
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(2);
  EffMC2->SetLineColor(2);
  EffMC2->Draw("pesame");  
  legg = new TLegend(0.5,0.2,0.7,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data Pos","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP Data Neg","p"); legge->SetTextColor(kRed);
  legg->Draw();
  c902->SaveAs("PosVsNeg_MuID2.pdf");
  
  TCanvas *c903 = new TCanvas("c903","c903",800,600); 
  EffData3->SetTitle("");
  EffData3->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData3->GetYaxis()->SetTitle("Efficieny");
  EffData3->SetMinimum(0.75);
  EffData3->SetMaximum(1.05);
  EffData3->SetMarkerStyle(22);
  EffData3->SetMarkerColor(1);
  EffData3->SetLineColor(1);
  EffData3->Draw("pe");
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(2);
  EffMC3->SetLineColor(2);
  EffMC3->Draw("pesame");  
  legg = new TLegend(0.5,0.2,0.7,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data Pos","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP Data Neg","p"); legge->SetTextColor(kRed);
  legg->Draw();
  c903->SaveAs("PosVsNeg_MuID3.pdf");
  
  TCanvas *c904 = new TCanvas("c904","c904",800,600); 
  EffData4->SetTitle("");
  EffData4->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData4->GetYaxis()->SetTitle("Efficieny");
  EffData4->SetMinimum(0.75);
  EffData4->SetMaximum(1.05);
  EffData4->SetMarkerStyle(22);
  EffData4->SetMarkerColor(1);
  EffData4->SetLineColor(1);
  EffData4->Draw("pe");
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(2);
  EffMC4->SetLineColor(2);
  EffMC4->Draw("pesame");  
  legg = new TLegend(0.5,0.2,0.7,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data Pos","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP Data Neg","p"); legge->SetTextColor(kRed);
  legg->Draw();
  c904->SaveAs("PosVsNeg_MuID4.pdf");
  
  TCanvas *c905 = new TCanvas("c905","c905",800,600); 
  EffData5->SetTitle("");
  EffData5->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData5->GetYaxis()->SetTitle("Efficieny");
  EffData5->SetMinimum(0.75);
  EffData5->SetMaximum(1.05);
  EffData5->SetMarkerStyle(22);
  EffData5->SetMarkerColor(1);
  EffData5->SetLineColor(1);
  EffData5->Draw("pe");
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(2);
  EffMC5->SetLineColor(2);
  EffMC5->Draw("pesame");  
  legg = new TLegend(0.5,0.2,0.7,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data Pos","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP Data Neg","p"); legge->SetTextColor(kRed);
  legg->Draw();
  c905->SaveAs("PosVsNeg_MuID5.pdf");
  
}

double f_Turnon(double *x, double *par) {
  
  Double_t fitval = par[0] + par[1]*((TMath::Exp(2*(par[2]*(x[0]-par[3])))-1)/(TMath::Exp(2*(par[2]*(x[0]-par[3])))+1));
  return fitval;
    
}



void Trig_DATA_Fit(){
  
  gStyle->SetOptStat(00000000000);
  int Npt(5); int Neta(5);
  double effMC(-99); double effMCErr(-99);  
  fPidTableMC = new PidTable("PtMmbTrigBothv2-jpsi.8ptbin.DATA.dat");
    
  double pt[9] = {3., 4., 5., 6., 8., 10., 14., 20., 50.};
  double eta[6] = {-2.4, -1.2, -0.4, 0.4, 1.2, 2.4};
  
  TH1D *EffMC1; TH1D *EffMC2;  TH1D *EffMC3; TH1D *EffMC4;  TH1D *EffMC5; 
  EffMC1 = new TH1D("TrigEfficiencyPos1", "TrigEfficiencyPos1", 7, pt); 
  EffMC2 = new TH1D("TrigEfficiencyPos2", "TrigEfficiencyPos2", 7, pt); 
  EffMC3 = new TH1D("TrigEfficiencyPos3", "TrigEfficiencyPos3", 7, pt); 
  EffMC4 = new TH1D("TrigEfficiencyPos4", "TrigEfficiencyPos4", 7, pt); 
  EffMC5 = new TH1D("TrigEfficiencyPos5", "TrigEfficiencyPos5", 7, pt);  
  
  f0 = new TF1("f0", f_Turnon, 3., 14., 4);
    
  for ( int i = 0; i < 7 ; i++){
    for ( int j = 0; j < 5 ; j++){
      
      effMC = fPidTableMC->effD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMCErr = fPidTableMC->errD(pt[i]+0.1, eta[j]+0.1, 0.);
            
      if ( j == 0 ){
	EffMC1->SetBinContent(i+1,effMC);
	EffMC1->SetBinError(i+1,effMCErr);
      }
      
      if ( j == 1 ){
	if ( i > 0 ){
	  EffMC2->SetBinContent(i+1,effMC);
	  EffMC2->SetBinError(i+1,effMCErr);
	}
      }
      
      if ( j == 2 ){
	if ( i > 0 ){
	  EffMC3->SetBinContent(i+1,effMC);
	  EffMC3->SetBinError(i+1,effMCErr);
	}      
      }
      
      if ( j == 3 ){
	if ( i > 0 ){
	  EffMC4->SetBinContent(i+1,effMC);
	  EffMC4->SetBinError(i+1,effMCErr);
	}
      }
      
      if ( j == 4 ){
	EffMC5->SetBinContent(i+1,effMC);
	EffMC5->SetBinError(i+1,effMCErr);
      }        
      
    }
  }
  const char* Status; int status(0);
  f0->ReleaseParameter(0); 
  f0->ReleaseParameter(1); 
  f0->ReleaseParameter(2); 
  f0->ReleaseParameter(3);   
  f0->SetParameters(0.5, 0.5, 0.1, 0.1);
  //f0->SetParLimits(0, -2, -1.);
  //f0->FixParameter(0, 0.5);
  //f0->FixParameter(1, 0.5);
  f0->SetParLimits(0, 0., 10.);
  f0->SetParLimits(1, 0.1, 2.5);
  f0->SetParLimits(2, 0., 5.);
  f0->SetParLimits(3, 0., 2.);
  
  EffMC1->Fit(f0);
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC1->Fit(f0);
  }
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC1->Fit(f0);
  }  
  
  EffMC2->Fit(f0);
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC2->Fit(f0);
  } 
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC2->Fit(f0);
  } 
    
  EffMC3->Fit(f0);
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC3->Fit(f0);
  }
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC3->Fit(f0);
  }  
  
  EffMC4->Fit(f0);
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC4->Fit(f0);
  }
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC4->Fit(f0);
  }  
  
  EffMC5->Fit(f0);
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC5->Fit(f0);
  }
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC5->Fit(f0);
  }
  
  
  TCanvas *c8 = new TCanvas("c8","c8",1200,600); 
  c8->Divide(3,2);
  c8->cd(1);
  EffMC1->SetMinimum(0.4);
  EffMC1->SetMaximum(1.05);
  EffMC1->SetMarkerStyle(23);
  EffMC1->SetMarkerColor(1);
  EffMC1->SetLineColor(1);
  EffMC1->Draw("pe");
  legg = new TLegend(0.6,0.3,0.8,0.5);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffMC1,"b = 0.938 #pm 0.008"); legge->SetTextColor(kBlack);
  legg->Draw();
  c8->cd(2);
  EffMC2->SetMinimum(0.4);
  EffMC2->SetMaximum(1.05);
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(1);
  EffMC2->SetLineColor(1);
  EffMC2->Draw("pe"); 
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffMC2,"b = 0.961 #pm 0.021"); legge->SetTextColor(kBlack);
  legg->Draw();  
  c8->cd(3);
  EffMC3->SetMinimum(0.4);
  EffMC3->SetMaximum(1.05);
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(1);
  EffMC3->SetLineColor(1);
  EffMC3->Draw("pe");    
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffMC3,"b = 0.943 #pm 0.015"); legge->SetTextColor(kBlack);
  legg->Draw();   
  c8->cd(4);
  EffMC4->SetMinimum(0.4);
  EffMC4->SetMaximum(1.05);
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(1);
  EffMC4->SetLineColor(1);
  EffMC4->Draw("pe"); 
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffMC4,"b = 0.967 #pm 0.008"); legge->SetTextColor(kBlack);
  legg->Draw();   
  c8->cd(5);
  EffMC5->SetMinimum(0.4);
  EffMC5->SetMaximum(1.05);
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(1);
  EffMC5->SetLineColor(1);
  EffMC5->Draw("pe");
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffMC5,"b = 0.904 #pm 0.083"); legge->SetTextColor(kBlack);
  legg->Draw();     
  
  
  TCanvas *c801 = new TCanvas("c801","c801",800,600); 
  EffMC1->SetTitle("");
  EffMC1->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffMC1->GetYaxis()->SetTitle("Efficieny");
  EffMC1->SetMinimum(0.65);
  EffMC1->SetMaximum(1.05);
  EffMC1->SetMarkerStyle(23);
  EffMC1->SetMarkerColor(1);
  EffMC1->SetLineColor(1);
  EffMC1->Draw("pe");  
  legg = new TLegend(0.6,0.3,0.8,0.5);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffMC1,"b = 0.938 #pm 0.008"); legge->SetTextColor(kBlack);
  legg->Draw();
  c801->SaveAs("TrigFIT1.pdf");
  
  TCanvas *c802 = new TCanvas("c802","c802",800,600); 
  EffMC2->SetTitle("");
  EffMC2->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffMC2->GetYaxis()->SetTitle("Efficieny");
  EffMC2->SetMinimum(0.65);
  EffMC2->SetMaximum(1.05);
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(1);
  EffMC2->SetLineColor(1);
  EffMC2->Draw("pe");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffMC2,"b = 0.961 #pm 0.021"); legge->SetTextColor(kBlack);
  legg->Draw();
  c802->SaveAs("TrigFIT2.pdf");
  
  TCanvas *c803 = new TCanvas("c803","c803",800,600); 
  EffMC3->SetTitle("");
  EffMC3->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffMC3->GetYaxis()->SetTitle("Efficieny");
  EffMC3->SetMinimum(0.65);
  EffMC3->SetMaximum(1.05);
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(1);
  EffMC3->SetLineColor(1);
  EffMC3->Draw("pe");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffMC3,"b = 0.943 #pm 0.015"); legge->SetTextColor(kBlack);
  legg->Draw();
  c803->SaveAs("TrigFIT3.pdf");
  
  TCanvas *c804 = new TCanvas("c804","c804",800,600); 
  EffMC4->SetTitle("");
  EffMC4->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffMC4->GetYaxis()->SetTitle("Efficieny");
  EffMC4->SetMinimum(0.65);
  EffMC4->SetMaximum(1.05);
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(1);
  EffMC4->SetLineColor(1);
  EffMC4->Draw("pe"); 
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffMC4,"b = 0.967 #pm 0.008"); legge->SetTextColor(kBlack);
  legg->Draw();
  c804->SaveAs("TrigFIT4.pdf");
  
  TCanvas *c805 = new TCanvas("c805","c805",800,600); 
  EffMC5->SetTitle("");
  EffMC5->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffMC5->GetYaxis()->SetTitle("Efficieny");
  EffMC5->SetMinimum(0.65);
  EffMC5->SetMaximum(1.05);
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(1);
  EffMC5->SetLineColor(1);
  EffMC5->Draw("pe");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffMC5,"b = 0.904 #pm 0.083"); legge->SetTextColor(kBlack);
  legg->Draw();
  c805->SaveAs("TrigFIT5.pdf");
  
}

void MuID_DATA_Fit(){
  
  gStyle->SetOptStat(00000000000);
  int Npt(5); int Neta(5);
  double effMC(-99); double effMCErr(-99);  
  fPidTableMC = new PidTable("PtMmbMuidBothv2-jpsi.8ptbin.DATA.dat");
  
  double pt[9] = {3., 4., 5., 6., 8., 10., 14., 20., 50.};
  double eta[6] = {-2.4, -1.2, -0.4, 0.4, 1.2, 2.4};
  
  TH1D *EffMC1; TH1D *EffMC2;  TH1D *EffMC3; TH1D *EffMC4;  TH1D *EffMC5; 
  EffMC1 = new TH1D("TrigEfficiencyPos1", "TrigEfficiencyPos1", 7, pt); 
  EffMC2 = new TH1D("TrigEfficiencyPos2", "TrigEfficiencyPos2", 7, pt); 
  EffMC3 = new TH1D("TrigEfficiencyPos3", "TrigEfficiencyPos3", 7, pt); 
  EffMC4 = new TH1D("TrigEfficiencyPos4", "TrigEfficiencyPos4", 7, pt); 
  EffMC5 = new TH1D("TrigEfficiencyPos5", "TrigEfficiencyPos5", 7, pt);  
  
  f0 = new TF1("f0", f_Turnon, 3., 20., 4);
    
  for ( int i = 0; i < 7 ; i++){
    for ( int j = 0; j < 5 ; j++){
      
      effMC = fPidTableMC->effD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMCErr = fPidTableMC->errD(pt[i]+0.1, eta[j]+0.1, 0.);
            
      if ( j == 0 ){
	EffMC1->SetBinContent(i+1,effMC);
	EffMC1->SetBinError(i+1,effMCErr);
      }
      
      if ( j == 1 ){
	//if ( i > 0 ){
	  EffMC2->SetBinContent(i+1,effMC);
	  EffMC2->SetBinError(i+1,effMCErr);
	  //}
      }
      
      if ( j == 2 ){
	//if ( i > 0 ){
	  EffMC3->SetBinContent(i+1,effMC);
	  EffMC3->SetBinError(i+1,effMCErr);
	  //}      
      }
      
      if ( j == 3 ){
	//if ( i > 0 ){
	  EffMC4->SetBinContent(i+1,effMC);
	  EffMC4->SetBinError(i+1,effMCErr);
	  //}
      }
      
      if ( j == 4 ){
	EffMC5->SetBinContent(i+1,effMC);
	EffMC5->SetBinError(i+1,effMCErr);
      }        
      
    }
  }
  const char* Status; int status(0);
  f0->ReleaseParameter(0); 
  f0->ReleaseParameter(1); 
  f0->ReleaseParameter(2); 
  f0->ReleaseParameter(3);   
  f0->SetParameters(0.5, 0.5, 0.1, 0.1);
  //f0->SetParLimits(0, -2, -1.);
  //f0->FixParameter(0, 0.5);
  //f0->FixParameter(1, 0.5);
  f0->SetParLimits(0, 0., 10.);
  f0->SetParLimits(1, 0.1, 2.5);
  f0->SetParLimits(2, 0., 5.);
  f0->SetParLimits(3, 0., 2.);
  
  EffMC1->Fit(f0);
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC1->Fit(f0);
  }
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC1->Fit(f0);
  }  
  
  EffMC2->Fit(f0);
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC2->Fit(f0);
  } 
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC2->Fit(f0);
  } 
    
  EffMC3->Fit(f0);
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC3->Fit(f0);
  }
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC3->Fit(f0);
  }  
  
  EffMC4->Fit(f0);
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC4->Fit(f0);
  }
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC4->Fit(f0);
  }  
  
  EffMC5->Fit(f0);
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC5->Fit(f0);
  }
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC5->Fit(f0);
  }
  
  
  TCanvas *c8 = new TCanvas("c8","c8",1200,600); 
  c8->Divide(3,2);
  c8->cd(1);
  EffMC1->SetMinimum(0.4);
  EffMC1->SetMaximum(1.05);
  EffMC1->SetMarkerStyle(23);
  EffMC1->SetMarkerColor(1);
  EffMC1->SetLineColor(1);
  EffMC1->Draw("pe");
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffMC1,"b = 0.960 #pm 0.006"); legge->SetTextColor(kBlack);
  legg->Draw();
  c8->cd(2);
  EffMC2->SetMinimum(0.4);
  EffMC2->SetMaximum(1.05);
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(1);
  EffMC2->SetLineColor(1);
  EffMC2->Draw("pe"); 
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffMC2,"b = 0.990 #pm 0.003"); legge->SetTextColor(kBlack);
  legg->Draw();  
  c8->cd(3);
  EffMC3->SetMinimum(0.4);
  EffMC3->SetMaximum(1.05);
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(1);
  EffMC3->SetLineColor(1);
  EffMC3->Draw("pe");    
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffMC3,"b = 0.994 #pm 0.003"); legge->SetTextColor(kBlack);
  legg->Draw();   
  c8->cd(4);
  EffMC4->SetMinimum(0.4);
  EffMC4->SetMaximum(1.05);
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(1);
  EffMC4->SetLineColor(1);
  EffMC4->Draw("pe"); 
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffMC4,"b = 0.114 #pm 0.033"); legge->SetTextColor(kBlack);
  legg->Draw();   
  c8->cd(5);
  EffMC5->SetMinimum(0.4);
  EffMC5->SetMaximum(1.05);
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(1);
  EffMC5->SetLineColor(1);
  EffMC5->Draw("pe");
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffMC5,"b = 0.131 #pm 0.078"); legge->SetTextColor(kBlack);
  legg->Draw();     
  
  
  TCanvas *c801 = new TCanvas("c801","c801",800,600); 
  EffMC1->SetTitle("");
  EffMC1->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffMC1->GetYaxis()->SetTitle("Efficieny");
  EffMC1->SetMinimum(0.65);
  EffMC1->SetMaximum(1.05);
  EffMC1->SetMarkerStyle(23);
  EffMC1->SetMarkerColor(1);
  EffMC1->SetLineColor(1);
  EffMC1->Draw("pe");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffMC1,"b = 0.960 #pm 0.006"); legge->SetTextColor(kBlack);
  legg->Draw();
  c801->SaveAs("MuidFIT1.pdf");
  
  TCanvas *c802 = new TCanvas("c802","c802",800,600); 
  EffMC2->SetTitle("");
  EffMC2->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffMC2->GetYaxis()->SetTitle("Efficieny");
  EffMC2->SetMinimum(0.65);
  EffMC2->SetMaximum(1.05);
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(1);
  EffMC2->SetLineColor(1);
  EffMC2->Draw("pe");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffMC2,"b = 0.990 #pm 0.003"); legge->SetTextColor(kBlack);
  legg->Draw();
  c802->SaveAs("MuidFIT2.pdf");
  
  TCanvas *c803 = new TCanvas("c803","c803",800,600); 
  EffMC3->SetTitle("");
  EffMC3->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffMC3->GetYaxis()->SetTitle("Efficieny");
  EffMC3->SetMinimum(0.65);
  EffMC3->SetMaximum(1.05);
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(1);
  EffMC3->SetLineColor(1);
  EffMC3->Draw("pe");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffMC3,"b = 0.994 #pm 0.003"); legge->SetTextColor(kBlack);
  legg->Draw();
  c803->SaveAs("MuidFIT3.pdf");
  
  TCanvas *c804 = new TCanvas("c804","c804",800,600); 
  EffMC4->SetTitle("");
  EffMC4->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffMC4->GetYaxis()->SetTitle("Efficieny");
  EffMC4->SetMinimum(0.65);
  EffMC4->SetMaximum(1.05);
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(1);
  EffMC4->SetLineColor(1);
  EffMC4->Draw("pe"); 
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffMC4,"b = 0.114 #pm 0.033"); legge->SetTextColor(kBlack);
  legg->Draw();
  c804->SaveAs("MuidFIT4.pdf");
  
  TCanvas *c805 = new TCanvas("c805","c805",800,600); 
  EffMC5->SetTitle("");
  EffMC5->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffMC5->GetYaxis()->SetTitle("Efficieny");
  EffMC5->SetMinimum(0.65);
  EffMC5->SetMaximum(1.05);
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(1);
  EffMC5->SetLineColor(1);
  EffMC5->Draw("pe");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffMC5,"b = 0.131 #pm 0.078"); legge->SetTextColor(kBlack);
  legg->Draw();
  c805->SaveAs("MuidFIT5.pdf");
  
}



void Trig_MC_Fit(){
  
  gStyle->SetOptStat(00000000000);
  int Npt(5); int Neta(5);
  double effMC(-99); double effMCErr(-99);  
  //fPidTableMC = new PidTable("../tnp/PidTables/MC/Jpsi/Trig/CowboyVeto/PtMmbPos-jpsi.tma.nb.dat");
  fPidTableMC = new PidTable("PtMmbMuIDBoth-jpsi.8ptbin.MC.dat");
  //double pt[6] = {3., 4., 5., 6., 8., 50.};
  double pt[9] = {3., 4., 5., 6., 8., 10., 14., 20., 50};
  double eta[6] = {-2.4, -1.2, -0.4, 0.4, 1.2, 2.4};
  
  TH1D *EffMC1; TH1D *EffMC2;  TH1D *EffMC3; TH1D *EffMC4;  TH1D *EffMC5; 
  EffMC1 = new TH1D("TrigEfficiencyPos1", "TrigEfficiencyPos1", 7, pt); 
  EffMC2 = new TH1D("TrigEfficiencyPos2", "TrigEfficiencyPos2", 7, pt); 
  EffMC3 = new TH1D("TrigEfficiencyPos3", "TrigEfficiencyPos3", 7, pt); 
  EffMC4 = new TH1D("TrigEfficiencyPos4", "TrigEfficiencyPos4", 7, pt); 
  EffMC5 = new TH1D("TrigEfficiencyPos5", "TrigEfficiencyPos5", 7, pt);  
  
  f0 = new TF1("f0", f_Turnon, 3., 20., 4);
    
  for ( int i = 0; i < 7 ; i++){
    for ( int j = 0; j < 5 ; j++){
      
      effMC = fPidTableMC->effD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMCErr = fPidTableMC->errD(pt[i]+0.1, eta[j]+0.1, 0.);
            
      if ( j == 0 ){
	EffMC1->SetBinContent(i+1,effMC);
	EffMC1->SetBinError(i+1,effMCErr);
      }
      
      if ( j == 1 ){
	if ( i > 0 ){
	  EffMC2->SetBinContent(i+1,effMC);
	  EffMC2->SetBinError(i+1,effMCErr);
	}
      }
      
      if ( j == 2 ){
	if ( i > 0 ){
	  EffMC3->SetBinContent(i+1,effMC);
	  EffMC3->SetBinError(i+1,effMCErr);
	}      
      }
      
      if ( j == 3 ){
	if ( i > 0 ){
	  EffMC4->SetBinContent(i+1,effMC);
	  EffMC4->SetBinError(i+1,effMCErr);
	}
      }
      
      if ( j == 4 ){
	EffMC5->SetBinContent(i+1,effMC);
	EffMC5->SetBinError(i+1,effMCErr);
      }        
      
    }
  }
  const char* Status; int status(0);
  f0->ReleaseParameter(0); 
  f0->ReleaseParameter(1); 
  f0->ReleaseParameter(2); 
  f0->ReleaseParameter(3);   
  f0->SetParameters(0.5, 0.5, 0.1, 0.1);
  //f0->SetParLimits(0, -2, -1.);
  //f0->FixParameter(0, 0.5);
  //f0->FixParameter(1, 0.5);
  f0->SetParLimits(0, 0., 10.);
  f0->SetParLimits(1, 0.1, 2.5);
  f0->SetParLimits(2, 0., 5.);
  f0->SetParLimits(3, 0., 2.);
  EffMC1->Fit(f0);
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC1->Fit(f0);
  }
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC1->Fit(f0);
  }
  
  EffMC2->Fit(f0);
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC2->Fit(f0);
  }  
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC2->Fit(f0);
  }  
  
  EffMC3->Fit(f0);
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC3->Fit(f0);
  }
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC3->Fit(f0);
  }  
  
  EffMC4->Fit(f0);
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC4->Fit(f0);
  }
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC4->Fit(f0);
  } 
  
  EffMC5->Fit(f0);
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC5->Fit(f0);
  }
  Status = gMinuit->fCstatu.Data();
  if ( Status[0] == 'F' || (Status[0] == 'C' && Status[1] == 'A')){ 
    f0->SetParameters( f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2), f0->GetParameter(3));
    EffMC5->Fit(f0);
  }
  
  
  TCanvas *c8 = new TCanvas("c8","c8",1200,600); 
  c8->Divide(3,2);
  c8->cd(1);
  EffMC1->SetMinimum(0.2);
  EffMC1->SetMaximum(1.05);
  EffMC1->SetMarkerStyle(23);
  EffMC1->SetMarkerColor(1);
  EffMC1->SetLineColor(1);
  EffMC1->Draw("pe");
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  c8->cd(2);
  EffMC2->SetMinimum(0.2);
  EffMC2->SetMaximum(1.05);
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(1);
  EffMC2->SetLineColor(1);
  EffMC2->Draw("pe"); 
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffMC2,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legg->Draw();  
  c8->cd(3);
  EffMC3->SetMinimum(0.2);
  EffMC3->SetMaximum(1.05);
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(1);
  EffMC3->SetLineColor(1);
  EffMC3->Draw("pe");    
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffMC3,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legg->Draw();   
  c8->cd(4);
  EffMC4->SetMinimum(0.2);
  EffMC4->SetMaximum(1.05);
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(1);
  EffMC4->SetLineColor(1);
  EffMC4->Draw("pe"); 
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffMC4,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legg->Draw();   
  c8->cd(5);
  EffMC5->SetMinimum(0.2);
  EffMC5->SetMaximum(1.05);
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(1);
  EffMC5->SetLineColor(1);
  EffMC5->Draw("pe");
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffMC5,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legg->Draw();     
  
  
  /*TCanvas *c801 = new TCanvas("c801","c801",800,600); 
  EffMC1->SetTitle("");
  EffMC1->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffMC1->GetYaxis()->SetTitle("Efficieny");
  EffMC1->SetMinimum(0.65);
  EffMC1->SetMaximum(1.05);
  EffMC1->SetMarkerStyle(23);
  EffMC1->SetMarkerColor(1);
  EffMC1->SetLineColor(1);
  EffMC1->Draw("pe");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  f0->SetParameters(-1.2, -0.2);
  EffMC1->Fit(f0);
  c801->SaveAs("TrigPos1FIT.pdf");
  
  TCanvas *c802 = new TCanvas("c802","c802",800,600); 
  EffMC2->SetTitle("");
  EffMC2->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffMC2->GetYaxis()->SetTitle("Efficieny");
  EffMC2->SetMinimum(0.65);
  EffMC2->SetMaximum(1.05);
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(1);
  EffMC2->SetLineColor(1);
  EffMC2->Draw("pe");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffMC2,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  c802->SaveAs("TrigPos2FIT.pdf");
  
  TCanvas *c803 = new TCanvas("c803","c803",800,600); 
  EffMC3->SetTitle("");
  EffMC3->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffMC3->GetYaxis()->SetTitle("Efficieny");
  EffMC3->SetMinimum(0.65);
  EffMC3->SetMaximum(1.05);
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(1);
  EffMC3->SetLineColor(1);
  EffMC3->Draw("pe");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffMC3,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  c803->SaveAs("TrigPos3FIT.pdf");
  
  TCanvas *c804 = new TCanvas("c804","c804",800,600); 
  EffMC4->SetTitle("");
  EffMC4->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffMC4->GetYaxis()->SetTitle("Efficieny");
  EffMC4->SetMinimum(0.65);
  EffMC4->SetMaximum(1.05);
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(1);
  EffMC4->SetLineColor(1);
  EffMC4->Draw("pe"); 
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffMC4,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  c804->SaveAs("TrigPos4FIT.pdf");
  
  TCanvas *c805 = new TCanvas("c805","c805",800,600); 
  EffMC5->SetTitle("");
  EffMC5->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffMC5->GetYaxis()->SetTitle("Efficieny");
  EffMC5->SetMinimum(0.65);
  EffMC5->SetMaximum(1.05);
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(1);
  EffMC5->SetLineColor(1);
  EffMC5->Draw("pe");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffMC5,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  c805->SaveAs("TrigPos5FIT.pdf");

  */  
  
}



void MuID(){
  gStyle->SetOptStat(00000000000);
  int Npt(5); int Neta(5);
  double effData(-99); double effMC(-99); double effDataErr(-99); double effMCErr(-99);  double effMCTruth(-99); double effMCTruthErr(-99); 
  fPidTableData = new PidTable("PtMmbMuidBothv2-jpsi.8ptbin.DATA.dat");
  fPidTableMC = new PidTable("PtMmbMuIDBothv2-jpsi.8ptbin.MC.dat");
  fPidTableMCTruth = new PidTable("PtMctBoth-jpsi.8ptbin.MC.dat");
  
  double pt[9] = {3., 4., 5., 6., 8., 10., 14., 20., 50.};
  double eta[6] = {-2.4, -1.2, -0.4, 0.4, 1.2, 2.4};
  

  TH1D *EffData1; TH1D *EffMC1; TH1D *EffData2; TH1D *EffMC2; TH1D *EffData3; TH1D *EffMC3; TH1D *EffMCTruth4; TH1D *EffMCTruth5;  
  TH1D *EffData4; TH1D *EffMC4; TH1D *EffData5; TH1D *EffMC5; TH1D *EffMCTruth1; TH1D *EffMCTruth2; TH1D *EffMCTruth3; 
  EffData1 = new TH1D("MuID Efficiency", "MuID Efficiency", 7, pt); EffMC1 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 7, pt); 
  EffData2 = new TH1D("MuID Efficiency", "MuID Efficiency", 7, pt); EffMC2 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 7, pt); 
  EffData3 = new TH1D("MuID Efficiency", "MuID Efficiency", 7, pt); EffMC3 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 7, pt);
  EffData4 = new TH1D("MuID Efficiency", "MuID Efficiency", 7, pt); EffMC4 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 7, pt); 
  EffData5 = new TH1D("MuID Efficiency", "MuID Efficiency", 7, pt); EffMC5 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 7, pt);  
  EffMCTruth1 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 7, pt);EffMCTruth2 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 7, pt); 
  EffMCTruth3 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 7, pt);EffMCTruth4 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 7, pt); 
  EffMCTruth5 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 7, pt);

    
  for ( int i = 0; i < 7 ; i++){
    for ( int j = 0; j < 5 ; j++){
      
      effData = fPidTableData->effD(pt[i]+0.1, eta[j]+0.1, 0.);
      effDataErr = fPidTableData->errD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMC = fPidTableMC->effD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMCErr = fPidTableMC->errD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMCTruth = fPidTableMCTruth->effD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMCTruthErr = fPidTableMCTruth->errD(pt[i]+0.1, eta[j]+0.1, 0.);      
      cout<< pt[i] << "-" << pt[i+1] << " " << eta[j] << "-" << eta[j+1] <<  "effData = " << effData << "effMC = " << effMC << endl;
      
      if ( j == 0 ){
	EffData1->SetBinContent(i+1,effData);
	EffData1->SetBinError(i+1,effDataErr);
	EffMC1->SetBinContent(i+1,effMC);
	EffMC1->SetBinError(i+1,effMCErr);
	EffMCTruth1->SetBinContent(i+1,effMCTruth);
	EffMCTruth1->SetBinError(i+1,effMCTruthErr);	
      }
      
      if ( j == 1 ){
	if ( i > 0 ){
	  EffData2->SetBinContent(i+1,effData);
	  EffData2->SetBinError(i+1,effDataErr);
	  EffMC2->SetBinContent(i+1,effMC);
	  EffMC2->SetBinError(i+1,effMCErr);
	  EffMCTruth2->SetBinContent(i+1,effMCTruth);
	  EffMCTruth2->SetBinError(i+1,effMCTruthErr);	  
	}
      }
      
      if ( j == 2 ){
	if ( i > 0 ){
	  EffData3->SetBinContent(i+1,effData);
	  EffData3->SetBinError(i+1,effDataErr);
	  EffMC3->SetBinContent(i+1,effMC);
	  EffMC3->SetBinError(i+1,effMCErr);
	  EffMCTruth3->SetBinContent(i+1,effMCTruth);
	  EffMCTruth3->SetBinError(i+1,effMCTruthErr);
	}      
      }
      
      if ( j == 3 ){
	if ( i > 0 ){
	  EffData4->SetBinContent(i+1,effData);
	  EffData4->SetBinError(i+1,effDataErr);
	  EffMC4->SetBinContent(i+1,effMC);
	  EffMC4->SetBinError(i+1,effMCErr);
	  EffMCTruth4->SetBinContent(i+1,effMCTruth);
	  EffMCTruth4->SetBinError(i+1,effMCTruthErr);
	}
      }
      
      if ( j == 4 ){
	EffData5->SetBinContent(i+1,effData);
	EffData5->SetBinError(i+1,effDataErr);
	EffMC5->SetBinContent(i+1,effMC);
	EffMC5->SetBinError(i+1,effMCErr);
	EffMCTruth5->SetBinContent(i+1,effMCTruth);
	EffMCTruth5->SetBinError(i+1,effMCTruthErr);
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
  EffMC1->SetMarkerColor(1);
  EffMC1->SetLineColor(1);
  EffMC1->Draw("pesame");
  EffMCTruth1->SetMarkerStyle(24);
  EffMCTruth1->SetMarkerColor(4);
  EffMCTruth1->SetLineColor(4);
  EffMCTruth1->Draw("pesame");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(EffMCTruth1,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c1->cd(2);
  EffData2->SetMinimum(0.2);
  EffData2->SetMaximum(1.05);
  EffData2->SetMarkerStyle(22);
  EffData2->SetMarkerColor(2);
  EffData2->SetLineColor(2);
  EffData2->Draw("pe");
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(1);
  EffMC2->SetLineColor(1);
  EffMC2->Draw("pesame"); 
  EffMCTruth2->SetMarkerStyle(24);
  EffMCTruth2->SetMarkerColor(4);
  EffMCTruth2->SetLineColor(4);
  EffMCTruth2->Draw("pesame");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffData2,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC2,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(EffMCTruth2,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();  
  c1->cd(3);
  EffData3->SetMinimum(0.2);
  EffData3->SetMaximum(1.05);
  EffData3->SetMarkerStyle(22);
  EffData3->SetMarkerColor(2);
  EffData3->SetLineColor(2);
  EffData3->Draw("pe");
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(1);
  EffMC3->SetLineColor(1);
  EffMC3->Draw("pesame");    
  EffMCTruth3->SetMarkerStyle(24);
  EffMCTruth3->SetMarkerColor(4);
  EffMCTruth3->SetLineColor(4);
  EffMCTruth3->Draw("pesame");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffData3,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC3,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(EffMCTruth3,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();   
  c1->cd(4);
  EffData4->SetMinimum(0.2);
  EffData4->SetMaximum(1.05);
  EffData4->SetMarkerStyle(22);
  EffData4->SetMarkerColor(2);
  EffData4->SetLineColor(2);
  EffData4->Draw("pe");
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(1);
  EffMC4->SetLineColor(1);
  EffMC4->Draw("pesame"); 
  EffMCTruth4->SetMarkerStyle(24);
  EffMCTruth4->SetMarkerColor(4);
  EffMCTruth4->SetLineColor(4);
  EffMCTruth4->Draw("pesame");   
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffData4,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC4,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(EffMCTruth4,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();   
  c1->cd(5);
  EffData5->SetMinimum(0.2);
  EffData5->SetMaximum(1.05);
  EffData5->SetMarkerStyle(22);
  EffData5->SetMarkerColor(2);
  EffData5->SetLineColor(2);
  EffData5->Draw("pe");
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(1);
  EffMC5->SetLineColor(1);
  EffMC5->Draw("pesame");
  EffMCTruth5->SetMarkerStyle(24);
  EffMCTruth5->SetMarkerColor(4);
  EffMCTruth5->SetLineColor(4);
  EffMCTruth5->Draw("pesame");   
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffData5,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC5,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(EffMCTruth5,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();     
  
  TCanvas *c101 = new TCanvas("c101","c101",800,600); 
  EffData1->SetTitle("");
  EffData1->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData1->GetYaxis()->SetTitle("Efficieny");
  EffData1->SetMinimum(0.75);
  EffData1->SetMaximum(1.05);
  EffData1->SetMarkerStyle(22);
  EffData1->SetMarkerColor(2);
  EffData1->SetLineColor(2);
  EffData1->Draw("pe");
  EffMC1->SetMarkerStyle(23);
  EffMC1->SetMarkerColor(1);
  EffMC1->SetLineColor(1);
  EffMC1->Draw("pesame");  
  EffMCTruth1->SetMarkerStyle(24);
  EffMCTruth1->SetMarkerColor(4);
  EffMCTruth1->SetLineColor(4);
  EffMCTruth1->Draw("pesame"); 
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(EffMCTruth1,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c101->SaveAs("MuID1.pdf");
  
  TCanvas *c102 = new TCanvas("c102","c102",800,600); 
  EffData2->SetTitle("");
  EffData2->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData2->GetYaxis()->SetTitle("Efficieny");
  EffData2->SetMinimum(0.75);
  EffData2->SetMaximum(1.05);
  EffData2->SetMarkerStyle(22);
  EffData2->SetMarkerColor(2);
  EffData2->SetLineColor(2);
  EffData2->Draw("pe");
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(1);
  EffMC2->SetLineColor(1);
  EffMC2->Draw("pesame");  
  EffMCTruth2->SetMarkerStyle(24);
  EffMCTruth2->SetMarkerColor(4);
  EffMCTruth2->SetLineColor(4);
  EffMCTruth2->Draw("pesame");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffData2,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC2,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(EffMCTruth2,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c102->SaveAs("MuID2.pdf");
  
  TCanvas *c103 = new TCanvas("c103","c103",800,600); 
  EffData3->SetTitle("");
  EffData3->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData3->GetYaxis()->SetTitle("Efficieny");
  EffData3->SetMinimum(0.75);
  EffData3->SetMaximum(1.05);
  EffData3->SetMarkerStyle(22);
  EffData3->SetMarkerColor(2);
  EffData3->SetLineColor(2);
  EffData3->Draw("pe");
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(1);
  EffMC3->SetLineColor(1);
  EffMC3->Draw("pesame");  
  EffMCTruth3->SetMarkerStyle(24);
  EffMCTruth3->SetMarkerColor(4);
  EffMCTruth3->SetLineColor(4);
  EffMCTruth3->Draw("pesame");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffData3,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC3,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(EffMCTruth3,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c103->SaveAs("MuID3.pdf");
  
  TCanvas *c104 = new TCanvas("c104","c104",800,600); 
  EffData4->SetTitle("");
  EffData4->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData4->GetYaxis()->SetTitle("Efficieny");
  EffData4->SetMinimum(0.75);
  EffData4->SetMaximum(1.05);
  EffData4->SetMarkerStyle(22);
  EffData4->SetMarkerColor(2);
  EffData4->SetLineColor(2);
  EffData4->Draw("pe");
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(1);
  EffMC4->SetLineColor(1);
  EffMC4->Draw("pesame"); 
  EffMCTruth4->SetMarkerStyle(24);
  EffMCTruth4->SetMarkerColor(4);
  EffMCTruth4->SetLineColor(4);
  EffMCTruth4->Draw("pesame");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffData4,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC4,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(EffMCTruth4,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue); 
  legg->Draw();
  c104->SaveAs("MuID4.pdf");
  
  TCanvas *c105 = new TCanvas("c105","c105",800,600); 
  EffData5->SetTitle("");
  EffData5->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData5->GetYaxis()->SetTitle("Efficieny");
  EffData5->SetMinimum(0.75);
  EffData5->SetMaximum(1.05);
  EffData5->SetMarkerStyle(22);
  EffData5->SetMarkerColor(2);
  EffData5->SetLineColor(2);
  EffData5->Draw("pe");
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(1);
  EffMC5->SetLineColor(1);
  EffMC5->Draw("pesame");  
  EffMCTruth5->SetMarkerStyle(24);
  EffMCTruth5->SetMarkerColor(4);
  EffMCTruth5->SetLineColor(4);
  EffMCTruth5->Draw("pesame");
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legge = legg->AddEntry(EffMCTruth4,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue); 
  legg->Draw();
  c105->SaveAs("MuID5.pdf");
  
  
}

void Trig(){
  gStyle->SetOptStat(00000000000);
  int Npt(5); int Neta(5);
  double effData(-99); double effMC(-99); double effDataErr(-99); double effMCErr(-99); double effMCTruth(-99); double effMCTruthErr(-99);
  fPidTableData = new PidTable("PtMmbTrigBothv2-jpsi.8ptbin.DATA.dat");
  fPidTableMC = new PidTable("PtMmbTrigBothv2-jpsi.8ptbin.MC.dat");
    
  double pt[9] = {3., 4., 5., 6., 8., 10., 14., 20., 50.};
  double eta[6] = {-2.4, -1.2, -0.4, 0.4, 1.2, 2.4};

  TH1D *EffData1; TH1D *EffMC1; TH1D *EffData2; TH1D *EffMC2; TH1D *EffData3; TH1D *EffMC3; 
  TH1D *EffData4; TH1D *EffMC4; TH1D *EffData5; TH1D *EffMC5;
  EffData1 = new TH1D("MuID Efficiency", "MuID Efficiency", 7, pt); EffMC1 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 7, pt); 
  EffData2 = new TH1D("MuID Efficiency", "MuID Efficiency", 7, pt); EffMC2 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 7, pt); 
  EffData3 = new TH1D("MuID Efficiency", "MuID Efficiency", 7, pt); EffMC3 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 7, pt);
  EffData4 = new TH1D("MuID Efficiency", "MuID Efficiency", 7, pt); EffMC4 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 7, pt); 
  EffData5 = new TH1D("MuID Efficiency", "MuID Efficiency", 7, pt); EffMC5 = new TH1D("MuIDEfficiency", "MuIDEfficiency", 7, pt);  
    
  for ( int i = 0; i < 7 ; i++){
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
	if ( i > 0 ){
	  EffData2->SetBinContent(i+1,effData);
	  EffData2->SetBinError(i+1,effDataErr);
	  EffMC2->SetBinContent(i+1,effMC);
	  EffMC2->SetBinError(i+1,effMCErr);
	}
      }
      
      if ( j == 2 ){
	if ( i > 0 ){
	  EffData3->SetBinContent(i+1,effData);
	  EffData3->SetBinError(i+1,effDataErr);
	  EffMC3->SetBinContent(i+1,effMC);
	  EffMC3->SetBinError(i+1,effMCErr);
	}      
      }
      
      if ( j == 3 ){
	if ( i > 0 ){
	  EffData4->SetBinContent(i+1,effData);
	  EffData4->SetBinError(i+1,effDataErr);
	  EffMC4->SetBinContent(i+1,effMC);
	  EffMC4->SetBinError(i+1,effMCErr);
	}
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
  EffMC1->SetMarkerColor(1);
  EffMC1->SetLineColor(1);
  EffMC1->Draw("pesame"); 
  legg = new TLegend(0.4,0.2,0.6,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  c2->cd(2);
  EffData2->SetMinimum(0.2);
  EffData2->SetMaximum(1.05);
  EffData2->SetMarkerStyle(22);
  EffData2->SetMarkerColor(2);
  EffData2->SetLineColor(2);
  EffData2->Draw("pe");
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(1);
  EffMC2->SetLineColor(1);
  EffMC2->Draw("pesame");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffData2,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC2,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legg->Draw();  
  c2->cd(3);
  EffData3->SetMinimum(0.2);
  EffData3->SetMaximum(1.05);
  EffData3->SetMarkerStyle(22);
  EffData3->SetMarkerColor(2);
  EffData3->SetLineColor(2);
  EffData3->Draw("pe");
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(1);
  EffMC3->SetLineColor(1);
  EffMC3->Draw("pesame"); 
  legg = new TLegend(0.6,0.15,0.8,0.35);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffData3,  "J/#psi T&P Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC3,  "J/#psi T&P MC","p"); legge->SetTextColor(kBlack);
  legg->Draw();   
  c2->cd(4);
  EffData4->SetMinimum(0.2);
  EffData4->SetMaximum(1.05);
  EffData4->SetMarkerStyle(22);
  EffData4->SetMarkerColor(2);
  EffData4->SetLineColor(2);
  EffData4->Draw("pe");
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(1);
  EffMC4->SetLineColor(1);
  EffMC4->Draw("pesame");  
  legg = new TLegend(0.6,0.15,0.8,0.35);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffData4,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC4,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legg->Draw();   
  c2->cd(5);
  EffData5->SetMinimum(0.2);
  EffData5->SetMaximum(1.05);
  EffData5->SetMarkerStyle(22);
  EffData5->SetMarkerColor(2);
  EffData5->SetLineColor(2);
  EffData5->Draw("pe");
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(1);
  EffMC5->SetLineColor(1);
  EffMC5->Draw("pesame"); 
  legg = new TLegend(0.6,0.4,0.8,0.6);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffData5,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC5,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legg->Draw();     
  
  TCanvas *c201 = new TCanvas("c201","c201",800,600); 
  EffData1->SetTitle("");
  EffData1->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData1->GetYaxis()->SetTitle("Efficieny");
  EffData1->SetMinimum(0.65);
  EffData1->SetMaximum(1.05);
  EffData1->SetMarkerStyle(22);
  EffData1->SetMarkerColor(2);
  EffData1->SetLineColor(2);
  EffData1->Draw("pe");
  EffMC1->SetMarkerStyle(23);
  EffMC1->SetMarkerColor(1);
  EffMC1->SetLineColor(1);
  EffMC1->Draw("pesame");  
  legg = new TLegend(0.4,0.2,0.6,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  c201->SaveAs("Trig1.pdf");
  
  TCanvas *c202 = new TCanvas("c202","c202",800,600); 
  EffData2->SetTitle("");
  EffData2->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData2->GetYaxis()->SetTitle("Efficieny");
  EffData2->SetMinimum(0.65);
  EffData2->SetMaximum(1.05);
  EffData2->SetMarkerStyle(22);
  EffData2->SetMarkerColor(2);
  EffData2->SetLineColor(2);
  EffData2->Draw("pe");
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(1);
  EffMC2->SetLineColor(1);
  EffMC2->Draw("pesame");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  c202->SaveAs("Trig2.pdf");
  
  TCanvas *c203 = new TCanvas("c203","c203",800,600); 
  EffData3->SetTitle("");
  EffData3->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData3->GetYaxis()->SetTitle("Efficieny");
  EffData3->SetMinimum(0.65);
  EffData3->SetMaximum(1.05);
  EffData3->SetMarkerStyle(22);
  EffData3->SetMarkerColor(2);
  EffData3->SetLineColor(2);
  EffData3->Draw("pe");
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(1);
  EffMC3->SetLineColor(1);
  EffMC3->Draw("pesame");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  c203->SaveAs("Trig3.pdf");
  
  TCanvas *c204 = new TCanvas("c204","c204",800,600); 
  EffData4->SetTitle("");
  EffData4->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData4->GetYaxis()->SetTitle("Efficieny");
  EffData4->SetMinimum(0.65);
  EffData4->SetMaximum(1.05);
  EffData4->SetMarkerStyle(22);
  EffData4->SetMarkerColor(2);
  EffData4->SetLineColor(2);
  EffData4->Draw("pe");
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(1);
  EffMC4->SetLineColor(1);
  EffMC4->Draw("pesame"); 
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  c204->SaveAs("Trig4.pdf");
  
  TCanvas *c205 = new TCanvas("c205","c205",800,600); 
  EffData5->SetTitle("");
  EffData5->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData5->GetYaxis()->SetTitle("Efficieny");
  EffData5->SetMinimum(0.65);
  EffData5->SetMaximum(1.05);
  EffData5->SetMarkerStyle(22);
  EffData5->SetMarkerColor(2);
  EffData5->SetLineColor(2);
  EffData5->Draw("pe");
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(1);
  EffMC5->SetLineColor(1);
  EffMC5->Draw("pesame");  
  legg = new TLegend(0.4,0.2,0.6,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  legg->Draw();
  c205->SaveAs("Trig5.pdf");

}

void Trig_Pos(){
  gStyle->SetOptStat(00000000000);
  int Npt(5); int Neta(5);
  double effData(-99); double effMC(-99); double effDataErr(-99); double effMCErr(-99);  double effMCTruth(-99); double effMCTruthErr(-99); 
  fPidTableData = new PidTable("../tnp/PidTables/DATA/Jpsi/Trig/MuOnia/CowboyVeto/PtMmbPos-jpsi.tma.nb.dat");
  fPidTableMC = new PidTable("../tnp/PidTables/MC/Jpsi/Trig/CowboyVeto/PtMmbPos-jpsi.tma.nb.dat");
  fPidTableMCTruth = new PidTable("../tnp/PidTables/MC/Jpsi/Trig/MCTruth/PtMctPos-jpsi.dat");
  double pt[6] = {3., 4., 5., 6., 8., 50.};
  double eta[6] = {-2.4, -1.2, -0.4, 0.4, 1.2, 2.4};
  

  TH1D *EffData1; TH1D *EffMC1; TH1D *EffData2; TH1D *EffMC2; TH1D *EffData3; TH1D *EffMC3; TH1D *EffMCTruth4; TH1D *EffMCTruth5;  
  TH1D *EffData4; TH1D *EffMC4; TH1D *EffData5; TH1D *EffMC5; TH1D *EffMCTruth1; TH1D *EffMCTruth2; TH1D *EffMCTruth3; 
  EffData1 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); EffMC1 = new TH1D("TrigEfficiency", "TrigEfficiency", 5, pt); 
  EffData2 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); EffMC2 = new TH1D("TrigEfficiency", "TrigEfficiency", 5, pt); 
  EffData3 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); EffMC3 = new TH1D("TrigEfficiency", "TrigEfficiency", 5, pt);
  EffData4 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); EffMC4 = new TH1D("TrigEfficiency", "TrigEfficiency", 5, pt); 
  EffData5 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); EffMC5 = new TH1D("TrigEfficiency", "TrigEfficiency", 5, pt);  
  EffMCTruth1 = new TH1D("TrigEfficiency", "TrigEfficiency", 5, pt);EffMCTruth2 = new TH1D("TrigEfficiency", "TrigEfficiency", 5, pt); 
  EffMCTruth3 = new TH1D("TrigEfficiency", "TrigEfficiency", 5, pt);EffMCTruth4 = new TH1D("TrigEfficiency", "TrigEfficiency", 5, pt); 
  EffMCTruth5 = new TH1D("TrigEfficiency", "TrigEfficiency", 5, pt);

    
  for ( int i = 0; i < 5 ; i++){
    for ( int j = 0; j < 5 ; j++){
      
      effData = fPidTableData->effD(pt[i]+0.1, eta[j]+0.1, 0.);
      effDataErr = fPidTableData->errD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMC = fPidTableMC->effD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMCErr = fPidTableMC->errD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMCTruth = fPidTableMCTruth->effD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMCTruthErr = fPidTableMCTruth->errD(pt[i]+0.1, eta[j]+0.1, 0.);      
      cout<< pt[i] << "-" << pt[i+1] << " " << eta[j] << "-" << eta[j+1] <<  "effData = " << effData << "effMC = " << effMC << endl;
      
      if ( j == 0 ){
	EffData1->SetBinContent(i+1,effData);
	EffData1->SetBinError(i+1,effDataErr);
	EffMC1->SetBinContent(i+1,effMC);
	EffMC1->SetBinError(i+1,effMCErr);
	EffMCTruth1->SetBinContent(i+1,effMCTruth);
	EffMCTruth1->SetBinError(i+1,effMCTruthErr);	
      }
      
      if ( j == 1 ){
	if ( i > 0 ){
	  EffData2->SetBinContent(i+1,effData);
	  EffData2->SetBinError(i+1,effDataErr);
	  EffMC2->SetBinContent(i+1,effMC);
	  EffMC2->SetBinError(i+1,effMCErr);
	  EffMCTruth2->SetBinContent(i+1,effMCTruth);
	  EffMCTruth2->SetBinError(i+1,effMCTruthErr);	  
	}
      }
      
      if ( j == 2 ){
	if ( i > 0 ){
	  EffData3->SetBinContent(i+1,effData);
	  EffData3->SetBinError(i+1,effDataErr);
	  EffMC3->SetBinContent(i+1,effMC);
	  EffMC3->SetBinError(i+1,effMCErr);
	  EffMCTruth3->SetBinContent(i+1,effMCTruth);
	  EffMCTruth3->SetBinError(i+1,effMCTruthErr);
	}      
      }
      
      if ( j == 3 ){
	if ( i > 0 ){
	  EffData4->SetBinContent(i+1,effData);
	  EffData4->SetBinError(i+1,effDataErr);
	  EffMC4->SetBinContent(i+1,effMC);
	  EffMC4->SetBinError(i+1,effMCErr);
	  EffMCTruth4->SetBinContent(i+1,effMCTruth);
	  EffMCTruth4->SetBinError(i+1,effMCTruthErr);
	}
      }
      
      if ( j == 4 ){
	EffData5->SetBinContent(i+1,effData);
	EffData5->SetBinError(i+1,effDataErr);
	EffMC5->SetBinContent(i+1,effMC);
	EffMC5->SetBinError(i+1,effMCErr);
	EffMCTruth5->SetBinContent(i+1,effMCTruth);
	EffMCTruth5->SetBinError(i+1,effMCTruthErr);
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
  EffMC1->SetMarkerColor(1);
  EffMC1->SetLineColor(1);
  EffMC1->Draw("pesame");
  EffMCTruth1->SetMarkerStyle(24);
  EffMCTruth1->SetMarkerColor(4);
  EffMCTruth1->SetLineColor(4);
  //EffMCTruth1->Draw("pesame");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  //legge = legg->AddEntry(EffMCTruth1,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c3->cd(2);
  EffData2->SetMinimum(0.2);
  EffData2->SetMaximum(1.05);
  EffData2->SetMarkerStyle(22);
  EffData2->SetMarkerColor(2);
  EffData2->SetLineColor(2);
  EffData2->Draw("pe");
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(1);
  EffMC2->SetLineColor(1);
  EffMC2->Draw("pesame"); 
  EffMCTruth2->SetMarkerStyle(24);
  EffMCTruth2->SetMarkerColor(4);
  EffMCTruth2->SetLineColor(4);
  //EffMCTruth2->Draw("pesame");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffData2,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC2,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  //legge = legg->AddEntry(EffMCTruth2,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();  
  c3->cd(3);
  EffData3->SetMinimum(0.2);
  EffData3->SetMaximum(1.05);
  EffData3->SetMarkerStyle(22);
  EffData3->SetMarkerColor(2);
  EffData3->SetLineColor(2);
  EffData3->Draw("pe");
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(1);
  EffMC3->SetLineColor(1);
  EffMC3->Draw("pesame");    
  EffMCTruth3->SetMarkerStyle(24);
  EffMCTruth3->SetMarkerColor(4);
  EffMCTruth3->SetLineColor(4);
  //EffMCTruth3->Draw("pesame");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffData3,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC3,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  //legge = legg->AddEntry(EffMCTruth3,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();   
  c3->cd(4);
  EffData4->SetMinimum(0.2);
  EffData4->SetMaximum(1.05);
  EffData4->SetMarkerStyle(22);
  EffData4->SetMarkerColor(2);
  EffData4->SetLineColor(2);
  EffData4->Draw("pe");
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(1);
  EffMC4->SetLineColor(1);
  EffMC4->Draw("pesame"); 
  EffMCTruth4->SetMarkerStyle(24);
  EffMCTruth4->SetMarkerColor(4);
  EffMCTruth4->SetLineColor(4);
  //EffMCTruth4->Draw("pesame");   
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffData4,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC4,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  //legge = legg->AddEntry(EffMCTruth4,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();   
  c3->cd(5);
  EffData5->SetMinimum(0.2);
  EffData5->SetMaximum(1.05);
  EffData5->SetMarkerStyle(22);
  EffData5->SetMarkerColor(2);
  EffData5->SetLineColor(2);
  EffData5->Draw("pe");
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(1);
  EffMC5->SetLineColor(1);
  EffMC5->Draw("pesame");
  EffMCTruth5->SetMarkerStyle(24);
  EffMCTruth5->SetMarkerColor(4);
  EffMCTruth5->SetLineColor(4);
  //EffMCTruth5->Draw("pesame");   
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffData5,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC5,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  //legge = legg->AddEntry(EffMCTruth5,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();     
  
  TCanvas *c301 = new TCanvas("c301","c301",800,600); 
  EffData1->SetTitle("");
  EffData1->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData1->GetYaxis()->SetTitle("Efficieny");
  EffData1->SetMinimum(0.65);
  EffData1->SetMaximum(1.05);
  EffData1->SetMarkerStyle(22);
  EffData1->SetMarkerColor(2);
  EffData1->SetLineColor(2);
  EffData1->Draw("pe");
  EffMC1->SetMarkerStyle(23);
  EffMC1->SetMarkerColor(1);
  EffMC1->SetLineColor(1);
  EffMC1->Draw("pesame");  
  EffMCTruth1->SetMarkerStyle(24);
  EffMCTruth1->SetMarkerColor(4);
  EffMCTruth1->SetLineColor(4);
  //EffMCTruth1->Draw("pesame"); 
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  //legge = legg->AddEntry(EffMCTruth1,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c301->SaveAs("TrigPos1.pdf");
  
  TCanvas *c302 = new TCanvas("c302","c302",800,600); 
  EffData2->SetTitle("");
  EffData2->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData2->GetYaxis()->SetTitle("Efficieny");
  EffData2->SetMinimum(0.65);
  EffData2->SetMaximum(1.05);
  EffData2->SetMarkerStyle(22);
  EffData2->SetMarkerColor(2);
  EffData2->SetLineColor(2);
  EffData2->Draw("pe");
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(1);
  EffMC2->SetLineColor(1);
  EffMC2->Draw("pesame");  
  EffMCTruth2->SetMarkerStyle(24);
  EffMCTruth2->SetMarkerColor(4);
  EffMCTruth2->SetLineColor(4);
  //EffMCTruth2->Draw("pesame");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffData2,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC2,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  //legge = legg->AddEntry(EffMCTruth2,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c302->SaveAs("TrigPos2.pdf");
  
  TCanvas *c303 = new TCanvas("c303","c303",800,600); 
  EffData3->SetTitle("");
  EffData3->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData3->GetYaxis()->SetTitle("Efficieny");
  EffData3->SetMinimum(0.65);
  EffData3->SetMaximum(1.05);
  EffData3->SetMarkerStyle(22);
  EffData3->SetMarkerColor(2);
  EffData3->SetLineColor(2);
  EffData3->Draw("pe");
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(1);
  EffMC3->SetLineColor(1);
  EffMC3->Draw("pesame");  
  EffMCTruth3->SetMarkerStyle(24);
  EffMCTruth3->SetMarkerColor(4);
  EffMCTruth3->SetLineColor(4);
  //EffMCTruth3->Draw("pesame");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffData3,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC3,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  //legge = legg->AddEntry(EffMCTruth3,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c303->SaveAs("TrigPos3.pdf");
  
  TCanvas *c304 = new TCanvas("c304","c304",800,600); 
  EffData4->SetTitle("");
  EffData4->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData4->GetYaxis()->SetTitle("Efficieny");
  EffData4->SetMinimum(0.65);
  EffData4->SetMaximum(1.05);
  EffData4->SetMarkerStyle(22);
  EffData4->SetMarkerColor(2);
  EffData4->SetLineColor(2);
  EffData4->Draw("pe");
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(1);
  EffMC4->SetLineColor(1);
  EffMC4->Draw("pesame"); 
  EffMCTruth4->SetMarkerStyle(24);
  EffMCTruth4->SetMarkerColor(4);
  EffMCTruth4->SetLineColor(4);
  //EffMCTruth4->Draw("pesame");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffData4,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC4,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  //legge = legg->AddEntry(EffMCTruth4,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue); 
  legg->Draw();
  c304->SaveAs("TrigPos4.pdf");
  
  TCanvas *c305 = new TCanvas("c305","c305",800,600); 
  EffData5->SetTitle("");
  EffData5->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData5->GetYaxis()->SetTitle("Efficieny");
  EffData5->SetMinimum(0.65);
  EffData5->SetMaximum(1.05);
  EffData5->SetMarkerStyle(22);
  EffData5->SetMarkerColor(2);
  EffData5->SetLineColor(2);
  EffData5->Draw("pe");
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(1);
  EffMC5->SetLineColor(1);
  EffMC5->Draw("pesame");  
  EffMCTruth5->SetMarkerStyle(24);
  EffMCTruth5->SetMarkerColor(4);
  EffMCTruth5->SetLineColor(4);
  //EffMCTruth5->Draw("pesame");
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  //legge = legg->AddEntry(EffMCTruth4,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue); 
  legg->Draw();
  c305->SaveAs("TrigPos5.pdf");
  
  
}

void Trig_Neg(){
  gStyle->SetOptStat(00000000000);
  int Npt(5); int Neta(5);
  double effData(-99); double effMC(-99); double effDataErr(-99); double effMCErr(-99); double effMCTruth(-99); double effMCTruthErr(-99);
  fPidTableData = new PidTable("../tnp/PidTables/DATA/Jpsi/Trig/MuOnia/CowboyVeto/PtMmbNeg-jpsi.tma.nb.dat");
  fPidTableMC = new PidTable("../tnp/PidTables/MC/Jpsi/Trig/CowboyVeto/PtMmbNeg-jpsi.tma.nb.dat");
  fPidTableMCTruth = new PidTable("../tnp/PidTables/MC/Jpsi/Trig/MCTruth/PtMctNeg-jpsi.dat");
  double pt[6] = {3., 4., 5., 6., 8., 50.};
  double eta[6] = {-2.4, -1.2, -0.4, 0.4, 1.2, 2.4};

  TH1D *EffData1; TH1D *EffMC1; TH1D *EffData2; TH1D *EffMC2; TH1D *EffData3; TH1D *EffMC3; 
  TH1D *EffData4; TH1D *EffMC4; TH1D *EffData5; TH1D *EffMC5;
  EffData1 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); EffMC1 = new TH1D("TrigEfficiency", "TrigEfficiency", 5, pt); 
  EffData2 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); EffMC2 = new TH1D("TrigEfficiency", "TrigEfficiency", 5, pt); 
  EffData3 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); EffMC3 = new TH1D("TrigEfficiency", "TrigEfficiency", 5, pt);
  EffData4 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); EffMC4 = new TH1D("TrigEfficiency", "TrigEfficiency", 5, pt); 
  EffData5 = new TH1D("Trig Efficiency", "Trig Efficiency", 5, pt); EffMC5 = new TH1D("TrigEfficiency", "TrigEfficiency", 5, pt);  
  EffMCTruth1 = new TH1D("TrigEfficiency", "TrigEfficiency", 5, pt);EffMCTruth2 = new TH1D("TrigEfficiency", "TrigEfficiency", 5, pt); 
  EffMCTruth3 = new TH1D("TrigEfficiency", "TrigEfficiency", 5, pt);EffMCTruth4 = new TH1D("TrigEfficiency", "TrigEfficiency", 5, pt); 
  EffMCTruth5 = new TH1D("TrigEfficiency", "TrigEfficiency", 5, pt);
  
  
  for ( int i = 0; i < 5 ; i++){
    for ( int j = 0; j < 5 ; j++){
      
      effData = fPidTableData->effD(pt[i]+0.1, eta[j]+0.1, 0.);
      effDataErr = fPidTableData->errD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMC = fPidTableMC->effD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMCErr = fPidTableMC->errD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMCTruth = fPidTableMCTruth->effD(pt[i]+0.1, eta[j]+0.1, 0.);
      effMCTruthErr = fPidTableMCTruth->errD(pt[i]+0.1, eta[j]+0.1, 0.);
      cout<< pt[i] << "-" << pt[i+1] << " " << eta[j] << "-" << eta[j+1] <<  "effData = " << effData << "effMC = " << effMC << endl;
      
      if ( j == 0 ){
	EffData1->SetBinContent(i+1,effData);
	EffData1->SetBinError(i+1,effDataErr);
	EffMC1->SetBinContent(i+1,effMC);
	EffMC1->SetBinError(i+1,effMCErr);
	EffMCTruth1->SetBinContent(i+1,effMCTruth);
	EffMCTruth1->SetBinError(i+1,effMCTruthErr);
      }
      
      if ( j == 1 ){
	if ( i > 0 ){
	  EffData2->SetBinContent(i+1,effData);
	  EffData2->SetBinError(i+1,effDataErr);
	  EffMC2->SetBinContent(i+1,effMC);
	  EffMC2->SetBinError(i+1,effMCErr);
	  EffMCTruth2->SetBinContent(i+1,effMCTruth);
	  EffMCTruth2->SetBinError(i+1,effMCTruthErr);
	}
      }
      
      if ( j == 2 ){
	if ( i > 0 ){
	  EffData3->SetBinContent(i+1,effData);
	  EffData3->SetBinError(i+1,effDataErr);
	  EffMC3->SetBinContent(i+1,effMC);
	  EffMC3->SetBinError(i+1,effMCErr);
	  EffMCTruth3->SetBinContent(i+1,effMCTruth);
	  EffMCTruth3->SetBinError(i+1,effMCTruthErr);
	}      
      }
      
      if ( j == 3 ){
	if ( i > 0 ){
	  EffData4->SetBinContent(i+1,effData);
	  EffData4->SetBinError(i+1,effDataErr);
	  EffMC4->SetBinContent(i+1,effMC);
	  EffMC4->SetBinError(i+1,effMCErr);
	  EffMCTruth4->SetBinContent(i+1,effMCTruth);
	  EffMCTruth4->SetBinError(i+1,effMCTruthErr);
	}
      }
      
      if ( j == 4 ){
	EffData5->SetBinContent(i+1,effData);
	EffData5->SetBinError(i+1,effDataErr);
	EffMC5->SetBinContent(i+1,effMC);
	EffMC5->SetBinError(i+1,effMCErr);
	EffMCTruth5->SetBinContent(i+1,effMCTruth);
	EffMCTruth5->SetBinError(i+1,effMCTruthErr);
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
  EffMC1->SetMarkerColor(1);
  EffMC1->SetLineColor(1);
  EffMC1->Draw("pesame"); 
  EffMCTruth1->SetMarkerStyle(24);
  EffMCTruth1->SetMarkerColor(4);
  EffMCTruth1->SetLineColor(4);
  //EffMCTruth1->Draw("pesame");
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  //legge = legg->AddEntry(EffMCTruth1,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c4->cd(2);
  EffData2->SetMinimum(0.2);
  EffData2->SetMaximum(1.05);
  EffData2->SetMarkerStyle(22);
  EffData2->SetMarkerColor(2);
  EffData2->SetLineColor(2);
  EffData2->Draw("pe");
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(1);
  EffMC2->SetLineColor(1);
  EffMC2->Draw("pesame");  
  EffMCTruth2->SetMarkerStyle(24);
  EffMCTruth2->SetMarkerColor(4);
  EffMCTruth2->SetLineColor(4);
  //EffMCTruth2->Draw("pesame");
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffData2,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC2,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  //legge = legg->AddEntry(EffMCTruth2,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();  
  c4->cd(3);
  EffData3->SetMinimum(0.2);
  EffData3->SetMaximum(1.05);
  EffData3->SetMarkerStyle(22);
  EffData3->SetMarkerColor(2);
  EffData3->SetLineColor(2);
  EffData3->Draw("pe");
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(1);
  EffMC3->SetLineColor(1);
  EffMC3->Draw("pesame"); 
  EffMCTruth3->SetMarkerStyle(24);
  EffMCTruth3->SetMarkerColor(4);
  EffMCTruth3->SetLineColor(4);
  //EffMCTruth3->Draw("pesame"); 
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffData3,  "J/#psi T&P Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC3,  "J/#psi T&P MC","p"); legge->SetTextColor(kBlack);
  //legge = legg->AddEntry(EffMCTruth3,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();   
  c4->cd(4);
  EffData4->SetMinimum(0.2);
  EffData4->SetMaximum(1.05);
  EffData4->SetMarkerStyle(22);
  EffData4->SetMarkerColor(2);
  EffData4->SetLineColor(2);
  EffData4->Draw("pe");
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(1);
  EffMC4->SetLineColor(1);
  EffMC4->Draw("pesame");  
  EffMCTruth4->SetMarkerStyle(24);
  EffMCTruth4->SetMarkerColor(4);
  EffMCTruth4->SetLineColor(4);
  //EffMCTruth4->Draw("pesame");   
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffData4,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC4,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  //legge = legg->AddEntry(EffMCTruth4,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();   
  c4->cd(5);
  EffData5->SetMinimum(0.2);
  EffData5->SetMaximum(1.05);
  EffData5->SetMarkerStyle(22);
  EffData5->SetMarkerColor(2);
  EffData5->SetLineColor(2);
  EffData5->Draw("pe");
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(1);
  EffMC5->SetLineColor(1);
  EffMC5->Draw("pesame"); 
  EffMCTruth5->SetMarkerStyle(24);
  EffMCTruth5->SetMarkerColor(4);
  EffMCTruth5->SetLineColor(4);
  //EffMCTruth5->Draw("pesame"); 
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffData5,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC5,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  //legge = legg->AddEntry(EffMCTruth5,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();     
  
  
  TCanvas *c401 = new TCanvas("c401","c401",800,600); 
  EffData1->SetTitle("");
  EffData1->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData1->GetYaxis()->SetTitle("Efficieny");
  EffData1->SetMinimum(0.65);
  EffData1->SetMaximum(1.05);
  EffData1->SetMarkerStyle(22);
  EffData1->SetMarkerColor(2);
  EffData1->SetLineColor(2);
  EffData1->Draw("pe");
  EffMC1->SetMarkerStyle(23);
  EffMC1->SetMarkerColor(1);
  EffMC1->SetLineColor(1);
  EffMC1->Draw("pesame");  
  EffMCTruth1->SetMarkerStyle(24);
  EffMCTruth1->SetMarkerColor(4);
  EffMCTruth1->SetLineColor(4);
  //EffMCTruth1->Draw("pesame"); 
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  //legge = legg->AddEntry(EffMCTruth1,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c401->SaveAs("TrigNeg1.pdf");
  
  TCanvas *c402 = new TCanvas("c402","c402",800,600); 
  EffData2->SetTitle("");
  EffData2->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData2->GetYaxis()->SetTitle("Efficieny");
  EffData2->SetMinimum(0.65);
  EffData2->SetMaximum(1.05);
  EffData2->SetMarkerStyle(22);
  EffData2->SetMarkerColor(2);
  EffData2->SetLineColor(2);
  EffData2->Draw("pe");
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(1);
  EffMC2->SetLineColor(1);
  EffMC2->Draw("pesame");  
  EffMCTruth2->SetMarkerStyle(24);
  EffMCTruth2->SetMarkerColor(4);
  EffMCTruth2->SetLineColor(4);
  //EffMCTruth2->Draw("pesame");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  //legge = legg->AddEntry(EffMCTruth2,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c402->SaveAs("TrigNeg2.pdf");
  
  TCanvas *c403 = new TCanvas("c403","c403",800,600); 
  EffData3->SetTitle("");
  EffData3->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData3->GetYaxis()->SetTitle("Efficieny");
  EffData3->SetMinimum(0.65);
  EffData3->SetMaximum(1.05);
  EffData3->SetMarkerStyle(22);
  EffData3->SetMarkerColor(2);
  EffData3->SetLineColor(2);
  EffData3->Draw("pe");
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(1);
  EffMC3->SetLineColor(1);
  EffMC3->Draw("pesame");  
  EffMCTruth3->SetMarkerStyle(24);
  EffMCTruth3->SetMarkerColor(4);
  EffMCTruth3->SetLineColor(4);
  //EffMCTruth3->Draw("pesame");  
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  //legge = legg->AddEntry(EffMCTruth3,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c403->SaveAs("TrigNeg3.pdf");
  
  TCanvas *c404 = new TCanvas("c404","c404",800,600); 
  EffData4->SetTitle("");
  EffData4->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData4->GetYaxis()->SetTitle("Efficieny");
  EffData4->SetMinimum(0.65);
  EffData4->SetMaximum(1.05);
  EffData4->SetMarkerStyle(22);
  EffData4->SetMarkerColor(2);
  EffData4->SetLineColor(2);
  EffData4->Draw("pe");
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(1);
  EffMC4->SetLineColor(1);
  EffMC4->Draw("pesame"); 
  EffMCTruth4->SetMarkerStyle(24);
  EffMCTruth4->SetMarkerColor(4);
  EffMCTruth4->SetLineColor(4);
  //EffMCTruth4->Draw("pesame"); 
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  //legge = legg->AddEntry(EffMCTruth4,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c404->SaveAs("TrigNeg4.pdf");
  
  TCanvas *c405 = new TCanvas("c405","c405",800,600); 
  EffData5->SetTitle("");
  EffData5->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData5->GetYaxis()->SetTitle("Efficieny");
  EffData5->SetMinimum(0.65);
  EffData5->SetMaximum(1.05);
  EffData5->SetMarkerStyle(22);
  EffData5->SetMarkerColor(2);
  EffData5->SetLineColor(2);
  EffData5->Draw("pe");
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(1);
  EffMC5->SetLineColor(1);
  EffMC5->Draw("pesame");  
  EffMCTruth5->SetMarkerStyle(24);
  EffMCTruth5->SetMarkerColor(4);
  EffMCTruth5->SetLineColor(4);
  //EffMCTruth5->Draw("pesame");
  legg = new TLegend(0.6,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffData1,  "J/#psi TNP Data","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi TNP MC","p"); legge->SetTextColor(kBlack);
  //legge = legg->AddEntry(EffMCTruth4,  "J/#psi MCTruth","p"); legge->SetTextColor(kBlue); 
  legg->Draw();
  c405->SaveAs("TrigNeg5.pdf");
  
  
}

void TrigCompare_Pos(){
  gStyle->SetOptStat(00000000000);
  int Npt(5); int Neta(5);
  double effData(-99); double effMC(-99); double effDataErr(-99); double effMCErr(-99); 
  fPidTableData = new PidTable("../tnp/PidTables/DATA/Jpsi/Trig/MuOnia/CowboyVeto/PtMmbPos-jpsi.tma.nb.dat");
  fPidTableMC = new PidTable("../tnp/PidTables/DATA/Jpsi/Trig/MuOnia/CowboyVeto/PtMmbPos-jpsi.runbp1.tma.nb.dat");
    
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
	if ( i > 0 ){
	  EffData2->SetBinContent(i+1,effData);
	  EffData2->SetBinError(i+1,effDataErr);
	  EffMC2->SetBinContent(i+1,effMC);
	  EffMC2->SetBinError(i+1,effMCErr);
	}	
      }
      
      if ( j == 2 ){
	if ( i > 0 ){
	  EffData3->SetBinContent(i+1,effData);
	  EffData3->SetBinError(i+1,effDataErr);
	  EffMC3->SetBinContent(i+1,effMC);
	  EffMC3->SetBinError(i+1,effMCErr);
	}	
      }      
      
      if ( j == 3 ){
	if ( i > 0 ){
	  EffData4->SetBinContent(i+1,effData);
	  EffData4->SetBinError(i+1,effDataErr);
	  EffMC4->SetBinContent(i+1,effMC);
	  EffMC4->SetBinError(i+1,effMCErr);
	}
      }
      
      if ( j == 4 ){
	EffData5->SetBinContent(i+1,effData);
	EffData5->SetBinError(i+1,effDataErr);
	EffMC5->SetBinContent(i+1,effMC);
	EffMC5->SetBinError(i+1,effMCErr);	
      }        
      
    }
  }
  TCanvas *c5 = new TCanvas("c5","c5",1200,600); 
  c5->Divide(3,2);
  c5->cd(1);
  EffData1->SetMinimum(0.2);
  EffData1->SetMaximum(1.05);
  EffData1->SetMarkerStyle(22);
  EffData1->SetMarkerColor(2);
  EffData1->SetLineColor(2);
  EffData1->Draw("pe");
  EffMC1->SetMarkerStyle(23);
  EffMC1->SetMarkerColor(4);
  EffMC1->SetLineColor(4);
  EffMC1->Draw("pesame");  
  legg = new TLegend(0.25,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi->#mu#mu HLT_DoubleMu0_Quarkonium_v1","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi->#mu#mu HLT_DoubleMu0","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c5->cd(2);
  EffData2->SetMinimum(0.2);
  EffData2->SetMaximum(1.05);
  EffData2->SetMarkerStyle(22);
  EffData2->SetMarkerColor(2);
  EffData2->SetLineColor(2);
  EffData2->Draw("pe");
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(4);
  EffMC2->SetLineColor(4);
  EffMC2->Draw("pesame");  
  legg = new TLegend(0.25,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffData2,  "J/#psi->#mu#mu HLT_DoubleMu0_Quarkonium_v1","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC2,  "J/#psi->#mu#mu HLT_DoubleMu0","p"); legge->SetTextColor(kBlue);
  legg->Draw();  
  c5->cd(3);
  EffData3->SetMinimum(0.2);
  EffData3->SetMaximum(1.05);
  EffData3->SetMarkerStyle(22);
  EffData3->SetMarkerColor(2);
  EffData3->SetLineColor(2);
  EffData3->Draw("pe");
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(4);
  EffMC3->SetLineColor(4);
  EffMC3->Draw("pesame");  
  legg = new TLegend(0.25,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffData3,  "J/#psi->#mu#mu HLT_DoubleMu0_Quarkonium_v1","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC3,  "J/#psi->#mu#mu HLT_DoubleMu0","p"); legge->SetTextColor(kBlue);
  legg->Draw();   
  c5->cd(4);
  EffData4->SetMinimum(0.2);
  EffData4->SetMaximum(1.05);
  EffData4->SetMarkerStyle(22);
  EffData4->SetMarkerColor(2);
  EffData4->SetLineColor(2);
  EffData4->Draw("pe");
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(4);
  EffMC4->SetLineColor(4);
  EffMC4->Draw("pesame");  
  legg = new TLegend(0.25,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffData4,  "J/#psi->#mu#mu HLT_DoubleMu0_Quarkonium_v1","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC4,  "J/#psi->#mu#mu HLT_DoubleMu0","p"); legge->SetTextColor(kBlue);
  legg->Draw();   
  c5->cd(5);
  EffData5->SetMinimum(0.2);
  EffData5->SetMaximum(1.05);
  EffData5->SetMarkerStyle(22);
  EffData5->SetMarkerColor(2);
  EffData5->SetLineColor(2);
  EffData5->Draw("pe");
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(4);
  EffMC5->SetLineColor(4);
  EffMC5->Draw("pesame");  
  legg = new TLegend(0.25,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffData5,  "J/#psi->#mu#mu HLT_DoubleMu0_Quarkonium_v1","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC5,  "J/#psi->#mu#mu HLT_DoubleMu0","p"); legge->SetTextColor(kBlue);
  legg->Draw();     
  
  
  TCanvas *c501 = new TCanvas("c501","c501",800,600); 
  EffData1->SetTitle("");
  EffData1->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData1->GetYaxis()->SetTitle("Efficieny");
  EffData1->SetMinimum(0.6);
  EffData1->SetMaximum(1.05);
  EffData1->SetMarkerStyle(22);
  EffData1->SetMarkerColor(2);
  EffData1->SetLineColor(2);
  EffData1->Draw("pe");
  EffMC1->SetMarkerStyle(23);
  EffMC1->SetMarkerColor(4);
  EffMC1->SetLineColor(4);
  EffMC1->Draw("pesame");  
  legg = new TLegend(0.25,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi->#mu#mu HLT_DoubleMu0_Quarkonium_v1","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi->#mu#mu HLT_DoubleMu0","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c501->SaveAs("TrigComPos1.pdf");
  
  TCanvas *c502 = new TCanvas("c502","c502",800,600); 
  EffData2->SetTitle("");
  EffData2->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData2->GetYaxis()->SetTitle("Efficieny");
  EffData2->SetMinimum(0.6);
  EffData2->SetMaximum(1.05);
  EffData2->SetMarkerStyle(22);
  EffData2->SetMarkerColor(2);
  EffData2->SetLineColor(2);
  EffData2->Draw("pe");
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(4);
  EffMC2->SetLineColor(4);
  EffMC2->Draw("pesame");  
  legg = new TLegend(0.25,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffData1,  "J/#psi->#mu#mu HLT_DoubleMu0_Quarkonium_v1","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi->#mu#mu HLT_DoubleMu0","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c502->SaveAs("TrigComPos2.pdf");
  
  TCanvas *c503 = new TCanvas("c503","c503",800,600); 
  EffData3->SetTitle("");
  EffData3->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData3->GetYaxis()->SetTitle("Efficieny");
  EffData3->SetMinimum(0.6);
  EffData3->SetMaximum(1.05);
  EffData3->SetMarkerStyle(22);
  EffData3->SetMarkerColor(2);
  EffData3->SetLineColor(2);
  EffData3->Draw("pe");
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(4);
  EffMC3->SetLineColor(4);
  EffMC3->Draw("pesame");  
  legg = new TLegend(0.25,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffData1,  "J/#psi->#mu#mu HLT_DoubleMu0_Quarkonium_v1","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi->#mu#mu HLT_DoubleMu0","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c503->SaveAs("TrigComPos3.pdf");
  
  TCanvas *c504 = new TCanvas("c504","c504",800,600); 
  EffData4->SetTitle("");
  EffData4->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData4->GetYaxis()->SetTitle("Efficieny");
  EffData4->SetMinimum(0.6);
  EffData4->SetMaximum(1.05);
  EffData4->SetMarkerStyle(22);
  EffData4->SetMarkerColor(2);
  EffData4->SetLineColor(2);
  EffData4->Draw("pe");
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(4);
  EffMC4->SetLineColor(4);
  EffMC4->Draw("pesame");  
  legg = new TLegend(0.25,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi->#mu#mu HLT_DoubleMu0_Quarkonium_v1","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi->#mu#mu HLT_DoubleMu0","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c504->SaveAs("TrigComPos4.pdf");
  
  TCanvas *c505 = new TCanvas("c505","c505",800,600); 
  EffData5->SetTitle("");
  EffData5->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData5->GetYaxis()->SetTitle("Efficieny");
  EffData5->SetMinimum(0.6);
  EffData5->SetMaximum(1.05);
  EffData5->SetMarkerStyle(22);
  EffData5->SetMarkerColor(2);
  EffData5->SetLineColor(2);
  EffData5->Draw("pe");
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(4);
  EffMC5->SetLineColor(4);
  EffMC5->Draw("pesame");  
  legg = new TLegend(0.25,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffData1,  "J/#psi->#mu#mu HLT_DoubleMu0_Quarkonium_v1","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi->#mu#mu HLT_DoubleMu0","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c505->SaveAs("TrigComPos5.pdf");
  
  
}

void TrigCompare_Neg(){
  gStyle->SetOptStat(00000000000);
  int Npt(5); int Neta(5);
  double effData(-99); double effMC(-99); double effDataErr(-99); double effMCErr(-99); 
  fPidTableData = new PidTable("../tnp/PidTables/DATA/Jpsi/Trig/MuOnia/CowboyVeto/PtMmbNeg-jpsi.tma.nb.dat");
  fPidTableMC = new PidTable("../tnp/PidTables/DATA/Jpsi/Trig/MuOnia/CowboyVeto/PtMmbNeg-jpsi.runbp1.tma.nb.dat");
    
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
	if ( i > 0 ){
	  EffData2->SetBinContent(i+1,effData);
	  EffData2->SetBinError(i+1,effDataErr);
	  EffMC2->SetBinContent(i+1,effMC);
	  EffMC2->SetBinError(i+1,effMCErr);
	}	
      }
      
      if ( j == 2 ){
	if ( i > 0 ){
	  EffData3->SetBinContent(i+1,effData);
	  EffData3->SetBinError(i+1,effDataErr);
	  EffMC3->SetBinContent(i+1,effMC);
	  EffMC3->SetBinError(i+1,effMCErr);
	}	
      }      
      
      if ( j == 3 ){
	if ( i > 0 ){
	  EffData4->SetBinContent(i+1,effData);
	  EffData4->SetBinError(i+1,effDataErr);
	  EffMC4->SetBinContent(i+1,effMC);
	  EffMC4->SetBinError(i+1,effMCErr);
	}
      }
      
      if ( j == 4 ){
	EffData5->SetBinContent(i+1,effData);
	EffData5->SetBinError(i+1,effDataErr);
	EffMC5->SetBinContent(i+1,effMC);
	EffMC5->SetBinError(i+1,effMCErr);	
      }        
      
    }
  }
  TCanvas *c6 = new TCanvas("c6","c6",1200,600); 
  c6->Divide(3,2);
  c6->cd(1);
  EffData1->SetMinimum(0.2);
  EffData1->SetMaximum(1.05);
  EffData1->SetMarkerStyle(22);
  EffData1->SetMarkerColor(2);
  EffData1->SetLineColor(2);
  EffData1->Draw("pe");
  EffMC1->SetMarkerStyle(23);
  EffMC1->SetMarkerColor(4);
  EffMC1->SetLineColor(4);
  EffMC1->Draw("pesame");  
  legg = new TLegend(0.25,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi->#mu#mu HLT_DoubleMu0_Quarkonium_v1","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi->#mu#mu HLT_DoubleMu0","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c6->cd(2);
  EffData2->SetMinimum(0.2);
  EffData2->SetMaximum(1.05);
  EffData2->SetMarkerStyle(22);
  EffData2->SetMarkerColor(2);
  EffData2->SetLineColor(2);
  EffData2->Draw("pe");
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(4);
  EffMC2->SetLineColor(4);
  EffMC2->Draw("pesame");  
  legg = new TLegend(0.25,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffData2,  "J/#psi->#mu#mu HLT_DoubleMu0_Quarkonium_v1","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC2,  "J/#psi->#mu#mu HLT_DoubleMu0","p"); legge->SetTextColor(kBlue);
  legg->Draw();  
  c6->cd(3);
  EffData3->SetMinimum(0.2);
  EffData3->SetMaximum(1.05);
  EffData3->SetMarkerStyle(22);
  EffData3->SetMarkerColor(2);
  EffData3->SetLineColor(2);
  EffData3->Draw("pe");
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(4);
  EffMC3->SetLineColor(4);
  EffMC3->Draw("pesame");  
  legg = new TLegend(0.25,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffData3,  "J/#psi->#mu#mu HLT_DoubleMu0_Quarkonium_v1","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC3,  "J/#psi->#mu#mu HLT_DoubleMu0","p"); legge->SetTextColor(kBlue);
  legg->Draw();   
  c6->cd(4);
  EffData4->SetMinimum(0.2);
  EffData4->SetMaximum(1.05);
  EffData4->SetMarkerStyle(22);
  EffData4->SetMarkerColor(2);
  EffData4->SetLineColor(2);
  EffData4->Draw("pe");
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(4);
  EffMC4->SetLineColor(4);
  EffMC4->Draw("pesame");  
  legg = new TLegend(0.25,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffData4,  "J/#psi->#mu#mu HLT_DoubleMu0_Quarkonium_v1","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC4,  "J/#psi->#mu#mu HLT_DoubleMu0","p"); legge->SetTextColor(kBlue);
  legg->Draw();   
  c6->cd(5);
  EffData5->SetMinimum(0.2);
  EffData5->SetMaximum(1.05);
  EffData5->SetMarkerStyle(22);
  EffData5->SetMarkerColor(2);
  EffData5->SetLineColor(2);
  EffData5->Draw("pe");
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(4);
  EffMC5->SetLineColor(4);
  EffMC5->Draw("pesame");  
  legg = new TLegend(0.25,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffData5,  "J/#psi->#mu#mu HLT_DoubleMu0_Quarkonium_v1","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC5,  "J/#psi->#mu#mu HLT_DoubleMu0","p"); legge->SetTextColor(kBlue);
  legg->Draw();     
  
  
  TCanvas *c601 = new TCanvas("c601","c601",800,600); 
  EffData1->SetTitle("");
  EffData1->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData1->GetYaxis()->SetTitle("Efficieny");
  EffData1->SetMinimum(0.6);
  EffData1->SetMaximum(1.05);
  EffData1->SetMarkerStyle(22);
  EffData1->SetMarkerColor(2);
  EffData1->SetLineColor(2);
  EffData1->Draw("pe");
  EffMC1->SetMarkerStyle(23);
  EffMC1->SetMarkerColor(4);
  EffMC1->SetLineColor(4);
  EffMC1->Draw("pesame");  
  legg = new TLegend(0.25,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("  -2.4 < #eta^{#mu} < -1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi->#mu#mu HLT_DoubleMu0_Quarkonium_v1","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi->#mu#mu HLT_DoubleMu0","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c601->SaveAs("TrigComNeg1.pdf");
  
  TCanvas *c602 = new TCanvas("c602","c602",800,600); 
  EffData2->SetTitle("");
  EffData2->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData2->GetYaxis()->SetTitle("Efficieny");
  EffData2->SetMinimum(0.6);
  EffData2->SetMaximum(1.05);
  EffData2->SetMarkerStyle(22);
  EffData2->SetMarkerColor(2);
  EffData2->SetLineColor(2);
  EffData2->Draw("pe");
  EffMC2->SetMarkerStyle(23);
  EffMC2->SetMarkerColor(4);
  EffMC2->SetLineColor(4);
  EffMC2->Draw("pesame");  
  legg = new TLegend(0.25,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("  -1.2 < #eta^{#mu} < -0.4");
  legge = legg->AddEntry(EffData1,  "J/#psi->#mu#mu HLT_DoubleMu0_Quarkonium_v1","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi->#mu#mu HLT_DoubleMu0","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c602->SaveAs("TrigComNeg2.pdf");
  
  TCanvas *c603 = new TCanvas("c603","c603",800,600); 
  EffData3->SetTitle("");
  EffData3->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData3->GetYaxis()->SetTitle("Efficieny");
  EffData3->SetMinimum(0.6);
  EffData3->SetMaximum(1.05);
  EffData3->SetMarkerStyle(22);
  EffData3->SetMarkerColor(2);
  EffData3->SetLineColor(2);
  EffData3->Draw("pe");
  EffMC3->SetMarkerStyle(23);
  EffMC3->SetMarkerColor(4);
  EffMC3->SetLineColor(4);
  EffMC3->Draw("pesame");  
  legg = new TLegend(0.25,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("  -0.4 < #eta^{#mu} < 0.4");
  legge = legg->AddEntry(EffData1,  "J/#psi->#mu#mu HLT_DoubleMu0_Quarkonium_v1","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi->#mu#mu HLT_DoubleMu0","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c603->SaveAs("TrigComNeg3.pdf");
  
  TCanvas *c604 = new TCanvas("c604","c604",800,600); 
  EffData4->SetTitle("");
  EffData4->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData4->GetYaxis()->SetTitle("Efficieny");
  EffData4->SetMinimum(0.6);
  EffData4->SetMaximum(1.05);
  EffData4->SetMarkerStyle(22);
  EffData4->SetMarkerColor(2);
  EffData4->SetLineColor(2);
  EffData4->Draw("pe");
  EffMC4->SetMarkerStyle(23);
  EffMC4->SetMarkerColor(4);
  EffMC4->SetLineColor(4);
  EffMC4->Draw("pesame");  
  legg = new TLegend(0.25,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("  0.4 < #eta^{#mu} < 1.2");
  legge = legg->AddEntry(EffData1,  "J/#psi->#mu#mu HLT_DoubleMu0_Quarkonium_v1","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi->#mu#mu HLT_DoubleMu0","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c604->SaveAs("TrigComNeg4.pdf");
  
  TCanvas *c605 = new TCanvas("c605","c605",800,600); 
  EffData5->SetTitle("");
  EffData5->GetXaxis()->SetTitle("Probe p_{T} [GeV/c]");
  EffData5->GetYaxis()->SetTitle("Efficieny");
  EffData5->SetMinimum(0.6);
  EffData5->SetMaximum(1.05);
  EffData5->SetMarkerStyle(22);
  EffData5->SetMarkerColor(2);
  EffData5->SetLineColor(2);
  EffData5->Draw("pe");
  EffMC5->SetMarkerStyle(23);
  EffMC5->SetMarkerColor(4);
  EffMC5->SetLineColor(4);
  EffMC5->Draw("pesame");  
  legg = new TLegend(0.25,0.2,0.8,0.4);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04); legg->SetTextFont(132); 
  legg->SetHeader("  1.2 < #eta^{#mu} < 2.4");
  legge = legg->AddEntry(EffData1,  "J/#psi->#mu#mu HLT_DoubleMu0_Quarkonium_v1","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(EffMC1,  "J/#psi->#mu#mu HLT_DoubleMu0","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  c605->SaveAs("TrigComNeg5.pdf");
    
}

