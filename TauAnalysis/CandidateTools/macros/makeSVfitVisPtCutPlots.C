
#include <TFile.h>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraphAsymmErrors.h>
#include <TF1.h>
#include <TF2.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TMath.h>
#include <TROOT.h>
#include <TStyle.h>

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <assert.h>

TH1* getHistogram(TFile* inputFile, const TString& directory, const TString& histogramName)
{  
  TString histogramName_full = TString(directory);
  if ( !histogramName_full.EndsWith("/") ) histogramName_full.Append("/");
  histogramName_full.Append(histogramName);

  TH1* histogram = (TH1*)inputFile->Get(histogramName_full.Data());

  if ( histogram && !histogram->GetSumw2N() ) histogram->Sumw2();
  else if ( !histogram) 
    std::cerr << "Failed to load histogram = " << histogramName_full << " from file = " << inputFile->GetName() << " !!" << std::endl;

  return histogram;
}

TH1* getHistogram(TFile* inputFile, const TString& directory, double massPoint, const TString& histogramName)
{
  TString directory_full = TString(directory);
  if ( !directory_full.EndsWith("/") ) directory_full.Append("/");
  directory_full.Append(Form("massEq%1.0fGeV", massPoint));
  return getHistogram(inputFile, directory_full, histogramName);
}

TH1* compRatioHistogram(const TString& ratioHistogramName, const TH1* numerator, const TH1* denominator)
{
  assert(numerator->GetDimension() == denominator->GetDimension());
  assert(numerator->GetNbinsX() == denominator->GetNbinsX());
  if ( denominator->GetDimension() >= 2 ) assert(numerator->GetNbinsY() == denominator->GetNbinsY());

  TH1* histogramRatio = (TH1*)numerator->Clone(ratioHistogramName.Data());
  histogramRatio->Divide(denominator);

  return histogramRatio;
}

TF1* fit1d(TH1* histogram, std::vector<std::pair<double, double> >* fitParameter)
{
  TString fitFunctionName = TString(histogram->GetName()).Append("_fit");
  TAxis* xAxis = histogram->GetXaxis();  
  double xMin = xAxis->GetXmin();
  if ( xMin < 0. ) xMin = 0.;
  double xMax = xAxis->GetXmax();
  TF1* fitFunction = 
    new TF1(fitFunctionName.Data(), 
	    "[0]*0.5*(1.0 + [1]*x)*(1.0 - TMath::Erf(-[2]*(x - [3])))", xMin, xMax);

//--- determine initial values for function parameters:
//    find x-value where histogram reaches 50% of maximum
  TString histogramName_rebinned = TString(histogram->GetName()).Append("_rebinned");
  TH1* histogram_rebinned = (TH1*)histogram->Clone();
  //histogram_rebinned->Rebin(histogram->GetNbinsX()/20);
  double yMin = +1.e+6;
  double yMax = -1.e+6;
  TAxis* xAxis_rebinned = histogram_rebinned->GetXaxis();
  int numBins_rebinned = xAxis_rebinned->GetNbins();
  for ( int iBin = 1; iBin <= numBins_rebinned; ++iBin ) {
    double binContent = histogram_rebinned->GetBinContent(iBin);
    if ( binContent < yMin ) yMin = binContent;
    if ( binContent > yMax ) yMax = binContent;
  }
  //std::cout << "yMin = " << yMin << ", yMax = " << yMax << std::endl;
  double xHalfMax = 0.;
  int numPointsHalfMax = 0;
  bool lastBinGtHalfMax;
  for ( int iBin = 1; iBin <= numBins_rebinned; ++iBin ) {
    double x = xAxis_rebinned->GetBinCenter(iBin);
    double binContent = histogram_rebinned->GetBinContent(iBin);
    bool binGtHalfMax = (binContent > 0.5*(yMin + yMax));    
    //std::cout << "x = " << x << ": binGtHalfMax = " << binGtHalfMax << ", lastBinGtHalfMax = " << lastBinGtHalfMax << std::endl;
    if ( iBin > 1 && ((binGtHalfMax && !lastBinGtHalfMax) || (lastBinGtHalfMax && !binGtHalfMax)) ) {
      // CV: require that y-value is close to 50% of maximum,
      //     in order to reject rising/falling edges with low event statistics
      //    (causing "unphysical" 50% points to be found)
      if ( x > 0.10 && binContent > 0.2*(yMin + yMax) && binContent < 0.8*(yMin + yMax) ) {
	//std::cout << "--> adding xHalfMax = " << x << std::endl;
	xHalfMax += x;
	++numPointsHalfMax;
      }
    }
    lastBinGtHalfMax = binGtHalfMax;
  }
  
  if ( numPointsHalfMax >= 1 ) {
    xHalfMax /= numPointsHalfMax;
  } else {
    xHalfMax = 0.5*(xMin + xMax);
  }
  //std::cout << "xHalfMax = " << xHalfMax << std::endl;
  int binHalfMax = histogram->FindBin(xHalfMax);

  double p0_initial = 0.5*(yMin + yMax);
  double p1_initial = 0.;
  double p2_initial;
  if ( histogram->Integral(1, binHalfMax) > histogram->Integral(binHalfMax, histogram->GetNbinsX()) ) p2_initial = +1.e1;
  else p2_initial = -5.e0;
  double p3_initial = xHalfMax;

  //std::cout << "initializing fitParameters:" << std::endl;
  //std::cout << " p0 = " << p0_initial << std::endl;
  //std::cout << " p1 = " << p1_initial << std::endl;
  //std::cout << " p2 = " << p2_initial << std::endl;
  //std::cout << " p3 = " << p3_initial << std::endl;
  
//--- fit function in two iterations:
//    in the first iteration keep p1 fixed at zero;
//    allow p1 to vary in the second iteration, once "good" values for all other function parameters are found
//   (the aim of this two step procedure is to make the fit more robust)
  fitFunction->SetParameter(0, p0_initial);
  fitFunction->SetParameter(1, p1_initial);
  fitFunction->FixParameter(1, p1_initial);
  fitFunction->SetParameter(2, p2_initial);
  fitFunction->SetParameter(3, p3_initial);
  histogram->Fit(fitFunction, "0");

  fitFunction->ReleaseParameter(1);
  histogram->Fit(fitFunction, "0");

  if ( fitParameter ) {
    int numFitParameter = fitFunction->GetNpar();
    for ( int iFitParameter = 0; iFitParameter < numFitParameter; ++iFitParameter ) {
      double fitParameterValue = fitFunction->GetParameter(iFitParameter);
      double fitParameterError = fitFunction->GetParError(iFitParameter);
      fitParameter->push_back(std::pair<double, double>(fitParameterValue, fitParameterError));
    }
  }

  return fitFunction;
}

