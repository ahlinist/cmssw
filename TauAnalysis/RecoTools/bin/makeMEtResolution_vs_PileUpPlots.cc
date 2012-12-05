
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "TauAnalysis/RecoTools/bin/fitZllRecoilCorrectionAuxFunctions.h"

#include <TFile.h>
#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TMath.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TBenchmark.h>

#include <string>
#include <map>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <assert.h>

const double p0_data        =  0.8208;
const double p0ErrUp_data   =  0.0358;
const double p0ErrDown_data =  0.0358;
const double p1_data        =  0.7189;
const double p1ErrUp_data   =  0.0074;
const double p1ErrDown_data =  0.0074;
const double p2_data        = -0.0010;
const double p2ErrUp_data   =  0.0003;
const double p2ErrDown_data =  0.0003;

const double p0_mc          =  1.0002;
const double p0ErrUp_mc     =  0.0005;
const double p0ErrDown_mc   =  0.0005;
const double p1_mc          =  0.7067;
const double p1ErrUp_mc     =  0.0008;
const double p1ErrDown_mc   =  0.0008;
const double p2_mc          = -0.0018;
const double p2ErrUp_mc     =  0.0001;
const double p2ErrDown_mc   =  0.0001;

const bool make_plots            = true;
const bool draw1_data            = true;
const bool draw1_mc              = true;
const bool draw2_data            = true;
const bool draw2_mc              = true;
const bool draw3_data            = true;
const bool draw3_mc              = true;
const bool compute_uncertainties = false;

enum { kGenNumPileUpInteractions, kRecVertices };
const int xAxis_mode = kRecVertices;

typedef std::map<std::string, double> valueMap1;
typedef std::map<std::string, valueMap1> valueMap2;
typedef std::map<std::string, valueMap2> valueMap3;

TH1* getHistogram(TFile* inputFile, const TString& dqmDirectory, const TString& meName)
{  
  //std::cout << "inputFile = " << inputFile->GetName() << std::endl;

  TString histogramName = TString(dqmDirectory);
  if ( !histogramName.EndsWith("/") ) histogramName.Append("/");
  histogramName.Append("beforeAddPUreweight/");
  histogramName.Append(meName);

  TH1* histogram = (TH1*)inputFile->Get(histogramName.Data());
  if ( !histogram ) {
    std::cout << "histogramName = " << histogramName.Data() << ": histogram = " << histogram;
    if ( histogram ) std::cout << ", integral = " << histogram->Integral();
    std::cout << std::endl; 
  }
  assert(histogram);

  if ( !histogram->GetSumw2N() ) histogram->Sumw2();

  return histogram;
}

TGraphErrors* compMEtResponse_vs_PileUp(TFile* inputFile, 
					TObjArray& processes, const std::string& central_or_shift,
					const TString& data_or_mcType, const TString& runPeriod, 
					double p0, double p1, double p2)
{
  std::cout << "<compMEtResponse_vs_PileUp>:" << std::endl;
  std::cout << " " << data_or_mcType.Data() << " " << runPeriod.Data() << std::endl;

  int numVertices = 0;
  if      ( std::string(runPeriod.Data()) == "2011runA"   ) numVertices = 24;
  else if ( std::string(runPeriod.Data()) == "2011runB"   ) numVertices = 24;
  else if ( std::string(runPeriod.Data()) == "2012runA"   ) numVertices = 34;
  else if ( std::string(runPeriod.Data()) == "2012runABC" ) numVertices = 34;
  else assert(0);

  TGraphErrors* graph = new TGraphErrors(numVertices);

  for ( int iVertex = 1; iVertex <= numVertices; ++iVertex ) {

    TH1* histogram_qT_sum = 0;
    TH2* histogram_uParl_vs_qT_sum = 0;
  
    int numProcesses = processes.GetEntries();
    for ( int iProcess = 0; iProcess < numProcesses; ++iProcess ) {
      TObjString* process = dynamic_cast<TObjString*>(processes.At(iProcess));
      
      TString dqmDirectory = process->GetString();
      if ( central_or_shift != "central" ) dqmDirectory.Append("/").Append(central_or_shift.data());
      
      TString histogramName_uParl_vs_qT_process = Form("uParlVsQtNumVerticesEq%i", iVertex);
      TH2* histogram_uParl_vs_qT_process = 
	dynamic_cast<TH2*>(getHistogram(inputFile, dqmDirectory, histogramName_uParl_vs_qT_process));
      if ( histogram_uParl_vs_qT_sum ) {
	histogram_uParl_vs_qT_sum->Add(histogram_uParl_vs_qT_process);
      } else {
	histogram_uParl_vs_qT_sum = (TH2*)histogram_uParl_vs_qT_process->Clone(
          TString(histogram_uParl_vs_qT_process->GetName()).Append("sum"));
      }

      TString histogramName_qT_process = Form("qTNumVerticesEq%i", iVertex);
      TH1* histogram_qT_process = 
	getHistogram(inputFile, dqmDirectory, histogramName_qT_process);
      if ( histogram_qT_sum ) {
	histogram_qT_sum->Add(histogram_qT_process);
      } else {
	histogram_qT_sum = (TH1*)histogram_qT_process->Clone(
          TString(histogram_qT_process->GetName()).Append("sum"));
      }           
    }

    if ( !(histogram_qT_sum->GetEntries() > 1.e+3) ) continue;

    TGraphAsymmErrors* graph_uParl_vs_qT_mean = 
      makeGraph_mean_or_rms("graph_uParl_mean", 
			    "graph_uParl_mean", histogram_uParl_vs_qT_sum, histogram_qT_sum, kMean);

    TF1* fit_response = fitGraph_uParl_mean("fit_response", graph_uParl_vs_qT_mean);

    double response_plateau = -fit_response->GetParameter(0);
    double responseErr_plateau = fit_response->GetParError(0);

    double x = -1.;
    if ( xAxis_mode == kGenNumPileUpInteractions ) {
      assert(p1 > 0.);
      if ( p2 != 0. ) {
	if ( (p1*p1 + 4*iVertex*p2 - 4*p0*p2) < 0. ) continue;
	x = (-p1 + TMath::Sqrt(p1*p1 + 4*iVertex*p2 - 4*p0*p2))/(2*p2);
      } else {
	x = (iVertex - p0)/p1;
      }
    } else if ( xAxis_mode == kRecVertices ) {
      x = iVertex;
    } else assert(0);
    if ( x < 0. ) continue;
    //std::cout << "p0 = " << p0 << ", p1 = " << p1 << ", p2 = " << p2 << ":" 
    //	        << " numVtx = " << iVertex << " --> numPU = " << x << std::endl;
    double y = response_plateau;
    double yErr = responseErr_plateau;
    
    std::cout << "x = " << x << ": y = " << y << " +/- " << yErr << std::endl;

    graph->SetPoint(iVertex - 1, x, y);
    graph->SetPointError(iVertex - 1, 0., yErr);

    delete graph_uParl_vs_qT_mean;
    delete fit_response;
  }

  TString graphName = Form("graph_%s_%s", data_or_mcType.Data(), runPeriod.Data());
  graph->SetName(graphName.Data());

  return graph;
}

TGraphErrors* compMEtResponse_vs_PileUp_data(TFile* inputFile, 
					     const std::string& central_or_shift, const TString& runPeriod)
{
  TObjArray processes;
  processes.Add(new TObjString("Data"));
  double p0 = p0_data;
  if ( central_or_shift == "vertexRecoEff_p0Up"   ) p0 += p0ErrUp_data;
  if ( central_or_shift == "vertexRecoEff_p0Down" ) p0 -= p0ErrDown_data;
  double p1 = p1_data;
  if ( central_or_shift == "vertexRecoEff_p1Up"   ) p1 += p1ErrUp_data;
  if ( central_or_shift == "vertexRecoEff_p1Down" ) p1 -= p1ErrDown_data;
  double p2 = p2_data;
  if ( central_or_shift == "vertexRecoEff_p2Up"   ) p2 += p2ErrUp_data;
  if ( central_or_shift == "vertexRecoEff_p2Down" ) p2 -= p2ErrDown_data;
  std::string central_or_shift_mod = ( central_or_shift.find("vertexRecoEff") == std::string::npos ) ?
    central_or_shift : "central";
  TGraphErrors* graph = compMEtResponse_vs_PileUp(inputFile, processes, central_or_shift_mod , 
						  "data", runPeriod, p0, p1, p2);
  if ( central_or_shift != "central" ) graph->SetName(Form("%s_%s", graph->GetName(), central_or_shift.data()));
  return graph;
}

