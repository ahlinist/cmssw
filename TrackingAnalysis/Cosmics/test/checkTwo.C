#include "checkTwo.h"
#include <TArrow.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TLegend.h>
#include "overlapHelper.C"

using namespace std;

checkTwo::checkTwo(TString fileN1, TString fileN2)
{
  oC0 = new overlapCheck(fileN1);
  oC0->loadSummaryPlots();
  oC1 = new overlapCheck(fileN2);
  oC1->loadSummaryPlots();

  Long64_t entries0 = oC0->entries();
  Long64_t entries1 = oC1->entries();

  IImap idMap_tmp;
  for (Long64_t i=0; i<entries0; ++i) {
    DetIdPair idPair = oC0->getPair(i);
    idMap_tmp[idPair] = IntPair(i,-1);
  }

  for (Long64_t i=0; i<entries1; ++i) {
    DetIdPair idPair = oC1->getPair(i);
    IImap::iterator it = idMap_tmp.find(idPair);
    if ( it!=idMap_tmp.end() ) {
      (*it).second.second = i;
    }
  }
  
  for (IImap::iterator it = idMap_tmp.begin(); it!=idMap_tmp.end(); ++it) {
    if ((*it).second.second!=-1) idMap[(*it).first] = (*it).second;
    //else cout<<"no\n";
    //cout << (*it).second.first <<" "<< (*it).second.second<<endl;
    
      }
  cout << "Found "<<idMap.size()<< " common detectors" <<endl;
  int ii = 0;
  for (IImap::iterator it = idMap.begin(); it!=idMap.end(); ++it) {
  cout << "Pair "<< ii++ <<" ( ";
    cout << (*it).second.first <<" , "<< (*it).second.second<<" ) : \t";
    oC0->idPrint(it->first);
  }


}


void checkTwo::comparePlots(int which, bool wErr)
{
  TH1 *meanDiffsO[2];
  if (which==0) {
    meanDiffsO[0] = oC0->meanDiffs;
    meanDiffsO[1] = oC1->meanDiffs;
  } else if (which==1){
    meanDiffsO[0] = oC0->sigmaDiffs;
    meanDiffsO[1] = oC1->sigmaDiffs;
  } else if (which==2){
    meanDiffsO[0] = oC0->ddVsLocalYSlope;
    meanDiffsO[1] = oC1->ddVsLocalYSlope;
  } else if (which==3){
    meanDiffsO[0] = oC0->ddVsDxdzSlope;
    meanDiffsO[1] = oC1->ddVsDxdzSlope;
  } else return;
  TH1F *meanDiffs[2];
  char hn[32];
  int n= idMap.size();
  for ( int j=0; j<2; ++j ) {
    sprintf(hn,"new%d", j);
    meanDiffs[j] = new TH1F(hn,hn,n,-0.5,n-0.5);
    meanDiffs[j]->GetXaxis()->SetTitle(meanDiffsO[j]->GetXaxis()->GetTitle());
    meanDiffs[j]->GetYaxis()->SetTitle(meanDiffsO[j]->GetYaxis()->GetTitle());
    meanDiffs[j]->SetMarkerStyle(24+j*2);
    meanDiffs[j]->SetMarkerColor(2+j*2);
  }
  compare(meanDiffs, meanDiffsO);
  TCanvas *s1 = new TCanvas("DDs","Summary Double differences", 900, 600);
  meanDiffs[0]->Draw("hist p");
  if (wErr) {
    meanDiffs[0]->Draw("same");
    meanDiffs[1]->Draw("same");
  } else {
    meanDiffs[1]->Draw("same hist p");
  }
  addLayer(meanDiffs[0]);
  putLegend(meanDiffs[0], meanDiffs[1]);
}

