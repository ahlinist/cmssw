
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/FWLite/interface/InputSource.h"

#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"

#include <TFile.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TBenchmark.h>
#include <TDirectory.h>
#include <TH1.h>
#include <TH2.h>
#include <THStack.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TString.h>
#include <TMath.h>

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

typedef std::vector<std::string> vstring;
typedef std::vector<edm::ParameterSet> vParameterSet;

struct variableEntryType
{
  variableEntryType(const edm::ParameterSet& cfg)
    : meName_(cfg.getParameter<std::string>("meName")),
      xAxisTitle_(cfg.getParameter<std::string>("xAxisTitle"))
  {}
  ~variableEntryType() {}
  std::string meName_;
  std::string xAxisTitle_;
};

std::string terminate_dqmDirectory(const std::string& dqmDirectory)
{
  std::string dqmDirectory_terminated = dqmDirectory;

//--- add trailing '/'
  if ( dqmDirectory_terminated != "" &&
      dqmDirectory_terminated.find_last_of("/") != (dqmDirectory_terminated.length() - 1) )
    dqmDirectory_terminated.append("/");

  return dqmDirectory_terminated;
}

TH1* loadHistogram(TFile* inputFile, const std::string& directory, const std::string& meName)
{
  std::string meName_full = terminate_dqmDirectory(directory).append(meName);
  TH1* me = dynamic_cast<TH1*>(inputFile->Get(meName_full.data()));
  if ( !me ) 
    throw cms::Exception("makeZllRecoilCorrectionFinalPlots") 
      << "Failed to load histogram = " << meName_full << " from file = " << inputFile->GetName() << " !!\n";

  return me;
}

double square(double x)
{
  return x*x;
}

void addSysErr(TFile* inputFile, const variableEntryType& variable, const std::string& directory,
	       const TH1* meMC_central, const vstring& sysShiftsUp, const vstring& sysShiftsDown,
	       double mcScaleFactor,
	       std::vector<double>& errUp2MC_smSum, std::vector<double>& errDown2MC_smSum)
{
  assert(sysShiftsUp.size() == sysShiftsDown.size());

  size_t numSysShifts = sysShiftsUp.size();
  for ( size_t iSysShift = 0; iSysShift < numSysShifts; ++iSysShift ) {
    const std::string& sysShiftUp = sysShiftsUp[iSysShift];
    std::string directory_sysShiftUp = terminate_dqmDirectory(directory).append(sysShiftUp);
    TH1* meMC_sysShiftUp = loadHistogram(inputFile, directory_sysShiftUp, variable.meName_);
    meMC_sysShiftUp->Scale(mcScaleFactor);
    
    const std::string& sysShiftDown = sysShiftsDown[iSysShift];
    std::string directory_sysShiftDown = terminate_dqmDirectory(directory).append(sysShiftDown);
    TH1* meMC_sysShiftDown = loadHistogram(inputFile, directory_sysShiftDown, variable.meName_);
    meMC_sysShiftDown->Scale(mcScaleFactor);
    
    int numBins = meMC_sysShiftUp->GetNbinsX();
    for ( int iBin = 1; iBin <= numBins; ++iBin ) {
      double value_central      = meMC_central->GetBinContent(iBin);
      double value_sysShiftUp   = meMC_sysShiftUp->GetBinContent(iBin);
      double value_sysShiftDown = meMC_sysShiftDown->GetBinContent(iBin);

      double value_max = TMath::Max(value_sysShiftUp, value_sysShiftDown);
      if ( value_max > value_central ) errUp2MC_smSum[iBin - 1]   += square(value_max - value_central); 
      double value_min = TMath::Min(value_sysShiftUp, value_sysShiftDown);
      if ( value_central > value_min ) errDown2MC_smSum[iBin - 1] += square(value_central - value_min); 
    }
  }
}

