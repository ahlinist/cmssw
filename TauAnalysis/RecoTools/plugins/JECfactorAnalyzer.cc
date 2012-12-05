#include "TauAnalysis/RecoTools/plugins/JECfactorAnalyzer.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

#include <TCanvas.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TAxis.h>
#include <TH1.h>
#include <TLegend.h>
#include <TMath.h>
#include <TString.h>

#include <vector>
#include <string>

JECfactorAnalyzer::JECfactorAnalyzer(const edm::ParameterSet& cfg)
  : hasRun_(false)
{
  etaMin_         = cfg.getParameter<double>("etaMin");
  etaMax_         = cfg.getParameter<double>("etaMax");
  etaStepSize_    = cfg.getParameter<double>("etaStepSize");
  ptMin_          = cfg.getParameter<double>("ptMin");
  ptMax_          = cfg.getParameter<double>("ptMax");
  ptStepSize_     = cfg.getParameter<double>("ptStepSize");

  levels_         = cfg.getParameter<vstring>("levels");

  payloads_       = cfg.getParameter<vstring>("payloads");

  yMin_           = cfg.getParameter<double>("yMin");
  yMax_           = cfg.getParameter<double>("yMax");

  outputFileName_ = cfg.getParameter<std::string>("outputFileName");
}

JECfactorAnalyzer::~JECfactorAnalyzer()
{
  // nothing to be done yet...
}

void applyDrawOptions(TGraph* graph, int index,
		      int* colors, int* markerStyles, int* fillStyles)
{
  graph->SetMarkerColor(colors[index]);
  graph->SetMarkerStyle(markerStyles[index]);
  graph->SetFillColor(colors[index]);
  graph->SetFillStyle(fillStyles[index]);
  graph->SetLineColor(colors[index]);
    
  graph->SetTitle("");
}

void applyAxisOptions_top(TGraph* graph, double xMin, double xMax, double yMin, double yMax)
{
  TAxis* xAxis_top = graph->GetXaxis();
  xAxis_top->SetLimits(xMin, xMax);
  xAxis_top->SetLabelColor(10);
  xAxis_top->SetTitleColor(10);
  
  TAxis* yAxis_top = graph->GetYaxis();
  yAxis_top->SetTitle("Corr. Factor");

  graph->SetMaximum(yMax);
  graph->SetMinimum(yMin);
}

template <typename T>
void applyAxisOptions_bottom(T* graph, double xMin, double xMax, const std::string& xAxisTitle)
{
  TAxis* xAxis_bottom = graph->GetXaxis();
  xAxis_bottom->SetLimits(xMin, xMax);
  xAxis_bottom->SetLabelColor(1);
  xAxis_bottom->SetTitleColor(1);
  xAxis_bottom->SetTitle(xAxisTitle.data());
  xAxis_bottom->SetTitleOffset(1.20);
  xAxis_bottom->SetTitleSize(0.08);
  xAxis_bottom->SetLabelOffset(0.02);
  xAxis_bottom->SetLabelSize(0.08);
  xAxis_bottom->SetTickLength(0.055);
    
  TAxis* yAxis_bottom = graph->GetYaxis();
  yAxis_bottom->SetTitle("Corr. Factor wrt. Previous");
  yAxis_bottom->SetTitleOffset(1.10);
  yAxis_bottom->CenterTitle();
  yAxis_bottom->SetTitleOffset(0.9);
  yAxis_bottom->SetTitleSize(0.08);
  yAxis_bottom->SetLabelSize(0.08);
  yAxis_bottom->SetTickLength(0.04);
  
  graph->SetMaximum(1.2);
  graph->SetMinimum(0.8);
}

double square(double x)
{
  return x*x;
}

