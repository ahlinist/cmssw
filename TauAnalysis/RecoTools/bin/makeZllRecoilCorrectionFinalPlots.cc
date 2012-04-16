
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/FWLite/interface/InputSource.h"

#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"
#include "TauAnalysis/RecoTools/bin/fitZllRecoilCorrectionAuxFunctions.h"

#include <TFile.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TBenchmark.h>
#include <TDirectory.h>
#include <TH1.h>
#include <TH2.h>
#include <THStack.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
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

double getScaleFactor(const std::map<std::string, double>& mcScaleFactors, const std::string& directory, bool& isScaleFactorDefined)
{
  double scaleFactor = 1.;

  int numMatches = 0;

  for ( std::map<std::string, double>::const_iterator mcScaleFactor_it = mcScaleFactors.begin();
	mcScaleFactor_it != mcScaleFactors.end(); ++mcScaleFactor_it ) {
    if ( directory.find(mcScaleFactor_it->first) != std::string::npos ) {
      scaleFactor *= mcScaleFactor_it->second;
      ++numMatches;
    }
  }
  
  if ( numMatches >  1 )
    throw cms::Exception("makeZllRecoilCorrectionFinalPlots") 
      << "Definition of scale-factor for directory = " << directory << " is ambiguous !!\n";
  
  if ( numMatches == 1 ) isScaleFactorDefined = true;
  
  return scaleFactor;
}

TH1* loadHistogram(TFile* inputFile, const std::string& directory, 
		   const std::map<std::string, double>& mcScaleFactors, const std::string& meName)
{
  static std::map<std::string, TH1*> histograms; // key = inputFileName_meName_full

  std::string meName_full = terminate_dqmDirectory(directory).append(meName);

  std::string key = std::string(inputFile->GetName()).append("_").append(meName_full);

  if ( histograms.find(key) == histograms.end() ) {
    TH1* me = dynamic_cast<TH1*>(inputFile->Get(meName_full.data()));
    if ( !me ) 
      throw cms::Exception("makeZllRecoilCorrectionFinalPlots") 
	<< "Failed to load histogram = " << meName_full << " from file = " << inputFile->GetName() << " !!\n";
    
    bool isScaleFactorDefined = false;
    double scaleFactor = getScaleFactor(mcScaleFactors, directory, isScaleFactorDefined);
    if ( isScaleFactorDefined ) me->Scale(scaleFactor);
    
    histograms[key] = me;
  }
  
  return histograms[key];
}

std::string getDirectorySysErr(const std::string& directory, const std::string& sysShift)
{
  //std::string directory_sysShift = terminate_dqmDirectory(directory).append(sysShift);
  size_t idx = directory.find_last_of('/');
  std::string directory_sysShift = std::string(directory, 0, idx);
  directory_sysShift = terminate_dqmDirectory(directory_sysShift).append(sysShift);
  if ( idx != std::string::npos ) directory_sysShift.append(std::string(directory, idx));
  return directory_sysShift;
}

void addSysErr(TFile* inputFile, const variableEntryType& variable, const std::string& directory,
	       const std::map<std::string, double>& mcScaleFactors,
	       const TH1* meMC_central, const vstring& sysShiftsUp, const vstring& sysShiftsDown,
	       double mcToDataScaleFactor,
	       std::vector<double>& errUp2MC_smSum, std::vector<double>& errDown2MC_smSum)
{
  std::cout << "<addSysErr>:" << std::endl;

  assert(sysShiftsUp.size() == sysShiftsDown.size());

  size_t numSysShifts = sysShiftsUp.size();
  for ( size_t iSysShift = 0; iSysShift < numSysShifts; ++iSysShift ) {
    const std::string& sysShiftUp = sysShiftsUp[iSysShift];
    std::string directory_sysShiftUp = getDirectorySysErr(directory, sysShiftUp);
    TH1* meMC_sysShiftUp = loadHistogram(inputFile, directory_sysShiftUp, mcScaleFactors, variable.meName_);
    
    const std::string& sysShiftDown = sysShiftsDown[iSysShift];
    std::string directory_sysShiftDown = getDirectorySysErr(directory, sysShiftDown);
    TH1* meMC_sysShiftDown = loadHistogram(inputFile, directory_sysShiftDown, mcScaleFactors, variable.meName_);
    
    int numBins = meMC_sysShiftUp->GetNbinsX();
    for ( int iBin = 1; iBin <= numBins; ++iBin ) {
      double value_central      = meMC_central->GetBinContent(iBin);
      double value_sysShiftUp   = mcToDataScaleFactor*meMC_sysShiftUp->GetBinContent(iBin);
      double value_sysShiftDown = mcToDataScaleFactor*meMC_sysShiftDown->GetBinContent(iBin);

      std::cout << "bin = " << iBin << ": central = " << value_central << "," 
		<< " " << sysShiftUp << " = " << value_sysShiftUp << ", " << sysShiftDown << " = " << value_sysShiftDown << std::endl;
      
      double value_max = TMath::Max(value_sysShiftUp, value_sysShiftDown);
      if ( value_max > value_central ) errUp2MC_smSum[iBin - 1]   += square(value_max - value_central); 
      double value_min = TMath::Min(value_sysShiftUp, value_sysShiftDown);
      if ( value_central > value_min ) errDown2MC_smSum[iBin - 1] += square(value_central - value_min); 
    }
  }
}

