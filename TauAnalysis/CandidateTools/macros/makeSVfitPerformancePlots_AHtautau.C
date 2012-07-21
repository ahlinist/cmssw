
#include "RooDataHist.h"
#include "RooRealVar.h"
#include "RooConstVar.h"
#include "RooGenericPdf.h"
#include "RooGaussian.h"
#include "RooFFTConvPdf.h"
#include "RooFit.h"
#include "RooPlot.h"

#include <TFile.h>
#include <TString.h>
#include <TH1.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TMath.h>
#include <TROOT.h>
#include <TStyle.h>

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <iomanip>
#include <assert.h>

TH1* getHistogram(TFile* inputFile, const std::string& directory, const std::string& histogramName)
{  
  TString histogramName_full = directory.data();
  if ( !histogramName_full.EndsWith("/") ) histogramName_full.Append("/");
  histogramName_full.Append(histogramName.data());

  TH1* histogram = (TH1*)inputFile->Get(histogramName_full.Data());
  if ( !histogram) 
    std::cerr << "Failed to load histogram = " << histogramName_full 
	      << " from file = " << inputFile->GetName() << " !!" << std::endl;
  assert(histogram);

  if ( !histogram->GetSumw2N() ) histogram->Sumw2();

  return histogram;
}

TH1* getHistogram(TFile* inputFile, const std::string& channel, double massPoint, 
		  const std::string& directory, const std::string& histogramName, double metResolution)
{
  std::string process_gg = "";
  if      ( massPoint > 135. ) process_gg = Form("ggPhi%1.0f", massPoint);
  else if ( massPoint >  95. ) process_gg = Form("ggHiggs%1.0f", massPoint);
  std::string process_qq = "";
  if      ( massPoint <  95. ) process_qq = "ZplusJets";
  else if ( massPoint < 135. ) process_qq = Form("vbfHiggs%1.0f", massPoint);
  std::string process_bb = "";
  if      ( massPoint > 155. ) process_bb = Form("bbPhi%1.0f", massPoint);
  
  std::string metResolution_label;
  if ( metResolution > 0. ) metResolution_label = Form("pfMEtRes%1.0f", metResolution);
  else metResolution_label = "pfMEtResMC";

  std::vector<TH1*> histograms;
  if ( process_gg != "" ) {
    std::string directory_gg = 
      Form("DQMData/%s/%s/%s/%s/plotEntryType1", process_gg.data(), channel.data(), metResolution_label.data(), directory.data());
    histograms.push_back(getHistogram(inputFile, directory_gg, histogramName));
  }
  if ( process_qq != "" ) {
    std::string directory_qq = 
      Form("DQMData/%s/%s/%s/%s/plotEntryType1", process_qq.data(), channel.data(), metResolution_label.data(), directory.data());
    histograms.push_back(getHistogram(inputFile, directory_qq, histogramName));
  }
  if ( process_bb != "" ) {
    std::string directory_bb = 
      Form("DQMData/%s/%s/%s/%s/plotEntryType1", process_bb.data(), channel.data(), metResolution_label.data(), directory.data());
    histograms.push_back(getHistogram(inputFile, directory_bb, histogramName));
  }
  TH1* histogramSum = NULL;
  for ( std::vector<TH1*>::const_iterator histogram = histograms.begin();
	histogram != histograms.end(); ++histogram ) {
    if ( !histogramSum ) {
      std::string histogramSumName = std::string((*histogram)->GetName()).append("_summed");
      histogramSum = (TH1*)(*histogram)->Clone(histogramSumName.data());
    } else {
      histogramSum->Add(*histogram);
    }
  }

  assert(histogramSum);

  if ( !histogramSum->GetSumw2N() ) histogramSum->Sumw2();
  if ( histogramSum->Integral() > 0. ) histogramSum->Scale(1./histogramSum->Integral());
  
  return histogramSum;
}

TH1* rebinHistogram(const TH1* histogram, unsigned numBinsMin_rebinned, double xMin, double xMax)
{
  TH1* histogram_rebinned = 0;

  if ( histogram ) {
    histogram_rebinned = (TH1*)histogram->Clone(std::string(histogram->GetName()).append("_rebinned").data());
    if ( !histogram_rebinned->GetSumw2N() ) histogram_rebinned->Sumw2();
    
    unsigned numBins = histogram->GetNbinsX();
    unsigned numBins_withinRange = 0;
    for ( unsigned iBin = 1; iBin <= numBins; ++iBin ) {
      double binCenter = histogram->GetBinCenter(iBin);
      if ( binCenter >= xMin && binCenter <= xMax ) ++numBins_withinRange;
    }
    
    assert(histogram_rebinned->GetNbinsX() == (int)numBins);

    std::cout << "histogram = " << histogram->GetName() << ":" 
	      << " numBins(" << xMin << ".." << "xMax) = " << numBins_withinRange << std::endl;

    for ( int combineNumBins = 5; combineNumBins >= 2; --combineNumBins ) {
      if ( numBins_withinRange > (combineNumBins*numBinsMin_rebinned) && (numBins % combineNumBins) == 0 ) {
	histogram_rebinned->Rebin(combineNumBins);
	numBins /= combineNumBins;
	numBins_withinRange /= combineNumBins;
      }
    }

    std::cout << "histogram(rebinned) = " << histogram_rebinned->GetName() << ":" 
	      << " numBins = " << histogram_rebinned->GetNbinsX() << std::endl;
  }

  return histogram_rebinned;
}

void showHistograms(double canvasSizeX, double canvasSizeY,
		    TH1* histogram1, const std::string& legendEntry1,
		    TH1* histogram2, const std::string& legendEntry2,
		    TH1* histogram3, const std::string& legendEntry3,
		    TH1* histogram4, const std::string& legendEntry4,
		    double legendTextSize, double legendPosX, double legendPosY, double legendSizeX, double legendSizeY, 
		    std::vector<std::string>& labelTextLines, double labelTextSize,
		    double labelPosX, double labelPosY, double labelSizeX, double labelSizeY,
		    double xMin, double xMax, const std::string& xAxisTitle, double xAxisOffset,
		    bool useLogScale, double yMin, double yMax, const std::string& yAxisTitle, double yAxisOffset,
		    const std::string& outputFileName)
{
  unsigned numBinsMin_rebinned = 20;
  TH1* histogram1_rebinned = rebinHistogram(histogram1, numBinsMin_rebinned, xMin, xMax);
  TH1* histogram2_rebinned = rebinHistogram(histogram2, numBinsMin_rebinned, xMin, xMax);
  TH1* histogram3_rebinned = rebinHistogram(histogram3, numBinsMin_rebinned, xMin, xMax);
  TH1* histogram4_rebinned = rebinHistogram(histogram4, numBinsMin_rebinned, xMin, xMax);

  TCanvas* canvas = new TCanvas("canvas", "canvas", canvasSizeX, canvasSizeY);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);
  
  canvas->SetLeftMargin(0.14);
  canvas->SetBottomMargin(0.12);

  canvas->SetLogy(useLogScale);

  histogram1_rebinned->SetTitle("");
  histogram1_rebinned->SetStats(false);
  histogram1_rebinned->SetMinimum(yMin);
  histogram1_rebinned->SetMaximum(yMax);

  TAxis* xAxis = histogram1_rebinned->GetXaxis();
  xAxis->SetTitle(xAxisTitle.data());
  xAxis->SetTitleSize(0.045);
  xAxis->SetTitleOffset(xAxisOffset);  
  if ( xMax > xMin ) {
    std::cout << "limiting x-axis range to " << xMin << ".." << xMax << std::endl;
    xAxis->SetRangeUser(xMin, xMax);
  }

  TAxis* yAxis = histogram1_rebinned->GetYaxis();
  yAxis->SetTitle(yAxisTitle.data());
  yAxis->SetTitleSize(0.045);
  yAxis->SetTitleOffset(yAxisOffset);

  int colors[4] = { 1, 2, 3, 4 };
  int lineStyles[4] = { 1, 7, 4, 3 };

  histogram1_rebinned->SetLineColor(colors[0]);
  histogram1_rebinned->SetLineWidth(2);
  histogram1_rebinned->SetLineStyle(lineStyles[0]);
  histogram1_rebinned->Draw("hist");

  if ( histogram2_rebinned ) {
    histogram2_rebinned->SetLineColor(colors[1]);
    histogram2_rebinned->SetLineWidth(2);
    histogram2_rebinned->SetLineStyle(lineStyles[1]);
    histogram2_rebinned->Draw("histsame");
  }

  if ( histogram3_rebinned ) {
    histogram3_rebinned->SetLineColor(colors[2]);
    histogram3_rebinned->SetLineWidth(2);
    histogram3_rebinned->SetLineStyle(lineStyles[2]);
    histogram3_rebinned->Draw("histsame");
  }

  if ( histogram4_rebinned ) {
    histogram4_rebinned->SetLineColor(colors[3]);
    histogram4_rebinned->SetLineWidth(2);
    histogram4_rebinned->SetLineStyle(lineStyles[3]);
    histogram4_rebinned->Draw("histsame");
  }

  TLegend* legend = new TLegend(legendPosX, legendPosY, legendPosX + legendSizeX, legendPosY + legendSizeY, "", "brNDC"); 
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  legend->SetTextSize(legendTextSize);
  legend->AddEntry(histogram1_rebinned, legendEntry1.data(), "l");
  if ( histogram2_rebinned ) legend->AddEntry(histogram2_rebinned, legendEntry2.data(), "l");
  if ( histogram3_rebinned ) legend->AddEntry(histogram3_rebinned, legendEntry3.data(), "l");
  if ( histogram4_rebinned ) legend->AddEntry(histogram4_rebinned, legendEntry4.data(), "l");
  legend->Draw();

  TPaveText* label = 0;
  if ( labelTextLines.size() > 0 ) {
    label = new TPaveText(labelPosX, labelPosY, labelPosX + labelSizeX, labelPosY + labelSizeY, "brNDC");
    for ( std::vector<std::string>::const_iterator labelTextLine = labelTextLines.begin();
	  labelTextLine != labelTextLines.end(); ++labelTextLine ) {
      label->AddText(labelTextLine->data());
    }
    label->SetFillColor(10);
    label->SetBorderSize(0);
    label->SetTextColor(1);
    label->SetTextAlign(12);
    label->SetTextSize(labelTextSize);
    label->Draw();
  }

  canvas->Update();
  std::string outputFileName_plot = "plots/";
  size_t idx = outputFileName.find_last_of('.');
  outputFileName_plot.append(std::string(outputFileName, 0, idx));
  if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_plot).append(std::string(outputFileName, idx)).data());
  canvas->Print(std::string(outputFileName_plot).append(".png").data());
  canvas->Print(std::string(outputFileName_plot).append(".pdf").data());
  
  delete label;
  delete legend;
  delete canvas;  

  delete histogram1_rebinned;
  delete histogram2_rebinned;
  delete histogram3_rebinned;
  delete histogram4_rebinned;
}