void drawHistogram1d(TFile* inputFile, const variableEntryType& variable, 
		     const std::string& directoryData, const std::string& directoryMC_signal, const vstring& directoryMCs_background, 
		     const vstring& sysShiftsUp, const vstring& sysShiftsDown, 
		     bool scaleMCtoData,
		     const std::string& outputFileName)
{
  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 900);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);

  TH1* meData = loadHistogram(inputFile, directoryData, variable.meName_);
  if ( !meData->GetSumw2N() ) meData->Sumw2();
  meData->SetLineColor(1);
  meData->SetMarkerColor(1);
  meData->SetMarkerStyle(20);
  
  TH1* meMC_signal = loadHistogram(inputFile, directoryMC_signal, variable.meName_);
  TH1* meMC_signal_cloned = ( scaleMCtoData ) ?
    (TH1*)meMC_signal->Clone(std::string(meMC_signal->GetName()).append("_cloned").data()) : meMC_signal;
  if ( !meMC_signal_cloned->GetSumw2N() ) meMC_signal_cloned->Sumw2();
  meMC_signal_cloned->SetLineColor(2);
  meMC_signal_cloned->SetLineWidth(2);
  meMC_signal_cloned->SetFillColor(10);
  
  TH1* meMC_bgrSum = 0;
  for ( vstring::const_iterator directoryMC_bgr = directoryMCs_background.begin();
	directoryMC_bgr != directoryMCs_background.end(); ++directoryMC_bgr ) {
    TH1* meMC_bgr = loadHistogram(inputFile, *directoryMC_bgr, variable.meName_);
    if ( !meMC_bgr->GetSumw2N() ) meMC_bgr->Sumw2();
    if ( !meMC_bgrSum ) meMC_bgrSum = (TH1*)meMC_bgr->Clone(std::string(meMC_bgr->GetName()).append("_cloned").data());
    else meMC_bgrSum->Add(meMC_bgr);
  }
  if ( meMC_bgrSum ) meMC_bgrSum->SetFillColor(46);

  TH1* meMC_smSum = (TH1*)meMC_signal_cloned->Clone(std::string(meMC_signal_cloned->GetName()).append("_smSum").data());
  if ( meMC_bgrSum ) meMC_smSum->Add(meMC_bgrSum);

  double mcScaleFactor = 1.;
  if ( scaleMCtoData ) {
    mcScaleFactor = meData->Integral()/meMC_smSum->Integral();
    std::cout << "mcScaleFactor = " << mcScaleFactor << std::endl;
    meMC_signal_cloned->Scale(mcScaleFactor);
    meMC_bgrSum->Scale(mcScaleFactor);
    meMC_smSum->Scale(mcScaleFactor);
  }

  std::vector<double> errUp2MC_smSum(meData->GetNbinsX());
  std::vector<double> errDown2MC_smSum(meData->GetNbinsX());
  addSysErr(inputFile, variable, directoryMC_signal, 
	    meMC_signal_cloned, sysShiftsUp, sysShiftsDown, mcScaleFactor, errUp2MC_smSum, errDown2MC_smSum);
  for ( vstring::const_iterator directoryMC_bgr = directoryMCs_background.begin();
	directoryMC_bgr != directoryMCs_background.end(); ++directoryMC_bgr ) {
    TH1* meMC_bgr = loadHistogram(inputFile, *directoryMC_bgr, variable.meName_);
    addSysErr(inputFile, variable, *directoryMC_bgr, 
	      meMC_bgr, sysShiftsUp, sysShiftsDown, mcScaleFactor, errUp2MC_smSum, errDown2MC_smSum);
  }

  TH1* meMC_smErr = (TH1*)meMC_signal_cloned->Clone(std::string(meMC_signal_cloned->GetName()).append("_smErr").data());
  int numBins = meMC_smSum->GetNbinsX();
  for ( int iBin = 1; iBin <= numBins; ++iBin ) {
    double value_central = meMC_smSum->GetBinContent(iBin);

    double errUp   = TMath::Sqrt(errUp2MC_smSum[iBin - 1]);
    double errDown = TMath::Sqrt(errDown2MC_smSum[iBin - 1]);

    // CV: set bin content such that errors in up/down direction become symmetric
    meMC_smErr->SetBinContent(iBin, value_central + 0.5*(errUp - errDown));
    meMC_smErr->SetBinError(iBin, 0.5*(errUp + errDown));
  }
  meMC_smErr->SetLineColor(17);
  meMC_smErr->SetLineWidth(0);
  meMC_smErr->SetFillColor(17);
  meMC_smErr->SetFillStyle(3001);

  THStack stack_smSum("smSum", "smSum");
  stack_smSum.Add(meMC_signal_cloned);
  if ( meMC_bgrSum ) stack_smSum.Add(meMC_bgrSum);

  TPad* topPad = new TPad("topPad", "topPad", 0.00, 0.35, 1.00, 1.00);
  topPad->SetFillColor(10);
  topPad->SetTopMargin(0.04);
  topPad->SetLeftMargin(0.15);
  topPad->SetBottomMargin(0.03);
  topPad->SetRightMargin(0.05);

  TPad* bottomPad = new TPad("bottomPad", "bottomPad", 0.00, 0.00, 1.00, 0.35);
  bottomPad->SetFillColor(10);
  bottomPad->SetTopMargin(0.02);
  bottomPad->SetLeftMargin(0.15);
  bottomPad->SetBottomMargin(0.20);
  bottomPad->SetRightMargin(0.05);

  canvas->cd();
  topPad->Draw();
  topPad->cd();
  topPad->SetLogy(true);

  stack_smSum.SetMaximum(5.e1*TMath::Max(meData->GetMaximum(), stack_smSum.GetMaximum()));
  stack_smSum.SetMinimum(5.e-1);
  stack_smSum.SetTitle("");

  stack_smSum.Draw("hist");
  // CV: x-axis and y-axis of THStack do not exist until THStack::Draw is called,
  //     so need to postpone initialization of axes titles !!
  TAxis* xAxis_top = stack_smSum.GetXaxis();
  xAxis_top->SetTitle(variable.xAxisTitle_.data());
  xAxis_top->SetTitleOffset(1.2);
  xAxis_top->SetLabelColor(10);
  xAxis_top->SetTitleColor(10);

  TAxis* yAxis_top = stack_smSum.GetYaxis();
  yAxis_top->SetTitle("Events");
  yAxis_top->SetTitleOffset(1.4);

  meMC_smErr->Draw("e2same");

  meData->Draw("e1psame");
  
  TLegend legend(0.185, 0.77, 0.52, 0.95, "", "brNDC"); 
  legend.SetBorderSize(0);
  legend.SetFillColor(0);
  legend.AddEntry(meData, "Data", "p");
  legend.AddEntry(meMC_signal, "exp. Signal", "l");
  if ( meMC_bgrSum ) legend.AddEntry(meMC_bgrSum, "exp. Background", "f");
  legend.Draw();

  TPaveText statsData(0.70, 0.84, 0.95, 0.94, "brNDC"); 
  statsData.SetBorderSize(0);
  statsData.SetFillColor(0);
  statsData.AddText(Form("Mean = %2.2f", meData->GetMean()));
  statsData.AddText(Form("RMS  = %2.2f", meData->GetRMS()));
  statsData.SetTextColor(1);
  statsData.SetTextSize(0.045);
  statsData.Draw();
  
  TPaveText statsMC(0.70, 0.74, 0.95, 0.84, "brNDC"); 
  statsMC.SetBorderSize(0);
  statsMC.SetFillColor(0);
  statsMC.AddText(Form("Mean = %2.2f", meMC_smSum->GetMean()));
  statsMC.AddText(Form("RMS  = %2.2f", meMC_smSum->GetRMS()));
  statsMC.SetTextColor(2);
  statsMC.SetTextSize(0.045);
  statsMC.Draw();

  canvas->cd();
  bottomPad->Draw();
  bottomPad->cd();
  bottomPad->SetLogy(false);

  assert(meMC_smSum->GetNbinsX() == meData->GetNbinsX());
  TGraphErrors* graphDataToMCdiff = new TGraphErrors(numBins);
  for ( int iBin = 1; iBin <= numBins; ++iBin ) {
    double x = meMC_smSum->GetBinCenter(iBin);

    double y_data = meData->GetBinContent(iBin);
    double yErr_data = meData->GetBinError(iBin);

    double y_smSum = meMC_smSum->GetBinContent(iBin);
    if ( !(y_smSum > 0.) ) continue;
    double yErr_smSum = meMC_smSum->GetBinError(iBin);

    double diff = (y_data - y_smSum)/y_smSum;
    double diffErr2 = 0.;
    if ( y_data  > 0. ) diffErr2 += square(yErr_data/y_data);
    if ( y_smSum > 0. ) diffErr2 += square(yErr_smSum/y_smSum);
    diffErr2 *= square(diff);

    graphDataToMCdiff->SetPoint(iBin - 1, x, diff);
    graphDataToMCdiff->SetPointError(iBin - 1, 0., TMath::Sqrt(diffErr2));
  }

  TGraphErrors* graphMCerr = new TGraphErrors(numBins);
  for ( int iBin = 1; iBin <= numBins; ++iBin ) {
    double x = meMC_smSum->GetBinCenter(iBin);
    
    if ( !(meMC_smSum->GetBinContent(iBin) > 0.) ) continue;
    double rel = meMC_smErr->GetBinContent(iBin)/meMC_smSum->GetBinContent(iBin);
    double relErr = meMC_smErr->GetBinError(iBin)/meMC_smSum->GetBinContent(iBin);

    graphMCerr->SetPoint(iBin - 1, x, rel);
    graphMCerr->SetPointError(iBin - 1, 0., relErr);
  }

  TAxis* xAxis_bottom = graphDataToMCdiff->GetXaxis();
  xAxis_bottom->SetTitle(variable.xAxisTitle_.data());
  xAxis_bottom->SetTitleOffset(1.20);
  xAxis_bottom->SetNdivisions(505);
  xAxis_bottom->SetTitleOffset(1.1);
  xAxis_bottom->SetTitleSize(0.08);
  xAxis_bottom->SetLabelOffset(0.02);
  xAxis_bottom->SetLabelSize(0.08);
  xAxis_bottom->SetTickLength(0.055);
    
  TAxis* yAxis_bottom = graphDataToMCdiff->GetYaxis();
  yAxis_bottom->SetTitle("#frac{Data - Simulation}{Simulation}");
  yAxis_bottom->SetTitleOffset(1.10);
  yAxis_bottom->CenterTitle();
  yAxis_bottom->SetTitleOffset(0.9);
  yAxis_bottom->SetTitleSize(0.08);
  yAxis_bottom->SetLabelSize(0.08);
  yAxis_bottom->SetTickLength(0.04);
    
  graphDataToMCdiff->SetTitle("");
  double maxDiff = 0.;    
  for ( int iPoint = 0; iPoint < numBins; ++iPoint ) {
    double x, diff;
    graphDataToMCdiff->GetPoint(iPoint, x, diff);
    double err = graphDataToMCdiff->GetErrorY(iPoint);
    diff = TMath::Max(TMath::Abs(diff + err), TMath::Abs(diff - err));
    if ( diff > maxDiff ) maxDiff = diff;
  }
  double maxDiff01 = 0.1*TMath::Ceil(1.2*maxDiff*10.);
  graphDataToMCdiff->SetMaximum(+maxDiff01);
  graphDataToMCdiff->SetMinimum(-maxDiff01);
  
  graphDataToMCdiff->SetMarkerStyle(meData->GetMarkerStyle());
  graphDataToMCdiff->SetMarkerColor(meData->GetMarkerColor());
  graphDataToMCdiff->SetLineColor(meData->GetLineColor());
  graphDataToMCdiff->Draw("AP");   

  graphMCerr->SetMarkerSize(0);
  graphMCerr->SetMarkerStyle(1);
  graphMCerr->SetLineColor(meMC_smErr->GetLineColor());
  graphMCerr->SetFillColor(meMC_smErr->GetFillColor());
  graphMCerr->SetFillStyle(meMC_smErr->GetFillStyle());
  graphMCerr->Draw("2"); 

  canvas->Update();

  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  outputFileName_plot.append("_").append(variable.meName_);
  if ( scaleMCtoData ) outputFileName_plot.append("_scaled");
  if ( idx != std::string::npos ) outputFileName_plot.append(std::string(outputFileName, idx));
  else                            outputFileName_plot.append(".png");
  canvas->Print(outputFileName_plot.data());
  
  if ( meMC_signal_cloned != meMC_signal ) delete meMC_signal_cloned;
  delete graphDataToMCdiff;
  delete canvas;
}	     

