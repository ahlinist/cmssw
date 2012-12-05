#ifndef TauAnalysis_RecoTools_fitZllRecoilCorrectionAuxFunctions_h
#define TauAnalysis_RecoTools_fitZllRecoilCorrectionAuxFunctions_h

#include "FWCore/Utilities/interface/Exception.h"

#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TF1.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TMath.h>

#include <string>
#include <vector>
#include <assert.h>

enum { kMean, kRMS };

namespace 
{
  double square(double x)
  {
    return x*x;
  }
}

//
//-------------------------------------------------------------------------------
//

TGraphAsymmErrors* makeGraph_mean_or_rms(const std::string& name, const std::string& title, 
					 const TH2* histogram_u, const TH1* histogram_qT, int mode)
{
  const TAxis* xAxis = histogram_u->GetXaxis();
  int numBins = xAxis->GetNbins();

  TGraphAsymmErrors* graph = new TGraphAsymmErrors(numBins);
  graph->SetName(name.data());
  graph->SetTitle(title.data());  

  for ( int iBin = 1; iBin <= numBins; ++iBin ) {
    double qTmin = xAxis->GetBinLowEdge(iBin);
    double qTmax = xAxis->GetBinUpEdge(iBin);

    int binLowIndex = const_cast<TH1*>(histogram_qT)->FindBin(qTmin);
    int binUpIndex  = const_cast<TH1*>(histogram_qT)->FindBin(qTmax);
    histogram_qT->GetXaxis()->SetRange(binLowIndex, binUpIndex);

    double x        = histogram_qT->GetMean();
    double xErrUp   = qTmax - x;
    double xErrDown = x - qTmin;

    TString histogramName_u_proj = Form("%s_py_%i", histogram_u->GetName(), iBin);
    TH1D* histogram_u_proj = histogram_u->ProjectionY(histogramName_u_proj.Data(), iBin, iBin, "e");
    // CV: skip qT bins with limited event statistics
    if ( !(histogram_u_proj->GetEntries() >= 100) ) continue;

    double y = 0.;
    double yErr = 0.;
    if ( mode == kMean ) {
      y = histogram_u_proj->GetMean();
      yErr = histogram_u_proj->GetMeanError();
    } else if ( mode == kRMS ) {
      y = histogram_u_proj->GetRMS();
      yErr = histogram_u_proj->GetRMSError();
    } else assert (0);

    graph->SetPoint(iBin - 1, x, y);
    graph->SetPointError(iBin - 1, xErrDown, xErrUp, yErr, yErr);
  }

  // reset x-axis range selection 
  histogram_qT->GetXaxis()->SetRange(1., 0.);

  return graph;
}

TGraphAsymmErrors* makeGraph_mean(const std::string& name, const std::string& title, 
				  const TH2* histogram_u, const TH1* histogram_qT)
{
  return makeGraph_mean_or_rms(name, title, histogram_u, histogram_qT, kMean);
}

TGraphAsymmErrors* makeGraph_rms(const std::string& name, const std::string& title, 
				 const TH2* histogram_u, const TH1* histogram_qT)
{
  return makeGraph_mean_or_rms(name, title, histogram_u, histogram_qT, kRMS);
}

TGraphAsymmErrors* makeGraph_uParl_div_qT(const std::string& name, const std::string& title, 
					  const TH2* histogram_uParl, const TH1* histogram_qT,
					  bool isCaloMEt)
{
  const TAxis* xAxis = histogram_uParl->GetXaxis();
  int numBins = xAxis->GetNbins();

  TGraphAsymmErrors* graph = new TGraphAsymmErrors(numBins);
  graph->SetName(name.data());
  graph->SetTitle(title.data());  

  for ( int iBin = 1; iBin <= numBins; ++iBin ) {
    double qTmin = xAxis->GetBinLowEdge(iBin);
    double qTmax = xAxis->GetBinUpEdge(iBin);

    int binLowIndex = const_cast<TH1*>(histogram_qT)->FindBin(qTmin);
    int binUpIndex  = const_cast<TH1*>(histogram_qT)->FindBin(qTmax);
    histogram_qT->GetXaxis()->SetRange(binLowIndex, binUpIndex);

    double x        = histogram_qT->GetMean();
    double xErrUp   = qTmax - x;
    double xErrDown = x - qTmin;

    TString histogramName_uParl_proj = Form("%s_py_%i", histogram_uParl->GetName(), iBin);
    TH1D* histogram_uParl_proj = histogram_uParl->ProjectionY(histogramName_uParl_proj.Data(), iBin, iBin, "e");
    // CV: skip qT bins with limited event statistics
    if ( !(histogram_uParl_proj->GetEntries() >= 100) ) continue;

    if ( x > 0. ) {
      double y = -histogram_uParl_proj->GetMean()/x;    
      if ( isCaloMEt ) y -= 1.; 
      double yErr = histogram_uParl_proj->GetMeanError()/x;
      graph->SetPoint(iBin - 1, x, y);      
      graph->SetPointError(iBin - 1, xErrDown, xErrUp, yErr, yErr);
    }
  }
  
  return graph;
}

TGraphAsymmErrors* makeGraph_metXorY_vs_sumEt(const std::string& name, const std::string& title, 
					      const TH2* histogram_metXorY, const TH1* histogram_sumEt)
{
  return makeGraph_mean_or_rms(name, title, histogram_metXorY, histogram_sumEt, kRMS);
}

//
//-------------------------------------------------------------------------------
//

TF1* fitGraph_uParl_div_qT(const std::string& name, TGraph* graph, double xMin = 0., double xMax = 300.)
{
  TF1* f = new TF1(name.data(), "[0]*0.5*(1.0 - TMath::Erf(-[1]*TMath::Power(x, [2])))", xMin, xMax);
  f->SetLineWidth(0);
  f->SetParameter(0, 1.0);
  f->SetParameter(1, 5.e-2);
  f->SetParameter(2, 1.0);
  TGraph* dummyGraph = (TGraph*)graph->Clone(); // CV: fit 'dummyGraph', to avoid TF1 getting attached to 'graph' object
  dummyGraph->Fit(f, "E");
  return f;
}

