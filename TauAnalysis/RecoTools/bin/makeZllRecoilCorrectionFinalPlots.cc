
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
#include <TH3.h>
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
  variableEntryType(const std::string& meName, const std::string& xAxisTitle)
    : meName_(meName),
      xAxisTitle_(xAxisTitle)
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
    if ( isScaleFactorDefined ) {
      me->Scale(scaleFactor);
      //std::cout << key << ": scale-factor = " << scaleFactor << std::endl;
    } else {
      //std::cout << key << ": no scale-factor defined." << std::endl;
    }
    
    histograms[key] = me;
  }
  
  return histograms[key];
}

TH1* loadAndSumHistograms(TFile* inputFile, const vstring& directories, 
			  const std::map<std::string, double>& mcScaleFactors, const std::string& meName)
{
  TH1* me_sum = 0;

  for ( vstring::const_iterator directory = directories.begin();
	directory != directories.end(); ++directory ) {
    TH1* me = loadHistogram(inputFile, *directory, mcScaleFactors, meName);
    if ( !me->GetSumw2N() ) me->Sumw2();
    if ( !me_sum ) {
      me_sum = (TH1*)me->Clone(std::string(me->GetName()).append("_cloned").data());
      if ( !me_sum->GetSumw2N() ) me_sum->Sumw2();
    }
    else me_sum->Add(me);
  }

  return me_sum;
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
  std::cout << " variable = " << variable.meName_ << std::endl;

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

      std::cout << "bin = " << iBin << " (x = " << meMC_central->GetBinCenter(iBin) << "): central = " << value_central << "," 
		<< " " << sysShiftUp << " = " << value_sysShiftUp << ", " << sysShiftDown << " = " << value_sysShiftDown << std::endl;
      
      double value_max = TMath::Max(value_sysShiftUp, value_sysShiftDown);
      if ( value_max > value_central ) errUp2MC_smSum[iBin - 1]   += square(value_max - value_central); 
      double value_min = TMath::Min(value_sysShiftUp, value_sysShiftDown);
      if ( value_central > value_min ) errDown2MC_smSum[iBin - 1] += square(value_central - value_min); 
    }
  }
}

void drawHistogram1d(const variableEntryType& variable, TH1* meData, TH1* meMC_signal, TH1* meMC_bgrSum, const std::string& runPeriod,
		     const std::vector<double>& errUp2MC_smSum, const std::vector<double>& errDown2MC_smSum,
		     bool scaleMCtoData, bool drawDataToMCratio, bool useLogScaleY, bool drawSysUncertainty,
		     const std::string& outputFileName)
{
  int canvasSizeY = ( drawDataToMCratio ) ? 900 : 600;
  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, canvasSizeY);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);

  if ( !meData->GetSumw2N() ) meData->Sumw2();
  meData->SetLineColor(1);
  meData->SetMarkerColor(1);
  meData->SetMarkerStyle(20);

  TH1* meMC_signal_cloned = ( scaleMCtoData ) ?
    (TH1*)meMC_signal->Clone(std::string(meMC_signal->GetName()).append("_cloned").data()) : meMC_signal;
  if ( !meMC_signal_cloned->GetSumw2N() ) meMC_signal_cloned->Sumw2();
  meMC_signal_cloned->SetLineColor(2);
  meMC_signal_cloned->SetLineWidth(2);
  meMC_signal_cloned->SetFillColor(10);

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

  TH1* meMC_smErr = 0;
  TH1* meMC_smErrUp = 0;
  TH1* meMC_smErrDown = 0;
  if ( drawSysUncertainty ) {
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
    stack_smSum.SetMaximum(1.e+2*TMath::Max(meData->GetMaximum(), stack_smSum.GetMaximum()));
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
  if ( useLogScaleY ) yAxis_top->SetTitleOffset(1.4);
  else yAxis_top->SetTitleOffset(1.6);

  if ( drawSysUncertainty && meMC_smErr ) meMC_smErr->Draw("e2same");

  meData->Draw("e1psame");
  
  double legendPosX = ( drawDataToMCratio ) ? 0.715 : 0.65;
  TLegend legend(0.185, legendPosX, 0.52, legendPosX + 0.24, "", "brNDC"); 
  legend.SetBorderSize(0);
  legend.SetFillColor(0);
  legend.AddEntry(meData, std::string(runPeriod).append(" Data").data(), "p");
  legend.AddEntry(meMC_signal, "Sim. Z #rightarrow #mu^{+}#mu^{-}", "l");
  if ( meMC_bgrSum ) legend.AddEntry(meMC_bgrSum, "Sim. Background", "f");
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
  outputFileName_plot.append("_").append(TString(variable.meName_.data()).ReplaceAll("/", "_"));  
  if ( scaleMCtoData ) outputFileName_plot.append("_scaled");
  if ( useLogScaleY ) outputFileName_plot.append("_log");
  else outputFileName_plot.append("_linear");
  if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_plot).append(std::string(outputFileName, idx)).data());
  canvas->Print(std::string(outputFileName_plot).append(".png").data());
  canvas->Print(std::string(outputFileName_plot).append(".pdf").data());
  canvas->Print(std::string(outputFileName_plot).append(".root").data());
  
  if ( meMC_signal_cloned != meMC_signal ) delete meMC_signal_cloned;
  //delete meMC_bgrSum;
  //delete meMC_smSum;
  delete graphDataToMCdiff;
  delete topPad;
  delete dummyHistogram_bottom;
  delete bottomPad;
  delete canvas;
}	     