double getMaximum(TGraphErrors* graph)
{
  double retVal = 0.;

  int numPoints = graph->GetN();
  for ( int iPoint = 0; iPoint < numPoints; ++iPoint ) {
    Double_t x, y;
    graph->GetPoint(iPoint, x, y);

    Double_t yErr = graph->GetErrorY(iPoint);

    if ( (y + yErr) > retVal ) retVal = y + yErr;
  }

  return retVal;
}

void drawGraphs(const std::string& yAxisTitle, const std::string& outputFileName, const std::string& outputFileLabel,
		TGraphErrors* graphData1, const std::string& legendEntryData1, 
		TGraphErrors* graphMC1, const std::string& legendEntryMC1, 
		TGraphErrors* graphData2 = 0, const std::string& legendEntryData2 = "", 
		TGraphErrors* graphMC2 = 0, const std::string& legendEntryMC2 = "", 
		TGraphErrors* graphData3 = 0, const std::string& legendEntryData3 = "", 
		TGraphErrors* graphMC3 = 0, const std::string& legendEntryMC3 = "", 
		TGraphErrors* graphData4 = 0, const std::string& legendEntryData4 = "", 
		TGraphErrors* graphMC4 = 0, const std::string& legendEntryMC4 = "", 
		TGraphErrors* graphData5 = 0, const std::string& legendEntryData5 = "", 
		TGraphErrors* graphMC5 = 0, const std::string& legendEntryMC5 = "")
{
  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);
  
  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);
  
  canvas->SetLogy(false);
  
  double yMax = getMaximum(graphData1);
  yMax = TMath::Max(yMax, getMaximum(graphMC1));
  unsigned numGraphs = 2;
  if ( graphData2 ) {
    yMax = TMath::Max(yMax, getMaximum(graphData2));
    ++numGraphs;
  }
  if ( graphMC2   ) {
    yMax = TMath::Max(yMax, getMaximum(graphMC2));
    ++numGraphs;
  }
  if ( graphData3 ) {
    yMax = TMath::Max(yMax, getMaximum(graphData3));
    ++numGraphs;
  }
  if ( graphMC3   ) {
    yMax = TMath::Max(yMax, getMaximum(graphMC3));
    ++numGraphs;
  }
  if ( graphData4 ) {
    yMax = TMath::Max(yMax, getMaximum(graphData4));
    ++numGraphs;
  }
  if ( graphMC4   ) {
    yMax = TMath::Max(yMax, getMaximum(graphMC4));
    ++numGraphs;
  }
  if ( graphData5 ) {
    yMax = TMath::Max(yMax, getMaximum(graphData5));
    ++numGraphs;
  }
  if ( graphMC5   ) {
    yMax = TMath::Max(yMax, getMaximum(graphMC5));
    ++numGraphs;
  }

  graphData1->SetTitle("");
  
  graphData1->SetMaximum(1.2*yMax);
  graphData1->SetMinimum(0.);

  graphData1->GetXaxis()->SetTitle("q_{T} / GeV");
  graphData1->GetXaxis()->SetTitleOffset(1.2);
  graphData1->GetYaxis()->SetTitle(yAxisTitle.data());
  graphData1->GetYaxis()->SetTitleOffset(1.4);

  graphData1->SetLineColor(1);
  graphData1->SetMarkerColor(1);
  graphData1->SetMarkerStyle(20);
  graphData1->Draw("ap");

  graphMC1->SetLineColor(1);
  graphMC1->SetMarkerColor(1);
  graphMC1->SetMarkerStyle(24);
  graphMC1->Draw("p");

  double textSize = ( numGraphs <= 8 ) ? 0.045 : 0.035;
  TLegend legend(0.64, 0.89 - textSize*numGraphs, 0.89, 0.89, "", "brNDC"); 
  legend.SetBorderSize(0);
  legend.SetFillColor(0);
  legend.AddEntry(graphData1, legendEntryData1.data(), "p");
  legend.AddEntry(graphMC1,   legendEntryMC1.data(),   "p");

  if ( graphData2 ) {
    graphData2->SetLineColor(2);
    graphData2->SetMarkerColor(2);
    graphData2->SetMarkerStyle(21);
    graphData2->Draw("p");
    legend.AddEntry(graphData2, legendEntryData2.data(), "p");
  }

  if ( graphMC2 ) {
    graphMC2->SetLineColor(2);
    graphMC2->SetMarkerColor(2);
    graphMC2->SetMarkerStyle(25);
    graphMC2->Draw("p");
    legend.AddEntry(graphMC2, legendEntryMC2.data(), "p");
  }

  if ( graphData3 ) {
    graphData3->SetLineColor(3);
    graphData3->SetMarkerColor(3);
    graphData3->SetMarkerStyle(22);
    graphData3->Draw("p");
    legend.AddEntry(graphData3, legendEntryData3.data(), "p");
  }

  if ( graphMC3 ) {
    graphMC3->SetLineColor(3);
    graphMC3->SetMarkerColor(3);
    graphMC3->SetMarkerStyle(26);
    graphMC3->Draw("p");
    legend.AddEntry(graphMC3, legendEntryMC3.data(), "p");
  }

  if ( graphData4 ) {
    graphData4->SetLineColor(4);
    graphData4->SetMarkerColor(4);
    graphData4->SetMarkerStyle(23);
    graphData4->Draw("p");
    legend.AddEntry(graphData4, legendEntryData4.data(), "p");
  }

  if ( graphMC4 ) {
    graphMC4->SetLineColor(4);
    graphMC4->SetMarkerColor(4);
    graphMC4->SetMarkerStyle(32);
    graphMC4->Draw("p");
    legend.AddEntry(graphMC4, legendEntryMC4.data(), "p");
  }

  if ( graphData5 ) {
    graphData5->SetLineColor(6);
    graphData5->SetMarkerColor(6);
    graphData5->SetMarkerStyle(34);
    graphData5->Draw("p");
    legend.AddEntry(graphData5, legendEntryData5.data(), "p");
  }

  if ( graphMC5 ) {
    graphMC5->SetLineColor(6);
    graphMC5->SetMarkerColor(6);
    graphMC5->SetMarkerStyle(28);
    graphMC5->Draw("p");
    legend.AddEntry(graphMC5, legendEntryMC5.data(), "p");
  }

  legend.Draw();

  canvas->Update();

  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  outputFileName_plot.append("_").append(outputFileLabel);
  if ( idx != std::string::npos ) outputFileName_plot.append(std::string(outputFileName, idx));
  else                            outputFileName_plot.append(".png");
  canvas->Print(outputFileName_plot.data());
}