void drawHistogram1d(TFile* inputFile, const variableEntryType& variable, 
		     const std::string& directoryData, const std::string& directoryMC_signal, const vstring& directoryMCs_background, 
		     const std::map<std::string, double>& mcScaleFactors, const std::string& runPeriod,
		     const vstring& sysShiftsUp, const vstring& sysShiftsDown, 
		     bool scaleMCtoData, bool drawDataToMCratio, bool useLogScaleY, bool drawSysUncertainty,
		     const std::string& outputFileName)
{
  int canvasSizeY = ( drawDataToMCratio ) ? 900 : 600;
  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, canvasSizeY);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);

  TH1* meData = loadHistogram(inputFile, directoryData, mcScaleFactors, variable.meName_);
  if ( !meData->GetSumw2N() ) meData->Sumw2();
  meData->SetLineColor(1);
  meData->SetMarkerColor(1);
  meData->SetMarkerStyle(20);

  TH1* meMC_signal = loadHistogram(inputFile, directoryMC_signal, mcScaleFactors, variable.meName_);
  TH1* meMC_signal_cloned = ( scaleMCtoData ) ?
    (TH1*)meMC_signal->Clone(std::string(meMC_signal->GetName()).append("_cloned").data()) : meMC_signal;
  if ( !meMC_signal_cloned->GetSumw2N() ) meMC_signal_cloned->Sumw2();
  meMC_signal_cloned->SetLineColor(2);
  meMC_signal_cloned->SetLineWidth(2);
  meMC_signal_cloned->SetFillColor(10);

  TH1* meMC_bgrSum = 0;
  for ( vstring::const_iterator directoryMC_bgr = directoryMCs_background.begin();
	directoryMC_bgr != directoryMCs_background.end(); ++directoryMC_bgr ) {
    TH1* meMC_bgr = loadHistogram(inputFile, *directoryMC_bgr, mcScaleFactors, variable.meName_);
    if ( !meMC_bgr->GetSumw2N() ) meMC_bgr->Sumw2();
    if ( !meMC_bgrSum ) {
      meMC_bgrSum = (TH1*)meMC_bgr->Clone(std::string(meMC_bgr->GetName()).append("_cloned").data());
      if ( !meMC_bgrSum->GetSumw2N() ) meMC_bgrSum->Sumw2();
    }
    else meMC_bgrSum->Add(meMC_bgr);
  }
  if ( meMC_bgrSum ) meMC_bgrSum->SetFillColor(46);

  TH1* meMC_smSum = (TH1*)meMC_signal_cloned->Clone(std::string(meMC_signal_cloned->GetName()).append("_smSum").data());
  if ( !meMC_smSum->GetSumw2N() ) meMC_smSum->Sumw2();
  if ( meMC_bgrSum ) meMC_smSum->Add(meMC_bgrSum);

  double mcToDataScaleFactor = 1.;
  if ( scaleMCtoData ) {
    mcToDataScaleFactor = meData->Integral()/meMC_smSum->Integral();
    std::cout << "mcToDataScaleFactor = " << mcToDataScaleFactor << std::endl;
    meMC_signal_cloned->Scale(mcToDataScaleFactor);
    meMC_bgrSum->Scale(mcToDataScaleFactor);
    meMC_smSum->Scale(mcToDataScaleFactor);
  }

  std::vector<double> errUp2MC_smSum(meData->GetNbinsX());
  std::vector<double> errDown2MC_smSum(meData->GetNbinsX());
  std::cout << "adding systematic uncertainties for signal..." << std::endl;
  addSysErr(inputFile, variable, directoryMC_signal, mcScaleFactors,
	    meMC_signal_cloned, sysShiftsUp, sysShiftsDown, mcToDataScaleFactor, errUp2MC_smSum, errDown2MC_smSum);
  for ( vstring::const_iterator directoryMC_bgr = directoryMCs_background.begin();
	directoryMC_bgr != directoryMCs_background.end(); ++directoryMC_bgr ) {
    std::cout << "adding systematic uncertainties for " << (*directoryMC_bgr) << " background..." << std::endl;
    TH1* meMC_bgr = loadHistogram(inputFile, *directoryMC_bgr, mcScaleFactors, variable.meName_);
    TH1* meMC_bgr_cloned = ( scaleMCtoData ) ?
      (TH1*)meMC_bgr->Clone(std::string(meMC_bgr->GetName()).append("_cloned").data()) : meMC_bgr;
    if ( !meMC_bgr_cloned->GetSumw2N() ) meMC_bgr_cloned->Sumw2();
    meMC_bgr_cloned->Scale(mcToDataScaleFactor);
    addSysErr(inputFile, variable, *directoryMC_bgr, mcScaleFactors,
	      meMC_bgr_cloned, sysShiftsUp, sysShiftsDown, mcToDataScaleFactor, errUp2MC_smSum, errDown2MC_smSum);
    if ( meMC_bgr_cloned != meMC_bgr ) delete meMC_bgr_cloned;
  }

  TH1* meMC_smErr = 0;
  TH1* meMC_smErrUp = 0;
  TH1* meMC_smErrDown = 0;
  if ( sysShiftsUp.size() > 0 ) {
    meMC_smErr = (TH1*)meMC_signal_cloned->Clone(std::string(meMC_signal_cloned->GetName()).append("_smErr").data());
    meMC_smErrUp = (TH1*)meMC_signal_cloned->Clone(std::string(meMC_signal_cloned->GetName()).append("_smErrUp").data());
    meMC_smErrDown = (TH1*)meMC_signal_cloned->Clone(std::string(meMC_signal_cloned->GetName()).append("_smErrDown").data());
    int numBins = meMC_smSum->GetNbinsX();
    for ( int iBin = 1; iBin <= numBins; ++iBin ) {
      double value_central = meMC_smSum->GetBinContent(iBin);
      
      double errUp   = TMath::Sqrt(errUp2MC_smSum[iBin - 1]);      
      double errDown = TMath::Sqrt(errDown2MC_smSum[iBin - 1]);
      std::cout << "variable = " << variable.meName_ << ", bin = " << iBin << ":" 
		<< " value = " << value_central << " + " << errUp << " - " << errDown << std::endl;
      
      // CV: set bin content such that errors in up/down direction become symmetric
      meMC_smErr->SetBinContent(iBin, value_central + 0.5*(errUp - errDown));
      meMC_smErr->SetBinError(iBin, 0.5*(errUp + errDown));

      meMC_smErrUp->SetBinContent(iBin, value_central + errUp);
      meMC_smErrDown->SetBinContent(iBin, value_central - errUp);
    }
    meMC_smErr->SetMarkerStyle(0);
    meMC_smErr->SetMarkerSize(0);
    meMC_smErr->SetLineColor(13);
    meMC_smErr->SetLineWidth(0);
    meMC_smErr->SetFillColor(13);
    meMC_smErr->SetFillStyle(3001);
  }

  THStack stack_smSum("smSum", "smSum");
  stack_smSum.Add(meMC_signal_cloned);
  if ( meMC_bgrSum ) stack_smSum.Add(meMC_bgrSum);

  TPad* topPad = 0;
  TPad* bottomPad = 0;
  if ( drawDataToMCratio ) {
    topPad = new TPad("topPad", "topPad", 0.00, 0.35, 1.00, 1.00);
    topPad->SetFillColor(10);
    topPad->SetTopMargin(0.04);
    topPad->SetLeftMargin(0.15);
    topPad->SetBottomMargin(0.03);
    topPad->SetRightMargin(0.05);

    bottomPad = new TPad("bottomPad", "bottomPad", 0.00, 0.00, 1.00, 0.35);
    bottomPad->SetFillColor(10);
    bottomPad->SetTopMargin(0.02);
    bottomPad->SetLeftMargin(0.15);
    bottomPad->SetBottomMargin(0.20);
    bottomPad->SetRightMargin(0.05);

    canvas->cd();
    topPad->Draw();
    topPad->cd();
    topPad->SetLogy(useLogScaleY);
  } else {
    canvas->SetLogy(useLogScaleY);
  }

  if ( useLogScaleY ) {
    stack_smSum.SetMaximum(7.5e1*TMath::Max(meData->GetMaximum(), stack_smSum.GetMaximum()));
    stack_smSum.SetMinimum(5.e-1);
  } else {
    stack_smSum.SetMaximum(1.55*TMath::Max(meData->GetMaximum(), stack_smSum.GetMaximum()));
    stack_smSum.SetMinimum(0.);
  }
  stack_smSum.SetTitle("");

  stack_smSum.Draw("hist");
  // CV: x-axis and y-axis of THStack do not exist until THStack::Draw is called,
  //     so need to postpone initialization of axes titles !!
  TAxis* xAxis_top = stack_smSum.GetXaxis();
  xAxis_top->SetTitle(variable.xAxisTitle_.data());
  xAxis_top->SetTitleOffset(1.2);
  if ( drawDataToMCratio ) {
    xAxis_top->SetLabelColor(10);
    xAxis_top->SetTitleColor(10);
  }

  TAxis* yAxis_top = stack_smSum.GetYaxis();
  yAxis_top->SetTitle("Events");
  yAxis_top->SetTitleOffset(1.4);

  if ( drawSysUncertainty && meMC_smErr ) meMC_smErr->Draw("e2same");

  meData->Draw("e1psame");
  
  double legendPosX = ( drawDataToMCratio ) ? 0.715 : 0.65;
  TLegend legend(0.185, legendPosX, 0.52, legendPosX + 0.24, "", "brNDC"); 
  legend.SetBorderSize(0);
  legend.SetFillColor(0);
  legend.AddEntry(meData, std::string(runPeriod).append(" Data").data(), "p");
  //legend.AddEntry(meMC_signal, "exp. Signal", "l");
  legend.AddEntry(meMC_signal, "exp. Z #rightarrow #mu^{+}#mu^{-}", "l");
  if ( meMC_bgrSum ) legend.AddEntry(meMC_bgrSum, "exp. Background", "f");
  if ( drawSysUncertainty && meMC_smErr ) legend.AddEntry(meMC_smErr, "Sys. Uncertainty", "f");
  legend.Draw();
  
  double statsSizeX = ( meMC_smErrUp && meMC_smErrDown ) ? 0.30 : 0.26;
  double statsPosX = ( drawDataToMCratio ) ? 0.94 : 0.89;
  statsPosX -= statsSizeX;
  double statsPosY = ( drawDataToMCratio ) ? 0.84 : 0.79;
  TPaveText statsData(statsPosX, statsPosY, statsPosX + statsSizeX, statsPosY + 0.10, "brNDC"); 
  statsData.SetBorderSize(0);
  statsData.SetFillColor(0);
   std::cout << "Data: mean = " << meData->GetMean() << std::endl;
  statsData.AddText(Form("Mean = %2.2f", meData->GetMean()));
  statsData.AddText(Form("RMS  = %2.2f", meData->GetRMS()));
  statsData.SetTextColor(1);
  statsData.SetTextSize(0.045);
  statsData.Draw();

  TPaveText statsMC(statsPosX, statsPosY - 0.11, statsPosX + statsSizeX, statsPosY - 0.01, "brNDC"); 
  statsMC.SetBorderSize(0);
  statsMC.SetFillColor(0);
  if ( drawSysUncertainty && meMC_smErrUp && meMC_smErrDown ) {
    double mcMeanErr = TMath::Sqrt(square(meMC_smSum->GetMean() - meMC_smErrUp->GetMean()) 
                                 + square(meMC_smSum->GetMean() - meMC_smErrDown->GetMean()));
    std::cout << "MC: mean = " << meMC_smSum->GetMean() << " +/- " << mcMeanErr << std::endl;
    statsMC.AddText(Form("Mean = %2.2f #pm %2.2f", meMC_smSum->GetMean(), mcMeanErr));
    double mcRMSerr = TMath::Sqrt(square(meMC_smSum->GetRMS() - meMC_smErrUp->GetRMS()) 
				+ square(meMC_smSum->GetRMS() - meMC_smErrDown->GetRMS()));
    statsMC.AddText(Form("RMS  = %2.2f #pm %2.2f", meMC_smSum->GetRMS(), mcRMSerr));
  } else {
    std::cout << "MC: mean = " << meMC_smSum->GetMean() << std::endl;
    statsMC.AddText(Form("Mean = %2.2f", meMC_smSum->GetMean()));
    statsMC.AddText(Form("RMS  = %2.2f", meMC_smSum->GetRMS()));
  }
  statsMC.SetTextColor(2);
  statsMC.SetTextSize(0.045);
  statsMC.Draw();

  TH1* dummyHistogram_bottom = 0;
  TGraphErrors* graphDataToMCdiff = 0;
  if ( drawDataToMCratio ) {
    canvas->cd();
    bottomPad->Draw();
    bottomPad->cd();
    bottomPad->SetLogy(false);
    
    dummyHistogram_bottom = (TH1*)meData->Clone("dummyHistogram_bottom");
    dummyHistogram_bottom->SetTitle("");
    dummyHistogram_bottom->SetStats(false);  
    
    assert(meMC_smSum->GetNbinsX() == meData->GetNbinsX());
    int numBins = meMC_smSum->GetNbinsX();
    graphDataToMCdiff = new TGraphErrors(numBins);
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
      diffErr2 += square(yErr_smSum/y_smSum);
      diffErr2 *= square(y_data/y_smSum);
      
      graphDataToMCdiff->SetPoint(iBin - 1, x, diff);
      graphDataToMCdiff->SetPointError(iBin - 1, 0., TMath::Sqrt(diffErr2));
    }
    
    TGraphErrors* graphMCerr = 0;
    if ( meMC_smErr ) {
      graphMCerr = new TGraphErrors(numBins);
      for ( int iBin = 1; iBin <= numBins; ++iBin ) {
	double x = meMC_smSum->GetBinCenter(iBin);
	
	if ( !(meMC_smSum->GetBinContent(iBin) > 0.) ) continue;
	double rel = (meMC_smErr->GetBinContent(iBin)/meMC_smSum->GetBinContent(iBin)) - 1.;
	double relErr = meMC_smErr->GetBinError(iBin)/meMC_smSum->GetBinContent(iBin);
	//std::cout << "variable = " << variable.meName_ << ", bin = " << iBin << ":" 
	//	  << " rel. = " << rel << " +/- " << relErr << std::endl;
	
	graphMCerr->SetPoint(iBin - 1, x, rel);
	graphMCerr->SetPointError(iBin - 1, 0.5*meMC_smSum->GetBinWidth(iBin), relErr);
      }
    }
    
    TAxis* xAxis_bottom = dummyHistogram_bottom->GetXaxis();
    xAxis_bottom->SetTitle(variable.xAxisTitle_.data());
    xAxis_bottom->SetTitleOffset(1.20);
    xAxis_bottom->SetNdivisions(505);
    xAxis_bottom->SetTitleOffset(1.1);
    xAxis_bottom->SetTitleSize(0.08);
    xAxis_bottom->SetLabelOffset(0.02);
    xAxis_bottom->SetLabelSize(0.08);
    xAxis_bottom->SetTickLength(0.055);
    
    TAxis* yAxis_bottom = dummyHistogram_bottom->GetYaxis();
    yAxis_bottom->SetTitle("#frac{Data - Simulation}{Simulation}");
    yAxis_bottom->SetTitleOffset(1.10);
    yAxis_bottom->CenterTitle();
    yAxis_bottom->SetTitleOffset(0.9);
    yAxis_bottom->SetTitleSize(0.08);
    yAxis_bottom->SetLabelSize(0.08);
    yAxis_bottom->SetTickLength(0.04);
    
    double maxDiff = 0.;    
    for ( int iPoint = 0; iPoint < numBins; ++iPoint ) {
      double x, diff;
      graphDataToMCdiff->GetPoint(iPoint, x, diff);
      double err = graphDataToMCdiff->GetErrorY(iPoint);
      diff = TMath::Max(TMath::Abs(diff + err), TMath::Abs(diff - err));
      if ( diff > maxDiff ) maxDiff = diff;
    }
    double maxDiff01 = 0.1*TMath::Ceil(1.2*maxDiff*10.);
    //dummyHistogram_bottom->SetMaximum(+maxDiff01);
    //dummyHistogram_bottom->SetMinimum(-maxDiff01);
    dummyHistogram_bottom->SetMaximum(+1.5);
    dummyHistogram_bottom->SetMinimum(-1.5);
    
    dummyHistogram_bottom->Draw("axis");
    
    if ( drawSysUncertainty && graphMCerr ) {
      graphMCerr->SetMarkerStyle(meMC_smErr->GetMarkerStyle());
      graphMCerr->SetMarkerSize(meMC_smErr->GetMarkerSize());
      graphMCerr->SetLineColor(meMC_smErr->GetLineColor());
      graphMCerr->SetLineWidth(meMC_smErr->GetLineWidth());
      graphMCerr->SetFillColor(meMC_smErr->GetFillColor());
      graphMCerr->SetFillStyle(meMC_smErr->GetFillStyle());
      graphMCerr->Draw("2"); 
    }
    
    graphDataToMCdiff->SetMarkerStyle(meData->GetMarkerStyle());
    graphDataToMCdiff->SetMarkerColor(meData->GetMarkerColor());
    graphDataToMCdiff->SetLineColor(meData->GetLineColor());
    graphDataToMCdiff->Draw("P");   
  }

  canvas->Update();

  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  outputFileName_plot.append("_").append(variable.meName_);
  if ( scaleMCtoData ) outputFileName_plot.append("_scaled");
  if ( useLogScaleY ) outputFileName_plot.append("_log");
  else outputFileName_plot.append("_linear");
  if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_plot).append(std::string(outputFileName, idx)).data());
  canvas->Print(std::string(outputFileName_plot).append(".png").data());
  canvas->Print(std::string(outputFileName_plot).append(".pdf").data());
  
  if ( meMC_signal_cloned != meMC_signal ) delete meMC_signal_cloned;
  delete graphDataToMCdiff;
  delete topPad;
  delete dummyHistogram_bottom;
  delete bottomPad;
  delete canvas;
}	     