void drawHistogram1d(TFile* inputFile, const variableEntryType& variable, 
		     const std::string& directoryData, const std::string& directoryMC_signal, const vstring& directoryMCs_background, 
		     const std::map<std::string, double>& mcScaleFactors, const std::string& runPeriod,
		     const vstring& sysShiftsUp, const vstring& sysShiftsDown, 
		     bool scaleMCtoData, bool drawDataToMCratio, bool useLogScaleY, bool drawSysUncertainty,
		     const std::string& outputFileName)
{
  TH1* meData = loadHistogram(inputFile, directoryData, mcScaleFactors, variable.meName_);

  TH1* meMC_signal = loadHistogram(inputFile, directoryMC_signal, mcScaleFactors, variable.meName_);
  TH1* meMC_signal_cloned = ( scaleMCtoData ) ?
    (TH1*)meMC_signal->Clone(std::string(meMC_signal->GetName()).append("_cloned").data()) : meMC_signal;
  if ( !meMC_signal_cloned->GetSumw2N() ) meMC_signal_cloned->Sumw2();

  TH1* meMC_bgrSum = loadAndSumHistograms(inputFile, directoryMCs_background, mcScaleFactors, variable.meName_);
  
  double mcToDataScaleFactor = 1.;
  if ( scaleMCtoData ) {
    mcToDataScaleFactor = meData->Integral()/(meMC_signal->Integral() + meMC_bgrSum->Integral());
    meMC_signal_cloned->Scale(mcToDataScaleFactor);
    meMC_bgrSum->Scale(mcToDataScaleFactor);
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

  drawHistogram1d(variable, meData, meMC_signal, meMC_bgrSum, runPeriod, errUp2MC_smSum, errDown2MC_smSum,
		  scaleMCtoData, drawDataToMCratio, useLogScaleY, drawSysUncertainty, outputFileName);

  if ( meMC_signal_cloned != meMC_signal ) delete meMC_signal_cloned;
  //delete meMC_bgrSum;
}	     

struct plotUvsQtNumObjType
{
  plotUvsQtNumObjType(TFile* inputFile, const std::string& numObjLabel, int numObjMin, int numObjMax,
		      const std::string& numObjLabel_subscript,
		      const std::string& directoryData, 
		      const std::string& directoryMC_signal, const vstring& directoryMCs_background,
		      const std::map<std::string, double>& mcScaleFactors, const std::string& runPeriod,
		      const vstring& sysShiftsUp, const vstring& sysShiftsDown, 
		      bool isCaloMEt)
    : numObjMin_(numObjMin),
      numObjMax_(numObjMax)
  {
    if      ( numObjMin_ == -1 &&
	      numObjMax_ == -1         ) meLabel_ = "";
    else if ( numObjMin_ == -1         ) meLabel_ = Form("%sLe%i",   numObjLabel.data(), numObjMax_);
    else if ( numObjMax_ == -1         ) meLabel_ = Form("%sGe%i",   numObjLabel.data(), numObjMin_);
    else if ( numObjMin_ == numObjMax_ ) meLabel_ = Form("%sEq%i",   numObjLabel.data(), numObjMin_);
    else                                 meLabel_ = Form("%s%ito%i", numObjLabel.data(), numObjMin_, numObjMax_);

    if      ( numObjMin_ == -1 && 
	      numObjMax_ == -1         ) plotLabel_ = ""; 
    else if ( numObjMin_ == -1         ) plotLabel_ = Form("N_{%s} < %i",      numObjLabel_subscript.data(), numObjMax_);
    else if ( numObjMax_ == -1         ) plotLabel_ = Form("N_{%s} > %i",      numObjLabel_subscript.data(), numObjMin_);
    else if ( numObjMin_ == numObjMax_ ) plotLabel_ = Form("N_{%s} = %i",      numObjLabel_subscript.data(), numObjMin_);
    else                                 plotLabel_ = Form("%i < N_{%s} < %i", numObjMin_, numObjLabel_subscript.data(), numObjMax_);
    
    loadHistograms(inputFile, directoryData, directoryMC_signal, directoryMCs_background, 
		   mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown, isCaloMEt);
  }
  plotUvsQtNumObjType(TFile* inputFile, const std::string& label,
		      const std::string& directoryData, 
		      const std::string& directoryMC_signal, const vstring& directoryMCs_background,
		      const std::map<std::string, double>& mcScaleFactors, const std::string& runPeriod,
		      const vstring& sysShiftsUp, const vstring& sysShiftsDown, 
		      bool isCaloMEt)
    : numObjMin_(-1),
      numObjMax_(99)
  {
    meLabel_ = label;
    
    plotLabel_ = ""; 

    loadHistograms(inputFile, directoryData, directoryMC_signal, directoryMCs_background, 
		   mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown, isCaloMEt);
  }
  void loadHistograms(TFile* inputFile, 
		      const std::string& directoryData, 
		      const std::string& directoryMC_signal, const vstring& directoryMCs_background,  
		      const std::map<std::string, double>& mcScaleFactors, const std::string& runPeriod,
		      const vstring& sysShiftsUp, const vstring& sysShiftsDown, 
		      bool isCaloMEt)
  {
    meUparlDivQtVsQtData_ = dynamic_cast<TH2*>(
      loadHistogram(inputFile, directoryData, mcScaleFactors, TString("uParlDivQtVsQt").Append(meLabel_).Data()));
    meUparlVsQtData_ = dynamic_cast<TH2*>(
      loadHistogram(inputFile, directoryData, mcScaleFactors, TString("uParlVsQt").Append(meLabel_).Data()));
    meUperpVsQtData_ = dynamic_cast<TH2*>(
      loadHistogram(inputFile, directoryData, mcScaleFactors, TString("uPerpVsQt").Append(meLabel_).Data()));
    meQtData_ = dynamic_cast<TH1*>(
      loadHistogram(inputFile, directoryData, mcScaleFactors, TString("qT").Append(meLabel_).Data()));
    meMEtXvsSumEtData_ = dynamic_cast<TH2*>(
      loadHistogram(inputFile, directoryData, mcScaleFactors, TString("metXvsSumEt").Append(meLabel_).Data()));
    meMEtYvsSumEtData_ = dynamic_cast<TH2*>(
      loadHistogram(inputFile, directoryData, mcScaleFactors, TString("metYvsSumEt").Append(meLabel_).Data()));
    meSumEtData_ = dynamic_cast<TH1*>(
      loadHistogram(inputFile, directoryData, mcScaleFactors, TString("sumEt").Append(meLabel_).Data()));
    meMEtXvsSumEtExclMuonsData_ = dynamic_cast<TH2*>(
      loadHistogram(inputFile, directoryData, mcScaleFactors, TString("metXvsSumEtExclMuons").Append(meLabel_).Data()));
    meMEtYvsSumEtExclMuonsData_ = dynamic_cast<TH2*>(
      loadHistogram(inputFile, directoryData, mcScaleFactors, TString("metYvsSumEtExclMuons").Append(meLabel_).Data()));
    meSumEtExclMuonsData_ = dynamic_cast<TH1*>(
      loadHistogram(inputFile, directoryData, mcScaleFactors, TString("sumEtExclMuons").Append(meLabel_).Data()));

    graphUparlResponseData_ = makeGraph_uParl_div_qT(
      "graph_uParl_div_qT_mean_data", "<-u_{#parallel} >/q_{T} as function of q_{T}",
      meUparlVsQtData_, meQtData_, isCaloMEt);
    graphUparlResolutionData_ = makeGraph_rms(
      "graph_uParl_rms_data", "RMS(u_{#parallel} ) as function of q_{T}", 
      meUparlVsQtData_, meQtData_);
    graphUperpResolutionData_ = makeGraph_rms(
      "graph_uPerp_rms_data", "RMS(u_{#perp}  ) as function of q_{T}", 
      meUperpVsQtData_, meQtData_);
    graphMEtXvsSumEtData_ = makeGraph_metXorY_vs_sumEt(
      "graphMEtXvsSumEt_data", "E_{X}^{miss} as function of #Sigma E_{T}",
      meMEtXvsSumEtData_, meSumEtData_);
    graphMEtYvsSumEtData_ = makeGraph_metXorY_vs_sumEt(
      "graphMEtYvsSumEt_data", "E_{Y}^{miss} as function of #Sigma E_{T}",
      meMEtYvsSumEtData_, meSumEtData_);
    graphMEtXvsSumEtExclMuonsData_ = makeGraph_metXorY_vs_sumEt(
      "graphMEtXvsSumEtExclMuons_data", "E_{X}^{miss} as function of #Sigma E_{T} (excl. Muons)",
      meMEtXvsSumEtExclMuonsData_, meSumEtExclMuonsData_);
    graphMEtYvsSumEtExclMuonsData_ = makeGraph_metXorY_vs_sumEt(
      "graphMEtYvsSumEtExclMuons_data", "E_{Y}^{miss} as function of #Sigma E_{T} (excl. Muons)",
      meMEtYvsSumEtExclMuonsData_, meSumEtExclMuonsData_);

    meUparlDivQtVsQtMC_signal_ = dynamic_cast<TH2*>(
      loadHistogram(inputFile, directoryMC_signal, mcScaleFactors, TString("uParlDivQtVsQt").Append(meLabel_).Data()));
    meUparlVsQtMC_signal_ = dynamic_cast<TH2*>(
      loadHistogram(inputFile, directoryMC_signal, mcScaleFactors, TString("uParlVsQt").Append(meLabel_).Data()));
    meUperpVsQtMC_signal_ = dynamic_cast<TH2*>(
      loadHistogram(inputFile, directoryMC_signal, mcScaleFactors, TString("uPerpVsQt").Append(meLabel_).Data()));
    meQtMC_signal_ = dynamic_cast<TH1*>(
      loadHistogram(inputFile, directoryMC_signal, mcScaleFactors, TString("qT").Append(meLabel_).Data()));
    meMEtXvsSumEtMC_signal_ = dynamic_cast<TH2*>(
      loadHistogram(inputFile, directoryMC_signal, mcScaleFactors, TString("metXvsSumEt").Append(meLabel_).Data()));
    meMEtYvsSumEtMC_signal_ = dynamic_cast<TH2*>(
      loadHistogram(inputFile, directoryMC_signal, mcScaleFactors, TString("metYvsSumEt").Append(meLabel_).Data()));
    meSumEtMC_signal_ = dynamic_cast<TH1*>(
      loadHistogram(inputFile, directoryMC_signal, mcScaleFactors, TString("sumEt").Append(meLabel_).Data()));
    meMEtXvsSumEtExclMuonsMC_signal_ = dynamic_cast<TH2*>(
      loadHistogram(inputFile, directoryMC_signal, mcScaleFactors, TString("metXvsSumEtExclMuons").Append(meLabel_).Data()));
    meMEtYvsSumEtExclMuonsMC_signal_ = dynamic_cast<TH2*>(
      loadHistogram(inputFile, directoryMC_signal, mcScaleFactors, TString("metYvsSumEtExclMuons").Append(meLabel_).Data()));
    meSumEtExclMuonsMC_signal_ = dynamic_cast<TH1*>(
      loadHistogram(inputFile, directoryMC_signal, mcScaleFactors, TString("sumEtExclMuons").Append(meLabel_).Data()));

    graphUparlResponseMC_signal_ = makeGraph_uParl_div_qT(
      "graph_uParl_div_qT_mean_mc_signal", "<-u_{#parallel} >/q_{T} as function of q_{T}",
      meUparlVsQtMC_signal_, meQtMC_signal_, isCaloMEt);
    graphUparlResolutionMC_signal_ = makeGraph_rms(
      "graph_uParl_rms_mc_signal", "RMS(u_{#parallel} ) as function of q_{T}", 
      meUparlVsQtMC_signal_, meQtMC_signal_);
    graphUperpResolutionMC_signal_ = makeGraph_rms(
      "graph_uPerp_rms_mc_signal", "RMS(u_{#perp}  ) as function of q_{T}", 
      meUperpVsQtMC_signal_, meQtMC_signal_);
    graphMEtXvsSumEtMC_signal_ = makeGraph_metXorY_vs_sumEt(
      "graphMEtXvsSumEt_mc_signal", "E_{X}^{miss} as function of #Sigma E_{T}",
      meMEtXvsSumEtMC_signal_, meSumEtMC_signal_);
    graphMEtYvsSumEtMC_signal_ = makeGraph_metXorY_vs_sumEt(
      "graphMEtYvsSumEt_mc_signal", "E_{Y}^{miss} as function of #Sigma E_{T}",
      meMEtYvsSumEtMC_signal_, meSumEtMC_signal_);
    graphMEtXvsSumEtExclMuonsMC_signal_ = makeGraph_metXorY_vs_sumEt(
      "graphMEtXvsSumEtExclMuonsMC_mc_signal", "E_{X}^{miss} as function of #Sigma E_{T} (excl. Muons)",
      meMEtXvsSumEtExclMuonsMC_signal_, meSumEtExclMuonsMC_signal_);
    graphMEtYvsSumEtExclMuonsMC_signal_ = makeGraph_metXorY_vs_sumEt(
      "graphMEtYvsSumEtExclMuonsMC_mc_signal", "E_{Y}^{miss} as function of #Sigma E_{T} (excl. Muons)",
      meMEtYvsSumEtExclMuonsMC_signal_, meSumEtExclMuonsMC_signal_);

    vstring directoryMCs;
    directoryMCs.push_back(directoryMC_signal);
    for ( vstring::const_iterator directoryMC_background = directoryMCs_background.begin();
	  directoryMC_background != directoryMCs_background.end(); ++directoryMC_background ) {
      directoryMCs.push_back(*directoryMC_background);
    }

    meUparlDivQtVsQtMC_ = dynamic_cast<TH2*>(
      loadAndSumHistograms(inputFile, directoryMCs, mcScaleFactors, TString("uParlDivQtVsQt").Append(meLabel_).Data()));
    meUparlVsQtMC_ = dynamic_cast<TH2*>(
      loadAndSumHistograms(inputFile, directoryMCs, mcScaleFactors, TString("uParlVsQt").Append(meLabel_).Data()));
    meUperpVsQtMC_ = dynamic_cast<TH2*>(
      loadAndSumHistograms(inputFile, directoryMCs, mcScaleFactors, TString("uPerpVsQt").Append(meLabel_).Data()));
    meQtMC_ = dynamic_cast<TH1*>(
      loadAndSumHistograms(inputFile, directoryMCs, mcScaleFactors, TString("qT").Append(meLabel_).Data()));
    meMEtXvsSumEtMC_ = dynamic_cast<TH2*>(
      loadAndSumHistograms(inputFile, directoryMCs, mcScaleFactors, TString("metXvsSumEt").Append(meLabel_).Data()));
    meMEtYvsSumEtMC_ = dynamic_cast<TH2*>(
      loadAndSumHistograms(inputFile, directoryMCs, mcScaleFactors, TString("metYvsSumEt").Append(meLabel_).Data()));
    meSumEtMC_ = dynamic_cast<TH1*>(
      loadAndSumHistograms(inputFile, directoryMCs, mcScaleFactors, TString("sumEt").Append(meLabel_).Data()));
    meMEtXvsSumEtExclMuonsMC_ = dynamic_cast<TH2*>(
      loadAndSumHistograms(inputFile, directoryMCs, mcScaleFactors, TString("metXvsSumEtExclMuons").Append(meLabel_).Data()));
    meMEtYvsSumEtExclMuonsMC_ = dynamic_cast<TH2*>(
      loadAndSumHistograms(inputFile, directoryMCs, mcScaleFactors, TString("metYvsSumEtExclMuons").Append(meLabel_).Data()));
    meSumEtExclMuonsMC_ = dynamic_cast<TH1*>(
      loadAndSumHistograms(inputFile, directoryMCs, mcScaleFactors, TString("sumEtExclMuons").Append(meLabel_).Data()));

    graphUparlResponseMC_ = makeGraph_uParl_div_qT(
      "graph_uParl_div_qT_mean_mc", "<-u_{#parallel} >/q_{T} as function of q_{T}",
      meUparlVsQtMC_, meQtMC_, isCaloMEt);
    graphUparlResolutionMC_ = makeGraph_rms(
      "graph_uParl_rms_mc", "RMS(u_{#parallel} ) as function of q_{T}", 
      meUparlVsQtMC_, meQtMC_);
    graphUperpResolutionMC_ = makeGraph_rms(
      "graph_uPerp_rms_mc", "RMS(u_{#perp}  ) as function of q_{T}", 
      meUperpVsQtMC_, meQtMC_);
    graphMEtXvsSumEtMC_ = makeGraph_metXorY_vs_sumEt(
      "graphMEtXvsSumEt_mc", "E_{X}^{miss} as function of #Sigma E_{T}",
      meMEtXvsSumEtMC_, meSumEtMC_);
    graphMEtYvsSumEtMC_ = makeGraph_metXorY_vs_sumEt(
      "graphMEtYvsSumEt_mc", "E_{Y}^{miss} as function of #Sigma E_{T}",
      meMEtYvsSumEtMC_, meSumEtMC_);
    graphMEtXvsSumEtExclMuonsMC_ = makeGraph_metXorY_vs_sumEt(
      "graphMEtXvsSumEtExclMuons_mc", "E_{X}^{miss} as function of #Sigma E_{T} (excl. Muons)",
      meMEtXvsSumEtExclMuonsMC_, meSumEtExclMuonsMC_);
    graphMEtYvsSumEtExclMuonsMC_ = makeGraph_metXorY_vs_sumEt(
      "graphMEtYvsSumEtExclMuons_mc", "E_{Y}^{miss} as function of #Sigma E_{T} (excl. Muons)",
      meMEtYvsSumEtExclMuonsMC_, meSumEtExclMuonsMC_);
    
    assert(sysShiftsUp.size() == sysShiftsDown.size());
    int numSysShifts = sysShiftsUp.size();
    for ( int iSysShift = 0; iSysShift < numSysShifts; ++iSysShift ) {
      sysUncertainties_.push_back(sysShiftsUp[iSysShift]);
      sysUncertainties_.push_back(sysShiftsDown[iSysShift]);
    }
    
    for ( vstring::const_iterator sysUncertainty = sysUncertainties_.begin();
	  sysUncertainty != sysUncertainties_.end(); ++sysUncertainty ) {
      std::string directoryMCsysUncertainty_signal = getDirectorySysErr(directoryMC_signal, *sysUncertainty);

      TH2* meUparlDivQtVsQtMCsysUncertainty_signal = dynamic_cast<TH2*>(
	loadHistogram(inputFile, directoryMCsysUncertainty_signal, mcScaleFactors, TString("uParlDivQtVsQt").Append(meLabel_).Data()));
      meUparlDivQtVsQtMCsysUncertainty_signal_.push_back(meUparlDivQtVsQtMCsysUncertainty_signal);
      TH2* meUparlVsQtMCsysUncertainty_signal = dynamic_cast<TH2*>(
        loadHistogram(inputFile, directoryMCsysUncertainty_signal, mcScaleFactors, TString("uParlVsQt").Append(meLabel_).Data()));
      meUparlVsQtMCsysUncertainty_signal_.push_back(meUparlVsQtMCsysUncertainty_signal);
      TH2* meUperpVsQtMCsysUncertainty_signal = dynamic_cast<TH2*>(
        loadHistogram(inputFile, directoryMCsysUncertainty_signal, mcScaleFactors, TString("uPerpVsQt").Append(meLabel_).Data()));
      meUperpVsQtMCsysUncertainty_signal_.push_back(meUperpVsQtMCsysUncertainty_signal);
      TH1* meQtMCsysUncertainty_signal = dynamic_cast<TH1*>(
        loadHistogram(inputFile, directoryMCsysUncertainty_signal, mcScaleFactors, TString("qT").Append(meLabel_).Data()));
      meQtMCsysUncertainty_signal_.push_back(meQtMCsysUncertainty_signal);
      TH2* meMEtXvsSumEtMCsysUncertainty_signal = dynamic_cast<TH2*>(
        loadHistogram(inputFile, directoryMCsysUncertainty_signal, mcScaleFactors, TString("metXvsSumEt").Append(meLabel_).Data()));
      meMEtXvsSumEtMCsysUncertainty_signal_.push_back(meMEtXvsSumEtMCsysUncertainty_signal);
      TH2* meMEtYvsSumEtMCsysUncertainty_signal = dynamic_cast<TH2*>(
        loadHistogram(inputFile, directoryMCsysUncertainty_signal, mcScaleFactors, TString("metYvsSumEt").Append(meLabel_).Data()));
      meMEtYvsSumEtMCsysUncertainty_signal_.push_back(meMEtYvsSumEtMCsysUncertainty_signal);
      TH1* meSumEtMCsysUncertainty_signal = dynamic_cast<TH1*>(
        loadHistogram(inputFile, directoryMCsysUncertainty_signal, mcScaleFactors, TString("sumEt").Append(meLabel_).Data()));
      meSumEtMCsysUncertainty_signal_.push_back(meSumEtMCsysUncertainty_signal);
      TH2* meMEtXvsSumEtExclMuonsMCsysUncertainty_signal = dynamic_cast<TH2*>(
        loadHistogram(inputFile, directoryMCsysUncertainty_signal, mcScaleFactors, TString("metXvsSumEtExclMuons").Append(meLabel_).Data()));
      meMEtXvsSumEtExclMuonsMCsysUncertainty_signal_.push_back(meMEtXvsSumEtExclMuonsMCsysUncertainty_signal);
      TH2* meMEtYvsSumEtExclMuonsMCsysUncertainty_signal = dynamic_cast<TH2*>(
        loadHistogram(inputFile, directoryMCsysUncertainty_signal, mcScaleFactors, TString("metYvsSumEtExclMuons").Append(meLabel_).Data()));
      meMEtYvsSumEtExclMuonsMCsysUncertainty_signal_.push_back(meMEtYvsSumEtExclMuonsMCsysUncertainty_signal);
      TH1* meSumEtExclMuonsMCsysUncertainty_signal = dynamic_cast<TH1*>(
        loadHistogram(inputFile, directoryMCsysUncertainty_signal, mcScaleFactors, TString("sumEtExclMuons").Append(meLabel_).Data()));
      meSumEtExclMuonsMCsysUncertainty_signal_.push_back(meSumEtExclMuonsMCsysUncertainty_signal);

      graphUparlResponseMCsysUncertainty_signal_.push_back(makeGraph_uParl_div_qT(
	Form("graph_uParl_div_qT_mean_mc_signal_%s", sysUncertainty->data()), "<-u_{#parallel} >/q_{T} as function of q_{T}",
        meUparlVsQtMCsysUncertainty_signal, meQtMCsysUncertainty_signal, isCaloMEt));
      graphUparlResolutionMCsysUncertainty_signal_.push_back(makeGraph_rms(
        Form("graph_uParl_rms_mc_signal_%s", sysUncertainty->data()), "RMS(u_{#parallel} ) as function of q_{T}", 
        meUparlVsQtMCsysUncertainty_signal, meQtMCsysUncertainty_signal));
      graphUperpResolutionMCsysUncertainty_signal_.push_back(makeGraph_rms(
        Form("graph_uPerp_rms_mc_signal_%s", sysUncertainty->data()), "RMS(u_{#perp}  ) as function of q_{T}", 
        meUperpVsQtMCsysUncertainty_signal, meQtMCsysUncertainty_signal));
      graphMEtXvsSumEtMCsysUncertainty_signal_.push_back(makeGraph_metXorY_vs_sumEt(
        Form("graphMEtXvsSumEt_mc_signal_%s", sysUncertainty->data()), "E_{X}^{miss} as function of #Sigma E_{T}",
        meMEtXvsSumEtMCsysUncertainty_signal, meSumEtMCsysUncertainty_signal));
      graphMEtYvsSumEtMCsysUncertainty_signal_.push_back(makeGraph_metXorY_vs_sumEt(
        Form("graphMEtYvsSumEt_mc_signal_%s", sysUncertainty->data()), "E_{Y}^{miss} as function of #Sigma E_{T}",
        meMEtYvsSumEtMCsysUncertainty_signal, meSumEtMCsysUncertainty_signal));
      graphMEtXvsSumEtExclMuonsMCsysUncertainty_signal_.push_back(makeGraph_metXorY_vs_sumEt(
        Form("graphMEtXvsSumEtExclMuons_mc_signal_%s", sysUncertainty->data()), "E_{X}^{miss} as function of #Sigma E_{T} (excl. Muons)",
        meMEtXvsSumEtExclMuonsMCsysUncertainty_signal, meSumEtExclMuonsMCsysUncertainty_signal));
      graphMEtYvsSumEtExclMuonsMCsysUncertainty_signal_.push_back(makeGraph_metXorY_vs_sumEt(
        Form("graphMEtYvsSumEtExclMuons_mc_signal_%s", sysUncertainty->data()), "E_{Y}^{miss} as function of #Sigma E_{T} (excl. Muons)",
        meMEtYvsSumEtExclMuonsMCsysUncertainty_signal, meSumEtExclMuonsMCsysUncertainty_signal));

      vstring directoryMCsSysUncertainty;
      for ( vstring::const_iterator directoryMC = directoryMCs.begin();
	    directoryMC != directoryMCs.end(); ++directoryMC ) {
	directoryMCsSysUncertainty.push_back(getDirectorySysErr(*directoryMC, *sysUncertainty));
      }
      
      TH2* meUparlDivQtVsQtMCsysUncertainty = dynamic_cast<TH2*>(
	loadAndSumHistograms(inputFile, directoryMCsSysUncertainty, mcScaleFactors, TString("uParlDivQtVsQt").Append(meLabel_).Data()));
      meUparlDivQtVsQtMCsysUncertainty_.push_back(meUparlDivQtVsQtMCsysUncertainty);
      TH2* meUparlVsQtMCsysUncertainty = dynamic_cast<TH2*>(
        loadAndSumHistograms(inputFile, directoryMCsSysUncertainty, mcScaleFactors, TString("uParlVsQt").Append(meLabel_).Data()));
      meUparlVsQtMCsysUncertainty_.push_back(meUparlVsQtMCsysUncertainty);
      TH2* meUperpVsQtMCsysUncertainty = dynamic_cast<TH2*>(
        loadAndSumHistograms(inputFile, directoryMCsSysUncertainty, mcScaleFactors, TString("uPerpVsQt").Append(meLabel_).Data()));
      meUperpVsQtMCsysUncertainty_.push_back(meUperpVsQtMCsysUncertainty);
      TH1* meQtMCsysUncertainty = dynamic_cast<TH1*>(
        loadAndSumHistograms(inputFile, directoryMCsSysUncertainty, mcScaleFactors, TString("qT").Append(meLabel_).Data()));
      meQtMCsysUncertainty_.push_back(meQtMCsysUncertainty);
      TH2* meMEtXvsSumEtMCsysUncertainty = dynamic_cast<TH2*>(
        loadAndSumHistograms(inputFile, directoryMCsSysUncertainty, mcScaleFactors, TString("metXvsSumEt").Append(meLabel_).Data()));
      meMEtXvsSumEtMCsysUncertainty_.push_back(meMEtXvsSumEtMCsysUncertainty);
      TH2* meMEtYvsSumEtMCsysUncertainty = dynamic_cast<TH2*>(
        loadAndSumHistograms(inputFile, directoryMCsSysUncertainty, mcScaleFactors, TString("metYvsSumEt").Append(meLabel_).Data()));
      meMEtYvsSumEtMCsysUncertainty_.push_back(meMEtYvsSumEtMCsysUncertainty);
      TH1* meSumEtMCsysUncertainty = dynamic_cast<TH1*>(
        loadAndSumHistograms(inputFile, directoryMCsSysUncertainty, mcScaleFactors, TString("sumEt").Append(meLabel_).Data()));
      meSumEtMCsysUncertainty_.push_back(meSumEtMCsysUncertainty);
      TH2* meMEtXvsSumEtExclMuonsMCsysUncertainty = dynamic_cast<TH2*>(
        loadAndSumHistograms(inputFile, directoryMCsSysUncertainty, mcScaleFactors, TString("metXvsSumEtExclMuons").Append(meLabel_).Data()));
      meMEtXvsSumEtExclMuonsMCsysUncertainty_.push_back(meMEtXvsSumEtExclMuonsMCsysUncertainty);
      TH2* meMEtYvsSumEtExclMuonsMCsysUncertainty = dynamic_cast<TH2*>(
        loadAndSumHistograms(inputFile, directoryMCsSysUncertainty, mcScaleFactors, TString("metYvsSumEtExclMuons").Append(meLabel_).Data()));
      meMEtYvsSumEtExclMuonsMCsysUncertainty_.push_back(meMEtYvsSumEtExclMuonsMCsysUncertainty);
      TH1* meSumEtExclMuonsMCsysUncertainty = dynamic_cast<TH1*>(
        loadAndSumHistograms(inputFile, directoryMCsSysUncertainty, mcScaleFactors, TString("sumEtExclMuons").Append(meLabel_).Data()));
      meSumEtExclMuonsMCsysUncertainty_.push_back(meSumEtExclMuonsMCsysUncertainty);

      graphUparlResponseMCsysUncertainty_.push_back(makeGraph_uParl_div_qT(
	Form("graph_uParl_div_qT_mean_mc_%s", sysUncertainty->data()), "<-u_{#parallel} >/q_{T} as function of q_{T}",
        meUparlVsQtMCsysUncertainty, meQtMCsysUncertainty, isCaloMEt));
      graphUparlResolutionMCsysUncertainty_.push_back(makeGraph_rms(
        Form("graph_uParl_rms_mc_%s", sysUncertainty->data()), "RMS(u_{#parallel} ) as function of q_{T}", 
        meUparlVsQtMCsysUncertainty, meQtMCsysUncertainty));
      graphUperpResolutionMCsysUncertainty_.push_back(makeGraph_rms(
        Form("graph_uPerp_rms_mc_%s", sysUncertainty->data()), "RMS(u_{#perp}  ) as function of q_{T}", 
        meUperpVsQtMCsysUncertainty, meQtMCsysUncertainty));
      graphMEtXvsSumEtMCsysUncertainty_.push_back(makeGraph_metXorY_vs_sumEt(
        Form("graphMEtXvsSumEt_mc_%s", sysUncertainty->data()), "E_{X}^{miss} as function of #Sigma E_{T}",
        meMEtXvsSumEtMCsysUncertainty, meSumEtMCsysUncertainty));
      graphMEtYvsSumEtMCsysUncertainty_.push_back(makeGraph_metXorY_vs_sumEt(
        Form("graphMEtYvsSumEt_mc_%s", sysUncertainty->data()), "E_{Y}^{miss} as function of #Sigma E_{T}",
        meMEtYvsSumEtMCsysUncertainty, meSumEtMCsysUncertainty));
      graphMEtXvsSumEtExclMuonsMCsysUncertainty_.push_back(makeGraph_metXorY_vs_sumEt(
        Form("graphMEtXvsSumEtExclMuons_mc_%s", sysUncertainty->data()), "E_{X}^{miss} as function of #Sigma E_{T} (excl. Muons)",
        meMEtXvsSumEtExclMuonsMCsysUncertainty, meSumEtExclMuonsMCsysUncertainty));
      graphMEtYvsSumEtExclMuonsMCsysUncertainty_.push_back(makeGraph_metXorY_vs_sumEt(
        Form("graphMEtYvsSumEtExclMuons_mc_%s", sysUncertainty->data()), "E_{Y}^{miss} as function of #Sigma E_{T} (excl. Muons)",
        meMEtYvsSumEtExclMuonsMCsysUncertainty, meSumEtExclMuonsMCsysUncertainty));
    }

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
  TH2* meMEtXvsSumEtData_;
  TH2* meMEtYvsSumEtData_;
  TH1* meSumEtData_;
  TH2* meMEtXvsSumEtExclMuonsData_;
  TH2* meMEtYvsSumEtExclMuonsData_;
  TH1* meSumEtExclMuonsData_;

  TGraphAsymmErrors* graphUparlResponseData_;
  TGraphAsymmErrors* graphUparlResolutionData_;
  TGraphAsymmErrors* graphUperpResolutionData_;
  TGraphAsymmErrors* graphMEtXvsSumEtData_;
  TGraphAsymmErrors* graphMEtYvsSumEtData_;
  TGraphAsymmErrors* graphMEtXvsSumEtExclMuonsData_;
  TGraphAsymmErrors* graphMEtYvsSumEtExclMuonsData_;

  TH2* meUparlDivQtVsQtMC_signal_;
  TH2* meUparlVsQtMC_signal_;
  TH2* meUperpVsQtMC_signal_;
  TH1* meQtMC_signal_;
  TH2* meMEtXvsSumEtMC_signal_;
  TH2* meMEtYvsSumEtMC_signal_;
  TH1* meSumEtMC_signal_;
  TH2* meMEtXvsSumEtExclMuonsMC_signal_;
  TH2* meMEtYvsSumEtExclMuonsMC_signal_;
  TH1* meSumEtExclMuonsMC_signal_;

  TGraphAsymmErrors* graphUparlResponseMC_signal_;
  TGraphAsymmErrors* graphUparlResolutionMC_signal_;
  TGraphAsymmErrors* graphUperpResolutionMC_signal_;
  TGraphAsymmErrors* graphMEtXvsSumEtMC_signal_;
  TGraphAsymmErrors* graphMEtYvsSumEtMC_signal_;
  TGraphAsymmErrors* graphMEtXvsSumEtExclMuonsMC_signal_;
  TGraphAsymmErrors* graphMEtYvsSumEtExclMuonsMC_signal_;
  
  TH2* meUparlDivQtVsQtMC_;
  TH2* meUparlVsQtMC_;
  TH2* meUperpVsQtMC_;
  TH1* meQtMC_;
  TH2* meMEtXvsSumEtMC_;
  TH2* meMEtYvsSumEtMC_;
  TH1* meSumEtMC_;
  TH2* meMEtXvsSumEtExclMuonsMC_;
  TH2* meMEtYvsSumEtExclMuonsMC_;
  TH1* meSumEtExclMuonsMC_;

  TGraphAsymmErrors* graphUparlResponseMC_;
  TGraphAsymmErrors* graphUparlResolutionMC_;
  TGraphAsymmErrors* graphUperpResolutionMC_;
  TGraphAsymmErrors* graphMEtXvsSumEtMC_;
  TGraphAsymmErrors* graphMEtYvsSumEtMC_;
  TGraphAsymmErrors* graphMEtXvsSumEtExclMuonsMC_;
  TGraphAsymmErrors* graphMEtYvsSumEtExclMuonsMC_;

  vstring sysUncertainties_;

  std::vector<TH2*> meUparlDivQtVsQtMCsysUncertainty_signal_; 
  std::vector<TH2*> meUparlVsQtMCsysUncertainty_signal_;
  std::vector<TH2*> meUperpVsQtMCsysUncertainty_signal_;
  std::vector<TH1*> meQtMCsysUncertainty_signal_;
  std::vector<TH2*> meMEtXvsSumEtMCsysUncertainty_signal_;
  std::vector<TH2*> meMEtYvsSumEtMCsysUncertainty_signal_;
  std::vector<TH1*> meSumEtMCsysUncertainty_signal_;
  std::vector<TH2*> meMEtXvsSumEtExclMuonsMCsysUncertainty_signal_;
  std::vector<TH2*> meMEtYvsSumEtExclMuonsMCsysUncertainty_signal_;
  std::vector<TH1*> meSumEtExclMuonsMCsysUncertainty_signal_;

  std::vector<TGraphAsymmErrors*> graphUparlResponseMCsysUncertainty_signal_;
  std::vector<TGraphAsymmErrors*> graphUparlResolutionMCsysUncertainty_signal_;
  std::vector<TGraphAsymmErrors*> graphUperpResolutionMCsysUncertainty_signal_;
  std::vector<TGraphAsymmErrors*> graphMEtXvsSumEtMCsysUncertainty_signal_;
  std::vector<TGraphAsymmErrors*> graphMEtYvsSumEtMCsysUncertainty_signal_;
  std::vector<TGraphAsymmErrors*> graphMEtXvsSumEtExclMuonsMCsysUncertainty_signal_;
  std::vector<TGraphAsymmErrors*> graphMEtYvsSumEtExclMuonsMCsysUncertainty_signal_;

  std::vector<TH2*> meUparlDivQtVsQtMCsysUncertainty_; 
  std::vector<TH2*> meUparlVsQtMCsysUncertainty_;
  std::vector<TH2*> meUperpVsQtMCsysUncertainty_;
  std::vector<TH1*> meQtMCsysUncertainty_;
  std::vector<TH2*> meMEtXvsSumEtMCsysUncertainty_;
  std::vector<TH2*> meMEtYvsSumEtMCsysUncertainty_;
  std::vector<TH1*> meSumEtMCsysUncertainty_;
  std::vector<TH2*> meMEtXvsSumEtExclMuonsMCsysUncertainty_;
  std::vector<TH2*> meMEtYvsSumEtExclMuonsMCsysUncertainty_;
  std::vector<TH1*> meSumEtExclMuonsMCsysUncertainty_;

  std::vector<TGraphAsymmErrors*> graphUparlResponseMCsysUncertainty_;
  std::vector<TGraphAsymmErrors*> graphUparlResolutionMCsysUncertainty_;
  std::vector<TGraphAsymmErrors*> graphUperpResolutionMCsysUncertainty_;
  std::vector<TGraphAsymmErrors*> graphMEtXvsSumEtMCsysUncertainty_;
  std::vector<TGraphAsymmErrors*> graphMEtYvsSumEtMCsysUncertainty_;
  std::vector<TGraphAsymmErrors*> graphMEtXvsSumEtExclMuonsMCsysUncertainty_;
  std::vector<TGraphAsymmErrors*> graphMEtYvsSumEtExclMuonsMCsysUncertainty_;

  std::string plotLabel_;

  std::string legendEntryData_;
  std::string legendEntryMC_;
};

void fitAndMakeControlPlots(plotUvsQtNumObjType* plotUvsQtNumObj, const std::string& outputFileName)
{
  const double xMin_qT    =    0.;
  const double xMax_qT    =  300.;

  const double xMin_sumEt =    0.;
  const double xMax_sumEt = 3000.;

  int numSysUncertainties = plotUvsQtNumObj->sysUncertainties_.size();

  TF1* f_uParl_div_qT_mean_data = fitGraph_uParl_div_qT("f_uParl_div_qT_mean_data", plotUvsQtNumObj->graphUparlResponseData_, xMin_qT, xMax_qT);
  TF1* f_uParl_rms_data = fitGraph_uParl_rms("f_uParl_rms_data", plotUvsQtNumObj->graphUparlResolutionData_, xMin_qT, xMax_qT);
  TF1* f_uPerp_rms_data = fitGraph_uPerp_rms("f_uPerp_rms_data", plotUvsQtNumObj->graphUperpResolutionData_, xMin_qT, xMax_qT);
  TF1* f_metX_vs_sumEt_data = fitGraph_metXorY_vs_sumEt("f_metX_vs_sumEt_data", plotUvsQtNumObj->graphMEtXvsSumEtData_, xMin_sumEt, xMax_sumEt);
  TF1* f_metY_vs_sumEt_data = fitGraph_metXorY_vs_sumEt("f_metY_vs_sumEt_data", plotUvsQtNumObj->graphMEtYvsSumEtData_, xMin_sumEt, xMax_sumEt);  
  TF1* f_metX_vs_sumEtExclMuons_data = fitGraph_metXorY_vs_sumEt("f_metX_vs_sumEtExclMuons_data", plotUvsQtNumObj->graphMEtXvsSumEtExclMuonsData_, xMin_sumEt, xMax_sumEt);
  TF1* f_metY_vs_sumEtExclMuons_data = fitGraph_metXorY_vs_sumEt("f_metY_vs_sumEtExclMuons_data", plotUvsQtNumObj->graphMEtYvsSumEtExclMuonsData_, xMin_sumEt, xMax_sumEt);  

  TF1* f_uParl_div_qT_mean_mc_signal = fitGraph_uParl_div_qT("f_uParl_div_qT_mean_mc_signal", plotUvsQtNumObj->graphUparlResponseMC_signal_, xMin_qT, xMax_qT);
  TF1* f_uParl_rms_mc_signal = fitGraph_uParl_rms("f_uParl_rms_mc_signal", plotUvsQtNumObj->graphUparlResolutionMC_signal_, xMin_qT, xMax_qT);
  TF1* f_uPerp_rms_mc_signal = fitGraph_uPerp_rms("f_uPerp_rms_mc_signal", plotUvsQtNumObj->graphUperpResolutionMC_signal_, xMin_qT, xMax_qT);
  TF1* f_metX_vs_sumEt_mc_signal = fitGraph_metXorY_vs_sumEt("f_metX_vs_sumEt_mc_signal", plotUvsQtNumObj->graphMEtXvsSumEtMC_signal_, xMin_sumEt, xMax_sumEt);
  TF1* f_metY_vs_sumEt_mc_signal = fitGraph_metXorY_vs_sumEt("f_metY_vs_sumEt_mc_signal", plotUvsQtNumObj->graphMEtYvsSumEtMC_signal_, xMin_sumEt, xMax_sumEt);  
  TF1* f_metX_vs_sumEtExclMuons_mc_signal = fitGraph_metXorY_vs_sumEt("f_metX_vs_sumEtExclMuons_mc_signal", plotUvsQtNumObj->graphMEtXvsSumEtExclMuonsMC_signal_, xMin_sumEt, xMax_sumEt);
  TF1* f_metY_vs_sumEtExclMuons_mc_signal = fitGraph_metXorY_vs_sumEt("f_metY_vs_sumEtExclMuons_mc_signal", plotUvsQtNumObj->graphMEtYvsSumEtExclMuonsMC_signal_, xMin_sumEt, xMax_sumEt);  
  std::vector<TF1*> f_uParl_div_qT_mean_mcSysUncertainties_signal;
  std::vector<TF1*> f_uParl_rms_mcSysUncertainties_signal;
  std::vector<TF1*> f_uPerp_rms_mcSysUncertainties_signal;
  std::vector<TF1*> f_metX_vs_sumEt_mcSysUncertainties_signal;
  std::vector<TF1*> f_metY_vs_sumEt_mcSysUncertainties_signal;
  std::vector<TF1*> f_metX_vs_sumEtExclMuons_mcSysUncertainties_signal;
  std::vector<TF1*> f_metY_vs_sumEtExclMuons_mcSysUncertainties_signal;
  for ( int iSysUncertainty = 0; iSysUncertainty < numSysUncertainties; ++iSysUncertainty ) {
    const std::string& sysUncertainty = plotUvsQtNumObj->sysUncertainties_[iSysUncertainty];
    f_uParl_div_qT_mean_mcSysUncertainties_signal.push_back(fitGraph_uParl_div_qT(
      std::string("f_uParl_div_qT_mean_mc_signal").append("_").append(sysUncertainty), 
      plotUvsQtNumObj->graphUparlResponseMCsysUncertainty_signal_[iSysUncertainty], xMin_qT, xMax_qT));
    f_uParl_rms_mcSysUncertainties_signal.push_back(fitGraph_uParl_rms(
      std::string("f_uParl_rms_mc_signal").append("_").append(sysUncertainty),
      plotUvsQtNumObj->graphUparlResolutionMCsysUncertainty_signal_[iSysUncertainty], xMin_qT, xMax_qT));
    f_uPerp_rms_mcSysUncertainties_signal.push_back(fitGraph_uPerp_rms(
      std::string("f_uPerp_rms_mc_signal").append("_").append(sysUncertainty),
      plotUvsQtNumObj->graphUperpResolutionMCsysUncertainty_signal_[iSysUncertainty], xMin_qT, xMax_qT));
    f_metX_vs_sumEt_mcSysUncertainties_signal.push_back(fitGraph_metXorY_vs_sumEt(
      std::string("f_metX_vs_sumEt_mc_signal").append("_").append(sysUncertainty),
      plotUvsQtNumObj->graphMEtXvsSumEtMCsysUncertainty_signal_[iSysUncertainty], xMin_sumEt, xMax_sumEt));
    f_metY_vs_sumEt_mcSysUncertainties_signal.push_back(fitGraph_metXorY_vs_sumEt(
      std::string("f_metY_vs_sumEt_mc_signal").append("_").append(sysUncertainty),
      plotUvsQtNumObj->graphMEtYvsSumEtMCsysUncertainty_signal_[iSysUncertainty], xMin_sumEt, xMax_sumEt));
    f_metX_vs_sumEtExclMuons_mcSysUncertainties_signal.push_back(fitGraph_metXorY_vs_sumEt(
      std::string("f_metX_vs_sumEtExclMuons_mc_signal").append("_").append(sysUncertainty),
      plotUvsQtNumObj->graphMEtXvsSumEtExclMuonsMCsysUncertainty_signal_[iSysUncertainty], xMin_sumEt, xMax_sumEt));
    f_metY_vs_sumEtExclMuons_mcSysUncertainties_signal.push_back(fitGraph_metXorY_vs_sumEt(
      std::string("f_metY_vs_sumEtExclMuons_mc_signal").append("_").append(sysUncertainty),
      plotUvsQtNumObj->graphMEtYvsSumEtExclMuonsMCsysUncertainty_signal_[iSysUncertainty], xMin_sumEt, xMax_sumEt));
  }

  TF1* f_uParl_div_qT_mean_mc = fitGraph_uParl_div_qT("f_uParl_div_qT_mean_mc", plotUvsQtNumObj->graphUparlResponseMC_, xMin_qT, xMax_qT);
  TF1* f_uParl_rms_mc = fitGraph_uParl_rms("f_uParl_rms_mc", plotUvsQtNumObj->graphUparlResolutionMC_, xMin_qT, xMax_qT);
  TF1* f_uPerp_rms_mc = fitGraph_uPerp_rms("f_uPerp_rms_mc", plotUvsQtNumObj->graphUperpResolutionMC_, xMin_qT, xMax_qT);
  TF1* f_metX_vs_sumEt_mc = fitGraph_metXorY_vs_sumEt("f_metX_vs_sumEt_mc", plotUvsQtNumObj->graphMEtXvsSumEtMC_, xMin_sumEt, xMax_sumEt);
  TF1* f_metY_vs_sumEt_mc = fitGraph_metXorY_vs_sumEt("f_metY_vs_sumEt_mc", plotUvsQtNumObj->graphMEtYvsSumEtMC_, xMin_sumEt, xMax_sumEt);
  TF1* f_metX_vs_sumEtExclMuons_mc = fitGraph_metXorY_vs_sumEt("f_metX_vs_sumEtExclMuons_mc", plotUvsQtNumObj->graphMEtXvsSumEtExclMuonsMC_, xMin_sumEt, xMax_sumEt);
  TF1* f_metY_vs_sumEtExclMuons_mc = fitGraph_metXorY_vs_sumEt("f_metY_vs_sumEtExclMuons_mc", plotUvsQtNumObj->graphMEtYvsSumEtExclMuonsMC_, xMin_sumEt, xMax_sumEt);
  std::vector<TF1*> f_uParl_div_qT_mean_mcSysUncertainties;
  std::vector<TF1*> f_uParl_rms_mcSysUncertainties;
  std::vector<TF1*> f_uPerp_rms_mcSysUncertainties;
  std::vector<TF1*> f_metX_vs_sumEt_mcSysUncertainties;
  std::vector<TF1*> f_metY_vs_sumEt_mcSysUncertainties;
  std::vector<TF1*> f_metX_vs_sumEtExclMuons_mcSysUncertainties;
  std::vector<TF1*> f_metY_vs_sumEtExclMuons_mcSysUncertainties;
  for ( int iSysUncertainty = 0; iSysUncertainty < numSysUncertainties; ++iSysUncertainty ) {
    const std::string& sysUncertainty = plotUvsQtNumObj->sysUncertainties_[iSysUncertainty];
    f_uParl_div_qT_mean_mcSysUncertainties.push_back(fitGraph_uParl_div_qT(
      std::string("f_uParl_div_qT_mean_mc").append("_").append(sysUncertainty), 
      plotUvsQtNumObj->graphUparlResponseMCsysUncertainty_[iSysUncertainty], xMin_qT, xMax_qT));
    f_uParl_rms_mcSysUncertainties.push_back(fitGraph_uParl_rms(
      std::string("f_uParl_rms_mc").append("_").append(sysUncertainty),
      plotUvsQtNumObj->graphUparlResolutionMCsysUncertainty_[iSysUncertainty], xMin_qT, xMax_qT));
    f_uPerp_rms_mcSysUncertainties.push_back(fitGraph_uPerp_rms(
      std::string("f_uPerp_rms_mc").append("_").append(sysUncertainty),
      plotUvsQtNumObj->graphUperpResolutionMCsysUncertainty_[iSysUncertainty], xMin_qT, xMax_qT));
    f_metX_vs_sumEt_mcSysUncertainties.push_back(fitGraph_metXorY_vs_sumEt(
      std::string("f_metX_vs_sumEt_mc").append("_").append(sysUncertainty),
      plotUvsQtNumObj->graphMEtXvsSumEtMCsysUncertainty_[iSysUncertainty], xMin_sumEt, xMax_sumEt));
    f_metY_vs_sumEt_mcSysUncertainties.push_back(fitGraph_metXorY_vs_sumEt(
      std::string("f_metY_vs_sumEt_mc").append("_").append(sysUncertainty),
      plotUvsQtNumObj->graphMEtYvsSumEtMCsysUncertainty_[iSysUncertainty], xMin_sumEt, xMax_sumEt));
    f_metX_vs_sumEtExclMuons_mcSysUncertainties.push_back(fitGraph_metXorY_vs_sumEt(
      std::string("f_metX_vs_sumEtExclMuons_mc").append("_").append(sysUncertainty),
      plotUvsQtNumObj->graphMEtXvsSumEtExclMuonsMCsysUncertainty_[iSysUncertainty], xMin_sumEt, xMax_sumEt));
    f_metY_vs_sumEtExclMuons_mcSysUncertainties.push_back(fitGraph_metXorY_vs_sumEt(
      std::string("f_metY_vs_sumEtExclMuons_mc").append("_").append(sysUncertainty),
      plotUvsQtNumObj->graphMEtYvsSumEtExclMuonsMCsysUncertainty_[iSysUncertainty], xMin_sumEt, xMax_sumEt));
  }

  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 900);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);

  TH1* dummyHistogram_qT = new TH1D("dummyHistogram_qT", "dummyHistogram_qT", (xMax_qT - xMin_qT)/10., xMin_qT, xMax_qT);
  TH1* dummyHistogram_sumEt = new TH1D("dummyHistogram_sumEt", "dummyHistogram_sumEt", (xMax_sumEt - xMin_sumEt)/10., xMin_sumEt, xMax_sumEt);  

  drawZllRecoilFitResult(canvas, dummyHistogram_qT, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphUparlResponseData_, f_uParl_div_qT_mean_data,
			  "Data", 0.62, 0.165, false, true, "<-u_{#parallel} /q_{T}>", 0.4, 1.2, true, 0.10,
			 outputFileName, "uParlResponseFitData");
  drawZllRecoilFitResult(canvas, dummyHistogram_qT, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphUparlResponseMC_signal_, f_uParl_div_qT_mean_mc_signal,
			 "Sim. Z #rightarrow #mu^{+} #mu^{-}", 0.62, 0.165, false, true, "<-u_{#parallel} /q_{T}>", 0.4, 1.2, true, 0.10,
			 outputFileName, "uParlResponseFitMC",
			 &plotUvsQtNumObj->graphUparlResponseMCsysUncertainty_signal_, &f_uParl_div_qT_mean_mcSysUncertainties_signal);  
  drawZllRecoilFitResult(canvas, dummyHistogram_qT, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphUparlResponseMC_, f_uParl_div_qT_mean_mc,
			 "Simulation", 0.62, 0.165, false, true, "<-u_{#parallel} /q_{T}>", 0.4, 1.2, true, 0.10,
			 outputFileName, "uParlResponseFitMC",
			 &plotUvsQtNumObj->graphUparlResponseMCsysUncertainty_, &f_uParl_div_qT_mean_mcSysUncertainties); 

  drawZllRecoilFitResult(canvas, dummyHistogram_qT, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphUparlResolutionMC_, f_uParl_rms_mc_signal,
			 "Sim. Z #rightarrow #mu^{+} #mu^{-}", 0.19, 0.62, false, false, "RMS(u_{#parallel} ) / GeV", 0., 50., true, 0.50, 
			 outputFileName, "uParlResolutionFitMCsignal",
			 &plotUvsQtNumObj->graphUparlResolutionMCsysUncertainty_signal_, &f_uParl_rms_mcSysUncertainties);
  drawZllRecoilFitResult(canvas, dummyHistogram_qT, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphUparlResolutionMC_, f_uParl_rms_mc,
			 "Simulation",  0.19, 0.62, false, false, "RMS(u_{#parallel} ) / GeV", 0., 50., true, 0.50, 
			 outputFileName, "uParlResolutionFitMC",
			 &plotUvsQtNumObj->graphUparlResolutionMCsysUncertainty_, &f_uParl_rms_mcSysUncertainties);
  drawZllRecoilFitResult(canvas, dummyHistogram_qT, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphUperpResolutionData_, f_uPerp_rms_data,
			 "Data", 0.19, 0.62, false, false, "RMS(u_{#perp}  ) / GeV", 0., 50., true, 0.50,
			 outputFileName, "uPerpResolutionFitData");
  drawZllRecoilFitResult(canvas, dummyHistogram_qT, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphUperpResolutionMC_signal_, f_uPerp_rms_mc_signal,
			 "Sim. Z #rightarrow #mu^{+} #mu^{-}", 0.19, 0.62, false, false, "RMS(u_{#perp}  ) / GeV", 0., 50., true, 0.50,
			 outputFileName, "uPerpResolutionFitMCsignal",
			 &plotUvsQtNumObj->graphUperpResolutionMCsysUncertainty_signal_, &f_uPerp_rms_mcSysUncertainties_signal);
  drawZllRecoilFitResult(canvas, dummyHistogram_qT, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphUperpResolutionMC_, f_uPerp_rms_mc,
			 "Simulation",  0.19, 0.62, false, false, "RMS(u_{#perp}  ) / GeV", 0., 50., true, 0.50,
			 outputFileName, "uPerpResolutionFitMC",
			 &plotUvsQtNumObj->graphUperpResolutionMCsysUncertainty_, &f_uPerp_rms_mcSysUncertainties);
  drawZllRecoilFitResult(canvas, dummyHistogram_qT, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphUperpResolutionData_, f_uPerp_rms_data,
			 "Data", 0.19, 0.62, false, false, "RMS(u_{#perp}  ) / GeV", 0., 50., true, 0.50,
			 outputFileName, "uPerpResolutionFitData",
			 &plotUvsQtNumObj->graphUperpResolutionMCsysUncertainty_, &f_uPerp_rms_mcSysUncertainties);
  
  drawZllRecoilFitResult(canvas, dummyHistogram_sumEt, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphMEtXvsSumEtMC_signal_, f_metX_vs_sumEt_mc_signal,
			 "Sim. Z #rightarrow #mu^{+} #mu^{-}", 0.19, 0.62, false, false, "RMS(E_{X}^{miss} ) / GeV", 0., 75., true, 0.50,
			 outputFileName, "metXvsSumEtFitMCsignal",
			 &plotUvsQtNumObj->graphMEtXvsSumEtMCsysUncertainty_signal_, &f_metX_vs_sumEt_mcSysUncertainties_signal);
  drawZllRecoilFitResult(canvas, dummyHistogram_sumEt, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphMEtXvsSumEtMC_, f_metX_vs_sumEt_mc,
			 "Simulation", 0.19, 0.62, false, false, "RMS(E_{X}^{miss} ) / GeV", 0., 75., true, 0.50,
			 outputFileName, "metXvsSumEtFitMC",
			 &plotUvsQtNumObj->graphMEtXvsSumEtMCsysUncertainty_, &f_metX_vs_sumEt_mcSysUncertainties);
  drawZllRecoilFitResult(canvas, dummyHistogram_sumEt, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphMEtXvsSumEtData_, f_metX_vs_sumEt_data,
			 "Data", 0.19, 0.62, false, false, "RMS(E_{X}^{miss} ) / GeV", 0., 75., true, 0.50,
			 outputFileName, "metXvsSumEtFitData");
  drawZllRecoilFitResult(canvas, dummyHistogram_sumEt, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphMEtYvsSumEtMC_signal_, f_metY_vs_sumEt_mc_signal,
			 "Sim. Z #rightarrow #mu^{+} #mu^{-}", 0.19, 0.62, false, false, "RMS(E_{Y}^{miss} ) / GeV", 0., 75., true, 0.50,
			 outputFileName, "metYvsSumEtFitMCsignal",
			 &plotUvsQtNumObj->graphMEtYvsSumEtMCsysUncertainty_signal_, &f_metY_vs_sumEt_mcSysUncertainties_signal);
  drawZllRecoilFitResult(canvas, dummyHistogram_sumEt, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphMEtYvsSumEtMC_, f_metY_vs_sumEt_mc,
			 "Simulation", 0.19, 0.62, false, false, "RMS(E_{Y}^{miss} ) / GeV", 0., 75., true, 0.50,
			 outputFileName, "metYvsSumEtFitMC",
			 &plotUvsQtNumObj->graphMEtYvsSumEtMCsysUncertainty_, &f_metY_vs_sumEt_mcSysUncertainties);
  drawZllRecoilFitResult(canvas, dummyHistogram_sumEt, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphMEtYvsSumEtData_, f_metY_vs_sumEt_data,
			 "Data", 0.19, 0.62, false, false, "RMS(E_{Y}^{miss} ) / GeV", 0., 75., true, 0.50,
			 outputFileName, "metYvsSumEtFitData");

  drawZllRecoilFitResult(canvas, dummyHistogram_sumEt, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphMEtXvsSumEtExclMuonsMC_signal_, f_metX_vs_sumEtExclMuons_mc_signal,
			 "Sim. Z #rightarrow #mu^{+} #mu^{-}", 0.19, 0.62, false, false, "RMS(E_{X}^{miss} ) / GeV", 0., 75., true, 0.50,
			 outputFileName, "metXvsSumEtExclMuonsFitMCsignal",
			 &plotUvsQtNumObj->graphMEtXvsSumEtExclMuonsMCsysUncertainty_signal_, &f_metX_vs_sumEtExclMuons_mcSysUncertainties_signal);
  drawZllRecoilFitResult(canvas, dummyHistogram_sumEt, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphMEtXvsSumEtExclMuonsMC_, f_metX_vs_sumEtExclMuons_mc,
			 "Simulation", 0.19, 0.62, false, false, "RMS(E_{X}^{miss} ) / GeV", 0., 75., true, 0.50,
			 outputFileName, "metXvsSumEtExclMuonsFitMC",
			 &plotUvsQtNumObj->graphMEtXvsSumEtExclMuonsMCsysUncertainty_, &f_metX_vs_sumEtExclMuons_mcSysUncertainties);
  drawZllRecoilFitResult(canvas, dummyHistogram_sumEt, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphMEtXvsSumEtExclMuonsData_, f_metX_vs_sumEtExclMuons_data,
			 "Data", 0.19, 0.62, false, false, "RMS(E_{X}^{miss} ) / GeV", 0., 75., true, 0.50,
			 outputFileName, "metXvsSumEtExclMuonsFitData");
  drawZllRecoilFitResult(canvas, dummyHistogram_sumEt, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphMEtYvsSumEtExclMuonsMC_signal_, f_metY_vs_sumEtExclMuons_mc_signal,
			 "Sim. Z #rightarrow #mu^{+} #mu^{-}", 0.19, 0.62, false, false, "RMS(E_{Y}^{miss} ) / GeV", 0., 75., true, 0.50,
			 outputFileName, "metYvsSumEtFitExclMuonsMCsignal",
			 &plotUvsQtNumObj->graphMEtYvsSumEtExclMuonsMCsysUncertainty_signal_, &f_metY_vs_sumEtExclMuons_mcSysUncertainties_signal);
  drawZllRecoilFitResult(canvas, dummyHistogram_sumEt, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphMEtYvsSumEtExclMuonsMC_, f_metY_vs_sumEtExclMuons_mc,
			 "Simulation", 0.19, 0.62, false, false, "RMS(E_{Y}^{miss} ) / GeV", 0., 75., true, 0.50,
			 outputFileName, "metYvsSumEtExclMuonsFitMC",
			 &plotUvsQtNumObj->graphMEtYvsSumEtExclMuonsMCsysUncertainty_, &f_metY_vs_sumEtExclMuons_mcSysUncertainties);
  drawZllRecoilFitResult(canvas, dummyHistogram_sumEt, plotUvsQtNumObj->plotLabel_, 
			 plotUvsQtNumObj->graphMEtYvsSumEtExclMuonsData_, f_metY_vs_sumEtExclMuons_data,
			 "Data", 0.19, 0.62, false, false, "RMS(E_{Y}^{miss} ) / GeV", 0., 75., true, 0.50,
			 outputFileName, "metYvsSumEtExclMuonsFitData");

  drawData_vs_MCcomparison(canvas, dummyHistogram_qT, plotUvsQtNumObj->plotLabel_, 
			   "Data", plotUvsQtNumObj->graphUparlResponseData_, 
			   "Sim. Z #rightarrow #mu^{+} #mu^{-}", plotUvsQtNumObj->graphUparlResponseMC_signal_,
			   "Simulation", plotUvsQtNumObj->graphUparlResponseMC_, 
			   0.62, 0.165, false, true, "<-u_{#parallel} /q_{T}>", 0.4, 1.2, true, 0.20,
			   outputFileName, "uParlResponseData_vs_MC",
			   &plotUvsQtNumObj->graphUparlResponseMCsysUncertainty_);
  drawData_vs_MCcomparison(canvas, dummyHistogram_qT, plotUvsQtNumObj->plotLabel_, 			   
			   "Data", plotUvsQtNumObj->graphUparlResolutionData_, 
			   "Sim. Z #rightarrow #mu^{+} #mu^{-}", plotUvsQtNumObj->graphUparlResolutionMC_signal_, 
			   "Simulation", plotUvsQtNumObj->graphUparlResolutionMC_, 
			   0.19, 0.62, false, false, "RMS(u_{#parallel} ) / GeV", 0., 50., true, 0.50, 
			   outputFileName, "uParlResolutionData_vs_MC",
			   &plotUvsQtNumObj->graphUparlResolutionMCsysUncertainty_);
  drawData_vs_MCcomparison(canvas, dummyHistogram_qT, plotUvsQtNumObj->plotLabel_, 			   
			   "Data", plotUvsQtNumObj->graphUperpResolutionData_, 
			   "Sim. Z #rightarrow #mu^{+} #mu^{-}", plotUvsQtNumObj->graphUperpResolutionMC_signal_, 
			   "Simulation", plotUvsQtNumObj->graphUperpResolutionMC_, 
			   0.19, 0.62, false, false, "RMS(u_{#perp}  ) / GeV", 0., 50., true, 0.50,
			   outputFileName, "uPerpResolutionData_vs_MC",
			   &plotUvsQtNumObj->graphUperpResolutionMCsysUncertainty_);
  drawData_vs_MCcomparison(canvas, dummyHistogram_sumEt, plotUvsQtNumObj->plotLabel_, 			   
			   "Data", plotUvsQtNumObj->graphMEtXvsSumEtData_, 
			   "Sim. Z #rightarrow #mu^{+} #mu^{-}", plotUvsQtNumObj->graphMEtXvsSumEtMC_signal_, 
			   "Simulation", plotUvsQtNumObj->graphMEtXvsSumEtMC_, 
			   0.19, 0.62, false, false, "RMS(E_{X}^{miss} ) / GeV", 0., 50., true, 0.50,
			   outputFileName, "metXvsSumEtData_vs_MC",
			   &plotUvsQtNumObj->graphMEtXvsSumEtMCsysUncertainty_);
  drawData_vs_MCcomparison(canvas, dummyHistogram_sumEt, plotUvsQtNumObj->plotLabel_, 			   
			   "Data", plotUvsQtNumObj->graphMEtYvsSumEtData_, 
			   "Sim. Z #rightarrow #mu^{+} #mu^{-}", plotUvsQtNumObj->graphMEtYvsSumEtMC_signal_, 
			   "Simulation", plotUvsQtNumObj->graphMEtYvsSumEtMC_, 
			   0.19, 0.62, false, false, "RMS(E_{Y}^{miss} ) / GeV", 0., 50., true, 0.50,
			   outputFileName, "metYvsSumEtData_vs_MC",
			   &plotUvsQtNumObj->graphMEtYvsSumEtMCsysUncertainty_);
  drawData_vs_MCcomparison(canvas, dummyHistogram_sumEt, plotUvsQtNumObj->plotLabel_, 			   
			   "Data", plotUvsQtNumObj->graphMEtXvsSumEtExclMuonsData_, 
			   "Sim. Z #rightarrow #mu^{+} #mu^{-}", plotUvsQtNumObj->graphMEtXvsSumEtExclMuonsMC_signal_, 
			   "Simulation", plotUvsQtNumObj->graphMEtXvsSumEtExclMuonsMC_, 
			   0.19, 0.62, false, false, "RMS(E_{X}^{miss} ) / GeV", 0., 50., true, 0.50,
			   outputFileName, "metXvsSumEtExclMuonsData_vs_MC",
			   &plotUvsQtNumObj->graphMEtXvsSumEtExclMuonsMCsysUncertainty_);
  drawData_vs_MCcomparison(canvas, dummyHistogram_sumEt, plotUvsQtNumObj->plotLabel_, 			   
			   "Data", plotUvsQtNumObj->graphMEtYvsSumEtExclMuonsData_, 
			   "Sim. Z #rightarrow #mu^{+} #mu^{-}", plotUvsQtNumObj->graphMEtYvsSumEtExclMuonsMC_signal_, 
			   "Simulation", plotUvsQtNumObj->graphMEtYvsSumEtExclMuonsMC_, 
			   0.19, 0.62, false, false, "RMS(E_{Y}^{miss} ) / GeV", 0., 50., true, 0.50,
			   outputFileName, "metYvsSumEtExclMuonsData_vs_MC",
			   &plotUvsQtNumObj->graphMEtYvsSumEtExclMuonsMCsysUncertainty_);
  
  delete f_uParl_div_qT_mean_data;
  delete f_uParl_rms_data;
  delete f_uPerp_rms_data;
  delete f_metX_vs_sumEt_data;
  delete f_metY_vs_sumEt_data;
  delete f_metX_vs_sumEtExclMuons_data;
  delete f_metY_vs_sumEtExclMuons_data;
  delete f_uParl_div_qT_mean_mc_signal;
  delete f_uParl_rms_mc_signal;
  delete f_uPerp_rms_mc_signal;
  delete f_metX_vs_sumEt_mc_signal;
  delete f_metY_vs_sumEt_mc_signal;
  delete f_metX_vs_sumEtExclMuons_mc_signal;
  delete f_metY_vs_sumEtExclMuons_mc_signal;  
  delete f_uParl_div_qT_mean_mc;
  delete f_uParl_rms_mc;
  delete f_uPerp_rms_mc;
  delete f_metX_vs_sumEt_mc;
  delete f_metY_vs_sumEt_mc;
  delete f_metX_vs_sumEtExclMuons_mc;
  delete f_metY_vs_sumEtExclMuons_mc;
  for ( int iSysUncertainty = 0; iSysUncertainty < numSysUncertainties; ++iSysUncertainty ) {    
    delete f_uParl_div_qT_mean_mcSysUncertainties_signal[iSysUncertainty];
    delete f_uParl_rms_mcSysUncertainties_signal[iSysUncertainty];
    delete f_uPerp_rms_mcSysUncertainties_signal[iSysUncertainty];
    delete f_metX_vs_sumEt_mcSysUncertainties_signal[iSysUncertainty];
    delete f_metY_vs_sumEt_mcSysUncertainties_signal[iSysUncertainty];
    delete f_metX_vs_sumEtExclMuons_mcSysUncertainties_signal[iSysUncertainty];
    delete f_metY_vs_sumEtExclMuons_mcSysUncertainties_signal[iSysUncertainty];
    delete f_uParl_div_qT_mean_mcSysUncertainties[iSysUncertainty];
    delete f_uParl_rms_mcSysUncertainties[iSysUncertainty];
    delete f_uPerp_rms_mcSysUncertainties[iSysUncertainty];
    delete f_metX_vs_sumEt_mcSysUncertainties[iSysUncertainty];
    delete f_metY_vs_sumEt_mcSysUncertainties[iSysUncertainty];
    delete f_metX_vs_sumEtExclMuons_mcSysUncertainties[iSysUncertainty];
    delete f_metY_vs_sumEtExclMuons_mcSysUncertainties[iSysUncertainty];
  }

  delete canvas;
  delete dummyHistogram_qT;
  delete dummyHistogram_sumEt;
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

  const double xMin_qT    =    0.;
  const double xMax_qT    =  200.;

  const double xMin_sumEt =    0.;
  const double xMax_sumEt = 2500.;

  std::vector<TGraphAsymmErrors*> graphs_uParl_div_qT_mean_data;
  std::vector<TF1*> fitFunctions_uParl_div_qT_mean_data;
  std::vector<TGraphAsymmErrors*> graphs_uParl_rms_data;
  std::vector<TF1*> fitFunctions_uParl_rms_data;
  std::vector<TGraphAsymmErrors*> graphs_uPerp_rms_data;
  std::vector<TF1*> fitFunctions_uPerp_rms_data;
  std::vector<TGraphAsymmErrors*> graphs_metX_vs_sumEt_data;
  std::vector<TF1*> fitFunctions_metX_vs_sumEt_data;
  std::vector<TGraphAsymmErrors*> graphs_metY_vs_sumEt_data;
  std::vector<TF1*> fitFunctions_metY_vs_sumEt_data;
  std::vector<TGraphAsymmErrors*> graphs_metX_vs_sumEtExclMuons_data;
  std::vector<TF1*> fitFunctions_metX_vs_sumEtExclMuons_data;
  std::vector<TGraphAsymmErrors*> graphs_metY_vs_sumEtExclMuons_data;
  std::vector<TF1*> fitFunctions_metY_vs_sumEtExclMuons_data;

  std::vector<TGraphAsymmErrors*> graphs_uParl_div_qT_mean_mc;
  std::vector<TF1*> fitFunctions_uParl_div_qT_mean_mc;
  std::vector<TGraphAsymmErrors*> graphs_uParl_rms_mc;
  std::vector<TF1*> fitFunctions_uParl_rms_mc;
  std::vector<TGraphAsymmErrors*> graphs_uPerp_rms_mc;
  std::vector<TF1*> fitFunctions_uPerp_rms_mc;
  std::vector<TGraphAsymmErrors*> graphs_metX_vs_sumEt_mc;
  std::vector<TF1*> fitFunctions_metX_vs_sumEt_mc;
  std::vector<TGraphAsymmErrors*> graphs_metY_vs_sumEt_mc;
  std::vector<TF1*> fitFunctions_metY_vs_sumEt_mc;
  std::vector<TGraphAsymmErrors*> graphs_metX_vs_sumEtExclMuons_mc;
  std::vector<TF1*> fitFunctions_metX_vs_sumEtExclMuons_mc;
  std::vector<TGraphAsymmErrors*> graphs_metY_vs_sumEtExclMuons_mc;
  std::vector<TF1*> fitFunctions_metY_vs_sumEtExclMuons_mc;

  size_t numObjs = plotUvsQtNumObjs.size();
  for ( size_t iObj = 0; iObj < numObjs; ++iObj ) {
    plotUvsQtNumObjType* plotUvsQtNumObj = plotUvsQtNumObjs[iObj];
    std::cout << " processing bin #" << iObj << ": " << plotUvsQtNumObj->meLabel_ << std::endl;

    graphs_uParl_div_qT_mean_data.push_back(plotUvsQtNumObj->graphUparlResponseData_);
    fitFunctions_uParl_div_qT_mean_data.push_back(
      fitGraph_uParl_div_qT("f_uParl_div_qT_mean_data", plotUvsQtNumObj->graphUparlResponseData_, xMin_qT, xMax_qT));
    graphs_uParl_rms_data.push_back(plotUvsQtNumObj->graphUparlResolutionData_);
    fitFunctions_uParl_rms_data.push_back(
      fitGraph_uParl_rms("f_uParl_rms_data", plotUvsQtNumObj->graphUparlResolutionData_, xMin_qT, xMax_qT));
    graphs_uPerp_rms_data.push_back(plotUvsQtNumObj->graphUperpResolutionData_);
    fitFunctions_uPerp_rms_data.push_back(
      fitGraph_uPerp_rms("f_uPerp_rms_data", plotUvsQtNumObj->graphUperpResolutionData_, xMin_qT, xMax_qT));
    graphs_metX_vs_sumEt_data.push_back(plotUvsQtNumObj->graphMEtXvsSumEtData_);
    fitFunctions_metX_vs_sumEt_data.push_back(
      fitGraph_metXorY_vs_sumEt("f_metX_vs_sumEt_data", plotUvsQtNumObj->graphMEtXvsSumEtData_, xMin_sumEt, xMax_sumEt));
    graphs_metY_vs_sumEt_data.push_back(plotUvsQtNumObj->graphMEtYvsSumEtData_);
    fitFunctions_metY_vs_sumEt_data.push_back(
      fitGraph_metXorY_vs_sumEt("f_metY_vs_sumEt_data", plotUvsQtNumObj->graphMEtYvsSumEtData_, xMin_sumEt, xMax_sumEt));
    graphs_metX_vs_sumEtExclMuons_data.push_back(plotUvsQtNumObj->graphMEtXvsSumEtExclMuonsData_);
    fitFunctions_metX_vs_sumEtExclMuons_data.push_back(
      fitGraph_metXorY_vs_sumEt("f_metX_vs_sumEtExclMuons_data", plotUvsQtNumObj->graphMEtXvsSumEtExclMuonsData_, xMin_sumEt, xMax_sumEt));
    graphs_metY_vs_sumEtExclMuons_data.push_back(plotUvsQtNumObj->graphMEtYvsSumEtExclMuonsData_);
    fitFunctions_metY_vs_sumEtExclMuons_data.push_back(
      fitGraph_metXorY_vs_sumEt("f_metY_vs_sumEtExclMuons_data", plotUvsQtNumObj->graphMEtYvsSumEtExclMuonsData_, xMin_sumEt, xMax_sumEt));

    graphs_uParl_div_qT_mean_mc.push_back(plotUvsQtNumObj->graphUparlResponseMC_);
    fitFunctions_uParl_div_qT_mean_mc.push_back(
      fitGraph_uParl_div_qT("f_uParl_div_qT_mean_mc", plotUvsQtNumObj->graphUparlResponseMC_, xMin_qT, xMax_qT));
    graphs_uParl_rms_mc.push_back(plotUvsQtNumObj->graphUparlResolutionMC_);
    fitFunctions_uParl_rms_mc.push_back(
      fitGraph_uParl_rms("f_uParl_rms_mc", plotUvsQtNumObj->graphUparlResolutionMC_, xMin_qT, xMax_qT));
    graphs_uPerp_rms_mc.push_back(plotUvsQtNumObj->graphUperpResolutionMC_);
    fitFunctions_uPerp_rms_mc.push_back(
      fitGraph_uPerp_rms("f_uPerp_rms_mc", plotUvsQtNumObj->graphUperpResolutionMC_, xMin_qT, xMax_qT));
    graphs_metX_vs_sumEt_mc.push_back(plotUvsQtNumObj->graphMEtXvsSumEtMC_);
    fitFunctions_metX_vs_sumEt_mc.push_back(
      fitGraph_metXorY_vs_sumEt("f_metX_vs_sumEt_mc", plotUvsQtNumObj->graphMEtXvsSumEtMC_, xMin_sumEt, xMax_sumEt));
    graphs_metY_vs_sumEt_mc.push_back(plotUvsQtNumObj->graphMEtYvsSumEtMC_);
    fitFunctions_metY_vs_sumEt_mc.push_back(
      fitGraph_metXorY_vs_sumEt("f_metY_vs_sumEt_mc", plotUvsQtNumObj->graphMEtYvsSumEtMC_, xMin_sumEt, xMax_sumEt));
    graphs_metX_vs_sumEtExclMuons_mc.push_back(plotUvsQtNumObj->graphMEtXvsSumEtExclMuonsMC_);
    fitFunctions_metX_vs_sumEtExclMuons_mc.push_back(
      fitGraph_metXorY_vs_sumEt("f_metX_vs_sumEtExclMuons_mc", plotUvsQtNumObj->graphMEtXvsSumEtExclMuonsMC_, xMin_sumEt, xMax_sumEt));
    graphs_metY_vs_sumEtExclMuons_mc.push_back(plotUvsQtNumObj->graphMEtYvsSumEtExclMuonsMC_);
    fitFunctions_metY_vs_sumEtExclMuons_mc.push_back(
      fitGraph_metXorY_vs_sumEt("f_metY_vs_sumEtExclMuons_mc", plotUvsQtNumObj->graphMEtYvsSumEtExclMuonsMC_, xMin_sumEt, xMax_sumEt));
  }

  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);

  TH1* dummyHistogram_qT = new TH1D("dummyHistogram_qT", "dummyHistogram_qT", (xMax_qT - xMin_qT)/10., xMin_qT, xMax_qT);
  TH1* dummyHistogram_sumEt = new TH1D("dummyHistogram_sumEt", "dummyHistogram_sumEt", (xMax_sumEt - xMin_sumEt)/10., xMin_sumEt, xMax_sumEt); 

  drawZllRecoilFitResult(canvas, dummyHistogram_qT, 
			 graphs_uParl_div_qT_mean_mc, fitFunctions_uParl_div_qT_mean_mc, drawGraphs && showMC, drawFits && showMC,
			 graphs_uParl_div_qT_mean_data, fitFunctions_uParl_div_qT_mean_data, drawGraphs && showData, drawFits && showData,
			 legendEntries, 0.61, 0.165, false, true, "-u_{#parallel} /q_{T}", 0.4, 1.2,
			 outputFileName, "uParlResponseFit");
  drawZllRecoilFitResult(canvas, dummyHistogram_qT, 
			 graphs_uParl_rms_mc, fitFunctions_uParl_rms_mc, drawGraphs && showMC, drawFits && showMC,
			 graphs_uParl_rms_data, fitFunctions_uParl_rms_data, drawGraphs && showData, drawFits && showData,
			 legendEntries, 0.19, 0.62, false, false, "RMS(u_{#parallel} ) / GeV", 0., 50., 
			 outputFileName, "uParlResolutionFit");
  drawZllRecoilFitResult(canvas, dummyHistogram_qT, 
			 graphs_uPerp_rms_mc, fitFunctions_uPerp_rms_mc, drawGraphs && showMC, drawFits && showMC,
			 graphs_uPerp_rms_data, fitFunctions_uPerp_rms_data, drawGraphs && showData, drawFits && showData,
			 legendEntries, 0.19, 0.62, false, false, "RMS(u_{#perp}  ) / GeV", 0., 50.,
			 outputFileName, "uPerpResolutionFit");

  drawZllRecoilFitResult(canvas, dummyHistogram_sumEt, 
			 graphs_metX_vs_sumEt_mc, fitFunctions_metX_vs_sumEt_mc, drawGraphs && showMC, drawFits && showMC,
			 graphs_metX_vs_sumEt_data, fitFunctions_metX_vs_sumEt_data, drawGraphs && showData, drawFits && showData,
			 legendEntries, 0.19, 0.62, false, false, "RMS(E_{X}^{miss} ) / GeV", 0., 50.,
			 outputFileName, "metX_vs_sumEtFit");
  drawZllRecoilFitResult(canvas, dummyHistogram_sumEt, 
			 graphs_metY_vs_sumEt_mc, fitFunctions_metY_vs_sumEt_mc, drawGraphs && showMC, drawFits && showMC,
			 graphs_metY_vs_sumEt_data, fitFunctions_metY_vs_sumEt_data, drawGraphs && showData, drawFits && showData,
			 legendEntries, 0.19, 0.62, false, false, "RMS(E_{Y}^{miss} ) / GeV", 0., 50.,
			 outputFileName, "metY_vs_sumEtFit");

  drawZllRecoilFitResult(canvas, dummyHistogram_sumEt, 
			 graphs_metX_vs_sumEtExclMuons_mc, fitFunctions_metX_vs_sumEtExclMuons_mc, drawGraphs && showMC, drawFits && showMC,
			 graphs_metX_vs_sumEtExclMuons_data, fitFunctions_metX_vs_sumEtExclMuons_data, drawGraphs && showData, drawFits && showData,
			 legendEntries, 0.19, 0.62, false, false, "RMS(E_{X}^{miss} ) / GeV", 0., 50.,
			 outputFileName, "metX_vs_sumEtExclMuonsFit");
  drawZllRecoilFitResult(canvas, dummyHistogram_sumEt, 
			 graphs_metY_vs_sumEtExclMuons_mc, fitFunctions_metY_vs_sumEtExclMuons_mc, drawGraphs && showMC, drawFits && showMC,
			 graphs_metY_vs_sumEtExclMuons_data, fitFunctions_metY_vs_sumEtExclMuons_data, drawGraphs && showData, drawFits && showData,
			 legendEntries, 0.19, 0.62, false, false, "RMS(E_{Y}^{miss} ) / GeV", 0., 50.,
			 outputFileName, "metY_vs_sumEtExclMuonsFit");
  
  deleteAll(fitFunctions_uParl_div_qT_mean_data);
  deleteAll(fitFunctions_uParl_rms_data);
  deleteAll(fitFunctions_uPerp_rms_data);
  deleteAll(fitFunctions_metX_vs_sumEt_data);
  deleteAll(fitFunctions_metY_vs_sumEt_data);
  deleteAll(fitFunctions_metX_vs_sumEtExclMuons_data);
  deleteAll(fitFunctions_metY_vs_sumEtExclMuons_data);

  deleteAll(fitFunctions_uParl_div_qT_mean_mc);
  deleteAll(fitFunctions_uParl_rms_mc);
  deleteAll(fitFunctions_uPerp_rms_mc);
  deleteAll(fitFunctions_metX_vs_sumEt_mc);
  deleteAll(fitFunctions_metY_vs_sumEt_mc);
  deleteAll(fitFunctions_metX_vs_sumEtExclMuons_mc);
  deleteAll(fitFunctions_metY_vs_sumEtExclMuons_mc);
  
  delete canvas;
  delete dummyHistogram_qT;
  delete dummyHistogram_sumEt;
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
    std::cout << "setting scale-factor for " << (*sampleName) << " = " << scaleFactor << std::endl;
  }

  vstring sysShiftsUp   = cfgMakeZllRecoilCorrectionPlots.getParameter<vstring>("sysShiftsUp");
  vstring sysShiftsDown = cfgMakeZllRecoilCorrectionPlots.getParameter<vstring>("sysShiftsDown");
  if ( sysShiftsUp.size() != sysShiftsDown.size() )
    throw cms::Exception("makeZllRecoilCorrectionFinalPlots") 
      << "Number of 'up' shifts must match number of 'down' shifts !!\n";

  bool isCaloMEt = cfgMakeZllRecoilCorrectionPlots.getParameter<bool>("isCaloMEt");

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
			    directoryData, directoryMC_signal, directoryMCs_background, 
			    mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown,
			    isCaloMEt);
  std::cout << "running fits for inclusive event sample..." << std::endl;
  fitAndMakeControlPlots(plotUvsQt, outputFileName);

