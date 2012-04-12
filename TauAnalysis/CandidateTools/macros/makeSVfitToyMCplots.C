
#include <TFile.h>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TAxis.h>
#include <TGraphAsymmErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TMath.h>
#include <TROOT.h>
#include <TStyle.h>

#include <string>
#include <map>
#include <iostream>
#include <iomanip>

TH1* getHistogram(TFile* inputFile, const TString& dqmDirectory, const TString& meName)
{  
  //std::cout << "inputFile = " << inputFile->GetName() << std::endl;

  TString histogramName = TString(dqmDirectory);
  if ( !histogramName.EndsWith("/") ) histogramName.Append("/");
  histogramName.Append(meName);

  TH1* histogram = (TH1*)inputFile->Get(histogramName.Data());
  //std::cout << "histogramName = " << histogramName.Data() << ": histogram = " << histogram;
  //if ( histogram ) std::cout << ", integral = " << histogram->Integral();
  //std::cout << std::endl; 

  if ( histogram && !histogram->GetSumw2N() ) histogram->Sumw2();
  else if ( !histogram) 
    std::cerr << "Failed to load histogram = " << histogramName << " from file = " << inputFile->GetName() << " !!" << std::endl;

  //if ( histogram->Integral() > 0. ) histogram->Scale(histogram->GetEntries()/histogram->Integral());
  if ( histogram->Integral() > 0. ) histogram->Scale(1./histogram->Integral());

  return histogram;
}

void showHistograms1d(std::vector<TH1*>& histograms, double massPoint, const std::vector<std::string>& legendEntries, 
		      const TString& xAxisTitle, const TString& yAxisTitle,
		      const std::string& outputFileName)
{
  if ( histograms.size() == 0 ) return;
  assert(legendEntries.size() >= histograms.size());

  double yMin = +1.e+6;
  double yMax = -1.e+6;
  for ( std::vector<TH1*>::iterator histogram = histograms.begin();
	histogram != histograms.end(); ++histogram ) {
    int numBinsX = (*histogram)->GetNbinsX();
    for ( int iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
      double y = (*histogram)->GetBinContent(iBinX);
      if ( y < yMin ) yMin = y;
      if ( y > yMax ) yMax = y;
    }
  }
  yMin -= 0.06*(yMax - yMin);
  yMax += 0.06*(yMax - yMin);

  yMin = 0.;

  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);
  
  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);

  TH1* refHistogram = histograms.front();
  refHistogram->SetStats(false);
  refHistogram->SetTitle("");
  refHistogram->SetMinimum(yMin);
  refHistogram->SetMaximum(yMax);
  
  TAxis* xAxis = refHistogram->GetXaxis();
  xAxis->SetTitle(xAxisTitle.Data());
  xAxis->SetTitleOffset(1.15);

  TAxis* yAxis = refHistogram->GetYaxis();
  yAxis->SetTitle(yAxisTitle.Data());
  yAxis->SetTitleOffset(1.30);

  int colors[] = { 1, 4, 12 };
  int lineStyles[] = { 1, 7, 4 };
  int numHistograms = histograms.size();
  if ( numHistograms > 3 ) {
    std::cerr << "<showHistograms1d>:" << std::endl;
    std::cerr << "Number of histograms must not exceed 3 !!" << std::endl;
    assert(0);
  }

  double legendX0, legendY0;
  if ( xAxisTitle.CompareTo("X_{1}") == 0 ) {
    legendX0 = 0.64;
    legendY0 = 0.87 - 0.06*histograms.size();
  } else if ( xAxisTitle.CompareTo("X_{2}") == 0 ) {
    legendX0 = 0.64;
    legendY0 = 0.19;
  } else {
    std::cerr << "Undefined x-axis title = " << xAxisTitle.Data() << " !!" << std::endl;
    assert(0);
  }
  TLegend* legend = new TLegend(legendX0, legendY0, legendX0 + 0.25, legendY0 + 0.06*histograms.size(), "", "brNDC"); 
  legend->SetBorderSize(0);
  legend->SetFillColor(0);

  for ( int iHistogram = 0; iHistogram < numHistograms; ++iHistogram ) {
    TH1* histogram = histograms[iHistogram];
    histogram->SetLineColor(colors[iHistogram]);
    histogram->SetLineStyle(lineStyles[iHistogram]);
    histogram->SetLineWidth(2);

    std::string drawOption = "hist";
    if ( iHistogram > 0 ) drawOption.append("same");
    histogram->Draw(drawOption.data());

    std::string legendEntry = legendEntries[iHistogram];
    legend->AddEntry(histogram, legendEntry.data(), "l");
  }

  legend->Draw();

  double labelX0, labelY0;
  if ( xAxisTitle.CompareTo("X_{1}") == 0 ) {
    labelX0 = 0.16;
    labelY0 = 0.14;
  } else if ( xAxisTitle.CompareTo("X_{2}") == 0 ) {
    labelX0 = 0.16;
    labelY0 = 0.69;
  } else {
    std::cerr << "Undefined x-axis title = " << xAxisTitle.Data() << " !!" << std::endl;
    assert(0);
  }
  TPaveText* label = new TPaveText(labelX0, labelY0, labelX0 + 0.28, labelY0 + 0.17, "brNDC");
  if ( massPoint < 95. ) label->AddText("Z #rightarrow #tau#tau MC");
  else label->AddText("Higgs #rightarrow #tau#tau MC");
  label->AddText(Form("M = %1.0f GeV", massPoint));
  label->SetFillColor(10);
  label->SetBorderSize(0);
  label->SetTextColor(1);
  label->SetTextAlign(11);
  label->SetTextSize(0.04);
  label->Draw();

  canvas->Update();
  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_plot).append(std::string(outputFileName, idx)).data());
  canvas->Print(std::string(outputFileName_plot).append(".png").data());
  canvas->Print(std::string(outputFileName_plot).append(".pdf").data());
  canvas->Print(std::string(outputFileName_plot).append(".root").data());
  
  delete legend;
  delete label;
  delete canvas;
}