TGraphErrors* compMEtResponse_vs_PileUp_mc(TFile* inputFile, const std::string& central_or_shift,
					   const TString& runPeriod)
{
  TObjArray processes;
  processes.Add(new TObjString("ZplusJets_madgraph"));
  processes.Add(new TObjString("TTplusJets_madgraph"));
  processes.Add(new TObjString("Tbar_tW"));
  processes.Add(new TObjString("T_tW"));
  processes.Add(new TObjString("WW"));
  processes.Add(new TObjString("WZ"));
  processes.Add(new TObjString("ZZ"));
  processes.Add(new TObjString("QCD"));
  double p0 = p0_mc;
  if ( central_or_shift == "vertexRecoEff_p0Up"   ) p0 += p0ErrUp_mc;
  if ( central_or_shift == "vertexRecoEff_p0Down" ) p0 -= p0ErrDown_mc;
  double p1 = p1_mc;
  if ( central_or_shift == "vertexRecoEff_p1Up"   ) p1 += p1ErrUp_mc;
  if ( central_or_shift == "vertexRecoEff_p1Down" ) p1 -= p1ErrDown_mc;
  double p2 = p2_mc;
  if ( central_or_shift == "vertexRecoEff_p2Up"   ) p2 += p2ErrUp_mc;
  if ( central_or_shift == "vertexRecoEff_p2Down" ) p2 -= p2ErrDown_mc;
  std::string central_or_shift_mod = ( central_or_shift.find("vertexRecoEff") == std::string::npos ) ?
    central_or_shift : "central";
  TGraphErrors* graph = compMEtResponse_vs_PileUp(inputFile, processes, central_or_shift_mod , 
						  "mc", runPeriod, p0, p1, p2);
  if ( central_or_shift != "central" ) graph->SetName(Form("%s_%s", graph->GetName(), central_or_shift.data()));
  return graph;
}

TGraphErrors* compMEtResolution_vs_PileUp(TFile* inputFile, 
					  TObjArray& processes, const std::string& central_or_shift,
					  const TString& data_or_mcType, const TString& runPeriod, const TString& projection,
					  double p0, double p1, double p2,
					  bool divide_by_response)
{
  std::cout << "<compMEtResolution_vs_PileUp>:" << std::endl;
  std::cout << " " << data_or_mcType.Data() << " " << runPeriod.Data() << " " << projection.Data() << std::endl;

  int numVertices = 0;
  if      ( std::string(runPeriod.Data()) == "2011runA"   ) numVertices = 24;
  else if ( std::string(runPeriod.Data()) == "2011runB"   ) numVertices = 24;
  else if ( std::string(runPeriod.Data()) == "2012runA"   ) numVertices = 34;
  else if ( std::string(runPeriod.Data()) == "2012runABC" ) numVertices = 34;
  else assert(0);

  TGraphErrors* graph = new TGraphErrors(numVertices);

  for ( int iVertex = 1; iVertex <= numVertices; ++iVertex ) {

    TH1* histogram_qT_sum = 0;
    TH2* histogram_uParl_vs_qT_sum = 0;
    TH2* histogram_projection_vs_qT_sum = 0;
  
    int numProcesses = processes.GetEntries();
    for ( int iProcess = 0; iProcess < numProcesses; ++iProcess ) {
      TObjString* process = dynamic_cast<TObjString*>(processes.At(iProcess));
      
      TString dqmDirectory = process->GetString();
      if ( central_or_shift != "central" ) dqmDirectory.Append("/").Append(central_or_shift.data());
      
      TString histogramName_uParl_vs_qT_process = Form("uParlVsQtNumVerticesEq%i", iVertex);
      TH2* histogram_uParl_vs_qT_process = 
	dynamic_cast<TH2*>(getHistogram(inputFile, dqmDirectory, histogramName_uParl_vs_qT_process));
      if ( histogram_uParl_vs_qT_sum ) {
	histogram_uParl_vs_qT_sum->Add(histogram_uParl_vs_qT_process);
      } else {
	histogram_uParl_vs_qT_sum = (TH2*)histogram_uParl_vs_qT_process->Clone(
          TString(histogram_uParl_vs_qT_process->GetName()).Append("sum"));
      }

      TString histogramName_projection_vs_qT_process = Form("%sVsQtNumVerticesEq%i", projection.Data(), iVertex);
      TH2* histogram_projection_vs_qT_process = 
	dynamic_cast<TH2*>(getHistogram(inputFile, dqmDirectory, histogramName_projection_vs_qT_process));
      if ( histogram_projection_vs_qT_sum ) {
	histogram_projection_vs_qT_sum->Add(histogram_projection_vs_qT_process);
      } else {
	histogram_projection_vs_qT_sum = (TH2*)histogram_projection_vs_qT_process->Clone(
          TString(histogram_projection_vs_qT_process->GetName()).Append("sum"));
      }

      TString histogramName_qT_process = Form("qTNumVerticesEq%i", iVertex);
      TH1* histogram_qT_process = 
	getHistogram(inputFile, dqmDirectory, histogramName_qT_process);
      if ( histogram_qT_sum ) {
	histogram_qT_sum->Add(histogram_qT_process);
      } else {
	histogram_qT_sum = (TH1*)histogram_qT_process->Clone(
          TString(histogram_qT_process->GetName()).Append("sum"));
      }           
    }

    if ( !(histogram_qT_sum->GetEntries() > 1.e+3) ) continue;

    TGraphAsymmErrors* graph_uParl_vs_qT_mean = 
      makeGraph_mean_or_rms("graph_uParl_mean", 
			    "graph_uParl_mean", histogram_uParl_vs_qT_sum, histogram_qT_sum, kMean);

    TF1* fit_response = fitGraph_uParl_mean("fit_response", graph_uParl_vs_qT_mean);

    // CV: use temporary histogram to compute RMS of uParl distribution
    //     around mean, given by uParl(qT) value of fitted response curve
    TString histogramName_1d = TString(histogram_projection_vs_qT_sum->GetName()).Append("_1d");
    TH1* histogram_1d = new TH1D(histogramName_1d.Data(), histogramName_1d.Data(), 75, -75., +75.);
    if ( !histogram_1d->GetSumw2N() ) histogram_1d->Sumw2();

    int numBinsX = histogram_projection_vs_qT_sum->GetNbinsX();
    for ( int iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
      double x = histogram_projection_vs_qT_sum->GetXaxis()->GetBinCenter(iBinX);

      double qTmin = histogram_projection_vs_qT_sum->GetXaxis()->GetBinLowEdge(iBinX);
      double qTmax = histogram_projection_vs_qT_sum->GetXaxis()->GetBinUpEdge(iBinX);

      int binLowIndex = histogram_qT_sum->FindBin(qTmin);
      int binUpIndex  = histogram_qT_sum->FindBin(qTmax);
      histogram_qT_sum->GetXaxis()->SetRange(binLowIndex, binUpIndex);
      if ( !(histogram_qT_sum->Integral(qTmin, qTmax) > 10. && histogram_qT_sum->GetMean() > 0.) ) continue;

      double response = -fit_response->Eval(x)/x;
      //double responseErr = getFitError(fit_response, x)/x;
      if ( !(response > 0.) ) continue;

      int numBinsY = histogram_projection_vs_qT_sum->GetNbinsY();
      for ( int iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
	double y = histogram_projection_vs_qT_sum->GetYaxis()->GetBinCenter(iBinY);
	
	double y_expected = 0.;
	if      ( projection == "uParl" ) y_expected = -response*x;
	else if ( projection == "uPerp" ) y_expected = 0.;
	else assert(0);
	
	double diff_y = y - y_expected;
	if ( divide_by_response ) diff_y /= response; 
	
	int bin_1d = histogram_1d->FindBin(diff_y);
	
	double binContent_1d = histogram_1d->GetBinContent(bin_1d);
	double binError_1d = histogram_1d->GetBinError(bin_1d);

	double addBinContent = histogram_projection_vs_qT_sum->GetBinContent(iBinX, iBinY);
	double addBinError = histogram_projection_vs_qT_sum->GetBinError(iBinX, iBinY);
	
	histogram_1d->SetBinContent(bin_1d, binContent_1d + addBinContent);
	histogram_1d->SetBinError(bin_1d, TMath::Sqrt(binError_1d*binError_1d + addBinError*addBinError));
      }
    }

    double resolution = histogram_1d->GetRMS();
    double resolutionErr = histogram_1d->GetRMSError();

    double x = -1.;
    if ( xAxis_mode == kGenNumPileUpInteractions ) {
      assert(p1 > 0.);
      if ( p2 != 0. ) {
	if ( (p1*p1 + 4*iVertex*p2 - 4*p0*p2) < 0. ) continue;
	x = (-p1 + TMath::Sqrt(p1*p1 + 4*iVertex*p2 - 4*p0*p2))/(2*p2);
      } else {
	x = (iVertex - p0)/p1;
      }
    } else if ( xAxis_mode == kRecVertices ) {
      x = iVertex;
    } else assert(0);
    if ( x < 0. ) continue;
    //std::cout << "p0 = " << p0 << ", p1 = " << p1 << ", p2 = " << p2 << ":" 
    //	        << " numVtx = " << iVertex << " --> numPU = " << x << std::endl;
    double y = resolution;
    double yErr = resolutionErr;
    
    std::cout << "x = " << x << ": y = " << y << " +/- " << yErr << std::endl;

    if ( y > 5. ) { // CV: reject "bad" points
      graph->SetPoint(iVertex - 1, x, y);
      graph->SetPointError(iVertex - 1, 0., yErr);
    }

    delete graph_uParl_vs_qT_mean;
    delete fit_response;
    delete histogram_1d;
  }

  TString graphName = Form("graph_%s_%s_%s", data_or_mcType.Data(), runPeriod.Data(), projection.Data());
  graph->SetName(graphName.Data());

  return graph;
}

