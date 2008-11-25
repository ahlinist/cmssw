#define overlapCheck_cxx
#include "overlapCheck.h"
#include <TH2.h>
#include <TArrow.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TLegend.h>

using namespace std;

void overlapCheck::searchDetId(unsigned int detId)
{
  if (fChain == 0) return;
  Long64_t nbytes = 0, nb = 0;
  int totEntries = fChain->GetEntriesFast();
  for (Long64_t jentry=0; jentry<totEntries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    DetIdPair idPair = getPair(jentry);
    if ((idPair.first==detId) || (idPair.second==detId) ) {
      cout << "Pair number: "<< jentry<<endl;
      idPrint(idPair);
    }
  }

}
  /**
   * Get the 2 detIds of an overlap from the tree
   */
overlapCheck::DetIdPair overlapCheck::getPair(int pairNbr)
{
  if (fChain == 0) return DetIdPair();
  Long64_t nentries = fChain->GetEntriesFast();
  if (pairNbr>=nentries) {
    cout << "Only "<< nentries <<" available\n";
    return DetIdPair();
  }
  Long64_t nbytes = 0, nb = 0;
  Long64_t ientry = LoadTree(pairNbr);
  if (ientry < 0) return DetIdPair();
  nb = fChain->GetEntry(pairNbr);   nbytes += nb;
  return DetIdPair(detids[0], detids[1]);
}

  /**
   * Print the full module info for the pair
   */
void overlapCheck::idPrint(DetIdPair idPair)
{
  cout << idPair.first<< " ( ";
  printId(idPair.first);
  cout <<" ) " << idPair.second << " ( ";
  printId(idPair.second);
  cout <<" )\n";
}

  /**
   * For an overlap, print the summary info and diaply all 
   * the distributions
   */
void overlapCheck::getPairInfo(int pairNbr)
{
  DetIdPair idPair = getPair(pairNbr);
  if (idPair.first==0) return;
  cout << "Pair number: "<< pairNbr<<endl;
  idPrint(idPair);

  loadHistosPair(pairNbr);
  if (! summaryOK) {
    cout << "No summary plots available\n";
    return;
  }
  AdjacentType type = checkAdjacentType(idPair.first, idPair.second);
  printSummaryPair(pairNbr, type);
  displayPair(pairNbr, type);

}

  /**
   * Print the summary info for a specific overlap
   */
void overlapCheck::printSummaryPair(int pairNbr, AdjacentType type)
{
  cout << "Statistics 	      : "<< statHisto->GetBinContent(pairNbr+1) << endl;
  cout << "Average R / Phi / z :"<< radHisto->GetBinContent(pairNbr+1)
    << " / "<< phiHisto->GetBinContent(pairNbr+1)
    << " / "<< zHisto->GetBinContent(pairNbr+1) << endl;
  cout <<"Average local x / y : "<< xHisto->GetBinContent(pairNbr+1)
    << " / "<< yHisto->GetBinContent(pairNbr+1) << endl;
  
  cout << "DD mean   : "<< meanDiffs->GetBinContent(pairNbr+1) 
       << " +/- "<< meanDiffs->GetBinError(pairNbr+1) << endl;
  cout << "DD width  : "<< sigmaDiffs->GetBinContent(pairNbr+1)
       << " +/- "<< sigmaDiffs->GetBinError(pairNbr+1) << endl;
//   cout << "meanDiffsPlus      : "<< meanDiffsPlus->GetBinContent(pairNbr+1) << endl;
//   cout << "meanDiffsMinus     : "<< meanDiffsMinus->GetBinContent(pairNbr+1) << endl;

  cout << "Mean unc of prediction : "<< predErrMeans->GetBinContent(pairNbr+1) << endl;
  cout << "Mean unc of pred mod 1 : "<< predErrMeansFirst->GetBinContent(pairNbr+1) << endl;
  cout << "Mean unc of pred mod 2 : "<< predErrMeansSecond->GetBinContent(pairNbr+1) << endl;
  cout << "Mean uncertainty on hit Diff: "<< hitErrMeans->GetBinContent(pairNbr+1) << endl;

  cout << "simRec 	      : "<< simRec->GetBinContent(pairNbr+1) << endl;
  cout << "simTrk 	      : "<< simTrk->GetBinContent(pairNbr+1) << endl;
  cout << "dxdzMeans 	      : "<< dxdzMeans->GetBinContent(pairNbr+1) << endl;
  cout << "doublePulls        : "<< doublePulls->GetBinContent(pairNbr+1) << endl;
  cout << "hitPulls 	      : "<< hitPulls->GetBinContent(pairNbr+1) << endl;
  cout << "predPulls 	      : "<< predPulls->GetBinContent(pairNbr+1) << endl;

#ifdef HISTOS2D
  cout << "DD vs. Local-X slope 	      : "<< ddVsLocalXSlope->GetBinContent(pairNbr+1) << endl;
  cout << "DD vs. Local-dY/dZ slope 	      : "<< ddVsDydzSlope->GetBinContent(pairNbr+1) << endl;

  if (type == parallelToStrips) {
    cout << "Overlap parallel to strips: DD vs. Local-Y slope : "<< ddVsLocalYSlope->GetBinContent(pairNbr+1) << endl;
  } else {
    cout << "Overlap perpendicular to strips: DD vs. Local-dX/dZ slope 	      : "<< ddVsDxdzSlope->GetBinContent(pairNbr+1) << endl;
  }
#endif
}

  /**
   * Display all the summary plots
   */