struct histogram_vs_X_Type
{
  histogram_vs_X_Type(TH1* histogram, double x, double xErrUp, double xErrDown)
    : histogram_(histogram),
      x_(x),
      xErrUp_(xErrUp),
      xErrDown_(xErrDown)
  {}
   ~histogram_vs_X_Type() {}
  TH1* histogram_;
  double x_;
  double xErrUp_;
  double xErrDown_;
};

struct fitParameterType
{
  fitParameterType()
    : value_(0.),
      errUp_(0.),
      errDown_(0.)
  {}
  fitParameterType(const RooAbsReal* fitParameter)
    : errUp_(0.),
      errDown_(0.)
  {
    value_ = fitParameter->getVal();
    const RooRealVar* fitParameter_error = dynamic_cast<const RooRealVar*>(fitParameter);
    if ( fitParameter_error ) {
      if ( fitParameter_error->hasAsymError() ) {
	errUp_ = fitParameter_error->getErrorHi();
	errDown_ = fitParameter_error->getErrorLo();
      } else if ( fitParameter_error->hasError() ) {
	errUp_ = fitParameter_error->getError();
	errDown_ = fitParameter_error->getError();
      } 
    }
  }
  ~fitParameterType() {}
  double value_;
  double errUp_;
  double errDown_;
};

void fit(TH1* histogram, double xMin, double xMax, double massValue, std::vector<fitParameterType>& fitParameter)
{
  // create fit variable
  TAxis* xAxis = histogram->GetXaxis();
  if ( xMin < 0. ) xMin = xAxis->GetXmin(); 
  if ( xMax < 0. ) xMax = xAxis->GetXmax(); 
  std::string fitVariableName = Form("%s_fitVariable", histogram->GetName());
  RooRealVar fitVariable(fitVariableName.data(), fitVariableName.data(), xMin, xMax);
   
  // convert histogram to RooFit format
  std::string histogramName_data = Form("%s_data", histogram->GetName());
  RooDataHist histogram_data(histogramName_data.data(), histogramName_data.data(), fitVariable, histogram, 1.0);

  // create product of linear * Heaviside * Gaussian Error function
  std::string svFitLineShapeName_slope = Form("%s_svFitLineShape_slope", histogram->GetName());
  RooRealVar svFitLineShape_slope(svFitLineShapeName_slope.data(), svFitLineShapeName_slope.data(), 2./(massValue*massValue), 0., 1.);
  std::string svFitLineShapeName_offset = Form("%s_svFitLineShape_offset", histogram->GetName());
  RooRealVar svFitLineShape_offset(svFitLineShapeName_offset.data(), svFitLineShapeName_offset.data(), 0., -1., +1.);

  std::string heavisideName_threshold = Form("%s_heaviside_threshold", histogram->GetName());
  RooRealVar heaviside_threshold(heavisideName_threshold.data(), heavisideName_threshold.data(), 0.2*massValue, 1.e-2*massValue, 0.5*massValue);

  std::string sculptingName_bias = Form("%s_sculpting_bias", histogram->GetName());
  RooRealVar sculpting_bias(sculptingName_bias.data(), sculptingName_bias.data(), 0.25*massValue, 0.*massValue, 0.9*massValue);
  std::string sculptingName_width = Form("%s_sculpting_width", histogram->GetName());
  RooRealVar sculpting_width(sculptingName_width.data(), sculptingName_width.data(), 0.1*massValue, 1.e-2*massValue, 1.0*massValue);

  std::string svFitLineShapeName = Form("%s_svFitLineShape", histogram->GetName());
  std::string svFitLineShapeFormula = "(@0*@1 + @2)*0.5*(1.0 + TMath::Sign(+1, @0 - @3))*0.5*(1.0 + TMath::Erf((@0 - @4)/@5))";
  RooArgList svFitLineShapeArgs(fitVariable, 
				svFitLineShape_slope,
				svFitLineShape_offset,
				heaviside_threshold,
				sculpting_bias,
				sculpting_width);
  RooGenericPdf svFitLineShape(svFitLineShapeName.data(), svFitLineShapeName.data(), svFitLineShapeFormula.data(), svFitLineShapeArgs);

  // create Gaussian 
  std::string gaussianName_mean = Form("%s_gaussian_mean", histogram->GetName());
  RooConstVar gaussian_mean(gaussianName_mean.data(), gaussianName_mean.data(), 0.);
  std::string gaussianName_sigma = Form("%s_gaussian_sigma", histogram->GetName());
  RooRealVar gaussian_sigma(gaussianName_sigma.data(), gaussianName_sigma.data(), 0.2*massValue, 1.e-2*massValue, 0.5*massValue);
  std::string gaussianName = Form("%s_gaussian", histogram->GetName());
  RooGaussian gaussian(gaussianName.data(), gaussianName.data(), fitVariable, gaussian_mean, gaussian_sigma);

  // numerically convolute both PDFs using fast Fourier transform
  std::string fitFunctionName = Form("%s_fitFunction", histogram->GetName());
  RooFFTConvPdf fitFunction(fitFunctionName.data(), fitFunctionName.data(), fitVariable, svFitLineShape, gaussian);

  // fit histogram
  fitFunction.fitTo(histogram_data);

  // save fit parameter
  fitParameter.resize(6);
  fitParameter[0] = fitParameterType(&svFitLineShape_slope);
  fitParameter[1] = fitParameterType(&svFitLineShape_offset);
  fitParameter[2] = fitParameterType(&gaussian_sigma);
  fitParameter[3] = fitParameterType(&heaviside_threshold);
  fitParameter[4] = fitParameterType(&sculpting_bias);
  fitParameter[5] = fitParameterType(&sculpting_width);

  // create control plot
  std::string frameTitle = Form("%s_frame", histogram->GetName());
  RooPlot* frame = fitVariable.frame(RooFit::Title(frameTitle.data()));
  histogram_data.plotOn(frame);
  fitFunction.plotOn(frame, RooFit::LineColor(kRed));
  
  std::string canvasName = Form("%s_canvas", histogram->GetName());
  TCanvas* canvas = new TCanvas(canvasName.data(), canvasName.data(), 800, 600);
  gPad->SetLeftMargin(0.15); 
  frame->GetYaxis()->SetTitleOffset(1.4);
  frame->Draw();
    
  std::string outputFileName_plot = Form("svFitPerformance_%s_fit", histogram->GetName());
  canvas->Print(std::string(outputFileName_plot).append(".eps").data());
  canvas->Print(std::string(outputFileName_plot).append(".png").data());
  canvas->Print(std::string(outputFileName_plot).append(".pdf").data());
  
  delete canvas;
}

double square(double x)
{
  return x*x;
}

double fourth(double x)
{
  return x*x*x*x;
}

void compHistogram_yMax(const TH1* histogram, double& xMax, double& xMaxErr_high, double& xMaxErr_low)
{
  xMax = 0.;
  double yMax = -1.e+6;
  double yMaxErr = 0.;

  const TAxis* xAxis = histogram->GetXaxis();

//-- first iteration over histogram bins: find bin with maximum density (= binContent/binWidth)
  unsigned numBinsX = histogram->GetNbinsX();
  for ( unsigned iBin = 1; iBin <= numBinsX; ++iBin ) {
    double binContent = histogram->GetBinContent(iBin);
    double binError = histogram->GetBinError(iBin);
    
    double binCenter = xAxis->GetBinCenter(iBin);
    double binWidth = xAxis->GetBinWidth(iBin);
    if ( !(binWidth > 0.) ) continue;

    double binContent_div_binWidth = binContent/binWidth;
    double binError_div_binWidth = binError/binWidth;

    if ( binContent_div_binWidth > yMax ) {
      xMax = binCenter;
      yMax = binContent_div_binWidth;
      yMaxErr = binError_div_binWidth;
    }    
  }
  
  double xMax_high = -1.e+6;
  double xMax_low = +1.e+6;

//-- second iteration over histogram bins: find "lowest" and "highest" bins 
//   which reach the maximum density within one standard deviation of statistical uncertainties
  for ( unsigned iBin = 1; iBin <= numBinsX; ++iBin ) {
    double binContent = histogram->GetBinContent(iBin);
    double binError = histogram->GetBinError(iBin);
    
    double binCenter = xAxis->GetBinCenter(iBin);
    double binWidth = xAxis->GetBinWidth(iBin);
    if ( !(binWidth > 0.) ) continue;

    double binContent_div_binWidth = binContent/binWidth;
    double binError_div_binWidth = binError/binWidth;

    if ( (yMax - binContent_div_binWidth) < TMath::Sqrt(square(yMaxErr) + square(binError_div_binWidth)) ) {
      xMax_high = TMath::Max(xMax_high, binCenter);
      xMax_low = TMath::Min(xMax_low, binCenter);
    }
  }

  xMaxErr_high = xMax_high - xMax;
  xMaxErr_low = xMax - xMax_low;  
}

