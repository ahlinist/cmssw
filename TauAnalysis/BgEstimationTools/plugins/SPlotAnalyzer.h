#ifndef TauAnalysis_BgEstimationTools_SPlotAnalyzer_h
#define TauAnalysis_BgEstimationTools_SPlotAnalyzer_h

/** \class SPlotAnalyzer
 *
 * Utility class for applying SPlot method
 * to make control plots of tau id. observables
 * in event sample containing background contributions
 *
 * NOTE: This utility class uses the SPlot implementation provided by the RooStats project;
 *       for a general introduction into the SPlot method, see:
 *
 *         M. Pivk and F. R. Le Diberder,
 *         "SPlot: A statistical tool to unfold data distributions", 
 *         Nucl. Instrum. Meth. A 555, 356 (2005).
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: SPlotAnalyzer.h,v 1.2 2010/10/07 13:51:46 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <RooDataHist.h>
#include <RooDataSet.h>
#include <RooAbsPdf.h>
#include <RooHistPdf.h>
#include <RooRealVar.h>
#include <RooStats/SPlot.h>

#include <TH1.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TPostScript.h>

#include <vector>
#include <string>

class SPlotAnalyzer : public edm::EDAnalyzer 
{
 public:
  // constructor 
  explicit SPlotAnalyzer(const edm::ParameterSet&);
    
  // destructor
  virtual ~SPlotAnalyzer();
    
 private:
//--- functions inherited from EDAnalyzer base class
  void beginJob() {}
  void analyze(const edm::Event&, const edm::EventSetup&) {}
  void endJob();

//--- auxiliary functions
  void buildFitData();
  void buildFitModel();

  void buildSPlotWeights();

//--- configuration parameters
  typedef std::vector<std::string> vstring;

  struct dataEntryType
  {
    dataEntryType(const edm::ParameterSet&, const std::string& = "distributions");
    ~dataEntryType();

    void buildAuxConcatenatedHistogram();
    
    void setFitVar(RooRealVar* fitVar) { fitVarRef_ = fitVar; }

    void buildDataHist();

    std::string processName_;

    vstring meNames_;

    RooRealVar* fitVarRef_;

    TH1* auxConcatenatedHistogram_;   
    RooDataHist* fitDataHist_;  

    void buildSPlotDataSet(const RooArgSet&);

    vstring sPlotNtupleFileNames_;
    std::string sPlotTreeName_;
    
    TChain* sPlotNtuple_;
    RooDataSet* sPlotDataSet_;

    int error_;
  };

  struct processEntryType : public dataEntryType
  {
    processEntryType(const edm::ParameterSet&, const std::string&, const std::string& = "templates");
    ~processEntryType();
    
    void buildPdf();
    
    RooHistPdf* modelHistPdf_;

    RooRealVar* norm_;
  };

  //---------------------------------------------------------
  std::vector<processEntryType*> processes_;

  RooAbsPdf* fitModel_;
  //---------------------------------------------------------

  //---------------------------------------------------------
  dataEntryType* data_;

  RooRealVar* fitVar_;

  RooStats::SPlot* sPlotAlgorithm_;
  //---------------------------------------------------------

  //---------------------------------------------------------
  struct sPlotEntryType
  {
    sPlotEntryType(const edm::ParameterSet&);
    ~sPlotEntryType();

    std::string branchName_;

    RooRealVar* var_;

    std::string title_;

    double xMin_;
    double xMax_;

    vstring selProcesses_;
  };

  void makeControlPlot(const std::string&, const sPlotEntryType&);

  std::vector<sPlotEntryType*> sPlots_;

  RooArgSet sPlotVariables_;
  //---------------------------------------------------------

  std::string outputFilePath_;
  std::string outputFileName_;
  std::string indOutputFileName_;

  TCanvas* canvas_;
  TPostScript* ps_;

  int cfgError_;
  int dqmError_;
};

#endif   