void overlapCheck::displaySummary()
{
  if (! summaryOK) {
    cout << "No summary plots available\n";
    return;
  }
  if (statUpdate) {
//     delete s1; delete s2; delete s3; delete s4; ; delete s5;
// #ifdef HISTOS2D
//     delete s6;
// #endif
  } else {
    s1 = new TCanvas("DDs","Summary Double differences", 900, 600);
    s2 = new TCanvas("Errs","Summary Uncertainties", 900, 600);
    s3 = new TCanvas("ps","Summary Pulls", 900, 600);
    s4 = new TCanvas("gps","Summary Global Positions", 900, 600);
    s5 = new TCanvas("lps","Summary Local positions", 900, 600);
  s1->Divide(1,2);
  s2->Divide(2,2);
  s3->Divide(2,2);
  s4->Divide(2,2);
  s5->Divide(2,2);
  } 
  s1->cd(1); meanDiffs->Draw("hist p");meanDiffs->Draw("same");
//   s1->cd(2); meanDiffsPlus->Draw();
//   s1->cd(3); meanDiffsMinus->Draw();
  s1->cd(2); sigmaDiffs->Draw("hist p");//sigmaDiffs->Draw("same");

  s2->cd(1); predErrMeans->Draw();
  s2->cd(2); predErrMeansFirst->Draw();
  s2->cd(3); predErrMeansSecond->Draw();
  s2->cd(4); hitErrMeans->Draw();

  s3->cd(1); statHisto->Draw();
  s3->cd(2); doublePulls->Draw();
  s3->cd(3); hitPulls->Draw();
  s3->cd(4); predPulls->Draw();

  s4->cd(1); layerHisto->Draw("p");
  s4->Update();
   //scale hint1 to the pad coordinates
   Float_t rightmax = 1.1*stereoHisto->GetMaximum();
   Float_t scale = s4->cd(1)->GetUymax()/rightmax;
   cout << s4->cd(1)->GetUymax()<<endl;
   stereoHisto->Scale(scale);
  stereoHisto->SetMarkerColor(2);
  stereoHisto->Draw("same p");
  s4->cd(2); radHisto->Draw();
  s4->cd(3); phiHisto->Draw();
  s4->cd(4); zHisto->Draw();

  s5->cd(1); xHisto->Draw();
  s5->cd(2); yHisto->Draw();
  s5->cd(3); dxdzMeans->Draw();
#ifdef HISTOS2D
  s6 = new TCanvas("DD2S","Summary 2D Double differences", 900, 600);
  s6->Divide(2,2);
  s6->cd(2); ddVsLocalYSlope->Draw();
//  if (ddVsDxdzSlope!=0) {
  s6->cd(1); ddVsLocalXSlope->Draw();
    s6->cd(3); ddVsDxdzSlope->Draw();
    s6->cd(4); ddVsDydzSlope->Draw();
//  }
#endif
 statUpdate = true;
}

  /**
   * Print all the summary plots to a ps file
   */

void overlapCheck::printSummaryToFile(TString name)
{
  if (! summaryOK) {
    cout << "No summary plots available\n";
    return;
  }
  if (!canvasUpdated) displaySummary();
  s1->Print(name+".ps[");
  s1->Draw();
  s1->Print(name+".ps");
  s2->Draw();
  s2->Print(name+".ps");
  s3->Draw();
  s3->Print(name+".ps");
  s4->Draw();
  s4->Print(name+".ps");
  s5->Draw();
  s5->Print(name+".ps");
  s5->Print(name+".ps]");
}

void overlapCheck::spotProblems()
{
  if (! summaryOK) {
    cout << "No summary plots available\n";
    return;
  }

  Long64_t nbytes = 0, nb = 0;
  int totEntries = fChain->GetEntriesFast();
  bool prb;

  for (Long64_t jentry=0; jentry<totEntries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    DetIdPair idPair = getPair(jentry);
    if (idPair.first==0) return;
    prb=false;
    
    prb=false;
    if ((meanDiffs->GetBinError(jentry+1)==0.)&&((meanDiffs->GetBinError(jentry+1)==0.))){
      cout << "DD info not filled, maybe an under/overflow problem\n";
      prb=true;
    }

    if (prb) {
      cout << "Pair number: "<< jentry<<" ( "<<jentry<<" ) ";
      idPrint(idPair); cout<<endl<<endl;
    }

  }
  
}

  /**
   * Display all the distributions for an overlap
   */
void overlapCheck::displayPair(int pairNbr, AdjacentType type, TString tag)
{
  if (canvasUpdated) {
//     delete c1; delete c2; //delete c3; 
//     //delete c4; 
// #ifdef HISTOS2D
//     delete c5; delete c6;
// #endif
  } else {
  c1 = new TCanvas("DD"+tag,"Double differences"+tag, 600, 600);
  c1->Divide(1,1);
  c2 = new TCanvas("Err"+tag,"Uncertainties"+tag, 900, 600);
  c2->Divide(3,2);
//   c3 = new TCanvas("Pulls"+tag,"Pulls and Local Positions"+tag, 900, 600);
//   c3->Divide(3,2);
//   c4 = new TCanvas("Pos"+tag,"Positions"+tag, 900, 600);
//   c4->Divide(3,2);
#ifdef HISTOS2D
  c5 = new TCanvas("DD2D"+tag,"2D Double differences"+tag, 600, 600);
  c5->Divide(2,2);
  if (localXVsLocalYHistos!=0) {
    c6 = new TCanvas("LPos2D"+tag,"2D Local Positions"+tag, 900, 600);
    c6->Divide(3,2);
  }
#endif
  }

  c1->cd(1); resHistos->Draw();

//   c1->cd(2); resHistosPlusY->Draw();
//   c1->cd(3); resHistosMinusY->Draw();
//   c1->cd(4); resHistosPlusX->Draw();
//   c1->cd(5); resHistosMinusX->Draw();

  c2->cd(1); predErrHistos->Draw();
  c2->cd(2); predErrHistosFirst->Draw();
  c2->cd(3); predErrHistosSecond->Draw();
  c2->cd(4); hitErrHistos->Draw();
  c2->cd(5); hitErrHistosFirst->Draw();
  c2->cd(6); hitErrHistosSecond->Draw();

//   c3->cd(1); doublePullHistos->Draw();
//   c3->cd(2); hitPullHistos->Draw();
//   c3->cd(3); predPullHistos->Draw();
//   c3->cd(4); posHistosLocal[0]->Draw();
//   c3->cd(5); posHistosLocal[1]->Draw();
//   c3->cd(6); dxdzHistos->Draw();

//   c4->cd(1); posHistos[0]->Draw();
//   c4->cd(2); posHistos[1]->Draw();
//   c4->cd(3); posHistos[2]->Draw();
  
#ifdef HISTOS2D
  c5->cd(1); ddVsLocalXHistos->Draw();
  drawFunc(pairNbr, ddVsLocalXSlope, ddVsLocalXOffset);

  c5->cd(2); ddVsLocalYHistos->Draw();
  if (type == parallelToStrips) drawFunc(pairNbr, ddVsLocalYSlope, ddVsLocalYOffset);
  c5->cd(3); ddVsDxdzHistos->Draw();
  if (type == perpendicularToStrips) drawFunc(pairNbr, ddVsDxdzSlope, ddVsDxdzOffset);

  c5->cd(4); ddVsDydzHistos->Draw();
  drawFunc(pairNbr, ddVsDydzSlope, ddVsDydzOffset);

  if (localXVsLocalYHistos!=0) {
    c6->cd(1); localXVsLocalYHistos->Draw();
    c6->cd(2); dxdzVsDydzHistos->Draw();
    c6->cd(3); localXVsDxdzHistos->Draw();
    c6->cd(4); localYVsDxdzHistos->Draw();
    c6->cd(5); localXVsDydzHistos->Draw();
    c6->cd(6); localYVsDydzHistos->Draw();
  }
#endif
  canvasUpdated = true;
}
void overlapCheck::drawFunc(int pairNbr, TH1* resultSlopeHisto, TH1* resultOffsetHisto)
{
  //delete f1;
  f1 = new TF1("f1","pol1",-10,10);
  f1->SetParameter(1, resultSlopeHisto->GetBinContent(pairNbr+1));
  f1->SetParameter(0, resultOffsetHisto->GetBinContent(pairNbr+1));
  f1->Draw("same");
//   cout <<resultSlopeHisto->GetBinContent(pairNbr+1) <<" "<<resultOffsetHisto->GetBinContent(pairNbr+1)<<endl;
}

  /**
   * Redo the computation of all the quantities for a specific pair 
   * from the different distributions
   */
