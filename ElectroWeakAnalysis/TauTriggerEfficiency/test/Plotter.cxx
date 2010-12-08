#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TGraphAsymmErrors.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TPad.h"
#include "TCut.h"
#include "TEventList.h"
#include "TChain.h"

#include "tdrstyle.cxx"

#include <string>
#include <stdint.h>

class Plotter {
public:
  Plotter(TString filename="tteffAnalysis.root", TString treename="TTEffTree", TString pickEventsFile = ""){
    init();
    chain = new TChain(treename);
    chain->AddFile(filename);
  }
  Plotter(TChain* chIN){
    init();
    chain = chIN;
  }
  ~Plotter(){
    delete chain;
    delete inFile;
  };

  TGraphAsymmErrors *DrawHistogram(const char* varexp, const TCut& selection);
  TGraphAsymmErrors *DrawHistogram(const char* varexp, const TCut& selection, const TCut& selection2);
  TH1 *DrawDistribution(const char* varexp, const TCut& selection);
  void SetXTitle(const char* title) {plotXtitle   = title;}
  void SetYTitle(const char* title) {plotYtitle   = title;}
  void SetFileName(TString name){plotFileName = name;}
  void SetFormat(const char *frmt){format = frmt;}
  void SetSave(bool s) {save = s;}
  int GetNEvents(const TCut& selection = "");

private:
//  TTree* pickEvents(TString,TTree*);

  void init(){
    //gROOT->LoadMacro("./tdrstyle.cxx");
    setTDRStyle();

    plotXtitle = 0;
    plotYtitle = 0;
    format = "";
    save = false;
  }

  TFile* inFile;
//  TTree* tree;
  TChain* chain;
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

  chain->Draw(varexp1,selection,"e");
  chain->Draw(varexp2,"","h");

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

  size_t posbegin = s_varexp.find("hnum");
  s_varexp.replace(posbegin, 4, "hden");

  const char* varexp2 = s_varexp.c_str();

  chain->Draw(varexp1,selection&&selection2,"e");
  chain->Draw(varexp2,selection2,"h");

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

TH1 *Plotter::DrawDistribution(const char* varexp, const TCut& selection){

  const char* varexp1 = varexp;
  chain->Draw(varexp1,selection);
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
  return hnum;
}
/*
TTree* Plotter::pickEvents(TString fPickEvents,TTree* intree){
	TList* treeList = new TList();

	string line;

	ifstream fIN(fPickEvents.Data(),ios::in);
	while(!fIN.eof()){
		getline (fIN,line);
		size_t col1 = line.find_first_of(':');
		size_t col2 = line.find_last_of(':');
		string s_run   = line.substr(0,col1);
		string s_lumi  = line.substr(col1+1,col2-col1-1);
		string s_event = line.substr(col2+1,line.length() - col2 - 1);
		if(s_event.length() > 0){
		  cout << "picking run:lumi:event " << line << endl;
		  string selection = "run == " + s_run + " && lumi == " + s_lumi + " && event == " + s_event;
		  TTree* pickEventTree = intree->CopyTree(selection.c_str());
		  treeList->Add(pickEventTree);
		}
	}

	TTree* tree = TTree::MergeTrees(treeList);
	cout << "picked events tree size " << tree->GetEntries() << endl;
	return tree;
}
*/

int Plotter::GetNEvents(const TCut& selection){
	int events = 0;

	TEventList *elist = new TEventList("elist");
	chain->Draw(">>elist",selection);

        uint32_t run,lumi,event;
        uint32_t run_old   = -1,
                 lumi_old  = -1,
                 event_old = -1;

        chain->SetBranchAddress("run",&run);
        chain->SetBranchAddress("lumi",&lumi);
        chain->SetBranchAddress("event",&event);

	for(int i = 0; i < elist->GetN(); ++i){
		chain->GetEntry(elist->GetEntry(i));

		if(i%1000 == 0) cout << "Entries processed " << i << endl;

                if(run == run_old && lumi == lumi_old && event == event_old) continue;
                run_old   = run;
                lumi_old  = lumi;
                event_old = event;
                ++events;
        }
	delete elist;
	return events;
}