struct plotUvsQtNumObjType
{
  plotUvsQtNumObjType(TFile* inputFile, const std::string& numObjLabel, int numObjMin, int numObjMax,
		      const std::string& numObjLabel_subscript,
		      const std::string& directoryData, const std::string& directoryMC,
		      const std::map<std::string, double>& mcScaleFactors, const std::string& runPeriod,
		      const vstring& sysShiftsUp, const vstring& sysShiftsDown)
    : numObjMin_(numObjMin),
      numObjMax_(numObjMax)
  {
    if      ( numObjMin_ == -1 &&
	      numObjMax_ == -1         ) meLabel_ = "";
    else if ( numObjMin_ == -1         ) meLabel_ = Form("%sLe%i",   numObjLabel.data(), numObjMax_);
    else if ( numObjMax_ == -1         ) meLabel_ = Form("%sGe%i",   numObjLabel.data(), numObjMin_);
    else if ( numObjMin_ == numObjMax_ ) meLabel_ = Form("%sEq%i",   numObjLabel.data(), numObjMin_);
    else                                 meLabel_ = Form("%s%ito%i", numObjLabel.data(), numObjMin_, numObjMax_);

    meUparlDivQtVsQtData_ = dynamic_cast<TH2*>(
      loadHistogram(inputFile, directoryData, mcScaleFactors, TString("uParlDivQtVsQt").Append(meLabel_).Data()));
    meUparlVsQtData_ = dynamic_cast<TH2*>(
      loadHistogram(inputFile, directoryData, mcScaleFactors, TString("uParlVsQt").Append(meLabel_).Data()));
    meUperpVsQtData_ = dynamic_cast<TH2*>(
      loadHistogram(inputFile, directoryData, mcScaleFactors, TString("uPerpVsQt").Append(meLabel_).Data()));
    meQtData_ = dynamic_cast<TH1*>(
      loadHistogram(inputFile, directoryData, mcScaleFactors, TString("qT").Append(meLabel_).Data()));

    graphUparlResponseData_ = makeGraph_uParl_div_qT(
      "graph_uParl_div_qT_mean_data", "<-u_{#parallel} >/q_{T} as function of q_{T}",
      meUparlVsQtData_, meQtData_);
    graphUparlResolutionData_ = makeGraph_rms(
      "graph_uParl_rms_data", "RMS(u_{#parallel} ) as function of q_{T}", 
      meUparlVsQtData_, meQtData_);
    graphUperpResolutionData_ = makeGraph_rms(
      "graph_uPerp_rms_data", "RMS(u_{#perp}  ) as function of q_{T}", 
      meUperpVsQtData_, meQtData_);

    meUparlDivQtVsQtMC_ = dynamic_cast<TH2*>(
      loadHistogram(inputFile, directoryMC, mcScaleFactors, TString("uParlDivQtVsQt").Append(meLabel_).Data()));
    meUparlVsQtMC_ = dynamic_cast<TH2*>(
      loadHistogram(inputFile, directoryMC, mcScaleFactors, TString("uParlVsQt").Append(meLabel_).Data()));
    meUperpVsQtMC_ = dynamic_cast<TH2*>(
      loadHistogram(inputFile, directoryMC, mcScaleFactors, TString("uPerpVsQt").Append(meLabel_).Data()));
    meQtMC_ = dynamic_cast<TH1*>(
      loadHistogram(inputFile, directoryMC, mcScaleFactors, TString("qT").Append(meLabel_).Data()));

    graphUparlResponseMC_ = makeGraph_uParl_div_qT(
      "graph_uParl_div_qT_mean_mc", "<-u_{#parallel} >/q_{T} as function of q_{T}",
      meUparlVsQtMC_, meQtMC_);
    graphUparlResolutionMC_ = makeGraph_rms(
      "graph_uParl_rms_mc", "RMS(u_{#parallel} ) as function of q_{T}", 
      meUparlVsQtMC_, meQtMC_);
    graphUperpResolutionMC_ = makeGraph_rms(
      "graph_uPerp_rms_mc", "RMS(u_{#perp}  ) as function of q_{T}", 
      meUperpVsQtMC_, meQtMC_);
    
    assert(sysShiftsUp.size() == sysShiftsDown.size());
    int numSysShifts = sysShiftsUp.size();
    for ( int iSysShift = 0; iSysShift < numSysShifts; ++iSysShift ) {
      sysUncertainties_.push_back(sysShiftsUp[iSysShift]);
      sysUncertainties_.push_back(sysShiftsDown[iSysShift]);
    }
    
    for ( vstring::const_iterator sysUncertainty = sysUncertainties_.begin();
	  sysUncertainty != sysUncertainties_.end(); ++sysUncertainty ) {
      std::string directoryMCsysUncertainty = getDirectorySysErr(directoryMC, *sysUncertainty);
      TH2* meUparlDivQtVsQtMCsysUncertainty_sysUncertainty = dynamic_cast<TH2*>(
	loadHistogram(inputFile, directoryMCsysUncertainty,mcScaleFactors, TString("uParlDivQtVsQt").Append(meLabel_).Data()));
      meUparlDivQtVsQtMCsysUncertainty_.push_back(meUparlDivQtVsQtMCsysUncertainty_sysUncertainty);
      TH2* meUparlVsQtMCsysUncertainty_sysUncertainty = dynamic_cast<TH2*>(
        loadHistogram(inputFile, directoryMCsysUncertainty, mcScaleFactors, TString("uParlVsQt").Append(meLabel_).Data()));
      meUparlVsQtMCsysUncertainty_.push_back(meUparlVsQtMCsysUncertainty_sysUncertainty);
      TH2* meUperpVsQtMCsysUncertainty_sysUncertainty = dynamic_cast<TH2*>(
        loadHistogram(inputFile, directoryMCsysUncertainty, mcScaleFactors, TString("uPerpVsQt").Append(meLabel_).Data()));
      meUperpVsQtMCsysUncertainty_.push_back(meUperpVsQtMCsysUncertainty_sysUncertainty);
      TH1* meQtMCsysUncertainty_sysUncertainty = dynamic_cast<TH1*>(
        loadHistogram(inputFile, directoryMCsysUncertainty, mcScaleFactors, TString("qT").Append(meLabel_).Data()));
      meQtMCsysUncertainty_.push_back(meQtMCsysUncertainty_sysUncertainty);

      graphUparlResponseMCsysUncertainty_.push_back(makeGraph_uParl_div_qT(
	Form("graph_uParl_div_qT_mean_mc_%s", sysUncertainty->data()), "<-u_{#parallel} >/q_{T} as function of q_{T}",
        meUparlVsQtMCsysUncertainty_sysUncertainty, meQtMCsysUncertainty_sysUncertainty));
      graphUparlResolutionMCsysUncertainty_.push_back(makeGraph_rms(
        Form("graph_uParl_rms_mc_%s", sysUncertainty->data()), "RMS(u_{#parallel} ) as function of q_{T}", 
        meUparlVsQtMCsysUncertainty_sysUncertainty, meQtMCsysUncertainty_sysUncertainty));
      graphUperpResolutionMCsysUncertainty_.push_back(makeGraph_rms(
        Form("graph_uPerp_rms_mc_%s", sysUncertainty->data()), "RMS(u_{#perp}  ) as function of q_{T}", 
        meUperpVsQtMCsysUncertainty_sysUncertainty, meQtMCsysUncertainty_sysUncertainty));
    }

    if      ( numObjMin_ == -1 && 
	      numObjMax_ == -1         ) plotLabel_ = ""; 
    else if ( numObjMin_ == -1         ) plotLabel_ = Form("N_{%s} < %i",      numObjLabel_subscript.data(), numObjMax_);
    else if ( numObjMax_ == -1         ) plotLabel_ = Form("N_{%s} > %i",      numObjLabel_subscript.data(), numObjMin_);
    else if ( numObjMin_ == numObjMax_ ) plotLabel_ = Form("N_{%s} = %i",      numObjLabel_subscript.data(), numObjMin_);
    else                                 plotLabel_ = Form("%i < N_{%s} < %i", numObjMin_, numObjLabel_subscript.data(), numObjMax_);

    legendEntryData_ = std::string(runPeriod).append(" Data");
    legendEntryMC_   = std::string("Simulation");
  }
  ~plotUvsQtNumObjType() {}