void overlapCheck::computeSummaryPair(int i)
{
  const int kNotDraw = 1<<9;
    //
    // Choose DetId order with more entries and retrieve histogram pointers
    //
    TH1* resHisto = resHistos;
    TH1* resHistoPlusX = resHistosPlusX;
    TH1* resHistoMinusX = resHistosMinusX;
    TH1* resHistoPlusY = resHistosPlusY;
    TH1* resHistoMinusY = resHistosMinusY;
    TH1* predErrHisto = predErrHistos;
    TH1* predErrHistoFirst = predErrHistosFirst;
    TH1* predErrHistoSecond = predErrHistosSecond;
//     TH1* simRecHisto = simRecHistos;
//     TH1* simTrkHisto = simTrkHistos;
    TH1* hitErrHisto = hitErrHistos;
    TH1* dxdzHisto = dxdzHistos;
  TH1* posHisto[3];
  TH1* posHistoLocal[2];
    posHisto[0] = posHistos[0];
    posHisto[1] = posHistos[1];
    posHisto[2] = posHistos[2];
    posHistoLocal[0] = posHistosLocal[0];
    posHistoLocal[1] = posHistosLocal[1];
    TH1* doublePullHisto = doublePullHistos;
    TH1* hitPullHisto = hitPullHistos;
    TH1* predPullHisto = predPullHistos;
//     std::cout << "det IDs: " << detIdPairs_.first << " "
//	      << detIdPairs_.second << " ";
//     std::cout << resHisto->GetName() << " " << resHisto->GetEntries() << " ";
//     if ( resHisto->GetEntries()>0 )
//       std::cout << (resHisto->GetBinContent(0)+resHisto->GetBinContent(201))/
// 	resHisto->GetEntries() << " ";
//     else
//       std::cout << "0 ";
//     std::cout << std::endl;

    unsigned int subdet;
    unsigned int layer;
    unsigned int stereo;
    int part;
    
    DetIdPair idPair = getPair(i);

    unsigned int id = idPair.first;
    decode(id,subdet,layer,part,stereo);
    // sequential layer number in TIB & TOB (for debugging)
    int k(-1);
    if ( subdet==3 && layer>0 && layer<5 ) {
      k = layer;
    }
    else if ( subdet==5 && layer>0 && layer<7 ) {
      k = layer + 4;
    }

    //
    // fill summaries
    //
      // debug info on layers
      layerHisto->SetBinContent(i+1,k);
      stereoHisto->SetBinContent(i+1,stereo);
      // statistics
      statHisto->SetBinContent(i+1,resHisto->Integral());
      // global position
      fillMean(i+1,radHisto,posHisto[0]);
      fillMean(i+1,phiHisto,posHisto[1]);
      fillMean(i+1,zHisto,posHisto[2]);
      fillMean(i+1,xHisto,posHistoLocal[0]);
      fillMean(i+1,yHisto,posHistoLocal[1]);
//      if (k >= 1 && k <= 4) {
#ifdef HISTOS2D
      fillSlope(i+1,ddVsLocalXSlope, ddVsLocalXOffset, ddVsLocalXHistos);
      fillSlope(i+1,ddVsLocalYSlope, ddVsLocalYOffset, ddVsLocalYHistos);
      fillSlope(i+1,ddVsDxdzSlope, ddVsDxdzOffset, ddVsDxdzHistos);
      fillSlope(i+1,ddVsDydzSlope, ddVsDydzOffset, ddVsDydzHistos);
#endif
      //curDir->cd();
//      }
      // Gaussian fit to double-difference
      //

      //resHisto->Fit("gaus","Q0R","",resHisto->GetMean()-0.025,resHisto->GetMean()+0.025);
      resHisto->Fit("gaus","Q0R","",resHisto->GetMean()-3*resHisto->GetRMS(),resHisto->GetMean()+3*resHisto->GetRMS());
      //gStyle->SetOptFit(1011);
      //resHisto->Fit("gaus","LQ0R");
      resHisto->GetFunction("gaus")->ResetBit(kNotDraw);
      //cout << "for index " << i << " chisq is " << resHisto->GetFunction("gaus")->GetChisquare() << "  RMS is " << resHisto->GetRMS() << "  fit sigma is " << resHisto->GetFunction("gaus")->GetParameter(2)  << endl;
      double mDiff = resHisto->GetFunction("gaus")->GetParameter(1);
      double emDiff = resHisto->GetFunction("gaus")->GetParError(1);
      meanDiffs->SetBinContent(i+1,10000*mDiff);
      meanDiffs->SetBinError(i+1,10000*emDiff);

//  fillMean(i+1,meanDiffs,resHisto,10000.);

//       if( predY[0] > 0) { //need to sort out if predY>0 before the overlap summary
// 	//cout << "positive: predY = " << predY[0] << endl;
// 	meanDiffsPlus->SetBinContent(i+1,10000*mDiff);
// 	meanDiffsPlus->SetBinError(i+1,10000*emDiff);
//       }
//       if( predY[0] < 0) {
// 	//cout << "negative: predY = " << predY[0] << endl;
// 	meanDiffsMinus->SetBinContent(i+1,10000*mDiff);
// 	meanDiffsMinus->SetBinError(i+1,10000*emDiff);
//       }
      double sDiff = resHisto->GetFunction("gaus")->GetParameter(2);
      double esDiff = resHisto->GetFunction("gaus")->GetParError(2);
      sigmaDiffs->SetBinContent(i+1,10000*sDiff);
      sigmaDiffs->SetBinError(i+1,10000*esDiff);
      // predicted error // fold in some uncertainty from the spread (systematic)
      fillMean(i+1,predErrMeans,predErrHisto,10000.);
      // fillMeanWithSyst(i+1,predErrMeans,predErrHisto,predErrHistoFirst,10000.);
      //predicted error for each overlap module individually
      fillMean(i+1,predErrMeansFirst,predErrHistoFirst,10000.);
      fillMean(i+1,predErrMeansSecond,predErrHistoSecond,10000.);
//       fillWidth(i+1,simRec,simRecHisto,10000);
//       fillWidth(i+1,simTrk,simTrkHisto,10000);
      // hit error
      fillMean(i+1,hitErrMeans,hitErrHisto,10000.);
      // local dxdz
      fillMean(i+1,dxdzMeans,dxdzHisto);
      // double pull
      fillWidth(i+1,doublePulls,doublePullHisto,1.);
      // hit pull
      fillWidth(i+1,hitPulls,hitPullHisto,1.);
      // pred pull
//       fillWidth(i+1,predPulls,predPullHisto,1.);
      predPulls->SetBinContent(i+1,predPullHisto->GetRMS());

      //print out to check the ordering of the index within each layer
//             std::cout << "Index: " << i << "   Layer: " << k << "   Phi: " << posHistos[1]->GetMean() << "   r: " << posHistos[0]->GetMean() << "   z: " << posHistos[2]->GetMean() << "  dx/dz: " << dxdzHisto->GetMean() << "   detID = " << detIdPairs_[i].first << " and " << detIdPairs_[i].second << std::endl;
      //    std::cout << " geographical ID: " << id << std::endl;

//       predErrMeans->SetBinContent(i+1,predErrHisto->GetMean());
//       predErrMeans->SetBinError(i+1,predErrHisto->GetRMS()/predErrHisto->Integral());
//       hitErrMeans->SetBinContent(i+1,hitErrHisto->GetMean());
//       hitErrMeans->SetBinError(i+1,hitErrHisto->GetRMS()/hitErrHisto->Integral());
//       double pred2Mean = predErrHisto->GetMean();
//       double pred2Sigma = predErrHisto->GetRMS()/sqrt(predErrHisto->Integral());
//       double sum2Mean = resHisto->GetFunction("gaus")->GetParameter(2);
//       double sum2Sigma = 2*sum2Mean*resHisto->GetFunction("gaus")->GetParError(2);
//       sum2Mean *= sum2Mean;
//       double diffMean = sum2Mean - pred2Mean;
//       diffMean = diffMean>0 ? sqrt(diffMean) : -sqrt(-diffMean);
//       double diffSigma = sqrt(sum2Sigma*sum2Sigma+pred2Sigma*pred2Sigma)/2./fabs(diffMean);
//       predErrMeans->SetBinContent(i+1,diffMean);
//       predErrMeans->SetBinError(i+1,diffSigma);

  }

  /**
   * Recreate the summary plots for all pairs from the distributions.
   * These will be written to a new file
   */