//--- make plots of mean(uParl)/qT, rms(uParl)/qT, rms(uPerp)/qT
//    in different bins of reconstructed jet multiplicity
  plotUvsQtNumObjType* plotUvsQtNumJetsPtGt10Eq0 = 
    new plotUvsQtNumObjType(inputFile, "NumJetsPtGt10", 0,  0, "jet", 
			    directoryData, directoryMC_signal, directoryMCs_background, 
			    mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown,
			    isCaloMEt);
  std::cout << "running fits for events with #jets(Pt > 10 GeV) = 0..." << std::endl;
  fitAndMakeControlPlots(plotUvsQtNumJetsPtGt10Eq0, getOutputFileName_plot(outputFileName, plotUvsQtNumJetsPtGt10Eq0->meLabel_));
  plotUvsQtNumObjType* plotUvsQtNumJetsPtGt10Eq1 = 
    new plotUvsQtNumObjType(inputFile, "NumJetsPtGt10", 1,  1, "jet", 
			    directoryData, directoryMC_signal, directoryMCs_background, 
			    mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown,
			    isCaloMEt);
  std::cout << "running fits for events with #jets(Pt > 10 GeV) = 1..." << std::endl;
  fitAndMakeControlPlots(plotUvsQtNumJetsPtGt10Eq1, getOutputFileName_plot(outputFileName, plotUvsQtNumJetsPtGt10Eq1->meLabel_));
  plotUvsQtNumObjType* plotUvsQtNumJetsPtGt10Eq2 = 
    new plotUvsQtNumObjType(inputFile, "NumJetsPtGt10", 2,  2, "jet", 
			    directoryData, directoryMC_signal, directoryMCs_background, 
			    mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown,
			    isCaloMEt);
  std::cout << "running fits for events with #jets(Pt > 10 GeV) = 2..." << std::endl;
  fitAndMakeControlPlots(plotUvsQtNumJetsPtGt10Eq2, getOutputFileName_plot(outputFileName, plotUvsQtNumJetsPtGt10Eq2->meLabel_));
  plotUvsQtNumObjType* plotUvsQtNumJetsPtGt10Ge3 = 
    new plotUvsQtNumObjType(inputFile, "NumJetsPtGt10", 3, -1, "jet", 
			    directoryData, directoryMC_signal, directoryMCs_background, 
			    mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown,
			    isCaloMEt);
  std::cout << "running fits for events with #jets(Pt > 10 GeV) >= 3..." << std::endl;
  fitAndMakeControlPlots(plotUvsQtNumJetsPtGt10Ge3, getOutputFileName_plot(outputFileName, plotUvsQtNumJetsPtGt10Ge3->meLabel_));

  plotUvsQtNumObjType* plotUvsQtNumJetsPtGt20Eq0 = 
    new plotUvsQtNumObjType(inputFile, "NumJetsPtGt20", 0,  0, "jet", 
			    directoryData, directoryMC_signal, directoryMCs_background, 
			    mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown,
			    isCaloMEt);
  std::cout << "running fits for events with #jets(Pt > 20 GeV) = 0..." << std::endl;
  fitAndMakeControlPlots(plotUvsQtNumJetsPtGt20Eq0, getOutputFileName_plot(outputFileName, plotUvsQtNumJetsPtGt20Eq0->meLabel_));
  plotUvsQtNumObjType* plotUvsQtNumJetsPtGt20Eq1 = 
    new plotUvsQtNumObjType(inputFile, "NumJetsPtGt20", 1,  1, "jet", 
			    directoryData, directoryMC_signal, directoryMCs_background,
			    mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown,
			    isCaloMEt);
  std::cout << "running fits for events with #jets(Pt > 20 GeV) = 1..." << std::endl;
  fitAndMakeControlPlots(plotUvsQtNumJetsPtGt20Eq1, getOutputFileName_plot(outputFileName, plotUvsQtNumJetsPtGt20Eq1->meLabel_));
  plotUvsQtNumObjType* plotUvsQtNumJetsPtGt20Eq2 = 
    new plotUvsQtNumObjType(inputFile, "NumJetsPtGt20", 2,  2, "jet", 
			    directoryData, directoryMC_signal, directoryMCs_background, 
			    mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown,
			    isCaloMEt);
  std::cout << "running fits for events with #jets(Pt > 20 GeV) = 2..." << std::endl;
  fitAndMakeControlPlots(plotUvsQtNumJetsPtGt20Eq2, getOutputFileName_plot(outputFileName, plotUvsQtNumJetsPtGt20Eq2->meLabel_));
  plotUvsQtNumObjType* plotUvsQtNumJetsPtGt20Ge3 = 
    new plotUvsQtNumObjType(inputFile, "NumJetsPtGt20", 3, -1, "jet", 
			    directoryData, directoryMC_signal, directoryMCs_background, 
			    mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown,
			    isCaloMEt);
  std::cout << "running fits for events with #jets(Pt > 20 GeV) >= 3..." << std::endl;
  fitAndMakeControlPlots(plotUvsQtNumJetsPtGt20Ge3, getOutputFileName_plot(outputFileName, plotUvsQtNumJetsPtGt20Ge3->meLabel_));

  plotUvsQtNumObjType* plotUvsQtNumJetsPtGt30Eq0 = 
    new plotUvsQtNumObjType(inputFile, "NumJetsPtGt30", 0,  0, "jet", 
			    directoryData, directoryMC_signal, directoryMCs_background, 
			    mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown,
			    isCaloMEt);
  std::cout << "running fits for events with #jets(Pt > 30 GeV) = 0..." << std::endl;
  fitAndMakeControlPlots(plotUvsQtNumJetsPtGt30Eq0, getOutputFileName_plot(outputFileName, plotUvsQtNumJetsPtGt30Eq0->meLabel_));
  plotUvsQtNumObjType* plotUvsQtNumJetsPtGt30Eq1 = 
    new plotUvsQtNumObjType(inputFile, "NumJetsPtGt30", 1,  1, "jet", 
			    directoryData, directoryMC_signal, directoryMCs_background, 
			    mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown,
			    isCaloMEt);
  std::cout << "running fits for events with #jets(Pt > 30 GeV) = 1..." << std::endl;
  fitAndMakeControlPlots(plotUvsQtNumJetsPtGt30Eq1, getOutputFileName_plot(outputFileName, plotUvsQtNumJetsPtGt30Eq1->meLabel_));
  plotUvsQtNumObjType* plotUvsQtNumJetsPtGt30Eq2 = 
    new plotUvsQtNumObjType(inputFile, "NumJetsPtGt30", 2,  2, "jet", 
			    directoryData, directoryMC_signal, directoryMCs_background, 
			    mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown,
			    isCaloMEt);
  std::cout << "running fits for events with #jets(Pt > 30 GeV) = 2..." << std::endl;
  fitAndMakeControlPlots(plotUvsQtNumJetsPtGt30Eq2, getOutputFileName_plot(outputFileName, plotUvsQtNumJetsPtGt30Eq2->meLabel_));
  plotUvsQtNumObjType* plotUvsQtNumJetsPtGt30Ge3 = 
    new plotUvsQtNumObjType(inputFile, "NumJetsPtGt30", 3, -1, "jet", 
			    directoryData, directoryMC_signal, directoryMCs_background, 
			    mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown,
			    isCaloMEt);
  std::cout << "running fits for events with #jets(Pt > 30 GeV) >= 3..." << std::endl;
  fitAndMakeControlPlots(plotUvsQtNumJetsPtGt30Ge3, getOutputFileName_plot(outputFileName, plotUvsQtNumJetsPtGt30Ge3->meLabel_));

