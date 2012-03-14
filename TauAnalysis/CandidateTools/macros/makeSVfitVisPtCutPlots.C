
#include <TFile.h>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraphAsymmErrors.h>
#include <TF1.h>
#include <TF2.h>
#include <TFitResultPtr.h>
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

void add_indentation(ostream& stream, int indent)
{
  for ( int iSpace = 0; iSpace < indent; ++iSpace ) {
    stream << " ";
  }
}

struct fitParameterTypeBase
{
  fitParameterTypeBase()
    : numFitParameter_(0),
      fitStatus_(-1)
  {}
  fitParameterTypeBase(const fitParameterTypeBase& bluePrint)
  {
    variable_ = bluePrint.variable_;
    formula_ = bluePrint.formula_;
    xMin_ = bluePrint.xMin_;
    xMax_ = bluePrint.xMax_;
    numFitParameter_ = bluePrint.numFitParameter_;
    fitStatus_ = bluePrint.fitStatus_;
  }
  fitParameterTypeBase(const std::string& variable, const TF1* fitFunction, double xMin, double xMax, int fitStatus)
  {
    set(variable, fitFunction, xMin, xMax, fitStatus);
  }
  void set(const std::string& variable, const TF1* fitFunction, double xMin, double xMax, int fitStatus)
  {
    variable_ = variable;
    formula_ = fitFunction->GetTitle();
    xMin_ = xMin;
    xMax_ = xMax;
    numFitParameter_ = fitFunction->GetNpar();
    fitStatus_ = fitStatus;
  }  
  virtual fitParameterTypeBase* clone() = 0;
  fitParameterTypeBase& operator=(const fitParameterTypeBase& other)
  {
    variable_ = other.variable_;
    formula_ = other.formula_;
    xMin_ = other.xMin_;
    xMax_ = other.xMax_;
    numFitParameter_ = other.numFitParameter_;
    fitStatus_ = other.fitStatus_;
    return (*this);
  }
  virtual void dump(std::ostream& stream, int indent)
  {
    add_indentation(stream, indent);
    stream << "variable = cms.string('" << variable_ << "')," << std::endl;
    add_indentation(stream, indent);
    stream << "formula = cms.string('" << formula_ << "')," << std::endl;
    add_indentation(stream, indent);
    stream << "xMin = cms.double('" << xMin_ << "')," << std::endl;
    add_indentation(stream, indent);
    stream << "xMax = cms.double('" << xMax_ << "')," << std::endl;
  }
  std::string variable_;
  std::string formula_;
  double xMin_;
  double xMax_;
  int numFitParameter_;
  int fitStatus_;
};

struct fitParameterType_const : public fitParameterTypeBase
{
  fitParameterType_const()
  {}
  fitParameterType_const(const fitParameterType_const& bluePrint)
    : fitParameterTypeBase(bluePrint)
  {
    fitParameterValues_.resize(bluePrint.numFitParameter_);
    fitParameterErrors_.resize(bluePrint.numFitParameter_);
    for ( int iFitParameter = 0; iFitParameter < numFitParameter_; ++iFitParameter ) {
      fitParameterValues_[iFitParameter] = bluePrint.fitParameterValues_[iFitParameter];
      fitParameterErrors_[iFitParameter] = bluePrint.fitParameterErrors_[iFitParameter];
    }
  }
  fitParameterType_const(const std::string& variable, const TF1* fitFunction, double xMin, double xMax, int fitStatus)
    : fitParameterTypeBase(variable, fitFunction, xMin, xMax, fitStatus)
  {
    set(variable, fitFunction, xMin, xMax, fitStatus);
  }
  void set(const std::string& variable, const TF1* fitFunction, double xMin, double xMax, int fitStatus)
  {
    fitParameterTypeBase::set(variable, fitFunction, xMin, xMax, fitStatus);
    fitParameterValues_.resize(numFitParameter_);
    fitParameterErrors_.resize(numFitParameter_);
    for ( int iFitParameter = 0; iFitParameter < numFitParameter_; ++iFitParameter ) {
      fitParameterValues_[iFitParameter] = fitFunction->GetParameter(iFitParameter);
      fitParameterErrors_[iFitParameter] = fitFunction->GetParError(iFitParameter);
    }
  }
  fitParameterTypeBase* clone() { return new fitParameterType_const(*this); }
  fitParameterType_const& operator=(const fitParameterType_const& other)
  {
    fitParameterTypeBase::operator=(other);
    fitParameterValues_.resize(numFitParameter_);
    fitParameterErrors_.resize(numFitParameter_);
    for ( int iFitParameter = 0; iFitParameter < numFitParameter_; ++iFitParameter ) {
      fitParameterValues_[iFitParameter] = other.fitParameterValues_[iFitParameter];
      fitParameterErrors_[iFitParameter] = other.fitParameterErrors_[iFitParameter];
    }
    return (*this);
  }
  void dump(std::ostream& stream, int indent)
  {
    fitParameterTypeBase::dump(stream, indent);
    add_indentation(stream, indent);
    stream << "parameter = cms.PSet(" << std::endl;
    for ( int iFitParameter = 0; iFitParameter < numFitParameter_; ++iFitParameter ) {
      add_indentation(stream, indent + 4);
      stream << Form("par%i = cms.double(", iFitParameter) << fitParameterValues_[iFitParameter] << ")," << std::endl;
    }
    add_indentation(stream, indent);
    stream << ")," << std::endl;
  }
  std::vector<double> fitParameterValues_;
  std::vector<double> fitParameterErrors_;
};

