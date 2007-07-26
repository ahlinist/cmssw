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
#include <fstream>
#include <vector>
#include "FWCore/FWLite/src/AutoLibraryLoader.h"
#include "AnalysisDataFormats/TopObjects/interface/TtSemiMassSolution.h"
#include "TopQuarkAnalysis/TopTools/test/tdrstyle.C"

using namespace std;


//////////////////////////////////////////////////////////////////////////////////////////////////
// Input Parameters      								        //
//////////////////////////////////////////////////////////////////////////////////////////////////

static const TString  	dataDir		  = "file:/beo5/heyninck/CMSSW/src/TopQuarkAnalysis/TopMassMeasurement/test";
static const TString  	combBFitFile	  = "../../TtSemiCombBackFit_simple.root";
static const TString  	channelBFitFile	  = "../../TtSemiChannelBackFit.root";
       const int	nSamples	  = 1;
       const TString  	samples[nSamples] = {"TtSemiMassInput"};
       const int  	nrFiles[nSamples] = {1};
       
static const int 	doMeasurement 	  = 2;		//0: SimpleMeasurement;  1: Likelihood, no ideo scan ;  2: with ideo scan;  3: all three

static const Int_t      nrMtHistBins      = 120;
static const Float_t 	minMtHist     	  = 50;
static const Float_t 	maxMtHist     	  = 350;

static const Float_t 	gammaT		  = 1.5;
	
static const Bool_t  	inclChannelBgr     = false;

///////////////////////////////////////////////////////////////////////////////////////////////////


int 	 nrAnalysed[nSamples], measurement, sample;
ofstream asciFile[3][nSamples];
bool  	 scanPointsRead;
int  	 nrScannedMtValues, mstep;
double 	 minScanMtValue, maxScanMtValue, minlik, maxlik;
void 	 AnalyseSolution_SimpleMeasurement(TtSemiMassSolution&);
void 	 AnalyseSolution_Likelihood(TtSemiMassSolution&);
TF1 	 *fmtcomb, *fmtchannel;
double   jetCombProbFixed;




//
// Main program
//

int main() {

  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  setTDRStyle();
   
  // read in combinatorial and channel background fitted top mass template
  TFile combBFile(combBFitFile);
  fmtcomb = new TF1(*((TH1 *) combBFile.GetKey("hmtcomb")->ReadObj())->GetFunction("fmtcomb"));
  TH1F *hmttrue = (TH1F *) (combBFile.GetKey("hmttrue")->ReadObj());
  TH1F *hmtcomb = (TH1F *) (combBFile.GetKey("hmtcomb")->ReadObj());
  jetCombProbFixed = hmttrue->GetSumOfWeights()/(hmttrue->GetSumOfWeights()+hmtcomb->GetSumOfWeights());
  cout<<"found a jetCombProbFixed = "<<jetCombProbFixed*100.<<"%"<<endl;
  if(inclChannelBgr){
    TFile channelBFile(channelBFitFile);
    fmtchannel = new TF1(*((TH1 *) channelBFile.GetKey("hmtchannel")->ReadObj())->GetFunction("fmtchannel"));
  } 
   
  // create and open an asci file for each measurement and each sample
  int start = doMeasurement;
  int end   = doMeasurement;
  if(doMeasurement == 3) {start = 0; end = 2;};
  for(sample = 0; sample<nSamples; sample++){
    nrAnalysed[sample] = 0;
    for(measurement = start; measurement<=end; measurement++){
      //simple measurement
      if(measurement == 0){
        cout<<"*** Creating asciFile for measurement "<<measurement<<" and sample "<<samples[sample]<<" ..."<<endl;
        TString simpleName         = "../asciFiles/SimpleMeasurement_"; simpleName += samples[sample]; simpleName += ".txt";
        asciFile[measurement][sample].open(simpleName, ofstream::out | ofstream::app);
      }
      else
      {
        cout<<"*** Creating asciFile for measurement "<<measurement<<" and sample "<<samples[sample]<<" ..."<<endl;
        TString LikFileName = "../asciFiles/LikeLihood_";
        if(measurement == 1) LikFileName += "gaussianIdeogram_";
        if(measurement == 2) LikFileName += "scannedIdeogram_";
        LikFileName += samples[sample]; LikFileName += ".txt";
        asciFile[measurement][sample].open(LikFileName, ofstream::out | ofstream::app);
      }
    }
  }
  
  
  // loop over all events in the different samples
  scanPointsRead = false;
  for(sample = 0; sample<nSamples; sample++){
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
	  
	  // Read the nr of scanned points, and the range
	  if(!scanPointsRead){
	    nrScannedMtValues = sol.getScanValues().size();
	    minScanMtValue    = sol.getScanValues()[0].first;
	    maxScanMtValue    = sol.getScanValues()[nrScannedMtValues-1].first;
	    cout<<"Found "<<nrScannedMtValues<<" scanned mtop values between "<<minScanMtValue<<" & "<<maxScanMtValue<<endl;
	    mstep  = nrScannedMtValues;
	    minlik = minScanMtValue;
	    maxlik = maxScanMtValue;
	  }
	  
          // perform different mass measurements
          for(measurement = start; measurement<=end; measurement++){
            if(measurement == 0) AnalyseSolution_SimpleMeasurement(sol);
            if(measurement > 0)  AnalyseSolution_Likelihood(sol);
          }
	}
      }
    }
  }
  
  // close the asci files and write some statistics
  for(sample = 0; sample<nSamples; sample++){
    cout<<"Analysed "<<nrAnalysed[sample]<<" "<<samples[sample]<<" events"<<endl;
    int start = doMeasurement;
    int end   = doMeasurement;
    if(doMeasurement == 3) {start = 0; end = 2;};
    for(measurement = start; measurement<=end; measurement++){
      asciFile[measurement][sample].close(); 
    }
  }
}