//--- make plots of mean(uParl)/qT, rms(uParl)/qT and rms(uPerp)/qT
//    in different bins of reconstructed vertex multiplicity (pile-up)
  plotUvsQtNumObjType* plotUvsQtNumVerticesEq4 =
    new plotUvsQtNumObjType(inputFile, "NumVertices",  4,  4, "vertex", 
			    directoryData, directoryMC_signal, directoryMCs_background,
			    mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown,
			    isCaloMEt);
  plotUvsQtNumObjType* plotUvsQtNumVerticesEq12 =
    new plotUvsQtNumObjType(inputFile, "NumVertices", 12, 12, "vertex", 
			    directoryData, directoryMC_signal, directoryMCs_background, 
			    mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown,
			    isCaloMEt);
  plotUvsQtNumObjType* plotUvsQtNumVerticesEq20 =
    new plotUvsQtNumObjType(inputFile, "NumVertices", 20, 20, "vertex", 
			    directoryData, directoryMC_signal, directoryMCs_background, 
			    mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown,
			    isCaloMEt);
  plotUvsQtNumObjType* plotUvsQtNumVerticesEq28 =
    new plotUvsQtNumObjType(inputFile, "NumVertices", 28, 28, "vertex", 
			    directoryData, directoryMC_signal, directoryMCs_background, 
			    mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown,
			    isCaloMEt);
  std::vector<plotUvsQtNumObjType*> plotUvsQtNumObjsNvtx;
  plotUvsQtNumObjsNvtx.push_back(plotUvsQtNumVerticesEq4);
  plotUvsQtNumObjsNvtx.push_back(plotUvsQtNumVerticesEq12);
  plotUvsQtNumObjsNvtx.push_back(plotUvsQtNumVerticesEq20);
  plotUvsQtNumObjsNvtx.push_back(plotUvsQtNumVerticesEq28);
  std::vector<std::string> legendEntriesNvtx;
  legendEntriesNvtx.push_back("4 Vertices");
  legendEntriesNvtx.push_back("12 Vertices");
  legendEntriesNvtx.push_back("20 Vertices");
  legendEntriesNvtx.push_back("28 Vertices");
  std::cout << "running fits for different Nvtx..." << std::endl;
  fitAndCompare(plotUvsQtNumObjsNvtx, legendEntriesNvtx, true, false, true, false, 
		getOutputFileName_plot(outputFileName, "puDependenceMC"));
  fitAndCompare(plotUvsQtNumObjsNvtx, legendEntriesNvtx, true, false, false, true, 
		getOutputFileName_plot(outputFileName, "puDependenceData"));