void drawGraphs(const std::vector<std::string>& levels, 
		std::map<std::string, TGraph*>& graphs_correction, std::map<std::string, TGraphErrors*>& graphs_uncertainty, 
		const std::string& payload, const std::string& varName, double varValue, 
		double xMin, double xMax, const std::string& xAxisTitle, double yMin, double yMax, const std::string& outputFileName)
{
  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 900);
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
  bottomPad->SetBottomMargin(0.20);
  bottomPad->SetRightMargin(0.05);

  canvas->cd();
  topPad->Draw();
  topPad->cd();

  int colors[]       = {    2,    3,    4,    6,    7 };
  int markerStyles[] = {   20,   21,   22,   23,   34 };
  //int fillStyles[]   = { 3004, 3005, 3006, 3007, 3012 };
  int fillStyles[]   = { 3003, 3003, 3003, 3003, 3003 };

  int numGraphs = levels.size();

  if ( numGraphs > 5 ) 
    throw cms::Exception("drawGraphs")
      << "Number of levels = " << levels.size() << " exceeds limit !!\n";

  double textSize = ( numGraphs <= 5 ) ? 0.055 : 0.045;
  TLegend* legend = new TLegend(0.67, 0.34 - textSize*(numGraphs + 1), 0.93, 0.34, "", "brNDC");
  legend->SetBorderSize(0);
  legend->SetFillColor(0);

  for ( int graph_index = 0; graph_index < numGraphs; ++graph_index ) {
    const std::string& corrLevel = levels[graph_index];

    TGraph* graph_uncertainty = graphs_uncertainty[corrLevel];
    applyDrawOptions(graph_uncertainty, graph_index, colors, markerStyles, fillStyles);
    applyAxisOptions_top(graph_uncertainty, xMin, xMax, yMin, yMax);

    //std::string drawOption = ( graph_index == 0 ) ? "A" : "";    
    //graph_uncertainty->Draw(drawOption.append("3").data()); 
  }

  graphs_uncertainty[levels[numGraphs - 1]]->Draw("A3");

  for ( int graph_index = 0; graph_index < numGraphs; ++graph_index ) {
    const std::string& corrLevel = levels[graph_index];

    TGraph* graph_correction = graphs_correction[corrLevel];
    applyDrawOptions(graph_correction, graph_index, colors, markerStyles, fillStyles);
    applyAxisOptions_top(graph_correction, xMin, xMax, yMin, yMax);

    graph_correction->Draw("P"); 

    legend->AddEntry(graph_correction, corrLevel.data(), "p");
  }

  legend->AddEntry(graphs_uncertainty[levels[numGraphs - 1]], "Uncertainty", "f");

  legend->Draw();

  canvas->cd();
  bottomPad->Draw();
  bottomPad->cd();
  bottomPad->SetLogy(false);

  std::map<std::string, TGraph*> graphs_correction_wrtPrevious;
  std::map<std::string, TGraphErrors*> graphs_uncertainty_wrtPrevious;

  for ( int graph_index = 0; graph_index < numGraphs; ++graph_index ) {
    const std::string& corrLevel = levels[graph_index];

    TGraph* graph_correction = graphs_correction[corrLevel];
    TGraph* graph_uncertainty = graphs_uncertainty[corrLevel];

    int numPoints = graph_correction->GetN();

    TGraph* graph_correction_wrtPrevious = new TGraph(numPoints);
    applyDrawOptions(graph_correction_wrtPrevious, graph_index, colors, markerStyles, fillStyles);
    applyAxisOptions_bottom(graph_correction_wrtPrevious, xMin, xMax, xAxisTitle);

    TGraphErrors* graph_uncertainty_wrtPrevious = new TGraphErrors(numPoints);
    applyDrawOptions(graph_uncertainty_wrtPrevious, graph_index, colors, markerStyles, fillStyles);
    applyAxisOptions_bottom(graph_uncertainty_wrtPrevious, xMin, xMax, xAxisTitle);

    for ( int iPoint = 0; iPoint < numPoints; ++iPoint ) {
      double x, y;
      graph_correction->GetPoint(iPoint, x, y);

      double yErr = graph_uncertainty->GetErrorY(iPoint);

      if ( graph_index == 0 ) {
	graph_correction_wrtPrevious->SetPoint(iPoint, x, y);
	graph_uncertainty_wrtPrevious->SetPointError(iPoint, 0., yErr);
      } else {
	const std::string& corrLevel_previous = levels[graph_index - 1];

	TGraph* graph_correction_previous = graphs_correction[corrLevel_previous];
	double x_previous, y_previous;
	graph_correction_previous->GetPoint(iPoint, x_previous, y_previous);
	assert(x_previous == x);

	//TGraph* graph_uncertainty_previous = graphs_uncertainty[corrLevel_previous];
	//double yErr_previous = graph_uncertainty_previous->GetErrorY(iPoint);

	graph_correction_wrtPrevious->SetPoint(iPoint, x, y/y_previous);
	graph_uncertainty_wrtPrevious->SetPoint(iPoint, x, y/y_previous);
	//graph_uncertainty_wrtPrevious->SetPointError(iPoint, 0., TMath::Sqrt(TMath::Max(square(yErr) - square(yErr_previous), 0.)));
	graph_uncertainty_wrtPrevious->SetPointError(iPoint, 0., yErr);
      }
    }
    
    graphs_correction_wrtPrevious[corrLevel] = graph_correction_wrtPrevious;
    graphs_uncertainty_wrtPrevious[corrLevel] = graph_uncertainty_wrtPrevious;
  }

  TH1* dummyHistogram_bottom = new TH1F("dummyHistogram_bottom", "",  graphs_correction[levels[numGraphs - 1]]->GetN(), xMin, xMax);
  dummyHistogram_bottom->SetStats(false);
  applyAxisOptions_bottom(dummyHistogram_bottom, xMin, xMax, xAxisTitle);
  dummyHistogram_bottom->Draw();

  //for ( int graph_index = 0; graph_index < numGraphs; ++graph_index ) {
  //  const std::string& corrLevel = levels[graph_index];
  //
  //  std::string drawOption = ( graph_index == 0 ) ? "A" : "";    
  //  graphs_uncertainty_wrtPrevious[corrLevel]->Draw(drawOption.append("3").data()); 
  //}
  graphs_uncertainty_wrtPrevious[levels[numGraphs - 1]]->Draw("3");

  for ( int graph_index = 0; graph_index < numGraphs; ++graph_index ) {
    const std::string& corrLevel = levels[graph_index];

    graphs_correction_wrtPrevious[corrLevel]->Draw("P");
  }

  canvas->Update();

  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  TString plotLabel = Form("%s_%s%1.1f", payload.data(), varName.data(), varValue);
  plotLabel.ReplaceAll(".", "_");
  outputFileName_plot.append("_").append(plotLabel.Data());
  if ( idx != std::string::npos ) outputFileName_plot.append(std::string(outputFileName, idx));
  else                            outputFileName_plot.append(".png");
  canvas->Print(outputFileName_plot.data());

  for ( std::map<std::string, TGraph*>::iterator it = graphs_correction_wrtPrevious.begin();
	it != graphs_correction_wrtPrevious.end(); ++it ) {
    delete it->second;
  }

  for ( std::map<std::string, TGraphErrors*>::iterator it = graphs_uncertainty_wrtPrevious.begin();
	it != graphs_uncertainty_wrtPrevious.end(); ++it ) {
    delete it->second;
  }
	
  delete legend;

  delete topPad;
  delete bottomPad;
  delete canvas;
}