struct fitParameterType_formula : public fitParameterTypeBase
{  
  fitParameterType_formula()
  {}
  fitParameterType_formula(const fitParameterType_formula& bluePrint)
    : fitParameterTypeBase(bluePrint)
  {    
    fitParameters_.resize(bluePrint.numFitParameter_);
    for ( int iFitParameter = 0; iFitParameter < numFitParameter_; ++iFitParameter ) {
      fitParameters_[iFitParameter] = bluePrint.fitParameters_[iFitParameter]->clone();
    }
  }
  fitParameterTypeBase* clone() { return new fitParameterType_formula(*this); }
  fitParameterType_formula& operator=(const fitParameterType_formula& other)
  {
    fitParameterTypeBase::operator=(other);
    fitParameters_.resize(numFitParameter_);
    for ( int iFitParameter = 0; iFitParameter < numFitParameter_; ++iFitParameter ) {
      fitParameters_[iFitParameter] = other.fitParameters_[iFitParameter]->clone();
    }
    return (*this);
  }
  void dump(std::ostream& stream, int indent)
  {
    fitParameterTypeBase::dump(stream, indent);
    add_indentation(stream, indent);
    stream << "parameter = cms.VPSet(" << std::endl;
    for ( int iFitParameter = 0; iFitParameter < numFitParameter_; ++iFitParameter ) {
      add_indentation(stream, indent + 4);
      stream << Form("par%i = cms.PSet(", iFitParameter) << std::endl;
      fitParameters_[iFitParameter]->dump(stream, indent + 8);
      add_indentation(stream, indent + 4);
      stream << ")," << std::endl;
    }
    add_indentation(stream, indent);
    stream << ")," << std::endl;
  }
  std::vector<fitParameterTypeBase*> fitParameters_;
};

