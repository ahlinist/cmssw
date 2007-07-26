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
#include "TopQuarkAnalysis/TopTools/test/tdrstyle.C"

using namespace std;


//////////////////////////////////////////////////////////////////////////////////////////////////
// Input Parameters      								        //
//////////////////////////////////////////////////////////////////////////////////////////////////

static const TString  	dataDir		  = "file:/beo5/heyninck/CMSSW/src/TopQuarkAnalysis/TopMassMeasurement/test/";
       const int	nSamples	  = 2;
       const TString  	samples[nSamples] = {"TtSemiMassInput_semimu","TtSemiMassInput_otherTt"};
       const int  	nrFiles[nSamples] = {1};

static const Int_t      nrMtHistBins      = 120;
static const Float_t 	minMtHist     	  = 50;
static const Float_t 	maxMtHist     	  = 350;

///////////////////////////////////////////////////////////////////////////////////////////////////



TH1F *hmtsign, *hmtotherTt, *hmtall;
TF1  *fmtchannel;


//
// Main program
//

int main() {

  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  setTDRStyle();
  TGaxis::SetMaxDigits(3);
  
  // define histograms & fit
  hmtsign    = new TH1F("hmtsign","",nrMtHistBins,minMtHist,maxMtHist);
  hmtotherTt = new TH1F("hmtotherTt","",nrMtHistBins,minMtHist,maxMtHist);
  hmtall     = new TH1F("hmtall","",nrMtHistBins,minMtHist,maxMtHist);
  fmtchannel    = new TF1("fmtchannel","landau",minMtHist,maxMtHist);


  // loop over all events
  int nrAnalysed[nSamples];
  for(int sample = 0; sample<nSamples; sample++){
    nrAnalysed[sample] = 0;
    TString fileName = dataDir; fileName += "/"; fileName += samples[sample]; //fileName += "_";
    for(int fn=0; fn<nrFiles[sample]; fn ++){
      //fileName += fn; 
      fileName += ".root";
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
	  if( samples[sample].Contains("semimu"))  hmtsign    -> Fill(sol.getFitHadt().mass());
	  if( samples[sample].Contains("otherTt")) hmtotherTt -> Fill(sol.getFitHadt().mass());
	}
      }
    }
    cout<<"Analysed "<<nrAnalysed[sample]<<" "<<samples[sample]<<" events"<<endl;
  }


/*
  // rescale the histograms wrt the cross section
  double intB = hmtotherTt -> Integral(0,hmtotherTt->GetXaxis()->GetNbins(),"width");
  for(int b=0; b<=nrMtHistBins; b++){
    hmtsign -> SetBinContent(b,hmtsign -> GetBinContent(b)/intB);
    hmtotherTt -> SetBinContent(b,hmtotherTt -> GetBinContent(b)/intB);
    hmtall  -> SetBinContent(b,hmtall  -> GetBinContent(b)/intB);
    hmtsign -> SetBinError  (b,hmtsign -> GetBinError(b)/intB);
    hmtotherTt -> SetBinError  (b,hmtotherTt -> GetBinError(b)/intB);
    hmtall  -> SetBinError  (b,hmtall  -> GetBinError(b)/intB);
  }
*/

  // normalize and fit the combintorial background spectrum
  double intB = hmtotherTt -> Integral(0,hmtotherTt->GetXaxis()->GetNbins(),"width");
  for(int b=0; b<=nrMtHistBins; b++){
    hmtsign -> SetBinContent(b,hmtsign -> GetBinContent(b)/intB);
    hmtotherTt -> SetBinContent(b,hmtotherTt -> GetBinContent(b)/intB);
    hmtall  -> SetBinContent(b,hmtall  -> GetBinContent(b)/intB);
    hmtsign -> SetBinError  (b,hmtsign -> GetBinError(b)/intB);
    hmtotherTt -> SetBinError  (b,hmtotherTt -> GetBinError(b)/intB);
    hmtall  -> SetBinError  (b,hmtall  -> GetBinError(b)/intB);
  }
  cout<<"Fraction of events where a true matching was found = "<<(hmtsign -> GetEntries()*100.)/(hmtall -> GetEntries()*1.)<<"%"<<endl;
  
  cout<<"Normalized? "<<hmtotherTt -> Integral(0,hmtotherTt->GetXaxis()->GetNbins(),"width")<<endl;
  hmtotherTt -> Fit(fmtchannel->GetName(),"RQ");
  
  
  // write output to ps-file
  TString fname = "../data/TtSemiChannelBackFit";
  TCanvas c("dummy","",1);
  c.Print(fname + ".ps[","landscape");
  TCanvas c1("c1","mtfitSpectrum",1);
  hmtall  -> GetXaxis() -> SetTitle("fitted top mass (GeV/c^{2})");
  hmtall  -> GetYaxis() -> SetTitle("entries");
  hmtall  -> SetFillColor(5); hmtall  -> Draw("hist");
  hmtotherTt -> SetFillColor(2); hmtotherTt -> Draw("histsame");
  TLegend *leg = new TLegend(0.65,0.7,0.87,0.87,NULL,"brNDC"); 
  leg->SetBorderSize(1); leg->SetFillStyle(1001); leg->SetFillColor(10);
  leg->AddEntry("hmtall","correct comb.","f");
  leg->AddEntry("hmtotherTt","wrong comb.","f");
  leg->Draw();
  c1.Print(fname + ".ps","landscape");
  gStyle->SetOptFit(1);
  TCanvas c2("c2","true jet combinations",1);
  hmtsign  -> GetXaxis() -> SetTitle("fitted top mass (GeV/c^{2})");
  hmtsign  -> GetYaxis() -> SetTitle("entries");
  hmtsign -> Draw();
  hmtsign -> Fit("gaus");
  c2.Print(fname + ".ps","landscape");
  TCanvas c3("c3","wrong jet combinations",1);
  hmtotherTt  -> SetFillColor(0);
  hmtotherTt  -> GetXaxis() -> SetTitle("fitted top mass (GeV/c^{2})");
  hmtotherTt  -> GetYaxis() -> SetTitle("entries");
  hmtotherTt -> Draw();
  c3.Print(fname + ".ps","landscape");  
  c.Print(fname + ".ps]","landscape");

  
  
  // write output to root-file
  TFile fOut(fname+".root","RECREATE");
  fOut.cd();
  hmtsign -> Write();
  hmtotherTt -> Write();
  hmtall -> Write();
  fOut.cd();
  fOut.Write();
  fOut.Close();

} 
