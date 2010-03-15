#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TF1.h"
#include "TLegend.h"
#include "TPad.h"
#include "TLatex.h"
#include "TPaveStats.h"

void setMyStyle() {

   gStyle->Reset("Default");
   gStyle->SetCanvasColor(0);
   gStyle->SetPadColor(0);
   gStyle->SetTitleFillColor(10);
   gStyle->SetCanvasBorderMode(0);
   gStyle->SetStatColor(0);
   gStyle->SetPadBorderMode(0);
   gStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
   gStyle->SetPadTickY(1);
   gStyle->SetFrameBorderMode(0);
   gStyle->SetPalette(1);
   
//    gStyle->SetTitleSize(0.06, "XYZ");
//    gStyle->SetLabelSize(0.06, "XYZ");
   
   gStyle->SetStatH(0.18);
   gStyle->SetStatW(0.18);
   
//    gStyle->SetOptStat(kFALSE);
   gStyle->SetOptStat(1111111);
   gStyle->SetOptFit(1);
//    gStyle->SetOptTitle(0);
//    gStyle->SetPadLeftMargin(0.13);
//    gStyle->SetPadRightMargin(0.07);
//    gStyle->SetStatFont(42);
//    gStyle->SetTitleFont(42);
//    gStyle->SetTitleFont(42, "XYZ");
//    gStyle->SetLabelFont(42, "XYZ");
}