TGraph* makeGraph_response_asymmetric(std::vector<histogram_vs_X_Type>& histograms_vs_X, const std::vector<double>& y_true_array)
{
  //std::cout << "<makeGraph_response_asymmetric>:" << std::endl;

  unsigned numPoints = histograms_vs_X.size();
  assert(numPoints > 0);

  TGraphAsymmErrors* graph = new TGraphAsymmErrors(numPoints);

  for ( unsigned iPoint = 0; iPoint < numPoints; ++iPoint ) {
    double x = histograms_vs_X[iPoint].x_;
    double xErrUp = histograms_vs_X[iPoint].xErrUp_;
    double xErrDown = histograms_vs_X[iPoint].xErrDown_;

    TH1* histogram = histograms_vs_X[iPoint].histogram_;

    double max, maxErrUp, maxErrDown;
    compHistogram_yMax(histogram, max, maxErrUp, maxErrDown);
    //std::cout << "histogram = " << histogram->GetName() << ": max = " << max << " + " << maxErrUp << " - " << maxErrDown << std::endl;

    double y_true = y_true_array[iPoint];
    //std::cout << " y(true) = " << y_true << std::endl;

    double y = max/y_true;
    double yErrUp = maxErrUp/y_true;
    double yErrDown = maxErrDown/y_true;

    graph->SetPoint(iPoint, x, y);
    graph->SetPointError(iPoint, xErrDown, xErrUp, yErrDown, yErrUp);
  }

  return graph;
}

void compHistogram_rms(const TH1* histogram, double xRef, int firstBin, int lastBin,  double& rms, double& rmsErr)
{
  //std::cout << "<makeGraph_response_asymmetric>:" << std::endl;
  
  rms = 0.;
  rmsErr = 0.;
  
  const TAxis* xAxis = histogram->GetXaxis();

  double dx2Sum = 0.;
  double dx4Sum = 0.;
  double N_effective = 0.;

  for ( int iBin = firstBin; iBin < lastBin; ++iBin ) {
    double binContent = histogram->GetBinContent(iBin);
    double binError = histogram->GetBinError(iBin);
    if ( !(binError > 0.) ) continue;
    
    double binCenter = xAxis->GetBinCenter(iBin);

    double n_effective = square(binContent/binError);

    dx2Sum += n_effective*square(binCenter - xRef);
    dx4Sum += n_effective*fourth(binCenter - xRef);

    N_effective += n_effective;
  }

  if ( N_effective > 0. ) {
    rms = TMath::Sqrt(dx2Sum/N_effective);
    rmsErr = (1./(4.*square(rms)*N_effective))*(dx4Sum/N_effective - fourth(rms));
  }
}

void compHistogram_rms(const TH1* histogram, double xMax, double& rms_high, double& rmsErr_high, double& rms_low, double& rmsErr_low)
{
  int xMax_bin = (const_cast<TH1*>(histogram))->FindBin(xMax);

  compHistogram_rms(histogram, xMax, 1, xMax_bin, rms_low, rmsErr_low);

  int numBinsX = histogram->GetNbinsX();
  compHistogram_rms(histogram, xMax, xMax_bin, numBinsX, rms_high, rmsErr_high);
}

std::pair<TGraph*, TGraph*> makeGraph_resolution_asymmetric(std::vector<histogram_vs_X_Type>& histograms_vs_X)
{
  //std::cout << "<makeGraph_resolution_asymmetric>:" << std::endl;

  unsigned numPoints = histograms_vs_X.size();
  assert(numPoints > 0);

  TGraphAsymmErrors* graph_left = new TGraphAsymmErrors(numPoints);
  TGraphAsymmErrors* graph_right = new TGraphAsymmErrors(numPoints);

  for ( unsigned iPoint = 0; iPoint < numPoints; ++iPoint ) {
    double x = histograms_vs_X[iPoint].x_;
    double xErrUp = histograms_vs_X[iPoint].xErrUp_;
    double xErrDown = histograms_vs_X[iPoint].xErrDown_;

    TH1* histogram = histograms_vs_X[iPoint].histogram_;

    double max, maxErrUp, maxErrDown;
    compHistogram_yMax(histogram, max, maxErrUp, maxErrDown);
    //std::cout << "histogram = " << histogram->GetName() << ": max = " << max << " + " << maxErrUp << " - " << maxErrDown << std::endl;

    if ( !(max > 0.) ) continue;

    double rms_high_max, rmsErr_high_max, rms_low_max, rmsErr_low_max;
    compHistogram_rms(histogram, max, rms_high_max, rmsErr_high_max, rms_low_max, rmsErr_low_max);
    //std::cout << " rms: high = " << rms_high_max << " +/- " << rmsErr_high_max << "," 
    //	        << " low = " << rms_low_max << " +/- " << rmsErr_low_max << std::endl;

    double rms_high_maxErrUp, rmsErr_high_maxErrUp, rms_low_maxErrUp, rmsErr_low_maxErrUp;
    compHistogram_rms(histogram, max + maxErrUp, rms_high_maxErrUp, rmsErr_high_maxErrUp, rms_low_maxErrUp, rmsErr_low_maxErrUp);
    //std::cout << " rms err.up: high = " << rms_high_maxErrUp << " +/- " << rmsErr_high_maxErrUp << "," 
    //	        << " low = " << rms_low_maxErrUp << " +/- " << rmsErr_low_maxErrUp << std::endl;

    double rms_high_maxErrDown, rmsErr_high_maxErrDown, rms_low_maxErrDown, rmsErr_low_maxErrDown;
    compHistogram_rms(histogram, max - maxErrDown, rms_high_maxErrDown, rmsErr_high_maxErrDown, rms_low_maxErrDown, rmsErr_low_maxErrDown);
    //std::cout << " rms err.down: high = " << rms_high_maxErrDown << " +/- " << rmsErr_high_maxErrDown << "," 
    //	        << " low = " << rms_low_maxErrDown << " +/- " << rmsErr_low_maxErrDown << std::endl;

    double y_left = rms_low_max/max;
    double yErrUp_left = TMath::Sqrt(square(rmsErr_low_max) + square(rms_low_max - rms_low_maxErrDown))/max;
    double yErrDown_left = TMath::Sqrt(square(rmsErr_low_max) + square(rms_low_maxErrUp - rms_low_max))/max;

    graph_left->SetPoint(iPoint, x, y_left);
    graph_left->SetPointError(iPoint, xErrDown, xErrUp, yErrDown_left, yErrUp_left);

    double y_right = rms_high_max/max;
    double yErrUp_right = TMath::Sqrt(square(rmsErr_high_max) + square(rms_high_maxErrDown - rms_high_max))/max; 
    double yErrDown_right = TMath::Sqrt(square(rmsErr_high_max) + square(rms_high_max - rms_high_maxErrUp))/max;

    graph_right->SetPoint(iPoint, x, y_right);
    graph_right->SetPointError(iPoint, xErrDown, xErrUp, yErrDown_right, yErrUp_right);
  }

  return std::pair<TGraphAsymmErrors*, TGraphAsymmErrors*>(graph_left, graph_right);
}

TGraph* makeGraph(std::vector<histogram_vs_X_Type>& histograms_vs_X, const std::vector<double>& y_true_array, const std::string& mode)
{
  //std::cout << "<makeGraph>:" << std::endl;

  enum { kResponse, kResponse_asymmetric, kResolution, kResolution_asymmetric_left, kResolution_asymmetric_right, kFitResponse, kFitResolution };
  int mode_int = -1;
  if      ( mode == "response"                    ) mode_int = kResponse;
  else if ( mode == "response_asymmetric"         ) mode_int = kResponse_asymmetric;
  else if ( mode == "resolution"                  ) mode_int = kResolution;
  else if ( mode == "resolution_asymmetric_left"  ) mode_int = kResolution_asymmetric_left;
  else if ( mode == "resolution_asymmetric_right" ) mode_int = kResolution_asymmetric_right;
  else if ( mode == "fit_resolution"              ) mode_int = kFitResolution;
  else if ( mode == "fit_resolution"              ) mode_int = kFitResolution;
  else assert(0);

  unsigned numPoints = histograms_vs_X.size();
  //std::cout << " numPoints = " << numPoints << std::endl;
  assert(numPoints > 0);

  TGraphAsymmErrors* graph = new TGraphAsymmErrors(numPoints);

  for ( unsigned iPoint = 0; iPoint < numPoints; ++iPoint ) {
    double x = histograms_vs_X[iPoint].x_;
    double xErrUp = histograms_vs_X[iPoint].xErrUp_;
    double xErrDown = histograms_vs_X[iPoint].xErrDown_;

    TH1* histogram = histograms_vs_X[iPoint].histogram_;

    double histogram_mean = histogram->GetMean();
    double histogram_meanErr = histogram->GetMeanError();
    double histogram_rms = histogram->GetRMS();
    double histogram_rmsErr = histogram->GetRMSError();

    double y_true = y_true_array[iPoint];

    double y, yErrUp, yErrDown;
    if ( mode_int == kResponse ) {
      y = histogram_mean/y_true;
      yErrUp = histogram_meanErr/y_true;
      yErrDown = yErrUp;
    } else if ( mode_int == kResponse_asymmetric ) {
      return makeGraph_response_asymmetric(histograms_vs_X, y_true_array);
    } else if ( mode_int == kResolution ) {
      y = histogram_rms/histogram_mean;
      yErrUp = y*TMath::Sqrt(TMath::Power(histogram_rms/histogram_rmsErr, 2.) + TMath::Power(histogram_mean/histogram_meanErr, 2.));
      yErrDown = yErrUp;
    } else if ( mode_int == kResolution_asymmetric_left ) {
      return makeGraph_resolution_asymmetric(histograms_vs_X).first;
    } else if ( mode_int == kResolution_asymmetric_right ) {
      return makeGraph_resolution_asymmetric(histograms_vs_X).second;
    } else {
      std::vector<fitParameterType> fitParameter;
      fit(histogram, -1., -1., y_true, fitParameter);
      fitParameterType fitParameterOfInterest;
      if      ( mode_int == kFitResolution ) fitParameterOfInterest = fitParameter[2];
      else if ( mode_int == kFitResponse   ) fitParameterOfInterest = fitParameter[3];
      else assert(0);
      y = fitParameterOfInterest.value_;
      yErrUp = fitParameterOfInterest.errUp_;
      yErrDown = fitParameterOfInterest.errDown_;
    }

    graph->SetPoint(iPoint, x, y);
    graph->SetPointError(iPoint, xErrDown, xErrUp, yErrDown, yErrUp);
  }

  return graph;
}