  int numObjMin_;
  int numObjMax_;

  std::string meLabel_;						    

  TH2* meUparlDivQtVsQtData_;
  TH2* meUparlVsQtData_;
  TH2* meUperpVsQtData_;
  TH1* meQtData_;

  TGraphAsymmErrors* graphUparlResponseData_;
  TGraphAsymmErrors* graphUparlResolutionData_;
  TGraphAsymmErrors* graphUperpResolutionData_;

  TH2* meUparlDivQtVsQtMC_;
  TH2* meUparlVsQtMC_;
  TH2* meUperpVsQtMC_;
  TH1* meQtMC_;

  TGraphAsymmErrors* graphUparlResponseMC_;
  TGraphAsymmErrors* graphUparlResolutionMC_;
  TGraphAsymmErrors* graphUperpResolutionMC_;

  vstring sysUncertainties_;

  std::vector<TH2*> meUparlDivQtVsQtMCsysUncertainty_; 
  std::vector<TH2*> meUparlVsQtMCsysUncertainty_;
  std::vector<TH2*> meUperpVsQtMCsysUncertainty_;
  std::vector<TH1*> meQtMCsysUncertainty_;

  std::vector<TGraphAsymmErrors*> graphUparlResponseMCsysUncertainty_;
  std::vector<TGraphAsymmErrors*> graphUparlResolutionMCsysUncertainty_;
  std::vector<TGraphAsymmErrors*> graphUperpResolutionMCsysUncertainty_;

