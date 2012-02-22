
#include <TFile.h>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TMath.h>

#include <vector>
#include <iostream>
#include <iomanip>

TH1* getHistogram(TFile* inputFile, const TString& dqmDirectory, const TString& meName)
{  
  TString histogramName = dqmDirectory;
  if ( histogramName.Length() > 0 && !histogramName.EndsWith("/") ) histogramName.Append("/");
  histogramName.Append(meName);
  std::cout << "histogramName = " << histogramName.Data() << std::endl; 

  TH1* histogram = (TH1*)inputFile->Get(histogramName.Data());
  std::cout << "histogram = " << histogram << std::endl; 

  return histogram;
}

void makeNeuralMtautauPerformancePlots()
{
  std::string inputFileName = "../test/testNeuralMtautau.root";
  TFile* inputFile = TFile::Open(inputFileName.data());

  std::string histogramName = "histogramRecVsGenMass";
  TH2* histogram2d = dynamic_cast<TH2*>(getHistogram(inputFile, "", histogramName.data()));  

  const int numGenMassBins = 17;
  double genMassBins[] = { 0., 50., 60., 70., 80., 90., 100., 110., 120., 130., 140., 160., 200., 250., 300., 350., 400., 500. };
  int genMassBin_index = 0;

  TH1* histogram1dTruncatedSum = 0;

  TGraphErrors* graph_mean = new TGraphErrors(numGenMassBins);
  TGraphErrors* graph_rms  = new TGraphErrors(numGenMassBins);

  int numBins2dX = histogram2d->GetNbinsX();
  for ( int iBin2dX = 1; iBin2dX <= numBins2dX; ++iBin2dX ) {
    std::string histogram1dName = std::string(histogram2d->GetName()).append(Form("BinX%i", iBin2dX));
    TH1* histogram1d = histogram2d->ProjectionX(histogram1dName.data(), iBin2dX, iBin2dX);
    
    std::string histogram1dTruncatedName = std::string(histogram2d->GetName()).append(Form("BinX%i_truncated", iBin2dX));
    TH1* histogram1dTruncated = new TH1D(histogram1dTruncatedName.data(), histogram1dTruncatedName.data(), 200, 0., 2.);
    
    double mTauTau_gen = histogram2d->GetXaxis()->GetBinCenter(iBin2dX);
    //if ( mTauTau_gen > 100. ) continue;
    //std::cout << "iBin2dX = " << iBin2dX << ": mTauTau(gen) = " << mTauTau_gen << std::endl;
    //std::cout << "histogram1d: mean = " << histogram1d->GetMean() << "," 
    //	        << " rms = " << histogram1d->GetRMS() << std::endl;

    int numBins1d = histogram1d->GetNbinsX();
    for ( int iBin1d = 1; iBin1d <= numBins1d; ++iBin1d ) {
      double mTauTau_rec = histogram1d->GetBinCenter(iBin1d);
      //std::cout << "iBin1d = " << iBin1d << ": mTauTau(rec) = " << mTauTau_rec << std::endl;
      double ratio = mTauTau_rec/mTauTau_gen;
      //std::cout << "ratio = " << ratio << std::endl;
      double binContent = histogram1d->GetBinContent(iBin1d);
      if ( binContent > 1.e-2 ) histogram1dTruncated->Fill(ratio, binContent);
    }
    //std::cout << "histogram1dTruncated: mean = " << histogram1dTruncated->GetMean() << "," 
    //	        << " rms = " << histogram1dTruncated->GetRMS() << std::endl;

    delete histogram1d;

    if ( mTauTau_gen > genMassBins[genMassBin_index + 1] && mTauTau_gen < genMassBins[numGenMassBins - 1] ) {
      double x = 0.5*(genMassBins[genMassBin_index] + genMassBins[genMassBin_index + 1]);
      double xErr = 0.5*(genMassBins[genMassBin_index + 1] - genMassBins[genMassBin_index]);
      double y_mean = histogram1dTruncatedSum->GetMean();
      double yErr_mean = histogram1dTruncatedSum->GetMeanError();
      double y_rms = histogram1dTruncatedSum->GetRMS();
      double yErr_rms = histogram1dTruncatedSum->GetRMSError();
      //std::cout << "mTauTau(gen) = " << x << ": mean = " << y_mean << " +/- " << yErr_mean << "," 
      //	  << " rms = " << y_rms << " +/- " << yErr_rms << std::endl;
      graph_mean->SetPoint(genMassBin_index, x, y_mean);
      graph_mean->SetPointError(genMassBin_index, xErr, yErr_mean);
      graph_rms->SetPoint(genMassBin_index, x, y_rms);
      graph_rms->SetPointError(genMassBin_index, xErr, yErr_rms);
      delete histogram1dTruncatedSum;
      histogram1dTruncatedSum = 0;
      ++genMassBin_index;
    } 

    if ( !histogram1dTruncatedSum ) {
      histogram1dTruncatedSum = histogram1dTruncated;
    } else {
      histogram1dTruncatedSum->Add(histogram1dTruncated);
      delete histogram1dTruncated;
    }
  }

  delete histogram1dTruncatedSum;

  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);
  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);
  canvas->SetLogy();

  TH1* dummyHistogram = new TH1F("dummyHistogram", "dummyHistogram", 50, 0., 500.);
  dummyHistogram->SetStats(false);
  dummyHistogram->SetTitle("");
  dummyHistogram->SetMinimum(1.e-2);
  dummyHistogram->SetMaximum(1.e+1);

  TAxis* xAxis = dummyHistogram->GetXaxis();
  xAxis->SetTitle("M_{#tau#tau}^{gen} / GeV");
  xAxis->SetTitleOffset(1.15);

  TAxis* yAxis = dummyHistogram->GetYaxis();
  yAxis->SetTitle("M_{#tau#tau}^{rec} / GeV");
  yAxis->SetTitleOffset(1.30);

  dummyHistogram->Draw("axis");

  graph_mean->SetMarkerStyle(20);
  graph_mean->SetMarkerColor(1);
  graph_mean->Draw("P");

  graph_rms->SetMarkerStyle(20);
  graph_rms->SetMarkerColor(2);
  graph_rms->Draw("P");
  
  TLegend* legend = new TLegend(0.64, 0.69, 0.88, 0.87, "", "brNDC"); 
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  legend->AddEntry(graph_mean, "<M_{#tau#tau}^{rec}/M_{#tau#tau}^{gen}>", "p");
  legend->AddEntry(graph_rms, "#sigma(M_{#tau#tau}^{rec}/M_{#tau#tau}^{gen})", "p");
  legend->Draw();

  canvas->Update();
  std::string outputFileName = "neuralMtautauPerformancePlot.eps";
  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_plot).append(std::string(outputFileName, idx)).data());
  canvas->Print(std::string(outputFileName_plot).append(".png").data());
  canvas->Print(std::string(outputFileName_plot).append(".pdf").data());
  
  delete graph_mean;
  delete graph_rms;
  delete dummyHistogram;
  delete legend;
  delete canvas;
}