TGraphErrors* compMEtResolution_vs_PileUp_data(TFile* inputFile, 
					       const std::string& central_or_shift, const TString& runPeriod, const TString& projection,
					       bool divide_by_response)
{
  TObjArray processes;
  processes.Add(new TObjString("Data"));
  double p0 = p0_data;
  if ( central_or_shift == "vertexRecoEff_p0Up"   ) p0 += p0ErrUp_data;
  if ( central_or_shift == "vertexRecoEff_p0Down" ) p0 -= p0ErrDown_data;
  double p1 = p1_data;
  if ( central_or_shift == "vertexRecoEff_p1Up"   ) p1 += p1ErrUp_data;
  if ( central_or_shift == "vertexRecoEff_p1Down" ) p1 -= p1ErrDown_data;
  double p2 = p2_data;
  if ( central_or_shift == "vertexRecoEff_p2Up"   ) p2 += p2ErrUp_data;
  if ( central_or_shift == "vertexRecoEff_p2Down" ) p2 -= p2ErrDown_data;
  std::string central_or_shift_mod = ( central_or_shift.find("vertexRecoEff") == std::string::npos ) ?
    central_or_shift : "central";
  TGraphErrors* graph = compMEtResolution_vs_PileUp(inputFile, processes, central_or_shift_mod , 
						    "data", runPeriod, projection, p0, p1, p2, divide_by_response);
  if ( central_or_shift != "central" ) graph->SetName(Form("%s_%s", graph->GetName(), central_or_shift.data()));
  return graph;
}

TGraphErrors* compMEtResolution_vs_PileUp_mc(TFile* inputFile, const std::string& central_or_shift,
					     const TString& runPeriod, const TString& projection,
					     bool divide_by_response)
{
  TObjArray processes;
  processes.Add(new TObjString("ZplusJets_madgraph"));
  processes.Add(new TObjString("TTplusJets_madgraph"));
  processes.Add(new TObjString("Tbar_tW"));
  processes.Add(new TObjString("T_tW"));
  processes.Add(new TObjString("WW"));
  processes.Add(new TObjString("WZ"));
  processes.Add(new TObjString("ZZ"));
  processes.Add(new TObjString("QCD"));
  double p0 = p0_mc;
  if ( central_or_shift == "vertexRecoEff_p0Up"   ) p0 += p0ErrUp_mc;
  if ( central_or_shift == "vertexRecoEff_p0Down" ) p0 -= p0ErrDown_mc;
  double p1 = p1_mc;
  if ( central_or_shift == "vertexRecoEff_p1Up"   ) p1 += p1ErrUp_mc;
  if ( central_or_shift == "vertexRecoEff_p1Down" ) p1 -= p1ErrDown_mc;
  double p2 = p2_mc;
  if ( central_or_shift == "vertexRecoEff_p2Up"   ) p2 += p2ErrUp_mc;
  if ( central_or_shift == "vertexRecoEff_p2Down" ) p2 -= p2ErrDown_mc;
  std::string central_or_shift_mod = ( central_or_shift.find("vertexRecoEff") == std::string::npos ) ?
    central_or_shift : "central";
  TGraphErrors* graph = compMEtResolution_vs_PileUp(inputFile, processes, central_or_shift_mod , 
						    "mc", runPeriod, projection, p0, p1, p2, divide_by_response);
  if ( central_or_shift != "central" ) graph->SetName(Form("%s_%s", graph->GetName(), central_or_shift.data()));
  return graph;
}

TF1* fitMEtResponse_vs_PileUp(TGraph* graph, const TString& runPeriod)
{
  double xMin = 0;
  double xMax = 0;
  if ( std::string(runPeriod.Data()) == "2011runA" ) { 
    xMin = 0.; 
    xMax = 25.;
  } else if ( std::string(runPeriod.Data()) == "2011runB" ) {
    xMin = 0.; 
    xMax = 25.;
  } else if ( std::string(runPeriod.Data()) == "2012runA" ) {
    xMin =  7.;
    xMax = 20.;
  } else if ( std::string(runPeriod.Data()) == "2012runABC" ) {
    xMin =  4.;
    xMax = 25.;
  } else assert(0);

  std::cout << "fitting " << graph->GetName() << ":" << std::endl;
  TString fitName = TString(graph->GetName()).Append("_fit");
  TF1* fit = new TF1(fitName.Data(), "[0] + [1]*x", xMin, xMax);
  fit->SetParameter(0, 0.9);
  fit->SetParameter(1, 0.e-2);
  graph->Fit(fit, "0");
  return fit;
}

TF1* fitMEtResolution_vs_PileUp(TGraph* graph, const TString& runPeriod)
{
  double xMin = 0;
  double xMax = 0;
  if ( std::string(runPeriod.Data()) == "2011runA" ) { 
    xMin = 0.; 
    xMax = 25.;
  } else if ( std::string(runPeriod.Data()) == "2011runB" ) {
    xMin = 0.; 
    xMax = 25.;
  } else if ( std::string(runPeriod.Data()) == "2012runA" ) {
    xMin =  7.;
    xMax = 20.;
  } else if ( std::string(runPeriod.Data()) == "2012runABC" ) {
    xMin =  4.;
    xMax = 25.;
  } else assert(0);

  std::cout << "fitting " << graph->GetName() << ":" << std::endl;
  TString fitName = TString(graph->GetName()).Append("_fit");
  TF1* fit = new TF1(fitName.Data(), "TMath::Sqrt([0]*[0] + TMath::Power(x, [1])*[2]*[2])", xMin, xMax);
  fit->SetParameter(0, 8.);
  fit->SetParameter(1, 1.);
  //fit->FixParameter(1, 1.);
  fit->SetParameter(2, 4.);
  graph->Fit(fit, "0");
  return fit;
}

void cloneStyleOptions_graph(TGraph* graph, const TGraph* graph_ref)
{
  graph->SetMarkerStyle(graph_ref->GetMarkerStyle());
  graph->SetMarkerSize(graph_ref->GetMarkerSize());
  graph->SetMarkerColor(graph_ref->GetMarkerColor());
  graph->SetLineColor(graph_ref->GetLineColor());
  graph->SetLineWidth(graph_ref->GetLineWidth());
  graph->SetFillColor(graph_ref->GetFillColor());
  graph->SetFillStyle(graph_ref->GetFillStyle());
}