void overlapCheck::recreateSummaryPlots(TString file, unsigned int det, int sign, unsigned int layer)
{
  if (fChain == 0) return;
  int n = 0;
  Long64_t nbytes = 0, nb = 0;
  unsigned int newdet, newlayer, stereo;
  int newsign;
  cout << det<<endl;
  int totEntries = fChain->GetEntriesFast();
  if (det<1) {
    n = totEntries;
    partialPlots = true;
  } else {
    for (Long64_t jentry=0; jentry<totEntries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      cout <<ientry<<endl;
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      DetIdPair idPair = getPair(jentry);
      cout <<idPair.first<<endl;
      if (idPair.first==0) return;
      
      decode (idPair.first, newdet, newlayer, newsign, stereo);
//       idPrint(idPair);cout <<newdet<<det;
//       cout << ((det==newdet)&&(sign==0 || sign==newsign) && (layer == 0 || layer==newlayer));
//       cout<<
//       (det==newdet)<<(sign==0) << (sign==newsign) << (layer == 0) << (layer==newlayer)<<endl;

      if ((det==newdet)&&(sign==0 || sign==newsign) && (layer == 0 || layer==newlayer)) ++n;
    }
    partialPlots = true;
  }
cout << n<<endl;
  cout <<"Open file\n";
  TFile * outFile = new TFile(file, "RECREATE");
  cout <<"File opened\n";

  char hn[32];
  sprintf(hn,"sigMean");
  predErrMeans = new TH1F(hn,hn,n,-0.5,n-0.5);
  predErrMeans->GetXaxis()->SetTitle("module pair index");
  predErrMeans->GetYaxis()->SetTitle("<#sigma(#Deltax_{pred}> [#mum]");
  sprintf(hn,"sigMeanFirst");
  predErrMeansFirst = new TH1F(hn,hn,n,-0.5,n-0.5);
  predErrMeansFirst->GetXaxis()->SetTitle("module pair index");
  predErrMeansFirst->GetYaxis()->SetTitle("<#sigma(x_{pred1}> [#mum]");
  sprintf(hn,"sigMeanSecond");
  predErrMeansSecond = new TH1F(hn,hn,n,-0.5,n-0.5);
  predErrMeansSecond->GetXaxis()->SetTitle("module pair index");
  predErrMeansSecond->GetYaxis()->SetTitle("<#sigma(x_{pred2}> [#mum]");
  sprintf(hn,"simRec");
  simRec = new TH1F(hn,hn,n,-0.5,n-0.5);
  simRec->GetXaxis()->SetTitle("module pair index");
  simRec->GetYaxis()->SetTitle("#sigma(SimHit-RecHit) [#mum]");
  sprintf(hn,"simTrk");
  simTrk = new TH1F(hn,hn,n,-0.5,n-0.5);
  simTrk->GetXaxis()->SetTitle("module pair index");
  simTrk->GetYaxis()->SetTitle("#sigma(SimHit-PredHit) [#mum]");
  sprintf(hn,"mean");
  meanDiffs = new TH1F(hn,hn,n,-0.5,n-0.5);
  meanDiffs->GetXaxis()->SetTitle("module pair index");
  meanDiffs->GetYaxis()->SetTitle("<#Deltax_{hit}-#Deltax_{pred}> [#mum]");
  sprintf(hn,"meanPlus");
  meanDiffsPlus = new TH1F(hn,hn,n,-0.5,n-0.5);
  meanDiffsPlus->GetXaxis()->SetTitle("module pair index");
  meanDiffsPlus->GetYaxis()->SetTitle("fitted mean #Deltax_{hit}-#Deltax_{pred} y>0[#mum]");
  sprintf(hn,"meanMinus");
  meanDiffsMinus = new TH1F(hn,hn,n,-0.5,n-0.5);
  meanDiffsMinus->GetXaxis()->SetTitle("module pair index");
  meanDiffsMinus->GetYaxis()->SetTitle("fitted mean #Deltax_{hit}-#Deltax_{pred} y<0[#mum]");
  sprintf(hn,"width");
  sigmaDiffs = new TH1F(hn,hn,n,-0.5,n-0.5);
  sigmaDiffs->GetXaxis()->SetTitle("module pair index");
  sigmaDiffs->GetYaxis()->SetTitle("fitted width #Deltax_{hit}-#Deltax_{pred} [#mum]");
  hitErrMeans = new TH1F("hitSigX","hitSigX",n,-0.5,n-0.5);
  dxdzMeans = new TH1F("dxdz","dxdz",n,-0.5,n-0.5);
  layerHisto = new TH1F("layer","layer",n,-0.5,n-0.5);
  stereoHisto = new TH1F("stereo","stereo",n,-0.5,n-0.5);
  radHisto = new TH1F("radius","radius",n,-0.5,n-0.5);
  phiHisto = new TH1F("phi","phi",n,-0.5,n-0.5);
  zHisto = new TH1F("z","z",n,-0.5,n-0.5);
  xHisto = new TH1F("x","x",n,-0.5,n-0.5);
  yHisto = new TH1F("y","y",n,-0.5,n-0.5);
  statHisto = new TH1F("stat","stat",n,-0.5,n-0.5);
  doublePulls = new TH1F("doublePull","doublePull",n,-0.5,n-0.5);
  hitPulls = new TH1F("hitPull","hitPull",n,-0.5,n-0.5);
  predPulls = new TH1F("predPull","predPull",n,-0.5,n-0.5);

  hitErrMeans->GetYaxis()->SetTitle("#sigma(#Deltax_{hit}) [#mum]");
  dxdzMeans->GetYaxis()->SetTitle("Local dx/dz");
  layerHisto->GetYaxis()->SetTitle("Layer");
  stereoHisto->GetYaxis()->SetTitle("Stereo");
  radHisto->GetYaxis()->SetTitle("Radius [cm]");
  phiHisto->GetYaxis()->SetTitle("#phi");
  zHisto->GetYaxis()->SetTitle("z [cm]");
  xHisto->GetYaxis()->SetTitle("Local x [cm]");
  yHisto->GetYaxis()->SetTitle("Local y [cm]");
  statHisto->GetYaxis()->SetTitle("Entries");
  doublePulls->GetYaxis()->SetTitle("Pull DD");
  hitPulls->GetYaxis()->SetTitle("Pull (#Deltax_{hit})");
  predPulls->GetYaxis()->SetTitle("Pull (#Deltax_{pred})");


#ifdef HISTOS2D
  ddVsLocalXSlope = new TH1F("ddXslope","ddXslope",n,-0.5,n-0.5);
  ddVsLocalXSlope->GetXaxis()->SetTitle("double diff vs local X slope");
  ddVsLocalXOffset = new TH1F("ddXoffset","ddXoffset",n,-0.5,n-0.5);
  ddVsLocalXOffset->GetXaxis()->SetTitle("double diff vs local X offset");
  ddVsLocalYSlope = new TH1F("ddYslope","ddYslope",n,-0.5,n-0.5);
  ddVsLocalYSlope->GetYaxis()->SetTitle("double diff vs local Y slope");
  ddVsLocalYOffset = new TH1F("ddYoffset","ddYoffset",n,-0.5,n-0.5);
  ddVsLocalYOffset->GetYaxis()->SetTitle("double diff vs local Y offset");

  ddVsDxdzSlope = new TH1F("ddDxslope","ddDxslope",n,-0.5,n-0.5);
  ddVsDxdzSlope->GetYaxis()->SetTitle("double diff vs local dX/dZ slope");
  ddVsDxdzOffset = new TH1F("ddDxoffset","ddDxoffset",n,-0.5,n-0.5);
  ddVsDxdzOffset->GetYaxis()->SetTitle("double diff vs local dX/dZ offset");
  ddVsDydzSlope = new TH1F("ddDyslope","ddDyslope",n,-0.5,n-0.5);
  ddVsDydzSlope->GetYaxis()->SetTitle("double diff vs local dy/dZ slope");
  ddVsDydzOffset = new TH1F("ddDyoffset","ddDyoffset",n,-0.5,n-0.5);
  ddVsDydzOffset->GetYaxis()->SetTitle("double diff vs local dy/dZ offset");
#endif

  nbytes = 0, nb = 0, n=0;
  for (Long64_t jentry=0; jentry<totEntries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    DetIdPair idPair = getPair(jentry);
    if (idPair.first==0) return;

    decode (idPair.first, newdet, newlayer, newsign, stereo);
    if ((det==0 || det==newdet)&&(sign==0 || sign==newsign) && (layer == 0 || layer==newlayer)) {
      cout << "Pair number: "<< jentry<<" ( "<<n<<" ) ";
      idPrint(idPair); cout<<endl;
      loadHistosPair(jentry);
      computeSummaryPair(n);
      ++n;
    }

  }
  outFile->Write();
  summaryOK = true;
}

  /**
   * Plot the positions of the overlap modules
   */
