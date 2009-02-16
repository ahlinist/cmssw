#include <string>
class Plotter {
public:
  Plotter(){

    gROOT->LoadMacro("./tdrstyle.cxx");
    setTDRStyle();

    inFile = new TFile("tteffAnalysis.root");
    //inFile = new TFile("tteffAnalysis-merged.root");
    tree = (TTree *) (inFile->Get("TTEffTree"));

    plotXtitle = 0;
    plotYtitle = 0;
    format = 0;
    save = false;
  }
  ~Plotter(){
    delete tree;
    delete inFile;
  };

  TGraphAsymmErrors *DrawHistogram(const char* varexp, const TCut& selection);
  TGraphAsymmErrors *DrawHistogram(const char* varexp, const TCut& selection, const TCut& selection2);
  void SetXTitle(char* title) {plotXtitle   = title;}
  void SetYTitle(char* title) {plotYtitle   = title;}
  void SetFileName(TString name){plotFileName = name;}
  void SetFormat(char *frmt){format = frmt;}
  void SetSave(bool s) {save = s;}

private:
  TFile* inFile;
  TTree* tree;
  char* plotXtitle;
  char* plotYtitle;
  TString plotFileName;
  char *format;
  bool save;
};

TGraphAsymmErrors *Plotter::DrawHistogram(const char* varexp, const TCut& selection){
  //  TCanvas *c = new TCanvas();
  // c->cd();

  char* varexp1 = varexp;

  string s_varexp = string(varexp);

  size_t posbegin = s_varexp.find(">>");
  size_t posend   = s_varexp.find("(");

  s_varexp = s_varexp.substr(0,posbegin+2) + "hden" + s_varexp.substr(posend,s_varexp.length()-posend);

  char* varexp2 = s_varexp.c_str();

  tree->Draw(varexp1,selection,"e");
  tree->Draw(varexp2,"","h");

  TH1F *hnum = (TH1F*)gDirectory->Get("hnum");
  TH1F *hden = (TH1F*)gDirectory->Get("hden");

  hnum->SetStats(0);
  hnum->SetTitle("");

  TGraphAsymmErrors* heff = new TGraphAsymmErrors(hnum,hden);
  heff -> BayesDivide(hnum,hden);
  heff -> GetXaxis()->SetTitle(plotXtitle);
  heff -> GetYaxis()->SetTitle(plotYtitle);
  //heff ->SetMinimum(0);
  //heff ->SetMaximum(1.1);
  heff -> SetMarkerColor(kBlack);
  heff -> SetMarkerSize(1.);
  heff -> SetLineWidth(1);
  heff -> SetLineColor(kBlack);
  heff -> SetMarkerStyle(kFullDotLarge);
  heff -> Draw("PAE");

  if(save) {
    if(format)
      gPad->SaveAs(plotFileName+format);
    else
      gPad->SaveAs(plotFileName);
  }
  return heff;
}


TGraphAsymmErrors *Plotter::DrawHistogram(const char* varexp, const TCut& selection, const TCut& selection2){

  char* varexp1 = varexp;

  string s_varexp = string(varexp);

  size_t posbegin = s_varexp.find(">>");
  size_t posend   = s_varexp.find("(");

  s_varexp = s_varexp.substr(0,posbegin+2) + "hden" + s_varexp.substr(posend,s_varexp.length()-posend);

  char* varexp2 = s_varexp.c_str();

  tree->Draw(varexp1,selection&&selection2,"e");
  tree->Draw(varexp2,selection2,"h");

  TH1F *hnum = (TH1F*)gDirectory->Get("hnum");
  TH1F *hden = (TH1F*)gDirectory->Get("hden");

  hnum->SetStats(0);
  hnum->SetTitle("");

  TGraphAsymmErrors* heff = new TGraphAsymmErrors(hnum,hden);
  heff -> BayesDivide(hnum,hden);
  heff -> GetXaxis()->SetTitle(plotXtitle);
  heff -> GetYaxis()->SetTitle(plotYtitle);
  //heff ->SetMinimum(0);
  //heff ->SetMaximum(1.1);
  heff -> SetMarkerColor(kBlack);
  heff -> SetMarkerSize(1.);
  heff -> SetLineWidth(1);
  heff -> SetLineColor(kBlack);
  heff -> SetMarkerStyle(kFullDotLarge);
  heff -> Draw("PAE");

  if(save) {
    if(format)
      gPad->SaveAs(plotFileName+format);
    else
      gPad->SaveAs(plotFileName);
  }
  return heff;
}

void Plotter::DrawDistribution(const char* varexp, const TCut& selection){

  char* varexp1 = varexp;
  tree->Draw(varexp1,selection);
  TH1F *hnum = (TH1F*)gDirectory->Get("hnum");

  hnum->SetStats(0);
  hnum->SetTitle("");
  hnum->Sumw2();

  hnum -> GetXaxis()->SetTitle(plotXtitle);
  hnum -> GetYaxis()->SetTitle(plotYtitle);
  hnum -> Draw("HIST");

  if(save) {
    if(format)
      gPad->SaveAs(plotFileName+format);
    else
      gPad->SaveAs(plotFileName);
  }
}