std::pair<TGraphErrors*, TGraphErrors*> makeGraphs_mean_and_rms(TH2* histogram)
{
  TAxis* xAxis = histogram->GetXaxis();

  int numBins = xAxis->GetNbins();

  TGraphErrors* graph_mean = new TGraphErrors(numBins);
  TGraphErrors* graph_rms  = new TGraphErrors(numBins);

  for ( int iBin = 1; iBin <= numBins; ++iBin ) {
    TString histogramName_proj = Form("%s_py_%i", histogram->GetName(), iBin);
    TH1D* histogram_proj = histogram->ProjectionY(histogramName_proj.Data(), iBin, iBin, "e");

    double x = xAxis->GetBinCenter(iBin);

    double mean = TMath::Abs(histogram_proj->GetMean());
    double meanErr = TMath::Abs(histogram_proj->GetMeanError());
    
    double rms = histogram_proj->GetRMS();
    double rmsErr = histogram_proj->GetRMSError();

    int iPoint = iBin - 1;

    graph_mean->SetPoint(iPoint, x, mean);
    graph_mean->SetPointError(iPoint, 0., meanErr);

    graph_rms->SetPoint(iPoint, x, rms);
    graph_rms->SetPointError(iPoint, 0., rmsErr);
  }

  return std::pair<TGraphErrors*, TGraphErrors*>(graph_mean, graph_rms);
}

