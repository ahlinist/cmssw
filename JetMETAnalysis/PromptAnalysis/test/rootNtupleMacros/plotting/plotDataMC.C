using namespace std;
#include "tdrstyle.C"

void plotDataMC(){
   
  // specify the DATA/MC root file!!!
  // --------------------------------------------------------------------------
  TFile *data = TFile::Open("./data/output/corrected/900GeVjan23input.root");
  TFile *mc = TFile::Open("./data/output/corrected/mcInput900.root");
  char *legDataTitle = "Data, 900 GeV, corrected jets";
  TString dataset="data_900_GeV"; 
  double rebin=4.;
  //---------------------------------------------------------------------------
  setTDRStyle();
  gROOT->ForceStyle();
  TIter next (data->GetListOfKeys ());
  
  for (TKey* key = 0; (key = (TKey *) next());) {
    //  cout << key->GetClassName()<< endl;  
    TH1D * h_data= data->Get(key->GetName());
    TH1D * h_mc=mc->Get(key->GetName());; 
    // only for filled histos
    if( h_mc->GetEntries()!=0) {
    TCanvas *c = new TCanvas("c","",600,600); 
    double scalefactor= h_data->Integral()/h_mc->Integral();
    h_mc->Scale(scalefactor);
    h_mc->Rebin(rebin);
    h_data->Rebin(rebin);
    h_mc->SetMaximum(1.5*h_data->GetMaximum());
    h_mc->SetFillColor(5);
    h_data->SetMarkerStyle(20);
    std::cout << "Plotting: " << key->GetName()  <<  std::endl;
    MoveStatsAndDraw(h_data , h_mc, legDataTitle, legDataTitle, 1);
    char histname [256];
    sprintf (histname, "%s.gif", key->GetName());
    c.Print (histname);
    delete c;
    } //if Entries != 0
  } // loop over keys
}



//-------------------------------------------------------------------------------------
void MoveStatsAndDraw (TH1 *data, TH1 *mc, char *dataTitle, char* mcTitle, int log){
  c->SetLogy(log);
  mc->Draw();
  data->Draw("pesames");   
  //gStyle->SetOptStat(00000000);  
  gStyle->SetOptStat(1111111111); 
  TLegend *leg = new TLegend(0.40,0.80,0.75,0.90);
  leg->SetFillStyle(4000);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(0);
  leg->SetTextFont(42);
  leg->AddEntry(data,dataTitle,"p");
  leg->AddEntry(mc,mcTitle,"f");
  leg->Draw();
  c->Modified();
  c->Update();
  
  TPaveStats * ps = (TPaveStats *) mc->GetListOfFunctions()->FindObject("stats"); 
  TPaveStats * ps2 = (TPaveStats *) data->GetListOfFunctions()->FindObject("stats");
  Int_t ci; 
  ci = TColor::GetColor(5);
  if( ps ){ 
    ps->SetTextColor(1); 
    ps->SetLineColor(5);   
  }
  else std::cout << "Null pointer to TPaveStats: " << ps << std::endl; 
  ps->SetY1NDC(0.40);
  ps->SetY2NDC(0.68);
  if( ps2 ) ps2->SetTextColor(ci); else std::cout << "Null pointer to TPaveStats: " << ps2 << std::endl;
}