TF1* fitGraph_uParl_mean(const std::string& name, TGraph* graph, double xMin = 0., double xMax = 300.)
{
  TF1* f = new TF1(name.data(), "[0]*x*0.5*(1.0 - TMath::Erf(-[1]*TMath::Power(x, [2])))", xMin, xMax);
  f->SetLineWidth(0);
  f->SetParameter(0, -1.0);
  f->SetParameter(1,  5.e-2);
  f->SetParameter(2,  1.0);
  TGraph* dummyGraph = (TGraph*)graph->Clone(); // CV: fit 'dummyGraph', to avoid TF1 getting attached to 'graph' object
  dummyGraph->Fit(f, "E");
  return f;
}

TF1* fitGraph_uParl_rms(const std::string& name, TGraph* graph, double xMin = 0., double xMax = 300.)
{
  TF1* f = new TF1(name.data(), "[0]*(1.0 + [1]*x + [2]*x*x)", xMin, xMax);
  f->SetLineWidth(0);
  f->SetParameter(0, 10.);
  f->SetParameter(1,  5.e-2);
  f->SetParameter(2,  0.);
  TGraph* dummyGraph = (TGraph*)graph->Clone(); // CV: fit 'dummyGraph', to avoid TF1 getting attached to 'graph' object
  dummyGraph->Fit(f, "E");
  return f;
}

TF1* fitGraph_uPerp_rms(const std::string& name, TGraph* graph, double xMin = 0., double xMax = 300.)
{
  TF1* f = new TF1(name.data(), "[0]*(1.0 + [1]*x + [2]*x*x)", xMin, xMax);
  f->SetLineWidth(0);
  f->SetParameter(0, 10.);
  f->SetParameter(1,  5.e-2);
  f->SetParameter(2,  0.);
  TGraph* dummyGraph = (TGraph*)graph->Clone(); // CV: fit 'dummyGraph', to avoid TF1 getting attached to 'graph' object
  dummyGraph->Fit(f, "E");
  return f;
}

TF1* fitGraph_metXorY_vs_sumEt(const std::string& name, TGraph* graph, double xMin = 0., double xMax = 2500.)
{
  TF1* f = new TF1(name.data(), "[0]*[0]*TMath::Power(x - [1]*[1], [2]*[2]) + [3]", xMin, xMax);
  f->SetLineWidth(0);
  f->SetParameter(0, TMath::Sqrt(0.50));
  //f->SetParameter(1, 0.);
  f->FixParameter(1, 0.);
  f->SetParameter(2, TMath::Sqrt(0.5));
  //f->SetParameter(3, 0.);
  f->FixParameter(3, 0.);
  TGraph* dummyGraph = (TGraph*)graph->Clone(); // CV: fit 'dummyGraph', to avoid TF1 getting attached to 'graph' object
  dummyGraph->Fit(f, "E");
  std::cout << "<fitGraph_metXorY_vs_sumEt>:" << std::endl;
  std::cout << " name = " << name << std::endl;
  std::cout << " p0 = " << square(f->GetParameter(0)) << " +/- " << square(f->GetParError(0)) << std::endl;
  std::cout << " p1 = " << square(f->GetParameter(1)) << " +/- " << square(f->GetParError(1)) << std::endl;
  std::cout << " p2 = " << square(f->GetParameter(2)) << " +/- " << square(f->GetParError(2)) << std::endl;
  std::cout << " p3 = " << f->GetParameter(3) << " +/- " << f->GetParError(3) << std::endl;  
  return f;
}

double getFitError(TF1* fit, double x)
{
  double fitError2 = 0.;

  double y0 = fit->Eval(x);
  unsigned numParameter = fit->GetNpar();
  for ( unsigned iParameter = 0; iParameter < numParameter; ++iParameter ) {
    double fitParameter = fit->GetParameter(iParameter);
    double fitParameterErr = fit->GetParError(iParameter);

    fit->SetParameter(iParameter, fitParameter + fitParameterErr);
    double yUp = fit->Eval(x);
    
    fit->SetParameter(iParameter, fitParameter - fitParameterErr);
    double yDown = fit->Eval(x);
    
    double yMin = TMath::Min(yUp, yDown);
    double yMax = TMath::Max(yUp, yDown);

    if ( !(TMath::IsNaN(yMin) || TMath::IsNaN(yMax)) ) fitError2 += (0.5*(square(y0 - yMin) + square(yMax - y0)));
    else if ( !TMath::IsNaN(yMin) ) fitError2 += square(y0 - yMin);
    else if ( !TMath::IsNaN(yMax) ) fitError2 += square(yMax - y0);

    fit->SetParameter(iParameter, fitParameter);
  }
  
  return TMath::Sqrt(fitError2);
}

//
//-------------------------------------------------------------------------------
//

std::vector<TF1*> makeFitUncertaintyFunctions(const TF1* f)
{
  std::vector<TF1*> retVal;

  int numParameter = f->GetNpar();
  for ( int iParameter = 0; iParameter < numParameter; ++iParameter ) {
    std::string nameUp = std::string(f->GetName()).append("_").append(f->GetParName(iParameter)).append("Up");
    TF1* fUp = (TF1*)f->Clone(nameUp.data());
    fUp->SetParameter(iParameter, f->GetParameter(iParameter) + f->GetParError(iParameter));
    retVal.push_back(fUp);

    std::string nameDown = std::string(f->GetName()).append("_").append(f->GetParName(iParameter)).append("Down");
    TF1* fDown = (TF1*)f->Clone(nameDown.data());
    fDown->SetParameter(iParameter, f->GetParameter(iParameter) - f->GetParError(iParameter));
    retVal.push_back(fDown);
  }

  return retVal;
}

//
//-------------------------------------------------------------------------------
//

TGraphAsymmErrors* makeGraph_fitValue(const std::string& name, const std::string& title, 
				      const TF1* f_central_value, 
				      double xMin = 0., double xMax = 300., double dx = 1.)
{
  int numPoints = 1 + TMath::FloorNint((xMax - xMin)/dx);
  
  TGraphAsymmErrors* graph = new TGraphAsymmErrors(numPoints);
  graph->SetName(name.data());
  graph->SetTitle(title.data());  
  
  for ( int iPoint = 0; iPoint < numPoints; ++iPoint ) {
    double x = xMin + iPoint*dx;    
    
    double y = f_central_value->Eval(x);
    
    graph->SetPoint(iPoint, x, y);
    graph->SetPointError(iPoint, 0., 0., 0., 0.);
  }
  
  return graph;
}