//--- compare mean(uParl)/qT, rms(uParl)/qT and rms(uPerp)/qT
//    for events in which the highest Pt jet is in the Barrel, the Endcap or the HF
  plotUvsQtNumObjType* plotUvsQtLeadJetBarrel =
    new plotUvsQtNumObjType(inputFile, "LeadJetBarrel",
			    directoryData, directoryMC_signal, directoryMCs_background, 
			    mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown,
			    isCaloMEt);
  plotUvsQtNumObjType* plotUvsQtLeadJetEndcap =
    new plotUvsQtNumObjType(inputFile, "LeadJetEndcap", 
			    directoryData, directoryMC_signal, directoryMCs_background, 
			    mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown,
			    isCaloMEt);
  plotUvsQtNumObjType* plotUvsQtLeadJetForward =
    new plotUvsQtNumObjType(inputFile, "LeadJetForward", 
			    directoryData, directoryMC_signal, directoryMCs_background, 
			    mcScaleFactors, runPeriod, sysShiftsUp, sysShiftsDown,
			    isCaloMEt);
  std::vector<plotUvsQtNumObjType*> plotUvsQtNumObjsLeadJetEta;
  plotUvsQtNumObjsLeadJetEta.push_back(plotUvsQtLeadJetBarrel);
  plotUvsQtNumObjsLeadJetEta.push_back(plotUvsQtLeadJetEndcap);
  plotUvsQtNumObjsLeadJetEta.push_back(plotUvsQtLeadJetForward);
  std::vector<std::string> legendEntriesLeadJetEta;
  legendEntriesLeadJetEta.push_back("Lead. Jet Barrel");
  legendEntriesLeadJetEta.push_back("Lead. Jet Endcap");
  legendEntriesLeadJetEta.push_back("Lead. Jet Forward");
  std::cout << "running fits for different lead. jet eta regions..." << std::endl;
  fitAndCompare(plotUvsQtNumObjsLeadJetEta, legendEntriesLeadJetEta, true, false, true, false, 
		getOutputFileName_plot(outputFileName, "leadJetEtaDependenceMC"));
  fitAndCompare(plotUvsQtNumObjsLeadJetEta, legendEntriesLeadJetEta, true, false, false, true, 
		getOutputFileName_plot(outputFileName, "leadJetEtaDependenceData"));
 
  delete inputFile;

//--print time that it took macro to run
  std::cout << "finished executing makeZllRecoilCorrectionFinalPlots macro:" << std::endl;
  clock.Show("makeZllRecoilCorrectionFinalPlots");

  return 0;
}