void overlapCheck::scan()
{
  if (! summaryOK) {
    cout << "No summary plots available\n";
    return;
  }
  rzScan = new TH2F("rzScan", "rzScan", 400, -100, 100., 200, 0., 120.);
  xyScan = new TH2F("xyScan", "xyScan", 200, -120., 120., 200, -120., 120.);
  rzScan->GetXaxis()->SetTitle("z [cm]"); rzScan->GetYaxis()->SetTitle("r [cm]");
  xyScan->GetXaxis()->SetTitle("x [cm]"); xyScan->GetYaxis()->SetTitle("y [cm]");
  for (int i = 1; i < radHisto->GetEntries()+1;++i)
  {
    rzScan->Fill(zHisto->GetBinContent(i), radHisto->GetBinContent(i));
    xyScan->Fill(radHisto->GetBinContent(i) * cos (phiHisto->GetBinContent(i)),
    	radHisto->GetBinContent(i) * sin (phiHisto->GetBinContent(i)) );


  }
  TCanvas * scanC = new TCanvas("scan","Scan", 1200, 600);
  scanC->Divide(2,1);
  scanC->cd(1);xyScan->Draw(); 
  scanC->cd(2);rzScan->Draw(); 
}

  /**
   * For a specific overlap, load all the distributions from the file
   */ 