TGraphErrors* makeGraph_data_div_mc(TGraphErrors* graph_data, TGraphErrors* graph_mc)
{
  TGraphErrors* graph_ref = 0;
  int numPoints_data = graph_data->GetN();
  int numPoints_mc = graph_mc->GetN();
  if ( numPoints_mc < numPoints_data ) {
    graph_ref = graph_mc;
  } else {
    graph_ref = graph_data;
  }
  int numPoints = graph_ref->GetN();

  TGraphErrors* graph_diff = new TGraphErrors(numPoints);
  
  for ( int iPoint = 0; iPoint < numPoints; ++iPoint ) {
    double x_ref, y_ref;
    graph_ref->GetPoint(iPoint, x_ref, y_ref);
    //std::cout << "x_ref  = " << x_ref << std::endl;

    int iPoint_data = -1;
    double dxMin_data = 1.e+3;
    for ( int iPoint_test = 0; iPoint_test < numPoints_data; ++iPoint_test ) {
      double x_test, y_test;
      graph_data->GetPoint(iPoint_test, x_test, y_test);
      //std::cout << "x_test(data) = " << x_test << std::endl;
      double dx = TMath::Abs(x_test - x_ref);
      if ( dx < dxMin_data ) {
	iPoint_data = iPoint_test;
	dxMin_data = dx;
      }
    }
    assert(iPoint_data >= 0);
    double x_data, y_data;
    graph_data->GetPoint(iPoint_data, x_data, y_data);
    double xErr_data = graph_data->GetErrorX(iPoint_data);
    double yErr_data = graph_data->GetErrorY(iPoint_data);
    
    int iPoint_mc = -1;
    double dxMin_mc = 1.e+3;
    for ( int iPoint_test = 0; iPoint_test < numPoints_mc; ++iPoint_test ) {
      double x_test, y_test;
      graph_mc->GetPoint(iPoint_test, x_test, y_test);
      //std::cout << "x_test(mc) = " << x_test << std::endl;
      double dx = TMath::Abs(x_test - x_ref);
      if ( dx < dxMin_mc ) {
	iPoint_mc = iPoint_test;
	dxMin_mc = dx;
      }
    }
    assert(iPoint_mc >= 0);
    double x_mc, y_mc;
    graph_mc->GetPoint(iPoint_mc, x_mc, y_mc);
    //double xErr_mc = graph_mc->GetErrorX(iPoint_mc);
    double yErr_mc = graph_mc->GetErrorY(iPoint_mc);

    if ( TMath::Nint(x_data - x_mc) > 0 ) continue;
    
    double y_diff = (y_data - y_mc)/y_mc;
    //std::cout << "x = " << x_data << ": y(data) = " << y_data << ", y(mc) = " << y_mc << " --> dy = " << y_diff << std::endl;
    double yErr2_diff = 0.;
    if ( y_data > 0. ) yErr2_diff += square(yErr_data/y_data);
    yErr2_diff += square(yErr_mc/y_mc);
    yErr2_diff *= square(y_data/y_mc);

    graph_diff->SetPoint(iPoint, x_data, y_diff);
    graph_diff->SetPointError(iPoint, xErr_data, TMath::Sqrt(yErr2_diff));
  }

  return graph_diff;
}

void cloneStyleOptions_fit(TF1* fit, const TF1* fit_ref)
{
  fit->SetLineColor(fit_ref->GetLineColor());
  fit->SetLineStyle(fit_ref->GetLineStyle());
  fit->SetLineWidth(fit_ref->GetLineWidth());
}

TF1* makeFit_data_div_mc(TF1* fit_data, TF1* fit_mc)
{
  TString fitName_diff = TString(fit_data->GetName()).Append("_div_").Append(fit_mc->GetName());
  TString formula_data = fit_data->GetTitle();
  int numParameter_data = fit_data->GetNpar();
  TString formula_mc = fit_mc->GetTitle();
  int numParameter_mc = fit_mc->GetNpar();
  for ( int iParameter_mc = numParameter_mc - 1; iParameter_mc >= 0; --iParameter_mc ) {
    TString parameterName_old = Form("[%i]", iParameter_mc);
    TString parameterName_new = Form("[%i]", iParameter_mc + numParameter_data);
    formula_mc.ReplaceAll(parameterName_old, parameterName_new);
  }
  TString formula_diff = TString("((").Append(formula_data).Append(")/(").Append(formula_mc).Append(")) - 1.0");
  double xMin_diff = TMath::Max(fit_data->GetXmin(), fit_mc->GetXmin());
  double xMax_diff = TMath::Min(fit_data->GetXmax(), fit_mc->GetXmax());
  TF1* fit_diff = new TF1(fitName_diff.Data(), formula_diff, xMin_diff, xMax_diff);
  for ( int iParameter_data = 0; iParameter_data < numParameter_data; ++iParameter_data ) {
    fit_diff->SetParameter(iParameter_data, fit_data->GetParameter(iParameter_data));
  }
  for ( int iParameter_mc = 0; iParameter_mc < numParameter_mc; ++iParameter_mc ) {
    fit_diff->SetParameter(iParameter_mc + numParameter_data, fit_mc->GetParameter(iParameter_mc));
  }
  return fit_diff;
}