double compJetEnergy(double pt, double eta)
{
  double theta = 2.*TMath::ATan(TMath::Exp(-eta));
  //std::cout << "<compJetEnergy>: eta = " << eta << " --> theta = " << theta << std::endl;
  assert(theta > 0. && theta < TMath::Pi());
  return pt/TMath::Sin(theta);
}

double getJECcorrection(FactorizedJetCorrector* jesCorrection, int corrLevelIndex, double pt, double eta)
{
  jesCorrection->setJetEta(eta);
  jesCorrection->setJetPt(pt); 
  jesCorrection->setJetE(compJetEnergy(pt, eta));
  jesCorrection->setNPV(15);  // typical value for Summer'12 data
  jesCorrection->setRho(10.); // typical value for Summer'12 data
  jesCorrection->setJetA(TMath::Pi()*square(0.5));	   
  return jesCorrection->getSubCorrections()[corrLevelIndex];
}

double getJECuncertainty(JetCorrectionUncertainty* jesUncertainty, double pt, double eta)
{
  jesUncertainty->setJetEta(eta);
  jesUncertainty->setJetPt(pt);
  return jesUncertainty->getUncertainty(true);
}

void JECfactorAnalyzer::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
//--- run this only ever once !!
  if ( hasRun_ ) return;
  hasRun_ = true;