void overlapCheck::loadHistosPair(int pairNbr)
{
 // Load distributions:
  char hn[32];

//   TDirectory* curDir = gDirectory;

  baseDir->cd();
  TH1* resHistosV[2];
  gFile->cd("ResHistos");
  cout <<"a\n";
    sprintf(hn,"rh%4.4d",pairNbr);
    resHistos = (TH1*) gDirectory->Get(hn);
    sprintf(hn,"rplusXh%4.4d",pairNbr);
    resHistosPlusX = (TH1*) gDirectory->Get(hn);
    sprintf(hn,"rminusXh%4.4d",pairNbr);
    resHistosMinusX = (TH1*) gDirectory->Get(hn);
    cout << hn<<" "<<resHistosMinusX<<endl;
    sprintf(hn,"rplusYh%4.4d",pairNbr);
    resHistosPlusY = (TH1*) gDirectory->Get(hn);
    sprintf(hn,"rminusYh%4.4d",pairNbr);
    resHistosMinusY = (TH1*) gDirectory->Get(hn);
    cout << hn<<" "<<resHistosMinusY<<endl;

    gFile->cd();
    gFile->cd("PredHistos");
    sprintf(hn,"pSig%4.4d",pairNbr);
    predErrHistos = (TH1*) gDirectory->Get(hn);
    sprintf(hn,"pSigFirst%4.4d",pairNbr);
    predErrHistosFirst = (TH1*) gDirectory->Get(hn);
    sprintf(hn,"pSigSecond%4.4d",pairNbr);
    predErrHistosSecond = (TH1*) gDirectory->Get(hn);

    gFile->cd();
    gFile->cd("HitHistos");
    sprintf(hn,"hSig%4.4d",pairNbr);
    hitErrHistos = (TH1*) gDirectory->Get(hn);
    sprintf(hn,"hSigFirst%4.4d",pairNbr);
    hitErrHistosFirst = (TH1*) gDirectory->Get(hn);
    sprintf(hn,"hSigSecond%4.4d",pairNbr);
    hitErrHistosSecond = (TH1*) gDirectory->Get(hn);

    gFile->cd();
    gFile->cd("PosHistos");
    sprintf(hn,"rad%4.4d",pairNbr);
    posHistos[0] = (TH1*) gDirectory->Get(hn);
    sprintf(hn,"phi%4.4d",pairNbr);
    posHistos[1] = (TH1*) gDirectory->Get(hn);
    sprintf(hn,"z%4.4d",pairNbr);
    posHistos[2] = (TH1*) gDirectory->Get(hn);
    sprintf(hn,"x%4.4d",pairNbr);
    posHistosLocal[0] = (TH1*) gDirectory->Get(hn);
    sprintf(hn,"y%4.4d",pairNbr);
    posHistosLocal[1] = (TH1*) gDirectory->Get(hn);

   gFile->cd();
    gFile->cd("PullHistos");
    sprintf(hn,"doublePull%4.4d",pairNbr);
    doublePullHistos = (TH1*) gDirectory->Get(hn);
    sprintf(hn,"hitPull%4.4d",pairNbr);
    hitPullHistos = (TH1*) gDirectory->Get(hn);
    sprintf(hn,"predPull%4.4d",pairNbr);
    predPullHistos = (TH1*) gDirectory->Get(hn);


   gFile->cd();
    gFile->cd("ResidualHistograms");
//     sprintf(hn,"hSimRec2%4.4d",pairNbr);
//     simRecHistos = (TH1*) gDirectory->Get(hn);
//     sprintf(hn,"hSimTrk2%4.4d",pairNbr);
//     simTrkHistos = (TH1*) gDirectory->Get(hn);
    sprintf(hn,"hDxdz%4.4d",pairNbr);
    dxdzHistos = (TH1*) gDirectory->Get(hn);
  cout <<"a\n";

    resHistos->GetXaxis()->SetTitle("(#Deltax_{hit}-#Deltax_{pred}) [cm]");
  cout <<"a2\n";
    resHistosPlusY->GetXaxis()->SetTitle("(#Deltax_{hit}-#Deltax_{pred}) [cm] (y>0)");
  cout <<"a3\n";
    resHistosMinusY->GetXaxis()->SetTitle("(#Deltax_{hit}-#Deltax_{pred}) [cm] (y<0)");
  cout <<"a4\n";
    resHistosPlusX->GetXaxis()->SetTitle("(#Deltax_{hit}-#Deltax_{pred}) [cm] (x>0)");
  cout <<"a5\n";
    resHistosMinusX->GetXaxis()->SetTitle("(#Deltax_{hit}-#Deltax_{pred}) [cm] (x<0)");
  cout <<"a\n";

    predErrHistos->GetXaxis()->SetTitle("#sigma(#Deltax_{pred}) [#mum]");
    predErrHistosFirst->GetXaxis()->SetTitle("#sigma(x_{pred1}) [#mum]");
    predErrHistosSecond->GetXaxis()->SetTitle("#sigma(x_{pred2}) [#mum]");
    hitErrHistos->GetXaxis()->SetTitle("#sigma(#Deltax_{hit}) [#mum]");
    hitErrHistosFirst->GetXaxis()->SetTitle("#sigma(#Deltax_{hit1}) [#mum]");
    hitErrHistosSecond->GetXaxis()->SetTitle("#sigma(#Deltax_{hit2}) [#mum]");
  cout <<"a\n";
//     simRecHistos
//     simTrkHistos;
    dxdzHistos->GetXaxis()->SetTitle("Local dx/dz");
    posHistos[0]->GetXaxis()->SetTitle("R [cm]");
    posHistos[1]->GetXaxis()->SetTitle("#phi");
    posHistos[2]->GetXaxis()->SetTitle("z [cm]");
    posHistosLocal[0]->GetXaxis()->SetTitle("Local x [cm]");
    posHistosLocal[1]->GetXaxis()->SetTitle("Local y [cm]");
    doublePullHistos->GetXaxis()->SetTitle("Pull DD");
    hitPullHistos->GetXaxis()->SetTitle("Pull (#Deltax_{hit})");
    predPullHistos->GetXaxis()->SetTitle("Pull (#Deltax_{pred})");

#ifdef HISTOS2D
    gFile->cd("CorrelationHistos");
    sprintf(hn,"resVsAngle%4.4d",pairNbr);
    resVsAngleHistos = (TH2*) gDirectory->Get(hn);
    sprintf(hn,"ddVsLocalX%4.4d",pairNbr);
    ddVsLocalXHistos = (TH2*) gDirectory->Get(hn);
    sprintf(hn,"ddVsLocalY%4.4d",pairNbr);
    ddVsLocalYHistos = (TH2*) gDirectory->Get(hn);
    sprintf(hn,"ddVsDxdz%4.4d",pairNbr);
    ddVsDxdzHistos = (TH2*) gDirectory->Get(hn);
    sprintf(hn,"ddVsDydz%4.4d",pairNbr);
    ddVsDydzHistos = (TH2*) gDirectory->Get(hn);

    resVsAngleHistos->GetXaxis()->SetTitle("dx/dz");		resVsAngleHistos->GetYaxis()->SetTitle("(#Deltax_{hit}-#Deltax_{pred}) [cm]");
    ddVsLocalXHistos->GetXaxis()->SetTitle("Local x [cm]");	ddVsLocalXHistos->GetYaxis()->SetTitle("(#Deltax_{hit}-#Deltax_{pred}) [cm]");
    ddVsLocalYHistos->GetXaxis()->SetTitle("Local y [cm]");	ddVsLocalYHistos->GetYaxis()->SetTitle("(#Deltax_{hit}-#Deltax_{pred}) [cm]");
    ddVsDxdzHistos->GetXaxis()->SetTitle("dx/dz");		ddVsDxdzHistos->GetYaxis()->SetTitle("(#Deltax_{hit}-#Deltax_{pred}) [cm]");
    ddVsDydzHistos->GetXaxis()->SetTitle("dy/dz");		ddVsDydzHistos->GetYaxis()->SetTitle("(#Deltax_{hit}-#Deltax_{pred}) [cm]");

    sprintf(hn,"localXVsLocalY%4.4d",pairNbr);
    localXVsLocalYHistos = (TH2*) gDirectory->Get(hn);
    if (localXVsLocalYHistos!=0) {
      sprintf(hn,"dxdzVsDydz%4.4d",pairNbr);
      dxdzVsDydzHistos = (TH2*) gDirectory->Get(hn);
      sprintf(hn,"localXVsDxdz%4.4d",pairNbr);
      localXVsDxdzHistos = (TH2*) gDirectory->Get(hn);
      sprintf(hn,"localYVsDxdz%4.4d",pairNbr);
      localYVsDxdzHistos = (TH2*) gDirectory->Get(hn);
      sprintf(hn,"localXVsDydz%4.4d",pairNbr);
      localXVsDydzHistos = (TH2*) gDirectory->Get(hn);
      sprintf(hn,"localYVsDydz%4.4d",pairNbr);
      localYVsDydzHistos = (TH2*) gDirectory->Get(hn);
      sprintf(hn,"dPreddSimVsdHitdSim%4.4d",pairNbr);
      dPreddSimVsdHitdSimHistos = (TH2*) gDirectory->Get(hn);
      localXVsLocalYHistos->GetXaxis()->SetTitle("Local x [cm]");  localXVsLocalYHistos->GetYaxis()->SetTitle("Local y [cm]");
      dxdzVsDydzHistos->GetXaxis()->SetTitle("dy/dz");		dxdzVsDydzHistos->GetYaxis()->SetTitle("dx/dz");
      localXVsDxdzHistos->GetXaxis()->SetTitle("dx/dz");	localXVsDxdzHistos->GetYaxis()->SetTitle("Local x [cm]");
      localYVsDxdzHistos->GetXaxis()->SetTitle("dx/dz");	localYVsDxdzHistos->GetYaxis()->SetTitle("Local y [cm]");
      localXVsDydzHistos->GetXaxis()->SetTitle("dy/dz");	localXVsDydzHistos->GetYaxis()->SetTitle("Local x [cm]");
      localYVsDydzHistos->GetXaxis()->SetTitle("dy/dz");	localYVsDydzHistos->GetYaxis()->SetTitle("Local y [cm]");
      dPreddSimVsdHitdSimHistos->GetXaxis()->SetTitle("");	dPreddSimVsdHitdSimHistos->GetYaxis()->SetTitle("");
    }

#endif

}