void showPlot(TGraphErrors* graph1_data, TF1* fit1_data, TGraphErrors* graph1_mc, TF1* fit1_mc, const TString& legendEntry1,
	      TGraphErrors* graph2_data, TF1* fit2_data, TGraphErrors* graph2_mc, TF1* fit2_mc, const TString& legendEntry2,
	      TGraphErrors* graph3_data, TF1* fit3_data, TGraphErrors* graph3_mc, TF1* fit3_mc, const TString& legendEntry3,
	      double yMin, double yMax, const TString& yAxisTitle,
	      const std::string& outputFileName)
{
  //TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 720);
  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 1020);
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

  TPad* bottomPad = new TPad("bottomPad", "bottomPad", 0.00, 0.00, 1.00, 0.35);
  bottomPad->SetFillColor(10);
  bottomPad->SetTopMargin(0.02);
  bottomPad->SetLeftMargin(0.15);
  bottomPad->SetBottomMargin(0.24);
  bottomPad->SetRightMargin(0.05);

  canvas->cd();
  topPad->Draw();
  topPad->cd();

  double xAxisMin = 0.;
  double xAxisMax = 0.;
  if ( fit1_data ) xAxisMax = TMath::Max(xAxisMax, fit1_data->GetXmax());
  if ( fit1_mc   ) xAxisMax = TMath::Max(xAxisMax, fit1_mc->GetXmax());
  if ( fit2_data ) xAxisMax = TMath::Max(xAxisMax, fit2_data->GetXmax());
  if ( fit2_mc   ) xAxisMax = TMath::Max(xAxisMax, fit2_mc->GetXmax());  
  if ( fit3_data ) xAxisMax = TMath::Max(xAxisMax, fit3_data->GetXmax());
  if ( fit3_mc   ) xAxisMax = TMath::Max(xAxisMax, fit3_mc->GetXmax());  
  if ( xAxisMax == 0. ) {
    std::cerr << "Warning: failed to determine x-axis range --> setting xAxisMax = 35 !!" << std::endl;
    xAxisMax = 35.;
  }
  TH1* dummyHistogram_top = new TH1D("dummyHistogram_top", "dummyHistogram_top", TMath::Nint(xAxisMax) + 1, -0.5, xAxisMax + 0.5);
  dummyHistogram_top->SetTitle("");
  dummyHistogram_top->SetStats(false);
  dummyHistogram_top->SetMinimum(yMin);
  dummyHistogram_top->SetMaximum(yMax);
  
  TAxis* xAxis_top = dummyHistogram_top->GetXaxis();
  if      ( xAxis_mode == kGenNumPileUpInteractions ) xAxis_top->SetTitle("N_{PU}");
  else if ( xAxis_mode == kRecVertices              ) xAxis_top->SetTitle("N_{vtx}");
  else assert(0);

  xAxis_top->SetTitleOffset(1.15);
  xAxis_top->SetLabelColor(10);
  xAxis_top->SetTitleColor(10);

  TAxis* yAxis_top = dummyHistogram_top->GetYaxis();
  yAxis_top->SetTitle(yAxisTitle.Data());
  yAxis_top->SetTitleOffset(1.20);
  yAxis_top->SetTitleSize(0.06);

  dummyHistogram_top->Draw("axis");

  if ( draw1_data ) {
    if ( graph1_data ) {
      graph1_data->SetLineColor(1);
      graph1_data->SetMarkerColor(1);
      graph1_data->SetMarkerStyle(20);
      graph1_data->Draw("p");
    }
    if ( fit1_data ) {
      fit1_data->SetLineColor(graph1_data->GetLineColor());
      fit1_data->SetLineWidth(2);
      fit1_data->Draw("same");
    }
  }

  if ( draw1_mc ) {
    if ( graph1_mc ) {
      graph1_mc->SetLineColor(1);
      graph1_mc->SetMarkerColor(1);
      graph1_mc->SetMarkerStyle(24);
      graph1_mc->Draw("p");
    }
    if ( fit1_mc ) {
      fit1_mc->SetLineColor(graph1_mc->GetLineColor());
      fit1_mc->SetLineStyle(2);
      fit1_mc->SetLineWidth(2);
      fit1_mc->Draw("same");
    }
  }

  if ( draw2_data ) {
    if ( graph2_data ) {
      graph2_data->SetLineColor(2);
      graph2_data->SetMarkerColor(2);
      graph2_data->SetMarkerStyle(21);
      graph2_data->Draw("p");
    }
    if ( fit2_data ) {
      fit2_data->SetLineColor(graph2_data->GetLineColor());
      fit2_data->SetLineWidth(2);
      fit2_data->Draw("same");
    }
  }

  if ( draw2_mc ) {
    if ( graph2_mc ) {
      graph2_mc->SetLineColor(2);
      graph2_mc->SetMarkerColor(2);
      graph2_mc->SetMarkerStyle(25);
      graph2_mc->Draw("p");
    }
    if ( fit2_mc ) {
      fit2_mc->SetLineColor(graph2_mc->GetLineColor());
      fit2_mc->SetLineStyle(2);
      fit2_mc->SetLineWidth(2);
      fit2_mc->Draw("same");
    }
  }

  if ( draw3_data ) {
    if ( graph3_data ) {
      graph3_data->SetLineColor(4);
      graph3_data->SetMarkerColor(4);
      graph3_data->SetMarkerStyle(33);
      graph3_data->SetMarkerSize(2);
      graph3_data->Draw("p");
    }
    if ( fit3_data ) {
      fit3_data->SetLineColor(graph3_data->GetLineColor());
      fit3_data->SetLineWidth(2);
      fit3_data->Draw("same");
    }
  }

  if ( draw3_mc ) {
    if ( graph3_mc ) {
      graph3_mc->SetLineColor(4);
      graph3_mc->SetMarkerColor(4);
      graph3_mc->SetMarkerStyle(27);
      graph3_mc->SetMarkerSize(2);
      graph3_mc->Draw("p");
    }
    if ( fit3_mc ) {
      fit3_mc->SetLineColor(graph3_mc->GetLineColor());
      fit3_mc->SetLineStyle(2);
      fit3_mc->SetLineWidth(2);
      fit3_mc->Draw("same");
    }
  }

  TLegend* legend = new TLegend(0.165, 0.685, 0.54, 0.925, "", "brNDC"); 
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  if ( draw1_data && graph1_data ) legend->AddEntry(graph1_data, TString(legendEntry1).Append(" Data"), "p");
  if ( draw1_mc   && graph1_mc   ) legend->AddEntry(graph1_mc,   TString(legendEntry1).Append(" MC"),   "p");
  if ( draw2_data && graph2_data ) legend->AddEntry(graph2_data, TString(legendEntry2).Append(" Data"), "p");
  if ( draw2_mc   && graph2_mc   ) legend->AddEntry(graph2_mc,   TString(legendEntry2).Append(" MC"),   "p");
  if ( draw3_data && graph3_data ) legend->AddEntry(graph3_data, TString(legendEntry3).Append(" Data"), "p");
  if ( draw3_mc   && graph3_mc   ) legend->AddEntry(graph3_mc,   TString(legendEntry3).Append(" MC"),   "p");
  legend->Draw();	

  canvas->cd();
  bottomPad->Draw();
  bottomPad->cd();

  TH1* dummyHistogram_bottom = (TH1*)dummyHistogram_top->Clone("dummyHistogram_bottom");

  TAxis* xAxis_bottom = dummyHistogram_bottom->GetXaxis();
  xAxis_bottom->SetTitle(xAxis_top->GetTitle());
  xAxis_bottom->SetTitleOffset(1.20);
  xAxis_bottom->SetLabelColor(1);
  xAxis_bottom->SetTitleColor(1);
  xAxis_bottom->SetTitleSize(0.08);
  xAxis_bottom->SetLabelOffset(0.02);
  xAxis_bottom->SetLabelSize(0.08);
  xAxis_bottom->SetTickLength(0.055);

  TAxis* yAxis_bottom = dummyHistogram_bottom->GetYaxis();
  yAxis_bottom->SetTitle("#frac{Data - Simulation}{Simulation}");
  yAxis_bottom->SetTitleOffset(0.85);
  yAxis_bottom->SetNdivisions(505);
  yAxis_bottom->CenterTitle();
  yAxis_bottom->SetTitleSize(0.08);
  yAxis_bottom->SetLabelSize(0.08);
  yAxis_bottom->SetTickLength(0.04);

  double yDiffMax = +0.25;

  dummyHistogram_bottom->SetTitle("");
  dummyHistogram_bottom->SetStats(false);
  dummyHistogram_bottom->SetMaximum(+yDiffMax);
  dummyHistogram_bottom->SetMinimum(-yDiffMax);
  dummyHistogram_bottom->SetMarkerColor(1);
  dummyHistogram_bottom->SetMarkerStyle(20);
  dummyHistogram_bottom->Draw("axis");

  TF1* line_at_zero = new TF1("line_at_zeto", "0.", xAxisMin, xAxisMax);
  line_at_zero->SetLineColor(8);
  line_at_zero->SetLineWidth(1);
  line_at_zero->Draw("same");

  if ( draw1_data && draw1_mc ) {
    if ( graph1_data && graph1_mc ) {
      TGraphErrors* graph1_data_div_mc = makeGraph_data_div_mc(graph1_data, graph1_mc);
      cloneStyleOptions_graph(graph1_data_div_mc, graph1_data);
      graph1_data_div_mc->Draw("p");
    }
    if ( fit1_data && fit1_mc ) {
      TF1* fit1_data_div_mc = makeFit_data_div_mc(fit1_data, fit1_mc);
      cloneStyleOptions_fit(fit1_data_div_mc, fit1_data);
      fit1_data_div_mc->Draw("same");
    }
  }

  if ( draw2_data && draw2_mc ) {
    if ( graph2_data && graph2_mc ) {
      TGraphErrors* graph2_data_div_mc = makeGraph_data_div_mc(graph2_data, graph2_mc);
      cloneStyleOptions_graph(graph2_data_div_mc, graph2_data);
      graph2_data_div_mc->Draw("p");
    }
    if ( fit2_data && fit2_mc ) {
      TF1* fit2_data_div_mc = makeFit_data_div_mc(fit2_data, fit2_mc);
      cloneStyleOptions_fit(fit2_data_div_mc, fit2_data);
      fit2_data_div_mc->Draw("same");
    }
  }

  if ( draw3_data && draw3_mc ) {
    if ( graph3_data && graph3_mc ) {
      TGraphErrors* graph3_data_div_mc = makeGraph_data_div_mc(graph3_data, graph3_mc);
      cloneStyleOptions_graph(graph3_data_div_mc, graph3_data);
      graph3_data_div_mc->Draw("p");
    }
    if ( fit3_data && fit3_mc ) {
      TF1* fit3_data_div_mc = makeFit_data_div_mc(fit3_data, fit3_mc);
      cloneStyleOptions_fit(fit3_data_div_mc, fit3_data);
      fit3_data_div_mc->Draw("same");
    }
  }

  canvas->Update();
  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_plot).append(std::string(outputFileName, idx)).data());
  canvas->Print(std::string(outputFileName_plot).append(".png").data());
  canvas->Print(std::string(outputFileName_plot).append(".pdf").data());
  canvas->Print(std::string(outputFileName_plot).append(".root").data());

  delete dummyHistogram_top;
  delete legend;
  delete topPad;
  delete dummyHistogram_bottom;
  delete bottomPad;
  delete canvas;
}