void checkTwo::addLayer (TH1* h)
{
    double y= h->GetMinimum()*1.03;
    cout << "y "<<y<<endl;
    TLine *l = new TLine(h->GetXaxis()->GetXmin(), 0., h->GetXaxis()->GetXmax(), 0.);
    l->SetLineStyle(2);
    l->Draw();

  char hn[3];
  unsigned int newdet, newlayer, stereo;
  int newsign;
  unsigned int last=1, lastPos=0;

  int ii = 0;
  for (IImap::iterator it = idMap.begin(); it!=idMap.end(); ++it) {

      DetIdPair idPair = it->first;
      //cout <<idPair.first<<endl;
      if (idPair.first==0) return;
      
      
      decode (idPair.first, newdet, newlayer, newsign, stereo);
     if ( newdet==5  ) newlayer = newlayer + 4;

      if (newlayer>last){
	TArrow *ar1 = new TArrow(lastPos-0.4, y, ii-0.6,y,0.04,"<|>");
	ar1->SetArrowSize(0.02); ar1->SetAngle(40);
	ar1->SetLineWidth(2); ar1->SetLineColor(last); ar1->Draw();
	ar1->SetFillStyle(1001); ar1->SetFillColor(last);
        sprintf(hn,"%d",last);
	TLatex* taga = new TLatex((lastPos+ii)/2-1, y*0.95, hn);
	taga->SetTextSize(0.04); taga->SetTextColor(last);
	taga->Draw();
        last = newlayer;
	lastPos = ii;
      }
      ++ii;
    }
    
      
     TArrow *ar1 = new TArrow(lastPos-0.4, y, ii-0.6,y,0.04,"<|>");
     ar1->SetArrowSize(0.02); ar1->SetAngle(40);
     ar1->SetLineWidth(2); ar1->SetLineColor(last); ar1->Draw();
     ar1->SetFillStyle(1001); ar1->SetFillColor(last);
     ar1->SetFillStyle(1001); ar1->SetFillColor(last);
     sprintf(hn,"%d",last);
     TLatex* taga = new TLatex((lastPos+ii)/2-1, y*0.9, hn);
     taga->SetTextSize(0.04); taga->SetTextColor(newlayer);
     taga->Draw();

}


void checkTwo::compareDD()
{
  TH1F *meanDiffs[2], *sigmaDiffs[2];
  TH1 *meanDiffsO[2], *sigmaDiffsO[2];
  char hn[32];
  int n= idMap.size();
  for ( int j=0; j<2; ++j ) {
    sprintf(hn,"mean%d", j);
    meanDiffs[j] = new TH1F(hn,hn,n,-0.5,n-0.5);
    meanDiffs[j]->GetXaxis()->SetTitle("module pair index");
    meanDiffs[j]->GetYaxis()->SetTitle("<#Deltax_{hit}-#Deltax_{pred}> [#mum]");
    meanDiffs[j]->SetMarkerStyle(24+j*2);
    meanDiffs[j]->SetMarkerColor(2+j*2);
    sprintf(hn,"width%d", j);
    sigmaDiffs[j] = new TH1F(hn,hn,n,-0.5,n-0.5);
    sigmaDiffs[j]->GetXaxis()->SetTitle("module pair index");
    sigmaDiffs[j]->GetYaxis()->SetTitle("fitted width #Deltax_{hit}-#Deltax_{pred} [#mum]");
    sigmaDiffs[j]->SetMarkerStyle(24+j*2);
    sigmaDiffs[j]->SetMarkerColor(2+j*2);
  }
  meanDiffsO[0] = oC0->meanDiffs;
  meanDiffsO[1] = oC1->meanDiffs;
  sigmaDiffsO[0] = oC0->sigmaDiffs;
  sigmaDiffsO[1] = oC1->sigmaDiffs;
  compare(meanDiffs, meanDiffsO);
  compare(sigmaDiffs, sigmaDiffsO);
   TCanvas *s1 = new TCanvas("DDs","Summary Double differences", 450, 600);
  s1->Divide(1,2);
  s1->cd(1);
  meanDiffs[0]->Draw("hist p");
  meanDiffs[0]->Draw("same");
  meanDiffs[1]->Draw("same");
  putLegend(meanDiffs[0], meanDiffs[1]);
  s1->cd(2); 
  sigmaDiffs[0]->Draw("hist p");
  sigmaDiffs[0]->Draw("same");
  sigmaDiffs[1]->Draw("same");
}


void checkTwo::compare(TH1F *res[2], TH1* sum[2])
{
  int ii = 0;
  for (IImap::iterator it = idMap.begin(); it!=idMap.end(); ++it) {
    ++ii;
    res[0]->SetBinContent(ii, sum[0]->GetBinContent((*it).second.first+1));
    res[1]->SetBinContent(ii, sum[1]->GetBinContent((*it).second.second+1));
    res[0]->SetBinError(ii, sum[0]->GetBinError((*it).second.first+1));
    res[1]->SetBinError(ii, sum[1]->GetBinError((*it).second.second+1));
  }
}

void checkTwo::putLegend(TH1 *h0, TH1* h1)
{
  TLegend* leg_hist = new TLegend(0.0,0.,0.15,0.15);
  leg_hist->AddEntry(h0,label0,"p");
  leg_hist->AddEntry(h1,label1,"p");
  leg_hist->SetTextSize(0.055);
  leg_hist->SetFillColor(0);
  leg_hist->Draw("same");
}

