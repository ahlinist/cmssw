#ifndef TauAnalysis_RecoTools_fitZllRecoilCorrectionAuxFunctions_h
#define TauAnalysis_RecoTools_fitZllRecoilCorrectionAuxFunctions_h

#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TF1.h>
#include <TMath.h>

#include <string>
#include <vector>

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
					  const TH2* histogram_uParl, const TH1* histogram_qT)
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
    
    if ( x > 0. ) {
      double y = -histogram_uParl_proj->GetMean()/x;
      double yErr = histogram_uParl_proj->GetMeanError()/x;
      graph->SetPoint(iBin - 1, x, y);
      graph->SetPointError(iBin - 1, xErrDown, xErrUp, yErr, yErr);
    }
  }
  
  return graph;
}

//
//-------------------------------------------------------------------------------
//

TF1* fitGraph_uParl_dix_qT(const std::string& name, TGraph* graph, double xMin = 0., double xMax = 300.)
{
  TF1* f = new TF1(name.data(), "[0]*0.5*(1.0 - TMath::Erf(-[1]*TMath::Power(x, [2])))", xMin, xMax);
  f->SetLineWidth(0);
  f->SetParameter(0, -1.0);
  f->SetParameter(1,  5.e-2);
  f->SetParameter(2,  1.0);
  graph->Fit(f, "E");
  return f;
}

TF1* fitGraph_uParl_mean(const std::string& name, TGraph* graph, double xMin = 0., double xMax = 300.)
{
  TF1* f = new TF1(name.data(), "[0]*x*0.5*(1.0 - TMath::Erf(-[1]*TMath::Power(x, [2])))", xMin, xMax);
  f->SetLineWidth(0);
  f->SetParameter(0, -1.0);
  f->SetParameter(1,  5.e-2);
  f->SetParameter(2,  1.0);
  graph->Fit(f, "E");
  return f;
}

TF1* fitGraph_uParl_rms(const std::string& name, TGraph* graph, double xMin = 0., double xMax = 300.)
{
  TF1* f = new TF1(name.data(), "[0]*(1.0 + [1]*x + [2]*x*x)", xMin, xMax);
  f->SetLineWidth(0);
  f->SetParameter(0, 10.);
  f->SetParameter(1,  5.e-2);
  f->SetParameter(2,  0.);
  graph->Fit(f, "E");
  return f;
}

TF1* fitGraph_uPerp_rms(const std::string& name, TGraph* graph, double xMin = 0., double xMax = 300.)
{
  TF1* f = new TF1(name.data(), "[0]*(1.0 + [1]*x + [2]*x*x)", xMin, xMax);
  f->SetLineWidth(0);
  f->SetParameter(0, 10.);
  f->SetParameter(1,  5.e-2);
  f->SetParameter(2,  0.);
  graph->Fit(f, "E");
  return f;
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
					    const std::vector<TGraphAsymmErrors*>& graphs_sysUncertainty, bool center_on_central_value)
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
      assert(xUp == x);

      const TGraphAsymmErrors* graphDown = graphs_sysUncertainty[2*iSysUncertainty + 1];
      assert(graphDown->GetN() == numPoints);
      double xDown, yDown;
      graphDown->GetPoint(iPoint, xDown, yDown);
      double yErrDown_high = graphDown->GetErrorYhigh(iPoint); 
      double yErrDown_low = graphDown->GetErrorYlow(iPoint); 
      assert(xDown == x);

      double yMin = TMath::Min(yUp + yErrUp_high, yDown + yErrDown_high);
      double yMax = TMath::Max(yUp - yErrUp_low, yDown - yErrDown_low);
      if ( yMin < y ) yErrMin2 += square(y - yMin);
      if ( yMax > y ) yErrMax2 += square(yMax - y);
    }

    if ( !center_on_central_value ) graph->SetPoint(iPoint, x, y);
    else graph->SetPoint(iPoint, x, 0.);
    graph->SetPointError(iPoint, xErrDown, xErrUp, TMath::Sqrt(yErrMin2), TMath::Sqrt(yErrMax2));
  }

  return graph;
}

#endif