void showPlot(TH1* histogramRef, const std::string& legendEntryRef,
	      TH1* histogram1, const std::string& legendEntry1,
	      TH1* histogram2, const std::string& legendEntry2,
	      TF1* (*fitFunction)(TH1*, std::vector<std::pair<double, double> >*), std::vector<std::pair<double, double> >* fitParameter,
	      const std::string& xAxisTitle, 
	      const std::string& outputFileName)
{
  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 800);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);
  
  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);

  TPad* topPad = new TPad("topPad", "topPad", 0.00, 0.35, 1.00, 1.00);
  topPad->SetFillColor(10);
  topPad->SetTopMargin(0.04);
  topPad->SetLeftMargin(0.15);
  topPad->SetBottomMargin(0.03);
  topPad->SetRightMargin(0.05);
  topPad->SetLogy(true);

  TPad* bottomPad = new TPad("bottomPad", "bottomPad", 0.00, 0.00, 1.00, 0.35);
  bottomPad->SetFillColor(10);
  bottomPad->SetTopMargin(0.02);
  bottomPad->SetLeftMargin(0.15);
  bottomPad->SetBottomMargin(0.24);
  bottomPad->SetRightMargin(0.05);
  bottomPad->SetLogy(false);

  canvas->cd();
  topPad->Draw();
  topPad->cd();

  histogramRef->SetTitle("");
  histogramRef->SetStats(false);
  histogramRef->SetMinimum(1.e-3);
  histogramRef->SetMaximum(1.e0);
  histogramRef->SetLineColor(1);
  histogramRef->SetLineWidth(2);
  histogramRef->Draw("hist");

  TAxis* xAxis_top = histogramRef->GetXaxis();
  xAxis_top->SetTitle(xAxisTitle.data());
  xAxis_top->SetTitleOffset(1.15);
  xAxis_top->SetLabelColor(10);
  xAxis_top->SetTitleColor(10);

  TAxis* yAxis_top = histogramRef->GetYaxis();
  yAxis_top->SetTitle("a.u.");
  yAxis_top->SetTitleOffset(1.30);

  histogram1->SetLineColor(4);
  histogram1->SetLineWidth(2);
  histogram1->Draw("histsame");

  histogram2->SetLineColor(2);
  histogram2->SetLineWidth(2);
  histogram2->Draw("histsame");

  TLegend* legend = new TLegend(0.50, 0.74, 0.94, 0.94, "", "brNDC"); 
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  legend->AddEntry(histogramRef, legendEntryRef.data(), "l");
  legend->AddEntry(histogram1,   legendEntry1.data(),   "l");
  legend->AddEntry(histogram2,   legendEntry2.data(),   "l");
  legend->Draw();

  canvas->cd();
  bottomPad->Draw();
  bottomPad->cd();

  TString histogram1divRefName = TString(histogram1->GetName()).Append("_div_").Append(histogramRef->GetName());
  TH1* histogram1divRef = compRatioHistogram(histogram1divRefName, histogram1, histogramRef);
  histogram1divRef->SetTitle("");
  histogram1divRef->SetStats(false);
  histogram1divRef->SetMinimum(0.);
  histogram1divRef->SetMaximum(3.);

  TAxis* xAxis_bottom = histogram1divRef->GetXaxis();
  xAxis_bottom->SetTitle(xAxis_top->GetTitle());
  xAxis_bottom->SetLabelColor(1);
  xAxis_bottom->SetTitleColor(1);
  xAxis_bottom->SetTitleOffset(1.20);
  xAxis_bottom->SetTitleSize(0.08);
  xAxis_bottom->SetLabelOffset(0.02);
  xAxis_bottom->SetLabelSize(0.08);
  xAxis_bottom->SetTickLength(0.055);
  
  TAxis* yAxis_bottom = histogram1divRef->GetYaxis();
  yAxis_bottom->SetTitle("Ratio");
  yAxis_bottom->SetTitleOffset(0.65);
  yAxis_bottom->SetNdivisions(505);
  yAxis_bottom->CenterTitle();
  yAxis_bottom->SetTitleSize(0.08);
  yAxis_bottom->SetLabelSize(0.08);
  yAxis_bottom->SetTickLength(0.04);  
  
  histogram1divRef->Draw("axis");

  TGraph* graph_line = new TGraph(2);
  graph_line->SetPoint(0, xAxis_bottom->GetXmin(), 1.);
  graph_line->SetPoint(1, xAxis_bottom->GetXmax(), 1.);
  graph_line->SetLineColor(8);
  graph_line->SetLineWidth(1);
  graph_line->Draw("L");

  TF1* fitFunction1 = (*fitFunction)(histogram1divRef, fitParameter);
  fitFunction1->SetLineColor(histogram1divRef->GetLineColor());
  fitFunction1->SetLineWidth(1);
  fitFunction1->Draw("same");

  TString histogram2divRefName = TString(histogram2->GetName()).Append("_div_").Append(histogramRef->GetName());
  TH1* histogram2divRef = compRatioHistogram(histogram2divRefName, histogram2, histogramRef);

  TF1* fitFunction2 = (*fitFunction)(histogram2divRef, NULL);
  fitFunction2->SetLineColor(histogram2divRef->GetLineColor());
  fitFunction2->SetLineWidth(1);
  fitFunction2->Draw("same");
  
  histogram1divRef->Draw("histsame");
  
  histogram2divRef->Draw("histsame");
  
  canvas->Update();
  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_plot).append(std::string(outputFileName, idx)).data());
  canvas->Print(std::string(outputFileName_plot).append(".png").data());
  canvas->Print(std::string(outputFileName_plot).append(".pdf").data());
  
  delete legend;
  delete canvas;  
}

TH1* getHistogram_normalized(const TH1* histogram)
{
  TString histogramName_normalized = TString(histogram->GetName()).Append("_normalized");
  TH1* histogram_normalized = (TH1*)histogram->Clone(histogramName_normalized.Data());
  double integral = histogram_normalized->Integral();
  if ( integral > 0. ) histogram_normalized->Scale(1./integral);
  return histogram_normalized;
}

void makePlotX(int legIdx, 
	       TH1* histogram_beforeVisPtCuts, 
	       TH1* histogram_afterVisPtCutsSingleLeg, 
	       TH1* histogram_afterVisPtCutsBothLegs,
	       const std::string& outputFileName,
	       std::vector<std::pair<double, double> >& fitParameter)
{
  TH1* histogram_beforeVisPtCuts_normalized         = getHistogram_normalized(histogram_beforeVisPtCuts);
  TH1* histogram_afterVisPtCutsSingleLeg_normalized = getHistogram_normalized(histogram_afterVisPtCutsSingleLeg);
  TH1* histogram_afterVisPtCutsBothLegs_normalized  = getHistogram_normalized(histogram_afterVisPtCutsBothLegs);
  
  std::string legendEntry_beforeVisPtCuts           = "before Cuts";
  std::string legendEntry_afterVisPtCutsSingleLeg   = "after Cuts Leg 1";
  std::string legendEntry_afterVisPtCutsBothLegs    = "after Cuts Legs 1&2";

  TF1* (*fitFunction)(TH1*, std::vector<std::pair<double, double> >*) = &fit1d;

  std::string xAxisTitle = Form("X_{%i}", legIdx);

  showPlot(histogram_beforeVisPtCuts_normalized, legendEntry_beforeVisPtCuts,
	   histogram_afterVisPtCutsSingleLeg_normalized, legendEntry_afterVisPtCutsSingleLeg,
	   histogram_afterVisPtCutsBothLegs_normalized, legendEntry_afterVisPtCutsBothLegs,
	   fitFunction, &fitParameter, xAxisTitle, outputFileName);
}