TGraphErrors* makeGraph_mean(TH2* histogram)
{
  return makeGraphs_mean_and_rms(histogram).first;
}

TGraphErrors* makeGraph_rms(TH2* histogram)
{
  return makeGraphs_mean_and_rms(histogram).second;
}

struct plotUvsQtNumVtxType
{
  plotUvsQtNumVtxType(TFile* inputFile, int numVtxMin, int numVtxMax, 
		      const std::string& directoryData, const std::string& directoryMC)
    : numVtxMin_(numVtxMin),
      numVtxMax_(numVtxMax)
  {
    TString label;
    if      ( numVtxMin_ == -1 ) label = Form("Le%i",   numVtxMax_);
    else if ( numVtxMax_ == -1 ) label = Form("Ge%i",   numVtxMin_);
    else                         label = Form("%ito%i", numVtxMin_, numVtxMax_);

    meUparlDivQtVsQtData_ = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryData, TString("uParlDivQtVsQt").Append(label).Data()));
    meUparlVsQtData_      = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryData, TString("uParlVsQt").Append(label).Data()));
    meUperpVsQtData_      = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryData, TString("uPerpVsQt").Append(label).Data()));

    graphUparlResponseData_   = makeGraph_mean(meUparlDivQtVsQtData_);
    graphUparlResolutionData_ = makeGraph_rms(meUparlVsQtData_);
    graphUperpResolutionData_ = makeGraph_rms(meUperpVsQtData_);

    meUparlDivQtVsQtMC_   = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryMC,   TString("uParlDivQtVsQt").Append(label).Data()));
    meUparlVsQtMC_        = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryMC,   TString("uParlVsQt").Append(label).Data()));
    meUperpVsQtMC_        = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryMC,   TString("uPerpVsQt").Append(label).Data()));

    graphUparlResponseMC_     = makeGraph_mean(meUparlDivQtVsQtMC_);
    graphUparlResolutionMC_   = makeGraph_rms(meUparlVsQtMC_);
    graphUperpResolutionMC_   = makeGraph_rms(meUperpVsQtMC_);

    if      ( numVtxMin_ == -1 ) label = Form("N_{vtx} < %i",      numVtxMax_);
    else if ( numVtxMax_ == -1 ) label = Form("N_{vtx} > %i",      numVtxMin_);
    else                         label = Form("%i < N_{vtx} < %i", numVtxMin_, numVtxMax_);

    legendEntryData_ = std::string("Data").append(": ").append(label);
    legendEntryMC_   = std::string("Sim.").append(": ").append(label);
  }
  ~plotUvsQtNumVtxType() {}

  int numVtxMin_;
  int numVtxMax_;

  TH2* meUparlDivQtVsQtData_;
  TH2* meUparlVsQtData_;
  TH2* meUperpVsQtData_;

  TGraphErrors* graphUparlResponseData_;
  TGraphErrors* graphUparlResolutionData_;
  TGraphErrors* graphUperpResolutionData_;

  TH2* meUparlDivQtVsQtMC_;
  TH2* meUparlVsQtMC_;
  TH2* meUperpVsQtMC_;

  TGraphErrors* graphUparlResponseMC_;
  TGraphErrors* graphUparlResolutionMC_;
  TGraphErrors* graphUperpResolutionMC_;

  std::string legendEntryData_;
  std::string legendEntryMC_;
};