TF1* fitX1d(TH1* histogram_afterVisPtCuts_div_beforeVisPtCuts, 
	    TH1* histogram_beforeVisPtCuts, 
	    double visPtCut, double mass,
	    fitParameterType_const* fitParameter)
{
  std::cout << "<fitX1d>:" << std::endl;
  std::cout << " histogram = " << histogram_beforeVisPtCuts->GetName() << std::endl;
  //std::cout << " visPtCut = " << visPtCut << std::endl;
  //std::cout << " mass = " << mass << std::endl;
  
  TString fitFunctionName = TString(histogram_afterVisPtCuts_div_beforeVisPtCuts->GetName()).Append("_fit");
  TAxis* xAxis = histogram_afterVisPtCuts_div_beforeVisPtCuts->GetXaxis();  
  double xMin = xAxis->GetXmin();
  if ( xMin < 0. ) xMin = 0.;
  double xMax = xAxis->GetXmax();
  TF1* fitFunction = 
    new TF1(fitFunctionName.Data(), 
	    "[0]*[1]*0.5*(1.0 - TMath::Erf(-(x - ([2] + [3]))/(TMath::Sqrt(2.)*[4])))", xMin, xMax);
  
  double x0 = visPtCut/(0.5*mass);  
  //std::cout << "x0 = " << x0 << std::endl;
  int bin0 = histogram_beforeVisPtCuts->FindBin(x0);
  //std::cout << "bin0 = " << bin0 << std::endl;
  double integral = histogram_beforeVisPtCuts->Integral(1, histogram_beforeVisPtCuts->GetNbinsX());
  //std::cout << "integral = " << integral << std::endl;
  double integralXgtX0 = histogram_beforeVisPtCuts->Integral(bin0, histogram_beforeVisPtCuts->GetNbinsX());
  //std::cout << "integralXgtX0 = " << integralXgtX0 << std::endl;
  double norm0 = integral/integralXgtX0;
  //std::cout << "norm0 = " << norm0 << std::endl;
  
  double p0         = norm0;
  double p1_initial = 1.;
  double p2         = x0;
  double p3_initial = 0.;
  double p4_initial = 1.e-1;

  //std::cout << "initializing fitParameters:" << std::endl;
  //std::cout << " p0 = " << p0 << " (fixed)" << std::endl;
  //std::cout << " p1 = " << p1_initial << std::endl;
  //std::cout << " p2 = " << p2 << " (fixed)" << std::endl;
  //std::cout << " p3 = " << p3_initial << std::endl;
  //std::cout << " p4 = " << p3_initial << std::endl;
  
//--- fit function in two iterations:
//    in the first iteration keep p1 fixed at zero;
//    allow p1 to vary in the second iteration, once "good" values for all other function parameters are found
//   (the aim of this two step procedure is to make the fit more robust)
  fitFunction->FixParameter(0, p0);
  fitFunction->SetParameter(1, p1_initial);
  fitFunction->FixParameter(2, p2);
  fitFunction->SetParameter(3, p3_initial);
  fitFunction->SetParameter(4, p4_initial);
  TFitResultPtr fitResult = histogram_afterVisPtCuts_div_beforeVisPtCuts->Fit(fitFunction, "0");
  std::cout << "fitStatus = " << (int)fitResult << std::endl;

  if ( fitParameter ) {
    fitParameter->set("X", fitFunction, xMin, xMax, (int)fitResult);
  }

  return fitFunction;
}

void showPlot(TH1* histogramRef, const std::string& legendEntryRef,
	      TH1* histogram1, const std::string& legendEntry1,
	      TH1* histogram2, const std::string& legendEntry2,
	      TF1* (*fitFunction)(TH1*, TH1*, double, double, fitParameterType_const*), 
	      double visPtCut, double mass,
	      fitParameterType_const* fitParameter,
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

  TF1* fitFunction1 = (*fitFunction)(histogram1divRef, histogramRef, visPtCut, mass, NULL);
  fitFunction1->SetLineColor(histogram1divRef->GetLineColor());
  fitFunction1->SetLineWidth(1);
  fitFunction1->Draw("same");

  TString histogram2divRefName = TString(histogram2->GetName()).Append("_div_").Append(histogramRef->GetName());
  TH1* histogram2divRef = compRatioHistogram(histogram2divRefName, histogram2, histogramRef);

  TF1* fitFunction2 = (*fitFunction)(histogram2divRef, histogramRef, visPtCut, mass, fitParameter);
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
	       double visPtCut, double mass, 
	       const std::string& outputFileName,
	       fitParameterType_const* fitParameter)
{
  TH1* histogram_beforeVisPtCuts_normalized         = getHistogram_normalized(histogram_beforeVisPtCuts);
  TH1* histogram_afterVisPtCutsSingleLeg_normalized = getHistogram_normalized(histogram_afterVisPtCutsSingleLeg);
  TH1* histogram_afterVisPtCutsBothLegs_normalized  = getHistogram_normalized(histogram_afterVisPtCutsBothLegs);
  
  std::string legendEntry_beforeVisPtCuts           = "before Cuts";
  std::string legendEntry_afterVisPtCutsSingleLeg   = "after Cuts Leg 1";
  std::string legendEntry_afterVisPtCutsBothLegs    = "after Cuts Legs 1&2";

  TF1* (*fitFunction)(TH1*, TH1*, double, double, fitParameterType_const*) = &fitX1d;

  std::string xAxisTitle = Form("X_{%i}", legIdx);

  showPlot(histogram_beforeVisPtCuts_normalized, legendEntry_beforeVisPtCuts,
	   histogram_afterVisPtCutsSingleLeg_normalized, legendEntry_afterVisPtCutsSingleLeg,
	   histogram_afterVisPtCutsBothLegs_normalized, legendEntry_afterVisPtCutsBothLegs,
	   fitFunction, visPtCut, mass, fitParameter, xAxisTitle, outputFileName);

  //if ( fitParameter ) fitParameter->dump(std::cout, 0);
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
}

