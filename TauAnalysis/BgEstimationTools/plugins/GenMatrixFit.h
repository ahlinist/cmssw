#ifndef TauAnalysis_BgEstimationTools_GenMatrixFit_h  
#define TauAnalysis_BgEstimationTools_GenMatrixFit_h

/** \class GenMatrixFit
 *
 * Estimate contribution of signal and background processes
 * to final event sample by fitting number of events 
 * observed in different control regions in data
 * to signal and background models,
 * based on the assumption that observables are uncorrelated
 * (class implements "generalized matrix" method for data-driven background estimation)
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: GenMatrixFit.h,v 1.1 2010/06/27 14:08:48 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/Core/interface/BinningServiceBase.h"
#include "TauAnalysis/Core/interface/BinningBase.h"
#include "TauAnalysis/Core/interface/BinGrid.h"

#include "TauAnalysis/BgEstimationTools/interface/GenMatrix1dPdf.h"

#include <RooRealVar.h>
#include <RooAbsReal.h>
#include <RooAbsPdf.h>
#include <RooDataSet.h>
#include <RooFitResult.h>

#include <string>
#include <vector>
#include <iostream>

class GenMatrixFit : public edm::EDAnalyzer
{
  struct objVarEntryType
  {
    objVarEntryType(const std::string&, double, double, double);
    ~objVarEntryType();

    std::string name_;

    RooRealVar* x_;

    double xMin_;
    double xBoundary_;
    double xMax_;  
  };

  struct drawOptionsType
  {
    drawOptionsType(const edm::ParameterSet& cfg)
      : lineColor_(cfg.getParameter<int>("lineColor")),
	lineStyle_(cfg.getParameter<int>("lineStyle")),
	lineWidth_(cfg.getParameter<int>("lineWidth"))
    {}
    ~drawOptionsType() {}
    int lineColor_;
    int lineStyle_;
    int lineWidth_;
  };

  struct model1dEntryType
  {
    model1dEntryType(const std::string&, objVarEntryType*, double, bool, double, double);
    ~model1dEntryType();
    
    void print(std::ostream&) const;

    std::string name_;

    GenMatrix1dPdf* pdf1d_;

    bool fixProb_;
    RooAbsReal* prob1_;
    RooAbsReal* prob2_;

//--- data-members for adding constraints obtained from dedicated studies
//    on the probability values to be determined by the "generalized matrix method" fit
    bool applyProbConstraint_;
    RooAbsPdf* pdfProbConstraint_;
    RooConstVar* meanProbConstraint_;
    RooConstVar* sigmaProbConstraint_;
  };

  struct modelNdEntryType
  {
    modelNdEntryType(const std::string&, const std::vector<objVarEntryType*>&, double, bool, double, double,
		     const std::vector<double>&, const std::vector<bool>&, const std::vector<double>&, const std::vector<double>&);
    ~modelNdEntryType();

    std::string getRegionTitle(unsigned, const std::vector<std::string>&);
    std::string getContrTitle(unsigned);
    double getContr(unsigned);
    double getContrErr(unsigned);
    
    void print(std::ostream&) const;

    std::string name_;

    RooRealVar* norm_;

    RooAbsPdf* pdfNd_;

    std::vector<model1dEntryType*> model1dEntries_;

//--- data-members for adding constraints obtained from dedicated studies
//    on the normalization factors to be determined by the "generalized matrix method" fit
    bool applyNormConstraint_;
    RooAbsReal* pdfNormConstraint_;
    RooConstVar* meanNormConstraint_;
    RooConstVar* sigmaNormConstraint_;
  };

 public:
  
  explicit GenMatrixFit(const edm::ParameterSet&);
  ~GenMatrixFit();
  
 private:

  void beginJob(const edm::EventSetup&) {}
  void analyze(const edm::Event&, const edm::EventSetup&) {}
  void endJob();

//--- private auxiliary functions
  void buildFitData();
  void buildFitModel();
  void fit(int, int);
  double compChi2red();
  void saveFitResults();
  void makeControlPlots();

  void print(std::ostream&);

//--- configuration parameters 
  std::string dqmDirectory_data_;
  std::string dqmDirectory_fitResult_;
  std::string controlPlotsFileName_;

  int printLevel_;
  bool printWarnings_;

//--- other data-members
  std::string moduleLabel_;

  typedef std::vector<std::string> vstring;
  vstring processNames_;
  vstring varNames_;

  std::map<std::string, objVarEntryType*> objVarEntries_; // key = varName

  std::map<std::string, drawOptionsType*> drawOptions_; // key = processName

  BinningServiceBase* binningService_;

  BinningBase* dataBinningResults_;
  const BinGrid* dataBinGrid_;

  RooAbsData* dataSet_;

  RooAbsPdf* pdfModel_;

  typedef std::map<std::string, modelNdEntryType*> modelNdEntryMap;
  modelNdEntryMap modelNdEntries_; // key = processName

  RooFitResult* fitResult_;

  int cfgError_;
};

#endif  