void makePlotNuMass(int legIdx, 
		    TH1* histogram_beforeVisPtCuts, 
		    TH1* histogram_afterVisPtCutsSingleLeg, 
		    TH1* histogram_afterVisPtCutsBothLegs,
		    const std::string& outputFileName)
{
  TH1* histogram_beforeVisPtCuts_normalized         = getHistogram_normalized(histogram_beforeVisPtCuts);
  TH1* histogram_afterVisPtCutsSingleLeg_normalized = getHistogram_normalized(histogram_afterVisPtCutsSingleLeg);
  TH1* histogram_afterVisPtCutsBothLegs_normalized  = getHistogram_normalized(histogram_afterVisPtCutsBothLegs);
  
  std::string legendEntry_beforeVisPtCuts           = "before Cuts";
  std::string legendEntry_afterVisPtCutsSingleLeg   = "after Cuts Leg 1";
  std::string legendEntry_afterVisPtCutsBothLegs    = "after Cuts Legs 1&2";

  TF1* (*fitFunction)(TH1*, std::vector<std::pair<double, double> >*) = &fit1d;

  std::string xAxisTitle = Form("M_{#nu#nu}^{%i} / GeV", legIdx);

  showPlot(histogram_beforeVisPtCuts_normalized, legendEntry_beforeVisPtCuts,
	   histogram_afterVisPtCutsSingleLeg_normalized, legendEntry_afterVisPtCutsSingleLeg,
	   histogram_afterVisPtCutsBothLegs_normalized, legendEntry_afterVisPtCutsBothLegs,
	   fitFunction, NULL, xAxisTitle, outputFileName);
}

void showHistogram2d(TH2* histogram, 
		     const std::string& xAxisTitle, const std::string& yAxisTitle,
		     const std::string& outputFileName)
{
  TCanvas* canvas = new TCanvas("canvas", "canvas", 900, 800);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  canvas->SetTopMargin(0.10);
  canvas->SetLeftMargin(0.12);
  canvas->SetRightMargin(0.14);
  canvas->SetBottomMargin(0.12);

  histogram->SetTitle("");
  histogram->SetStats(false);
  std::vector<double> binContents;
  int numBinsX = histogram->GetNbinsX();
  for ( int iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
    int numBinsY = histogram->GetNbinsY();
    for ( int iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
      binContents.push_back(histogram->GetBinContent(iBinX, iBinY));
    }
  }
  std::sort(binContents.begin(), binContents.end());
  histogram->SetMinimum(binContents[TMath::Nint(0.05*binContents.size())]);
  histogram->SetMaximum(binContents[TMath::Nint(0.95*binContents.size())]);

  TAxis* xAxis = histogram->GetXaxis();
  xAxis->SetTitle(xAxisTitle.data());
  xAxis->SetTitleOffset(1.15);

  TAxis* yAxis = histogram->GetYaxis();
  yAxis->SetTitle(yAxisTitle.data());
  yAxis->SetTitleOffset(1.30);

  gStyle->SetPalette(1,0);
  histogram->Draw("COLZ");

  canvas->Update();
  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_plot).append(std::string(outputFileName, idx)).data());
  canvas->Print(std::string(outputFileName_plot).append(".png").data());
  canvas->Print(std::string(outputFileName_plot).append(".pdf").data());
  canvas->Print(std::string(outputFileName_plot).append(".root").data());

  delete canvas;  
}