void showGraphDPhi(const std::string& type,
		   const std::vector<double>& dPhiValues,
		   const std::vector<double>& dPhiErrsUp, const std::vector<double>& dPhiErrsDown, 
		   std::map<double, fitParameterType_const>& fitParameter_in,
		   fitParameterType_formula* fitParameter_out,
		   const std::string& outputFileName)
{
  const fitParameterType_const& fitParameter_ref = fitParameter_in.begin()->second;
  if ( fitParameter_out ) {
    fitParameter_out->fitParameterTypeBase::operator=(fitParameter_ref);
    fitParameter_out->fitParameters_.resize(fitParameter_out->numFitParameter_);
  }

  int numFitParameter_in = fitParameter_ref.numFitParameter_;
  for ( int iFitParameter_in = 0; iFitParameter_in < numFitParameter_in; ++iFitParameter_in ) {

    std::vector<double> graph_xs;
    std::vector<double> graph_xErrsUp;
    std::vector<double> graph_xErrsDown;
    std::vector<double> graph_ys;
    std::vector<double> graph_yErrsUp;
    std::vector<double> graph_yErrsDown;

    int numDPhiValues = dPhiValues.size();
    for ( int iDPhiValue = 0; iDPhiValue < numDPhiValues; ++iDPhiValue ) {
      double dPhiValue = dPhiValues[iDPhiValue];

      const fitParameterType_const& fitParameter_in_dPhi = fitParameter_in[dPhiValue];
      assert(fitParameter_in_dPhi.numFitParameter_ == numFitParameter_in);

      double x         = dPhiValue;
      double xErrUp    = dPhiErrsUp[iDPhiValue];
      double xErrDown  = dPhiErrsDown[iDPhiValue];
      
      double y         = fitParameter_in_dPhi.fitParameterValues_[iFitParameter_in];
      double yErr      = fitParameter_in_dPhi.fitParameterErrors_[iFitParameter_in];

      if ( fitParameter_in_dPhi.fitStatus_ == 0 ) {
	graph_xs.push_back(x);
        graph_xErrsUp.push_back(xErrUp);
        graph_xErrsDown.push_back(xErrDown);
        graph_ys.push_back(y);
        graph_yErrsUp.push_back(yErr);
	graph_yErrsDown.push_back(yErr);
      }
    }

    size_t numPoints = graph_xs.size();
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

    double xMin = 0.;
    double xMax = TMath::Pi();
    
    TF1* fitFunction = 0;
    std::string fitOptions;
    if ( iFitParameter_in == 0 ) {
      fitFunction = new TF1("fitFunction", "[0]", xMin, xMax);
      fitOptions = "W";
    } else if ( iFitParameter_in == 1 ) {
      fitFunction = new TF1("fitFunction", "[0] + [1]*x + [2]*0.5*(3.*x*x - 1.)", xMin, xMax);
    } else if ( iFitParameter_in == 2 ) {
      fitFunction = new TF1("fitFunction", "[0]", xMin, xMax);
      fitOptions = "W";
    } else if ( iFitParameter_in == 3 ) {
      fitFunction = new TF1("fitFunction", "[0] + [1]*x", xMin, xMax);
    } else if ( iFitParameter_in == 4 ) {
      fitFunction = new TF1("fitFunction", "[0] + [1]*x", xMin, xMax);
      fitOptions = "W";
    } else assert(0);
    fitOptions.append("0");
    TFitResultPtr fitResult = graph->Fit(fitFunction, fitOptions.data());   
    if ( fitParameter_out ) {
      fitParameter_out->fitParameters_[iFitParameter_in] = new fitParameterType_const("dPhi", fitFunction, xMin, xMax, (int)fitResult);
    }

    double yMin = +1.e+6;
    double yMax = -1.e+6;
    for ( size_t iPoint = 0; iPoint < numPoints; ++iPoint ) {
      double x, y;
      graph->GetPoint(iPoint, x, y);
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
    
    std::string xAxisTitle = "#Delta#phi / Rad";
    std::string yAxisTitle = Form("p_{%i}", iFitParameter_in);

    TAxis* xAxis = dummyHistogram->GetXaxis();
    xAxis->SetTitle(xAxisTitle.data());
    xAxis->SetTitleOffset(1.15);
    
    TAxis* yAxis = dummyHistogram->GetYaxis();
    yAxis->SetTitle(yAxisTitle.data());
    yAxis->SetTitleOffset(1.30);
    
    dummyHistogram->Draw("axis");

    fitFunction->SetLineColor(2);
    fitFunction->SetLineWidth(2);
    fitFunction->Draw("same");

    graph->SetMarkerStyle(20);
    graph->SetMarkerColor(1);
    graph->Draw("P");

    canvas->Update();
    size_t idx = outputFileName.find_last_of('.');
    std::string outputFileName_param = std::string(outputFileName, 0, idx);
    outputFileName_param = Form(outputFileName_param.data(), iFitParameter_in);
    if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_param).append(std::string(outputFileName, idx)).data());
    canvas->Print(std::string(outputFileName_param).append(".png").data());
    canvas->Print(std::string(outputFileName_param).append(".pdf").data());
    canvas->Print(std::string(outputFileName_param).append(".root").data());

    delete fitFunction;
    delete dummyHistogram;
    delete canvas;
  }

  //if ( fitParameter_out ) fitParameter_out->dump(std::cout, 0);
}

