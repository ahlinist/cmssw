{

#include <iostream.h>
#include <stdio.h>
#include <fstream.h>
#include "TH1.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TF1.h"
#include "TKey.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TProfile.h"
#include "TGraph.h"
using namespace std;

  /* -----------------------------------------------------------------------------
      ROOT file analysis for likelihood analysis
      Jan Heyninck - May 2007

      Aim    : Combine the event likelihoods for pull tests
      Input  : txt file of likelihoods
      Output : ps-file of several plots and likelihoods
               fitted top mass with uncertainty
     ----------------------------------------------------------------------------- */

  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1,0);
  gStyle->SetOptStat(0);

  /* -----------------------------------------------------------------------------
      Histo definition
     ----------------------------------------------------------------------------- */ 

  TH1F* histTopMass = new TH1F("histTopMass","Top Mass Reconstructed",200,150.,200.);
  TH1F* histTopMassError = new TH1F("histTopMassError","Top Mass Error",200,3.,12.);
  TH1F* histPull = new TH1F("histPull","Top Mass Pull",200,-2.,2.);

  ifstream LikFile;
  LikFile.open("../asciFiles/LikeLihood_gaussianIdeogram_TtSemiMuEvents.txt");

  /* -----------------------------------------------------------------------------
      Several ROOT files may have been created (split the Monte Carlo samples).
      Read the different ROOT files and add the histograms.
    -----------------------------------------------------------------------------*/

  int nevt = 100000;
  
  double lik[21][nevt], maxlik;
  double chitot[21];
  float massT, mt, dmt;

  for (int i=0; i<=20; i++) {
    chitot[i] = 0.;
  }
    
   if(!LikFile) {
     cout << "NO INPUT FILE" << endl;
     return -1;
   }
   int i=0;
   while (!LikFile.eof()) {
     LikFile >>lik[0][i]>>lik[1][i]>>lik[2][i]>>lik[3][i]>>lik[4][i]>>lik[5][i]>>lik[6][i]>>lik[7][i]>>lik[8][i]>>lik[9][i]>>lik[10][i]>>lik[11][i]>>lik[12][i]>>lik[13][i]>>lik[14][i]>>lik[15][i]>>lik[16][i]>>lik[17][i]>>lik[18][i]>>lik[19][i]>>lik[20][i]>>maxlik;
     if(i>=0. && lik[4][i]!=0) {
       for(int ii=0; ii<=20; ii++) {
         chitot[ii] = (double)(chitot[ii] - 2.*log(lik[ii][i]));
       }
     }
     i+=1;
   }
 
   LikFile.close();

   cout << " Total number of events = " << i << endl;
   int nevt=i;

  double mlikfunc[21];
  for (int i=0; i<=20; i++) {
    float m=(float)(125.+(i*5.));
    mlikfunc[i]=(double)(m);
    cout << "Total ChiSquare at " << mlikfunc[i] << " = " << chitot[i] << endl;
  }

  /* -----------------------------------------------------------------------------
      Perform maximum likelihood fit (parabola) - fit all points
     ----------------------------------------------------------------------------- */

  float thea, theb, thec, mlik, xmin, xplus;
  float x1, x2, x3, y1, y2, y3;
  float mtopfinal;
  float dmtopfinal;

  TGraph *alllikfuncgraph = new TGraph (21,mlikfunc,chitot);
  
  double mp[11];
  double likp[11];
  for (int i=0; i<11; i++) {
    mp[i] = mlikfunc[i+4];
    likp[i] = chitot[i+4];
  }

  TGraph *likfuncgraph = new TGraph (11,mp,likp);
  likfuncgraph.Fit("pol2","Q");
  TF1 *fit = likfuncgraph->GetFunction("pol2");
  thea = fit->GetParameter(0);
  theb = fit->GetParameter(1);
  thec = fit->GetParameter(2);

  TCanvas * myLikTot2 = new TCanvas("myLikTot2","Total Likelihood");
  myLikTot2->cd(1);
  alllikfuncgraph->Draw("AP*");
  likfuncgraph->SetMarkerColor(2); likfuncgraph->Draw("*");
  myLikTot2->Print("Result-mt.ps[","Portrait");
  myLikTot2->Print("Result-mt.ps","Portrait");


  /* -----------------------------------------------------------------------------
      Search maximum of the likelihood
     ----------------------------------------------------------------------------- */

  mtopfinal = - theb/(2*thec);

  mlik = thec*pow(mtopfinal,2)+theb*mtopfinal+thea;            //cout << " mlik = " << mlik << endl;
  xmin = (-theb-sqrt(pow(theb,2)-4*thec*(thea-mlik-1.)))/(2*thec);   //cout << " xmin = " << xmin << endl;
  xplus = (-theb+sqrt(pow(theb,2)-4*thec*(thea-mlik-1.)))/(2*thec);  //cout << " xplus = " << xplus <<endl;
  
  cout << endl << endl;
  cout << "******************************************************************************" << endl; 
  cout << " Full fit " << endl;
  cout << "******************************************************************************" << endl;
  cout << " Result on top mass with " << nevt << " events" << endl;
  cout << " mtop =        " << mtopfinal << " GeV " << endl;
  cout << " uncertainty = " << mtopfinal-xmin << " and " << xplus-mtopfinal << endl; 

  float diff;
  diff = (0.5)*((mtopfinal-xmin)+(xplus-mtopfinal));

  cout << " All together --> mt = " << mtopfinal << " +- " << diff << " GeV " << endl;

  cout << "******************************************************************************" << endl;
  cout << "******************************************************************************" << endl;









  /* -------------------------------------------------------------------------------------
      Make some resampling tests (pull)
     ------------------------------------------------------------------------------------- */

  int nsample=10000	; // number of samples
  int nievt=1000; // number of events per sample
  int r;
  float chitots[21], mliks, xmins, xpluss;
  float mtopfinals[nsample];
  float dmtopfinals, diffs[nsample], res, pull,a,b,c;

  for (int isample=0; isample<nsample; isample++) {
    for (int ii=0; ii<=20; ii++) {
      chitots[ii]=0;
    }
    for (int ievt=0; ievt<=nievt; ievt++) {
      r = (int)((gRandom->Uniform())*nevt);
      if(lik[3][r] != 0){
        for (int ii=0; ii<=20; ii++) {
	  chitots[ii]=chitots[ii]-2.*log(lik[ii][r]);
        }
      }
      else
      {
        --ievt;
      }
    }
    for (int i=0; i<11; i++) {
      mp[i] = mlikfunc[i+4];
      likp[i] = chitots[i+4];
    }
    TGraph *likfuncgraph = new TGraph (11,mp,likp);
    likfuncgraph.Fit("pol2","Q");
    TF1 *fit = likfuncgraph->GetFunction("pol2");
    a = fit->GetParameter(0);
    b = fit->GetParameter(1);
    c = fit->GetParameter(2);
    mtopfinals[isample] = - b/(2*c);
    mliks = c*pow(mtopfinals[isample],2)+b*mtopfinals[isample]+a; 
    xmins = (-b-sqrt(pow(b,2)-4*c*(a-mliks-1.)))/(2*c);  
    xpluss = (-b+sqrt(pow(b,2)-4*c*(a-mliks-1.)))/(2*c); 
    diffs[isample] = (0.5)*((mtopfinals[isample]-xmins)+(xpluss-mtopfinals[isample]));
    delete likfuncgraph;
  }	
  
  float mtopaverage=0.;
  for (int isample=0;isample<nsample; isample++) {
	mtopaverage = mtopaverage + (1./((float)nsample))*mtopfinals[isample];
  }

  for (int isample=0; isample<nsample; isample++) {
    res=mtopfinals[isample]-mtopaverage;
    pull=res/diffs[isample];
    histTopMass->Fill(mtopfinals[isample]);
    histTopMassError->Fill(diffs[isample]);
    histPull->Fill(pull);
  }

  /* -------------------------------------------------------------------------------------
      Make some special plots for cross-check
      ------------------------------------------------------------------------------------- */

  cout << " Start making plots " << endl;

  gStyle->SetOptFit(1111);
  gStyle->SetOptStat(1);

  TCanvas * myPull = new TCanvas("myPull","Pull distribution");
  histPull->SetLineStyle(1); histPull->SetLineColor(4); histPull->SetLineWidth(3);
  myPull->cd(1);
  histPull->Fit("gaus","","",-2.,2.);
  histPull->Draw("");
  myPull->Print("Result-mt.ps","Portrait");

  TCanvas * myTop = new TCanvas("myTop","Top distribution");
  myTop->Divide(1,2);
  histTopMass->SetLineStyle(1); histTopMass->SetLineColor(4); histTopMass->SetLineWidth(3);
  histTopMassError->SetLineStyle(1); histTopMassError->SetLineColor(4); histTopMassError->SetLineWidth(3);
  myTop->cd(1);
  histTopMass->Fit("gaus","","",160.,190.);
  histTopMass->Draw("");
  myTop->cd(2);
  histTopMassError->Draw("");
  myTop->Print("Result-mt.ps]","Portrait");

}