  std::string plotLabel_;

  std::string legendEntryData_;
  std::string legendEntryMC_;
};

void fitAndMakeControlPlots(plotUvsQtNumObjType* plotUvsQtNumObj, const std::string& outputFileName)
{
  const double xMin = 0.;
  const double xMax = 300.;

  TF1* f_uParl_div_qT_mean_data = fitGraph_uParl_div_qT("f_uParl_div_qT_mean_data", plotUvsQtNumObj->graphUparlResponseData_, xMin, xMax);
  TF1* f_uParl_rms_data = fitGraph_uParl_rms("f_uParl_rms_data", plotUvsQtNumObj->graphUparlResolutionData_, xMin, xMax);
  TF1* f_uPerp_rms_data = fitGraph_uPerp_rms("f_uPerp_rms_data", plotUvsQtNumObj->graphUperpResolutionData_, xMin, xMax);

  TF1* f_uParl_div_qT_mean_mc = fitGraph_uParl_div_qT("f_uParl_div_qT_mean_mc", plotUvsQtNumObj->graphUparlResponseMC_, xMin, xMax);
  TF1* f_uParl_rms_mc = fitGraph_uParl_rms("f_uParl_rms_mc", plotUvsQtNumObj->graphUparlResolutionMC_, xMin, xMax);
  TF1* f_uPerp_rms_mc = fitGraph_uPerp_rms("f_uPerp_rms_mc", plotUvsQtNumObj->graphUperpResolutionMC_, xMin, xMax);
  std::vector<TF1*> f_uParl_div_qT_mean_mcSysUncertainties;
  std::vector<TF1*> f_uParl_rms_mcSysUncertainties;
  std::vector<TF1*> f_uPerp_rms_mcSysUncertainties;
  int numSysUncertainties = plotUvsQtNumObj->sysUncertainties_.size();
  for ( int iSysUncertainty = 0; iSysUncertainty < numSysUncertainties; ++iSysUncertainty ) {
    const std::string& sysUncertainty = plotUvsQtNumObj->sysUncertainties_[iSysUncertainty];
    f_uParl_div_qT_mean_mcSysUncertainties.push_back(fitGraph_uParl_div_qT(
      std::string("f_uParl_div_qT_mean_mc").append("_").append(sysUncertainty), 
      plotUvsQtNumObj->graphUparlResponseMCsysUncertainty_[iSysUncertainty], xMin, xMax));
    f_uParl_rms_mcSysUncertainties.push_back(fitGraph_uParl_rms(
      std::string("f_uParl_rms_mc").append("_").append(sysUncertainty),
      plotUvsQtNumObj->graphUparlResolutionMCsysUncertainty_[iSysUncertainty], xMin, xMax));
    f_uPerp_rms_mcSysUncertainties.push_back(fitGraph_uPerp_rms(
      std::string("f_uPerp_rms_mc").append("_").append(sysUncertainty),
      plotUvsQtNumObj->graphUperpResolutionMCsysUncertainty_[iSysUncertainty], xMin, xMax));
  }

  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 900);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);

  TH1* dummyHistogram = new TH1D("dummyHistogram", "dummyHistogram", (xMax - xMin)/10., xMin, xMax);

  drawZllRecoilFitResult(canvas, dummyHistogram, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphUparlResponseData_, f_uParl_div_qT_mean_data,
			  "Data", 0.62, 0.165, false, true, "-u_{#parallel} /q_{T}", 0.4, 1.2, true, 0.10,
			 outputFileName, "uParlResponseFitData");
  drawZllRecoilFitResult(canvas, dummyHistogram, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphUparlResponseMC_, f_uParl_div_qT_mean_mc,
			 "Simulation", 0.62, 0.165, false, true, "-u_{#parallel} /q_{T}", 0.4, 1.2, true, 0.10,
			 outputFileName, "uParlResponseFitMC",
			 &plotUvsQtNumObj->graphUparlResponseMCsysUncertainty_, &f_uParl_div_qT_mean_mcSysUncertainties);  
  drawZllRecoilFitResult(canvas, dummyHistogram, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphUparlResolutionData_, f_uParl_rms_data,
			 "Data",  0.19, 0.62, false, false, "RMS(u_{#parallel} ) / GeV", 0., 50., true, 0.50, 
			 outputFileName, "uParlResolutionFitData");
  drawZllRecoilFitResult(canvas, dummyHistogram, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphUparlResolutionMC_, f_uParl_rms_mc,
			 "Simulation",  0.19, 0.62, false, false, "RMS(u_{#parallel} ) / GeV", 0., 50., true, 0.50, 
			 outputFileName, "uParlResolutionFitMC",
			 &plotUvsQtNumObj->graphUparlResolutionMCsysUncertainty_, &f_uParl_rms_mcSysUncertainties);
  drawZllRecoilFitResult(canvas, dummyHistogram, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphUperpResolutionData_, f_uPerp_rms_data,
			 "Data", 0.19, 0.62, false, false, "RMS(u_{#perp}  ) / GeV", 0., 50., true, 0.50,
			 outputFileName, "uPerpResolutionFitData");
  drawZllRecoilFitResult(canvas, dummyHistogram, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphUperpResolutionMC_, f_uPerp_rms_mc,
			 "Simulation", 0.19, 0.62, false, false, "RMS(u_{#perp}  ) / GeV", 0., 50., true, 0.50,
			 outputFileName, "uPerpResolutionFitMC",
			 &plotUvsQtNumObj->graphUperpResolutionMCsysUncertainty_, &f_uPerp_rms_mcSysUncertainties);

  drawData_vs_MCcomparison(canvas, dummyHistogram, plotUvsQtNumObj->plotLabel_, 
			   plotUvsQtNumObj->graphUparlResponseData_, plotUvsQtNumObj->graphUparlResponseMC_, 
			   "Data", "Simulation", 0.62, 0.165, false, true, "-u_{#parallel} /q_{T}", 0.4, 1.2, true, 0.10,
			   outputFileName, "uParlResponseData_vs_MC",
			   &plotUvsQtNumObj->graphUparlResponseMCsysUncertainty_);
  drawData_vs_MCcomparison(canvas, dummyHistogram, plotUvsQtNumObj->plotLabel_, 
			   plotUvsQtNumObj->graphUparlResolutionData_, plotUvsQtNumObj->graphUparlResolutionMC_, 
			   "Data", "Simulation", 0.19, 0.62, false, false, "RMS(u_{#parallel} ) / GeV", 0., 50., true, 0.25, 
			   outputFileName, "uParlResolutionData_vs_MC",
			   &plotUvsQtNumObj->graphUparlResolutionMCsysUncertainty_);
  drawData_vs_MCcomparison(canvas, dummyHistogram, plotUvsQtNumObj->plotLabel_, 
			   plotUvsQtNumObj->graphUperpResolutionData_, plotUvsQtNumObj->graphUperpResolutionMC_, 
			   "Data", "Simulation", 0.19, 0.62, false, false, "RMS(u_{#perp}  ) / GeV", 0., 50., true, 0.25,
			   outputFileName, "uPerpResolutionData_vs_MC",
			   &plotUvsQtNumObj->graphUperpResolutionMCsysUncertainty_);

  delete f_uParl_div_qT_mean_data;
  delete f_uParl_rms_data;
  delete f_uPerp_rms_data;
  delete f_uParl_div_qT_mean_mc;
  delete f_uParl_rms_mc;
  delete f_uPerp_rms_mc;
  for ( int iSysUncertainty = 0; iSysUncertainty < numSysUncertainties; ++iSysUncertainty ) {
    delete f_uParl_div_qT_mean_mcSysUncertainties[iSysUncertainty];
    delete f_uParl_rms_mcSysUncertainties[iSysUncertainty];
    delete f_uPerp_rms_mcSysUncertainties[iSysUncertainty];
  }

  delete canvas;
  delete dummyHistogram;
}
 