void optimize(const string& fFile, const string& fPlot, const double fXmin, const double fXmax, const double fYmin, const double fYmax, const double fYatXmax, const double fYstep, const double fFrac_thresh, const string& fXAxisLabel, const string& fYAxisLabel, const string& fTitle, const string& fName, const int fLogX, const int fLogY, const int fLogZ) {

   vector<double> slopes;
   vector<double> intercepts;
   
   TF1* E_PET = new TF1("E_PET","[0]+[1]*abs(x)+[2]*pow(x,2)",28,42);
   E_PET->SetParameter(0,162.4);
   E_PET->SetParameter(1,-10.19);
   E_PET->SetParameter(2,0.21);
   
   TCanvas *c = new TCanvas("c","",1200,800);
//    TCanvas *c = new TCanvas("c");
   c->cd();
 
   TH2F *h2;
   TF1* f1;
   
   for(int ieta=30; ieta<=41; ieta++ ) {

     cout<<"------------------------------------------------"<<endl;
     cout<<">> ieta: "<<ieta<<endl;
     cout<<"------------------------------------------------"<<endl;
     
     TFile file(fFile.c_str());
     h2 = (TH2F*)file.Get((fPlot+Form("_%i",ieta)).c_str());
     
     double threshold = E_PET->Eval(ieta);
     
     cout<<">> E threshold: "<<threshold<<" GeV"<<endl;
     
     int firstbinx = h2->GetXaxis()->FindBin(threshold);
     int lastbinx = h2->GetXaxis()->GetNbins()+1;
     int firstbiny = 0;
     int lastbiny = h2->GetYaxis()->GetNbins()+1;
     double allRecHits = h2->Integral(firstbinx,lastbinx,firstbiny,lastbiny);
     cout<<">> No. of all RecHits above the energy threshold: "<<allRecHits<<endl;
     
     // Calculate the fraction of flagged RecHits above the energy threshold
     double fraction = 0;
     
     double YatXmax = fYatXmax;
     
     double slope = 0;
     double intercept = 0;
     
     int iteration=1;
     bool optimized = false;
     bool startedbig = false;
     
     do {
     
       double temp_slope = YatXmax/(log(fXmax) - log(threshold));
       double temp_intercept = -temp_slope*log(threshold);
        
       f1 = new TF1("f1","[0]+[1]*log(x)",threshold-15,fXmax);
       f1->SetParameter(0,temp_intercept);
       f1->SetParameter(1,temp_slope);
       
       double flaggedRecHits = 0;
       
       for(int ix=0; ix<=lastbinx; ix++) {
         
         if(ix<firstbinx) continue;
         
         for(int iy=0; iy<=lastbiny; iy++) {
          
           double binx = h2->GetXaxis()->GetBinCenter(ix);
           double biny = h2->GetYaxis()->GetBinCenter(iy);
           
           if(biny>f1->Eval(binx)) continue;
           
           flaggedRecHits+=h2->GetBinContent(ix,iy);
         }
       }
       
       fraction = flaggedRecHits/allRecHits;
       
       cout<<">> Iteration: "<<iteration<<endl;
       cout<<">> YatXmax:"<<YatXmax<<endl;
       cout<<">> No. of flagged RecHits above the energy threshold: "<<flaggedRecHits<<endl;
       cout<<">> Fraction of flagged RecHits above the energy threshold: "<<fraction<<endl;
       
       if(iteration==1 && fraction>fFrac_thresh) {
         startedbig=true;
         YatXmax-=fYstep;
       } else if(iteration==1 && fraction<=fFrac_thresh) {
         YatXmax+=fYstep;
       } else if (iteration>1 && startedbig && fraction>fFrac_thresh) {
         YatXmax-=fYstep;
       } else if (iteration>1 && startedbig && fraction<=fFrac_thresh) {
         optimized=true;
         cout<<">> Optimization completed after "<<iteration<<" iteration(s)"<<endl;
         cout<<">> Optimized YatXmax: "<<YatXmax<<endl;
       } else if (iteration>1 && !startedbig && fraction<=fFrac_thresh) {
         YatXmax+=fYstep;
       } else if (iteration>1 && !startedbig && fraction>fFrac_thresh) {
         optimized=true;
         cout<<">> Optimization completed after "<<iteration<<" iteration(s)"<<endl;
         cout<<">> Optimized YatXmax: "<<(YatXmax-fYstep)<<endl;
       }

       if(!optimized) {
         slope = temp_slope;
         intercept = temp_intercept;
       } else if(optimized && startedbig) {
         slope = temp_slope;
         intercept = temp_intercept;
       }
       
       iteration++;
       
       delete f1;
       
     } while (!optimized);
     
     cout<<">> Optimized cut line: "<<Form("f(x)=%.3f*log(x)%.3f",slope,intercept)<<endl;
     
     slopes.push_back(slope);
     intercepts.push_back(intercept);
     
     h2->GetXaxis()->SetTitle(fXAxisLabel.c_str());
     h2->GetYaxis()->SetTitle(fYAxisLabel.c_str());
     h2->GetXaxis()->SetRangeUser(fXmin,fXmax);
     h2->GetYaxis()->SetRangeUser(fYmin,fYmax);

     h2->Draw("colz");
     
     //update the current pad, needed to modify statboxes
     gPad->Update();
     
     // get the statboxes and set color
     TPaveStats *st1 = (TPaveStats*)h2->GetListOfFunctions()->FindObject("stats");
  //    st1->SetTextColor(kBlack);
  //    st1->SetLineColor(kBlack);
     st1->SetOptStat(111111);

     // set the position of the statboxes
     double x1 = st1->GetX1NDC();
     double x2 = st1->GetX2NDC();
     st1->SetX1NDC(x1-0.08);
     st1->SetX2NDC(x2-0.08);
     gPad->Modified();
     
     f1 = new TF1("f1",Form("%f*log(x)+%f",slope,intercept),threshold-15,fXmax);
     f1->SetLineColor(kRed);
     f1->Draw("same");
     
     TLatex l;
     l.SetTextAlign(12);
     l.SetTextSize(0.04);
     l.SetTextFont(62);
     l.SetNDC();
     l.DrawLatex(0.3,0.9,fTitle.c_str());
     l.DrawLatex(0.6,0.15,Form("f(x)=%.3f*log(x)%.3f",slope,intercept));
     
     if(fLogX) c->SetLogx();
     if(fLogY) c->SetLogy();
     if(fLogZ) c->SetLogz();
     c->SaveAs((fPlot+Form("_%i_",ieta)+fName).c_str());

     delete f1;
   }

   cout<<">> slopes: {";
   
   for(int i=0; i<(int)slopes.size(); i++) {
     cout<<slopes[i];
     if(i<((int)slopes.size()-1)) cout<<",";
     if(i==((int)slopes.size()-1)) cout<<"}"<<endl;
   }

   cout<<">> intercepts: {";
   
   for(int i=0; i<(int)intercepts.size(); i++) {
     cout<<intercepts[i];
     if(i<((int)intercepts.size()-1)) cout<<",";
     if(i==((int)intercepts.size()-1)) cout<<"}"<<endl;
   }

   delete c;
   delete E_PET;
}


void plotGraphs() {

   double ieta[] = {30,31,32,33,34,35,36,37,38,39,40,41};
   double slopes[] = {0.0171519,0.0245339,0.0311146,0.0384983,0.0530911,0.0608012,0.0789118,0.084833,0.0998253,0.118896,0.0913756,0.0589927};
   double intercepts[] = {-0.0655561,-0.0951387,-0.122556,-0.154149,-0.21621,-0.251908,-0.332643,-0.36381,-0.435446,-0.527373,-0.411981,-0.27025};
   
   TCanvas *c = new TCanvas("c","",1200,800);
//    TCanvas *c = new TCanvas("c");
   c->cd();
   
   TGraph* g1 = new TGraph(12,ieta,intercepts);
//    g1->SetMarkerSize(2.);
   g1->SetMarkerStyle(20);
   g1->SetMarkerColor(kRed);
   g1->GetXaxis()->SetTitle("i#eta");
   g1->GetYaxis()->SetTitle("Intercept");
   g1->Draw("AP");

   c->SetGridx();
   c->SetGridy();
//    c->SaveAs("intercepts.png");
   
   delete g1;
   
   g1 = new TGraph(12,ieta,slopes);
   g1->SetMarkerStyle(20);
   g1->SetMarkerColor(kRed);
   g1->SetMaximum(0.16);
   g1->GetXaxis()->SetTitle("i#eta");
   g1->GetYaxis()->SetTitle("Slope");
   g1->Draw("AP");

   TF1* fit = new TF1("fit","pol2",29,39.5);
   
   g1->Fit("fit","R");
   
   c->SetGridx();
   c->SetGridy();
   c->SaveAs("slopes.png");
   
   delete fit;
   delete g1;
   delete c;
}


