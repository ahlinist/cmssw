#ifndef TauAnalysis_TauIdEfficiency_fitSysMEtShiftAuxFunctions_h
#define TauAnalysis_TauIdEfficiency_fitSysMEtShiftAuxFunctions_h

#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraphAsymmErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>

#include <vector>
#include <string>
#include <assert.h>

TF1* fitMETxyVsSumEt(const TH2* histogram2d, double xMin, double xMax, double xSeparationMin,
		     const TString& fitVariable, const TString& xAxisTitle, const TString& yAxisTitle, 
		     const std::string& outputFileName)
{
  std::vector<double> graph_xs;
  std::vector<double> graph_xErrsUp;
  std::vector<double> graph_xErrsDown;
  std::vector<double> graph_ys;
  std::vector<double> graph_yErrsUp;
  std::vector<double> graph_yErrsDown;

  int numBins2dX = histogram2d->GetNbinsX();
  TH1* histogram1dSum = 0;
  double xSum = 0.;
  double xMin_bin = 0.;
  double xMax_bin = 0.;
  double integralSum = 0.;
  for ( int iBin2dX = 1; iBin2dX <= numBins2dX; ++iBin2dX ) {
    TAxis* xAxis = histogram2d->GetXaxis();
    double x = xAxis->GetBinCenter(iBin2dX);
    double xErr = 0.5*xAxis->GetBinWidth(iBin2dX);
    //std::cout << "x = " << x << " +/- " << xErr << std::endl;
    
    if ( !(x > xMin && x < xMax) ) continue;

    std::string histogram1dName = std::string(histogram2d->GetName()).append(Form("BinX%i", iBin2dX));
    TH1* histogram1d = histogram2d->ProjectionY(histogram1dName.data(), iBin2dX, iBin2dX, "e");
    //std::cout << "histogram1d: integral = " << histogram1d->Integral() 
    //	        << " (entries = " << histogram1d->GetEntries() << ")" << std::endl;

    if ( integralSum > 1.e+3 && (xMax_bin - xMin_bin) >= xSeparationMin ) {
      double graph_x = xSum/integralSum;
      graph_xs.push_back(graph_x);
      double graph_xErrUp = xMax_bin - graph_x;
      graph_xErrsUp.push_back(graph_xErrUp);
      double graph_xErrDown = graph_x - xMin_bin;
      graph_xErrsDown.push_back(graph_xErrDown);

      double graph_y = histogram1d->GetMean();
      graph_ys.push_back(graph_y);
      double graph_yErr = histogram1d->GetMeanError();
      graph_yErrsUp.push_back(graph_yErr);
      graph_yErrsDown.push_back(graph_yErr);

      delete histogram1dSum;
      histogram1dSum = 0;
    }

    if ( !histogram1dSum ) {
      histogram1dSum = histogram1d;
      xSum = 0.;
      xMin_bin = x - xErr;
      integralSum = 0.;
    } else {
      histogram1dSum->Add(histogram1d);
    }

    double integral = histogram1d->Integral();
    xSum += (x*integral);
    xMax_bin = x + xErr;
    integralSum += integral;

    if ( histogram1dSum != histogram1d ) delete histogram1d;
  }

  delete histogram1dSum;

  size_t numPoints = graph_xs.size();
  std::cout << "numPoints = " << numPoints << std::endl;
  assert(graph_xErrsUp.size()   == numPoints);
  assert(graph_xErrsDown.size() == numPoints);
  assert(graph_ys.size()        == numPoints);
  assert(graph_yErrsUp.size()   == numPoints);
  assert(graph_yErrsDown.size() == numPoints);
  TGraphAsymmErrors* graph = new TGraphAsymmErrors(numPoints);
  for ( size_t iPoint = 0; iPoint < numPoints; ++iPoint ) {
    graph->SetPoint(
      iPoint, 
      graph_xs[iPoint], 
      graph_ys[iPoint]);
    graph->SetPointError(
      iPoint, 
      graph_xErrsDown[iPoint], graph_xErrsUp[iPoint], 
      graph_yErrsDown[iPoint], graph_yErrsUp[iPoint]);
  }

  double xMin_fit = histogram2d->GetXaxis()->GetXmin();
  double xMax_fit = histogram2d->GetXaxis()->GetXmax();
  std::string fitFunctionName = Form("%s_fit", histogram2d->GetName());
  TF1* fitFunction = new TF1(fitFunctionName.data(), "[0] + [1]*x", xMin_fit, xMax_fit);
  graph->Fit(fitFunction, "0");

  double yMin = +1.e+6;
  double yMax = -1.e+6;
  for ( size_t iPoint = 0; iPoint < numPoints; ++iPoint ) {
    double x, y;
    graph->GetPoint(iPoint, x, y);
    if ( y < yMin ) yMin = y;
    if ( y > yMax ) yMax = y;
  }
  yMin -= 0.06*(yMax - yMin);
  yMax += 0.06*(yMax - yMin);

  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);
  
  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);

  TH1* dummyHistogram = new TH1F("dummyHistogram", "dummyHistogram", 10, xMin_fit, xMax_fit);
  dummyHistogram->SetStats(false);
  dummyHistogram->SetTitle("");
  dummyHistogram->SetMinimum(yMin);
  dummyHistogram->SetMaximum(yMax);
  
  TAxis* xAxis = dummyHistogram->GetXaxis();
  xAxis->SetTitle(xAxisTitle.Data());
  xAxis->SetTitleOffset(1.15);

  TAxis* yAxis = dummyHistogram->GetYaxis();
  yAxis->SetTitle(yAxisTitle.Data());
  yAxis->SetTitleOffset(1.30);

  dummyHistogram->Draw("axis");

  fitFunction->SetLineColor(2);
  fitFunction->SetLineWidth(2);
  fitFunction->Draw("Lsame");

  graph->SetMarkerColor(1);
  graph->SetMarkerStyle(20);
  graph->SetLineColor(1);
  graph->SetLineWidth(2);
  graph->Draw("P");

  double offset = fitFunction->GetParameter(0);
  double slope  = fitFunction->GetParameter(1);  
  double legendX0, legendY0;
  if ( slope > 0. ) {
    legendX0 = 0.54;
    legendY0 = 0.22;
  } else {
    legendX0 = 0.54;
    legendY0 = 0.68;
  }
  TPaveText* fitFunctionLabel = new TPaveText(legendX0, legendY0, legendX0 + 0.31, legendY0 + 0.17, "brNDC");
  TString ordinate = yAxisTitle;
  ordinate = ordinate.ReplaceAll(" / GeV", "");
  fitFunctionLabel->AddText(Form("%s = c_{0} + c_{1}*%s", ordinate.Data(), fitVariable.Data()));
  fitFunctionLabel->AddText(Form("c_{0} = %1.3e", offset));
  fitFunctionLabel->AddText(Form("c_{1} = %1.3e", slope));
  fitFunctionLabel->SetFillColor(10);
  fitFunctionLabel->SetBorderSize(0);
  fitFunctionLabel->SetTextColor(2);
  fitFunctionLabel->SetTextAlign(11);
  fitFunctionLabel->SetTextSize(0.04);
  fitFunctionLabel->Draw();

  canvas->Update();
  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  outputFileName_plot.append("_fit");
  if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_plot).append(std::string(outputFileName, idx)).data());
  canvas->Print(std::string(outputFileName_plot).append(".png").data());
  canvas->Print(std::string(outputFileName_plot).append(".pdf").data());
  
  delete dummyHistogram;
  delete canvas;
  delete graph;

  return fitFunction;
}

#endif