int main(int argc, const char* argv[])
{
//--- parse command-line arguments
  if ( argc < 2 ) {
    std::cout << "Usage: " << argv[0] << " [parameters.py]" << std::endl;
    return 0;
  }

  std::cout << "<makeZllRecoilCorrectionFinalPlots>:" << std::endl;  

//--- disable pop-up windows showing graphics output
  gROOT->SetBatch(true);

//--- load framework libraries
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();

//--- keep track of time it takes the macro to execute
  TBenchmark clock;
  clock.Start("makeZllRecoilCorrectionFinalPlots");

//--- read python configuration parameters
  if ( !edm::readPSetsFrom(argv[1])->existsAs<edm::ParameterSet>("process") ) 
    throw cms::Exception("makeZllRecoilCorrectionFinalPlots") 
      << "No ParameterSet 'process' found in configuration file = " << argv[1] << " !!\n";

  edm::ParameterSet cfg = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("process");

  edm::ParameterSet cfgMakeZllRecoilCorrectionPlots = cfg.getParameter<edm::ParameterSet>("makeZllRecoilCorrectionFinalPlots");

  std::string directoryData           = cfgMakeZllRecoilCorrectionPlots.getParameter<std::string>("directoryData");
  std::string directoryMC_signal      = cfgMakeZllRecoilCorrectionPlots.getParameter<std::string>("directoryMC_signal");
  vstring     directoryMCs_background = cfgMakeZllRecoilCorrectionPlots.getParameter<vstring>("directoryMCs_background");

  vstring sysShiftsUp   = cfgMakeZllRecoilCorrectionPlots.getParameter<vstring>("sysShiftsUp");
  vstring sysShiftsDown = cfgMakeZllRecoilCorrectionPlots.getParameter<vstring>("sysShiftsDown");
  if ( sysShiftsUp.size() != sysShiftsDown.size() )
    throw cms::Exception("makeZllRecoilCorrectionFinalPlots") 
      << "Number of 'up' shifts must match number of 'down' shifts !!\n";

  vParameterSet cfgVariables = cfgMakeZllRecoilCorrectionPlots.getParameter<vParameterSet>("variables");
  std::vector<variableEntryType> variables;
  for ( vParameterSet::const_iterator cfgVariable = cfgVariables.begin();
	cfgVariable != cfgVariables.end(); ++cfgVariable ) {
    variables.push_back(variableEntryType(*cfgVariable));
  }

  std::string outputFileName = cfgMakeZllRecoilCorrectionPlots.getParameter<std::string>("outputFileName");

  fwlite::InputSource inputFiles(cfg); 
  if ( inputFiles.files().size() != 1 ) 
    throw cms::Exception("makeZllRecoilCorrectionFinalPlots") 
      << "Input file must be unique, got = " << format_vstring(inputFiles.files()) << " !!\n";
  std::string inputFileName = (*inputFiles.files().begin());
  
//--- open input file
  TFile* inputFile = TFile::Open(inputFileName.data());
  if ( !inputFile ) 
    throw cms::Exception("makeZllRecoilCorrectionFinalPlots") 
      << "Failed to open inputFile = " << inputFileName << " !!\n";

//--- make control plots of different variable distributions
  for ( std::vector<variableEntryType>::const_iterator variable = variables.begin();
	variable != variables.end(); ++variable ) {
    drawHistogram1d(inputFile, *variable, directoryData, directoryMC_signal, directoryMCs_background, 
		    sysShiftsUp, sysShiftsDown, false, outputFileName);
    drawHistogram1d(inputFile, *variable, directoryData, directoryMC_signal, directoryMCs_background, 
		    sysShiftsUp, sysShiftsDown, true,  outputFileName);
  }

//--- make plots of mean(uParl)/qT, rms(uParl)/qT, rms(uPerp)/qT
  TH2* meUparlDivQtVsQt_data = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryData,      "uParlDivQtVsQt"));
  TH2* meUparlVsQt_data      = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryData,      "uParlVsQt"));
  TH2* meUperpVsQt_data      = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryData,      "uPerpVsQt"));

  TGraphErrors* graphUparlResponse_data   = makeGraph_mean(meUparlDivQtVsQt_data);
  TGraphErrors* graphUparlResolution_data = makeGraph_rms(meUparlVsQt_data);
  TGraphErrors* graphUperpResolution_data = makeGraph_rms(meUperpVsQt_data);

  TH2* meUparlDivQtVsQt_mc   = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryMC_signal, "uParlDivQtVsQt"));
  TH2* meUparlVsQt_mc        = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryMC_signal, "uParlVsQt"));
  TH2* meUperpVsQt_mc        = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryMC_signal, "uPerpVsQt"));

  TGraphErrors* graphUparlResponse_mc     = makeGraph_mean(meUparlDivQtVsQt_mc);
  TGraphErrors* graphUparlResolution_mc   = makeGraph_rms(meUparlVsQt_mc);
  TGraphErrors* graphUperpResolution_mc   = makeGraph_rms(meUperpVsQt_mc);

  drawGraphs("<u_{parl}>/q_{T}", outputFileName, "uParlResponse",
	     graphUparlResponse_data, "Data", graphUparlResponse_mc, "Simulation");
  drawGraphs("RMS(u_{parl})", outputFileName, "uParlResolution",
	     graphUparlResolution_data, "Data", graphUparlResolution_mc, "Simulation");
  drawGraphs("RMS(u_{perp})", outputFileName, "uPerpResolution",
	     graphUperpResolution_data, "Data", graphUperpResolution_mc, "Simulation");