//
// simple top mass measurement
//
void AnalyseSolution_SimpleMeasurement(TtSemiMassSolution &sol){
  if(combBFitFile.Contains("simple")) 			asciFile[measurement][sample] << sol.getFitHadt().mass() << 1. << endl;
  if(combBFitFile.Contains("LR") && (!inclChannelBgr)) 	asciFile[measurement][sample] << sol.getFitHadt().mass() << sol.getLRJetCombProb() << endl;
  if(combBFitFile.Contains("LR") &&   inclChannelBgr)   asciFile[measurement][sample] << sol.getFitHadt().mass() << sol.getLRSignalEvtProb()*sol.getLRJetCombProb() << endl;
}






//
// Likelihood method (Ideogram assumed gaussian or fully scanned)
//
void AnalyseSolution_Likelihood(TtSemiMassSolution &sol){
  double mtop  = sol.getFitHadt().mass(); 
  if (mtop<1) return;
  double dmtop = sol.getMtopUncertainty();
  cout<<"mtop = "<<sol.getCalHadt().mass()<<" "<<mtop<<" +/- "<<dmtop<<endl;
  
  // produce a gaussian ideogram or resolution function (1D) weighted by chi2Prob
  // ideo = 1/(sqrt(2.PI)*dmfit) * exp(-0.5((m-mfit)/dmfit)^2)
  double masses[50], ideo[50],scanmax = 0, maxpoint;
  for (int istep=0; istep<nrScannedMtValues; istep++) {
    masses[istep] = sol.getScanValues()[istep].first;
    
    //gaussian ideogram x Prob(Chi2) OR full scanned ideogram 
    if( measurement == 1 ) ideo[istep] = exp(-0.5 * pow((masses[istep]-mtop)/dmtop,2));
    if( measurement == 2 ) {
      ideo[istep] = sol.getScanValues()[istep].second;
      if(scanmax<ideo[istep]){
        scanmax  = ideo[istep];
        maxpoint = masses[istep];
      }
    }
    if(ideo[istep]<0) return;
  }

  //Absolute Rescaling plotIdeoScan (doesn't affect physics, just to improve ideo comparison plots)
  if( measurement == 2 ) {
    for (int istep=0; istep<nrScannedMtValues; istep++) {ideo[istep] = ideo[istep]/scanmax;};
  }
 
  
  // convolution of ideogram with Breit-Wigner BW(m,M) and Combinatorial Background fit
  // lik(M) = \int_m P(x|mt).P(mt|Mt) dmt
  // with P(x|mt) = ideogram
  // and P(mt|MT) = P_bgr [ P_comb.BW(mt|MT) + (1-P_comb).B_comb(mt) ] + (1-P_bgr).B_proc(mt)
  double massT, BW[50], CombBG[50], ChannelBG[50], lik[50];
  for (int mistep=0; mistep<mstep; mistep++) {
    lik[mistep]=(double)(0);
    massT=(double)(minlik+(double)mistep*(maxlik-minlik)/((double)mstep*1.-1.));
    lik[mistep]=0;
    for (int istep=0; istep<nrScannedMtValues; istep++) {
      BW[istep]=(double)((1./(2.*3.1415))*gammaT/(pow(masses[istep]-massT,2)+pow(gammaT/2.,2)));
      CombBG[istep] = fmtcomb->Eval(masses[istep]);
      if (CombBG[istep] < 0.) CombBG[istep] = 0.;
      ChannelBG[istep] = 0;
      if(inclChannelBgr){
        ChannelBG[istep] = fmtchannel->Eval(masses[istep]);
        if (ChannelBG[istep] < 0.) ChannelBG[istep] = 0.;
      }
      
      double combPurity = 1.;
      if(combBFitFile.Contains("simple")) combPurity = jetCombProbFixed;
      if(combBFitFile.Contains("LR"))     combPurity = sol.getLRJetCombProb();
      double signalPurity = 1.;
      if(inclChannelBgr) combPurity = sol.getLRSignalEvtProb();
      
      if(inclChannelBgr) 
      lik[mistep] += (signalPurity * ( combPurity*BW[istep] + (1.-combPurity)*CombBG[istep]) + (1.-signalPurity)*ChannelBG[istep])   * ideo[istep]*(maxScanMtValue-minScanMtValue)/((double)nrScannedMtValues*1.-1.);
    }
  }
  asciFile[measurement][sample] <<lik[0]<< " " <<lik[1]<< " " <<lik[2]<< " " <<lik[3]<< " " <<lik[4]<< " " <<lik[5]<<" " <<lik[6]<< " " <<lik[7]<< " " <<lik[8]<< " " <<lik[9]<< " " <<lik[10]<< " " <<lik[11]<< " " <<lik[12]<< " " <<lik[13]<< " " <<lik[14]<< " " <<lik[15]<<" " <<lik[16]<< " " <<lik[17]<< " " <<lik[18]<< " " <<lik[19]<< " " <<lik[20]<< " " <<minlik<< endl;
}
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  //h.Fill(scanmax);
  //if(fabs(maxpoint-mtop)>20) return;
  //if(scanmax<0.2) return;

  /*if(nrfilledIdeos<20){
    TString name = "Ideograms for event "; name += itEv->ideogram.evtNr; name += " in run "; name += itEv->ideogram.runNr;
    TCanvas c("c",name,1); //c.SetLogy();
    TGraph *id2 = new TGraph(nrScannedMtValues,masses,plotIdeoScan);
    id2->SetFillStyle(1); id2->SetFillColor(0); id2->SetLineStyle(2); id2->SetLineWidth(2); id2->SetLineColor(13);
    id2->SetTitle(name);
    id2->GetXaxis()->SetTitle("rec. hadr. m_{top}");
    id2->Draw("AL*"); 
    TF1 f("f","exp(-0.5 * [2]*pow((x-[0])/([1]),2))",0,300);
    f.SetParameters(mtop,dmtop,1.);
    f.Draw("same");
    TLegend l1(0.6, 0.72, 0.87, .87);
    l1.SetBorderSize(1);
    l1.SetFillStyle(1001);
    l1.SetFillColor(10); 
    l1.AddEntry(f.GetName(),"constructed");
    l1.AddEntry(id2,"scanned");
    l1.Draw();
    c.Print("IdeoComparison.ps","Landscape");
    //TString t2 = "EventIdeos"; t2 += nrfilledIdeos; t2 += ".C"; c.Print(t2);
    //TString t3 = "EventIdeos"; t3 += nrfilledIdeos; t3 += ".eps"; c.Print(t3);
    ++nrfilledIdeos;
  }
  else if(nrfilledIdeos==20) {
    cout<<"Filled 100 ideogram comparisons"<<endl;
    dummy->Print("IdeoComparison.ps]","Landscape");
    ++nrfilledIdeos;
  }*/
  
 
