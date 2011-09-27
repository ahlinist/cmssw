#include "TauAnalysis/RecoTools/plugins/JECfactorAnalyzer.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include <TCanvas.h>
#include <TGraph.h>
#include <TAxis.h>
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

void drawGraphs(const std::vector<std::string>& levels, 
		std::map<std::string, TGraph*>& graphs, 
		const std::string& payload, const std::string& varName, double varValue, 
		const std::string& xAxisTitle, double yMin, double yMax, const std::string& outputFileName)
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

  int colors[]       = {  2,  3,  4,  6,  7 };
  int markerStyles[] = { 20, 21, 22, 23, 34 };

  int numGraphs = levels.size();

  if ( numGraphs > 5 ) 
    throw cms::Exception("drawGraphs")
      << "Number of levels = " << levels.size() << " exceeds limit !!\n";

  double textSize = ( numGraphs <= 5 ) ? 0.055 : 0.045;
  TLegend* legend = new TLegend(0.58, 0.35 - textSize*numGraphs, 0.93, 0.35, "", "brNDC");
  legend->SetBorderSize(0);
  legend->SetFillColor(0);

  for ( int graph_index = 0; graph_index < numGraphs; ++graph_index ) {
    const std::string& corrLevel = levels[graph_index];

    TGraph* graph = graphs[corrLevel];
    graph->SetMarkerColor(colors[graph_index]);
    graph->SetMarkerStyle(markerStyles[graph_index]);
    graph->SetLineColor(colors[graph_index]);
    
    graph->SetTitle("");

    TAxis* xAxis_top = graph->GetXaxis();
    xAxis_top->SetLabelColor(10);
    xAxis_top->SetTitleColor(10);

    TAxis* yAxis_top = graph->GetYaxis();
    yAxis_top->SetTitle("Corr. Factor");

    graph->SetMaximum(yMax);
    graph->SetMinimum(yMin);

    std::string drawOption = ( graph_index == 0 ) ? "A" : "";    
    graph->Draw(drawOption.append("P").data()); 

    legend->AddEntry(graph, corrLevel.data(), "p");
  }

  legend->Draw();

  canvas->cd();
  bottomPad->Draw();
  bottomPad->cd();
  bottomPad->SetLogy(false);

  std::map<std::string, TGraph*> graphs_wrtPrevious;

  for ( int graph_index = 0; graph_index < numGraphs; ++graph_index ) {
    const std::string& corrLevel = levels[graph_index];

    TGraph* graph = graphs[corrLevel];

    int numPoints = graph->GetN();

    TGraph* graph_wrtPrevious = new TGraph(numPoints);
    graph_wrtPrevious->SetMarkerColor(graph->GetMarkerColor());
    graph_wrtPrevious->SetMarkerStyle(graph->GetMarkerStyle());
    graph_wrtPrevious->SetLineColor(graph->GetLineColor());

    graph_wrtPrevious->SetTitle("");

    for ( int iPoint = 0; iPoint < numPoints; ++iPoint ) {
      double x, y;
      graph->GetPoint(iPoint, x, y);
      if ( graph_index == 0 ) {
	graph_wrtPrevious->SetPoint(iPoint, x, y);
      } else {
	double x_previous, y_previous;
	const std::string& corrLevel_previous = levels[graph_index - 1];
	TGraph* graph_previous = graphs[corrLevel_previous];
	graph_previous->GetPoint(iPoint, x_previous, y_previous);
	assert(x_previous == x);
	graph_wrtPrevious->SetPoint(iPoint, x, y/y_previous);
      }
    }

    TAxis* xAxis_bottom = graph_wrtPrevious->GetXaxis();
    xAxis_bottom->SetTitle(xAxisTitle.data());
    xAxis_bottom->SetTitleOffset(1.20);
    //xAxis_bottom->SetNdivisions(505);
    xAxis_bottom->SetTitleSize(0.08);
    xAxis_bottom->SetLabelOffset(0.02);
    xAxis_bottom->SetLabelSize(0.08);
    xAxis_bottom->SetTickLength(0.055);
    
    TAxis* yAxis_bottom = graph_wrtPrevious->GetYaxis();
    yAxis_bottom->SetTitle("Corr. Factor wrt. Previous");
    yAxis_bottom->SetTitleOffset(1.10);
    yAxis_bottom->CenterTitle();
    yAxis_bottom->SetTitleOffset(0.9);
    yAxis_bottom->SetTitleSize(0.08);
    yAxis_bottom->SetLabelSize(0.08);
    yAxis_bottom->SetTickLength(0.04);

    graph_wrtPrevious->SetMaximum(1.2);
    graph_wrtPrevious->SetMinimum(0.8);

    std::string drawOption = ( graph_index == 0 ) ? "A" : "";    
    graph_wrtPrevious->Draw(drawOption.append("P").data()); 

    graphs_wrtPrevious[corrLevel] = graph_wrtPrevious;
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

  for ( std::map<std::string, TGraph*>::iterator it = graphs_wrtPrevious.begin();
	it != graphs_wrtPrevious.end(); ++it ) {
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

double square(double x)
{
  return x*x;
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

    std::vector<JetCorrectorParameters> jecParameters;
    for ( vstring::const_iterator corrLevel = levels_.begin();
	  corrLevel != levels_.end(); ++corrLevel ) {
      jecParameters.push_back((*jecParameterSet)[*corrLevel]);
    }
    FactorizedJetCorrector* corrector = new FactorizedJetCorrector(jecParameters);
  
    std::cout << "making plots for payload = " << (*payload) << std::endl;

    std::cout << " eta = "; 
    for ( double eta = etaMin_; eta <= etaMax_; eta += 5.*etaStepSize_ ) {
      std::cout << eta << " ";
    
      std::map<std::string, TGraph*> graphs; // key = level

      int numCorrLevels = levels_.size();
      for ( int corrLevelIndex = 0; corrLevelIndex < numCorrLevels; ++corrLevelIndex ) {
	const std::string& corrLevel = levels_[corrLevelIndex];
	
	int numPoints = TMath::FloorNint((ptMax_ - ptMin_)/ptStepSize_) + 1;
	TGraph* graph = new TGraph(numPoints);
	
	int iPoint = 0;
	for ( double pt = ptMin_; pt <= ptMax_; pt += ptStepSize_ ) {
	  if ( iPoint < numPoints ) {
	    // evaluate jet energy correction factor
	    corrector->setJetEta(eta);
	    corrector->setJetPt(pt); 
	    corrector->setJetE(compJetEnergy(pt, eta));
	    corrector->setNPV(6);   // typical value for Summer'11 data
	    corrector->setRho(4.0); // typical value for Summer'11 data
	    corrector->setJetA(TMath::Pi()*square(0.5));
	    
	    double jecFactor = corrector->getSubCorrections()[corrLevelIndex];
	    
	    graph->SetPoint(iPoint, pt, jecFactor);	  
	  }
	  ++iPoint;
	}
	
	graphs[corrLevel] = graph;
      }

      drawGraphs(levels_, graphs, *payload, "eta", eta, "P_{T}^{jet} / GeV", yMin_, yMax_, outputFileName_);

      for ( std::map<std::string, TGraph*>::iterator it = graphs.begin();
	    it != graphs.end(); ++it ) {
	delete it->second;
      }
    }
    std::cout << std::endl;

    std::cout << " pt = "; 
    for ( double pt = ptMin_; pt <= ptMax_; pt += 5.*ptStepSize_ ) {
      std::cout << pt << " ";
    
      std::map<std::string, TGraph*> graphs; // key = level

      int numCorrLevels = levels_.size();
      for ( int corrLevelIndex = 0; corrLevelIndex < numCorrLevels; ++corrLevelIndex ) {
	const std::string& corrLevel = levels_[corrLevelIndex];
	
	int numPoints = TMath::FloorNint((etaMax_ - etaMin_)/etaStepSize_) + 1;
	TGraph* graph = new TGraph(numPoints);
	
	int iPoint = 0;
	for ( double eta = etaMin_; eta <= etaMax_; eta += etaStepSize_ ) {
	  if ( iPoint < numPoints ) {
	    // evaluate jet energy correction factor
	    corrector->setJetEta(eta);
	    corrector->setJetPt(pt); 
	    corrector->setJetE(compJetEnergy(pt, eta));
	    corrector->setNPV(6);   // typical value for Summer'11 data
	    corrector->setRho(4.0); // typical value for Summer'11 data
	    corrector->setJetA(TMath::Pi()*square(0.5));
	    
	    double jecFactor = corrector->getSubCorrections()[corrLevelIndex];
	    
	    graph->SetPoint(iPoint, eta, jecFactor);	  
	  }
	  ++iPoint;
	}
	
	graphs[corrLevel] = graph;
      }

      drawGraphs(levels_, graphs, *payload, "pt", pt, "#eta_{jet}", yMin_, yMax_, outputFileName_);

      for ( std::map<std::string, TGraph*>::iterator it = graphs.begin();
	    it != graphs.end(); ++it ) {
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