void checkTwo::compareRaw(int pair)
{
  IImap::iterator it = idMap.begin();
  for (int i=0;i<pair && it!=idMap.end(); ++i, ++it){}

  cout << "Pair "<< pair <<" ( ";
  cout << (*it).second.first <<" , "<< (*it).second.second<<" ) : \t";
  oC0->idPrint(it->first);

  oC0->loadHistosPair((*it).second.first);
  oC1->loadHistosPair((*it).second.second);
  oC0->displayPair(it->second.first, "Red");
  oC1->displayPair(it->second.second, "Blue");
  cout << label0<<endl;
  oC0->printSummaryPair(it->second.first);
  cout << endl<<label1<<endl;
  oC1->printSummaryPair(it->second.second);

}


//   char hn[32];
//   int n = idMap.size();
//   sprintf(hn,"sigMean");
//   predErrMeans = new TH1F(hn,hn,n,-0.5,n-0.5);
//   predErrMeans->GetXaxis()->SetTitle("module pair index");
//   predErrMeans->GetYaxis()->SetTitle("<#sigma(#Deltax_{pred}> [#mum]");
//   sprintf(hn,"sigMeanFirst");
//   predErrMeansFirst = new TH1F(hn,hn,n,-0.5,n-0.5);
//   predErrMeansFirst->GetXaxis()->SetTitle("module pair index");
//   predErrMeansFirst->GetYaxis()->SetTitle("<#sigma(x_{pred1}> [#mum]");
//   sprintf(hn,"sigMeanSecond");
//   predErrMeansSecond = new TH1F(hn,hn,n,-0.5,n-0.5);
//   predErrMeansSecond->GetXaxis()->SetTitle("module pair index");
//   predErrMeansSecond->GetYaxis()->SetTitle("<#sigma(x_{pred2}> [#mum]");
//   sprintf(hn,"simRec");
//   simRec = new TH1F(hn,hn,n,-0.5,n-0.5);
//   simRec->GetXaxis()->SetTitle("module pair index");
//   simRec->GetYaxis()->SetTitle("#sigma(SimHit-RecHit) [#mum]");
//   sprintf(hn,"simTrk");
//   simTrk = new TH1F(hn,hn,n,-0.5,n-0.5);
//   simTrk->GetXaxis()->SetTitle("module pair index");
//   simTrk->GetYaxis()->SetTitle("#sigma(SimHit-PredHit) [#mum]");
//   sprintf(hn,"mean");
//   meanDiffs = new TH1F(hn,hn,n,-0.5,n-0.5);
//   meanDiffs->GetXaxis()->SetTitle("module pair index");
//   meanDiffs->GetYaxis()->SetTitle("<#Deltax_{hit}-#Deltax_{pred}> [#mum]");
//   sprintf(hn,"meanPlus");
//   meanDiffsPlus = new TH1F(hn,hn,n,-0.5,n-0.5);
//   meanDiffsPlus->GetXaxis()->SetTitle("module pair index");
//   meanDiffsPlus->GetYaxis()->SetTitle("fitted mean #Deltax_{hit}-#Deltax_{pred} y>0[#mum]");
//   sprintf(hn,"meanMinus");
//   meanDiffsMinus = new TH1F(hn,hn,n,-0.5,n-0.5);
//   meanDiffsMinus->GetXaxis()->SetTitle("module pair index");
//   meanDiffsMinus->GetYaxis()->SetTitle("fitted mean #Deltax_{hit}-#Deltax_{pred} y<0[#mum]");
//   sprintf(hn,"width");
//   sigmaDiffs = new TH1F(hn,hn,n,-0.5,n-0.5);
//   sigmaDiffs->GetXaxis()->SetTitle("module pair index");
//   sigmaDiffs->GetYaxis()->SetTitle("fitted width #Deltax_{hit}-#Deltax_{pred} [#mum]");
//   hitErrMeans = new TH1F("hitSigX","hitSigX",n,-0.5,n-0.5);
//   dxdzMeans = new TH1F("dxdz","dxdz",n,-0.5,n-0.5);
//   layerHisto = new TH1F("layer","layer",n,-0.5,n-0.5);
//   stereoHisto = new TH1F("stereo","stereo",n,-0.5,n-0.5);
//   radHisto = new TH1F("radius","radius",n,-0.5,n-0.5);
//   phiHisto = new TH1F("phi","phi",n,-0.5,n-0.5);
//   zHisto = new TH1F("z","z",n,-0.5,n-0.5);
//   xHisto = new TH1F("x","x",n,-0.5,n-0.5);
//   yHisto = new TH1F("y","y",n,-0.5,n-0.5);
//   statHisto = new TH1F("stat","stat",n,-0.5,n-0.5);
//   doublePulls = new TH1F("doublePull","doublePull",n,-0.5,n-0.5);
//   hitPulls = new TH1F("hitPull","hitPull",n,-0.5,n-0.5);
//   predPulls = new TH1F("predPull","predPull",n,-0.5,n-0.5);
// 
//   hitErrMeans->GetYaxis()->SetTitle("#sigma(#Deltax_{hit}) [#mum]");
//   dxdzMeans->GetYaxis()->SetTitle("Local dx/dz");
//   layerHisto->GetYaxis()->SetTitle("Layer");
//   stereoHisto->GetYaxis()->SetTitle("Stereo");
//   radHisto->GetYaxis()->SetTitle("Radius [cm]");
//   phiHisto->GetYaxis()->SetTitle("#phi");
//   zHisto->GetYaxis()->SetTitle("z [cm]");
//   xHisto->GetYaxis()->SetTitle("Local x [cm]");
//   yHisto->GetYaxis()->SetTitle("Local y [cm]");
//   statHisto->GetYaxis()->SetTitle("Entries");
//   doublePulls->GetYaxis()->SetTitle("Pull DD");
//   hitPulls->GetYaxis()->SetTitle("Pull (#Deltax_{hit})");
//   predPulls->GetYaxis()->SetTitle("Pull (#Deltax_{pred})");
// 
// 
// #ifdef HISTOS2D
//   ddVsLocalYSlope = new TH1F("ddYslope","ddYslope",n,-0.5,n-0.5);
//   ddVsLocalYSlope->GetYaxis()->SetTitle("double diff vs local Y slope");
//   ddVsLocalYOffset = new TH1F("ddYoffset","ddYoffset",n,-0.5,n-0.5);
//   ddVsLocalYOffset->GetYaxis()->SetTitle("double diff vs local Y offset");
// 
//   ddVsDxdzSlope = new TH1F("ddDxslope","ddDxslope",n,-0.5,n-0.5);
//   ddVsDxdzSlope->GetYaxis()->SetTitle("double diff vs local dX/dZ slope");
//   ddVsDxdzOffset = new TH1F("ddDxoffset","ddDxoffset",n,-0.5,n-0.5);
//   ddVsDxdzOffset->GetYaxis()->SetTitle("double diff vs local dX/dZ offset");
// #endif
// 
//   vector<TH1F*> sPlots0, sPlots1;
//   vector<TH1F*> sPlotsSmall0, sPlotsSmall1;
// 
//   sPlots0.push_back(predErrMeans);
//   sPlots0.push_back(predErrMeansFirst);
//   sPlots0.push_back(predErrMeansSecond);
//   sPlots0.push_back(simRec);
//   sPlots0.push_back(simTrk);
//   sPlots0.push_back(meanDiffs);
//   sPlots0.push_back(meanDiffsPlus);
//   sPlots0.push_back(meanDiffsMinus);
//   sPlots0.push_back(sigmaDiffs);
//   sPlots0.push_back(hitErrMeans);
//   sPlots0.push_back(dxdzMeans);
//   sPlots0.push_back(layerHisto);
//   sPlots0.push_back(stereoHisto);
//   sPlots0.push_back(radHisto);
//   sPlots0.push_back(phiHisto);
//   sPlots0.push_back(zHisto);
//   sPlots0.push_back(xHisto);
//   sPlots0.push_back(yHisto);
//   sPlots0.push_back(statHisto);
//   sPlots0.push_back(doublePulls);
//   sPlots0.push_back(hitPulls);
//   sPlots0.push_back(predPulls);
// 
//   sPlots1.push_back(predErrMeans);
//   sPlots1.push_back(predErrMeansFirst);
//   sPlots1.push_back(predErrMeansSecond);
//   sPlots1.push_back(simRec);
//   sPlots1.push_back(simTrk);
//   sPlots1.push_back(meanDiffs);
//   sPlots1.push_back(meanDiffsPlus);
//   sPlots1.push_back(meanDiffsMinus);
//   sPlots1.push_back(sigmaDiffs);
//   sPlots1.push_back(hitErrMeans);
//   sPlots1.push_back(dxdzMeans);
//   sPlots1.push_back(layerHisto);
//   sPlots1.push_back(stereoHisto);
//   sPlots1.push_back(radHisto);
//   sPlots1.push_back(phiHisto);
//   sPlots1.push_back(zHisto);
//   sPlots1.push_back(xHisto);
//   sPlots1.push_back(yHisto);
//   sPlots1.push_back(statHisto);
//   sPlots1.push_back(doublePulls);
//   sPlots1.push_back(hitPulls);
//   sPlots1.push_back(predPulls);
// 
// #ifdef HISTOS2D
//   TH1* ddVsLocalYSlope;
//   TH1* ddVsLocalYOffset;
//   TH1* ddVsDxdzSlope;
//   TH1* ddVsDxdzOffset;
// #endif
// 
// 
// }
// 