void overlapCheck::presentation (TH1* h, bool wErr)
{

  s1 = new TCanvas("DDs","Summary Double differences", 900, 600);
  h->Draw("hist p");
  if (wErr) h->Draw("same");
  addLayer(h);
}

void overlapCheck::addLayer (TH1* h)
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
  int n = 0;
  Long64_t nbytes = 0, nb = 0;
  int totEntries = fChain->GetEntriesFast();
  Long64_t jentry;
    for (jentry=0; jentry<totEntries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      DetIdPair idPair = getPair(jentry);
      //cout <<idPair.first<<endl;
      if (idPair.first==0) return;
      
      
      decode (idPair.first, newdet, newlayer, newsign, stereo);
     if ( newdet==5  ) newlayer = newlayer + 4;

      if (newlayer>last){
	TArrow *ar1 = new TArrow(lastPos-0.4, y, jentry-0.6,y,0.04,"<|>");
	ar1->SetArrowSize(0.02); ar1->SetAngle(40);
	ar1->SetLineWidth(2); ar1->SetLineColor(last); ar1->Draw();
	ar1->SetFillStyle(1001); ar1->SetFillColor(last);
        sprintf(hn,"%d",last);
	TLatex* taga = new TLatex((lastPos+jentry)/2-1, y*0.95, hn);
	taga->SetTextSize(0.04); taga->SetTextColor(last);
	taga->Draw();
        last = newlayer;
	lastPos = jentry;
      }
    }
    
      
     TArrow *ar1 = new TArrow(lastPos-0.4, y, jentry-0.6,y,0.04,"<|>");
     ar1->SetArrowSize(0.02); ar1->SetAngle(40);
     ar1->SetLineWidth(2); ar1->SetLineColor(last); ar1->Draw();
     ar1->SetFillStyle(1001); ar1->SetFillColor(last);
     ar1->SetFillStyle(1001); ar1->SetFillColor(last);
     sprintf(hn,"%d",last);
     TLatex* taga = new TLatex((lastPos+jentry)/2-1, y*0.9, hn);
     taga->SetTextSize(0.04); taga->SetTextColor(newlayer);
     taga->Draw();

}