template<typename T>
void deleteAll(std::vector<T*>& objs)
{
  for ( typename std::vector<T*>::iterator it = objs.begin();
	it != objs.end(); ++it ) {
    delete (*it);
  }
}

void fitAndCompare(std::vector<plotUvsQtNumObjType*>& plotUvsQtNumObjs, const std::vector<std::string>& legendEntries,
		   bool drawGraphs, bool drawFits, bool showMC, bool showData,
		   const std::string& outputFileName)
{
  assert(plotUvsQtNumObjs.size() == legendEntries.size());

  const double xMin = 0.;
  const double xMax = 200.;

  std::vector<TGraphAsymmErrors*> graphs_uParl_div_qT_mean_data;
  std::vector<TF1*> fitFunctions_uParl_div_qT_mean_data;
  std::vector<TGraphAsymmErrors*> graphs_uParl_rms_data;
  std::vector<TF1*> fitFunctions_uParl_rms_data;
  std::vector<TGraphAsymmErrors*> graphs_uPerp_rms_data;
  std::vector<TF1*> fitFunctions_uPerp_rms_data;
  
  std::vector<TGraphAsymmErrors*> graphs_uParl_div_qT_mean_mc;
  std::vector<TF1*> fitFunctions_uParl_div_qT_mean_mc;
  std::vector<TGraphAsymmErrors*> graphs_uParl_rms_mc;
  std::vector<TF1*> fitFunctions_uParl_rms_mc;
  std::vector<TGraphAsymmErrors*> graphs_uPerp_rms_mc;
  std::vector<TF1*> fitFunctions_uPerp_rms_mc;

  size_t numObjs = plotUvsQtNumObjs.size();
  for ( size_t iObj = 0; iObj < numObjs; ++iObj ) {
    plotUvsQtNumObjType* plotUvsQtNumObj = plotUvsQtNumObjs[iObj];

    graphs_uParl_div_qT_mean_data.push_back(plotUvsQtNumObj->graphUparlResponseData_);
    fitFunctions_uParl_div_qT_mean_data.push_back(
      fitGraph_uParl_div_qT("f_uParl_div_qT_mean_data", plotUvsQtNumObj->graphUparlResponseData_, xMin, xMax));
    graphs_uParl_rms_data.push_back(plotUvsQtNumObj->graphUparlResolutionData_);
    fitFunctions_uParl_rms_data.push_back(
      fitGraph_uParl_rms("f_uParl_rms_data", plotUvsQtNumObj->graphUparlResolutionData_, xMin, xMax));
    graphs_uPerp_rms_data.push_back(plotUvsQtNumObj->graphUperpResolutionData_);
    fitFunctions_uPerp_rms_data.push_back(
      fitGraph_uPerp_rms("f_uPerp_rms_data", plotUvsQtNumObj->graphUperpResolutionData_, xMin, xMax));

    graphs_uParl_div_qT_mean_mc.push_back(plotUvsQtNumObj->graphUparlResponseMC_);
    fitFunctions_uParl_div_qT_mean_mc.push_back(
      fitGraph_uParl_div_qT("f_uParl_div_qT_mean_mc", plotUvsQtNumObj->graphUparlResponseMC_, xMin, xMax));
    graphs_uParl_rms_mc.push_back(plotUvsQtNumObj->graphUparlResolutionMC_);
    fitFunctions_uParl_rms_mc.push_back(
      fitGraph_uParl_rms("f_uParl_rms_mc", plotUvsQtNumObj->graphUparlResolutionMC_, xMin, xMax));
    graphs_uPerp_rms_mc.push_back(plotUvsQtNumObj->graphUperpResolutionMC_);
    fitFunctions_uPerp_rms_mc.push_back(
      fitGraph_uPerp_rms("f_uPerp_rms_mc", plotUvsQtNumObj->graphUperpResolutionMC_, xMin, xMax));
  }

  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);

  TH1* dummyHistogram = new TH1D("dummyHistogram", "dummyHistogram", (xMax - xMin)/10., xMin, xMax);

  drawZllRecoilFitResult(canvas, dummyHistogram, 
			 graphs_uParl_div_qT_mean_mc, fitFunctions_uParl_div_qT_mean_mc, drawGraphs && showMC, drawFits && showMC,
			 graphs_uParl_div_qT_mean_data, fitFunctions_uParl_div_qT_mean_data, drawGraphs && showData, drawFits && showData,
			 legendEntries, 0.61, 0.165, false, true, "-u_{#parallel} /q_{T}", 0.4, 1.2,
			 outputFileName, "uParlResponseFit");
  drawZllRecoilFitResult(canvas, dummyHistogram, 
			 graphs_uParl_rms_mc, fitFunctions_uParl_rms_mc, drawGraphs && showMC, drawFits && showMC,
			 graphs_uParl_rms_data, fitFunctions_uParl_rms_data, drawGraphs && showData, drawFits && showData,
			 legendEntries, 0.19, 0.62, false, false, "RMS(u_{#parallel} ) / GeV", 0., 50., 
			 outputFileName, "uParlResolutionFit");
  drawZllRecoilFitResult(canvas, dummyHistogram, 
			 graphs_uPerp_rms_mc, fitFunctions_uPerp_rms_mc, drawGraphs && showMC, drawFits && showMC,
			 graphs_uPerp_rms_data, fitFunctions_uPerp_rms_data, drawGraphs && showData, drawFits && showData,
			 legendEntries, 0.19, 0.62, false, false, "RMS(u_{#perp}  ) / GeV", 0., 50.,
			 outputFileName, "uPerpResolutionFit");
  
  deleteAll(fitFunctions_uParl_div_qT_mean_data);
  deleteAll(fitFunctions_uParl_rms_data);
  deleteAll(fitFunctions_uPerp_rms_data);

  deleteAll(fitFunctions_uParl_div_qT_mean_mc);
  deleteAll(fitFunctions_uParl_rms_mc);
  deleteAll(fitFunctions_uPerp_rms_mc);

  delete canvas;
  delete dummyHistogram;
}