void makeMEtResponse_vs_PileUpPlot(const TString& metType, 
				   const TString& projection, const TString& yAxisTitle,
				   TFile* inputFile1, const TString& runPeriod1, const TString& legendEntry1,
				   TFile* inputFile2, const TString& runPeriod2, const TString& legendEntry2,
				   TFile* inputFile3, const TString& runPeriod3, const TString& legendEntry3)
{
  TGraphErrors* graph1_data = 0;
  TF1* fit1_data = 0;
  if ( draw1_data && inputFile1 ) {
    graph1_data = compMEtResponse_vs_PileUp_data(inputFile1, "central", runPeriod1.Data());
    fit1_data = fitMEtResponse_vs_PileUp(graph1_data, runPeriod1);
  }

  TGraphErrors* graph1_mc = 0;
  TF1* fit1_mc = 0;
  if ( draw1_mc && inputFile1 ) {
    graph1_mc = compMEtResponse_vs_PileUp_mc(inputFile1, "central", runPeriod1.Data());
    fit1_mc = fitMEtResponse_vs_PileUp(graph1_mc, runPeriod1);
  }

  TGraphErrors* graph2_data = 0;
  TF1* fit2_data = 0;
  if ( draw2_data && inputFile2 ) {
    graph2_data = compMEtResponse_vs_PileUp_data(inputFile2, "central", runPeriod2.Data());
    fit2_data = fitMEtResponse_vs_PileUp(graph2_data, runPeriod2);
  }

  TGraphErrors* graph2_mc = 0;
  TF1* fit2_mc = 0;
  if ( draw2_mc && inputFile2 ) {
    graph2_mc = compMEtResponse_vs_PileUp_mc(inputFile2, "central", runPeriod2.Data());
    fit2_mc = fitMEtResponse_vs_PileUp(graph2_mc, runPeriod2);
  }

  TGraphErrors* graph3_data = 0;
  TF1* fit3_data = 0;
  if ( draw3_data && inputFile3 ) {
    graph3_data = compMEtResponse_vs_PileUp_data(inputFile3, "central", runPeriod3.Data());
    fit3_data = fitMEtResponse_vs_PileUp(graph3_data, runPeriod3);
  }

  TGraphErrors* graph3_mc = 0;
  TF1* fit3_mc = 0;
  if ( draw3_mc && inputFile3 ) {
    graph3_mc = compMEtResponse_vs_PileUp_mc(inputFile3, "central", runPeriod3.Data());
    fit3_mc = fitMEtResponse_vs_PileUp(graph3_mc, runPeriod3);
  }

  std::string outputFileName = Form("plots/metResponse_vs_PileUp_%s.eps", metType.Data());

  showPlot(//graph1_data, fit1_data, graph1_mc, fit1_mc, legendEntry1,
	   //graph2_data, fit2_data, graph2_mc, fit2_mc, legendEntry2,
	   //graph3_data, fit3_data, graph3_mc, fit3_mc, legendEntry3,
	   graph1_data, 0, graph1_mc, 0, legendEntry1,
	   graph2_data, 0, graph2_mc, 0, legendEntry2,
	   graph3_data, 0, graph3_mc, 0, legendEntry3,
	   0.7, 1.4, yAxisTitle,
	   outputFileName);
}

void makeMEtResolution_vs_PileUpPlot(const TString& metType, 
				     const TString& projection, const TString& yAxisTitle,
				     TFile* inputFile1, const TString& runPeriod1, const TString& legendEntry1,
				     TFile* inputFile2, const TString& runPeriod2, const TString& legendEntry2,
				     TFile* inputFile3, const TString& runPeriod3, const TString& legendEntry3,
				     bool divide_by_response)
{
  TGraphErrors* graph1_data = 0;
  TF1* fit1_data = 0;
  if ( draw1_data && inputFile1 ) {
    graph1_data = compMEtResolution_vs_PileUp_data(inputFile1, "central", runPeriod1.Data(), projection, divide_by_response);
    fit1_data = fitMEtResolution_vs_PileUp(graph1_data, runPeriod1);
  }

  TGraphErrors* graph1_mc = 0;
  TF1* fit1_mc = 0;
  if ( draw1_mc && inputFile1 ) {
    graph1_mc = compMEtResolution_vs_PileUp_mc(inputFile1, "central", runPeriod1.Data(), projection, divide_by_response);
    fit1_mc = fitMEtResolution_vs_PileUp(graph1_mc, runPeriod1);
  }

  TGraphErrors* graph2_data = 0;
  TF1* fit2_data = 0;
  if ( draw2_data && inputFile2 ) {
    graph2_data = compMEtResolution_vs_PileUp_data(inputFile2, "central", runPeriod2.Data(), projection, divide_by_response);
    fit2_data = fitMEtResolution_vs_PileUp(graph2_data, runPeriod2);
  }

  TGraphErrors* graph2_mc = 0;
  TF1* fit2_mc = 0;
  if ( draw2_mc && inputFile2 ) {
    graph2_mc = compMEtResolution_vs_PileUp_mc(inputFile2, "central", runPeriod2.Data(), projection, divide_by_response);
    fit2_mc = fitMEtResolution_vs_PileUp(graph2_mc, runPeriod2);
  }

  TGraphErrors* graph3_data = 0;
  TF1* fit3_data = 0;
  if ( draw3_data && inputFile3 ) {
    graph3_data = compMEtResolution_vs_PileUp_data(inputFile3, "central", runPeriod3.Data(), projection, divide_by_response);
    fit3_data = fitMEtResolution_vs_PileUp(graph3_data, runPeriod3);
  }

  TGraphErrors* graph3_mc = 0;
  TF1* fit3_mc = 0;
  if ( draw3_mc && inputFile3 ) {
    graph3_mc = compMEtResolution_vs_PileUp_mc(inputFile3, "central", runPeriod3.Data(), projection, divide_by_response);
    fit3_mc = fitMEtResolution_vs_PileUp(graph3_mc, runPeriod3);
  }

  std::string outputFileName = Form("plots/metResolution_vs_PileUp_%s_%s", metType.Data(), projection.Data());
  if ( divide_by_response ) outputFileName.append("_div_response");
  outputFileName.append(".eps");
  
  showPlot(graph1_data, fit1_data, graph1_mc, fit1_mc, legendEntry1,
	   graph2_data, fit2_data, graph2_mc, fit2_mc, legendEntry2,
	   graph3_data, fit3_data, graph3_mc, fit3_mc, legendEntry3,
	   0., 40., yAxisTitle,
	   outputFileName);
}

void compErr(std::map<std::string, double>& p, double p_central_value, double pErr_central_value, 
	     TObjArray& sysUncertainties, double& errUp, double& errDown)
{
  double errUp2   = 0.;
  double errDown2 = 0.;

  int numSysUncertainties = sysUncertainties.GetEntries();
  assert((numSysUncertainties % 2) == 0);
  for ( int iSysUncertainty = 0; iSysUncertainty < (numSysUncertainties / 2); ++iSysUncertainty ) {
    TObjString* sysUncertaintyUp = dynamic_cast<TObjString*>(sysUncertainties.At(2*iSysUncertainty));
    TObjString* sysUncertaintyDown = dynamic_cast<TObjString*>(sysUncertainties.At(2*iSysUncertainty + 1));

    double pUp   = p[sysUncertaintyUp->GetString().Data()];
    double pDown = p[sysUncertaintyDown->GetString().Data()];

    double pMin  = TMath::Min(pUp, pDown);
    double pMax  = TMath::Max(pUp, pDown);

    if ( pMin < p_central_value ) errDown2 += square(p_central_value - pMin);
    if ( pMax > p_central_value ) errUp2   += square(pMax - p_central_value);
  }

//--- add in quadrature statistical uncertainty of fit
  errUp2   += square(pErr_central_value);
  errDown2 += square(pErr_central_value);

  errUp   = TMath::Sqrt(errUp2);
  errDown = TMath::Sqrt(errDown2);
}

typedef TGraphErrors* (compMEtResolution_function)(TFile*, const std::string&, const TString&, const TString&, bool);