void makeSVfitToyMCplots()
{
  //std::string inputFileName = Form("../test/studySVfitVisPtCuts_2012Mar04.root");
  std::string inputFileName = Form("../test/studySVfitVisPtCuts_2012Apr10.root");
  TFile* inputFile = new TFile(inputFileName.data());

  gROOT->SetBatch(true);

  std::vector<std::string> directories;
  directories.push_back(std::string("plotsBeforeVisEtaAndPtCuts"));
  directories.push_back(std::string("plotsAfterVisEtaAndPtCutsLeg1"));
  directories.push_back(std::string("plotsAfterVisEtaAndPtCutsLeg2"));
  directories.push_back(std::string("plotsAfterVisEtaAndPtCutsLeg1and2"));
  
  std::map<std::string, std::string> selectionLabels;
  selectionLabels["plotsBeforeVisEtaAndPtCuts"]        = "beforeVisPtCuts";
  selectionLabels["plotsAfterVisEtaAndPtCutsLeg1"]     = "afterVisPtCutsLeg1";
  selectionLabels["plotsAfterVisEtaAndPtCutsLeg2"]     = "afterVisPtCutsLeg2";
  selectionLabels["plotsAfterVisEtaAndPtCutsLeg1and2"] = "afterVisPtCutsLeg1and2";
 
  std::vector<std::string> histogramNames;
  histogramNames.push_back(std::string("histogramLeg1X"));
  //histogramNames.push_back(std::string("histogramLeg1VisMass"));
  //histogramNames.push_back(std::string("histogramLeg1NuMass"));
  histogramNames.push_back(std::string("histogramLeg2X"));
  //histogramNames.push_back(std::string("histogramLeg2VisMass"));
  //histogramNames.push_back(std::string("histogramLeg2NuMass"));
  //histogramNames.push_back(std::string("histogramSVfitMassFromGenInput"));

  std::map<std::string, std::string> plotLabels;
  plotLabels["histogramLeg1X"]                  = "X1";
  plotLabels["histogramLeg1VisMass"]            = "visMass1";
  plotLabels["histogramLeg1NuMass"]             = "nuMass1";
  plotLabels["histogramLeg2X"]                  = "X2";
  plotLabels["histogramLeg2VisMass"]            = "visMass2";
  plotLabels["histogramLeg2NuMass"]             = "nuMass2";
  plotLabels["histogramSVfitMassFromGenInput"]  = "svFitMass";

  std::map<std::string, std::string> xAxisTitles;
  xAxisTitles["histogramLeg1X"]                 = "X_{1}";
  xAxisTitles["histogramLeg1VisMass"]           = "M_{vis}^{1}";
  xAxisTitles["histogramLeg1NuMass"]            = "M_{#nu#nu}^{1}";
  xAxisTitles["histogramLeg2X"]                 = "X_{2}";
  xAxisTitles["histogramLeg2VisMass"]           = "M_{vis}^{2}";
  xAxisTitles["histogramLeg2NuMass"]            = "M_{#nu#nu}^{2}";
  xAxisTitles["histogramSVfitMassFromGenInput"] = "M_{#tau#tau} (SVfit) / GeV";
  
  std::vector<double> massPoints;
  massPoints.push_back(90.);
/*
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
 */
  std::vector<std::string> legendEntries;
  legendEntries.push_back(std::string("TAUOLA"));
  legendEntries.push_back(std::string("PS model"));
  legendEntries.push_back(std::string("ME model"));

  for ( std::vector<double>::const_iterator massPoint = massPoints.begin();
	massPoint != massPoints.end(); ++massPoint ) {

    std::cout << "processing mass-point = " << (*massPoint) << " GeV:" << std::endl;

    for ( std::vector<std::string>::const_iterator directory = directories.begin();
	  directory != directories.end(); ++directory ) {
      for ( std::vector<std::string>::const_iterator histogramName = histogramNames.begin();
	    histogramName != histogramNames.end(); ++histogramName ) {
	TString directory_full = directory->data();
	if ( !directory_full.EndsWith("/") ) directory_full.Append("/");
	directory_full.Append(Form("massEq%1.0fGeV", *massPoint));
	TH1* histogram = getHistogram(inputFile, directory_full, histogramName->data());
	TString directory_toyMCps_full = std::string(*directory).append("_toyMCps").data();
	if ( !directory_toyMCps_full.EndsWith("/") ) directory_toyMCps_full.Append("/");
	directory_toyMCps_full.Append(Form("massEq%1.0fGeV", *massPoint));
	TH1* histogram_toyMCps = getHistogram(inputFile, directory_toyMCps_full, histogramName->data());
	TString directory_toyMCme_full = std::string(*directory).append("_toyMCme").data();
	if ( !directory_toyMCme_full.EndsWith("/") ) directory_toyMCme_full.Append("/");
	directory_toyMCme_full.Append(Form("massEq%1.0fGeV", *massPoint));
	TH1* histogram_toyMCme = getHistogram(inputFile, directory_toyMCme_full, histogramName->data());
	std::vector<TH1*> histograms;
	histograms.push_back(histogram);
	histograms.push_back(histogram_toyMCps);
	if ( (*histogramName) != "histogramLeg2X" ) histograms.push_back(histogram_toyMCme);
	std::string outputFileName = 
	  Form("plots/makeSVfitToyMCplots_%s_m%1.0f_%s.eps", 
	       plotLabels[*histogramName].data(), *massPoint, selectionLabels[*directory].data());
	showHistograms1d(histograms, *massPoint, legendEntries, xAxisTitles[*histogramName].data(), "a.u", outputFileName);
      }
    }
  }

  delete inputFile;
}