TGraph* makeGraph(std::vector<histogram_vs_X_Type>& histograms_vs_X, double y_true, const std::string& mode)
{
  std::vector<double> y_true_array(histograms_vs_X.size());
  
  unsigned numHistograms = histograms_vs_X.size();
  for ( unsigned iHistogram = 0; iHistogram < numHistograms; ++iHistogram ) {
    y_true_array[iHistogram] = y_true;
  }
  
  return makeGraph(histograms_vs_X, y_true_array, mode);
}

void showGraphs(double canvasSizeX, double canvasSizeY,
		TGraph* graph1, const std::string& legendEntry1,
		TGraph* graph2, const std::string& legendEntry2,
		TGraph* graph3, const std::string& legendEntry3,
		TGraph* graph4, const std::string& legendEntry4,
		TGraph* graph5, const std::string& legendEntry5,
		TGraph* graph6, const std::string& legendEntry6,
		int colors[], int markerStyles[], 
		double legendTextSize, double legendPosX, double legendPosY, double legendSizeX, double legendSizeY, 
		std::vector<std::string>& labelTextLines, double labelTextSize,
		double labelPosX, double labelPosY, double labelSizeX, double labelSizeY,
		double xMin, double xMax, const std::string& xAxisTitle, double xAxisOffset,
		double yMin, double yMax, const std::string& yAxisTitle, double yAxisOffset,
		const std::string& outputFileName)
{
  TCanvas* canvas = new TCanvas("canvas", "canvas", canvasSizeX, canvasSizeY);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);
  
  canvas->SetLeftMargin(0.14);
  canvas->SetBottomMargin(0.12);

  TH1* dummyHistogram = new TH1D("dummyHistogram", "dummyHistogram", 100, xMin, xMax);
  dummyHistogram->SetTitle("");
  dummyHistogram->SetStats(false);
  dummyHistogram->SetMinimum(yMin);
  dummyHistogram->SetMaximum(yMax);

  TAxis* xAxis = dummyHistogram->GetXaxis();
  xAxis->SetTitle(xAxisTitle.data());
  xAxis->SetTitleSize(0.045);
  xAxis->SetTitleOffset(xAxisOffset);

  TAxis* yAxis = dummyHistogram->GetYaxis();
  yAxis->SetTitle(yAxisTitle.data());
  yAxis->SetTitleSize(0.045);
  yAxis->SetTitleOffset(yAxisOffset);

  dummyHistogram->Draw("axis");

  graph1->SetLineColor(colors[0]);
  graph1->SetLineWidth(2);
  graph1->SetMarkerColor(colors[0]);
  graph1->SetMarkerStyle(markerStyles[0]);
  graph1->SetMarkerSize(2);
  graph1->Draw("p");

  if ( graph2 ) {
    graph2->SetLineColor(colors[1]);
    graph2->SetLineWidth(2);
    graph2->SetMarkerColor(colors[1]);
    graph2->SetMarkerStyle(markerStyles[1]);
    graph2->SetMarkerSize(2);
    graph2->Draw("p");
  }
  
  if ( graph3 ) {
    graph3->SetLineColor(colors[2]);
    graph3->SetLineWidth(2);
    graph3->SetMarkerColor(colors[2]);
    graph3->SetMarkerStyle(markerStyles[2]);
    graph3->SetMarkerSize(2);
    graph3->Draw("p");
  }

  if ( graph4 ) {
    graph4->SetLineColor(colors[3]);
    graph4->SetLineWidth(2);
    graph4->SetMarkerColor(colors[3]);
    graph4->SetMarkerStyle(markerStyles[3]);
    graph4->SetMarkerSize(2);
    graph4->Draw("p");
  }

  if ( graph5 ) {
    graph5->SetLineColor(colors[4]);
    graph5->SetLineWidth(2);
    graph5->SetMarkerColor(colors[4]);
    graph5->SetMarkerStyle(markerStyles[4]);
    graph5->SetMarkerSize(2);
    graph5->Draw("p");
  }

  if ( graph6 ) {
    graph6->SetLineColor(colors[5]);
    graph6->SetLineWidth(2);
    graph6->SetMarkerColor(colors[5]);
    graph6->SetMarkerStyle(markerStyles[5]);
    graph6->SetMarkerSize(2);
    graph6->Draw("p");
  }
  
  TLegend* legend = new TLegend(legendPosX, legendPosY, legendPosX + legendSizeX, legendPosY + legendSizeY, "", "brNDC"); 
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  legend->SetTextSize(legendTextSize);
  legend->AddEntry(graph1, legendEntry1.data(), "p");
  if ( graph2 ) legend->AddEntry(graph2, legendEntry2.data(), "p");
  if ( graph3 ) legend->AddEntry(graph3, legendEntry3.data(), "p");
  if ( graph4 ) legend->AddEntry(graph4, legendEntry4.data(), "p");
  if ( graph5 ) legend->AddEntry(graph5, legendEntry5.data(), "p");
  if ( graph6 ) legend->AddEntry(graph6, legendEntry6.data(), "p");
  legend->Draw();

  TPaveText* label = 0;
  if ( labelTextLines.size() > 0 ) {
    label = new TPaveText(labelPosX, labelPosY, labelPosX + labelSizeX, labelPosY + labelSizeY, "brNDC");
    for ( std::vector<std::string>::const_iterator labelTextLine = labelTextLines.begin();
	  labelTextLine != labelTextLines.end(); ++labelTextLine ) {
      label->AddText(labelTextLine->data());
    }
    label->SetFillColor(10);
    label->SetBorderSize(0);
    label->SetTextColor(1);
    label->SetTextAlign(12);
    label->SetTextSize(labelTextSize);
    label->Draw();
  }

  canvas->Update();
  std::string outputFileName_plot = "plots/";
  size_t idx = outputFileName.find_last_of('.');
  outputFileName_plot.append(std::string(outputFileName, 0, idx));
  if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_plot).append(std::string(outputFileName, idx)).data());
  canvas->Print(std::string(outputFileName_plot).append(".png").data());
  canvas->Print(std::string(outputFileName_plot).append(".pdf").data());
  
  delete dummyHistogram;
  delete label;
  delete legend;
  delete canvas;  
}