void computeSysUncertainties(const TString& projection,
			     TFile* inputFile, compMEtResolution_function* f, 
			     const TString& runPeriod, TObjArray& sysUncertainties,
			     valueMap3& resolution)
{
  TGraphErrors* graph_central_value = (*f)(inputFile, "central", runPeriod, projection, false);
  TF1* fit_central_value = fitMEtResolution_vs_PileUp(graph_central_value, runPeriod);	  
						
  double p0_central_value    = fit_central_value->GetParameter(0);
  double p0Err_central_value = fit_central_value->GetParError(0);
  double p1_central_value    = fit_central_value->GetParameter(1);
  double p1Err_central_value = fit_central_value->GetParError(1);
  double p2_central_value    = fit_central_value->GetParameter(2);
  double p2Err_central_value = fit_central_value->GetParError(2);
    
  std::map<std::string, double> p0; // key = central or systematic uncertainty
  std::map<std::string, double> p1; 
  std::map<std::string, double> p2;

  int numSysUncertainties = sysUncertainties.GetEntries();
  for ( int iSysUncertainty = 0; iSysUncertainty < numSysUncertainties; ++iSysUncertainty ) {
    TObjString* sysUncertainty = dynamic_cast<TObjString*>(sysUncertainties.At(iSysUncertainty));
    TGraphErrors* graph_i = (*f)(inputFile, sysUncertainty->GetString().Data(), runPeriod, projection, false);
    TF1* fit_i = fitMEtResolution_vs_PileUp(graph_i, runPeriod);
    p0[sysUncertainty->GetString().Data()] = fit_i->GetParameter(0);
    p1[sysUncertainty->GetString().Data()] = fit_i->GetParameter(1);
    p2[sysUncertainty->GetString().Data()] = fit_i->GetParameter(2);
  }

  double p0ErrUp, p0ErrDown;
  compErr(p0, p0_central_value, p0Err_central_value, sysUncertainties, p0ErrUp, p0ErrDown);
  resolution[projection.Data()]["sigmaZ"]["value"] = p0_central_value;
  resolution[projection.Data()]["sigmaZ"]["errUp"] = p0ErrUp;
  resolution[projection.Data()]["sigmaZ"]["errDown"] = p0ErrDown;
  std::cout << "sigmaZ = " << p0_central_value << " + " << p0ErrUp << " - " << p0ErrDown << std::endl;

  double p1ErrUp, p1ErrDown;
  compErr(p1, p1_central_value, p1Err_central_value, sysUncertainties, p1ErrUp, p1ErrDown);
  resolution[projection.Data()]["alpha"]["value"] = p1_central_value;
  resolution[projection.Data()]["alpha"]["errUp"] = p1ErrUp;
  resolution[projection.Data()]["alpha"]["errDown"] = p1ErrDown;
  std::cout << "alpha = " << p1_central_value << " + " << p1ErrUp << " - " << p1ErrDown << std::endl;

  double p2ErrUp, p2ErrDown;
  compErr(p2, p2_central_value, p2Err_central_value, sysUncertainties, p2ErrUp, p2ErrDown);
  resolution[projection.Data()]["sigmaMB"]["value"] = p2_central_value;
  resolution[projection.Data()]["sigmaMB"]["errUp"] = p2ErrUp;
  resolution[projection.Data()]["sigmaMB"]["errDown"] = p2ErrDown;
  std::cout << "sigmaMB = " << p2_central_value << " + " << p2ErrUp << " - " << p2ErrDown << std::endl;
}

TString getFileName_full(const TString& path, const TString& fileName)
{
  TString fileName_full = path;
  if ( !fileName_full.EndsWith("/") ) fileName_full.Append("/");
  fileName_full.Append(fileName);
  return fileName_full;
}

void printResolutionRow(std::ofstream& table, 
			const std::string& projection,
			const std::string& parameterTitle, const std::string& parameterName,
			valueMap3& resolution_data, valueMap3& resolution_mc)
{
  table << "$" << parameterTitle << "$ & $" 
	<< std::fixed << std::setprecision(3)
	<< resolution_data[projection][parameterName]["value"]  
	<< "^{+" << resolution_data[projection][parameterName]["errUp"] << "}"
    	<< "_{-" << resolution_data[projection][parameterName]["errDown"] << "}$ & $" 
	<< resolution_mc[projection][parameterName]["value"]  
	<< "^{+" << resolution_mc[projection][parameterName]["errUp"] << "}"
    	<< "_{-" << resolution_mc[projection][parameterName]["errDown"] << "}$ \\\\" << std::endl;
}

void printResolutionTable(std::ofstream& table, 
			  const std::string& runPeriod, 
			  valueMap3& resolution_data, valueMap3& resolution_mc)
{
  table << "\\hline" << std::endl;
  table << "\\multicolumn{3}{|c|}{" << runPeriod << "} \\\\" << std::endl;
  table << "\\hline" << std::endl;
  printResolutionRow(table, "uParl", "\\sigma_{Z}^{\\parallel}",  "sigmaZ",  resolution_data, resolution_mc);
  printResolutionRow(table, "uParl", "\\sigma_{MB}^{\\parallel}", "sigmaMB", resolution_data, resolution_mc);
  printResolutionRow(table, "uParl", "\\alpha_{\\parallel}",      "alpha",   resolution_data, resolution_mc);
  table << "\\hline" << std::endl;
  printResolutionRow(table, "uPerp", "\\sigma_{Z}^{\\perp}",      "sigmaZ",  resolution_data, resolution_mc);
  printResolutionRow(table, "uPerp", "\\sigma_{MB}^{\\perp}",     "sigmaMB", resolution_data, resolution_mc);
  printResolutionRow(table, "uPerp", "\\alpha_{\\perp}",          "alpha",   resolution_data, resolution_mc);
  table << "\\hline" << std::endl;
}

