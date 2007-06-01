// general
#include <TH1.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TSystem.h>
#include <TF1.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TKey.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TGaxis.h>
#include <fstream>
#include <vector>
#include "FWCore/FWLite/src/AutoLibraryLoader.h"
#include "AnalysisDataFormats/TopObjects/interface/TtSemiMassSolution.h"
#include "TopQuarkAnalysis/TopEventProducers/bin/tdrstyle.C"

using namespace std;


//////////////////////////////////////////////////////////////////////////////////////////////////
// Input Parameters      								        //
//////////////////////////////////////////////////////////////////////////////////////////////////

static const TString  	dataDir		  = "file:/beo5/heyninck/CMSSW/src/TopQuarkAnalysis/TopMassMeasurement/test";
       const int	nSamples	  = 1;
       const TString  	samples[nSamples] = {"TtSemiMassInput"};
       const int  	nrFiles[nSamples] = {1};

static const Int_t      nrMtHistBins      = 120;
static const Float_t 	minMtHist     	  = 50;
static const Float_t 	maxMtHist     	  = 350;

       const TString  	solChoiceInInput  = "simple";      		       // "simple" or "LR"
       const bool  	useSpaceAngle     = true;
       const double 	SumAlphaCut  	  = 0.7;

///////////////////////////////////////////////////////////////////////////////////////////////////



TH1F *hmttrue, *hmtcomb, *hmtall;
TF1  *fmtcomb;


//
// Main program
//

int main() {

  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  setTDRStyle();
  TGaxis::SetMaxDigits(3);
  
  // define histograms & fit
  hmttrue = new TH1F("hmttrue","",nrMtHistBins,minMtHist,maxMtHist);
  hmtcomb = new TH1F("hmtcomb","",nrMtHistBins,minMtHist,maxMtHist);
  hmtall  = new TH1F("hmtall","",nrMtHistBins,minMtHist,maxMtHist);
  fmtcomb  = new TF1("fmtcomb","landau",minMtHist,maxMtHist);


  // loop over all events
  int nrAnalysed[nSamples];
  for(int sample = 0; sample<nSamples; sample++){
    nrAnalysed[sample] = 0;
    TString fileName = dataDir; fileName += "/"; fileName += samples[sample]; fileName += "_";
    for(int fn=0; fn<nrFiles[sample]; fn ++){
      fileName += fn; fileName += ".root";
      TFile fIn(fileName);
      TTree * events = dynamic_cast<TTree*>( fIn.Get( "Events" ) );
      assert( events != 0 );
      TBranch * solBranch;
      solBranch = events->GetBranch( "TtSemiMassSolution_ideoSol__TtSemiMass.obj" );
      assert( solBranch != 0 );
      TtSemiMassSolution sol;
      solBranch -> SetAddress( & sol );
      
      int nev = events->GetEntries();
      for( int ev = 0; ev < nev; ++ ev ) {
        solBranch    -> GetEntry( ev );
        if(sol.getSimpleCorrJetComb() > -1){
          ++nrAnalysed[sample];
	  bool signal = false;
	  if( (solChoiceInInput == "simple") && (sol.getSimpleCorrJetComb() == sol.getMCCorrJetComb()) && (sol.getSumDeltaRjp() < SumAlphaCut) ) signal = true;
	  if( (solChoiceInInput == "LR")     && (sol.getLRCorrJetComb()     == sol.getMCCorrJetComb()) && (sol.getSumDeltaRjp() < SumAlphaCut) ) signal = true;
	  if( signal) hmttrue -> Fill(sol.getFitHadt().mass());
	  if(!signal) hmtcomb -> Fill(sol.getFitHadt().mass());
	  hmtall -> Fill(sol.getFitHadt().mass());
	}
      }
    }
    cout<<"Analysed "<<nrAnalysed[sample]<<" "<<samples[sample]<<" events"<<endl;
  }


  // normalize and fit the combintorial background spectrum
  double intB = hmtcomb -> Integral(0,hmtcomb->GetXaxis()->GetNbins(),"width");
  for(int b=0; b<=nrMtHistBins; b++){
    hmttrue -> SetBinContent(b,hmttrue -> GetBinContent(b)/intB);
    hmtcomb -> SetBinContent(b,hmtcomb -> GetBinContent(b)/intB);
    hmtall  -> SetBinContent(b,hmtall  -> GetBinContent(b)/intB);
    hmttrue -> SetBinError  (b,hmttrue -> GetBinError(b)/intB);
    hmtcomb -> SetBinError  (b,hmtcomb -> GetBinError(b)/intB);
    hmtall  -> SetBinError  (b,hmtall  -> GetBinError(b)/intB);
  }
  cout<<"Fraction of events where a true matching was found = "<<(hmttrue -> GetEntries()*100.)/(hmtall -> GetEntries()*1.)<<"%"<<endl;
  
  cout<<"Normalized? "<<hmtcomb -> Integral(0,hmtcomb->GetXaxis()->GetNbins(),"width")<<endl;
  hmtcomb -> Fit(fmtcomb->GetName(),"RQ");
  
  
  // write output to ps-file
  TString fname = "../data/TtSemiCombBackFit_"; fname += solChoiceInInput;
  TCanvas c("dummy","",1);
  c.Print(fname + ".ps[","landscape");
  TCanvas c1("c1","mtfitSpectrum",1);
  hmtall  -> GetXaxis() -> SetTitle("fitted top mass (GeV/c^{2})");
  hmtall  -> GetYaxis() -> SetTitle("entries");
  hmtall  -> SetFillColor(5); hmtall  -> Draw("hist");
  hmtcomb -> SetFillColor(2); hmtcomb -> Draw("histsame");
  TLegend *leg = new TLegend(0.65,0.7,0.87,0.87,NULL,"brNDC"); 
  leg->SetBorderSize(1); leg->SetFillStyle(1001); leg->SetFillColor(10);
  leg->AddEntry("hmtall","correct comb.","f");
  leg->AddEntry("hmtcomb","wrong comb.","f");
  leg->Draw();
  c1.Print(fname + ".ps","landscape");
  gStyle->SetOptFit(1);
  TCanvas c2("c2","true jet combinations",1);
  hmttrue  -> GetXaxis() -> SetTitle("fitted top mass (GeV/c^{2})");
  hmttrue  -> GetYaxis() -> SetTitle("entries");
  hmttrue -> Draw();
  hmttrue -> Fit("gaus");
  c2.Print(fname + ".ps","landscape");
  TCanvas c3("c3","wrong jet combinations",1);
  hmtcomb  -> SetFillColor(0);
  hmtcomb  -> GetXaxis() -> SetTitle("fitted top mass (GeV/c^{2})");
  hmtcomb  -> GetYaxis() -> SetTitle("entries");
  hmtcomb -> Draw();
  c3.Print(fname + ".ps","landscape");  
  c.Print(fname + ".ps]","landscape");

  
  
  // write output to root-file
  TFile fOut(fname+".root","RECREATE");
  fOut.cd();
  hmttrue -> Write();
  hmtcomb -> Write();
  hmtall -> Write();
  fOut.cd();
  fOut.Write();
  fOut.Close();

} 
