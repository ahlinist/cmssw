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
 * \version $Revision: 1.1 $
 *
 * $Id: SPlotAnalyzer.h,v 1.1 2010/09/27 12:18:47 veelken Exp $
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
  void beginJob();
  void analyze(const edm::Event&, const edm::EventSetup&);
  void endJob();

//--- auxiliary functions
  void buildFitData();
  void buildFitModel();

  void buildSPlotWeights();

  void makeControlPlot();

//--- configuration parameters
  typedef std::vector<std::string> vstring;

  struct processEntryType
  {
    processEntryType(const std::string&, const vstring&);
    ~processEntryType();

    void setAuxVar(RooRealVar* fitVar) { fitVarRef_ = fitVar; }
    
    void buildPdf();

    std::string processName_;

    vstring meNames_;

    RooRealVar* fitVarRef_;

    TH1* auxConcatenatedHistogram_;   
    RooDataHist* modelDataHist_;  
    RooHistPdf* modelHistPdf_;

    RooRealVar* norm_;

    int error_;
  };

  std::vector<processEntryType> processes_;

  RooAbsPdf* fitModel_;

  //---------------------------------------------------------
  std::vector<std::string> meNamesFitVariables_;

  TH1* auxConcatenatedHistogram_;   
  RooDataHist* fitDataHist_;  

  RooRealVar* fitVar_;
  //---------------------------------------------------------

  //---------------------------------------------------------
  vstring sPlotFileNames_;
  std::string sPlotTreeName_;
  TChain* sPlotNtuple_;

  RooDataSet* sPlotDataSet_;

  RooStats::SPlot* sPlot_;

  vstring controlVariableNames_;
  RooArgSet controlVariables_;
  //---------------------------------------------------------

  std::string outputFilePath_;
  std::string outputFileName_;
  std::string indOutputFileName_;

  int cfgError_;
  int dqmError_;
};

#endif   