void makeSVfitPerformancePlots_AHtautau()
{
  std::string inputFileName = "/data1/veelken/tmp/svFitStudies/all/svFitPerformanceAnalysisPlots_all_2012Mar13.root";
  TFile* inputFile = new TFile(inputFileName.data());

  gROOT->SetBatch(true);

  std::string directory_PSkine_woLogM_Int      = "nSVfitAnalyzerOption1b";
  std::string directory_PSkine_woLogM_Fit      = "nSVfitAnalyzerOption1a";
  std::string directory_PSkine_wLogM_Int       = "nSVfitAnalyzerOption0b";
  std::string directory_PSkine_wLogM_Fit       = "nSVfitAnalyzerOption0a";
  std::string directory_MCkine_all_Int         = "nSVfitAnalyzerOption4b";
  std::string directory_MCkine_selected_Int    = "nSVfitAnalyzerOption5b";
  std::string directory_MEkine1_woPol_Int      = "nSVfitAnalyzerOption6b";
  std::string directory_MEkine1_woPol_Fit      = "nSVfitAnalyzerOption6a";
  std::string directory_MEkine12_wPolZorAH_Int = "nSVfitAnalyzerOption12b";
  std::string directory_MEkine12_wPolZorAH_Fit = "nSVfitAnalyzerOption12a";
  
  std::string histogramName_svFitMass = "svFitMass";
  std::string xAxisTitle_svFitMass    = "M_{#tau#tau} / GeV";
  std::string histogramName_dPhi12    = "dPhi12";

  typedef std::pair<double, double> pdouble;
  std::vector<pdouble> xRanges_dPhi;
  xRanges_dPhi.push_back(pdouble(  0.,  30.));
  xRanges_dPhi.push_back(pdouble( 30.,  60.));
  xRanges_dPhi.push_back(pdouble( 60.,  90.));
  xRanges_dPhi.push_back(pdouble( 90., 120.));
  xRanges_dPhi.push_back(pdouble(120., 140.));
  xRanges_dPhi.push_back(pdouble(140., 160.));
  xRanges_dPhi.push_back(pdouble(160., 170.));
  xRanges_dPhi.push_back(pdouble(170., 175.));
  xRanges_dPhi.push_back(pdouble(175., 180.));

  std::vector<double> xPoints_metResolution;
  xPoints_metResolution.push_back(5.);
  xPoints_metResolution.push_back(10.);
  xPoints_metResolution.push_back(15.);
  xPoints_metResolution.push_back(20.);

  std::vector<double> massPoints;
  massPoints.push_back(90.);
  massPoints.push_back(120.);
  massPoints.push_back(130.);
  massPoints.push_back(160.);
  massPoints.push_back(200.);
  massPoints.push_back(300.);
  massPoints.push_back(450.);

  std::vector<double> massPointsToCompare;
  massPointsToCompare.push_back(90.);
  massPointsToCompare.push_back(130.);
  massPointsToCompare.push_back(300.);

  double metResolution_nominal = -1.;
  //double metResolution_nominal = 15.;

  std::vector<std::string> channels;
  channels.push_back("eleMu");
  channels.push_back("muTau");
  channels.push_back("diTau");

  std::vector<std::string> label_ZplusJets;
  label_ZplusJets.push_back(std::string("Sim. Z #rightarrow #tau#tau"));

  std::vector<std::string> label_Higgs130;
  label_Higgs130.push_back(std::string("Sim. H #rightarrow #tau#tau"));
  label_Higgs130.push_back(std::string("M = 130 GeV"));

  std::vector<std::string> label_Higgs300;
  label_Higgs300.push_back(std::string("Sim. H #rightarrow #tau#tau"));
  label_Higgs300.push_back(std::string("M = 300 GeV"));

  //-----------------------------------------------------------------------------
  // make plots comparing Fit vs. Integration 
  // in terms of resolution
  //-----------------------------------------------------------------------------

  for ( std::vector<std::string>::const_iterator channel = channels.begin();
	channel != channels.end(); ++channel ) {
    for ( std::vector<double>::const_iterator massPoint = massPointsToCompare.begin();
	  massPoint != massPointsToCompare.end(); ++massPoint ) {
      TH1* histogram1 = 
	getHistogram(inputFile, *channel, *massPoint, directory_PSkine_woLogM_Int, 
		     histogramName_svFitMass, metResolution_nominal);
      std::string legendEntry1 = "PS, Int";
      TH1* histogram2 = 
	getHistogram(inputFile, *channel, *massPoint, directory_PSkine_wLogM_Fit, 
		     histogramName_svFitMass, metResolution_nominal);
      std::string legendEntry2 = "PS + log(M), Fit";
      std::vector<std::string> label;
      std::string process = "";
      double xMax = -1.;
      if ( (*massPoint) == 90. ) {
	label = label_ZplusJets;
	process = "ZplusJets";
	xMax = 250.;
      } else if  ( (*massPoint) == 130. ) {
	label = label_Higgs130;
	process = "Higgs130";
	xMax = 250.;
      } else if ( (*massPoint) == 300. ) {
	label = label_Higgs300;
	process = "Higgs300";
	xMax = 500;
      } else assert(0);
      double yMax = 0.;
      if ( (*channel) == "eleMu" ) {
	if      ( (*massPoint) <  95. ) yMax = 0.30;
	else if ( (*massPoint) < 135. ) yMax = 0.20;
	else                            yMax = 0.15;
      } else if ( (*channel) == "muTau" ) {
	if      ( (*massPoint) <  95. ) yMax = 0.30;
	else if ( (*massPoint) < 135. ) yMax = 0.25;
	else                            yMax = 0.15;
      } else if ( (*channel) == "diTau" ) {	
	if      ( (*massPoint) <  95. ) yMax = 0.40;
	else if ( (*massPoint) < 135. ) yMax = 0.30;
	else                            yMax = 0.20;
      } else assert(0);
      double labelSizeY = label.size()*0.05;
      showHistograms(800, 600, 
		     histogram1, legendEntry1,
		     histogram2, legendEntry2,
		     NULL, "",
		     NULL, "",
		     0.04, 0.61, 0.74, 0.28, 0.15,
		     label, 0.04, 0.17, 0.89 - labelSizeY, 0.18, labelSizeY, 
		     0., xMax, xAxisTitle_svFitMass, 1.2,
		     false, 0., yMax, "a.u.", 1.4,
		     Form("svFitPerformance_%s_%s_Fit_vs_Int_linear.eps", process.data(), channel->data()));
      showHistograms(800, 600, 
		     histogram1, legendEntry1,
		     histogram2, legendEntry2,
		     NULL, "",
		     NULL, "",
		     0.04, 0.61, 0.74, 0.28, 0.15,
		     label, 0.04, 0.17, 0.89 - labelSizeY, 0.18, labelSizeY, 
		     0., xMax, xAxisTitle_svFitMass, 1.2,
		     true, 1.e-4, 1.e0, "a.u.", 1.4,
		     Form("svFitPerformance_%s_%s_Fit_vs_Int_log.eps", process.data(), channel->data()));
    }
  }
    
  //-----------------------------------------------------------------------------
  // make plots comparing different Likelihood models 
  // for leptonic and hadronic tau decays (in Integration mode)
  // in terms of resolution
  //-----------------------------------------------------------------------------
    
  for ( std::vector<std::string>::const_iterator channel = channels.begin();
	channel != channels.end(); ++channel ) {
    for ( std::vector<double>::const_iterator massPoint = massPointsToCompare.begin();
	  massPoint != massPointsToCompare.end(); ++massPoint ) {
      TH1* histogram1 = 0;
      std::string legendEntry1 = "";
      TH1* histogram2 = 0;
      std::string legendEntry2 = "";
      TH1* histogram3 = 0;
      std::string legendEntry3 = "";
      TH1* histogram4 = 0;
      std::string legendEntry4 = "";
      double legendSizeX = 0.;      
      double yMax = 0.;
      if ( (*channel) == "eleMu" ) {
	histogram1 = 
	  getHistogram(inputFile, *channel, *massPoint, directory_PSkine_woLogM_Int, 
		       histogramName_svFitMass, metResolution_nominal);
	legendEntry1 = "PS, Int";
	histogram2 = 
	  getHistogram(inputFile, *channel, *massPoint, directory_MEkine12_wPolZorAH_Int, 
		       histogramName_svFitMass, metResolution_nominal);
	legendEntry2 = "ME, Int";			
	//histogram3 = 
	//  getHistogram(inputFile, *channel, *massPoint, directory_MCkine_selected_Int, 
	//	         histogramName_svFitMass, metResolution_nominal);
	//legendEntry3 = "MC, Int";
	legendSizeX = 0.12;
	if      ( (*massPoint) <  95. ) yMax = 0.30;
	else if ( (*massPoint) < 135. ) yMax = 0.20;
	else                            yMax = 0.15;
      } else if ( (*channel) == "muTau" ) {
        histogram1 = 
	  getHistogram(inputFile, *channel, *massPoint, directory_MEkine1_woPol_Int, 
		       histogramName_svFitMass, metResolution_nominal);
	legendEntry1 = "ME_{lep} + PS_{had}, Int";
	histogram2 = 
	  getHistogram(inputFile, *channel, *massPoint, directory_PSkine_woLogM_Int, 
		       histogramName_svFitMass, metResolution_nominal);
	legendEntry2 = "PS_{lep} + PS_{had}, Int";
	histogram3 = 
	  getHistogram(inputFile, *channel, *massPoint, directory_MEkine12_wPolZorAH_Int, 
		       histogramName_svFitMass, metResolution_nominal);
	legendEntry3 = "ME_{lep} + ME_{had}, Int";	
	//histogram4 = 
	//  getHistogram(inputFile, *channel, *massPoint, directory_MCkine_selected_Int, 
	//	         histogramName_svFitMass, metResolution_nominal);
	//legendEntry4 = "MC_{lep} + MC_{had}, Int";
	legendSizeX = 0.28;
	if      ( (*massPoint) <  95. ) yMax = 0.30;
	else if ( (*massPoint) < 135. ) yMax = 0.25;
	else                            yMax = 0.15;
      } else if ( (*channel) == "diTau" ) {	
	histogram1 = 
	  getHistogram(inputFile, *channel, *massPoint, directory_PSkine_woLogM_Int, 
		       histogramName_svFitMass, metResolution_nominal);
	legendEntry1 = "PS, Int";
	histogram2 = 
	  getHistogram(inputFile, *channel, *massPoint, directory_MEkine12_wPolZorAH_Int, 
		       histogramName_svFitMass, metResolution_nominal);
	legendEntry2 = "ME, Int";
	//histogram3 = 
	//  getHistogram(inputFile, *channel, *massPoint, directory_MCkine_selected_Int, 
	//	       histogramName_svFitMass, metResolution_nominal);
	//legendEntry3 = "MC, Int";
	legendSizeX = 0.12;
	if      ( (*massPoint) <  95. ) yMax = 0.40;
	else if ( (*massPoint) < 135. ) yMax = 0.30;
	else                            yMax = 0.20;
      } else assert(0);
      unsigned numLegendEntries = 0;
      if ( legendEntry1 != "" ) ++numLegendEntries;
      if ( legendEntry2 != "" ) ++numLegendEntries;
      if ( legendEntry3 != "" ) ++numLegendEntries;
      if ( legendEntry4 != "" ) ++numLegendEntries;
      double legendSizeY = 0.05*numLegendEntries;
      std::vector<std::string> label;
      std::string process = "";
      double xMax = -1.;
      if ( (*massPoint) == 90. ) {
	label = label_ZplusJets;
	process = "ZplusJets";
	xMax = 250.;
      } else if  ( (*massPoint) == 130. ) {
	label = label_Higgs130;
	process = "Higgs130";
	xMax = 250.;
      } else if ( (*massPoint) == 300. ) {
	label = label_Higgs300;
	process = "Higgs300";
	xMax = 500;
      } else assert(0);
      double labelSizeY = label.size()*0.05;
      showHistograms(800, 600, 
		     histogram1, legendEntry1,
		     histogram2, legendEntry2,
		     histogram3, legendEntry3,
		     histogram4, legendEntry4,
		     0.04, 0.89 - legendSizeX, 0.89 - legendSizeY, legendSizeX, legendSizeY,
		     label, 0.04, 0.17, 0.89 - labelSizeY, 0.18, labelSizeY, 
		     0., xMax, xAxisTitle_svFitMass, 1.2,
		     false, 0., yMax, "a.u.", 1.4,
		     Form("svFitPerformance_%s_%s_compLikelihoodModels_linear.eps", process.data(), channel->data()));
      showHistograms(800, 600, 
		     histogram1, legendEntry1,
		     histogram2, legendEntry2,
		     histogram3, legendEntry3,
		     histogram4, legendEntry4,
		     0.04, 0.89 - legendSizeX, 0.74, legendSizeX, 0.15,
		     label, 0.04, 0.17, 0.89 - labelSizeY, 0.18, labelSizeY, 
		     0., xMax, xAxisTitle_svFitMass, 1.2,
		     true, 1.e-4, 1.e0, "a.u.", 1.4,
		     Form("svFitPerformance_%s_%s_compLikelihoodModels_log.eps", process.data(), channel->data()));
    }
  }

  //-----------------------------------------------------------------------------
  // make plots comparing SVfit mass distributions 
  // reconstructed in different bins of dPhi(leg1, leg2)
  // in terms of response and resolution
  //-----------------------------------------------------------------------------
    
  for ( std::vector<std::string>::const_iterator channel = channels.begin();
	channel != channels.end(); ++channel ) {
    for ( std::vector<double>::const_iterator massPoint = massPointsToCompare.begin();
	  massPoint != massPointsToCompare.end(); ++massPoint ) {
      std::vector<std::string> label;
      std::string process = "";
      double xMax = -1.;
      if ( (*massPoint) == 90. ) {
	label = label_ZplusJets;
	process = "ZplusJets";
	xMax = 250.;
      } else if  ( (*massPoint) == 130. ) {
	label = label_Higgs130;
	process = "Higgs130";
	xMax = 250.;
      } else if ( (*massPoint) == 300. ) {
	label = label_Higgs300;
	process = "Higgs300";
	xMax = 500;
      } else assert(0);
      for ( std::vector<pdouble>::const_iterator xRange_dPhi = xRanges_dPhi.begin();
	    xRange_dPhi != xRanges_dPhi.end(); ++xRange_dPhi ) {
	double dPhi_min = xRange_dPhi->first;
	double dPhi_max = xRange_dPhi->second;
	std::string dPhi_label;
	if      ( dPhi_min >  0. && dPhi_max <  180. ) dPhi_label = Form("dPhi%1.0fto%1.0f", dPhi_min, dPhi_max);
	else if ( dPhi_min == 0. && dPhi_max <  180. ) dPhi_label = Form("dPhiLt%1.0f", dPhi_max);
	else if ( dPhi_min >  0. && dPhi_max == 180. ) dPhi_label = Form("dPhiGt%1.0f", dPhi_min);
	else assert(0);
	std::string histogramName_svFitMass_full = Form("%s/%s", dPhi_label.data(), histogramName_svFitMass.data());
	TH1* histogram = 
	  getHistogram(inputFile, *channel, *massPoint, directory_PSkine_woLogM_Int, 
		       histogramName_svFitMass_full, metResolution_nominal);
	std::string dPhi_label2;
	if      ( dPhi_min >  0. && dPhi_max <  180. ) dPhi_label2 = Form("%1.0f < #Delta#phi < %1.0f", dPhi_min, dPhi_max);
	else if ( dPhi_min == 0. && dPhi_max <  180. ) dPhi_label2 = Form("#Delta#phi < %1.0f", dPhi_max);
	else if ( dPhi_min >  0. && dPhi_max == 180. ) dPhi_label2 = Form("#Delta#phi > %1.0f", dPhi_min);
	else assert(0);
	std::vector<std::string> label_full = label;
	label_full.push_back(dPhi_label2);
	showHistograms(800, 600, 
		       histogram, "PS, Int", 
		       NULL, "",
		       NULL, "",
		       NULL, "",
		       0.04, 0.61, 0.74, 0.28, 0.15,
		       label, 0.04, 0.175, 0.725, 0.24, 0.165, 
		       0., xMax, xAxisTitle_svFitMass, 1.2,
		       false, 0., 0.35, "a.u.", 1.4,
		       Form("svFitPerformance_%s_%s_PSkine_woLogM_Int_%s_linear.eps", process.data(), channel->data(), dPhi_label.data()));
	showHistograms(800, 600, 
		       histogram, "PS, Int", 
		       NULL, "",
		       NULL, "",
		       NULL, "",
		       0.04, 0.61, 0.74, 0.28, 0.15,
		       label, 0.04, 0.175, 0.725, 0.24, 0.165, 
		       0., xMax, xAxisTitle_svFitMass, 1.2,
		       true, 1.e-4, 1.e0, "a.u.", 1.4,
		       Form("svFitPerformance_%s_%s_PSkine_woLogM_Int_%s_log.eps", process.data(), channel->data(), dPhi_label.data()));
      }
    }
  }
  
  int colors_style1[3] = { 1, 2, 4 };                      // style 1: used to show respone and symmetric resolution for Z vs. Higgs(130) vs. Higgs (300)
  int markerStyles_style1[3] = { 20, 21, 33 };

  int colors_style2[6] = { 1, 1, 2, 2, 4, 4 };             // style 2: used to show asymmetric resolution for Z vs. Higgs(130) vs. Higgs (300)
  int markerStyles_style2[6] = { 22, 32, 20, 24, 21, 25 }; //   (22 = right tail of resolution, 23 = left tail of resolution)
  
  for ( std::vector<std::string>::const_iterator channel = channels.begin();
	channel != channels.end(); ++channel ) {
    std::cout << "processing channel = " << (*channel) << std::endl;
    std::map<std::string, std::vector<histogram_vs_X_Type> > histograms_vs_dPhi; // key = process
    for ( std::vector<double>::const_iterator massPoint = massPointsToCompare.begin();
	  massPoint != massPointsToCompare.end(); ++massPoint ) {
      std::string process = "";
      if      ( (*massPoint) ==  90. ) process = "ZplusJets";
      else if ( (*massPoint) == 130. ) process = "Higgs130";
      else if ( (*massPoint) == 300. ) process = "Higgs300";
      else assert(0);
      unsigned numRanges_dPhi = xRanges_dPhi.size();
      unsigned rebin_dPhi = 2;
      for ( unsigned iRange_dPhi = 0; iRange_dPhi < numRanges_dPhi; iRange_dPhi += rebin_dPhi ) {
	double dPhi_min = +1.e+3;
	double dPhi_max = -1.e+3;
	TH1* histogram_dPhi12 = 0;
	TH1* histogram_svFitMass = 0;
	for ( unsigned jRange_dPhi = iRange_dPhi; jRange_dPhi < TMath::Min(iRange_dPhi + rebin_dPhi, numRanges_dPhi); ++jRange_dPhi ) {
	  pdouble xRange_dPhi = xRanges_dPhi[jRange_dPhi];
	  double dPhi_min_j = xRange_dPhi.first;
	  dPhi_min = TMath::Min(dPhi_min, dPhi_min_j);
	  double dPhi_max_j = xRange_dPhi.second;
	  dPhi_max = TMath::Max(dPhi_max, dPhi_max_j);
	  std::string dPhi_label;
	  if      ( dPhi_min_j >  0. && dPhi_max_j <  180. ) dPhi_label = Form("dPhi%1.0fto%1.0f", dPhi_min_j, dPhi_max_j);
	  else if ( dPhi_min_j == 0. && dPhi_max_j <  180. ) dPhi_label = Form("dPhiLt%1.0f", dPhi_max_j);
	  else if ( dPhi_min_j >  0. && dPhi_max_j == 180. ) dPhi_label = Form("dPhiGt%1.0f", dPhi_min_j);
	  else assert(0);
	  std::string histogramName_dPhi12_full = Form("%s/%s", dPhi_label.data(), histogramName_dPhi12.data());
	  TH1* histogram_dPhi12_j = 
	    getHistogram(inputFile, *channel, *massPoint, directory_PSkine_woLogM_Int, 
			 histogramName_dPhi12_full, metResolution_nominal);
	  if ( !histogram_dPhi12 ) {
	    histogram_dPhi12 = (TH1*)histogram_dPhi12_j->Clone(std::string(histogram_dPhi12_j->GetName()).append("_cloned").data());
	  } else {
	    histogram_dPhi12->Add(histogram_dPhi12_j);
	  }
	  std::string histogramName_svFitMass_full = Form("%s/%s", dPhi_label.data(), histogramName_svFitMass.data());
	  TH1* histogram_svFitMass_j = 
	    getHistogram(inputFile, *channel, *massPoint, directory_PSkine_woLogM_Int, 
			 histogramName_svFitMass_full, metResolution_nominal);
	  if ( !histogram_svFitMass ) {
	    histogram_svFitMass = (TH1*)histogram_svFitMass_j->Clone(std::string(histogram_svFitMass_j->GetName()).append("_cloned").data());
	  } else {
	    histogram_svFitMass->Add(histogram_svFitMass_j);
	  }
	}
	double dPhi_mean = histogram_dPhi12->GetMean();
	std::cout << " adding histogram for process = " << process << ", dPhi = " << dPhi_min << ".." << dPhi_max << std::endl;
	histograms_vs_dPhi[process].push_back(
	  histogram_vs_X_Type(histogram_svFitMass, dPhi_mean, dPhi_max - dPhi_mean, dPhi_mean - dPhi_min));
      }    
    }
    std::vector<std::string> label_dPhi;
    TGraph* graph_ZplusJets_response = makeGraph(histograms_vs_dPhi["ZplusJets"], 90., "response");
    TGraph* graph_Higgs130_response  = makeGraph(histograms_vs_dPhi["Higgs130"], 130., "response");
    TGraph* graph_Higgs300_response  = makeGraph(histograms_vs_dPhi["Higgs300"], 300., "response");
    showGraphs(800, 600,
	       graph_ZplusJets_response, "Sim. Z #rightarrow #tau#tau", 
	       graph_Higgs130_response, "Sim. H_{130} #rightarrow #tau#tau",
	       graph_Higgs300_response, "Sim. H_{300} #rightarrow #tau#tau",
	       NULL, "", 
	       NULL, "", 
	       NULL, "", 
	       colors_style1, markerStyles_style1,
	       0.04, 0.61, 0.74, 0.28, 0.15,
	       label_dPhi, 0.04, 0.175, 0.725, 0.24, 0.165, 
	       0., 180., "#Delta#phi / #circ", 1.2,
	       0.75, 1.25, "<M_{#tau#tau}>/M", 1.4,
	       Form("svFitPerformance_%s_PSkine_woLogM_Int_response_vs_dPhi.eps", channel->data()));
    TGraph* graph_ZplusJets_response_asymmetric = makeGraph(histograms_vs_dPhi["ZplusJets"], 90., "response_asymmetric");
    TGraph* graph_Higgs130_response_asymmetric  = makeGraph(histograms_vs_dPhi["Higgs130"], 130., "response_asymmetric");
    TGraph* graph_Higgs300_response_asymmetric  = makeGraph(histograms_vs_dPhi["Higgs300"], 300., "response_asymmetric");
    showGraphs(800, 600,
	       graph_ZplusJets_response_asymmetric, "Sim. Z #rightarrow #tau#tau", 
	       graph_Higgs130_response_asymmetric, "Sim. H_{130} #rightarrow #tau#tau",
	       graph_Higgs300_response_asymmetric, "Sim. H_{300} #rightarrow #tau#tau",
	       NULL, "", 
	       NULL, "", 
	       NULL, "", 
	       colors_style1, markerStyles_style1,
	       0.04, 0.61, 0.74, 0.28, 0.15,
	       label_dPhi, 0.04, 0.175, 0.725, 0.24, 0.165, 
	       0., 180., "#Delta#phi / #circ", 1.2,
	       0.75, 1.25, "M_{#tau#tau}^{max}/M", 1.4,
	       Form("svFitPerformance_%s_PSkine_woLogM_Int_response_vs_dPhi_asymmetric.eps", channel->data()));
    TGraph* graph_ZplusJets_resolution = makeGraph(histograms_vs_dPhi["ZplusJets"], 90., "resolution");
    TGraph* graph_Higgs130_resolution  = makeGraph(histograms_vs_dPhi["Higgs130"], 130., "resolution");
    TGraph* graph_Higgs300_resolution  = makeGraph(histograms_vs_dPhi["Higgs300"], 300., "resolution");
    showGraphs(800, 600,
	       graph_ZplusJets_resolution, "Sim. Z #rightarrow #tau#tau", 
	       graph_Higgs130_resolution, "Sim. H_{130} #rightarrow #tau#tau",
	       graph_Higgs300_resolution, "Sim. H_{300} #rightarrow #tau#tau",
	       NULL, "", 
	       NULL, "", 
	       NULL, "", 
	       colors_style1, markerStyles_style1,
	       0.04, 0.61, 0.74, 0.28, 0.15,
	       label_dPhi, 0.04, 0.175, 0.725, 0.24, 0.165, 
	       0., 180., "#Delta#phi / #circ", 1.2,
	       0., 0.50, "#sigmaM_{#tau#tau}/<M_{#tau#tau}>", 1.4,
	       Form("svFitPerformance_%s_PSkine_woLogM_Int_resolution_vs_dPhi.eps", channel->data()));
    TGraph* graph_ZplusJets_resolution_asymmetric_left  = makeGraph(histograms_vs_dPhi["ZplusJets"], 90., "resolution_asymmetric_left");
    TGraph* graph_ZplusJets_resolution_asymmetric_right = makeGraph(histograms_vs_dPhi["ZplusJets"], 90., "resolution_asymmetric_right");
    TGraph* graph_Higgs130_resolution_asymmetric_left   = makeGraph(histograms_vs_dPhi["Higgs130"], 130., "resolution_asymmetric_left");
    TGraph* graph_Higgs130_resolution_asymmetric_right  = makeGraph(histograms_vs_dPhi["Higgs130"], 130., "resolution_asymmetric_right");
    TGraph* graph_Higgs300_resolution_asymmetric_left   = makeGraph(histograms_vs_dPhi["Higgs300"], 300., "resolution_asymmetric_left");
    TGraph* graph_Higgs300_resolution_asymmetric_right  = makeGraph(histograms_vs_dPhi["Higgs300"], 300., "resolution_asymmetric_right");
    showGraphs(800, 600,
	       graph_ZplusJets_resolution_asymmetric_right, "Sim. Z #rightarrow #tau#tau +",
	       graph_ZplusJets_resolution_asymmetric_left, "Sim. Z #rightarrow #tau#tau -",
	       graph_Higgs130_resolution_asymmetric_right, "Sim. H_{130} #rightarrow #tau#tau +",
	       graph_Higgs130_resolution_asymmetric_left, "Sim. H_{130} #rightarrow #tau#tau -",
	       graph_Higgs300_resolution_asymmetric_right, "Sim. H_{300} #rightarrow #tau#tau +",
	       graph_Higgs300_resolution_asymmetric_left, "Sim. H_{300} #rightarrow #tau#tau -",
	       colors_style2, markerStyles_style2,
	       0.04, 0.175, 0.64, 0.30, 0.25,
	       label_dPhi, 0.04, 0.175, 0.725, 0.24, 0.165, 
	       0., 180., "#Delta#phi / #circ", 1.2,
	       0., 0.60, "#sigmaM_{#tau#tau}/M_{#tau#tau}^{max}", 1.4,
	       Form("svFitPerformance_%s_PSkine_woLogM_Int_resolution_vs_dPhi_asymmetric.eps", channel->data()));
   }

  //-----------------------------------------------------------------------------
  // make plots comparing SVfit mass distributions 
  // obtained for different values of MET resolution
  // in terms of response and resolution
  //-----------------------------------------------------------------------------
    
  for ( std::vector<std::string>::const_iterator channel = channels.begin();
	channel != channels.end(); ++channel ) {
    for ( std::vector<double>::const_iterator massPoint = massPointsToCompare.begin();
	  massPoint != massPointsToCompare.end(); ++massPoint ) {
      std::vector<std::string> label;
      std::string process = "";
      double xMax = -1.;
      if ( (*massPoint) == 90. ) {
	label = label_ZplusJets;
	process = "ZplusJets";
	xMax = 250.;
      } else if  ( (*massPoint) == 130. ) {
	label = label_Higgs130;
	process = "Higgs130";
	xMax = 250.;
      } else if ( (*massPoint) == 300. ) {
	label = label_Higgs300;
	process = "Higgs300";
	xMax = 500;
      } else assert(0);
      int numEntries = TMath::Max(4, (int)xPoints_metResolution.size());
      std::vector<TH1*> histograms(numEntries);
      std::vector<std::string> legendEntries(numEntries);
      int idx = 0;
      for ( std::vector<double>::const_iterator metResolution = xPoints_metResolution.begin();
	    metResolution != xPoints_metResolution.end(); ++metResolution ) {
	histograms[idx] =  
	  getHistogram(inputFile, *channel, *massPoint, directory_PSkine_woLogM_Int, 
		       histogramName_svFitMass, *metResolution);
	legendEntries[idx] = Form("#sigma_{X} = #sigma_{Y} = %1.0f GeV", *metResolution);
	++idx;
      }
      showHistograms(800, 600, 
		     histograms[0], legendEntries[0],
		     histograms[1], legendEntries[1],
		     histograms[2], legendEntries[2],
		     histograms[3], legendEntries[3],
		     0.04, 0.61, 0.74, 0.28, 0.15,
		     label, 0.04, 0.175, 0.725, 0.24, 0.165, 
		     0., xMax, xAxisTitle_svFitMass, 1.2,
		     false, 0., 0.35, "a.u.", 1.4,
		     Form("svFitPerformance_%s_%s_PSkine_woLogM_Int_vs_metResolution_linear.eps", process.data(), channel->data()));
      showHistograms(800, 600,
		     histograms[0], legendEntries[0],
		     histograms[1], legendEntries[1],
		     histograms[2], legendEntries[2],
		     histograms[3], legendEntries[3],
		     0.04, 0.61, 0.74, 0.28, 0.15,
		     label, 0.04, 0.175, 0.725, 0.24, 0.165, 
		     0., xMax, xAxisTitle_svFitMass, 1.2,
		     true, 1.e-4, 1.e0, "a.u.", 1.4,
		     Form("svFitPerformance_%s_%s_PSkine_woLogM_Int_vs_metResolution_log.eps", process.data(), channel->data()));
    }
  }

  for ( std::vector<std::string>::const_iterator channel = channels.begin();
	channel != channels.end(); ++channel ) {
    std::map<std::string, std::vector<histogram_vs_X_Type> > histograms_vs_metResolution; // key = process
    for ( std::vector<double>::const_iterator massPoint = massPointsToCompare.begin();
	  massPoint != massPointsToCompare.end(); ++massPoint ) {
      std::string process = "";
      if      ( (*massPoint) ==  90. ) process = "ZplusJets";
      else if ( (*massPoint) == 130. ) process = "Higgs130";
      else if ( (*massPoint) == 300. ) process = "Higgs300";
      else assert(0);
      for ( std::vector<double>::const_iterator metResolution = xPoints_metResolution.begin();
	    metResolution != xPoints_metResolution.end(); ++metResolution ) {
	TH1* histogram =  
	  getHistogram(inputFile, *channel, *massPoint, directory_PSkine_woLogM_Int, 
		       histogramName_svFitMass, *metResolution);
	histograms_vs_metResolution[process].push_back(
	  histogram_vs_X_Type(histogram, *metResolution, 0., 0.));
      }    
    }
    std::vector<std::string> label_metResolution;
    TGraph* graph_ZplusJets_response = makeGraph(histograms_vs_metResolution["ZplusJets"], 90., "response");
    TGraph* graph_Higgs130_response  = makeGraph(histograms_vs_metResolution["Higgs130"], 130., "response");
    TGraph* graph_Higgs300_response  = makeGraph(histograms_vs_metResolution["Higgs300"], 300., "response");
    showGraphs(800, 600,
	       graph_ZplusJets_response, "Sim. Z #rightarrow #tau#tau", 
	       graph_Higgs130_response, "Sim. H_{130} #rightarrow #tau#tau",
	       graph_Higgs300_response, "Sim. H_{300} #rightarrow #tau#tau",
	       NULL, "", 
	       NULL, "", 
	       NULL, "", 
	       colors_style1, markerStyles_style1,
	       0.04, 0.61, 0.74, 0.28, 0.15,
	       label_metResolution, 0.04, 0.175, 0.725, 0.24, 0.165, 
	       0., 25., "#sigma_{X/Y} / GeV", 1.2,
	       0.75, 1.25, "<M_{#tau#tau}>/M", 1.4,
	       Form("svFitPerformance_%s_PSkine_woLogM_Int_response_vs_metResolution.eps", channel->data()));
    TGraph* graph_ZplusJets_response_asymmetric = makeGraph(histograms_vs_metResolution["ZplusJets"], 90., "response_asymmetric");
    TGraph* graph_Higgs130_response_asymmetric  = makeGraph(histograms_vs_metResolution["Higgs130"], 130., "response_asymmetric");
    TGraph* graph_Higgs300_response_asymmetric  = makeGraph(histograms_vs_metResolution["Higgs300"], 300., "response_asymmetric");
    showGraphs(800, 600,
	       graph_ZplusJets_response_asymmetric, "Sim. Z #rightarrow #tau#tau", 
	       graph_Higgs130_response_asymmetric, "Sim. H_{130} #rightarrow #tau#tau",
	       graph_Higgs300_response_asymmetric, "Sim. H_{300} #rightarrow #tau#tau",
	       NULL, "", 
	       NULL, "", 
	       NULL, "", 
	       colors_style1, markerStyles_style1,
	       0.04, 0.61, 0.74, 0.28, 0.15,
	       label_metResolution, 0.04, 0.175, 0.725, 0.24, 0.165, 
	       0., 25., "#sigma_{X/Y} / GeV", 1.2,
	       0.75, 1.25, "M_{#tau#tau}^{max}/M", 1.4,
	       Form("svFitPerformance_%s_PSkine_woLogM_Int_response_vs_metResolution_asymmetric.eps", channel->data()));
    TGraph* graph_ZplusJets_resolution = makeGraph(histograms_vs_metResolution["ZplusJets"], 90., "resolution");
    TGraph* graph_Higgs130_resolution  = makeGraph(histograms_vs_metResolution["Higgs130"], 130., "resolution");
    TGraph* graph_Higgs300_resolution  = makeGraph(histograms_vs_metResolution["Higgs300"], 300., "resolution");
    showGraphs(800, 600,
	       graph_ZplusJets_resolution, "Sim. Z #rightarrow #tau#tau", 
	       graph_Higgs130_resolution, "Sim. H_{130} #rightarrow #tau#tau",
	       graph_Higgs300_resolution, "Sim. H_{300} #rightarrow #tau#tau",
	       NULL, "", 
	       NULL, "", 
	       NULL, "", 
	       colors_style1, markerStyles_style1,
	       0.04, 0.61, 0.74, 0.28, 0.15,
	       label_metResolution, 0.04, 0.175, 0.725, 0.24, 0.165, 
	       0., 25., "#sigma_{X/Y} / GeV", 1.2,
	       0., 0.50, "#sigmaM_{#tau#tau}/<M_{#tau#tau}>", 1.4,
	       Form("svFitPerformance_%s_PSkine_woLogM_Int_resolution_vs_metResolution.eps", channel->data()));
    TGraph* graph_ZplusJets_resolution_asymmetric_left  = makeGraph(histograms_vs_metResolution["ZplusJets"], 90., "resolution_asymmetric_left");
    TGraph* graph_ZplusJets_resolution_asymmetric_right = makeGraph(histograms_vs_metResolution["ZplusJets"], 90., "resolution_asymmetric_right");
    TGraph* graph_Higgs130_resolution_asymmetric_left   = makeGraph(histograms_vs_metResolution["Higgs130"], 130., "resolution_asymmetric_left");
    TGraph* graph_Higgs130_resolution_asymmetric_right  = makeGraph(histograms_vs_metResolution["Higgs130"], 130., "resolution_asymmetric_right");
    TGraph* graph_Higgs300_resolution_asymmetric_left   = makeGraph(histograms_vs_metResolution["Higgs300"], 300., "resolution_asymmetric_left");
    TGraph* graph_Higgs300_resolution_asymmetric_right  = makeGraph(histograms_vs_metResolution["Higgs300"], 300., "resolution_asymmetric_right");
    showGraphs(800, 600,
	       graph_ZplusJets_resolution_asymmetric_right, "Sim. Z #rightarrow #tau#tau +",
	       graph_ZplusJets_resolution_asymmetric_left, "Sim. Z #rightarrow #tau#tau -",
	       graph_Higgs130_resolution_asymmetric_right, "Sim. H_{130} #rightarrow #tau#tau +",
	       graph_Higgs130_resolution_asymmetric_left, "Sim. H_{130} #rightarrow #tau#tau -",
	       graph_Higgs300_resolution_asymmetric_right, "Sim. H_{300} #rightarrow #tau#tau +",
	       graph_Higgs300_resolution_asymmetric_left, "Sim. H_{300} #rightarrow #tau#tau -",	       
	       colors_style2, markerStyles_style2,
	       0.04, 0.59, 0.64, 0.30, 0.25,
	       label_metResolution, 0.04, 0.175, 0.725, 0.24, 0.165, 
	       0., 25., "#sigma_{X/Y} / GeV", 1.2,
	       0., 0.60, "#sigmaM_{#tau#tau}/M_{#tau#tau}^{max}", 1.4,
	       Form("svFitPerformance_%s_PSkine_woLogM_Int_resolution_vs_metResolution_asymmetric.eps", channel->data()));
  }

  //-----------------------------------------------------------------------------
  // make plots of response and resolution 
  // obtained for different values of the true tau-pair mass
  //-----------------------------------------------------------------------------

  for ( std::vector<std::string>::const_iterator channel = channels.begin();
	channel != channels.end(); ++channel ) {
    std::vector<histogram_vs_X_Type> histograms_vs_mA;
    std::vector<double> y_true_array;
    for ( std::vector<double>::const_iterator massPoint = massPoints.begin();
	  massPoint != massPoints.end(); ++massPoint ) {
      std::string process = "";
      if ( (*massPoint) == 90. ) {
	process = "ZplusJets";
      } else {
	process = Form("Higgs%1.0f", *massPoint);
      }
      TH1* histogram = 
	getHistogram(inputFile, *channel, *massPoint, directory_PSkine_woLogM_Int, 
		     histogramName_svFitMass, metResolution_nominal);
      histograms_vs_mA.push_back(
	histogram_vs_X_Type(histogram, *massPoint, 0., 0.));
      y_true_array.push_back(*massPoint);
    }
    std::vector<std::string> label_mA;
    TGraph* graph_response = makeGraph(histograms_vs_mA, y_true_array, "response");
    showGraphs(800, 600,
	       graph_response, "Sim. Z/H #rightarrow #tau#tau",
	       NULL, "", 
	       NULL, "",
	       NULL, "", 
	       NULL, "", 
	       NULL, "", 
	       colors_style1, markerStyles_style1,
	       0.04, 0.61, 0.74, 0.28, 0.15,
	       label_mA, 0.04, 0.175, 0.725, 0.24, 0.165, 
	       70., 470., "M / GeV", 1.2,
	       0.75, 1.25, "<M_{#tau#tau}>/M", 1.4,
	       Form("svFitPerformance_%s_PSkine_woLogM_Int_response_vs_mA.eps", channel->data()));
    TGraph* graph_response_asymmetric = makeGraph(histograms_vs_mA, y_true_array, "response_asymmetric");
    showGraphs(800, 600,
	       graph_response_asymmetric, "Sim. Z/H #rightarrow #tau#tau", 
	       NULL, "", 
	       NULL, "", 
	       NULL, "", 
	       NULL, "", 
	       NULL, "", 
	       colors_style1, markerStyles_style1,
	       0.04, 0.61, 0.74, 0.28, 0.15,
	       label_mA, 0.04, 0.175, 0.725, 0.24, 0.165, 
	       70., 470., "M / GeV", 1.2,
	       0.75, 1.25, "M_{#tau#tau}^{max}/M", 1.4,
	       Form("svFitPerformance_%s_PSkine_woLogM_Int_response_vs_mA_asymmetric.eps", channel->data()));
    TGraph* graph_resolution = makeGraph(histograms_vs_mA, y_true_array, "resolution");
    showGraphs(800, 600,
	       graph_resolution, "Sim. Z/H #rightarrow #tau#tau", 
	       NULL, "", 
	       NULL, "",
	       NULL, "", 
	       NULL, "", 
	       NULL, "", 
	       colors_style1, markerStyles_style1,
	       0.04, 0.61, 0.74, 0.28, 0.15,
	       label_mA, 0.04, 0.175, 0.725, 0.24, 0.165, 
	       70., 470., "M / GeV", 1.2,
	       0., 0.50, "#sigmaM_{#tau#tau}/<M_{#tau#tau}>", 1.4,
	       Form("svFitPerformance_%s_PSkine_woLogM_Int_resolution_vs_mA.eps", channel->data()));
    TGraph* graph_resolution_asymmetric_left  = makeGraph(histograms_vs_mA, y_true_array, "resolution_asymmetric_left");
    TGraph* graph_resolution_asymmetric_right = makeGraph(histograms_vs_mA, y_true_array, "resolution_asymmetric_right");
    showGraphs(800, 600,
	       graph_resolution_asymmetric_right, "Sim. Z/H #rightarrow #tau#tau +",
	       graph_resolution_asymmetric_left, "Sim. Z/H #rightarrow #tau#tau -",
	       NULL, "",
	       NULL, "", 
	       NULL, "", 
	       NULL, "", 
	       colors_style2, markerStyles_style2,
	       0.04, 0.59, 0.74, 0.30, 0.15,
	       label_mA, 0.04, 0.175, 0.725, 0.24, 0.165, 
	       70., 470., "M / GeV", 1.2,
	       0., 0.50, "#sigmaM_{#tau#tau}/M_{#tau#tau}^{max}", 1.4,
	       Form("svFitPerformance_%s_PSkine_woLogM_Int_resolution_vs_mA_asymmetric.eps", channel->data()));
  }
      
  delete inputFile;
}