TGraphAsymmErrors* makeGraph_fitUncertainty(const std::string& name, const std::string& title, 
					    const TF1* f_central_value, const std::vector<TF1*>& f_fitUncertainties,
					    double xMin = 0., double xMax = 300., double dx = 1.)
{
  assert((f_fitUncertainties.size() % 2) == 0);
  int numParameter = (f_fitUncertainties.size() / 2);
  
  int numPoints = 1 + TMath::FloorNint((xMax - xMin)/dx);
  
  TGraphAsymmErrors* graph = new TGraphAsymmErrors(numPoints);
  graph->SetName(name.data());
  graph->SetTitle(title.data());  
  
  for ( int iPoint = 0; iPoint < numPoints; ++iPoint ) {
    double x = xMin + iPoint*dx;    

    double y = f_central_value->Eval(x);
    
    double yErrMin2 = 0.;
    double yErrMax2 = 0.;
    for ( int iParameter = 0; iParameter < numParameter; ++iParameter ) {
      const TF1* fUp = f_fitUncertainties[2*iParameter];
      double yUp = fUp->Eval(x);

      const TF1* fDown = f_fitUncertainties[2*iParameter + 1];
      double yDown = fDown->Eval(x);
      
      double yMin = TMath::Min(yUp, yDown);
      double yMax = TMath::Max(yUp, yDown);
      if ( yMin < y ) yErrMin2 += square(y - yMin);
      if ( yMax > y ) yErrMax2 += square(yMax - y);
    }
    
    graph->SetPoint(iPoint, x, y);
    graph->SetPointError(iPoint, 0., 0., TMath::Sqrt(yErrMin2), TMath::Sqrt(yErrMax2));
  }
  
  return graph;
}

//
//-------------------------------------------------------------------------------
//

TGraphAsymmErrors* makeGraph_sysUncertainty(const TGraphAsymmErrors* graph_central_value, 
					    const std::vector<TGraphAsymmErrors*>& graphs_sysUncertainty, 
					    bool center_on_central_value, bool divide_by_central_value)
{
  assert((graphs_sysUncertainty.size() % 2) == 0);
  int numSysUncertainties = (graphs_sysUncertainty.size() / 2);

  int numPoints = graph_central_value->GetN();

  TGraphAsymmErrors* graph = new TGraphAsymmErrors(numPoints);
  for ( int iPoint = 0; iPoint < numPoints; ++iPoint ) {
    double x, y;
    graph_central_value->GetPoint(iPoint,x, y);
    double xErrUp = graph_central_value->GetErrorXhigh(iPoint); 
    double xErrDown = graph_central_value->GetErrorXlow(iPoint);

    double yErrMin2 = 0.;
    double yErrMax2 = 0.;
    for ( int iSysUncertainty = 0; iSysUncertainty < numSysUncertainties; ++iSysUncertainty ) {
      const TGraphAsymmErrors* graphUp = graphs_sysUncertainty[2*iSysUncertainty];
      assert(graphUp->GetN() == numPoints);
      double xUp, yUp;
      graphUp->GetPoint(iPoint, xUp, yUp);
      double yErrUp_high = graphUp->GetErrorYhigh(iPoint); 
      double yErrUp_low = graphUp->GetErrorYlow(iPoint); 
      //assert(xUp == x); // CV: x-positions of central value and systematic uncertainty graph may be slightly different,
                          //     in case x-position is computed as center of gravity of qT distribution
                          //    (events passing event selection for central value and systematic uncertainty may differ)

      const TGraphAsymmErrors* graphDown = graphs_sysUncertainty[2*iSysUncertainty + 1];
      assert(graphDown->GetN() == numPoints);
      double xDown, yDown;
      graphDown->GetPoint(iPoint, xDown, yDown);
      double yErrDown_high = graphDown->GetErrorYhigh(iPoint); 
      double yErrDown_low = graphDown->GetErrorYlow(iPoint); 
      //assert(xDown == x);

      double yMin = TMath::Min(yUp + yErrUp_high, yDown + yErrDown_high);
      double yMax = TMath::Max(yUp - yErrUp_low, yDown - yErrDown_low);
      if ( yMin < y ) yErrMin2 += square(y - yMin);
      if ( yMax > y ) yErrMax2 += square(yMax - y);
    }
    double yErrMin = TMath::Sqrt(yErrMin2);
    double yErrMax = TMath::Sqrt(yErrMax2);

    if ( !center_on_central_value ) graph->SetPoint(iPoint, x, y);
    else graph->SetPoint(iPoint, x, 0.);
    if ( !divide_by_central_value ) graph->SetPointError(iPoint, xErrDown, xErrUp, yErrMin, yErrMax);
    else if ( y != 0. ) graph->SetPointError(iPoint, xErrDown, xErrUp, yErrMin/TMath::Abs(y), yErrMax/TMath::Abs(y));
  }

  return graph;
}

//
//-------------------------------------------------------------------------------
//