//--- make plots of mean(uParl)/qT, rms(uParl)/qT, rms(uPerp)/qT
//    in different bins of reconstructed vertex multiplicity
  plotUvsQtNumVtxType* plotUvsQtNumVtxLe2   = new plotUvsQtNumVtxType(inputFile, -1,  2, directoryData, directoryMC_signal);
  plotUvsQtNumVtxType* plotUvsQtNumVtx3to5  = new plotUvsQtNumVtxType(inputFile,  3,  5, directoryData, directoryMC_signal);
  plotUvsQtNumVtxType* plotUvsQtNumVtx6to8  = new plotUvsQtNumVtxType(inputFile,  6,  8, directoryData, directoryMC_signal);
  plotUvsQtNumVtxType* plotUvsQtNumVtx9to11 = new plotUvsQtNumVtxType(inputFile,  9, 11, directoryData, directoryMC_signal);
  plotUvsQtNumVtxType* plotUvsQtNumVtxGe12  = new plotUvsQtNumVtxType(inputFile, 12, -1, directoryData, directoryMC_signal);
  
  drawGraphs("<u_{parl}>/q_{T}", outputFileName, "uParlResponse_binnedVtxMultiplicity",
	     plotUvsQtNumVtxLe2->graphUparlResponseData_, plotUvsQtNumVtxLe2->legendEntryData_,
	     plotUvsQtNumVtxLe2->graphUparlResponseMC_, plotUvsQtNumVtxLe2->legendEntryMC_,
	     plotUvsQtNumVtx3to5->graphUparlResponseData_, plotUvsQtNumVtx3to5->legendEntryData_,
	     plotUvsQtNumVtx3to5->graphUparlResponseMC_, plotUvsQtNumVtx3to5->legendEntryMC_,
	     plotUvsQtNumVtx6to8->graphUparlResponseData_, plotUvsQtNumVtx6to8->legendEntryData_,
	     plotUvsQtNumVtx6to8->graphUparlResponseMC_, plotUvsQtNumVtx6to8->legendEntryMC_,
	     plotUvsQtNumVtx9to11->graphUparlResponseData_, plotUvsQtNumVtx9to11->legendEntryData_,
	     plotUvsQtNumVtx9to11->graphUparlResponseMC_, plotUvsQtNumVtx9to11->legendEntryMC_,
	     plotUvsQtNumVtxGe12->graphUparlResponseData_, plotUvsQtNumVtxGe12->legendEntryData_,
	     plotUvsQtNumVtxGe12->graphUparlResponseMC_, plotUvsQtNumVtxGe12->legendEntryMC_);
  drawGraphs("RMS(u_{parl})", outputFileName, "uParlResolution_binnedVtxMultiplicity",
	     plotUvsQtNumVtxLe2->graphUparlResolutionData_, plotUvsQtNumVtxLe2->legendEntryData_,
	     plotUvsQtNumVtxLe2->graphUparlResolutionMC_, plotUvsQtNumVtxLe2->legendEntryMC_,
	     plotUvsQtNumVtx3to5->graphUparlResolutionData_, plotUvsQtNumVtx3to5->legendEntryData_,
	     plotUvsQtNumVtx3to5->graphUparlResolutionMC_, plotUvsQtNumVtx3to5->legendEntryMC_,
	     plotUvsQtNumVtx6to8->graphUparlResolutionData_, plotUvsQtNumVtx6to8->legendEntryData_,
	     plotUvsQtNumVtx6to8->graphUparlResolutionMC_, plotUvsQtNumVtx6to8->legendEntryMC_,
	     plotUvsQtNumVtx9to11->graphUparlResolutionData_, plotUvsQtNumVtx9to11->legendEntryData_,
	     plotUvsQtNumVtx9to11->graphUparlResolutionMC_, plotUvsQtNumVtx9to11->legendEntryMC_,
	     plotUvsQtNumVtxGe12->graphUparlResolutionData_, plotUvsQtNumVtxGe12->legendEntryData_,
	     plotUvsQtNumVtxGe12->graphUparlResolutionMC_, plotUvsQtNumVtxGe12->legendEntryMC_);
  drawGraphs("RMS(u_{perp})", outputFileName, "uPerpResolution_binnedVtxMultiplicity",
	     plotUvsQtNumVtxLe2->graphUperpResolutionData_, plotUvsQtNumVtxLe2->legendEntryData_,
	     plotUvsQtNumVtxLe2->graphUperpResolutionMC_, plotUvsQtNumVtxLe2->legendEntryMC_,
	     plotUvsQtNumVtx3to5->graphUperpResolutionData_, plotUvsQtNumVtx3to5->legendEntryData_,
	     plotUvsQtNumVtx3to5->graphUperpResolutionMC_, plotUvsQtNumVtx3to5->legendEntryMC_,
	     plotUvsQtNumVtx6to8->graphUperpResolutionData_, plotUvsQtNumVtx6to8->legendEntryData_,
	     plotUvsQtNumVtx6to8->graphUperpResolutionMC_, plotUvsQtNumVtx6to8->legendEntryMC_,
	     plotUvsQtNumVtx9to11->graphUperpResolutionData_, plotUvsQtNumVtx9to11->legendEntryData_,
	     plotUvsQtNumVtx9to11->graphUperpResolutionMC_, plotUvsQtNumVtx9to11->legendEntryMC_,
	     plotUvsQtNumVtxGe12->graphUperpResolutionData_, plotUvsQtNumVtxGe12->legendEntryData_,
	     plotUvsQtNumVtxGe12->graphUperpResolutionMC_, plotUvsQtNumVtxGe12->legendEntryMC_);
  
  delete inputFile;

//--print time that it took macro to run
  std::cout << "finished executing makeZllRecoilCorrectionFinalPlots macro:" << std::endl;
  clock.Show("makeZllRecoilCorrectionFinalPlots");

  return 0;
}