int main(int argc, const char* argv[])
{
  std::cout << "<makeMEtResolution_vs_PileUpPlots>:" << std::endl;  

//--- stop ROOT from keeping references to all histograms
  TH1::AddDirectory(false);

//--- disable pop-up windows showing graphics output
  gROOT->SetBatch(true);

//--- load framework libraries
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();

//--- keep track of time it takes the macro to execute
  TBenchmark clock;
  clock.Start("makeMEtResolution_vs_PileUpPlots");

  TString inputFilePath1 = "/data1/veelken/tmp/ZllRecoilCorrection/v7_00_wMEtShiftCorr_pfMEtCov_2/2012RunABC/";
  TString inputFileName1 = "analyzeZllRecoilCorrectionHistograms_all_pfMEtTypeIcorrectedSmeared.root";
  TString legendEntry1   = "corr. PFMEt";
  TString runPeriod1     = "2012runABC";

  TString inputFilePath2 = "/data1/veelken/tmp/ZllRecoilCorrection/v7_00_wMEtShiftCorr_pfMEtCov_2/2012RunABC/";
  TString inputFileName2 = "analyzeZllRecoilCorrectionHistograms_all_pfMEtNoPileUpSmeared.root";
  TString legendEntry2   = "No-PU MEt";
  TString runPeriod2     = "2012runABC";

  TString inputFilePath3 = "/data1/veelken/tmp/ZllRecoilCorrection/v7_00_wMEtShiftCorr_pfMEtCov_2/2012RunABC/";
  TString inputFileName3 = "analyzeZllRecoilCorrectionHistograms_all_pfMEtMVASmeared.root";
  TString legendEntry3   = "MVA MEt";
  TString runPeriod3     = "2012runABC";

  TFile* inputFile1 = new TFile(getFileName_full(inputFilePath1, inputFileName1));
  TFile* inputFile2 = new TFile(getFileName_full(inputFilePath2, inputFileName2));
  TFile* inputFile3 = new TFile(getFileName_full(inputFilePath3, inputFileName3));
  
  TString fit_uParl_formula = "-[0]*x*0.5*(1.0 - TMath::Erf(-[1]*TMath::Power(x, [2])))";

  makeMEtResponse_vs_PileUpPlot("pfMEtType1corr_vs_noPileUpMEt_vs_mvaMEt", 
				"uParl", "<-u_{#parallel} > / q_{T}",
				inputFile1, runPeriod1, legendEntry1,
				inputFile2, runPeriod2, legendEntry2,
				inputFile3, runPeriod3, legendEntry3);

  makeMEtResolution_vs_PileUpPlot("pfMEtType1corr_vs_noPileUpMEt_vs_mvaMEt", 
				  "uParl", "RMS(u_{#parallel} ) [GeV]", 
				  inputFile1, runPeriod1, legendEntry1,
				  inputFile2, runPeriod2, legendEntry2,
				  inputFile3, runPeriod3, legendEntry3, false);
  makeMEtResolution_vs_PileUpPlot("pfMEtType1corr_vs_noPileUpMEt_vs_mvaMEt", 
				  "uParl", "RMS(u_{#parallel} ) / (<-u_{#parallel} > / q_{T}) [GeV]", 
				  inputFile1, runPeriod1, legendEntry1,
				  inputFile2, runPeriod2, legendEntry2,
				  inputFile3, runPeriod3, legendEntry3, true);
  makeMEtResolution_vs_PileUpPlot("pfMEtType1corr_vs_noPileUpMEt_vs_mvaMEt", 
				  "uPerp", "RMS(u_{#perp}  ) [GeV]", 
				  inputFile1, runPeriod1, legendEntry1,
				  inputFile2, runPeriod2, legendEntry2, 
				  inputFile3, runPeriod3, legendEntry3, false);
  makeMEtResolution_vs_PileUpPlot("pfMEtType1corr_vs_noPileUpMEt_vs_mvaMEt", 
				  "uPerp", "RMS(u_{#perp}  ) / (<-u_{#parallel} > / q_{T}) [GeV]", 
				  inputFile1, runPeriod1, legendEntry1,
				  inputFile2, runPeriod2, legendEntry2, 
				  inputFile3, runPeriod3, legendEntry3, true);

  if ( compute_uncertainties ) {
    TObjArray sysUncertainties_data;
    sysUncertainties_data.Add(new TObjString("vertexRecoEff_p0Up"));
    sysUncertainties_data.Add(new TObjString("vertexRecoEff_p0Down"));
    sysUncertainties_data.Add(new TObjString("vertexRecoEff_p1Up"));
    sysUncertainties_data.Add(new TObjString("vertexRecoEff_p1Down"));
    sysUncertainties_data.Add(new TObjString("vertexRecoEff_p2Up"));
    sysUncertainties_data.Add(new TObjString("vertexRecoEff_p2Down"));
    
    valueMap3 resolution1_data; // key = 'uParl'/'uPerp', 'sigmaZ'/'sigmaMB'/'alpha', 'value'/'errUp'/'errDown'
    
    std::cout << "computing uncertainties on uParl, data (1):" << std::endl;
    computeSysUncertainties("uParl", inputFile1, 
			    &compMEtResolution_vs_PileUp_data,
			    runPeriod1, sysUncertainties_data, resolution1_data);
    std::cout << "computing uncertainties on uPerp, data (1):" << std::endl;
    computeSysUncertainties("uPerp", inputFile1, 
			    &compMEtResolution_vs_PileUp_data,
			    runPeriod1, sysUncertainties_data, resolution1_data);
    
    valueMap3 resolution2_data; // key = 'uParl'/'uPerp', 'sigmaZ'/'sigmaMB'/'alpha', 'value'/'errUp'/'errDown'
    
    std::cout << "computing uncertainties on uParl, data (2):" << std::endl;
    computeSysUncertainties("uParl", inputFile2, 
			    &compMEtResolution_vs_PileUp_data,
			    runPeriod2, sysUncertainties_data, resolution2_data);
    std::cout << "computing uncertainties on uPerp, data (2):" << std::endl;
    computeSysUncertainties("uPerp", inputFile2, 
			    &compMEtResolution_vs_PileUp_data,
			    runPeriod2, sysUncertainties_data, resolution2_data);

    valueMap3 resolution3_data; // key = 'uParl'/'uPerp', 'sigmaZ'/'sigmaMB'/'alpha', 'value'/'errUp'/'errDown'
    
    std::cout << "computing uncertainties on uParl, data (3):" << std::endl;
    computeSysUncertainties("uParl", inputFile3, 
			    &compMEtResolution_vs_PileUp_data,
			    runPeriod3, sysUncertainties_data, resolution3_data);
    std::cout << "computing uncertainties on uPerp, data (3):" << std::endl;
    computeSysUncertainties("uPerp", inputFile3, 
			    &compMEtResolution_vs_PileUp_data,
			    runPeriod3, sysUncertainties_data, resolution3_data);
        
    TObjArray sysUncertainties_mc;
    sysUncertainties_mc.Add(new TObjString("vertexRecoEff_p0Up"));
    sysUncertainties_mc.Add(new TObjString("vertexRecoEff_p0Down"));
    sysUncertainties_mc.Add(new TObjString("vertexRecoEff_p1Up"));
    sysUncertainties_mc.Add(new TObjString("vertexRecoEff_p1Down"));
    sysUncertainties_mc.Add(new TObjString("vertexRecoEff_p2Up"));
    sysUncertainties_mc.Add(new TObjString("vertexRecoEff_p2Down"));
    sysUncertainties_mc.Add(new TObjString("jetEnUp"));
    sysUncertainties_mc.Add(new TObjString("jetEnDown"));
    sysUncertainties_mc.Add(new TObjString("jetResUp"));
    sysUncertainties_mc.Add(new TObjString("jetResDown"));
    sysUncertainties_mc.Add(new TObjString("unclEnUp"));
    sysUncertainties_mc.Add(new TObjString("unclEnDown"));
    sysUncertainties_mc.Add(new TObjString("vertexRecoEffUp"));
    sysUncertainties_mc.Add(new TObjString("vertexRecoEffDown"));
    
    valueMap3 resolution1_mc; // key = 'uParl'/'uPerp', 'sigmaZ'/'sigmaMB'/'alpha', 'value'/'errUp'/'errDown'
    
    std::cout << "computing uncertainties on uParl, Monte Carlo (1):" << std::endl;
    computeSysUncertainties("uParl", inputFile1, 
			    &compMEtResolution_vs_PileUp_mc,
			    runPeriod1, sysUncertainties_mc, resolution1_mc);    
    std::cout << "computing uncertainties on uPerp, Monte Carlo (1):" << std::endl;
    computeSysUncertainties("uPerp", inputFile1, 
			    &compMEtResolution_vs_PileUp_mc,
			    runPeriod1, sysUncertainties_mc, resolution1_mc);
    
    valueMap3 resolution2_mc; // key = 'uParl'/'uPerp', 'sigmaZ'/'sigmaMB'/'alpha', 'value'/'errUp'/'errDown'
    
    std::cout << "computing uncertainties on uParl, Monte Carlo (2):" << std::endl;
    computeSysUncertainties("uParl", inputFile2, 
			    &compMEtResolution_vs_PileUp_mc,
			    runPeriod2, sysUncertainties_mc, resolution2_mc);
    std::cout << "computing uncertainties on uPerp, Monte Carlo (2):" << std::endl;
    computeSysUncertainties("uPerp", inputFile2, 
			    &compMEtResolution_vs_PileUp_mc,
			    runPeriod2, sysUncertainties_mc, resolution2_mc);

    valueMap3 resolution3_mc; // key = 'uParl'/'uPerp', 'sigmaZ'/'sigmaMB'/'alpha', 'value'/'errUp'/'errDown'
    
    std::cout << "computing uncertainties on uParl, Monte Carlo (3):" << std::endl;
    computeSysUncertainties("uParl", inputFile3, 
			    &compMEtResolution_vs_PileUp_mc,
			    runPeriod3, sysUncertainties_mc, resolution3_mc);
    std::cout << "computing uncertainties on uPerp, Monte Carlo (3):" << std::endl;
    computeSysUncertainties("uPerp", inputFile3, 
			    &compMEtResolution_vs_PileUp_mc,
			    runPeriod3, sysUncertainties_mc, resolution3_mc);
        
    std::ofstream* resolutionTable = new std::ofstream("makeMEtResolution_vs_PileUpPlots.tex", std::ios::out);
    printResolutionTable(*resolutionTable, runPeriod1.Data(), resolution1_data, resolution1_mc);
    printResolutionTable(*resolutionTable, runPeriod2.Data(), resolution2_data, resolution2_mc);
    printResolutionTable(*resolutionTable, runPeriod3.Data(), resolution3_data, resolution3_mc);
    delete resolutionTable;
  }

  delete inputFile1;
  delete inputFile2;
  delete inputFile3;

//--print time that it took macro to run
  std::cout << "finished executing makeMEtResolution_vs_PileUpPlots macro:" << std::endl;
  clock.Show("makeMEtResolution_vs_PileUpPlots");

  return 0;
}