void showGraphMass(const std::string& type,
		   const std::vector<double>& massPoints,
		   const std::vector<double>& massPointErrsUp, const std::vector<double>& massPointErrsDown, 
		   std::map<double, fitParameterType_formula>& fitParameter_in,
		   fitParameterType_formula* fitParameter_out,
		   const std::string& outputFileName)
{
  assert(massPoints.size() > 0);
  double massPoint_ref = massPoints.front();
  const fitParameterType_formula& fitParameter_ref_formula = fitParameter_in[massPoint_ref];
  if ( fitParameter_out ) {
    (*fitParameter_out) = fitParameter_ref_formula;
    fitParameter_out->fitParameters_.resize(fitParameter_out->numFitParameter_);
  }
  
  int numFitParameter_in_formula = fitParameter_ref_formula.numFitParameter_;
  for ( int iFitParameter_in_formula = 0; iFitParameter_in_formula < numFitParameter_in_formula; ++iFitParameter_in_formula ) {
    std::vector<fitParameterType_const> tmpFitParameter_out;
    int numFitParameter_in_const = fitParameter_ref_formula.fitParameters_[iFitParameter_in_formula]->numFitParameter_;
    for ( int iFitParameter_in_const = 0; iFitParameter_in_const < numFitParameter_in_const; ++iFitParameter_in_const ) {
      
      std::vector<double> graph_xs;
      std::vector<double> graph_xErrsUp;
      std::vector<double> graph_xErrsDown;
      std::vector<double> graph_ys;
      std::vector<double> graph_yErrsUp;
      std::vector<double> graph_yErrsDown;
      
      int numMassPoints = massPoints.size();
      for ( int iMassPoint = 0; iMassPoint < numMassPoints; ++iMassPoint ) {
	double massPoint = massPoints[iMassPoint];

	const fitParameterType_formula& fitParameter_in_formula = fitParameter_in[massPoint];
	assert(fitParameter_in_formula.numFitParameter_ == numFitParameter_in_formula);
	const fitParameterType_const* fitParameter_in_const = dynamic_cast<const fitParameterType_const*>(
          fitParameter_in_formula.fitParameters_[iFitParameter_in_formula]);
	assert(fitParameter_in_const->numFitParameter_ == numFitParameter_in_const);

	double x         = massPoint;
	double xErrUp    = massPointErrsUp[iMassPoint];
	double xErrDown  = massPointErrsDown[iMassPoint];
      
	double y         = fitParameter_in_const->fitParameterValues_[iFitParameter_in_const];
	double yErr      = fitParameter_in_const->fitParameterErrors_[iFitParameter_in_const];

	if ( fitParameter_in_formula.fitStatus_ == 0 &&
	     fitParameter_in_const->fitStatus_  == 0 ) {
	  graph_xs.push_back(x);
	  graph_xErrsUp.push_back(xErrUp);
	  graph_xErrsDown.push_back(xErrDown);
	  graph_ys.push_back(y);
	  graph_yErrsUp.push_back(yErr);
	  graph_yErrsDown.push_back(yErr);
	}
      }

      size_t numPoints = graph_xs.size();
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

      double xMin = massPoints[0];
      double xMax = massPoints[numMassPoints - 1];
    
      TF1* fitFunction = 0;
      std::string fitOptions;
      if ( iFitParameter_in_formula == 0 ) {
        fitFunction = new TF1("fitFunction", "[0]/TMath::Power(TMath::Max(1.e-2, x - [1]), [2]) + [3]", xMin, xMax);
	fitFunction->SetParameter(0, 1.5);
	fitFunction->SetParameter(1, 1.e+2);
	fitFunction->SetParameter(2, 1.e-1);
	fitFunction->SetParameter(3, 0.);
      } else if ( iFitParameter_in_formula == 1 ) {
	if ( iFitParameter_in_const == 0 ) {
	  fitFunction = new TF1("fitFunction", "[0]", xMin, xMax);
	} else if ( iFitParameter_in_const == 1 ) {
	  fitFunction = new TF1("fitFunction", "[0]", xMin, xMax);
	} else if ( iFitParameter_in_const == 2 ) {
	  fitFunction = new TF1("fitFunction", "[0]", xMin, xMax);
	} else assert(0);
      } else if ( iFitParameter_in_formula == 2 ) {
	fitFunction = new TF1("fitFunction", "[0]/([1] + x)", xMin, xMax);
      } else if ( iFitParameter_in_formula == 3 ) {
	if ( iFitParameter_in_const == 0 ) {
	  fitFunction = new TF1("fitFunction", "[0]*0.5*([1] - TMath::Erf(-[2]*(x - [3])*TMath::Power(TMath::Abs(x - [3]), [4])))", xMin, xMax);
	  fitFunction->SetParameter(0, 1.5e-1);
	  fitFunction->SetParameter(1, -2.e-1);
	  fitFunction->SetParameter(2, 1.e-1);
	  fitFunction->SetParameter(3, 1.5e+2);
	  fitFunction->SetParameter(4, 1.e-1);
	} else if ( iFitParameter_in_const == 1 ) {
	  fitFunction = new TF1("fitFunction", "TMath::Max([0]*(x - [1]), [2])", xMin, xMax);
	  fitFunction->SetParameter(0, -5.e-4);
	  fitFunction->SetParameter(1, 1.5e+2);
	  fitFunction->SetParameter(2, -1.e-2);
	} else assert(0);
      } else if ( iFitParameter_in_formula == 4 ) {
	if ( iFitParameter_in_const == 0 ) {
	  fitFunction = new TF1("fitFunction", "[0]", xMin, xMax);
	} else if ( iFitParameter_in_const == 1 ) {
	  fitFunction = new TF1("fitFunction", "[0]", xMin, xMax);
	} else assert(0);
      } else assert(0);
      fitOptions.append("0");
      TFitResultPtr fitResult = graph->Fit(fitFunction, fitOptions.data());
      tmpFitParameter_out.push_back(fitParameterType_const("mass", fitFunction, xMin, xMax, (int)fitResult));

      double yMin = +1.e+6;
      double yMax = -1.e+6;
      for ( size_t iPoint = 0; iPoint < numPoints; ++iPoint ) {
	double x, y;
	graph->GetPoint(iPoint, x, y);
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
      std::string yAxisTitle = Form("p_{%i%i}", iFitParameter_in_formula, iFitParameter_in_const);
      
      TAxis* xAxis = dummyHistogram->GetXaxis();
      xAxis->SetTitle(xAxisTitle.data());
      xAxis->SetTitleOffset(1.15);
      
      TAxis* yAxis = dummyHistogram->GetYaxis();
      yAxis->SetTitle(yAxisTitle.data());
      yAxis->SetTitleOffset(1.30);
      
      dummyHistogram->Draw("axis");
      
      fitFunction->SetLineColor(2);
      fitFunction->SetLineWidth(2);
      fitFunction->Draw("same");
      
      graph->SetMarkerStyle(20);
      graph->SetMarkerColor(1);
      graph->Draw("P");
      
      canvas->Update();
      size_t idx = outputFileName.find_last_of('.');
      TString outputFileName_param = std::string(outputFileName, 0, idx).data();
      outputFileName_param = outputFileName_param.ReplaceAll("%i", "%i%i");
      outputFileName_param = Form(outputFileName_param.Data(), iFitParameter_in_formula, iFitParameter_in_const);
      if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_param).append(std::string(outputFileName, idx)).data());
      canvas->Print(std::string(outputFileName_param).append(".png").data());
      canvas->Print(std::string(outputFileName_param).append(".pdf").data());
      canvas->Print(std::string(outputFileName_param).append(".root").data());
      
      delete fitFunction;
      delete dummyHistogram;
      delete canvas;
    }

    if ( fitParameter_out ) {      
      fitParameterType_formula* fitParameter_out_const = new fitParameterType_formula();
      const fitParameterType_const* fitParameter_ref_const = dynamic_cast<const fitParameterType_const*>(
        fitParameter_ref_formula.fitParameters_[iFitParameter_in_formula]);
      fitParameter_out_const->fitParameterTypeBase::operator=(*fitParameter_ref_const);
      fitParameter_out_const->fitParameters_.resize(numFitParameter_in_const);
      for ( int iFitParameter_in_const = 0; iFitParameter_in_const < numFitParameter_in_const; ++iFitParameter_in_const ) {
	fitParameter_out_const->fitParameters_[iFitParameter_in_const] = tmpFitParameter_out[iFitParameter_in_const].clone();
      }
      fitParameter_out->fitParameters_[iFitParameter_in_formula] = fitParameter_out_const;
    }
  }

  //if ( fitParameter_out ) fitParameter_out->dump(std::cout, 0);
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
  TString histogramName_dPhi12       = "histogramLeg12dPhi";

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

  const double visPtCut1 = 15.;
  const double visPtCut2 = 20.;
  
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
  std::vector<double> massPointErrsUp;
  std::vector<double> massPointErrsDown;
  
  std::map<std::string, std::map<double, fitParameterType_const> > fitParameterX1_dPhi_mass; // keys = dPhiLabel, mass-point
  std::map<double, fitParameterType_formula> fitParameterX1_mass;                            // key = mass-point
  fitParameterType_formula fitParameterX1;
  std::map<std::string, std::map<double, fitParameterType_const> > fitParameterX2_dPhi_mass; // keys = dPhiLabel, mass-point
  std::map<double, fitParameterType_formula> fitParameterX2_mass;                            // key = mass-point
  fitParameterType_formula fitParameterX2;

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
    
    std::cout << "events:" << std::endl;
    std::cout << " before cuts = " << integral_EventCounter_beforeVisPtCuts << std::endl;
    std::cout << " after leg1-cuts = " << integral_EventCounter_afterVisPtCutsLeg1 
	      << " (" << (integral_EventCounter_afterVisPtCutsLeg1/integral_EventCounter_beforeVisPtCuts)*100. << "%)" << std::endl;
    std::cout << " after leg2-cuts = " << integral_EventCounter_afterVisPtCutsLeg2
	      << " (" << (integral_EventCounter_afterVisPtCutsLeg2/integral_EventCounter_beforeVisPtCuts)*100. << "%)" << std::endl;
    std::cout << " after leg1&2-cuts = " << integral_EventCounter_afterVisPtCutsLeg1and2
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
	      visPtCut1, massPoint,
	      Form("plots/plotSVfitVisPtCut_X1_m%1.0f.eps", massPoint),
	      NULL);

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
	      visPtCut2, massPoint,
	      Form("plots/plotSVfitVisPtCut_X2_m%1.0f.eps", massPoint),
	      NULL);

    for ( int legIdx = 1; legIdx <= 2; ++legIdx ) {
      std::string directory_afterVisPtCutsLeg1or2;
      double visPtCut = 0.;
      std::map<std::string, std::map<double, fitParameterType_const> >* fitParameterX_dPhi_mass = NULL;
      std::map<double, fitParameterType_formula>* fitParameterX_mass = NULL;
      if ( legIdx == 1 ) {
	directory_afterVisPtCutsLeg1or2 = directory_afterVisPtCutsLeg1;
	visPtCut = visPtCut1;
	fitParameterX_dPhi_mass = &fitParameterX1_dPhi_mass;
	fitParameterX_mass = &fitParameterX1_mass;
      } else if ( legIdx == 2 ) {
	directory_afterVisPtCutsLeg1or2 = directory_afterVisPtCutsLeg2;
	visPtCut = visPtCut2;
	fitParameterX_dPhi_mass = &fitParameterX2_dPhi_mass;
	fitParameterX_mass = &fitParameterX2_mass;
      } else assert(0);
      std::vector<double> dPhiValues;
      std::vector<double> dPhiErrsUp;
      std::vector<double> dPhiErrsDown;
      std::map<double, fitParameterType_const> tmpFitParameterX_dPhi; // key = dPhi 
      for ( std::vector<pdouble>::const_iterator xRange_dPhi = xRanges_dPhi.begin();
	    xRange_dPhi != xRanges_dPhi.end(); ++xRange_dPhi ) {
	double dPhiMin_deg = xRange_dPhi->first;
	double dPhiMin_rad = dPhiMin_deg*TMath::DegToRad();
	double dPhiMax_deg = xRange_dPhi->second;
	double dPhiMax_rad = dPhiMax_deg*TMath::DegToRad();
	const double epsilon = 1.e-3;
	std::string dPhiLabel;
	if      ( dPhiMin_rad <                epsilon  ) dPhiLabel = Form("dPhiLt%0.0f", dPhiMax_deg);
	else if ( dPhiMax_rad > (TMath::Pi() - epsilon) ) dPhiLabel = Form("dPhiGt%0.0f", dPhiMin_deg);
	else                                              dPhiLabel = Form("dPhi%0.0fto%0.0f", dPhiMin_deg, dPhiMax_deg); 
	std::string histogramName_X_dPhi = Form("histogramLeg%iX%s", legIdx, dPhiLabel.data());
	std::string plotLabel_X_dPhi = Form("X%i_%s", legIdx, dPhiLabel.data());
	TH1* histogram_X_dPhi_beforeVisPtCuts = 
	  getHistogram(inputFile, directory_beforeVisPtCuts, massPoint, histogramName_X_dPhi);      
	TH1* histogram_X_dPhi_afterVisPtCutsLeg1or2 = 
	  getHistogram(inputFile, directory_afterVisPtCutsLeg1or2, massPoint, histogramName_X_dPhi);
	TH1* histogram_X_dPhi_afterVisPtCutsLeg1and2 = 
	  getHistogram(inputFile, directory_afterVisPtCutsLeg1and2, massPoint, histogramName_X_dPhi);   	
    	makePlotX(legIdx, 
		  histogram_X_dPhi_beforeVisPtCuts, 
		  histogram_X_dPhi_afterVisPtCutsLeg1or2, 
		  histogram_X_dPhi_afterVisPtCutsLeg1and2,
		  visPtCut, massPoint,
		  Form("plots/plotSVfitVisPtCut_%s_m%1.0f.eps", plotLabel_X_dPhi.data(), massPoint),
		  &(*fitParameterX_dPhi_mass)[plotLabel_X_dPhi][massPoint]);
	TH1* histogram_dPhi =
	  getHistogram(inputFile, directory_beforeVisPtCuts, massPoint, histogramName_dPhi12.Data());
	int bin0 = histogram_dPhi->FindBin(dPhiMin_rad);
	int bin1 = histogram_dPhi->FindBin(dPhiMax_rad);
	histogram_dPhi->GetXaxis()->SetRange(bin0, bin1);
	double dPhiMean_rad = histogram_dPhi->GetMean();
	dPhiValues.push_back(dPhiMean_rad);
	dPhiErrsUp.push_back(dPhiMean_rad - dPhiMin_rad);
	dPhiErrsDown.push_back(dPhiMax_rad - dPhiMean_rad);
	tmpFitParameterX_dPhi[dPhiMean_rad] = (*fitParameterX_dPhi_mass)[plotLabel_X_dPhi][massPoint];
      }
      showGraphDPhi(Form("X%i", legIdx),
		    dPhiValues, dPhiErrsUp, dPhiErrsDown, 
		    tmpFitParameterX_dPhi,
		    &(*fitParameterX_mass)[massPoint],
		    Form("plots/plotSVfitVisPtCut_p%sfit1dX%ivsDPhi_m%1.0f.eps", "%i", legIdx, massPoint));
    }

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
 
  if ( massPoints.size() >= 3 ) {
    showGraphMass("X1",
		  massPoints, massPointErrsUp, massPointErrsDown, 
		  fitParameterX1_mass,  
		  &fitParameterX1,
		  "plots/plotSVfitVisPtCut_p%ifit1dX1vsMass.eps");
    showGraphMass("X2",
		  massPoints, massPointErrsUp, massPointErrsDown, 
		  fitParameterX2_mass,
		  &fitParameterX2,
		  "plots/plotSVfitVisPtCut_p%ifit1dX2vsMass.eps");
  }

  delete inputFile;

  std::string outputFileName_txt = "plots/paramSVfitVisPtCut.txt";
  std::ofstream* outputFile_txt = new std::ofstream(outputFileName_txt.data(), std::ios::out);
  (*outputFile_txt) << "visPtCutCorrectionLeg1 = cms.PSet(" << std::endl;
  fitParameterX1.dump(*outputFile_txt, 4);
  (*outputFile_txt) << ")" << std::endl;
  (*outputFile_txt) << std::endl;
  (*outputFile_txt) << "visPtCutCorrectionLeg2 = cms.PSet(" << std::endl;
  fitParameterX2.dump(*outputFile_txt, 4);
  (*outputFile_txt) << ")" << std::endl;
  delete outputFile_txt;
}