void makePlotNuMassVsX(int legIdx, 
		       TH2* histogram2d_beforeVisPtCuts, 
		       TH2* histogram2d_afterVisPtCutsSingleLeg, 
		       TH2* histogram2d_afterVisPtCutsBothLegs,
		       const std::string& outputFileName)
{
  std::string histogram2dName_afterVisPtCutsSingleLeg_div_beforeVisPtCuts = std::string(
    histogram2d_afterVisPtCutsSingleLeg->GetName()).append("_div_beforeVisPtCuts");
  TH2* histogram2d_afterVisPtCutsSingleLeg_div_beforeVisPtCuts = dynamic_cast<TH2*>(compRatioHistogram(
    histogram2dName_afterVisPtCutsSingleLeg_div_beforeVisPtCuts.data(),
    histogram2d_afterVisPtCutsSingleLeg,
    histogram2d_beforeVisPtCuts));
  histogram2d_afterVisPtCutsSingleLeg_div_beforeVisPtCuts->Scale(
    histogram2d_beforeVisPtCuts->Integral()/histogram2d_afterVisPtCutsSingleLeg->Integral());
  size_t idx2d = outputFileName.find_last_of('.');
  std::string outputFileName_afterVisPtCutsSingleLeg_div_beforeVisPtCuts = std::string(outputFileName, 0, idx2d);
  outputFileName_afterVisPtCutsSingleLeg_div_beforeVisPtCuts.append("_afterVisPtCutsSingleLeg_div_beforeVisPtCuts");
  if ( idx2d != std::string::npos ) outputFileName_afterVisPtCutsSingleLeg_div_beforeVisPtCuts.append(std::string(outputFileName, idx2d));
  std::string xAxisTitle2d = Form("X_{%i}", legIdx);
  std::string yAxisTitle2d = Form("M_{#nu#nu}^{%i} / GeV", legIdx);
  showHistogram2d(histogram2d_afterVisPtCutsSingleLeg_div_beforeVisPtCuts, 
		  xAxisTitle2d, yAxisTitle2d, outputFileName_afterVisPtCutsSingleLeg_div_beforeVisPtCuts);

  std::string histogram2dName_afterVisPtCutsBothLegs_div_beforeVisPtCuts = std::string(
    histogram2d_afterVisPtCutsBothLegs->GetName()).append("_div_beforeVisPtCuts");
  TH2* histogram2d_afterVisPtCutsBothLegs_div_beforeVisPtCuts = dynamic_cast<TH2*>(compRatioHistogram(
    histogram2dName_afterVisPtCutsBothLegs_div_beforeVisPtCuts.data(),
    histogram2d_afterVisPtCutsBothLegs,
    histogram2d_beforeVisPtCuts));
  histogram2d_afterVisPtCutsBothLegs_div_beforeVisPtCuts->Scale(
    histogram2d_beforeVisPtCuts->Integral()/histogram2d_afterVisPtCutsBothLegs->Integral());
  std::string outputFileName_afterVisPtCutsBothLegs_div_beforeVisPtCuts = std::string(outputFileName, 0, idx2d);
  outputFileName_afterVisPtCutsBothLegs_div_beforeVisPtCuts.append("_afterVisPtCutsBothLegs_div_beforeVisPtCuts");
  if ( idx2d != std::string::npos ) outputFileName_afterVisPtCutsBothLegs_div_beforeVisPtCuts.append(std::string(outputFileName, idx2d));
  showHistogram2d(histogram2d_afterVisPtCutsBothLegs_div_beforeVisPtCuts, 
		  xAxisTitle2d, yAxisTitle2d, outputFileName_afterVisPtCutsBothLegs_div_beforeVisPtCuts);

  std::vector<double> x_bins;
  std::vector<double> xErrUp_bins;
  std::vector<double> xErrDown_bins;
  std::vector<std::vector<std::pair<double, double> > > fitParameter_bins;

  const double minEventFraction = 0.05;

  TH2* histogram2dRef = histogram2d_afterVisPtCutsSingleLeg;
  double integral2dRef = histogram2dRef->Integral();

  int numBins2dX = histogram2dRef->GetNbinsX();
  TH1* histogram1dSum_beforeVisPtCuts         = 0;  
  TH1* histogram1dSum_afterVisPtCutsSingleLeg = 0;
  TH1* histogram1dSum_afterVisPtCutsBothLegs  = 0;
  double xSum = 0.;
  double xMin_bin = 0.;
  double xMax_bin = 0.;
  double integral1dRefSum = 0.;
  double integral1dRefRemaining = integral2dRef;
  for ( int iBin2dX = 1; iBin2dX <= numBins2dX; ++iBin2dX ) {
    TAxis* xAxis = histogram2dRef->GetXaxis();
    double x = xAxis->GetBinCenter(iBin2dX);
    double xErr = 0.5*xAxis->GetBinWidth(iBin2dX);

    std::string histogram1dName_beforeVisPtCuts = std::string(
      histogram2d_beforeVisPtCuts->GetName()).append(Form("BinX%ibeforeVisPtCuts", iBin2dX));
    TH1* histogram1d_beforeVisPtCuts = histogram2d_beforeVisPtCuts->ProjectionY(
      histogram1dName_beforeVisPtCuts.data(), iBin2dX, iBin2dX);
    std::string histogram1dName_afterVisPtCutsSingleLeg = std::string(
      histogram2d_afterVisPtCutsSingleLeg->GetName()).append(Form("BinX%iafterVisPtCutsSingleLeg", iBin2dX));
    TH1* histogram1d_afterVisPtCutsSingleLeg = histogram2d_afterVisPtCutsSingleLeg->ProjectionY(
      histogram1dName_afterVisPtCutsSingleLeg.data(), iBin2dX, iBin2dX);
    std::string histogram1dName_afterVisPtCutsBothLegs = std::string(
      histogram2d_afterVisPtCutsBothLegs->GetName()).append(Form("BinX%iafterVisPtCutsBothLegs", iBin2dX));
    TH1* histogram1d_afterVisPtCutsBothLegs = histogram2d_afterVisPtCutsBothLegs->ProjectionY(
      histogram1dName_afterVisPtCutsBothLegs.data(), iBin2dX, iBin2dX);

    TH1* histogram1dRef = histogram1d_afterVisPtCutsSingleLeg;
    double integral1dRef = histogram1dRef->Integral();

    if ( (integral1dRefSum > (minEventFraction*integral2dRef) && integral1dRefRemaining > (minEventFraction*integral2dRef)) ||
	 iBin2dX == (numBins2dX - 1) ) {
 
      std::string legendEntry_beforeVisPtCuts         = "before Cuts";
      std::string legendEntry_afterVisPtCutsSingleLeg = "after Cuts Leg 1";
      std::string legendEntry_afterVisPtCutsBothLegs  = "after Cuts Legs 1&2";
      
      TF1* (*fitFunction)(TH1*, std::vector<std::pair<double, double> >*) = &fit1d;
      
      std::string xAxisTitle = Form("M_{#nu#nu}^{%i} / GeV", legIdx);

      TH1* histogram1dSum_beforeVisPtCuts_normalized         = getHistogram_normalized(histogram1dSum_beforeVisPtCuts);
      TH1* histogram1dSum_afterVisPtCutsSingleLeg_normalized = getHistogram_normalized(histogram1dSum_afterVisPtCutsSingleLeg);
      TH1* histogram1dSum_afterVisPtCutsBothLegs_normalized  = getHistogram_normalized(histogram1dSum_afterVisPtCutsBothLegs);

      std::vector<std::pair<double, double> > fitParameter_bin;

      size_t idx = outputFileName.find_last_of('.');
      TString outputFileName_bin = std::string(outputFileName, 0, idx).data();
      outputFileName_bin.Append(Form("_X%ifrom%1.2fto%1.2f", legIdx, xMin_bin, xMax_bin));
      outputFileName_bin = outputFileName_bin.ReplaceAll(".", "_");
      if ( idx != std::string::npos ) outputFileName_bin.Append(std::string(outputFileName, idx).data());

      showPlot(histogram1dSum_beforeVisPtCuts_normalized, legendEntry_beforeVisPtCuts,
	       histogram1dSum_afterVisPtCutsSingleLeg_normalized, legendEntry_afterVisPtCutsSingleLeg,
	       histogram1dSum_afterVisPtCutsBothLegs_normalized, legendEntry_afterVisPtCutsBothLegs,
	       fitFunction, &fitParameter_bin, xAxisTitle, outputFileName_bin.Data());

      double x_bin = xSum/integral1dRefSum;
      x_bins.push_back(x_bin);
      xErrUp_bins.push_back(xMax_bin - x_bin);
      xErrDown_bins.push_back(x_bin - xMin_bin);
      fitParameter_bins.push_back(fitParameter_bin);

      histogram1dSum_beforeVisPtCuts         = 0;  
      histogram1dSum_afterVisPtCutsSingleLeg = 0;
      histogram1dSum_afterVisPtCutsBothLegs  = 0;
      xSum = 0.;
      xMin_bin = x - xErr;
      integral1dRefRemaining -= integral1dRefSum;
      integral1dRefSum = 0.;
    }

    if ( !histogram1dSum_beforeVisPtCuts ) histogram1dSum_beforeVisPtCuts = histogram1d_beforeVisPtCuts;
    else histogram1dSum_beforeVisPtCuts->Add(histogram1d_beforeVisPtCuts);
    if ( !histogram1dSum_afterVisPtCutsSingleLeg ) histogram1dSum_afterVisPtCutsSingleLeg = histogram1d_afterVisPtCutsSingleLeg;
    else histogram1dSum_afterVisPtCutsSingleLeg->Add(histogram1d_afterVisPtCutsSingleLeg);
    if ( !histogram1dSum_afterVisPtCutsBothLegs ) histogram1dSum_afterVisPtCutsBothLegs = histogram1d_afterVisPtCutsBothLegs;
    else histogram1dSum_afterVisPtCutsBothLegs->Add(histogram1d_afterVisPtCutsBothLegs);

    xSum += (x*integral1dRef);
    xMax_bin = x + xErr;
    integral1dRefSum += integral1dRef;

    if ( histogram1dSum_beforeVisPtCuts         != histogram1d_beforeVisPtCuts         ) delete histogram1d_beforeVisPtCuts;
    if ( histogram1dSum_afterVisPtCutsSingleLeg != histogram1d_afterVisPtCutsSingleLeg ) delete histogram1d_afterVisPtCutsSingleLeg;
    if ( histogram1dSum_afterVisPtCutsBothLegs  != histogram1d_afterVisPtCutsBothLegs  ) delete histogram1d_afterVisPtCutsBothLegs;
  }
}

