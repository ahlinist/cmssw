#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TGraphAsymmErrors.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TPad.h"
#include "TCut.h"

#include "tdrstyle.cxx"

#include <string>
class Plotter {
public:
  Plotter(TString filename="tteffAnalysis.root"){

    //gROOT->LoadMacro("./tdrstyle.cxx");
    setTDRStyle();

    inFile = TFile::Open(filename);
    //inFile = TFile::Open("tteffAnalysis.root");
    //inFile = TFile::Open("tteffAnalysis-merged.root");
    tree = (TTree *) (inFile->Get("TTEffTree"));

    plotXtitle = 0;
    plotYtitle = 0;
    format = "";
    save = false;
  }
  ~Plotter(){
    delete tree;
    delete inFile;
  };

  TGraphAsymmErrors *DrawHistogram(const char* varexp, const TCut& selection);
  TGraphAsymmErrors *DrawHistogram(const char* varexp, const TCut& selection, const TCut& selection2);
  void DrawDistribution(const char* varexp, const TCut& selection);
  void SetXTitle(const char* title) {plotXtitle   = title;}
  void SetYTitle(const char* title) {plotYtitle   = title;}
  void SetFileName(TString name){plotFileName = name;}
  void SetFormat(const char *frmt){format = frmt;}
  void SetSave(bool s) {save = s;}

private:
  TFile* inFile;
  TTree* tree;
  const char* plotXtitle;
  const char* plotYtitle;
  TString plotFileName;
  TString format;
  bool save;
};

TGraphAsymmErrors *Plotter::DrawHistogram(const char* varexp, const TCut& selection){
  //  TCanvas *c = new TCanvas();
  // c->cd();

  const char* varexp1 = varexp;

  std::string s_varexp(varexp);

  size_t posbegin = s_varexp.find(">>");
  size_t posend   = s_varexp.find("(", posbegin);

  s_varexp = s_varexp.substr(0,posbegin+2) + "hden" + s_varexp.substr(posend,s_varexp.length()-posend);

  const char* varexp2 = s_varexp.c_str();

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
    if(format != "")
      gPad->SaveAs(plotFileName+format);
    else
      gPad->SaveAs(plotFileName);
  }
  return heff;
}


TGraphAsymmErrors *Plotter::DrawHistogram(const char* varexp, const TCut& selection, const TCut& selection2){

  const char* varexp1 = varexp;

  std::string s_varexp(varexp);

  size_t posbegin = s_varexp.find(">>");
  size_t posend   = s_varexp.find("(", posbegin);

  s_varexp = s_varexp.substr(0,posbegin+2) + "hden" + s_varexp.substr(posend,s_varexp.length()-posend);

  const char* varexp2 = s_varexp.c_str();

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
    if(format != "")
      gPad->SaveAs(plotFileName+format);
    else
      gPad->SaveAs(plotFileName);
  }
  return heff;
}

void Plotter::DrawDistribution(const char* varexp, const TCut& selection){

  const char* varexp1 = varexp;
  tree->Draw(varexp1,selection);
  TH1F *hnum = (TH1F*)gDirectory->Get("hnum");

  hnum->SetStats(0);
  hnum->SetTitle("");
  hnum->Sumw2();

  hnum -> GetXaxis()->SetTitle(plotXtitle);
  hnum -> GetYaxis()->SetTitle(plotYtitle);
  hnum -> Draw("HIST");

  if(save) {
    if(format != "")
      gPad->SaveAs(plotFileName+format);
    else
      gPad->SaveAs(plotFileName);
  }
}
