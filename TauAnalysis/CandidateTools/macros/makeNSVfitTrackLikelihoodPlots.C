
#include <TFile.h>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraphAsymmErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include <TMath.h>

#include <vector>
#include <iostream>
#include <iomanip>

TH1* getHistogram(TFile* inputFile, const std::string& directory, const std::string& histogramName)
{  
  TString histogramName_full = Form("DQMData/%s", directory.data());
  if ( !histogramName_full.EndsWith("/") ) histogramName_full.Append("/");
  histogramName_full.Append(histogramName.data());

  TH1* histogram = (TH1*)inputFile->Get(histogramName_full.Data());

  if ( histogram && !histogram->GetSumw2N() ) histogram->Sumw2();
  else if ( !histogram) 
    std::cerr << "Failed to load histogram = " << histogramName_full << " from file = " << inputFile->GetName() << " !!" << std::endl;

  return histogram;
}

TH1* getHistogram(TFile* inputFile, const std::vector<std::string>& directories, size_t idx, const std::string& histogramName)
{
  if ( idx < directories.size() ) return getHistogram(inputFile, directories.at(idx), histogramName);
  else return 0;
}

std::string getLegendEntry(const std::vector<std::string>& legendEntries, size_t idx)
{
  if ( idx < legendEntries.size() ) return legendEntries.at(idx);
  else return "";
}