void plot2D(const string& fFile, const string& fPlot, const double fXmin, const double fXmax, const double fYmin, const double fYmax,const string& fXAxisLabel, const string& fYAxisLabel, const string& fTitle, const string& fName, const int fLogX, const int fLogY, const int fLogZ) {

   TF1* E_PET = new TF1("E_PET","[0]+[1]*abs(x)+[2]*pow(x,2)",28,42);
   E_PET->SetParameter(0,162.4);
   E_PET->SetParameter(1,-10.19);
   E_PET->SetParameter(2,0.21);
   
   TF1* slope_func = new TF1("slope_func","[0]+[1]*abs(x)+[2]*pow(x,2)",28,42);
   slope_func->SetParameter(0,0.3084);
   slope_func->SetParameter(1,-0.02577);
   slope_func->SetParameter(2,0.0005351);
   
   double slopes[] = {0.0171519,0.0245339,0.0311146,0.0384983,0.0530911,0.0608012,0.0789118,0.084833,0.0998253,0.118896,0.0913756,0.0589927};
 
   TCanvas *c = new TCanvas("c","",1200,800);
//    TCanvas *c = new TCanvas("c");
   c->cd();
 
   TH2F *h2;
   TF1* f1;
   
   for(int ieta=29; ieta<=41; ieta++ ) {

     cout<<"------------------------------------------------"<<endl;
     cout<<">> ieta: "<<ieta<<endl;
     cout<<"------------------------------------------------"<<endl;
    
     TFile file(fFile.c_str());
     h2 = (TH2F*)file.Get((fPlot+Form("_%i",ieta)).c_str());
     
     double threshold = E_PET->Eval(ieta);
     
     cout<<">> E threshold: "<<threshold<<" GeV"<<endl;
     
     int firstbinx = h2->GetXaxis()->FindBin(threshold);
     int lastbinx = h2->GetXaxis()->GetNbins()+1;
     int firstbiny = 0;
     int lastbiny = h2->GetYaxis()->GetNbins()+1;
     double allRecHits = h2->Integral(firstbinx,lastbinx,firstbiny,lastbiny);
     cout<<">> No. of all RecHits above the energy threshold: "<<allRecHits<<endl;
     
     double slope = slope_func->Eval(ieta);
     if( abs(ieta)>39 ) slope = slopes[ieta-30];
     double intercept = -slope*log(threshold);
     
     f1 = new TF1("f1","[0]+[1]*log(x)",threshold-15,fXmax);
     f1->SetParameter(0,intercept);
     f1->SetParameter(1,slope);
     f1->SetLineColor(kRed);
     
     double flaggedRecHits = 0;
     
     for(int ix=0; ix<=lastbinx; ix++) {
       
       if(ix<firstbinx) continue;
       
       for(int iy=0; iy<=lastbiny; iy++) {
        
         double binx = h2->GetXaxis()->GetBinCenter(ix);
         double biny = h2->GetYaxis()->GetBinUpEdge(iy);
         
         if(biny>f1->Eval(binx)) continue;
         
         flaggedRecHits+=h2->GetBinContent(ix,iy);
       }
     }
     
     cout<<">> No. of flagged RecHits above the energy threshold: "<<flaggedRecHits<<endl;
     if( allRecHits>0 ) cout<<">> Fraction of flagged RecHits above the energy threshold: "<<flaggedRecHits/allRecHits<<endl;

     h2->GetXaxis()->SetTitle(fXAxisLabel.c_str());
     h2->GetYaxis()->SetTitle(fYAxisLabel.c_str());
     h2->GetXaxis()->SetRangeUser(fXmin,fXmax);
     h2->GetYaxis()->SetRangeUser(fYmin,fYmax);

     h2->Draw("colz");
     if(ieta>29) f1->Draw("same");
     
     //update the current pad, needed to modify statboxes
     gPad->Update();
     
     // get the statboxes and set color
     TPaveStats *st1 = (TPaveStats*)h2->GetListOfFunctions()->FindObject("stats");
  //    st1->SetTextColor(kBlack);
  //    st1->SetLineColor(kBlack);
     st1->SetOptStat(111111);

     // set the position of the statboxes
     double x1 = st1->GetX1NDC();
     double x2 = st1->GetX2NDC();
     st1->SetX1NDC(x1-0.08);
     st1->SetX2NDC(x2-0.08);
     gPad->Modified();
     
     TLatex l;
     l.SetTextAlign(12);
     l.SetTextSize(0.04);
     l.SetTextFont(62);
     l.SetNDC();
     l.DrawLatex(0.3,0.9,fTitle.c_str());
     if(ieta>29) l.DrawLatex(0.6,0.15,Form("f(x)=%.3f*log(x)%.3f",slope,intercept));
     
     if(fLogX) c->SetLogx();
     if(fLogY) c->SetLogy();
     if(fLogZ) c->SetLogz();
     c->SaveAs((fPlot+Form("_%i_",ieta)+fName).c_str());

     delete f1;
   }

   delete c;
}