std::string getOutputFileName_plot(const std::string& outputFileName, const std::string& plotLabel)
{
  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  outputFileName_plot.append("_").append(plotLabel);
  if ( idx != std::string::npos ) outputFileName_plot.append(std::string(outputFileName, idx));
  return outputFileName_plot;
}
 
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

  std::string runPeriod = cfgMakeZllRecoilCorrectionPlots.getParameter<std::string>("runPeriod");

  std::string directoryData           = cfgMakeZllRecoilCorrectionPlots.getParameter<std::string>("directoryData");
  std::string directoryMC_signal      = cfgMakeZllRecoilCorrectionPlots.getParameter<std::string>("directoryMC_signal");
  vstring     directoryMCs_background = cfgMakeZllRecoilCorrectionPlots.getParameter<vstring>("directoryMCs_background");

  std::map<std::string, double> mcScaleFactors; // key = sample
  edm::ParameterSet cfgScaleFactors = cfgMakeZllRecoilCorrectionPlots.getParameter<edm::ParameterSet>("mcScaleFactors");
  vstring sampleNames = cfgScaleFactors.getParameterNamesForType<double>();
  for ( vstring::const_iterator sampleName = sampleNames.begin();
	sampleName != sampleNames.end(); ++sampleName ) {
    double scaleFactor = cfgScaleFactors.getParameter<double>(*sampleName);
    mcScaleFactors[*sampleName] = scaleFactor;
  }

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
    drawHistogram1d(inputFile, *variable, directoryData, directoryMC_signal, directoryMCs_background, mcScaleFactors, runPeriod,
		    sysShiftsUp, sysShiftsDown, false, true,  true,  true,  outputFileName);
    drawHistogram1d(inputFile, *variable, directoryData, directoryMC_signal, directoryMCs_background, mcScaleFactors, runPeriod,
		    sysShiftsUp, sysShiftsDown, true,  true,  true,  true,  outputFileName);
    drawHistogram1d(inputFile, *variable, directoryData, directoryMC_signal, directoryMCs_background, mcScaleFactors, runPeriod,
		    sysShiftsUp, sysShiftsDown, true,  false, false, false, outputFileName);
  }