TGraphAsymmErrors* makeGraph_diff(const TGraphAsymmErrors* graph, const TGraphAsymmErrors* graph_ref,
				  bool addRefErr, bool divideByRef)
{
//--- take binning of 'graph_diff' from 'graph'
  int numPoints = graph->GetN();
  TGraphAsymmErrors* graph_diff = new TGraphAsymmErrors(numPoints);

  for ( int iPoint = 0; iPoint < numPoints; ++iPoint ) {
    Double_t x, y;
    graph->GetPoint(iPoint, x, y);
    Double_t xErrUp = graph->GetErrorXhigh(iPoint);
    Double_t xErrDown = graph->GetErrorXlow(iPoint);
    Double_t yErrUp = graph->GetErrorYhigh(iPoint);
    Double_t yErrDown = graph->GetErrorYlow(iPoint);

//--- find point of 'graph_ref'
//    matching point of 'graph' closest in x
//   (do not assume that both graphs have the same binning)
    Double_t y_ref = 0.;
    Double_t yErrUp_ref = 0.;
    Double_t yErrDown_ref = 0.;
    Double_t dxMin = 1.e+6; 
    for ( int iPoint_ref = 0; iPoint_ref < graph_ref->GetN(); ++iPoint_ref ) {
      Double_t x_i, y_i;
      graph_ref->GetPoint(iPoint_ref, x_i, y_i);
      
      Double_t dx = x_i - x;
      if ( TMath::Abs(dx) < dxMin ) {
	y_ref = y_i;
	yErrUp_ref = graph_ref->GetErrorYhigh(iPoint_ref);
	yErrDown_ref = graph_ref->GetErrorYlow(iPoint_ref);
	dxMin = TMath::Abs(dx);
      }
    }

    Double_t y_diff = 0.;
    Double_t yErrUp2_diff = 0.;
    Double_t yErrDown2_diff = 0.;
    if ( divideByRef ) {
      if ( y_ref != 0. ) {
	y_diff = (y/y_ref) - 1.;
	if ( y != 0. ) yErrUp2_diff += square(yErrUp/y);
	if ( addRefErr ) yErrUp2_diff += square(yErrDown_ref/y_ref);
	yErrUp2_diff *= (y/y_ref);
	if ( y != 0. ) yErrDown2_diff += square(yErrDown/y);
	if ( addRefErr ) yErrDown2_diff += square(yErrUp_ref/y_ref);
	yErrDown2_diff *= (y/y_ref);
      }
    } else {
      y_diff = y - y_ref;	
      yErrUp2_diff = square(yErrUp);
      if ( addRefErr ) yErrUp2_diff += square(yErrDown_ref);
      yErrDown2_diff = square(yErrDown);
      if ( addRefErr ) yErrDown2_diff += square(yErrUp_ref);
    }
    
    graph_diff->SetPoint(iPoint, x, y_diff);
    graph_diff->SetPointError(iPoint, xErrDown, xErrUp, TMath::Sqrt(yErrDown2_diff), TMath::Sqrt(yErrUp2_diff));
  }

  return graph_diff;
}

//
//-------------------------------------------------------------------------------
//

void cloneStyleOptions(TGraphAsymmErrors* graph, const TGraphAsymmErrors* graph_ref)
{
  graph->SetMarkerStyle(graph_ref->GetMarkerStyle());
  graph->SetMarkerSize(graph_ref->GetMarkerSize());
  graph->SetMarkerColor(graph_ref->GetMarkerColor());
  graph->SetLineColor(graph_ref->GetLineColor());
  graph->SetLineWidth(graph_ref->GetLineWidth());
  graph->SetFillColor(graph_ref->GetFillColor());
  graph->SetFillStyle(graph_ref->GetFillStyle());
}

template <typename T>
void applyStyleOptions(T* obj, int color, int markerStyle, int markerSize, int lineStyle, int lineWidth)
{
  obj->SetMarkerStyle(markerStyle);
  obj->SetMarkerSize(markerSize);
  obj->SetMarkerColor(color);
  obj->SetLineStyle(lineStyle);
  obj->SetLineWidth(lineWidth);
  obj->SetLineColor(color);
}