void showGraphMass(const std::string& type,
		   const std::vector<double>& massPoints,
		   const std::vector<double>& massPointErrsUp, const std::vector<double>& massPointErrsDown, 
		   std::map<double, std::vector<std::pair<double, double> > >& fitParameter,
		   const std::string& outputFileName_txt,
		   const std::string& outputFileName)
{
  std::ofstream* outputFile_txt = new std::ofstream(outputFileName_txt.data(), std::ios::out);

  int numFitParameter = fitParameter.begin()->second.size();
  for ( int iFitParameter = 0; iFitParameter < numFitParameter; ++iFitParameter ) {
        
    int numMassPoints = massPoints.size();

    TGraphAsymmErrors* graph = new TGraphAsymmErrors(numMassPoints);

    for ( int iMassPoint = 0; iMassPoint < numMassPoints; ++iMassPoint ) {
      double massPoint = massPoints[iMassPoint];
      
      double x         = massPoint;
      double xErrUp    = massPointErrsUp[iMassPoint];
      double xErrDown  = massPointErrsDown[iMassPoint];
      
      if ( !((int)fitParameter[massPoint].size() == numFitParameter) ) continue;

      double y         = fitParameter[massPoint][iFitParameter].first;
      double yErr      = fitParameter[massPoint][iFitParameter].second;

      graph->SetPoint(iMassPoint, x, y);
      graph->SetPointError(iMassPoint, xErrDown, xErrUp, yErr, yErr);
    }

    double xMin = massPoints[0];
    double xMax = massPoints[numMassPoints - 1];
    
    TF1* fitFunction = 0;
    std::string fitOptions;
    if ( iFitParameter == 0 ) {
      if ( type == "X1" ) {
	fitFunction = 
	  new TF1("fitFunction", 
		  "[0] + [1]/TMath::Power(TMath::Max(10., x - [2]), [3])", xMin, xMax);
	fitFunction->SetParameter(0, 1.);
	fitFunction->SetParameter(1, 1.);
	fitFunction->SetParameter(2, 80.);
	fitFunction->SetParameter(3, 0.4);
	fitOptions = "W";
      } else if ( type == "X2" ) {
	fitFunction = 
	  new TF1("fitFunction", 
		  "[0]*0.5*(1.0 - TMath::Erf(-[1]*TMath::Power(TMath::Max(10., x - [2]), [3])))", xMin, xMax);
	fitFunction->SetParameter(0, 1.);
	fitFunction->SetParameter(1, 2.e-4);
	fitFunction->SetParameter(2, -50.);
	fitFunction->SetParameter(3, 2.);
	fitOptions = "";
      } else assert(0);
    } else if ( iFitParameter == 1 ) {
      fitFunction = new TF1("fitFunction", "[0]/TMath::Power(TMath::Max(10., x - [1]), [2])", xMin, xMax);
      fitFunction->SetParameter(0, 1.e+1);
      fitFunction->SetParameter(1, 75.);
      fitFunction->SetParameter(2, 0.75);
      fitOptions = "";
    } else if ( iFitParameter == 2 ) {
      if ( type == "X1" ) {
	if ( xMax > 400. ) xMax = 400.;
	fitFunction = 
	  new TF1("fitFunction", 
		  "[0]*0.5*(1.0 - [1]*TMath::Erf(-[2]*(x - [3])*TMath::Power(TMath::Abs(x - [3]), [4])))", xMin, xMax);
	fitFunction->SetParameter(0, 1.5e+1);
	fitFunction->SetParameter(1, 3.);
	fitFunction->SetParameter(2, 5.e-4);
	fitFunction->SetParameter(3, 100.);
	fitFunction->SetParameter(4, 0.5);
/*
   1  p0           1.76360e+01   1.11525e+00   0.00000e+00  -5.29835e-10
   2  p1           2.63433e+00   2.72693e-01  -0.00000e+00   2.32757e-10
   3  p2           1.65353e-04   2.00113e-04  -0.00000e+00   2.57768e-05
   4  p3           1.06536e+02   7.04461e+00  -0.00000e+00  -6.60006e-11
   5  p4           8.32556e-01   2.72235e-01   0.00000e+00  -2.26697e-08
 */
	fitFunction->FixParameter(0, 1.76360e+01);
	fitFunction->FixParameter(1, 2.63433e+00);
	fitFunction->FixParameter(2, 1.65353e-04);
	fitFunction->FixParameter(3, 1.06536e+02);
	fitFunction->FixParameter(4, 8.32556e-01);
	fitOptions = "W";
      } else if ( type == "X2" ) {
	if ( xMax > 300. ) xMax = 300.;
	fitFunction = 
	  new TF1("fitFunction", 
		  "[0]*0.5*(1.0 - [1]*TMath::Erf(-[2]*(x - [3])*TMath::Power(TMath::Abs(x - [3]), [4])))", xMin, xMax);
	fitFunction->SetParameter(0, 1.5e+1);
	fitFunction->SetParameter(1, 5.);
	fitFunction->SetParameter(2, 1.e-4);
	fitFunction->SetParameter(3, 100.);
	fitFunction->SetParameter(4, 0.6);
/*
   1  p0           1.34063e+01   9.19850e-01   6.29425e-04  -3.05896e-06
   2  p1           4.62657e+00   1.25590e+01   4.44717e-04   1.74896e-04
   3  p2           1.34817e-04   2.30412e-04   1.44291e-08   4.93970e+00
   4  p3           1.05208e+02   7.35747e+00   5.62431e-03  -4.89739e-06
   5  p4           6.31860e-01   4.60492e-01   2.22535e-05   3.75255e-03

 */
	fitFunction->FixParameter(0, 1.34063e+01);
	fitFunction->FixParameter(1, 4.62657e+00);
	fitFunction->FixParameter(2, 1.34817e-04);
	fitFunction->FixParameter(3, 1.05208e+02);
	fitFunction->FixParameter(4, 6.31860e-01);
	fitOptions = "W";
      } else assert(0);
    } else if ( iFitParameter == 3 ) {
      if ( xMax > 400. ) xMax = 400.;      
      fitFunction = new TF1("fitFunction", "[0] + [1]/TMath::Power(TMath::Max(10., x - [2]), [3])", xMin, xMax);
      fitFunction->SetParameter(0, -0.8);
      fitFunction->SetParameter(1, +1.5);
      fitFunction->SetParameter(2, 80.);
      fitFunction->SetParameter(3, 0.08);
      fitOptions = "";
    } else assert(0);
    fitOptions.append("0");
    graph->Fit(fitFunction, fitOptions.data());
    
    double yMin = +1.e+6;
    double yMax = -1.e+6;
    for ( int iMassPoint = 0; iMassPoint < numMassPoints; ++iMassPoint ) {
      double x, y;
      graph->GetPoint(iMassPoint, x, y);
      if ( x > xMin && x < xMax ) {
	if ( y < yMin ) yMin = y;
	if ( y > yMax ) yMax = y;
      }
    }    
    yMin -= 0.06*(yMax - yMin);
    yMax += 0.06*(yMax - yMin);

    TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 600);
    canvas->SetFillColor(10);
    canvas->SetBorderSize(2);
  
    canvas->SetLeftMargin(0.12);
    canvas->SetBottomMargin(0.12);
   
    TH1* dummyHistogram = new TH1F("dummyHistogram", "dummyHistogram", 10, xMin, xMax);
    dummyHistogram->SetStats(false);
    dummyHistogram->SetTitle("");
    dummyHistogram->SetMinimum(yMin);
    dummyHistogram->SetMaximum(yMax);
    
    std::string xAxisTitle = "M_{#tau#tau} / GeV";
    std::string yAxisTitle = Form("p_{%i}", iFitParameter);

    TAxis* xAxis = dummyHistogram->GetXaxis();
    xAxis->SetTitle(xAxisTitle.data());
    xAxis->SetTitleOffset(1.15);
    
    TAxis* yAxis = dummyHistogram->GetYaxis();
    yAxis->SetTitle(yAxisTitle.data());
    yAxis->SetTitleOffset(1.30);
    
    dummyHistogram->Draw("axis");

    (*outputFile_txt) << "xMin = cms.double(" << xMin << ")," << std::endl;
    (*outputFile_txt) << "xMax = cms.double(" << xMax << ")," << std::endl;
    (*outputFile_txt) << "formula = cms.string('" << fitFunction->GetTitle() << "')," << std::endl;
    (*outputFile_txt) << "parameter = cms.PSet(" << std::endl;
    int numFitFunctionParameter = fitFunction->GetNpar();
    for ( int iFitFunctionParameter = 0; iFitFunctionParameter < numFitFunctionParameter; ++iFitFunctionParameter ) {
      double fitFunctionParameterValue = fitFunction->GetParameter(iFitFunctionParameter);
      double fitFunctionParameterError = fitFunction->GetParError(iFitFunctionParameter);
      (*outputFile_txt) << "    p" << iFitFunctionParameter << " = cms.double(" << fitFunctionParameterValue << ")," << std::endl;
    }
    (*outputFile_txt) << ")" << std::endl;

    fitFunction->SetLineColor(2);
    fitFunction->SetLineWidth(2);
    fitFunction->Draw("same");

    graph->SetMarkerStyle(20);
    graph->SetMarkerColor(1);
    graph->Draw("P");

    canvas->Update();
    size_t idx = outputFileName.find_last_of('.');
    std::string outputFileName_param = std::string(outputFileName, 0, idx);
    outputFileName_param = Form(outputFileName_param.data(), iFitParameter);
    if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_param).append(std::string(outputFileName, idx)).data());
    canvas->Print(std::string(outputFileName_param).append(".png").data());
    canvas->Print(std::string(outputFileName_param).append(".pdf").data());
    canvas->Print(std::string(outputFileName_param).append(".root").data());
  
    delete fitFunction;
    delete dummyHistogram;
    delete canvas;
  }

  delete outputFile_txt;
}