void normalizeHistogram(TH1* histogram)
{
  if ( !histogram->GetSumw2N() ) histogram->Sumw2();
  if ( histogram->Integral() > 0. ) histogram->Scale(1./histogram->Integral());
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
      if ( (xMin == -1. || binCenter >= xMin) && 
	   (xMax == -1. || binCenter <= xMax) ) ++numBins_withinRange;
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

void drawHistogram(TH1* histogram, size_t idx, const std::string& drawOption)
{
  assert(idx >= 0 && idx < 6);

  int colors[6] = { 1, 2, 3, 4, 6, 7 };
  int lineStyles[6] = { 1, 7, 4, 1, 7, 4 };
  int markerStyles[6] = { 20, 21, 34, 24, 25, 28 };

  histogram->SetLineColor(colors[idx]);
  histogram->SetLineWidth(2);
  if ( drawOption == "hist" ) histogram->SetLineStyle(lineStyles[idx]);
  histogram->SetMarkerColor(colors[idx]);
  histogram->SetMarkerStyle(markerStyles[idx]);
  if ( idx == 0 ) histogram->Draw(drawOption.data());
  else histogram->Draw(std::string(drawOption).append("same").data());
}

void showHistograms(double canvasSizeX, double canvasSizeY,
		    TH1* histogram1, const std::string& legendEntry1,
		    TH1* histogram2, const std::string& legendEntry2,
		    TH1* histogram3, const std::string& legendEntry3,
		    TH1* histogram4, const std::string& legendEntry4,
		    TH1* histogram5, const std::string& legendEntry5,
		    TH1* histogram6, const std::string& legendEntry6,
		    double legendTextSize, double legendPosX, double legendPosY, double legendSizeX, double legendSizeY, 
		    std::vector<std::string>& labelTextLines, double labelTextSize,
		    double labelPosX, double labelPosY, double labelSizeX, double labelSizeY,
		    double xMin, double xMax, const std::string& xAxisTitle, double xAxisOffset,
		    bool useLogScale, double yMin, double yMax, const std::string& yAxisTitle, double yAxisOffset,
		    const std::string& outputFileName, const std::string& drawOption)
{
  unsigned numBinsMin_rebinned = 25;
  TH1* histogram1_rebinned = rebinHistogram(histogram1, numBinsMin_rebinned, xMin, xMax);
  TH1* histogram2_rebinned = rebinHistogram(histogram2, numBinsMin_rebinned, xMin, xMax);
  TH1* histogram3_rebinned = rebinHistogram(histogram3, numBinsMin_rebinned, xMin, xMax);
  TH1* histogram4_rebinned = rebinHistogram(histogram4, numBinsMin_rebinned, xMin, xMax);
  TH1* histogram5_rebinned = rebinHistogram(histogram5, numBinsMin_rebinned, xMin, xMax);
  TH1* histogram6_rebinned = rebinHistogram(histogram6, numBinsMin_rebinned, xMin, xMax);

  TCanvas* canvas = new TCanvas("canvas", "canvas", canvasSizeX, canvasSizeY);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  canvas->SetLeftMargin(0.14);
  canvas->SetBottomMargin(0.12);

  canvas->SetLogy(useLogScale);

  normalizeHistogram(histogram1_rebinned);
  histogram1_rebinned->SetTitle("");
  histogram1_rebinned->SetStats(false);
  histogram1_rebinned->SetMinimum(yMin);
  histogram1_rebinned->SetMaximum(yMax);

  TAxis* xAxis = histogram1_rebinned->GetXaxis();
  xAxis->SetTitle(xAxisTitle.data());
  xAxis->SetTitleOffset(xAxisOffset);
  if ( xMax > xMin ) {
    std::cout << "limiting x-axis range to " << xMin << ".." << xMax << std::endl;
    xAxis->SetRangeUser(xMin, xMax);
  }

  TAxis* yAxis = histogram1_rebinned->GetYaxis();
  yAxis->SetTitle(yAxisTitle.data());
  yAxis->SetTitleOffset(yAxisOffset);

  drawHistogram(histogram1_rebinned, 0, drawOption);

  if ( histogram2_rebinned ) {
    normalizeHistogram(histogram2_rebinned);
    drawHistogram(histogram2_rebinned, 1, drawOption);
  }

  if ( histogram3_rebinned ) {
    normalizeHistogram(histogram3_rebinned);
    drawHistogram(histogram3_rebinned, 2, drawOption);
  }

  if ( histogram4_rebinned ) {
    normalizeHistogram(histogram4_rebinned);
    drawHistogram(histogram4_rebinned, 3, drawOption);
  }

  if ( histogram5_rebinned ) {
    normalizeHistogram(histogram5_rebinned);
    drawHistogram(histogram5_rebinned, 4, drawOption);
  }

  if ( histogram6_rebinned ) {
    normalizeHistogram(histogram6_rebinned);
    drawHistogram(histogram6_rebinned, 5, drawOption);
  }

  TLegend* legend = new TLegend(legendPosX, legendPosY, legendPosX + legendSizeX, legendPosY + legendSizeY, "", "brNDC"); 
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  //legend->SetTextSize(legendTextSize);
  std::string legendEntry_symbol;
  if ( drawOption == "hist" ) legendEntry_symbol = "l";
  else if ( drawOption == "e1p" ) legendEntry_symbol = "p";
  else assert(0);
  legend->AddEntry(histogram1_rebinned, legendEntry1.data(), "l");
  if ( histogram2_rebinned ) legend->AddEntry(histogram2_rebinned, legendEntry2.data(), legendEntry_symbol.data());
  if ( histogram3_rebinned ) legend->AddEntry(histogram3_rebinned, legendEntry3.data(), legendEntry_symbol.data());
  if ( histogram4_rebinned ) legend->AddEntry(histogram4_rebinned, legendEntry4.data(), legendEntry_symbol.data());
  if ( histogram5_rebinned ) legend->AddEntry(histogram5_rebinned, legendEntry5.data(), legendEntry_symbol.data());
  if ( histogram6_rebinned ) legend->AddEntry(histogram6_rebinned, legendEntry6.data(), legendEntry_symbol.data());
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
  delete histogram5_rebinned;
  delete histogram6_rebinned;
}

void makePlots(TFile* inputFile,
	       const std::vector<std::string>& dqmDirectories, 
	       const std::vector<std::string>& legendEntries,
	       const std::vector<std::string>& meNames,
	       const std::vector<std::string>& xAxisLabels)
{
  assert(dqmDirectories.size() == legendEntries.size());
  assert(meNames.size() == xAxisLabels.size());

  for ( size_t idx = 0; idx < meNames.size(); ++idx ) {
    const std::string& meName = meNames.at(idx);
    const std::string& xAxisLabel = xAxisLabels.at(idx);
    
    TH1* histogram1 = getHistogram(inputFile, dqmDirectories, 0, meName);
    std::string legendEntry1 = getLegendEntry(legendEntries, 0);
    TH1* histogram2 = getHistogram(inputFile, dqmDirectories, 1, meName);
    std::string legendEntry2 = getLegendEntry(legendEntries, 1);
    TH1* histogram3 = getHistogram(inputFile, dqmDirectories, 2, meName);
    std::string legendEntry3 = getLegendEntry(legendEntries, 2);
    TH1* histogram4 = getHistogram(inputFile, dqmDirectories, 3, meName);
    std::string legendEntry4 = getLegendEntry(legendEntries, 3);
    TH1* histogram5 = getHistogram(inputFile, dqmDirectories, 4, meName);
    std::string legendEntry5 = getLegendEntry(legendEntries, 4);
    TH1* histogram6 = getHistogram(inputFile, dqmDirectories, 5, meName);
    std::string legendEntry6 = getLegendEntry(legendEntries, 5);
    
    std::vector<std::string> labelTextLines_empty;

    TString outputFileName = Form("makeNSVfitTrackLikelihoodPlot_%s.eps", meName.data());
    outputFileName = outputFileName.ReplaceAll("/", "_");

    showHistograms(800, 600,
		   histogram1, legendEntry1,
		   histogram2, legendEntry2,
		   histogram3, legendEntry3,
		   histogram4, legendEntry4,
		   histogram5, legendEntry5,
		   histogram6, legendEntry6,
		   0.040, 0.55, 0.74, 0.34, 0.15,
		   labelTextLines_empty, 0.055, 
		   0.74, 0.16, 0.15, 0.06,
		   -1., -1., xAxisLabel, 1.3,
		   true, 1.e-4, 1.e0, "a.u.", 1.2,
		   outputFileName.Data(), "e1p");
  }
}

void makeNSVfitTrackLikelihoodPlots()
{
  TString inputFileName = "/data1/veelken/tmp/svFitStudies/testNSVfitTrackLikelihoods2_Higgs_2012Sep04_genVisP4_genEvtVertex.root";
  TFile* inputFile = TFile::Open(inputFileName.Data());
  if ( !inputFile ) {
    std::cerr << "Failed to open input file = " << inputFileName.Data() << " !!" << std::endl;
    assert(0);
  }

  std::vector<std::string> dqmDirectories1;
  //dqmDirectories1.push_back("nSVfitProducerByIntegrationWtracksFor1prongsAnd3prongs");
  dqmDirectories1.push_back("nSVfitProducerByIntegrationWtracksFor3prongsOnly");
  dqmDirectories1.push_back("nSVfitProducerByIntegrationWOtracks");
  
  std::vector<std::string> legendEntries1;
  legendEntries1.push_back("w. Tracking info.");
  legendEntries1.push_back("wo. Tracking info.");

  std::vector<std::string> meNames1;
  meNames1.push_back("svFitMass");
  
  std::vector<std::string> xAxisLabels1;
  xAxisLabels1.push_back("M_{#tau#tau}");

  makePlots(inputFile, dqmDirectories1, legendEntries1, meNames1, xAxisLabels1);

  std::vector<std::string> dqmDirectories2;
  dqmDirectories2.push_back("nSVfitProducerByIntegrationWtracksFor1prongsAnd3prongsbetterThannSVfitProducerByIntegrationWOtracks");
  //dqmDirectories2.push_back("nSVfitProducerByIntegrationWOtracksbetterThannSVfitProducerByIntegrationWtracksFor1prongsAnd3prongs");
  dqmDirectories2.push_back("nSVfitProducerByIntegrationWOtracksbetterThannSVfitProducerByIntegrationWtracksFor3prongsOnly");

  std::vector<std::string> legendEntries2;
  legendEntries2.push_back("M_{#tau#tau} w. Tracking info. better");
  legendEntries2.push_back("M_{#tau#tau} wo. Tracking info. better");

  std::vector<std::string> meNames2;
  meNames2.push_back("leg1/genTau_gjAngle");
  meNames2.push_back("leg1/genTau_phi_lab");
  meNames2.push_back("leg1/genTauVisEnFrac");
  meNames2.push_back("leg1/genTauDecayDistance");
  meNames2.push_back("leg2/genTau_gjAngle");
  meNames2.push_back("leg2/genTau_phi_lab");
  meNames2.push_back("leg2/genTauVisEnFrac");
  meNames2.push_back("leg2/genTauDecayDistance");
  meNames2.push_back("leg2/genTauVisMass");
  meNames2.push_back("leg2/genTauDecayMode");
  meNames2.push_back("leg2/recTauDecayMode");
  meNames2.push_back("leg2/genLeadTrackPt");
  meNames2.push_back("leg2/recTauHadDeltaPt_absolute");
  meNames2.push_back("leg2/recTauHadDeltaPt_relative");

  std::vector<std::string> xAxisLabels2;
  xAxisLabels2.push_back("#theta_{GJ}^{1}");
  xAxisLabels2.push_back("#phi_{lab}^{1}");
  xAxisLabels2.push_back("X_{1}");
  xAxisLabels2.push_back("d_{1} / cm");
  xAxisLabels2.push_back("#theta_{GJ}^{2}");
  xAxisLabels2.push_back("#phi_{lab}^{2}");
  xAxisLabels2.push_back("X_{2}");
  xAxisLabels2.push_back("d_{2} / cm");
  xAxisLabels2.push_back("M_{vis}^{2} / GeV");
  xAxisLabels2.push_back("");
  xAxisLabels2.push_back("");
  xAxisLabels2.push_back("lead. Track_{2} P_{T} / GeV");
  xAxisLabels2.push_back("Error on P_{T}^{vis2} / GeV");
  xAxisLabels2.push_back("rel. Error on P_{T}^{vis2}");

  makePlots(inputFile, dqmDirectories2, legendEntries2, meNames2, xAxisLabels2);

  delete inputFile;
}
