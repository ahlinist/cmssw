////////////////////////////
// Script to plot sensor efficiency layer by layer
// The input files are the output from TrackingAnalysis/Cosmics/src/HitRes.cc
// Usage in root: root> .x plotHitEffSummary.C
// Original Author:  Keith Ulmer  University of Colorado 
//                   keith.ulmer@colorado.edu
////////////////////////////

{
#include <vector>
  
  gROOT->Reset();
  
  int nLayers = 21;
  float RatioNoBad[nLayers+1];
  float errRatio[nLayers+1];
  float RatioAll[nLayers+1];
  float errRatioAll[nLayers+1];
  float Mod[nLayers+1];
  float ModErr[nLayers+1];
  int cont[nLayers+1];
  
  float SNoBad[nLayers+1],BNoBad[nLayers+1];
  float SAll[nLayers+1],BAll[nLayers+1];
  
  SNoBad[0] = 0;
  BNoBad[0] = 6;
  SAll[0] = 0;
  BAll[0] = 6;
  
  for (Long_t i=1; i< nLayers+1; ++i) {
    if (i==10) i++;
    TString file = TString("hitEff_CRAFT_run69912_layer") + i;
    TFile* f = TFile::Open(file+"_220_ReReco_redotracking/hiteff_CRAFT.root");
    f->cd("anEff");
    traj->Draw("ModIsBad","SiStripQualBad==0","goff");
    SNoBad[i] =  htemp->GetBinContent(1);
    BNoBad[i] =  htemp->GetBinContent(2);
    if ((SNoBad[i]+BNoBad[i]) > 5) {
      float ratio = (SNoBad[i]*1. / (SNoBad[i]+BNoBad[i])*1.);
      RatioNoBad[i] = ratio;
      float deno = (SNoBad[i] + BNoBad[i]) * (SNoBad[i] + BNoBad[i]) * (SNoBad[i] + BNoBad[i]);
      errRatio[i] = sqrt( (SNoBad[i]*BNoBad[i]) / deno*1.);
    } else {
      RatioNoBad[i] = -1.0;
      errRatio[i] = 0.0;
    }
    
    cout << i << " SNoBad " << SNoBad[i] << " BNoBad " << BNoBad[i] << " ratio "  << ratio*100. <<  " +o- " << errRatio[i]*100. << endl;
    Mod[i] = i;
    ModErr[i] = 0.;

    traj->Draw("ModIsBad>>htemp2","","goff");
    SAll[i] =  htemp2->GetBinContent(1);
    BAll[i] =  htemp2->GetBinContent(2);
    if ((SAll[i]+BAll[i]) > 5) {
      float ratio = (SAll[i]*1. / (SAll[i]+BAll[i])*1.);
      RatioAll[i] = ratio;
      float deno = (SAll[i] + BAll[i]) * (SAll[i] + BAll[i]) * (SAll[i] + BAll[i]);
      errRatioAll[i] = sqrt( (SAll[i]*BAll[i]) / deno*1.);
    } else {
      RatioAll[i] = -1.0;
      errRatioAll[i] = 0.0;
    }

    cout << i << " SAll " << SAll[i] << " BAll " << BAll[i] << " ratio "  << ratio*100. <<  " +o- " << errRatioAll[i]*100. << endl;

  }
  
  TCanvas *c7 =new TCanvas("c7"," test ",10,10,800,600);
  c7->SetFillColor(0);
  c7->SetGrid();
  c7->SetBottomMargin(20);
  //c7->GetFrame()->SetFillColor(21);
  //c7->GetFrame()->SetBorderSize(12);
  graphNoBad = new TGraphErrors(nLayers+1,Mod,RatioNoBad,ModErr,errRatio);
  graphNoBad->SetName("graphNoBad");
  graphNoBad->SetTitle(" Hit Efficiency in CRAFT Data Run 69912");
  graphNoBad->GetYaxis()->SetTitle("Efficiency");
  graphNoBad->SetMarkerColor(2);
  graphNoBad->SetMarkerSize(1.2);
  graphNoBad->SetLineColor(2);
  graphNoBad->SetLineWidth(4);
  graphNoBad->SetMarkerStyle(20);
  graphNoBad->SetMinimum(0.80);
  graphNoBad->SetMaximum(1.005);
  graphNoBad->GetXaxis()->SetLimits(1,nLayers);
  for ( int j=1; j<nLayers+1; j++) {
    if (j==10) j++;
    TString label;
    if (j<5) {
      label = TString("TIB ")+j;
    } else if (j>4&&j<11) {
      label = TString("TOB ")+(j-4);
    } else if (j>10&&j<14) {
      label = TString("TID ")+(j-10);
    } else if (j>13) {
      label = TString("TEC ")+(j-13);
    }
    graphNoBad->GetXaxis()->SetBinLabel(j==1?1:(j-1)*(100/(nLayers-1)),label);
  }
  graphNoBad->Draw("AP");




  //create a transparent pad drawn on top of the main pad
  c7->cd();
  TPad *overlay = new TPad("overlay","",0,0,1,1);
  overlay->SetFillStyle(4000);
  overlay->SetFillColor(0);
  overlay->SetFrameFillStyle(4000);
  overlay->Draw();
  overlay->cd();  
  graphAll = new TGraphErrors(nLayers+1,Mod,RatioAll,ModErr,errRatioAll);
  graphAll->SetMarkerColor(1);
  graphAll->SetMarkerSize(1.2);
  graphAll->SetLineColor(1);
  graphAll->SetLineWidth(4);
  graphAll->SetMarkerStyle(21);
  graphAll->SetMinimum(0.80);
  graphAll->SetMaximum(1.005);
  graphAll->SetTitle("");
  graphAll->GetYaxis()->SetTitle("");
  graphAll->GetXaxis()->SetLimits(1,nLayers);
  graphAll->GetXaxis()->SetLabelSize(0);
  graphAll->Draw("AP");

  leg = new TLegend(0.28,0.20,0.64,0.39);
  leg->AddEntry(graphNoBad,"Known bad modules removed","p");
  leg->AddEntry(graphAll,"All modules","p");
  leg->SetTextSize(0.055);
  leg->SetFillColor(0);
  leg->Draw("same");

  //c7->SaveAs("HitEffSummary.png");
  //c7->SaveAs("HitEffSummary.eps");
  //graphAll->Draw("ap,same");
  c7->SaveAs("HitEffSummaryAll.png");
  c7->SaveAs("HitEffSummaryAll.eps");
}