void makeSVfitVisPtCutPlots()
{
  TString inputFileName = "../test/studySVfitVisPtCuts_2012Mar04.root";
  TFile* inputFile = new TFile(inputFileName.Data());

  gROOT->SetBatch(true);

  TString directory_beforeVisPtCuts        = "plotsBeforeVisEtaAndPtCuts";
  TString directory_afterVisPtCutsLeg1     = "plotsAfterVisEtaAndPtCutsLeg1";
  TString directory_afterVisPtCutsLeg2     = "plotsAfterVisEtaAndPtCutsLeg2";
  TString directory_afterVisPtCutsLeg1and2 = "plotsAfterVisEtaAndPtCutsLeg1and2";
  
  TString histogramName_EventCounter = "histogramEventCounter";
  TString histogramName_X1           = "histogramLeg1X";
  TString histogramName_NuMass1      = "histogramLeg1NuMass";
  TString histogramName_NuMass1vsX1  = "histogramLeg1NuMassVsX";
  TString histogramName_X2           = "histogramLeg2X";
  TString histogramName_NuMass2      = "histogramLeg2NuMass";
  TString histogramName_NuMass2vsX2  = "histogramLeg2NuMassVsX";

  std::vector<std::string> histogramNames_X_dPhi;
  histogramNames_X_dPhi.push_back(std::string("histogramLeg1XdPhiLt30"));
  histogramNames_X_dPhi.push_back(std::string("histogramLeg1XdPhi30to60"));
  histogramNames_X_dPhi.push_back(std::string("histogramLeg1XdPhi60to90"));
  histogramNames_X_dPhi.push_back(std::string("histogramLeg1XdPhi90to120"));
  histogramNames_X_dPhi.push_back(std::string("histogramLeg1XdPhi120to140"));
  histogramNames_X_dPhi.push_back(std::string("histogramLeg1XdPhi140to160"));
  histogramNames_X_dPhi.push_back(std::string("histogramLeg1XdPhi160to170"));
  histogramNames_X_dPhi.push_back(std::string("histogramLeg1XdPhi170to175"));
  histogramNames_X_dPhi.push_back(std::string("histogramLeg1XdPhiGt175"));
  histogramNames_X_dPhi.push_back(std::string("histogramLeg2XdPhiLt30"));
  histogramNames_X_dPhi.push_back(std::string("histogramLeg2XdPhi30to60"));
  histogramNames_X_dPhi.push_back(std::string("histogramLeg2XdPhi60to90"));
  histogramNames_X_dPhi.push_back(std::string("histogramLeg2XdPhi90to120"));
  histogramNames_X_dPhi.push_back(std::string("histogramLeg2XdPhi120to140"));
  histogramNames_X_dPhi.push_back(std::string("histogramLeg2XdPhi140to160"));
  histogramNames_X_dPhi.push_back(std::string("histogramLeg2XdPhi160to170"));
  histogramNames_X_dPhi.push_back(std::string("histogramLeg2XdPhi170to175"));
  histogramNames_X_dPhi.push_back(std::string("histogramLeg2XdPhiGt175"));

  std::map<std::string, std::string> plotLabels_X_dPhi;
  plotLabels_X_dPhi["histogramLeg1XdPhiLt30"]     = "X1_dPhiLt30";
  plotLabels_X_dPhi["histogramLeg1XdPhi30to60"]   = "X1_dPhi30to60";
  plotLabels_X_dPhi["histogramLeg1XdPhi60to90"]   = "X1_dPhi60to90";
  plotLabels_X_dPhi["histogramLeg1XdPhi90to120"]  = "X1_dPhi90to120";
  plotLabels_X_dPhi["histogramLeg1XdPhi120to140"] = "X1_dPhi120to140";
  plotLabels_X_dPhi["histogramLeg1XdPhi140to160"] = "X1_dPhi140to160";
  plotLabels_X_dPhi["histogramLeg1XdPhi160to170"] = "X1_dPhi160to170";
  plotLabels_X_dPhi["histogramLeg1XdPhi170to175"] = "X1_dPhi170to175";
  plotLabels_X_dPhi["histogramLeg1XdPhiGt175"]    = "X1_dPhiGt175";
  plotLabels_X_dPhi["histogramLeg2XdPhiLt30"]     = "X2_dPhiLt30";
  plotLabels_X_dPhi["histogramLeg2XdPhi30to60"]   = "X2_dPhi30to60";
  plotLabels_X_dPhi["histogramLeg2XdPhi60to90"]   = "X2_dPhi60to90";
  plotLabels_X_dPhi["histogramLeg2XdPhi90to120"]  = "X2_dPhi90to120";
  plotLabels_X_dPhi["histogramLeg2XdPhi120to140"] = "X2_dPhi120to140";
  plotLabels_X_dPhi["histogramLeg2XdPhi140to160"] = "X2_dPhi140to160";
  plotLabels_X_dPhi["histogramLeg2XdPhi160to170"] = "X2_dPhi160to170";
  plotLabels_X_dPhi["histogramLeg2XdPhi170to175"] = "X2_dPhi170to175";
  plotLabels_X_dPhi["histogramLeg2XdPhiGt175"]    = "X2_dPhiGt175";

  std::vector<double> massPoints;
  massPoints.push_back(90.);
  massPoints.push_back(105.);
  massPoints.push_back(110.);
  massPoints.push_back(115.);
  massPoints.push_back(120.);
  massPoints.push_back(125.);
  massPoints.push_back(130.);
  massPoints.push_back(135.);
  massPoints.push_back(140.);
  massPoints.push_back(160.);
  massPoints.push_back(200.);
  massPoints.push_back(250.);
  massPoints.push_back(350.);
  massPoints.push_back(500.);
  massPoints.push_back(700.);
  massPoints.push_back(1000.);

  std::vector<double> massPointErrsUp;
  std::vector<double> massPointErrsDown;
  
  typedef std::pair<double, double> fitParameterType; // first entry = value, second entry = estimated uncertainty
  std::map<double, std::vector<fitParameterType> > fitParameterX1; // key = mass-point
  std::map<double, std::vector<fitParameterType> > fitParameterX2; // key = mass-point

  std::map<std::string, std::map<double, std::vector<fitParameterType> > > fitParameterX1_dPhi; // keys = plotLabel, mass-point
  std::map<std::string, std::map<double, std::vector<fitParameterType> > > fitParameterX2_dPhi; // keys = plotLabel, mass-point

  int numMassPoints = massPoints.size();
  for ( int iMassPoint = 0; iMassPoint < numMassPoints; ++iMassPoint ) {
    double massPoint = massPoints[iMassPoint];

    std::cout << "processing mass-point = " << massPoint << " GeV:" << std::endl;

    double massPointErrUp, massPointErrDown;
    if ( iMassPoint == 0 ) massPointErrDown = 0.5*(massPoints[iMassPoint + 1] - massPoint);
    else massPointErrDown = 0.5*(massPoint - massPoints[iMassPoint - 1]);
    if ( iMassPoint == (numMassPoints - 1) ) massPointErrUp = 0.5*(massPoint - massPoints[iMassPoint - 1]);
    else massPointErrUp = 0.5*(massPoints[iMassPoint + 1] - massPoint);
    massPointErrsUp.push_back(massPointErrUp);
    massPointErrsDown.push_back(massPointErrDown);
    
    TH1* histogram_EventCounter_beforeVisPtCuts = 
      getHistogram(inputFile, directory_beforeVisPtCuts, massPoint, histogramName_EventCounter);
    TH1* histogram_EventCounter_afterVisPtCutsLeg1 = 
      getHistogram(inputFile, directory_afterVisPtCutsLeg1, massPoint, histogramName_EventCounter);
    TH1* histogram_EventCounter_afterVisPtCutsLeg2 = 
      getHistogram(inputFile, directory_afterVisPtCutsLeg2, massPoint, histogramName_EventCounter);
    TH1* histogram_EventCounter_afterVisPtCutsLeg1and2 = 
      getHistogram(inputFile, directory_afterVisPtCutsLeg1and2, massPoint, histogramName_EventCounter);
    
    double integral_EventCounter_beforeVisPtCuts        = histogram_EventCounter_beforeVisPtCuts->Integral();
    double integral_EventCounter_afterVisPtCutsLeg1     = histogram_EventCounter_afterVisPtCutsLeg1->Integral();
    double integral_EventCounter_afterVisPtCutsLeg2     = histogram_EventCounter_afterVisPtCutsLeg2->Integral();
    double integral_EventCounter_afterVisPtCutsLeg1and2 = histogram_EventCounter_afterVisPtCutsLeg1and2->Integral();
    
    std::cout << "events:" 
	      << " before cuts = " << integral_EventCounter_beforeVisPtCuts
	      << " after leg1-cuts = " << integral_EventCounter_afterVisPtCutsLeg1 
	      << " (" << (integral_EventCounter_afterVisPtCutsLeg1/integral_EventCounter_beforeVisPtCuts)*100. << "%),"
	      << " after leg2-cuts = " << integral_EventCounter_afterVisPtCutsLeg2
	      << " (" << (integral_EventCounter_afterVisPtCutsLeg2/integral_EventCounter_beforeVisPtCuts)*100. << "%),"
	      << " after leg1&2-cuts = " << integral_EventCounter_afterVisPtCutsLeg1and2
	      << " (" << (integral_EventCounter_afterVisPtCutsLeg1and2/integral_EventCounter_beforeVisPtCuts)*100. << "%)" << std::endl;
    
    TH1* histogram_X1_beforeVisPtCuts = 
      getHistogram(inputFile, directory_beforeVisPtCuts, massPoint, histogramName_X1);
    TH1* histogram_X1_afterVisPtCutsLeg1 = 
      getHistogram(inputFile, directory_afterVisPtCutsLeg1, massPoint, histogramName_X1);
    TH1* histogram_X1_afterVisPtCutsLeg1and2 = 
      getHistogram(inputFile, directory_afterVisPtCutsLeg1and2, massPoint, histogramName_X1);
    makePlotX(1, 
	      histogram_X1_beforeVisPtCuts, 
	      histogram_X1_afterVisPtCutsLeg1, 
	      histogram_X1_afterVisPtCutsLeg1and2,
	      Form("plots/plotSVfitVisPtCut_X1_m%1.0f.eps", massPoint),
	      fitParameterX1[massPoint]);

    TH1* histogram_X2_beforeVisPtCuts = 
      getHistogram(inputFile, directory_beforeVisPtCuts, massPoint, histogramName_X2);
    TH1* histogram_X2_afterVisPtCutsLeg2 = 
      getHistogram(inputFile, directory_afterVisPtCutsLeg2, massPoint, histogramName_X2);
    TH1* histogram_X2_afterVisPtCutsLeg1and2 = 
      getHistogram(inputFile, directory_afterVisPtCutsLeg1and2, massPoint, histogramName_X2);
    makePlotX(2, 
	      histogram_X2_beforeVisPtCuts, 
	      histogram_X2_afterVisPtCutsLeg2, 
	      histogram_X2_afterVisPtCutsLeg1and2,
	      Form("plots/plotSVfitVisPtCut_X2_m%1.0f.eps", massPoint),
	      fitParameterX2[massPoint]);

    for ( std::vector<std::string>::const_iterator histogramName_X_dPhi = histogramNames_X_dPhi.begin();
	  histogramName_X_dPhi != histogramNames_X_dPhi.end(); ++histogramName_X_dPhi ) {
      int legIdx = -1;
      std::string directory_afterVisPtCutsLeg1or2;
      std::map<std::string, std::map<double, std::vector<fitParameterType> > >* fitParameterX_dPhi = NULL;
      if ( histogramName_X_dPhi->find("Leg1X") != std::string::npos ) {
	legIdx = 1;
	directory_afterVisPtCutsLeg1or2 = directory_afterVisPtCutsLeg1and2;
	fitParameterX_dPhi = &fitParameterX1_dPhi;
      } else if ( histogramName_X_dPhi->find("Leg2X") != std::string::npos ) {
	legIdx = 2;
	directory_afterVisPtCutsLeg1or2 = directory_afterVisPtCutsLeg1and2;
	fitParameterX_dPhi = &fitParameterX2_dPhi;
      } else assert(0);
      std::cout << (*histogramName_X_dPhi) << ": legIdx = " << legIdx << std::endl;
      TH1* histogram_X_dPhi_beforeVisPtCuts = 
	getHistogram(inputFile, directory_beforeVisPtCuts, massPoint, *histogramName_X_dPhi);      
      TH1* histogram_X_dPhi_afterVisPtCutsLeg1or2 = 
	getHistogram(inputFile, directory_afterVisPtCutsLeg1or2, massPoint, *histogramName_X_dPhi);
      TH1* histogram_X_dPhi_afterVisPtCutsLeg1and2 = 
	getHistogram(inputFile, directory_afterVisPtCutsLeg1and2, massPoint, *histogramName_X_dPhi);   
      const std::string& plotLabel = plotLabels_X_dPhi[*histogramName_X_dPhi];
      makePlotX(legIdx, 
		histogram_X_dPhi_beforeVisPtCuts, 
		histogram_X_dPhi_afterVisPtCutsLeg1or2, 
		histogram_X_dPhi_afterVisPtCutsLeg1and2,
		Form("plots/plotSVfitVisPtCut_%s_m%1.0f.eps", plotLabel.data(), massPoint),
		(*fitParameterX_dPhi)[plotLabel][massPoint]);
    }

    TH1* histogram_NuMass1_beforeVisPtCuts = 
      getHistogram(inputFile, directory_beforeVisPtCuts, massPoint, histogramName_NuMass1);
    TH1* histogram_NuMass1_afterVisPtCutsLeg1 = 
      getHistogram(inputFile, directory_afterVisPtCutsLeg1, massPoint, histogramName_NuMass1);
    TH1* histogram_NuMass1_afterVisPtCutsLeg1and2 = 
      getHistogram(inputFile, directory_afterVisPtCutsLeg1and2, massPoint, histogramName_NuMass1);
    if ( histogram_NuMass1_beforeVisPtCuts->GetMean() > 0.1 ) {
      makePlotNuMass(1, 
		     histogram_NuMass1_beforeVisPtCuts, 
		     histogram_NuMass1_afterVisPtCutsLeg1,
		     histogram_NuMass1_afterVisPtCutsLeg1and2,
		     Form("plots/plotSVfitVisPtCut_NuMass1_m%1.0f.eps", massPoint));

      TH2* histogram_NuMass1vsX1_beforeVisPtCuts = dynamic_cast<TH2*>(
	getHistogram(inputFile, directory_beforeVisPtCuts, massPoint, histogramName_NuMass1vsX1));
      TH2* histogram_NuMass1vsX1_afterVisPtCutsLeg1 = dynamic_cast<TH2*>(
	getHistogram(inputFile, directory_afterVisPtCutsLeg1, massPoint, histogramName_NuMass1vsX1));
      TH2* histogram_NuMass1vsX1_afterVisPtCutsLeg1and2 = dynamic_cast<TH2*>(
	getHistogram(inputFile, directory_afterVisPtCutsLeg1and2, massPoint, histogramName_NuMass1vsX1));

      makePlotNuMassVsX(1, 
			histogram_NuMass1vsX1_beforeVisPtCuts, 
			histogram_NuMass1vsX1_afterVisPtCutsLeg1, 
			histogram_NuMass1vsX1_afterVisPtCutsLeg1and2,
			Form("plots/plotSVfitVisPtCut_NuMass1vsX1_m%1.0f.eps", massPoint));
    }

    TH1* histogram_NuMass2_beforeVisPtCuts = 
      getHistogram(inputFile, directory_beforeVisPtCuts, massPoint, histogramName_NuMass2);
    TH1* histogram_NuMass2_afterVisPtCutsLeg2 = 
      getHistogram(inputFile, directory_afterVisPtCutsLeg2, massPoint, histogramName_NuMass2);
    TH1* histogram_NuMass2_afterVisPtCutsLeg1and2 = 
      getHistogram(inputFile, directory_afterVisPtCutsLeg1and2, massPoint, histogramName_NuMass2);
    if ( histogram_NuMass2_beforeVisPtCuts->GetMean() > 0.1 ) {
      makePlotNuMass(2, 
		     histogram_NuMass2_beforeVisPtCuts, 
		     histogram_NuMass2_afterVisPtCutsLeg2, 
		     histogram_NuMass2_afterVisPtCutsLeg1and2,
		     Form("plots/plotSVfitVisPtCut_NuMass2_m%1.0f.eps", massPoint));

      TH2* histogram_NuMass2vsX2_beforeVisPtCuts = dynamic_cast<TH2*>(
	getHistogram(inputFile, directory_beforeVisPtCuts, massPoint, histogramName_NuMass2vsX2));
      TH2* histogram_NuMass2vsX2_afterVisPtCutsLeg1 = dynamic_cast<TH2*>(
	getHistogram(inputFile, directory_afterVisPtCutsLeg1, massPoint, histogramName_NuMass2vsX2));
      TH2* histogram_NuMass2vsX2_afterVisPtCutsLeg1and2 = dynamic_cast<TH2*>(
	getHistogram(inputFile, directory_afterVisPtCutsLeg1and2, massPoint, histogramName_NuMass2vsX2));

      makePlotNuMassVsX(2, 
			histogram_NuMass2vsX2_beforeVisPtCuts, 
			histogram_NuMass2vsX2_afterVisPtCutsLeg1, 
			histogram_NuMass2vsX2_afterVisPtCutsLeg1and2,
			Form("plots/plotSVfitVisPtCut_NuMass2vsX2_m%1.0f.eps", massPoint));
    }
  }
 
  if ( massPoints.size() >= 10 ) {
    showGraphMass("X1",
		  massPoints, massPointErrsUp, massPointErrsDown, fitParameterX1, 
		  "plots/paramSVfitVisPtCut_fit1dX1vsMass.txt",
		  "plots/plotSVfitVisPtCut_p%ifit1dX1vsMass.eps");
    showGraphMass("X2",
		  massPoints, massPointErrsUp, massPointErrsDown, fitParameterX2,
		  "plots/paramSVfitVisPtCut_fit1dX2vsMass.txt",
		  "plots/plotSVfitVisPtCut_p%ifit1dX2vsMass.eps");

    for ( std::vector<std::string>::const_iterator histogramName_X_dPhi = histogramNames_X_dPhi.begin();
	  histogramName_X_dPhi != histogramNames_X_dPhi.end(); ++histogramName_X_dPhi ) {
      std::string type;
      std::map<std::string, std::map<double, std::vector<fitParameterType> > >* fitParameterX_dPhi = NULL;
      if ( histogramName_X_dPhi->find("Leg1X") != std::string::npos ) {
	type = "X1";
	fitParameterX_dPhi = &fitParameterX1_dPhi;
      } else if ( histogramName_X_dPhi->find("Leg2X") != std::string::npos ) {
	type = "X2";
	fitParameterX_dPhi = &fitParameterX2_dPhi;
      } else assert(0);
      const std::string& plotLabel = plotLabels_X_dPhi[*histogramName_X_dPhi];
      std::string outputFileName_txt = Form("plots/paramSVfitVisPtCut_fit1d%svsMass.txt", plotLabel.data());
      std::string outputFileName = Form("plots/plotSVfitVisPtCut_p%sfit1d%svsMass.eps", "%i", plotLabel.data());
      showGraphMass(type,
		    massPoints, massPointErrsUp, massPointErrsDown, (*fitParameterX_dPhi)[plotLabel],
		    outputFileName_txt,
		    outputFileName);
    }
  }

  delete inputFile;
}