void drawZllRecoilFitResult(
       TCanvas* canvas, 
       TH1* dummyHistogram, 
       const std::string& plotLabel,
       TGraphAsymmErrors* graph_central_value, TF1* fitFunction_central_value,
       const std::string& legendEntry, double legendX0, double legendY0,
       bool showIdLine, bool showConstLine, 
       const std::string& yAxisLabel, double yMin, double yMax, bool yDiffDivideByRef, double yDiffMax, 
       const std::string& outputFileName, const std::string& outputFileLabel,
       const std::vector<TGraphAsymmErrors*>* graphs_sysUncertainty = 0, const std::vector<TF1*>* fitFunctions_sysUncertainty = 0)
{
  TAxis* xAxis = dummyHistogram->GetXaxis();
  double xMin = xAxis->GetXmin();
  double xMax = xAxis->GetXmax();
  double dx = 1.;
  
  TGraphAsymmErrors* graph_fit_central_value_top = 
    makeGraph_fitValue("graph_fit_central_value", "",
		       fitFunction_central_value, xMin, xMax, dx);
  TGraphAsymmErrors* graph_fit_central_value_bottom = 
    makeGraph_diff(graph_fit_central_value_top, graph_fit_central_value_top, false, yDiffDivideByRef);
  TGraphAsymmErrors* graph_fitErr_central_value_top = 
    makeGraph_fitUncertainty("graph_fitErr_central_value", "", 
			     fitFunction_central_value, makeFitUncertaintyFunctions(fitFunction_central_value), xMin, xMax, dx);
  TGraphAsymmErrors* graph_fitErr_central_value_bottom = 
    makeGraph_diff(graph_fitErr_central_value_top, graph_fit_central_value_top, false, yDiffDivideByRef);
  
  TGraphAsymmErrors* graph_central_value_bottom = 
    makeGraph_diff(graph_central_value, graph_fit_central_value_top, false, yDiffDivideByRef);

  TGraphAsymmErrors* graphErr_sysUncertainty_top = 0;
  TGraphAsymmErrors* graphErr_sysUncertainty_bottom = 0;
  if ( graphs_sysUncertainty ) {
    graphErr_sysUncertainty_top = 
      makeGraph_sysUncertainty(graph_central_value, *graphs_sysUncertainty, false, false);
    TGraphAsymmErrors* graph_tmp = 
      makeGraph_sysUncertainty(graph_central_value, *graphs_sysUncertainty, false, yDiffDivideByRef);
    graphErr_sysUncertainty_bottom = 
      makeGraph_diff(graph_tmp, graph_fit_central_value_top, false, yDiffDivideByRef);
  }

  TGraphAsymmErrors* graph_fitErr_sysUncertainty_top = 0;
  TGraphAsymmErrors* graph_fitErr_sysUncertainty_bottom = 0;
  if ( fitFunctions_sysUncertainty ) {
    std::vector<TGraphAsymmErrors*> graphs_fitErr_sysUncertainty;
    for ( std::vector<TF1*>::const_iterator fitFunction_sysUncertainty = fitFunctions_sysUncertainty->begin();
	  fitFunction_sysUncertainty != fitFunctions_sysUncertainty->end(); ++fitFunction_sysUncertainty ) {
      TGraphAsymmErrors* graph_fitErr_sysUncertainty =
	makeGraph_fitUncertainty("graph_fitErr_sysUncertainty", "", 
				 *fitFunction_sysUncertainty, makeFitUncertaintyFunctions(*fitFunction_sysUncertainty), xMin, xMax, dx);
      graphs_fitErr_sysUncertainty.push_back(graph_fitErr_sysUncertainty);
    }
    graph_fitErr_sysUncertainty_top = 
      makeGraph_sysUncertainty(graph_fit_central_value_top, graphs_fitErr_sysUncertainty, false, false);
    graph_fitErr_sysUncertainty_bottom = 
      makeGraph_sysUncertainty(graph_fit_central_value_top, graphs_fitErr_sysUncertainty, true, yDiffDivideByRef);
  }

  canvas->SetLogy(false);
  canvas->Clear();
  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);

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
  bottomPad->SetBottomMargin(0.24);
  bottomPad->SetRightMargin(0.05);

  canvas->cd();
  topPad->Draw();
  topPad->cd();

  TH1* dummyHistogram_top = (TH1*)dummyHistogram->Clone("dummyHistogram_top");

  TAxis* xAxis_top = dummyHistogram_top->GetXaxis();
  xAxis_top->SetLabelColor(10);
  xAxis_top->SetTitleColor(10);
    
  TAxis* yAxis_top = dummyHistogram_top->GetYaxis();
  yAxis_top->SetTitle(yAxisLabel.data());
  yAxis_top->SetTitleOffset(1.20);
  yAxis_top->SetTitleSize(0.06);

  dummyHistogram_top->SetTitle("");
  dummyHistogram_top->SetStats(false);
  dummyHistogram_top->SetMaximum(yMax);
  dummyHistogram_top->SetMinimum(yMin);
  dummyHistogram_top->SetMarkerColor(1);
  dummyHistogram_top->SetMarkerStyle(20);
  dummyHistogram_top->Draw("axis");

  if ( graph_fitErr_sysUncertainty_top ) {
    graph_fitErr_sysUncertainty_top->SetLineColor(396);
    graph_fitErr_sysUncertainty_top->SetLineWidth(0);
    graph_fitErr_sysUncertainty_top->SetFillColor(396);
    graph_fitErr_sysUncertainty_top->SetFillStyle(1001);
    graph_fitErr_sysUncertainty_top->Draw("3");
  }
  
  graph_fitErr_central_value_top->SetLineColor(797);
  graph_fitErr_central_value_top->SetLineWidth(0);
  graph_fitErr_central_value_top->SetFillColor(797);
  graph_fitErr_central_value_top->SetFillStyle(1001);
  graph_fitErr_central_value_top->Draw("3");

  graph_fit_central_value_top->SetLineColor(28);
  graph_fit_central_value_top->SetLineWidth(2);
  graph_fit_central_value_top->Draw("L");

  if ( showIdLine || showConstLine ) {
    int numPoints = graph_fit_central_value_top->GetN();
    TGraph* graph_line = new TGraph(numPoints);
    for ( int iPoint = 0; iPoint < numPoints; ++iPoint ) {
      Double_t x, y;
      graph_fit_central_value_top->GetPoint(iPoint, x, y);
      if      ( showIdLine    ) graph_line->SetPoint(iPoint, x,  -x);
      else if ( showConstLine ) graph_line->SetPoint(iPoint, x, 1.0);
      else assert(0);
    }
    graph_line->SetLineColor(8);
    graph_line->SetLineWidth(1);
    graph_line->Draw("L");
  }

  if ( graphErr_sysUncertainty_top ) {
    graphErr_sysUncertainty_top->SetMarkerStyle(20);
    graphErr_sysUncertainty_top->SetMarkerSize(1);
    graphErr_sysUncertainty_top->SetMarkerColor(1);
    graphErr_sysUncertainty_top->SetLineColor(1);
    graphErr_sysUncertainty_top->SetLineWidth(1);
    graphErr_sysUncertainty_top->Draw("P");
  }

  graph_central_value->SetMarkerStyle(20);
  graph_central_value->SetMarkerSize(1);
  graph_central_value->SetMarkerColor(1);
  graph_central_value->SetLineColor(1);
  graph_central_value->SetLineWidth(1);
  graph_central_value->Draw("P");

  double legendX1 = legendX0 + 0.30;
  double legendY1 = ( graph_fitErr_sysUncertainty_top ) ?
    legendY0 + 0.330 : legendY0 + 0.275;
  TLegend legend(legendX0, legendY0, legendX1, legendY1, "", "brNDC"); 
  legend.SetBorderSize(0);
  legend.SetFillColor(0);
  legend.AddEntry(graph_central_value,            legendEntry.data(), "p");
  legend.AddEntry(graph_fit_central_value_top,    "Fit",              "l");
  legend.AddEntry(graph_fitErr_central_value_top, "Fit Uncertainty",  "f");
  if ( graph_fitErr_sysUncertainty_top ) 
    legend.AddEntry(graph_fitErr_sysUncertainty_top, "Sys. Uncertainty",  "f");
  legend.Draw();
  
  TPaveText* plotLabel_text = 0;
  if ( plotLabel != "" ) {
    double plotLabelX0 = ( legendX0 < 0.50 ) ? 0.64 : 0.15;
    double plotLabelY0 = 0.86;
    plotLabel_text = new TPaveText(plotLabelX0, plotLabelY0, plotLabelX0 + 0.25, plotLabelY0 + 0.06, "NDC");
    plotLabel_text->SetBorderSize(0);
    plotLabel_text->SetFillColor(0);
    plotLabel_text->AddText(plotLabel.data());
    plotLabel_text->SetTextSize(0.055);
    plotLabel_text->Draw();
  }

  canvas->cd();
  bottomPad->Draw();
  bottomPad->cd();

  TH1* dummyHistogram_bottom = (TH1*)dummyHistogram->Clone("dummyHistogram_bottom");

  TAxis* xAxis_bottom = dummyHistogram_bottom->GetXaxis();
  xAxis_bottom->SetTitle("q_{T} / GeV");
  xAxis_bottom->SetTitleOffset(1.20);
  xAxis_bottom->SetTitleSize(0.08);
  xAxis_bottom->SetLabelOffset(0.02);
  xAxis_bottom->SetLabelSize(0.08);
  xAxis_bottom->SetTickLength(0.055);

  TAxis* yAxis_bottom = dummyHistogram_bottom->GetYaxis();
  if ( yDiffDivideByRef ) yAxis_bottom->SetTitle("#frac{Measured - Fit}{Fit}");
  else  yAxis_bottom->SetTitle("Measured - Fit");
  yAxis_bottom->SetTitleOffset(0.85);
  yAxis_bottom->SetNdivisions(505);
  yAxis_bottom->CenterTitle();
  yAxis_bottom->SetTitleSize(0.08);
  yAxis_bottom->SetLabelSize(0.08);
  yAxis_bottom->SetTickLength(0.04);

  dummyHistogram_bottom->SetTitle("");
  dummyHistogram_bottom->SetStats(false);
  dummyHistogram_bottom->SetMaximum(+yDiffMax);
  dummyHistogram_bottom->SetMinimum(-yDiffMax);
  dummyHistogram_bottom->SetMarkerColor(1);
  dummyHistogram_bottom->SetMarkerStyle(20);
  dummyHistogram_bottom->Draw("axis");

  if ( graph_fitErr_sysUncertainty_bottom ) {
    cloneStyleOptions(graph_fitErr_sysUncertainty_bottom, graph_fitErr_sysUncertainty_top);
    graph_fitErr_sysUncertainty_bottom->Draw("3");
  }

  cloneStyleOptions(graph_fitErr_central_value_bottom, graph_fitErr_central_value_top);
  graph_fitErr_central_value_bottom->Draw("3");
  
  cloneStyleOptions(graph_fit_central_value_bottom, graph_fit_central_value_top);
  graph_fit_central_value_bottom->Draw("L");

  if ( graphErr_sysUncertainty_bottom ) {
    cloneStyleOptions(graphErr_sysUncertainty_bottom, graphErr_sysUncertainty_top);
    graphErr_sysUncertainty_bottom->Draw("P");
  }
  
  cloneStyleOptions(graph_central_value_bottom, graph_central_value);
  graph_central_value_bottom->Draw("P");

  canvas->Update();

  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  outputFileName_plot.append("_").append(outputFileLabel);
  if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_plot).append(std::string(outputFileName, idx)).data());
  canvas->Print(std::string(outputFileName_plot).append(".png").data());
  canvas->Print(std::string(outputFileName_plot).append(".pdf").data());
  canvas->Print(std::string(outputFileName_plot).append(".root").data());

  delete dummyHistogram_top;
  delete graph_fit_central_value_top;
  delete graph_fitErr_central_value_top;
  delete graphErr_sysUncertainty_top;
  delete graph_fitErr_sysUncertainty_top;
  delete plotLabel_text;
  delete topPad;
  delete dummyHistogram_bottom;
  delete graph_fit_central_value_bottom;
  delete graph_fitErr_central_value_bottom;
  delete graph_central_value_bottom;
  delete graphErr_sysUncertainty_bottom;
  delete graph_fitErr_sysUncertainty_bottom;
  delete bottomPad;
}

