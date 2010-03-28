using namespace std;
#include "tdrstyle.C"

void plotDataMC(){
   
  // specify the DATA/MC root file!!!
  // --------------------------------------------------------------------------
  TFile *data = TFile::Open("../../rootNtupleAnalyzer/data/output/v22NEWEEST.root");
  TFile *mc = TFile::Open("../../rootNtupleAnalyzer/data/output/siini.root");
 //   TFile *data = TFile::Open("./datasumB.root");
//    TFile *mc = TFile::Open("./mcsumB.root");

  char *legDataTitle = "Data";
  char *legMCTitle = "Simulation";
  TString dataset="";
  TString overallTitle="CMS preliminary 2009";
  TString CMen="#sqrt{s}=900GeV";
  double rebin=4.;
  //---------------------------------------------------------------------------
  setTDRStyle();
  gROOT->ForceStyle();
  TIter next (data->GetListOfKeys ());

  TString name;
  TRegexp jpt("JPT");  
  TRegexp pf("PF");
  int colour;
  TRegexp twodim("map");
  //non log plots 
  TRegexp eta("eta");
  TRegexp phi("phi");
  TRegexp hits("hits");


  int islog=1;

  for (TKey* key = 0; (key = (TKey *) next());) {
    //determine wether it is a Calo/JPT or PF plots to make
    name=key->GetName();
    if(name.Index(twodim)>-1) continue; //dont want comparisons for 2D histos
    colour=5;
    if(name.Index(jpt)>-1) colour=896;
    if(name.Index(pf)>-1) colour=38;
    islog=1;
    if(name.Index(eta)>-1||name.Index(phi)>-1||name.Index(hits)>-1) islog=0;
    TH1D * h_data= data->Get(key->GetName());
    TH1D * h_mc=mc->Get(key->GetName());; 
    // only for filled histos
    if( h_mc->GetEntries()!=0) {
    TCanvas *c = new TCanvas("c","",600,600); 
    double scalefactor= h_data->Integral()/h_mc->Integral();
    h_mc->Scale(scalefactor);
    //    h_mc->Rebin(rebin);
    //    h_data->Rebin(rebin);
    h_mc->SetMaximum(1.5*h_data->GetMaximum());
    h_mc->SetFillColor(colour);
    h_data->SetMarkerStyle(20);
    //    h_data->SetTitle(overallTitle);
    //    h_mc->SetTitle(overallTitle);
    std::cout << "Plotting: " << key->GetName()  <<  std::endl;
    h_mc->SetMinimum(0.01);
    h_data->SetMinimum(0.01);
    //draw title
    MoveStatsAndDraw(h_data , h_mc, legDataTitle, legMCTitle, overallTitle, CMen, islog, colour, false);
    char histname [256];
    sprintf (histname, "%s.gif", key->GetName());
    c->Print (histname);
    delete c;
    } //if Entries != 0 
  } // loop over keys
}



//-------------------------------------------------------------------------------------
void MoveStatsAndDraw (TH1 *data, TH1 *mc, char *dataTitle, char* mcTitle, char* firstline, char* secondline, int log, int col, bool stat){
  c->SetLogy(log);
  mc->Draw();
  data->Draw("pesames");   
  gStyle->SetOptStat(00000000); 
  if(stat) gStyle->SetOptStat(1111111111); 

  TPaveText *title = new TPaveText(0.50,0.85,0.95,0.95,"blNDC");
  title->SetFillStyle(4000);
  title->SetFillColor(kWhite);
  title->SetBorderSize(0.1);
  title->SetTextFont(42);
  title->AddText(firstline);
  title->AddText(secondline);
  title->Draw();  

  TLegend *leg = new TLegend(0.60,0.70,0.95,0.85);
  leg->SetFillStyle(4000);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(0.1);
  leg->SetTextFont(42);
  leg->AddEntry(data,dataTitle,"p");
  leg->AddEntry(mc,mcTitle,"f");
  leg->Draw();

  c->Modified();
  c->Update();



  if(stat){
  TPaveStats * ps = (TPaveStats *) mc->GetListOfFunctions()->FindObject("stats"); 
  TPaveStats * ps2 = (TPaveStats *) data->GetListOfFunctions()->FindObject("stats");
  Int_t ci; 
  ci = TColor::GetColor(col);
  if( ps ){ 
    ps->SetTextColor(1); 
    ps->SetLineColor(col);   
  }
  else std::cout << "Null pointer to TPaveStats: " << ps << std::endl; 
  ps->SetY1NDC(0.40);
  ps->SetY2NDC(0.68);
  if( ps2 ) ps2->SetTextColor(ci); else std::cout << "Null pointer to TPaveStats: " << ps2 << std::endl;
  }
}