//--- make plots of mean(uParl)/qT, rms(uParl)/qT, rms(uPerp)/qT
  plotUvsQtNumObjType* plotUvsQt  = 
    new plotUvsQtNumObjType(inputFile, "", -1, -1, "", 
			    directoryData, directoryMC_signal, mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown);
  fitAndMakeControlPlots(plotUvsQt, outputFileName);

//--- make plots of mean(uParl)/qT, rms(uParl)/qT, rms(uPerp)/qT
//    in different bins of reconstructed jet multiplicity
  plotUvsQtNumObjType* plotUvsQtNumJetsEq0 = 
    new plotUvsQtNumObjType(inputFile, "NumJets", 0,  0, "jet", 
			    directoryData, directoryMC_signal, mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown);
  fitAndMakeControlPlots(plotUvsQtNumJetsEq0, getOutputFileName_plot(outputFileName, plotUvsQtNumJetsEq0->meLabel_));
  plotUvsQtNumObjType* plotUvsQtNumJetsEq1 = 
    new plotUvsQtNumObjType(inputFile, "NumJets", 1,  1, "jet", 
			    directoryData, directoryMC_signal, mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown);
  fitAndMakeControlPlots(plotUvsQtNumJetsEq1, getOutputFileName_plot(outputFileName, plotUvsQtNumJetsEq1->meLabel_));
  plotUvsQtNumObjType* plotUvsQtNumJetsEq2 = 
    new plotUvsQtNumObjType(inputFile, "NumJets", 2,  2, "jet", 
			    directoryData, directoryMC_signal, mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown);
  fitAndMakeControlPlots(plotUvsQtNumJetsEq2, getOutputFileName_plot(outputFileName, plotUvsQtNumJetsEq2->meLabel_));
  plotUvsQtNumObjType* plotUvsQtNumJetsGe3 = 
    new plotUvsQtNumObjType(inputFile, "NumJets", 3, -1, "jet", 
			    directoryData, directoryMC_signal, mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown);
  fitAndMakeControlPlots(plotUvsQtNumJetsGe3, getOutputFileName_plot(outputFileName, plotUvsQtNumJetsGe3->meLabel_));

//--- make plots of mean(uParl)/qT, rms(uParl)/qT and rms(uPerp)/qT
//    in different bins of reconstructed vertex multiplicity (pile-up)
  plotUvsQtNumObjType* plotUvsQtNumVerticesEq1 =
    new plotUvsQtNumObjType(inputFile, "NumVertices",  1,  1, "vertex", 
			    directoryData, directoryMC_signal, mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown);
  plotUvsQtNumObjType* plotUvsQtNumVerticesEq5 =
    new plotUvsQtNumObjType(inputFile, "NumVertices",  5,  5, "vertex", 
			    directoryData, directoryMC_signal, mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown);
  plotUvsQtNumObjType* plotUvsQtNumVerticesEq8 =
    new plotUvsQtNumObjType(inputFile, "NumVertices",  8,  8, "vertex", 
			    directoryData, directoryMC_signal, mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown);
  plotUvsQtNumObjType* plotUvsQtNumVerticesEq11 =
    new plotUvsQtNumObjType(inputFile, "NumVertices", 11, 11, "vertex", 
			    directoryData, directoryMC_signal, mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown);
  std::vector<plotUvsQtNumObjType*> plotUvsQtNumObjs;
  plotUvsQtNumObjs.push_back(plotUvsQtNumVerticesEq1);
  plotUvsQtNumObjs.push_back(plotUvsQtNumVerticesEq5);
  plotUvsQtNumObjs.push_back(plotUvsQtNumVerticesEq8);
  plotUvsQtNumObjs.push_back(plotUvsQtNumVerticesEq11);
  std::vector<std::string> legendEntries;
  legendEntries.push_back("1 Vertex (No PU)");
  legendEntries.push_back("5 Vertices");
  legendEntries.push_back("8 Vertices");
  legendEntries.push_back("11 Vertices");
  fitAndCompare(plotUvsQtNumObjs, legendEntries, true, false, true, false, getOutputFileName_plot(outputFileName, "puDependenceMC"));
  fitAndCompare(plotUvsQtNumObjs, legendEntries, true, false, true, false, getOutputFileName_plot(outputFileName, "puDependenceData"));

  delete inputFile;

//--print time that it took macro to run
  std::cout << "finished executing makeZllRecoilCorrectionFinalPlots macro:" << std::endl;
  clock.Show("makeZllRecoilCorrectionFinalPlots");

  return 0;
}