void drawZllRecoilFitResult(
       TCanvas* canvas, 
       TH1* dummyHistogram, 
       std::vector<TGraphAsymmErrors*>& graphs_mc, std::vector<TF1*>& fitFunctions_mc, bool showGraphs_mc, bool showFits_mc,
       std::vector<TGraphAsymmErrors*>& graphs_data, std::vector<TF1*>& fitFunctions_data, bool showGraphs_data, bool showFits_data,
       const std::vector<std::string>& legendEntries, double legendX0, double legendY0,
       bool showIdLine, bool showConstLine, 
       const std::string& yAxisLabel, double yMin, double yMax, 
       const std::string& outputFileName, const std::string& outputFileLabel)
{
  assert(graphs_mc.size()         == legendEntries.size());
  assert(fitFunctions_mc.size()   == legendEntries.size());
  assert(graphs_data.size()       == legendEntries.size());
  assert(fitFunctions_data.size() == legendEntries.size());
  
  canvas->SetLogy(false);
  canvas->Clear();
  canvas->SetLeftMargin(0.15);
  canvas->SetBottomMargin(0.15);

  TAxis* xAxis = dummyHistogram->GetXaxis();
  xAxis->SetTitle("q_{T} / GeV");
  xAxis->SetTitleOffset(1.15);
  xAxis->SetTitleSize(0.06);

  TAxis* yAxis = dummyHistogram->GetYaxis();
  yAxis->SetTitle(yAxisLabel.data());
  yAxis->SetTitleOffset(1.15);
  yAxis->SetTitleSize(0.06);

  dummyHistogram->SetTitle("");
  dummyHistogram->SetStats(false);
  dummyHistogram->SetMaximum(yMax);
  dummyHistogram->SetMinimum(yMin);
  dummyHistogram->SetMarkerColor(1);
  dummyHistogram->SetMarkerStyle(20);
  dummyHistogram->Draw("axis");

  int colors[4] = { kOrange + 9, 1, 38, 896 };
  int markerStyles[4] = { 20, 24, 25, 26 };
  int markerStyles_mc[4] = { 20, 21, 22, 33 };
  int markerStyles_data[4] = { 24, 25, 26, 27 };
  int lineStyles[4] = { 1, 1, 1, 1 };
  int lineStyles_mc[4] = { 2, 2, 2, 2 };
  int lineStyles_data[4] = { 1, 1, 1, 1 };

  size_t numEntries = legendEntries.size();
  if ( numEntries > 4 )
    throw cms::Exception("drawZllRecoilFitResult") 
      << "Maximum number of plots supported = 4, requested = " << numEntries << "!!\n";

  if ( showIdLine || showConstLine ) {
    int numPoints = graphs_data.front()->GetN();
    TGraph* graph_line = new TGraph(numPoints);
    for ( int iPoint = 0; iPoint < numPoints; ++iPoint ) {
      Double_t x, y;
      graphs_data.front()->GetPoint(iPoint, x, y);
      if      ( showIdLine    ) graph_line->SetPoint(iPoint, x,  -x);
      else if ( showConstLine ) graph_line->SetPoint(iPoint, x, 1.0);
      else assert(0);
    }
    graph_line->SetLineColor(8);
    graph_line->SetLineWidth(1);
    graph_line->Draw("L");
  }

  // CV: draw all fits first...
  for ( size_t iEntry = 0; iEntry < numEntries; ++iEntry ) {
    if ( showFits_mc ) {
      TF1* fitFunction_mc = fitFunctions_mc[iEntry];
      if ( showFits_data ) applyStyleOptions(fitFunction_mc, colors[iEntry], 1, 1, lineStyles_mc[iEntry], 2);
      else applyStyleOptions(fitFunction_mc, colors[iEntry], 1, 1, lineStyles[iEntry], 2);
      fitFunction_mc->Draw("L");
    }

    if ( showFits_data ) {
      TF1* fitFunction_data = fitFunctions_data[iEntry];
      if ( showFits_mc ) applyStyleOptions(fitFunction_data, colors[iEntry], 1, 1, lineStyles_data[iEntry], 2);
      else applyStyleOptions(fitFunction_data, colors[iEntry], 1, 1, lineStyles[iEntry], 2);
      fitFunction_data->Draw("P");
    }
  }

  for ( size_t iEntry = 0; iEntry < numEntries; ++iEntry ) {
    if ( showGraphs_mc ) {
      TGraphAsymmErrors* graph_mc = graphs_mc[iEntry];
      if ( showGraphs_data ) applyStyleOptions(graph_mc, colors[iEntry], markerStyles_mc[iEntry], 2, 1, 1);
      else applyStyleOptions(graph_mc, colors[iEntry], markerStyles[iEntry], 1, 1, 1);
      graph_mc->Draw("P");
    }

    if ( showGraphs_data ) {
      TGraphAsymmErrors* graph_data = graphs_data[iEntry];
      if ( showGraphs_mc ) applyStyleOptions(graph_data, colors[iEntry], markerStyles_data[iEntry], 2, 1, 1);
      else applyStyleOptions(graph_data, colors[iEntry], markerStyles[iEntry], 1, 1, 1);
      graph_data->Draw("P");
    }
  }
  
  double legendX1 = legendX0 + 0.28;
  double legendY1 = legendY0 + 0.275;
  TLegend legend(legendX0, legendY0, legendX1, legendY1, "", "brNDC"); 
  legend.SetBorderSize(0);
  legend.SetFillColor(0);
  for ( size_t iEntry = 0; iEntry < numEntries; ++iEntry ) {
    if      ( showGraphs_data ) legend.AddEntry(graphs_data[iEntry],       legendEntries[iEntry].data(), "p");
    else if ( showGraphs_mc   ) legend.AddEntry(graphs_mc[iEntry],         legendEntries[iEntry].data(), "p");
    else if ( showFits_data   ) legend.AddEntry(fitFunctions_data[iEntry], legendEntries[iEntry].data(), "p");
    else if ( showGraphs_data ) legend.AddEntry(fitFunctions_mc[iEntry],   legendEntries[iEntry].data(), "p");
  }
  legend.Draw();

  canvas->Update();

  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  outputFileName_plot.append("_").append(outputFileLabel);
  if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_plot).append(std::string(outputFileName, idx)).data());
  canvas->Print(std::string(outputFileName_plot).append(".png").data());
  canvas->Print(std::string(outputFileName_plot).append(".pdf").data());
  canvas->Print(std::string(outputFileName_plot).append(".root").data());
}