void makePlots() {
   // turn on/off batch mode
   gROOT->SetBatch(kTRUE);
   // set ROOT style
   setMyStyle();
//    gStyle->SetOptTitle(0);
   //********************************************
   // root files
   //********************************************
   // *** data ***
   string data_900 = "data/output/output_DATA_900GeV_HFPMT_PET.root";
   string data_900_S9oS1 = "data/output/output_DATA_900GeV_HFPMT_S9oS1.root";
   string data_2360 = "data/output/output_DATA_2360GeV_HFPMT_PET.root";
   string data_2360_S9oS1 = "data/output/output_DATA_2360GeV_HFPMT_S9oS1.root";
   // *** MC ***
   string MC_MB_900 = "data/output/output_MC_MB_900GeV_HFPMT_PET.root";
   string MC_MB_2360 = "data/output/output_MC_MB_2360GeV_HFPMT_PET.root";
   string MC_MB_7000 = "data/output/output_MC_MB_7TeV_HFPMT_PET.root";
   string MC_QCD_FlatPt_7000 = "data/output/output_MC_QCD_FlatPt_7TeV_HFPMT_PET.root";
   string MC_SinglePhotons = "data/output/output_MC_SinglePhotonsInHF_HFPMT_PET.root";
   //********************************************
   // make plots
   //********************************************

//    optimize(MC_SinglePhotons, "h2_S9oS1_vs_E_L_ieta", 0.1, 630, -0.1, 0.5, 0.1, 0.001, 0.001, "E [GeV]", "S9/S1", "MC SinglePhotons", "MC_SinglePhotons_opt.png",1,0,1);

//    plotGraphs();

   plot2D(data_900, "h2_S9oS1_vs_E_L_Flagged_ieta", 0.1, 630, -0.1, 0.5, "E [GeV]", "S9/S1", "Collisions  #sqrt{s}=900 GeV", "Data_900GeV.png",1,0,1);
//    plot2D(data_900_S9oS1, "h2_S9oS1_vs_E_L_Flagged_ieta", 0.1, 630, -0.1, 0.5, "E [GeV]", "S9/S1", "Collisions  #sqrt{s}=900 GeV", "Data_900GeV_S9oS1.png",1,0,1);
//    plot2D(data_2360, "h2_S9oS1_vs_E_L_Flagged_ieta", 0.1, 630, -0.1, 0.5, "E [GeV]", "S9/S1", "Collisions  #sqrt{s}=2360 GeV", "Data_2360GeV.png",1,0,0);
//    plot2D(data_2360_S9oS1, "h2_S9oS1_vs_E_L_Flagged_ieta", 0.1, 630, -0.1, 0.5, "E [GeV]", "S9/S1", "Collisions  #sqrt{s}=2360 GeV", "Data_2360GeV_S9oS1.png",1,0,0);
   plot2D(MC_MB_7000, "h2_S9oS1_vs_E_L_ieta", 0.1, 630, -0.1, 0.5, "E [GeV]", "S9/S1", "MC MinBias  #sqrt{s}=7 TeV", "MC_MinBias_7TeV.png",1,0,1);
   plot2D(MC_QCD_FlatPt_7000, "h2_S9oS1_vs_E_L_ieta", 0.1, 630, -0.1, 0.5, "E [GeV]", "S9/S1", "MC QCD FlatPt  #sqrt{s}=7 TeV", "MC_QCD_FlatPt_7TeV.png",1,0,1);
   plot2D(MC_SinglePhotons, "h2_S9oS1_vs_E_L_ieta", 0.1, 630, -0.1, 0.5, "E [GeV]", "S9/S1", "MC SinglePhotons", "MC_SinglePhotons.png",1,0,1);

}