void overlapCheck::sigmas ()
{  
  s1 = new TCanvas("DDs","Summary Double differences", 900, 600);
  sigmaDiffs->Draw("hist p");
  //sigmaDiffs->Draw("same");
  
  sigmaDiffs->SetMinimum(0);
  sigmaDiffs->SetMaximum(200);
  predErrMeans->SetMarkerStyle(24);    //20
  predErrMeans->SetMarkerColor(2);
  sigmaDiffs->SetMarkerStyle(26);  //22
  hitErrMeans->SetLineColor(4);
  hitErrMeans->SetMarkerStyle(25);  //21
  hitErrMeans->SetMarkerColor(4);
//   sigmaDiffs->Draw();
  predErrMeans->Draw("hist p same");
  hitErrMeans->Draw("hist p same");

  addLayer(sigmaDiffs);
  TLegend* leg_hist = new TLegend(0.60,0.72,0.94,0.91);
  leg_hist->AddEntry(sigmaDiffs,"Fitted width (#Deltax_{hit}-#Deltax_{pred})","p");
  leg_hist->AddEntry(predErrMeans,"<#sigma(#Deltax_{pred})>","p");
  leg_hist->AddEntry(hitErrMeans,"<#sigma(#Deltax_{hit})>","p");
  leg_hist->SetTextSize(0.045);
  leg_hist->SetFillColor(0);
  leg_hist->Draw("same");
//   s1->cd(4); sigmaDiffs->Draw();("hist p");//sigmaDiffs->Draw("same");
// 
//   s2->cd(1); predErrMeans->Draw();
//   s2->cd(2); predErrMeansFirst->Draw();
//   s2->cd(3); predErrMeansSecond->Draw();
//   s2->cd(4); hitErrMeans->Draw();
}

void overlapCheck::checkAll (TString name)
{

    TCanvas* cc = new TCanvas("DDs","Summary Double differences", 600, 900);
    cc->Divide(2,4);
    cc->Print(name+".ps[");
  TStyle *style = new TStyle("tdrStyle","Style for P-TDR");
  style->SetPaperSize(21.,28.);
    style->SetOptTitle(0);


  char hn[3];
  int n = 0;
  int totEntries = fChain->GetEntriesFast();
  Long64_t jentry;
    for (jentry=0; jentry<totEntries;jentry++) {
      DetIdPair idPair = getPair(jentry);
      if (idPair.first==0) return;
      cout << "Pair number: "<< jentry<<endl;
      idPrint(idPair);
      loadHistosPair(jentry);
      ++n;
      cc->cd(n);
      resHistos->Draw();
      if (n==8){
	cc->Print(name+".ps");
	n=0;
      }
    }
  cc->Print(name+".ps]");
  delete cc;
    
         style->SetOptTitle(0);
 

}