//--- retrieve JEC parameters from the DB and build jet corrector objects
  for ( vstring::const_iterator payload = payloads_.begin();
	payload != payloads_.end(); ++payload ) {
    edm::ESHandle<JetCorrectorParametersCollection> jecParameterSet;
    es.get<JetCorrectionsRecord>().get(*payload, jecParameterSet); 

    std::vector<JetCorrectorParameters> jecParameters_correction;
    for ( vstring::const_iterator corrLevel = levels_.begin();
	  corrLevel != levels_.end(); ++corrLevel ) {
      jecParameters_correction.push_back((*jecParameterSet)[*corrLevel]);
    }
    FactorizedJetCorrector* jesCorrection = new FactorizedJetCorrector(jecParameters_correction);

    std::vector<JetCorrectionUncertainty*> jesUncertainty_total; // index = level
    //std::vector<JetCorrectionUncertainty*> jesUncertainty_time;  // index = level
    std::vector<JetCorrectorParameters> jecParameters_uncertainty;
    for ( vstring::const_iterator corrLevel = levels_.begin();
	  corrLevel != levels_.end(); ++corrLevel ) {
      //jecParameters_uncertainty.push_back((*jecParameterSet)[*corrLevel]);
      //jesUncertainty.push_back(new JetCorrectionUncertainty(jecParameters_uncertainty));
      jesUncertainty_total.push_back(new JetCorrectionUncertainty((*jecParameterSet)["Uncertainty"]));
      //jesUncertainty_time.push_back(new JetCorrectionUncertainty((*jecParameterSet)["Time"]));
    }
  
    std::cout << "making plots for payload = " << (*payload) << std::endl;

    std::cout << " eta = "; 
    for ( double eta = etaMin_; eta <= etaMax_; eta += 5.*etaStepSize_ ) {
      std::cout << eta << " ";
    
      std::map<std::string, TGraph*> graphs_correction; // key = level
      std::map<std::string, TGraphErrors*> graphs_uncertainty; // key = level

      int numCorrLevels = levels_.size();
      for ( int corrLevelIndex = 0; corrLevelIndex < numCorrLevels; ++corrLevelIndex ) {
	const std::string& corrLevel = levels_[corrLevelIndex];
	
	int numPoints = TMath::FloorNint((ptMax_ - ptMin_)/ptStepSize_) + 1;
	TGraph* graph_correction = new TGraph(numPoints);
	TGraphErrors* graph_uncertainty = new TGraphErrors(numPoints);
	
	int iPoint = 0;
	for ( double pt = ptMin_; pt <= ptMax_; pt += ptStepSize_ ) {
	  if ( iPoint < numPoints ) {
	    // evaluate jet energy correction factor
	    double jecFactor = getJECcorrection(jesCorrection, corrLevelIndex, pt, eta);
	    graph_correction->SetPoint(iPoint, pt, jecFactor);	  

	    // evaluate uncertainty on jet energy correction factor
	    double jecUncertainty_total = getJECuncertainty(jesUncertainty_total[corrLevelIndex], pt, eta);
	    //double jecUncertainty_time = getJECuncertainty(jesUncertainty_time[corrLevelIndex], pt, eta);
	    //double jecUncertainty = TMath::Sqrt(square(jecUncertainty_total) - square(jecUncertainty_time));
	    double jecUncertainty = jecUncertainty_total;
	    graph_uncertainty->SetPoint(iPoint, pt, jecFactor);
	    graph_uncertainty->SetPointError(iPoint, pt, jecUncertainty);
	  }
	  ++iPoint;
	}
	
	graphs_correction[corrLevel] = graph_correction;
	graphs_uncertainty[corrLevel] = graph_uncertainty;
      }

      drawGraphs(levels_, graphs_correction, graphs_uncertainty, 
		 *payload, "eta", eta, ptMin_, ptMax_, "P_{T}^{jet} / GeV", yMin_, yMax_, outputFileName_);

      for ( std::map<std::string, TGraph*>::iterator it = graphs_correction.begin();
	    it != graphs_correction.end(); ++it ) {
	delete it->second;
      }

      for ( std::map<std::string, TGraphErrors*>::iterator it = graphs_uncertainty.begin();
	    it != graphs_uncertainty.end(); ++it ) {
	delete it->second;
      }
    }
    std::cout << std::endl;

    std::cout << " pt = "; 
    for ( double pt = ptMin_; pt <= ptMax_; pt += 5.*ptStepSize_ ) {
      std::cout << pt << " ";
    
      std::map<std::string, TGraph*> graphs_correction; // key = level
      std::map<std::string, TGraphErrors*> graphs_uncertainty; // key = level

      int numCorrLevels = levels_.size();
      for ( int corrLevelIndex = 0; corrLevelIndex < numCorrLevels; ++corrLevelIndex ) {
	const std::string& corrLevel = levels_[corrLevelIndex];
	
	int numPoints = TMath::FloorNint((etaMax_ - etaMin_)/etaStepSize_) + 1;
	TGraph* graph_correction = new TGraph(numPoints);
	TGraphErrors* graph_uncertainty = new TGraphErrors(numPoints);
	
	int iPoint = 0;
	for ( double eta = etaMin_; eta <= etaMax_; eta += etaStepSize_ ) {
	  if ( iPoint < numPoints ) {
	    // evaluate jet energy correction factor
	    double jecFactor = getJECcorrection(jesCorrection, corrLevelIndex, pt, eta);
	    graph_correction->SetPoint(iPoint, eta, jecFactor);	  

	    // evaluate uncertainty on jet energy correction factor
	    double jecUncertainty_total = getJECuncertainty(jesUncertainty_total[corrLevelIndex], pt, eta);
	    //double jecUncertainty_time = getJECuncertainty(jesUncertainty_time[corrLevelIndex], pt, eta);
	    //double jecUncertainty = TMath::Sqrt(square(jecUncertainty_total) - square(jecUncertainty_time));
	    double jecUncertainty = jecUncertainty_total;
	    graph_uncertainty->SetPoint(iPoint, eta, jecFactor);
	    graph_uncertainty->SetPointError(iPoint, eta, jecUncertainty);
	  }
	  ++iPoint;
	}
	
	graphs_correction[corrLevel] = graph_correction;
	graphs_uncertainty[corrLevel] = graph_uncertainty;
      }

      drawGraphs(levels_, graphs_correction, graphs_uncertainty,
		 *payload, "pt", pt, etaMin_, etaMax_, "#eta_{jet}", yMin_, yMax_, outputFileName_);

      for ( std::map<std::string, TGraph*>::iterator it = graphs_correction.begin();
	    it != graphs_correction.end(); ++it ) {
	delete it->second;
      }

      for ( std::map<std::string, TGraphErrors*>::iterator it = graphs_uncertainty.begin();
	    it != graphs_uncertainty.end(); ++it ) {
	delete it->second;
      }
    }
    std::cout << std::endl;
  }
}

void JECfactorAnalyzer::endJob()
{
// nothing to be done yet...
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(JECfactorAnalyzer);