void drawData_vs_MCcomparison(
       TCanvas* canvas, 
       TH1* dummyHistogram, 
       const std::string& plotLabel,
       const std::string& legendEntry_data, TGraphAsymmErrors* graph_data, 
       const std::string& legendEntry_mc_signal, TGraphAsymmErrors* graph_mc_signal, 
       const std::string& legendEntry_mc, TGraphAsymmErrors* graph_mc, 
       double legendX0, double legendY0,
       bool showIdLine, bool showConstLine, 
       const std::string& yAxisLabel, double yMin, double yMax, bool yDiffDivideByRef, double yDiffMax, 
       const std::string& outputFileName, const std::string& outputFileLabel,
       const std::vector<TGraphAsymmErrors*>* graphs_mcSysUncertainty = 0)
{
  TGraphAsymmErrors* graph_diff = makeGraph_diff(graph_data, graph_mc, true, yDiffDivideByRef);

  TGraphAsymmErrors* graph_mcErr_sysUncertainty_top = 0;
  TGraphAsymmErrors* graph_mcErr_sysUncertainty_bottom = 0;
  if ( graphs_mcSysUncertainty ) {
    graph_mcErr_sysUncertainty_top = makeGraph_sysUncertainty(graph_mc, *graphs_mcSysUncertainty, false, false);
    graph_mcErr_sysUncertainty_bottom = makeGraph_sysUncertainty(graph_mc, *graphs_mcSysUncertainty, true, yDiffDivideByRef);
  }

  canvas->SetLogy(false);
  canvas->Clear();
  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);

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
  bottomPad->SetBottomMargin(0.24);
  bottomPad->SetRightMargin(0.05);

  canvas->cd();
  topPad->Draw();
  topPad->cd();

  TH1* dummyHistogram_top = (TH1*)dummyHistogram->Clone("dummyHistogram_top");

  TAxis* xAxis_top = dummyHistogram_top->GetXaxis();
  xAxis_top->SetLabelColor(10);
  xAxis_top->SetTitleColor(10);
    
  TAxis* yAxis_top = dummyHistogram_top->GetYaxis();
  yAxis_top->SetTitle(yAxisLabel.data());
  yAxis_top->SetTitleOffset(1.20);
  yAxis_top->SetTitleSize(0.06);

  dummyHistogram_top->SetTitle("");
  dummyHistogram_top->SetStats(false);
  dummyHistogram_top->SetMaximum(yMax);
  dummyHistogram_top->SetMinimum(yMin);
  dummyHistogram_top->SetMarkerColor(1);
  dummyHistogram_top->SetMarkerStyle(20);
  dummyHistogram_top->Draw("axis");

  if ( showIdLine || showConstLine ) {
    int numPoints = graph_data->GetN();
    TGraph* graph_line = new TGraph(numPoints);
    for ( int iPoint = 0; iPoint < numPoints; ++iPoint ) {
      Double_t x, y;
      graph_data->GetPoint(iPoint, x, y);
      if      ( showIdLine    ) graph_line->SetPoint(iPoint, x,  -x);
      else if ( showConstLine ) graph_line->SetPoint(iPoint, x, 1.0);
      else assert(0);
    }
    graph_line->SetLineColor(8);
    graph_line->SetLineWidth(1);
    graph_line->Draw("L");
  }

  graph_mc_signal->SetMarkerStyle(27);
  graph_mc_signal->SetMarkerSize(1);
  graph_mc_signal->SetMarkerColor(1);
  graph_mc_signal->SetLineColor(1);
  graph_mc_signal->SetLineWidth(1);

  graph_mc->SetMarkerStyle(24);
  graph_mc->SetMarkerSize(1);
  graph_mc->SetMarkerColor(1);
  graph_mc->SetLineColor(1);
  graph_mc->SetLineWidth(1);

  if ( graph_mcErr_sysUncertainty_top ) {
    graph_mcErr_sysUncertainty_top->SetLineColor(396);
    graph_mcErr_sysUncertainty_top->SetLineWidth(0);
    graph_mcErr_sysUncertainty_top->SetFillColor(396);
    graph_mcErr_sysUncertainty_top->SetFillStyle(1001);
    graph_mcErr_sysUncertainty_top->Draw("2");
  }

  //graph_mc_signal->Draw("P");

  graph_mc->Draw("P");

  graph_data->SetMarkerStyle(20);
  graph_data->SetMarkerSize(1);
  graph_data->SetMarkerColor(1);
  graph_data->SetLineColor(1);
  graph_data->SetLineWidth(1);
  graph_data->Draw("P");

  double legendX1 = legendX0 + 0.30;
  double legendY1 = ( graph_mcErr_sysUncertainty_top ) ?
    legendY0 + 0.270 : legendY0 + 0.215;
  TLegend legend(legendX0, legendY0, legendX1, legendY1, "", "brNDC"); 
  legend.SetBorderSize(0);
  legend.SetFillColor(0);
  legend.AddEntry(graph_data,      legendEntry_data.data(),      "p");
  //legend.AddEntry(graph_mc_signal, legendEntry_mc_signal.data(), "p");
  legend.AddEntry(graph_mc,        legendEntry_mc.data(),        "p");
  legend.Draw();

  TPaveText* plotLabel_text = 0;
  if ( plotLabel != "" ) {
    double plotLabelX0 = ( legendX0 < 0.50 ) ? 0.64 : 0.15;
    double plotLabelY0 = 0.86;
    plotLabel_text = new TPaveText(plotLabelX0, plotLabelY0, plotLabelX0 + 0.25, plotLabelY0 + 0.06, "NDC");
    plotLabel_text->SetBorderSize(0);
    plotLabel_text->SetFillColor(0);
    plotLabel_text->AddText(plotLabel.data());
    plotLabel_text->SetTextSize(0.055);
    plotLabel_text->Draw();
  }

  canvas->cd();
  bottomPad->Draw();
  bottomPad->cd();

  TH1* dummyHistogram_bottom = (TH1*)dummyHistogram->Clone("dummyHistogram_bottom");

  TAxis* xAxis_bottom = dummyHistogram_bottom->GetXaxis();
  xAxis_bottom->SetTitle("q_{T} / GeV");
  xAxis_bottom->SetTitleOffset(1.20);
  xAxis_bottom->SetTitleSize(0.08);
  xAxis_bottom->SetLabelOffset(0.02);
  xAxis_bottom->SetLabelSize(0.08);
  xAxis_bottom->SetTickLength(0.055);

  TAxis* yAxis_bottom = dummyHistogram_bottom->GetYaxis();
  if ( yDiffDivideByRef ) yAxis_bottom->SetTitle("#frac{Data - Simulation}{Simulation}");
  else  yAxis_bottom->SetTitle("Data - Simulation");
  yAxis_bottom->SetTitleOffset(0.85);
  yAxis_bottom->SetNdivisions(505);
  yAxis_bottom->CenterTitle();
  yAxis_bottom->SetTitleSize(0.08);
  yAxis_bottom->SetLabelSize(0.08);
  yAxis_bottom->SetTickLength(0.04);

  dummyHistogram_bottom->SetTitle("");
  dummyHistogram_bottom->SetStats(false);
  dummyHistogram_bottom->SetMaximum(+yDiffMax);
  dummyHistogram_bottom->SetMinimum(-yDiffMax);
  dummyHistogram_bottom->SetMarkerColor(1);
  dummyHistogram_bottom->SetMarkerStyle(20);
  dummyHistogram_bottom->Draw("axis");

  if ( graph_mcErr_sysUncertainty_bottom ) {
    cloneStyleOptions(graph_mcErr_sysUncertainty_bottom, graph_mcErr_sysUncertainty_top);
    graph_mcErr_sysUncertainty_bottom->Draw("2");
  }
  
  cloneStyleOptions(graph_diff, graph_data);
  graph_diff->Draw("P");

  canvas->Update();

  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  outputFileName_plot.append("_").append(outputFileLabel);
  if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_plot).append(std::string(outputFileName, idx)).data());
  canvas->Print(std::string(outputFileName_plot).append(".png").data());
  canvas->Print(std::string(outputFileName_plot).append(".pdf").data());
  canvas->Print(std::string(outputFileName_plot).append(".root").data());

  delete dummyHistogram_top;
  delete graph_mcErr_sysUncertainty_top;
  delete plotLabel_text;
  delete topPad;
  delete dummyHistogram_bottom;
  delete graph_diff;
  delete graph_mcErr_sysUncertainty_bottom;
  delete bottomPad;
}

#endif
